export XOS_DEVICE_DIR_NAME ?= core/board

#CSRCS += $(shell $(CMD_FIND) $(XOS_DIR)/$(LV_QCORE_DIR_NAME) -type f -name "*.c" -not -path "*/\.*")
#CSRCS += $(wildcard $(XOS_DIR)/$(LV_QCORE_DIR_NAME)/*.c)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_DEVICE_DIR_NAME) && find -L $(XOS_DIR)/$(XOS_DEVICE_DIR_NAME) -name "*.c")
#$(info debug XOS_DEVICE_DIR_NAME = $(XOS_DEVICE_DIR_NAME))
###
## Filter out all $(VENDOR) source files and add appointed vendor and board source files
###
GENERIC_DIR = $(XOS_DEVICE_DIR_NAME)/generic
#$(info debug GENERIC_DIR = $(GENERIC_DIR))
GENERIC_DIR_SRC := $(shell test -d $(XOS_DIR)/$(GENERIC_DIR) && find -L $(XOS_DIR)/$(GENERIC_DIR) -name "*.c")
CSRCS := $(filter-out $(GENERIC_DIR_SRC), $(CSRCS))

LV_BOARD_DIR = $(XOS_DEVICE_DIR_NAME)/generic/$(BOARD)
CSRCS += $(shell test -d $(XOS_DIR)/$(LV_BOARD_DIR) && find -L $(XOS_DIR)/$(LV_BOARD_DIR) -name "*.c")
#$(info debug LV_BOARD_DIR = $(LV_BOARD_DIR))

GENERIC_BOARD_DRIVER_DIR = $(XOS_DEVICE_DIR_NAME)/generic/$(BOARD)/driver
ifneq ($(wildcard $(GENERIC_BOARD_DRIVER_DIR)),)
#$(info debug GENERIC_BOARD_DRIVER_DIR = $(GENERIC_BOARD_DRIVER_DIR))
GENERIC_BOARD_DRIVER_DIR_SRC := $(shell test -d $(XOS_DIR)/$(GENERIC_BOARD_DRIVER_DIR) && find -L $(XOS_DIR)/$(GENERIC_BOARD_DRIVER_DIR) -name "*.c")
CSRCS := $(filter-out $(GENERIC_BOARD_DRIVER_DIR_SRC), $(CSRCS))
endif

#add special flag or copy resource or copy so libraries.
include $(XOS_DIR)/$(XOS_DEVICE_DIR_NAME)/generic/$(BOARD)/device_board.mk
