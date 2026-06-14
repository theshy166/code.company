cmd_sdl/lib/ctype.o := arm-molv2-linux-uclibcgnueabi-gcc -Wp,-MD,sdl/lib/.ctype.o.d -nostdinc -isystem /home/luoxin/work/X-AIOS-ESL/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/../lib/gcc/arm-molv2-linux-uclibcgnueabi/10.3.1/include -Iinclude  -I../include  -I../arch/arm/include -include ../include/linux/kconfig.h -I../arch/arm/mach-molchip/include  -I../sdl/lib -Isdl/lib -D__KERNEL__ -D__UBOOT__ -DLOGO_YADDR0=0x43E00000 -DLOGO_CADDR0=0x43E98000 -DLOGO_YADDR1=0x00000000 -DLOGO_CADDR1=0x00000000 -DCONFIG_SDL_BUILD -Wall -Wstrict-prototypes -Wno-format-security -Wno-attributes -fno-builtin -ffreestanding -Os -fno-stack-protector -fno-delete-null-pointer-checks -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -Wno-address-of-packed-member -Werror -DLINUX_KERNEL_SIZE_HEX=0x27a21c -DRECOVERY_KERNEL_SIZE_HEX=0x0 -ffunction-sections -fdata-sections -D__ARM__ -marm -mno-thumb-interwork -mabi=aapcs-linux -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv7-a -D__LINUX_ARM_ARCH__=7  -I../../arch/arm/mach-molchip/include -I../arch/arm/mach-molchip/include -Werror    -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(ctype)"  -D"KBUILD_MODNAME=KBUILD_STR(ctype)" -c -o sdl/lib/ctype.o ../lib/ctype.c

source_sdl/lib/ctype.o := ../lib/ctype.c

deps_sdl/lib/ctype.o := \
  ../include/linux/ctype.h \

sdl/lib/ctype.o: $(deps_sdl/lib/ctype.o)

$(deps_sdl/lib/ctype.o):
