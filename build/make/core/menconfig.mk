export XOS_ROOT := $(shell pwd)
export PATH := $(PATH):$(XOS_ROOT)/tools/ubuntu/kconfig
CONFIG_PATH = .config
CHIP_ID="QMxx"

JOB_NUMBER = $(word 1, $(filter -j%, $(MAKEFLAGS)))

.PHONY: clean xos menuconfig distclean xos-% %_defconfig

#-include $(CONFIG_PATH)
#-include ../Rules.make

distclean:
	rm -rf output
	rm -rf .config
	rm -rf .config.old
	rm -rf .config.env
	rm -rf .config.env.unset
	rm -rf .config.file
	rm -rf inc/generated/autoconf.h
	rm -rf product/.BoardConfig.mk
	rm -rf product/.Kconfig

%_defconfig:
	@CONFIG_FILE=$(shell find product/ -type f -name "$@") && \
	if [ -n "$$CONFIG_FILE" ]; then \
		echo "CONFIG_FILE found: $$CONFIG_FILE"; \
		cp $$CONFIG_FILE .config; \
		./build/make/create_kconfig_link.sh; \
		echo "$$CONFIG_FILE" >.config.file; \
	else \
		echo "$@ not exists"; \
	fi
	@python build/kconf/menu_anno.py .config inc/generated/autoconf.h $(CHIP_ID)
	@rm -f .config.old .config.cmd

menuconfig:
	@echo "XOS_ROOT: $(XOS_ROOT)"
	./build/make/create_kconfig_link.sh
	mconf $(XOS_ROOT)/Kconfig
	@python build/kconf/menu_anno.py .config inc/generated/autoconf.h $(CHIP_ID)
	@rm -f .config.old .config.cmd
	./build/make/create_kconfig_link.sh

savedefconfig:
	@echo "start savedefconfig in $(XOS_ROOT)"
	conf $< $(silent) --$@=defconfig $(XOS_ROOT)/Kconfig

xos:
	@echo "start cmake build all in $(XOS_ROOT)"
	@set -e
	@echo "MAKEFLAGS $(MAKEFLAGS)"
	@echo "JOBNUMBER $(JOB_NUMBER)"
	./build/script/build-xos.sh $(XOS_ROOT) $(JOB_NUMBER)
	./build/script/build-xos.sh $(XOS_ROOT) base $(JOB_NUMBER)
	@set +e
	@$(check_delete_dcim_dir)
	@$(check_delete_out_xos)

xos-%:
	@echo "start cmake build $* in $(XOS_ROOT)"
	./build/script/build-xos.sh $(XOS_ROOT) $* $(JOB_NUMBER)
	@$(check_delete_dcim_dir)
	@$(check_delete_out_xos)

define check_delete_dcim_dir
    if [ -d "DCIM" ]; then \
        rm -r DCIM; \
    fi
endef

define check_delete_out_xos
    if [ -d "out/xos" ]; then \
        rm -r out/xos; \
    fi
endef
