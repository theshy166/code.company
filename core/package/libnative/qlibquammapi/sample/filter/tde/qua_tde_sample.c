/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#include <unistd.h>
#include <string.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "filter/qua_mm_filter.h"

#define ALIGN_UP(x, a) ((x+a-1)&(~(a-1)))

#define TEMPLATE_FILE "3840x2160.rgb"
#define TEMPLATE_WIDTH 3840
#define TEMPLATE_HEIGHT 2160

static void print_usage(char* prog)
{
    printf("Usage: %s -c chip_name -o os_name -w img_width -h img_height -i input_file -d output_file -p 1\n",prog);
    printf("\t -c --chip-name    : chip name, such as mc6870 or qm10xh\n");
    printf("\t -o --os-name      : os name, such as android or linux\n");
    printf("\t -w --img-width    : image width, such as 960\n");
    printf("\t -h --img-height   : image height, such as 1280\n");
    printf("\t -i --input-file   : input file, such as 960x1280.rgb\n");
    printf("\t -d --output-file  : output file, such as 960x1280_new.rgb\n");
    printf("\t -p --primary-user : primary user, such as 1 or 0\n");
}

int main(int argc, char** argv)
{
    QUA_CHAR chip_name[16];
    QUA_CHAR os_name[16];
    QUA_CHAR platform[32];
    QUA_CHAR input_file[32];
    QUA_CHAR output_file[32];
    QUA_CHAR tmp_buf[8];
    QUA_S32 ret, len, img_width, img_height;

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CHIP_NAME, chip_name, sizeof(chip_name));
    if (ret == QUA_FAILURE) {
        printf("get chip name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    printf("get chip_name=%s\n", chip_name);
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OS_NAME, os_name, sizeof(os_name));
    if (ret == QUA_FAILURE) {
        printf("get os name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    printf("get os_name=%s\n", os_name);
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_WIDTH, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image width error!\n");
        print_usage(argv[0]);
        return ret;
    }

    img_width = atoi(tmp_buf);
    printf("get img_width=%d\n", img_width);
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_HEIGHT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image height error!\n");
        print_usage(argv[0]);
        return ret;
    }

    img_height = atoi(tmp_buf);
    printf("get img_height=%d\n", img_height);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary user error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_BOOL is_primary_user = atoi(tmp_buf);
    printf("get primary_user=%d\n", is_primary_user);

    ret = qua_make_platform(chip_name, os_name, platform, sizeof(platform));
    if (ret == QUA_FAILURE) {
        printf("make platform name error!\n");
        return ret;
    }

    qua_mm_system_t *system = NULL;
    printf("make platform=%s\n", platform);
    ret = qua_mm_init(is_primary_user, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        printf("qua_mm_init error!\n");
        return ret;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)system;
    if (is_primary_user) {
        QUA_U32 blk_size;
        qua_vb_config_t vb_cfg;

        memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
        blk_size = img_width*img_height*2;
        vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
        vb_cfg.common_pools[0].block_size = blk_size;
        vb_cfg.common_pools[0].block_cnt = 6;
        sys_ops->sys_init(&vb_cfg);
    }

    const qua_mm_module_t *filter_module = NULL;
    ret = qua_mm_load_module(QUA_MM_MODULE_FILTER, &filter_module);
    if (ret != QUA_SUCCESS || filter_module == NULL) {
        printf("load filter module failed\n");
        return -1;
    }

    printf("Module %s, API version %d\n", filter_module->id, filter_module->api_version);
    qua_mm_device_t *mm_device = NULL;
    ret = filter_module->open_device(filter_module, QUA_MM_FILTER_DEV_TDE, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open tde dev failed\n");
        return -1;
    }

    qua_mm_tde_device_t *tde_device = (qua_mm_tde_device_t *)mm_device;
    printf("Device %s\n", tde_device->parent.id);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INPUT_FILE, input_file, sizeof(input_file));
    if (ret == QUA_FAILURE) {
        printf("get input file error!\n");
        print_usage(argv[0]);
        return ret;
    }

    printf("get input_file=%s\n", input_file);
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OUTPUT_FILE, output_file, sizeof(output_file));
    if (ret == QUA_FAILURE) {
        printf("get output file error!\n");
        print_usage(argv[0]);
        return ret;
    }

    printf("get output_file=%s\n", output_file);

    FILE *pfd = fopen(input_file, "rb");
    if (pfd == NULL) {
        printf("open input file error!\n");
        return QUA_FAILURE;
    }

    FILE *pfd_temp = fopen(TEMPLATE_FILE, "rb");
    if (pfd_temp == NULL) {
        printf("open temp file error!\n");
        return QUA_FAILURE;
    }

    FILE *pfd_out = fopen(output_file, "wb");
    if (pfd_out == NULL) {
        printf("open output file error!\n");
        return QUA_FAILURE;
    }

    QUA_ULONG phy_addr, phy_addr_scale, phy_addr_temp;
    QUA_VOID_PTR virt_addr;
    QUA_VOID_PTR virt_addr_scale;
    QUA_VOID_PTR virt_addr_temp;

    ret = sys_ops->sys_mmz_alloc("image_in", NULL, img_width*img_height*4, &phy_addr, &virt_addr);
    if (ret != QUA_SUCCESS) {
        printf("alloc input buffer error! ret=%d\n", ret);
        return ret;
    }

    ret = sys_ops->sys_mmz_alloc("image_scale", NULL, TEMPLATE_WIDTH*TEMPLATE_HEIGHT*2/16, &phy_addr_scale, &virt_addr_scale);
    if (ret != QUA_SUCCESS) {
        printf("alloc scale buffer error! ret=%d\n", ret);
        return ret;
    }

    ret = sys_ops->sys_mmz_alloc("image_temp", NULL, TEMPLATE_WIDTH*TEMPLATE_HEIGHT*2, &phy_addr_temp, &virt_addr_temp);
    if (ret != QUA_SUCCESS) {
        printf("alloc temp buffer error! ret=%d\n", ret);
        return ret;
    }

    len = fread(virt_addr, 1, img_width*img_height*4, pfd);
    if (len != img_width*img_height*4) {
        printf("read input file failed! len=%d\n", len);
        return -1;
    }

    len = fread(virt_addr_temp, 1, TEMPLATE_WIDTH*TEMPLATE_HEIGHT*2, pfd_temp);
    if (len != TEMPLATE_WIDTH*TEMPLATE_HEIGHT*2) {
        printf("read temp file failed! len=%d\n", len);
        return -1;
    }

    QUA_TDE_HANDLE handle;
    qua_tde2_opt_t tde_opt;
    qua_tde2_blend_opt_t bld_opt;
    qua_tde2_surface_t src_surface, scale_surface, temp_surface;
    qua_tde2_rect_t src_rect, scale_rect, temp_rect;

    memset(&src_surface, 0, sizeof(qua_tde2_surface_t));
    memset(&scale_surface, 0, sizeof(qua_tde2_surface_t));
    memset(&temp_surface, 0, sizeof(qua_tde2_surface_t));

    handle = tde_device->qua_tde2_begin_job();
    if (handle == QUA_FAILURE) {
        printf("tde begin job failed! ret=%#X\n", ret);
        return -1;
    }

    temp_surface.phy_addr = phy_addr_temp;
    temp_surface.color_fmt = QUA_TDE2_COLOR_FMT_ARGB1555;
    temp_surface.width = TEMPLATE_WIDTH;
    temp_surface.height = TEMPLATE_HEIGHT;
    temp_surface.stride = TEMPLATE_WIDTH*2;
    temp_surface.alpha_max255 = QUA_TRUE;
    temp_surface.alpha1 = 255;
    temp_surface.fbc_compress = QUA_FALSE;
    temp_rect.x_pos = 0;
    temp_rect.y_pos = 0;
    temp_rect.width = TEMPLATE_WIDTH;
    temp_rect.height = TEMPLATE_HEIGHT;
    scale_surface.phy_addr = phy_addr_scale;
    scale_surface.color_fmt = QUA_TDE2_COLOR_FMT_ARGB1555;
    scale_surface.width = TEMPLATE_WIDTH/4;
    scale_surface.height = TEMPLATE_HEIGHT/4;
    scale_surface.stride = TEMPLATE_WIDTH*2/4;
    scale_surface.alpha_max255 = QUA_TRUE;
    scale_surface.alpha1 = 255;
    scale_surface.fbc_compress = QUA_FALSE;
    scale_rect.x_pos = 0;
    scale_rect.y_pos = 0;
    scale_rect.width = TEMPLATE_WIDTH/4;
    scale_rect.height = TEMPLATE_HEIGHT/4;
    ret = tde_device->qua_tde2_quick_resize(handle, &temp_surface, &temp_rect, &scale_surface, &scale_rect);
    if (ret != QUA_SUCCESS) {
        printf("tde add scale task failed! ret=%#X\n", ret);
        return -1;
    }

    memset(&tde_opt, 0, sizeof(qua_tde2_opt_t));
    memset(&bld_opt, 0, sizeof(qua_tde2_blend_opt_t));
    bld_opt.pixel_alpha_enable = QUA_TRUE;
    bld_opt.global_alpha_enable = QUA_FALSE;
    tde_opt.resize = QUA_FALSE;
    tde_opt.global_alpha = 64;
    tde_opt.blend_opt = bld_opt;
    tde_opt.mirror = QUA_TDE2_MIRROR_NONE;
    src_surface.phy_addr = phy_addr;
    //src_surface.color_fmt = QUA_TDE2_COLOR_FMT_YUV420;
    src_surface.color_fmt = QUA_TDE2_COLOR_FMT_ARGB8888;
    src_surface.width = img_width;
    src_surface.height = img_height;
    //src_surface.stride = img_width;
    src_surface.stride = img_width*4;
    //src_surface.cbcr_phy_addr = src_surface.phy_addr + src_surface.stride*src_surface.height;
    //src_surface.cbcr_stride = src_surface.stride;
    //src_surface.ycbcr_clut = QUA_FALSE;
    src_surface.alpha_max255 = QUA_TRUE;
    src_surface.fbc_compress = QUA_FALSE;
    src_rect.x_pos = (img_width-TEMPLATE_WIDTH/4)/2;
    src_rect.y_pos = (img_height-TEMPLATE_HEIGHT/4)/2;
    src_rect.width = TEMPLATE_WIDTH/4;
    src_rect.height = TEMPLATE_HEIGHT/4;
    ret = tde_device->qua_tde2_bitblit(handle, &src_surface, &src_rect, &scale_surface, &scale_rect, &src_surface, &src_rect, &tde_opt);
    if (ret != QUA_SUCCESS) {
        printf("tde add blit task failed! ret=%#X\n", ret);
        return -1;
    }

    ret = tde_device->qua_tde2_end_job(handle, QUA_FALSE, QUA_TRUE, 5000);
    if (ret != QUA_SUCCESS) {
        printf("tde end job failed! ret=%#X\n", ret);
        return -1;
    }

    len = fwrite(virt_addr, 1, img_width*img_height*4, pfd_out);
    if (len != img_width*img_height*4) {
        printf("write output file failed! len=%d\n", len);
        return -1;
    }

    printf("Test success!\n");
    sys_ops->sys_mmz_free(phy_addr, virt_addr);
    sys_ops->sys_mmz_free(phy_addr_scale, virt_addr_scale);
    sys_ops->sys_mmz_free(phy_addr_temp, virt_addr_temp);
    fclose(pfd);
    fclose(pfd_out);
    tde_device->parent.close((struct qua_mm_device_t*)tde_device);
    return 0;
}
