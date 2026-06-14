SRC := qua_player_sample.c
SRC2 := qua_player_sample2.c

QUAMMAPI_BIN = qua_player_sample
QUAMMAPI_BIN2 = qua_player_sample2
QUAMMAPI_CFLAGS := -I$(COMMON_INC)
QUAMMAPI_CFLAGS += -Wall

QUAMMAPI_LDFLAGS += -L$(LINK_COMMON_LIB_INC) -lquamm_common
QUAMMAPI_LDFLAGS += -L$(LINK_UTILS_LIB_INC) -lquamm_utils
QUAMMAPI_LDFLAGS += -L$(LINK_PLAYER_LIB_INC) -l"quamm.player"

ifeq ($(OS_NAME), rtt)
    QUAMMAPI_BIN = qua_player_sample.elf
    QUAMMAPI_LDFLAGS += -static -Wl,--gc-sections -T$(RTT_SCONS_LDS_PATH)
    QUAMMAPI_LDFLAGS += -L$(YUV_LIB_INC) -lyuv
endif

TARGET := $(OUT_DIR)$(QUAMMAPI_BIN)
TARGET2 := $(OUT_DIR)$(QUAMMAPI_BIN2)

.PHONY : clean all
all: $(TARGET) $(TARGET2)

$(TARGET):$(SRC)
	$(CC) $(QUAMMAPI_CFLAGS) $(SRC) -o $(OUT_DIR)$(QUAMMAPI_BIN) $(QUAMMAPI_LDFLAGS)
$(TARGET2):$(SRC2)
	$(CC) $(QUAMMAPI_CFLAGS) $(SRC2) -o $(OUT_DIR)$(QUAMMAPI_BIN2) $(QUAMMAPI_LDFLAGS)

clean:
	@rm -f $(TARGET) $(TARGET2)
	@rm -f $(OBJ)
