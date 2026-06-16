
set(LIB_NAME "xos_lang")
set(LIB_SRC 
  QuaStrPackage.c
)

set(LIB_INC 
  ${XOS_ROOT}/core/package/framework/quaextra/inc
  ${XOS_ROOT}/core/package/framework/quaextra/inc/lang
)

if (DEFINED CONFIG_OS_RTT)
add_library(${LIB_NAME} OBJECT ${LIB_SRC})
else()
add_library(${LIB_NAME} STATIC ${LIB_SRC})
endif()
target_include_directories(${LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC} ${LIB_INC})
target_compile_definitions(${LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})

if (DEFINED CONFIG_XOS_USE_APP_SPHOTO)
  target_link_libraries(${LIB_NAME} PUBLIC sphoto)
endif()

if (DEFINED CONFIG_XOS_EXT_CJSON)
  target_link_libraries(${LIB_NAME} PUBLIC -lcjson)
endif()

target_link_libraries(${LIB_NAME} PUBLIC xos_res)

