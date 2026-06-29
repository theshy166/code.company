#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
BIN_PATH="$ROOT_DIR/output/qxosui.elf"
LIB_DIR="$ROOT_DIR/output/lib"
VIDEO_PATH="$ROOT_DIR/core/apps/esl2/view/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4"
FRAME_DIR="$ROOT_DIR/core/apps/esl2/view/video_frames"
KEEP_ADB_PATH="$ROOT_DIR/tools/board_keep_adb.sh"
STAMP="$(date +%Y%m%d_%H%M%S)"
DEMO_MODE="${DEMO_MODE:-image}"

if [ "${ENABLE_VIDEO_OVERLAY:-0}" = "1" ]; then
    DEMO_MODE="video"
fi

if [ ! -f "$BIN_PATH" ]; then
    echo "missing binary: $BIN_PATH" >&2
    exit 1
fi

if [ ! -d "$LIB_DIR" ]; then
    echo "missing library directory: $LIB_DIR" >&2
    exit 1
fi

if [ ! -f "$VIDEO_PATH" ]; then
    echo "missing video: $VIDEO_PATH" >&2
    exit 1
fi

if [ "$DEMO_MODE" != "none" ] && [ ! -d "$FRAME_DIR" ]; then
    echo "missing frame directory: $FRAME_DIR" >&2
    exit 1
fi

if [ ! -f "$KEEP_ADB_PATH" ]; then
    echo "missing keep-adb helper: $KEEP_ADB_PATH" >&2
    exit 1
fi

adb connect "$DEVICE"
adb -s "$DEVICE" shell "mkdir -p /data/bin /data/res/img"
adb -s "$DEVICE" shell "rm -f /data/bin/qxosui.backup_* /data/bin/qxosui.bak_codex_* /data/bin/qxosui.factory_backup_* /data/bin/qxosui.factory_before_codex_* /data/bin/qxosui.factory_running_codex_* /data/bin/qxosui.old_running_codex_* /data/bin/qxosui.skipwifi_codex_*"
adb -s "$DEVICE" shell "cp /data/bin/qxosui /data/bin/qxosui.backup_${STAMP} 2>/dev/null || true"
adb -s "$DEVICE" push "$KEEP_ADB_PATH" /data/bin/keep_adb.sh
adb -s "$DEVICE" shell "chmod 755 /data/bin/keep_adb.sh && touch /data/.skip_wifi_restart"
adb -s "$DEVICE" shell 'if [ -f /data/lib/libquamm.player.so ]; then mv /data/lib /data/lib.codex_old_full_$(date +%s); fi; mkdir -p /data/lib'
adb -s "$DEVICE" push "$LIB_DIR/libagora-rtc-sdk.so" /data/lib/
adb -s "$DEVICE" push "$LIB_DIR/libquaesl.so" /data/lib/
adb -s "$DEVICE" push "$VIDEO_PATH" /data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4
adb -s "$DEVICE" push "$VIDEO_PATH" /data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640_0.mp4
adb -s "$DEVICE" push "$VIDEO_PATH" /data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640_1.mp4
if [ "$DEMO_MODE" = "video" ]; then
    adb -s "$DEVICE" shell "mkdir -p /data/res/img/video_frames && touch /data/.enable_video_overlay && rm -f /data/.enable_image_overlay"
    adb -s "$DEVICE" push "$FRAME_DIR/." /data/res/img/video_frames/
elif [ "$DEMO_MODE" = "image" ]; then
    adb -s "$DEVICE" shell "mkdir -p /data/res/img/video_frames && touch /data/.enable_image_overlay && rm -f /data/.enable_video_overlay"
    adb -s "$DEVICE" push "$FRAME_DIR/." /data/res/img/video_frames/
else
    adb -s "$DEVICE" shell "rm -f /data/.enable_video_overlay /data/.enable_image_overlay"
fi
adb -s "$DEVICE" push "$BIN_PATH" /data/bin/qxosui.new
adb -s "$DEVICE" shell "chmod 755 /data/bin/qxosui.new && mv /data/bin/qxosui.new /data/bin/qxosui && touch /data/.skip_wifi_restart && sync"
adb -s "$DEVICE" shell "/etc/init.d/S99launcher restart"
