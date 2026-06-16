#!/bin/sh

echo $#
cd /bin
cfg_path="/data/wifi/"
mkdir -p ${cfg_path}
if [ $# -ne 2 ]; then
    echo "Usage: $0 arg1 arg2"
    #exit 1
else
echo "input param!!"
   # echo $2
    if [  -f  "${cfg_path}wpa_supplicant.conf" ];then
        rm "${cfg_path}wpa_supplicant.conf"
        echo "wpa_supplicant.conf delsted!!"
    else
        echo "wpa_supplicant.conf not exist"
    fi

    echo "ctrl_interface=/var/wpa_supplicant" >> ${cfg_path}wpa_supplicant.conf
echo " " >>  ${cfg_path}wpa_supplicant.conf

echo "#wpa/wpa2/wpa3 auto" >>  ${cfg_path}wpa_supplicant.conf
echo "network={" >>  ${cfg_path}wpa_supplicant.conf

fi



if [  -n  "$1" ]; then
  echo "     ssid=\"$1\"" >> ${cfg_path}wpa_supplicant.conf
else
  echo "$1 is NULL"
fi

if [  -n  "$2" ]; then
  echo "     psk=\"$2\"" >> ${cfg_path}wpa_supplicant.conf
else
  echo "$2 is NULL"
fi


if [ $# -ne 2 ]; then
    echo "Usage: $0 less than 2"
    #exit 1
else
echo "}" >> ${cfg_path}wpa_supplicant.conf
echo " " >> ${cfg_path}wpa_supplicant.conf

fi

pkill wpa_supplicant;
pkill  udhcpc;
sleep 1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib;
wpa_supplicant -iwlan0 -D nl80211 -c ..${cfg_path}wpa_supplicant.conf -B &
sleep 1
counter=0
while true; do
    interface_status=$(ip addr show wlan0 | grep LOWER_UP)
    if [ -n "$interface_status" ] || [ $counter -gt 60 ]; then
        udhcpc -q -iwlan0 &
        break
    fi
    counter=$((counter + 1))
    sleep 0.5
done
exit 0
