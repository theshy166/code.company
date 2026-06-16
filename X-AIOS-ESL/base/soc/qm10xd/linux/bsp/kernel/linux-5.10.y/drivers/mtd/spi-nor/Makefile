# SPDX-License-Identifier: GPL-2.0

ifeq ($(TARGET_CONFIG_BOARD),mc_evb)
subdir-ccflags-y += -DSFC_SAMPLE_DELAY=4
else
subdir-ccflags-y += -DSFC_SAMPLE_DELAY=2
endif

spi-nor-objs			:= core.o sfdp.o
spi-nor-objs			+= atmel.o
spi-nor-objs			+= catalyst.o
spi-nor-objs			+= dosilicon.o
spi-nor-objs			+= eon.o
spi-nor-objs			+= esmt.o
spi-nor-objs			+= everspin.o
spi-nor-objs			+= fujitsu.o
spi-nor-objs			+= gigadevice.o
spi-nor-objs			+= intel.o
spi-nor-objs			+= issi.o
spi-nor-objs			+= macronix.o
spi-nor-objs			+= micron-st.o
spi-nor-objs			+= spansion.o
spi-nor-objs			+= sst.o
spi-nor-objs			+= winbond.o
spi-nor-objs			+= xilinx.o
spi-nor-objs			+= xmc.o
spi-nor-objs			+= fudanmicro.o
obj-$(CONFIG_MTD_SPI_NOR)	+= spi-nor.o
obj-$(CONFIG_SPI_MC_SFC)	+= mc-sfc.o
obj-$(CONFIG_MTD_SPI_NOR)	+= controllers/
