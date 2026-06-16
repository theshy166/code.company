#!/bin/bash
#
# function: usage
#
function usage()
{
	echo -e "
Usage: ./rsa_sign.sh <u-boot.bin> <key> <header-len>
  Where,
      u-boot.bin: The bootloader that is to be signed .
             key: The rsa private key file. Ex: rsa_priv.pem
      header-len: The u-boot file header length.

  Command Example:

     ./rsa_sign.sh u-boot.bin rsa_priv.pem header-len
	 ";
}

debug=0

function debug_print()
{
	local message="$1";

	if [ ${debug} -ne 0 ]; then
		echo "${message}"
	fi;
}

function sign_img()
{
	local target_path=$1
	local wb=$2;
	local header_file="$3";
	local key="$4";
	local padding=$5;
	local klen=256
	local rsa_e_file=rsa_e;
	local size_file=size;
	
	echo $target_path, $wb, $header_file, $key, $padding
	
	if [ "${padding}" == 'pss' ]; then
		openssl dgst -sha256 -sigopt rsa_padding_mode:pss -sigopt\
				rsa_pss_saltlen:-1 -sign "${key}" -out "${wb}".rsa.sig "$target_path${wb}"
	else
		echo "error: not support!"
		exit 1
	fi

	debug_print " Inject rsa signature ${wb}.rsa.sig into wb_header"
	dd conv=notrunc bs=1 seek=1024 count=256 \
		if="${wb}".rsa.sig of="${header_file}" > /dev/null 2>&1;
	echo "rrrrrrrrrrrrrrrrrrrrrrr"
	echo ${header_file}
	debug_print " Generate public key modulus and save to ${key}.mod"
	openssl rsa -in "${key}" -noout -modulus -out "${key}".mod
	# remove prefix
	cut -d= -f2 < "${key}".mod > "${key}".mod.tmp
	# convert from hexdecimal to binary
	xxd -r -p -l 256 "${key}".mod.tmp "${key}".mod.tmp2
	# reverse byte order"
	objcopy -I binary --reverse-bytes=256 "${key}".mod.tmp2 "${key}".mod.rev

	debug_print " Inject public key modulus ${key}.mod.rev into wb_header"
	dd conv=notrunc bs=1 seek=1280 count=256 \
		if="${key}".mod.rev of="${header_file}" > /dev/null 2>&1;

	echo "01000100000000000000000000000000" > ${rsa_e_file}.tmp
	xxd -r -p -l 16 "${rsa_e_file}".tmp "${rsa_e_file}".bin.tmp

	dd conv=notrunc bs=1 seek=1536 count=16 \
		if="${rsa_e_file}".bin.tmp of="${header_file}" > /dev/null 2>&1;

	debug_print " replace filesize with actual size"

	dd skip=1280 bs=1 count=260 \
		if="${header_file}" of="${header_file}".rsa2048.pub.bin > /dev/null 2>&1;

	openssl dgst -sha256 "${header_file}".rsa2048.pub.bin 
	debug_print " Image signed to file ${wb}"
	
	#if [ ${debug} -eq 0 ]; then
	#rm -f *.sig *.tosig *.mod *.rev *.tmp *.tmp2
	#fi;
}