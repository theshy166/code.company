#!/bin/bash

echo "PRJ_TOP_DIR: $PRJ_TOP_DIR"
echo "MKIMG_PATH_ROOT: $MKIMG_PATH_ROOT"
echo "QM_ALL_IMAGES: $QM_ALL_IMAGES"
DES_OUT_ROOT_DIR=$MKIMG_PATH_ROOT
DES_OUT_FINAL_DIR=$QM_ALL_IMAGES
PROGRAMMER_DIR=$QM_ALL_IMAGES/programmer
MC_IMAGE_BUILDER=$PRJ_TOP_DIR/base/soc/qm10xd/linux/bsp/uboot/uboot/.build/tools/mc-image-builder

SRC_OUT_BSP_DIR=${QM_ALL_IMAGES}/../image/bsp

_soc_pkg_path=base/soc/qm10xd/linux
PREBUILT_OUT_BSP_DIR=${_soc_pkg_path}/prebuilts/esl/bsp

echo "mv images to qmimages start"
echo "MKIMG_PATH_IMGOUT:$MKIMG_PATH_IMGOUT"
echo "CONFIG_STORAGE_TYPE_SPINAND:$CONFIG_STORAGE_TYPE_SPINAND"
echo "MKIMG_TGT_OUT:$MKIMG_TGT_OUT"
echo ""


if [[ "$CONFIG_STORAGE_TYPE_EMMC" = true ]]; then
        echo "CONFIG_STORAGE_TYPE_EMMC=$CONFIG_STORAGE_TYPE_EMMC"
        mv $MKIMG_PATH_IMGOUT/*.img $QM_ALL_IMAGES
        mv $MKIMG_PATH_IMGOUT/*.txt $QM_ALL_IMAGES
elif [[ "$CONFIG_STORAGE_TYPE_SPINAND" = true ]]; then
        echo "CONFIG_STORAGE_TYPE_SPINAND=$CONFIG_STORAGE_TYPE_SPINAND"
        mv $MKIMG_PATH_IMGOUT/*128KB*.ubi $QM_ALL_IMAGES
	cp $SRC_OUT_BSP_DIR/spl/u-boot-spl-header.img	$QM_ALL_IMAGES
	cp $SRC_OUT_BSP_DIR/boot/u-boot.bin $QM_ALL_IMAGES
	cp $PRJ_TOP_DIR/product/$CONFIG_XOS_PROJECT_TYPE/logo.img $QM_ALL_IMAGES
        cp -r $MKIMG_TGT_OUT/image/*  $QM_ALL_IMAGES
        echo "product $CONFIG_XOS_PROJECT_TYPE board $CONFIG_XOS_BUILD_BOARD"
        cp $PRJ_TOP_DIR/tools/partition/$CONFIG_XOS_BUILD_BOARD/los_spi-nand/64M/$CONFIG_XOS_PROJECT_TYPE/script.ini $QM_ALL_IMAGES
	cp $PRJ_TOP_DIR/tools/partition/$CONFIG_XOS_BUILD_BOARD/los_spi-nand/64M/$CONFIG_XOS_PROJECT_TYPE/script-ota.ini $QM_ALL_IMAGES
	cp $PREBUILT_OUT_BSP_DIR/recovery/recovery.img $QM_ALL_IMAGES
	cp $PREBUILT_OUT_BSP_DIR/recovery/recovery-rootfs_2KB_128KB_*.ubi $QM_ALL_IMAGES
        #replace with the actual file size in the script.ini file
	splfilesize=$(stat -c%s $QM_ALL_IMAGES/u-boot-spl-header.img)
        splhex_size=$(printf "%x" "$splfilesize")
        echo "spl file hex size:$splhex_size"

	ubootfilesize=$(stat -c%s $QM_ALL_IMAGES/u-boot.bin)
        uboothex_size=$(printf "%x" "$ubootfilesize")
        echo "uboot file hex size:$uboothex_size"

	logofilesize=$(stat -c%s $QM_ALL_IMAGES/logo.img)
        logohex_size=$(printf "%x" "$logofilesize")
        echo "logo file hex size:$logohex_size"

	kernelfilesize=$(stat -c%s $QM_ALL_IMAGES/zImage-dtb)
        kernelhex_size=$(printf "%x" "$kernelfilesize")
        echo "kernel file hex size:$kernelhex_size"

        systemfilesize=$(stat -c%s $QM_ALL_IMAGES/system_2KB_128KB_*MB.ubi)
        systemhex_size=$(printf "%x" "$systemfilesize")
        echo "system file hex size:$systemhex_size"

        rootfsfilesize=$(stat -c%s $QM_ALL_IMAGES/rootfs_2KB_128KB_*MB.ubi)
        rootfshex_size=$(printf "%x" "$rootfsfilesize")
        echo "rootfs file hex size:$rootfshex_size"

        datafilesize=$(stat -c%s $QM_ALL_IMAGES/data_2KB_128KB_*MB.ubi)
        datahex_size=$(printf "%x" "$datafilesize")
        echo "data file hex size:$datahex_size"

	recoveryfilesize=$(stat -c%s $QM_ALL_IMAGES/recovery.img)
        recoveryhex_size=$(printf "%x" "$recoveryfilesize")
        echo "recovery file hex size:$recoveryhex_size"

	recoveryfsfilesize=$(stat -c%s $QM_ALL_IMAGES/recovery-rootfs_2KB_128KB_*MB.ubi)
        recoveryfshex_size=$(printf "%x" "$recoveryfsfilesize")
        echo "recovery-rootfs file hex size:$recoveryfshex_size"

        sed -i "s/splfilesize/$splhex_size/g; s/ubootfilesize/$uboothex_size/g; s/logofilesize/$logohex_size/g; s/kernelfilesize/$kernelhex_size/g; s/systemfilesize/$systemhex_size/g; s/rootfsfilesize/$rootfshex_size/g; s/datafilesize/$datahex_size/g; s/recoveryfilesize/$recoveryhex_size/g;s/recoveryfsfilesize/$recoveryfshex_size/g" $QM_ALL_IMAGES/script.ini
	sed -i "s/splfilesize/$splhex_size/g; s/ubootfilesize/$uboothex_size/g; s/logofilesize/$logohex_size/g; s/kernelfilesize/$kernelhex_size/g; s/systemfilesize/$systemhex_size/g; s/rootfsfilesize/$rootfshex_size/g; s/datafilesize/$datahex_size/g; s/recoveryfsfilesize/$recoveryfshex_size/g" $QM_ALL_IMAGES/script-ota.ini
        echo "start create programmer version"
        mkdir -p $PROGRAMMER_DIR
	cp $PRJ_TOP_DIR/tools/partition/$CONFIG_XOS_BUILD_BOARD/los_spi-nand/64M/$CONFIG_XOS_PROJECT_TYPE/programmer.ini $PROGRAMMER_DIR
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  -s -f $QM_ALL_IMAGES/u-boot-spl-header.img $QM_ALL_IMAGES/u-boot-spl-header_out.img
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/u-boot-spl-header_out.img $PROGRAMMER_DIR/u-boot-spl-header.img.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/u-boot.bin $PROGRAMMER_DIR/u-boot.bin.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/logo.img $PROGRAMMER_DIR/logo.img.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/misc.img $PROGRAMMER_DIR/misc.img.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/recovery.img $PROGRAMMER_DIR/recovery.img.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/recovery-rootfs_2KB_128KB_10MB.ubi $PROGRAMMER_DIR/recovery-rootfs_2KB_128KB_10MB.ubi.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/zImage-dtb $PROGRAMMER_DIR/zImage-dtb.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/system_2KB_128KB_24MB.ubi $PROGRAMMER_DIR/system_2KB_128KB_24MB.ubi.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/rootfs_2KB_128KB_6MB.ubi $PROGRAMMER_DIR/rootfs_2KB_128KB_6MB.ubi.burn
        $MC_IMAGE_BUILDER -c  8/1024   -p 2048  -o 64  -e 0x20000  -n 64  $QM_ALL_IMAGES/data_2KB_128KB_77MB.ubi $PROGRAMMER_DIR/data_2KB_128KB_77MB.ubi.burn
        echo "end programmer version"
elif [[ "$CONFIG_STORAGE_TYPE_SPINOR" = true ]]; then
        echo "CONFIG_STORAGE_TYPE_SPINOR=$CONFIG_STORAGE_TYPE_SPINOR"
        cp -r $MKIMG_TGT_OUT/image/*  $QM_ALL_IMAGES
        echo "product $CONFIG_XOS_PROJECT_TYPE"
        cp $PRJ_TOP_DIR/tools/partition/qm10xv/los_spi-nor/16M/$CONFIG_XOS_PROJECT_TYPE/script.ini $QM_ALL_IMAGES
fi


echo "QM: copy boot QM_VERSION:$QM_VERSION-$BUILD_NUMBER"

###copy recovery files
echo "::copy recovery files to $QM_ALL_IMAGES"


###copy logo files
echo "::copy logo files to $QM_ALL_IMAGES"

###copy res files
echo "::copy resource files to $QM_ALL_IMAGES"

### copy gpt and create rootfs.* files
echo "QM  $PRJ_TOP_DIR"

### pack ota update.img
echo "start to pack ota update package"

if [ -d "$QM_ALL_IMAGES/update" ]; then
	rm -rf $QM_ALL_IMAGES/update
fi

mkdir -p $QM_ALL_IMAGES/update
cp $QM_ALL_IMAGES/rootfs_2KB_128KB_6MB.ubi $QM_ALL_IMAGES/update/
cp $QM_ALL_IMAGES/system_2KB_128KB_24MB.ubi $QM_ALL_IMAGES/update/
cp $QM_ALL_IMAGES/u-boot.bin $QM_ALL_IMAGES/update/
#cp $QM_ALL_IMAGES/u-boot-spl-header.img $QM_ALL_IMAGES/update/
cp $QM_ALL_IMAGES/zImage-dtb $QM_ALL_IMAGES/update/
cp $QM_ALL_IMAGES/logo.img $QM_ALL_IMAGES/update/
#cp $QM_ALL_IMAGES/recovery-rootfs_2KB_128KB_10MB.ubi $QM_ALL_IMAGES/update/
mv $QM_ALL_IMAGES/script-ota.ini $QM_ALL_IMAGES/update/
cd $QM_ALL_IMAGES/update/
lzma *.ubi zImage-dtb u-boot.bin logo.img
# u-boot-spl-header.img
tar -czvf update-$CONFIG_QM_VERSION.tar.gz *
mv update-$CONFIG_QM_VERSION.tar.gz $QM_ALL_IMAGES/
cd -
rm -rf $QM_ALL_IMAGES/update/

echo "pack ota update end"


