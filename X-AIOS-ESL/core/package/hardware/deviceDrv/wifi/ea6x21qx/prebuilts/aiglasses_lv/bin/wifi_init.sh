#/bin/sh


devmem  0x0B010008  32 0x00000008
devmem  0x0B010004  32 0x00000008
devmem  0x0B010000  32 0x00000000
devmem  0x0B010000  32 0x00000008



echo 1 > /sys/class/fhmci/mmc_rescan

cd /vendor/qua/wifi/ko

insmod skw_sdio_lite.ko
insmod swt6621s_wifi.ko
insmod skwbt.ko
#sleep 5
#wpa_supplicant -iwlan0 -Dnl80211 -c /data/wifi/wpa_supplicant.conf -B &
#udhcpc -q -i wlan0 &

