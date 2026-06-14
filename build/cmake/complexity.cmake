if(NOT DEFINED CONFIG_XOS_USE_NO_QUAUI)
    # add_subdirectory(core/package/framework/quaplus)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_MQTTSER_MIDDLE ${XOS_ROOT}/core/package/framework/services/mqtt_service)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_QUAEXTRA       ${XOS_ROOT}/core/package/framework/quaextra)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_LCDSRV         ${XOS_ROOT}/core/package/framework/services/lcd_service)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_CADP           ${XOS_ROOT}/core/package/framework/cloudadp)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_QUAAITALK      ${XOS_ROOT}/core/package/framework/quaaitalk)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_UART_SERVICE   ${XOS_ROOT}/core/package/framework/services/uart_service)
    add_subdirectory_ifdef_and_exists(CONFIG_XOS_FWK_TSYNC_SERVICE  ${XOS_ROOT}/core/package/framework/services/tsync_service)

    add_subdirectory_ifexists(${XOS_ROOT}/core/apps)
    if (EXISTS "${XOS_ROOT}/product/${CONFIG_XOS_PROJECT_TYPE}/src/strings")
        add_subdirectory_ifexists(${XOS_ROOT}/product/${CONFIG_XOS_PROJECT_TYPE}/src)
    else()
        add_subdirectory_ifexists(${XOS_ROOT}/core/res)
    endif()

    if (DEFINED CONFIG_XOS_FWK_GUIENG_LVGL9)
        add_subdirectory_ifexists(${XOS_ROOT}/core/package/guiengine/lvgl-v9/lvgl)
    endif()

    if(DEFINED CONFIG_CHIPSET_SIMULATOR OR "$ENV{XOS_BUILD_SIMULATOR}" STREQUAL "1")
        add_subdirectory_ifexists(${XOS_ROOT}/core/board/generic/simulator)
    else()
        add_subdirectory_ifexists(${XOS_ROOT}/core/board/generic/${CONFIG_XOS_BUILD_BOARD})
    endif()

    if ((DEFINED CONFIG_CHIPSET_MC331X) AND (DEFINED CONFIG_OS_RTT))
        list(APPEND post_build_commands
            COMMAND ${CMAKE_COMMAND} -E echo "post command"
            COMMAND ${CMAKE_COMMAND} -E copy ${XOS_TARGET}.elf ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_userapps/root/bin
            COMMAND ${CMAKE_COMMAND} -E copy ${XOS_ROOT}/resource/res/ttf/2312.ttf ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_userapps/root/bin
            COMMAND ${CMAKE_COMMAND} -E chdir ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_userapps fakeroot ./mkrootfs.sh
            COMMAND ${CMAKE_COMMAND} -E copy ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_userapps/rootfs.fat ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_kernel/bsp/molchip-a7-smart
        )
        add_custom_command(TARGET ${XOS_TARGET}
            POST_BUILD
            ${post_build_commands}
        )
    endif()
endif()

message(STATUS "build jobs num: XOS_JOBS=${XOS_JOBS}")
message(STATUS "build simlator: XOS_BUILD_SIMULATOR=${XOS_BUILD_SIMULATOR}")

set(UBOOT_TARGET "uboot")
add_custom_target(${UBOOT_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "this is uboot TARGET"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_UBOOT=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -U
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(LINUX_TARGET "kernel")
add_custom_target(${LINUX_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "this is kernel TARGET"
    COMMAND ${CMAKE_COMMAND} -E env 
    BUILD_KERNEL=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -K
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(QUADRV_TARGET "quadrv")
add_custom_target(${QUADRV_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "this is quadrv TARGET"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_QUADRV=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -Q
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(PACKAGE_TARGET "package")
add_custom_target(${PACKAGE_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "this is package TARGET"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_FW_IMG=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -I
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(BASE_TARGET "base")
add_custom_target(${BASE_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "this is base TARGET"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_ALL=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS}
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(BASE_CLEAN "baseclean")
add_custom_target(${BASE_CLEAN}
    COMMAND ${CMAKE_COMMAND} -E echo "this is base clean"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_CLEAN=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -C
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(BASE_CLEAN "basedistclean")
add_custom_target(${BASE_CLEAN}
    COMMAND ${CMAKE_COMMAND} -E echo "this is base distclean"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_DISTCLEAN=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -N
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(LINUX_TARGET "rttap")
add_custom_target(${LINUX_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "this is post build rtt-ap kernel TARGET"
    COMMAND ${CMAKE_COMMAND} -E env
    BUILD_RTOS_POSTBUILD=true
    ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py ${XOS_ROOT}/build/script/build-base.sh -J${XOS_JOBS} -R
    WORKING_DIRECTORY ${XOS_ROOT}
)
set(GUI_TARGET "simulator")
add_custom_target(${GUI_TARGET}
    COMMAND ${CMAKE_COMMAND} -E echo "Reconfiguring for simulator..."
    COMMAND ${CMAKE_COMMAND} -E env XOS_BUILD_SIMULATOR=1
    ${CMAKE_COMMAND} -D XOS_BUILD_SIMULATOR=1 -B ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${XOS_TARGET} -j ${XOS_JOBS}
    COMMENT "Building for simulator"
    WORKING_DIRECTORY ${XOS_ROOT}
)
add_custom_target(sim DEPENDS ${GUI_TARGET})
set(XOS_OUT_DIR "${XOS_ROOT}/out/simulator/xos")
add_custom_command(TARGET ${GUI_TARGET} POST_BUILD
  # 创建目录
  COMMAND ${CMAKE_COMMAND} -E make_directory "${XOS_OUT_DIR}/bin"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${XOS_OUT_DIR}/res"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${XOS_OUT_DIR}/lib"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${XOS_OUT_DIR}/res/ttf"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${XOS_OUT_DIR}/res/DCIM"

  # 解压资源
  COMMAND ${CMAKE_COMMAND} -E tar xf 
        "${XOS_ROOT}/core/board/generic/simulator/etc/DCIM.tar.xz"
        WORKING_DIRECTORY "${XOS_OUT_DIR}/res"

  COMMAND ${CMAKE_COMMAND} -E tar xf 
        "${XOS_ROOT}/core/board/generic/simulator/etc/rotties.tar.xz" 
         WORKING_DIRECTORY "${XOS_OUT_DIR}/res"

  COMMAND ${CMAKE_COMMAND} -E copy 
          "${CMAKE_BINARY_DIR}/qxosui.elf" 
          "${XOS_OUT_DIR}/bin/qxosui"

  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_BINARY_DIR}/res/DCIM" 
          "${XOS_OUT_DIR}/bin/DCIM"

  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_BINARY_DIR}/res" 
          "${XOS_OUT_DIR}/res"

  COMMAND bash -c "cp -arf ${CMAKE_BINARY_DIR}/lib/* ${XOS_OUT_DIR}/lib"
  COMMAND ${CMAKE_COMMAND} -E echo "Simulator compiled successfully"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

if(DEFINED CONFIG_XOS_USE_NO_QUAUI)
    if(DEFINED CONFIG_XOS_UTILS_HELLO)
        add_subdirectory_ifexists(${XOS_ROOT}/core/package/utils/hello)
    endif()
endif()
