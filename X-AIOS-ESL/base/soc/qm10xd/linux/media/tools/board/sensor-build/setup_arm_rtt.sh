#!/bin/bash

CUR_DIR=$(pwd)

echo "export DSP_ROOT=$CUR_DIR/../../..
export ISP_ROOT=$CUR_DIR/../../../mpp
export RTT_ROOT=$CUR_DIR/../../../../bsp/rtt_ap
export INSTALL_DIR=$CUR_DIR/_output
export RTT_INSTALL_DIR=$CUR_DIR/_output
export ARCH=arm
export CHIP_ID=XC01
export CROSS_COMPILE=arm-fullhanv2-eabi-
export RTT=1
export BUILD_MODE=1
export RTT_CROSS_COMPILE=arm-fullhanv2-eabi-
export A7=1
export RTTV3=1
export VFP=1
" > personal.make

