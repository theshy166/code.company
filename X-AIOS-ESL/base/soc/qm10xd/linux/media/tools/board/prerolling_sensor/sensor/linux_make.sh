env=$(export)

export ARCH=arm

if [[ ! $CROSS_COMPILE ]];then
    echo -e '\n请设置环境变量CROSS_COMPILE，该值为当前编译器路径:\n'
    echo -e 'export CROSS_COMPILE=xxxxx\n'
    exit
fi

if [[ $SDK_DIR ]];then
    export ISP_ROOT=$SDK_DIR'/media_support'
    export DSP_ROOT=$SDK_DIR'/media_support'
    env=$(export)
fi

if [[ $ISP_ROOT && $DSP_ROOT ]];then
    make clean
    make
else
    echo -e '\n请设置ISP_ROOT和DSP_ROOT的路径:\n'
    echo '    export ISP_ROOT=/xxx/isp'
    echo '    export ISP_ROOT=/xxx/dsp'
    echo -e '\n或者设置SDK_DIR的路径，该路径为SDK包的路径\n'
    echo '    export SDK_DIR=xxxxxx'
    exit 1
fi