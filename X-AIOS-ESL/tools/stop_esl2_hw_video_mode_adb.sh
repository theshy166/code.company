#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"

adb connect "$DEVICE"
adb -s "$DEVICE" shell 'rm -f /data/.enable_hw_video_overlay; sleep 1; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; sleep 2; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done; if ! pidof qxosui >/dev/null 2>&1; then /etc/init.d/S99launcher start; fi; ps | grep "[q]xosui"'
