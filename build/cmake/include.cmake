
list(APPEND XOS_INTERFACE_INC
  ${XOS_ROOT}
  ${XOS_ROOT}/inc
  ${XOS_ROOT}/core/package/libnative/qlibquadrv/include
)

list(APPEND XOS_INTERFACE_INC
          ${XOS_ROOT}/core/apps/inc
          )

list(APPEND XOS_INTERFACE_INC
          ${XOS_ROOT}/core/package/framework/quaextra/inc
          )

list(APPEND XOS_INTERFACE_INC
          ${XOS_ROOT}/core/package/framework/services/lcd_service
          )

if (DEFINED CONFIG_XOS_FWK_UART_SERVICE)
list(APPEND XOS_INTERFACE_INC
          ${XOS_ROOT}/core/package/framework/services/uart_service/inc
          )
endif()

if (DEFINED CONFIG_XOS_FWK_GUIENG_LVGL9)
  set(XOS_GUIENG_DIR "${XOS_ROOT}/core/package/guiengine/lvgl-v9")
  list(APPEND XOS_INTERFACE_INC
          ${XOS_GUIENG_DIR}
          )
endif()

if (DEFINED CONFIG_CHIPSET_SIMULATOR OR "$ENV{XOS_BUILD_SIMULATOR}" STREQUAL "1")
  list(APPEND XOS_INTERFACE_INC
    ${XOS_ROOT}/core/board/generic/simulator/conf
  )
  message("XOS_INTERFACE_INC simulator: ${XOS_INTERFACE_INC}")
else()
  if ((DEFINED CONFIG_CHIPSET_MC331X) AND (DEFINED CONFIG_OS_RTT))
    list(APPEND XOS_INTERFACE_INC
      ${XOS_ROOT}/core/board/generic/fy00rtt/conf
      ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_mcdrivers/include
      ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_mpi/include
      ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_userapps
      ${XOS_ROOT}/base/soc/mc331x/rtt/rtt_userapps/sdk/rt-thread/include
    )
  endif()

  if ((DEFINED CONFIG_CHIPSET_QM10XD) AND (DEFINED CONFIG_OS_RTT))
    list(APPEND XOS_INTERFACE_INC
      ${XOS_ROOT}/base/soc/qm10xd/rtt/bsp/rt-thread/compat/include
      ${XOS_ROOT}/base/soc/qm10xd/rtt/bsp/rt-thread/kernel/include
      ${XOS_ROOT}/base/soc/qm10xd/rtt/bsp/rt-thread/kernel/libc/compilers/newlib
      ${XOS_ROOT}/base/soc/qm10xd/rtt/bsp/rt-thread/kernel/libc/termios
      ${XOS_ROOT}/base/soc/qm10xd/rtt/bsp/rt-thread/out/rtconfig
      ${XOS_ROOT}/base/soc/qm10xd/rtt/bsp/rt-thread/shell
    )
    message("XOS_INTERFACE_INC: ${XOS_INTERFACE_INC}")
  endif()
  list(APPEND XOS_INTERFACE_INC
    ${XOS_ROOT}/core/board/generic/${CONFIG_XOS_BUILD_BOARD}/conf
  )
endif()

if (EXISTS "${XOS_ROOT}/product/${CONFIG_XOS_PROJECT_TYPE}/src/strings")
  list(APPEND XOS_INTERFACE_INC
    ${XOS_ROOT}/product/${CONFIG_XOS_PROJECT_TYPE}/src/strings/src
    ${XOS_ROOT}/product/${CONFIG_XOS_PROJECT_TYPE}/src/strings/inc
  )
else()
  list(APPEND XOS_INTERFACE_INC
    ${XOS_ROOT}/core/res/inc
    ${XOS_ROOT}/core/res/strings
  )
endif()