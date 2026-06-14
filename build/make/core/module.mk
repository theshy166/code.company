APPS_COBJS = $(APPS_CSRCS:.c=$(OBJEXT))
APPS_TARGET = $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(APPS_COBJS)))

GUIENGINE_COBJS = $(GUIENGINE_CSRCS:.c=$(OBJEXT)) $(GUIENGINE_ASRCS:.S=.S$(OBJEXT)) $(GUIENGINE_CXXSRCS:.cpp=$(OBJEXT)) $(LVDRIVERS_CSRCS:.c=$(OBJEXT))
GUIENGINE_TARGET = $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(GUIENGINE_COBJS)))

.PHONY: apps apps_clean guiengine guiengine_clean

guiengine:$(GUIENGINE_TARGET)
	@echo "guiengine is done!"

guiengine_clean:
	@rm -rf $(GUIENGINE_TARGET)
	@echo "guiengine clean is done!"

apps: $(APPS_TARGET)
	@echo "apps is done!"

apps_clean:
	@rm -rf $(APPS_TARGET)
	@echo "apps clean is done!"