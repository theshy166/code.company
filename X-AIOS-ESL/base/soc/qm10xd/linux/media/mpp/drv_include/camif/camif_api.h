#ifndef CAMIF_API_H
#define CAMIF_API_H
#include <cam_common.h>

typedef enum{
    CAMIF_STREAMON_NORMAL,//just prerolling
    CAMIF_STREAMON_TUNE_OFFLINE,//tune mode -> ddr
    CAMIF_STREAMON_TUNE_ONLINE,//tune mode->isp460, not need buf
    CAMIF_STREAMON_NUM
}Camif_Stream_On_Mode_E;

typedef enum{
    CAMIF_PORT_TYPE_DVP,
    CAMIF_PORT_TYPE_P2S,//只支持raw8
    CAMIF_PORT_TYPE_SCB,
    CAMIF_PORT_TYPE_SPI,
    CAMIF_PORT_TYPE_PIXART,
    CAMIF_PORT_TYPE_NUM
}Camif_Port_Type_E;

typedef struct
{
    struct platform_device *pDev;
    int irq;
    struct clk *camif_en_clk;
    struct clk *camif_freq_clk;
    UINT32 reserve;
}Camif_Init_Input_Param_S;

typedef struct
{
    UINT32 reserve;
}Camif_Deinit_S;

typedef struct
{
    Camif_Stream_On_Mode_E streamOnMode;
    int pic_bypass;
}Camif_StreamOn_S;

typedef struct
{
    Camif_Port_Type_E port_type;
    UINT32 reserve;
}Camif_StreamOff_S;

typedef struct{
	int (*init)(Camif_Init_Input_Param_S *input_param);
	int (*setcfg)(int cmd, void *input_param, void *output_param);
	int (*getcfg)(int cmd, void *input_param, void *output_param);
	int (*stream_on)(Camif_StreamOn_S *input_param);
	int (*stream_off)(Camif_StreamOff_S *input_param);
	int (*deinit)(Camif_Deinit_S *input_param);
}CAMIF_EXPORT_FUNC_S;

int camif_init(Camif_Init_Input_Param_S *input_param);
int camif_setconfig(int cmd, void *input_param, void *output_param);
int camif_getconfig(int cmd, void *input_param, void *output_param);
int camif_stream_on(Camif_StreamOn_S *input_param);
int camif_stream_off(Camif_StreamOff_S *input_param);
int camif_deinit(Camif_Deinit_S *input_param);
#endif
