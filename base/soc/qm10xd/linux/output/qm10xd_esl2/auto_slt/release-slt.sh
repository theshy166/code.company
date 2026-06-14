#!/bin/sh
PWD=`pwd`
SDK_ROOT=${PWD}/../../../..
echo "${SDK_ROOT}"
export OUTPUT_DIR=${SDK_ROOT}/output/lt00/bsp

CHIP=lt00
#BOARD=`pwd | awk -F "/" '{print $NF}'`
GLOBAL_CONF=${SDK_ROOT}/output/.config
#include ${GLOBAL_CONF}
ROOTFS_DIR=${OUTPUT_DIR}/rootfs
UBOOT_DIR=${SDK_ROOT}/bsp/uboot/uboot
RAMBOOT_DIR=${SDK_ROOT}/bsp/ramboot
KERNEL_DIR=${SDK_ROOT}/bsp/kernel/linux-5.10.y
PLATFORM=demoboard

TARGET_MAKEFILE=${RAMBOOT_DIR}/board/molchip/dram/Makefile
INSERT_LINE=7
CONTENT_FILE=./version.mk

#ramboot
pushd ${RAMBOOT_DIR}
git stash
popd

#boot
pushd ${UBOOT_DIR}
git stash
popd

#kernel
pushd ${KERNEL_DIR}
git stash
popd


#sed -i 's#host->host_caps = MMC_MODE_8BIT | MMC_MODE_EMMC_HS200 | MMC_MODE_EMMC_HS400;#host->host_caps = MMC_MODE_8BIT | MMC_MODE_EMMC_HS200;// | MMC_MODE_EMMC_HS400;#g' ${UBOOT_DIR}/drivers/mmc/mc_sdhci.c
#sed -i 's/#define MMC_HS200_MAX_DTR	200000000/#define MMC_HS200_MAX_DTR	40000000/g' ${KERNEL_DIR}/include/linux/mmc/card.h
#sed -i 's/#define MY_STR_MMC_BOOTIMAGE_ARG_(AK,AD,CK,CD) /#define MY_STR_MMC_BOOTIMAGE_ARG_(AK,AD,CK,CD) \\\n\t"hash;" /g' ${UBOOT_DIR}/include/configs/lt00.h
#sed -i 's#disable_default_clk();#//disable_default_clk();#g' ${RAMBOOT_DIR}/board/molchip/spl/spl.c
sed -i 's/"sf read 0x40008000 0x100000 0x3c0000;" /"sf read 0x40008000 0x100000 0x5A0000;"/g' ${UBOOT_DIR}/include/configs/lt00.h
sed -i '/can: can@26500000/,/status/ s/status = "disabled"/status = "okay"/' ${KERNEL_DIR}/arch/arm/boot/dts/molchip-kernel-lt00.dtsi
sed -i '/MOLCHIP_FASTBOOT/d' ${RAMBOOT_DIR}/configs/molchip_lt00_defconfig
echo "Inserting content from ${CONTENT_FILE} into ${TARGET_MAKEFILE} at line ${INSERT_LINE}"
sed -i "${INSERT_LINE}r ${CONTENT_FILE}" ${TARGET_MAKEFILE}

cp ${PWD}/molchip_lt00_defconfig ${UBOOT_DIR}/configs/molchip_lt00_defconfig
cp ${PWD}/dram.c ${RAMBOOT_DIR}/board/molchip/dram/dram.c
cp ${PWD}/reset_mc.c ${RAMBOOT_DIR}/board/molchip/lib/reset_mc.c

pushd ${SDK_ROOT}
./envsetup.sh 1 evb  #Target board:1200-esmt
popd

pushd ${SDK_ROOT}
make boot
popd


pushd ${SDK_ROOT}
make kernel
make rootfs
popd

#slt_bsp_file
#cp -rf ./SLT_BSP_FILE/ ${ROOTFS_DIR}/SLT_BSP_FILE/
cp ./SLT_BSP_FILE/S06BSP ${ROOTFS_DIR}/etc/init.d/S06BSP
#cp ../../test-suite/iperf_arm_64/iperf3 ${ROOTFS_DIR}/usr/bin/
#cp ../../test-suite/iperf_arm_64/libiperf.so.0 ${ROOTFS_DIR}/usr/bin/

#slt_MM_file
cp ./MM/S05devs ${ROOTFS_DIR}/etc/init.d/
cp ./MM/clean_sync.sh ${ROOTFS_DIR}/etc/init.d/
cp -rf ./MM/ko/* ${ROOTFS_DIR}/ko/
#cp -rf ./MM/test/* ${ROOTFS_DIR}/test/
cp -rf ./MM/lib/* ${ROOTFS_DIR}/lib/

cd ${SDK_ROOT}
make ramdisk
make kernel_image

#Restore compilation environment
#rm -rf ${ROOTFS_DIR}/SLT_BSP_FILE
rm -rf ${ROOTFS_DIR}/etc/init.d/S06BSP

rm -rf ${ROOTFS_DIR}/etc/init.d/S05devs
rm -rf ${ROOTFS_DIR}/etc/init.d/clean_sync.sh
rm -rf ${ROOTFS_DIR}/ko/*
#rm -rf ${ROOTFS_DIR}/test/*

make ramdisk

#ramboot
pushd ${RAMBOOT_DIR}
git checkout .
git stash pop
popd

#boot
pushd ${UBOOT_DIR}
git checkout .
git stash pop
popd

#kernel
pushd ${KERNEL_DIR}
git checkout .
git stash pop
popd
echo "release finished"
