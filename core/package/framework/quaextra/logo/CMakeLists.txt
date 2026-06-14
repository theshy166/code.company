set(LIB_NAME "xos_logo")
set(LIB_SRC 
  logo_img.c
)

if (DEFINED CONFIG_OS_RTT)
add_library(${LIB_NAME} OBJECT ${LIB_SRC})
else()
add_library(${LIB_NAME} STATIC ${LIB_SRC})
endif()
target_include_directories(${LIB_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC})

target_link_libraries(${LIB_NAME} PUBLIC -lpthread)

if(NOT DEFINED CONFIG_OS_RTT)
set(LOGO_TEST_NAME "logo_test")
set(LOGO_TEST_SRC
  logo_test.c
)

add_executable(${LOGO_TEST_NAME} ${LOGO_TEST_SRC})
target_include_directories(${LOGO_TEST_NAME} PRIVATE ${XOS_QUAEXTRA_INC} ${XOS_INTERFACE_INC} ${LIB_SRC})
target_link_libraries(${LOGO_TEST_NAME} PRIVATE ${LIB_NAME})
endif()
