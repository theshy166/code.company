#/bin/bash
#echo 0 > /sys/class/gpio/gpio15/value
#echo 1 > /sys/class/gpio/gpio15/value
#sleep 2
ifconfig lo up
ifconfig wlan0 169.254.57.1 up
cd  /data/host
touch /data/udhcpd.leases
mkdir  /data/run
echo /data/%e.%p.%s.%t.core > /proc/sys/kernel/core_pattern
./udhcpd  udhcpd2G.conf wlan0 &
export PATH=$PATH:/vendor/qua/wifi/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib
./hostapd  hostapd2G.conf  -B



