#!/bin/bash
if [ $# -lt 1 ]; then
    echo "Usage: $0 <chip_name> <os_name> <options>"
    echo "build qm10xh linux, $0 qm10xh linux"
    echo "build qm10xh rtt, $0 qm10xh rtt"
    echo "build mc331x linux, $0 mc331x linux"
    echo "build mc331x rtt, $0 mc331x rtt"
    echo "clean chip_platform output $0 chip_name os_name clean"
    exit 1
fi

export CHIP_NAME=$1
export OS_NAME=$2
OPTIONS=$3
export ROOT_DIR=$(pwd)
CUR_DIR=$ROOT_DIR
export COMMON_INC=$CUR_DIR/include
export MODULE_INC=$CUR_DIR/modules

if [ "$OPTIONS" == "clean" ]; then
    OUT_DIR=$CUR_DIR/out/${CHIP_NAME}_${OS_NAME}

    if [ -d "$OUT_DIR" ]; then
        rm -fr $OUT_DIR
    fi
    exit 1
fi

if [ $CHIP_NAME = mc331x ]; then
    export MPP_INC=$CUR_DIR/../../../../base/mc331x/linux/mpp/include
    export MPP_DRV_INC=$CUR_DIR/../../../../base/mc331x/linux/mpp/drv_include
    export LINK_LIB_INC_MPI=$CUR_DIR/../../../../base/mc331x/linux/prebuilt/lib
    export LINK_MPI_LIBS="-lmpi"
    export FFMPEG_INC=$CUR_DIR/../../external/ffmpeg/FFmpeg-release-6.1/output/$CHIP_NAME/$OS_NAME/include
    export FFMPEG_LIB_INC=$CUR_DIR/../../external/ffmpeg/FFmpeg-release-6.1/output/$CHIP_NAME/$OS_NAME/lib
    if [ $OS_NAME = linux ]; then
        export LINK_MPI_DEPENDENT_LIBS="-lisp -lispcore -lgc1054 -lg7xx"
        CROSS=arm-mol-linux-uclibcgnueabihf-
    else
        export FFMPEG_INC=$CUR_DIR/../../external/ffmpeg/FFmpeg-release-6.1/output/$CHIP_NAME/$OS_NAME/include
        export FFMPEG_LIB_INC=$CUR_DIR/../../external/ffmpeg/FFmpeg-release-6.1/output/$CHIP_NAME/$OS_NAME/lib
        export RTT_GNUC_BIN_PATH=$CUR_DIR/../../../../tools/toolchain/arm-linux-musleabi/bin
        export RTT_SCONS_LDS_PATH=$CUR_DIR/../../../../build/scons/link.lds
        export PATH=$RTT_GNUC_BIN_PATH:$PATH
        export MPP_INC=$CUR_DIR/../../../../base/mc331x/rtt/rtt_mpi/include
        export MPP_DRV_INC=$CUR_DIR/../../../../base/mc331x/rtt/rtt_mcdrivers/include
        export LINK_LIB_INC_MPI=$CUR_DIR/../../../../base/mc331x/rtt/rtt_mpi/build
        CROSS=arm-linux-musleabi-
    fi
fi

if [ $CHIP_NAME = qm10xh ]; then
    export MPP_INC=$CUR_DIR/../../../../base/qm10xh/linux/media/include
    export LINK_LIB_INC_MPI=$CUR_DIR/../../../../base/qm10xh/linux/output/common_lib
    export FFMPEG_INC=$CUR_DIR/../../external/ffmpeg/FFmpeg-release-6.1/output/$CHIP_NAME/$OS_NAME/include
    export FFMPEG_LIB_INC=$CUR_DIR/../../external/ffmpeg/FFmpeg-release-6.1/output/$CHIP_NAME/$OS_NAME/lib
    if [ $OS_NAME = linux ]; then
        export LINUX_GNUC_BIN_PATH=$CUR_DIR/../../../../tools/toolchain/arm-fhv512-linux/bin
        export PATH=$LINUX_GNUC_BIN_PATH:$PATH
        CROSS=arm-fhv512-linux-uclibcgnueabihf-
    else
        export RTT_GNUC_BIN_PATH=$CUR_DIR/../../../../tools/toolchain/arm-fhv512-linux/bin
        export PATH=$RTT_GNUC_BIN_PATH:$PATH
        CROSS=arm-fhv512-linux-uclibcgnueabihf-
    fi
   # export LINK_MPI_LIBS="-lmpi"
    #export LINK_MPI_DEPENDENT_LIBS="-laudio_codec -lfhses"
fi

export CC=${CROSS}gcc
export AR=${CROSS}ar

function build_sample() {
    SAMPLE_NAME=$1
    SAMPLE_SRC_PATH=sample/$SAMPLE_NAME
    SAMPLE_OUT_PATH=${CHIP_NAME}_${OS_NAME}/sample/$SAMPLE_NAME
    OUT_DIR=$CUR_DIR/out/$SAMPLE_OUT_PATH/
    mkdir -p $OUT_DIR
    cd $CUR_DIR/$SAMPLE_SRC_PATH
    make
}

function build_samples() {
    build_sample "audio"
    build_sample "system"
    build_sample "avext"
    build_sample "display/fb"
    build_sample "display/vo"
    build_sample "filter/tde"
    build_sample "filter/vgs"
    build_sample "video/dec"
    build_sample "video/enc"
    build_sample "pipeline/vdec_display"
    build_sample "pipeline/video_decoder"
    build_sample "pipeline/filter_display"
    build_sample "player"
    build_sample "camera"
}

function build_all() {
    mkdir -p out
    export OUT_DIR
if [ $CHIP_NAME = qm10xh ]; then
    export LINK_COMMON_LIB_INC=$CUR_DIR/qm10xh_linux
    export LINK_UTILS_LIB_INC=$CUR_DIR/qm10xh_linux
    export LINK_PLAYER_LIB_INC=$CUR_DIR/qm10xh_linux
elif [ $CHIP_NAME = mc331x ]; then
    if [ $OS_NAME = linux ];then
        export LINK_COMMON_LIB_INC=$CUR_DIR/fy00_linux
        export LINK_UTILS_LIB_INC=$CUR_DIR/fy00_linux
        export LINK_PLAYER_LIB_INC=$CUR_DIR/fy00_linux
    else
        export LINK_COMMON_LIB_INC=$CUR_DIR/rtt
        export LINK_UTILS_LIB_INC=$CUR_DIR/rtt
        export LINK_PLAYER_LIB_INC=$CUR_DIR/rtt
    fi
fi

    build_samples
}
build_all
