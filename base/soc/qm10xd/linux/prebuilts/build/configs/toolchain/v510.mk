#CROSS toolchain
export CROSS=arm-molv2-linux-uclibcgnueabi-
#CROSS toolchain runtime library
export CROSS_RUNTIME_LIB=runtime_uclibc
#CROSS toolchain default CFLAGS
export CROSS_C_FLAGS=-mcpu=cortex-a7 -mfloat-abi=soft
#CROSS toolchain default LDFLAGS
export CROSS_LD_FLAGS=-mcpu=cortex-a7 -mfloat-abi=soft
#CROSS toolchain ID
export CROSS_VID=v510



