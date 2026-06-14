#!/bin/sh

CONFIG_FILE="/data/config/param.ini"

# 使用BusyBox支持的grep语法提取SSID（替代-P参数）
SSID=$(grep "^SSID[[:space:]]*=" "$CONFIG_FILE" | \
       sed -n 's/^SSID[[:space:]]*=[[:space:]]*//p' | \
       sed 's/^[[:space:]\"]*//;s/[[:space:]\"]*$//')

# 同方式提取PASSWD
PASSWD=$(grep "^PASSWD[[:space:]]*=" "$CONFIG_FILE" | \
         sed -n 's/^PASSWD[[:space:]]*=[[:space:]]*//p' | \
         sed 's/^[[:space:]\"]*//;s/[[:space:]\"]*$//')

echo "Extracted SSID: $SSID"
echo "Extracted PASSWD: $PASSWD"


mkdir -p /data/wifi
cfg_path="/data/wifi/"


rm ${cfg_path}wpa_supplicant.conf
echo "ctrl_interface=${cfg_path}wpa_supplicant" >> ${cfg_path}wpa_supplicant.conf
echo " " >>  ${cfg_path}wpa_supplicant.conf

echo "#wpa/wpa2/wpa3 auto" >>  ${cfg_path}wpa_supplicant.conf
echo "network={" >>  ${cfg_path}wpa_supplicant.conf

if [  -n  "$SSID" ]; then
  echo "     ssid=\"$SSID\"" >> ${cfg_path}wpa_supplicant.conf
else
  echo "     ssid=\"\"" >> ${cfg_path}wpa_supplicant.conf
fi

if [  -n  "$PASSWD" ]; then
  echo "     psk=\"$PASSWD\"" >> ${cfg_path}wpa_supplicant.conf
else
   echo "     psk=\"\"" >> ${cfg_path}wpa_supplicant.conf
fi

echo "}" >> ${cfg_path}wpa_supplicant.conf
echo " " >> ${cfg_path}wpa_supplicant.conf

