#!/bin/sh

service_addr="192.168.0.254"

# ... 保持原有变量定义不变 ...
MISC_DEVICE="/dev/mtd5"
TEMP_FILE="/tmp/misc_bcb.bin"
BCB_SIZE=2048

boot2Recovery() {
    # 读取misc分区
    if ! nanddump --bb=skipbad -l $BCB_SIZE -s 0 $MISC_DEVICE | head -c $BCB_SIZE > $TEMP_FILE; then
        echo "Failed to read misc partition" >&2
        return 1
    fi

    # 写入恢复命令（前32字节为命令区）
    if ! (
        # 先清空命令区
        dd if=/dev/zero of=$TEMP_FILE bs=1 count=32 conv=notrunc >/dev/null 2>&1
        # 写入boot-recovery命令（Android BCB规范）
        printf "boot-recovery\0" | dd of=$TEMP_FILE bs=1 count=32 conv=notrunc >/dev/null 2>&1
    ); then
        echo "Failed to write recovery command" >&2
        rm -f $TEMP_FILE
        return 2
    fi

    # 擦除NAND块（带坏块处理）
    if ! flash_erase $MISC_DEVICE 0 0; then
        echo "Failed to erase misc partition" >&2
        # rm -f $TEMP_FILE
        # return 3
    fi
    # 写回misc分区
    if ! nandwrite -p $MISC_DEVICE $TEMP_FILE; then
        echo "Failed to write misc partition" >&2
        rm -f $TEMP_FILE
        return 3
    fi

    rm -f $TEMP_FILE
    echo "Boot to Recovery configuration success"
    return 0
}

# 参数检查
if [ $# -eq 0 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi

current_version="$1"  # 从第一个参数获取版本号

echo "Waiting for IP address..."
loop=1

# 修改1：增加wlan0接口状态检查
while [ $loop -le 10 ]; do
    sleep 1
    # 精确匹配wlan0接口且已启用（UP状态）
    if ip a show wlan0 | grep "inet" | grep -qv " lo " && ip link show wlan0 | grep -q "UP"; then
        # 增加IP有效性验证
        wifi_Bcast=$(ip -4 addr show wlan0 | grep inet | awk 'NR==1{print $2}' | cut -d '/' -f 1 | cut -d '.' -f 1-3)
        # 验证前三段IP格式
        if [ $(echo "$wifi_Bcast" | grep -o '\.' | wc -l) -eq 2 ]; then
            service_addr="${wifi_Bcast}.254"
            echo "Valid service_addr: $service_addr"
            break
        else
            echo "Invalid IP segment: $wifi_Bcast"
        fi
    fi
    loop=$((loop + 1))
    echo "IP check loop: $loop"
done

# 新增IP地址验证
if [ -z "$wifi_Bcast" ]; then
    echo "Error: Failed to get valid wlan0 IP address after 10 attempts"
    # exit 2
fi

echo "Start ping test..."
loop=1
# 修改2：增加ping超时重试逻辑
while [ $loop -le 10 ]; do
    sleep 1
    # 增加地址格式验证
    if [[ $service_addr =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        if ping -c 1 -W 2 "$service_addr" >/dev/null 2>&1; then
            echo "Ping success"
            break
        fi
    else
        echo "Invalid IP format: $service_addr"
        exit 3
    fi
    loop=$((loop + 1))
    echo "Ping loop: $loop"
done

echo "Ping loop count: $loop"
sleep 1
echo "Start download process..."
rm -f /data/*.lzma
rm -f /data/script-ota.ini

if [ $loop -le 10 ]; then
    echo "Ping $service_addr success. Starting check_update..."

    MAX_RETRY=3
    ATTEMPT=1
    final_ret=1  # 记录最终状态

    # 修改3：完善重试逻辑
    while [ $ATTEMPT -le $MAX_RETRY ]; do
        echo "check_update attempt $ATTEMPT/$MAX_RETRY"
        # /data/update.tar.gz
        downloader_esl "$current_version" --check_update --addr $service_addr
        RET=$?
        case $RET in
            0)  # 
                echo "has_new = true"
                final_ret=$RET
                break
                ;;
            1)  #
                echo "has_new = false"
                sleep 3
                ;;
            *)  # 
                echo "(code: $RET)."
                final_ret=$RET
                sleep 3
                ;;
        esac
        ATTEMPT=$((ATTEMPT + 1))
    done
    if [ $final_ret -eq 0 ]; then
        # delete update.ini, then download update.tar.gz from QM server
        rm -f /data/update.ini
        sync
        echo "check_update_only pass, then download update.tar.gz from QM server"
    else
	    echo "check_update_only fail, then download update.tar.gz from Customer's server"
        # TODO
        # delete exit 1 after Customer's server is ready
        exit 1
    fi
else
    echo "connect to $service_addr failed,then download update.tar.gz from Customer's server"
    # TODO
    # delete exit 1 after Customer's server is ready
    exit 1
fi
boot2Recovery
#sleep 3
#reboot
exit 0
