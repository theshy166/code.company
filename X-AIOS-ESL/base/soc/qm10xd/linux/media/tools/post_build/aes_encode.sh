#!/bin/bash

#
# function: usage
#

debug=0

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
	local aes_cfg="$2";
	local out_encode_file="$3";
	
	cfg_info=$(cat ${aes_cfg})

	var=`echo $cfg_info | awk -F '=' '{print $0}'|sed "s/=/ /g "`
	aes_key=$(echo ${var} | awk '{print $2}')
	aes_iv=$(echo ${var} | awk '{print $4}')
	
	debug_print "key:${aes_key} iv:${aes_iv}"

	dd bs=1 skip=0 if="${encode_file}" of="${encode_file}".encode.tmp2 \
		> /dev/null 2>&1;

	filesize=$(stat -c%s "${encode_file}")
	encode_len=$[(${filesize} + 15) / 16 * 16] > /dev/null
	if ((${encode_len} % 16)); then
		echo -n "Error: To encode length is not 16 bytes aligned"
		echo " ${encode_len}";
		exit 1;
	fi;

	debug_print "aes encode filesize=${filesize} encode_len=${encode_len}"

	#if [ ! -n "$encode_len" ]; then
		objcopy -I binary -O binary --gap-fill=0x00 --pad-to="${encode_len}" "${encode_file}".encode.tmp2 "${encode_file}".encode
	#else
		#echo "------->"$encode_len
	#fi
		
	
	openssl enc -e -aes-128-cbc -in "${encode_file}".encode -out "${encode_file}".encode.tmp -nopad -nosalt -iv "${aes_iv}" -K "${aes_key}"

	dd conv=notrunc bs=1 seek=0 count="$((${filesize}))" if="${encode_file}.encode.tmp" \
		of="${out_encode_file}" > /dev/null 2>&1;
	rm ${encode_file}.encode.tmp
	rm "${encode_file}".encode.tmp2
	rm "${encode_file}".encode
}