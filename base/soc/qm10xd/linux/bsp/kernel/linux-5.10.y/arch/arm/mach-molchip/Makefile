#
# Makefile for the linux kernel.
#
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := \
	-I$(srctree)/arch/arm/plat-molchip/include

obj-$(CONFIG_ARCH_MOLCHIP)		:= v2m.o
obj-$(CONFIG_ARCH_MOLCHIP_DCSCB)	+= dcscb.o	dcscb_setup.o
CFLAGS_dcscb.o				+= -march=armv7-a
CFLAGS_REMOVE_dcscb.o			= -pg
obj-$(CONFIG_ARCH_MOLCHIP_SPC)		+= spc.o
CFLAGS_REMOVE_spc.o			= -pg
obj-$(CONFIG_ARCH_MOLCHIP_TC2_PM)	+= tc2_pm.o
CFLAGS_tc2_pm.o				+= -march=armv7-a
CFLAGS_REMOVE_tc2_pm.o			= -pg
obj-$(CONFIG_SMP)			+= platsmp.o
obj-$(CONFIG_HOTPLUG_CPU)		+= hotplug.o

obj-$(CONFIG_ARCH_MPS2)			+= v2m-mps2.o
obj-$(CONFIG_ARCH_MOLCHIP_DEBUG)        += molchip_perf.o
obj-y					+= spinlock.o
obj-y					+= c2c.o
obj-$(CONFIG_ARCH_MOLCHIP_SUSPEND)	+= pm.o sleep.o molchip_io.o molchip_gate.o
