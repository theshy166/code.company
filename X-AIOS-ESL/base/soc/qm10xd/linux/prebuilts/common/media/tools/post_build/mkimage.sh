#!/bin/bash

#source image_tool.sh
FILE_OFFSET=48
SCRIPT_PATH_xx=$(readlink -f ${BASH_SOURCE})
SCRIPT_DIR_xx=$(dirname ${SCRIPT_PATH_xx})
TOP_DIR_xx=$(readlink -f ${SCRIPT_DIR_xx}/../../../../..)

source <(grep 'TARGET_OUTPUT_DIR=' ${TOP_DIR_xx}/output/.config)

echo "#SCRIPT_PATH_xx=${SCRIPT_PATH_xx}"
echo "#SCRIPT_DIR_xx=${SCRIPT_DIR_xx}"
target_file=${TOP_DIR_xx}/output/${TARGET_OUTPUT_DIR}/bsp/kernel/Image
echo "target_file=$target_file"

FILE_SIZE_HEX=$(printf "%08X" $(stat -c %s "$target_file"))
#Fill the size of the kernel image
echo -n -e "\x${FILE_SIZE_HEX:6:2}\x${FILE_SIZE_HEX:4:2}\x${FILE_SIZE_HEX:2:2}\x${FILE_SIZE_HEX:0:2}" | dd of=${target_file} bs=1 seek=${FILE_OFFSET} count=4 conv=notrunc
