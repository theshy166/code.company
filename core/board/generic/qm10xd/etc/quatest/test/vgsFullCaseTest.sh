#=====================================================
#-----------------------VGS TEST----------------------
#=====================================================

:<<PARA
vgs_test [format] [filename] [inWidth] [inHeight] [outWidth] [outHeight] [enRotate] [bSysInit] [type]
type: 0 -> resize
      1 -> draw
      2 -> rotate
      3 -> decompress
      4 -> convert
      5 -> combine
      6 -> copy
      7 -> blend
PARA

###1. vgs resize test###
#:<<RESIZE
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 176 144 0 0 0
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 352 288 0 0 0
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 720 576 0 0 0
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 1280 720 0 0 0
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 1920 1080 0 0 0
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 3840 2160 0 0 0
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 4096 2160 0 0 0

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 176 144 0 0 0
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 352 288 0 0 0
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 0
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 1280 720 0 0 0
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 1920 1080 0 0 0
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 3840 2160 0 0 0
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 4096 2160 0 0 0

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 176 144 0 0 0
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 352 288 0 0 0
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 720 576 0 0 0
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1280 720 0 0 0
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 0
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 3840 2160 0 0 0
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 4096 2160 0 0 0

./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 176 144 0 0 0
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 352 288 0 0 0
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 720 576 0 0 0
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 1280 720 0 0 0
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 1920 1080 0 0 0
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 3840 2160 0 0 0
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 4096 2160 0 0 0

./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 176 144 0 0 0
./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 352 288 0 0 0
./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 720 576 0 0 0
./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 1280 720 0 0 0
./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 1920 1080 0 0 0
./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 3840 2160 0 0 0
./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 4096 2160 0 0 0
#RESIZE

###2. vgs draw test###
#:<<DRAW
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 352 288 0 0 1

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 1

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 1

./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 3840 2160 0 0 1

./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 4096 2160 0 0 1
#DRAW

###3. vgs rotate test###	[rotate]1:90 2:180 3:270
#:<<ROTATE
./vgs_test YUV420 res/color_sp420_352x288.yuv 352 288 288 352 1 0 2

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 2 0 2		#Y ok, C abnormal, nv16->nv12->rotate->nv16

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 576 720 3 0 2		#Y ok, C abnormal, nv16->nv12->rotate->nv16

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1080 1920 1 0 2

./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 3840 2160 2 0 2

./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 2160 4096 3 0 2

./vgs_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 576 720 1 0 2

./vgs_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 1280 720 2 0 2

./vgs_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1080 1920 3 0 2

#ROTATE

###4. vgs decompress test###
#:<<DECOMPRESS

#DECOMPRESS

###5. vgs format convert test###
#:<<CONVERT
./vgs_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 4 ARGB1555
./vgs_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 4 ARGB4444
./vgs_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 4 YUV420
./vgs_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 4 YUV422

./vgs_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 1280 720 0 0 4 ARGB1555
./vgs_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 1280 720 0 0 4 ARGB8888
./vgs_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 1280 720 0 0 4 YUV420
./vgs_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 1280 720 0 0 4 YUV422

./vgs_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1920 1080 0 0 4 ARGB4444
./vgs_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1920 1080 0 0 4 ARGB8888
./vgs_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1920 1080 0 0 4 YUV420
./vgs_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1920 1080 0 0 4 YUV422

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 4 ARGB1555
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 4 ARGB4444
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 4 ARGB8888
./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 4 YUV420

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 4 ARGB1555
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 4 ARGB4444
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 4 ARGB8888
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 4 YUV422

#TODO add YUV package format

#CONVERT

###6. vgs combine test###
#:<<COMBINE
./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 5

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 5
#COMBINE

###7. vgs copy test###
#:<<COPY
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 3840 2160 0 0 6

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 6

./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 4096 2160 0 0 6

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 720 576 0 0 6
#COPY

###8. vgs blend test###
#:<<BLEND
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 3840 2160 0 0 7

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 7

./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 4096 2160 0 0 7

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 1920 1080 0 0 7
#BLEND

###9. vgs osd test###
#:<<OSD
./vgs_test YUV420 res/tes_sp420_3840x2160.yuv 3840 2160 3840 2160 0 0 8

./vgs_test YUV420 res/lab_sp420_1920x1080.yuv 1920 1080 1920 1080 0 0 8

./vgs_test YUV420 res/vid_sp420_4096x2160.yuv 4096 2160 4096 2160 0 0 8

./vgs_test YUV422 res/situ_sp422_720x576.yuv 720 576 1920 1080 0 0 8
#OSD