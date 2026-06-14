# SPDX-License-Identifier: GPL-2.0

prepare:
	@python gen_dmc_reg.txt prg1_sleep.xlsx
	@python gen_dmc_reg.txt prg2_sleep.xlsx
	@python gen_dmc_reg.txt deep_sleep.xlsx

obj-y += mc_sleep_debug.o

all: prepare mc_sleep_debug.o