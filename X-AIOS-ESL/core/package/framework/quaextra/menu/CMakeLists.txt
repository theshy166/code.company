set(LIB_NAME "xos_menu")
set(LIB_SRC 
  menu_struct.c
  menu_view.c
)

set(LIB_INC 
  ${XOS_ROOT}/core/package/libnative/qlibcjson/include
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

if (DEFINED CONFIG_XOS_USE_APP_SETTING)
  target_link_libraries(${LIB_NAME} PUBLIC setting)
endif()

if (DEFINED CONFIG_XOS_FWK_PARAM)
  target_include_directories(${LIB_NAME} PUBLIC ${XOS_ROOT}/core/package/libnative/qlibparam/include)
endif()

if (DEFINED CONFIG_XOS_EXT_CJSON)
  target_link_libraries(${LIB_NAME} PUBLIC -lcjson)
endif()

target_link_libraries(${LIB_NAME} PUBLIC xos_lang)
