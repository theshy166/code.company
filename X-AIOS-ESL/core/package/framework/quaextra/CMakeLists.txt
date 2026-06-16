cmake_minimum_required(VERSION 3.13)

list(APPEND XOS_QUAEXTRA_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

#why
if (DEFINED CONFIG_XOS_EXT_LIBPNG)
list(APPEND XOS_QUAEXTRA_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/../../libnative/qlibpng/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../libnative/qlibjpegturbo/include
)
endif()

list(APPEND XOS_QUAEXTRA_LIBS)

set(XOS_QUAEXTRA_LIB_NAME "quaextra")

set(XOS_QUAEXTRA_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

if (DEFINED CONFIG_XOS_FWK_ABILITY)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/ability)
list(APPEND XOS_QUAEXTRA_LIBS xos_ability)
endif()

if (DEFINED CONFIG_XOS_FWK_DEBUGGER)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/debugger)
list(APPEND XOS_QUAEXTRA_LIBS xos_debugger)
endif()

if (DEFINED CONFIG_XOS_FWK_FONT)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/font)
list(APPEND XOS_QUAEXTRA_LIBS xos_font)
endif()

if (DEFINED CONFIG_XOS_FWK_MENU)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/menu)
list(APPEND XOS_QUAEXTRA_LIBS xos_menu)
endif()

if (DEFINED CONFIG_XOS_FWK_COMPONENTS)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/components)
list(APPEND XOS_QUAEXTRA_LIBS xos_components)
endif()

if (DEFINED CONFIG_XOS_FWK_FLOATWINDOW)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/floatwindow)
list(APPEND XOS_QUAEXTRA_LIBS xos_floatwindow)
endif()

if (DEFINED CONFIG_XOS_INN_QUAMISC)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/lang)
list(APPEND XOS_QUAEXTRA_LIBS xos_lang)
endif()

if (DEFINED CONFIG_XOS_FWK_PLAYER)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/player)
list(APPEND XOS_QUAEXTRA_LIBS xos_player)
endif()

if (DEFINED CONFIG_XOS_FWK_UTILITY)
list(APPEND XOS_QUAEXTRA_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/../../libnative/qlibquammapi/include
)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/utility)
list(APPEND XOS_QUAEXTRA_LIBS xos_utility)
endif()

if (DEFINED CONFIG_XOS_FWK_LOGO)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/logo)
list(APPEND XOS_QUAEXTRA_LIBS xos_logo)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_CURL)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/quacurl)
list(APPEND XOS_QUAEXTRA_LIBS quacurl)
endif()

if (DEFINED CONFIG_XOS_FWK_CADP_DSN)
add_subdirectory_ifexists(${XOS_QUAEXTRA_DIR}/quadevicesn)
list(APPEND XOS_QUAEXTRA_LIBS quadsn)
list(APPEND XOS_QUAEXTRA_LIBS xosdevicesn)
endif()

MESSAGE("XOS_QUAEXTRA_LIBS=${XOS_QUAEXTRA_LIBS}")
MESSAGE("XOS_QUAEXTRA_DIR=${XOS_QUAEXTRA_DIR}")

if (DEFINED CONFIG_OS_RTT)
add_library(${XOS_QUAEXTRA_LIB_NAME} STATIC ${EMPTY_FILE})
# 添加所有子库作为依赖
foreach(sub_lib IN LISTS XOS_QUAEXTRA_LIBS)
    # 关键：使用INTERFACE_LINK_LIBRARIES
    target_link_libraries(${XOS_QUAEXTRA_LIB_NAME} PUBLIC ${sub_lib})
endforeach()
else()
add_library(${XOS_QUAEXTRA_LIB_NAME} STATIC ${EMPTY_FILE})
# MESSAGE(STATUS "XOS_INTERFACE_INC extra : ${XOS_INTERFACE_INC}")
target_link_libraries(${XOS_QUAEXTRA_LIB_NAME} PRIVATE ${XOS_QUAEXTRA_LIBS})
target_include_directories(${XOS_QUAEXTRA_LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUAEXTRA_LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})
endif()
