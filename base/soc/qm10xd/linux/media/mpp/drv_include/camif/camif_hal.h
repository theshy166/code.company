#ifndef CAMIF_HAL_H
#define CAMIF_HAL_H
#include <cam_common.h>
#include <camif_api.h>
#if 0
FY_S32 HAL_CAMIF_INIT(void);
FY_S32 HAL_CAMIF_SET_PORT_INFO(Camif_Port_Conf_S portConf);
FY_S32 HAL_CAMIF_SET_VIATTR(Camif_Viattr camifViattr);
FY_S32 HAL_CAMIF_STREAMON(void);
FY_S32 HAL_CAMIF_SET_BUF(Camif_Buf_S rawecBuf);
FY_S32 HAL_CAMIF_SET_2ACONFIG(Camif_2aconfig config);
FY_S32 HAL_CAMIF_SET_SHMMCONFIG(Camif_Cmd_Set_Axi_Pulse_Conf_Input_Param_S pulse);
FY_S32 HAL_CAMIF_STREAMOFF(Camif_StreamOff_S streamOff);
FY_S32 HAL_CAMIF_GET_STREAM(Camif_GetStream_S* pInfo);
FY_S32 HAL_CAMIF_SET_CROP(Camif_Crop_Conf_S iParam);
FY_S32 HAL_CAMIF_SET_SIZE(Camif_Size_Conf_S iParam);
FY_S32 HAL_CAMIF_SET_FORMAT(Camif_Format_Conf_S iParam);
FY_S32 HAL_CAMIF_SET_AXI_PULSE(Camif_Cmd_Set_Axi_Pulse_Conf_Input_Param_S iParam);
FY_S32 HAL_CAMIF_SET_SKIP_CNT(Camif_Skip_Frame_Cnt_S iParam);
FY_S32 HAL_CAMIF_SET_RAWEC_ENABLE(Camif_Cmd_Set_Rawec_Enable_Input_Param_S iParam);
FY_S32 HAL_CAMIF_SET_AXI_PULSE_ENABLE(Camif_Cmd_Set_Axi_Pulse_Enable_Input_Param_S iParam);
FY_S32 HAL_CAMIF_SET_SHMM_ENABLE(Camif_Cmd_Set_Shmm_Enable_Input_Param_S iParam);
FY_S32 HAL_CAMIF_SET_AECONFIG(Camif_Ae_Conf_S config);
FY_S32 HAL_CAMIF_SET_AWBCONFIG(Camif_Awb_Conf_S config);
FY_S32 HAL_CAMIF_SET_BLCCONFIG(Camif_Blc_Conf_S config);
FY_S32 HAL_CAMIF_DEINIT(Camif_Deinit_S iParam);
#endif
#endif
