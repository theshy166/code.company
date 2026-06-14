#!/bin/sh

#wifi driver capablit
devmem 0x25b00018  32  0x00000002
devmem 0x25b0001c  32  0x00000002
devmem 0x25b00020  32  0x00000002
devmem 0x25b00024  32  0x00000002
devmem 0x25b00028  32  0x00000002
devmem 0x25b0002c  32  0x00000002
devmem 0x25b00030  32  0x00000002

#bt  uart
sleep 0.1
devmem 0x102000ac  32 0x00000003
devmem 0x102000b0  32 0x00000003
devmem 0x102000b4  32 0x00000003
devmem 0x102000b8  32 0x00000003

#GPIO
devmem 0x10200050  32 0x00000001 #gpio2_6
devmem 0x10200020  32 0x00000001 #gpio1_1
#devmem 0x1020009c  32 0x00000001

echo 9 > /sys/class/gpio/export
sleep 0.1
echo out > /sys/class/gpio/gpio9/direction
sleep 0.1
echo 0 > /sys/class/gpio/gpio9/value

echo 22 > /sys/class/gpio/export
sleep 0.1
echo out > /sys/class/gpio/gpio22/direction
#sleep 0.2
echo 1 > /sys/class/gpio/gpio22/value
sleep 0.1
echo 0 > /sys/class/gpio/gpio22/value
#cat  /sys/class/gpio/gpio22/value
sleep 0.1

#echo 9 > /sys/class/gpio/export
#sleep 0.2
#echo out > /sys/class/gpio/gpio9/direction
echo 1 > /sys/class/gpio/gpio9/value
sleep 0.1
echo 0 > /sys/class/gpio/gpio9/value
sleep 0.2
echo 1 > /sys/class/gpio/gpio9/value
cat  /sys/class/gpio/gpio9/value

cd /vendor/qua/wifi/ko

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib:/system/qua/wifi/lib/
export PATH=$PATH:/vendor/qua/wifi/bin:/system/qua/wifi/bin

#echo 8 > /proc/sys/kernel/printk

insmod skw_sdio_lite.ko


MAC_FILE="/data/mac.txt"

if [ -f "$MAC_FILE" ]; then

    MAC=$(cat "$MAC_FILE")

    MAC=${MAC//:/}

    FORMATTED_MAC=$(echo "$MAC" | sed -r 's/(..)/0x\1,/g' | sed 's/,$//')
    echo "MACADDRESS: $FORMATTED_MAC"
else

    MAC=$(printf "FE%02x%02x%02x%02x%02x" \
        $((RANDOM%256)) $((RANDOM%256)) \
        $((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)))


    echo "${MAC:0:2}:${MAC:2:2}:${MAC:4:2}:${MAC:6:2}:${MAC:8:2}:${MAC:10:2}" > "$MAC_FILE"


    FORMATTED_MAC=$(echo "$MAC" | sed -r 's/(..)/0x\1,/g' | sed 's/,$//')
    echo "MACADDRESS: $FORMATTED_MAC"
fi

insmod swt6621s_wifi.ko mac=$FORMATTED_MAC
#insmod skwbt.ko bd_addr=34:9A:98:86:74:32
mac_address=$(printf '02:%02X:%02X:%02X:%02X:%02X\n' \
$((RANDOM%256)) $((RANDOM%256)) $((RANDOM%256)) \
$((RANDOM%256)) $((RANDOM%256)))

echo "mac address: $mac_address"
insmod skwbt.ko bd_addr=$mac_address


if [ ! -f /data/wifi/wpa_supplicant.conf ];then
mkdir -p /data/wifi/
#cp /etc/wpa_supplicant.conf /data/wifi/wpa_supplicant.conf
wificfg.sh
fi
if [ -f /data/wifi/wpa_supplicant.conf ];then
ifconfig wlan0 down
sleep 0.1
wpa_supplicant -iwlan0 -D nl80211 -c /data/wifi/wpa_supplicant.conf -B
sleep 1
udhcpc -T 2 -q -iwlan0 &
fi

dmesg -n 1

cd /vendor/qua/wifi/host;
./hciconfig hci0 up
./btgatt-server &
#./host_start.sh
#./btgatt-server
#sleep 2
#pkill -9 btgatt-server
#./btgatt-server
#cat /dev/ATC &
#echo -n "AT+WIFIMACPSCTRL=2,0,1" > /dev/ATC
# 计数器
count=0

# 每秒检查一次，60秒后执行命令
while [ $count -lt 60 ]; do
    sleep 1
    count=$((count+1))
done

# 发送AT命令
echo -n "AT+WIFIMACPSCTRL=2,0,1" > /dev/ATC
echo "AT+WIFIMACPSCTRL=2,0,1 is OK"





