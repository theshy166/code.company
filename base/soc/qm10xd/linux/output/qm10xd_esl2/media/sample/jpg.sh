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

#jpg encode NV12 yuv(muti-pictures-150 pictures) of different size to mjpeg file
./jpege_test -f 420 -r 32x32  -i jpg_yuv/420/hs_h264_32x32.yuv  -o jpg_yuv/jpg/hs_h264_32x32.mjpg
./jpege_test -f 420 -r 64x64  -i jpg_yuv/420/hs_h264_64x64.yuv  -o jpg_yuv/jpg/hs_h264_64x64.mjpg
./jpege_test -f 420 -r 128x128  -i jpg_yuv/420/hs_h264_128x128.yuv  -o jpg_yuv/jpg/hs_h264_128x128.mjpg
./jpege_test -f 420 -r 256x256  -i jpg_yuv/420/hs_h264_256x256.yuv  -o jpg_yuv/jpg/hs_h264_256x256.mjpg
./jpege_test -f 420 -r 352x288  -i jpg_yuv/420/hs_h264_352x288.yuv  -o jpg_yuv/jpg/hs_h264_352x288.mjpg
./jpege_test -f 420 -r 384x240  -i jpg_yuv/420/hs_h264_384x240.yuv  -o jpg_yuv/jpg/hs_h264_384x240.mjpg
./jpege_test -f 420 -r 400x240  -i jpg_yuv/420/hs_h264_400x240.yuv  -o jpg_yuv/jpg/hs_h264_400x240.mjpg
./jpege_test -f 420 -r 416x240  -i jpg_yuv/420/hs_h264_416x240.yuv  -o jpg_yuv/jpg/hs_h264_416x240.mjpg
./jpege_test -f 420 -r 432x240  -i jpg_yuv/420/hs_h264_432x240.yuv  -o jpg_yuv/jpg/hs_h264_432x240.mjpg
./jpege_test -f 420 -r 640x480  -i jpg_yuv/420/hs_h264_640x480.yuv  -o jpg_yuv/jpg/hs_h264_640x480.mjpg
./jpege_test -f 420 -r 720x576  -i jpg_yuv/420/hs_h264_720x576.yuv  -o jpg_yuv/jpg/hs_h264_720x576.mjpg
./jpege_test -f 420 -r 1280x720  -i jpg_yuv/420/hs_h264_1280x720.yuv  -o jpg_yuv/jpg/hs_h264_1280x720.mjpg


#jpg encode blk yuv of different size
./jpege_test -f blk -r 400x240  -i jpg_yuv/blk/400x240_BLK.yuv  -o jpg_yuv/jpg/400x240_BLK.jpg
./jpege_test -f blk -r 416x240  -i jpg_yuv/blk/416x240_BLK.yuv  -o jpg_yuv/jpg/416x240_BLK.jpg
./jpege_test -f blk -r 432x240  -i jpg_yuv/blk/432x240_BLK.yuv  -o jpg_yuv/jpg/432x240_BLK.jpg
./jpege_test -f blk -r 720x576  -i jpg_yuv/blk/720x576_BLK.yuv  -o jpg_yuv/jpg/720x576_BLK.jpg
./jpege_test -f blk -r 1280x720  -i jpg_yuv/blk/1280x720_BLK.yuv  -o jpg_yuv/jpg/1280x720_BLK.jpg
./jpege_test -f blk -r 1920x1088  -i jpg_yuv/blk/1920x1088_BLK.yuv  -o jpg_yuv/jpg/1920x1088_BLK.jpg
./jpege_test -f blk -r 4096x3072  -i jpg_yuv/blk/4096x3072_BLK.yuv  -o jpg_yuv/jpg/4096x3072_BLK.jpg


#jpg encode tile encoded yuv of different size
./jpege_test -f tile192 -r 144x128  -i jpg_yuv/tile/144x128_tile192.yuv  -o jpg_yuv/jpg/144x128_tile192.jpg
./jpege_test -f tile224 -r 144x128  -i jpg_yuv/tile/144x128_tile224.yuv  -o jpg_yuv/jpg/144x128_tile224.jpg
./jpege_test -f tile256 -r 144x128  -i jpg_yuv/tile/144x128_tile256.yuv  -o jpg_yuv/jpg/144x128_tile256.jpg

./jpege_test -f tile192 -r 176x144  -i jpg_yuv/tile/176x144_tile192.yuv  -o jpg_yuv/jpg/176x144_tile192.jpg
./jpege_test -f tile224 -r 176x144  -i jpg_yuv/tile/176x144_tile224.yuv  -o jpg_yuv/jpg/176x144_tile224.jpg
./jpege_test -f tile256 -r 176x144  -i jpg_yuv/tile/176x144_tile256.yuv  -o jpg_yuv/jpg/176x144_tile256.jpg

./jpege_test -f tile192 -r 352x288  -i jpg_yuv/tile/352x288_TILE192.yuv  -o jpg_yuv/jpg/352x288_tile192.jpg
./jpege_test -f tile224 -r 352x288  -i jpg_yuv/tile/352x288_TILE224.yuv  -o jpg_yuv/jpg/352x288_tile224.jpg
./jpege_test -f tile256 -r 352x288  -i jpg_yuv/tile/352x288_TILE256.yuv  -o jpg_yuv/jpg/352x288_tile256.jpg

./jpege_test -f tile192 -r 384x320  -i jpg_yuv/tile/384x320_tile192.yuv  -o jpg_yuv/jpg/384x320_tile192.jpg
./jpege_test -f tile224 -r 384x320  -i jpg_yuv/tile/384x320_tile224.yuv  -o jpg_yuv/jpg/384x320_tile224.jpg
./jpege_test -f tile256 -r 384x320  -i jpg_yuv/tile/384x320_tile256.yuv  -o jpg_yuv/jpg/384x320_tile256.jpg

./jpege_test -f tile192 -r 640x480  -i jpg_yuv/tile/640x480_TILE192.yuv  -o jpg_yuv/jpg/640x480_tile192.jpg
./jpege_test -f tile224 -r 640x480  -i jpg_yuv/tile/640x480_TILE224.yuv  -o jpg_yuv/jpg/640x480_tile224.jpg
./jpege_test -f tile256 -r 640x480  -i jpg_yuv/tile/640x480_TILE256.yuv  -o jpg_yuv/jpg/640x480_tile256.jpg

./jpege_test -f tile192 -r 704x576  -i jpg_yuv/tile/704x576_tile192.yuv  -o jpg_yuv/jpg/704x576_tile192.jpg
./jpege_test -f tile224 -r 704x576  -i jpg_yuv/tile/704x576_tile224.yuv  -o jpg_yuv/jpg/704x576_tile224.jpg
./jpege_test -f tile256 -r 704x576  -i jpg_yuv/tile/704x576_tile256.yuv  -o jpg_yuv/jpg/704x576_tile256.jpg


./jpege_test -f tile192 -r 720x576  -i jpg_yuv/tile/720x576_tile192.yuv  -o jpg_yuv/jpg/720x576_tile192.jpg
./jpege_test -f tile224 -r 720x576  -i jpg_yuv/tile/720x576_tile224.yuv  -o jpg_yuv/jpg/720x576_tile224.jpg
./jpege_test -f tile256 -r 720x576  -i jpg_yuv/tile/720x576_tile256.yuv  -o jpg_yuv/jpg/720x576_tile256.jpg

./jpege_test -f tile192 -r 1280x768  -i jpg_yuv/tile/1280x768_tile192.yuv  -o jpg_yuv/jpg/1280x768_tile192.jpg
./jpege_test -f tile224 -r 1280x768  -i jpg_yuv/tile/1280x768_tile224.yuv  -o jpg_yuv/jpg/1280x768_tile224.jpg
./jpege_test -f tile256 -r 1280x768  -i jpg_yuv/tile/1280x768_tile256.yuv  -o jpg_yuv/jpg/1280x768_tile256.jpg

./jpege_test -f tile192 -r 1920x1088  -i jpg_yuv/tile/1920x1088_tile192.yuv  -o jpg_yuv/jpg/1920x1088_tile192.jpg
./jpege_test -f tile224 -r 1920x1088  -i jpg_yuv/tile/1920x1088_tile224.yuv  -o jpg_yuv/jpg/1920x1088_tile224.jpg
./jpege_test -f tile256 -r 1920x1088  -i jpg_yuv/tile/1920x1088_tile256.yuv  -o jpg_yuv/jpg/1920x1088_tile256.jpg


./jpege_test -f tile192 -r 3840x2160  -i jpg_yuv/tile/3840x2160_tile192.yuv  -o jpg_yuv/jpg/3840x2160_tile192.jpg
./jpege_test -f tile224 -r 3840x2160  -i jpg_yuv/tile/3840x2160_tile224.yuv  -o jpg_yuv/jpg/3840x2160_tile224.jpg
./jpege_test -f tile256 -r 3840x2160  -i jpg_yuv/tile/3840x2160_tile256.yuv  -o jpg_yuv/jpg/3840x2160_tile256.jpg


./jpege_test -f tile192 -r 4096x3072  -i jpg_yuv/tile/4096x3072_tile192.yuv  -o jpg_yuv/jpg/4096x3072_tile192.jpg
./jpege_test -f tile224 -r 4096x3072  -i jpg_yuv/tile/4096x3072_tile224.yuv  -o jpg_yuv/jpg/4096x3072_tile224.jpg
./jpege_test -f tile256 -r 4096x3072  -i jpg_yuv/tile/4096x3072_tile256.yuv  -o jpg_yuv/jpg/4096x3072_tile256.jpg

