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
#include "display/qua_mm_display.h"
#include "utils/qua_display_parser.h"

#define ALIGN_UP(x, a) ((x+a-1)&(~(a-1)))

static QUA_S32 vo_num, fb_num;
static QUA_CONST_CHAR *vo_id = NULL;
static QUA_CONST_CHAR *fb_id = NULL;

static void print_usage(char* prog)
{
    printf("Usage: %s -c chip_name -o os_name -w img_width -h img_height -t intf_type -s intf_sync -d display_id -p 1\n",prog);
    printf("\t -c --chip-name    : chip name, such as mc6870 or qm10xd\n");
    printf("\t -o --os-name      : os name, such as android or linux\n");
    printf("\t -w --img-width    : image width, such as 1280 or 800\n");
    printf("\t -h --img-height   : image height, such as 720 or 1280\n");
    printf("\t -t --intf-type    : intf type, such as 36 or 64\n");
    printf("\t -s --intf-sync    : intf sync, such as 8 or 38\n");
    printf("\t -d --output-file  : display id, such as id:display0\n");
    printf("\t -p --primary-user : primary user, such as 1 or 0\n");
}

int main(int argc, char** argv)
{
    QUA_CHAR chip_name[16];
    QUA_CHAR os_name[16];
    QUA_CHAR platform[32];
    QUA_CHAR display_id[16];
    QUA_CHAR tmp_buf[8];
    QUA_S32 ret, img_width, img_height;

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

    printf("make platform=%s\n", platform);
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OUTPUT_FILE, display_id, sizeof(display_id));
    if (ret == QUA_FAILURE) {
        printf("get display id error!\n");
        print_usage(argv[0]);
        return ret;
    }

    printf("get display_id=%s\n", display_id);
    qua_init_display_parser();
    ret = qua_parse_display_id(display_id, &vo_id, &vo_num, &fb_id, &fb_num);
    if (ret != QUA_SUCCESS) {
        printf("%s, qua_parse_display_id error!\n", __func__);
        return ret;
    }

    printf("vo_id=%s vo_num=%d fb_id=%s fb_num=%d\n", vo_id, vo_num, fb_id, fb_num);
    qua_mm_system_t *system = NULL;
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

    const qua_mm_module_t *display_module = NULL;
    ret = qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module);
    if (ret != QUA_SUCCESS || display_module == NULL) {
        printf("load display module failed\n");
        return -1;
    }

    printf("Module %s, API version %d\n", display_module->id, display_module->api_version);
    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, vo_id, vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open vo dev failed\n");
        return -1;
    }

    qua_mm_vo_device_t *vo_device = (qua_mm_vo_device_t *)mm_device;
    printf("Device %s, number %d\n", vo_device->parent.id, vo_device->dev_num);

    qua_vo_pub_attr_t dev_attr = {0};
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INTF_TYPE, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get intf type error!\n");
        print_usage(argv[0]);
        return ret;
    }

    dev_attr.intf_type = atoi(tmp_buf);
    printf("get intf_type=%d\n", dev_attr.intf_type);
    dev_attr.bg_color = 0;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INTF_SYNC, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get intf sync error!\n");
        print_usage(argv[0]);
        return ret;
    }

    dev_attr.intf_sync = atoi(tmp_buf);
    printf("get intf_sync=%d\n", dev_attr.intf_sync);
    ret = vo_device->set_public_attr(vo_num, &dev_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: set_public_attr return %d\n", ret);
        return -1;
    }

    if (vo_device->set_hdmi_fmt != NULL && (dev_attr.intf_type & QUA_VO_INTF_HDMI) != 0) {
        qua_vo_hdmi_fmt_t hdmi_fmt;
        hdmi_fmt.hdmi_fmt = QUA_HDMI_FORMAT_EXT_YUV422_16BIT;
        ret = vo_device->set_hdmi_fmt(vo_num, &hdmi_fmt);
        if (ret != QUA_SUCCESS) {
            printf("Error: set_hdmi_fmt return %d\n", ret);
            return -1;
        }
    }

    ret = vo_device->enable(vo_num);
    if (ret != QUA_SUCCESS) {
        printf("Error: enable return %d\n", ret);
        return -1;
    }

    mm_device = NULL;
    ret = display_module->open_device(display_module, fb_id, fb_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open fb dev failed\n");
        return -1;
    }

    qua_mm_fb_device_t *fb_device = (qua_mm_fb_device_t *)mm_device;
    printf("Device %s, number %d\n", fb_device->parent.id, fb_device->dev_num);

    QUA_CHAR *vaddr;
    qua_fb_fix_screeninfo_t fix_info;
    qua_fb_var_screeninfo_t var_info;
    qua_fb_bitfield_t stR = {16,8,0};
    qua_fb_bitfield_t stG = {8,8,0};
    qua_fb_bitfield_t stB = {0,8,0};
    qua_fb_bitfield_t stA = {24,8,0};

    fb_device->show(fb_device, QUA_FALSE);
    fb_device->compress(fb_device, QUA_FALSE);

    qua_point_t position;
    position.x = 0;
    position.y = 0;
    if (fb_device->put_origin(fb_device, &position) != QUA_SUCCESS) {
        printf("put origin failed\n");
        return -1;
    }

    qua_size_t screen_size;
    screen_size.width = ALIGN_UP(img_width, 8);
    screen_size.height = img_height;
    if (fb_device->put_size(fb_device, &screen_size) != QUA_SUCCESS) {
        printf("put size failed\n");
        return -1;
    }

    if (fb_device->get_var_info(fb_device, &var_info) != QUA_SUCCESS) {
        printf("get var info failed\n");
        return -1;
    }

    var_info.xres_virtual = ALIGN_UP(img_width, 8);
    var_info.yres_virtual = img_height;
    var_info.xres = ALIGN_UP(img_width, 8);
    var_info.yres = img_height;
    var_info.activate = QUA_FB_ACTIVATE_NOW;
    var_info.bits_per_pixel = 32;
    var_info.xoffset = 0;
    var_info.yoffset = 0;
    var_info.red   = stR;
    var_info.green = stG;
    var_info.blue  = stB;
    var_info.transp = stA;
    if (fb_device->put_var_info(fb_device, &var_info) != QUA_SUCCESS) {
        printf("put var info failed\n");
        return -1;
    }

    if (fb_device->get_fix_info(fb_device, &fix_info) != QUA_SUCCESS) {
        printf("get fix info failed\n");
        return -1;
    }

    vaddr = fb_device->map(fb_device, fix_info.smem_len, 0);
    if (vaddr == QUA_NULL) {
        printf("map failed\n");
        return -1;
    }

    memset(vaddr, 0x00, fix_info.smem_len);
    if (fb_device->show(fb_device, QUA_TRUE) != QUA_SUCCESS) {
        printf("show failed\n");
        return -1;
    }

    fb_device->render(fb_device, &var_info);

    printf("Setup success, drawing...\n");
    QUA_U8 value = 0;
    while (value < 245) {
        value += 8;
        memset(vaddr, value, fix_info.smem_len);
        printf("Draw value 0x%x\n", value);
        if (fb_device->render(fb_device, &var_info) != QUA_SUCCESS)
        {
            printf("display failed\n");
        }
        usleep(500*1000);
    }

    printf("Sleep for 2 seconds...\n");
    sleep(2);

    printf("Test success!\n");
    fb_device->show(fb_device, QUA_FALSE);
    fb_device->unmap(fb_device, vaddr, fix_info.smem_len);
    sleep(1);
    fb_device->parent.close((struct qua_mm_device_t*)fb_device);

    vo_device->disable(vo_num);
    vo_device->parent.close((struct qua_mm_device_t*)vo_device);

    return 0;
}
