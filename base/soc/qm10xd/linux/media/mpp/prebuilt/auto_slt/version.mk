#********************* Macro for version management*****************************
VER_X ?= 1
VER_Y ?= 0
VER_Z ?= 0
VER_P ?= 0
## generate the version from the git commit

ifneq ( $(SDK_BASE_TAG), )
        REV_COMMIT:=$(shell git describe --tags --long --always --dirty='-dev' --match="$(SDK_BASE_TAG)"  2>/dev/null)
endif

TMP_TAG_VER:=$(findstring g, $(REV_COMMIT))
ifeq ($(TMP_TAG_VER),)
    REV_COMMIT:=$(shell git describe --tags --long --always --dirty='-dev'  2>/dev/null)
endif

VER_B ?= $(if $(REV_COMMIT),$(REV_COMMIT),00)
COMPILE_BY := $(shell whoami | sed 's/[^A-Za-z0-9_]/_/g')
COMPILE_HOST :=$(shell hostname | sed 's/[^A-Za-z0-9_]/_/g')
VER_P=$(COMPILE_BY)@$(COMPILE_HOST)
ccflags-y += -DVER_B=$(VER_B)
# End!!
#include Makefile.version
