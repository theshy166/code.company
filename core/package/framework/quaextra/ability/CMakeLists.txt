set(LIB_NAME "xos_ability")

list(APPEND LIB_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/xos_ability.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua_sys_plat.c
  ${CMAKE_CURRENT_SOURCE_DIR}/qua_sys_para.c
)

if (DEFINED CONFIG_XOS_FWK_TEST_LCMD)
list(APPEND LIB_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/test/lcmd/qua_sys_lcmd_test.c
  ${CMAKE_CURRENT_SOURCE_DIR}/test/lcmd/qua_sys_lcmd_rttos.c
  ${CMAKE_CURRENT_SOURCE_DIR}/test/lcmd/qua_sys_lcmd_linux.c
)
endif()

if (DEFINED CONFIG_OS_RTT)
add_library(${LIB_NAME} OBJECT ${LIB_SRC})
else()
add_library(${LIB_NAME} STATIC ${LIB_SRC})
endif()
target_include_directories(${LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})
