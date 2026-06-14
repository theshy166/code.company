
if [ "$BUILD_SDK" != "simulator" ]; then
    echo "Warnning! wrong SDK is set, here must be simulator."
    _sdk_name_=simulator
    _board_name_=simulator
fi

export _top_img_out_dir_=$_sdk_name_

_xos_etc_path=core/board/generic/

_sdk_cross_compile=""

echo "_top_img_out_dir_=$_top_img_out_dir_"
echo ""

function build_xosui_so()
{
    echo "build_xosui_so"
    #build gui engine shared.
    make -f ./build/make/core/board.mk module=lvgl CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN all -j$BUILD_JOBS || exit "$$?"
    #build modules
    #common videoplayer imageviewer gallery fileexplorer videocall camera audioplayer facerecognize coffeemill health demolist smartdoor setting
    module_list="App"
    ldflag=""
    for m in $module_list;do
        ldflag="${ldflag};-l${m}"
        make -f ./build/make/core/board.mk module=$m CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN all -j$BUILD_JOBS || exit "$$?"
    done
    # build main app executable file.
    make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN -j$BUILD_JOBS || exit "$$?"
    make linkmodules=$ldflag CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ all -j$BUILD_JOBS || exit "$$?"
}

#param is 1 will exit; 0 will continue(for build all)
function build_xosui_default()
{
    echo "build_xosui_default"
    make -f ./build/make/core/board.mk CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $CLEAN all -j$BUILD_JOBS || exit "$$?"
}

function build_xosui_module()
{
    echo "build module $BUILD_MODULE only."
    if [ "$CLEAN" != "" ]; then
        rm -rf $_top_img_out_dir_/xos/
        make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ ${BUILD_MODULE}_${CLEAN} -j$BUILD_JOBS || exit "$$?"
    fi
    make CROSS_COMPILE=$_sdk_cross_compile BOARD=$_board_name_ $BUILD_MODULE -j$BUILD_JOBS || exit "$$?"
}

function build_xosui_system()
{
    if [ "$CLEAN" != "" ]; then
        rm -rf $_xos_img_out_dir_
    fi

    if [[ "$BUILD_SHARED_APP" = true ]];then
        build_xosui_so
    else
        build_xosui_default
    fi
}

function build_xosui()
{
    local __start_time __end_time __exec_time

    echo "build_xosui arg = "$1
    __start_time=$(date +%s)

    if [[ "$BUILD_MODULE" = "" ]];then
        build_xosui_system
    else
        build_xosui_module
    fi

    if [ $? -ne 0 ]; then
        echo "ERROR: BUILD simulator is error!."
    fi
        echo "SUCCESS: BUILD simulator is ok!!."

    __end_time=$(date +%s)
    __exec_time=$((__end_time - __start_time))
    echo "build xosui executed time: $__exec_time seconds"
}

function build_all()
{
    echo "build_all"
    build_xosui 0

    echo "End to build linux xos!"
    exit
}

if [[ "$BUILD_XOSUI" = true ]]; then
    echo "build ui only >> $CLEAN"
    build_xosui 1
    exit
fi

if [[ "$BUILD_ALL" = true ]]; then
    echo "build all cmd = true"
    build_all
fi

echo "build all default. $CLEAN"

build_all
