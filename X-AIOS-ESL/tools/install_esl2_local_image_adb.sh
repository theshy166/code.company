#!/bin/sh
set -eu

DEVICE="${1:-192.168.110.85:5555}"
ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
IMAGE_PATH="${IMAGE_PATH:-$ROOT_DIR/core/apps/esl2/view/video_frames/frame_001.jpg}"
TMP_DIR="${TMPDIR:-/tmp}/esl2_local_image_model"

if [ ! -f "$IMAGE_PATH" ]; then
    echo "missing image: $IMAGE_PATH" >&2
    exit 1
fi

mkdir -p "$TMP_DIR"

write_model()
{
    file="$1"
    cat >"$file" <<'JSON'
{"width":800,"height":1280,"color":"#FFFFFF","opacity":1,"bgColor":"#FFFFFF","video_urls":[],"img_urls":[],"child_list":[{"type":1,"content":"http://codex.local/codex_demo.jpg","color":"","fontSize":0,"textAlign":"left","fontFamily":"","bgColor":"","textDecoration":"none","borderRadius":0,"borderWidth":0,"borderColor":"","x":0,"y":0,"w":800,"h":1280,"angle":0,"opacity":1,"padding":0,"lineSpace":0,"letterSpace":0,"intFontSize":0,"floatFontSize":0,"numFloatStyle":""}]}
JSON
}

write_model "$TMP_DIR/product_base_model_FF2610043161.json"
write_model "$TMP_DIR/product_base_model_FF2610043162.json"
write_model "$TMP_DIR/product_base_model_FF261004316_1.json"
write_model "$TMP_DIR/product_base_model_FF261004316_2.json"

adb connect "$DEVICE"
adb -s "$DEVICE" shell 'mkdir -p /data/res/img /data/res/json'
adb -s "$DEVICE" push "$IMAGE_PATH" /data/res/img/codex_demo_0.jpg
adb -s "$DEVICE" push "$IMAGE_PATH" /data/res/img/codex_demo_1.jpg
adb -s "$DEVICE" push "$TMP_DIR/product_base_model_FF2610043161.json" /data/res/json/
adb -s "$DEVICE" push "$TMP_DIR/product_base_model_FF2610043162.json" /data/res/json/
adb -s "$DEVICE" push "$TMP_DIR/product_base_model_FF261004316_1.json" /data/res/json/
adb -s "$DEVICE" push "$TMP_DIR/product_base_model_FF261004316_2.json" /data/res/json/
adb -s "$DEVICE" shell 'chmod 644 /data/res/img/codex_demo_0.jpg /data/res/img/codex_demo_1.jpg /data/res/json/product_base_model_FF261004316*.json && sync'
adb -s "$DEVICE" shell 'md5sum /data/res/img/codex_demo_0.jpg /data/res/img/codex_demo_1.jpg; ls -l /data/res/json/product_base_model_FF261004316*.json'
