
# https://cmake.org/cmake/help/latest/command/xos_add_compile_definitions.html
function(xos_add_compile_definitions)

  list(APPEND XOS_INTERFACE_DEF )
  message(STATUS "Add compile definitions from: ${CMAKE_SOURCE_DIR}/.config")
  # 指定 .config 文件路径
  set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/.config")

  # 打开 .config 文件并读取其中的内容
  file(READ "${CONFIG_FILE}" CONFIG_CONTENTS)

  # 使用正则表达式提取所有宏定义（包含小写字母的宏）
  string(REGEX MATCHALL "CONFIG_[A-Za-z0-9_]+[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" CONFIG_DEFS "${CONFIG_CONTENTS}")

  # 遍历所有找到的宏定义并添加到编译选项
  foreach(CONFIG_DEF ${CONFIG_DEFS})
      # 提取宏名称和宏值
      string(REGEX REPLACE "CONFIG_([A-Za-z0-9_]+)[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" "\\1" CONFIG_NAME ${CONFIG_DEF})
      string(REGEX REPLACE "CONFIG_[A-Za-z0-9_]+[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" "\\1" CONFIG_VALUE ${CONFIG_DEF})
      string(REGEX REPLACE "CONFIG_([A-Za-z0-9_]+)[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" "CONFIG_\\1" CONFIG_EXTN ${CONFIG_DEF})

      # 添加编译选项 -D 宏名=宏值
      #add_compile_definitions("${CONFIG_NAME}=${CONFIG_VALUE}")
      #add_compile_definitions("${CONFIG_EXTN}=${CONFIG_VALUE}")

      #list(APPEND XOS_INTERFACE_DEF
      #  "${CONFIG_EXTN}=${CONFIG_VALUE}"
      #)

      if (${CONFIG_VALUE} STREQUAL "y")
        list(APPEND XOS_INTERFACE_DEF
          "${CONFIG_EXTN}=1"
        )
      else()
        list(APPEND XOS_INTERFACE_DEF
          "${CONFIG_EXTN}=${CONFIG_VALUE}"
        )
      endif()
      # 打印日志，显示被添加的宏定义
      message(STATUS "Added compile definition: ${CONFIG_NAME}=${CONFIG_VALUE}")
  endforeach()

endfunction()

# https://cmake.org/cmake/help/latest/command/xos_build_qm108h_uboot.html
function(xos_build_qm108h_uboot)

  message(STATUS "Start build qm108h uboot")
  set(UBOOT_TARGET "uboot")
  add_custom_target(${UBOOT_TARGET}
      COMMAND ${CMAKE_COMMAND} -E echo "this is UBOOT_TARGET"
      COMMAND ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py build.sh -U -Slinux_128
      WORKING_DIRECTORY ${XOS_ROOT}
  )

endfunction()


# https://cmake.org/cmake/help/latest/command/xos_build_qm108h_kernel.html
function(xos_build_qm108h_kernel)

  message(STATUS "Start build qm108h kernel")
  set(LINUX_TARGET "kernel")
  add_custom_target(${LINUX_TARGET}
      COMMAND ${CMAKE_COMMAND} -E echo "this is LINUX_TARGET"
      COMMAND ${Python_EXECUTABLE} ${XOS_ROOT}/build/script/run_cmd.py build.sh -K -Slinux_128
      WORKING_DIRECTORY ${XOS_ROOT}
  )

endfunction()
