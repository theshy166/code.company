set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(KCONFIG_ROOT ${CMAKE_SOURCE_DIR}/build/kconf/Kconfig)
set(AUTOCONF_H ${CMAKE_CURRENT_BINARY_DIR}/kconfig/include/generated/autoconf.h)
if(NOT DEFINED BOARD_DIR)
    set(BOARD_DIR ${CMAKE_SOURCE_DIR}/product/.target_product)
endif()

# Re-configure (Re-execute all CMakeLists.txt code) when autoconf.h changes
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${AUTOCONF_H})

include(build/cmake/extensions.cmake)
include(build/cmake/python.cmake)
include(build/cmake/kconfig.cmake)
include(build/cmake/build.cmake)

