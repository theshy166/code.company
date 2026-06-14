#!/bin/bash

# 检查是否传入了目录路径作为参数
if [ $# -ne 1 ]; then
    echo "Usage: $0 <path_to_directory>"
    exit 1
fi

# 获取传入的目录路径
dir="$1"

# 检查目录是否存在
if [ ! -d "$dir" ]; then
    echo "Error: Directory not found at $dir"
    exit 1
fi

# 查找 u-boot.bin 和 rtthread.bin.lz4 文件
u_boot_file="$dir/u-boot.bin"
rtthread_file="$dir/rtthread.bin.lz4"

# 检查 u-boot.bin 是否存在
if [ ! -f "$u_boot_file" ]; then
    echo "Error: u-boot.bin not found in $dir"
    exit 1
fi

# 检查 rtthread.bin.lz4 是否存在
if [ ! -f "$rtthread_file" ]; then
    echo "Error: rtthread.bin.lz4 not found in $dir"
    exit 1
fi

# 计算 rtthread.bin.lz4 的文件大小，并将其对齐为 4KB
kernelfilesize=$(stat -c%s "$rtthread_file")
aligned_size=$(( ((kernelfilesize + 4095) / 4096) * 4096 ))
kernelhex_size=$(printf "0x%x" "$aligned_size")

# 输出计算的 kernelhex_size
echo "Calculated kernel hex size: $kernelhex_size"

# 查找关键字并替换
key="bootcmd=sf probe 0;nand read 0x41000000 0x1200000 0x800000"
new_value="bootcmd=sf probe 0;nand read 0x41000000 0x1200000 $kernelhex_size"

# 检查是否安装了 Perl
if ! command -v perl &> /dev/null; then
    echo "Error: Perl is not installed. Please install Perl to proceed."
    exit 1
fi

# 使用 Perl 替换二进制文件中的关键字
if ! perl -pi -e "s/\Q$key\E/$new_value/g" "$u_boot_file"; then
    echo "Error: Perl substitution failed. Please check bootcmd in uboot env."
    exit 1
fi

echo "Replaced size: 0x800000 with $kernelhex_size"
