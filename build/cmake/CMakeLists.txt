cmake_minimum_required(VERSION 3.13)

set(XOS_ROOT $ENV{XOS_ROOT})

# Prevent CMake from testing the toolchain
set(CMAKE_C_COMPILER_FORCED   1)
set(CMAKE_CXX_COMPILER_FORCED 1)

project(xos-project
    VERSION     1.0
    DESCRIPTION "xos project"
    LANGUAGES ASM C CXX
)

include(build/cmake/top.cmake)

find_package(Python COMPONENTS Interpreter Development)

# Python
message(STATUS "Found Python: ${Python_EXECUTABLE}")  
message(STATUS "Python include directories: ${Python_INCLUDE_DIRS}")  
message(STATUS "Python libraries: ${Python_LIBRARIES}")  

set(EMPTY_FILE ${CMAKE_CURRENT_BINARY_DIR}/empty_file.c)
if(NOT EXISTS ${EMPTY_FILE})
  file(WRITE ${EMPTY_FILE})
endif()

#######################
### create an main target
#######################
xos_declare_project(xos)

#######################
### create an main target
#######################
set(XOS_TARGET "qxosui")

if(NOT DEFINED CONFIG_XOS_USE_NO_QUAUI)
add_executable(${XOS_TARGET})
target_sources(${XOS_TARGET} PRIVATE ${EMPTY_FILE})
set_target_properties(${XOS_TARGET} PROPERTIES LINK_FLAGS "-Wl,--gc-sections")
endif()
# 设置全局策略
cmake_policy(SET CMP0079 NEW)

# 允许静态库继承依赖
set(CMAKE_LINK_LIBRARIES_ONLY_TARGETS TRUE)
#######################
### create an interface target
#######################
set(XOS_INTERFACE_NAME "xos_interface")
add_library(${XOS_INTERFACE_NAME} INTERFACE)

# set(XOS_INTERFACE_INC PARENT_SCOPE)
list(APPEND XOS_INTERFACE_DEF )
include (build/cmake/definition.cmake)
target_compile_definitions(${XOS_INTERFACE_NAME} INTERFACE ${XOS_INTERFACE_DEF})

list(APPEND XOS_INTERFACE_INC )
include (build/cmake/include.cmake)
target_include_directories(${XOS_INTERFACE_NAME} INTERFACE ${XOS_INTERFACE_INC})

list(APPEND XOS_COMPILE_FLAGS)
list(APPEND XOS_INTERFACE_OPT)
include (build/cmake/options.cmake)
add_compile_options(${XOS_COMPILE_FLAGS})
target_link_options(${XOS_INTERFACE_NAME} INTERFACE ${XOS_INTERFACE_OPT})

include (build/cmake/complexity.cmake)

if(NOT DEFINED CONFIG_XOS_USE_NO_QUAUI)
  #get_target_property(libs ${XOS_CURRENT_INTERFACE} associated_libs)
  #add_dependencies(${XOS_TARGET} ${libs} ${XOS_CURRENT_CATCH_LIB})
  target_link_libraries(${XOS_TARGET} ${XOS_INTERFACE_NAME} 
    ${XOS_CURRENT_INTERFACE}
    ${XOS_CURRENT_CATCH_LIB}
    ${XOS_CURRENT_LIBRARY}
  )
endif()
