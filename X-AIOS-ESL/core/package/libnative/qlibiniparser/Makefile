#
# iniparser Makefile
#
.PHONY: default clean test iniparse

# Compiler settings
CC      = $(CROSS_COMPILE)gcc
$(info $(CC))
CFLAGS  += -fPIC -Wall -Wextra -ansi -pedantic
ifndef DEBUG
ADDITIONAL_CFLAGS  ?= -O2
else
ADDITIONAL_CFLAGS  ?= -g
endif

CFLAGS += ${ADDITIONAL_CFLAGS}

# Ar settings to build the library
AR	    = $(CROSS_COMPILE)ar
ARFLAGS = rcv

SHLD = ${CC} ${CFLAGS}
LDSHFLAGS = -shared -Wl,-Bsymbolic
LDFLAGS += -Wl,-rpath -Wl,/usr/lib -Wl,-rpath,/usr/lib

# .so.0 is for version 3.x, .so.1 is 4.x
SO_TARGET ?= libiniparser.so.1

# Set RANLIB to ranlib on systems that require it (Sun OS < 4, Mac OSX)
# RANLIB  = ranlib
RANLIB = true

RM      ?= rm -f


# Implicit rules

SUFFIXES = .o .c .h .a .so .sl

COMPILE.c	?= $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

ifndef V
QUIET_AR	= @echo "AR	$@";
QUIET_CC	= @echo "CC	$@";
QUIET_LINK	= @echo "LINK	$@";
QUIET_RANLIB	= @echo "RANLIB	$@";
endif

BUILD_DIR 		= ./out


SRCS = src/iniparser.c src/dictionary.c

OBJx = $(SRCS:.c=.o)
OBJS = $(addprefix $(BUILD_DIR)/, $(patsubst ./%, %, $(OBJx)))

ORISRC_DIR = ../../libexternal/iniparser/

default:	libiniparser.a $(SO_TARGET)

$(BUILD_DIR)/%.o: %.c
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(COMPILE.c) $(OUTPUT_OPTION) $< -o $@

libiniparser.a:	$(OBJS)
	$(QUIET_AR)$(AR) $(ARFLAGS) $@ $^
	$(QUIET_RANLIB)$(RANLIB) $@

$(SO_TARGET):	$(OBJS)
	$(QUIET_LINK)$(SHLD) $(LDSHFLAGS) $(LDFLAGS) -o $(SO_TARGET) $(OBJS) \
		-Wl,-soname=`basename $(SO_TARGET)`

clean:
	$(RM) $(OBJS) libiniparser.a $(SO_TARGET) iniparse
	rm -rf $(BUILD_DIR)

test:
	mkdir -p ./out/.tmp/
	if [ ! -d "./out/.tmp/iniparser" ]; then cp -rf $(ORISRC_DIR)/ ./out/.tmp/; fi
	cd ./out/.tmp/iniparser/ && make V=1 CC=$(CC) AR=$(AR) check && make V=1 CC=$(CC) AR=$(AR) example


iniparse: iniparse.c
	$(CC) -g -o iniparse iniparse.c -Isrc -L. -liniparser
