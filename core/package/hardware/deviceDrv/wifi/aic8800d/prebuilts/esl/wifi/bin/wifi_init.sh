#!/bin/sh
#sleep 2
#echo "SDIO INIT SETTING"


#wifi driver capablit
devmem 0x25b00018  32  0x00000000
devmem 0x25b0001c  32  0x00000000
devmem 0x25b00020  32  0x00000000
devmem 0x25b00024  32  0x00000000
devmem 0x25b00028  32  0x00000000
devmem 0x25b0002c  32  0x00000000
devmem 0x25b00030  32  0x00000000



#bt  uart

sleep 0.1
devmem 0x102000ac  32 0x00000003
devmem 0x102000b0  32 0x00000003
devmem 0x102000b4  32 0x00000003
devmem 0x102000b8  32 0x00000003

#GPIO 
devmem 0x10200020  32 0x00000001
#devmem 0x1020009c  32 0x00000001

#sleep 0.1

#echo 41 > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio41/direction
#echo 0 > /sys/class/gpio/gpio41/value
#echo 1 > /sys/class/gpio/gpio41/value
#echo 0 > /sys/class/gpio/gpio41/value

#sleep 0.5

echo 9 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio9/direction
echo 0 > /sys/class/gpio/gpio9/value
echo 1 > /sys/class/gpio/gpio9/value
#sleep  0.5 

#echo "SDIO INIT begin"

#cd bin;./wifi_drv_ins.sh;cd ../
#cp /vendor/qua/wifi/bin/* /bin/; cp /vendor/qua/wifi/lib/* /lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib:/system/qua/wifi/lib/
export PATH=$PATH:/vendor/qua/BT:/vendor/qua/wifi/bin:/system/qua/BT:/system/qua/wifi/bin
cd /vendor/qua/wifi/ko;insmod aic8800_bsp.ko aic_fw_path=/vendor/qua/wifi/aic8800D80
insmod aic8800_fdrv.ko;insmod aic8800_btlpm.ko;cd ../../BT;
if [ ! -f /data/wifi/wpa_supplicant.conf ];then
mkdir -p /data/wifi/
#cp /etc/wpa_supplicant.conf /data/wifi/wpa_supplicant.conf
wificfg.sh
fi
if [ -f /data/wifi/wpa_supplicant.conf ];then
wpa_supplicant -iwlan0 -D nl80211 -c /data/wifi/wpa_supplicant.conf -B
sleep 1
udhcpc -T 2 -q -iwlan0 &
fi
#bt is not
sleep 30
lbh_server -p "ble_userconfig.json" -s ble uart 1500000 /dev/ttyS2 1>/dev/null 2>&1 &
echo -100 >  /proc/$(pidof lbh_server)/oom_score_adj
sleep 5
lbh_client 1>/dev/null 2>&1 &
echo -100 >  /proc/$(pidof lbh_client)/oom_score_adj





