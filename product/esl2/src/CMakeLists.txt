
add_subdirectory(menu/src)
add_subdirectory(strings/src)

set(XOS_PRODUCT_LIB_NAME "xos_res")

#MESSAGE("STRINGS_SRC=${STRINGS_SRC}")
#MESSAGE("XOS_RES_STRINGS_SRC=${XOS_RES_STRINGS_SRC}")
add_library(${XOS_PRODUCT_LIB_NAME} STATIC ${STRINGS_SRC} ${MENU_SRC})

list(APPEND XOS_PRODUCT_INC
    ${XOS_ROOT}
    ${XOS_ROOT}/inc
)

target_include_directories(${XOS_PRODUCT_LIB_NAME} PRIVATE ${XOS_PRODUCT_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_PRODUCT_LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})