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
#include "filter/qua_mm_filter.h"
#include "utils/qua_display_parser.h"

#define VPSS_GROUP_ID      0
#define VGS_CHANNEL_ID     0
#define BUF_NUM            3

#define ALIGN_UP(x, a)      ((x+a-1)&(~(a-1)))
#define ALIGN_BACK(x, a)    ((a) * (((x) / (a))))

static QUA_VO_LAYER video_layer;
static QUA_S32 vo_num, fb_num;
static QUA_CONST_CHAR *vo_id = NULL;
static QUA_CONST_CHAR *fb_id = NULL;

static void print_usage(char* prog)
{
    printf("Usage: %s -c chip_name -o os_name -w img_width -h img_height -t intf_type -s intf_sync -r frame_rate -d display_id -p 1\n",prog);
    printf("\t -c --chip-name    : chip name, such as mc6870 or qm10xd\n");
    printf("\t -o --os-name      : os name, such as android or linux\n");
    printf("\t -w --img-width    : image width, such as 1280 or 800\n");
    printf("\t -h --img-height   : image height, such as 720 or 1280\n");
    printf("\t -t --intf-type    : intf type, such as 36 or 64\n");
    printf("\t -s --intf-sync    : intf sync, such as 8 or 38\n");
    printf("\t -r --frame-rate   : frame rate, such as 60 or 30\n");
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
    QUA_S32 ret, img_width, img_height, w_align, h_align;

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
        blk_size = 640*480*3/2;
        vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
        vb_cfg.common_pools[0].block_size = blk_size;
        vb_cfg.common_pools[0].block_cnt = 1;
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

    if (vo_num == 1)
        video_layer = 1;
    else
        video_layer = 0;

    if (vo_device->bind_video_layer != NULL ) {
        ret = vo_device->bind_video_layer(video_layer, vo_num);
        if (ret != QUA_SUCCESS) {
            printf("Error: bind video layer return %d\n", ret);
            return -1;
        }
    }

    qua_vo_compress_attr_t vo_compress_attr;
    qua_vo_video_layer_attr_t vo_layer_attr_hd;
    ret = vo_device->get_video_layer_attr(video_layer, &vo_layer_attr_hd, &vo_compress_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: get video layer attr return %d\n", ret);
        return -1;
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_FRAME_RATE, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get frame rate error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_S32 frame_rate = atoi(tmp_buf);
    printf("get frame_rate=%d\n", frame_rate);

    vo_layer_attr_hd.disp_rect.x=0;
    vo_layer_attr_hd.disp_rect.y=0;
    vo_layer_attr_hd.disp_rect.width=img_width;
    vo_layer_attr_hd.disp_rect.height=img_height;
    vo_layer_attr_hd.image_size.width=img_width;
    vo_layer_attr_hd.image_size.height=img_height;
    vo_layer_attr_hd.disp_frmrt = frame_rate;
    vo_layer_attr_hd.pix_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    vo_layer_attr_hd.double_frame = QUA_FALSE;
    vo_layer_attr_hd.cluster_mode = QUA_TRUE;
    vo_compress_attr.support_compress = QUA_FALSE;

    ret = vo_device->set_video_layer_attr(video_layer, &vo_layer_attr_hd, &vo_compress_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: set video layer attr return %d\n", ret);
        return -1;
    }

    ret = vo_device->set_video_disp_buflen(video_layer, BUF_NUM);
    if (ret != QUA_SUCCESS) {
        printf("Error: set video disp bufferlen return %d\n", ret);
        return -1;
    }

    ret = vo_device->enable_video_layer(video_layer);
    if (ret != QUA_SUCCESS) {
        printf("Error: enable video layer return %d\n", ret);
        return -1;
    }

    QUA_S32 chn_id = 0;
    qua_mm_channel_t *mm_channel;
    qua_mm_vo_channel_t *vo_chn;
    ret = vo_device->parent.create_channel(mm_device, &chn_id, NULL, &mm_channel);
    if (ret != QUA_SUCCESS) {
        printf("Error: create channel return %d\n", ret);
        return -1;
    }

    vo_chn = (qua_mm_vo_channel_t *)mm_channel;
    qua_vo_chn_attr_t vo_chn_attr;
    ret = vo_chn->get_chn_attr(video_layer, chn_id, &vo_chn_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: get  chn attr return %d\n", ret);
        return -1;
    }

    printf("u32Priority %d rect x %d y %d w %d h %d bDeflicker %d\n", vo_chn_attr.priority, vo_chn_attr.rect.x,
            vo_chn_attr.rect.y, vo_chn_attr.rect.width, vo_chn_attr.rect.height, vo_chn_attr.deflicker);

    vo_chn_attr.priority = 0;
    vo_chn_attr.rect.x = 0;
    vo_chn_attr.rect.y = 0;
    vo_chn_attr.rect.width = img_width;
    vo_chn_attr.rect.height = img_height;
    vo_chn_attr.deflicker = QUA_FALSE;
    ret = vo_chn->set_chn_attr(video_layer, chn_id, &vo_chn_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: set  chn attr return %d\n", ret);
        return -1;
    }

    ret = vo_chn->set_chn_frame_rate(video_layer, chn_id, frame_rate);
    if (ret != QUA_SUCCESS) {
        printf("Error: set chn frame rate return %d\n", ret);
        return -1;
    }

    ret = vo_chn->enable_chn(video_layer, chn_id);
    if (ret != QUA_SUCCESS) {
        printf("Error: enable chn return %d\n", ret);
        return -1;
    }

    const qua_mm_module_t *filter_module = NULL;
    qua_mm_vppu_device_t *vppu_device = NULL;
    qua_mm_vpss_channel_t *vpss_chn = NULL;
    qua_mm_vgs_channel_t *vgs_chn = NULL;
    qua_mm_vpu_channel_t *vpu_chn = NULL;
    qua_mpp_chn_t srcChn;
    qua_mpp_chn_t destChn;
    qua_filter_chn_attr_t filter_chn_attr;
    QUA_VB_POOL pool_id;
    QUA_U32 blk_handle;
    QUA_U64 phy_addr;
    QUA_VOID *vir_addr;

    ret = qua_mm_load_module(QUA_MM_MODULE_FILTER, &filter_module);
    if (ret != QUA_SUCCESS || filter_module == NULL) {
        printf("load filter module failed\n");
        return -1;
    }

    printf("Module %s, API version %d\n", filter_module->id, filter_module->api_version);
    mm_device = NULL;
    ret = filter_module->open_device(filter_module, QUA_MM_FILTER_DEV_VPPU, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("Error: open_device QUA_MM_FILTER_DEV_VPPU return %d\n", ret);
        return -1;
    }

    vppu_device = (qua_mm_vppu_device_t *)mm_device;
    ret = vppu_device->init();
    if (ret != QUA_SUCCESS) {
        printf("Error: init vppu device return %#X\n", ret);
        return -1;
    }

    memset(&filter_chn_attr, 0x00, sizeof(qua_filter_chn_attr_t));
    mm_channel = NULL;
    if (strcmp(QUA_SYS_CHIP_MC6870, chip_name) == 0) {
        filter_chn_attr.filter_chn = QUA_MM_FILTER_VPSS_CHN;
        ret = vppu_device->parent.create_channel((qua_mm_device_t *)vppu_device, &chn_id, (QUA_VOID_PTR)&filter_chn_attr, &mm_channel);
        if (ret != QUA_SUCCESS) {
            printf("Error: create vpps channel return %d\n", ret);
            return -1;
        }

        vpss_chn = (qua_mm_vpss_channel_t *)mm_channel;
        qua_vpss_grp_attr_t vpss_grp_attr;

        memset(&vpss_grp_attr, 0x00, sizeof(qua_vpss_grp_attr_t));
        w_align = ALIGN_UP(img_width, 8);
        h_align = ALIGN_UP(img_height, 2);
        vpss_grp_attr.max_width = w_align;
        vpss_grp_attr.max_height = h_align;
        vpss_grp_attr.pix_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
        vpss_grp_attr.die_mode = QUA_VPSS_DIE_MODE_AUTO;
        ret = vppu_device->create_grp(VPSS_GROUP_ID, &vpss_grp_attr);
        if (ret != QUA_SUCCESS) {
            printf("Error: create vpps group return %#X\n", ret);
            return -1;
        }

        ret = vppu_device->start_grp(VPSS_GROUP_ID);
        if (ret != QUA_SUCCESS) {
            printf("Error: start vpps group return %d\n", ret);
            return -1;
        }

        ret = vpss_chn->enable_chn(VPSS_GROUP_ID, 0);
        if (ret != QUA_SUCCESS) {
            printf("Error: start vpps group return %d\n", ret);
            return -1;
        }

        srcChn.mod = QUA_MPP_MOD_VPSS;
        srcChn.dev = VPSS_GROUP_ID;
        srcChn.chn = 0;
        destChn.mod = QUA_MPP_MOD_VOU;
        destChn.dev = video_layer;
        destChn.chn = 0;
    } else if (strcmp(QUA_SYS_CHIP_MC331x, chip_name) == 0) {
        filter_chn_attr.filter_chn = QUA_MM_FILTER_VGS_CHN;
        ret = vppu_device->parent.create_channel((qua_mm_device_t *)vppu_device, &chn_id, (QUA_VOID_PTR)&filter_chn_attr, &mm_channel);
        if (ret != QUA_SUCCESS) {
            printf("Error: create vgs channel return %d\n", ret);
            return -1;
        }

        vgs_chn = (qua_mm_vgs_channel_t *)mm_channel;
        qua_vgs_chn_mode_t vgs_chn_mode;

        w_align = ALIGN_UP(img_width, 8);
        h_align = ALIGN_UP(img_height, 8);
        vgs_chn_mode.chn_mode = QUA_CHN_MODE_AUTO;
        vgs_chn_mode.width = w_align;
        vgs_chn_mode.height = h_align;
        vgs_chn_mode.pixel_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
        ret = vgs_chn->set_chn_mode(VGS_CHANNEL_ID, 0, &vgs_chn_mode);
        if (ret != QUA_SUCCESS) {
            printf("Error: set vgs chn mode %d\n", ret);
            return -1;
        }

        srcChn.mod = QUA_MPP_MOD_VPPU;
        srcChn.dev = 0;
        srcChn.chn = VGS_CHANNEL_ID;
        destChn.mod = QUA_MPP_MOD_VOU;
        destChn.dev = video_layer;
        destChn.chn = 0;
    }else if (strcmp(QUA_SYS_CHIP_QM10XH, chip_name) == 0) {
        qua_vpss_grp_attr_t vpu_grp_attr;
        qua_vpu_size_t vpu_size;
        qua_vpu_chn_info_t vpu_chn_attr;
        qua_vpu_chn_config_t chn_cfg;

        memset(&vpu_grp_attr, 0, sizeof(vpu_grp_attr));
        w_align = ALIGN_UP(img_width, 16);
        h_align = ALIGN_UP(img_height, 16);
        vpu_grp_attr.max_width = w_align;
        vpu_grp_attr.max_height = h_align;
        vpu_grp_attr.ycmean_en = QUA_FALSE;
        vpu_grp_attr.ycmean_ds = 16;
        ret = vppu_device->create_grp(VPSS_GROUP_ID, &vpu_grp_attr);
        if (ret != QUA_SUCCESS) {
            printf("Error: create vpps group return %#X\n", ret);
            return -1;
        }

        vpu_size.size.width = img_width;
        vpu_size.size.height = img_height;
        vpu_size.crop_info.enable = QUA_FALSE;
        vpu_size.crop_info.rect.x = 0;
        vpu_size.crop_info.rect.y = 0;
        vpu_size.crop_info.rect.width = img_width;
        vpu_size.crop_info.rect.height = img_height;
        ret = vppu_device->set_vi_attr(VPSS_GROUP_ID, &vpu_size);
        if (ret != QUA_SUCCESS) {
            printf("Error: set vpu size return %#X\n", ret);
            return -1;
        }

        ret = vppu_device->enable(VPSS_GROUP_ID, QUA_VPU_MODE_MEM);
        if (ret != QUA_SUCCESS) {
            printf("Error: enable vpu group return %d\n", ret);
            return -1;
        }

        vpu_chn_attr.bgm_enable             = QUA_FALSE;
        vpu_chn_attr.cpy_enable             = QUA_FALSE;
        vpu_chn_attr.sad_enable             = QUA_FALSE;
        vpu_chn_attr.bgm_ds                 = 8;
        vpu_chn_attr.out_mode               = QUA_VPU_VOMODE_SCAN;
        vpu_chn_attr.support_mode           = 0x45f;
        vpu_chn_attr.bufnum                 = 0;    //use comm vb
        vpu_chn_attr.chn_max_size.width  = w_align;
        vpu_chn_attr.chn_max_size.height = h_align;
        vpu_chn_attr.max_stride = w_align;
        filter_chn_attr.filter_chn = QUA_MM_FILTER_VPU_CHN;
        filter_chn_attr.group_id = VPSS_GROUP_ID;
        filter_chn_attr.attr = (QUA_VOID_PTR)&vpu_chn_attr;
        ret = vppu_device->parent.create_channel((qua_mm_device_t *)vppu_device, &chn_id, (QUA_VOID_PTR)&filter_chn_attr, &mm_channel);
        if (ret != QUA_SUCCESS) {
            printf("Error: create vpu channel return %d\n", ret);
            return -1;
        }

        vpu_chn = (qua_mm_vpu_channel_t *)mm_channel;
        chn_cfg.chn_size.width = img_width;
        chn_cfg.chn_size.height = img_height;
        chn_cfg.stride = w_align;
        chn_cfg.offset = 0;
        chn_cfg.depth = 1;
        chn_cfg.crop_area.enable = QUA_FALSE;
        ret = vpu_chn->set_chn_attr(VPSS_GROUP_ID, chn_id, &chn_cfg);
        if (ret != QUA_SUCCESS) {
            printf("Error: set vpu chn attr return %d\n", ret);
            return -1;
        }

        ret = vpu_chn->open_chn(VPSS_GROUP_ID, chn_id);
        if (ret != QUA_SUCCESS) {
            printf("Error: open vpu chn attr return %d\n", ret);
            return -1;
        }

        ret = vpu_chn->set_vo_mode(VPSS_GROUP_ID, chn_id, QUA_VPU_VOMODE_SCAN);
        if (ret != QUA_SUCCESS) {
            printf("Error: open vpu chn attr return %d\n", ret);
            return -1;
        }

        srcChn.obj = QUA_OBJ_VPU_VO;
        srcChn.dev = VPSS_GROUP_ID;
        srcChn.chn = chn_id;
        destChn.obj= QUA_OBJ_VOU;
        destChn.dev = video_layer;
        destChn.chn = 0;
    } else if (strcmp(QUA_SYS_CHIP_QM10XD, chip_name) == 0) {
        filter_chn_attr.filter_chn = QUA_MM_FILTER_VGS_CHN;
        ret = vppu_device->parent.create_channel((qua_mm_device_t *)vppu_device, &chn_id, (QUA_VOID_PTR)&filter_chn_attr, &mm_channel);
        if (ret != QUA_SUCCESS) {
            printf("Error: create vgs channel return %d\n", ret);
            return -1;
        }

        vgs_chn = (qua_mm_vgs_channel_t *)mm_channel;
        qua_vgs_chn_mode_t vgs_chn_mode;

        w_align = ALIGN_UP(img_width, 8);
        h_align = ALIGN_UP(img_height, 8);
        vgs_chn_mode.chn_mode = QUA_CHN_MODE_AUTO;
        vgs_chn_mode.width = w_align;
        vgs_chn_mode.height = h_align;
        vgs_chn_mode.pixel_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
        ret = vgs_chn->set_chn_mode(VGS_CHANNEL_ID, 0, &vgs_chn_mode);
        if (ret != QUA_SUCCESS) {
            printf("Error: set vgs chn mode %d\n", ret);
            return -1;
        }

        srcChn.obj = QUA_OBJ_VPPU;
        srcChn.dev = 0;
        srcChn.chn = VGS_CHANNEL_ID;
        destChn.obj = QUA_OBJ_VOU;
        destChn.dev = video_layer;
        destChn.chn = 0;
    }else if (strcmp(QUA_SYS_CHIP_QM10XV, chip_name) == 0) {
        w_align = ALIGN_UP(img_width, 16);
        h_align = ALIGN_UP(img_height, 16);
    }

    if (strcmp(QUA_SYS_CHIP_QM10XV, chip_name) != 0) {
        sys_ops->sys_bind(&srcChn, &destChn);
        if (ret != QUA_SUCCESS) {
            printf("Error: sys bind return %d\n", ret);
            return -1;
        }
    }

    QUA_S32 yuv_size = w_align * h_align * 3 / 2;
    QUA_S32 y_size = w_align * h_align;
    QUA_S32 i = 0;

    pool_id = sys_ops->vb_create_pool(yuv_size, 2, NULL);
    if (pool_id == QUA_VB_INVALID_POOLID) {
        printf("Error: vb create pool w_align=%d h_align=%d yuv_size=%d\n", w_align, h_align, yuv_size);
        return -1;
    }

    for (; i < 100; i++) {
        blk_handle = sys_ops->vb_get_block(pool_id, yuv_size, NULL);
        if (blk_handle == QUA_VB_INVALID_HANDLE) {
            printf("Error: vb get blk %d\n", ret);
            return -1;
        }
        phy_addr = sys_ops->vb_handle_to_phyaddr(blk_handle);
        if (phy_addr == 0) {
            printf("Error: vb handle 2 phy addr  %d\n", ret);
            return -1;
        }
        vir_addr = sys_ops->sys_mmap(phy_addr, yuv_size);
        if (vir_addr == 0) {
            printf("Error: vb sys mmap %d\n", ret);
            return -1;
        }

        memset(vir_addr, (i%2 ? 0x0F : 0xFF), yuv_size);

        qua_video_frame_info_t frame_info;
        memset(&frame_info, 0, sizeof(qua_video_frame_info_t));
        frame_info.pool_id = pool_id;
        frame_info.video_frame.width = img_width;
        frame_info.video_frame.height = img_height;
        frame_info.video_frame.field = QUA_VIDOE_FIELD_FRAME;
        frame_info.video_frame.pixel_fmt = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
        frame_info.video_frame.video_fmt = QUA_VIDEO_FORMAT_LINEAR;
        frame_info.video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
        frame_info.video_frame.stride[0]= w_align;
        frame_info.video_frame.stride[1]= h_align;
        frame_info.video_frame.stride[2]= 0;

        frame_info.video_frame.phy_addr[0]= phy_addr;
        frame_info.video_frame.phy_addr[1]= phy_addr + y_size;
        frame_info.video_frame.phy_addr[2]= 0;

        frame_info.video_frame.vir_addr[0]= (QUA_U64)vir_addr;
        frame_info.video_frame.vir_addr[1]= (QUA_U64)vir_addr + y_size;
        frame_info.video_frame.vir_addr[2]= 0;

        frame_info.video_frame.offset_top = 0;
        frame_info.video_frame.offset_bottom = 0;//frame_info.video_frame.height;
        frame_info.video_frame.offset_left = 0;
        frame_info.video_frame.offset_right = 0;//frame_info.video_frame.width;

        if (vpss_chn != NULL)
            vppu_device->send_frame(VPSS_GROUP_ID, &frame_info, 0);
        else if (vgs_chn != NULL)
            vgs_chn->send_frame(VGS_CHANNEL_ID, &frame_info, QUA_TRUE, 0);
        else if (vpu_chn != NULL) {
            qua_vpu_user_pic_t pic;

            memset(&pic, 0, sizeof(qua_vpu_user_pic_t));
            pic.yluma = frame_info.video_frame.phy_addr[0];
            pic.chroma = frame_info.video_frame.phy_addr[1];
            pic.ystride = w_align;
            pic.cstride = w_align;
            pic.pic_size.width = img_width;
            pic.pic_size.height = img_height;
            pic.data_format = QUA_VPU_VOMODE_SCAN;
            pic.work_mode = QUA_VPU_MODE_MEM;
            ret = vppu_device->send_user_pic(VPSS_GROUP_ID, &pic);
            printf("send by vpu_chn, ret=%d\n", ret);
        } else if (vo_chn != NULL)
            vo_chn->send_frame(video_layer, chn_id, &frame_info, 0);

        usleep(30000);
        sys_ops->sys_munmap(vir_addr, yuv_size);
        sys_ops->vb_release_block(blk_handle);
    }

    printf("Test success!\n");
    sleep(2);
    if (vpss_chn != NULL) {
        vpss_chn->disable_chn(VPSS_GROUP_ID, chn_id);
        vppu_device->stop_grp(VPSS_GROUP_ID);
        vppu_device->destroy_grp(VPSS_GROUP_ID);
        vpss_chn->parent.release((qua_mm_channel_t *)vpss_chn);
    }

    if (vgs_chn != NULL)
        vgs_chn->parent.release((qua_mm_channel_t *)vgs_chn);

    if (vpu_chn != NULL) {
        vppu_device->disable(VPSS_GROUP_ID);
        vpu_chn->close_chn(VPSS_GROUP_ID, chn_id);
        vpu_chn->parent.release((qua_mm_channel_t *)vpu_chn);
        vppu_device->destroy_grp(VPSS_GROUP_ID);
    }

    if (vppu_device != NULL) {
        vppu_device->exit();
        vppu_device->parent.close((qua_mm_device_t *)vppu_device);
    }

    vo_chn->disable_chn(video_layer, chn_id);
    vo_chn->parent.release((qua_mm_channel_t *)vo_chn);
    vo_device->disable_video_layer(video_layer);
    if (vo_device->unbind_video_layer != NULL )
        vo_device->unbind_video_layer(video_layer, vo_num);

    vo_device->disable(vo_num);
    vo_device->parent.close((qua_mm_device_t *)vo_device);
    if (strcmp(QUA_SYS_CHIP_QM10XV, chip_name) != 0)
        sys_ops->sys_unbind(&srcChn, &destChn);

    sys_ops->vb_destroy_pool(pool_id);
    return 0;
}
