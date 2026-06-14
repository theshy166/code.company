#!/bin/sh

#jpg decode of different size
./vdec_dump -t jpg -s jpg/32x32.jpg
./vdec_dump -t jpg -s jpg/64x64.jpg
./vdec_dump -t jpg -s jpg/128x128.jpg
./vdec_dump -t jpg -s jpg/256x256.jpg
./vdec_dump -t jpg -s jpg/384x288.jpg
./vdec_dump -t jpg -s jpg/704x576.jpg
./vdec_dump -t jpg -s jpg/1280x720.jpg
./vdec_dump -t jpg -s jpg/1920x1088.jpg
./vdec_dump -r 4096x2160 -t jpg -s jpg/2560x1440.jpg
./vdec_dump -r 4096x2160 -t jpg -s jpg/3840x2160.jpg
./vdec_dump -r 1920x1088 -t jpg -s jpg/baskdrive_1920x1080.jpg -d -1 (-d = -1 Jpegd DirectMode)

#different size of h264
./vdec_dump -t h264 -d 150 -s vdec_stream/1080P.h264.32x32.h264
./vdec_dump -t h264 -d 150 -s vdec_stream/1080P.h264.64x64.h264
./vdec_dump -t h264 -d 150  -s vdec_stream/1080P.h264.128x128.h264
./vdec_dump -t h264 -d 150  -s vdec_stream/1080P.h264.256x256.h264
./vdec_dump -t h264 -d 150  -s vdec_stream/1080P.h264.352x288.h264
./vdec_dump -t h264 -d 150  -s vdec_stream/1080P.h264.704x576.h264
./vdec_dump -t h264 -d 150  -s vdec_stream/1080P.h264.1280x720.h264
./vdec_dump -t h264 -d 300  -s vdec_stream/1080P.h264.1920x1080.h264
./vdec_dump -r 2560x1440  -t h264 -d 600 -v 3 -s vdec_stream/1080P.h264.2560x1440.h264
./vdec_dump -r 3840x2160  -t h264 -d 600 -v 3 -s vdec_stream/1080P.h264.3840x2160.h264
./vdec_dump -r 4096x2160  -t h264 -d 600 -v 3 -s vdec_stream/1080P.h264.4096x2160.h264
./vdec_dump -r 4096x4096  -t h264 -d 600 -v 3 -s vdec_stream/1080P.h264.4096x4096.h264

#h264 cabac
./vdec_dump -t h264 -d 300  -s vdec_stream/1080P.h264.1920x1080.cabac.h264
#h264 mutli-slice
./vdec_dump -t h264 -d 150  -s vdec_stream/clhj.h264


#different size of h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.32x32.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.64x64.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.128x128.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.128x128.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.128x128.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.256x256.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.352x288.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.704x576.h265
./vdec_dump -t h265 -d 150  -s vdec_stream/1080P.h264.1280x720.h265
./vdec_dump -t h265 -d 300  -s vdec_stream/1080P.h264.1920x1080.h265
./vdec_dump -r 2560x1440  -t h265 -d 600 -v 3 -s vdec_stream/1080P.h264.2560x1440.h265
./vdec_dump -r 3840x2160  -t h265 -d 600 -v 3 -s vdec_stream/1080P.h264.3840x2160.h265
./vdec_dump -r 4096x2160  -t h265 -d 600 -v 3 -s vdec_stream/1080P.h264.4096x2160.h265
./vdec_dump -r 4096x4096  -t h265 -d 600 -v 3 -s vdec_stream/1080P.h264.4096x4096.h265

#different ctu size of h265,echo h265mb=0 >/proc/umap/vdec
echo h265mb=0 >/proc/umap/vdec
./vdec_dump -t h265 -d 300  -s vdec_stream/1080P.h264.1920x1080.ctu16.h265
echo h265mb=1 >/proc/umap/vdec
./vdec_dump -t h265 -d 300  -s vdec_stream/1080P.h264.1920x1080.ctu32.h265
./vdec_dump -t h265 -d 300  -s vdec_stream/1080P.h264.1920x1080.ctu64.h265

#muti-tile of h265
./vdec_dump -t h265 -d 300  -s vdec_stream/mtiles_H265_1920_1080_ES.mp4_frames107.h265
./vdec_dump -t h265 -d 300  -s vdec_stream/ipc_frames100.h265


#2~4 DPB of h264 & h265
./vdec_dump -t h264  -v 2 -d 150 -s vdec_stream/normal/h264/vpu_veu_ch0_720x576_720x576_cmpno_cbr_noskip_norm.264
./vdec_dump -t h265  -v 2 -d 150 -s vdec_stream/normal/h265/vpu_veu_ch0_720x576_720x576_cmp50_cbr_noskip_norm.265
./vdec_dump -t h264  -v 3 -d 150 -s vdec_stream/jump/h264/vpu_veu_ch0_720x576_720x576_cmpno_avbr_skip4_norm.264
./vdec_dump -t h265  -v 3 -d 150 -s vdec_stream/jump/h265/vpu_veu_ch0_720x576_720x576_cmpno_avbr_skip4_norm.265
./vdec_dump -t h264  -v 3 -d 150 -s vdec_stream/smart/h264/vpu_veu_ch0_720x576_720x576_cmpno_cbr_noskip_smt.264
./vdec_dump -t h265  -v 3 -d 150 -s vdec_stream/smart/h265/vpu_veu_ch0_720x576_720x576_cmpno_cbr_noskip_smt.265
./vdec_dump -t h264  -v 4 -d 150 -s vdec_stream/smart_jump/h264/vpu_veu_ch0_720x576_720x576_cmpno_avbr_skip4_smt.264
./vdec_dump -t h265  -v 4 -d 150 -s vdec_stream/smart_jump/h265/vpu_veu_ch0_720x576_720x576_cmpno_avbr_skip4_smt.265


