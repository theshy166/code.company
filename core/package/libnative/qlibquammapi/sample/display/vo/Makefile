SRC := qua_vo_sample.c

OBJ = $(patsubst %.c,$(OUT_DIR)%.o,$(SRC))

QUAMMAPI_BIN = qua_vo_sample
QUAMMAPI_CFLAGS := -I./include -I$(COMMON_INC)
QUAMMAPI_LDFLAGS := -L$(LINK_COMMON_LIB_INC) -lquamm_common -L$(LINK_UTILS_LIB_INC) -lquamm_utils
QUAMMAPI_CFLAGS += -Wall

ifeq ($(OS_NAME), rtt)
QUAMMAPI_BIN = qua_vo_sample.elf
QUAMMAPI_LDFLAGS += -static -Wl,--gc-sections -T$(RTT_SCONS_LDS_PATH)
QUAMMAPI_LDFLAGS += -L$(YUV_LIB_INC) -lyuv
endif

TARGET := $(OUT_DIR)$(QUAMMAPI_BIN)

.PHONY : clean all
all: $(TARGET)

$(TARGET):$(SRC)
	$(CC) $(QUAMMAPI_CFLAGS) $(SRC) -o $(OUT_DIR)$(QUAMMAPI_BIN) $(QUAMMAPI_LDFLAGS)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)
