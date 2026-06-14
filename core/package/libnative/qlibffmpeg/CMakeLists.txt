set(LIB_NAME "ffmpeg")
add_library(${LIB_NAME} STATIC IMPORTED GLOBAL)
set_target_properties(${LIB_NAME} PROPERTIES 
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ubuntu/libavcodec.a"
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ubuntu/libavformat.a"
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ubuntu/libavutil.a"
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ubuntu/libswresample.a"
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ubuntu/libswscale.a"
)