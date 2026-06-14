#!/bin/bash
_sdk_build_FB_=""
_sdk_fb_=""
if [ "$BUILD_FASTBOOT" = true ];then
    _sdk_build_FB_="FB=1"
    _sdk_fb_="_fb"
fi


if [[ "$BUILD_SDK" != "qm10xd_linux" ]] ; then
    echo "Warnning! none lt00 sdk setting, default will be qm102d_sdk."
    _sdk_name_=qm102d_sdk
fi

_sdk_build_name_=$BUILD_SDK

source ./build/script/envsetup4qm102d.sh




echo "source encsetup4qm102d.sh"
echo $PATH

export _top_img_out_dir_=$_sdk_build_name_$_is_fb_
_xos_img_out_dir_=out/$_top_img_out_dir_/xos
_sdk_img_out_dir_=out/$_top_img_out_dir_/bsp
#_sdk_img_cpy_dir_=target/$_top_img_out_dir_/image
export _soc_pkg_path=base/soc/qm10xd/linux
_soc_pkg_to_root_dir=../../../..
export CUR_PROJ_TYPE=""

#_sdk_cross_compile=arm-molv510-linux-
_sdk_cross_compile=arm-molv2-linux-uclibcgnueabi-

_soc_pkg_build_ouput_dir=$_soc_pkg_path/output
_soc_pkg_build_board_dir=output/lt00
_soc_pkg_build_bsp_dir=$_soc_pkg_build_board_dir/bsp            # $_sdk_fb_
_soc_pkg_build_rootfs_dir=$_soc_pkg_build_bsp_dir/rootfs


echo "_top_img_out_dir_=$_top_img_out_dir_"
echo ""
echo "BUILD_PROJECT=$BUILD_PROJECT"
echo "_board_name_=$_board_name_"
echo "_sdk_name_=$_sdk_name_"
echo ""

export _soc_pkg_bsp_path=$_soc_pkg_path/bsp
export _soc_pkg_media_path=$_soc_pkg_path/media
export _soc_bsp_sub_path=bsp
export _soc_bsp_uboot_path=$_soc_bsp_sub_path/uboot
export _soc_bsp_kernel_path=$_soc_bsp_sub_path/kernel
export _soc_media_sub_path=media


#
PRJ_XOS_PRODUCT_DIR="product"
PRJ_DIR_NAME=$(echo "$BUILD_PROJECT" | tr '_' ' ' | cut -d' ' -f1)
PRJ_PRODUCT_CONFIG="$PRJ_XOS_PRODUCT_DIR/$PRJ_DIR_NAME/project_${BUILD_PROJECT}_conf.mk"

#
CMD1=`realpath $0`
export XOS_SCRIPT_DIR=`dirname $CMD1`
export PRJ_TOP_DIR=$(realpath $XOS_SCRIPT_DIR/../..)
export _top_board_conf_dir_=$PRJ_TOP_DIR/$PRJ_XOS_PRODUCT_DIR/${PRJ_DIR_NAME}

#echo "XOS_SCRIPT_DIR:$XOS_SCRIPT_DIR"
#echo "PRJ_TOP_DIR:$PRJ_TOP_DIR"
echo "_top_board_conf_dir_:$_top_board_conf_dir_"


function build_select_board()
{
    echo "build_select_board"
	ln -rfs $PRJ_PRODUCT_CONFIG ${PRJ_XOS_PRODUCT_DIR}/.BoardConfig.mk
	echo "switching to board: `realpath $BOARD_CONFIG`"
}

function unset_board_config_all()
{
    echo "unset_board_config_all"
	local tmp_file=`mktemp`
	grep -o "^export.*QM_SYS_.*=" `find $PRJ_TOP_DIR/product -name "project*.mk" -type f` -h | sort | uniq > $tmp_file
	grep -o "^export.*CONFIG_XOS_.*=" `find $PRJ_TOP_DIR/product -name "project*.mk" -type f` -h | sort | uniq >> $tmp_file
	source $tmp_file
	rm -f $tmp_file
}

BOARD_CONFIG="$PRJ_TOP_DIR/${PRJ_XOS_PRODUCT_DIR}/.BoardConfig.mk"

if [ ! -L "$BOARD_CONFIG" -a  "$1" != "lunch" ]; then
	build_select_board
fi

unset_board_config_all

[ -L "$BOARD_CONFIG" ] && source $BOARD_CONFIG
echo "BOARD_CONFIG:$BOARD_CONFIG"

function finish_build(){
	echo "Running ${FUNCNAME[1]} succeeded."
	cd $PRJ_TOP_DIR
}

function copy_etc_to_rootfs()
{
    #mkdir
    cp_to_rootfs_dir=$_soc_pkg_path/$_soc_pkg_build_rootfs_dir
    _xos_etc_dir_=core/board/generic/lt00/etc

    echo "copy rcS to $cp_to_rootfs_dir"
    #cp -arf $_xos_etc_dir_/rcS $cp_to_rootfs_dir/../../bsp_fb/rootfs/etc/init.d/
    cp -arf $_xos_etc_dir_/rcS $cp_to_rootfs_dir/../../bsp/rootfs/etc/init.d/
    #cp -arf $_xos_etc_dir_/qboot.sh $cp_to_rootfs_dir/../../bsp_fb/rootfs/usr/qsh/qboot.sh
    cp -arf $_xos_etc_dir_/qboot.sh $cp_to_rootfs_dir/../../bsp/rootfs/usr/qsh/qboot.sh
}

function copy_app_to_rootfs()
{
    #mkdir
    cp_to_rootfs_dir=$_soc_pkg_path/$_soc_pkg_build_rootfs_dir
    if [ ! -d "$cp_to_rootfs_dir/usr/elf/" ]; then
        mkdir -p "$cp_to_rootfs_dir/usr/elf/"
    fi
    #cp or exit
    if [ -f "$_xos_img_out_dir_/bin/qxosui" ];then
        cp -arf $_xos_img_out_dir_/bin/qxosui "$cp_to_rootfs_dir/usr/elf/"
    else
        echo "copy_app_to_rootfs failed. no qxosui and exit."
        exit
    fi
}

function build_pkg_envsetup()
{
    ./envsetup.sh
    echo set env for $CUR_PROJ_TYPE
}

function fastbuild_zImage()
{
#    copy_app_to_rootfs

    #copy bin/* to sdk/out and package to zImage
    pushd $_soc_pkg_path
    build_pkg_envsetup
    make ramdisk -j$BUILD_JOBS || exit "$$?"
    make kernel_image -j$BUILD_JOBS || exit "$$?"

    mkdir -p $_soc_pkg_to_root_dir$_sdk_img_out_dir_
    cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    popd

    echo "fastbuild_zImage"
}

function build_xosui_so()
{
    echo "build_xosui_so"
    #build gui engine shared.
    make -f ./build/make/core/board.mk module=lvgl CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN all -j$BUILD_JOBS || exit "$$?"
    #build modules
    #common videoplayer imageviewer gallery fileexplorer videocall camera audioplayer facerecognize coffeemill health demolist smartdoor setting
    module_list="App"
    ldflag=""
    for m in $module_list;do
        ldflag="${ldflag};-l${m}"
        make -f ./build/make/core/board.mk module=$m CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN all -j$BUILD_JOBS || exit "$$?"
    done
    # build main app executable file.
    make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN -j$BUILD_JOBS || exit "$$?"
    make linkmodules=$ldflag CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ all -j$BUILD_JOBS || exit "$$?"
}

#param is 1 will exit; 0 will continue(for build all)
function build_xosui_default()
{
    echo "build_xosui_default"
    make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN all -j$BUILD_JOBS || exit "$$?"
}

function build_xosui_module()
{
    echo "build module $BUILD_MODULE only."
    if [ "$CLEAN" != "" ]; then
        make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ ${BUILD_MODULE}_${CLEAN} || exit "$$?"
    fi
    make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ ${BUILD_MODULE} -j$BUILD_JOBS || exit "$$?"
}

function build_xosui_system()
{
    if [ "$CLEAN" != "" ]; then
        rm -rf $_xos_img_out_dir_
        rm -rf $PRJ_XOS_PRODUCT_DIR/.BoardConfig.mk
    fi

    if [[ "$BUILD_SHARED_APP" = true ]];then
        build_xosui_so
    else
        build_xosui_default
    fi
}

function build_framework()
{
    if [ -f ./core/package/framework/quaAI/build.sh ]; then
        cd ./core/package/framework/quaAI/
        ./build.sh $BUILD_SDK || exit
        cd -
    fi
	
    if [ -f ./core/package/framework/quacamera/build.sh ]; then
        cd ./core/package/framework/quacamera/
        ./build.sh $BUILD_SDK || exit
        cd -
    fi
}

function build_xosui()
{
    ##build_framework
    local __start_time __end_time __exec_time

    echo "build_xosui arg = "$1
    __start_time=$(date +%s)

    if [[ "$BUILD_MODULE" = "" ]];then
        build_xosui_system
    else
        build_xosui_module
    fi

    __end_time=$(date +%s)
    __exec_time=$((__end_time - __start_time))
    echo "build xosui executed time: $__exec_time seconds"
}

function xz_overlay()
{
        tmppath=$(mktemp -d)
        pushd $tmppath
	FAKEROOT_PATH=$(PATH=".:$PATH" which fakeroot)
        if [ -z "$FAKEROOT_PATH" ]; then
          echo "Error: fakeroot not found. Please install with: sudo apt install fakeroot"
          popd >/dev/null
          rm -rf "$tmppath"
          exit 1
        fi
        "$FAKEROOT_PATH" xz -d -c "$1"|cpio -id
        rsync -aHAXr --exclude='overlay_for_precopy.sh' "$2/"  $tmppath
        rm  dev/console > /dev/null 2>&1
        rm  dev/null > /dev/null 2>&1
        "$FAKEROOT_PATH" -u <<EOF
        mknod -m 666 dev/console c 5 1
        mknod -m 666 dev/null c 1 3
        find . -print0 | cpio --null -ov --format=newc | xz --check=crc32 -c -k --lzma2 >"$3"
EOF
popd
         rm -rf $tmppath
}


function build_soc_pkg
{
	echo "build_soc_pkg start"

	#pushd $_soc_bsp_uboot_path

    #make O=.build clean -j$BUILD_JOBS
    #make O=.build || exit "$$?"
	make boot #Uboot 全编译
	make boot_build #Uboot&SDL 
	make ramboot #PDL&SPL
    echo "make boot -j$BUILD_JOBS done"

	#popd
	
	#pushd $_soc_bsp_kernel_path
    OVERLAY_ROOTFS=$_top_board_conf_dir_/rootfs_overlay
    echo $OVERLAY_ROOTFS
    echo $(pwd)
    $OVERLAY_ROOTFS/overlay_for_precopy.sh
    ${_sdk_cross_compile}strip $OVERLAY_ROOTFS/usr/lib/*
    ${_sdk_cross_compile}strip $OVERLAY_ROOTFS/usr/lib/quamm/*
    ${_sdk_cross_compile}strip $OVERLAY_ROOTFS/bin/*
    ${_sdk_cross_compile}strip $OVERLAY_ROOTFS/lib/*
    pushd bsp/rootfs/
    rootfs_origin=$(realpath "origin-initramfs-v510.cpio.xz")
    rootfs_overlay=$(realpath "$OVERLAY_ROOTFS")
    rootfs_dest=$(realpath "initramfs-v510.cpio.xz")
    xz_overlay "$rootfs_origin" "$rootfs_overlay" "$rootfs_dest"
    popd
    #make O=.build clean -j$BUILD_JOBS
    #make O=.build || exit "$$?"
    make kernel 
    echo "$_soc_pkg_to_root_dir"
    echo "$_sdk_img_out_dir_"
    echo "make kernel -j$BUILD_JOBS done"

	#popd
	echo "build_soc_pkg end"
    #mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
}

function build_all()
{
    echo "build_all"
    local __start_time __end_time __exec_time
    __start_time=$(date +%s)
    proj_type=$(echo ${CONFIG_XOS_PROJECT_TYPE} | sed -n 's/^\([[:alnum:]]*\).*/\1/p')	
    CUR_PROJ_TYPE=$proj_type
    build_xosui 0

    pushd $_soc_pkg_path
    build_pkg_envsetup
#    make clean
###    make rootfs || exit "$$?"
###    copy_etc_to_rootfs
#    make ramdisk || exit "$$?"
    #make driver_clean -j$BUILD_JOBS
    #make driver -j$BUILD_JOBS || exit "$$?"

    # do this after rootfs unzip.
    popd
#    copy_app_to_rootfs

    pushd $_soc_pkg_path
    echo "build_all  _soc_pkg_path: $_soc_pkg_path"

    build_soc_pkg

    mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

    #copy uboot
    #cp -arf $_soc_bsp_uboot_path/uboot/.build/u-boot.bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
	#copy kernel
    #cp -arf $_soc_bsp_kernel_path/linux-5.10.y/.build/arch/arm/boot/uImage $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

	cp -arf output/qm10xd_${proj_type}/bsp/boot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
	cp -arf output/qm10xd_${proj_type}/bsp/pdl $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf output/qm10xd_${proj_type}/bsp/spl $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
	#copy kernel
	cp -arf output/qm10xd_${proj_type}/bsp/kernel $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

    popd

	###build_firmware

    echo "End to build linux xos!"
    __end_time=$(date +%s)
    __exec_time=$((__end_time - __start_time))
    echo "build all executed time: $__exec_time seconds"
    exit
}

function build_cleanall(){
	echo " build_cleanall"

	#rm -rf $MKIMAGE_PATH_ROOT
	finish_build
}

function build_firmware(){
	echo " build_firmware"
	pushd $XOS_SCRIPT_DIR
	source ./mkfirmware.sh $BOARD_CONFIG
	local __err=$?
	popd
	if [ $__err -eq 0 ]; then
		if [[ "$BUILD_SDK" == "qm10xd_linux" ]] && [[ ${PRJ_DIR_NAME} == *camera* ]]; then
			echo " remove image xos mkimg_part"
			rm -rf out/$BUILD_SDK/image
			rm -rf out/$BUILD_SDK/xos
			rm -rf out/$BUILD_SDK/mkimg_part
		fi
	fi
	finish_build
}

if [[ "$BUILD_XOSUI" = true ]]; then
    echo "build ui only."
    if [ "$BUILD_MODULE" != "" ]; then
        build_xosui 3
    else
        build_xosui 1
    fi
    exit
fi

if [[ "$BUILD_ALL" = true ]]; then
    echo "build all cmd = true"
    build_all
fi

if [[ "$BUILD_UBOOT" = true ]]; then
    #build uboot from eh05
    echo "build uboot from eh05"
#    pushd $_soc_pkg_path/bsp/bootloader/build
#    ./qm_build_eh05_uboot.sh
#    echo "qm_build_eh05_uboot.sh finished"
#    popd
    pushd $_soc_pkg_path
    build_pkg_envsetup
    #make O=.build boot_clean -j$BUILD_JOBS
    #make O=.build boot -j$BUILD_JOBS || exit "$$?"
	make boot #Uboot 全编译
	make boot_build #Uboot&SDL 
	make ramboot #PDL&SPL
    mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    cp -arf output/lt00/bsp/boot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
	cp -arf output/lt00/bsp/pdl $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    cp -arf output/lt00/bsp/spl $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    popd

    exit
fi

if [[ "$BUILD_KERNEL" = true ]]; then
    pushd $_soc_pkg_path
    build_pkg_envsetup
    #make O=.build kernel_clean -j$BUILD_JOBS
    #make O=.build kernel || exit "$$?"
	make kernel
    echo "$_soc_pkg_to_root_dir"
    echo "$_sdk_img_out_dir_"
    mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    cp -arf output/lt00/bsp/kernel $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    popd

    exit
fi

if [[ "$BUILD_PACKAGES" = true ]]; then
    echo "build_packages"
    build_xosui 0
    fastbuild_zImage
    exit
fi

if [[ "$BUILD_FW_IMG" = true ]]; then
    echo "build_fireware images"
    build_firmware
    exit
fi

echo "build all default. BUILD_PACKAGES=$BUILD_PACKAGES"

build_all
echo -e "\e[36m build_all: ready \e[0m"
