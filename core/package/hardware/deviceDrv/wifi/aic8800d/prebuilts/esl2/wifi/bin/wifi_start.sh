#!/bin/sh
pkill wpa_supplicant
pkill udhcpc
sleep 0.5
echo $#
mkdir -p /data/wifi
cfg_path="/data/wifi/"
if [ $# -ne 2 ]; then
    echo "Usage: $0 arg1 arg2"
    #exit 1
else
echo "input param!!"
   # echo $1
   # echo $2
    if [  -f  "${cfg_path}wpa_supplicant.conf" ];then
        rm "${cfg_path}wpa_supplicant.conf"
        echo "wpa_supplicant.conf delsted!!"
    else
        echo "wpa_supplicant.conf not exist"
    fi

    echo "ctrl_interface=/data/wpa_supplicant" >> ${cfg_path}wpa_supplicant.conf
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
  echo "     key_mgmt=WPA-PSK" >> ${cfg_path}wpa_supplicant.conf
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

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib
ifconfig wlan0 down
/vendor/qua/wifi/bin/./wpa_supplicant -iwlan0 -D nl80211 -c ${cfg_path}wpa_supplicant.conf -B
udhcpc -iwlan0 &

exit 1
