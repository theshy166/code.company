cmake_minimum_required(VERSION 3.0)

project(quacadp)


if (DEFINED CONFIG_XOS_FWK_CADP_AGORA)
set(SOURCE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/agora/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/agora/third-party/file_parser/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/agora/utility"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/agora/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/agora/third-party/file_parser/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/agora/third-party/json_parser/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../board/generic/xc00/conf"
    ${XOS_ROOT}/core/package/libnative/qlibquacamera/include
    ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
)

file(GLOB_RECURSE ALL_C_SOURCES "")
foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB CUR_DIR_SOURCES "${SOURCE_DIR}/*.c")
    list(APPEND ALL_C_SOURCES ${CUR_DIR_SOURCES})
endforeach()

MESSAGE("quacadp_agora source:${ALL_C_SOURCES}")

# *.a
add_library(quacadp_agora STATIC ${ALL_C_SOURCES})
target_include_directories(quacadp_agora PUBLIC ${INCLUDE_DIRS} ${XOS_INTERFACE_INC})
target_compile_definitions(quacadp_agora PUBLIC ${XOS_INTERFACE_DEF})
target_link_options(quacadp_agora PUBLIC ${XOS_INTERFACE_OPT})

endif()

if (DEFINED CONFIG_XOS_FWK_CADP_KVS)
set(SOURCE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/aws-kvs/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/aivendor/aws-kvs/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../board/generic/xc00/conf"
    ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
    ${XOS_ROOT}/core/package/libnative/qlibquacamera/include
)

file(GLOB_RECURSE ALL_C_SOURCES "")
foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB CUR_DIR_SOURCES "${SOURCE_DIR}/*.c")
    list(APPEND ALL_C_SOURCES ${CUR_DIR_SOURCES})
endforeach()

MESSAGE("quacadp_kvs source:${ALL_C_SOURCES}")

# *.a
add_library(quacadp_kvs STATIC ${ALL_C_SOURCES})
target_include_directories(quacadp_kvs PUBLIC ${INCLUDE_DIRS} ${XOS_INTERFACE_INC})
target_compile_definitions(quacadp_kvs PUBLIC ${XOS_INTERFACE_DEF})
target_link_options(quacadp_kvs PUBLIC ${XOS_INTERFACE_OPT})

endif()

if (DEFINED CONFIG_XOS_FWK_CADP_QUA)

list(APPEND XOS_QUACADP_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket
  ${XOS_ROOT}/core/package/libnative/qlibcjson/include
)

set(XOS_QUACADP_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cadp_aiskill.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cadp_aiskill_template.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/src/quaMqtt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/src/JsonParse.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/src/publish_msg_list.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/src/comInteraction.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTClient.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTConnectClient.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTConnectServer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTDeserializePublish.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTFormat.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTLinux.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTPacket.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTSerializePublish.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTSubscribeClient.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTSubscribeServer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTUnsubscribeClient.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua/quayun/MQTTPacket/MQTTUnsubscribeServer.c
)

list(APPEND XOS_QUACADP_LIBS quacadp_qua)
MESSAGE("XOS_QUACADP_LIBS=${XOS_QUACADP_LIBS}")

add_library(${XOS_QUACADP_LIBS} STATIC ${XOS_QUACADP_SRC})
target_include_directories(${XOS_QUACADP_LIBS} PRIVATE ${XOS_QUACADP_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUACADP_LIBS} PRIVATE ${XOS_INTERFACE_DEF})

set(XOS_QUACADP_LIB_NAME "xoscadp")
add_library(${XOS_QUACADP_LIB_NAME} STATIC ${EMPTY_FILE})
# MESSAGE(STATUS "XOS_INTERFACE_INC extra : ${XOS_INTERFACE_INC}")
target_link_libraries(${XOS_QUACADP_LIB_NAME} PRIVATE ${XOS_QUACADP_LIBS})
target_include_directories(${XOS_QUACADP_LIB_NAME} PRIVATE ${XOS_QUACADP_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUACADP_LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})

endif()