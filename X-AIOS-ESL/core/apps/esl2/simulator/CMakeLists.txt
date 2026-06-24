cmake_minimum_required(VERSION 3.16)
project(esl2_simulator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ── ★ Debug 符号 + 无优化（方便 GDB 调试）──
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g -O0)

# ── 路径 ──
set(XOS_ROOT "/home/luoxin/work/X-AIOS-ESL")
set(LVGL_DIR "${XOS_ROOT}/core/package/guiengine/lvgl-v9/lvgl")
set(ESL2_DIR "${XOS_ROOT}/core/apps/esl2")

# ── ★ BUILD_SIMULATOR 宏 — 让 conf.h 等使用模拟器路径 ──
add_definitions(-DBUILD_SIMULATOR=1)
add_definitions(-DCONFIG_XOS_APP_ESL2)
add_definitions(-DCONFIG_XOS_FWK_APPMANAGER)
add_definitions(-DCONFIG_XOS_FWK_CADP_CURL)
add_definitions(-DCONFIG_XOS_FWK_CADP_QUA)
add_definitions(-DCONFIG_PLATFORM_ID=\"ESL10D\")

# ── LVGL 配置 ──
set(LV_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h")
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ── 头文件路径 ──
include_directories(
    ${LVGL_DIR}
    ${LVGL_DIR}/src
    ${LVGL_DIR}/..

    # ESL2 子目录
    ${ESL2_DIR}
    ${ESL2_DIR}/view
    ${ESL2_DIR}/ctrl
    ${ESL2_DIR}/utils
    ${ESL2_DIR}/model
    ${ESL2_DIR}/conf

    # 框架库
    ${XOS_ROOT}/core/package/framework/quaextra/inc
    ${XOS_ROOT}/core/package/libnative/qlibquaesl/include
    ${XOS_ROOT}/core/package/libnative/qlibcjson/include
    ${XOS_ROOT}/core/package/libnative/qlibfreetype/include
    ${XOS_ROOT}/core/package/framework/cloudadp/inc
    ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/inc
    ${XOS_ROOT}/core/package/framework/cloudadp/qua/quayun/MQTTPacket

    # xos_app_conf.h 路径（模拟器配置）
    ${XOS_ROOT}/core/board/generic/simulator/conf

    # xos_string_ids.h 路径
    ${XOS_ROOT}/product/esl2/src/strings/src

    # 自动生成配置
    ${XOS_ROOT}/inc
    ${XOS_ROOT}/inc/generated

    # 模拟器自身
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

# ── ★ 真实 ESL2 View 源文件 — 将 view 代码编译进模拟器 ★ ──
set(ESL2_VIEW_SOURCES
    # 真实 esl2 view 入口和组件
    ${ESL2_DIR}/view/h5esl_native.c
    ${ESL2_DIR}/view/page_esl.c
    ${ESL2_DIR}/view/page_overlay.c
    ${ESL2_DIR}/view/qm_product_comp.c
    ${ESL2_DIR}/view/qm_2_label.c
    ${ESL2_DIR}/view/qm_3_label.c
    ${ESL2_DIR}/view/page_footer.c

    # ctrl — 真实页面初始化、缓存、通信路径依赖
    ${ESL2_DIR}/ctrl/esl_service.c
    ${ESL2_DIR}/ctrl/common_service.c
    ${ESL2_DIR}/ctrl/esl_file.c
    ${ESL2_DIR}/ctrl/esl_socket_service.c
    ${ESL2_DIR}/ctrl/esl_socket_udp_service.c
    ${ESL2_DIR}/ctrl/mqtt_control.c
    ${ESL2_DIR}/ctrl/mqtt_subscriber.c
    ${ESL2_DIR}/ctrl/esl_message_queue.c

    # utils — 工具库
    ${ESL2_DIR}/utils/esl_ui_utils.c
    ${ESL2_DIR}/utils/esl_json_utils.c
    ${ESL2_DIR}/utils/esl_sign.c
    ${ESL2_DIR}/utils/md5.c
    ${ESL2_DIR}/utils/hashmap.c
    ${ESL2_DIR}/utils/str_util.c

    # 模拟器桩（硬件和网络 mock）
    ${CMAKE_CURRENT_SOURCE_DIR}/xos_stub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

# ── Ubuntu 预编译库 ──
set(UBUNTU_DIR "${XOS_ROOT}/core/package/libnative")

set(UBUNTU_LIBS
    ${UBUNTU_DIR}/qlibeffects/ubuntu/libqm_effects.so
    ${UBUNTU_DIR}/qlibcjson/ubuntu/libcjson.so
    ${UBUNTU_DIR}/qlibquaesl/ubuntu/libquaesl.so
)

# ── 可执行文件 ──
add_executable(esl2_sim
    ${LVGL_SOURCES}
    ${ESL2_VIEW_SOURCES}
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
    # 系统字体/图像库（Ubuntu 上只有带版本号的 .so，用 -l: 语法精确指定）
    /usr/lib/x86_64-linux-gnu/libfreetype.so.6
    /usr/lib/x86_64-linux-gnu/libjpeg.so.8
    png16
    z
)

# ── 设置 rpath ──
set_target_properties(esl2_sim PROPERTIES
    BUILD_RPATH "${UBUNTU_DIR}/qlibeffects/ubuntu;${UBUNTU_DIR}/qlibcjson/ubuntu;${UBUNTU_DIR}/qlibquaesl/ubuntu"
    INSTALL_RPATH "${UBUNTU_DIR}/qlibeffects/ubuntu;${UBUNTU_DIR}/qlibcjson/ubuntu;${UBUNTU_DIR}/qlibquaesl/ubuntu"
)
