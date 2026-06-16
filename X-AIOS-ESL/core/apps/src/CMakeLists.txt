set(LIB_NAME "appsrc")
set(LIB_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/xn_time.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/xos_misc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/appsmain.c
)

list(APPEND LIB_INC
  ${XOS_ROOT}/core/package/libnative/qlibparam/include
)


set(src_SRC ${LIB_SRC} PARENT_SCOPE)

add_library(${LIB_NAME} STATIC ${LIB_SRC})
target_link_libraries(${LIB_NAME} app_interface)

target_include_directories(${LIB_NAME} PUBLIC ${LIB_INC})

# MESSAGE(STATUS "XOS_INTERFACE_DEF src: ${INTERFACE_LIB_DEF}")
# MESSAGE(STATUS "XOS_INTERFACE_INC src: ${XOS_INTERFACE_INC}")
target_include_directories(${LIB_NAME} INTERFACE ${XOS_INTERFACE_INC})
target_compile_definitions(${LIB_NAME} INTERFACE ${XOS_INTERFACE_DEF})
