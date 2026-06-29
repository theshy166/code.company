#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.72:5555}"

adb connect "$DEVICE"
adb -s "$DEVICE" shell "cp /system/factory/bin/qxosui /data/bin/qxosui && chmod 755 /data/bin/qxosui && rm -f /data/.skip_wifi_restart && sync"
adb -s "$DEVICE" shell "/etc/init.d/S99launcher restart"
