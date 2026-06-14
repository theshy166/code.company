export XOS_RES_DIR_NAME ?=$(XOS_CORE_DIR)/res

$(info strings PROJECT_DIR_NAME=$(PROJECT_DIR_NAME))
ifeq ($(wildcard $(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/strings),)
	CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_RES_DIR_NAME) && find -L $(XOS_DIR)/$(XOS_RES_DIR_NAME) -name "*.c")
else
	CSRCS += $(shell test -d $(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/strings && find -L $(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/strings -name "*.c")
	CSRCS += $(shell test -d $(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/menu && find -L $(XOS_DIR)/product/$(PROJECT_DIR_NAME)/src/menu -name "*.c")
endif
