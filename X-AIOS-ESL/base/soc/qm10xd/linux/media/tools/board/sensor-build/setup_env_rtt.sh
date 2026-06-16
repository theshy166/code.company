#!/bin/bash

CUR_DIR=$(pwd)

echo "export DSP_ROOT=$CUR_DIR/../../..
export ISP_ROOT=$CUR_DIR/../../../mpp
export RTT_ROOT=$CUR_DIR/../../../../bsp/rt-thread
export INSTALL_DIR=$CUR_DIR/_output
export RTT_INSTALL_DIR=$CUR_DIR/_output
export ARCH=arc
export CHIP_ID=XC01
export CROSS_COMPILE=arc-fullhan-elf32-
export RTT=1
export BUILD_MODE=1
export RTT_CROSS_COMPILE=arc-fullhan-elf32-
export RPC_RTT=1
" > personal.make

