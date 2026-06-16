#
# device_board.mk
# This file only used for:
# 1. Add flag for search header file and libraries
# 2. Copy config app's .so or Res
#


RELATIVE_BOARD_PATH = $(XOS_DIR)/$(XOS_DEVICE_DIR_NAME)/generic/$(BOARD)
PRJ_RESOURCE_PATH = $(XOS_DIR)/$(XOS_CORE_DIR)/res/$(XOS_APP_ENTRY)

#verify:
ifeq ($(BUILD_BIN_DIR),)
$(error "Var BUILD_BIN_DIR can not empty.")
endif

$(shell mkdir -p $(BUILD_DIR)/res)
$(shell mkdir -p $(BUILD_DIR)/res/ttf)
BUILD_DIR_LIB_PATH:=$(BUILD_DIR)/lib/
$(shell mkdir -p $(BUILD_DIR_LIB_PATH)/quamm/)

#unzip skin
$(shell if [ -f "$(XOS_DIR)/$(XOS_DEVICE_DIR_NAME)/$(XOS_APP_ENTRY)/$(XOS_APP_ENTRY).tar.xz" ];then tar -xf $(XOS_DIR)/$(XOS_DEVICE_DIR_NAME)/$(XOS_APP_ENTRY)/$(XOS_APP_ENTRY).tar.xz -C $(BUILD_DIR)/res/;fi)
#copy resource/ttf
ifeq ($(CONFIG_XOS_USE_TINY_TTF),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_CORE_DIR)/res/res/ttf/2312_v9.ttf $(BUILD_DIR)/res/ttf/)
endif
ifeq ($(CONFIG_XOS_EXT_FREETYPE),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_CORE_DIR)/res/res/ttf/2312.ttf $(BUILD_DIR)/res/ttf/)
endif
#override ttf if project has ttf.
$(shell cp -r $(PRJ_RESOURCE_PATH)/* $(BUILD_DIR)/res/)

#common:lib
LDFLAGS += -L$(BUILD_DIR_LIB_PATH)
LDFLAGS	+= -lm -lpthread

ifeq ($(CONFIG_XOS_HW_QUADRV),true)
## -L libsysutils.so
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquadrv/fy00_linux/libsysutils.so* ${BUILD_DIR_LIB_PATH})
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qligquadrv/include
LDFLAGS	+= -lsysutils
endif

ifeq ($(CONFIG_XOS_FWK_APPMANAGER),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibappmgr/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibappmgr/qm10xd_linux/* $(BUILD_DIR)/lib/)
LDFLAGS += -lxos_appmanager
endif

ifeq ($(CONFIG_XOS_FWK_SYSMANAGER),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibsysmgr/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibsysmgr/qm10xd_linux/* $(BUILD_DIR)/lib/)
LDFLAGS += -lxos_sysmgr
endif

#common base
ifeq ($(CONFIG_XOS_INN_QUAMISC),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquamisc/include
LDFLAGS	+= -lquamisc
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquamisc/linux/libquamisc.so* $(BUILD_DIR_LIB_PATH))
endif

ifeq ($(CONFIG_XOS_INN_QUANTP),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquantp/include
LDFLAGS	+= -lquantp
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquantp/linux/libquantp.so* $(BUILD_DIR_LIB_PATH))
endif

#module png
ifeq ($(CONFIG_XOS_USE_PNG),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/arm/libpng16.so* $(BUILD_DIR_LIB_PATH))
LDFLAGS += -lpng16 -lz
endif

ifeq ($(CONFIG_XOS_USE_INNER_RES),true)
override CFLAGS  += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/include
override CFLAGS  += -I$(XOS_DIR)/$(XOS_CORE_DIR)/res/strings
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/fy00_linux/* $(BUILD_DIR_LIB_PATH))
LDFLAGS += -lz
endif

#module: freetype
ifeq ($(CONFIG_XOS_EXT_FREETYPE),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibfreetype/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibfreetype/include/freetype
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibfreetype/arm/libfreetype.so* $(BUILD_DIR_LIB_PATH))
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/arm/libpng16.so* $(BUILD_DIR_LIB_PATH))
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/fy00_linux/libz.so* $(BUILD_DIR_LIB_PATH))

LDFLAGS += -lz -lpng16 -lfreetype -lc
endif

#module: tiny ttf
ifeq ($(CONFIG_XOS_USE_TINY_TTF),true)
endif

ifeq ($(CONFIG_XOS_HW_QUAMMAPI),true)
#module: effects, todo: remove ffmpeg
#ifeq ($(CONFIG_XOS_FWK_EFFECTS),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/include
override CFLAGS += -I$(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/include
LDFLAGS += -L$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/qm10xd_linux -l"quamm.player" -lquamm_common -lquamm_utils
LDFLAGS += -L$(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/prebuilt/media/lib -lmpi
#endif
endif

ifeq ($(CONFIG_XOS_BASE_QM10XD),true)
override CFLAGS += -I$(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/include/dsp
override CFLAGS += -I$(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/include/types
override CFLAGS += -I$(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/drv_include
override CFLAGS += -I$(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/include
MPI_LIBS := $(XOS_DIR)/base/soc/qm10xd/linux/media/mpp/prebuilt/media/lib/libmpi.a

LDFLAGS	+= $(MPI_LIBS)
endif

#module: qua_mm_player
ifeq ($(CONFIG_XOS_FWK_PLAYER),true)
LDFLAGS += -l"quamm.player" -lquamm_utils
LDFLAGS	+= -lavformat -lavcodec -lavutil -lswresample
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibffmpeg/fy00_linux/lib*.so* $(BUILD_DIR_LIB_PATH))
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/fy00_linux/lib*.so* $(BUILD_DIR_LIB_PATH))
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/fy00_linux/quamm/*.so* $(BUILD_DIR_LIB_PATH)/quamm)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibffmpeg/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/include
endif

#module: H5-browser
ifeq ($(CONFIG_XOS_USE_APP_H5_ESL),true)
ifeq ($(CONFIG_XOS_EXT_CURL),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibcurl/lib/libcurl.so* $(BUILD_DIR_LIB_PATH))
LDFLAGS += -lcurl
endif
ifeq ($(CONFIG_XOS_EXT_BROWSER),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmbrowser/arm/*.so $(BUILD_DIR_LIB_PATH))
$(shell cp -arf $(XOS_DIR)/tools/toolchain/arm-molchip-linux/arm-mol-linux-uclibcgnueabihf/sysroot/lib/libstdc++.so.6* $(BUILD_DIR_LIB_PATH))
$(shell rm $(BUILD_DIR_LIB_PATH)/*.py)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmbrowser/include
LDFLAGS += -lqmbrowser
endif
endif

ifeq ($(CONFIG_XOS_EXT_RLOTTIE),true)
LDFLAGS += -lrlottie
LDFLAGS += -L $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibrlottie/arm
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibrlottie/arm/librlottie.so* $(BUILD_DIR_LIB_PATH))
$(shell cp -arf $(XOS_DIR)/tools/toolchain/arm-molchip-linux/arm-mol-linux-uclibcgnueabihf/sysroot/lib/libstdc++.so.6* $(BUILD_DIR_LIB_PATH))
$(shell rm $(BUILD_DIR_LIB_PATH)/*.py)
endif

# module: uart service
ifeq ($(CONFIG_XOS_FWK_OTA_UART),true)
# LDFLAGS += $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmuart/fy00_rtt/libqmuart.a
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/services/inc
endif

##video call
ifeq ($(CONFIG_XOS_USE_APP_VIDEO_CALL),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/third-party/file_parser/include/
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/third-party/http_parser/include/
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/third-party/json_parser/include/
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/utility/
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/videocall/* $(BUILD_DIR)/res/)
ifeq ($(CONFIG_XOS_FWK_CADP_AGORA),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/include
LDFLAGS += -lagora-rtc-sdk
LDFLAGS += -L $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/qm10xh_linux
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/qm10xh_linux/libagora-rtc-sdk.so $(BUILD_DIR_LIB_PATH))
endif
endif

ifeq ($(CONFIG_XOS_EXT_CJSON),true)
LDFLAGS += -lcjson
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibcjson/include
$(shell cp -rf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibcjson/fy00_linux/*.so* $(BUILD_DIR_LIB_PATH) 2>> /dev/null)
endif

#module: setting
ifeq ($(CONFIG_XOS_USE_APP_SETTING),true)
$(shell cp -rf $(XOS_DIR)/$(XOS_PRODUCT_DIR)/$(XOS_APP_ENTRY)/settings.json $(BUILD_DIR)/res)
endif

#module: launcher
ifeq ($(CONFIG_XOS_USE_APP_LAUNCHER),true)
$(shell mkdir -p $(BUILD_DIR)/res/launcher/)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/launcher/* $(BUILD_DIR)/res/launcher/)
endif

#module: analogclock
ifeq ($(CONFIG_XOS_USE_APP_ANALOG_CLOCK),true)
$(shell mkdir -p $(BUILD_DIR)/res/analogclock/)
$(shell cp -rf $(XOS_DIR)/$(XOS_CORE_DIR)/res/analogclock/* $(BUILD_DIR)/res/analogclock/)
endif

ifeq ($(CONFIG_XOS_LCM_800x1280),true)
$(shell mkdir -p $(BUILD_DIR)/etc/temp/)
$(shell cp -arf $(RELATIVE_BOARD_PATH)/etc/lcm_base/lcm_800x1280/*.ko $(BUILD_DIR)/etc/temp/)
$(shell cp -arf $(RELATIVE_BOARD_PATH)/etc/lcm_base/lcm_800x1280/*.sh $(BUILD_DIR)/etc/temp/)
$(shell cp -arf $(RELATIVE_BOARD_PATH)/etc/lcm_base/lcm_800x1280/*.dts $(BUILD_DIR)/etc/temp/)
endif

ifeq ($(CONFIG_XOS_LCM_800x480),true)
$(shell mkdir -p $(BUILD_DIR)/etc/temp/)
$(shell cp -arf $(RELATIVE_BOARD_PATH)/etc/lcm_base/lcm_800x480/*.ko $(BUILD_DIR)/etc/temp/)
$(shell cp -arf $(RELATIVE_BOARD_PATH)/etc/lcm_base/lcm_800x480/*.sh $(BUILD_DIR)/etc/temp/)
$(shell cp -arf $(RELATIVE_BOARD_PATH)/etc/lcm_base/lcm_800x480/*.dts $(BUILD_DIR)/etc/temp/)
endif

ifeq ($(CONFIG_XOS_FWK_PARAM),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibparam/include
$(shell mkdir -p $(BUILD_DIR)/res/config/)
ifneq ($(XOS_APP_ENTRY), )
$(shell cp -r $(XOS_DIR)/$(XOS_PRODUCT_DIR)/$(XOS_APP_ENTRY)/config/param.ini $(BUILD_DIR)/res/config/)
endif
endif
