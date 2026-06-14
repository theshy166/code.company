#!/bin/bash

# 检查是否传入目标文件路径
if [ -z "$1" ]; then
    echo "Usage: $0 <output_path>"
    exit 1
fi

# 设置变量
OUTPUT_PATH="$1"
IMAGE_SIZE=4096  # 指定大小（字节），例如4096字节
CONTENT=""  # 要写入的内容，可以修改为"boot-recovery"或空

# 确保目标目录存在，如果不存在则创建
OUTPUT_DIR=$(dirname "$OUTPUT_PATH")
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
fi

# 创建指定大小的空文件
dd if=/dev/zero of="$OUTPUT_PATH" bs=1 count=0 seek="$IMAGE_SIZE"

# 如果内容不为空，写入内容
if [ -n "$CONTENT" ]; then
    echo -n "$CONTENT" | dd of="$OUTPUT_PATH" bs=1 conv=notrunc
fi

echo "Created $OUTPUT_PATH with size $IMAGE_SIZE bytes and content: '$CONTENT'"


# 设置结构体字段初始值
{
    # command[32]全0（默认）
    # status[32]全0（默认）

    # tries_remaining[0]=3, tries_remaining[1]=0 (小端模式)
    printf '\x03\x00' | dd of="$OUTPUT_PATH" bs=1 seek=64 conv=notrunc

    # successful_boot[2]全0（默认）

    # 写入stage字段（偏移量68+764=832）
    # stage[32]全0（默认）

    # 写入reserved字段（偏移量832+32=864）
    # reserved[1184]全0（默认）

} 2>/dev/null

echo "Created $OUTPUT_PATH with bootloader_message structure initialized"
