#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
VIDEO_PATH="${VIDEO_PATH:-$ROOT_DIR/core/apps/esl2/view/13ad34351bdf701e9db0a1c53f2838ef_h264_800x640.mp4}"
FRAME_SRC_DIR="${FRAME_SRC_DIR:-$ROOT_DIR/core/apps/esl2/view/video_frames}"
WORK_DIR="${TMPDIR:-/tmp}/esl2_video_fb_overlay"
EXTRACT_DIR="$WORK_DIR/source_frames"
RAW_DIR="$WORK_DIR/video_frames_bgra"
ARCHIVE="$WORK_DIR/video_frames_bgra.tgz"
BOARD_FRAME_DIR="${BOARD_FRAME_DIR:-/data/res/img/video_frames_bgra}"
BOARD_SCRIPT="/data/bin/show_demo_video_fb.sh"
FRAME_US="${FRAME_US:-200000}"
MAX_VIDEO_FRAMES="${MAX_VIDEO_FRAMES:-8}"

if [ ! -f "$VIDEO_PATH" ]; then
    echo "missing video: $VIDEO_PATH" >&2
    exit 1
fi

rm -rf "$WORK_DIR"
mkdir -p "$EXTRACT_DIR" "$RAW_DIR"

if command -v ffmpeg >/dev/null 2>&1; then
    ffmpeg -y -i "$VIDEO_PATH" \
        -vf "scale=800:1280:force_original_aspect_ratio=increase,crop=800:1280" \
        "$EXTRACT_DIR/frame_%03d.jpg" >/dev/null 2>&1
    FRAME_INPUT_DIR="$EXTRACT_DIR"
else
    if [ ! -d "$FRAME_SRC_DIR" ]; then
        echo "ffmpeg not found and missing frame directory: $FRAME_SRC_DIR" >&2
        exit 1
    fi
    FRAME_INPUT_DIR="$FRAME_SRC_DIR"
fi

MAX_VIDEO_FRAMES="$MAX_VIDEO_FRAMES" python3 - "$FRAME_INPUT_DIR" "$RAW_DIR" <<'PY'
from pathlib import Path
from PIL import Image, ImageOps
import os
import sys

src_dir = Path(sys.argv[1])
dst_dir = Path(sys.argv[2])
dst_dir.mkdir(parents=True, exist_ok=True)

by_stem = {}
for path in sorted(src_dir.glob("frame_*.*")):
    if path.suffix.lower() not in {".jpg", ".jpeg", ".png", ".webp"}:
        continue
    current = by_stem.get(path.stem)
    if current is None or path.suffix.lower() in {".jpg", ".jpeg"}:
        by_stem[path.stem] = path

frames = [by_stem[key] for key in sorted(by_stem)]
if not frames:
    raise SystemExit(f"no frame images found in {src_dir}")

limit = int(os.environ.get("MAX_VIDEO_FRAMES", "8"))
if limit > 0 and len(frames) > limit:
    if limit == 1:
        positions = [0]
    else:
        positions = [round(i * (len(frames) - 1) / (limit - 1)) for i in range(limit)]
    frames = [frames[pos] for pos in positions]

resample = getattr(Image, "Resampling", Image).LANCZOS
for index, path in enumerate(frames, 1):
    img = Image.open(path)
    img = ImageOps.exif_transpose(img).convert("RGBA")
    if img.size != (800, 1280):
        img = ImageOps.fit(img, (800, 1280), method=resample, centering=(0.5, 0.5))
    out = dst_dir / f"frame_{index:03d}.bgra"
    out.write_bytes(img.tobytes("raw", "BGRA"))

total = sum(p.stat().st_size for p in dst_dir.glob("frame_*.bgra"))
print(f"converted {len(frames)} frames to {dst_dir}, {total} bytes")
print("selected:", " ".join(path.name for path in frames))
PY

tar -czf "$ARCHIVE" -C "$RAW_DIR" .

adb connect "$DEVICE"
adb -s "$DEVICE" shell 'mkdir -p /data/bin /data/res/img'
adb -s "$DEVICE" push "$ROOT_DIR/tools/show_demo_video_fb.sh" "$BOARD_SCRIPT"
adb -s "$DEVICE" shell "chmod 755 '$BOARD_SCRIPT'"
adb -s "$DEVICE" push "$ARCHIVE" /data/video_frames_bgra.tgz

if ! adb -s "$DEVICE" shell "rm -rf '$BOARD_FRAME_DIR' && mkdir -p '$BOARD_FRAME_DIR' && tar -xzf /data/video_frames_bgra.tgz -C '$BOARD_FRAME_DIR' && rm -f /data/video_frames_bgra.tgz"; then
    adb -s "$DEVICE" shell "rm -rf '$BOARD_FRAME_DIR' && mkdir -p '$BOARD_FRAME_DIR'"
    adb -s "$DEVICE" push "$RAW_DIR/." "$BOARD_FRAME_DIR/"
fi

adb -s "$DEVICE" shell "rm -f /data/.enable_image_overlay; touch /data/.enable_video_overlay"
adb -s "$DEVICE" shell "for p in \$(ps | grep '[s]how_demo_image_fb.sh' | awk '{print \$1}'); do kill \$p 2>/dev/null || true; done; for p in \$(ps | grep '[s]how_demo_video_fb.sh' | awk '{print \$1}'); do kill \$p 2>/dev/null || true; done"
adb -s "$DEVICE" shell "FRAME_US='$FRAME_US' start-stop-daemon -S -b -x '$BOARD_SCRIPT' 2>/dev/null || FRAME_US='$FRAME_US' '$BOARD_SCRIPT' >/dev/null 2>&1 &"
adb -s "$DEVICE" shell "sync; echo video_overlay_started; ls -l '$BOARD_FRAME_DIR' | head; ps | grep '[s]how_demo_video_fb.sh' || true"
