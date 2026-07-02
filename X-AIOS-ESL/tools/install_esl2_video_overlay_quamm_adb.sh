#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
FRAME_SRC_DIR="${FRAME_SRC_DIR:-$ROOT_DIR/core/apps/esl2/view/video_frames}"
WORK_DIR="${TMPDIR:-/tmp}/esl2_video_overlay_quamm"
RAW_DIR="$WORK_DIR/video_frames_bgra"
ARCHIVE="$WORK_DIR/video_frames_bgra.tgz"
BOARD_FRAME_DIR="${BOARD_FRAME_DIR:-/data/res/img/video_frames_bgra}"
BOARD_BIN="/data/bin/video_overlay_quamm"
BOARD_START_SCRIPT="/data/bin/start_video_overlay_quamm.sh"
FRAME_US="${FRAME_US:-33333}"
MAX_VIDEO_FRAMES="${MAX_VIDEO_FRAMES:-60}"
DISPLAY_MODE="${DISPLAY_MODE:-both}"

if [ ! -d "$FRAME_SRC_DIR" ]; then
    echo "missing frame source directory: $FRAME_SRC_DIR" >&2
    exit 1
fi

rm -rf "$WORK_DIR"
mkdir -p "$RAW_DIR"

MAX_VIDEO_FRAMES="$MAX_VIDEO_FRAMES" python3 - "$FRAME_SRC_DIR" "$RAW_DIR" <<'PY'
from pathlib import Path
from PIL import Image, ImageOps
import os
import sys

src_dir = Path(sys.argv[1])
dst_dir = Path(sys.argv[2])
dst_dir.mkdir(parents=True, exist_ok=True)

frames = [
    path for path in sorted(src_dir.glob("frame_*.*"))
    if path.suffix.lower() in {".jpg", ".jpeg", ".png", ".webp"}
]
if not frames:
    raise SystemExit(f"no frame images found in {src_dir}")

limit = int(os.environ.get("MAX_VIDEO_FRAMES", "30"))
if limit > 0 and len(frames) > limit:
    positions = [round(i * (len(frames) - 1) / (limit - 1)) for i in range(limit)]
    frames = [frames[pos] for pos in positions]

resample = getattr(Image, "Resampling", Image).LANCZOS
for index, path in enumerate(frames, 1):
    img = Image.open(path)
    img = ImageOps.exif_transpose(img).convert("RGBA")
    img = ImageOps.fit(img, (800, 1280), method=resample, centering=(0.5, 0.5))
    out = dst_dir / f"frame_{index:03d}.bgra"
    out.write_bytes(img.tobytes("raw", "BGRA"))

total = sum(p.stat().st_size for p in dst_dir.glob("frame_*.bgra"))
print(f"converted {len(frames)} frames to {dst_dir}, {total} bytes")
PY

tar -czf "$ARCHIVE" -C "$RAW_DIR" .

CC="$ROOT_DIR/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/arm-linux-gcc"
"$CC" -O2 -Wall -Wextra \
    -I"$ROOT_DIR/core/package/libnative/qlibquammapi/include" \
    -o "$WORK_DIR/video_overlay_quamm" \
    "$ROOT_DIR/tools/video_overlay_quamm.c" \
    -L"$ROOT_DIR/output/lib" \
    -lquamm_common -lquamm_utils -lsysutils \
    -Wl,-rpath,/system/qua/lib

adb connect "$DEVICE"
adb -s "$DEVICE" shell 'mkdir -p /data/bin /data/res/img'
adb -s "$DEVICE" push "$WORK_DIR/video_overlay_quamm" "$BOARD_BIN"
adb -s "$DEVICE" shell "chmod 755 '$BOARD_BIN'"
adb -s "$DEVICE" push "$ROOT_DIR/tools/start_video_overlay_quamm.sh" "$BOARD_START_SCRIPT"
adb -s "$DEVICE" shell "chmod 755 '$BOARD_START_SCRIPT'"
adb -s "$DEVICE" push "$ARCHIVE" /data/video_frames_bgra.tgz
adb -s "$DEVICE" shell "rm -rf '$BOARD_FRAME_DIR' && mkdir -p '$BOARD_FRAME_DIR' && tar -xzf /data/video_frames_bgra.tgz -C '$BOARD_FRAME_DIR' && rm -f /data/video_frames_bgra.tgz"

adb -s "$DEVICE" shell 'rm -f /data/.enable_image_overlay; touch /data/.enable_video_overlay'
adb -s "$DEVICE" shell 'for p in $(ps | grep "[i]mage_overlay_quamm" | awk "{print \$1}"); do kill $p 2>/dev/null || true; done'
adb -s "$DEVICE" shell 'for p in $(ps | grep "[v]ideo_overlay_quamm" | awk "{print \$1}"); do kill $p 2>/dev/null || true; done'
adb -s "$DEVICE" shell "FRAME_DIR='$BOARD_FRAME_DIR' FRAME_US='$FRAME_US' FRAME_W=800 FRAME_H=1280 DISPLAY_MODE='$DISPLAY_MODE' start-stop-daemon -S -b -x '$BOARD_START_SCRIPT'"
adb -s "$DEVICE" shell "sync; echo video_overlay_started; ps | grep '[v]ideo_overlay_quamm' || true; tail -20 /data/video_overlay_quamm.log || true; df -h /data"
