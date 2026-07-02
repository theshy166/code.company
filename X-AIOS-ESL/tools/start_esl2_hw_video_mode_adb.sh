#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
VIDEO_PATH="${VIDEO_PATH:-$ROOT_DIR/core/apps/esl2/view/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4}"
BOARD_VIDEO="${BOARD_VIDEO:-/data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4}"
BOARD_STEM="${BOARD_VIDEO%.*}"
BOARD_EXT="${BOARD_VIDEO##*.}"
BOARD_VIDEO_0="${BOARD_VIDEO_0:-${BOARD_STEM}_0.${BOARD_EXT}}"
BOARD_VIDEO_1="${BOARD_VIDEO_1:-${BOARD_STEM}_1.${BOARD_EXT}}"
BOARD_BIN="/data/bin/hw_video_overlay_quamm"
DISPLAY_MODE="${DISPLAY_MODE:-both}"
FILL_SCREEN="${FILL_SCREEN:-1}"
HIDE_FB="${HIDE_FB:-1}"
ROTATION="${ROTATION:-0}"
CLEANUP="${CLEANUP:-0}"
PUNCH_HOLE="${PUNCH_HOLE:-0}"
VIDEO_PRIORITY="${VIDEO_PRIORITY:-3}"

if [ ! -f "$VIDEO_PATH" ]; then
    echo "missing video: $VIDEO_PATH" >&2
    exit 1
fi

adb connect "$DEVICE"
adb -s "$DEVICE" shell 'mkdir -p /data/res/img'
adb -s "$DEVICE" push "$VIDEO_PATH" "$BOARD_VIDEO_0.new"
adb -s "$DEVICE" shell "rm -f '$BOARD_VIDEO_0' '$BOARD_VIDEO_1' '$BOARD_VIDEO'; mv '$BOARD_VIDEO_0.new' '$BOARD_VIDEO_0'; cp '$BOARD_VIDEO_0' '$BOARD_VIDEO_1'; cp '$BOARD_VIDEO_0' '$BOARD_VIDEO'; chmod 666 '$BOARD_VIDEO_0' '$BOARD_VIDEO_1' '$BOARD_VIDEO' 2>/dev/null || true; sync"
adb -s "$DEVICE" shell 'rm -f /data/.enable_video_overlay /data/.enable_image_overlay /data/.enable_native_video_overlay'
adb -s "$DEVICE" shell 'rm -f /data/.enable_hw_h264_overlay; for p in $(pidof hw_h264_dual_overlay_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; sleep 1; for p in $(pidof hw_h264_dual_overlay_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done'
adb -s "$DEVICE" shell 'rm -f /data/.enable_hw_video_overlay; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; sleep 1; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done'
adb -s "$DEVICE" shell "rm -f /data/hw_video_overlay_quamm.log; touch /data/.enable_hw_video_overlay; nohup '$BOARD_BIN' '$BOARD_VIDEO' /data/.enable_hw_video_overlay '$DISPLAY_MODE' '$FILL_SCREEN' '$HIDE_FB' '$ROTATION' '$CLEANUP' '$PUNCH_HOLE' '$VIDEO_PRIORITY' >/data/hw_video_overlay_quamm.log 2>&1 &"
adb -s "$DEVICE" shell 'sleep 2; ps | grep "[h]w_video_overlay_quamm"; ps | grep "[q]xosui"; tail -n 80 /data/hw_video_overlay_quamm.log 2>/dev/null'
