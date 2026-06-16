#
# (C) Copyright 2012 Stephen Warren
#
# SPDX-License-Identifier:	GPL-2.0
#

obj-$(TARGET_MOLCHIP) += lowlevel_init.o
obj-y	+= init.o

ifneq ($(findstring $(CONFIG_PDL_BUILD),y),)
obj-y   += crc16.o string.o
endif

ifdef CONFIG_SDL_BUILD
obj-$(CONFIG_TRACE_DEBUG) += trace_debug.o
obj-y += crc16.o
endif
obj-$(CONFIG_ARM64) += mmu-arm64.o

ifdef CONFIG_SPL_BUILD
obj-y += string.o
endif
