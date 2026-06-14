#!/bin/bash

# 检查 .config 文件是否存在
if [ ! -f .config.env ]; then
  echo ".config.env file not found!"
#  exit 1
fi

# 解析 .config.env 并输出为 unset 语句到临时文件
> .config.env.unset  # 清空文件
while IFS= read -r line; do
  if [[ "$line" =~ ^# ]] || [[ -z "$line" ]]; then
    continue
  fi
  #echo $line
  if [[ $line =~ ^export\ CONFIG_.*=.*$ ]]; then
    var_name=$(sed -r 's/^export\s+(CONFIG_[^=]+)=.*/\1/' <<<"$line")
    var_name=${var_name#export }
    echo "unset $var_name" >> .config.env.unset
  fi
done < .config.env

#echo "Environment variables have been unset based on .config.env file."
