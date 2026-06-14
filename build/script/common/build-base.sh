#!/bin/bash

[ -e ".config.env.unset" ] && source ./.config.env.unset
source ./.config.env

export BUILD_FASTBOOT=${CONFIG_XOS_BUILD_FASTBOOT}
export BUILD_SDK=${CONFIG_XOS_BUILD_SDK}
export BUILD_STORAGE=${CONFIG_STORAGE_TYPE_SPINOR}
export BUILD_NOQUI=${CONFIG_XOS_USE_NO_QUAUI}

echo "BUILD_SDK=$BUILD_SDK"
echo "BUILD_FASTBOOT=$BUILD_FASTBOOT"
echo "BUILD_STORAGE=$BUILD_STORAGE"
echo "BUILD_NOQUI=$BUILD_NOQUI"

usage()
{
    echo "USAGE: [-C] [-U] [-K] [-Q] [-A] [-V] [-I] [-J] [-R] [-?] [-H] [-h]"
    echo "No ARGS means use default build option                   "
    echo "Option: -C = build clean                                 "
    echo "        -U = build uboot                                 "
    echo "        -K = build kernel                                "
    echo "        -Q = build quadrv                                "
    echo "        -A = build ALL                                   "
    echo "        -V = build with 'userdebug' or 'user'            "
    echo "        -I = generate firmware images(*.img)             "
    echo "        -J = jobs number                                 "
    echo "        -N = distclean                                   "
    echo "        -R = post-build RTOS                             "
    echo "  -?/-h/-H = help information                            "
    exit 1
}

export BUILD_MEDIUM=""
export BUILD_CLEAN=false
export BUILD_UBOOT=false
export BUILD_KERNEL=false
export BUILD_QUADRV=false
export BUILD_ALL=false
export BUILD_TYPE="userdebug"
export BUILD_FW_IMG=false
export BUILD_PACKAGES=false
export BUILD_JOBS=1     # =$(nproc)
export BUILD_DISTCLEAN=false
export BUILD_RTOS_POSTBUILD=false

if [ "$BUILD_STORAGE" = true ]; then
    BUILD_MEDIUM="norflash"
fi
# check pass argument
while getopts "NCUKQARIV:J:hH?" OPT
do
    case $OPT in
        U)
            echo "build u-boot"
            BUILD_UBOOT=true
            ;;
        K)
            echo "build kernel"
            BUILD_KERNEL=true
            ;;
	    Q)
            echo "build quadrv"
            BUILD_QUADRV=true
            ;;
        A)
            echo "build all"
            BUILD_ALL=true

            ;;
        V)
            echo "build with $OPTARG"
            BUILD_TYPE=$OPTARG
            ;;
        J)
            echo "build CPU JOBS $OPTARG"
            BUILD_JOBS=$OPTARG
            ;;
        C)
            echo "build with clean"
            CLEAN=clean
            BUILD_CLEAN=true
            ;;
        N)
            echo "distclean"
            BUILD_DISTCLEAN=true
            ;;
        I)
            echo "generate firmware images(*.img)"
            BUILD_FW_IMG=true
            ;;
        R)
            echo "RTOS Post Build"
            BUILD_RTOS_POSTBUILD=true
            ;;
        h|H)
            usage ;;
        ?)
            usage ;;
    esac
done

_sdk_fb_=""
_sdk_FB_=""
if [ "$BUILD_FASTBOOT" = true ];then
    _sdk_fb_="_fb"
    _sdk_FB_="FB=1"
fi

_sdk_64mb_=""
if [ "$BUILD_SDK" = "linux_64" ]; then
    echo "Information! 64MB linux SDK will be selected."
    _sdk_64mb_="_64mb"
fi

_sdk_name_=""
_realboard=""
if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
    _sdk_name_=fy00_linux
    _realboard=fy00b$_sdk_64mb_$_sdk_fb_/$CONFIG_XOS_PROJECT_TYPE
    BUILD_JOBS=8
fi

if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
    source ./build/script/envsetup4qm102d.sh
    _sdk_cross_compile=arm-molv2-linux-uclibcgnueabi-
fi

if [[ "$BUILD_SDK" = "qm10xd_rtt" ]]; then
    source ./build/script/qm10xd/envsetup4rttos.sh
    _sdk_cross_compile=arm-molv2-linux-uclibcgnueabi-
fi

if [[ "$BUILD_SDK" = "qm10xh_linux" ]]; then
    source ./build/script/envsetup4qm108h.sh
    _sdk_cross_compile=arm-fhv512-linux-uclibcgnueabihf-
fi

if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
    source ./build/script/envsetup4qm102v.sh
    _sdk_cross_compile=arm-fhv512-linux-uclibcgnueabihf-
fi


if [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
    source ./build/script/envsetup4qm108hv2.sh
    _sdk_cross_compile=arm-fhva12c-linux-uclibcgnueabihf-
fi

if [[ "$BUILD_SDK" = "qm10xv_rtt" ]]; then
    source ./build/script/qm10xv/envsetup4rttos.sh
    _sdk_cross_compile=arm-fullhanv2-eabi-
fi

if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
    source ./build/script/envsetup4linux.sh
    _sdk_cross_compile=arm-mol-linux-uclibcgnueabihf-
fi

if [[ "$BUILD_SDK" = "rtt" || "$BUILD_SDK" = "fy00_rtt" ]]; then
    source ./build/script/envsetup4rttos.sh
    _sdk_cross_compile=arm-linux-musleabi-
fi

if [[ "$BUILD_SDK" = "qm10xh_linux" ]]; then
    _soc_pkg_vxxx=qm10xh_linux
fi
_sdk_build_name_=${BUILD_SDK}

export _top_img_out_dir_=$_sdk_build_name_$_sdk_64mb_$_is_fb_
export _xos_img_out_dir_=out/$_top_img_out_dir_/xos
_sdk_img_out_dir_=out/$_top_img_out_dir_/image

export _soc_pkg_path=${CONFIG_XOS_SDK_PATH}
export _soc_pkg_to_root_dir=../../../..
_xos_etc_path=core/board/generic/

if [[ "$BUILD_SDK" = "qm10xv_rtt" ]]; then
    _soc_pkg_build_board_dir=output/${BUILD_SDK}_${CONFIG_XOS_PROJECT_TYPE}
    _soc_pkg_build_bsp_dir=output
fi

if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
    _soc_pkg_build_board_dir=output
    _soc_pkg_build_bsp_dir=output
fi

if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
    _soc_pkg_build_board_dir=output/$CONFIG_XOS_PROJECT_TYPE
    _soc_pkg_build_bsp_dir=$_soc_pkg_build_board_dir/bsp            # $_sdk_fb_
fi

if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
    _soc_pkg_build_board_dir=output/qm102v
    _soc_pkg_build_bsp_dir=$_soc_pkg_build_board_dir/bsp
    create_xmodem_tools_dir=prebuilts/${CONFIG_XOS_PROJECT_TYPE}/bsp/tools/flash_upgrade

    export _soc_pkg_bsp_path=$_soc_pkg_path/board_support
    export _soc_pkg_media_path=$_soc_pkg_path/media_support
    export _soc_bsp_sub_path=board_support
    export _soc_bsp_uboot_path=$_soc_bsp_sub_path/uboot
    export _soc_bsp_kernel_path=$_soc_bsp_sub_path/kernel
    export _soc_media_sub_path=media_support
fi

if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
    _soc_pkg_build_board_dir=output/qm10xd_$CONFIG_XOS_PROJECT_TYPE$CONFIG_STORAGE_TYPE
    _soc_pkg_build_bsp_dir=$_soc_pkg_build_board_dir/bsp            # $_sdk_fb_
    _soc_pkg_prebuilts_recovery_dir=prebuilts/$CONFIG_XOS_PROJECT_TYPE/bsp/recovery
fi

if [[ "$BUILD_SDK" = "qm10xd_rtt" ]]; then
    _soc_pkg_build_board_dir=output/lt00_rtt_nand_dmb
    _soc_pkg_build_bsp_dir=$_soc_pkg_build_board_dir/bsp            # $_sdk_fb_
    _soc_pkg_prebuilts_recovery_dir=prebuilts/$CONFIG_XOS_PROJECT_TYPE/bsp/recovery
fi

_soc_pkg_build_ouput_dir=$_soc_pkg_path/output
_soc_pkg_build_rootfs_dir=$_soc_pkg_build_bsp_dir/rootfs

echo "_top_img_out_dir_=$_top_img_out_dir_"
echo "_soc_pkg_build_board_dir=$_soc_pkg_build_board_dir"
echo "_soc_pkg_build_bsp_dir=$_soc_pkg_build_bsp_dir"
echo "_soc_pkg_build_ouput_dir=$_soc_pkg_build_ouput_dir"
echo "_soc_pkg_build_rootfs_dir=$_soc_pkg_build_rootfs_dir"
echo "_soc_pkg_prebuilts_recovery_dir=$_soc_pkg_prebuilts_recovery_dir"

CMD1=`realpath $0`
export XOS_SCRIPT_DIR=`dirname $CMD1`
export PRJ_TOP_DIR=$(realpath $XOS_SCRIPT_DIR/../../..)
export _top_board_conf_dir_="${PRJ_TOP_DIR}/product/${CONFIG_XOS_PROJECT_TYPE}"
echo "_top_board_conf_dir_: ${_top_board_conf_dir_}"
export drv_path=core/package/hardware/quaDrv
export _kernel_obj_dest

function make_strip() {
    local dir_path="$1"
    if [ -d "$dir_path" ]; then
        #find "$dir_path" -type f -exec ${_sdk_cross_compile}strip -v --strip-debug {} \;
	find "$dir_path" -type f -not -name "*.ko" -exec ${_sdk_cross_compile}strip  {} \;
    else
        return 1
    fi
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

function finish_build(){
    echo "Running ${FUNCNAME[1]} succeeded."
    cd $PRJ_TOP_DIR
}

function copy_etc_to_rootfs()
{
    #mkdir
    cp_to_rootfs_dir=$_soc_pkg_path/$_soc_pkg_build_rootfs_dir
    _xos_etc_dir_=core/board/generic/${CONFIG_XOS_BUILD_BOARD}/etc

    echo "copy rcS to $cp_to_rootfs_dir"
    #cp -arf $_xos_etc_dir_/rcS $cp_to_rootfs_dir/../../bsp_fb/rootfs/etc/init.d/
    cp -arf $_xos_etc_dir_/rcS $cp_to_rootfs_dir/../../bsp/rootfs/etc/init.d/
    #cp -arf $_xos_etc_dir_/qboot.sh $cp_to_rootfs_dir/../../bsp_fb/rootfs/usr/qsh/qboot.sh
    cp -arf $_xos_etc_dir_/qboot.sh $cp_to_rootfs_dir/../../bsp/rootfs/usr/qsh/qboot.sh
}

function copy_output_to_out()
{
    if [ ! -d "$_xos_img_out_dir_/res" ]; then
        mkdir -p "$_xos_img_out_dir_/res"
    fi
    if [ ! -d "$_xos_img_out_dir_/lib" ]; then
        mkdir -p "$_xos_img_out_dir_/lib"
    fi
    if [ ! -d "$_xos_img_out_dir_/bin" ]; then
        mkdir -p "$_xos_img_out_dir_/bin"
    fi

    if [ -d "output/res" ];then
        cp -arf output/res/* "$_xos_img_out_dir_/res/"
    fi
    if [ -d "output/lib" ];then
        cp -arf output/lib/* "$_xos_img_out_dir_/lib/"
	    rm $_xos_img_out_dir_/lib/*.a
    fi
    if [ -d "output/bin" ];then
        cp -arf output/bin/* "$_xos_img_out_dir_/bin/"
    fi
    if [ -f "output/qxosui" ];then
        cp -arf output/qxosui "$_xos_img_out_dir_/bin/"
    fi
    if [ -f "output/qxosui.elf" ];then
        cp -arf output/qxosui.elf "$_xos_img_out_dir_/bin/"
        mv $_xos_img_out_dir_/bin/qxosui.elf $_xos_img_out_dir_/bin/qxosui
    fi
    ##${_sdk_cross_compile}strip $_xos_img_out_dir_/bin/qxosui
    if [ -d "output/etc/temp" ];then
        mkdir -p "$_xos_img_out_dir_/etc/temp"
        cp -arf output/etc/temp/* "$_xos_img_out_dir_/etc/temp/"
    fi
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
	"$FAKEROOT_PATH" xz -d -c "$1" | cpio -id
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

function qm10xh_make_rootfs()
{
    echo qm10xh_make_rootfs
    rootfs_overlay_path=$_top_board_conf_dir_/rootfs_overlay
    if [ -f "$rootfs_overlay_path/overlay_for_precopy.sh" ]; then
        $rootfs_overlay_path/overlay_for_precopy.sh
    else
        echo "Warning: $rootfs_overlay_path/overlay_for_precopy.sh not found"
        exit 1
    fi

    pushd prebuilts/${CONFIG_XOS_PROJECT_TYPE}/bsp/rootfs
    rootfs_origin=$(realpath "origin.rootfs.cpio.xz")
    rootfs_dest=$(realpath "rootfs.cpio.$CONFIG_ROOTFS_FORMAT")
    popd

    echo "rootfs_origin=$rootfs_origin"
    echo "rootfs_dest=$rootfs_dest"
    echo "rootfs_overlay_path=$rootfs_overlay_path"

    if [[ "$CONFIG_ROOTFS_FORMAT" = "xz" ]]; then
        mk_cmd="find . -print0 | cpio --null -ov --format=newc | xz --check=crc32 -c -k --lzma2 > $rootfs_dest"
    elif [[ "$CONFIG_ROOTFS_FORMAT" = "lz4" ]]; then
        mk_cmd="find . -print0 | cpio --null -ov --format=newc | lz4 -l -c > $rootfs_dest"
    elif [[ "$CONFIG_ROOTFS_FORMAT" = "ubifs" ]]; then
        mk_cmd="find . -print0 | cpio --null -ov --format=newc > $rootfs_dest"
    else
        echo "Unsupported compressed format: $CONFIG_ROOTFS_FORMAT"
        exit 1
    fi

    tmppath=$(mktemp -d)
    pushd $tmppath
    fakeroot -u sh -c "
        xz -d -c $rootfs_origin | cpio -id
        rsync -aHAXr --exclude='overlay_for_precopy.sh' $rootfs_overlay_path/ $tmppath
        $mk_cmd
    "
    popd
    rm -rf $tmppath
}

# 新增针对V2版本的rootfs构建函数
function qm10xhv2_make_rootfs()
{
    echo qm10xhv2_make_rootfs
    rootfs_overlay_path=$_top_board_conf_dir_/rootfs_overlay
    if [ -f "$rootfs_overlay_path/overlay_for_precopy.sh" ]; then
        $rootfs_overlay_path/overlay_for_precopy.sh
    else
        echo "Warning: $rootfs_overlay_path/overlay_for_precopy.sh not found"
        exit 1
    fi
    pushd prebuilts/${CONFIG_XOS_PROJECT_TYPE}/bsp/rootfs
    rootfs_origin=$(realpath "origin.rootfs.cpio.xz")
    # rootfs_dest=$(realpath "rootfs.cpio.$CONFIG_ROOTFS_FORMAT")
    rootfs_dest=$(realpath "rootfs.cpio")
    popd
    echo "rootfs_origin=$rootfs_origin"
    echo "rootfs_dest=$rootfs_dest"
    echo "rootfs_overlay_path=$rootfs_overlay_path"

    # if [[ "$CONFIG_ROOTFS_FORMAT" = "xz" ]]; then
    #     mk_cmd="find . -print0 | cpio --null -ov --format=newc | xz --check=crc32 -c -k --lzma2 > $rootfs_dest"
    # elif [[ "$CONFIG_ROOTFS_FORMAT" = "lz4" ]]; then
    #     mk_cmd="find . -print0 | cpio --null -ov --format=newc | lz4 -l -c > $rootfs_dest"
    # elif [[ "$CONFIG_ROOTFS_FORMAT" = "ubifs" ]]; then
    #     mk_cmd="find . -print0 | cpio --null -ov --format=newc > $rootfs_dest"
    # else
    #     echo "Unsupported compressed format: $CONFIG_ROOTFS_FORMAT"
    #     exit 1
    # fi
    mk_cmd="find . -print0 | cpio --null -ov --format=newc > $rootfs_dest"
    tmppath=$(mktemp -d)
    pushd $tmppath
    fakeroot -u sh -c "
        xz -d -c $rootfs_origin | cpio -id
        rsync -aHAXr --exclude='overlay_for_precopy.sh' $rootfs_overlay_path/ $tmppath
        mkdir -p $tmppath/system/qua
        cp -r $PRJ_TOP_DIR/$_soc_pkg_path/prebuilts/${CONFIG_XOS_PROJECT_TYPE}/media/* $tmppath/system/qua/
        $mk_cmd
    "
    popd
    rm -rf $tmppath
}

function qm10xv_make_rootfs()
{
    rootfs_overlay_path=$_top_board_conf_dir_/rootfs_overlay
    if [ -f "$rootfs_overlay_path/overlay_for_precopy.sh" ]; then
        $rootfs_overlay_path/overlay_for_precopy.sh
    else
        echo "Warning: $rootfs_overlay_path/overlay_for_precopy.sh not found"
        exit 1
    fi

    pushd prebuilts/${CONFIG_XOS_PROJECT_TYPE}/bsp/rootfs
    rootfs_origin=$(realpath "origin.rootfs.cpio.xz")
    rootfs_dest=$(realpath "rootfs.cpio.$CONFIG_ROOTFS_FORMAT")
    popd

    echo "rootfs_origin=$rootfs_origin"
    echo "rootfs_dest=$rootfs_dest"
    echo "rootfs_overlay_path=$rootfs_overlay_path"

    if [[ "$CONFIG_ROOTFS_FORMAT" = "xz" ]]; then
        mk_cmd="find . -print0 | cpio --null -ov --format=newc | xz --check=crc32 -c -k --lzma2 > $rootfs_dest"
    elif [[ "$CONFIG_ROOTFS_FORMAT" = "lz4" ]]; then
        mk_cmd="find . -print0 | cpio --null -ov --format=newc | lz4 -l -c > $rootfs_dest"
    elif [[ "$CONFIG_ROOTFS_FORMAT" = "ubifs" ]]; then
        mk_cmd="find . -print0 | cpio --null -ov --format=newc > $rootfs_dest"
    else
        echo "Unsupported compressed format: $CONFIG_ROOTFS_FORMAT"
        exit 1
    fi

    tmppath=$(mktemp -d)
    pushd $tmppath
    fakeroot -u sh -c "
        xz -d -c $rootfs_origin | cpio -id
        rsync -aHAXr --exclude='overlay_for_precopy.sh' $rootfs_overlay_path/ $tmppath
        $mk_cmd
    "
    popd
    rm -rf $tmppath
}

function copy_app_to_rootfs()
{
    echo "copy_app_to_rootfs"
    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        cp_to_rootfs_dir=$_soc_pkg_path/$_soc_pkg_build_rootfs_dir
        if [ ! -d $cp_to_rootfs_dir ];then
            mkdir -p $cp_to_rootfs_dir
        fi
        if [ ! -d "$cp_to_rootfs_dir/usr/elf/" ]; then
            mkdir -p "$cp_to_rootfs_dir/usr/elf/"
        fi
        if [[ "$BUILD_NOQUI" != true ]]; then
            #cp or exit
            if [ -f "$_xos_img_out_dir_/bin/qxosui" ];then
                cp -arf $_xos_img_out_dir_/bin/qxosui "$cp_to_rootfs_dir/usr/elf/"
            else
                echo "copy_app_to_rootfs failed. no qxosui and exit."
                exit
            fi
        fi
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        cp_to_rootfs_dir=$_soc_pkg_path/output/rootfs_uclibc
        mkdir -p "$cp_to_rootfs_dir/usr/elf/"
        mkdir -p "$cp_to_rootfs_dir/usr/lib/quamm/"
        mkdir -p "$cp_to_rootfs_dir/res/"
        mkdir -p "$cp_to_rootfs_dir/res/ttf/"
        if [[ "$BUILD_NOQUI" != true ]]; then
            #cp or exit
            if [ -f "$_xos_img_out_dir_/bin/qxosui" ];then
                cp -arf $_xos_img_out_dir_/bin/qxosui "$cp_to_rootfs_dir/usr/elf/"
                ${_sdk_cross_compile}strip $cp_to_rootfs_dir/usr/elf/qxosui
            else
                echo "copy_app_to_rootfs failed. no qxosui and exit."
                exit
            fi
        fi

        echo "copy to $cp_to_rootfs_dir"
        cp -arf $_xos_img_out_dir_/lib/*.so* $cp_to_rootfs_dir/usr/lib/
        cp -arf $_xos_img_out_dir_/lib/quamm/*.so* $cp_to_rootfs_dir/usr/lib/quamm/
        make_strip $cp_to_rootfs_dir/usr/lib/quamm/
        make_strip $cp_to_rootfs_dir/usr/lib/
        make_strip $cp_to_rootfs_dir/usr/bin/
        cp -arf $_xos_img_out_dir_/res/ttf/*.ttf $cp_to_rootfs_dir/res/ttf/
        cp -r $_xos_img_out_dir_/res/skin/ $cp_to_rootfs_dir/res/
        if [ -d "$_xos_img_out_dir_/res/video/" ];then
            cp -r $_xos_img_out_dir_/res/video/ $cp_to_rootfs_dir/res/
        fi
        if [ -f "$_xos_img_out_dir_/res/settings.json" ];then
            cp $_xos_img_out_dir_/res/settings.json $cp_to_rootfs_dir/res/
        fi
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        cp_to_rootfs_dir=$_soc_pkg_path/board_support/rootfs/rootfs_pub
        if [ ! -d "$cp_to_rootfs_dir/usr/elf/" ]; then
            mkdir -p "$cp_to_rootfs_dir/usr/elf/"
        fi
        if [[ "$BUILD_NOQUI" != true ]]; then
            if [ -f "$_xos_img_out_dir_/bin/qxosui" ];then
                cp -arf $_xos_img_out_dir_/bin/qxosui "$cp_to_rootfs_dir/usr/elf/"
            else
                echo "copy_app_to_rootfs failed. no qxosui and exit."
                exit
            fi
        fi
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        cp_to_rootfs_dir=$_soc_pkg_path/$_soc_pkg_build_rootfs_dir
        OVERLAY_ROOTFS=$_top_board_conf_dir_/rootfs_overlay
        echo $OVERLAY_ROOTFS
        #$OVERLAY_ROOTFS/overlay_for_precopy.sh
        #rsync -aHAXr --exclude='overlay_for_precopy.sh' $OVERLAY_ROOTFS/ $cp_to_rootfs_dir/
        if [ ! -d "$cp_to_rootfs_dir/usr/elf/" ]; then
            mkdir -p "$cp_to_rootfs_dir/usr/elf/"
        fi
        if [[ "$BUILD_NOQUI" != true ]]; then
            #cp or exit
            if [ -f "$_xos_img_out_dir_/bin/qxosui" ];then
                cp -arf $_xos_img_out_dir_/bin/qxosui "$cp_to_rootfs_dir/usr/elf/"
            else
                echo "copy_app_to_rootfs failed. no qxosui and exit."
                exit
            fi
        fi
    fi
}

function build_pkg_envsetup()
{
    echo $(pwd): $CONFIG_XOS_PROJECT_TYPE
    if [[ "$BUILD_SDK" = "qm10xh_linux" ]] || \
       [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        ./envsetup.sh
    fi
    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        ./envsetup.sh
    fi
    if [[ "$BUILD_SDK" = "qm10xd_rtt" ]]; then
        # envsetup for rtt_dmb and rtt_nand(1:nand 2:nor)
        ./envsetup.sh 2 1
    fi
}

function fastbuild_zImage()
{
    echo "fastbuild_zImage"
    copy_output_to_out
    copy_app_to_rootfs

    #copy bin/* to sdk/out and package to zImage
    pushd $_soc_pkg_path
    build_pkg_envsetup
    make ramdisk || exit "$$?"
    make kernel_image || exit "$$?"

    mkdir -p $_soc_pkg_to_root_dir$_sdk_img_out_dir_
    cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    popd

    echo "fastbuild_zImage done"
}

function build_soc_pkg_rootfs
{
    echo "build_soc_pkg_rootfs"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]; then
        build_pkg_envsetup
        # make clean
        make rootfs || exit "$$?"
        # copy_etc_to_rootfs
	    qm10xh_make_rootfs
    fi

      # 新增对linuxrttv2版本的独立处理
    if [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        build_pkg_envsetup
        make rootfs || exit "$$?"
        qm10xhv2_make_rootfs  # 调用新增的V2专用函数
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        echo "make myboard=$_realboard clean"
	    echo "make myboard=$_realboard rootfs -j$BUILD_JOBS || exit "$?""
        make myboard=$_realboard clean
        make myboard=$_realboard rootfs -j$BUILD_JOBS || exit "$$?"
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
	    qm10xv_make_rootfs
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        build_pkg_envsetup
        # make clean
        make rootfs || exit "$$?"
        # copy_etc_to_rootfs
    fi

    # do this after rootfs unzip.
    popd
    echo "build_soc_pkg_rootfs done"
}

function build_soc_pkg_rootfs_post
{
    echo "build_soc_pkg_rootfs_post"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        if [[ -f "$_top_board_conf_dir_/rootfs_post_copy.sh" ]]; then
            #execute rootfs_post_copy.sh
            $_top_board_conf_dir_/rootfs_post_copy.sh || exit "$?"
        fi
        make_strip output/rootfs_uclibc/lib
        make_strip output/rootfs_uclibc/usr/lib
        make_strip output/rootfs_uclibc/bin

        OVERLAY_ROOTFS=$_top_board_conf_dir_/rootfs_overlay
        echo $OVERLAY_ROOTFS
        if [[ -f "$OVERLAY_ROOTFS/overlay_for_precopy.sh" ]]; then
            $OVERLAY_ROOTFS/overlay_for_precopy.sh || exit "$?"
        fi
        rsync -aHAXr --exclude='overlay_for_precopy.sh' $OVERLAY_ROOTFS/ output/rootfs_uclibc/
        ${_sdk_cross_compile}strip --strip-debug output/rootfs_uclibc/ko/*.ko
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        fakeroot mkfs.cramfs output/rootfs_uclibc/ $_soc_pkg_to_root_dir/$_sdk_img_out_dir_/ramdisk
    fi

#    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
#        pushd board_support/rootfs/rootfs_pub
#        fakeroot chown -R root:root .
#        find . | cpio -o -H newc > ../rootfs.cpio
#        cd ..
#        xz --check=none rootfs.cpio
#        cp -arf rootfs.cpio.xz ../kernel/rootfs_scripts/
#        popd
#    fi
    popd
    echo "build_soc_pkg_rootfs_post done"
}

function build_soc_pkg_uboot
{
    echo "build_soc_pkg_uboot"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        build_pkg_envsetup
        make boot_clean -j$BUILD_JOBS
        make boot -j$BUILD_JOBS || exit "$$?"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf output/$CONFIG_XOS_PROJECT_TYPE/bsp/boot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi
    
    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        make O=./build myboard=$_realboard boot_clean -j$BUILD_JOBS
        make O=./build myboard=$_realboard boot -j$BUILD_JOBS || exit "$$?"
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        pushd $_soc_bsp_uboot_path
        make O=.build clean -j$BUILD_JOBS
        make O=.build || exit "$$?"
        popd

        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/uboot/uboot/.build/uboot.bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/uboot/uboot/.build/uboot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        #copy uboot to flash_upgrade dir
        cp -arf $_soc_bsp_uboot_path/uboot/.build/u-boot.bin $create_xmodem_tools_dir
        #create xmodem.img
        pushd $create_xmodem_tools_dir
        ./mkflashimgv4 QM102V_normal.ini
        popd
        #copy xmodem.img to output dir
        cp -arf $create_xmodem_tools_dir/xmodem.img $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        build_pkg_envsetup
        make O=.build boot #Uboot 全编译
        make O=.build boot_build #Uboot&SDL 
        make O=.build ramboot #PDL&SPL
        echo "make boot -j$BUILD_JOBS done"
    fi

    if [[ "$BUILD_SDK" = "qm10xd_rtt" ]]; then
        build_pkg_envsetup
        make boot || exit "$$?"         #boot 全编译

        echo "make boot rtt -j$BUILD_JOBS done"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf output/$CONFIG_XOS_PROJECT_TYPE/bsp/boot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    popd
    echo "build_soc_pkg_uboot done"
}

function build_soc_pkg_kernel
{
    echo "build_soc_pkg_kernel"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        build_pkg_envsetup
        make O=.build kernel_clean -j$BUILD_JOBS
        make O=.build kernel || exit "$$?"
        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
        echo "$_soc_pkg_to_root_dir"
        echo "$_sdk_img_out_dir_"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf output/$CONFIG_XOS_PROJECT_TYPE/bsp/kernel $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        make O=./.build myboard=$_realboard kernel_clean -j$BUILD_JOBS
        make O=./.build myboard=$_realboard kernel -j$BUILD_JOBS || exit "$$?"
        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        pushd $_soc_bsp_kernel_path
        make O=.build clean -j$BUILD_JOBS
        make O=.build || exit "$$?"
        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
        popd

        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/kernel/linux-4.9/.build/arch/arm/boot/uImage $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/kernel/linux-4.9/.build/arch/arm/boot/Image $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        build_pkg_envsetup
        make O=.build kernel
        echo "$_soc_pkg_to_root_dir"
        echo "$_sdk_img_out_dir_"
        echo "make kernel -j$BUILD_JOBS done"
    fi

    if [[ "$BUILD_SDK" = "qm10xd_rtt" ]]; then
        build_pkg_envsetup
        make rtt  || exit "$$?"         #rtt  全编译

        echo "make boot rtt -j$BUILD_JOBS done"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        # cp -arf bsp/rt-thread/app/out/bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf bsp/rt-thread/app/quaxos/out/bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    popd
    echo "build_soc_pkg_kernel done"
}

function build_soc_pkg
{
    echo "build_soc_pkg"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]; then
        make O=.build boot_clean -j$BUILD_JOBS
        make O=.build boot || exit "$$?"
        #echo "make boot -j$BUILD_JOBS done"

        #make kernel_clean -j$BUILD_JOBS
        make O=.build kernel || exit "$$?"
       # echo "make kernel -j$BUILD_JOBS done"

        make post_build FB=0 || exit "$$?"
        echo "make post_build -j$BUILD_JOBS done"

        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        make O=.build boot_clean -j$BUILD_JOBS
        make O=.build boot || exit "$$?"
        #echo "make boot -j$BUILD_JOBS done"

        #make kernel_clean -j$BUILD_JOBS
        make O=.build kernel || exit "$$?"
       # echo "make kernel -j$BUILD_JOBS done"

        make rtt || exit "$$?"
        make post_build FB=1 || exit "$$?"
        echo "make post_build -j$BUILD_JOBS done"

        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi
    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        make O=.build myboard=$_realboard kernel_clean -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard kernel -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard ramdisk -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard kernel_image -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard boot_clean -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard boot -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard jffs2 -j$BUILD_JOBS || exit "$$?"
        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir/boot/* $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir/kernel/* $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        pushd $_soc_bsp_uboot_path
        make O=.build clean -j$BUILD_JOBS
        make O=.build || exit "$$?"
        popd
        pushd $_soc_bsp_kernel_path
        make O=.build clean -j$BUILD_JOBS
        make O=.build || exit "$$?"
        _kernel_obj_dest=$(realpath $(dirname $(find ./ -iname .config |grep -i linux)))
        popd
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/uboot/uboot/.build/u-boot.bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/uboot/uboot/.build/u-boot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

        #copy uboot to flash_upgrade dir
        cp -arf $_soc_bsp_uboot_path/uboot/.build/u-boot.bin $create_xmodem_tools_dir
        #create xmodem.img
        pushd $create_xmodem_tools_dir
        ./mkflashimgv4 QM102V_normal.ini
        popd
        #copy xmodem.img to output dir
        cp -arf $create_xmodem_tools_dir/xmodem.img $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

        cp -arf board_support/kernel/linux-4.9/.build/arch/arm/boot/uImage $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf board_support/kernel/linux-4.9/.build/arch/arm/boot/Image $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        #make boot || exit "$$?"         #Uboot 全编译
        #make boot_build || exit "$$?"   #Uboot&SDL
        #make ramboot || exit "$$?"      #PDL&SPL
        #echo "make boot -j$BUILD_JOBS done"

        OVERLAY_ROOTFS=$_top_board_conf_dir_/rootfs_overlay
        echo $(pwd)
        $OVERLAY_ROOTFS/overlay_for_precopy.sh
	    SRC_FILE_ROOTFS=${PRJ_TOP_DIR}/$_soc_pkg_path/prebuilts/${CONFIG_XOS_PROJECT_TYPE}/bsp/rootfs
        pushd $SRC_FILE_ROOTFS
        rootfs_origin=$(realpath "origin-initramfs-v510.cpio.xz")
        rootfs_overlay=$(realpath "$OVERLAY_ROOTFS")
        rootfs_dest=$(realpath "initramfs-v510.cpio.xz")
        xz_overlay "$rootfs_origin" "$rootfs_overlay" "$rootfs_dest"
        popd
        #make O=.build clean -j$BUILD_JOBS
        #make O=.build || exit "$$?"
        make O=.build kernel || exit "$$?"
        echo "$_soc_pkg_to_root_dir"
        echo "$_sdk_img_out_dir_"
        echo "make kernel -j$BUILD_JOBS done"
	    #echo "majiwei test create $_soc_pkg_to_root_dir/$_sdk_img_out_dir_ dir"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        #cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp $_soc_pkg_build_bsp_dir/kernel/zImage-dtb $_soc_pkg_to_root_dir/$_sdk_img_out_dir_/
        #修改bootcmd里的kernel size，然后再编译uboot
	    linuxkernelfilesize=$(stat -c%s $_soc_pkg_build_bsp_dir/kernel/zImage-dtb)
        linuxkernelhex_size=$(printf "0x%x" "$linuxkernelfilesize")
        echo "kernel file hex size:$linuxkernelhex_size"
        # 导出 linuxkernelhex_size 环境变量
        export linuxkernelhex_size
        echo "bootcmd env linux kernel size replace as $linuxkernelhex_size"

        # 新增recovery内核尺寸处理
	    recoverykernelfilesize=$(stat -c%s $_soc_pkg_prebuilts_recovery_dir/recovery.img)
        recoverykernelhex_size=$(printf "0x%x" "$recoverykernelfilesize")
        echo "recovery kernel file hex size:$recoverykernelhex_size"
        # 导出 recoverykernelhex_size 环境变量
        export recoverykernelhex_size
        echo "bootcmd env recovery kernel size replace as $recoverykernelhex_size"

	    make O=.build boot || exit "$$?"         #Uboot 全编译
        make O=.build boot_build || exit "$$?"   #Uboot&SDL
        make O=.build ramboot || exit "$$?"      #PDL&SPL
        echo "make boot -j$BUILD_JOBS done"
	    cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

    fi

    if [[ "$BUILD_SDK" = "qm10xv_rtt" ]]; then
        local _soc_bsp_uboot_path=bsp/uboot-2016
        pushd $_soc_bsp_uboot_path
        make O=.build distclean -j$BUILD_JOBS
        make O=.build || exit "$$?"
        popd
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_bsp_uboot_path/uboot/.build/u-boot.bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_bsp_uboot_path/uboot/.build/u-boot $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

        local _soc_bsp_kernel_path=rt-thread
        pushd $_soc_bsp_kernel_path
        make clean
        make qm10xv_defconfig || exit "$$?"
        #make appconfig || exit "$$?"
        make || exit "$$?"
        popd
        cp -arf $_soc_bsp_kernel_path/app/helloworld/out/bin/*.bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_bsp_kernel_path/app/media_demo/out/bin/*.bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

        #copy uboot to flash_upgrade dir
        local _create_xmodem_tools_dir=$_soc_pkg_build_board_dir/mkflashimg
        mkdir -p $_create_xmodem_tools_dir
        cp -arf docs_tools/software/pc/flashimg-tool/QM10XV/* $_create_xmodem_tools_dir
        cp -arf docs_tools/software/pc/flashimg-tool/mkflashimgv4 $_create_xmodem_tools_dir
        cp -arf $_soc_bsp_uboot_path/uboot/u-boot.bin $_create_xmodem_tools_dir
        cp -arf $_soc_bsp_kernel_path/app/media_demo/out/bin/media_demo.img $_create_xmodem_tools_dir/app.img
        #create xmodem.img
        pushd $_create_xmodem_tools_dir
        ./mkflashimgv4 QM102V_normal.ini
        popd
        #copy xmodem.img to output dir
        cp -arf $_create_xmodem_tools_dir/xmodem.img $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_create_xmodem_tools_dir/Flash.img $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    if [[ "$BUILD_SDK" = "qm10xd_rtt" ]]; then
        build_pkg_envsetup
        make boot || exit "$$?"         #boot 全编译
        make rtt  || exit "$$?"         #rtt  全编译

        echo "make boot rtt -j$BUILD_JOBS done"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        # cp -arf bsp/rt-thread/app/out/bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf bsp/rt-thread/app/quaxos/out/bin $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    popd
    echo "build_soc_pkg done"
}

function build_backup_xos4dbg()
{
    echo "build_backup_xos4dbg"
    # 检查文件是否存在
    if [ -f "output/qxosui.elf" ]; then
        # 如果文件存在，执行拷贝操作
        mkdir -p out/$BUILD_SDK/qmimages/backup_xos4dbg/
        cp output/qxosui.elf "out/$BUILD_SDK/qmimages/backup_xos4dbg/"
        echo "File output/qxosui.elf copied to out/$BUILD_SDK/qmimages/backup_xos4dbg"
        # add more backup copy files here
        tar -czvf out/$BUILD_SDK/qmimages/backup_xos4dbg.tar.gz out/$BUILD_SDK/qmimages/backup_xos4dbg
        rm -rf out/$BUILD_SDK/qmimages/backup_xos4dbg
    fi
    echo "build_backup_xos4dbg end"
}

function build_misc()
{
    echo "build "$_sdk_img_out_dir_/misc.img
    ./build/script/mk-misc.sh $_sdk_img_out_dir_/misc.img
}

function build_firmware(){
    echo " build_firmware"

    #build_backup_xos4dbg
    # TODO
    # build_strip_all

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        pushd $XOS_SCRIPT_DIR
        source ./mkfirmware.sh
        local __err=$?
        popd
    fi

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        pushd $XOS_SCRIPT_DIR
        source ./mkfirmware.sh
        local __err=$?
        popd
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        pushd $_soc_pkg_path
        cp -arf $_soc_pkg_build_bsp_dir/boot/* $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir/kernel/* $_soc_pkg_to_root_dir/$_sdk_img_out_dir_

        if [[ "$CONFIG_XOS_USE_FW_STORE_SPINAND" = true ]]; then
            echo "copy spi nand 128M script.ini"
            cp -arf $_soc_pkg_to_root_dir/tools/partition/mc331x/los_spi-nand/64M/script.ini $_soc_pkg_to_root_dir/$_sdk_img_out_dir_/
        else
            echo "copy spi nor 16M script.ini"
            cp -arf $_soc_pkg_to_root_dir/tools/partition/mc331x/los_spi-nor/16M/script.ini $_soc_pkg_to_root_dir/$_sdk_img_out_dir_/
        fi
        popd
    fi

    #if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
    if [[ "$BUILD_SDK" = "qm10xd_linux" || "$BUILD_SDK" = "qm10xd_rtt" ]]; then
        pushd $XOS_SCRIPT_DIR
        source ./mkfirmware.sh
        local __err=$?
        popd
    fi

    finish_build
    echo " build_firmware done"
}

function build_clean()
{
    echo "build_clean"

    rm -rf out/

    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        make  boot_clean -j$BUILD_JOBS
        make  kernel_clean -j$BUILD_JOBS
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        make O=.build myboard=$_realboard kernel_clean -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard boot_clean -j$BUILD_JOBS || exit "$$?"
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        pushd $_soc_bsp_uboot_path
        make O=.build clean -j$BUILD_JOBS
        popd
	    pushd $_soc_bsp_kernel_path
        make O=.build clean -j$BUILD_JOBS
	    popd
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        make O=.build boot_clean -j$BUILD_JOBS
        make O=.build kernel_clean -j$BUILD_JOBS
    fi

    popd
    echo "build_clean done"
}

function build_drv()
{
    echo "Start to build_drv"

    _kernel_obj_dest=$(realpath $(dirname $(find $_soc_pkg_path/bsp/kernel -iname .config)))
    echo KERN_OBJ:$_kernel_obj_dest
    export drv_kernel_obj_dest=$(realpath $_kernel_obj_dest)
    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]; then
        drv_sdk_cross_compile=arm-fhv512-linux-uclibcgnueabihf
    fi  
	if [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        drv_sdk_cross_compile=arm-fhva12c-linux-uclibcgnueabihf
    fi
    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        drv_sdk_cross_compile=arm-fhv512-linux-uclibcgnueabihf
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        drv_sdk_cross_compile=arm-mol-linux-uclibcgnueabihf
    fi

    if [[ "$BUILD_SDK" = "rtt" ]]; then
        #drv_sdk_cross_compile=arm-linux-musleabi
        exit
    fi
    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        drv_sdk_cross_compile=arm-molv2-linux-uclibcgnueabi
    fi

    export drv_sdk_cross_compile

    echo $drv_sdk_cross_compile
    echo $drv_kernel_obj_dest
    pushd $drv_path
    #make clean && make $ARGS|| exit "$?"
    make clean && make || exit "$?"

    if [ ! -d "out" ]; then
        echo "out does not exist."
        exit 1
    fi

    if [ ! -d $_soc_pkg_to_root_dir/output/ko ]; then
        mkdir -p $_soc_pkg_to_root_dir/output/ko
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        cp -rvf out/lib/*  $_soc_pkg_to_root_dir/$_soc_pkg_path/output/rootfs_uclibc/lib
        cp -rvf out/ko/*   $_soc_pkg_to_root_dir/$_soc_pkg_path/output/rootfs_uclibc/ko
        #cp -rvf out/lib/*  $_soc_pkg_to_root_dir/$_soc_pkg_path/output/lib
        #cp -rvf out/ko/*   $_soc_pkg_to_root_dir/$_soc_pkg_path/output/ko
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        cp -rvf out/lib/*  $_soc_pkg_to_root_dir/output/lib
        cp -rvf out/ko/*   $_soc_pkg_to_root_dir/output/ko
    fi

    case "$BUILD_SDK" in
    "qm10xv_linux")
    target_dir="../../libnative/qlibquadrv/qm10xv_linux"
    ;;
    "qm10xd_linux")
    target_dir="../../libnative/qlibquadrv/qm10xd_linux"
    ;;
    "qm10xh_linux")
    target_dir="../../libnative/qlibquadrv/qm10xh_linux"
    ;;
	
    "qm10xh_linuxrttv2")
    target_dir="../../libnative/qlibquadrv/qm10xh_linux"
    ;;
	
    "linux_128"|"linux_64")
    target_dir="../../libnative/qlibquadrv/fy00_linux"
    ;;
    *)
    target_dir="../../libnative/qlibquadrv/fy00_rtt"
    ;;
    esac

    cp -rvf out/lib/* "$target_dir"

    popd
    echo "End to build_drv"
}

function build_distclean()
{
    echo "build_distclean"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]  || [[ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]]; then
        make O=.build distclean -j$BUILD_JOBS
    fi

    if [[ "$BUILD_SDK" = "linux_128" ]] || [[ "$BUILD_SDK" = "linux_64" ]]; then
        make O=.build myboard=$_realboard kernel_clean -j$BUILD_JOBS || exit "$$?"
        make O=.build myboard=$_realboard boot_clean -j$BUILD_JOBS || exit "$$?"
    fi

    if [[ "$BUILD_SDK" = "qm10xv_linux" ]]; then
        pushd $_soc_bsp_uboot_path
        make O=.build distclean -j$BUILD_JOBS
        popd
	    pushd $_soc_bsp_kernel_path
        make O=.build distclean -j$BUILD_JOBS
	    popd
    fi

    if [[ "$BUILD_SDK" = "qm10xd_linux" ]]; then
        make distclean -j$BUILD_JOBS
    fi

    popd
    echo "build_distclean done"
}

function build_all()
{
    echo "build_all"
    local __start_time __end_time __exec_time
    __start_time=$(date +%s)
    echo "BUILD_SDK=$BUILD_SDK"

   # 处理 qm10xv_rtt 的特殊情况
    if [[ "$BUILD_SDK" == "qm10xv_rtt" ]]; then
        build_soc_pkg

    # 处理 qm10xd_rtt 的情况
    elif [[ "$BUILD_SDK" == "qm10xd_rtt" ]]; then
        build_soc_pkg
        build_firmware  # 新增固件构建步骤

    # 处理非 qm10xv_rtt 的通用情况
    else
        # 核心构建步骤 (所有非 qm10xv_rtt 情况都执行)
        build_soc_pkg_rootfs
        copy_output_to_out

        # 特殊处理非 Linux 变体的情况
        if [[ "$BUILD_SDK" != "qm10xv_linux" && \
              "$BUILD_SDK" != "qm10xd_linux" ]]; then
            copy_app_to_rootfs
            build_soc_pkg_rootfs_post
        fi

        # 公共构建步骤
        build_soc_pkg
        build_misc
        build_firmware
    fi

    echo "End to build linux xos!"
    __end_time=$(date +%s)
    __exec_time=$((__end_time - __start_time))
    echo "build all executed time: $__exec_time seconds"
    exit
}

function postbuild_rttap()
{
    echo "postbuild_rttap"
    pushd $_soc_pkg_path

    if [[ "$BUILD_SDK" = "qm10xh_linux" ]]; then
        echo "make post_build FB=1 RTT_AP=1"
        make post_build FB=1 RTT_AP=1 || exit "$$?"

        echo "_sdk_img_out_dir_=$_sdk_img_out_dir_"
        mkdir -p $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
        cp -arf $_soc_pkg_build_bsp_dir $_soc_pkg_to_root_dir/$_sdk_img_out_dir_
    fi

    popd
    echo "postbuild_rttap done"
}

if [[ "$BUILD_DISTCLEAN" = true ]]; then
    echo "build distclean"
    build_distclean
    exit
fi

if [[ "$BUILD_CLEAN" = true ]]; then
    echo "build clean"
    build_clean
    exit
fi

if [[ "$BUILD_ALL" = true ]]; then
    echo "build all"
    build_all
fi

if [[ "$BUILD_UBOOT" = true ]]; then
    echo "build soc pkg uboot"
    build_soc_pkg_uboot
    exit
fi

if [[ "$BUILD_KERNEL" = true ]]; then
    echo "build soc pkg kernel"
    build_soc_pkg_rootfs
    build_soc_pkg_kernel
    exit
fi

if [[ "$BUILD_QUADRV" = true ]]; then
    echo "build quadrv"
    build_soc_pkg_rootfs
    build_soc_pkg_kernel
    build_drv
    exit
fi

if [[ "$BUILD_PACKAGES" = true ]]; then
    echo "build packages"
    fastbuild_zImage
    exit
fi

if [[ "$BUILD_FW_IMG" = true ]]; then
    echo "build fireware images"
    build_firmware
    exit
fi

if [[ "$BUILD_RTOS_POSTBUILD" = true ]]; then
    echo "build uboot"
    build_soc_pkg_uboot
    echo "post build rtt-ap"
    postbuild_rttap
    exit
fi

echo "build all default. BUILD_PACKAGES=$BUILD_PACKAGES"

build_all
echo -e "\e[36m build_all: ready \e[0m"
