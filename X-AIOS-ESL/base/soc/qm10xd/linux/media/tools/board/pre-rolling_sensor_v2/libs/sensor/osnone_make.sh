
env=$(export)

export ARCH=arc

if [[ ! $OS_NONE_CROSS_COMPILE ]];then
    echo -e '\n请设置环境变量OS_NONE_CROSS_COMPILE，该值为当前编译器路径:\n'
    echo -e 'export OS_NONE_CROSS_COMPILE=xxxxx\n'
    exit
fi

if [[ $ISP_ROOT && $DSP_ROOT || $LIB_ROOT && $LIB_ROOT_EN == 1 ]];then
    make clean
    make
else
    echo -e '\n请设置ISP_ROOT和DSP_ROOT的路径:\n'
    echo '    export ISP_ROOT=/xxx/isp'
    echo '    export DSP_ROOT=/xxx/dsp'
    echo -e '\n或者设置LIB_ROOT'
    echo '    export LIB_ROOT_EN=1'
    exit 1
fi