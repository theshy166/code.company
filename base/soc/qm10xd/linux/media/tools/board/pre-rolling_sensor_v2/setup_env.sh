#!/bin/bash

CUR_DIR=$(pwd)
TOP_DIR=$(readlink -f ${CUR_DIR}/../../../../)
#get the cross toolchain
CROSS=$(make -f ${TOP_DIR}/output/.config -p 2>/dev/null | sed '/^CROSS\s*=\s*/!d' | sed 's/^CROSS\s*=\s*//')

echo "export DSP_ROOT=$CUR_DIR/../../..
export ISP_ROOT=$CUR_DIR/../../../mpp
export INSTALL_DIR=$CUR_DIR/_output
export ARCH=arm
export CHIP_ID=XC01
export LINUX_PRE=1
export CROSS_COMPILE=$CROSS
export ARM_CORTEXA7_VFP=1
" > personal.make

