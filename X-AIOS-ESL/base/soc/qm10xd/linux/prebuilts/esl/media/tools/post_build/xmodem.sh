#!/bin/bash

pkg_type=3
pkg_mode=1
pos_pkg_mode=17
pos_image_size=1
pos_mem_base=5
pos_nvm_offset=9
pos_image_ver=0
pos_nvm_type=18
pos_page_size=19
pos_erase_size=21
#if_baud_rate=0

pos_nand_main_size=25
pos_nand_oob_size=29
pos_nand_ecc_mode=33
pos_nand_ecc_pos=37
pos_nand_sector_num=89
pos_nand_info_size=93
pos_nand_block_page=97

#emmc_partition_sel=0
pos_emmc_part=25

xmodem_nvm_file="./xmodem_nvm_csv"
#desc_frame_name="desc_frame_data"


#$1: nvm_type
function xmodem_frame_add()
{
	xmodem_page_size=0
	xmodem_erase_size=0
	xmodem_nor_page_size=0
	xmodem_nor_erase_size=0

	xmodem_nvm_type=$1
	desc_frame_name=$2
	xmodem_image_size=$3
	xmodem_mem_base=$4
	xmodem_nvm_offset=$5
	xmodem_emmc_part=$6

	tmp_file="tmp0"
	if [ -f "$desc_frame_name" ]; then
		echo "file exist...."
		rm $desc_frame_name
	fi
	dd if=/dev/zero of="${desc_frame_name}" bs=128 count=1

	while read line; do
		IFS='	', read -ra array <<< "$line"
		count=${#array[@]}
		if [ $count -eq 3 ]; then
			if [ ${array[0]} = "nvm_type" ]; then
				#echo "ignore title."
				continue
			fi
			if [ ${array[0]} = "nor" ]; then
				if [ ${array[1]} = "page_size" ]; then
					xmodem_nor_page_size=${array[2]}
				elif [ ${array[1]} = "erase_size" ]; then
					xmodem_nor_erase_size=${array[2]}
				else
					echo "unused nvm info."
				fi
			elif [ ${array[0]} = "nand" ]; then
				if [ ${array[1]} = "page_size" ]; then
					xmodem_page_size=${array[2]}
				elif [ ${array[1]} = "erase_size" ]; then
					xmodem_erase_size=${array[2]}
				elif [ ${array[1]} = "nand_main_size" ]; then
					xmodem_nand_main_size=${array[2]}
				elif [  ${array[1]} = "nand_oob_size" ]; then
					xmodem_nand_oob_size=${array[2]}
				elif [  ${array[1]} = "nand_ecc_mode" ]; then
					xmodem_nand_ecc_mode=${array[2]}
				elif [  ${array[1]} = "nand_ecc_pos" ]; then
					xmodem_nand_ecc_pos=${array[2]}
				elif [  ${array[1]} = "nand_sector_num" ]; then
					xmodem_nand_sector_num=${array[2]}
				elif [  ${array[1]} = "nand_info_size" ]; then
					xmodem_nand_info_size=${array[2]}
				elif [  ${array[1]} = "nand_block_page" ]; then
					xmodem_nand_block_page=${array[2]}
				else
					echo "unused nvm info."
				fi
			elif [ ${array[0]} = "emmc" ]; then
				if [ ${array[1]} = "block_size" ]; then
					emmc_page_size=${array[2]}
				else
					echo "unused nvm info"
				fi
			else
				echo "xmodem: unkown nvm type."
			fi
		else
			echo "xmodem: invalid nvm information."
		fi
	done < "$xmodem_nvm_file"

	#update package type...
	printf  "%08x" ${pkg_type} | xxd -r -p -l 4 > "$tmp_file".org
	objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
	dd conv=notrunc bs=1 seek=0 count=1 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

	#update package mode.
	printf  "%08x" ${pkg_mode} | xxd -r -p -l 4 > "$tmp_file".org
	objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
	dd conv=notrunc bs=1 seek="${pos_pkg_mode}" count=1 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

	#update nvm type.
	printf  "%08x" ${xmodem_nvm_type} | xxd -r -p -l 4 > "$tmp_file".org
	objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
	dd conv=notrunc bs=1 seek="${pos_nvm_type}" count=1 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

	if [ $xmodem_nvm_type -eq 1 ]; then
		#update page size.
		printf  "%08x" ${xmodem_nor_page_size} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_page_size}" count=1 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;
		#update erase size.
		printf  "%08x" ${xmodem_nor_erase_size} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_erase_size}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;
	fi
	if [ $xmodem_nvm_type -eq 3 ]; then
		#update page size.
		printf  "%08x" ${emmc_page_size} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_page_size}" count=1 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;
		printf  "%08x" ${xmodem_emmc_part} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_emmc_part}" count=1 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;
	fi

	#update image size.
	printf  "%08x" ${xmodem_image_size} | xxd -r -p -l 4 > "$tmp_file".org
	objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
	dd conv=notrunc bs=1 seek="${pos_image_size}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

	#update memory base.
	printf  "%08x" ${xmodem_mem_base} | xxd -r -p -l 4 > "$tmp_file".org
	objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
	dd conv=notrunc bs=1 seek="${pos_mem_base}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

	#update nvm offset.
	printf  "%08x" ${xmodem_nvm_offset} | xxd -r -p -l 4 > "$tmp_file".org
	objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
	dd conv=notrunc bs=1 seek="${pos_nvm_offset}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;


	#nand flash.
	if [ $xmodem_nvm_type -eq 2 ]; then
		printf  "%08x" ${xmodem_nand_main_size} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_main_size}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

		printf  "%08x" ${xmodem_nand_oob_size} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_oob_size}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

		printf  "%08x" ${xmodem_nand_ecc_mode} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_ecc_mode}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

		printf  "%08x" ${xmodem_nand_ecc_pos} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_ecc_pos}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

		printf  "%08x" ${xmodem_nand_sector_num} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_sector_num}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

		printf  "%08x" ${xmodem_nand_info_size} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_info_size}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;

		printf  "%08x" ${xmodem_nand_block_page} | xxd -r -p -l 4 > "$tmp_file".org
		objcopy -I binary --reverse-bytes=4 "${tmp_file}".org "${tmp_file}".rev
		dd conv=notrunc bs=1 seek="${pos_nand_block_page}" count=4 if="${tmp_file}".rev of="${desc_frame_name}" > /dev/null 2>&1;
	fi

	if [ -f "$tmp_file".org ]; then
		rm "$tmp_file".org
	fi
	if [ -f "$tmp_file".rev ]; then
		rm "$tmp_file".rev
	fi
}

