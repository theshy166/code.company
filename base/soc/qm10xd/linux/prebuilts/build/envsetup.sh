#!/bin/bash -e

if [ -z "${BASH_SOURCE}" ];then
    echo Not in bash, switching to it...
    bash -c "$0 $@"
fi

function choose_board()
{
    echo
    echo "You're building on Linux"
    echo "Lunch menu...pick a config:"
    echo ""

    echo ${LT_DEFCONFIG_ARRAY[@]} | xargs -n 1 | sed "=" | sed "N;s/\n/. /"

    local INDEX
    while true; do
        read -p "Which would you like? [1-$LT_DEFCONFIG_ARRAY_LEN, other exit]: " INDEX
        INDEX=$((${INDEX:-0} - 1))

        if [ "$INDEX" -eq -1 ]; then
            unset TARGET_OUTPUT_DIR
            unset LT_BUILD_CONFIG
            echo
            echo "Choice not available. Please try again."
            echo
            break

        fi

        if echo $INDEX | grep -vq [^0-9]; then
            LT_BUILD_CONFIG="${LT_DEFCONFIG_ARRAY[$INDEX]}"
            [ -n "$LT_BUILD_CONFIG" ] && break
        fi

        echo
        echo "Choice not available. Please try again."
        echo
    done
}


function expand_makefile()
{
    INPUT_FILE=$1
    OUTPUT_FILE=$2
    NEW_FILE=$3

    # 创建或清空输出文件
    if [ "$NEW_FILE" == "new" ]; then
        > "$OUTPUT_FILE"
    fi

    # 读取输入Makefile的每一行
    while IFS= read -r line || [[ -n "$line" ]]; do
        # 对于包含include语句的行
        if [[ $line == include* ]]; then
            # 获取include路径
            include_path=$(echo "$line" | sed -e 's/include //')

            # 检查文件是否存在
            if [ ! -f $include_path ]; then
                echo "Included makefile $include_path not found."
                exit 1
            fi

            # 将包含的Makefile文件内容递归地展开到输出文件
            cat "$include_path" >> "$OUTPUT_FILE"

        else
            # 如果不是include语句，直接输出到文件
            echo "$line" >> "$OUTPUT_FILE"
        fi
    done < "$INPUT_FILE"
}

function lunch_config()
{
    TARGET_DIR_NAME="$LT_BUILD_CONFIG"
    export TARGET_OUTPUT_DIR="$TOP_DIR/output"

    mkdir -p $TARGET_OUTPUT_DIR || return

    echo "==========================================="
    echo
    echo "#TARGET_BOARD=$LT_BUILD_CONFIG"
    echo "#OUTPUT_DIR=$TARGET_OUTPUT_DIR/$LT_BUILD_CONFIG"
    echo
    echo "==========================================="


    pushd  ${SCRIPT_DIR}/configs;
        cat common.mk ${TARGET_DIR_NAME}.mk > ${TARGET_OUTPUT_DIR}/.config
    popd

    odir=$(sed '/TARGET_OUTPUT_DIR/!d' ${TARGET_OUTPUT_DIR}/.config)
    #if no TARGET_OUTPUT_DIR given, deault set to the config name
    if [ X"$odir" == "X" ]; then
        echo -e "\nexport TARGET_OUTPUT_DIR=$LT_BUILD_CONFIG\n" >> ${TARGET_OUTPUT_DIR}/.config
    fi

    #append the config name, NIMI is name in Finnish, just to avoid conflict
    echo -e "\nexport TARGET_CONFIG_NIMI=$LT_BUILD_CONFIG\n" >> ${TARGET_OUTPUT_DIR}/.config

    # append the toolchain config
    echo -e "\ninclude $TOP_DIR/prebuilts/build/configs/toolchain/\$(CROSS_TOOLCHAIN_VERSION).mk\n" >> ${TARGET_OUTPUT_DIR}/.config

}

function mem_config()
{
	MEM_CFG_FILE=mem_layout.cfg
	if [ ! -e ${MEM_CFG_FILE} ]; then
		echo "mem cfg file not exist!!!"
		return
	fi
	source ${MEM_CFG_FILE}
	echo -e "\nexport MEM_TOTAL_SIZE=${MEM_TOTAL_SIZE}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MEM_OS_SIZE=${MEM_OS_SIZE}\n" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_SHOWLOGO_YADDR0=${MC_SHOWLOGO_YADDR0}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_SHOWLOGO_CADDR0=${MC_SHOWLOGO_CADDR0}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_SHOWLOGO_YADDR1=${MC_SHOWLOGO_YADDR1}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_SHOWLOGO_CADDR1=${MC_SHOWLOGO_CADDR1}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_DHD0_OSD_SIZE=${MC_DHD0_OSD_SIZE}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_DHD0_OSD_FBC_EB=${MC_DHD0_OSD_FBC_EB}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_DHD1_OSD_SIZE=${MC_DHD1_OSD_SIZE}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport MC_DHD1_OSD_FBC_EB=${MC_DHD1_OSD_FBC_EB}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport mem_mmz_start=${mem_mmz_start}" >> ${TARGET_OUTPUT_DIR}/.config
	echo -e "\nexport mem_mmz_size=${mem_mmz_size}\n" >> ${TARGET_OUTPUT_DIR}/.config
}


function parse_mem_config()
{
	MEM_CFG_FILE=mem_layout.cfg
	if [ ! -e ${MEM_CFG_FILE} ]; then
		echo "mem cfg file not exist!!!"
		return
	fi
	LINUX_OS_SIZE=$OS_SIZE
	LINUX_MMZ_SIZE=$MMZ_SIZE
	#echo "enter parse layout >>>>>target file:${MEM_CFG_FILE},os:$LINUX_OS_SIZE,mmz:$LINUX_MMZ_SIZE,dir:${SCRIPT_DIR}<<<<<"
	if [ ! -z "$LINUX_OS_SIZE" ]; then
		if [[ ! "$LINUX_OS_SIZE" =~ ^[0-9]+M ]]; then
			echo "param error, eg:OS_SIZE=36M"
			exit 1
		fi
		sed -i "s/^MEM_OS_SIZE=[^ ]*/MEM_OS_SIZE=${LINUX_OS_SIZE}/g" ${SCRIPT_DIR}/configs/${MEM_CFG_FILE}
	fi
	if [ ! -z "$LINUX_MMZ_SIZE" ]; then
		if [[ ! "$LINUX_MMZ_SIZE" =~ ^0x[0-9a-fA-F] ]]; then
			echo "param error, eg:MMZ_SIZE=0x01A00000"
			exit 1
		fi
		sed -i "s/^mem_mmz_size=[^ ]*/mem_mmz_size=${LINUX_MMZ_SIZE}/g" ${SCRIPT_DIR}/configs/${MEM_CFG_FILE}
	fi
	if [[ ! -z "$LINUX_OS_SIZE" && "$LINUX_MMZ_SIZE" ]]; then
		MMZ_START=$((0x40000000 + ${LINUX_OS_SIZE%M} * 1024 * 1024))
		sed -i "s/^mem_mmz_start=[^ ]*/mem_mmz_start=$(printf "0x%X\n" $MMZ_START)/g" ${SCRIPT_DIR}/configs/${MEM_CFG_FILE}
		echo "LINUX_OS_SIZE = $OS_SIZE, LINUX_MMZ_SIZE = $MMZ_SIZE"
	fi
}

function main()
{
    SCRIPT_PATH=$(readlink -f ${BASH_SOURCE})
    SCRIPT_DIR=$(dirname ${SCRIPT_PATH})
    TOP_DIR=$(readlink -f ${SCRIPT_DIR}/../..)
    echo Top of tree: ${TOP_DIR}
    echo "SCRIPT_DIR:$SCRIPT_DIR"
    echo "SCRIPT_PATH:$SCRIPT_PATH"

	# Check if there are any lt* files in the directory and add them to the array
    if ls ${SCRIPT_DIR}/configs/lt* &> /dev/null; then
		LT_DEFCONFIG_ARRAY=(
			$(cd ${SCRIPT_DIR}/configs/; ls lt*  | \
				sed "s/\..*$//" | sort)
		)
	fi

	# Check if there are any qm* files in the directory and add them to the array
    if ls ${SCRIPT_DIR}/configs/qm* &> /dev/null; then
		LT_DEFCONFIG_ARRAY+=(
			$(cd ${SCRIPT_DIR}/configs/; ls qm*  | \
				sed "s/\..*$//" | sort)
		)
	fi

    unset LT_BUILD_CONFIG
    LT_DEFCONFIG_ARRAY_LEN=${#LT_DEFCONFIG_ARRAY[@]}

    case $LT_DEFCONFIG_ARRAY_LEN in
        0)
            echo "No available configs${1:+" for: $1"}"
            ;;
        1)
            LT_BUILD_CONFIG=${LT_DEFCONFIG_ARRAY[0]}
            ;;
        *)
            idx=$1
            idx=${idx:=0}
            echo "index = $idx"
            if (( $idx > 0 && $idx < $(($LT_DEFCONFIG_ARRAY_LEN+1)))); then
                LT_BUILD_CONFIG="${LT_DEFCONFIG_ARRAY[(($idx - 1)) ]}"
            elif [ -f ${SCRIPT_DIR}/configs/qm10xd_${CONFIG_XOS_PROJECT_TYPE}${CONFIG_STORAGE_TYPE}.mk ]; then
                 LT_BUILD_CONFIG=qm10xd_${CONFIG_XOS_PROJECT_TYPE}${CONFIG_STORAGE_TYPE}
            else
                choose_board
            fi
            ;;
    esac

    [ -n "$LT_BUILD_CONFIG" ] || return

    lunch_config
	parse_mem_config
	mem_config
	#evb demoboarf sfc config
	if [ "$#" -eq 0 ]; then
		exit 1
	fi

	while [ $# -gt 1 ]; do
		shift
	done
	last_param=$1
	if [ $last_param == "evb" ]; then
		echo -e "\nexport TARGET_CONFIG_BOARD=mc_evb\n" >> ${TARGET_OUTPUT_DIR}/.config
	else
		echo -e "\nexport TARGET_CONFIG_BOARD=mc_demoboard\n" >> ${TARGET_OUTPUT_DIR}/.config
	fi

}

main "$@"
