#!/usr/bin/env bash

PUB_ROOTFS=$1
IMAGE_FILE=$2
if [ ! -d $PUB_ROOTFS ]; then
    echo "The rootfs: $PUB_ROOTFS doesn't exist!"
    exit -1
fi

if [ ! -d $PUB_ROOTFS/dev ]; then
    echo "The rootfs: $PUB_ROOTFS missing 'dev' folder !"
    exit -1
fi


SELF_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

if [ "$UID" == "0" ]; then
    echo "run in root or fake root mode ..."
    mknod -m 600 ${PUB_ROOTFS}/dev/console c 5 1
    mknod -m 600 ${PUB_ROOTFS}/dev/null c 1 3
else
    echo "run in sudo mode ..."
    sudo mknod -m 600 ${PUB_ROOTFS}/dev/console c 5 1
    sudo mknod -m 600 ${PUB_ROOTFS}/dev/null c 1 3
fi

lbase=$(basename ${PUB_ROOTFS})

pushd ${PUB_ROOTFS}/..
    [ -f $IMAGE_FILE ] && rm $IMAGE_FILE
    [ -f rootfs_temp.ubiimg ] && rm rootfs_temp.ubiimg
    mkfs.ubifs -d $lbase -m 2KiB -o rootfs_temp.ubiimg -e 126976 -c 776 –F -v
    ubinize -o $IMAGE_FILE -m 2KiB -p 128KiB  ${SELF_DIR}/ubi.cfg -v
popd


