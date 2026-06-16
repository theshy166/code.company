#ifndef __FY_Camif_DRV_IOC_H__
#define __FY_Camif_DRV_IOC_H__


/******************************************************************************
    Include other header files
 *****************************************************************************/
#include <camif_api.h>
#include <camif.h>
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/******************************************************************************
    Macros, Enums, Structures definition list
 *****************************************************************************/

/******************************************************************************
    Camif driver ioctl definition list
 *****************************************************************************/
#define Camif_IOC_MAGIC_CODE   'C'
#define IOC_Camif_INIT                    _IOWR(Camif_IOC_MAGIC_CODE,0,int)
#define IOC_Camif_SET_PORT_INFO           _IOWR(Camif_IOC_MAGIC_CODE,1,Camif_Port_Conf_S)
#define IOC_Camif_SET_VI_ATTR             _IOWR(Camif_IOC_MAGIC_CODE,2,Camif_Viattr)
#define IOC_Camif_STREAMON                _IOWR(Camif_IOC_MAGIC_CODE,3,Camif_StreamOn_S)
#define IOC_Camif_SET_BUF                 _IOWR(Camif_IOC_MAGIC_CODE,4,Camif_Buf_S)
#define IOC_Camif_GET_RAWEC               _IOWR(Camif_IOC_MAGIC_CODE,5,Camif_Buf_info_S)
#define IOC_Camif_SET_2ACONFIG            _IOWR(Camif_IOC_MAGIC_CODE,6,Camif_2aconfig)
#define IOC_Camif_SET_SHMMCONFIG          _IOWR(Camif_IOC_MAGIC_CODE,11,Camif_Cmd_Set_Axi_Pulse_Conf_Input_Param_S)
#define IOC_Camif_STREAMOFF               _IOWR(Camif_IOC_MAGIC_CODE,12,Camif_StreamOff_S)
#define IOC_Camif_GETSTREAM               _IOWR(Camif_IOC_MAGIC_CODE,13,Camif_GetStream_S)
#define IOC_Camif_SETCROP                 _IOWR(Camif_IOC_MAGIC_CODE,14,Camif_Crop_Conf_S)
#define IOC_Camif_SETSIZE                 _IOWR(Camif_IOC_MAGIC_CODE,15,Camif_Size_Conf_S)
#define IOC_Camif_SETFORMAT               _IOWR(Camif_IOC_MAGIC_CODE,16,Camif_Format_Conf_S)
#define IOC_Camif_SETAXIPULSE             _IOWR(Camif_IOC_MAGIC_CODE,17,Camif_Cmd_Set_Axi_Pulse_Conf_Input_Param_S)
#define IOC_Camif_SETSKIPCNT              _IOWR(Camif_IOC_MAGIC_CODE,18,Camif_Skip_Frame_Cnt_S)
#define IOC_Camif_SETRAEWECENABLE         _IOWR(Camif_IOC_MAGIC_CODE,19,Camif_Cmd_Set_Rawec_Enable_Input_Param_S)
#define IOC_Camif_SETAXIPULSEENABLE       _IOWR(Camif_IOC_MAGIC_CODE,20,Camif_Cmd_Set_Axi_Pulse_Enable_Input_Param_S)
#define IOC_Camif_SETSHMMENABLE           _IOWR(Camif_IOC_MAGIC_CODE,21,Camif_Cmd_Set_Shmm_Enable_Input_Param_S)
#define IOC_Camif_SETAECONF               _IOWR(Camif_IOC_MAGIC_CODE,22,Camif_Ae_Conf_S)
#define IOC_Camif_SETAWBCONF              _IOWR(Camif_IOC_MAGIC_CODE,23,Camif_Awb_Conf_S)
#define IOC_Camif_SETBLCCONF              _IOWR(Camif_IOC_MAGIC_CODE,24,Camif_Blc_Conf_S)
#define IOC_Camif_DEINIT                  _IOWR(Camif_IOC_MAGIC_CODE,25,Camif_Deinit_S)
#define IOC_Camif_GETAWBCONF              _IOWR(Camif_IOC_MAGIC_CODE,26,Camif_Awb_Conf_S)
#define IOC_Camif_GETBLCCONF              _IOWR(Camif_IOC_MAGIC_CODE,27,Camif_Blc_Conf_S)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__FY_Camif_DRV_H__
