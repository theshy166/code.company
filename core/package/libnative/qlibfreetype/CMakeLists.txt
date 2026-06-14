set(LIB_NAME "freetype")
add_library(${LIB_NAME} STATIC IMPORTED GLOBAL)
set_target_properties(${LIB_NAME} PROPERTIES 
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ubuntu/libfreetype.a"
)