export XOS_APPS_DIR_NAME ?= $(XOS_CORE_DIR)/apps

APPS_CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/src && find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/src -name "*.c")

ifeq ($(CONFIG_XOS_APP_ESL2),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/esl2 -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_NATIVE_ESL),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/h5esl -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_SCANPEN),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/scanpen -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOTO),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/moto -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOTOX),true)
APPS_CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox && find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOTOX_RD),true)
APPS_CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox_rd && find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox_rd -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOTOX7IN),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox7in -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOTOX102D),true)
APPS_CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox102d && find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motox102d -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOTOT),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/motot -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_LAYER_DEMO),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/layerdemo -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_SMART_DOOR),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/smartdoor -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_IMAGEVIEWER),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/imageviewer -name "*.c")
endif

ifeq ($(CONFIG_XOS_APP_XIAOMING),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/xiaoming -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_BOOT_ANIMATE),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/bootanimate -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_FILEEXPLORER),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/fileexplorer -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_GALLERY),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/gallery -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_FACE_RECOGNIZE),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/facerecognize -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_LOTTIEDEMO),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/lottiedemo -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_LAUNCHER),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/launcher -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_AUDIO_PLAYER),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/audioplayer -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_ANALOG_CLOCK),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/analogclock -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_HEALTH),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/health -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_VIDEO_CALL),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/videocall -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_MOMO_TALK),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/momotalk -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_SCANNER),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/scanner -name "*.c")
endif

# ifeq ($(CONFIG_XOS_USE_APP_DIGITAL_CLOCK),true)
# APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/digitalclock -name "*.c")
# endif

ifeq ($(CONFIG_XOS_USE_APP_COMMON),true)
APPS_CSRCS += $(shell test -d $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/common && find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/common -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_DEMOAPP),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/demoapp -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_DEMOLIST),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/demolist -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_COOLFLORID),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/coolflorid -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_FILEEXPLORER),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/fileexplorer -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_QETV),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/qetv -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_SETTING),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/setting -name "*.c")
endif

ifeq ($(CONFIG_XOS_APP_SETTING_PAD),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/settingpad -name "*.c")
endif
ifeq ($(CONFIG_XOS_USE_APP_SMARTHOME),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/smarthome -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_CAMERA),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/camera -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_AICAMERA),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/aicamera -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_SPHOTO),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/sphoto -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_DEMO),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/demo -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_CALENDAR),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/calendar -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_ALARM),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/alarm -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_CALCULATOR),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/calculator -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_AIBOX),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/aibox -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_AICHAT),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/aimodel -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_CASTSCREEN),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/castscreen -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_SETUPWIZARD),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/setupwizard -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_WIFIX),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/wifix -name "*.c")
endif

ifeq ($(CONFIG_XOS_USE_APP_STUDYPILOT),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/studypilot -name "*.c")
endif

CSRCS += $(APPS_CSRCS)
