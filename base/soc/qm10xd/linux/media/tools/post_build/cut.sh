#!/bin/bash

# 指定输入的bin文件名
input_bin="image_zip.bin"

# 指定输出的bin文件名和路径
output_bin="header.bin"

# 查找输入的bin文件并计算大小
bin_files=$(find . -type f -name "$input_bin")

# 检查是否找到bin文件
if [ -z "$bin_files" ]; then
  echo "未找到指定的bin文件"
  exit 1
fi

# 获取第一个匹配到的bin文件的大小
filesize=$(stat -c %s "$input_bin")
printf "aaa%08x\r\n" $filesize
# 将文件大小作为二进制数据写入输出bin文件的第52个字节位置

printf "%08x" ${filesize} > ${filesize_cal}.tmp
xxd -r -p -l 4 ${filesize_cal}.tmp ${filesize_cal}.bin.tmp
objcopy -I binary --reverse-bytes=4 "${filesize_cal}".bin.tmp "${filesize_cal}".bin.rev

dd conv=notrunc bs=1 seek=52 count=4 \
	if="${filesize_cal}".bin.rev of="$output_bin" > /dev/null 2>&1;

cat header.bin image_zip.bin  > header_image_zip.bin  
echo "计算结果已存储到 $output_bin"

