include ./build/make/core/menconfig.mk

#
# Makefile
# arm-mol-linux-uclibcgnueabihf-
CC 				:= $(CROSS_COMPILE)gcc
CXX				:= $(CROSS_COMPILE)g++

$(info debug CC = $(CC))
XOS_DIR 		?= .
XOS_CORE_DIR    ?= core
XOS_PKG_DIR     ?= core/package
XOS_BOARD_DIR	?= core/board
XOS_PRODUCT_DIR ?= product

#compile config
CONFIG_LVGL_VERSION 	?= lvgl-v9
#$(info lvglversion=$(CONFIG_LVGL_VERSION))
XOS_GUI_ENGINE_DIR 	?= $(XOS_PKG_DIR)/guiengine/$(CONFIG_LVGL_VERSION)
LVGL_DIR_NAME 	?= lvgl
LVGL_DIR 		?= $(XOS_GUI_ENGINE_DIR)

.PHONY: all prebuild clean default help

include $(XOS_DIR)/build/make/core/warning.mk

CFLAGS 			:= $(WARNINGS) \
					-D LV_CONF_INCLUDE_SIMPLE \
					-D _GNU_SOURCE \
					-I$(XOS_DIR)/inc \
					-I$(XOS_DIR)/$(XOS_CORE_DIR)/apps/inc \
					-I$(XOS_DIR)/core/board/generic/$(BOARD)/conf \
					-I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/quaextra/inc \
					-I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/quaextra/inc/lang \
					-I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/quacamera/include \
					-I$(XOS_DIR)/$(XOS_PKG_DIR)/framework/services/uart_service/inc

include $(XOS_DIR)/build/make/core/define.mk

#read project flag
include $(XOS_DIR)/$(XOS_PRODUCT_DIR)/common/projects.mk
include $(XOS_DIR)/$(XOS_PRODUCT_DIR)/common/projflag.mk
CFLAGS 			+= -I$(XOS_DIR)/$(XOS_GUI_ENGINE_DIR)

#
ifeq ($(CONFIG_XOS_USE_DEBUG),true)
CFLAGS 			+=  -g -O0
else
CFLAGS 			+=  -g0 -O3
endif

#ldflags
LDFLAGS 		?= 

#config target
BIN 			= qxosui
BUILD_DIR 		= ./out/$(_top_img_out_dir_)/xos
BUILD_OBJ_DIR 	= $(BUILD_DIR)/obj
BUILD_BIN_DIR 	= $(BUILD_DIR)/bin

#
$(shell mkdir -p $(BUILD_BIN_DIR))

#Collect the files to compile
#MAINSRC = 		./main.c
ifeq ($(BUILD_SHARED_APP),true)
include $(XOS_DIR)/build/make/shared_library.mk
else
ifneq ($(wildcard $(XOS_DIR)/$(XOS_GUI_ENGINE_DIR)/lvgl),)
#add special flag or copy resource or copy so libraries.
include $(XOS_DIR)/$(XOS_GUI_ENGINE_DIR)/lvgl/lvgl.mk
endif
ifeq ($(CONFIG_LVGL_VERSION),lvgl-v8)
include $(XOS_DIR)/$(XOS_GUI_ENGINE_DIR)/lv_drivers/lv_drivers.mk
endif
include $(XOS_DIR)/build/make/core/qcode.mk
endif

#CSRCS 			+=$(XOS_DIR)/mouse_cursor_icon.c

CPPFLAGS := $(filter-out -Wmissing-prototypes -std=c99, $(CFLAGS))
CPPFLAGS += -Wno-unused-variable -Wno-sized-deallocation --std=c++11 -fno-rtti

OBJEXT 			?= .o

AOBJS 			= $(ASRCS:.S=.S$(OBJEXT))
COBJS 			= $(CSRCS:.c=$(OBJEXT))
CPPOBJS 		= $(CPPSRCS:.cpp=$(OBJEXT))

MAINOBJ 		= $(MAINSRC:.c=$(OBJEXT))

SRCS 			= $(ASRCS) $(CSRCS) $(MAINSRC) $(CPPSRCS)
OBJS 			= $(AOBJS) $(COBJS) $(MAINOBJ) $(CPPOBJS)
TARGET 			= $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

## MAINOBJ -> OBJFILES
all: default

$(BUILD_OBJ_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

$(BUILD_OBJ_DIR)/%.S.o: %.S
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

$(BUILD_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

$(BUILD_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX)  $(CPPFLAGS) -c $< -o $@
	@echo "CC $<"

#-Wl,-rpath=./
default: $(TARGET)
	@$(CC) -Wl,-Map=$(BUILD_BIN_DIR)/qxosui.map -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(LDFLAGS)
	@echo "-----------------------size-------------------------" >> $(BUILD_BIN_DIR)/qxosui.map
	@$(CROSS_COMPILE)size -t $(TARGET) >> $(BUILD_BIN_DIR)/qxosui.map
	@echo "Here all compiling is done!"

clean:
	$(shell rm -rf $(BUILD_BIN_DIR)/$(BIN) 2>> /dev/null)
	$(shell rm $(TARGET) 2>> /dev/null)

help:
	@echo "CFLAGS = $(CFLAGS)"


include $(XOS_DIR)/build/make/core/rules.mk
include $(XOS_DIR)/build/make/core/module.mk
