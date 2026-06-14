cmd_arch/arm/lib/div0.o := arm-molv2-linux-uclibcgnueabi-gcc -Wp,-MD,arch/arm/lib/.div0.o.d -nostdinc -isystem /home/luoxin/work/X-AIOS-ESL/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/../lib/gcc/arm-molv2-linux-uclibcgnueabi/10.3.1/include -Iinclude  -I../include  -I../arch/arm/include -include ../include/linux/kconfig.h -I../arch/arm/mach-molchip/include  -I../arch/arm/lib -Iarch/arm/lib -D__KERNEL__ -D__UBOOT__ -DLOGO_YADDR0=0x43E00000 -DLOGO_CADDR0=0x43E98000 -DLOGO_YADDR1=0x00000000 -DLOGO_CADDR1=0x00000000 -Wall -Wstrict-prototypes -Wno-format-security -Wno-attributes -fno-builtin -ffreestanding -Os -fno-stack-protector -fno-delete-null-pointer-checks -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -Wno-address-of-packed-member -Werror -DLINUX_KERNEL_SIZE_HEX=0x27a21c -DRECOVERY_KERNEL_SIZE_HEX=0x0 -D__ARM__ -marm -mno-thumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv7-a -D__LINUX_ARM_ARCH__=7  -I../../arch/arm/mach-molchip/include -I../arch/arm/mach-molchip/include -Werror    -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(div0)"  -D"KBUILD_MODNAME=KBUILD_STR(div0)" -c -o arch/arm/lib/div0.o ../arch/arm/lib/div0.c

source_arch/arm/lib/div0.o := ../arch/arm/lib/div0.c

deps_arch/arm/lib/div0.o := \

arch/arm/lib/div0.o: $(deps_arch/arm/lib/div0.o)

$(deps_arch/arm/lib/div0.o):
