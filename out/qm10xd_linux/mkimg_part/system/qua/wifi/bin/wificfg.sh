#!/bin/sh

CONFIG_FILE="/data/config/param.ini"

# 兼容大小写的参数提取（支持SSID/PASSWD或ssid/passwd）
get_config_value() {
    local key="$1"
    grep -i "^[[:space:]]*${key}[[:space:]]*=" "$CONFIG_FILE" | \
    sed -n "s/^[[:space:]]*${key}[[:space:]]*=[[:space:]]*//pi" | \
    sed 's/^[[:space:]\"]*//;s/[[:space:]\"]*$//'
}

SSID=$(get_config_value "SSID")
PASSWD=$(get_config_value "PASSWD")

echo "Extracted SSID: $SSID"
echo "Extracted PASSWD: $PASSWD"

# 生成wpa_supplicant配置
mkdir -p /data/wifi
cfg_path="/data/wifi/"

cat > "${cfg_path}wpa_supplicant.conf" <<EOF
ctrl_interface=${cfg_path}wpa_supplicant

#wpa/wpa2/wpa3 auto
network={
    ssid="$SSID"
    psk="$PASSWD"
}
EOF
