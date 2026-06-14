#!/bin/sh

echo *****autorun script*****

cd /vendor/qua/ko
./loadko.sh -i

#run qxosui
if [ -f /data/.debug ];then
echo /data/%e_core_dump.%p > /proc/sys/kernel/core_pattern
fi

cd /app
echo "start run /app/qxosui &"
if [ -f /data/preload.so ];then
LD_PRELOAD=/data/preload.so /app/qxosui 2>&1 1>/dev/kmsg  &
else
./qxosui 2>&1 1>/dev/kmsg  &
fi
echo -1000 >  /proc/$(pidof qxosui)/oom_score_adj
cd -
