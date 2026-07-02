#!/bin/sh

FRAME_DIR="${FRAME_DIR:-/data/res/img/video_frames_bgra}"
MARKER="${MARKER:-/data/.enable_video_overlay}"
FRAME_US="${FRAME_US:-66666}"
FRAME_W="${FRAME_W:-800}"
FRAME_H="${FRAME_H:-1280}"
DISPLAY_MODE="${DISPLAY_MODE:-both}"
LOG_PATH="${LOG_PATH:-/data/video_overlay_quamm.log}"

exec </dev/null >"$LOG_PATH" 2>&1
exec /data/bin/video_overlay_quamm "$FRAME_DIR" "$MARKER" "$FRAME_US" "$FRAME_W" "$FRAME_H" "$DISPLAY_MODE"
