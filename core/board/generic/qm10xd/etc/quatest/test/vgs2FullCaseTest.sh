#=====================================================
#-----------------------VGS2 TEST----------------------
#=====================================================

:<<PARA
vgs2_test [format] [filename] [inWidth] [inHeight] [outWidth] [outHeight] [mode] [prior]
type: 0 -> mosaic(PM)
      1 -> convert(CVT)
      2 -> rotate(ROT)
      3 -> combine(JNT)
      4 -> scale(SCL)
      5 -> cne(CNE)
      6 -> intg(INTG)
      7 -> add(ADD)
      8 -> mrg(MRG)
      9 -> osdConvert(OSD_CVT)
      10-> osdOl(OSD_OL)
PARA

###1. vgs mosaic(PM) test###
#:<<PM
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 0 0 0 0
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 0 0
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 0 1
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 0 2
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 0 3
#PM

###2. convert(CVT) test###
#:<<CVT
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 0 0 1 YU12
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 0 0 1 YUY2	#buf size check

./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 0 0 1 NV12
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 0 0 1 YUY2

./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 1280 720 0 0 1 NV12
./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 1280 720 0 0 1 YU12
#CVT

###3. rotate(ROT) test###	[rotate]1:90 2:180 3:270
#:<<ROT
./vgs2_test NV12 res/osd_1920x1088_1bit.yuv 1920 1080 1080 1920 1 0 2 0	#1:90° 0:1bit
./vgs2_test NV12 res/osd_1920x1088_2bit.yuv 1920 1080 1080 1920 1 0 2 1	#1:90° 1:2bit
./vgs2_test NV12 res/osd_1920x1088_4bit.yuv 1920 1080 1080 1920 1 0 2 2	#1:90° 2:4bit

./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 720 1280 1 0 2 3	#1:90° 3:8bit
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 2 0 2 3
./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 720 1280 3 0 2 3

./vgs2_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1080 1920 1 0 2 4	#1:90° 4:16bit
./vgs2_test ARGB8888 res/ARGB8888_1920x1080_background.bits 1920 1080 1080 1920 1 0 2 5	#1:90° 5:32bit
#ROT

###4. combine(JNT) test###
#:<<JNT
./vgs2_test BLK res/1280x720_mb.yuv 1280 720 2560 720 0 0 3
#./vgs2_test BLK res/4096x3072_mb.yuv 4096 3072 8192 3072 0 0 3
./vgs2_test NV12 res/4096x2160_nv12.yuv 4096 2160 8192 2160 0 0 3
#./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 2560 720 0 0 3
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 2560 720 0 0 3
./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 2560 720 0 0 3
#JNT

###5. scale(SCL) test###
#:<<SCL
#./vgs2_test BLK res/1280x720_mb.yuv 1280 720 1920 1080 0 0 4	//bLK not support, timeout
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 8192 8192 0 0 4
./vgs2_test YU12 res/512x384_card_yu12.yuv 512 384 1920 1080 0 0 4
#./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1920 1080 0 0 4	//3rd part no u data
./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 1920 1080 0 0 4

./vgs2_test RGB888 res/1280x720_lab_888.rgb 1280 720 1920 1080 0 0 4
./vgs2_test RAW res/Bayer_1920x1080_8bit.raw 1920 1080 720 576 0 0 4
#SCL

###6. cne(CNE) test###
#:<<CNE
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 0 0 5
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 5
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 0 0 5
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 1 0 5

#./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 1280 720 0 0 5	//YUYV not support,
#CNE

###7. intg(INTG) test###
#:<<INTG
./vgs2_test NV12 res/512x384_data_nv12.yuv 512 384 512 384 0 0 6
./vgs2_test YU12 res/512x384_data_yu12.yuv 512 384 512 384 0 0 6
#./vgs2_test YU12 res/512x384_card_yu12.yuv 512 384 512 384 0 0 6
./vgs2_test YUY2 res/512x384_data_yuy2.yuv 512 384 512 384 0 0 6
#INTG

###8. add(ADD) test###
#:<<ADD
./vgs2_test BLK res/1280x720_mb.yuv 1280 720 1280 720 0 0 7
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 7
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 2 0 7
./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 1280 720 3 0 7
#ADD

###9. mrg(MRG) test###
#:<<MRG
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 0 0 8
./vgs2_test NV12 res/1280x720_lab_nv12.yuv 1280 720 1280 720 1 0 8
./vgs2_test YU12 res/1280x720_lab_yu12.yuv 1280 720 1280 720 2 0 8
./vgs2_test YUY2 res/1280x720_lab_yuy2.yuv 1280 720 1280 720 3 0 8
#MRG

###10. osdConvert(OSD_CVT) test###
#:<<OSD_CVT

#OSD_CVT

###11. osdOl(OSD_OL) test###
#16bit:ARGB1555，ARGB4444,ARGB0565 32bit:ARGB8888
#:<<OSD_OL
./vgs2_test NV12 res/1920x1088_test_nv12.yuv 1920 1088 1920 1088 0 0 10 0	#0:fill 0:1bit
./vgs2_test NV12 res/1920x1088_test_nv12.yuv 1920 1088 1920 1088 1 0 10 1	#1:copy 1:2bit
./vgs2_test NV12 res/1920x1088_test_nv12.yuv 1920 1088 1920 1088 1 0 10 2	#1:copy 2:4bit

./vgs2_test NV12 res/1920x1088_test_nv12.yuv 1920 1088 1920 1088 0 0 10 3	#1:copy 3:8bit

./vgs2_test RGB res/1920x1088_test_nv12.yuv 1920 1088 1920 1088 1 0 10 4	#1:copy 4:16bit
./vgs2_test RGB res/1920x1088_test_nv12.yuv 1920 1088 1920 1088 1 0 10 5	#1:copy 5:32bit
#OSD_OL