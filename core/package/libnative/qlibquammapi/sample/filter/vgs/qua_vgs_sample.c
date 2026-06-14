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

static void print_usage(char* prog)
{
    printf("Usage: %s -c chip_name -o os_name -w img_width -h img_height -i input_file -d output_file -p 1\n",prog);
    printf("\t -c --chip-name    : chip name, such as mc331x or qm10xh\n");
    printf("\t -o --os-name      : os name, such as linux or rtt\n");
    printf("\t -w --img-width    : image width, such as 640 or 1280\n");
    printf("\t -h --img-height   : image height, such as 480 or 960\n");
    printf("\t -i --input-file   : input file, such as 640x480_nv12.yuv\n");
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
    ret = filter_module->open_device(filter_module, QUA_MM_FILTER_DEV_VGS, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open vgs dev failed\n");
        return -1;
    }

    qua_mm_vgs_device_t *vgs_device = (qua_mm_vgs_device_t *)mm_device;
    printf("Device %s\n", vgs_device->parent.id);

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

    FILE *pfd_out = fopen(output_file, "wb");
    if (pfd_out == NULL) {
        printf("open output file error!\n");
        return QUA_FAILURE;
    }

    QUA_ULONG phy_addr, phy_addr_out, phy_addr_scale, phy_addr_convert;
    QUA_VOID_PTR virt_addr;
    QUA_VOID_PTR virt_addr_out;
    QUA_VOID_PTR virt_addr_scale;
    QUA_VOID_PTR virt_addr_convert;

    ret = sys_ops->sys_mmz_alloc("image_in", NULL, img_width*img_height*3/2, &phy_addr, &virt_addr);
    if (ret != QUA_SUCCESS) {
        printf("alloc input buffer error! ret=%d\n", ret);
        return ret;
    }

    ret = sys_ops->sys_mmz_alloc("image_out", NULL, img_width*img_height*3/2, &phy_addr_out, &virt_addr_out);
    if (ret != QUA_SUCCESS) {
        printf("alloc output buffer error! ret=%d\n", ret);
        return ret;
    }

    ret = sys_ops->sys_mmz_alloc("image_scale", NULL, 4*img_width*img_height*3/2, &phy_addr_scale, &virt_addr_scale);
    if (ret != QUA_SUCCESS) {
        printf("alloc scale buffer error! ret=%d\n", ret);
        return ret;
    }

    ret = sys_ops->sys_mmz_alloc("image_convert", NULL, 4*img_width*img_height*4, &phy_addr_convert, &virt_addr_convert);
    if (ret != QUA_SUCCESS) {
        printf("alloc scale buffer error! ret=%d\n", ret);
        return ret;
    }

    qua_vgs_task_attr_t vgs_task;

    memset(&vgs_task, 0, sizeof(vgs_task));
    vgs_task.img_in.video_frame.width = img_width;
    vgs_task.img_in.video_frame.height = img_height;
    vgs_task.img_in.video_frame.pixel_fmt = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    vgs_task.img_in.video_frame.phy_addr[0] = phy_addr;
    vgs_task.img_in.video_frame.phy_addr[1] = phy_addr + img_width*img_height;
    vgs_task.img_in.video_frame.stride[0] = img_width;
    vgs_task.img_in.video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
    vgs_task.img_out.video_frame.width = img_height;
    vgs_task.img_out.video_frame.height = img_width;
    vgs_task.img_out.video_frame.pixel_fmt = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    vgs_task.img_out.video_frame.phy_addr[0] = phy_addr_out;
    vgs_task.img_out.video_frame.phy_addr[1] = phy_addr_out + img_width*img_height;
    vgs_task.img_out.video_frame.stride[0] = img_height;
    vgs_task.img_out.video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
    len = fread(virt_addr, 1, img_width*img_height*3/2, pfd);
    if (len != img_width*img_height*3/2) {
        printf("read input file failed! len=%d\n", len);
        return -1;
    }

    if (vgs_device->qua_vgs_rotate_new) {
        ret = vgs_device->qua_vgs_rotate_new(&vgs_task, QUA_VGS_ROTATION_90);
        if (ret != QUA_SUCCESS) {
            printf("new vgs rotate failed! ret=%#X\n", ret);
            return -1;
        }
    } else if (vgs_device->qua_vgs_rotate) {
        qua_vgs_rotate_t vgs_rotate;

        vgs_rotate.mirror = QUA_FALSE;
        vgs_rotate.flip = QUA_FALSE;
        vgs_rotate.rotate = QUA_TRUE;
        ret = vgs_device->qua_vgs_rotate(&vgs_task, &vgs_rotate);
        if (ret != QUA_SUCCESS) {
            printf("vgs rotate failed! ret=%#X\n", ret);
            return -1;
        }
    } else {
        printf("vgs don't support to rotate\n");
        return -1;
    }

    qua_rect_t rect;

    rect.x = 0;
    rect.y = 0;
    rect.width = img_height/2;
    rect.height = img_width/2;
    if (vgs_device->qua_vgs_fill_color) {
        ret = vgs_device->qua_vgs_fill_color(&vgs_task.img_out, &rect, 0xff);
        if (ret != QUA_SUCCESS) {
            printf("vgs fill color failed! ret=%#X\n", ret);
            return -1;
        }
    }

    QUA_VGS_HANDLE handle;

    ret = vgs_device->qua_vgs_begin_job(&handle);
    if (ret != QUA_SUCCESS) {
        printf("vgs begin job failed! ret=%#X\n", ret);
        return -1;
    }

    vgs_task.img_in = vgs_task.img_out;
    vgs_task.img_out.video_frame.width = img_height*2;
    vgs_task.img_out.video_frame.height = img_width*2;
    vgs_task.img_out.video_frame.pixel_fmt = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    vgs_task.img_out.video_frame.phy_addr[0] = phy_addr_scale;
    vgs_task.img_out.video_frame.phy_addr[1] = phy_addr_scale + img_width*img_height*4;
    vgs_task.img_out.video_frame.stride[0] = img_height*2;
    vgs_task.img_out.video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
    ret = vgs_device->qua_vgs_add_scale_task(handle, &vgs_task);
    if (ret != QUA_SUCCESS) {
        printf("vgs add scale task failed! ret=%#X\n", ret);
        return -1;
    }

    if (vgs_device->qua_vgs_format_convert_new) {
        vgs_task.img_in = vgs_task.img_out;
        vgs_task.img_out.video_frame.width = img_height*2;
        vgs_task.img_out.video_frame.height = img_width*2;
        vgs_task.img_out.video_frame.pixel_fmt = QUA_PIXEL_FMT_RGB_8888;
        vgs_task.img_out.video_frame.phy_addr[0] = phy_addr_convert;
        vgs_task.img_out.video_frame.stride[0] = img_height*2*4;
        vgs_task.img_out.video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
        ret = vgs_device->qua_vgs_format_convert_new(handle, &vgs_task.img_in.video_frame, &vgs_task.img_out.video_frame);
        if (ret != QUA_SUCCESS) {
            printf("new vgs add format convert task failed! ret=%#X\n", ret);
            return -1;
        }
    } else if (vgs_device->qua_vgs_format_convert) {
        qua_tde2_surface_t surface;

        memset(&surface, 0, sizeof(surface));
        surface.color_fmt = QUA_TDE2_COLOR_FMT_ARGB8888;
        surface.width = img_height*2;
        surface.height = img_width*2;
        surface.stride = img_height*2*4;
        surface.phy_addr = phy_addr_convert;
        surface.alpha_max255 = 1;
        surface.alpha_ext_1555 = 0;
        surface.alpha0 = 0;
        surface.alpha1 = 255;
        ret = vgs_device->qua_vgs_format_convert(handle, &vgs_task.img_out.video_frame, &surface, QUA_TRUE);
        if (ret != QUA_SUCCESS) {
            printf("vgs add format convert task failed! ret=%#X\n", ret);
            return -1;
        }
    } else {
        printf("vgs don't support to add format convert task\n");
        return -1;
    }

    ret = vgs_device->qua_vgs_end_job(handle);
    if (ret != QUA_SUCCESS) {
        printf("vgs end job failed! ret=%#X\n", ret);
        return -1;
    }

    len = fwrite(virt_addr_convert, 1, img_width*img_height*4*4, pfd_out);
    if (len != img_width*img_height*4*4) {
        printf("write output file failed! len=%d\n", len);
        return -1;
    }

    printf("Test success!\n");
    sys_ops->sys_mmz_free(phy_addr, virt_addr);
    sys_ops->sys_mmz_free(phy_addr_out, virt_addr_out);
    sys_ops->sys_mmz_free(phy_addr_scale, virt_addr_scale);
    sys_ops->sys_mmz_free(phy_addr_convert, virt_addr_convert);
    fclose(pfd);
    fclose(pfd_out);
    vgs_device->parent.close((struct qua_mm_device_t*)vgs_device);
    return 0;
}
