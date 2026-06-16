#ifndef _ISP_API_H_
#define _ISP_API_H_


#include "isp_common.h"
#include "isp_sensor_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define lift_shift_bit_num(bit_num)			(1<<bit_num)
// #define DEBUG_API 1

#define CHECK_VALIDATION(x, a, b)                                                                                                  \
    do                                                                                                                             \
    {                                                                                                                              \
        int r;                                                                                                                     \
        if ((x < a) || (x > b))                                                                                                    \
        {                                                                                                                          \
            if (x < a)                                                                                                             \
                r = a;                                                                                                             \
            else                                                                                                                   \
                r = b;                                                                                                             \
            fh_printf("[WARNING]parameter out of range @%s %s= %d | range=[%d,%d] | auto clip to %d\n", __func__, #x, x, a, b, r); \
            x = r;                                                                                                                 \
        }                                                                                                                          \
    } while (0)

#define TRACE_API ispTrace_FunctionName(__func__)

#define FUNC_DEP __attribute__((deprecated))

enum ISP_HW_MODULE_LIST
{
    HW_MODUL_CLP           = lift_shift_bit_num(0),
    HW_MODUL_DPC           = lift_shift_bit_num(1),
    HW_MODUL_GAIN          = lift_shift_bit_num(2),
    HW_MODUL_NR2D          = lift_shift_bit_num(3),
    HW_MODUL_WB            = lift_shift_bit_num(4),
    HW_MODUL_RGBB          = lift_shift_bit_num(5),
    HW_MODUL_CGAMMA        = lift_shift_bit_num(6),
    HW_MODUL_YGAMMA        = lift_shift_bit_num(7),
    HW_MODUL_NR3D          = lift_shift_bit_num(8),
    HW_MODUL_APC           = lift_shift_bit_num(9),
    HW_MODUL_YIE           = lift_shift_bit_num(10),
    HW_MODUL_CIE           = lift_shift_bit_num(11),

    ISP_HW_MODULE_LIST_DUMMY=0xffffffff,
};

/**SYSTEM_CONTROL*/
/*
*   Name: API_ISP_MemInit
*            ISP内部使用的memory分配与初始化
*
*   Parameters:
*
*       [IN]  FH_UINT32 width
*            ISP处理的幅面宽度
*
*       [IN]  FH_UINT32 height
*            ISP处理的幅面高度
*
*   Return:
*            0(正确)
*           -1(ISP设备驱动打开失败)
*   Note:
*       无
*/
FH_SINT32 API_ISP_MemInit( FH_UINT32 width, FH_UINT32 height );
/*
*   Name: API_ISP_GetBinAddr
*            获取ISP的param参数的地址和大小
*
*   Parameters:
*
*       [OUT] FH_UINT32 *u32BinAddr
*            param的地址
*
*       [OUT] FH_UINT32 *u32BinSize
*            param的size
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetBinAddr(FH_UINT32 *u32BinAddr,FH_UINT32 *u32BinSize);
/*
*   Name: API_ISP_SetViAttr
*            配置vi相关的一些幅面信息
*
*   Parameters:
*
*       [IN] const ISP_VI_ATTR_S *pstViAttr
*            结构体ISP_VI_ATTR_S的指针
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetViAttr(const ISP_VI_ATTR_S *pstViAttr);
/*
*   Name: API_ISP_GetViAttr
*            获取当前幅面相关信息
*
*   Parameters:
*
*       [OUT] ISP_VI_ATTR_S *pstViAttr
*            结构体ISP_VI_ATTR_S的指针
*
*   Return:
*            0(正确)，
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetViAttr(ISP_VI_ATTR_S *pstViAttr);
/*
*   Name: API_ISP_SetVoInfo
*            配置输出图像相关的幅面信息
*
*   Parameters:
*
*       [IN] const ISP_SIZE *vo_size
*            结构体ISP_SIZE的指针
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetVoInfo(const ISP_SIZE *vo_size);
/*
*   Name: API_ISP_GetVoInfo
*            获取输出幅面相关信息
*
*   Parameters:
*
*       [OUT] ISP_SIZE *vo_size
*            结构体ISP_SIZE的指针
*
*   Return:
*            0(正确)，
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetVoInfo(ISP_SIZE *vo_size);
/*
*   Name: API_ISP_GetVoFrame
*            获取输出帧相关信息
*
*   Parameters:
*
*       [IN] FH_UINT32 timeout_ms
*            buf超时，若在timeout_ms时间内未获取到图像数据，则直接报错
*
*       [OUT] ISP_VO_FRAME *vo_frame
*            结构体ISP_VO_FRAME的指针
*
*   Return:
*            0(正确)，
*            非0(失败，详见错误码)
*   Note:
*       若在timeout_ms时间内未等到图像数据，则直接返回，一般建议设置时间超过1帧的时长，当buffer数量不小于3时（默认是2块buf），可减少timeout时间
*/
FH_SINT32 API_ISP_GetVoFrame(ISP_VO_FRAME *vo_frame, FH_UINT32 timeout_ms);
/*
*   Name: API_ISP_Init
*            初始化ISP硬件寄存器，并启动ISP
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*        -1003(ISP初始化异常)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Init(FH_VOID);
/*
*   Name: API_ISP_LoadIspParam
*            加载指定参数到DRV寄存器
*
*   Parameters:
*
*       [IN] char *isp_param_buff
*            指定参数的指针
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_LoadIspParam(char *isp_param_buff);
/*
*   Name: API_ISP_Pause
*            暂停ISP对图像的处理与输出
*
*   Parameters:
*
*            None
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Pause(FH_VOID);
/*
*   Name: API_ISP_Run
*            ISP策略处理
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)，
*           -1(图像丢失)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Run(FH_VOID);
/*
*   Name: API_ISP_Exit
*            ISP线程退出，清理状态
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Exit(FH_VOID);
/*
*   Name: API_ISP_SensorRegCb
*            拷贝sensor的回调函数到目标地址。
*
*   Parameters:
*
*       [IN] FH_UINT32 u32SensorId
*            无用
*
*       [IN]  struct isp_sensor_if* pstSensorFunc
*            类型为isp_sensor_if的结构体指针，详细成员变量请查看isp_sensor_if结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorRegCb(FH_UINT32 u32SensorId, struct isp_sensor_if* pstSensorFunc);
/*
*   Name: API_ISP_SensorUnRegCb
*            注销ISP中注册的sensor的回调函数。
*
*   Parameters:
*
*       [IN] FH_UINT32 u32SensorId
*            无用
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorUnRegCb(FH_UINT32 u32SensorId);
/*
*   Name: API_ISP_Set_HWmodule_cfg
*            配置ISP模块硬件使能
*
*   Parameters:
*
*       [IN] const FH_UINT32 u32modulecfg
*            每1bit对应一个硬件使能位0表示关闭，1表示打开，详见枚举ISP_HW_MODULE_LIST。
*
*   Return:
*            0(正确)
*   Note:
*       当需要一次开关多个模块时,使用此函数,若只需控制一个模块,则可使用API_ISP_Set_Determined_HWmodule函数实现
*/
FH_SINT32 API_ISP_Set_HWmodule_cfg(const FH_UINT32 u32modulecfg);
/*
*   Name: API_ISP_Get_HWmodule_cfg
*            获取当前ISP模块硬件状态
*
*   Parameters:
*
*       [OUT] FH_UINT32 *u32modulecfg
*            每1bit对应一个硬件使能位0表示关闭，1表示打开，详见枚举ISP_HW_MODULE_LIST。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Get_HWmodule_cfg(FH_UINT32 *u32modulecfg);
/*
*   Name: API_ISP_Set_Determined_HWmodule
*            配置ISP某个模块硬件使能
*
*   Parameters:
*
*       [IN] const HW_MODULE_CFG *pstModuleCfg
*            传入要控制的某个模块枚举变量以及开关状态，详见HW_MODULE_CFG。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       一次只能传入一个模块,否则会出错
*/
FH_SINT32 API_ISP_Set_Determined_HWmodule(const HW_MODULE_CFG *pstModuleCfg);
/*
*   Name: API_ISP_Get_Determined_HWmodule
*           获取ISP某个模块硬件使能状态
*
*   Parameters:
*
*       [INOUT] HW_MODULE_CFG *pstModuleCfg
*            传入要控制的某个模块枚举变量,获取该模块开关状态，详见HW_MODULE_CFG。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       一次只能传入一个模块,否则会出错
*/
FH_SINT32 API_ISP_Get_Determined_HWmodule(HW_MODULE_CFG *pstModuleCfg);
/*
*   Name: API_ISP_GetAlgCtrl
*            获取软件控制开关寄存器状态
*
*   Parameters:
*
*       [OUT] FH_UINT32 *u32AlgCtrl
*            每1bit对应一个软件使能位0表示关闭，1表示打开
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAlgCtrl(FH_UINT32 *u32AlgCtrl);
/*
*   Name: API_ISP_SetAlgCtrl
*            设置软件控制开关寄存器状态
*
*   Parameters:
*
*       [IN] FH_UINT32 u32AlgCtrl
*            每1bit对应一个软件使能位0表示关闭，1表示打开
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAlgCtrl(FH_UINT32 u32AlgCtrl);
/**SENSOR_CONTROL*/
/*
*   Name: API_ISP_SensorInit
*            sensor预初始化，并未配置sensor寄存器
*
*   Parameters:
*
*       [IN] FH_UINT32 devIdx
*            通信设备号
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorInit(FH_UINT32 devIdx);
/*
*   Name: API_ISP_SetSensorFmt
*            初始化sensor
*
*   Parameters:
*
*       [IN] FH_UINT32 format
*            传入的时枚举FORMAT中的枚举值，选择幅面
*
*   Return:
*            0(正确)
*        -3002(sensor初始化异常)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorFmt(FH_UINT32 format);

/*
*   Name: API_ISP_SensorKick
*            启动sensor输出，有的sensor需要用到
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorKick(FH_VOID);
/*
*   Name: API_ISP_SetSensorIntt
*            配置sensor的曝光值
*
*   Parameters:
*
*       [IN] FH_UINT32 intt
*            传入的曝光值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)。
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorIntt(FH_UINT32 intt);
/*
*   Name: API_ISP_SetSensorGain
*            配置sensor增益值
*
*   Parameters:
*
*       [IN] FH_UINT32 gain
*            增益值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorGain(FH_UINT32 gain);
/*
*   Name: API_ISP_SetSensorReg
*            配置sensor寄存器
*
*   Parameters:
*
*       [IN] FH_UINT16 addr
*            sensor寄存器地址
*
*       [IN] FH_UINT16 data
*            配置的值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorReg(FH_UINT16 addr,FH_UINT16 data);
/*
*   Name: API_ISP_GetSensorReg
*            读取sensor寄存器
*
*   Parameters:
*
*       [IN] FH_UINT16 addr
*            sensor寄存器地址
*
*       [OUT]  FH_UINT16 *data
*            读取的值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetSensorReg(FH_UINT16 addr, FH_UINT16 *data);

/*
*   Name: API_ISP_Resume
*            恢复ISP对图像处理与输出
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Resume(FH_VOID);

/** AE **/
/*
*   Name: API_ISP_SetAeCfg
*            设置ae相关参数
*
*   Parameters:
*
*       [IN] AE_CFG *pstAeDefaultCfg
*            类型为AE_CFG的结构体指针，详细成员变量请查看AE_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAeCfg(ISP_AE_CFG *pstAeDefaultCfg);
/*
*   Name: API_ISP_GetAeCfg
*            获取ae相关参数
*
*   Parameters:
*
*       [OUT] AE_CFG *pstAeDefaultCfg
*            类型为AE_CFG的结构体指针，详细成员变量请查看AE_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeCfg(ISP_AE_CFG *pstAeDefaultCfg);
/*
*   Name: API_ISP_SetAeInfo
*            设置ae曝光时间及增益参数至sensor及硬件寄存器
*
*   Parameters:
*
*       [IN] const ISP_AE_INFO *pstAeInfo
*            类型为ISP_AE_INFO的结构体指针，详细成员变量请查看ISP_AE_INFO结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAeInfo(const ISP_AE_INFO *pstAeInfo);
/*
*   Name: API_ISP_GetAeInfo
*           从sensor及硬件寄存器获取ae曝光时间及增益参数
*
*   Parameters:
*
*       [OUT] ISP_AE_INFO *pstAeInfo
*            类型为ISP_AE_INFO的结构体指针，详细成员变量请查看ISP_AE_INFO结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeInfo(ISP_AE_INFO *pstAeInfo);
/*
*   Name: API_ISP_GetAeStatus
*            获取ae状态值
*
*   Parameters:
*
*       [OUT] ISP_AE_STATUS *pstAeStatus
*            类型为ISP_AE_STATUS的结构体指针，详细成员变量请查看ISP_AE_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeStatus(ISP_AE_STATUS *pstAeStatus);
/*
*   Name: API_ISP_SetAeUserStatus
*            设置ae用户状态值
*
*   Parameters:
*
*       [IN] AE_USER_STATUS *pstAeUserStatus
*            类型为AE_USER_STATUS的结构体指针，详细成员变量请查看AE_USER_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       若用户使用自定义ae策略,则需要调用该函数配置ae相关状态值,提供给isp的其他策略使用,否则其他策略可能运行会有异常
*/
FH_SINT32 API_ISP_SetAeUserStatus(AE_USER_STATUS *pstAeUserStatus);

/** AWB **/
/*
*   Name: API_ISP_SetAwbCfg
*            设置awb相关参数
*
*   Parameters:
*
*       [IN] AWB_DEFAULT_CFG *pstAwbDefaultCfg
*            类型为AWB_DEFAULT_CFG的结构体指针，详细成员变量请查看AWB_DEFAULT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAwbCfg(ISP_AWB_CFG *pstAwbDefaultCfg);
/*
*   Name: API_ISP_SetAwbCfg
*            获取awb相关参数
*
*   Parameters:
*
*       [OUT] AWB_DEFAULT_CFG *pstAwbDefaultCfg
*            类型为AWB_DEFAULT_CFG的结构体指针，详细成员变量请查看AWB_DEFAULT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAwbCfg(ISP_AWB_CFG *pstAwbDefaultCfg);
/*
*   Name: API_ISP_GetAwbStatus
*            获取awb状态值
*
*   Parameters:
*
*       [OUT] ISP_AWB_STATUS *pstAwbStatus
*            类型为ISP_AWB_STATUS的结构体指针，详细成员变量请查看ISP_AWB_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAwbStatus(ISP_AWB_STATUS *pstAwbStatus);
/*
*   Name: API_ISP_SetAwbGain
*            设置awb增益至硬件寄存器
*
*   Parameters:
*
*       [IN] ISP_AWB_GAIN *pstAwbGain
*            类型为ISP_AWB_GAIN的结构体指针，详细成员变量请查看ISP_AWB_GAIN结构体定义。
*
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAwbGain(const ISP_AWB_GAIN *pstIspAwbGain);
/*
*   Name: API_ISP_GetAwbGain
*            从硬件寄存器获取awb增益值
*
*   Parameters:
*
*       [OUT] const ISP_AWB_GAIN *pstAwbGain
*            类型为ISP_AWB_GAIN的结构体指针，详细成员变量请查看ISP_AWB_GAIN结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)

*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAwbGain(ISP_AWB_GAIN *pstIspAwbGain);
/**CCM**/
/*
*   Name: API_ISP_SetCcmCfg
*            配置CCM。
*
*   Parameters:
*
*       [IN] CCM_TABLE *pstCcmCfg
*             类型为CCM_TABLE的结构体指针，详细成员变量请查看CCM_TABLE结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCcmCfg(CCM_TABLE *pstCcmCfg);
/*
*   Name: API_ISP_GetCcmCfg
*           获取CCM。
*
*   Parameters:
*
*       [IN] CCM_TABLE *pstCcmCfg
*             类型为CCM_TABLE的结构体指针，详细成员变量请查看CCM_TABLE结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetCcmCfg(CCM_TABLE *pstCcmCfg);
/**GAMMA*/
/*
*   Name: API_ISP_SetGammaCfg
*            配置gamma相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_GAMMA_CFG *pstGammaCfg
*            类型为ISP_GAMMA_CFG的结构体指针，详细成员变量请查看ISP_GAMMA_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetGammaCfg(ISP_GAMMA_CFG *pstGammaCfg);
/*
*   Name: API_ISP_GetGammaCfg
*            获取当前配置的gamma相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_GAMMA_CFG *pstGammaCfg
*            类型为ISP_GAMMA_CFG的结构体指针，详细成员变量请查看ISP_GAMMA_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetGammaCfg(ISP_GAMMA_CFG *pstGammaCfg);
/**BLC*/
/*
*   Name: API_ISP_SetBlcCfg
*            配置blc相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_BLC_CFG *pstBlcCfg
*            类型为ISP_BLC_CFG的结构体指针，详细成员变量请查看ISP_BLC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetBlcCfg(ISP_BLC_CFG *pstBlcCfg);
/*
*   Name: API_ISP_GetBlcCfg
*            获取当前配置的blc相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_BLC_CFG *pstBlcCfg
*            类型为ISP_BLC_CFG的结构体指针，详细成员变量请查看ISP_BLC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetBlcCfg(ISP_BLC_CFG *pstBlcCfg);
/**DPC*/
/*
*   Name: API_ISP_SetDpcCfg
*            配置DPC的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_DPC_CFG *pstDpcCfg
*            类型为ISP_DPC_CFG的结构体指针，详细成员变量请查看ISP_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetDpcCfg(ISP_DPC_CFG *pstDpcCfg);
/*
*   Name: API_ISP_GetDpcCfg
*            获取当前配置的DPC的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_DPC_CFG *pstDpcCfg
*            类型为ISP_DPC_CFG的结构体指针，详细成员变量请查看ISP_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetDpcCfg(ISP_DPC_CFG *pstDpcCfg);
/** NR3D*/
/*
*   Name: API_ISP_SetNr3dCfg
*            配置NR3D的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_NR3D_CFG *pstNr3dCfg
*            类型为ISP_NR3D_CFG的结构体指针，详细成员变量请查看ISP_NR3D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNR3DCfg(ISP_NR3D_CFG *pstNr3dCfg);
/*
*   Name: API_ISP_GetNR3DCfg
*            获取当前配置的NR3D的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_NR3D_CFG *pstNr3dCfg
*            类型为ISP_NR3D_CFG的结构体指针，详细成员变量请查看ISP_NR3D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNR3DCfg(ISP_NR3D_CFG *pstNr3dCfg);
/** NR2D*/
/*
*   Name: API_ISP_SetNR2DCfg
*            配置NR2D的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_NR2D_CFG *pstIspNr2dCfg
*            类型为ISP_NR2D_CFG的结构体指针，详细成员变量请查看ISP_NR2D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNR2DCfg(ISP_NR2D_CFG *pstIspNr2dCfg);
/*
*   Name: API_ISP_GetNR2DCfg
*            获取当前配置的NR2D的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_NR2D_CFG *pstIspNr2dCfg
*            类型为ISP_NR2D_CFG的结构体指针，详细成员变量请查看ISP_NR2D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNR2DCfg(ISP_NR2D_CFG *pstIspNr2dCfg);
/**IE*/
/*
*   Name: API_ISP_SetContrastCfg
*            配置对比度相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_IE_Y_CFG *pstYContrastCfg
*            类型为ISP_IE_Y_CFG的结构体指针，详细成员变量请查看ISP_IE_Y_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetContrastCfg(ISP_IE_Y_CFG *pstYContrastCfg);
/*
*   Name: API_ISP_GetContrastCfg
*            获取当前配置的对比度相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_IE_Y_CFG *pstYContrastCfg
*            类型为ISP_IE_Y_CFG的结构体指针，详细成员变量请查看ISP_IE_Y_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetContrastCfg(ISP_IE_Y_CFG *pstYContrastCfg);
/*
*   Name: API_ISP_SetColorCfg
*            配置对比度相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_IE_C_CFG *pstColorCfg
*            类型为ISP_IE_C_CFG的结构体指针，详细成员变量请查看ISP_IE_C_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetColorCfg(ISP_IE_C_CFG *pstColorCfg);
/*
*   Name: API_ISP_GetColorCfg
*            获取当前配置的对比度相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_IE_C_CFG *pstColorCfg
*            类型为ISP_IE_C_CFG的结构体指针，详细成员变量请查看ISP_IE_C_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetColorCfg(ISP_IE_C_CFG *pstColorCfg);
/**IE_APC*/
/*
*   Name: API_ISP_SetApcCfg
*            配置锐度相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_IE_APC_CFG *pstApcCfg
*            类型为ISP_IE_APC_CFG的结构体指针，详细成员变量请查看ISP_IE_APC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetApcCfg(ISP_IE_APC_CFG *pstApcCfg);
/*
*   Name: API_ISP_GetApcCfg
*            获取当前配置的锐度相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_IE_APC_CFG *pstApcCfg
*            类型为ISP_IE_APC_CFG的结构体指针，详细成员变量请查看ISP_IE_APC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetApcCfg(ISP_IE_APC_CFG *pstApcCfg);
/**GLOBAL_STAT**/
/*
*   Name: API_ISP_GetGlobalStat
*            获取GLOBAL统计值
*
*   Parameters:
*
*       [OUT] ISP_GLOBAL_STAT * pstGlobalStat
*            类型为ISP_GLOBAL_STAT的结构体指针，详细成员变量请查看ISP_GLOBAL_STAT结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       使用示例:
*               GLOBE_STAT glStat[64];  // 64是默认窗口配置,若改动的话通过API_ISP_GetGlobalStatCfg获取当前统计窗个数
*               API_ISP_GetGlobalStat(glStat);
*/
FH_SINT32 API_ISP_GetGlobalStat(ISP_GLOBAL_STAT * pstGlobalStat);
/*
*   Name: API_ISP_SetGlobalStatCfg
*            设置global统计配置相关参数
*
*   Parameters:
*
*       [IN] const ISP_GLOBAL_STAT_CFG *pstGlobalStatCfg
*            类型为ISP_GLOBAL_STAT_CFG的结构体指针，详细成员变量请查看ISP_GLOBAL_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetGlobalStatCfg(const ISP_GLOBAL_STAT_CFG *pstGlobalStatCfg);
/*
*   Name: API_ISP_GetGlobalStatCfg
*            获取global统计配置相关参数
*
*   Parameters:
*
*       [OUT] GLOBE_STAT_CFG *pstGlobeStatCfg
*            类型为GLOBE_STAT_CFG的结构体指针，详细成员变量请查看GLOBE_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetGlobalStatCfg(ISP_GLOBAL_STAT_CFG *pstGlobeStatCfg);
/**Debug_Interface**/
/*
*   Name: API_ISP_ReadMallocedMem
*            读取指定偏移的VMM分配的内存的值
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [OUT]  FH_UINT32 *pstData
*            存放读取到数据的地址。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_ReadMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
/*
*   Name: API_ISP_WriteMallocedMem
*            写指定偏移的VMM分配的内存的值
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [IN]  FH_UINT32 *pstData
*            目标值，将该值写入目标地址。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_WriteMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
/*
*   Name: API_ISP_ImportMallocedMem
*            导入指定大小的数据到指定偏移的VMM分配的内存
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [IN]  FH_UINT32 *pstSrc
*            导入数据的起始地址
*
*       [IN]  FH_UINT32 size
*            需要导入的数据大小
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_ImportMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstSrc, FH_UINT32 size);
/*
*   Name: API_ISP_ExportMallocedMem
*            从指定偏移的VMM分配的内存导出指定大小的数据
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [OUT]  FH_UINT32 *pstDst
*            存放导出数据的起始地址
*
*       [IN]  FH_UINT32 size
*            需要导出的数据大小
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_ExportMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstDst, FH_UINT32 size);
/*
*   Name: API_ISP_GetVIState
*            获取当前ISP的一些运行状态信息
*
*   Parameters:
*
*       [OUT] ISP_VI_STAT_S *pstStat
*            类型为ISP_VI_STAT_S的结构体指针，详细成员变量请查看ISP_VI_STAT_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetVIState(ISP_VI_STAT_S *pstStat);
/*
*   Name: API_ISP_SetSensorFrameRate
*            配置SENSOR的垂直消隐至指定倍率
*
*   Parameters:
*
*       [IN] int m
*            垂直消隐的倍率,U.8,也即0x100表示一倍
*
*   Return:
*            0(正确)
*   Note:
*       当ae策略关闭时可使用该函数,策略打开时的帧消隐由策略参数控制,且只支持大于1倍的降帧
*/
FH_SINT32 API_ISP_SetSensorFrameRate(int m);
/*
*   Name: API_ISP_Dump_Param
*            拷贝所有DRV寄存器值到指定地址
*
*   Parameters:
*
*       [OUT] FH_UINT32 *addr
*            存放DRV参数的地址
*
*       [IN] FH_UINT32 *size
*            拷贝数据的大小
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Dump_Param(FH_UINT32 *addr,FH_UINT32 *size);
/**OTHERS*/
/*
*   Name: *FH_ISP_Version
*            获取、打印ISP版本号。
*
*   Parameters:
*
*       [IN] FH_UINT32 print_enable
*            打印ISP库版本号(1)，不打印ISP库版本号(0)
*
*   Return:
*            ISP库版本号的字符串。
*   Note:
*       无
*/
FH_CHAR *FH_ISP_Version(FH_UINT32 print_enable);
/*
*   Name: *FH_ISPCORE_Version
*            获取、打印ISP版本号。
*
*   Parameters:
*
*       [IN] FH_UINT32 print_enable
*            打印ISP_CORE库的版本号(1)，不打印ISP_CORE库的版本号(0)
*
*   Return:
*            ISP_CORE库的版本号的字符串。
*   Note:
*       无
*/
FH_CHAR *FH_ISPCORE_Version(FH_UINT32 print_enable);
/*
*   Name: API_ISP_OutBufCfg
*            配置ISP写DDR输出时候的YC缓存配置
*
*   Parameters:
*
*       [OUT] ISP_DITHER_BUF_CFG_S* pstOutBufCfg
*            类型为ISP_DITHER_BUF_CFG_S的结构体指针，详细成员变量请查看ISP_DITHER_BUF_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_OutBufCfg(ISP_DITHER_BUF_CFG_S* pstOutBufCfg);
/*
*   Name: API_ISP_SetDsCfg
*            ISP的下采样模块配置
*
*   Parameters:
*
*       [OUT] ISP_DS_CFG_S* pstDsCfg
*            类型为ISP_DS_CFG_S的结构体指针，详细成员变量请查看ISP_DS_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetDsCfg(ISP_DS_CFG_S* pstDsCfg);
/*
*   Name: API_ISP_DsBufCfg
*            ISP下采样模块的缓存配置
*
*   Parameters:
*
*       [OUT] ISP_DS_BUF_CFG_S* pstDsBufCfg
*            类型为ISP_DS_BUF_CFG_S的结构体指针，详细成员变量请查看ISP_DS_BUF_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_UINT32 API_ISP_DsBufCfg(ISP_DS_BUF_CFG_S* pstDsBufCfg);
/*
*   Name: API_ISP_SetOffModeCfg
*            配置离线模式的参数
*
*   Parameters:
*
*       [OUT] ISP_OFF_MODE_CFG_S* pstOffModeCfg
*            类型为ISP_OFF_MODE_CFG_S的结构体指针，详细成员变量请查看ISP_OFF_MODE_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetOffModeCfg(ISP_OFF_MODE_CFG_S* pstOffModeCfg);
/*
*   Name: API_ISP_OffModeKick
*            配置离线模式读取的DDR地址，每次可以更新地址，也可以不更新
*
*   Parameters:
*
*       [OUT] ISP_OFF_ADDR_CFG_S* pstCfg
*            类型为ISP_OFF_ADDR_CFG_S的结构体指针，详细成员变量请查看ISP_OFF_ADDR_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_OffModeKick(ISP_OFF_ADDR_CFG_S* pstCfg);
/*
*   Name: API_ISP_Deinit
*            ISP注销接口，停止ISP
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Deinit(FH_VOID);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_ISP_API_H_*/
