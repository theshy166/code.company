#!/bin/bash

echo "PRJ_TOP_DIR: $PRJ_TOP_DIR"
echo "BUILD_PROJECT=$CONFIG_XOS_PROJECT_TYPE"
#echo "_top_board_conf_dir_: ${_top_board_conf_dir_}"
echo "MKIMG_PATH_ROOT: $MKIMG_PATH_ROOT"
echo "QM_ALL_IMAGES: $QM_ALL_IMAGES"

DES_OUT_ROOT_DIR=$MKIMG_PATH_ROOT
DES_OUT_FINAL_DIR=$QM_ALL_IMAGES

DES_OUT_RECOVERY_DIR=$DES_OUT_ROOT_DIR/recovery
DES_OUT_BOOT_DIR=$DES_OUT_ROOT_DIR/boot
DES_OUT_LOGO_DIR=$DES_OUT_ROOT_DIR/logo
DES_OUT_RES_DIR=$DES_OUT_ROOT_DIR/system/res
DES_OUT_QUA_DIR=$DES_OUT_ROOT_DIR/system/qua
DES_OUT_APP_DIR=$DES_OUT_ROOT_DIR/system/app
DES_OUT_ROOTFS_DIR=$DES_OUT_ROOT_DIR/rootfs
DES_OUT_DATA_DIR=$DES_OUT_ROOT_DIR/data



mkdir -p $DES_OUT_RES_DIR $DES_OUT_QUA_DIR $DES_OUT_APP_DIR $DES_OUT_DATA_DIR

OVERLAY_QUA_DIR=$PRJ_TOP_DIR/product/$CONFIG_XOS_PROJECT_TYPE/qua_overlay


_soc_pkg_path=base/soc/qm10xd/linux
echo "_soc_pkg_path: $_soc_pkg_path"
_soc_prebuilt_path=base/soc/qm10xd/linux/prebuilts/${CONFIG_XOS_PROJECT_TYPE}

SRC_FILE_QUA_COM_KO=$_soc_pkg_path/media/mpp/prebuilt/media/ko
# if folder SRC_FILE_QUA_COM_KO is not existed, use prebuilt path as instead
if [ -d "$_soc_prebuilt_path/media/ko" ]; then
    SRC_FILE_QUA_COM_KO=$_soc_prebuilt_path/media/ko
fi
echo "SRC_FILE_QUA_COM_KO:$SRC_FILE_QUA_COM_KO"

SRC_FILE_QUA_COM_LIB=$_soc_pkg_path/media/mpp/prebuilt/media/lib
if [ -d "$_soc_prebuilt_path/media/lib" ]; then
    SRC_FILE_QUA_COM_LIB=$_soc_prebuilt_path/media/lib
fi
echo "SRC_FILE_QUA_COM_LIB:$SRC_FILE_QUA_COM_LIB"

#copy files to $MKIMG_PATH_ROOT/xxx partition dir
echo ""
echo "QM: copy files to $MKIMG_PATH_ROOT xxx partitions"

###copy to partion: recovery
echo "::copy files to $DES_OUT_RECOVERY_DIR"
## TODO

###copy to partion: boot
echo "::copy files to $DES_OUT_BOOT_DIR"

###copy to partion: logo
echo "::copy files to $DES_OUT_LOGO_DIR"

###copy wifi
echo "copy wifi files "


cp -arf $PRJ_TOP_DIR/core/package/hardware/deviceDrv/wifi/${CONFIG_XOS_WIFI_QUADRV}/prebuilts/${CONFIG_XOS_PROJECT_TYPE}/*  $DES_OUT_QUA_DIR/

# $_soc_pkg_path/prebuilts/media/lib
mkdir -p $DES_OUT_QUA_DIR/lib && cp -rfa $SRC_FILE_QUA_COM_LIB/*.so $DES_OUT_QUA_DIR/lib
mkdir -p $DES_OUT_QUA_DIR/ko  && cp -rfa $SRC_FILE_QUA_COM_KO/* $DES_OUT_QUA_DIR/ko

#
####copy to partion: rootfs
function xz_decompress()
{
	SRC_FILE_ROOTFS=$PRJ_TOP_DIR/$_soc_pkg_path/prebuilts/${CONFIG_XOS_PROJECT_TYPE}/bsp/rootfs/initramfs-v510.cpio.xz
        pushd $DES_OUT_ROOTFS_DIR
        /bin/fakeroot xz -d -c $SRC_FILE_ROOTFS | /bin/fakeroot cpio -id
	echo "rm console and null"
        rm  dev/console > /dev/null 2>&1
        rm  dev/null > /dev/null 2>&1
	echo "mknod console and null"
        fakeroot -u <<EOF
        mknod -m 666 dev/console c 5 1
        mknod -m 666 dev/null c 1 3
EOF
popd
}

echo "::copy files to $DES_OUT_ROOTFS_DIR"
xz_decompress
echo "copy to rootfs dir end"
#
####copy to partion: data
echo "::copy files to $DES_OUT_DATA_DIR"
if [ -d "$DES_OUT_ROOT_DIR/../xos/bin/" ];then
cp -arf $DES_OUT_ROOT_DIR/../xos/bin/qxosui* $DES_OUT_APP_DIR
fi
if [ -d "$DES_OUT_ROOT_DIR/../xos/lib/" ];then
cp -arf $DES_OUT_ROOT_DIR/../xos/lib $DES_OUT_QUA_DIR
rm -rf $DES_OUT_QUA_DIR/*.a
fi
if [ -d "$DES_OUT_ROOT_DIR/../xos/res/ttf" ];then
cp -arf $DES_OUT_ROOT_DIR/../xos/res/ttf $DES_OUT_RES_DIR
fi
if [ -d "$DES_OUT_ROOT_DIR/../xos/res/${CONFIG_XOS_PROJECT_TYPE}" ];then
cp -arf $DES_OUT_ROOT_DIR/../xos/res/${CONFIG_XOS_PROJECT_TYPE} $DES_OUT_RES_DIR
fi
#
#

#overlay for qua
if [ -d "$OVERLAY_QUA_DIR/ko/" ];then
cp -rfa $OVERLAY_QUA_DIR/ko/* $DES_OUT_QUA_DIR/ko
fi
if [[ "$CONFIG_XOS_FB_USE_ARGB1555" = true ]];then
cp -rfa $OVERLAY_QUA_DIR/ko_ARGB1555/* $DES_OUT_QUA_DIR/ko
fi
#TODO
#add more overlay here
#overlay for res

#overlay for app

#
#echo "QM: pre mkimg::copy files to $DES_OUT_DIR, finish"
#
