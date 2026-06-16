cmd_lib/div64.o := arm-molv2-linux-uclibcgnueabi-gcc -Wp,-MD,lib/.div64.o.d -nostdinc -isystem /home/luoxin/work/X-AIOS-ESL/tools/toolchain/arm-molv2-linux-uclibcgnueabi/bin/../lib/gcc/arm-molv2-linux-uclibcgnueabi/10.3.1/include -Iinclude  -I../include  -I../arch/arm/include -include ../include/linux/kconfig.h -I../arch/arm/mach-molchip/include  -I../lib -Ilib -D__KERNEL__ -D__UBOOT__ -DLOGO_YADDR0=0x43E00000 -DLOGO_CADDR0=0x43E98000 -DLOGO_YADDR1=0x00000000 -DLOGO_CADDR1=0x00000000 -Wall -Wstrict-prototypes -Wno-format-security -Wno-attributes -fno-builtin -ffreestanding -Os -fno-stack-protector -fno-delete-null-pointer-checks -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -Wno-address-of-packed-member -Werror -DLINUX_KERNEL_SIZE_HEX=0x27a21c -DRECOVERY_KERNEL_SIZE_HEX=0x0 -D__ARM__ -marm -mno-thumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv7-a -D__LINUX_ARM_ARCH__=7  -I../../arch/arm/mach-molchip/include -I../arch/arm/mach-molchip/include -Werror    -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(div64)"  -D"KBUILD_MODNAME=KBUILD_STR(div64)" -c -o lib/div64.o ../lib/div64.c

source_lib/div64.o := ../lib/div64.c

deps_lib/div64.o := \
  ../include/linux/compat.h \
    $(wildcard include/config/lbdaf.h) \
  ../include/malloc.h \
    $(wildcard include/config/sys/malloc/simple.h) \
  ../include/linux/stddef.h \
  ../include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/use/stdint.h) \
  ../include/linux/posix_types.h \
  ../arch/arm/include/asm/posix_types.h \
  ../arch/arm/include/asm/types.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/dma/addr/t/64bit.h) \
  /home/luoxin/work/X-AIOS-ESL/tools/toolchain/arm-molv2-linux-uclibcgnueabi/lib/gcc/arm-molv2-linux-uclibcgnueabi/10.3.1/include/stdbool.h \
  ../include/linux/err.h \
  ../include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  ../include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  ../include/linux/errno.h \
  ../include/linux/kernel.h \
  ../include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  ../include/div64.h \
  ../include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  ../include/linux/bitops.h \
  ../include/asm-generic/bitsperlong.h \
  ../arch/arm/include/asm/bitops.h \
  ../arch/arm/include/asm/proc-armv/system.h \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
  ../include/asm-generic/bitops/__fls.h \
  ../include/asm-generic/bitops/__ffs.h \
  ../include/asm-generic/bitops/fls.h \
  ../include/asm-generic/bitops/fls64.h \

lib/div64.o: $(deps_lib/div64.o)

$(deps_lib/div64.o):
