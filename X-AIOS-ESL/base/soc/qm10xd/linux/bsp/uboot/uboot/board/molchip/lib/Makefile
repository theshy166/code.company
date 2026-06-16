#
# (C) Copyright 2000-2004
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#
ifeq ($(CONFIG_TARGET_FY02),y)
obj-$(CONFIG_ARCH_MOLCHIP)   += reset_fy02.o
else ifeq ($(CONFIG_TARGET_FY02B),y)
obj-$(CONFIG_ARCH_MOLCHIP)   += reset_fy02b.o
else ifeq ($(CONFIG_TARGET_FY02P),y)
obj-$(CONFIG_ARCH_MOLCHIP)   += reset_fy02p.o
else ifeq ($(CONFIG_TARGET_FY10P),y)
obj-$(CONFIG_ARCH_MOLCHIP)   += reset_fy10p.o
else ifeq ($(CONFIG_TARGET_XC01),y)
obj-$(CONFIG_ARCH_MOLCHIP)   += reset_xc01.o
obj-$(CONFIG_ARCH_MOLCHIP)   += ddr_info_xc01.o
else
obj-$(CONFIG_ARCH_MOLCHIP)   += reset_mc.o
obj-$(CONFIG_ARCH_MOLCHIP)   += ddr_info_mc.o
endif
obj-$(CONFIG_ARCH_MOLCHIP)   += vexpress_common.o
obj-$(CONFIG_TARGET_FY00)    += fy00_ddr_cali_info.o
obj-$(CONFIG_TARGET_FY10D)    += fy00_ddr_cali_info.o
obj-$(CONFIG_TARGET_FY10DS)    += fy00_ddr_cali_info.o
obj-$(CONFIG_TARGET_FY01)    += fy01_ddr_cali_info.o
obj-$(CONFIG_TARGET_FY12)    += fy00_ddr_cali_info.o

