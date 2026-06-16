#!/bin/bash

CUR_DIR=$(pwd)

echo "export DSP_ROOT=$CUR_DIR/../../..
export ISP_ROOT=$CUR_DIR/../../../mpp
export INSTALL_DIR=$CUR_DIR/_output
export ARCH=arc
export CHIP_ID=XC01
export CROSS_COMPILE=arc-fullhan-elf32-
export OS_NONE=1
export LINUX_PRE=1
export BUILD_MODE=6
export OS_NONE_CROSS_COMPILE=arc-fullhan-elf32-
" > personal.make

