#
# Makefile
#

#prepare resourse unzip

ifeq ($(BUILD_DIR),)
$(error BUILD_DIR is empty, something wrong)
endif

RELATIVE_BOARD_PATH = $(XOS_DIR)/$(XOS_DEVICE_DIR_NAME)/generic/$(BOARD)
PRJ_RESOURCE_PATH = $(XOS_DIR)/$(XOS_CORE_DIR)/res/$(XOS_APP_ENTRY)

$(shell mkdir -p $(BUILD_DIR)/res)
$(shell mkdir -p $(BUILD_DIR)/bin)
$(shell mkdir -p $(BUILD_DIR)/lib)
$(shell mkdir -p $(BUILD_DIR)/res/ttf)

$(shell tar -xf $(XOS_DIR)/$(RELATIVE_BOARD_PATH)/etc/DCIM.tar.xz -C $(BUILD_DIR)/res/)
$(shell tar -xf $(XOS_DIR)/$(RELATIVE_BOARD_PATH)/etc/rotties.tar.xz -C $(BUILD_DIR)/res/)
$(shell ln -s $(BUILD_DIR)/res/DCIM $(XOS_DIR) 2>> /dev/null)
$(shell cp -arf $(BUILD_DIR)/res/DCIM $(BUILD_DIR)/bin 2>> /dev/null)

#copy resource/ttf
ifeq ($(CONFIG_XOS_USE_TINY_TTF),true)
ifneq ($(CONFIG_XOS_USE_APP_MOTOX),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_CORE_DIR)/res/res/ttf/2312_v9.ttf $(BUILD_DIR)/res/ttf/)
endif
endif
ifeq ($(CONFIG_XOS_EXT_FREETYPE),true)
ifneq ($(CONFIG_XOS_USE_APP_MOTOX),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_CORE_DIR)/res/res/ttf/2312.ttf $(BUILD_DIR)/res/ttf/)
endif
endif
#override ttf if project has ttf.
$(shell [ -d "$PRJ_RESOURCE_PATH" ] && cp -r $(PRJ_RESOURCE_PATH)/* $(BUILD_DIR)/res/)

ifeq ($(CONFIG_XOS_USE_APP_NATIVE_ESL),true)
$(shell cp -arf $(XOS_DIR)/$(XOS_CORE_DIR)/res/h5esl $(BUILD_DIR)/res/)
endif

override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/services/uart_service_linux/qm_rpc_protocol/src

#basic
override CFLAGS	+= -D BUILD_SIMULATOR=1
override LDFLAGS += -lm -lpthread -lrt
override LDFLAGS += -L $(BUILD_DIR)/lib

lib_run_flags = -Wl,-rpath=$(BUILD_DIR)/lib:./simulator/lib:./lib:../lib:
override LDFLAGS += $(lib_run_flags)

override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibeffects/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibffmpeg/include

ifeq ($(CONFIG_XOS_FWK_APPMANAGER),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibappmgr/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibappmgr/ubuntu/lib*.so* $(BUILD_DIR)/lib/)
override LDFLAGS	+= -lxos_appmanager
endif

ifeq ($(CONFIG_XOS_FWK_SYSMANAGER),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibsysmgr/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibsysmgr/ubuntu/lib*.so* $(BUILD_DIR)/lib/)
override LDFLAGS	+= -lxos_sysmgr
endif

ifeq ($(CONFIG_XOS_INN_QUAMISC),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquamisc/include
override LDFLAGS += -lquamisc
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquamisc/ubuntu/libquamisc.so $(BUILD_DIR)/lib/)
endif

ifeq ($(CONFIG_XOS_INN_QUANTP),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquantp/include
override LDFLAGS += -lquantp
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquantp/ubuntu/libquantp.so $(BUILD_DIR)/lib/)
endif

ifeq ($(CONFIG_XOS_HW_QUADRV),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquadrv/include
override LDFLAGS += -lsysutils
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquadrv/ubuntu/libsysutils.so $(BUILD_DIR)/lib/)
endif

override LDFLAGS += -lSDL2

ifeq ($(CONFIG_XOS_USE_INNER_RES),true)
override CFLAGS  += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/include
ifeq ($(wildcard $(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/strings),)
override CFLAGS  += -I$(XOS_DIR)/$(XOS_CORE_DIR)/res/strings
else
override CFLAGS  += -I$(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/strings/src
endif
override LDFLAGS += -lz
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/ubuntu/*        $(BUILD_DIR)/lib/)
endif

#module: effects
ifeq ($(CONFIG_XOS_FWK_EFFECTS),true)
override LDFLAGS += -lqm_effects
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibeffects/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibeffects/ubuntu/lib*.so* $(BUILD_DIR)/lib/)
endif

#module: qmcore
ifeq ($(CONFIG_XOS_FWK_QMCORE),true)
override LDFLAGS += -lqmcore
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmcore/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmcore/ubuntu/lib*.so* $(BUILD_DIR)/lib/)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/ubuntu/* $(BUILD_DIR)/lib/)
override LDFLAGS += -lturbojpeg -ljpeg
endif

#module: freetype -lc 
ifeq ($(CONFIG_XOS_EXT_FREETYPE),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibfreetype/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibfreetype/include/freetype
override LDFLAGS += -lfreetype -lz -lpng16

$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibfreetype/ubuntu/*    $(BUILD_DIR)/lib/ 2>> /dev/null)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/ubuntu/*        $(BUILD_DIR)/lib/ 2>> /dev/null)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/ubuntu/*         $(BUILD_DIR)/lib/ 2>> /dev/null)
endif

#module: multimedia
# ifeq ($(CONFIG_XOS_FWK_PLAYER),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibffmpeg/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibeffects/include
override LDFLAGS +=  -lavformat -lavcodec -lavutil -lswscale -lswresample
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibffmpeg/ubuntu/*      $(BUILD_DIR)/lib/ 2>> /dev/null)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibzlib/ubuntu/*        $(BUILD_DIR)/lib/ 2>> /dev/null)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibpng/ubuntu/*         $(BUILD_DIR)/lib/ 2>> /dev/null)
override LDFLAGS += -lm -lz -lpng16
# endif

#module: turbo jpeg
ifeq ($(CONFIG_XOS_EXT_JPEG),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/include
override LDFLAGS += -lturbojpeg -ljpeg
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/ubuntu/libturbojpeg.so* $(BUILD_DIR)/lib/ 2>> /dev/null)
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/ubuntu/libjpeg.so* $(BUILD_DIR)/lib/ 2>> /dev/null)
endif

############### curl ###########################
ifeq ($(CONFIG_XOS_EXT_CURL),true)
override LDFLAGS += -lcurl
endif

# override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/include
# $(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/qm10xd_linux/*      $(BUILD_DIR)/lib/ 2>> /dev/null)
# override LDFLAGS += -L$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibjpegturbo/qm10xd_linux -lturbojpeg

ifeq ($(CONFIG_XOS_FWK_QUAAITALK),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/quaaitalk/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibwebsockets/ubuntu/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibwebsockets/ubuntu/libwebsockets.so* $(BUILD_DIR)/lib/ 2>> /dev/null)
override LDFLAGS += -lwebsockets
endif

ifeq ($(CONFIG_XOS_FWK_QUAIOTMGR),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquaiotmgr/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquaiotmgr/ubuntu/libquaiotmgr.so* $(BUILD_DIR)/lib/ 2>> /dev/null)
override LDFLAGS += -lquaiotmgr
endif

ifeq ($(CONFIG_XOS_FWK_QUAAISERVICE),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnativeAlgs/qlibquaaiservice/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnativeAlgs/qlibquaaiservice/ubuntu/libquaaiservice.so* $(BUILD_DIR)/lib/ 2>> /dev/null)
override LDFLAGS += -lquaaiservice
endif

ifeq ($(CONFIG_XOS_EXT_HIREDIS),true)
override LDFLAGS += -lcrypto
override LDFLAGS += -lssl
endif



############### copy so or res #################
ifeq ($(CONFIG_XOS_USE_APP_H5_ESL),true)
ifeq ($(CONFIG_XOS_EXT_BROWSER),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmbrowser/include
override LDFLAGS += -lqmbrowser
$(shell cp -rf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmbrowser/ubuntu/* $(BUILD_DIR)/lib/ 2>> /dev/null)
endif
endif

# module: uart service
ifeq ($(CONFIG_XOS_FWK_OTA_UART),true)
# override LDFLAGS += $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibqmuart/fy00_rtt/libqmuart.a
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/services/inc
endif

ifeq ($(CONFIG_XOS_FWK_MQTTSER_MIDDLE),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/services/mqtt_service
endif
#module: param
ifeq ($(CONFIG_XOS_FWK_PARAM),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibparam/include
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibparam/ubuntu/lib*.so* $(BUILD_DIR)/lib/)
override LDFLAGS	+= -lxos_param
$(shell mkdir -p $(BUILD_DIR)/res/config/)
ifneq ($(XOS_APP_ENTRY), )
$(shell cp -r $(XOS_DIR)/$(XOS_PRODUCT_DIR)/$(XOS_APP_ENTRY)/config/param.ini $(BUILD_DIR)/res/config/)
endif
endif

#module: moto
ifeq ($(CONFIG_XOS_USE_APP_MOTO),true)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/moto/* $(BUILD_DIR)/res/)
endif

#module: motox
ifeq ($(CONFIG_XOS_USE_APP_MOTOX),true)
$(shell cp -r  $(XOS_DIR)/$(XOS_CORE_DIR)/res/motox $(BUILD_DIR)/res/)
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/motox/
endif

#module: motox_rd
ifeq ($(CONFIG_XOS_USE_APP_MOTOX_RD),true)
$(shell cp -r  $(XOS_DIR)/$(XOS_CORE_DIR)/res/motox $(BUILD_DIR)/res/)
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/motox_rd/
endif

#module: motox7in
ifeq ($(CONFIG_XOS_USE_APP_MOTOX7IN),true)
$(shell cp -r  $(XOS_DIR)/$(XOS_CORE_DIR)/res/motox7in $(BUILD_DIR)/res/)
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/motox7in/
endif

#module: motot
ifeq ($(CONFIG_XOS_USE_APP_MOTOT),true)
$(shell cp -r  $(XOS_DIR)/$(XOS_CORE_DIR)/res/motot/ $(BUILD_DIR)/res/)
endif

#module: smarthome
ifeq ($(CONFIG_XOS_USE_APP_SMARTHOME),true)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/smarthome $(BUILD_DIR)/res/)
endif

#module: xiaoming
ifeq ($(CONFIG_XOS_APP_XIAOMING),true)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/xiaoming $(BUILD_DIR)/res/)
endif

##Rottie
ifeq ($(CONFIG_XOS_EXT_RLOTTIE),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibrlottie/include
override LDFLAGS += -lrlottie
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibrlottie/ubuntu/*     $(BUILD_DIR)/lib/ 2>> /dev/null)
endif

##video call
ifeq ($(CONFIG_XOS_USE_APP_VIDEO_CALL),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/third-party/file_parser/include/
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/third-party/http_parser/include/
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/third-party/json_parser/include/
override CFLAGS += -I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/videocall/utility/
override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibquacamera/include/
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/videocall/* $(BUILD_DIR)/res/)
ifeq ($(CONFIG_XOS_FWK_CADP_AGORA),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/include
override LDFLAGS += -lagora-rtc-sdk
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/ubuntu/*     $(BUILD_DIR)/lib/ 2>> /dev/null)
endif
endif

ifeq ($(CONFIG_XOS_USE_APP_MOMO_TALK),true)
override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibquacamera/include/
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/momotalk/* $(BUILD_DIR)/res/)
ifeq ($(CONFIG_XOS_FWK_CADP_AGORA),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/include
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibquammapi/include
override LDFLAGS += -lagora-rtc-sdk
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/ubuntu/*     $(BUILD_DIR)/lib/ 2>> /dev/null)
endif #CONFIG_XOS_FWK_CADP_AGORA
endif #CONFIG_XOS_USE_APP_MOMO_TALK

ifeq ($(CONFIG_XOS_EXT_CJSON),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibcjson/include
override LDFLAGS += -lcjson
$(shell cp -rf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibcjson/ubuntu/* $(BUILD_DIR)/lib/ 2>> /dev/null)
endif

ifeq ($(CONFIG_XOS_EXT_SQLITE3),true)
override CFLAGS += -I$(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibsqlite3/include
override LDFLAGS += -lsqlite3
$(shell cp -rf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibsqlite3/ubuntu/* $(BUILD_DIR)/lib/ 2>> /dev/null)
endif

#module: setting
ifeq ($(CONFIG_XOS_USE_APP_SETTING),true)
$(shell cp -rf $(XOS_DIR)/$(XOS_PRODUCT_DIR)/$(XOS_APP_ENTRY)/*settings.json $(BUILD_DIR)/res)
endif

#module: launcher
ifeq ($(CONFIG_XOS_USE_APP_LAUNCHER),true)
$(shell mkdir -p $(BUILD_DIR)/res/launcher/)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/launcher/* $(BUILD_DIR)/res/launcher/)
override CFLAGS += -I$(XOS_DIR)/core/package/framework/services/lcd_service
endif

#app: aichat
ifeq ($(CONFIG_XOS_USE_APP_AICHAT),true)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/aimodel/ $(BUILD_DIR)/res/)
ifeq ($(CONFIG_XOS_USE_APP_STUDYPILOT),true)
override CFLAGS += -I$(XOS_DIR)/core/apps/studypilot/inc
endif #CONFIG_XOS_USE_APP_STUDYPILOT
endif

ifeq ($(CONFIG_XOS_USE_APP_SPHOTO),true)
$(shell mkdir -p $(BUILD_DIR)/res/sphoto/)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/sphoto/* $(BUILD_DIR)/res/)
endif

#module: analogclock
ifeq ($(CONFIG_XOS_USE_APP_ANALOG_CLOCK),true)
$(shell mkdir -p $(BUILD_DIR)/res/analogclock/)
$(shell cp -rf $(XOS_DIR)/$(XOS_CORE_DIR)/res/analogclock/* $(BUILD_DIR)/res/analogclock/)
endif

#module: setupwizard
ifeq ($(CONFIG_XOS_USE_APP_SETUPWIZARD),true)
$(shell mkdir -p $(BUILD_DIR)/res/setupwizard/)
$(shell cp -rf $(XOS_DIR)/$(XOS_CORE_DIR)/res/setupwizard/* $(BUILD_DIR)/res/setupwizard/)
endif

ifeq ($(CONFIG_XOS_FWK_CADP),true)
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/inc/
ifeq ($(CONFIG_XOS_FWK_CADP_KUQI),true)
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/inc/co/
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/inc/coapi/
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/demo/inc/
KUQI_CADP_LIBS := $(XOS_DIR)/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/lib/libcoapi.a
LDFLAGS += $(KUQI_CADP_LIBS)
endif
ifeq ($(CONFIG_XOS_FWK_CADP_QUA),true)
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/qua/quayun/inc/
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/qua/quayun/MQTTPacket/
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/qua/quayun/MQTTPacket/inc
endif
ifeq ($(CONFIG_XOS_FWK_CADP_AGORA),true)
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/aivendor/agora/include
override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibagora/include
override CFLAGS += -I$(XOS_DIR)/core/package/framework/cloudadp/inc
LDFLAGS += -lagora-rtc-sdk
LDFLAGS += -L $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/qm10xh_linux
$(shell cp -arf $(XOS_DIR)/$(XOS_PKG_DIR)/libnative/qlibagora/qm10xh_linux/libagora-rtc-sdk.so $(BUILD_DIR_LIB_PATH))
endif
endif

ifeq ($(CONFIG_XOS_USE_APP_SCANNER),true)
override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibzxing/include
override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibquacamera/include/
#override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibcjson/include
#override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibcurl/include
override CFLAGS += -I$(XOS_DIR)/core/package/libnative/qlibparam/include
LDFLAGS += -L $(XOS_DIR)/core/package/libnative/qlibzxing/lib
LDFLAGS += -lzxing -lzxwrapper
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/scanner/skin/* $(BUILD_DIR)/res/skin/)
$(shell cp -arf $(XOS_DIR)/core/package/libnative/qlibzxing/lib/*.so $(BUILD_DIR)/lib/)
endif

ifeq ($(CONFIG_XOS_USE_APP_AICAMERA),true)
$(shell mkdir -p $(BUILD_DIR)/res/skin)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/aicamera/skin/* $(BUILD_DIR)/res/skin/)
$(shell cp -r $(XOS_DIR)/$(XOS_CORE_DIR)/res/kidcamera/skin/* $(BUILD_DIR)/res/skin/)
endif
