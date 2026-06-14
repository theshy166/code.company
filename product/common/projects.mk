PROJECT_CONF_DIR=$(XOS_DIR)/$(XOS_PRODUCT_DIR)
ifeq ($(wildcard $(XOS_DIR)/product/$(BUILD_PROJECT)),)
	PROJECT_DIR_NAME=$(word 1,$(subst _, ,$(strip $(BUILD_PROJECT))))
else
	PROJECT_DIR_NAME=$(BUILD_PROJECT)
endif

include $(PROJECT_CONF_DIR)/common/project_base_conf.mk

ifeq ($(BUILD_PROJECT),)
include $(PROJECT_CONF_DIR)/common/project_default_conf.mk
else
include $(PROJECT_CONF_DIR)/$(PROJECT_DIR_NAME)/project_$(BUILD_PROJECT)_conf.mk
endif
