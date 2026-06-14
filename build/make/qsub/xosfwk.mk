export XOS_FWK_DIR_NAME ?=$(XOS_PKG_DIR)/framework

QFWK_SUBDIRS = quaextra

# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS) -name "*.c")

CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/player && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/player -maxdepth 3 -name "*.c")
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/font && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/font -maxdepth 3 -name "*.c")
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/ability && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/ability -maxdepth 3 -name "*.c")
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/components && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/components -maxdepth 3 -name "*.c")
##CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/finger -maxdepth 3 -name "*.c")
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/floatwindow && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/floatwindow -maxdepth 3 -name "*.c")
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/debugger && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/debugger -maxdepth 3 -name "*.c")
##CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/parser -maxdepth 3 -name "*.c")
##CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/appmanager -maxdepth 3 -name "*.c")

ifeq ($(CONFIG_XOS_USE_INNER_RES),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/lang && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/lang -maxdepth 3 -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_INNER_CSC),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/csc && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/csc -maxdepth 3 -name "*.c")
endif

ifeq ($(CONFIG_XOS_EXT_CJSON),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/menu && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/menu -maxdepth 3 -name "*.c")
endif


ifeq ($(CONFIG_XOS_FWK_UTILITY),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/utility && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/utility -type f -name "*.c")
endif

QFWK_SUBDIRS_PLUS = quaplus

# ifeq ($(CONFIG_XOS_FWK_APPMANAGER),true)
# CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS_PLUS)/appmanager && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS_PLUS)/appmanager -maxdepth 3 -name "*.c")
# endif

# ifeq ($(CONFIG_XOS_FWK_PARAM),true)
# CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS_PLUS)/param && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS_PLUS)/param -maxdepth 3 -name "*.c")
# endif


ifeq ($(CONFIG_XOS_FWK_OTA_UART),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/services/uart_service && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/services/uart_service -maxdepth 3 -name "*.c")
endif

ifeq ($(CONFIG_XOS_FWK_MQTTSER_MIDDLE),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/services/mqtt_service && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/services/mqtt_service -maxdepth 3 -name "*.c")
endif

ifeq ($(CONFIG_XOS_FWK_CADP),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/src && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/src -type f -name "*.c")
ifeq ($(CONFIG_XOS_FWK_CADP_KUQI),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/aivendor/kuqi/coapi_uv && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/aivendor/kuqi/coapi_uv -type f -name "*.c")
endif
ifeq ($(CONFIG_XOS_FWK_CADP_QUA),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/qua/quayun && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/qua/quayun -type f -name "*.c")
endif
ifeq ($(CONFIG_XOS_FWK_CADP_KVS),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/aivendor/aws-kvs/src && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/aivendor/aws-kvs/src -type f -name "*.c")
endif
ifeq ($(CONFIG_XOS_FWK_CADP_AGORA),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/aivendor/agora && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/cloudadp/aivendor/agora -type f -name "*.c")
endif
endif

ifeq ($(CONFIG_XOS_FWK_LCDSRV),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/services/lcd_service && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/services/lcd_service -maxdepth 3 -name "*.c")
endif

ifeq ($(CONFIG_XOS_FWK_CADP_CURL),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/quacurl/src && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/quacurl/src -type f -name "*.c")
endif

ifeq ($(CONFIG_XOS_FWK_CADP_DSN),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/quadevicesn/src && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/$(QFWK_SUBDIRS)/quadevicesn/src -type f -name "*.c")
endif

ifeq ($(CONFIG_XOS_FWK_QUAAITALK),true)
CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/quaaitalk/src && find -L $(XOS_DIR)/$(XOS_FWK_DIR_NAME)/quaaitalk/src -type f -name "*.c")
endif
