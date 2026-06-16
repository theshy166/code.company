ifeq ($(CHIP_NAME), mc331x)
SRC := qua_audio_sample_mc331x.c
else ifeq ($(CHIP_NAME), qm10xh)
SRC := qua_audio_sample_qm10xh.c
else ifeq ($(CHIP_NAME), qm10xv)
SRC := qua_audio_sample_qm10xv.c
else ifeq ($(CHIP_NAME), qm10xd)
SRC := qua_audio_sample_qm10xv.c
else
SRC := qua_audio_sample.c
endif

OBJ = $(patsubst %.c,$(OUT_DIR)%.o,$(SRC))

ifeq ($(OS_NAME), rtt)
QUAMMAPI_BIN = qua_audio_sample.elf
else
QUAMMAPI_BIN = qua_audio_sample
endif

QUAMMAPI_CFLAGS := -I./include -I$(COMMON_INC)
QUAMMAPI_CFLAGS += -Wall

QUAMMAPI_LDFLAGS := -L$(LINK_COMMON_LIB_INC) -lquamm_common -L$(LINK_UTILS_LIB_INC) -lquamm_utils

ifeq ($(OS_NAME), rtt)
QUAMMAPI_LDFLAGS += -static -T$(RTT_SCONS_LDS_PATH)
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
