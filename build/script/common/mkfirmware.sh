#!/bin/bash

set -e

MKIMG_TGT_OUT=$PRJ_TOP_DIR/out/$_top_img_out_dir_
export QM_ALL_IMAGES=$MKIMG_TGT_OUT/qmimages
export MKIMG_PATH_ROOT=$MKIMG_TGT_OUT/mkimg_part
MKIMG_PATH_IMGOUT=$MKIMG_PATH_ROOT/_imgout_
MKIMG_PATH_CURR=""

#default fs type
QM_SYS_DEF_FS_TYPE=${CONFIG_QM_SYS_PARTI_DEFAULT_FS_TYPE}

function finish_build(){
	echo "Running ${FUNCNAME[1]} succeeded"
	cd $PRJ_TOP_DIR
}

function build_genMkimgdir(){
	echo " build_genMkimgdir"
	echo "BoardConfig CONFIG_QM_SYS_PARTI_NAME_LIST：${CONFIG_QM_SYS_PARTI_NAME_LIST}"
	echo "BoardConfig CONFIG_QM_SYS_PARTI_INFOFILE：${CONFIG_QM_SYS_PARTI_INFOFILE}"
	if [ -n "${CONFIG_QM_SYS_PARTI_NAME_LIST}" ]; then
		if [ -d "$MKIMG_TGT_OUT" ]; then
			rm -rf $MKIMG_PATH_ROOT
			mkdir -p $MKIMG_PATH_ROOT
			mkdir -p $QM_ALL_IMAGES
			mkdir -p $MKIMG_PATH_IMGOUT
			pushd $MKIMG_PATH_ROOT
			for partition in ${CONFIG_QM_SYS_PARTI_NAME_LIST}; do
				mkdir -p "${MKIMG_PATH_ROOT}/${partition}"
				#echo "mkdir: ${partition}"
			done
			cp -rf ${_top_board_conf_dir_}/${CONFIG_QM_SYS_PARTI_INFOFILE} ${MKIMG_PATH_IMGOUT}/${CONFIG_QM_SYS_PARTI_INFOFILE}
			popd
		else
			echo -e "Failed: MKIMG_TGT_OUT:$MKIMG_TGT_OUT is not exist"
			exit -1
		fi
	else
		echo -e "Failed: CONFIG_QM_SYS_PARTI_NAME_LIST:${CONFIG_QM_SYS_PARTI_NAME_LIST} is empty, please check"
		exit -1
	fi

	finish_build
}


function make_strip() {
    local dir_path="$1"
    if [ -d "$dir_path" ]; then
        #find "$dir_path" -type f -exec ${_sdk_cross_compile}strip -v  --strip-debug {} \;
	find "$dir_path" -type f -not -name "*.ko" -exec ${_sdk_cross_compile}strip  {} \;
    else
        return 1
    fi
}

function build_backup_xos4dbg()
{
    echo "build_backup_xos4dbg"

    # 检查目录是否存在
    if [ -d "$MKIMG_PATH_ROOT" ]; then

		# mkdir backup_xos4dbg
	    mkdir -p $QM_ALL_IMAGES/backup_xos4dbg/

		# add more backup copy files here
		# backup system
		if [ -d "$MKIMG_PATH_ROOT/system" ]; then
			echo "backup unstrip $MKIMG_PATH_ROOT/system"
			cp -arf $MKIMG_PATH_ROOT/system "$QM_ALL_IMAGES/backup_xos4dbg/"
		fi

		# backup rootfs
		if [ -d "$MKIMG_PATH_ROOT/rootfs" ]; then
			echo "backup unstrip $MKIMG_PATH_ROOT/rootfs"
			cp -arf $MKIMG_PATH_ROOT/rootfs "$QM_ALL_IMAGES/backup_xos4dbg/"
		fi

		# backup data
		if [ -d "$MKIMG_PATH_ROOT/data" ]; then
			echo "backup unstrip $MKIMG_PATH_ROOT/data"
			cp -arf $MKIMG_PATH_ROOT/data "$QM_ALL_IMAGES/backup_xos4dbg/"
		fi

		# backup qua
		if [ -d "$MKIMG_PATH_ROOT/qua" ]; then
			echo "backup unstrip $MKIMG_PATH_ROOT/qua"
			cp -arf $MKIMG_PATH_ROOT/qua "$QM_ALL_IMAGES/backup_xos4dbg/"
		fi
		# pack backup_xos4dbg.tar.gz
		if [ -d "$QM_ALL_IMAGES/backup_xos4dbg" ]; then
			pushd $QM_ALL_IMAGES
			echo "tar -czf backup_xos4dbg.tar.gz"
			tar -czf backup_xos4dbg.tar.gz backup_xos4dbg
			rm -rf backup_xos4dbg
			popd
		fi
    fi

    echo "build_backup_xos4dbg end"
}

function build_strip_all()
{
    echo "build_strip_all"
    # 检查目录是否存在
    if [ -d "$MKIMG_PATH_ROOT" ]; then
		# product-esl
		if [ -d "$MKIMG_PATH_ROOT/system/qua/lib" ]; then
			echo "strip $MKIMG_PATH_ROOT/system/qua/lib"
			make_strip	$MKIMG_PATH_ROOT/system/qua/lib/
		fi
		if [ -d "$MKIMG_PATH_ROOT/system/qua/lib/quamm" ]; then
			echo "strip $MKIMG_PATH_ROOT/system/qua/lib/quamm"
			make_strip	$MKIMG_PATH_ROOT/system/qua/lib/quamm/
		fi
		if [ -d "$MKIMG_PATH_ROOT/system/app" ]; then
			echo "strip $MKIMG_PATH_ROOT/system/app"
			make_strip	$MKIMG_PATH_ROOT/system/app/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/bin" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/bin"
			make_strip	$MKIMG_PATH_ROOT/rootfs/bin/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/sbin" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/sbin"
			make_strip	$MKIMG_PATH_ROOT/rootfs/sbin/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/lib" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/lib"
			make_strip	$MKIMG_PATH_ROOT/rootfs/lib/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/usr/lib" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/usr/lib"
			make_strip	$MKIMG_PATH_ROOT/rootfs/usr/lib/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/usr/lib/quamm" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/usr/lib/quamm"
			make_strip	$MKIMG_PATH_ROOT/rootfs/usr/lib/quamm
		fi

		if [ -d "$MKIMG_PATH_ROOT/rootfs/vendor/qua/lib" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/vendor/qua/lib"
			make_strip	$MKIMG_PATH_ROOT/rootfs/vendor/qua/lib
		fi

		if [ -d "$MKIMG_PATH_ROOT/rootfs/vendor/qua/lib/quamm" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/vendor/qua/lib/quamm"
			make_strip	$MKIMG_PATH_ROOT/rootfs/vendor/qua/lib/quamm
		fi

		if [ -d "$MKIMG_PATH_ROOT/rootfs/system/qua/lib" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/system/qua/lib"
			make_strip	$MKIMG_PATH_ROOT/rootfs/system/qua/lib/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/system/qua/lib/quamm" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/system/qua/lib/quamm"
			make_strip	$MKIMG_PATH_ROOT/rootfs/system/qua/lib/quamm/
		fi
		if [ -d "$MKIMG_PATH_ROOT/rootfs/system/app" ]; then
			echo "strip $MKIMG_PATH_ROOT/rootfs/system/app"
			make_strip	$MKIMG_PATH_ROOT/rootfs/system/app/
		fi

		# other product-aicamera/kidc/ipc...
		if [ -f "$MKIMG_PATH_ROOT/data/qxosui" ]; then
			echo "strip $MKIMG_PATH_ROOT/data/qxosui"
			${_sdk_cross_compile}strip $MKIMG_PATH_ROOT/data/qxosui
		fi
		if [ -d "$MKIMG_PATH_ROOT/qua/lib" ]; then
			echo "strip $MKIMG_PATH_ROOT/qua/lib"
			make_strip	$MKIMG_PATH_ROOT/qua/lib/
		fi
		if [ -d "$MKIMG_PATH_ROOT/qua/lib/quamm" ]; then
			echo "strip $MKIMG_PATH_ROOT/qua/lib/quamm"
			make_strip	$MKIMG_PATH_ROOT/qua/lib/quamm/
		fi
		if [ -d "$MKIMG_PATH_ROOT/system/qua/wifi/host" ]; then
			echo "strip $MKIMG_PATH_ROOT/system/qua/wifi/host"
			make_strip      $MKIMG_PATH_ROOT/system/qua/wifi/host/
		fi


    fi
    echo "build_strip_all end"
}

build_genMkimgdir


if [ $MKIMG_PATH_ROOT ]
then
	QM_PARAMETER=${CONFIG_QM_SYS_PARTI_INFOFILE}
else
	echo -e "CONFIG_QM_SYS_PARTI_INFOFILE:${CONFIG_QM_SYS_PARTI_INFOFILE} is not exist"
	exit -1
fi

PARAMETER=$MKIMG_PATH_ROOT/$QM_PARAMETER
echo "MKIMG_PATH_IMGOUT:$MKIMG_PATH_IMGOUT"


if which fakeroot; then
FAKEROOT_TOOL="`which fakeroot`"
else
	echo -e "Install fakeroot First"
	echo -e "  sudo apt-get install fakeroot"
	exit -1
fi

QUA_FAKEROOT_SCRIPT=$MKIMG_PATH_IMGOUT/parti.fs
MKIMAGE=$XOS_SCRIPT_DIR/mk-image.sh

echo ""
echo ""
#copy files to each partition
echo "QM: copy images before mkimg, call ${_top_board_conf_dir_}/pre_mkimg_copy_files.sh"
if [ -n "${CONFIG_QM_SYS_PARTI_NAME_LIST}" ]; then
	if [ -f "${_top_board_conf_dir_}/post_mkimg_copy_files.sh" ]; then
		source ${_top_board_conf_dir_}/pre_mkimg_copy_files.sh
	else
		echo -e "warning: ${_top_board_conf_dir_}/pre_mkimg_copy_files.sh is not exist"
		exit -1
	fi
else
	echo -e "warning: ${CONFIG_QM_SYS_PARTI_NAME_LIST} is empty"
	exit -1
fi
#end of copy files

#backup xos for debug(unstrip lib/app)
build_backup_xos4dbg

#do strip all so/bin/ko (except mpp ko)
build_strip_all

# SIZE list for each every image
# create or clean code-size.txt
echo "code-size:${QM_ALL_IMAGES}/code-size.txt"
> ${QM_ALL_IMAGES}/code-size.txt
CODE_SIZE_MB=$(du -sh "$MKIMG_TGT_OUT/image/bsp/kernel/zImage" | awk '{print $1}')
echo "kernel: ${CODE_SIZE_MB} Mbyte" >> ${QM_ALL_IMAGES}/code-size.txt
do_code_size() {
    pushd $MKIMG_PATH_ROOT

    # 获取目标目录的大小（以 1K 块为单位）
    CODE_SIZE_KB=$(du -s ${QM_PARTI_NAME} | cut -f1)

    # 将目录大小转换为 MB 并向上取整
    CODE_SIZE_MB=$(( (CODE_SIZE_KB + 1023) / 1024 ))

    # 计算 20% 的元数据空间，并向上取整
    METADATA_SIZE_MB=$(( (CODE_SIZE_MB + 9) / 10 ))

    # 确保元数据空间最小为 10MB
    if [ $METADATA_SIZE_MB -lt 10 ]; then
	    METADATA_SIZE_MB=$((METADATA_SIZE_MB + 10))
    fi

    # 设置扩展文件系统的大小（增加 10% 的空间用于文件系统元数据）
    EXT4_SIZE_MB=$(( CODE_SIZE_MB + METADATA_SIZE_MB ))

    # 比较 EXT4_SIZE_MB 和 QM_PARTI_SIZE，取较小值
    if [ $EXT4_SIZE_MB -gt $QM_PARTI_SIZE ]; then
        EXT4_SIZE_MB=$QM_PARTI_SIZE
    fi

    echo "EXT4_SIZE_MB=$EXT4_SIZE_MB"
    echo "METADATA_SIZE_MB=$METADATA_SIZE_MB"

    # write partition name and code size into code-size.txt
    echo "${QM_PARTI_NAME}: ${CODE_SIZE_MB} Mbyte" >> ${QM_ALL_IMAGES}/code-size.txt
    popd
}

echo ""
echo ""
do_mkimg() {
	echo "Creating image for partition: $QM_PARTI_NAME"
    echo "File system type: $QM_PARTI_FSTYPE"
    echo "Partition size: $QM_PARTI_SIZE"
    echo "Partition auto: $QM_PARTI_AUTO"
    echo "code size: $EXT4_SIZE_MB Mbyte"

	MKIMG_PATH_CURR=${MKIMG_PATH_ROOT}/${QM_PARTI_NAME}
	echo "mkimg path: $MKIMG_PATH_CURR!"
	if [ $MKIMG_PATH_CURR ]
	then
		if [ -d "$MKIMG_PATH_CURR" ]
		then
			echo "#!/bin/sh" > $QUA_FAKEROOT_SCRIPT
			echo "set -e" >> $QUA_FAKEROOT_SCRIPT

			echo "$MKIMAGE $MKIMG_PATH_CURR $MKIMG_PATH_IMGOUT/$QM_PARTI_NAME.$QM_PARTI_FSTYPE $QM_PARTI_FSTYPE $QM_PARTI_SIZE $QM_PARTI_NAME"  >> $QUA_FAKEROOT_SCRIPT
			chmod a+x $QUA_FAKEROOT_SCRIPT

			$FAKEROOT_TOOL -- $QUA_FAKEROOT_SCRIPT
			rm -f $QUA_FAKEROOT_SCRIPT
			if which img2simg; then
				IMG2SIMG_TOOL="`which img2simg`"
			else
				echo -e "Install img2simg First"
				echo -e "  sudo apt-get install img2simg"
				exit -1
			fi
			$IMG2SIMG_TOOL $MKIMG_PATH_IMGOUT/$QM_PARTI_NAME.$QM_PARTI_FSTYPE $MKIMG_PATH_IMGOUT/$QM_PARTI_NAME.img
		else
			echo "warning: $MKIMG_PATH_CURR not found!"
		fi
	fi
}

QM_PARTI_NAME=""
QM_PARTI_FSTYPE=""
QM_PARTI_SIZE=""
QM_PARTI_AUTO=""
EXT4_SIZE_MB=""
while IFS=',' read -r name fs size
do
    [[ "$line" == \#* ]] && continue

    if [[ "$name" == "::" ]]; then
        break
	fi

    QM_PARTI_NAME="$name"
    QM_PARTI_FSTYPE="$fs"
    if [[ "$size" == *"+"* ]]; then
        QM_PARTI_SIZE="${size%+*}"
        QM_PARTI_AUTO="1"
    else
        QM_PARTI_SIZE="$size"
    fi

    echo "QM_PARTI_NAME=$QM_PARTI_NAME"
    echo "QM_PARTI_FSTYPE=$QM_PARTI_FSTYPE"
    echo "QM_PARTI_SIZE=$QM_PARTI_SIZE"
    echo "QM_PARTI_AUTO=$QM_PARTI_AUTO"
    echo ""
	
    if [[ "$QM_PARTI_FSTYPE" != "raw" ]]; then
	do_code_size
        do_mkimg
    fi

done < $MKIMG_PATH_IMGOUT/${CONFIG_QM_SYS_PARTI_INFOFILE}

echo -e "\e[36m Image: image in $QM_ALL_IMAGES is ready \e[0m"

echo ""
echo ""
#copy files to $QM_ALL_IMAGES
echo "QM: call ${_top_board_conf_dir_}/post_mkimg_copy_files.sh to copy binary files"
if [ -f "${_top_board_conf_dir_}/post_mkimg_copy_files.sh" ]; then
	source ${_top_board_conf_dir_}/post_mkimg_copy_files.sh
else
	echo -e "warning: ${_top_board_conf_dir_}/post_mkimg_copy_files.sh is not exist"
	exit -1
fi
#end of copy files

echo "mv images to qmimages end"
