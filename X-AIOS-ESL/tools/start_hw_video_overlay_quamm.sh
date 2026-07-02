#!/bin/sh
# ============================================================
# 硬件视频覆盖启动脚本
# 用法：被 start-stop-daemon 调用，将输出重定向到日志文件
# ============================================================

# 视频文件路径
VIDEO_PATH="${VIDEO_PATH:-/data/res/img/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4}"
# 开关标记文件（删除此文件即停止播放）
MARKER="${MARKER:-/data/.enable_hw_video_overlay}"
# 显示模式：both=双屏, 0=仅display0, 1=仅display1
DISPLAY_MODE="${DISPLAY_MODE:-both}"
# ★ 是否隐藏 FB 层（1=隐藏qxosui界面, 0=保留）
HIDE_FB="${HIDE_FB:-1}"
# 日志文件路径
LOG_PATH="${LOG_PATH:-/data/hw_video_overlay_quamm.log}"

# 重定向 stdin/stdout/stderr 到日志
exec </dev/null >"$LOG_PATH" 2>&1

# 启动播放器
exec /data/bin/hw_video_overlay_quamm "$VIDEO_PATH" "$MARKER" "$DISPLAY_MODE" "$HIDE_FB"
