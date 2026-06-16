cmake_minimum_required(VERSION 3.16)
project(esl2_simulator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ── 路径 ──
set(XOS_ROOT "/home/luoxin/work/X-AIOS-ESL")
set(LVGL_DIR "${XOS_ROOT}/core/package/guiengine/lvgl-v9/lvgl")

# ── LVGL 配置 ──
set(LV_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h")
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ── 头文件路径 ──
include_directories(
    ${LVGL_DIR}
    ${LVGL_DIR}/src
    ${LVGL_DIR}/..
    ${XOS_ROOT}/core/apps/esl2
    ${XOS_ROOT}/core/apps/esl2/ctrl
    ${XOS_ROOT}/core/apps/esl2/utils
    ${XOS_ROOT}/core/apps/esl2/view
    ${XOS_ROOT}/core/apps/esl2/model
    ${XOS_ROOT}/core/package/framework/quaextra/inc
    ${XOS_ROOT}/core/package/libnative/qlibquaesl/include
    ${XOS_ROOT}/core/package/libnative/qlibfreetype/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ── SDL2 ──
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

# ── 收集 LVGL 源文件 ──
file(GLOB_RECURSE LVGL_SOURCES
    ${LVGL_DIR}/src/*.c
)
# 排除不需要的平台文件
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_fs_win32")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_fs_fatfs")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_fs_littlefs")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_fs_cbfs")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_fs_memfs")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_draw_vg_lite")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_draw_dave2d")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_draw_buf_vg_lite")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_draw_vglite")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_ffmpeg")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_libjpeg_turbo")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_rtthread")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_freertos")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_cmsis_rtos2")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "lv_windows")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "nxp/")
list(FILTER LVGL_SOURCES EXCLUDE REGEX "draw/sdl")

# ── ESL2 源文件 ──
set(ESL2_SOURCES
    ${XOS_ROOT}/core/apps/esl2/utils/md5.c
    ${XOS_ROOT}/core/apps/esl2/utils/hashmap.c
    ${XOS_ROOT}/core/apps/esl2/utils/str_util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xos_stub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

# ── Ubuntu 预编译库 ──
set(UBUNTU_DIR "${XOS_ROOT}/core/package/libnative")

set(UBUNTU_LIBS
    ${UBUNTU_DIR}/qlibeffects/ubuntu/libqm_effects.so
    ${UBUNTU_DIR}/qlibcjson/ubuntu/libcjson.so
    ${UBUNTU_DIR}/qlibquaesl/ubuntu/libquaesl.so
    ${UBUNTU_DIR}/qlibfreetype/ubuntu/libfreetype.so
)

# ── 可执行文件 ──
add_executable(esl2_sim
    ${LVGL_SOURCES}
    ${ESL2_SOURCES}
)

target_link_directories(esl2_sim PRIVATE ${SDL2_LIBRARY_DIRS})
target_link_libraries(esl2_sim PRIVATE
    ${SDL2_LIBRARIES}
    ${UBUNTU_LIBS}
    curl
    m
    pthread
    dl
    rt
)

# ── 设置 rpath 让运行时能找到 .so ──
set_target_properties(esl2_sim PROPERTIES
    BUILD_RPATH "${UBUNTU_DIR}/qlibeffects/ubuntu;${UBUNTU_DIR}/qlibcjson/ubuntu;${UBUNTU_DIR}/qlibquaesl/ubuntu;${UBUNTU_DIR}/qlibfreetype/ubuntu"
    INSTALL_RPATH "${UBUNTU_DIR}/qlibeffects/ubuntu;${UBUNTU_DIR}/qlibcjson/ubuntu;${UBUNTU_DIR}/qlibquaesl/ubuntu;${UBUNTU_DIR}/qlibfreetype/ubuntu"
)