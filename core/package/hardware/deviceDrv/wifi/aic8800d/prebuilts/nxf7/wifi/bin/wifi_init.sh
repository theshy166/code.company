#/bin/sh


devmem  0x1f000050 32  0x41
devmem  0x1f000054 32  0x41
devmem  0x1f000058 32  0x41
devmem  0x1f00005c 32  0x41
devmem  0x1f000060 32  0x41
devmem  0x1f000064 32  0x41
devmem  0x1f000068 32  0x41


echo 15 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio15/direction
echo 0 > /sys/class/gpio/gpio15/value
sleep 1
echo 1 > /sys/class/gpio/gpio15/value


echo 87 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio87/direction
echo 0 > /sys/class/gpio/gpio87/value
sleep 1
echo 1 > /sys/class/gpio/gpio87/value


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib
export PATH=$PATH:/vendor/qua/BT:/vendor/qua/wifi/bin:/system/qua/wifi/bin
cd /vendor/qua/wifi/ko;insmod aic8800_bsp.ko aic_fw_path=/vendor/qua/wifi/lib/aic8800D80
insmod aic8800_fdrv.ko;insmod aic8800_btlpm.ko;cd ../../BT;
#sleep 0.5
#lbh_server -p "ble_userconfig.json" -s ble uart 1500000 /dev/ttyS3  2>&1 1>/dev/null &


echo "insmod AIC8800D.ko"
#sleep 1
#echo 0 > /proc/net/rtl8821cs/log_level
#blueware /etc/bluetooth/blueware.properties &
#echo "blueware /etc/bluetooth/blueware.properties"
