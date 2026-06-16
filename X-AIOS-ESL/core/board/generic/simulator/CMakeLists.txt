set(BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mouse_cursor_icon.c
)

set(XOS_LIBNATIVE_DIR core/package/libnative)

set(BOARD_INC
  ${XOS_ROOT}/${XOS_GUIENG_DIR}/lvgl
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/conf
)

set(BOARD_LIB_DIR ${XOS_ROOT}/${XOS_LIBNATIVE_DIR})

execute_process(
  COMMAND bash -c 
  "if [ ! -d \"${CMAKE_BINARY_DIR}/bin\" ]; then
    mkdir -p \"${CMAKE_BINARY_DIR}/bin/\";
  fi"
  COMMAND bash -c 
  "if [ ! -d \"${CMAKE_BINARY_DIR}/lib\" ]; then
    mkdir -p \"${CMAKE_BINARY_DIR}/lib/\";
  fi"
  COMMAND bash -c 
  "if [ ! -d \"${CMAKE_BINARY_DIR}/res\" ]; then
    mkdir -p \"${CMAKE_BINARY_DIR}/res/\";
  fi;
  echo \"test_string_for_xc01_shell\""
  COMMAND bash -c 
  "if [ ! -d \"${CMAKE_BINARY_DIR}/res/ttf\" ]; then
    mkdir -p \"${CMAKE_BINARY_DIR}/res/ttf/\";
  fi"
)

execute_process(
  COMMAND bash -c "tar -xf ${XOS_ROOT}/core/board/generic/simulator/etc/DCIM.tar.xz -C ${CMAKE_BINARY_DIR}/res/"
  COMMAND bash -c "tar -xf ${XOS_ROOT}/core/board/generic/simulator/etc/rotties.tar.xz -C ${CMAKE_BINARY_DIR}/res/"
  # COMMAND bash -c "ln -s ${CMAKE_BINARY_DIR}/res/DCIM ${XOS_ROOT} 2>> /dev/null"
  COMMAND bash -c "cp -arf ${CMAKE_BINARY_DIR}/res/DCIM ${CMAKE_BINARY_DIR}/bin 2>> /dev/null"
)

#copy common ttf
if(CONFIG_XOS_USE_TINY_TTF AND NOT CONFIG_XOS_USE_APP_MOTOX)
execute_process(
  COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/res/ttf/2312_v9.ttf ${CMAKE_BINARY_DIR}/res/ttf/"
)
endif()

if(CONFIG_XOS_USE_TINY_TTF AND NOT CONFIG_XOS_USE_APP_MOTOX_RD)
execute_process(
  COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/res/ttf/2312_v9.ttf ${CMAKE_BINARY_DIR}/res/ttf/"
)
endif()

if(CONFIG_XOS_EXT_FREETYPE AND NOT CONFIG_XOS_USE_APP_MOTOX)
execute_process(
  COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/res/ttf/2312.ttf ${CMAKE_BINARY_DIR}/res/ttf/"
)
endif()

#override ttf if project has ttf.
if(CONFIG_XOS_PROJECT_TYPE)
set(XOS_APP_ENTRY ${CONFIG_XOS_PROJECT_TYPE})
MESSAGE("XOS_APP_ENTRY=${XOS_APP_ENTRY}")
  if(EXISTS "${XOS_ROOT}/core/res/${XOS_APP_ENTRY}")
    execute_process(
      COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/${XOS_APP_ENTRY}/* ${CMAKE_BINARY_DIR}/res/"
      COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/${XOS_APP_ENTRY}/ttf/* ${CMAKE_BINARY_DIR}/res/ttf/"
    )

    set(SOURCE_RES_DIR "${XOS_ROOT}/core/res/${XOS_APP_ENTRY}")
    if(DEFINED CONFIG_PRODUCT_KIDCAMERA_NOR OR 
      DEFINED CONFIG_PRODUCT_KIDCAMERA OR 
      DEFINED CONFIG_PRODUCT_AICAMERA OR 
      DEFINED CONFIG_PRODUCT_CAMERA)
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/res/launcher/")
        file(COPY "${SOURCE_RES_DIR}/" DESTINATION "${CMAKE_BINARY_DIR}/res/launcher/")
    endif()
  endif()
endif()

if (DEFINED CONFIG_XOS_USE_APP_SETTING)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/product/${XOS_APP_ENTRY}/*settings.json ${CMAKE_BINARY_DIR}/res/"
)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SPHOTO)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/sphoto/* ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: moto
if (DEFINED CONFIG_XOS_USE_APP_MOTO)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/moto/* ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: xiaoming
if (DEFINED CONFIG_XOS_APP_XIAOMING)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/xiaoming ${CMAKE_BINARY_DIR}/res/" 
)
endif()

#module: smarthome
if (DEFINED CONFIG_XOS_USE_APP_SMARTHOME)
execute_process(
    COMMAND bash -c "cp -r ${XOS_ROOT}/core/res/smarthome/* ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: videocall
if (DEFINED CONFIG_XOS_USE_APP_VIDEO_CALL)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/videocall/* ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: videocall
if (DEFINED CONFIG_XOS_USE_APP_AICAMERA)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/aicamera/* ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: launcher
if (DEFINED CONFIG_XOS_USE_APP_LAUNCHER_RES_OVERLAY)
  MESSAGE("CONFIG_XOS_USE_APP_LAUNCHER_RES_OVERLAY=${XOS_USE_APP_LAUNCHER_RES_OVERLAY}")
  if (DEFINED CONFIG_XOS_USE_APP_LAUNCHER)
    execute_process(
      COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/launcher ${CMAKE_BINARY_DIR}/res/"
      COMMAND bash -c "cp -arf ${XOS_ROOT}/product/${CONFIG_XOS_PROJECT_TYPE}/res_overlay/* ${CMAKE_BINARY_DIR}/res/"
    )
  endif()
else()
  if (DEFINED CONFIG_XOS_USE_APP_LAUNCHER)
    execute_process(
        COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/launcher ${CMAKE_BINARY_DIR}/res/"
    )
  endif()
endif()

#module: analogclock
if (DEFINED CONFIG_XOS_USE_APP_ANALOG_CLOCK)
execute_process(
    COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/analogclock/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/analogclock/* ${CMAKE_BINARY_DIR}/res/analogclock/"
)
endif()

#module: esl
if (DEFINED CONFIG_XOS_USE_APP_NATIVE_ESL)
execute_process(
  COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/h5esl/"
  COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/h5esl/* ${CMAKE_BINARY_DIR}/res/h5esl/"
  COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/h5esl/ttf/* ${CMAKE_BINARY_DIR}/res/ttf/"
)
endif()

#module: motox
if (DEFINED CONFIG_XOS_USE_APP_MOTOX)
execute_process(
    COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/motox/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/motox/* ${CMAKE_BINARY_DIR}/res/motox/"
)
endif()

#module: motox_rd
if (DEFINED CONFIG_XOS_USE_APP_MOTOX_RD)
execute_process(
    COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/motox/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/motox_rd/* ${CMAKE_BINARY_DIR}/res/motox/"
)
endif()

#module: motox7in
if (DEFINED CONFIG_XOS_USE_APP_MOTOX7IN)
execute_process(
    COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/motox7in/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/motox7in/* ${CMAKE_BINARY_DIR}/res/motox7in/"
)
endif()

#module: motot
if (DEFINED CONFIG_XOS_USE_APP_MOTOT)
execute_process(
    COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/motot/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/motot/* ${CMAKE_BINARY_DIR}/res/motot/"
)
endif()

#module: xdoor
if (DEFINED CONFIG_XOS_USE_APP_NATIVE_SMARTDOOR)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/smartdoor ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: lcmd
if (DEFINED CONFIG_XOS_FWK_TEST_LCMD)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/framework/quaextra/ability/test/lcmd/res_lcmd ${CMAKE_BINARY_DIR}/res/"
)
endif()

target_sources(${XOS_TARGET} PRIVATE ${BOARD_SRC})
target_include_directories(${XOS_TARGET} PRIVATE ${BOARD_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_TARGET} PUBLIC ${XOS_INTERFACE_DEF})

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  xos_app
  xos_res
  # lvgl_examples
  # lvgl_demos  
  # -lm -lpthread
  # -lrt
  # -lSDL2
)

target_link_directories(${XOS_TARGET} PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} quaextra)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/services/uart_service_linux/qm_rpc_protocol/src
)
if (DEFINED CONFIG_XOS_FWK_UART_SERVICE)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} uart_service)
endif()

if (DEFINED CONFIG_XOS_FWK_GUIENG_LVGL9)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  lvgl
)
endif()

if (DEFINED CONFIG_XOS_FWK_APPMANAGER)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibappmgr/ubuntu/libxos_appmanager.so ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibappmgr/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lxos_appmanager
)
endif()

if (DEFINED CONFIG_XOS_FWK_SYSMANAGER)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibsysmgr/ubuntu/libxos_sysmgr.so ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibsysmgr/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lxos_sysmgr
)
endif()

if (DEFINED CONFIG_XOS_HW_QUADRV)
execute_process(
  COMMAND bash -c "cp -arf ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquadrv/ubuntu/lib*.so ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquadrv/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lsysutils
)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/cloudadp/inc
)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_KUQI)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/inc/co/
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/inc/coapi/
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/demo/inc/
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/lib/libcoapi.a
)
endif()

#if (DEFINED CONFIG_XOS_FWK_CADP_QUA)
#target_include_directories(${XOS_TARGET} PUBLIC
#  ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/inc
#  ${XOS_ROOT}/core/package/core/package/framework/cloudadp/qua/quayun/MQTTPacket
#)
#endif()

if (DEFINED CONFIG_XOS_FWK_CADP_QUA)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  quacadp_qua
)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_AGORA)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibagora/ubuntu/libagora-rtc-sdk.so ${CMAKE_BINARY_DIR}/lib/"
)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibagora/include
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/agora/include
)


target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  quacadp_agora 
  -lagora-rtc-sdk
)
endif()



if (DEFINED CONFIG_XOS_FWK_QUAAITALK)
file(COPY 
  "${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibwebsockets/ubuntu/libwebsockets.so"
  "${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibwebsockets/ubuntu/libwebsockets.so.18"
  DESTINATION
  "${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibwebsockets/include
  ${XOS_ROOT}/core/package/framework/quaaitalk/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lwebsockets
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAAISERVICE)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnativeAlgs/qlibquaaiservice/ubuntu/libquaaiservice.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquaaiservice/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquaaiservice
)
endif()


if (DEFINED CONFIG_XOS_FWK_QUAIOTMGR)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnative/qlibquaiotmgr/ubuntu/libquaiotmgr.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVEALGS_DIR}/qlibquaiotmgr/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquaiotmgr
)
endif()

if (DEFINED CONFIG_XOS_FWK_PARAM)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibparam/ubuntu/lib*.so* ${CMAKE_BINARY_DIR}/lib/"
)
execute_process(
    COMMAND bash -c "mkdir -p ${CMAKE_BINARY_DIR}/res/config/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/product/${XOS_APP_ENTRY}/config/param.ini ${CMAKE_BINARY_DIR}/res/config/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibparam/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lxos_param
)
endif()

if (DEFINED CONFIG_XOS_EXT_HIREDIS)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lcrypto -lssl
)
endif()
if (DEFINED CONFIG_XOS_EXT_CJSON)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibcjson/ubuntu/*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibcjson/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lcjson
)
endif()

if (DEFINED CONFIG_XOS_EXT_CURL)
# execute_process(
#     COMMAND bash -c "cp -arf /usr/lib/x86_64-linux-gnu/libcurl.so* ${CMAKE_BINARY_DIR}/lib/"
# )
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibcurl/include
)
# find_package(curl REQUIRED)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lcurl
)
endif()

if (DEFINED CONFIG_XOS_FWK_EFFECTS)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibeffects/ubuntu/lib*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibeffects/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lqm_effects
)
endif()

if (DEFINED CONFIG_XOS_APP_ESL2)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquaesl/ubuntu/lib*.so ${CMAKE_BINARY_DIR}/lib/"
)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquaesl/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquaesl
)
endif()

if (DEFINED CONFIG_XOS_FWK_QMCORE)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibqmcore/ubuntu/lib*.so ${CMAKE_BINARY_DIR}/lib/"
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibjpegturbo/ubuntu/* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqmcore/include
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibjpegturbo/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lqmcore -lturbojpeg -ljpeg
)
endif()

if (DEFINED CONFIG_XOS_EXT_FFMPEG)
execute_process(
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibffmpeg/ubuntu/lib* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/ubuntu/libavformat.a
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/ubuntu/libavcodec.a
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/ubuntu/libavutil.a
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/ubuntu/libswscale.a
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/ubuntu/libswresample.a
  -lm -lz -lpng16
)
endif()

if (DEFINED CONFIG_XOS_EXT_FREETYPE)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibfreetype/ubuntu/libfreetype.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibfreetype/include
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibfreetype/include/freetype
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lfreetype
)
endif()

if (DEFINED CONFIG_XOS_EXT_LIBPNG)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibpng/ubuntu/libpng16.so* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibpng/ubuntu/libpng.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibpng/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lpng16
)
endif()

if (DEFINED CONFIG_XOS_EXT_JPEG)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibjpegturbo/ubuntu/* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibjpegturbo/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lturbojpeg -ljpeg
)
endif()

if (DEFINED CONFIG_XOS_EXT_BROWSER)
file(COPY 
  "${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqmbrowser/ubuntu/libqmbrowser.so"
  DESTINATION
  "${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqmbrowser/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lqmbrowser
)
endif()

if (DEFINED CONFIG_XOS_FWK_OTA_UART)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/services/inc
)
endif()

if (DEFINED CONFIG_XOS_FWK_MQTTSER_MIDDLE)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/services/mqtt_service
)
endif()

if (DEFINED CONFIG_XOS_EXT_QRCODE)
file(COPY 
  "${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqrcode/ubuntu/libZXing.so.2.2.1"
  DESTINATION
  "${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqrcode/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lZXing
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAAI)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquaAI/include
)

# target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
#   -lquaAI
# )
endif()

if (DEFINED CONFIG_XOS_FWK_QUACAMERA)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquacamera/include
)

# target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
#   -lquacamera
# )
endif()

if (DEFINED CONFIG_XOS_INN_QUAMISC)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquamisc/ubuntu/libquamisc.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquamisc/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquamisc
)
endif()

if (DEFINED CONFIG_XOS_INN_QUANTP)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquantp/ubuntu/libquantp.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquantp/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lquantp
)
endif()

if (DEFINED CONFIG_XOS_USE_INNER_RES)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibzlib/ubuntu/* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/libnative/qlibzlib/include
  ${XOS_ROOT}/product/$(${CONFIG_XOS_PROJECT_TYPE})/src/strings/src
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lz
)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SCREEN_PROTECT)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquadrv/ubuntu/libsysutils.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquadrv/include
)
if (EXISTS "${XOS_ROOT}/core/package/framework/services/lcd_service")
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/services/lcd_service
)
endif()
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lsysutils
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAOCR)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquaocr/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lqua_ocr
)
endif()

if (DEFINED CONFIG_XOS_EXT_RLOTTIE)
file(COPY 
  "${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibrlottie/ubuntu/librlottie.so"
  DESTINATION
  "${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibrlottie/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lrlottie
  -lc
)
endif()


if (DEFINED CONFIG_XOS_EXT_ZLIB)
execute_process(
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibzlib/ubuntu/libz*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibzlib/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lz
)
endif()

if (DEFINED CONFIG_XOS_HW_QUAMMAPI)
execute_process(
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquammapi/ubuntu_linux/*.so ${CMAKE_BINARY_DIR}/lib/"
)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquammapi/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquamm_utils
)
endif()

if (DEFINED CONFIG_XOS_EXT_SPEN)
file(COPY 
  "${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibspen/ubuntu/libspen.so"
  DESTINATION
  "${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibspen/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lspen
)
endif()

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
    -lrt
    -lpthread
    -lm
    -lSDL2
)
