cmake_minimum_required(VERSION 3.13)

list(APPEND ALL_APP_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

list(APPEND XOS_APP_LIBS )

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory(src)
list(APPEND ALL_APP_SRC ${src_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:src>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_AIBOX AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/aibox")
add_subdirectory(aibox)
list(APPEND ALL_APP_SRC ${aibox_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:aibox>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_AICAMERA AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/aicamera")
add_subdirectory(aicamera)
list(APPEND ALL_APP_SRC ${aicamera_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:aicamera>)
list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}/core/package/libnative/qlibquacamera/include
  ${XOS_ROOT}/core/package/framework/quacamera/include
)
endif()

if (DEFINED CONFIG_XOS_USE_APP_ALARM AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/alarm")
add_subdirectory(alarm)
list(APPEND ALL_APP_SRC ${alarm_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:alarm>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_ANALOG_CLOCK AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/analogclock")
add_subdirectory(analogclock)
list(APPEND ALL_APP_SRC ${analogclock_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:analogclock>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_AUDIO_PLAYER AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/audioplayer")
add_subdirectory(audioplayer)
list(APPEND ALL_APP_SRC ${audioplayer_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:audioplayer>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_BOOT_ANIMATE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bootanimate")
add_subdirectory(bootanimate)
list(APPEND ALL_APP_SRC ${bootanimate_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:bootanimate>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_CALCULATOR AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/calculator")
add_subdirectory(calculator)
list(APPEND ALL_APP_SRC ${calculator_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:calculator>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_CALENDAR AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/calendar")
add_subdirectory(calendar)
list(APPEND ALL_APP_SRC ${calendar_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:calendar>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_CAMERA AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/camera")
add_subdirectory(camera)
list(APPEND ALL_APP_SRC ${camera_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:camera>)
endif()

if (DEFINED CONFIG_XOS_APP_XIAOMING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/xiaoming")
add_subdirectory(xiaoming)
list(APPEND ALL_APP_SRC ${xiaoming_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:xiaoming>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_CASTSCREEN AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/castscreen")
add_subdirectory(castscreen)
list(APPEND ALL_APP_SRC ${castscreen_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:castscreen>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_COFFEE_MILL AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/coffeemill")
add_subdirectory(coffeemill)
list(APPEND ALL_APP_SRC ${coffeemill_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:coffeemill>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_COOLFLORID AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/coolflorid")
add_subdirectory(coolflorid)
list(APPEND ALL_APP_SRC ${coolflorid_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:coolflorid>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_DEMOAPP AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/demoapp")
add_subdirectory(demoapp)
list(APPEND ALL_APP_SRC ${demoapp_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:demoapp>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_DEMOLIST AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/demolist")
add_subdirectory(demolist)
list(APPEND ALL_APP_SRC ${demolist_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:demolist>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_DIGITAL_CLOCK AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/digitalclock")
add_subdirectory(digitalclock)
list(APPEND ALL_APP_SRC ${digitalclock_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:digitalclock>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_FACE_RECOGNIZE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/facerecognize")
add_subdirectory(facerecognize)
list(APPEND ALL_APP_SRC ${facerecognize_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:facerecognize>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_GALLERY AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gallery")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquammapi/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibeffects/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibsysmgr/include)
add_subdirectory(gallery)
list(APPEND ALL_APP_SRC ${gallery_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:gallery>)
endif()

if ((DEFINED CONFIG_XOS_USE_APP_H5_ESL) OR (DEFINED CONFIG_XOS_USE_APP_NATIVE_ESL) AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/h5esl")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quacurl/inc)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibcurl/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quadevicesn/inc)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquammapi/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibzxing/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quaextra/logo)
add_subdirectory(h5esl)
list(APPEND ALL_APP_SRC ${h5esl_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:h5esl>)
endif()

if ((DEFINED CONFIG_XOS_APP_ESL2) AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/esl2")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quacurl/inc)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibcurl/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quadevicesn/inc)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquammapi/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibzxing/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quaextra/logo)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquaesl/include)
add_subdirectory(esl2)
list(APPEND ALL_APP_SRC ${esl2_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:esl2>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_HEALTH AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/health")
add_subdirectory(health)
list(APPEND ALL_APP_SRC ${health_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:health>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_IMAGEVIEWER AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imageviewer")
add_subdirectory(imageviewer)
list(APPEND ALL_APP_SRC ${imageviewer_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:imageviewer>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_LAUNCHER AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/launcher")
add_subdirectory(launcher)
list(APPEND ALL_APP_SRC ${launcher_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:launcher>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_LAYER_DEMO AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/layerdemo")
add_subdirectory(layerdemo)
list(APPEND ALL_APP_SRC ${layerdemo_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:layerdemo>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_LOTTIEDEMO AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lottiedemo")
add_subdirectory(lottiedemo)
list(APPEND ALL_APP_SRC ${lottiedemo_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:lottiedemo>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_MOTO AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/moto")
add_subdirectory(moto)
list(APPEND ALL_APP_SRC ${moto_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:moto>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_MOTOT AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/motot")
add_subdirectory(motot)
list(APPEND ALL_APP_SRC ${motot_SRC})
list(APPEND ALL_APP_INC ${CMAKE_CURRENT_SOURCE_DIR}/motot/ctrl/linux)
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:motot>)
list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
  ${XOS_ROOT}/core/package/libnative/qlibeffects/include
)
endif()

if (DEFINED CONFIG_XOS_USE_APP_MOTOX_RD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/motox_rd")
list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
  ${XOS_ROOT}/core/package/libnative/qlibeffects/include
  ${XOS_ROOT}/core/package/libnative/qlibjpegturbo/include
)
add_subdirectory(motox_rd)
list(APPEND ALL_APP_SRC ${motox_rd_SRC})
list(APPEND ALL_APP_INC ${CMAKE_CURRENT_SOURCE_DIR}/motox_rd/ctrl/core)
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:motox_rd>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_MOTOX102D AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/motox102d")
add_subdirectory(motox102d)
list(APPEND ALL_APP_SRC ${motox102d_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:motox102d>)
list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
  ${XOS_ROOT}/core/package/libnative/qlibeffects/include
  ${XOS_ROOT}/core/package/libnative/qlibjpegturbo/include
)
endif()

if (DEFINED CONFIG_XOS_USE_APP_MOTOX7IN AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/motox7in")
list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
  ${XOS_ROOT}/core/package/libnative/qlibqmcore/include
  ${XOS_ROOT}/core/package/libnative/qlibsysmgr/include
  ${XOS_ROOT}/core/apps/motox7in
)
add_subdirectory(motox7in)
list(APPEND ALL_APP_SRC ${motox7in_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:motox7in>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_QETV AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/qetv")
add_subdirectory(qetv)
list(APPEND ALL_APP_SRC ${qetv_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:qetv>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SCANPEN AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scanpen")
add_subdirectory(scanpen)
list(APPEND ALL_APP_SRC ${scanpen_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:scanpen>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SETTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/setting")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquantp/include)
add_subdirectory(setting)
list(APPEND ALL_APP_SRC ${setting_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:setting>)
endif()

if (DEFINED CONFIG_XOS_APP_SETTING_PAD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/settingpad")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquadrv/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibsysmgr/include)
add_subdirectory(settingpad)
list(APPEND ALL_APP_SRC ${settingpad_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:settingpad>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SMART_DOOR AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/smartdoor")
add_subdirectory(smartdoor)
list(APPEND ALL_APP_SRC ${smartdoor_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:smartdoor>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SMARTHOME AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/smarthome")
add_subdirectory(smarthome)
list(APPEND ALL_APP_SRC ${smarthome_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:smarthome>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_XHOME AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/xhome")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquammapi/include)
add_subdirectory(xhome)
list(APPEND ALL_APP_SRC ${xhome_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:xhome>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SPHOTO AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sphoto")
add_subdirectory(sphoto)
list(APPEND ALL_APP_SRC ${sphoto_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:sphoto>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_WIFIX AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/wifix")
add_subdirectory(wifix)
list(APPEND ALL_APP_SRC ${wifix_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:wifix>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_VIDEO_CALL AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/videocall")
#list(APPEND ALL_APP_LIB quacadp_agora)
add_subdirectory(videocall)
list(APPEND ALL_APP_SRC ${videocall_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:videocall>)
list(APPEND ALL_APP_INC ${videocall_INC})
endif()

if(DEFINED CONFIG_XOS_USE_APP_PETROBOT  AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/petrobot")
add_subdirectory(petrobot)
list(APPEND ALL_APP_INC ${petrobot_INC})
list(APPEND ALL_APP_SRC ${petrobot_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:petrobot>)
endif()

if(DEFINED CONFIG_XOS_USE_APP_MOMO_TALK  AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/momotalk")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquadrv/include)
add_subdirectory(momotalk)
list(APPEND ALL_APP_INC ${momotalk_INC})
list(APPEND ALL_APP_SRC ${momotalk_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:momotalk>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SCANNER AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scanner")
add_subdirectory(scanner)
list(APPEND ALL_APP_INC ${scanner_INC})
list(APPEND ALL_APP_SRC ${scanner_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:scanner>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_VIDEOPLAYER AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/videoplayer")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquammapi/include)
add_subdirectory(videoplayer)
list(APPEND ALL_APP_SRC ${videoplayer_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:videoplayer>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_AICHAT AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/aimodel")
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquacamera/include)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquammapi/include)
if (DEFINED CONFIG_XOS_FWK_QUAAI)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquaAI/include)
endif()
if (DEFINED CONFIG_XOS_USE_APP_STUDYPILOT)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/apps/studypilot/inc)
endif()
add_subdirectory(aimodel)
list(APPEND ALL_APP_SRC ${aimodel_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:aimodel>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_SETUPWIZARD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/setupwizard")
add_subdirectory(setupwizard)
list(APPEND ALL_APP_SRC ${setupwizard_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:setupwizard>)
endif()

if (DEFINED CONFIG_XOS_USE_APP_STUDYPILOT AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/studypilot")
add_subdirectory(studypilot)
list(APPEND ALL_APP_SRC ${studypilot_SRC})
list(APPEND ALL_TARGET_FILE $<TARGET_FILE:studypilot>)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_QUA OR DEFINED CONFIG_XOS_USE_APP_SCANNER)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/cloudadp/inc)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/inc)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/MQTTPacket)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_CURL)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quacurl/inc)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_DSN)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quadevicesn/inc)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAAITALK)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/framework/quaaitalk/include)
endif()

if (DEFINED CONFIG_XOS_EXT_CJSON)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibcjson/include)
endif()

if (DEFINED CONFIG_XOS_EXT_CURL)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibcurl/include)
endif()

if (DEFINED CONFIG_XOS_EXT_FVAD)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibfvad/include)
endif()

if (DEFINED CONFIG_XOS_FWK_PARAM)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibparam/include)
endif()

if (DEFINED CONFIG_XOS_FWK_EFFECTS)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibeffects/include)
endif()

if (DEFINED CONFIG_XOS_FWK_QUACAST)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquamedia/quacast/include)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAAISERVICE)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnativeAlgs/qlibquaaiservice/include)
endif()

if (DEFINED CONFIG_XOS_FWK_QUAIOTMGR)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquaiotmgr/include)
endif()

if (DEFINED CONFIG_XOS_FWK_DVR)
list(APPEND XOS_INTERFACE_INC ${XOS_ROOT}/core/package/libnative/qlibquamedia/dvr/include)
endif()
#MESSAGE(STATUS "ALL_APP_SRC=${ALL_APP_SRC}")
#MESSAGE(STATUS "ALL_TARGET_FILE=${ALL_TARGET_FILE}")
#######################
### create an interface target
#######################
set(INTERFACE_LIB_NAME "app_interface")
add_library(${INTERFACE_LIB_NAME} INTERFACE)

MESSAGE(STATUS "ALL_APP_INC=${ALL_APP_INC}")
target_include_directories(${INTERFACE_LIB_NAME} INTERFACE ${XOS_INTERFACE_INC} ${ALL_APP_INC})
target_compile_definitions(${INTERFACE_LIB_NAME} INTERFACE ${XOS_INTERFACE_DEF})
# target_compile_options(${INTERFACE_LIB_NAME} INTERFACE
#   -Wno-implicit-function-declaration
#   -Wno-incompatible-pointer-types
#   -Wno-discarded-qualifiers
# )

#######################
### create app target
#######################
set(LIB_NAME "xos_app")

if (1)
# methor 1: collect each app 's source as libapp.a 's source
add_library(${LIB_NAME} STATIC ${EMPTY_FILE})
target_sources(${LIB_NAME} PRIVATE ${ALL_APP_SRC})
target_link_libraries(${LIB_NAME} ${INTERFACE_LIB_NAME} ${ALL_APP_LIB})

target_include_directories(${LIB_NAME} PRIVATE ${ALL_APP_INC})

if (DEFINED CONFIG_XOS_FWK_UART_SERVICE_LINUX)
target_include_directories(${LIB_NAME} PUBLIC ${XOS_ROOT}/core/package/framework/services/uart_service_linux/qm_rpc_protocol/src)
MESSAGE(STATUS "-------------------------XOS_ROOT: ${XOS_ROOT}/core/package/framework/services/uart_service_linux/qm_rpc_protocol/src")
target_link_libraries(${LIB_NAME} ${XOS_INTERFACE_NAME} qm_rpc_protocol)
endif()

if(DEFINED CONFIG_XOS_FWK_OTA_UART)
target_include_directories(${LIB_NAME} PUBLIC ${XOS_ROOT}/core/package/framework/services/uart_service/inc)
endif()

else()
# methor 2: archieve all app libs into the whole libapp.a
# https://stackoverflow.com/questions/3821916/how-to-merge-two-ar-static-libraries-into-one
set(APP_AR_TARGET ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
add_custom_target(${LIB_NAME} ALL
  COMMAND ${CMAKE_AR} -rcT ${APP_AR_TARGET} ${ALL_TARGET_FILE}
)
endif()


#######################
### create app clean target
#######################
set(APPCLEAN_TARGET "appclean")

list(APPEND
  appclean_commands
  COMMAND ${CMAKE_COMMAND} -E echo "clean app target"
)

add_custom_target(${APPCLEAN_TARGET}
  ${appclean_commands}
)
