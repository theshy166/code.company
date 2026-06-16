/*
 *  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "utils/qua_display_parser.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "player/qua_mm_player.h"

QUA_BOOL is_stop = QUA_FALSE;

QUA_S32 cnt = 0;
QUA_S32 call_back(QUA_VOID* user, QUA_S32 evt, QUA_VOID* info)
{
    (void)user;
    if (evt == PLAYER_EVT_PLAYBACK_COMPLETE) {
        is_stop = QUA_TRUE;
        printf("call_back evt = %d\n", evt);
    } else if (evt == PLAYER_EVT_CURRENT_POSITION) {
        // printf("pts = %lld\n", (long long)(*(QUA_U64*)info));
    }
    return 0;
}

static const QUA_CHAR* display_id[] = {
    "id:display0",
    "id:display1",
    "id:display2",
    "id:display3",
};

int main(int argc, char* argv[])
{
    char *chip_name = NULL;
    char *os_name = NULL;
    qua_mm_system_t *system = NULL;
    qua_mm_system_ops_t *sys_ops;
    qua_mm_vo_device_t *vo_dev;
    const qua_mm_module_t *disp_module = NULL;
    qua_mm_device_t *mm_device = NULL;
    qua_vo_pub_attr_t dev_attr = { 0 };
    QUA_U32 blkSize;
    qua_vb_config_t vbCfg;
    const char *src_filename = NULL;
    void *player = NULL;
    QUA_S32 ret = 0;
    QUA_S64 durationUs = 0;
    QUA_BOOL pos_cb = QUA_TRUE;
    QUA_CHAR platform[32] = { 0 };
    qua_rect_t rect = { };
    QUA_S32 width = 0;
    QUA_S32 height = 0;
    QUA_CONST_CHAR *vo_id = NULL;
    QUA_CONST_CHAR *fb_id = NULL;
    QUA_S32 vo_num;
    QUA_S32 fb_num;
    QUA_S32 display_idx = 0;
    if (argc != 7) {
        fprintf(stderr, "usage: %s with input_file \n", argv[0]);
        fprintf(stderr, "usage: ./qua_player_sample qm10xv linux src.mp4 320 240 \n");
        exit(1);
    }
    chip_name = argv[1];
    os_name = argv[2];
    src_filename = argv[3];
    width = atoi(argv[4]);
    height = atoi(argv[5]);
    display_idx = atoi(argv[6]);
    printf("width = %d, height = %d\n", width, height);
    ret = qua_make_platform(chip_name, os_name, platform, sizeof(platform));
    if (ret == QUA_FAILURE) {
        printf("make platform name error!\n");
        exit(1);
    }
    ret = qua_mm_init(QUA_TRUE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        printf("qua_mm_init error!\n");
        return ret;
    }
    sys_ops = (qua_mm_system_ops_t *)system;
    memset(&vbCfg, 0, sizeof(qua_vb_config_t));
    blkSize = 1024;
    vbCfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vbCfg.common_pools[0].block_size = blkSize;
    vbCfg.common_pools[0].block_cnt = 6;
    ret = sys_ops->sys_init(&vbCfg);
    if (ret != QUA_SUCCESS) {
        printf("sys_init error!\n");
        return ret;
    }

    // dlopen vo module
    ret = qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &disp_module);
    if (ret != QUA_SUCCESS || disp_module == NULL) {
        printf("ret = %#x, get disp_module fail", ret);
        exit(1);
    }
    mm_device = NULL;
    qua_init_display_parser();
    ret = qua_parse_display_id(display_id[display_idx], &vo_id, &vo_num, &fb_id, &fb_num);
    printf("vo_id = %s vo_num = %d", vo_id, vo_num);
    if (ret != QUA_SUCCESS) {
        printf("%s display_id = %s error", __func__, display_id[display_idx]);
        exit(1);
    }
    ret = disp_module->open_device(disp_module, vo_id, vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("ret = %#x, get vo_dev fail", ret);
        exit(1);
    }
    vo_dev = (qua_mm_vo_device_t *)mm_device;

    dev_attr.intf_type = QUA_VO_INTF_LCD;
    dev_attr.bg_color = 0xffffff;
    dev_attr.intf_sync = QUA_VO_OUTPUT_1024x600_60;
    ret = vo_dev->set_public_attr(vo_num, &dev_attr);
    if (ret != QUA_SUCCESS) {
        printf("vo_dev set attr fail ret = %#x", ret);
        exit(1);
    }
    ret = vo_dev->enable(vo_num);
    if (ret != QUA_SUCCESS) {
        printf("vo_dev enable chn fail ret = %#x", ret);
        exit(1);
    }

    printf("work start >>>>\n");
    printf("src_filename: %s\n", src_filename);
    player = qua_mm_player_create(src_filename);
    printf("player: %p\n", player);
    if (player == NULL) {
        exit(1);
    }
    rect.x = 0;
    rect.y = 0;
    rect.width = width;
    rect.height = height;
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_DISPLAY_ID, (QUA_VOID_PTR)display_id[display_idx]);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_DISPLAY_RECT, (QUA_VOID_PTR)&rect);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_CHN_RECT, (QUA_VOID_PTR)&rect);
    ret = qua_mm_player_prepare(player);
    if (ret != 0) {
        printf("player: prepare fail\n");
        goto fail;
    }
    ret = qua_mm_player_set_callback(player, call_back, NULL);
    if (ret != 0) {
        printf("player: set_callback fail\n");
        goto fail;
    }
    durationUs = qua_mm_player_get_durationUs(player);
    printf("durationUs: %lld\n", (long long)durationUs);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_SET_CURRENT_POSITION_CALLBACK, (QUA_VOID*)&pos_cb);

    ret = qua_mm_player_start(player);
    if (ret != 0) {
        printf("player: start fail\n");
        goto fail;
    }
    while (!is_stop) {
        usleep(100*1000); // 100ms
        switch (cnt) {
        case 10: {
            // printf("seek\n");
            // qua_mm_player_seek(player, durationUs * 2 / 3);
            // printf("speed_rate\n");
            // QUA_DOUBLE speed_rate = 0.5;
            // qua_mm_player_set_parameter(player, KEY_PARAMETER_SPEED_RATE, (QUA_VOID*)&speed_rate);
            // qua_mm_player_seek(player, durationUs / 2);
        } break;
        case 20: {
            // QUA_DOUBLE speed_rate = 0.5;
            // qua_mm_player_set_parameter(player, KEY_PARAMETER_SPEED_RATE, (QUA_VOID*)&speed_rate);
            // qua_mm_player_pause(player);
            // qua_mm_player_set_volume(player, 50);
        } break;
        case 30:{
            // printf("resume\n");
            // qua_mm_player_resume(player);
            // printf("seek durationUs 5/6\n");
            // qua_mm_player_seek(player, durationUs * 3 / 4);
            // qua_mm_player_set_volume(player, 95);
        } break;
        case 40: {
            // printf("pos callback\n");
            // QUA_BOOL pos_cb = QUA_TRUE;
            // qua_mm_player_set_parameter(player, KEY_PARAMETER_SET_CURRENT_POSITION_CALLBACK, (QUA_VOID*)&pos_cb);
            // qua_mm_player_set_volume(player, 30);
        } break;
        case 50:
            // printf("seek durationUs 5/6\n");
            // qua_mm_player_seek(player, durationUs * 5 / 6);
            break;
        default:
            break;
        }
        cnt++;
    };
    ret = qua_mm_player_stop(player);
    if (ret != 0) {
        printf("player: stop fail\n");
        goto fail;
    }
    printf("well down good job!\n");
fail:
    if (player) {
        qua_mm_player_destroy(player);
    }
    if (vo_dev) {
        vo_dev->disable(vo_num);
        vo_dev->parent.close((qua_mm_device_t *)vo_dev);
        vo_dev = NULL;
    }
    return 0;
}