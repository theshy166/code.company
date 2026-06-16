SRC := mpp_camera_pipeline_sample.c

OBJ = $(patsubst %.c,$(OUT_DIR)%.o,$(SRC))

QUAMMAPI_BIN = mpp_camera_pipeline_sample
QUAMMAPI_CFLAGS:= -I./include -I$(COMMON_INC) -I$(MPP_INC)
QUAMMAPI_LDFLAGS:=-L$(LINK_COMMON_LIB_INC) -lquamm_common -L$(LINK_UTILS_LIB_INC) -lquamm_utils -lquamm_mppipeline
QUAMMAPI_LDFLAGS += -L$(LINK_LIB_INC_MPI) $(LINK_MPI_LIBS) $(LINK_MPI_DEPENDENT_LIBS)
QUAMMAPI_LDFLAGS += -lisp -lispcore -ladvapi_smartir -ladvapi -lmipi

QUAMMAPI_CFLAGS += -Wall -g -o0

TARGET := $(OUT_DIR)$(QUAMMAPI_BIN)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(SRC)
	@echo "$(QUAMMAPI_CFLAGS)"
	$(CC) $(QUAMMAPI_CFLAGS) $(SRC) -o $(OUT_DIR)$(QUAMMAPI_BIN) $(QUAMMAPI_LDFLAGS)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)
