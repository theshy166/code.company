#!/bin/bash

# 检查 .config 文件是否存在
if [ ! -f .config ]; then
  echo ".config file not found!"
#  exit 1
fi

# 解析 .config 并输出为 export 语句到临时文件
> .config.env  # 清空文件
while IFS= read -r line; do
  if [[ "$line" =~ ^# ]] || [[ -z "$line" ]]; then
    continue
  fi

  if [[ "$line" =~ ^(CONFIG_[a-zA-Z0-9_]+)=y$ ]]; then
    var_name="${BASH_REMATCH[1]}"
    echo "export $var_name=true" >> .config.env

  elif [[ "$line" =~ ^(CONFIG_[a-zA-Z0-9_]+)=\"(.*)\"$ ]]; then
      var_name="${BASH_REMATCH[1]}"
      var_value="${BASH_REMATCH[2]}"
      if [[ "$var_value" =~ [[:space:]] ]]; then
        echo "export $var_name=\"$var_value\"" >>.config.env
      else
        echo "export $var_name=$var_value" >>.config.env
      fi

  elif [[ "$line" =~ ^(CONFIG_[a-zA-Z0-9_]+)=([0-9]+)$ ]]; then
    var_name="${BASH_REMATCH[1]}"
    var_value="${BASH_REMATCH[2]}"
    echo "export $var_name=$var_value" >> .config.env

  elif [[ "$line" =~ ^(CONFIG_[a-zA-Z0-9_]+)=0x([0-9A-Fa-f]+)$ ]]; then
    var_name="${BASH_REMATCH[1]}"
    var_value="${BASH_REMATCH[2]}"
    echo "export $var_name=0x$var_value" >> .config.env
  fi
done < .config

#echo "Environment variables have been set based on .config file."

