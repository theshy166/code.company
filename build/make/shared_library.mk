
#flag
CFLAGS += -fPIC

#for qxosui
ifeq ($(module),)
#-lxx -lxx
ifneq ($(linkmodules),)
new_linkmodules:=$(shell echo "${linkmodules}" | sed -e 's/;/ /g')
endif
# add to ldflag
LDFLAGS 		+=$(new_linkmodules)
LDFLAGS 		+= -llvgl
LDFLAGS			+= -L$(BUILD_BIN_DIR)
else
# for module
override BIN 			= lib$(module).so
endif

LDFLAGS			+= -shared

#for module: lvgl or app
ifeq ($(module),lvgl)
include $(XOS_DIR)/gui_engine/$(CONFIG_LVGL_VERSION)/lvgl/lvgl.mk
else
include $(XOS_DIR)/build/make/core/qcode.mk
endif
