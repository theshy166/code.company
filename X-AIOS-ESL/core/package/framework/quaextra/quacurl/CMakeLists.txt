cmake_minimum_required(VERSION 3.0)

project(quacurl)


list(APPEND XOS_QUACURL_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
  ${XOS_ROOT}/core/package/libnative/qlibcurl/include
  ${XOS_ROOT}/core/package/libnative/qlibpng/include
  ${XOS_ROOT}/core/package/libnative/qlibjpegturbo/include
)

set(XOS_QUACURL_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/http_utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/qm_http_utils.c
)

list(APPEND XOS_QUACURL_LIBS quacurl)
MESSAGE("XOS_QUACURL_LIBS=${XOS_QUACURL_LIBS}")

if (DEFINED CONFIG_OS_RTT)
add_library(${XOS_QUACURL_LIBS} OBJECT ${XOS_QUACURL_SRC})
else()
add_library(${XOS_QUACURL_LIBS} STATIC ${XOS_QUACURL_SRC})
endif()

target_include_directories(${XOS_QUACURL_LIBS} PRIVATE ${XOS_QUACURL_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUACURL_LIBS} PRIVATE ${XOS_INTERFACE_DEF})

set(XOS_QUACURL_LIB_NAME "xoscurl")
add_library(${XOS_QUACURL_LIB_NAME} STATIC ${EMPTY_FILE})
# MESSAGE(STATUS "XOS_INTERFACE_INC extra : ${XOS_INTERFACE_INC}")
target_link_libraries(${XOS_QUACURL_LIB_NAME} PRIVATE ${XOS_QUACURL_LIBS})
target_include_directories(${XOS_QUACURL_LIB_NAME} PRIVATE ${XOS_QUACURL_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUACURL_LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})


