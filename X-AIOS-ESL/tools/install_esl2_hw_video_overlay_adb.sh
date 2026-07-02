#!/bin/sh
# ============================================================
# 硬件视频覆盖部署脚本
# 用法：./install_esl2_hw_video_overlay_adb.sh [设备地址]
# ============================================================
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"

# 视频文件（本地路径 → 板端路径）
VIDEO_PATH="${VIDEO_PATH:-$ROOT_DIR/core/apps/esl2/view/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4}"
BOARD_VIDEO="${BOARD_VIDEO:-/data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4}"

# 二进制和脚本板端路径
BOARD_BIN="/data/bin/hw_video_overlay_quamm"
BOARD_START="/data/bin/start_hw_video_overlay_quamm.sh"

# 显示模式：both / 0 / 1
DISPLAY_MODE="${DISPLAY_MODE:-both}"
# ★ 默认隐藏 FB 层
HIDE_FB="${HIDE_FB:-1}"
# 开关标记文件
MARKER="${MARKER:-/data/.enable_hw_video_overlay}"

# ============================================================
# 交叉编译
# ============================================================
CC="$ROOT_DIR/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/arm-linux-gcc"

"$CC" -O2 -Wall -Wextra \
    -I"$ROOT_DIR/core/package/libnative/qlibquammapi/include" \
    -o /tmp/hw_video_overlay_quamm \
    "$ROOT_DIR/tools/hw_video_overlay_quamm.c" \
    -L"$ROOT_DIR/output/lib" \
    -lquamm.player -lquamm_common -lquamm_utils -lsysutils \
    -Wl,-rpath,/system/qua/lib

# ============================================================
# 推送到设备
# ============================================================
adb connect "$DEVICE"

# 创建目录
adb -s "$DEVICE" shell 'mkdir -p /data/bin /data/res/img'

# 推送二进制和脚本
adb -s "$DEVICE" push /tmp/hw_video_overlay_quamm "$BOARD_BIN"
adb -s "$DEVICE" push "$ROOT_DIR/tools/start_hw_video_overlay_quamm.sh" "$BOARD_START"
adb -s "$DEVICE" shell "chmod 755 '$BOARD_BIN' '$BOARD_START'"

# 推送视频文件
adb -s "$DEVICE" push "$VIDEO_PATH" "$BOARD_VIDEO"

# ============================================================
# 停止旧实例并启动新覆盖
# ============================================================

# 先停掉可能冲突的图片覆盖
adb -s "$DEVICE" shell 'rm -f /data/.enable_image_overlay /data/.enable_video_overlay'

# 杀掉旧的 hw_video_overlay_quamm 进程
adb -s "$DEVICE" shell 'for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -TERM $p 2>/dev/null || true; done; sleep 1; for p in $(pidof hw_video_overlay_quamm 2>/dev/null); do kill -9 $p 2>/dev/null || true; done'

# 创建标记文件并启动
adb -s "$DEVICE" shell "touch '$MARKER'"
adb -s "$DEVICE" shell "start-stop-daemon -S -b -v -x '$BOARD_BIN' -- '$BOARD_VIDEO' '$MARKER' '$DISPLAY_MODE' '$HIDE_FB'"

# 验证启动状态
adb -s "$DEVICE" shell 'sleep 2; echo "=== process ==="; ps | grep "[h]w_video_overlay_quamm"; echo "=== qxosui ==="; ps | grep "[q]xosui"; echo "=== log tail ==="; tail -10 /data/hw_video_overlay_quamm.log 2>/dev/null || true'
