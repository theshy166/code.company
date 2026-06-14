#!/bin/sh
function apath()
{
  REALPATH=$(realpath $1)
  echo $REALPATH
  PATH=$REALPATH:$PATH
}
unset platform
# Determine the current interpreter
interpreter=$(ps -p $$ -o comm=)

# Get the script path based on the interpreter type
if echo "$interpreter" | grep -q "bash"; then
    # If it's bash interpreter, use BASH_SOURCE
    script_path=$(realpath "${BASH_SOURCE[0]}")
    script_dir=$(dirname "$script_path")
elif echo "$interpreter" | grep -q "sh"; then
    # If it's sh interpreter, use $0
    script_path=$(realpath "$0")
    script_dir=$(dirname "$script_path")
else
    echo "usage: \". $0\", or \"source $0\""
    exit 1
fi
lc_toolchain=$script_dir/../../../tools/toolchain
platforms=("qm10xd" "qm10xv" "mc331x" "qm10xh" "qm10xhv2" "qm10xv_rtt" "qm10xd_rtt")
# 将 platforms 数组元素转换为小写
platforms_lower=()
for plat in "${platforms[@]}"; do
    platforms_lower+=("$(echo "$plat" | tr '[:upper:]' '[:lower:]')")
done

# 首先检查 $1 参数，转换为小写后进行匹配
if [ -n "$1" ]; then
    input=$(echo "$1" | tr '[:upper:]' '[:lower:]')
    for i in "${!platforms_lower[@]}"; do
        if [ "$input" = "${platforms_lower[$i]}" ]; then
            platform="${platforms[$i]}"
            break
        fi
    done
fi

# 若 $1 没有匹配，再检查 ENV_DEFAULT_PLATFORM 变量，转换为小写后进行匹配
if [ -z "$platform" ] && [ -n "$ENV_DEFAULT_PLATFORM" ]; then
    input=$(echo "$ENV_DEFAULT_PLATFORM" | tr '[:upper:]' '[:lower:]')
    for i in "${!platforms_lower[@]}"; do
        if [ "$input" = "${platforms_lower[$i]}" ]; then
            platform="${platforms[$i]}"
            break
        fi
    done
fi

# 若前面都没有匹配到，让用户输入
if [ -z "$platform" ]; then
    # 输出平台列表及对应的索引编号
    echo "support list："
    for i in "${!platforms[@]}"; do
        echo "$i: ${platforms[$i]}"
    done

    read -p "please choose index：" index
    if [[ "$index" =~ ^[0-4]$ ]]; then
        platform=${platforms[$index]}
        echo choose result is $platform
    else
        echo "input invalide..."
        return
    fi
fi

if [ "qm10xd" = "$platform" ]; then
    apath $lc_toolchain/arm-molv2-linux-uclibcgnueabi/bin
    CROSS_COMPILE=arm-molv2-linux-uclibcgnueabi-
    alias qmarmstrip=arm-molv2-linux-uclibcgnueabi-strip
elif [ "qm10xv" = "$platform" ]; then
    apath $lc_toolchain/arm-fhv512-linux/bin
    CROSS_COMPILE=arm-fhv512-linux-uclibcgnueabihf-
    alias qmarmstrip=arm-fhv512-linux-uclibcgnueabihf-strip
elif [ "mc331x" = "$platform" ]; then
    apath $lc_toolchain/arm-molchip-linux/bin
    CROSS_COMPILE=arm-mol-linux-uclibcgnueabihf-
    alias qmarmstrip=arm-mol-linux-uclibcgnueabihf-strip
elif [ "qm10xh" = "$platform" ]; then
    apath $lc_toolchain/arm-fhv512-linux/bin
    CROSS_COMPILE=arm-fhv512-linux-uclibcgnueabihf-
    alias qmarmstrip=arm-fhv512-linux-uclibcgnueabihf-strip
elif [ "qm10xhv2" = "$platform" ]; then
    apath $lc_toolchain/arm-fhv512-linux/bin
    CROSS_COMPILE=arm-fhva12c-linux-uclibcgnueabihf-
    alias qmarmstrip=arm-fhva12c-linux-uclibcgnueabihf-strip
elif [ "qm10xv_rtt" = "$platform" ] || [ "qm10xd_rtt" = "$platform" ]; then
    apath $lc_toolchain/arm-fullhanv2-eabi-b3/bin
    CROSS_COMPILE=arm-fullhanv2-eabi-
    alias qmarmstrip=arm-fullhanv2-eabi-strip
else
    apath $lc_toolchain/arm-fhv512-linux/bin
    CROSS_COMPILE=arm-fhv512-linux-uclibcgnueabihf-
    alias qmarmstrip=arm-fhv512-linux-uclibcgnueabihf-strip
fi
export CROSS_COMPILE
export CC=${CROSS_COMPILE}gcc
export STRIP=${CROSS_COMPILE}strip
export LD=${CROSS_COMPILE}ld
export host_prefix=$(echo $CROSS_COMPILE|sed 's/-$//')
echo export CROSS_COMPILE=$CROSS_COMPILE
echo export CC=${CROSS_COMPILE}gcc
echo export STRIP=${CROSS_COMPILE}strip
echo export LD=${CROSS_COMPILE}ld
echo export host_prefix="$host_prefix"

# Check if the environment variables are set successfully
if command -v "${CROSS_COMPILE}gcc" > /dev/null 2>&1; then
    echo -e "\nThe environment variables are set successfully.\n"
    echo "The result of executing ${CROSS_COMPILE}gcc -v is as follows:"
    echo
    "${CROSS_COMPILE}gcc" -v
    echo
else
    echo "Failed to set the environment variables. The command ${CROSS_COMPILE}gcc cannot be found."
fi

