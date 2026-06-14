ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif

obj-m = lcm.o

lcm-objs += reg/reg.o \
            reg/reg_pool.o \

lcm-objs += pin_mux/pin_lcd_v01_drv.o \

lcm-objs += ctrl/ctrl_lcm_v01_drv.o \

lcm-objs += clk/vou_clk_v01_drv.o \

lcm-objs += mcu/mol_lcm_mcu_api.o \
            mcu/mol_lcm_mcu_drv.o

lcm-objs += rgb/rgb_mpi.o \

lcm-objs += dsi/dsi_mpi.o \
             dsi/dsi_com.o \
             dsi/dsi_host_v01_drv.o \
             dsi/dsi_host_v01_drv_mpi.o \
             dsi/dsi_host_v01_reg_pool.o \
             dsi/dsi_phy_v01_drv.o \
             dsi/dsi_phy_v01_drv_mpi.o \
             dsi/dsi_phy_v01_reg_pool.o \
             dsi/dsi_pixel_info.o \
             dsi/dsi_timing_calc.o \
             dsi/dsi_phy_v0_pll_calc.o

lcm-objs += spi/spi_lcd/spi_cs_gpio_drv.o \
            spi/spi_lcd/spi_v01_irq.o \
            spi/spi_lcd/spi_v01_reg_pool.o \
            spi/spi_lcd/spi_v01_drv.o \
            spi/spi_lcd/spi_v01_drv_mpi.o \
            spi/spi_lcd/spi_data_dir.o \
            spi/spi_lcd/dma/spi_lcd_dma.o \
            spi/mol_lcm_spi_api.o \

lcm-objs += bus/mol_lcm_i2c.o \
            bus/mol_lcm_spi_gpio.o \
            bus/mol_lcm_sfc.o \
            bus/mol_gpio_spi.o \

lcm-objs += lcm_hal.o \
            mol_lcm.o \
            lcm_work_mgr.o \
            mc_lcm.o \
            lcm_com.o \
            os.o \
            debug.o \

EXTRA_CFLAGS += -I$(REL_INC) \


EXTRA_CFLAGS += $(DRV_CFLAGS) \
                $(MPP_CFLAGS) \

EXTRA_CFLAGS +=  -I$(PWD)/ \
                 -I$(PWD)/reg/ \
                 -I$(PWD)/pin_mux/ \
                 -I$(PWD)/clk/ \
                 -I$(PWD)/ctrl/ \
                 -I$(PWD)/dsi/ \
                 -I$(PWD)/bus/ \
                 -I$(PWD)/mcu/ \
                 -I$(PWD)/rgb/ \
                 -I$(PWD)/spi/ \
                 -I$(PWD)/spi/spi_lcd/ \
                 -I$(PWD)/spi/spi_lcd/dma/ \
                 -I$(PWD)/include/ \
                 -I$(PWD)/../lcm_module/include \
                 -I$(MPP_PATH)/drv_include \

#EXTRA_CFLAGS += -O2

ifeq ($(KERNEL_VER),4.9.138)
EXTRA_CFLAGS += -Wno-error=date-time
EXTRA_CFLAGS += -Wno-date-time
endif
ifeq ($(CONFIG_MULTIMEDIA_DEBUG), y)
EXTRA_CFLAGS += -DMM_DEBUG
endif

EXTRA_CFLAGS += -DARM_FW -DERR_DETECT -DTRACE_32=0 -D_MVC_=0
EXTRA_CFLAGS += -D$(CHIP) -DSW_BSM_0 -DSW_BSM_1 -DREG_MODE=1

ifeq ($(KERNEL_VER),5.10.y)
KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/osal/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/mmz/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/base/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/sys/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/lcm_module/Module.symvers
else
export KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/osal/Module.symvers
export KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/mmz/Module.symvers
export KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/base/Module.symvers
export KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/sys/Module.symvers
export KBUILD_EXTRA_SYMBOLS += $(DRIVER_PATH)/lcm_module/Module.symvers
endif

KERNEL_DIR = $(LINUX_ROOT)
#KERNEL_DIR :=/lib/modules/$(shell uname -r)/build

all:
	make O=.build -C $(KERNEL_DIR) M=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) modules
	@$(STRIP) *.ko
	@cp lcm.ko $(REL_KO)/lcm$(REL_TARGET_SUFFIX).ko
	@cp lcm.o $(REL_OBJ)/lcm$(REL_TARGET_SUFFIX).o
	#@cp lcm.ko $(MPP_PATH)/../../../nfs/lt00/ko

clean:
	make O=.build -C $(KERNEL_DIR) M=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) clean
