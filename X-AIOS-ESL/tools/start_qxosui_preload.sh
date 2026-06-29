#!/bin/sh
pid="$(pidof qxosui 2>/dev/null || true)"
if [ -n "$pid" ]; then
    kill $pid 2>/dev/null || true
fi
sleep 1
setsid -f /data/bin/qxosui_with_preload.sh >/data/qxosui_preload_start.log 2>&1 </dev/null
