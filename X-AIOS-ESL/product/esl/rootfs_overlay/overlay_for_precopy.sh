#!/bin/sh
#this script will be called at current dir
script_path=$(dirname "$(readlink -f "$0")")
cd "$script_path"
echo $script_path/overlay_for_precopy.sh called
#add overlay accroding requirments
#install -vD ../config/param.ini ./config/param.ini
#install -vD ../config/display.ini ./config/display.ini
#install -vD  ../config/etc/init.d/S22config-para ./etc/init.d/S22config-para
#install -vD  ../config/etc/init.d/S99config-devs ./etc/init.d/S99config-devs

#install -vD $NATIVE_PATH/qlibquadrv/qm10xd_linux/libsysutils.so ./usr/lib/

if [[ "$CONFIG_XOS_USE_DEBUG" = "true" ]]; then
    touch .init_enable_core
fi
