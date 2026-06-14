/*
 *  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "filter/qua_mm_filter.h"
#include "video/qua_mm_video.h"
#include "pipeline/qua_mpp_pipeline.h"

#define SNS_WIDTH   2592
#define SNS_HEIGHT  1944
#define ALIGNTO(x, align) (((x) + (align) - 1) & ~((align) - 1))
static QUA_BOOL g_stop = 0;
static qua_mm_system_ops_t *g_sys_ops = NULL;

static void print_usage(char* prog) {
    printf("Usage: %s -p primary -c chip-name -o os-name\n",prog);
    printf("\t -p --primary-user   : whether this sample is primary user(1: primary user; 0: non-primary user)\n");
    printf("\t -c --chip-name      : chip name, such as mc6870 or mc331x\n");
    printf("\t -o --os-name        : os name, such as android or rtt\n");
}

static void signal_handler(int sig)
{
    (void)sig;
    g_stop = 1;
}

int dump_vicap(int chn, int cnt, qua_video_frame_info_t *frame)
{
    char name[32]={0};
    snprintf(name, 32, "%d_%d.yuv", chn, cnt);

    int size = frame->video_frame.width * frame->video_frame.stride[0] * 3 / 2;
    void* vir_addr = (void*)frame->video_frame.vir_addr[0];
    QUA_U64 phy_addr = frame->video_frame.phy_addr[0];
    printf("vir_addr=0x%x, phy_addr=0x%llx\n", vir_addr, phy_addr);
    vir_addr = g_sys_ops->sys_mmap(phy_addr, size);
    printf("vir_addr=0x%x, phy_addr=0x%llx\n", vir_addr, phy_addr);
    if (vir_addr){
        
        FILE *fp = fopen(name, "wb");
         if (!fp) {
             perror("fopen failed");
             return -1;
         }
        size_t written = fwrite(vir_addr, 1, size, fp);
        if (written != size) {
            perror("fwrite failed");
            fclose(fp);
            return -1;
        }
        if (fclose(fp) != 0) {
            perror("fclose failed");
            return -1;
        }
        printf("%s save sucess\n",name);
    }

    return 0;
}

static void qua_mm_get_sys_ops(const char *platform, QUA_BOOL primary, qua_mm_system_ops_t **sys_ops) {
    qua_mm_system_t *system;
    qua_mm_system_ops_t *ops;
    QUA_U32 blk_size;
    qua_vb_config_t vb_cfg;
    qua_mm_init(primary, platform, &system);
    ops = (qua_mm_system_ops_t *)system;

    memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
    blk_size = SNS_WIDTH * SNS_HEIGHT * 2;
    vb_cfg.max_pool_cnt = 1;
    vb_cfg.common_pools[0].block_size = blk_size;
    vb_cfg.common_pools[0].block_cnt = 2;

    if (primary) {
        ops->sys_init(&vb_cfg);
    }

    *sys_ops = ops;
}


int main(int argc, char *argv[]) {
    QUA_S32 ret;
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);

    QUA_CHAR chip_name[16];
    QUA_CHAR os_name[16];
    QUA_CHAR platform[32];
    QUA_CHAR tmp_buf[8];

    QUA_BOOL primary;

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CHIP_NAME, chip_name, sizeof(chip_name));
    if (ret == QUA_FAILURE) {
        printf("get chip name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OS_NAME, os_name, sizeof(os_name));
    if (ret == QUA_FAILURE) {
        printf("get os name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    ret = qua_make_platform(chip_name, os_name, platform, sizeof(platform));
    if (ret == QUA_FAILURE) {
        printf("make platform name error!\n");
        return ret;
    }
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary value error!\n");
        print_usage(argv[0]);
        return ret;
    }
    primary = atoi(tmp_buf);

    printf("==============start qua mm camera test============\n");
    qua_mm_system_ops_t *sys_ops = NULL;
    qua_mm_get_sys_ops(platform, primary, &sys_ops);
    g_sys_ops = sys_ops;

    qua_mpp_pipeline_t *pipeline = NULL;
    pipeline = qua_mm_pipeline_create(MPP_PIPELINE_CAMERA_STREAMOUT, 0, sys_ops);
    if (!pipeline) {
        printf("qua_mm_pipeline_create failed\n");
        return -1;
    }
        
    quamm_param_t param;
    quamm_size_t res;
    QUA_BOOL ret_val = QUA_TRUE;

    param.key = QUAMM_PARAM_MEDIA_Resolution;
    res.direction = QUAMM_DIRECTION_IN;
    res.size.width = SNS_WIDTH;
    res.size.height = SNS_HEIGHT;
    param.data = (void*)&res;
    qua_mm_pipeline_set_param(pipeline, MPP_ELEMENT_VPROC, &param);

    param.key = QUAMM_PARAM_MEDIA_Resolution;
    res.direction = QUAMM_DIRECTION_OUT;
    res.size.width = SNS_WIDTH;
    res.size.height = SNS_HEIGHT;
    param.data = (void*)&res;
    qua_mm_pipeline_set_param(pipeline, MPP_ELEMENT_VPROC, &param);

    ret_val = qua_mm_pipeline_start(pipeline);
    if (!ret_val) {
        printf("qua_mm_pipeline_start failed\n");
        return -1;
    }

    int cnt = 0;
    qua_mpp_frame_t mpp_frame;
    while (!g_stop){
        usleep(30 * 1000);
        int ret_val = qua_mm_pipeline_get_frame(pipeline, MPP_ELEMENT_VICAP, 1000, &mpp_frame);
        if (QUA_FALSE == ret_val) {
            printf("qua_mm_pipeline_get_frame fail\n");
            continue;
        }

        cnt++;
        //dump_vicap(0, cnt, &mpp_frame.frame.video_frame)
        ret_val = qua_mm_pipeline_release_frame(pipeline, MPP_ELEMENT_VICAP, &mpp_frame);
    }
    
    qua_mm_pipeline_destroy(pipeline);
    qua_mm_pipeline_stop(pipeline);
    printf("==============qua mm camera pipeline test done============\n");
}
