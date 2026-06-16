#!/bin/bash

HEADER_FILE=$2;
IMAGE_FILE=$1;
ORG_FILE=$3;
BASE=$4;
CONFIG=01010000
LZMA_ZIP_LEN=123
#0040ffff
LZMA_CTRL=ffff4000
#00000000
LZMA_DECODE_REG=00000000
#00000000
LZMA_DMA_CTRL_REG=00000000
#48000000
LZMA_SRC=00000048
#40006800
LZMA_DST=00000130
CPU_SCHED=00000000

#
# FUNCTION:usage
#
function usage()
{
	echo -e "
Usage:./boot_header.sh <image> <header> <org_image>

Command Example:

     ./boot_header.sh u-boot-spl.bin header.bin org_spl.bin
	";
}

function handle()
{
	local file="$1";
	local header="$2";
	local org_file="$3";
	dd if=/dev/zero of="${header}" bs=1 count=2048;

	filesize=$(stat -c%s $file)
	#echo $filesize
	h_filesize=`printf "%08x" $filesize`
	#h_filesize=78563412
	echo $h_filesize
	
	org_filesize=$(stat -c%s $org_file)
	
	org_h_filesize=`printf "%08x" $org_filesize`
	
	echo $org_h_filesize
	
	
	execute_addr=50455246
	echo "0:${execute_addr:0:8}" | xxd -r - ${header}
	
	echo "4:${BASE:6:2}" | xxd -r - ${header}
	echo "5:${BASE:4:2}" | xxd -r - ${header}
	echo "6:${BASE:2:2}" | xxd -r - ${header}
	echo "7:${BASE:0:2}" | xxd -r - ${header}
	
	echo "8:${org_h_filesize:6:2}" | xxd -r - ${header}
	echo "9:${org_h_filesize:4:2}" | xxd -r - ${header}
	echo "A:${org_h_filesize:2:2}" | xxd -r - ${header}
	echo "B:${org_h_filesize:0:2}" | xxd -r - ${header}
	#hexdump -C ${header}
	
	echo "C:${CONFIG:0:8}" | xxd -r - ${header}
	
	echo "34:${h_filesize:6:2}" | xxd -r - ${header}
	echo "35:${h_filesize:4:2}" | xxd -r - ${header}
	echo "36:${h_filesize:2:2}" | xxd -r - ${header}
	echo "37:${h_filesize:0:2}" | xxd -r - ${header}
	
	echo "38:${LZMA_CTRL:0:8}" | xxd -r - ${header}
	echo "3C:${LZMA_DECODE_REG:0:8}" | xxd -r - ${header}
	echo "40:${LZMA_DMA_CTRL_REG:0:8}" | xxd -r - ${header}
	echo "4C:${LZMA_SRC:0:8}" | xxd -r - ${header}
	echo "50:${LZMA_DST:0:8}" | xxd -r - ${header}
	echo "54:${CPU_SCHED:0:8}" | xxd -r - ${header}
	
	
	SFC_CTRL_CFG1=0800701C0000000000000010F400701C
	SFC_CTRL_CFG2=0E04000000000010
	echo "19C:${SFC_CTRL_CFG1:0:32}" | xxd -r - ${header}
	echo "1AC:${SFC_CTRL_CFG2:0:16}" | xxd -r - ${header}

	cat ${header} ${file} > ${prefix}_header.bin
	hexdump -C ${header}
}

#
# Check input files
#
if [ $# -lt 2 ]; then
	usage;
	exit 1;
fi;

IMAGE_FILE=`readlink -f "${IMAGE_FILE}"`;
HEADER_FILE=`readlink -f "${HEADER_FILE}"`;

ORG_FILE=`readlink -f "${ORG_FILE}"`;

prefix="${IMAGE_FILE%.*}"
handle "${IMAGE_FILE}" "${HEADER_FILE}" "${ORG_FILE}" "$BASE"

#function handle()
#{
#	local wb="$1";
#	local sec_offset="$2";
#	local key="$3";
#	if [ "$1" == "pre_rolling.bin" ]; then
#	
#	elif [ "$1" == "rtt.bin" ]; then
#	else
#	fi
#}
