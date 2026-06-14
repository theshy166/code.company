
message(STATUS "Add compile definitions from: ${CMAKE_SOURCE_DIR}/.config")
# 指定 .config 文件路径
set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/.config")

# 打开 .config 文件并读取其中的内容
file(READ "${CONFIG_FILE}" CONFIG_CONTENTS)

# 使用正则表达式提取所有宏定义（包含小写字母的宏）
string(REGEX MATCHALL "CONFIG_[a-zA-Z0-9_]+[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+|y)" CONFIG_DEFS "${CONFIG_CONTENTS}")

# 遍历所有找到的宏定义并添加到编译选项
foreach(CONFIG_DEF ${CONFIG_DEFS})
    # 提取宏名称和宏值
    string(REGEX REPLACE "CONFIG_([a-zA-Z0-9_]+)[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" "\\1" CONFIG_NAME ${CONFIG_DEF})
    string(REGEX REPLACE "CONFIG_[a-zA-Z0-9_]+[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" "\\1" CONFIG_VALUE ${CONFIG_DEF})
    string(REGEX REPLACE "CONFIG_([a-zA-Z0-9_]+)[ ]*=[ ]*(\"[^\"]*\"|[a-zA-Z0-9_]+)" "CONFIG_\\1" CONFIG_EXTN ${CONFIG_DEF})

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
      if (NOT "${CONFIG_EXTN}" STREQUAL "CONFIG_XOS_APP_MAIN_ENTRY")
        list(APPEND XOS_INTERFACE_DEF
          "${CONFIG_EXTN}=${CONFIG_VALUE}"
        )
      endif()
    endif()
    # 打印日志，显示被添加的宏定义
    message(STATUS "Added compile definition: ${CONFIG_NAME}=${CONFIG_VALUE}")
endforeach()

# 添加BUILD_ID处理
if(DEFINED ENV{BUILD_ID})
    list(APPEND XOS_INTERFACE_DEF
        "CONFIG_BUILD_ID=\"$ENV{BUILD_ID}\""
    )
    message(STATUS "Using BUILD_ID from env: $ENV{BUILD_ID}")
else()
    list(APPEND XOS_INTERFACE_DEF
        "CONFIG_BUILD_ID=\"000\""
    )
    message(STATUS "BUILD_ID not found, using default: 000")
endif()

list(APPEND XOS_INTERFACE_DEF
  "HAVE_CCONFIG_H"
  "LV_CONF_INCLUDE_SIMPLE"
  "_GNU_SOURCE"
)

list(APPEND XOS_INTERFACE_DEF
  "__MMI_LANGUAGE_ENGLISH_US__"
  "__MMI_LANGUAGE_CHINESE_CN__"
)

if (DEFINED CONFIG_CHIPSET_SIMULATOR OR "$ENV{XOS_BUILD_SIMULATOR}" STREQUAL "1")
  list(APPEND XOS_INTERFACE_DEF "BUILD_SIMULATOR=1")
  list(APPEND XOS_INTERFACE_DEF "XOS_BUILD_SIMULATOR=1")

  if("$ENV{XOS_BUILD_SIMULATOR}" STREQUAL "1")
      message(STATUS "ENV XOS_BUILD_SIMULATOR boot simulator")
  endif()

  # list(REMOVE_ITEM XOS_INTERFACE_DEF "CONFIG_XOS_APP_XIAOMING")
  # list(REMOVE_ITEM XOS_INTERFACE_DEF "CONFIG_XOS_EXT_HIREDIS")
  # list(REMOVE_ITEM XOS_INTERFACE_DEF "CONFIG_XOS_FWK_QUAAITALK")
  # list(REMOVE_ITEM XOS_INTERFACE_DEF "CONFIG_XOS_FWK_QUAIOTMGR")
endif()

if (DEFINED CONFIG_XOS_FWK_APPMANAGER)
list(APPEND XOS_INTERFACE_DEF
  "XOS_USE_APP_MANAGER"
)
endif()

if (DEFINED CONFIG_XOS_FWK_GUIENG_LVGL9)
list(APPEND XOS_INTERFACE_DEF
  "CONFIG_LVGL_V9=1"
  "CONFIG_LVGL_VERSION=lvgl-v9"
)
endif()

if ((DEFINED CONFIG_CHIPSET_QM10XV) AND (DEFINED CONFIG_OS_LINUX))
  list(APPEND XOS_INTERFACE_DEF
    "BUILD_FH=1"
  )
endif()

if ((DEFINED CONFIG_CHIPSET_QM10XH) AND (DEFINED CONFIG_OS_LINUX))
  list(APPEND XOS_INTERFACE_DEF
    "BUILD_FH=1"
  )
endif()

if (DEFINED CONFIG_CHIPSET_SIMULATOR OR "$ENV{XOS_BUILD_SIMULATOR}" STREQUAL "1")
  list(REMOVE_ITEM XOS_INTERFACE_DEF "BUILD_RTT=1")
else()
  if (DEFINED CONFIG_OS_RTT)
    list(APPEND XOS_INTERFACE_DEF "BUILD_RTT=1")
    if (DEFINED CONFIG_CHIPSET_QM10XD)
      list(REMOVE_ITEM XOS_INTERFACE_DEF "HAVE_CCONFIG_H")
    endif()
  endif()
endif()

if (DEFINED CONFIG_XOS_PROJECT_TYPE AND NOT DEFINED CONFIG_XOS_APP_MAIN_ENTRY)
  list(APPEND XOS_INTERFACE_DEF
    "CONFIG_XOS_APP_MAIN_ENTRY=${CONFIG_XOS_PROJECT_TYPE}"
  )
endif()

if (DEFINED CONFIG_XOS_PROJECT_TYPE AND DEFINED CONFIG_XOS_APP_MAIN_ENTRY)
  list(APPEND XOS_INTERFACE_DEF
    "CONFIG_XOS_APP_MAIN_ENTRY=${CONFIG_XOS_APP_MAIN_ENTRY}"
  )
endif()

if (DEFINED CONFIG_PRODUCT_KIDCAMERA_NOR)
list(APPEND XOS_INTERFACE_DEF
  "CONFIG_PRODUCT_KIDCAMERA=1"
)
endif()
