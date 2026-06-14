#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <BUILD_SDK>"
    echo "build qm108h linux: $0 qm10xh_linux"
    echo "build qm102v linux: $0 qm10xv_linux"
    exit 1
fi

BUILD_SDK=$1
echo "BUILD_SDK is $BUILD_SDK"

rm output -rf
mkdir -p output
cd output
cmake -DBUILD_SDK=$BUILD_SDK -DCMAKE_TOOLCHAIN_FILE=../Platform/linux-arm.cmake ../
make clean
#make VERBOSE=1 || exit
make VERBOSE=1 || exit

#cp ../include/qua_camera.h ../../../libnative/qlibquacamera/include/
#if [ ${BUILD_SDK} = qm10xh_linux ];then
#    cp lib/libquacamera.so ../../../libnative/qlibquacamera/xc01
#else
#    cp lib/libquacamera.so ../../../libnative/qlibquacamera/xc00
#fi
