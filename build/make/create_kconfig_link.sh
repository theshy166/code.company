#!/bin/bash

# 目标：根据 .config 动态创建 Kconfig 的符号链接
CONFIG_PATH=".config"

# 检查 .config 中的 CONFIG_XOS_PROJECT_TYPE
if [ -f "$CONFIG_PATH" ]; then
    echo "start create_kconfig_link"
    PROJECT_TYPE=$(grep -E '^CONFIG_XOS_PROJECT_TYPE' "$CONFIG_PATH" | cut -d= -f2 | tr -d '\"')
    if [ -n "$PROJECT_TYPE" ]; then
        echo "Using project type: $PROJECT_TYPE"
        ln -sf "$PROJECT_TYPE/Kconfig" product/.Kconfig
    else
        echo "No CONFIG_XOS_PROJECT_TYPE found, defaulting to aicamera"
        ln -sf "aicamera/Kconfig" product/.Kconfig
    fi
else
    echo ".config file not found, defaulting to aicamera"
    ln -sf "aicamera/Kconfig" product/.Kconfig
fi

