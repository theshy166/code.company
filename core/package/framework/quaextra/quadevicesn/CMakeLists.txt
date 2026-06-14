cmake_minimum_required(VERSION 3.0)

project(quadsn)


list(APPEND XOS_QUADSN_INC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(XOS_QUADSN_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/serial_number.c
)

list(APPEND XOS_QUADSN_LIBS quadsn)
MESSAGE("XOS_QUADSN_LIBS=${XOS_QUADSN_LIBS}")

if (DEFINED CONFIG_OS_RTT)
add_library(${XOS_QUADSN_LIBS} OBJECT ${XOS_QUADSN_SRC})
else()
add_library(${XOS_QUADSN_LIBS} STATIC ${XOS_QUADSN_SRC})
endif()

target_include_directories(${XOS_QUADSN_LIBS} PRIVATE ${XOS_QUADSN_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUADSN_LIBS} PRIVATE ${XOS_INTERFACE_DEF})

set(XOS_QUADSN_LIB_NAME "xosdevicesn")
add_library(${XOS_QUADSN_LIB_NAME} STATIC ${EMPTY_FILE})
# MESSAGE(STATUS "XOS_INTERFACE_INC extra : ${XOS_INTERFACE_INC}")
target_link_libraries(${XOS_QUADSN_LIB_NAME} PRIVATE ${XOS_QUADSN_LIBS})
target_include_directories(${XOS_QUADSN_LIB_NAME} PRIVATE ${XOS_QUADSN_INC} ${XOS_INTERFACE_INC})
target_compile_definitions(${XOS_QUADSN_LIB_NAME} PRIVATE ${XOS_INTERFACE_DEF})


