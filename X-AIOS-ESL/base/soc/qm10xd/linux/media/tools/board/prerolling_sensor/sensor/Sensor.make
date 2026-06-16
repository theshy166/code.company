-include ../../../personal.make

export ISP_ROOT ?= $(shell pwd)/../../
export CHIP_ID ?= HL
export RTT ?= 0
export STATIC ?= 1
export DEBUG ?= 0
export CROSS_COMPILE ?= arm-molv510-linux-
export RTT_CROSS_COMPILE ?= arm-none-eabi-
export RTT_ARC_CROSS_COMPILE ?=arc-fullhan-elf32-
export RTOSV3 ?= 1
export FPGA_ENV ?= 0
export BUILD_MODE ?= 0
export LIB_ROOT_EN ?= 0
###### new add ###########
TOP_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

INSTALL_DIR ?= /home/nfs/opt/test
DSP_ROOT ?= $(HOME)/work/dsp
ISP_ROOT ?= $(HOME)/work/isp
LIBPATH=./


SENSOR_NAME?=$(shell pwd | sed 's/^\(.*\)[/]//')

ifneq (,$(filter $(BUILD_MODE),1 5))
RTT_TARGET = $(LIBPATH)/lib$(SENSOR_NAME)_rtt.a
TARGET_BIN = $(RTT_TARGET)
endif
ifneq (,$(filter $(BUILD_MODE),2 3))
RTT_TARGET = $(LIBPATH)/lib$(SENSOR_NAME)_rtt.a
TARGET_BIN = $(RTT_TARGET)
RPC_RTT=1
endif
ifneq (,$(filter $(BUILD_MODE),0 4))
LINUX_TARGET = $(LIBPATH)/lib$(SENSOR_NAME).a
TARGET_BIN = $(LINUX_TARGET)
endif
CSRC= $(wildcard *.c)
TEMSRC= $(wildcard *.hex)
TEMSRC := $(patsubst %.hex,%.c,$(TEMSRC))
CSRC+=../sensor_common/sensor_ops.c
CSRC+=../sensor_common/clock_ops.c
CSRC+=../sensor_common/gpio_reset.c
EXTRA_CFLAGS += -Wall -Werror -DISP -DARMLIB
EXTRA_CFLAGS += -I$(ISP_ROOT)/include/isp \
		-I$(ISP_ROOT)/include/isp_ext \
		-I$(ISP_ROOT)/include \
		-I$(ISP_ROOT)/sensor/sensor_common \
		-I$(ISP_ROOT)/drivers/fh_std/include


obj-m = $(patsubst %.ko,%.o,$(LINUX_TARGET))
$(patsubst %.ko,%,$(LINUX_TARGET))-y = $(patsubst %.c,%.o,$(CSRC))

ifeq ($(MS),1)
EXTRA_CFLAGS += -DMULTI_SENSOR=1
endif

ifeq ($(EMU),1)
EXTRA_CFLAGS += -DEMU
endif

ifeq ($(FPGA_ENV),1)
EXTRA_CFLAGS+=-DFPGA_ENV
endif

ifeq ($(RTOSV3),1)
EXTRA_CFLAGS+=-DRTOSV3
endif

ifeq ($(LIB_ROOT_EN), 1)
EXTRA_CFLAGS += -I$(LIB_ROOT)/isp
EXTRA_CFLAGS += -I$(LIB_ROOT)/isp_ext
EXTRA_CFLAGS += -I$(LIB_ROOT)/dsp
EXTRA_CFLAGS += -I$(LIB_ROOT)/../../inc
endif

ifeq ($(RTT),1)
export BUILD_MODE=1
endif
#############for RTT############################################
ifneq ($(RTT_TARGET),)
CC = $(RTT_CROSS_COMPILE)gcc
AR = $(RTT_CROSS_COMPILE)ar
EXTRA_CFLAGS+=$(RTT_EXTRA_CFLAGS)
EXTRA_CFLAGS+=-DOS_RTT
EXTRA_CFLAGS+=-D${CHIP_ID}
ifeq ($(RTT3),1)
EXTRA_CFLAGS+=-DUSE_RTT3_0
endif
EXTRA_CFLAGS+=-DHAVE_CCONFIG_H

ifeq ($(RPC_RTT),1)
export RTT_CROSS_COMPILE=$(RTT_ARC_CROSS_COMPILE)
export ARCH=ARC
EXTRA_CFLAGS+=-DRPC_RTT
endif

ifeq ($(OS_NONE),1)
export RTT_CROSS_COMPILE=$(RTT_ARC_CROSS_COMPILE)
export ARCH=ARC
EXTRA_CFLAGS+=-DOS_NONE
endif

ifeq ($(ARCH),arm)
CFLAGS+= -mno-unaligned-access -ffunction-sections -fdata-sections -fno-omit-frame-pointer -mapcs
ifeq ($(A7),1)
CFLAGS+= -mcpu=cortex-a7
else
CFLAGS+= -mcpu=arm1176jzf-s
endif
ifeq ($(VFP),1)
	ifeq ($(A7),1)
	CFLAGS += -mfpu=neon-vfpv4 -mfloat-abi=hard
	else
	CFLAGS += -mfpu=vfp -mfloat-abi=softfp
	endif
else
	CFLAGS += -mfloat-abi=soft
endif
else # ($(ARCH),arc)
CFLAGS+= -mno-sdata -mA6 -mmul32x16 -mno-volatile-cache
AFLAGS = ' -c  -mno-sdata -mA6 -x assembler-with-cpp'
endif

ifeq ($(DEBUG),1)
CFLAGS += -O0 -gdwarf-2 -DRT_USING_NEWLIB
else
CFLAGS += -O2
endif

CFLAGS+=$(EXTRA_CFLAGS)
CFLAGS += -I$(RTT_ROOT)/rtt_kernel/include
CFLAGS += -I$(RTT_ROOT)/rtt_kernel/components/drivers/include
CFLAGS += -I$(RTT_ROOT)/rtt_kernel/components/drivers/include/drivers
CFLAGS += -I$(RTT_ROOT)/rtt_kernel/components/finsh
CFLAGS += -I$(RTT_ROOT)/rtt_kernel/libcpu/arm/cortex-a/
CFLAGS += -I$(RTT_ROOT)/rtt_userapps/
endif


ifneq ($(LINUX_TARGET),)
CC = $(CROSS_COMPILE)gcc
AR = $(LINUX_CROSS_COMPILE)ar
EXTRA_CFLAGS += -DOS_LINUX
EXTRA_CFLAGS+=-D${CHIP_ID}
CFLAGS += $(EXTRA_CFLAGS)
endif

ifeq ($(STATIC),0)
TARGET_CP =$(basename $(TARGET_BIN)).so
else
TARGET_CP =$(basename $(TARGET_BIN)).a
endif

###### new add ###########
OBJFILES := $(patsubst %.c,%.o,$(CSRC))

all: lib prebuild-paraFile build-paraFile

lib: $(shell .././genversion.sh) $(TARGET_BIN)

ifeq ($(STATIC), 1)
$(TARGET_BIN): $(OBJFILES)
	$(AR) -rcs $(TARGET_CP) $^
else
$(TARGET_BIN): $(OBJFILES)
	$(Q)$(CC) -shared -fPIC -o $(TARGET_CP) $^
endif

####################################################

CUR_DIR         = $(CURDIR)/
BUILD_DIR       = $(ISP_ROOT)/build/linux/
ifeq ($(findstring $(ISP_ROOT),$(CURDIR)),)
BUILD_FOLDER_T  = $(CURDIR)/build/linux/
else
BUILD_FOLDER_T  = $(patsubst $(ISP_ROOT)%,$(BUILD_DIR)%/,$(CURDIR))
endif
BUILD_FOLDER    = $(patsubst %/src/,%/,$(BUILD_FOLDER_T))

ifneq ($(RTT_TARGET),)
install:
	@if [ ! -d "$(RTT_INSTALL_DIR)/sensors" ]; then mkdir $(RTT_INSTALL_DIR)/sensors; fi
ifeq ($(MS),1)
	@if [ ! -d "$(RTT_INSTALL_DIR)/sensors/multiple" ]; then mkdir $(RTT_INSTALL_DIR)/sensors/multiple; fi
	-cp -rf $(TARGET_CP) $(RTT_INSTALL_DIR)/sensors/multiple
else
	-cp -rf $(TARGET_CP) $(RTT_INSTALL_DIR)/sensors
endif
else
install:
	mkdir -p $(BUILD_FOLDER)
	-cp -rf $(TARGET_CP) $(BUILD_FOLDER)/
	-cp -rf $(TARGET_CP) $(INSTALL_DIR)/
endif

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(DEPENDS)
endif

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

.PHONY: build-headfile

prebuild-paraFile:
	mkdir -p parameters
	python3 ../prebuildHeadFile.py $(SENSOR_NAME) parameters

build-paraFile: $(TEMSRC)
%.c: %.hex
	mkdir -p parameters
	python3 ../hex2headfile.py $^ $@ parameters


.PHONY: delete-headfile

delete-paraFile:
	rm -rf parameters

clean: delete-paraFile
	find $(PWD) -name "*.o" | xargs rm -f
	find $(PWD) -name "*.d" | xargs rm -f
	find $(PWD) -name "*.a" | xargs rm -f
	find $(PWD) -name "*.so" | xargs rm -f