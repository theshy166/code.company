#!/bin/bash

# 设置目标目录
if [ -z "$1" ]; then
#default search core/apps folder, because currently img file put in this folder
    target_dir="../../core/apps"
else
    target_dir="$1"
fi

# 遍历目标目录及其子目录下的所有 .c 文件
find "$target_dir" -type f -name "*.c" | while read file
do
    # 检查 .header.cf = LV_IMG_CF_RAW_ALPHA, 是否存在
    if grep -q '\.header\.cf = LV_IMG_CF_RAW_ALPHA,' "$file" && ! grep -q '\.header\.cf = LV_COLOR_FORMAT_RAW_ALPHA,' "$file"; then
        sed -i '/^[ ]*\.header\.cf = LV_IMG_CF_RAW_ALPHA,/{N;N;s/^[ ]*\.header\.cf = LV_IMG_CF_RAW_ALPHA,\n\s*\.header\.always_zero = 0,\n\s*\.header\.reserved = 0,/\#if defined(CONFIG_LVGL_V9) \&\& CONFIG_LVGL_V9\
  .header.cf = LV_COLOR_FORMAT_RAW_ALPHA,\
  .header.magic = LV_IMAGE_HEADER_MAGIC,\
#else\
  .header.cf = LV_IMG_CF_RAW_ALPHA,\
  .header.always_zero = 0,\
  .header.reserved = 0,\
#endif/g;}' "$file"
    echo "File $file has been updated"
    fi

  # 检查 .header.cf = LV_IMG_CF_RAW, 是否存在
    if grep -q '\.header\.cf = LV_IMG_CF_RAW,' "$file" && ! grep -q '\.header\.cf = LV_COLOR_FORMAT_RAW,' "$file"; then
        sed -i '/^[ ]*\.header\.cf = LV_IMG_CF_RAW,/{N;N;s/^[ ]*\.header\.cf = LV_IMG_CF_RAW,\n\s*\.header\.always_zero = 0,\n\s*\.header\.reserved = 0,/\#if defined(CONFIG_LVGL_V9) \&\& CONFIG_LVGL_V9\
  .header.cf = LV_COLOR_FORMAT_RAW,\
  .header.magic = LV_IMAGE_HEADER_MAGIC,\
#else\
  .header.cf = LV_IMG_CF_RAW,\
  .header.always_zero = 0,\
  .header.reserved = 0,\
#endif/g;}' "$file"
    echo "File $file has been updated, cf LV_IMG_CF_RAW"
    fi

  # 检查 .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA, 是否存在
    if ! grep -q 'CONFIG_LVGL_V9' "$file"; then
        sed -i '/^[ ]*\.header\.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,/{N;N;s/^[ ]*\.header\.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,\n\s*\.header\.always_zero = 0,\n\s*\.header\.reserved = 0,/\#if defined(CONFIG_LVGL_V9) \&\& CONFIG_LVGL_V9\
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,\
  .header.magic = LV_IMAGE_HEADER_MAGIC,\
#else\
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,\
  .header.always_zero = 0,\
  .header.reserved = 0,\
#endif/g;}' "$file"
    echo "File $file has been updated, cf LV_IMG_CF_TRUE_COLOR_ALPHA"
    fi

   if ! grep -q 'CONFIG_LVGL_V9' "$file"; then
        sed -i '/^[ ]*\.header\.always_zero = 0,/s/^[ ]*\.header\.always_zero = 0,/\#if defined(CONFIG_LVGL_V9) \&\& CONFIG_LVGL_V9\
  .header.magic = LV_IMAGE_HEADER_MAGIC,\
#else\
  .header.always_zero = 0,\
#endif/g' "$file"
    echo "File $file has been updated, header always_zero"
    fi

  # 检查 .header.cf = LV_IMG_CF_TRUE_COLOR, 是否存在
    if ! grep -q 'CONFIG_LVGL_V9' "$file"; then
        sed -i '/^[ ]*\.header\.cf = LV_IMG_CF_TRUE_COLOR,/{N;N;s/^[ ]*\.header\.cf = LV_IMG_CF_TRUE_COLOR,\n\s*\.header\.always_zero = 0,\n\s*\.header\.reserved = 0,/\#if defined(CONFIG_LVGL_V9) \&\& CONFIG_LVGL_V9\
  .header.cf = LV_IMG_CF_TRUE_COLOR,\
  .header.magic = LV_IMAGE_HEADER_MAGIC,\
#else\
  .header.cf = LV_IMG_CF_TRUE_COLOR,\
  .header.always_zero = 0,\
  .header.reserved = 0,\
#endif/g;}' "$file"
    echo "File $file has been updated, cf LV_IMG_CF_TRUE_COLOR"
    fi

done
