#!/bin/bash

IMAGE_FILE=$1;
CFG_FILE=$2;
IMAGE_HEADER_LEN=$3;

#
# function: usage
#
function usage()
{
	echo -e "
Usage: ./aes_encode.sh <u-boot.bin> <key> <header-len>
  Where,
      u-boot.bin: The bootloader that aes encode.
             key: The aes cfg file which including private key and iv. Ex: aes_cfg
      header-len: The u-boot file header length.

  Command Example:

     ./aes_encode.sh u-boot.bin aes.cfg header-len
	 ";
}


function debug_print()
{
	local message="$1";

	if [ ${debug} -ne 0 ]; then
		echo "${message}"
	fi;
}

function aes_encode()
{
	local encode_file="$1";
	local encode_offset="$2";
	local aes_key="$3";
	local aes_iv="$4";

	dd bs=1 skip="$((${encode_offset}))" if="${encode_file}" of="${encode_file}".encode.tmp2 \
		> /dev/null 2>&1;

	filesize=`wc -c "${encode_file}".encode.tmp2 | awk '{print $1}'` > /dev/null
	encode_len=$[(${filesize} + 15) / 16 * 16] > /dev/null
	if ((${encode_len} % 16)); then
		echo -n "Error: To encode length is not 16 bytes aligned"
		echo " ${encode_len}";
		exit 1;
	fi;

	debug_print "aes encode filesize=${filesize} encode_len=${encode_len}"

	objcopy -I binary -O binary --gap-fill=0x00 --pad-to="${encode_len}" "${encode_file}".encode.tmp2 \
		"${encode_file}".encode

	openssl enc -e -aes-128-cbc -in "${encode_file}".encode -out "${encode_file}".encode.tmp -nopad -nosalt -iv "${aes_iv}" -K "${aes_key}"

	dd conv=notrunc bs=1 seek="$((${encode_offset}))" count="$((${filesize}))" if="${encode_file}.encode.tmp" \
		of="${encode_file}" > /dev/null 2>&1;
}

#
# Main
#

#
# Debug print on/off flag
#  1: on
#  0: off
debug=0

#
# Check input files
#
if [ $# -lt 3 ]; then
	usage;
	exit 1;
fi;

IMAGE_FILE=`readlink -f "${IMAGE_FILE}"`
KEY_FILE=`readlink -f "${KEY_FILE}"`

encode_ofs=${IMAGE_HEADER_LEN}
cfg_info=$(cat ${CFG_FILE})

iv_file="iv.tmp"

var=`echo $cfg_info | awk -F '=' '{print $0}'|sed "s/=/ /g "`
key=$(echo ${var} | awk '{print $2}')
iv=$(echo ${var} | awk '{print $4}')
debug_print "key:${key} iv:${iv}"

echo ${iv} | xxd -r -p -l 16 > ${iv_file}
objcopy -I binary --reverse-bytes=4 ${iv_file} ${iv_file}.rev

aes_encode "${IMAGE_FILE}" "${encode_ofs}" "${key}" "${iv}"

debug_print "Inject iv back to image"
dd bs=1 conv=notrunc seek=104 skip=0 count=16 \
	if="${iv_file}".rev of="${IMAGE_FILE}" > /dev/null 2>&1;

rev1=$"0x1"  
printf "%08x" ${rev1} > ${magic_num_cal}.tmp  
xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp  
objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev  
dd conv=notrunc bs=1 seek=100 count=4 \
    if="${magic_num_cal}".bin.rev of="${IMAGE_FILE}" > /dev/null 2>&1;  

# clean up all tmp files
if [ ${debug} -eq 0 ]; then
	rm -f *.encode *.rev *.tmp *.tmp2
fi;
popd > /dev/null 2>&1;

