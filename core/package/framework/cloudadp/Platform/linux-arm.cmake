# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# ~/los/tools/toolchain/arm-fhv512-linux/
# specify the cross compiler
set(TOOLCHAIN_DIR ${PROJECT_SOURCE_DIR}/../../../../tools/toolchain/arm-fhv512-linux/)

SET(CMAKE_C_COMPILER    ${TOOLCHAIN_DIR}/bin/arm-fhv512-linux-uclibcgnueabihf-gcc)
SET(CMAKE_CXX_COMPILER  ${TOOLCHAIN_DIR}/bin/arm-fhv512-linux-uclibcgnueabihf-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
