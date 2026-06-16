#CROSS toolchain
export CROSS=armv7-fullhanv3-linux-gnueabihf-
#CROSS toolchain runtime library
export CROSS_RUNTIME_LIB=runtime_glibc
#CROSS toolchain default CFLAGS
export CROSS_C_FLAGS=-mcpu=cortex-a7 -mfloat-abi=soft
#CROSS toolchain default LDFLAGS
export CROSS_LD_FLAGS=-mcpu=cortex-a7 -mfloat-abi=soft
#CROSS toolchain ID
export CROSS_VID=v610
