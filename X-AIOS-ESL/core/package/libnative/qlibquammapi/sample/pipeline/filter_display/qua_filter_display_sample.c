#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "pipeline/filter_display/qua_filter_display_pipeline.h"
#include "common/qua_sys_platform.h"
#include "common/qua_mm_common.h"
#include "system/qua_mm_system.h"

static void print_usage(char* prog) {
    printf("Usage: %s -p --primary-user -c --chip-name -o --os-name -i stream_file -w img-width -h img-height\n", prog);
    printf("\t -p --primary-user   : whether this sample is primary user(1: primary user; 0: non-primary user)\n");
    printf("\t -c --chip-name      : chip name, such as mc6870 or mc331x\n");
    printf("\t -o --os-name        : os name, such as android or rtt\n");
    printf("\t -i --stream_file    : the full path of the stream file\n");
    printf("\t -w --img-width      : the width of image\n");
    printf("\t -h --img-height     : the height of image\n");
}

int main(int argc, char* argv[]) {
    QUA_S32 ret;
    QUA_CHAR tmp_buf[16];
    FILE *fp_strm = NULL;
    FILE *fp_frm = NULL;
    QUA_CHAR platform[32];

    QUA_BOOL primary;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary value error!\n");
        print_usage(argv[0]);
        return ret;
    }
    primary = atoi(tmp_buf);

    QUA_CHAR chip_name[16];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CHIP_NAME, chip_name, sizeof(chip_name));
    if (ret == QUA_FAILURE) {
        printf("get chip name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_CHAR os_name[16];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OS_NAME, os_name, sizeof(os_name));
    if (ret == QUA_FAILURE) {
        printf("get os name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_S32 width;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_WIDTH, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image width error!\n");
        print_usage(argv[0]);
        return ret;
    }
    width = atoi(tmp_buf);

    QUA_S32 height;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_HEIGHT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image height error!\n");
        print_usage(argv[0]);
        return ret;
    }
    height = atoi(tmp_buf);

    QUA_CHAR stream_path[256];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INPUT_FILE, stream_path, sizeof(stream_path));
    if (ret == QUA_FAILURE) {
        printf("get input file error!\n");
        print_usage(argv[0]);
        return ret;
    }

    fp_strm = fopen(stream_path, "rb");
    if (fp_strm == NULL) {
        printf("can't open stream %s (%s)\n", stream_path, strerror(errno));
        return -1;
    }

    qua_mm_system_t *system;
    qua_mm_system_ops_t *ops;
    QUA_U32 blk_size;
    qua_vb_config_t vb_cfg;
    sprintf(platform, "%s_%s", chip_name, os_name);
    printf("platform %s\n", platform);
    qua_mm_init(primary, platform, &system);
    ops = (qua_mm_system_ops_t *)system;

    memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
    blk_size = 1024*600*2;
    vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vb_cfg.common_pools[0].block_size = blk_size;
    vb_cfg.common_pools[0].block_cnt = 4;

    if (primary) {
        ops->sys_init(&vb_cfg);
    }

    qua_filter_display_pipeline_attr_t attr;
    attr.in_width = width;
    attr.in_height = height;
    attr.in_format = QUA_PIXEL_FMT_RGB_8888;
    attr.x = 0;
    attr.y = 0;
    attr.display_width = width;
    attr.display_height = height;
    QUA_U32 size = width * height * 4;
    QUA_ULONG paddr;
    QUA_VOID_PTR vaddr;
    ops->sys_mmz_alloc("filter_display_buffer", NULL, size, &paddr, &vaddr);

    printf("alloc 0x%x\n", paddr);

    qua_filter_display_pipeline_ops_t pipeline_ops;
    ret = qua_filter_display_pipeline_init(&pipeline_ops);
    if (ret != QUA_SUCCESS)
        return -1;
    qua_filter_display_pipeline_t *pipeline = NULL;
    pipeline = pipeline_ops.pipeline_create(&attr);
    if (pipeline == NULL)
        return -1;

    ret = pipeline_ops.pipeline_start(pipeline);
    if (ret != QUA_SUCCESS)
        goto start_err;

    QUA_U64 buf_len;
    fseek(fp_strm, 0, SEEK_END);
    buf_len = ftell(fp_strm);
    fseek(fp_strm, 0, SEEK_SET);
    fread(vaddr, 1, buf_len, fp_strm);

    qua_filter_display_pipeline_frame_t frame;
    frame.phyAddr = paddr;
    ret = pipeline_ops.pipeline_send_frame(pipeline, &frame);
    if (ret != QUA_SUCCESS)
        goto send_err;

    sleep(10);
send_err:
    pipeline_ops.pipeline_stop(pipeline);
start_err:
    pipeline_ops.pipeline_destroy(pipeline);
    if (fp_strm)
        fclose(fp_strm);

    ops->sys_mmz_free(paddr, vaddr);
    return 0;
}
