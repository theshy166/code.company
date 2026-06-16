#ifndef _ISP_COMMON_H_
#define _ISP_COMMON_H_

#include "types/type_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#pragma pack(4)

#define MALLOCED_MEM_BASE_ISP (0)

#define MALLOCED_MEM_DRV (3)

#define GAIN_NODES 8


typedef int (*ispInitCfgCallback)(void);
typedef int (*ispIntCallback)(void);
typedef int (*ispCallBack)(void);
void ispTrace_FunctionName(const char* func);

/**|SYSTEM CONTROL|**/
typedef enum _ISP_BAYER_TYPE_S_ {
    BAYER_RGGB = 0x0, // 色彩模式RGGB
    BAYER_GRBG = 0x1, // 色彩模式GRBG
    BAYER_BGGR = 0x2, // 色彩模式BGGR
    BAYER_GBRG = 0x3, // 色彩模式GBRG
    ISP_BAYER_TYPE_DUMMY=0xffffffff,
} ISP_BAYER_TYPE;

typedef enum _TAB_COLOR_CODE {
    CC_R  = 0,
    CC_GR = 1,
    CC_B  = 2,
    CC_GB = 3,
    TAB_COLOR_CODE_DUMMY =0xffffffff,
} TAB_COLOR_CODE;

typedef enum _CURVE_TYPE_ {
    CURVE_BUILTIN     = 0x1, // 选用内定曲线
    CURVE_USER_DEFINE = 0x2, // 选用用户自定义曲线
    CURVE_TYPE_DUMMY  =0xffffffff,
} CURVE_TYPE;

typedef struct _HW_MODULE_CFG_S{
    FH_UINT32 moduleCfg;  // 需要配置的模块 | [ISP_HW_MODULE_LIST]
    FH_BOOL enable;  // 使能开关,0表示关闭,1表示打开 | [0~1]
} HW_MODULE_CFG;

typedef struct _ISP_VERSION_S_
{
    FH_UINT32 u32SdkVer;        // sdk版本号 | [0~0xffffffff]
    FH_UINT32 FH_UINT32ChipVer; // 芯片版本号 | [0~0xffffffff]
    FH_UINT8  u08SdkSubVer;     // sdk sub版本号 | [0~0xff]
    FH_UINT8  u08BuildTime[21]; // sdk构建时间 | [0~0xff]
} ISP_VERSION;

typedef struct _ISP_VI_ATTR_
{
    FH_UINT16      u16WndHeight;   ///<sensor幅面高度 | [0~0xffff]
    FH_UINT16      u16WndWidth;    /// <sensor幅面宽度 | [0~0xffff]
    FH_UINT16      u16InputHeight; ///<sensor输入图像高度 | [0~0x1fff]
    FH_UINT16      u16InputWidth;  ///<sensor输入图像宽度 | [0~0x1fff]
    FH_UINT16      u16OffsetX;     ///<裁剪水平偏移 | [0~0xfff]
    FH_UINT16      u16OffsetY;     ///<裁剪垂直偏移 | [0~0xfff]
    FH_UINT16      u16PicHeight;   ///<处理的图像高度 | [0~0x1fff]
    FH_UINT16      u16PicWidth;    ///<处理的图像宽度 | [0~0x1fff]
    FH_UINT16      u16FrameRate;   ///<帧率 | [0~0xffff]
    ISP_BAYER_TYPE enBayerType;    ///bayer数据格式 | [0~0x3]
} ISP_VI_ATTR_S;

typedef struct _ISP_SIZE_
{
    FH_UINT16 width;
    FH_UINT16 height;
} ISP_SIZE;

typedef struct
{
	FH_UINT32           base;        // 物理地址 | [ ]
	FH_VOID *           vbase;       // 虚拟地址 | [ ]
	FH_UINT32           size;        // 内存大小 | [ ]
} ISP_MEM_INFO;

typedef struct
{
    ISP_MEM_INFO yluma;
    ISP_MEM_INFO chroma;
    ISP_SIZE voSize;
} ISP_VO_FRAME;

/**|Debug_Interface|**/
typedef struct _ISP_VI_STAT_S
{
    FH_UINT32 u32IPBIntCnt;   // IPB中断计数 | [0~0xffffffff]
    FH_UINT32 u32IPFIntCnt;   // IPF中断计数 | [0~0xffffffff]
    FH_UINT32 u32FrmRate;     // 当前帧率 | [0~0xffffffff]
    FH_UINT32 u32PicWidth;    // 当前处理图像宽度 | [0~0xffffffff]
    FH_UINT32 u32PicHeight;   // 当前处理图像高度 | [0~0xffffffff]
    FH_UINT32 u32IpfOverFlow; // ISP Vi溢出中断计数 | [0~0xffffffff]
} ISP_VI_STAT_S;

/**|MIRROR|**/
typedef struct mirror_cfg_s
{
    FH_BOOL             bEN;  // 镜像使能开关 | [0~1]
    ISP_BAYER_TYPE      normal_bayer;  // 无效成员变量 | [0~3]
    ISP_BAYER_TYPE      mirror_bayer;  //　无效成员变量 | [0~3]
}MIRROR_CFG_S;

/**|AE|**/
typedef struct _ISP_AE_CFG_S_ {
	FH_BOOL bAgcEn;// Auto Gain Control 自动增益控制 | [0x0-0x1]
	FH_BOOL bAscEn;// Auto Shutter Control，自动快门控制 | [0x0-0x1]
	FH_UINT8 u08MeteringMode;// 测光模式：	0: global 全局曝光	1: center 中心测光	2: back light comp背光补偿	3: front light comp 前光补偿	4:center light comp 中心亮度测光	5: user define 用户定义 | [0x0-0xff]
	FH_UINT8 u08AeSpeed;// 曝光速度，0: fastest, 15: slowest | [0x0-0xff]
	FH_UINT8 u08MeteringParam;//  | [0x0-0x3f]
	FH_UINT16 u16LumaRef;// the reference luminance for auto exposure | [0x0-0xfff]
	FH_UINT8 u08StabCnt;// 稳定时间，按帧数计，超过稳定时间后，曝光才进入下一种状态。 | [0x0-0xf]
	FH_UINT8 u08StabZone0;// 稳定区域0，目标亮度偏差小于此阈值，曝光行为停止。 | [0x0-0xff]
	FH_UINT8 u08StabZone1;// 稳定区域1，目标亮度偏差小于此阈值，曝光调整启动。 | [0x0-0xff]
	FH_UINT16 u16IspGainMax;// the maximum value of ISP gain U10.6 | [0x0-0xffff]
	FH_UINT16 u16SensorGainMax;// the maximum value of Sensor gain U10.6 | [0x0-0xffff]
	FH_UINT16 u16InttMax;// the max sensor integration time in line | [0x0-0xffff]
	FH_UINT8 u08SenserUpMax;// 慢快门倍数的最大值 | [0x0-0xff]
	FH_UINT8 u08SensorGainAccur;// Sensor的增益精度 | [0x0-0xf]
	FH_UINT8 u08GetOpMode;//  | [0x0-0xf]
	FH_UINT8 u08AdjustMode;//  | [0x0-0xf]
} ISP_AE_CFG;
typedef struct _ISP_AE_INFO_S_
{
    FH_UINT32   u32Intt;          // sensor曝光行数 | [0~0xffffffff]
    FH_UINT32   u32IspGain;       // isp增益值U4.8, 0x100表示一倍 | [0~0xfff]
    FH_UINT32   u32SensorGain;    // sensor增益值U.6, 0x40表示一倍 |[0~0xffffffff]
    FH_UINT32   u32TotalGain;     // 总增益U.6 , 0x40表示一倍 | [0~0xffffffff]
} ISP_AE_INFO;

typedef struct _ISP_AE_STATUS_S_
{
    FH_UINT8 u08TarLuma;           // 目标亮度 | [0x0-0xff]
    FH_UINT8 u08CurrLuma;          // 当前亮度 | [0x0-0xff]
    FH_UINT32 u32CurrIntt;         // 当前曝光时间 | [0x0-0xfffff]
    FH_UINT16 u16CurrDgain;        // 当前isp增益，U.6 | [0x0-0xffff]
    FH_UINT16 u16CurrAgain;        // 当前sensor增益，U.6 | [0x0-0xffff]
    FH_UINT32 u32CurrTotalGain;    // 当前总增益，U.6 | [0x0-0xffffffff]
    FH_BOOL bOpStatus;             // Ae当前运行状态    0：停止 1：运行 | [0x0-0x1]
    FH_UINT16 u16IspInttMax;       // Isp配置的inttmax，以行为单位 | [0x0-0xffff]
    FH_UINT16 u16IspAgainMax;      // Isp配置的againmax，U.6 | [0x0-0xffff]
    FH_UINT16 u16IspDgainMax;      // Isp配置的dgainmax，U.6 | [0x0-0xffff]
} ISP_AE_STATUS;

typedef struct _AE_USER_STATUS_S_
{
	FH_UINT16 u16CurrLuma;// 当前环境测光值 | [0x0-0xffff]
	FH_UINT16 u16CurrIntt;// 当前sensor曝光时间 | [0x0-0xffff]
	FH_UINT32 u32CurrGainTotal;// 当前系统总增益（U14.6） | [0x0-0xfffff]
} AE_USER_STATUS;

/**|AWB|**/
typedef struct _ISP_AWB_GAIN_S_ {
	FH_UINT16 u16MRgain;// 手动R通道增益，U.9 | [0x0-0xfff]
	FH_UINT16 u16MBgain;// 手动B通道增益，U.9 | [0x0-0xfff]
	FH_UINT16 u16MGgain;// 手动G通道增益，U.9 | [0x0-0xfff]
} ISP_AWB_GAIN;

typedef struct _AWB_MANUAL_CFG_S_
{
    ISP_AWB_GAIN stAwbGain;  // wb手动增益值 | [ISP_AWB_GAIN]
} AWB_MANUAL_CFG;
typedef struct _STAT_WHITE_POINT_S_
{
    FH_UINT16 u16Coordinate_w;  // awb白框横坐标U3.7 | [0-0x3ff]
    FH_UINT16 u16Coordinate_h;  // awb白框纵坐标U3.7 | [0-0x3ff]
} STAT_WHITE_POINT;
typedef struct _ISP_AWB_CFG_S_ {
	FH_UINT8 u08Mode;// 0: AWB_GWB	1:ADV_GWB | [0x0-0xf]
	FH_BOOL bMwbEn;// 手动wb使能，当为1时配置手动awb值，当为0时根据统计实时调整 | [0x0-0x1]
	FH_BOOL bCompEn;// 增益补偿使能 | [0x0-0x1]
	FH_UINT8 u08AwbThl;// 白平衡统计门限下限 | [0x0-0xff]
	FH_UINT8 u08AwbThh;// 白平衡统计门限上限 | [0x0-0xff]
    STAT_WHITE_POINT stPoint[6];      //  awb白框点,A，B，C，D，E，F六个点，Ax=Bx By=Cy Dy=Ey Ex=Fx | [STAT_WHITE_POINT]
	FH_UINT8 u08StabCnt;// 稳定时间，按帧数计，超过稳定时间后，白平衡才进入下一种状态。 | [0x0-0xf]
	FH_UINT8 u08StabZone0;// 稳定区域0，颜色偏差小于此阈值，白平衡调整行为停止。 | [0x0-0xff]
	FH_UINT8 u08StabZone1;// 稳定区域1，颜色偏差小于此阈值，白平衡调整启动。 | [0x0-0xff]
    AWB_MANUAL_CFG stAwbManualCfg;  // awb手动增益配置 | [AWB_MANUAL_CFG]
	FH_UINT8 u08REnhance;// R通道增益补偿，U.6 | [0x0-0xff]
	FH_UINT8 u08GEnhance;// g通道增益补偿，U.6 | [0x0-0xff]
	FH_UINT8 u08BEnhance;// b通道增益补偿，U.6 | [0x0-0xff]
} ISP_AWB_CFG;

typedef struct _ISP_AWB_STATUS_S_
{
	FH_UINT16 u16Rgain;// 当前白平衡R通道增益（U4.9） | [0x0-0xffff]
	FH_UINT16 u16Bgain;// 当前白平衡B通道增益（U4.9） | [0x0-0xffff]
    FH_UINT16 u16CurrBOverG;// 当前B/G(水平坐标，U4.12) | [0x0-0xffff]
	FH_UINT16 u16CurrROverG;// 当前R/G(水平坐标，U4.12) | [0x0-0xffff]
	FH_UINT8 u08Pos;// 当前色温位置 | [0x0-0xff]
} ISP_AWB_STATUS;

/**|CCM|**/
typedef struct _CCM_TABLE_S_
{
    FH_SINT16 s16CcmTable[2][12];  // ccm矩阵表 | [-4096~0xfff]
} CCM_TABLE;

/**|GAMMA|**/
typedef struct _ISP_GAMMA_CFG_S_ {
	FH_UINT8 u08Curve;// 0: gamma 2.4	1: gamma 2.2	2: gamma 2.2 s1.2	3: gamma 1.8	4: gamma 1.4	5: gamma 1.0	6~14 reserved	15: user define | [0x0-0xf]
	FH_UINT8 u08Mode;// always equal to 0xF | [0x0-0xf]
	FH_UINT16 u16Gamma[40];// GAMMA1表写入数据 | [0x0-0x3ff]
} ISP_GAMMA_CFG;
/**|BLC|**/
typedef struct _ISP_BLC_CFG_S_ {
	FH_BOOL bGainMappingEn;// BLC控制模式	1:Gain mapping	其他: Manual mode | [0x0-0x1]
	FH_UINT16 u16BlcLevel;// 手动黑电平 | [0x0-0x1fff]
	FH_UINT16 u16BlcMap[GAIN_NODES];// gain=1时，BLC | [0x0-0xffff]
} ISP_BLC_CFG;
/**|DPC|**/
typedef struct _ISP_DPC_CFG_S_ {
	FH_BOOL bMode2;// 去坏点强弱模式 | [0x0-0x1]
	FH_BOOL bCtrlMode;// 0: Manual	1: Auto | [0x0-0x1]
	FH_BOOL bWEn;// 去白点使能 | [0x0-0x1]
	FH_BOOL bBEn;// 去黑点使能 | [0x0-0x1]
	FH_UINT8 u08WS;// 白点比例门限 | [0x0-0x3]
	FH_UINT8 u08BS;// 黑点比例门限 | [0x0-0x3]
	FH_BOOL bSdpcEn;// 静态坏点使能 | [0x0-0x1]
	FH_UINT8 u08WDc;// 白点门限DC值 | [0x0-0xff]
	FH_UINT8 u08BDc;// 黑点门限DC值 | [0x0-0xff]
} ISP_DPC_CFG;

/**|NR3D|**/
typedef struct _ISP_NR3D_CFG_S_ {
	FH_UINT8 u08Nr3dMode;// NR3D control mode	0: manual	1: gain mapping | [0x0-0x3]
	FH_UINT8 u08Nr3dStr;// 3D降噪强度  8:正常  0:最弱  15:最强 | [0x0-0xf]
	FH_UINT16 u16K1;// the slope of “slope K”, U4.8 (see Noise Reduction) | [0x0-0xfff]
	FH_UINT16 u16K2;// the offset of “slope K”, U4.8 | [0x0-0xfff]
	FH_UINT16 u16O1;// the slope of “offset O”, U8.8 (see Noise Reduction) | [0x0-0xffff]
	FH_UINT16 u16O2;// the offset of “offset O”, U12 | [0x0-0xfff]
} ISP_NR3D_CFG;


/**|NR2D|**/
typedef struct _ISP_NR2D_CFG_S_ {
	FH_UINT8 u08Nr2dMode;// NR2D control mode	0: manual	1: gain mapping | [0x0-0x3]
	FH_UINT8 u08Nr2dS;// 2D de-noise strength U4 | [0x0-0xf]
	FH_UINT16 u16K1;// the slope of “slope K”, U4.8 (see Noise Reduction) | [0x0-0xfff]
	FH_UINT16 u16K2;// the offset of “slope K”, U4.8 | [0x0-0xfff]
	FH_UINT16 u16O1;// the slope of “offset O”, U8.8 (see Noise Reduction) | [0x0-0xffff]
	FH_UINT16 u16O2;// the offset of “offset O”, U12 | [0x0-0xfff]
} ISP_NR2D_CFG;

/**|IE|**/
typedef struct _ISP_IE_Y_CFG_S_ {
	FH_UINT8 u08CtrMode;// 对比度控制模式	其他: Manual mode	1:Gain mapping | [0x0-0x3]
	FH_UINT8 u08IeCtr;// contrast U4.4 | [0x0-0xff]
	FH_SINT8 u08IeBrt;// brightness S8 | [0x0-0xff]
	FH_UINT8 u08IeMid;// mid luma for contrast adjust | [0x0-0xff]
	FH_UINT8 u08CtrMap[GAIN_NODES];// gain=1时，对比度U4.4 | [0x0-0xff]
	FH_SINT8 u08BrtMap[GAIN_NODES];// gain=1时，亮度S8 | [0x0-0xff]
} ISP_IE_Y_CFG;

typedef struct _ISP_IE_C_CFG_S_ {
	FH_UINT8 u08CsMode;// 饱和度控制模式	其他: Manual mode	1:Gain mapping | [0x0-0x3]
	FH_UINT8 u08RedSurp;// red suppression gain U2.6 | [0x0-0xff]
	FH_UINT8 u08BlueSurp;// blue suppression gain U2.6 | [0x0-0xff]
	FH_UINT8 u08SatGain;// saturate suppression gain U0.4 | [0x0-0xff]
	FH_UINT8 u08GainCb;// CB增益U2.6 | [0x0-0xff]
	FH_UINT8 u08GainCr;// CR增益U2.6 | [0x0-0xff]
	FH_UINT8 u08SatMap[GAIN_NODES];// gain=1时，饱和度U0.4 | [0x0-0xf]
} ISP_IE_C_CFG;

/**|IE_APC|**/
typedef struct _ISP_IE_APC_CFG_S_ {
	FH_UINT8 u08SpEdgeMode;// 边界锐化控制模式	其他: Manual mode	1:Gain mapping | [0x0-0x3]
	FH_UINT8 u08SpDetailMode;// 细节锐化控制模式	其他: Manual mode	1:Gain mapping | [0x0-0x3]
	FH_UINT8 u08SpGainN;// sharpness gain negative (U4.4) | [0x0-0xff]
	FH_UINT8 u08SpGainP;// sharpness gain positive (U4.4) | [0x0-0xff]
	FH_UINT8 u08EdgeLv;// edge enhancement level: 1 weakest ~15 strongest | [0x0-0xf]
	FH_UINT8 u08DetailLv;// detail level: 1 weakest ~ 7 strongest (0 for blur) | [0x0-0xf]
	FH_UINT8 u08SpDetailMap[GAIN_NODES];// gain=1时，细节增强等级 | [0x0-0xf]
	FH_UINT8 u08SpEdgeMap[GAIN_NODES];// gain=1时，边界增强等级 | [0x0-0xf]
} ISP_IE_APC_CFG;

/**|GLOBAL_STAT|**/
typedef struct _ISP_STAT_WIN_S_
{
    FH_UINT16 winHOffset;  // 统计窗口水平偏移 | [0~PicW]
    FH_UINT16 winVOffset;  // 统计窗口垂直偏移 | [0~PicH]
    FH_UINT16 winHSize;    // 统计窗口宽度 | [0~(PicW-winHOffset)]
    FH_UINT16 winVSize;    // 统计窗口高度 | [0~(PicH-winVOffset)]
} ISP_STAT_WIN;

typedef struct _ISP_GLOBAL_STAT_CFG_S_
{
    ISP_STAT_WIN stStatWin;  // 统计窗口配置 | [ISP_STAT_WIN]
} ISP_GLOBAL_STAT_CFG;

typedef struct _ISP_GLOBAL_STAT_S
{
    struct _Block_gstat
    {
        FH_UINT32 sum;  // 全局统计sum值 | [0~0xffffffff]
        FH_UINT32 cnt;  // 全局统计cnt值 | [0~0xffff]
    } r, gr, gb, b;
} ISP_GLOBAL_STAT;
/**|end|*/
/******************************************************************/
/******************end of isp init cfg structure*******************/
/******************************************************************/
/******************************************************************/
typedef struct _WB_INIT_CFG_S
{
    FH_UINT16 wbGain[3];  // 白平衡三通道增益值rgain, ggain, bgain; | [0~0x1fff]
} WB_INIT_CFG;

/******************isp sensor common_if interface******************/
typedef struct _MAP_INTT_GAIN_CFG_S
{
    FH_SINT32 currFormat;  //当前的sensor format | [0~0xffffffff]
    FH_SINT32 desFormat;   //目标的sensor format | [0~0xffffffff]
    FH_UINT32 currIntt;    //当前的曝光时间，单位为行 | [0~0xffffffff]
    FH_UINT32 currTotalGain; //当前的曝光增益，u.6精度 | [0~0xffffffff]
    FH_UINT32 desIntt;     //计算返回的曝光时间，单位为行 | [0~0xffffffff]
    FH_UINT32 desTotalGain; //计算返回的曝光增益，u.6精度 | [0~0xffffffff]
} MAP_INTT_GAIN_CFG;

typedef union
{
    MAP_INTT_GAIN_CFG mapInttGainCfg;
    unsigned int dw[128];
} ISP_SENSOR_COMMON_CMD_DATA0;
typedef union
{
    unsigned int dw[128];
} ISP_SENSOR_COMMON_CMD_DATA1;
/*******************************************************************/

/**|RUBBISH|**/
typedef enum _SNS_CLK_S_ {
    SNS_CLK_24_POINT_0   = 0x1,
    SNS_CLK_27_POINT_0   = 0x2,
    SNS_CLK_37_POINT_125 = 0x3,
    SNS_CLK_DUMMY =0xffffffff,
} SNS_CLK;

typedef enum _SNS_DATA_BIT_S_ {
    LINER_DATA_8_BITS  = 0x1,
    LINER_DATA_12_BITS = 0x2,
    LINER_DATA_14_BITS = 0x3,
    WDR_DATA_16_BITS   = 0x4,
    SNS_DATA_BITS_DUMMY =0xffffffff,
} SNS_DATA_BITS;

typedef enum _SIGNAL_POLARITY_S_ {
    ACTIVE_HIGH = 0x0,
    ACTIVE_LOW = 0x1,
    SIGNAL_POLARITY_DUMMY =0xffffffff,
} SIGNAL_POLARITY;
typedef struct _ISP_VI_HW_ATTR_S_
{
    SNS_CLK         eSnsClock;         // 配置的cis时钟 | [SNS_CLK]
    SNS_DATA_BITS   eSnsDataBits;      // 配置的sensor数据位宽 | [SNS_DATA_BITS]
    SIGNAL_POLARITY eHorizontalSignal; // 时钟水平极性 | [0~1]
    SIGNAL_POLARITY eVerticalSignal;   // 时钟垂直极性 | [0~1]
    FH_BOOL         u08Mode16;         //  | [ ]
    FH_UINT32       u32DataMaskHigh;   //  | [ ]
    FH_UINT32       u32DataMaskLow;    //  | [ ]
} ISP_VI_HW_ATTR;

typedef struct _ISP_ALGORITHM_S_
{
    FH_UINT8 u08Name[16];    //  | [ ]
    FH_UINT8 u08AlgorithmId; //  | [ ]
    FH_VOID (*run)(FH_VOID); //  | [ ]
} ISP_ALGORITHM;

typedef struct _ISP_DEFAULT_PARAM_
{
    ISP_BLC_CFG       stBlcCfg;     //　 | [ ]
    ISP_GAMMA_CFG      stGamma;      //  | [ ]
    ISP_IE_C_CFG       stSaturation; //  | [ ]
    ISP_IE_APC_CFG     stApc;        //  | [ ]
    ISP_IE_Y_CFG       stYContrast;   //  | [ ]
    ISP_NR3D_CFG       stNr3d;       //  | [ ]
    ISP_NR2D_CFG       stNr2d;       //  | [ ]
    ISP_DPC_CFG        stDpc;        //  | [ ]
} ISP_DEFAULT_PARAM;
typedef enum
{
    ISP_START_MODE_COLORBAR = (1 << 0),  // ISP是否用彩条模式输出
    ISP_START_MODE_BYPASS   = (1 << 2),  // ISPVI是否关闭
    ISP_START_MODE_BUTT,
}ISP_START_MODE_E;

typedef enum
{
    ISP_DS_OUT_TO_DITHER = 0,
    ISP_DS_OUT_10TO8     = 1,
}ISP_DS_OUT_SEL_D;

typedef enum
{
    ISP_HW_PINGPONG_16_16 = 0,  // 乒乓缓存16行+16行
    ISP_HW_PINGPONG_32_32 = 1,  // 乒乓缓存32行+32行
}ISP_HW_PING_PONG_MODE_E;

typedef struct
{
    FH_UINT16      u16WndWidth;    // sensor带消隐的幅面宽度 | [0~0xffff]
    FH_UINT16      u16WndHeight;   // sensor带消隐的幅面高度 | [0~0xffff]
    FH_UINT16      u16InputWidth;  // sensor实际输入图像宽度 | [0~0x1fff]
    FH_UINT16      u16InputHeight; // sensor实际输入图像高度 | [0~0x1fff]
    FH_UINT16      u16OffsetX;     // 裁剪水平偏移 | [0~0xfff]
    FH_UINT16      u16OffsetY;     // 裁剪垂直偏移 | [0~0xfff]
    FH_UINT16      u16PicWidth;    // ISP处理的图像宽度 | [0~0x1fff]
    FH_UINT16      u16PicHeight;   // ISP处理的图像高度 | [0~0x1fff]
    ISP_BAYER_TYPE enBayerType;    // bayer数据格式 | [0~0x3]
} ISP_PIC_IN_S;

typedef struct
{
    FH_UINT16 u16OutW;  // 配置输出的宽度
    FH_UINT16 u16OutH;  // 配置输出的高度
}ISP_PIC_OUT_S;

typedef struct
{
    FH_BOOL   bEn;          // ISP输出使能

    FH_UINT32 u32Yaddr;     // ISP输出的Y的地址
    FH_UINT32 u32Uvaddr;    // ISP输出的C的地址

    FH_BOOL   bHwSwitchEn;  // 是否开启硬件乒乓
    FH_UINT32 u32Yaddr1;    // ISP输出的Y1的地址，仅开启硬件乒乓时有效
    FH_UINT32 u32Uvaddr1;   // ISP输出的C1的地址，仅开启硬件乒乓时有效
    ISP_HW_PING_PONG_MODE_E enPingPongMode;  // 硬件乒乓的模式选择
}ISP_DITHER_BUF_CFG_S;

typedef struct
{
    FH_UINT8 u8DsHEn1;  // 一级水平下采样使能
    FH_UINT8 u8DsVEn1;  // 一级垂直下采样使能
    FH_UINT8 u8DsHEn2;  // 二级水平下采样使能
    FH_UINT8 u8DsVEn2;  // 二级水平下采样使能
    FH_UINT8 u8OutEn;
}ISP_DS_EN_S;

typedef struct
{
    FH_UINT32 u32Yaddr;     // ISP输出的Y的地址
    FH_UINT32 u32Uvaddr;    // ISP输出的C的地址，开启硬件乒乓时无效

    FH_BOOL   bHwSwitchEn;  // 是否开启硬件乒乓
    FH_UINT32 u32SwitchLine; // 间隔多少行切换地址
    FH_UINT32 u32Yaddr1;    // ISP输出的Y1的地址，仅开启硬件乒乓时有效
}ISP_DS_BUF_CFG_S;

typedef struct
{
    ISP_DS_EN_S stChroma;   // 色度相关的配置
    ISP_DS_EN_S stLuma;     // 亮度两棺的配置
    FH_UINT8 u8Chroma10To8En;   // 色度输出8bit使能
    ISP_DS_OUT_SEL_D enDsSel;   // DS模块输出配置
}ISP_DS_CFG_S;

typedef struct
{
    FH_UINT16 u16Width;     // 图像的宽度 | [0~0xffff]
    FH_UINT16 u16Height;     // 图像的高度 | [0~0xffff]
}ISP_SIZE_S;

typedef struct
{
    ISP_SIZE_S         stSize;      // 输入幅面大小 |[ISP_SIZE_S]
    FH_UINT32          u32Addr;     // 压缩数据存放的地址，不填也行，获取回来的地址就是随机值 | [0~0xffffffff]
    FH_UINT8           u8SourceBpp; // 压缩前的单个像素点的位宽 | [0~16]
    FH_UINT8           u8TargetBpp; // 压缩后的单个像素点的位宽 | [0~16]
    FH_UINT8           u8RgbMode;   // rgb模式 | [0~2]
    FH_UINT8           u8BayerPattern;  // 当前的bayer模式 | [ISP_BAYER_TYPE]
    FH_UINT8           u8Bypass;        // EC模块是否开启压缩 | [0~1]
    FH_UINT8           u8CirEn;         // EC模块是否开启循环缓冲 | [0~1]
    FH_UINT16          u32CirDepth;     // EC模块的循环缓冲深度 | [0~1]
}ISP_ECDC_BASE_CFG_S;

typedef struct
{
    FH_UINT32    u32EcParam[16]; // 存放的是PPS0~PPS15的压缩参数的配置
    FH_UINT32    u32DcParam[13]; // 存放的是PPS0~PPS12的解压缩参数的配置
}ISP_ECDC_PARAM_S;

typedef struct
{
    FH_BOOL    bEn;
    ISP_SIZE_S stBlank;
}ISP_OFF_MODE_CFG_S;

typedef struct
{
    FH_BOOL    bKick;
    FH_UINT32  u32BaseAddr;
}ISP_OFF_ADDR_CFG_S;
#pragma pack()
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_ISP_COMMON_H_*/
