add_subdirectory(qlibcjson)
add_subdirectory(qlibffmpeg)
add_subdirectory(qlibfreetype)
add_subdirectory(qlibiniparser)
add_subdirectory(qliblosextra)

if (0)
set(LIB_NAME "native")
add_library(${LIB_NAME} STATIC ${EMPTY_FILE})

### TODO
link_directories(${CMAKE_CURRENT_BINARY_DIR}/)
target_link_libraries(${LIB_NAME} freetype)
endif()