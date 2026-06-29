#!/bin/sh

FRAME_PATH="${FRAME_PATH:-/data/frame_bgra.bin}"
MARKER="${MARKER:-/data/.enable_image_overlay}"
LOG_PATH="${LOG_PATH:-/data/fb_image_overlay.log}"
SIZE_BLOCKS="${SIZE_BLOCKS:-1000}"

echo "fb image overlay start: frame=$FRAME_PATH marker=$MARKER" >"$LOG_PATH"

while [ -f "$MARKER" ]; do
    if [ -f "$FRAME_PATH" ]; then
        [ -e /dev/fb0 ] && dd if="$FRAME_PATH" of=/dev/fb0 bs=4096 count="$SIZE_BLOCKS" >/dev/null 2>&1
        [ -e /dev/fb4 ] && dd if="$FRAME_PATH" of=/dev/fb4 bs=4096 count="$SIZE_BLOCKS" >/dev/null 2>&1
    else
        echo "missing frame: $FRAME_PATH" >>"$LOG_PATH"
    fi
    sleep 2
done

echo "fb image overlay stop" >>"$LOG_PATH"
