#!/bin/bash

export ROOT_DIR=$(pwd)
export CMAKE_PATH=$ROOT_DIR/tools/ubuntu/cmake/cmake-3.28.1-linux-x86_64/bin
export NINJA_PATH=$ROOT_DIR/tools/ninja
export CROSS_PATH=$ROOT_DIR/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin
export PATH=$CMAKE_PATH:$NINJA_PATH:$PATH:$CROSS_PATH

export XOS_ROOT=$(pwd)
export XOS_BASE=""


function _gettop_once
{
    local TOPFILE=build/make/core/envsetup.mk
    if [ -n "$TOP" -a -f "$TOP/$TOPFILE" ] ; then
        # The following circumlocution ensures we remove symlinks from TOP.
        (cd "$TOP"; PWD= /bin/pwd)
    else
        if [ -f $TOPFILE ] ; then
            # The following circumlocution (repeated below as well) ensures
            # that we record the true directory name and not one that is
            # faked up with symlink names.
            PWD= /bin/pwd
        else
            local HERE=$PWD
            local T=
            while [ \( ! \( -f $TOPFILE \) \) -a \( "$PWD" != "/" \) ]; do
                \cd ..
                T=`PWD= /bin/pwd -P`
            done
            \cd "$HERE"
            if [ -f "$T/$TOPFILE" ]; then
                echo "$T"
            fi
        fi
    fi
}
T=$(_gettop_once)
if [ ! "$T" ]; then
    echo "Couldn't locate the top of the tree. Always source build/script/envsetup.sh from the root of the tree." >&2
    return 1
fi
IMPORTING_ENVSETUP=true source $T/build/make/shell_utils.sh


################################################################################
# Global Variable Configure
################################################################################

PROJECT_TOP_DIR=$T
#TARGET_PRODUCT_DIR=${PROJECT_TOP_DIR}/product
TARGET_PRODUCT="${PROJECT_TOP_DIR}/product/.target_product"
TARGET_PRODUCT_DIR=$(realpath ${TARGET_PRODUCT})



echo "PROJECT_TOP_DIR: $PROJECT_TOP_DIR"
echo "TARGET_PRODUCT_DIR: $TARGET_PRODUCT_DIR"

################################################################################
# Plubic Configure
################################################################################
C_BLACK="\e[30;1m"
C_RED="\e[31;1m"
C_GREEN="\e[32;1m"
C_YELLOW="\e[33;1m"
C_BLUE="\e[34;1m"
C_PURPLE="\e[35;1m"
C_CYAN="\e[36;1m"
C_WHITE="\e[37;1m"
C_NORMAL="\033[0m"

function msg_info()
{
	###echo -e "${C_GREEN}[$(basename $0):info] $1${C_NORMAL}"
	echo -e "${C_GREEN}[info] $1${C_NORMAL}"
}

function msg_warn()
{
	###echo -e "${C_YELLOW}[$(basename $0):warn] $1${C_NORMAL}"
	echo -e "${C_YELLOW}[warn] $1${C_NORMAL}"
}

function msg_error()
{
	###echo -e "${C_RED}[$(basename $0):error] $1${C_NORMAL}"
	echo -e "${C_RED}[error] $1${C_NORMAL}"
}

function finish_build(){
	echo "finish_build"
}

function build_target_board()
{
	local INDEX=$1
	echo $INDEX
	local link_target=$(readlink -f "$TARGET_PRODUCT")
	local target_path=$(dirname $(readlink -f "${XOS_TARGET_BOARD_PATHX[$INDEX]}"))
	local current_dir=$(pwd)
	local relative_board_target=$(realpath --relative-to="$current_dir" "$TARGET_PRODUCT")
	local relative_link_target=$(realpath --relative-to="$current_dir" "$link_target")
	local relative_target_path=$(realpath --relative-to="$current_dir" "$target_path")
	echo $current_dir
	echo $link_target
	echo $target_path
	echo $TARGET_PRODUCT
	if [ "$link_target" = $target_path ]; then
		echo "The symlink points to the same directory as \${XOS_TARGET_BOARD_PATHX[$INDEX]}: $link_target"
	else
		echo "The symlink does not point to the directory: $link_target", so create the symlink.
		rm -rf $TARGET_PRODUCT
		echo "1 $TARGET_PRODUCT 1"
		ln -s $target_path $TARGET_PRODUCT
		TARGET_PRODUCT_DIR=$(realpath ${TARGET_PRODUCT})
		echo "2 $TARGET_PRODUCT 2"
	fi  
}

function build_shell_config {  
    echo "Starting build_shell_config"  

    local __CONFIG_FILE="${TARGET_PRODUCT_DIR}/${XOS_BUILD_TARGET_BOARD}"  
    echo "Config file path: $__CONFIG_FILE"  s

    if [[ ! -f "$__CONFIG_FILE" ]]; then  
        echo "Error: Config file $__CONFIG_FILE does not exist."  
        return 1  
    fi  

    local __output_file="${__CONFIG_FILE}.sh"  
    {  
        echo "#!/bin/bash"
        echo ""

        while IFS= read -r line; do  

            if [[ $line == CONFIG_* ]]; then  

                var_name=$(echo "$line" | sed -e 's/^CONFIG_//; s/=.*//')  
                var_value=$(echo "$line" | sed -e 's/^CONFIG_[^=]*=//')  

                if [[ "$var_value" == "is not set" ]]; then  
                    var_value="false"  
                fi  
                
                echo "export $var_name=$var_value"
            fi  
        done < "$__CONFIG_FILE"

        if [[ -n "$line" && $line == CONFIG_* ]]; then  
            var_name=$(echo "$line" | sed -e 's/^CONFIG_//; s/=.*//')  
            var_value=$(echo "$line" | sed -e 's/^CONFIG_[^=]*=//')  

            if [[ "$var_value" == "is not set" ]]; then  
                var_value="false"  
            fi  

            echo "export $var_name=$var_value"
        fi  
    } > "$__output_file"  

    chmod +x "$__output_file"  

    echo "Variables have been exported from $__output_file"  
}

function build_target_base
{
	local config_file="${TARGET_PRODUCT_DIR}/${XOS_BUILD_TARGET_BOARD}"

	if [[ ! -f "$config_file" ]]; then
		echo "File $config_file does not exist."
		exit 1
	fi

	local __target_base
	if [ $CONFIG_CHIPSET_SIMULATOR == "y" ]; then
		__target_base="simulator"
	fi
	if [[ $CONFIG_CHIPSET_MC331X == "y" ]] && [[$CONFIG_OS_LINUX == "y"]]; then
		__target_base="mc331x_linux"
	fi
	if [[ $CONFIG_CHIPSET_MC331X == "y" ]] && [[$CONFIG_OS_RTT == "y"]];  then
		__target_base="qm108h_rttos"
	fi
	if [[ $CONFIG_CHIPSET_QM10XH == "y" ]] && [[$CONFIG_OS_LINUX == "y"]];  then
		__target_base="qm10xh_linux"
	fi
	if [[ $CONFIG_CHIPSET_QM10XD == "y" ]] && [[$CONFIG_OS_LINUX == "y"]];  then
		__target_base="qm10xd_linux"
	fi
	msg_info "__target_base is $__target_base"
	XOS_BASE=$config_value
	echo $XOS_BASE
}

function build_target_xos
{
	local config_value
	local config_file

	echo "build_target_xos 1 ${TARGET_PRODUCT_DIR}"
	config_file="${TARGET_PRODUCT_DIR}/${XOS_BUILD_TARGET_BOARD}"
	echo "build_target_xos 2 ${TARGET_PRODUCT_DIR}"

	if [[ ! -f "$config_file" ]]; then  
		echo "File $config_file does not exist."  
		exit 1  
	fi
	echo "build_target_xos 3 ${TARGET_PRODUCT_DIR}"
	config_value=$(grep -w "^CONFIG_XOS_BASE=" "$config_file" | cut -d'=' -f2 | tr -d '"') 
	if [[ -n "$config_value" ]]; then  
		echo "CONFIG_XOS_BASE is defined with value: $config_value"  
	else  
		echo "CONFIG_XOS_BASE is not defined in $config_file."  
	fi 
	XOS_BASE=$config_value
	echo $XOS_BASE
}

function choose_target_board()
{
	echo
	echo "You're building on Linux"
	echo "Lunch menu...pick a combo:"
	echo ""


	local cnt=0 space8="        "
	for item in ${XOS_TARGET_BOARD_ARRAY[@]}
	do
		echo "----------------------------------------------------------------"
		echo -e "${C_GREEN}$cnt. $item${C_NORMAL}"
		cnt=$(( cnt + 1 ))
		f0=${item#BoardConfig*-}
		#echo "f0: $f0"
	done

	local INDEX
	read -p "Which would you like? [0]: " INDEX
	INDEX=$((${INDEX:-0}))
	echo $INDEX

	if [ $XOS_TARGET_BOARD_ARRAY_LEN -lt $INDEX ];then
		msg_error "Input index is error"
		finish_build
		exit 0
	fi

	if echo $INDEX | grep -vq [^0-9]; then
		XOS_BUILD_TARGET_BOARD="${XOS_TARGET_BOARD_ARRAY[$INDEX]}"
		msg_info "Lunching for Select ${XOS_BUILD_TARGET_BOARD} boards..."
	else
		XOS_BUILD_TARGET_BOARD="${XOS_TARGET_BOARD_ARRAY[0]}"
		msg_info "Lunching for Default ${XOS_BUILD_TARGET_BOARD} boards..."
	fi
	echo "xxx 1 $XOS_BUILD_TARGET_BOARD"
	build_target_board $INDEX
	echo "xxx 2 $XOS_BUILD_TARGET_BOARD"
    if [ ! -d "${PROJECT_TOP_DIR}/output" ]; then
		mkdir -p ${PROJECT_TOP_DIR}/output
	fi

	build_target_base
	echo "1 XOS_BASE="$XOS_BASE
	TARGET_OS=$XOS_BASE
	echo "2 XOS_BASE="$XOS_BASE
	echo "TARGET_OS="$TARGET_OS
	cd ${PROJECT_TOP_DIR}/output

	local _board_defconfig="${XOS_BUILD_TARGET_BOARD}"
	local _board_name=$(expr "$_board_defconfig" : '\(.*\)_defconfig')	#$(echo "$_board_defconfig" | sed 's/_defconfig.*//')
	echo "$_board_name"
	local _TOOLCHAIN_NAME=$(grep "^CONFIG_TOOLCHAIN_" "$XOS_DIR/.config" | grep "=y" | sed -e 's/CONFIG_TOOLCHAIN_//' -e 's/=y//')
	_TOOLCHAIN_NAME=${_TOOLCHAIN_NAME:-simulator}
	#cmake -DCMAKE_TOOLCHAIN_FILE=${PROJECT_TOP_DIR}/build/cmake/toolchain/toolchain_${TARGET_OS}.cmake -DBOARD=test -DCONF_FILE=${TARGET_PRODUCT_DIR}/${XOS_BUILD_TARGET_BOARD} ${PROJECT_TOP_DIR} -Wno-dev
	###cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=${PROJECT_TOP_DIR}/build/cmake/toolchain/toolchain_${TARGET_OS}.cmake -DBOARD=test -DCONF_FILE=${TARGET_PRODUCT_DIR}/${XOS_BUILD_TARGET_BOARD} ${PROJECT_TOP_DIR} -Wno-dev
	cmake -DCMAKE_TOOLCHAIN_FILE=${PROJECT_TOP_DIR}/build/cmake/toolchain/toolchain_${_TOOLCHAIN_NAME}.cmake -DBOARD=$_board_name -DCONF_FILE=${TARGET_PRODUCT_DIR}/${XOS_BUILD_TARGET_BOARD} ${PROJECT_TOP_DIR} -Wno-dev
}

function build_select_board()
{
	#XOS_TARGET_BOARD_ARRAY=( $(cd ${TARGET_PRODUCT_DIR}/; ls BoardConfig*.conf BoardConfig_*/BoardConfig*.conf | sort) )
	XOS_TARGET_BOARD_PATHX=( $(find product -type d \( -not -path "product/.board_target" -a -not -path "product/common" \) -exec find {} -name "*_defconfig" -not -path "*/.board_target/*" -not -path "*/common/*" \; | sort -u) )
	XOS_TARGET_BOARD_ARRAY=( $(find product -type d \( -not -path "product/.board_target" -a -not -path "product/common" \) -exec find {} -name "*_defconfig" -not -path "*/.board_target/*" -not -path "*/common/*" \; | sed 's|^product/.*/||' | sort -u) )
	XOS_TARGET_BOARD_ARRAY_LEN=${#XOS_TARGET_BOARD_ARRAY[@]}
	if [ $XOS_TARGET_BOARD_ARRAY_LEN -eq 0 ]; then
		msg_error "No available Board Config"
		return
	fi

#	if [$TARGET_PRODUCT != "product/" ]
#	rm -rf $(TARGET_PRODUCT)
    choose_target_board
}

function lunch() {

    local T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP." >&2
        return
    fi

	echo "hi, this is lunch"
	echo "top dir is $T"

    build_select_board
}