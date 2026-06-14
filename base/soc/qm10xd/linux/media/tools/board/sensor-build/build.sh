#!/bin/bash

source personal.make &> /dev/null
set -e

cd isp

mkdir -p $INSTALL_DIR/multiple

make clean
make lib RTT=0 MS=1 STATIC=1

make clean
make lib RTT=0 MS=1 STATIC=0

mv $INSTALL_DIR/*.a $INSTALL_DIR/multiple  &> /dev/null
mv $INSTALL_DIR/*.so $INSTALL_DIR/multiple  &> /dev/null

make clean
make lib RTT=0 MS=0 STATIC=1

make clean
make lib RTT=0 MS=0 STATIC=0
