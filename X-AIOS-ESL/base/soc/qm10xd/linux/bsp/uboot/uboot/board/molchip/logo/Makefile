#
# (C) Copyright 2000-2004
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

CUR_MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
LOGO_PATH := $(dir $(CUR_MAKEFILE_PATH))
$(shell cd  ${LOGO_PATH};${CROSS_COMPILE}objcopy -I binary -O elf32-littlearm --set-section-alignment .data=64 -B arm logo.jpg logo.o)
obj-$(CONFIG_ARCH_MOLCHIP)   += logo.o
