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

#export PATH=$PATH:/vendor/qua/wifi/ko
#export PATH=$PATH:/vendor/qua/wifi/bin

cd  /vendor/qua/wifi/ko;
insmod skw_sdio_lite.ko;
insmod swt6621s_wifi.ko;
insmod skwbt.ko;
cd ../../../../;

echo "insmod swt6621s_wifi.ko"
dmesg -n 1
#blueware /etc/bluetooth/blueware.properties &
#echo "blueware /etc/bluetooth/blueware.properties"
