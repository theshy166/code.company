#!/bin/bash

source boot_header.sh
source user_header.sh

glb_cfg_file="./glb_config_csv"
if_cfg_file="./if_config_csv"


#$1: header_name
#$2: file_name
#$3: file_extra_name, which will be combine.
#$4: indicate whether fill config.
function header_prefill()
{
	header_name=$1
	
	echo $header_name
	if [ -f "$glb_cfg_file" ]; then
		glb_cfg_offset=172
		tmp_file="h_glb_tmp.bin"
		while read line; do
			IFS='	', read -ra reg_info <<< "$line"
			count=${#reg_info[@]}
			if [ $count -eq 3 ]; then
				if [ -n "$(echo ${reg_info[0]} | sed -n "/^0x[0-9,a-f,A-F]\+$/p")" ]; then
					#echo ${reg_info[0]}, ${reg_info[1]}, ${reg_info[2]}
					reg_addr=${reg_info[0]}
					printf  "%08x" ${reg_addr} | xxd -r -p -l 4 > "$tmp_file".org
					objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
					dd conv=notrunc bs=1 seek="${glb_cfg_offset}" count=4 if="${tmp_file}".rev of="${header_name}" > /dev/null 2>&1;
					reg_value=${reg_info[1]}
					glb_cfg_offset=$((glb_cfg_offset+4))
					#echo $glb_cfg_offset
					printf  "%08x" ${reg_value} | xxd -r -p -l 4 > "$tmp_file".org
					objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
					dd conv=notrunc bs=1 seek="${glb_cfg_offset}" count=4 if="${tmp_file}".rev of="${header_name}" > /dev/null 2>&1;
					delay_us=${reg_info[2]}
					glb_cfg_offset=$((glb_cfg_offset+4))
					#echo $glb_cfg_offset
					printf  "%08x" ${delay_us} | xxd -r -p -l 4 > "$tmp_file".org
					objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
					dd conv=notrunc bs=1 seek="${glb_cfg_offset}" count=4 if="${tmp_file}".rev of="${header_name}" > /dev/null 2>&1;
					glb_cfg_offset=$((glb_cfg_offset+4))					
				fi
			else
				echo "invalid config item..."
			fi
		done < "$glb_cfg_file"
	fi

	if [ -f "$if_cfg_file" ]; then
		if_cfg_offset=412
		tmp_file="h_if_tmp.bin"
		while read line; do
			IFS='	', read -ra if_reg_info <<< "$line"
			count=${#if_reg_info[@]}
			if [ $count -eq 3 ]; then
				if [ -n "$(echo ${if_reg_info[0]} | sed -n "/^0x[0-9,a-f,A-F]\+$/p")" ]; then
					#echo ${if_reg_info[0]}, ${if_reg_info[1]}, ${if_reg_info[2]}
					reg_addr=${if_reg_info[0]}
					printf  "%08x" ${reg_addr} | xxd -r -p -l 4 > "$tmp_file".org
					objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
					dd conv=notrunc bs=1 seek="${if_cfg_offset}" count=4 if="${tmp_file}".rev of="${header_name}" > /dev/null 2>&1;
					reg_value=${if_reg_info[1]}
					if_cfg_offset=$((if_cfg_offset+4))
					#echo $if_cfg_offset
					printf  "%08x" ${reg_value} | xxd -r -p -l 4 > "$tmp_file".org
					objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
					dd conv=notrunc bs=1 seek="${if_cfg_offset}" count=4 if="${tmp_file}".rev of="${header_name}" > /dev/null 2>&1;
					delay_us=${if_reg_info[2]}
					if_cfg_offset=$((if_cfg_offset+4))
					#echo $if_cfg_offset
					printf  "%08x" ${delay_us} | xxd -r -p -l 4 > "$tmp_file".org
					objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
					dd conv=notrunc bs=1 seek="${if_cfg_offset}" count=4 if="${tmp_file}".rev of="${header_name}" > /dev/null 2>&1;
					if_cfg_offset=$((if_cfg_offset+4))
				fi
			else
				echo "invalid config item..."
			fi
		done < "$if_cfg_file"
	fi
	echo "dddddddd"
	echo $header_name
	echo ${header_name}
}