#
# (C) Copyright 2000-2004
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifneq ($(findstring $(CONFIG_SPL_BUILD)$(CONFIG_PDL_BUILD)$(CONFIG_SDL_BUILD),y y y),)
obj-$(CONFIG_ARCH_MOLCHIP)   += board/
obj-$(CONFIG_ARCH_MOLCHIP)   += timer/
obj-$(CONFIG_ARCH_MOLCHIP)   += uart/
obj-$(CONFIG_ARCH_MOLCHIP)   += clk/
else
obj-$(CONFIG_ARCH_MOLCHIP)   += lib/
obj-$(CONFIG_ARCH_MOLCHIP)   += uart/
obj-$(CONFIG_ARCH_MOLCHIP)   += flash/
obj-$(CONFIG_ARCH_MOLCHIP)   += clk/
obj-$(CONFIG_ARCH_MOLCHIP)   += pwm/
obj-$(CONFIG_ARCH_MOLCHIP)   += pts/
obj-$(CONFIG_ARCH_MOLCHIP)   += sdcard/
# obj-$(CONFIG_ARCH_MOLCHIP)   += logo/
endif

ifneq ($(findstring $(CONFIG_SPL_BUILD)$(CONFIG_SDL_BUILD),y y),)
#obj-$(CONFIG_ARCH_MOLCHIP)   += sfc/
obj-$(CONFIG_ARCH_MOLCHIP)   += flash/
obj-$(CONFIG_ARCH_MOLCHIP)   += dma/
obj-$(CONFIG_ARCH_MOLCHIP)   += sdcard/
endif


ifneq ($(findstring $(CONFIG_PDL_BUILD)$(CONFIG_SDL_BUILD),y y),)
obj-$(CONFIG_ARCH_MOLCHIP)   += download/
obj-$(CONFIG_SUPPORT_USB2)   += usb2/
endif

ifneq ($(findstring $(CONFIG_PDL_BUILD)$(CONFIG_SPL_BUILD),y y),)
obj-$(CONFIG_ARCH_MOLCHIP)   += ddr/
endif

ifdef CONFIG_SDL_BUILD
obj-$(CONFIG_ARCH_MOLCHIP)   += timer/
obj-$(CONFIG_SDL_MMC_SUPPORT)   += emmc/
endif
ifdef CONFIG_SPL_BUILD
obj-$(CONFIG_ARCH_MOLCHIP)   += timer/
obj-$(CONFIG_ARCH_MOLCHIP)   += spl/
obj-$(CONFIG_SPL_MMC_SUPPORT)   += emmc/
obj-$(CONFIG_ARCH_MOLCHIP)   += sdcard/
endif
ifdef CONFIG_PDL_BUILD
obj-$(CONFIG_ARCH_MOLCHIP)   += timer/
obj-$(CONFIG_ARCH_MOLCHIP)   += uart/
endif
