set(LIB_NAME "esl2")
set(LIB_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/view/h5esl_native.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/h5esl_web.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/page_esl.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/page_overlay.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/page_footer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/qm_2_label.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/qm_3_label.c
  ${CMAKE_CURRENT_SOURCE_DIR}/view/qm_product_comp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/esl_sign.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/hashmap.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/md5.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/esl_json_utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/esl_ui_utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/str_util.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/common_service.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/esl_file.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/esl_service.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/esl_socket_service.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/esl_socket_udp_service.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/mqtt_control.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/mqtt_subscriber.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctrl/esl_message_queue.c
)
set(esl2_SRC ${LIB_SRC} PARENT_SCOPE)

add_library(${LIB_NAME} STATIC)
target_sources(${LIB_NAME} PRIVATE "${LIB_SRC}")
target_link_libraries(${LIB_NAME} app_interface)

target_include_directories(${LIB_NAME} PUBLIC ${XOS_INTERFACE_INC})
target_compile_definitions(${LIB_NAME} PUBLIC ${XOS_INTERFACE_DEF})

target_include_directories(${LIB_NAME} PUBLIC
  ${XOS_ROOT}/core/package/libnative/qlibcjson/include
  ${XOS_ROOT}/core/package/libnative/qlibquaesl/include
)

if (DEFINED CONFIG_XOS_FWK_CADP_CURL)
target_include_directories(${LIB_NAME} PUBLIC
  ${XOS_ROOT}/core/package/framework/quacurl/inc
)
endif()

if (DEFINED CONFIG_XOS_EXT_CURL)
target_include_directories(${LIB_NAME} PUBLIC
  ${XOS_ROOT}/core/package/libnative/qlibcurl/include
)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_QUA)
target_include_directories(${LIB_NAME} PUBLIC
  ${XOS_ROOT}/core/package/framework/cloudadp/inc
  ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/inc
  ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/MQTTPacket
)
endif()


#target_include_directories(${LIB_NAME} PUBLIC
#  ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
#)
