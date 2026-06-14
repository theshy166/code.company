#!/bin/sh
pkill  wpa_supplicant;
pkill  udhcpc;
sleep 0.2
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib;
ifconfig wlan0 down
sleep 0.1
wpa_supplicant -iwlan0 -D nl80211 -c /data/wifi/wpa_supplicant.conf -B &
sleep 1
counter=0
while true; do
    interface_status=$(ip addr show wlan0 | grep LOWER_UP)
    if [ -n "$interface_status" ] || [ $counter -gt 60 ]; then
        echo "wlan0 is ap connected,time to start udhcpc"
        udhcpc -q -iwlan0 &
        break
    fi
    counter=$((counter + 1))
    sleep 0.5
done
exit 0
