set(LIB_NAME "xos_player")
set(LIB_SRC 
  xosplayer.c
)

list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}/core/package/libnative/qlibquammapi/include
)
# MESSAGE(STATUS "XOS_INTERFACE_INC extra : ${XOS_INTERFACE_INC}")
if (DEFINED CONFIG_OS_RTT)
add_library(${LIB_NAME} OBJECT ${LIB_SRC})
else()
add_library(${LIB_NAME} STATIC ${LIB_SRC})
endif()
target_include_directories(${LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})
