cmd_lib/crc16.o := arm-molv2-linux-uclibcgnueabi-gcc -Wp,-MD,lib/.crc16.o.d -nostdinc -isystem /home/luoxin/work/X-AIOS-ESL/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/../lib/gcc/arm-molv2-linux-uclibcgnueabi/10.3.1/include -Iinclude  -I../include  -I../arch/arm/include -include ../include/linux/kconfig.h -I../arch/arm/mach-molchip/include  -I../lib -Ilib -D__KERNEL__ -D__UBOOT__ -DLOGO_YADDR0=0x43E00000 -DLOGO_CADDR0=0x43E98000 -DLOGO_YADDR1=0x00000000 -DLOGO_CADDR1=0x00000000 -Wall -Wstrict-prototypes -Wno-format-security -Wno-attributes -fno-builtin -ffreestanding -Os -fno-stack-protector -fno-delete-null-pointer-checks -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -Wno-address-of-packed-member -Werror -DLINUX_KERNEL_SIZE_HEX=0x27a21c -DRECOVERY_KERNEL_SIZE_HEX=0x0 -D__ARM__ -marm -mno-thumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv7-a -D__LINUX_ARM_ARCH__=7  -I../../arch/arm/mach-molchip/include -I../arch/arm/mach-molchip/include -Werror    -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(crc16)"  -D"KBUILD_MODNAME=KBUILD_STR(crc16)" -c -o lib/crc16.o ../lib/crc16.c

source_lib/crc16.o := ../lib/crc16.c

deps_lib/crc16.o := \
  ../include/crc.h \
  ../include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/use/stdint.h) \
  ../include/linux/posix_types.h \
  ../include/linux/stddef.h \
  ../arch/arm/include/asm/posix_types.h \
  ../arch/arm/include/asm/types.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/dma/addr/t/64bit.h) \
  /home/luoxin/work/X-AIOS-ESL/tools/toolchain/arm-molv2-linux-uclibcgnueabi/lib/gcc/arm-molv2-linux-uclibcgnueabi/10.3.1/include/stdbool.h \

lib/crc16.o: $(deps_lib/crc16.o)

$(deps_lib/crc16.o):
