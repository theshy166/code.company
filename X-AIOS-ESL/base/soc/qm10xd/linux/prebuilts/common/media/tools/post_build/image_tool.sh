#!/bin/bash

source header.sh
source xmodem.sh
source rsa_sign.sh
source aes_encode.sh
function usage()
{
	echo -e "Usage: ./image_tool.sh";
}

function file_replace_word()
{
        local data=$1
        local file_name=$2
        local offset=$3
		local rp_cnt=$4
		echo "file_replace_word: ${data} ${file_name} ${offset} ${rp_cnt}"
        printf "%08x" ${data} > data.tmp
        xxd -r -p -l 4 data.tmp data.bin.tmp
        objcopy -I binary --reverse-bytes=4 data.bin.tmp data.bin.rev
        dd conv=notrunc bs=1 seek=${offset} count=${rp_cnt} \
                if=data.bin.rev of="${file_name}" > /dev/null 2>&1;
}

if [ $1 == 1 ]; then
       prefix_nvm_type="emmc_"
elif [ $1 == 2 ]; then
       prefix_nvm_type="nand_"
elif [ $1 == 7 ]; then
       prefix_nvm_type="sdio_"
else
       prefix_nvm_type="nor_"
fi
if [ $2 == 1 ]; then
       prefix_fast_boot="fb_"
fi
if [ $3 == 1 ]; then
	prefix_rtt_ap="rtt_"
fi
image_info_file="./${prefix_nvm_type}${prefix_fast_boot}${prefix_rtt_ap}image_info_csv"
if [ ! -f "$image_info_file" ]; then
       echo "Error: config file $image_info_file not exist."
       exit -1
fi
echo "config file is $image_info_file"


iram_base=0x30000000
emmc_part_sel=0
uboot_online=$2
nvm_type=$1
dec2hex(){
	printf "%x" $1
}
if [ ! -n "$nvm_type" ]; then
	echo "pls input valid nvm type: 5,nor;2,nand;1,emmc;7,sdio"
		exit 1
fi

echo "---------------------------------------------------->", $uboot_online
sed -i 's/\r$//' $image_info_file
declare -A boot_image
while read line; do
	IFS='	', read -ra array <<< "$line"
	count=${#array[@]}
	if [ $count -eq 11 ]; then
		
		image_name=${array[0]}
		image_list=(${image_name//:/ })
		image_count=${#image_list[@]}

		raw_image_type=${array[1]}
		image_type_list=(${raw_image_type//:/ })
		image_type=${image_type_list[0]}
		image_boot_in_spl=${image_type_list[1]}
		load_mode=${array[2]}
		to_sign=${array[3]}
		to_lzma=${array[4]}
		to_crypt=${array[5]}
		header_to_crypt=${array[6]}

		mem_base=${array[7]}
		mem_list=(${mem_base//:/ })
		mem_count=${#mem_list[@]}

		nvm_offset=${array[8]}
		nvm_list=(${nvm_offset//:/ })
		nvm_count=${#nvm_list[@]}

		up_limit=${array[9]}
		output_path=${array[10]}
		#echo $image_name $image_type,$load_mode,$to_sign,$to_lzma,$to_crypt,$header_to_crypt,$mem_base,$nvm_offset,$up_limit

		if [ $image_name = "image_name" ]; then
			echo "title, ignore..."
			continue
		fi
	

		#general empty header file, size=2KB.
		if [ $image_type = "image" ]; then
			image_file=
			image_file_add=
			
			file_path=$post_bin_path$output_path

			echo "$post_bin_path"
			if [ $image_count -ge 2 ]; then
				
 				image_file0=$file_path${image_list[0]}
				image_file_add=$file_path${image_list[1]}
				if [ -f "$file_path${image_list[0]}" ]; then
					echo "image_file0 file exist"
				else
					echo "image_file0 file not exist..."
					continue
				fi
				
				file1_size=$(stat -c%s "$image_file0")
				file1_size2=$(stat -c%s "$image_file_add")
				
				#echo $image_file0, $image_file_add, ${mem_list[0]}, ${mem_list[1]}, ${nvm_list[0]}, ${nvm_list[1]}
				first_file_name_split=(${image_list[0]//./ })
				combine_file_name="${first_file_name_split[0]}_combine.bin"
				cat  "${image_file0}" "$image_file_add" >  $file_path"${combine_file_name}"
				image_file=${combine_file_name}
				echo "++++++++++++++++++++++++++++++++++++++", ${image_file}
				pushd $file_path
				
				if [ -f "$image_file" ]; then
					file_size=$(stat -c%s "$image_file")
					actual_size=$[(${file_size} + 15) / 16 * 16] > /dev/null
					objcopy -I binary -O binary --gap-fill=0x00 --pad-to="${actual_size}" "${image_file}" \
						"${image_file}"
					echo "file exist...",$actual_size
				else
					echo "file not exist..."
					continue
				fi
				popd
				
			else
				image_file=${image_list[0]}
				
				if [ -f "$file_path${image_list[0]}" ]; then
					echo "image_file file exist"
				else
					echo "image_file file not exist..."
					continue
				fi
				
				pushd $file_path
				#echo "$file_path"
			
				if [ -f "$image_file" ]; then
					file_size=$(stat -c%s "$image_file")
					actual_size=$[(${file_size} + 15) / 16 * 16] > /dev/null
					echo "file exist...",$actual_size
					objcopy -I binary -O binary --gap-fill=0x00 --pad-to="${actual_size}" "${image_file}" \
						"${image_file}"
				else
					echo "file not exist..."
					continue
				fi
				
				if [[ $image_name == *dtb* ]]; then
					data=$(dd if="$image_file" bs=1 skip=48 count=4 | xxd -p)
					reversed_data=$(echo "$data" | tac -rs .. | tr -d '\n')
					formatted_data="0x$reversed_data"
					echo "dtb offseet: $formatted_data"
				
					image_hex=$(dec2hex $actual_size)
					image_hex_size="0x$image_hex"
					echo "image size: $image_hex_size"
				
					dtb_len=$(($image_hex_size-$formatted_data))
					dtb_hex_data=$(dec2hex $dtb_len)
					dtb_use_data="0x$dtb_hex_data"
					echo "dtb use: $dtb_use_data"
				fi
				
				popd
			fi

			header_name=${image_file%.*}_header_only
			if [ -f "$header_name".bin ]; then
				rm "${header_name}".bin
			fi
			#echo $header_name
			dd if=/dev/zero of="${header_name}".bin bs=2K count=1
			
			nv_base=${nvm_list[0]}
			data_src=$(($nv_base+0x800))
			#echo "$nv_base"
			#echo "${data_src}"

			printf "%08x" ${data_src} > ${data_src_cfg_cal}.tmp
			xxd -r -p -l 4 ${data_src_cfg_cal}.tmp ${data_src_cfg_cal}.bin.tmp
			objcopy -I binary --reverse-bytes=4 "${data_src_cfg_cal}".bin.tmp "${data_src_cfg_cal}".bin.rev
			dd conv=notrunc bs=1 seek=28 count=4 \
				if="${data_src_cfg_cal}".bin.rev of="${header_name}".bin > /dev/null 2>&1;
			
			
			magic_num=$"0x46524550"
			printf "%08x" ${magic_num} > ${magic_num_cal}.tmp
			xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
			objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
			dd conv=notrunc bs=1 seek=0 count=4 \
				if="${magic_num_cal}".bin.rev of="${header_name}".bin > /dev/null 2>&1;
			
			
			printf "%08x" ${actual_size} > ${actual_size_cal}.tmp
			xxd -r -p -l 4 ${actual_size_cal}.tmp ${actual_size_cal}.bin.tmp
			objcopy -I binary --reverse-bytes=4 "${actual_size_cal}".bin.tmp "${actual_size_cal}".bin.rev
			dd conv=notrunc bs=1 seek=8 count=4 \
				if="${actual_size_cal}".bin.rev of="${header_name}".bin > /dev/null 2>&1;

			echo "${header_name}".bin
			#pre-fill information of original image file to header
			#sub_str="spl"
			#if [[ "$image_name" =~ .*"$sub_str".* ]]; then
			#	echo "hhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
				header_prefill "${header_name}".bin
			#fi
			#if [ $image_name = "u-boot-spl.bin" ]; then
				#header_prefill "${header_name}".bin
			#fi
			

			#combine multi image to one
			if [ $image_count -ge 2 ]; then
				echo "combine multi image to one."
			fi
			
			
			#signature
			#sign_img $header_name $image_file
			sign_in_file=${image_file}
			sign_out_file=
			KEY_FILE="./rsa_priv.pem"
			if [ $to_sign -eq 1 ]; then
				echo "signature..."
				echo "$file_path "${image_file}""
				sign_img $file_path "${image_file}" "${header_name}" "${KEY_FILE}" "pss"
				
				if [ $load_mode = "cpu_sched" ]; then
					hash_cfg=$"0xFFFFFFF9"
					printf "%08x" ${hash_cfg} > ${hash_cfg_cal}.tmp
					xxd -r -p -l 4 ${hash_cfg_cal}.tmp ${hash_cfg_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${hash_cfg_cal}".bin.tmp "${hash_cfg_cal}".bin.rev
					dd conv=notrunc bs=1 seek=16 count=4 \
					if="${hash_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				else
					hash_cfg=$"0x7FFFFFF9"
					printf "%08x" ${hash_cfg} > ${hash_cfg_cal}.tmp
					xxd -r -p -l 4 ${hash_cfg_cal}.tmp ${hash_cfg_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${hash_cfg_cal}".bin.tmp "${hash_cfg_cal}".bin.rev
					dd conv=notrunc bs=1 seek=16 count=4 \
					if="${hash_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				fi
				sign_out_file=${sign_in_file}
			else
				sign_out_file=${sign_in_file}
			fi

			#compress
			#lzma $header_name $image_file
			lzma_in_file=${sign_out_file}
			if [ $to_lzma -eq 1 ] && [ $to_crypt -eq 0 ]; then
				lzma_out_file="${lzma_in_file%.*}_zip.bin"
			else
				lzma_out_file=$file_path"${lzma_in_file%.*}_zip.bin"
			fi	
			if [ $to_lzma -eq 1 ]; then
				echo "compress..."
				./lzma e "$file_path${lzma_in_file}" "${lzma_out_file}"
				
				zip_file_size=$(stat -c%s "${lzma_out_file}")
				zip_actual_size=$[(${zip_file_size} + 15) / 16 * 16] > /dev/null
				echo "file exist...",$zip_actual_size
				objcopy -I binary -O binary --gap-fill=0x00 --pad-to="${zip_actual_size}" "${lzma_out_file}" \
				"${lzma_out_file}"
				
				printf "%08x" ${zip_file_size} > ${zip_file_size_cal}.tmp
				xxd -r -p -l 4 ${zip_file_size_cal}.tmp ${zip_file_size_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${zip_file_size_cal}".bin.tmp "${zip_file_size_cal}".bin.rev
				dd conv=notrunc bs=1 seek=52 count=4 \
				if="${zip_file_size_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				lzma_cfg=$"0x0040FFFF"
				printf "%08x" ${lzma_cfg} > ${lzma_cfg_cal}.tmp
				xxd -r -p -l 4 ${lzma_cfg_cal}.tmp ${lzma_cfg_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${lzma_cfg_cal}".bin.tmp "${lzma_cfg_cal}".bin.rev
				dd conv=notrunc bs=1 seek=56 count=4 \
				if="${lzma_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
			else
				lzma_out_file=${lzma_in_file}
			fi

			#crpyt image
			#aes_encode $header_name $image_file
			
			aes_in_file=${lzma_out_file}
			aes_out_file="${aes_in_file%.*}_crypted.bin"
			if [ $to_crypt -eq 1 ]; then
				echo "crypt image..."
				if [ $to_lzma -eq 1 ]; then
					CFG_INFO="aes.cfg"
					#echo "crypt image...in"
					aes_encode "${lzma_out_file}" "${CFG_INFO}"  "${aes_out_file}"
					zip_encode_file_size=$(stat -c%s "${aes_out_file}")
					printf "%08x" ${zip_encode_file_size} > ${zip_encode_file_size_cal}.tmp
					xxd -r -p -l 4 ${zip_encode_file_size_cal}.tmp ${zip_encode_file_size_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${zip_encode_file_size_cal}".bin.tmp "${zip_encode_file_size_cal}".bin.rev
					dd conv=notrunc bs=1 seek=20 count=4 \
					if="${zip_encode_file_size_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;

					echo "crypt zip image...ok"
				else
					CFG_INFO="aes.cfg"
					echo "crypt no lzma image...in"
					aes_encode "$file_path${image_file}" "${CFG_INFO}"  "${aes_out_file}"
					encode_file_size=$(stat -c%s "${aes_out_file}")
					printf "%08x" ${encode_file_size} > ${encode_file_size_cal}.tmp
					xxd -r -p -l 4 ${encode_file_size_cal}.tmp ${encode_file_size_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${encode_file_size_cal}".bin.tmp "${encode_file_size_cal}".bin.rev
					dd conv=notrunc bs=1 seek=20 count=4 \
					if="${encode_file_size_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
					echo "crypt no zip image...22"
				fi
			else
				echo "no crypt"
				aes_out_file=${aes_in_file}
			fi
			
			echo "$image_count"
			if [ $image_count -eq 1 ]; then
				if [[ $image_name == *dtb* ]]; then
					mem_base=${mem_list[0]}
					printf "%08x" ${mem_base} > ${mem_base_cal}.tmp
					xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
					dd conv=notrunc bs=1 seek=4 count=4 \
					if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
					mem_base1=${mem_list[1]}
					printf "%08x" ${mem_base1} > ${mem_base_cal}.tmp
					xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
					dd conv=notrunc bs=1 seek=44 count=4 \
					if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				else
					printf "%08x" ${mem_base} > ${mem_base_cal}.tmp
					xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
					dd conv=notrunc bs=1 seek=4 count=4 \
					if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				fi
			else
				mem_base=${mem_list[0]}
				printf "%08x" ${mem_base} > ${mem_base_cal}.tmp
				xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
				dd conv=notrunc bs=1 seek=4 count=4 \
				if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
			
				echo "$file1_size"
				file2_src=$(($mem_base+$file1_size))
				#echo "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"
				echo "$file2_src"
				#echo "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"
				
				if [ $image_count -ge 2 ]; then
					printf "%08x" ${file1_size2} > ${file1_size_cal}.tmp
					xxd -r -p -l 4 ${file1_size_cal}.tmp ${file1_size_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${file1_size_cal}".bin.tmp "${file1_size_cal}".bin.rev
					dd conv=notrunc bs=1 seek=48 count=4 \
					if="${file1_size_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				fi
				
				mem_base1=${mem_list[1]}
				printf "%08x" ${mem_base1} > ${mem_base_cal}.tmp
				xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
				dd conv=notrunc bs=1 seek=44 count=4 \
				if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				printf "%08x" ${file2_src} > ${file2_src_cal}.tmp
				xxd -r -p -l 4 ${file2_src_cal}.tmp ${file2_src_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${file2_src_cal}".bin.tmp "${file2_src_cal}".bin.rev
				dd conv=notrunc bs=1 seek=40 count=4 \
				if="${file2_src_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
					
			fi
			
			if [ $load_mode = "cpu_sched" ]; then
				cpu_sched_cfg=$"0x00071000"
				printf "%08x" ${cpu_sched_cfg} > ${cpu_sched_cfg_cal}.tmp
				xxd -r -p -l 4 ${cpu_sched_cfg_cal}.tmp ${cpu_sched_cfg_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${cpu_sched_cfg_cal}".bin.tmp "${cpu_sched_cfg_cal}".bin.rev
				dd conv=notrunc bs=1 seek=84 count=4 \
				if="${cpu_sched_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				
				rev1=$"0x3"
				printf "%08x" ${rev1} > ${rev1_cal}.tmp
				xxd -r -p -l 4 ${rev1_cal}.tmp ${rev1_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${rev1_cal}".bin.tmp "${rev1_cal}".bin.rev
				dd conv=notrunc bs=1 seek=88 count=4 \
				if="${rev1_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;

			fi
			#echo "sssssssssssssssssssssssssssssssssssssssssssssssssssss"

			if [ $image_name = "u-boot-spl.bin" ]; then
				emmc_part_sel=1
				#cnt=$((cnt + 1))
				###########EMMC SPL 
				aes_addr=$"0x30010800"
				printf "%08x" ${aes_addr} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=68 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				aes_addr=$"0x30010800"
				printf "%08x" ${aes_addr} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=72 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				aes_addr=$"0x30010800"
				printf "%08x" ${aes_addr} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=76 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;

				#nor flash bringup for sigle line 03 cmd
				rev1=$"0x3"
				printf "%08x" ${rev1} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=88 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
	
				magic_num=$"0x46524550"
				printf "%08x" ${magic_num} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=1680 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				printf "%08x" ${cnt} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=1684 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
			
				printf "%08x" ${nvm_type} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=1688 count=4 \
					if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
					
				dd if=filling.bin of="${header_name}" bs=1 seek=1692 count=$(stat -c %s filling.bin) conv=notrunc
				rm filling.bin
				p_name=0
				echo "$p_name"
				printf "%08x" $p_name > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=12 count=1 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
				p_type=1
				echo "$p_type"
				printf "%08x" $p_type > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=13 count=1 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
				if [ $load_mode = "cpu_sched" ]; then
					p_scheme=2
				else
					p_scheme=0
				fi	
						
				echo "$p_scheme"
				printf "%08x" $p_scheme > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=14 count=1 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
				p_head=1	
				printf "%08x" $p_head > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=15 count=1 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
				nvm_base=${array[8]}
				#echo "$nvm_base"
				printf "%08x" $nvm_base > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=24 count=4 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
						
				start_base=${mem_base}
				printf "%08x" $start_base > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=28 count=4 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
				nv_size=${array[9]}
				printf "%08x" $nv_size > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=32 count=4 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
				
				
			elif [ $image_name == "u-boot-pdl.bin" ]; then
			

				magic_num=$"0x46524550"
				printf "%08x" ${magic_num} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=0 count=4 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
				
				printf "%08x" ${cnt} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=4 count=4 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
			
				printf "%08x" ${nvm_type} > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=8 count=4 \
					if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
			

				dd if=p_filling.bin of="${header_name}" bs=1 seek=1680 count=$(stat -c %s p_filling.bin) conv=notrunc
				rm p_filling.bin
			else
				
				if [[ $image_name == *dtb* ]]; then
					name=3
					p_name=2
				elif [[ $image_name == *pre* ]]; then
					name=0
					p_name=3
					emmc_part_sel=1
				elif [[ $image_name == *rtt* ]]; then
					name=1
					type=2
					p_name=4
				elif [[ $image_name == *uboot* ]]; then
					name=2
					p_name=1
				elif [[ $image_name == *helloworld* ]]; then
					name=4
					p_name=5
				fi
				if [[ $image_name == *net* ]]; then
					name=5
					if [ $1 == 1 ]; then
						type=1
					else
					type=0
					fi
					p_type=0
				elif [[ $image_name == *sram* ]]; then
					name=6
					if [ $1 == 1 ]; then
						type=1
					else
						type=0
					fi
					p_type=0
				elif [[ $image_name == *dtb* ]]; then
					type=2
					p_type=2
				else
					type=1
					p_type=1
				fi

				if [ $image_count -ge 2 ]; then
					type=2
				fi

				if [ $load_mode = "cpu_sched" ]; then
					boot_scheme=2
				else
					boot_scheme=2
				fi


				if [ $nvm_type -eq 1 ]; then
					boot_scheme=0
				fi 
				
				flag=1
						
				if [ $image_count -eq 1 ]; then
					rtt0_boot_addr=${mem_list[0]}
					rtt1_boot_addr=${mem_list[0]}
				else
					rtt0_boot_addr=${mem_list[0]}
					rtt1_boot_addr=${mem_list[1]}
				fi

				
				nvm_base=${array[8]}
				start_base=${mem_base}
				nv_size=${array[9]}
				
				
				##########################################PDL#################################
				
				if [[ $image_name == *dtb* ]]; then
					p_name=2
					echo "$p_name"
					printf "%08x" $p_name > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=60 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					p_type=2
					echo "$p_type"
					printf "%08x" $p_type > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=61 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					if [ $load_mode = "cpu_sched" ]; then
						p_scheme=2
					else
						p_scheme=0
					fi	
						
					echo "$p_scheme"
					printf "%08x" $p_scheme > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=62 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					p_head=1	
					printf "%08x" $p_head > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=63 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nvm_base=${array[8]}
					#echo "$nvm_base"
					printf "%08x" $nvm_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=72 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
						
					start_base=${mem_base}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=76 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nv_size=${array[9]}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=80 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
				elif [[ $image_name == *pre* ]]; then
					p_name=3
					echo "$p_name"
					printf "%08x" $p_name > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=84 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
							
					p_type=1
					echo "$p_type"
					printf "%08x" $p_type > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=85 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
							
					if [ $load_mode = "cpu_sched" ]; then
						p_scheme=2
					else
						p_scheme=0
					fi
							
					echo "$p_scheme"
					printf "%08x" $p_scheme > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=86 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					p_head=1	
					printf "%08x" $p_head > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=87 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nvm_base=${array[8]}
					#echo "$nvm_base"
					printf "%08x" $nvm_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=96 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
						
					start_base=${mem_base}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=100 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nv_size=${array[9]}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=104 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
				elif [[ $image_name == *rtt* ]]; then
					p_name=4
					echo "$p_name"
					printf "%08x" $p_name > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=108 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					p_type=2
					echo "$p_type"
					printf "%08x" $p_type > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=109 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					if [ $load_mode = "cpu_sched" ]; then
						p_scheme=2
					else
						p_scheme=0
					fi	
						
					echo "$p_scheme"
					printf "%08x" $p_scheme > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=110 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					p_head=1	
					printf "%08x" $p_head > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=111 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					if [[ $image_name == *rtt* ]]; then
						if [ $image_count -eq 1 ]; then
							rtt0_boot_addr=${mem_list[0]}
							rtt1_boot_addr=${mem_list[0]}
						else
							rtt0_boot_addr=${mem_list[0]}
							rtt1_boot_addr=${mem_list[1]}
						fi
					else
						rtt0_boot_addr=0
						rtt1_boot_addr=0
					fi	
					printf "%08x" $rtt0_boot_addr > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=112 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
					printf "%08x" $rtt1_boot_addr > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=116 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
					
					nvm_base=${array[8]}
					#echo "$nvm_base"
					printf "%08x" $nvm_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=120 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
						
					start_base=${mem_base}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=124 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nv_size=${array[9]}
					printf "%08x" $nv_size > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=128 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
					
				elif [[ $image_name == *uboot* ]]; then
					p_name=1
					echo "$p_name"
					printf "%08x" $p_name > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=36 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					p_type=1
					echo "$p_type"
					printf "%08x" $p_type > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=37 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					if [ $load_mode = "cpu_sched" ]; then
						p_scheme=2
					else
						p_scheme=0
					fi	
						
					echo "$p_scheme"
					printf "%08x" $p_scheme > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=38 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
					p_head=1	
					printf "%08x" $p_head > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=39 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nvm_base=${array[8]}
					#echo "$nvm_base"
					printf "%08x" $nvm_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=48 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
						
					start_base=${mem_base}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=52 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nv_size=${array[9]}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=56 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
				elif [[ $image_name == *net* ]]; then
					p_name=5
					echo "$p_name"
					printf "%08x" $p_name > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=132 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nvm_base=${array[8]}
					#echo "$nvm_base"
					printf "%08x" $nvm_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=144 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
						
					start_base=${mem_base}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=148 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
						
					nv_size=${array[9]}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=152 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
				elif [[ $image_name == *sram* ]]; then		
					p_name=6
					echo "$p_name"
					printf "%08x" $p_name > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=180 count=1 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
					nvm_base=${array[8]}
					#echo "$nvm_base"
					printf "%08x" $nvm_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=192 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;	
					start_base=${mem_base}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=196 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
					nv_size=${array[9]}
					printf "%08x" $start_base > ${magic_num_cal}.tmp
					xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
					dd conv=notrunc bs=1 seek=200 count=4 \
						if="${magic_num_cal}".bin.rev of=p_filling.bin > /dev/null 2>&1;
				elif [[ $image_name == *helloworld* ]]; then
					p_name=1
					p_type=1
					file_replace_word ${p_name} p_filling.bin 156 1
					file_replace_word ${p_type} p_filling.bin 157 1

					if [ $load_mode = "cpu_sched" ]; then
						p_scheme=2
					else
						p_scheme=0
					fi
					file_replace_word ${p_scheme} p_filling.bin 158 1

					p_head=1	
					file_replace_word ${p_head} p_filling.bin 159 1
	
					nvm_base=${array[8]}
					file_replace_word ${nvm_base} p_filling.bin 168 4

					start_base=${mem_base}
					file_replace_word ${start_base} p_filling.bin 172 4

					nv_size=${array[9]}
					file_replace_word ${nv_size} p_filling.bin 176 4
				fi
				
				if [[ $image_boot_in_spl == 1 ]]; then
				
				echo "$name"
				printf "%08x" $name > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 0)) count=1 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;
				
				
				#echo "$type"
				printf "%08x" $type > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 1)) count=1 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;
					
					
				#echo "$boot_scheme"
				printf "%08x" $boot_scheme > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 2)) count=1 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;
		
				#echo "$flag"
				printf "%08x" $flag > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 3)) count=1 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;		
				
				#echo "$rtt0_boot_addr"
				printf "%08x" $rtt0_boot_addr > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 4)) count=4 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;
				
				#echo "$rtt1_boot_addr"
				printf "%08x" $rtt1_boot_addr > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 8)) count=4 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;
			
				#echo "$nvm_base"
				printf "%08x" $nvm_base > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 12)) count=4 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;				
				
				#echo "$start_base"
				printf "%08x" $start_base > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 16)) count=4 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;				
				

				#echo "$nv_size"
				printf "%08x" $nv_size > ${magic_num_cal}.tmp
				xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
				dd conv=notrunc bs=1 seek=$((cnt*24 + 20)) count=4 \
					if="${magic_num_cal}".bin.rev of=filling.bin > /dev/null 2>&1;					
				cnt=$((cnt + 1))
				fi
			fi
			
			#echo "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN"
	
			if [ $to_lzma -eq 1 ]; then
				sched_cfg=$"0x00010101"
				printf "%08x" ${sched_cfg} > ${sched_cfg_cal}.tmp
				xxd -r -p -l 4 ${sched_cfg_cal}.tmp ${sched_cfg_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${sched_cfg_cal}".bin.tmp "${sched_cfg_cal}".bin.rev
				dd conv=notrunc bs=1 seek=12 count=4 \
				if="${sched_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				if [ $image_count -ge 1 ]; then
					printf "%08x" ${mem_base} > ${lzma_dst}.tmp
					xxd -r -p -l 4 ${lzma_dst}.tmp ${lzma_dst}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${lzma_dst}".bin.tmp "${lzma_dst}".bin.rev
					dd conv=notrunc bs=1 seek=80 count=4 \
					if="${lzma_dst}".bin.rev of="${header_name}" > /dev/null 2>&1;
				else
					mem_base=${mem_list[0]}
					printf "%08x" ${mem_base} > ${lzma_dst}.tmp
					xxd -r -p -l 4 ${lzma_dst}.tmp ${lzma_dst}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${lzma_dst}".bin.tmp "${lzma_dst}".bin.rev
					dd conv=notrunc bs=1 seek=80 count=4 \
					if="${lzma_dst}".bin.rev of="${header_name}" > /dev/null 2>&1;
				fi
				
			else

				sched_cfg=$"0x00000101"
				printf "%08x" ${sched_cfg} > ${sched_cfg_cal}.tmp
				xxd -r -p -l 4 ${sched_cfg_cal}.tmp ${sched_cfg_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${sched_cfg_cal}".bin.tmp "${sched_cfg_cal}".bin.rev
				dd conv=notrunc bs=1 seek=12 count=4 \
				if="${sched_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				if [ $to_crypt -eq 1 ]; then
					if [ $1 == 1 ]; then
						if [ $image_name = "u-boot-spl.bin" ]; then
							aes_addr=$"0x30010800"
							printf "%08x" ${aes_addr} > ${magic_num_cal}.tmp
							xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
							objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
							dd conv=notrunc bs=1 seek=68 count=4 \
							if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
							aes_addr=$"0x30000800"
							printf "%08x" ${aes_addr} > ${magic_num_cal}.tmp
							xxd -r -p -l 4 ${magic_num_cal}.tmp ${magic_num_cal}.bin.tmp
							objcopy -I binary --reverse-bytes=4 "${magic_num_cal}".bin.tmp "${magic_num_cal}".bin.rev
							dd conv=notrunc bs=1 seek=72 count=4 \
							if="${magic_num_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
						else
					printf "%08x" ${mem_base} > ${mem_base_cal}.tmp
					xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
					objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
					dd conv=notrunc bs=1 seek=68 count=4 \
					if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
					dd conv=notrunc bs=1 seek=72 count=4 \
					if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
						fi
					else
						printf "%08x" ${mem_base} > ${mem_base_cal}.tmp
						xxd -r -p -l 4 ${mem_base_cal}.tmp ${mem_base_cal}.bin.tmp
						objcopy -I binary --reverse-bytes=4 "${mem_base_cal}".bin.tmp "${mem_base_cal}".bin.rev
						dd conv=notrunc bs=1 seek=68 count=4 \
						if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
						dd conv=notrunc bs=1 seek=72 count=4 \
						if="${mem_base_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
					fi
				fi
			fi
			
			if [[ $image_name == *dtb* ]]; then
				sched_cfg=$dtb_use_data
				printf "%08x" ${sched_cfg} > ${sched_cfg_cal}.tmp
				xxd -r -p -l 4 ${sched_cfg_cal}.tmp ${sched_cfg_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${sched_cfg_cal}".bin.tmp "${sched_cfg_cal}".bin.rev
				dd conv=notrunc bs=1 seek=48 count=4 \
				if="${sched_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
				
				sched_cfg=$(($mem_base + $formatted_data))
				printf "%08x" ${sched_cfg} > ${sched_cfg_cal}.tmp
				xxd -r -p -l 4 ${sched_cfg_cal}.tmp ${sched_cfg_cal}.bin.tmp
				objcopy -I binary --reverse-bytes=4 "${sched_cfg_cal}".bin.tmp "${sched_cfg_cal}".bin.rev
				dd conv=notrunc bs=1 seek=40 count=4 \
				if="${sched_cfg_cal}".bin.rev of="${header_name}" > /dev/null 2>&1;
		
			fi
	
			#crypt header
			#aes_encode $header_name
			if [ $header_to_crypt -eq 1 ]; then
				echo "crypt header..."
			fi

			#combine header & image
			#cut

			#xmodem add description frame.

			#xmodem_frame_add $nvm_type $image_size ${mem_list[0]} ${nvm_list[0]}
			if [ $nvm_type = 5 ]; then
				last_file_name="${aes_out_file%.*}_nor_header.bin"
				xmodem_nvm_type=1
			fi

			if [ $nvm_type = 1 ]; then
				last_file_name="${aes_out_file%.*}_emmc_header.bin"
				xmodem_nvm_type=3
			fi

			if [ $nvm_type = 2 ]; then
				last_file_name="${aes_out_file%.*}_nand_header.bin"
				xmodem_nvm_type=2
			fi

			echo "--------------------------------->", ${aes_out_file}
			if [ $to_lzma -eq 1 ] && [ $to_crypt -eq 1 ]; then
				cat  "${header_name}" "${aes_out_file}" > "${last_file_name}"
			elif [ $to_crypt -eq 1 ]; then
				cat  "${header_name}" "${aes_out_file}" > $file_path"${last_file_name}"
			elif [ $to_lzma -eq 1 ]; then
				cat  "${header_name}" "${aes_out_file}" > $file_path"${last_file_name}"
			else
				cat  "${header_name}" "$file_path${aes_out_file}" > $file_path"${last_file_name}"
			fi
			xmodem_tmp_file="xmodem_header"
			if [ $nvm_type = 2 ] && [ $image_name = "u-boot-spl.bin" ]; then
				xmodem_file_padto_name="u-boot-spl_nand_pad_to.bin"
				xmodem_file_size=0x10000
				objcopy -I binary -O binary --pad-to=0x10000 --gap-fill=0xff $file_path"${last_file_name}" $file_path"${xmodem_file_padto_name}"
				xmodem_last_file_name=$xmodem_file_padto_name

			else
				xmodem_file_size=$(stat -c%s "$file_path${last_file_name}")
				xmodem_last_file_name=$last_file_name
			fi
			xmodem_file_name="xmodem_${last_file_name}"
			echo "xmodem process: ${xmodem_nvm_type} ${xmodem_tmp_file} ${xmodem_file_size} ${mem_base} ${nv_base} ${emmc_part_sel}"
			xmodem_frame_add ${xmodem_nvm_type} "${xmodem_tmp_file}" ${xmodem_file_size} ${iram_base} ${nv_base} ${emmc_part_sel}
			cat "${xmodem_tmp_file}" $file_path"${xmodem_last_file_name}" > "$file_path${xmodem_file_name}"
			rm ${xmodem_tmp_file}
			emmc_part_sel=0
			rm ${header_name}
			if [ $nvm_type = 2 ] && [ $image_name = "u-boot-spl.bin" ]; then
				rm $file_path$xmodem_file_padto_name
			fi
			script_dir=$(dirname "$0")
			rm -f "$script_dir"/*.pub.bin "$script_dir"/*.tosig "$script_dir"/*.mod "$script_dir"/*.bin.rev "$script_dir"/*.tmp "$script_dir"/*.tmp2 "$script_dir"/*.bin.org
			rm -f "$script_dir"/*.rsa.sig "$script_dir"/*.mod.rev "$script_dir"/.bin.rev "$script_dir"/.tmp "$script_dir"/.bin.tmp  "$script_dir"/*.encode
		elif [ $image_type = "data" ]; then
			echo "data type..."
		else
			echo "invalid type..."
			exit 1
		fi

	else
		rm filling.bin
		echo "image info illegal."
		exit 1
	fi
done < "$image_info_file"

