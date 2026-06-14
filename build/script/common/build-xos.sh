#!/bin/bash
echo "XOS_DIR: $1"
XOS_DIR=$1
XOS_BUILD_MODULE=$2

# job number process

XOS_JOBS=1
# XOS_JOBS=$(echo "$@" | awk -F '-j' '{print $2}' | awk '{print $1}')
for arg in "$@"; do
    if [[ $arg =~ ^-j[0-9]+$ ]]; then
        XOS_JOBS="${arg#-j}"
        break
    fi
done
XOS_JOBS=$((XOS_JOBS))
[ $XOS_JOBS -le 0 ] && XOS_JOBS=1

echo "info:XOS_JOBS: $XOS_JOBS"

if [ -z "$XOS_DIR" ]; then
    echo "ERROR!! parameter is not correct !!! "
    exit 1
fi

if [ ! -f "$XOS_DIR/.config" ]; then
    echo "ERROR!! no $XOS_DIR/.config, please run 'make menuconfig/project_xxx_defconfig' first"
    exit 1
fi

# unset all env variables if already generated before
if [ -e "$XOS_DIR/.config.env" ]; then
    # file existed operation
    echo "The file $XOS_DIR/.config.env exists."
    source $XOS_DIR/build/script/unsetenv.sh > $XOS_DIR/.config.env.unset
fi

if [[ $XOS_BUILD_MODULE =~ ^sim(ulator)?$ ]]; then
    export XOS_BUILD_SIMULATOR=1
    TOOLCHAIN_NAME="simulator"
else
    # 从 .config 文件中提取 CONFIG_TOOLCHAIN_xxxxx=y 的 xxxxx 部分
    TOOLCHAIN_NAME=$(grep "^CONFIG_TOOLCHAIN_" "$XOS_DIR/.config" | grep "=y" | sed -e 's/CONFIG_TOOLCHAIN_//' -e 's/=y//')
    TOOLCHAIN_NAME=$(echo "$TOOLCHAIN_NAME" | tr '[:upper:]' '[:lower:]')
fi
export CONFIG_TOOLCHAIN_FILE=${TOOLCHAIN_NAME}
echo "${CONFIG_TOOLCHAIN_FILE}"

# 拼接成 toolchain_xxxxx.cmake
CMAKE_TOOLCHAIN="toolchain_${TOOLCHAIN_NAME}.cmake"

echo "${XOS_DIR}/build/cmake/toolchain/${CMAKE_TOOLCHAIN}"

# 检查提取出的 CMake toolchain 文件是否存在
if [ ! -f "${XOS_DIR}/build/cmake/toolchain/${CMAKE_TOOLCHAIN}" ]; then
    echo "ERROR!! no ${XOS_DIR}/build/cmake/toolchain/${CMAKE_TOOLCHAIN}, please configure CONFIG_TOOLCHAIN_xxxx=y correctly"
    exit 1
fi

# 从 .config 文件中提取 CONFIG_PRODUCT_xxxxx=y 的 xxxxx 部分
PRODUCT_NAME=$(grep "^CONFIG_PRODUCT_" "${XOS_DIR}/.config" | grep "=y" | sed -e 's/CONFIG_PRODUCT_//' -e 's/=y//')
PRODUCT_NAME=$(echo "${PRODUCT_NAME}" | tr '[:upper:]' '[:lower:]')
export CONFIG_XOS_PROJECT_TYPE=${PRODUCT_NAME}

# 拼接成 project_xxxxx_defconf
#CMAKE_PRODUCT_NAME="project_${PRODUCT_NAME}_defconfig"
CMAKE_DEFCONFIG_NAME=$(grep 'project_.*_defconfig' .config.file | sed 's/.*\(project_.*_defconfig\).*/\1/')
echo "_cmake2 ${XOS_DIR}/product/${PRODUCT_NAME}/${CMAKE_DEFCONFIG_NAME}"

# 检查提取出的 project_xxxx_defconf 文件是否存在
if [ ! -f "${XOS_DIR}/product/${PRODUCT_NAME}/${CMAKE_DEFCONFIG_NAME}" ]; then
    echo "ERROR!! no ${XOS_DIR}/product/${PRODUCT_NAME}/${CMAKE_DEFCONFIG_NAME}, please configure CONFIG_PRODUCT_xxxx=y correctly"
    exit 1
fi

# 切换到 XOS_DIR 目录
cd "$XOS_DIR"

# 如果没有创建 output 目录
mkdir -p output

#cd output
# 执行 cmake 命令
source $XOS_DIR/build/script/config2env.sh > $XOS_DIR/.config.env
cd output

cmake \
    -DXOS_JOBS=$XOS_JOBS \
    -DXOS_BUILD_SIMULATOR=$XOS_BUILD_SIMULATOR \
    -DCMAKE_TOOLCHAIN_FILE="$XOS_DIR/build/cmake/toolchain/$CMAKE_TOOLCHAIN" \
    -DBOARD_DIR=${XOS_DIR}/product/${PRODUCT_NAME} \
    -DCMAKE_DEFCONFIG=${CMAKE_DEFCONFIG_NAME} \
    -DCONF_FILE="${XOS_DIR}/.config" \
    "${XOS_DIR}" \
    -Wno-dev

make ${XOS_BUILD_MODULE} -j$XOS_JOBS || exit "$$?"

if [[ $XOS_BUILD_MODULE == clean ]]; then
    make baseclean -j$XOS_JOBS || exit "$$?"
fi

if [[ $XOS_BUILD_MODULE == distclean ]]; then
    make basedistclean -j$XOS_JOBS || exit "$$?"
fi
