###########################fy00 filter-out############################
#
# Filter out fbdev.c in lv_drivers/display/fbdev.c
#
ifeq ($(BOARD), fy00)
###
## Filter out all $(import) source files
###

FILTER_OUT_SRC = %display/fbdev.c
FILTER_SRC := $(filter $(FILTER_OUT_SRC), $(CSRCS))
$(info debug FILTER_SRC = $(FILTER_SRC))
CSRCS := $(filter-out $(FILTER_OUT_SRC), $(CSRCS))
endif

# fn_get_current_dir = $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
# $(info debug currentPATH = $(fn_get_current_dir))


###########################fy00rtt filter-out############################
###
## Filter out all $(import) source files
###
#IMPORT_DIR = $(XOS_DEVICE_DIR_NAME)/generic/$(BOARD)/import
#IMPORT_DIR_SRC := $(shell find -L $(XOS_DIR)/$(IMPORT_DIR) -name "*.c")
#CSRCS := $(filter-out $(IMPORT_DIR_SRC), $(CSRCS))

#
# Filter out fbdev.c in lv_drivers/display/fbdev.c
#
ifeq ($(BOARD), fy00rtt)
FILTER_OUT_SRC = %display/fbdev.c %indev/evdev.c
FILTER_SRC := $(filter $(FILTER_OUT_SRC), $(CSRCS))
$(info debug FILTER_SRC = $(FILTER_SRC))
CSRCS := $(filter-out $(FILTER_OUT_SRC), $(CSRCS))
endif
###########################simulator filter-out############################
ifeq ($(BOARD), simulator)
$(info "")
endif

###########################fy00 filter-out############################
#
# Filter out fbdev.c in lv_drivers/display/fbdev.c
#
ifeq ($(BOARD), qm10xh)
###
## Filter out all $(import) source files
###
FILTER_OUT_SRC = %display/fbdev.c
FILTER_SRC := $(filter $(FILTER_OUT_SRC), $(CSRCS))
$(info debug FILTER_SRC = $(FILTER_SRC))
CSRCS := $(filter-out $(FILTER_OUT_SRC), $(CSRCS))
endif