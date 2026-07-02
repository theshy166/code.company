#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common/qua_sys_platform.h"
#include "display/qua_mm_display.h"
#include "player/qua_mm_player.h"
#include "player/qua_mm_player_common.h"
#include "system/qua_mm_system.h"

typedef struct {
    void *player;
    int complete;
    const char *display_id;
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

static int start_player(player_state_t *state, const char *path, const char *display_id)
{
    memset(state, 0, sizeof(*state));
    state->display_id = display_id;

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

    fprintf(stderr, "%s player started path=%s\n", display_id, path);
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

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "/data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4";
    const char *marker = argc > 2 ? argv[2] : "/data/.enable_video_overlay";
    int dual = argc > 3 ? atoi(argv[3]) : 1;

    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);

    char platform[32] = {0};
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");
        return 1;
    }

    qua_mm_system_t *system = NULL;
    if (qua_mm_init(QUA_TRUE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init primary failed\n");
        return 1;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)system;
    qua_vb_config_t vb_cfg;
    memset(&vb_cfg, 0, sizeof(vb_cfg));
    vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vb_cfg.common_pools[0].block_size = 1024;
    vb_cfg.common_pools[0].block_cnt = 6;
    QUA_S32 sys_ret = sys_ops->sys_init(&vb_cfg);
    if (sys_ret != QUA_SUCCESS) {
        fprintf(stderr, "sys_init returned %d, continue\n", sys_ret);
    }

    player_state_t players[2];
    int count = 0;

    if (start_player(&players[count], path, "id:display0") == 0) {
        count++;
    }

    if (dual && start_player(&players[count], path, "id:display1") == 0) {
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
                qua_mm_player_seek(players[i].player, 0);
                qua_mm_player_start(players[i].player);
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
