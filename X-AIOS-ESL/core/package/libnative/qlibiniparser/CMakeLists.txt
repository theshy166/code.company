
set(QLIBINIPARSER_SRC
  src/dictionary.c
  src/iniparser.c
  iniparse.c
)

set(QLIBINIPARSER_INC
  src/
)

set(LIB_NAME "iniparser")
add_library(${LIB_NAME} STATIC ${QLIBINIPARSER_SRC})
target_include_directories(${LIB_NAME} PRIVATE ${QLIBINIPARSER_INC})