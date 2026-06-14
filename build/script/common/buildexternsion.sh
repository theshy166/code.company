#!/bin/bash

export LC_ALL=C
export LD_LIBRARY_PATH=
unset QUA_CFG_TOOLCHAIN

function choose_target_product()
{
	echo
	echo "You're building on Linux"
	echo "Lunch menu...pick a combo:"
	echo ""

	echo "0. common/project_default_conf.mk"
	echo ${QUA_TARGET_PRODUCT_ARRAY[@]} | xargs -n 1 | sed "=" | sed "N;s/\n/. /"

	local INDEX
	read -p "Which would you like? [0]: " INDEX
	INDEX=$((${INDEX:-0} - 1))

	if echo $INDEX | grep -vq [^0-9]; then
		QUA_BUILD_TARGET_BOARD="${QUA_TARGET_PRODUCT_ARRAY[$INDEX]}"
	else
		echo "Lunching for default boards..."
		QUA_BUILD_TARGET_BOARD=common/project_default_conf.mk
	fi
}

function build_select_product()
{
	QUA_TARGET_PRODUCT_ARRAY=( $(find product -maxdepth 0 -type d -exec find {} -name "project_*_conf.mk" \; | sed 's|^product/||' | sort) )

	QUA_TARGET_PRODUCT_ARRAY_LEN=${#QUA_TARGET_PRODUCT_ARRAY[@]}
	if [ $QUA_TARGET_PRODUCT_ARRAY_LEN -eq 0 ]; then
		echo "No available Board Config"
		return
	fi

	choose_target_product

	ln -rfs product/$QUA_BUILD_TARGET_BOARD product/.BoardConfig.mk
	echo "switching to board: `realpath $BOARD_CONFIG`"
}

function unset_board_config_all()
{
	local tmp_file=".mktemp"
	grep -oh "^export.*CONFIG_.*=" `find product -name "project_*_conf.mk"` | sed 's/^export/unset/;s/=/ /' > $tmp_file
	source $tmp_file
	rm -rf $tmp_file
}


CMD=`realpath $0`
COMMON_DIR=`dirname $CMD`
TOP_DIR=$(realpath $COMMON_DIR/../..)
cd $TOP_DIR

BOARD_CONFIG=$TOP_DIR/product/.BoardConfig.mk
TARGET_PRODUCT="$TOP_DIR/product/.target_product"
TARGET_PRODUCT_DIR=$(realpath ${TARGET_PRODUCT})

#if [ ! -L "$BOARD_CONFIG" -a  "$1" != "lunch" ]; then
#	build_select_product
#fi
unset_board_config_all
[ -L "$BOARD_CONFIG" ] && source $BOARD_CONFIG


OPTIONS="${@:-help}"

#QUA_TARGET_PRODUCT=$(echo $1 | cut -d'/' -f1)
#QUA_BOARD_PRE_BUILD_SCRIPT=$(echo $motox_proje_mk | cut -d'/' -f2)
#echo "QUA_TARGET_PRODUCT=$QUA_TARGET_PRODUCT"
#echo "QUA_BOARD_PRE_BUILD_SCRIPT=$QUA_BOARD_PRE_BUILD_SCRIPT"

function usage()
{
	echo "Usage: build.sh [OPTIONS]"
	echo "Available options:"
	echo "BoardConfig*.mk    -switch to specified board config"
	echo "lunch              -list current SDK boards and switch to specified board config"
	echo "uboot              -build uboot"
	echo "loader             -build loader"
	echo "kernel             -build kernel"
	echo "modules            -build kernel modules"
	echo "rootfs             -build default rootfs, currently build buildroot as default"
	echo "all                -build uboot, kernel, rootfs, recovery image"
	echo "cleanall           -clean uboot, kernel, rootfs, recovery"
	echo "check              -check the environment of building"
	echo "info               -see the current board building information"
	echo "app/<pkg>          -build packages in the dir of app/*"
	echo "help               -list help info"
	echo ""
	echo "Default option is 'help'."
}

function build_info(){
	echo "Current Building Information:"
	echo "Target Product: $TARGET_PRODUCT_DIR"
	echo "Target BoardConfig: `realpath $BOARD_CONFIG`"
	echo "`env |grep "^CONFIG_" | grep -v "=$" | sort`"
}

[ -f "product/$QUA_TARGET_PRODUCT/$QUA_BOARD_PRE_BUILD_SCRIPT" ] \
	&& source "product/$QUA_TARGET_PRODUCT/$QUA_BOARD_PRE_BUILD_SCRIPT"

for option in ${OPTIONS}; do
	echo "processing option: $option"
	case $option in
		project_*_conf.mk)
			_list=`find $TOP_DIR/product/* -maxdepth 0 -type d`
			for item in ${_list}; do
				if [ -f ${item}/$option ]; then
					option=${item}/$option
					break
				fi
			done
			if [ ! -f $option ]; then
				option=product/$option
			fi
			echo $option
			;&
		*.mk)
			CONF=$(realpath $option)
			echo "switching to board: $CONF"
			if [ ! -f $CONF ]; then
				echo "not exist!"
				exit 1
			fi

			ln -rsf $CONF $BOARD_CONFIG
			;;
		lunch) build_select_product ;;
		all) build_all ;;
		check) build_check ;;
		cleanall) build_cleanall ;;
		uboot) build_uboot ;;
		kernel) build_kernel ;;
		modules) build_modules ;;
		rootfs) build_rootfs $option ;;
		ramboot) build_ramboot ;;
		recovery) build_recovery ;;
		info) build_info ;;
		app/*|external/*) build_pkg $option ;;
		*) usage ;;
	esac
done

exit