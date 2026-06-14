set(LIB_NAME "xos_font")
set(LIB_SRC 
  qua_fontv8.c
  qua_fontv9.c
  qua_font_ll.c
)

if (DEFINED CONFIG_OS_RTT)
add_library(${LIB_NAME} OBJECT ${LIB_SRC})
else()
add_library(${LIB_NAME} STATIC ${LIB_SRC})
endif()
target_include_directories(${LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})
