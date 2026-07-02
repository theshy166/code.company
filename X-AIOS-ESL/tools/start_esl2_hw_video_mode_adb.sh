#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
VIDEO_PATH="${VIDEO_PATH:-$ROOT_DIR/core/apps/esl2/view/d425b305eccd909c6072b25232130e2d_h264_800x640.mp4}"
WORK_DIR="${TMPDIR:-/tmp}/esl2_hw_video_overlay"
LOCAL_BIN="${LOCAL_BIN:-$WORK_DIR/hw_video_overlay_quamm}"
LOCAL_H264="${LOCAL_H264:-$WORK_DIR/video_annexb.h264}"
LOCAL_RUNNER="${LOCAL_RUNNER:-$WORK_DIR/start_hw_video_overlay_quamm.sh}"
BOARD_BIN="/data/bin/hw_video_overlay_quamm"
BOARD_RUNNER="/data/bin/start_hw_video_overlay_quamm.sh"
BOARD_H264="${BOARD_H264:-/data/res/img/video_overlay_800x640.h264}"
MARKER="${MARKER:-/data/.enable_hw_video_overlay}"
DISPLAY_MODE="${DISPLAY_MODE:-both}"
HIDE_FB="${HIDE_FB:-1}"
FPS="${FPS:-30}"
STREAM_W="${STREAM_W:-800}"
STREAM_H="${STREAM_H:-640}"
DISPLAY_W="${DISPLAY_W:-800}"
DISPLAY_H="${DISPLAY_H:-1280}"
VDEC_VB_CNT="${VDEC_VB_CNT:-2}"
COPY_BUF_CNT="${COPY_BUF_CNT:-2}"
VIDEO_PRIORITY="${VIDEO_PRIORITY:-3}"
VIDEO_BITRATE="${VIDEO_BITRATE:-600k}"
CC="${CC:-$ROOT_DIR/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/arm-linux-gcc}"

if [ ! -f "$VIDEO_PATH" ]; then
    echo "missing video: $VIDEO_PATH" >&2
    exit 1
fi

mkdir -p "$WORK_DIR"

ffmpeg -y -hide_banner -loglevel error \
    -i "$VIDEO_PATH" \
    -vf "scale=${STREAM_W}:${STREAM_H}:force_original_aspect_ratio=decrease,pad=${STREAM_W}:${STREAM_H}:(ow-iw)/2:(oh-ih)/2" \
    -an -r "$FPS" -c:v libx264 -preset veryfast -profile:v baseline -level 3.1 \
    -pix_fmt yuv420p -b:v "$VIDEO_BITRATE" \
    -x264-params "bframes=0:ref=1:keyint=${FPS}:min-keyint=${FPS}:scenecut=0" \
    -f h264 "$LOCAL_H264"

"$CC" -O2 -Wall -Wextra \
    -I"$ROOT_DIR/core/package/libnative/qlibquammapi/include" \
    -o "$LOCAL_BIN" "$ROOT_DIR/tools/hw_video_overlay_quamm.c" \
    -L"$ROOT_DIR/output/lib" \
    -lquamm.player -lquamm_common -lquamm_utils -lsysutils \
    -Wl,-rpath,/system/qua/lib

cat >"$LOCAL_RUNNER" <<'EOF'
#!/bin/sh
exec /data/bin/hw_video_overlay_quamm "$@" >/data/hw_video_overlay_quamm.log 2>&1
EOF
chmod 755 "$LOCAL_RUNNER"

adb connect "$DEVICE"
adb -s "$DEVICE" shell 'mkdir -p /data/bin /data/res/img'
adb -s "$DEVICE" push "$LOCAL_BIN" "$BOARD_BIN"
adb -s "$DEVICE" push "$LOCAL_RUNNER" "$BOARD_RUNNER"
adb -s "$DEVICE" shell "chmod 755 '$BOARD_BIN'"
adb -s "$DEVICE" shell "chmod 755 '$BOARD_RUNNER'"
adb -s "$DEVICE" push "$LOCAL_H264" "$BOARD_H264.new"
adb -s "$DEVICE" shell "mv '$BOARD_H264.new' '$BOARD_H264'; chmod 666 '$BOARD_H264' 2>/dev/null || true"
adb -s "$DEVICE" shell 'rm -f /data/.enable_video_overlay /data/.enable_image_overlay /data/.enable_native_video_overlay'
adb -s "$DEVICE" shell 'rm -f /data/.enable_hw_video_overlay /data/.enable_hw_h264_overlay; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; for p in $(pidof start_hw_video_overlay_quamm.sh 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; for p in $(pidof hw_h264_dual_overlay_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; for p in $(pidof hw_video_player_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; sleep 1; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done; for p in $(pidof start_hw_video_overlay_quamm.sh 2>/dev/null); do kill -9 $p 2>/dev/null || true; done; for p in $(pidof hw_h264_dual_overlay_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done; for p in $(pidof hw_video_player_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done'
adb -s "$DEVICE" shell "rm -f /data/hw_video_overlay_quamm.log; touch '$MARKER'; busybox start-stop-daemon -S -b -x '$BOARD_RUNNER' -- '$BOARD_H264' '$MARKER' '$DISPLAY_MODE' '$HIDE_FB' '$FPS' '$STREAM_W' '$STREAM_H' '$DISPLAY_W' '$DISPLAY_H' '$VDEC_VB_CNT' '$COPY_BUF_CNT' '$VIDEO_PRIORITY'"
adb -s "$DEVICE" shell 'sleep 3; ps | grep "[h]w_video_overlay_quamm"; ps | grep "[q]xosui"; tail -n 160 /data/hw_video_overlay_quamm.log 2>/dev/null'
