set(LIB_NAME "xos_components")
file(GLOB LIB_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    images/*.c
    back_button.c
    cool_button.c
    qm_image_text.c
    qm_lv_obj.c
    qm_time_label.c
    qua_los_grad_arc.c
    qua_los_loading.c
    qua_los_msgpop.c
    qm_realtime_label.c
    qm_img_txt_flex.c
    qm_message_box.c
)

if (DEFINED CONFIG_OS_RTT)
add_library(${LIB_NAME} OBJECT ${LIB_SRC})
else()
add_library(${LIB_NAME} STATIC ${LIB_SRC})
endif()
target_include_directories(${LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})