/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#include <unistd.h>
#include <string.h>

#include "system/qua_mm_system.h"
#include "common/qua_sys_platform.h"

static void print_usage(char* prog)
{
    printf("Usage: %s -p primary -c chip_name -o os_name\n",prog);
    printf("\t -p --primary-user : whether this sample is primary user(1: primary user; 0: non-primary user)\n");
    printf("\t -c --chip-name    : chip name, such as mc6870 or mc331x\n");
    printf("\t -o --os-name      : os name, such as android or rtt\n");
}

int main(int argc, char *argv[])
{
    QUA_S32 ret;
    qua_mm_system_t *system = NULL;
    QUA_CHAR chip_name[16];
    QUA_CHAR os_name[16];
    QUA_CHAR platform[32];
    QUA_CHAR tmp_buf[8];
    QUA_BOOL primary;
    qua_mm_system_ops_t *sys_ops;
    QUA_U32 blkSize;
    qua_vb_config_t vbCfg;
    QUA_CHAR *mmz_buffer;
    QUA_ULONG mmz_phy_buffer;

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary value error!\n");
        print_usage(argv[0]);
        return ret;
    }
    primary = atoi(tmp_buf);

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

    ret = qua_mm_init(primary, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        printf("qua_mm_init error!\n");
        return ret;
    }
    sys_ops = (qua_mm_system_ops_t *)system;
    printf("qua_mm_init success, platform:%s, primary user:%d\n", platform, primary);

    memset(&vbCfg, 0, sizeof(qua_vb_config_t));
    blkSize = 640*480*2;
    vbCfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vbCfg.common_pools[0].block_size = blkSize;
    vbCfg.common_pools[0].block_cnt = 6;

    if (primary) {
        ret = sys_ops->sys_init(&vbCfg);
        if (ret != QUA_SUCCESS) {
            printf("sys_init error!\n");
            return ret;
        }
    }

    ret = sys_ops->sys_mmz_alloc("mmz_test", NULL, blkSize, &mmz_phy_buffer, (void**)&mmz_buffer);
    if (ret != QUA_SUCCESS)
        printf("sys_mmz_alloc error!\n");

    sleep(10);

    if (ret == QUA_SUCCESS)
        sys_ops->sys_mmz_free(mmz_phy_buffer, mmz_buffer);

    sleep(10);

    printf("Success!\n");

    return 0;
}
