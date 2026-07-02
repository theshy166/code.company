// ============================================================
// hw_video_player_quamm.c — 方案一：QUA MM Player API 双播放器方案
// ============================================================
// 【方案概述】
//   使用方式：QUA MM Player API (qua_mm_player.h) 高层封装
//   为每个屏幕创建独立 Player 实例，通过 KEY_PARAMETER_VO_DISPLAY_ID
//   指定输出到不同显示器。
//   QUA MM 初始化模式：SLAVE (QUA_FALSE)，与 qxosui 共存。
//   输入：MP4 文件。
//   优点：API 简单，单屏测试（display1）流畅循环播放成功。
//
//   Player 内部管线：MediaExtractor -> VDEC -> VPPO -> VO
//   每个 Player 实例有独立的 VDEC + VPPO 通道。
//
//   【失败原因】VPPO 通道分配冲突
//     - VPPO (Video Post Processing & Output) 按先到先得分配 vo_chn_id
//     - display1 先启动获取 vo_chn_id=0，走 decoder-init 路径成功
//     - display0 后启动获取 vo_chn_id=1，走 sys-bind 路径失败
//       错误：0xa0028009 (VPPO bind error)
//     - 改为启动顺序 display1 先 (start_order={1,0}) 则两者初始化都成功
//       但播放时出现 VGS send frame 错误 (0xa00f8012)
//     - 最终现象：一个屏幕出现一帧就白屏，另一个播放一段时间后卡住
//
//   【根本原因】
//     - VPPO 的 vo_chn_id 分配与显示硬件物理映射不一致
//     - 两个 Player 各自独立的 VDEC+VPPO 管道同时运行时产生资源冲突
//     - VPPO 只能管理一个 VO 输出通道，无法直接支持多屏
// ============================================================
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common/qua_sys_platform.h"
#include "display/qua_mm_display.h"
#include "filter/qua_mm_filter.h"
#include "player/qua_mm_player.h"
#include "player/qua_mm_player_common.h"
#include "system/qua_mm_system.h"

typedef struct {
    void *player;
    int complete;
    const char *display_id;
    const char *path;
    int group_id;
} player_state_t;

static volatile sig_atomic_t g_running = 1;

static void on_signal(int sig)
{
    (void)sig;
    g_running = 0;
}

static QUA_S32 player_callback(QUA_VOID *user, QUA_S32 evt, QUA_VOID *info)
{
    (void)info;
    player_state_t *state = (player_state_t *)user;
    if (evt == PLAYER_EVT_PLAYBACK_COMPLETE) {
        fprintf(stderr, "%s playback complete\n", state->display_id);
        state->complete = 1;
    } else if (evt == PLAYER_EVT_ERROR) {
        fprintf(stderr, "%s player error\n", state->display_id);
        state->complete = 1;
    }
    return 0;
}

static int start_player(player_state_t *state, const char *path, const char *display_id, int group_id)
{
    memset(state, 0, sizeof(*state));
    state->display_id = display_id;
    state->path = path;
    state->group_id = group_id;

    state->player = qua_mm_player_create(path);
    if (state->player == NULL) {
        fprintf(stderr, "%s create player failed\n", display_id);
        return -1;
    }

    QUA_S32 ret = qua_mm_player_set_data_source(state->player, path);
    if (ret != 0) {
        fprintf(stderr, "%s set data source failed: %d\n", display_id, ret);
        return -1;
    }

    qua_rect_t display_rect = {0, 0, 800, 1280};
    qua_rect_t video_rect = {0, 0, 800, 1280};
    int rotation = 0;
    QUA_BOOL pos_cb = QUA_TRUE;
    QUA_BOOL loop = QUA_TRUE;

    qua_mm_player_set_parameter(state->player, KEY_PARAMETER_VO_DISPLAY_ID, (QUA_VOID_PTR)display_id);
    qua_mm_player_set_parameter(state->player, KEY_PARAMETER_VO_DISPLAY_RECT, (QUA_VOID_PTR)&display_rect);
    qua_mm_player_set_parameter(state->player, KEY_PARAMETER_VO_CHN_RECT, (QUA_VOID_PTR)&video_rect);
    qua_mm_player_set_parameter(state->player, KEY_PARAMETER_VO_ROTATE, (QUA_VOID_PTR)&rotation);
    qua_filter_chn_attr_t filter_attr;
    memset(&filter_attr, 0, sizeof(filter_attr));
    filter_attr.filter_chn = QUA_MM_FILTER_VGS_CHN;
    filter_attr.group_id = group_id;
    qua_mm_player_set_parameter(state->player, KEY_PARAMETER_FILTER_CHN_ATTR, (QUA_VOID_PTR)&filter_attr);
    qua_mm_player_set_loop(state->player, loop);

    ret = qua_mm_player_prepare(state->player);
    if (ret != 0) {
        fprintf(stderr, "%s prepare failed: %d\n", display_id, ret);
        return -1;
    }

    ret = qua_mm_player_set_callback(state->player, player_callback, state);
    if (ret != 0) {
        fprintf(stderr, "%s set callback failed: %d\n", display_id, ret);
        return -1;
    }

    qua_mm_player_set_parameter(state->player, KEY_PARAMETER_SET_CURRENT_POSITION_CALLBACK, (QUA_VOID *)&pos_cb);

    QUA_S64 duration = qua_mm_player_get_durationUs(state->player);
    fprintf(stderr, "%s durationUs=%lld\n", display_id, (long long)duration);

    ret = qua_mm_player_start(state->player);
    if (ret != 0) {
        fprintf(stderr, "%s start failed: %d\n", display_id, ret);
        return -1;
    }

    fprintf(stderr, "%s player started path=%s group=%d\n", display_id, path, group_id);
    return 0;
}

static void stop_player(player_state_t *state)
{
    if (state->player != NULL) {
        qua_mm_player_stop(state->player);
        qua_mm_player_destroy(state->player);
        state->player = NULL;
    }
}

static int restart_player(player_state_t *state)
{
    const char *path = state->path;
    const char *display_id = state->display_id;
    int group_id = state->group_id;
    stop_player(state);
    return start_player(state, path, display_id, group_id);
}

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "/data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_video_overlay";
    int mode = argc > 3 ? atoi(argv[3]) : 2;

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);

    char platform[32] = {0};
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");
        return 1;
    }

    qua_mm_system_t *system = NULL;
    if (qua_mm_init(QUA_FALSE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init attach failed\n");
        return 1;
    }

    player_state_t players[2];
    int count = 0;

    if ((mode == 0 || mode == 2) && start_player(&players[count], path, "id:display0", 0) == 0) {
        count++;
    }

    if ((mode == 1 || mode == 2) && start_player(&players[count], path, "id:display1", 1) == 0) {
        count++;
    }

    if (count == 0) {
        fprintf(stderr, "no hardware player started\n");
        return 1;
    }

    while (g_running && access(marker, F_OK) == 0) {
        for (int i = 0; i < count; i++) {
            if (players[i].complete && players[i].player != NULL) {
                players[i].complete = 0;
                restart_player(&players[i]);
            }
        }
        usleep(100 * 1000);
    }

    for (int i = 0; i < count; i++) {
        stop_player(&players[i]);
    }

    fprintf(stderr, "hardware video player stop\n");
    return 0;
}
