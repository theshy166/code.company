
if ((DEFINED CONFIG_CHIPSET_MC331X) AND (DEFINED CONFIG_OS_RTT))
  list(APPEND XOS_INTERFACE_OPT
      -static
      -T ${XOS_ROOT}/build/cmake/link.lds
  )
  target_compile_options(${XOS_INTERFACE_NAME} INTERFACE -march=armv7-a -marm -msoft-float)
endif()

if (DEFINED CONFIG_XOS_USE_DEBUG)
  list(APPEND XOS_INTERFACE_OPT
    -g -O0
  )
endif()

if (DEFINED CONFIG_XOS_USE_DEBUG)
  list(APPEND XOS_COMPILE_FLAGS
    -g -O0
  )
endif()
