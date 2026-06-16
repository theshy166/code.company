#=====================================================
#-----------------------TDE TEST----------------------
#=====================================================

:<<PARA
tde_test [format] [filename] [inWidth] [inHeight] [outWidth] [outHeight] [bCompress] [bSysInit] [type]
type: 0 -> resize
      1 -> draw
      2 -> copy
      3 -> fill
      4 -> deflicker
      5 -> bitblit
      6 -> compress
PARA

###1. tde resize test###
#:<<RESIZE
./tde_test ARGB1555 res/ARGB1555_2x2_4color.bits 2 2 8 8 0 0 0
./tde_test ARGB1555 res/ARGB1555_2x2_4color.bits 2 2 32 32 0 0 0
./tde_test ARGB1555 res/ARGB1555_2x2_4color.bits 2 2 64 102 0 0 0
./tde_test ARGB1555 res/ARGB1555_2x2_4color.bits 2 2 128 128 0 0 0
./tde_test ARGB1555 res/ARGB1555_2x2_4color.bits 2 2 512 512 0 0 0	#wkbuf 1024x1024

./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 16 16 0 0 0
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 64 64 0 0 0
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 126 254 0 0 0
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 256 256 0 0 0
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 1024 1024 0 0 0

./tde_test ARGB4444 res/ARGB4444_2x100_5color.bits 2 100 8 400 0 0 0
./tde_test ARGB4444 res/ARGB4444_2x100_5color.bits 2 100 32 1600 0 0 0
./tde_test ARGB4444 res/ARGB4444_2x100_5color.bits 2 100 128 1600 0 0 0
./tde_test ARGB4444 res/ARGB4444_2x100_5color.bits 2 100 254 1080 0 0 0	#wkbuf step
./tde_test ARGB4444 res/ARGB4444_2x100_5color.bits 2 100 512 1080 0 0 0	#wkbuf 1024x1080

./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 400 8 0 0 0
./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 1600 32 0 0 0
./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 1600 128 0 0 0
./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 254 512 0 0 0
./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 1920 512 0 0 0	#wkbuf 1920x1024

./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 4 4 0 0 0
./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 8 8 0 0 0
./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 256 256 0 0 0
./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 1024 1024 0 0 0
./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 1920 1080 0 0 0

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 32 32 0 0 0
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 0
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 180 144 0 0 0
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 1280 720 0 0 0
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 1920 1080 0 0 0

./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 480 270 0 0 0
./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 720 576 0 0 0
./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 960 540 0 0 0
./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 1920 1080 0 0 0
./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 3840 2160 0 0 0

./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 480 270 0 0 0
./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 720 576 0 0 0
./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 960 540 0 0 0
./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 3840 2160 0 0 0
./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 4096 4094 0 0 0

./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 480 270 0 0 0	#wkbuf 960x540
./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 960 540 0 0 0
./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 1280 720 0 0 0
./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 3840 2160 0 0 0
./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 4096 4094 0 0 0

./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 480 270 0 0 0
./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 960 540 0 0 0
./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 1920 1080 0 0 0
./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 3840 2160 0 0 0
./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 4096 4094 0 0 0
#RESIZE

###2. tde draw test###
#:<<DRAW
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 4 4 0 0 1

./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 100 2 0 0 1

./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 32 32 0 0 1

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 1

./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 1280 720 0 0 1

./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 1920 1080 0 0 1

./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 3840 2160 0 0 1

./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 4096 2160 0 0 1
#DRAW

###3. tde copy test###
#:<<COPY
./tde_test ARGB1555 res/ARGB1555_2x2_4color.bits 2 2 2 2 0 0 2

./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 4 4 0 0 2

./tde_test ARGB4444 res/ARGB4444_2x100_5color.bits 2 100 2 100 0 0 2

./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 100 2 0 0 2

./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 32 32 0 0 2

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 2

./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 1280 720 0 0 2

./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 1920 1080 0 0 2

./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 3840 2160 0 0 2

./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 4096 2160 0 0 2
#COPY

###4. tde fill test###
#:<<FILL
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 4 4 0 0 3

./tde_test ARGB1555 res/ARGB1555_100x2_5color.bits 100 2 100 2 0 0 3

./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 32 32 0 0 3

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 3

./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 1280 720 0 0 3

./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 1920 1080 0 0 3

./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 3840 2160 0 0 3

./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 4096 2160 0 0 3
#FILL

###5. tde deflicker test###
#:<<DEFLICKER
./tde_test ARGB8888 res/ARGB8888_4x4_4color.bits 4 4 4 4 0 0 4

./tde_test ARGB4444 res/ARGB4444_32x32_fish.bits 32 32 32 32 0 0 4

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 4

./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 1280 720 0 0 4

./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 1920 1080 0 0 4

./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 3840 2160 0 0 4

./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 4096 2160 0 0 4
#DEFLICKER

###6. tde bitblit test###
#:<<BITBLIT
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 5 50 50 30 30 2 2 30 30
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 5 50 50 64 64 2 2 30 30
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 5 50 50 128 128 2 2 30 30

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 5 50 50 64 64 0 0 64 64
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 5 50 50 126 126 0 0 64 64
./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 64 64 0 0 5 50 50 280 280 0 0 64 64

./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 64 64 0 0 5 50 50 30 30 2 2 30 30
./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 64 64 0 0 5 50 50 64 64 2 2 30 30 
./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 64 64 0 0 5 50 50 128 128 2 2 30 30

./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 64 64 0 0 5 50 50 64 64 0 0 64 64
./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 64 64 0 0 5 50 50 126 126 0 0 64 64 
./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 64 64 0 0 5 50 50 280 280 0 0 64 64

./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 50 50 30 30 2 2 30 30
./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 50 50 64 64 2 2 30 30
./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 50 50 128 128 2 2 30 30
./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 0 0 1920 1080 2 2 28 28	#wkbuf 1920x1080

./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 50 50 64 64 0 0 64 64
./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 50 50 126 126 0 0 64 64
./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 50 50 280 280 0 0 64 64
./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 64 64 0 0 5 460 40 1000 1000 16 2 16 16	#wkbuf 1000x1000
#BITBLIT

###7. tde compress test###
#:<<COMPRESS
./tde_test ARGB1555 res/ARGB1555_720x576_background.bits 720 576 720 576 0 0 6

./tde_test ARGB1555 res/ARGB1555_1024x768_background.bits 1024 768 1024 768 0 0 6

./tde_test ARGB1555 res/ARGB1555_1280x720_background.bits 1280 720 1280 720 0 0 6

./tde_test ARGB1555 res/ARGB1555_1920x1080_background.bits 1920 1080 1920 1080 0 0 6

./tde_test ARGB1555 res/ARGB1555_3840x2160_background.bits 3840 2160 3840 2160 0 0 6

./tde_test ARGB1555 res/ARGB1555_4096x2160_testCard.bits 4096 2160 4096 2160 0 0 6

./tde_test ARGB4444 res/ARGB4444_720x576_background.bits 720 576 720 576 0 0 6

./tde_test ARGB4444 res/ARGB4444_1024x768_background.bits 1024 768 1024 768 0 0 6

./tde_test ARGB4444 res/ARGB4444_1280x720_background.bits 1280 720 1280 720 0 0 6

./tde_test ARGB4444 res/ARGB4444_1920x1080_background.bits 1920 1080 1920 1080 0 0 6

./tde_test ARGB4444 res/ARGB4444_3840x2160_background.bits 3840 2160 3840 2160 0 0 6

./tde_test ARGB4444 res/ARGB4444_4096x2160_background.bits 4096 2160 4096 2160 0 0 6

./tde_test ARGB8888 res/ARGB8888_720x576_background.bits 720 576 720 576 0 0 6

./tde_test ARGB8888 res/ARGB8888_1024x768_background.bits 1024 768 1024 768 0 0 6

./tde_test ARGB8888 res/ARGB8888_1280x720_background.bits 1280 720 1280 720 0 0 6

./tde_test ARGB8888 res/ARGB8888_1920x1080_background.bits 1920 1080 1920 1080 0 0 6

./tde_test ARGB8888 res/ARGB8888_3840x2160_background.bits 3840 2160 3840 2160 0 0 6

./tde_test ARGB8888 res/ARGB8888_4096x2160_background.bits 4096 2160 4096 2160 0 0 6
#COMPRESS