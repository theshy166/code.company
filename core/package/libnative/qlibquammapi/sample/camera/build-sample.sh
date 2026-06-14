#!/bin/bash

export PRODUCT_NAME=kidcamera_nor

if [ $# -lt 1 ]; then
    echo "Usage: $0 <chip_name> <os_name> <options>"
    echo "build qm10xh linux, $0 qm10xh linux"
    echo "build qm10xv linux, $0 qm10xv linux"
    echo "clean chip_os output $0 chip_name os_name clean"
    echo "@@@@@@ PRODUCT_NAME=$PRODUCT_NAME"
    exit 1
fi


export CHIP_NAME=$1
export OS_NAME=$2
OPTIONS=$3

CUR_DIR=$(pwd)
export SDK_TOP_DIR=${CUR_DIR}/../../../../../..
export QUA_LIBNATIVE_DIR=${SDK_TOP_DIR}/core/package/libnative
export COMMON_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/include
export DEBUG_CFLAGS="-g -O0"


if [ "$OPTIONS" == "clean" ]; then
    OUT_DIR=$CUR_DIR/out/${CHIP_NAME}_${OS_NAME}

    if [ -d "$OUT_DIR" ]; then
        rm -fr $OUT_DIR
    fi
    exit 1
fi

export FFMPEG_INC=${QUA_LIBNATIVE_DIR}/qlibffmpeg/include
export YUV_INC=${QUA_LIBNATIVE_DIR}/qlibyuv/include
export JPEG_TURBO_INC=${QUA_LIBNATIVE_DIR}/qlibjpegturbo/include
export VOICE_ALGO_INC=${QUA_LIBNATIVE_DIR}/qlibvoicealgo/include
export SPNG_INC=${QUA_LIBNATIVE_DIR}/qlibspng/include
export QUA_DRV_INC=${QUA_LIBNATIVE_DIR}/qlibquadrv/include


export YUV_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibyuv/${CHIP_NAME}_${OS_NAME}
export JPEG_TURBO_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibjpegturbo/${CHIP_NAME}_${OS_NAME}
export FFMPEG_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibffmpeg/${CHIP_NAME}_${OS_NAME}
export VOICE_ALGO_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibvoicealgo/${CHIP_NAME}_${OS_NAME}
export SPNG_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibspng/${CHIP_NAME}_${OS_NAME}
export QUA_DRV_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquadrv/${CHIP_NAME}_${OS_NAME}/ext


if [ $CHIP_NAME = qm10xh ]; then
	export MPP_INC=${SDK_TOP_DIR}/base/soc/qm10xh/linux/media/include
	export LINK_LIB_INC_MPI=${SDK_TOP_DIR}/base/soc/qm10xh/linux/output/common_lib
	if [ $OS_NAME = linux ]; then
		export LINUX_GNUC_BIN_PATH=${SDK_TOP_DIR}/tools/toolchain/arm-fhv512-linux/bin
		export PATH=$LINUX_GNUC_BIN_PATH:$PATH
		CROSS=arm-fhv512-linux-uclibcgnueabihf-
	else
		export RTT_GNUC_BIN_PATH=${SDK_TOP_DIR}/tools/toolchain/arm-fhv512-linux/bin
		export PATH=$RTT_GNUC_BIN_PATH:$PATH
		CROSS=arm-fhv512-linux-uclibcgnueabihf-
	fi
    export LINK_MPI_LIBS="-lmpi"
    export LINK_MPI_DEPENDENT_LIBS="-Wl,-Bstatic -laudio_codec -Wl,-Bdynamic -lfhses -lisp -lispcore"
fi

if [ $CHIP_NAME = qm10xv ]; then
    export MPP_INC=${SDK_TOP_DIR}/base/soc/qm10xv/linux/media_support/include
    export LINK_LIB_INC_MPI=${SDK_TOP_DIR}/base/soc/qm10xv/linux/prebuilts/${PRODUCT_NAME}/media/lib
    export LINUX_GNUC_BIN_PATH=${SDK_TOP_DIR}/tools/toolchain/arm-fhv512-linux/bin
    export PATH=$LINUX_GNUC_BIN_PATH:$PATH
    CROSS=arm-fhv512-linux-uclibcgnueabihf-
    export LINK_MPI_LIBS="-lvenc -lvo -ldsp -lacw_mpi -lvb_mpi -lvmm -ladvapi_osd"
fi

if [ $CHIP_NAME = qm10xd ]; then
    export MPP_INC=${SDK_TOP_DIR}/base/soc/qm10xd/linux/media/mpp/include
    export MPP_DRV_INC=${SDK_TOP_DIR}/base/soc/qm10xd/linux/media/mpp/drv_include
    export LINK_LIB_INC_MPI=${SDK_TOP_DIR}/base/soc/qm10xd/linux/media/mpp/prebuilt/media/lib
    export LINUX_GNUC_BIN_PATH=${SDK_TOP_DIR}/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin
    export PATH=$LINUX_GNUC_BIN_PATH:$PATH
    CROSS=arm-molv2-linux-uclibcgnueabi-
    export LINK_MPI_LIBS="-lmpi"
fi

export CC=${CROSS}gcc
export CXX=${CROSS}g++
export AR=${CROSS}ar
export STRIP=${CROSS}strip

function build_sample() {
    SAMPLE_NAME=$1
    SAMPLE_OUT_PATH=${CHIP_NAME}_${OS_NAME}/
    OUT_DIR=$CUR_DIR/out/$SAMPLE_OUT_PATH/
    mkdir -p $OUT_DIR
    make
}

export OUT_DIR
export LINK_COMMON_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}
export LINK_UTILS_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}
export LINK_PLAYER_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}
export LINK_KWSWAKEUP_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}
export LINK_PIPELINE_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}
export LINK_RETRIEVER_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}
export LINK_QUAGL_LIB_INC=${QUA_LIBNATIVE_DIR}/qlibquammapi/${CHIP_NAME}_${OS_NAME}

build_sample ""