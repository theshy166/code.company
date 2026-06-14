#!/bin/bash

usage()
{
    echo "USAGE: [-L] [-D] [-U] [-K] [-A] [-V] [-?] [-H] [-h]      "
    echo "No ARGS means use default build option                   "
    echo "Option: -L = build xosui                                 "
    echo "        -D = build dynamic lib                           "
    echo "        -U = build uboot                                 "
    echo "        -K = build kernel                                "
    echo "        -A = build ALL                                   "
    echo "        -V = build with 'userdebug' or 'user'            "
    echo "        -S = build SDK fy00 'linux_128','linux_64','rtt',"
    echo "             'qm10xh_linux', 'qm10xv_linux','qm10xd_linux','simulator'"
    echo "        -B = build board 'fy00', 'fy00rtt'               "
    echo "        -X = build packages                              "
    echo "        -m = build storage medium                        "
    echo "        -P = build project: h5esl, scanpen, ...          "
    echo "        -F = build fastboot                              "
    echo "        -C = build with clean                            "
    echo "        -M = build module with 'guiengine', 'apps',      "
    echo "        -I = generate firmware images(*.img)             "
    echo "  -?/-h/-H = help information                            "
    exit 1
}

if [ "$#" -eq 1 ] && [ "${1:0:1}" != "-" ]; then
    source build/script/buildexternsion.sh $*
fi

export BUILD_CLEAN=false
export BUILD_UBOOT=false
export BUILD_KERNEL=false
export BUILD_ALL=false
export BUILD_XOSUI=false
export BUILD_PACKAGES=false
export BUILD_TYPE="userdebug"
export BUILD_SDK="linux_128"
export BUILD_JOBS=1     # =$(nproc)
export BUILD_SHARED_APP=false
export BUILD_FASTBOOT=false
export BUILD_PROJECT=""  # for porject board
export BUILD_MODULE=""
export BUILD_MEDIUM=""   # for storage medium

export _sdk_name_=""
export _board_name_=""
export _build_qm10xh_rtt_=false
export _build_qm10xh_fb_=""

# check pass argument
while getopts "LDUKAXFCm:P:V:S:j:J:B:M:I:hH?zZ" OPT
do
    case $OPT in
        L)
            echo "build xosui"
            BUILD_XOSUI=true
            ;;
        D)
            echo "build dynamic"
            BUILD_SHARED_APP=true
            ;;
        U)
            echo "build u-boot"
            BUILD_UBOOT=true
            ;;
        K)
            echo "build kernel"
            BUILD_KERNEL=true
            ;;
        A)
            echo "build all"
            BUILD_ALL=true
            ;;
        V)
            echo "build with $OPTARG"
            BUILD_TYPE=$OPTARG
            ;;
        S)
            echo "build SDK $OPTARG"
            BUILD_SDK=$OPTARG
            ;;
        J)
            echo "build CPU JOBS $OPTARG"
            BUILD_JOBS=$OPTARG
            ;;
        j)
            echo "build CPU JOBS $OPTARG"
            BUILD_JOBS=$OPTARG
            ;;
        B)
            echo "build BOARD $OPTARG"
            BUILD_BOARD=$OPTARG
            ;;
        m)
            echo "build STORAGE $OPTARG"
            BUILD_MEDIUM=$OPTARG
            ;;
        P)
            echo "build XOS project $OPTARG"
            BUILD_PROJECT=$OPTARG
            ;;
        X)
            echo "build xosui packages."
            BUILD_PACKAGES=true
            ;;
        F)
            echo "build fastboot"
            BUILD_FASTBOOT=true
            ;;
        C)
            if [ "$#" -eq 1 ]; then
                echo "clean generated files in out dir"
                rm -rf out/*
                exit 0
            else
                echo "build with clean"
                CLEAN=clean
            fi
            ;;
        M)
            echo "with with module"
            BUILD_MODULE=$OPTARG
            ;;
        I)
            echo "generate firmware images(*.img)"
            BUILD_FW_IMG=true
            ;;
        z|Z)
            echo "build clean all"
            BUILD_CLEAN=true
            ;;
        h|H)
            usage ;;
        ?)
            usage ;;
    esac
done

if [ "$BUILD_SDK" = "linux_128" ]; then
    _sdk_name_=fy00_linux
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=fy00
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4linux.sh
fi

if [ "$BUILD_SDK" = "linux_64" ]; then
    _sdk_name_=fy00_linux
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=fy00
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4linux.sh
fi

if [ "$BUILD_SDK" = "rtt" ]; then
    _sdk_name_=fy00_rtt
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=fy00rtt
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4rttos.sh
fi
if [ "$BUILD_SDK" = "qm10xh_linux" ]; then
    _sdk_name_=qm108h_sdk
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=qm10xh
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4qm108h.sh
fi

if [ "$BUILD_SDK" = "qm10xh_linuxrttv2" ]; then
    _sdk_name_=qm108h_sdk
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=qm10xh
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4qm108hv2.sh
fi

if [ "$BUILD_SDK" = "qm10xv_linux" ]; then
    _sdk_name_=qm102v_sdk
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=qm10xv
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4qm102v.sh
fi


if [ "$BUILD_SDK" = "qm10xd_linux" ]; then
    _sdk_name_=qm102d_sdk
    if [ -z "$BUILD_BOARD" ]; then
        _board_name_=qm10xd
    else
        _board_name_="$BUILD_BOARD"
    fi
    source build/script/build4qm102d.sh
fi

if [ "$BUILD_SDK" = "simulator" ]; then
    _sdk_name_=simulator
    _board_name_=simulator
    source build/script/build4simulator.sh
fi

if [ "$BUILD_SDK" = "rtt_qemu" ]; then
    _sdk_name_=fy00_rtt
    _board_name_=fy00rtt
    source build/script/build4rttqemu.sh
fi
