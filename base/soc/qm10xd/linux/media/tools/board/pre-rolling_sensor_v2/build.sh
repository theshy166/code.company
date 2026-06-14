#!/bin/bash

source personal.make &> /dev/null
set -e

mkdir -p $INSTALL_DIR/repository

make clean
make lib 

# make lib RTT=0 MS=1 STATIC=1
# make clean
# make lib RTT=0 MS=1 STATIC=0

mv $INSTALL_DIR/*.a $INSTALL_DIR/repository  &> /dev/null
#mv $INSTALL_DIR/*.so $INSTALL_DIR/repository  &> /dev/null

# make clean
# make lib RTT=0 MS=0 STATIC=1

# make clean
# make lib RTT=0 MS=0 STATIC=0
