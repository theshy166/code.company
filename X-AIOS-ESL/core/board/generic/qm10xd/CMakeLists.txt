list(APPEND BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/fbchk.c
)

message("===> Using RAW fbdev (no MM dependency)")
list(APPEND BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/fbdev_raw.c
)

if (DEFINED CONFIG_XOS_USE_DLMPI)
list(APPEND BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/fbmpi.c
)
endif()

if (DEFINED CONFIG_XOS_USE_EVDEV)
list(APPEND BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/evdev.c
)
endif()

if (DEFINED CONFIG_XOS_USE_DTEST)
list(APPEND BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/evdev.c
  ${CMAKE_CURRENT_SOURCE_DIR}/etc/fbtest.c
#  ${CMAKE_CURRENT_SOURCE_DIR}/upgbar.c
#  ${CMAKE_CURRENT_SOURCE_DIR}/etc/240x320_1_16.c
#  ${CMAKE_CURRENT_SOURCE_DIR}/etc/240x320_2_16.c
  ${CMAKE_CURRENT_SOURCE_DIR}/etc/240x320_1_32.c
  ${CMAKE_CURRENT_SOURCE_DIR}/etc/240x320_2_32.c
# ${CMAKE_CURRENT_SOURCE_DIR}/etc/raw_800x1280_5.c
# ${CMAKE_CURRENT_SOURCE_DIR}/etc/raw_800x1280_4.c
)
endif()

if (DEFINED CONFIG_XOS_USE_UVC)
list(APPEND BOARD_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/uvc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/uvc_preview.c
)
endif()

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

MESSAGE("CONFIG_XOS_BUILD_SDK=${CONFIG_XOS_BUILD_SDK}")
if (DEFINED CONFIG_XOS_FWK_QUAAI)
execute_process(
    COMMAND bash -c 
    "if [ -f \"${XOS_ROOT}/core/package/framework/quaAI/build.sh\" ]; then
      cd \"${XOS_ROOT}/core/package/framework/quaAI/\";
      ./build.sh qm10xv_linux || exit;
      cd -;
    fi"
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUACAMERA)
execute_process(
    COMMAND bash -c 
    "if [ -f \"${XOS_ROOT}/core/package/framework/quacamera/build.sh\" ]; then
      cd \"${XOS_ROOT}/core/package/framework/quacamera/\";
      ./build.sh qm10xv_linux || exit;
      cd -;
    fi"
)
endif()

#copy common ttf,
#override ttf if project has ttf.
if (DEFINED CONFIG_XOS_USE_TINY_TTF)
if (NOT DEFINED CONFIG_XOS_USE_APP_MOTOX102D)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/res/ttf/2312_v9.ttf ${CMAKE_BINARY_DIR}/res/ttf/"
)
endif()
endif()

if (DEFINED CONFIG_XOS_EXT_FREETYPE)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/res/ttf/2312.ttf ${CMAKE_BINARY_DIR}/res/ttf/"
)
endif()

set(XOS_APP_ENTRY ${CONFIG_XOS_PROJECT_TYPE})
MESSAGE("----------------------------------------------------XOS_APP_ENTRY=${XOS_APP_ENTRY}")
if(EXISTS "${XOS_ROOT}/core/res/${XOS_APP_ENTRY}")
    file(COPY "${XOS_ROOT}/core/res/${XOS_APP_ENTRY}" DESTINATION "${CMAKE_BINARY_DIR}/res/" )
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

#module: xhome
if (DEFINED CONFIG_XOS_USE_APP_XHOME)
if(XOS_APP_ENTRY STREQUAL "xhome" AND IS_DIRECTORY "${CMAKE_BINARY_DIR}/res/${XOS_APP_ENTRY}")
  message(STATUS "Removing directory: ${CMAKE_BINARY_DIR}/res/${XOS_APP_ENTRY}")
  file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/res/${XOS_APP_ENTRY}")
endif()

execute_process(COMMAND bash -c "cp -r ${XOS_ROOT}/core/res/xhome/* ${CMAKE_BINARY_DIR}/res/")
endif()

#module: videocall
if (DEFINED CONFIG_XOS_USE_APP_VIDEO_CALL)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/res/videocall/* ${CMAKE_BINARY_DIR}/res/"
)
endif()

#module: launcher
if (DEFINED CONFIG_XOS_USE_APP_LAUNCHER_RES_OVERLAY)
  MESSAGE("CONFIG_XOS_USE_APP_LAUNCHER_RES_OVERLAY=${XOS_USE_APP_LAUNCHER_RES_OVERLAY}")
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
)
endif()

target_sources(${XOS_TARGET} PRIVATE ${BOARD_SRC})
target_include_directories(${XOS_TARGET} PRIVATE ${BOARD_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_TARGET} PUBLIC ${XOS_INTERFACE_DEF})
target_compile_definitions(${XOS_TARGET} PRIVATE CONFIG_XOS_USE_FB_RAW CONFIG_XOS_USE_FB_DIRECT)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  xos_app
  xos_res
  # lvgl_examples
  # lvgl_demos  
  -lm -lpthread
)

target_link_directories(${XOS_TARGET} PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} quaextra)

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
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibappmgr/qm10xd_linux/lib*.so ${CMAKE_BINARY_DIR}/lib/"
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
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibsysmgr/qm10xd_linux/lib*.so ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibsysmgr/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lxos_sysmgr
)
endif()


if (DEFINED CONFIG_XOS_HW_QUADRV)
## -L libsysutils.so
if (DEFINED CONFIG_XOS_HW_QUADRV_EXT)
execute_process(
	COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquadrv/qm10xd_linux/ext/libsysutils.so* ${CMAKE_BINARY_DIR}/lib/"
)
else()
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquadrv/qm10xd_linux/libsysutils.so* ${CMAKE_BINARY_DIR}/lib/"
)
endif()
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
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibagora/qm10xd_linux/libagora-rtc-sdk.so ${CMAKE_BINARY_DIR}/lib/"
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

if (DEFINED CONFIG_XOS_USE_APP_NATIVE_ESL)

execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibzxing/qm10xd_linux/*.so*  ${CMAKE_BINARY_DIR}/lib/"
    
    #COMMAND bash -c "cp -arf ${XOS_ROOT}/tools/toolchain/arm-molchip-linux/arm-mol-linux-uclibcgnueabihf/sysroot/lib/libstdc++.so.6* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/tools/toolchain/arm-molv2-linux-uclibcgnueabi/arm-molv2-linux-uclibcgnueabi/sysroot/usr/lib/libstdc++.so.6* ${CMAKE_BINARY_DIR}/lib/"

)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibzxing/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lzxing
  -lzxwrapper
)
endif()

if (DEFINED CONFIG_XOS_APP_ESL2)

execute_process(
     COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibzxing/qm10xd_linux/*.so*  ${CMAKE_BINARY_DIR}/lib/"
     COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquaesl/qm10xd_linux/*.so*  ${CMAKE_BINARY_DIR}/lib/"
    
    #COMMAND bash -c "cp -arf ${XOS_ROOT}/tools/toolchain/arm-molchip-linux/arm-mol-linux-uclibcgnueabihf/sysroot/lib/libstdc++.so.6* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/tools/toolchain/arm-molv2-linux-uclibcgnueabi/arm-molv2-linux-uclibcgnueabi/sysroot/usr/lib/libstdc++.so.6* ${CMAKE_BINARY_DIR}/lib/"

)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibzxing/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lzxing
  -lzxwrapper
  -lquaesl
)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_KVS)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/aws-kvs/qm10xh_linux/shared_lib/* ${CMAKE_BINARY_DIR}/lib/"
)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/aws-kvs/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lkvsWebrtcClient -lkvsWebrtcSignalingClient -lkvsCommonLws -lkvspicUtils -lkvspicState -lwebsockets -lsrtp2 -lusrsctp -lssl -lcrypto
)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_KUQI)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/inc/co
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/inc/coapi
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/demo/inc
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  ${XOS_ROOT}/core/package/framework/cloudadp/aivendor/kuqi/coapi_uv/api/lib/libcoapi.a
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAAITALK)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnativeAlgs/qlibvoicealgo/qm10xd_linux/libhuwen.so ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnativeAlgs/qlibvoicealgo/qm10xd_linux/libhuwenwakeup.so ${CMAKE_BINARY_DIR}/lib/"
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  quaaitalk
  -lhuwenwakeup -lhuwen
)

if (DEFINED CONFIG_XOS_FWK_QUAAITALK_RTC)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnative/qlibagora/qm10xd_linux/libagora-rtc-sdk.so ${CMAKE_BINARY_DIR}/lib/"
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lagora-rtc-sdk
)
else()  #CONFIG_XOS_FWK_QUAAITALK_RTC
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnative/qlibwebsockets/qm10xd_linux/libwebsockets.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lwebsockets
)
endif() #CONFIG_XOS_FWK_QUAAITALK_RTC
endif() #CONFIG_XOS_FWK_QUAAITALK

if (DEFINED CONFIG_XOS_FWK_QUAIOTMGR)
execute_process(
    COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnative/qlibquaiotmgr/qm10xd_linux/libquaiotmgr.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVEALGS_DIR}/qlibquaiotmgr/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  quaiotmgr
)
endif()

if (DEFINED CONFIG_XOS_EXT_FVAD)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibfvad/qm10xd_linux/*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibfvad/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lfvad
)
endif()

if (DEFINED CONFIG_XOS_FWK_PARAM)
execute_process(
  COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibparam/qm10xd_linux/lib*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/libnative/qlibparam/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lxos_param
)
endif()

if (DEFINED CONFIG_XOS_EXT_SQLITE3)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/core/package/libnative/qlibsqlite3/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibsqlite3/qm10xd_linux/libsqlite3.a)
endif()

if (DEFINED CONFIG_XOS_EXT_CJSON)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibcjson/qm10xd_linux/*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibcjson/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lcjson
)
endif()

if (DEFINED CONFIG_XOS_EXT_CURL)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibcurl/qm10xd_linux/lib/libcurl.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibcurl/qm10xd_linux/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lcurl
)
endif()

if (DEFINED CONFIG_XOS_EXT_OPENSSL)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibopenssl/qm10xd_linux/lib/*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibopenssl/qm10xd_linux/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lssl -lcrypto
)
endif()

if (DEFINED CONFIG_XOS_FWK_EFFECTS)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibeffects/qm10xd_linux/lib*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibeffects/include
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquammapi/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
 -lqm_effects
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/base/soc/qm10xv/linux/media_support/include
)

endif()

if (DEFINED CONFIG_XOS_FWK_QMCORE)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibqmcore/qm10xd_linux/lib*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqmcore/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lqmcore
)
endif()

if (DEFINED CONFIG_XOS_EXT_FFMPEG)
execute_process(
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibffmpeg/qm10xd_linux/lib*.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_link_directories(${XOS_TARGET} PUBLIC ${BOARD_LIB_DIR}/qlibffmpeg/qm10xd_linux)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibffmpeg/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lavformat -lavcodec -lavutil -lswresample -lswscale
)
endif()

if (DEFINED CONFIG_XOS_EXT_FREETYPE)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibfreetype/arm/libfreetype.so* ${CMAKE_BINARY_DIR}/lib/"
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
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibpng/qm10xd-linux/libpng16.so* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibpng/qm10xd-linux/libpng.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibpng/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lpng
)
endif()

if (DEFINED CONFIG_XOS_EXT_JPEG)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibjpegturbo/qm10xd_linux/libturbojpeg.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibjpegturbo/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
    -lturbojpeg
)
endif()

if (DEFINED CONFIG_XOS_USE_LIB_JPEG)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibjpegturbo/qm10xd_linux/libjpeg.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibjpegturbo/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
    -ljpeg
)
endif()

if (DEFINED CONFIG_XOS_EXT_BROWSER)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibqmbrowser/arm/*.so ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/tools/toolchain/arm-molchip-linux/arm-mol-linux-uclibcgnueabihf/sysroot/lib/libstdc++.so.6* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "rm -rf ${CMAKE_BINARY_DIR}/bin/*.py"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibqmbrowser/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lqmbrowser
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAAI)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquaAI/xc01/libquaAI.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquaAI/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquaAI
)
endif()

if (DEFINED CONFIG_XOS_FWK_QUACAMERA)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquacamera/qm10xv/libquacamera.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquacamera/include
)
get_target_property(current_link_dirs ${XOS_TARGET} INTERFACE_LINK_DIRECTORIES)
list(APPEND current_link_dirs "${XOS_ROOT}/base/soc/qm10xv/linux/media_support/lib/dynamic")
target_link_directories(${XOS_TARGET} PUBLIC ${current_link_dirs})

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquacamera -ldbi -lisp -lispcore -ldci -ldsp -lvmm -lmipi -ladvapi_isp -lrt -lsensor -lsc3336_mipi -lgc2083_mipi -lc2599_mipi -lov02b10_mipi
)
endif()

if (DEFINED CONFIG_XOS_INN_QUAMISC)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquamisc/linux/libquamisc.so* ${CMAKE_BINARY_DIR}/lib/"
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
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquantp/qm10xd_linux/libquantp.so* ${CMAKE_BINARY_DIR}/lib/"
)
target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquantp/include
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lquantp
)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SCREEN_PROTECT)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquadrv/qm10xd_linux/libsysutils.so* ${CMAKE_BINARY_DIR}/lib/"
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

if (DEFINED CONFIG_XOS_EXT_RLOTTIE)
execute_process(
    COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibrlottie/arm/librlottie.so* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "cp -arf ${XOS_ROOT}/tools/toolchain/arm-molchip-linux/arm-mol-linux-uclibcgnueabihf/sysroot/lib/libstdc++.so.6* ${CMAKE_BINARY_DIR}/lib/"
    COMMAND bash -c "rm -rf ${CMAKE_BINARY_DIR}/bin/*.py"
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
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibzlib/qm10xd_linux/* ${CMAKE_BINARY_DIR}/lib/"
)
target_link_directories(${XOS_TARGET} PUBLIC ${BOARD_LIB_DIR}/qlibzlib/qm10xd_linux)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibzlib/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lz
)
endif()

if (DEFINED CONFIG_XOS_HW_QUAMMAPI)
execute_process(
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquammapi/qm10xd_linux/*.so ${CMAKE_BINARY_DIR}/lib/"
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquammapi/qm10xd_linux/quamm ${CMAKE_BINARY_DIR}/lib/"
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibyuv/qm10xd_linux/*.so ${CMAKE_BINARY_DIR}/lib/"
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibspng/qm10xd_linux/*.so ${CMAKE_BINARY_DIR}/lib/"
)
target_link_directories(${XOS_TARGET} PUBLIC ${BOARD_LIB_DIR}/qlibquammapi/qm10xd_linux)

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/${XOS_LIBNATIVE_DIR}/qlibquammapi/include
  ${XOS_ROOT}/base/soc/qm10xd/linux/media/mpp/include
)

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -l"quamm.player" -lquamm_gl -lquamm_common -lquamm_utils -lmetadata_retriever
)

if (DEFINED CONFIG_XOS_APP_XIAOMING)
execute_process(
   COMMAND bash -c "cp -arf ${XOS_ROOT}/core/package/libnativeAlgs/qlibvoicealgo/qm10xd_linux/*.so ${CMAKE_BINARY_DIR}/lib/"
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
  -lquamm_kwswakeup -lhuwenwakeup -lhuwen
)
endif() 
endif()

if (DEFINED CONFIG_XOS_PLAY_BOOTANIMATION)
execute_process(
   COMMAND bash -c "cp -arf ${BOARD_LIB_DIR}/qlibquammapi/qm10xd_linux/bin/* ${CMAKE_BINARY_DIR}/bin/"
)
target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME}
  -lbootanimation_client
)
endif()

target_include_directories(${XOS_TARGET} PUBLIC
  ${XOS_ROOT}/base/soc/qm10xd/linux/media/mpp/include/dsp
  ${XOS_ROOT}/base/soc/qm10xd/linux/media/mpp/include/types
  ${XOS_ROOT}/base/soc/qm10xd/linux/media/mpp/drv_include
  ${XOS_ROOT}/base/soc/qm10xd/linux/media/mpp/include
)


# 检查目录是否存在
set(DIR_TO_CHECK "${XOS_ROOT}/base/soc/qm10xd/linux/prebuilts/${CONFIG_XOS_PROJECT_TYPE}/media/lib")
file(GLOB FILES_IN_DIR "${DIR_TO_CHECK}/*")
if(FILES_IN_DIR)
set(PREBUILT_LIB_DIR "${XOS_ROOT}/base/soc/qm10xd/linux/prebuilts/${CONFIG_XOS_PROJECT_TYPE}/media/lib")
else()
set(PREBUILT_LIB_DIR "${XOS_ROOT}/base/soc/qm10xd/linux/prebuilts/common/media/lib")
endif()
message(STATUS "Using Media library directory: ${PREBUILT_LIB_DIR}")

target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
	${PREBUILT_LIB_DIR}/libmpi.a
)
