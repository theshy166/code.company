#!/bin/bash

# Global definition for SDK GNU build tools
export XOS_ROOT=$(pwd)
#. /tools/toolchain/arm-molv2-linux-uclibcgnueabi/
echo $XOS_ROOT

export LT00_ARM_GNUC_BIN_PATH=$XOS_ROOT/tools/toolchain/arm-molv2-linux-uclibcgnueabi
export LT00_ARC_GNUC_BIN_PATH=$XOS_ROOT/tools/toolchain/arc-fullhan-elf32

export PATH="$LT00_ARM_GNUC_BIN_PATH/bin:$LT00_ARC_GNUC_BIN_PATH/bin:$PATH"

export PATH="$XOS_ROOT/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin:$PATH"


export LD_LIBRARY_PATH="$LT00_ARM_GNUC_BIN_PATH/lib:$LT00_ARC_GNUC_BIN_PATH/lib:$LD_LIBRARY_PATH"


#BUILD_NUMBER
# format $BUILD_NUMBER
export BUILD_NUMBER=$(printf "BUILD_BUMBER ####### %04d" "${BUILD_NUMBER}")

