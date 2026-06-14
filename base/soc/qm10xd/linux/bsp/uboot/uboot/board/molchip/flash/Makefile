#
# (C) Copyright 2000-2004
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#
ifneq ($(findstring $(CONFIG_SPL_BUILD)$(CONFIG_PDL_BUILD)$(CONFIG_SDL_BUILD),y y y),)
obj-$(CONFIG_ARCH_MOLCHIP) += molchip_nand_spl.o
obj-$(CONFIG_ARCH_MOLCHIP) += molchip_nor_spl.o
else
obj-$(CONFIG_ARCH_MOLCHIP) += molchip_nand_spl.o
endif
