#!/bin/sh
pkill -9 iw
pkill -9 wpa_supplicant;
pkill -9 udhcpc;
rm /data/wifi/wpa_supplicant.conf
sleep 1
exit 0
