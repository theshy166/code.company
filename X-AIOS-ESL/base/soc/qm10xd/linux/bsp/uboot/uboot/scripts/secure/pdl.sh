#!/bin/bash

# 从二进制文件中提取数据，跳过 8 字节，获取 4 个字节
data=$(dd if="u-boot-pdl-header.img" bs=1 skip=8 count=4 2>/dev/null | xxd -p)

# 将提取的十六进制字符串进行反转
reversed_data=$(echo "$data" | tac -rs .. | tr -d '\n')

# 将反转后的十六进制字符串添加前缀，形成格式化的数据
formatted_data="0x$reversed_data"

# secure addr
secure_addr=$(dd if="u-boot-pdl-header.img" bs=1 skip=4 count=4 2>/dev/null | xxd -p)

# 将提取的十六进制字符串进行反转
secure_data=$(echo "$secure_addr" | tac -rs .. | tr -d '\n')

# 将反转后的十六进制字符串添加前缀，形成格式化的数据
se_data="0x$secure_data"

se_del=$(printf "%d" "$se_data")

se_begain_addr=$((se_del - 528))

# 将十六进制字符串转换为十进制数
count_decimal=$(printf "%d" "$formatted_data")
aligned_data=$(printf "%d" "$formatted_data")
# 检查 reversed_data 是否为 16 字节对齐
if (( count_decimal % 16 != 0 )); then
    # 进行16字节对齐
    count_decimal=$(( (count_decimal + 15 ) / 16 * 16 ))
	aligned_data=$(( (count_decimal + 15 ) / 16 * 16 ))
fi
	count_decimal=$((count_decimal + 1024))
	
	secure_len=$((aligned_data + 528))
# 打印 dtb 偏移量
echo "Aligned dtb offset: $count_decimal"

# 使用 dd 命令从原始文件中提取数据写入新文件 (确保生成16字节对齐的文件)
dd if="u-boot-pdl-header.img" of="pdl.bin" bs=1 skip=0 count=$count_decimal 2>/dev/null

# 获取新文件的大小，如果不是 16 字节对齐，则填充 0 使其对齐
file_size=$(stat -c%s "pdl.bin")
if (( file_size % 16 != 0 )); then
    # 计算需要填充的字节数
    padding=$(( 16 - file_size % 16 ))
    dd if=/dev/zero bs=1 count=$padding >> pdl.bin 2>/dev/null
fi

# 打印 pdl.bin 的最终大小
final_size=$(stat -c%s "pdl.bin")
echo "Final pdl.bin size: $final_size bytes (aligned to 16 bytes)"

# 将对齐后的十六进制值转换为十六进制字符串
aligned_hex=$(printf "%08x" "$aligned_data")

# 反转字节顺序
reversed_aligned_hex=$(echo "$aligned_hex" | tac -rs .. | tr -d '\n')

# 将反转后的对齐值写入 pdl.bin 文件的第8字节位置
echo "$reversed_aligned_hex" | xxd -r -p | dd of="pdl.bin" bs=1 seek=8 conv=notrunc 2>/dev/null

# secure len
aligned_hex=$(printf "%08x" "$secure_len")

# 反转字节顺序
reversed_aligned_hex=$(echo "$aligned_hex" | tac -rs .. | tr -d '\n')

# 将反转后的对齐值写入 pdl.bin 文件的第16字节位置
echo "$reversed_aligned_hex" | xxd -r -p | dd of="pdl.bin" bs=1 seek=16 conv=notrunc 2>/dev/null

# secure addr
aligned_hex=$(printf "%08x" "$se_begain_addr")

# 反转字节顺序
reversed_aligned_hex=$(echo "$aligned_hex" | tac -rs .. | tr -d '\n')

# 将反转后的对齐值写入 pdl.bin 文件的第12字节位置
echo "$reversed_aligned_hex" | xxd -r -p | dd of="pdl.bin" bs=1 seek=12 conv=notrunc 2>/dev/null
# 输出最终 pdl.bin 文件的内容
echo "Updated pdl.bin with aligned dtb offset at position 8."
