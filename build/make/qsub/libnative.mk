#
# libnative Makefile
#
.PHONY: libnative

AR	    ?= $(CROSS_COMPILE)ar
ARFLAGS = rcv

QLIBNATIVE_SUBDIRS = qliblosextra

SA_TARGETS = $(addsuffix .a,  $(QLIBNATIVE_SUBDIRS))
SO_TARGETS = $(addsuffix .so, $(QLIBNATIVE_SUBDIRS))


$(INFO $(SA_TARGETS))
$(INFO $(SO_TARGETS))

libnative:	$(SA_TARGET) $(SO_TARGET)

BUILD_LIB_DIR 	= $(BUILD_DIR)/lib

$(BUILD_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

$(BUILD_OBJ_DIR)/%.o: %.c
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(COMPILE.c) $(OUTPUT_OPTION) $< -o $@

$(SA_TARGET):	$(OBJS)
	$(QUIET_AR)$(AR) $(ARFLAGS) $@ $^
	$(QUIET_RANLIB)$(RANLIB) $@

export XOS_LIBNATIVE_DIR_NAME ?= $(XOS_PKG_DIR)/libnative


# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/player -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/quampi -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/ability -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/components -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/finger -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/floatwindow -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/debugger -maxdepth 3 -name "*.c")
# CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_LIBNATIVE_DIR_NAME)/$(QLIBNATIVE_SUBDIRS)/parser -maxdepth 3 -name "*.c")
