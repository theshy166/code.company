#!/bin/bash

# 检查输入参数
if [ "$#" -ne 4 ] && [ "$#" -ne 3 ] && [ "$#" -ne 2 ]; then
    echo "Usage: $0 <folder_name> <chip_name> <platform> [clean]"
    echo "Examples:"
    echo "  $0 expat mc331x linux"
    echo "  $0 freetype qm108h linux"
    echo "  $0 curl qm108h rtt"
    echo "  $0 curl ubuntu"
    echo "  $0 freetype qm108h linux clean"
    exit 1
fi

# 获取参数
folder_name=$1
if [ "$#" -eq 3 ] || [ "$#" -eq 4 ]; then
    chip_name=$2
    platform=$3
else
    chip_name=""
    platform=$2
fi
rootdir=$(pwd)

# 根据平台和芯片型号设置编译工具链
case "$platform" in
    linux)
        case "$chip_name" in
            mc331x)
                TOOLCHIAN_PATH=${rootdir}/../../tools/toolchain/arm-molchip-linux/bin
                compiler_prefix="${TOOLCHIAN_PATH}/arm-mol-linux-uclibcgnueabihf-"
                ;;
            qm102v)
                TOOLCHIAN_PATH=${rootdir}/../../tools/toolchain/arm-fhv512-linux/bin
                compiler_prefix="${TOOLCHIAN_PATH}/arm-fhv512-linux-uclibcgnueabihf-"
                ;;
            qm108h)
                TOOLCHIAN_PATH=${rootdir}/../../tools/toolchain/arm-fhv512-linux/bin
                compiler_prefix="${TOOLCHIAN_PATH}/arm-fhv512-linux-uclibcgnueabihf-"
                ;;
            qm10xd)
                TOOLCHIAN_PATH=${rootdir}/../../tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin
                compiler_prefix="${TOOLCHIAN_PATH}/arm-molv2-linux-uclibcgnueabi-"
                ;;
            *)
                echo "Unsupported chip: $chip_name"
                exit 1
                ;;
        esac
        ;;
    rtt)
        case "$chip_name" in
            mc331x)
                TOOLCHIAN_PATH=${rootdir}/../../tools/toolchain/arm-linux-musleabi/bin
                compiler_prefix="${TOOLCHIAN_PATH}/arm-linux-musleabi-"
                ;;
            qm108h)
                TOOLCHIAN_PATH=${rootdir}/../../tools/toolchain/arm-fhv512-linux/bin
                compiler_prefix="${TOOLCHIAN_PATH}/arm-fhv512-linux-uclibcgnueabihf-"
                ;;
            *)
                echo "Unsupported chip: $chip_name"
                exit 1
                ;;
        esac
        ;;
    ubuntu)
        compiler_prefix=""
        ;;
    *)
        echo "Unsupported platform: $platform"
        exit 1
        ;;
esac

folder_path="${rootdir}/../../core/package/external/$folder_name"

last_index=$(($#))
last_arg=${!last_index}

# 确保文件夹存在
if [ ! -d "$folder_path" ]; then
    echo "Error: $folder_path does not exist"
    exit 1
fi

if [ -n "$chip_name" ]; then
    output_dir="$folder_path/out_${chip_name}_$platform"
else
    output_dir="$folder_path/out_$platform"
fi

if [ "$last_arg" == "clean" ]; then
    if [ -d "$output_dir" ]; then
        rm -rf $output_dir
    fi
    echo "$folder_path clean done"
    exit 1
fi

# 检查 CMakeLists.txt 文件是否存在
if [ -f "$folder_path/CMakeLists.txt" ]; then
    echo "Compiling in directory: $folder_path"
    mkdir -p "$output_dir"
    cd "$output_dir"
    if [ -n "$compiler_prefix" ]; then
        #compiler_prefix must contain full path due to CMAKE_AR bug
        cmake .. -DCMAKE_AR="${compiler_prefix}ar" -DCMAKE_C_COMPILER="${compiler_prefix}gcc" -DCMAKE_CXX_COMPILER="${compiler_prefix}g++"
    else
        cmake ..
    fi
    make -j8
else
    echo "No CMakeLists.txt found in directory"
    # 检查是否存在 Makefile 或 makefile
    if [ -f "$folder_path/Makefile" -o -f "$folder_path/makefile" ]; then
        echo "Found Makefile, compiling using make"
        cd $folder_path
        make -j8 DESTDIR="$output_dir" install
    else
        echo "No Makefile found, trying to generate one using configure"
        # 检查是否存在 configure 脚本
        if [ -f "$folder_path/configure" ]; then
            cd $folder_path
            ./configure \
            --host=arm-linux \
            --prefix=${output_dir} \
            CC=${compiler_prefix}gcc \
            CXX=${compiler_prefix}g++ \
            AR=${compiler_prefix}ar
            make -j8 DESTDIR="$output_dir" install
        else
            echo "No configure script found, cannot compile"
            exit 1
        fi
    fi
fi
