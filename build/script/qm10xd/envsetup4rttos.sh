#!/bin/bash


export XOS_ROOT=$(pwd)

echo $XOS_ROOT

export LT00_ARM_UNIX_BIN_PATH=$XOS_ROOT/tools/toolchain/arm-molv2-linux-uclibcgnueabi
export LT00_ARM_GNUC_BIN_PATH=$XOS_ROOT/tools/toolchain/arm-fullhanv2-eabi-b3
export LT00_ARC_GNUC_BIN_PATH=$XOS_ROOT/tools/toolchain/arc-fullhan-elf32

export PATH="$LT00_ARM_UNIX_BIN_PATH/bin:$LT00_ARM_GNUC_BIN_PATH/bin:$LT00_ARC_GNUC_BIN_PATH/bin:$PATH"

export LD_LIBRARY_PATH="$LT00_ARM_GNUC_BIN_PATH/lib:$LT00_ARC_GNUC_BIN_PATH/lib:$LD_LIBRARY_PATH"

