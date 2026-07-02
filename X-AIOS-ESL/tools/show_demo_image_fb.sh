#!/bin/sh
# ============================================================
# Framebuffer 图片覆盖显示脚本
# 通过直接写入 /dev/fb0 将 BGRA 格式的图片显示到屏幕上
# 与底层 qxosui 二进制共存，定期刷新覆盖其输出
# ============================================================

# 图片数据文件路径，4096000 字节 = 800x1280x4 (BGRA)
FRAME_PATH="${FRAME_PATH:-/data/frame_bgra.bin}"
# 开关标记文件，存在则运行覆盖，删除则停止
MARKER="${MARKER:-/data/.enable_image_overlay}"
# 日志文件路径
LOG_PATH="${LOG_PATH:-/data/fb_image_overlay.log}"
# dd 命令的块数：1000 * 4096 = 4096000 字节（刚好一帧）
SIZE_BLOCKS="${SIZE_BLOCKS:-1000}"

# 记录启动信息到日志
echo "fb image overlay start: frame=$FRAME_PATH marker=$MARKER" >"$LOG_PATH"

# 主循环：只要开关文件存在就持续运行
while [ -f "$MARKER" ]; do
    # 检查图片文件是否存在
    if [ -f "$FRAME_PATH" ]; then
        # 如果 fb0 设备存在，将图片数据直接写入 framebuffer
        # bs=4096 每次读写 4KB，count=1000 共 4096000 字节
        # >/dev/null 2>&1 重定向输出和错误，避免污染日志
        [ -e /dev/fb0 ] && dd if="$FRAME_PATH" of=/dev/fb0 bs=4096 count="$SIZE_BLOCKS" >/dev/null 2>&1
        # fb4 是第二个屏幕的 framebuffer（双屏设备）
        [ -e /dev/fb4 ] && dd if="$FRAME_PATH" of=/dev/fb4 bs=4096 count="$SIZE_BLOCKS" >/dev/null 2>&1
    else
        # 图片文件不存在时记录错误日志
        echo "missing frame: $FRAME_PATH" >>"$LOG_PATH"
    fi
    # 暂停 2 秒后再次刷新，防止覆盖频率过高占用 CPU
    sleep 2
done

# 开关文件被删除，循环退出，记录停止信息
echo "fb image overlay stop" >>"$LOG_PATH"
