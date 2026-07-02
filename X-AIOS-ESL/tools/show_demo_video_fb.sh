#!/bin/sh

FRAME_DIR="${FRAME_DIR:-/data/res/img/video_frames_bgra}"
MARKER="${MARKER:-/data/.enable_video_overlay}"
LOG_PATH="${LOG_PATH:-/data/fb_video_overlay.log}"
SIZE_BLOCKS="${SIZE_BLOCKS:-1000}"
FRAME_US="${FRAME_US:-200000}"

sleep_frame()
{
    if command -v usleep >/dev/null 2>&1; then
        usleep "$FRAME_US"
    else
        sleep 1
    fi
}

echo "fb video overlay start: frames=$FRAME_DIR marker=$MARKER frame_us=$FRAME_US" >"$LOG_PATH"

while [ -f "$MARKER" ]; do
    found=0
    for frame in "$FRAME_DIR"/frame_*.bgra; do
        [ -f "$MARKER" ] || break
        [ -f "$frame" ] || continue

        found=1
        [ -e /dev/fb0 ] && dd if="$frame" of=/dev/fb0 bs=4096 count="$SIZE_BLOCKS" >/dev/null 2>&1
        [ -e /dev/fb4 ] && dd if="$frame" of=/dev/fb4 bs=4096 count="$SIZE_BLOCKS" >/dev/null 2>&1
        sleep_frame
    done

    if [ "$found" -eq 0 ]; then
        echo "missing frames: $FRAME_DIR/frame_*.bgra" >>"$LOG_PATH"
        sleep 1
    fi
done

echo "fb video overlay stop" >>"$LOG_PATH"
