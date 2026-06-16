#!/usr/bin/env bash

PUB_ROOTFS=$1
KERNEL_DIR=$2
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


OUT_FILE=
if [[ $KERNEL_DIR == *"linux-5"* ]]; then
    OUT_FILE=initramfs-v510.cpio.gz
fi

pushd $PUB_ROOTFS/..
    $KERNEL_DIR/scripts/gen_initramfs_list.sh  $PUB_ROOTFS/ > initramfs.list
    $KERNEL_DIR/usr/gen_init_cpio initramfs.list > initramfs.cpio
    gzip -f initramfs.cpio
    mv initramfs.cpio.gz $KERNEL_DIR/../../rootfs/${OUT_FILE}
popd
