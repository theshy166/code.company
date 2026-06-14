
export XOS_DEVICE_DIR_NAME ?= core/board

ifneq ($(BUILD_SHARED_APP),true)
include $(XOS_DIR)/build/make/qsub/apps.mk
endif
include $(XOS_DIR)/build/make/qsub/resource.mk
include $(XOS_DIR)/build/make/qsub/device.mk
include $(XOS_DIR)/build/make/qsub/libnative.mk
include $(XOS_DIR)/build/make/qsub/xosfwk.mk
include $(XOS_DIR)/build/make/qsub/filter_out.mk
