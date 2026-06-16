#ifndef __FH_VENC_MPIPARA_NEW_H__
#define __FH_VENC_MPIPARA_NEW_H__
/**|VENC|**/
#include "types/type_def.h"
#include "fh_common.h"
#include "fh_bgm_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
#pragma pack(4)

#define MAX_NALU_CNT    (20)
#define MAX_CUSTOM_GOP  (50)
#define MAX_REF_FRM_CNT (2)
#define SCENARIO_NUM	(4)
#define RATE_NUM		(4)
#define CHN_NUM			(3) // MAX switch channel num




typedef enum
{
	FH_JPEG        = (1 << 0),//!< JPEG编码
	FH_MJPEG       = (1 << 1),//!< MJPEG编码
	FH_NORMAL_H264 = (1 << 2),//!< H264编码
	FH_SMART_H264  = (1 << 3),//!< 智能H264编码
	FH_NORMAL_H265 = (1 << 4),//!< H265编码
	FH_SMART_H265  = (1 << 5),//!< 智能H265编码
	FH_VENC_TYPE_DUMMY= 0xffffffff,
}FH_VENC_TYPE;

typedef enum
{
	FH_RC_MJPEG_FIXQP = 0, //!< MJPEG固定QP
	FH_RC_MJPEG_CBR   = 1, //!< MJPEG定码率
	FH_RC_MJPEG_VBR   = 2, //!< MJPEG变码率
	FH_RC_H264_VBR    = 3, //!< H264变码率
	FH_RC_H264_CBR    = 4, //!< H264定码率
	FH_RC_H264_FIXQP  = 5, //!< H264固定QP
	FH_RC_H264_AVBR   = 6, //!< H264自适应变码率
	FH_RC_H265_VBR    = 7, //!< H265变码率
	FH_RC_H265_CBR    = 8, //!< H265定码率
	FH_RC_H265_FIXQP  = 9, //!< H265固定QP
	FH_RC_H265_AVBR   = 10,//!< H265自适应变码率
	FH_RC_H264_CVBR   = 11,//!< H264 CVBR
	FH_RC_H265_CVBR   = 12,//!< H265 CVBR
	FH_RC_H264_QVBR   = 13,//!< H264 QVBR
	FH_RC_H265_QVBR   = 14,//!< H265 QVBR
	FH_RC_VENC_MAX        ,
	FH_VENC_RC_MODE_DUMMY= 0xffffffff,
}FH_VENC_RC_MODE;

typedef enum
{
	FH_STREAM_JPEG  = (1 << 0),//!< JPEG码流
	FH_STREAM_MJPEG = (1 << 1),//!< MJPEG码流
	FH_STREAM_H264  = (1 << 2),//!< H264码流
	FH_STREAM_H265  = (1 << 3),//!< H265码流

	FH_STREAM_ALL   = 0xffffffff,//!< 所有码率类型
	FH_STREAM_TYPE_DUMMY= 0xffffffff,
}FH_STREAM_TYPE;

typedef enum
{
	H264_PROFILE_BASELINE   = 66,//!< baseline profile
	H264_PROFILE_MAIN       = 77,//!< main profile
	H264_PROFILE_HIGH       = 100,//!< main profile
	FH_H264_PROFILE_DUMMY   = 0xffffffff,
}FH_H264_PROFILE;

typedef enum
{
	H265_PROFILE_MAIN = 1,//!< main profile
	FH_H265_PROFILE_DUMMY = 0xffffffff,
}FH_H265_PROFILE;

/*
* !!注意
* 因为H264和H265 对帧类型的定义不是完全一致
* H264:0-P 1-B 2-I
* H265:0-B 1-P 2-I
* 这里我们对两个标准使用统一的定义
* 同时我们加入我们对刷新帧的私有定义
*/
typedef enum {
	FH_FRAME_P              = 0,//!< P帧
	FH_FRAME_B              = 1,//!< B帧
	FH_FRAME_I              = 2,//!< I帧
	FH_FRAME_FRESH_P        = 20,//!< 刷新P帧
	FH_FRAME_FRESH_B        = 21,//!< 刷新B帧
	FH_FRAME_SKIP_P         = 30,//!< SKIP-P帧
	FH_ENC_FRAME_TYPE_DUMMY = 0xffffffff,
} FH_ENC_FRAME_TYPE;

typedef enum {
	FH_REF_IDR                = 0,//!< IDR帧
	FH_REF_P_REF_IDR          = 1,//!< 刷新P帧,只参考IDR帧
	FH_REF_P_REF_M            = 2,//!< 跳帧参考M层的关键P帧,或非跳帧参考的普通P帧
	FH_REF_P_REF_N            = 3,//!< 跳帧参考N层的关键P帧
	FH_REF_P_NOT_REF          = 4,//!< 跳帧参考非M&N的不被参考的P帧
	FH_ENC_REF_TYPE_DUMMY     = 0xffffffff,
} FH_ENC_REF_TYPE;

typedef enum {
	/* Common Type */
	NALU_P_SLICE            = 0,
	NALU_B_SLICE            = 1,
	NALU_I_SLICE            = 2,
	/* Nalu Type For H264*/
	NALU_IDR                = 5,
	NALU_SEI                = 6,
	NALU_SPS                = 7,
	NALU_PPS                = 8,
	NALU_AUD                = 9,
	/* Nalu Type For H265*/
	NALU_H265_VPS_NUT        = 32,
	NALU_H265_SPS_NUT        = 33,
	NALU_H265_PPS_NUT        = 34,
	NALU_H265_PREFIX_SEI_NUT = 39,
	FH_ENC_NALU_TYPE_DUMMY = 0xffffffff,
} FH_ENC_NALU_TYPE;


typedef enum {
	FH_STM_PUB_BUF   = 0,//!< 编码共用缓冲池
	FH_STM_CHN_BUF   = 1,//!< 编码通道独立缓冲池
	FH_STM_NOT_INIT  = 2,//!< 编码通道未初始化(未启动或通道被裁剪)
	FH_ENC_STREAM_BUF_MODE_DUMMY = 0xffffffff,
} FH_ENC_STREAM_BUF_MODE;

typedef struct
{
	FH_ENC_STREAM_BUF_MODE stm_mode;   //!< 通道码流工作模式
	FH_UINT32              stm_addr;   //!< 通道码流缓冲地址
	FH_UINT32              stm_size;   //!< 通道码流缓冲池大小
	FH_UINT32              max_stm_num;//!< 通道码流最大存放帧数(实际存放数量为此值-1)
}FH_VENC_STM_CONFIG;

typedef struct
{
	FH_UINT32 support_type;            //!< 编码类型,可以同时支持多种类型(FH_VENC_TYPE)
	FH_SIZE   max_size;                //!< 最大编码幅面
	FH_UINT32 bframe_num;              //!< 对B帧的支持,为保留字段,暂不支持
}FH_VENC_CHN_CAP;

/* H264 编码属性*/
typedef struct
{
	FH_H264_PROFILE profile;           //!< 编码档次
	FH_UINT32       i_frame_intterval; //!< GOP长度
	FH_SIZE         size;              //!< 编码图像幅面
}FH_VENC_H264_ATTR;

/*H264 智能编码属性*/
typedef struct
{
	FH_H264_PROFILE profile;                 //!< 编码档次
	FH_UINT32       refresh_frame_intterval; //!< 刷新帧间隔
	FH_SIZE         size;                    //!< 编码图像幅面

	FH_BOOL         smart_en;                //!< 智能编码使能。取值范围: [0-1]
	FH_BOOL         texture_en;              //!< 纹理映射结果使能。取值范围: [0-1]
	FH_BOOL         backgroudmodel_en;       //!< 背景建模结果使能。取值范围: [0-1]
	FH_BOOL         fresh_ltref_en;          //!< 使用刷新帧作为长期参考帧。取值范围: [0-1]
	FH_GOP_TH       gop_th;                  //!< 设置静止帧门限，决定GOP长度
	FH_UINT32       bgm_chn;                 //!< 获取BGM信息的BGM通道号
}FH_VENC_S264_ATTR;

/*H265 编码属性*/
typedef struct
{
	FH_H265_PROFILE profile;           //!< 编码档次
	FH_UINT32       i_frame_intterval; //!< GOP长度
	FH_SIZE         size;              //!< 编码图像幅面
}FH_VENC_H265_ATTR;

/*H265 智能编码属性*/
typedef struct
{
	FH_H265_PROFILE profile;                 //!< 编码档次
	FH_UINT32       refresh_frame_intterval; //!< 刷新帧间隔
	FH_SIZE         size;                    //!< 编码图像幅面

	FH_BOOL         smart_en;                //!< 智能编码使能。取值范围: [0-1]
	FH_BOOL         texture_en;              //!< 纹理映射结果使能。取值范围: [0-1]
	FH_BOOL         backgroudmodel_en;       //!< 背景建模结果使能。取值范围: [0-1]
	FH_BOOL         fresh_ltref_en;          //!< 使用刷新帧作为长期参考帧。取值范围: [0-1]
	FH_GOP_TH       gop_th;                  //!< 设置静止帧门限，决定GOP长度
	FH_UINT32       bgm_chn;                 //!< 获取BGM信息的BGM通道号
}FH_VENC_S265_ATTR;

/*JPEG 编码属性*/
typedef struct
{
	FH_UINT32 qp;           //!< 量化参数。取值范围: [0-98]
	FH_UINT32 rotate;       //!< 旋转角度。取值范围: [0-3]
	FH_UINT32 encode_speed; //!< 编码速度，推荐值0.值越小速度越快，对带宽占用越多。取值范围: [0-9]
}FH_VENC_JPEG_ATTR;

/*MJPEG 编码属性*/
typedef struct
{
	FH_SIZE   pic_size;     //!< 编码幅面
	FH_UINT32 rotate;       //!< 旋转角度。取值范围: [0-3]
	FH_UINT32 encode_speed; //!< 编码速度，推荐值0.值越小速度越快，对带宽占用越多。取值范围: [0-9]
}FH_VENC_MJPEG_ATTR;

typedef struct
{
	FH_VENC_TYPE enc_type;  //!< 编码类型
	union
	{
		FH_VENC_JPEG_ATTR  jpeg_attr;  //!< JPEG编码属性
		FH_VENC_MJPEG_ATTR mjpeg_attr; //!< MJPEG编码属性
		FH_VENC_H264_ATTR  h264_attr;  //!< 普通H264编码属性
		FH_VENC_S264_ATTR  s264_attr;  //!< 智能H264编码属性
		FH_VENC_H265_ATTR  h265_attr;  //!< 普通H265编码属性
		FH_VENC_S265_ATTR  s265_attr;  //!< 智能H265编码属性
	};
}FH_VENC_CHN_ATTR;

/*RC相关配置*/
typedef struct
{
	FH_FRAMERATE src_frmrate;            //!< 输入帧率，作为参考帧率。取值范围: [0-1]

	FH_UINT32    usrdrop_enable;         //!< 是否进行用户自定义丢帧。取值范围: [0-1]
	FH_FRAMERATE dst_frmrate;            //!< 输出帧率，输出帧率大于等于设定输入帧率，输出帧率为输入帧率
	FH_UINT32    usrdrop_pskip;          //!< 选择是否pskip编码。取值范围: [0-1]

	FH_UINT32    qpdrop_enable;          //!< 是否允许QP门限触发丢帧。取值范围: [0-1]
	FH_UINT32    qpdrop_qpth;            //!< QP门限触发丢帧,QP的门限值。取值范围: [H264/H265 : 0-51,MJPEG : 0-98]
	FH_FRAMERATE qpdrop_minfrmrate;      //!< QP门限触发丢帧,丢帧后不低于该帧率
	FH_UINT32    qpdrop_pskip;           //!< 选择是否pskip编码。取值范围: [0-1]

	FH_UINT32    InstantRate_enable;     //!< 是否允许瞬时码率触发丢帧。取值范围: [0-1]
	FH_UINT32    InstantRate_percentth;  //!< 触发瞬时码率丢帧的码率门限,为目标码率的百分比。取值范围: [110-1000]
	FH_FRAMERATE InstantRate_minfrmrate; //!< 瞬时码率的丢帧间隔，丢帧后不低于该帧率
	FH_UINT32    InstantRate_pskip;      //!< 选择是否pskip编码。取值范围: [0-1]
}FH_DROP_FRAME;

typedef struct
{
	FH_UINT32    iqp;                 //!< I帧QP。取值范围: [0-51]
	FH_UINT32    pqp;                 //!< P帧QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
}FH_H264_FIXQP_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
}FH_H264_CBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    IminQP;              //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;              //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;              //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;              //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
}FH_H264_VBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    IminQP;              //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;              //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;              //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;              //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
	FH_UINT32    stillrate_percent;   //!< 静止下最小码率，单位%。推荐值30。。取值范围: [25-100]
	FH_UINT32    maxstillqp;          //!< 静止最大QP，作为编码质量的限制。推荐值34。。取值范围: [0-51]
}FH_H264_AVBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;         //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    stbitrate;       //!< 短期目标码率，单位bit
	FH_UINT32    ltbitrate;       //!< 长期目标码率，单位bit
	FH_UINT32    maxrate_percent; //!< 最大码率，为短期目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IminQP;          //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;          //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;          //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;          //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;       //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    IFrmMaxBits;     //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;      //!< P帧最小QP与当前I帧QP 的限制，pqp > = iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;       //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;       //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level; //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
	FH_SINT32    acceptable_qp;   //!< cvbr模式时可接受图像质量阈值,QP小于等于此值时不会透支码率,推荐值:32。取值范围: [0-51]
}FH_H264_CVBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    IminQP;              //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;              //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;              //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;              //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
	FH_UINT32    minrate_percent;     //!< PSNR较高时最小码率，单位%。推荐值30。。取值范围: [25-100]
	FH_UINT32    psnr_ul;             //!< 当前PSNR上限,高于此值时会降低目标码流, 定点化，为实际值x100。。取值范围: [2000-4000]
	FH_UINT32    psnr_ll;             //!< 当前PSNR下限,低于此值时会提高目标码流, 定点化，为实际值x100。。取值范围: [2000-4000]
}FH_H264_QVBR_ATTR;

typedef struct
{
	FH_UINT32    iqp;                 //!< I帧QP。取值范围: [0-51]
	FH_UINT32    qp;                  //!< P/B帧QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
}FH_H265_FIXQP_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
}FH_H265_CBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    IminQP;              //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;              //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;              //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;              //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
}FH_H265_VBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    IminQP;              //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;              //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;              //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;              //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
	FH_UINT32    stillrate_percent;   //!< 静止下最小码率，单位%。推荐值30。。取值范围: [25-100]
	FH_UINT32    maxstillqp;          //!< 静止最大QP，作为编码质量的限制。推荐值34。。取值范围: [0-51]
}FH_H265_AVBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    IminQP;              //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;              //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;              //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;              //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    maxrate_percent;     //!< 最大码率，为目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IFrmMaxBits;         //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;          //!< P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;           //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;           //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level;     //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
	FH_UINT32    minrate_percent;     //!< PSNR较高时最小码率，单位%。推荐值30。。取值范围: [25-100]
	FH_UINT32    psnr_ul;             //!< 当前PSNR上限,高于此值时会降低目标码流, 定点化，为实际值x100。。取值范围: [2000-4000]
	FH_UINT32    psnr_ll;             //!< 当前PSNR下限,低于此值时会提高目标码流, 定点化，为实际值x100。。取值范围: [2000-4000]
}FH_H265_QVBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;         //!< 初始化QP。取值范围: [0-51]
	FH_UINT32    stbitrate;       //!< 短期目标码率，单位bit
	FH_UINT32    ltbitrate;       //!< 长期目标码率，单位bit
	FH_UINT32    maxrate_percent; //!< 最大码率，为短期目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_UINT32    IminQP;          //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32    ImaxQP;          //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32    PminQP;          //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32    PmaxQP;          //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE FrameRate;       //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32    IFrmMaxBits;     //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_SINT32    IP_QPDelta;      //!< P帧最小QP与当前I帧QP 的限制，pqp > = iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.
	FH_SINT32    I_BitProp;       //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32    P_BitProp;       //!< I帧P帧的码率分配比例，推荐值1。
	FH_SINT32    fluctuate_level; //!< 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。。取值范围: [0-6]
	FH_SINT32    acceptable_qp;   //!< cvbr模式时可接受图像质量阈值,QP小于等于此值时不会透支码率,推荐值:32。取值范围: [0-51]
}FH_H265_CVBR_ATTR;

typedef struct
{
	FH_UINT32    qp;                  //!< 量化参数。取值范围: [0-98]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
}FH_MJPEG_FIXQP_ATTR;

typedef struct
{
	FH_UINT32    initqp;              //!< 量化参数。取值范围: [0-98]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
}FH_MJPEG_CBR_ATTR;

typedef struct
{
	FH_UINT32    initqp;              //!< 量化参数。取值范围: [0-98]
	FH_UINT32    bitrate;             //!< 目标码率，单位bit
	FH_UINT32    minqp;               //!< 码控最小QP。取值范围: [0-98]
	FH_UINT32    maxqp;               //!< 码控最大QP。取值范围: [0-98]
	FH_FRAMERATE FrameRate;           //!< 参考帧率，没有帧率控制的作用。
}FH_MJPEG_VBR_ATTR;

typedef struct
{
	FH_VENC_RC_MODE rc_type;          //!< 码控模式
	union
	{
		FH_MJPEG_FIXQP_ATTR mjpeg_fixqp; //!< MJPEG 定QP
		FH_MJPEG_CBR_ATTR   mjpeg_cbr;   //!< MJPEG 定码率
		FH_MJPEG_VBR_ATTR   mjpeg_vbr;   //!< MJPEG 变码率
		FH_H264_VBR_ATTR    h264_vbr;    //!< H264 变码率
		FH_H264_CBR_ATTR    h264_cbr;    //!< H264 定码率
		FH_H264_FIXQP_ATTR  h264_fixqp;  //!< H264 定QP
		FH_H264_AVBR_ATTR   h264_avbr;   //!< H264 自适应变码率
		FH_H265_VBR_ATTR    h265_vbr;    //!< H265 变码率
		FH_H265_CBR_ATTR    h265_cbr;    //!< H265 定码率
		FH_H265_FIXQP_ATTR  h265_fixqp;  //!< H265 定QP
		FH_H265_AVBR_ATTR   h265_avbr;   //!< H265 自适应变码率
		FH_H264_CVBR_ATTR   h264_cvbr;   //!< H264 CVBR
		FH_H265_CVBR_ATTR   h265_cvbr;   //!< H265 CVBR
		FH_H264_QVBR_ATTR   h264_qvbr;   //!< H264 QVBR

		FH_H265_QVBR_ATTR   h265_qvbr;   //!< H265 QVBR
	};
}FH_VENC_RC_ATTR;

typedef struct
{
	FH_VENC_CHN_ATTR chn_attr;                  //!< 通道属性
	FH_VENC_RC_ATTR  rc_attr;                   //!< 码率属性
}FH_VENC_CHN_CONFIG;

typedef struct
{
	FH_PHYADDR              lumma_addr;         //!< 亮度地址
	FH_PHYADDR              chroma_addr;        //!< 色度地址
	FH_UINT64               time_stamp;         //!< 时间戳
	FH_SIZE                 size;               //!< 幅面
	FH_UINT32               pool_id;            //!< vb pool id(非VB模式建议配置为-1UL)
}FH_ENC_FRAME;

typedef struct
{
	FH_ENC_NALU_TYPE        type;               //!< NALU类型
	FH_UINT32               length;             //!< 码流长度
	FH_ADDR                 start;              //!< 码流地址
}FH_ENC_STREAM_NALU;

typedef struct
{
	FH_ADDR                 start;              //!< 码流地址
	FH_ENC_FRAME_TYPE       frame_type;         //!< 帧类型
	FH_UINT32               length;             //!< 码流长度
	FH_UINT64               time_stamp;         //!< 时间戳
	FH_UINT32               nalu_cnt;           //!< NALU数量
	FH_ENC_STREAM_NALU      nalu[MAX_NALU_CNT]; //!< NALU属性
	FH_ENC_REF_TYPE         ref_type;           //!< 帧参考类型
}FH_H264_STREAM;

typedef struct
{
	FH_ADDR                 start;              //!< 码流地址
	FH_ENC_FRAME_TYPE       frame_type;         //!< 帧类型
	FH_UINT32               length;             //!< 码流长度
	FH_UINT64               time_stamp;         //!< 时间戳
	FH_UINT32               nalu_cnt;           //!< NALU数量
	FH_ENC_STREAM_NALU      nalu[MAX_NALU_CNT]; //!< NALU属性
	FH_ENC_REF_TYPE         ref_type;           //!< 帧参考类型
}FH_H265_STREAM;

typedef struct
{
	FH_ADDR                 start;              //!< 码流地址
	FH_UINT32               length;             //!< 码流长度
	FH_UINT64               time_stamp;         //!< 时间戳
	FH_UINT32               qp;                 //!< 当前帧使用的量化值
}FH_JPEG_STREAM;

typedef struct
{
	FH_UINT32 MeanQp;         //!< 编码当前帧平均QP
	FH_UINT32 ResidualBitNum; //!< 编码当前帧残差bit数
	FH_UINT32 HeadBitNum;     //!< 编码当前帧头信息bit数
	FH_UINT32 MadiVal;        //!< 编码当前帧空域纹理复杂度Madi值
	FH_UINT32 MadpVal;        //!< 编码当前帧时域运动复杂度Madp值
	FH_UINT32 MseLcuCnt;      //!< 编码当前帧中的LCU个数
	FH_UINT32 MseSum;         //!< 编码当前帧中的MSE(均方差)
	FH_DOUBLE psnr;           //!< psnr，因为驱动定点化原因有一些精度损失
	FH_UINT64 distortion;     //!< 失真度,用户可以用此值得到准确psnr,psnr = 10log10(w*h*255*255/distortion)
}FH_H264_ADV_STREAM_INFO;

typedef struct
{
	FH_UINT32 MeanQp;         //!< 编码当前帧平均QP
	FH_UINT32 ResidualBitNum; //!< 编码当前帧残差bit数
	FH_UINT32 HeadBitNum;     //!< 编码当前帧头信息bit数
	FH_UINT32 MadiVal;        //!< 编码当前帧空域纹理复杂度Madi值
	FH_UINT32 MadpVal;        //!< 编码当前帧时域运动复杂度Madp值
	FH_UINT32 MseLcuCnt;      //!< 编码当前帧中的LCU个数
	FH_UINT32 MseSum;         //!< 编码当前帧中的MSE(均方差)
	FH_DOUBLE psnr;           //!< psnr，因为驱动定点化原因有一些精度损失
	FH_UINT64 distortion;     //!< 失真度,用户可以用此值得到准确psnr,psnr = 10log10(w*h*255*255/distortion)
}FH_H265_ADV_STREAM_INFO;

typedef struct
{
	FH_UINT32 reserved;   //!<保留属性
}FH_JPEG_ADV_STREAM_INFO;


typedef struct
{
	FH_STREAM_TYPE stmtype;                     //!< 码流类型
	FH_UINT32      chan;                        //!< 通道号
	union
	{
		FH_JPEG_STREAM jpeg_stream;                //!< JPEG码流
		FH_JPEG_STREAM mjpeg_stream;               //!< MJPEG 码流
		FH_H264_STREAM h264_stream;                //!< H264码流
		FH_H265_STREAM h265_stream;                //!< H265码流
	};
	union
	{
		FH_JPEG_ADV_STREAM_INFO jpeg_info;         //!< JPEG码流统计信息
		FH_JPEG_ADV_STREAM_INFO mjpeg_info;        //!< MPEG码流统计信息
		FH_H264_ADV_STREAM_INFO h264_info;         //!< H264码流统计信息
		FH_H265_ADV_STREAM_INFO h265_info;         //!< H265码流统计信息
	};
}FH_VENC_STREAM;

typedef struct
{
	FH_UINT32               lastqp;             //!< 上帧QP
	FH_UINT32               lastiqp;            //!< 上一I帧QP
	FH_UINT32               bps;                //!< 实际码率
	FH_UINT32               FrameToEnc;         //!< 待编码帧数
	FH_UINT32               framecnt;           //!< 已编码帧数
	FH_UINT32               streamcnt;          //!< 输出队列中的帧数
	FH_UINT32               lostcnt;            //!< 通道的累计丢帧数
	FH_UINT32               stat[5];            //!< 保留变量
}FH_CHN_STATUS;

typedef struct
{
 	FH_UINT32               enable;             //!< ROI使能。取值范围: [0-1]
	FH_SINT32               qp;                 //!< 对应level使用的QP。取值范围: [绝对QP:0-51，相对QP:-51-51]
	FH_AREA                 area;               //!< ROI区域
	FH_UINT32               level;              //!< ROI level,0代表不使用ROI。取值范围: [0-7]
	FH_BOOL                 isdeltaqp;          //!< 0:绝对QP,使用用户QP 1:相对QP,在RC QP基础上加减Delta值。取值范围: [0-1]
}FH_ROI;

typedef struct
{
	FH_SINT32               qp[7];              //!< 对应level-1使用的QP。取值范围: [绝对QP:0-51，相对QP:-51-51]
	FH_BOOL                 isdeltaqp[7];       //!< 对应level-1的设置，0:绝对QP,使用用户QP 1:相对QP,在RC QP基础上加减Delta值。取值范围: [0-1]
	FH_UINT32               size;               //!< map 长度, w/64 * h/64 * 16, w/h为64对齐后的宽高
	FH_PHYADDR              roi_addr;           //!< map 基地址
}FH_ROI_MAP;

typedef struct
{
	FH_UINT32               bitrate;            //!< 目标码率，单位bit
	FH_UINT32               IminQP;             //!< I帧最小QP。取值范围: [0-51]
	FH_UINT32               ImaxQP;             //!< I帧最大QP。取值范围: [0-51]
	FH_UINT32               PminQP;             //!< P帧最小QP。取值范围: [0-51]
	FH_UINT32               PmaxQP;             //!< P帧最大QP。取值范围: [0-51]
	FH_FRAMERATE            FrameRate;          //!< 参考帧率，没有帧率控制的作用。
	FH_UINT32               IFrmMaxBits;        //!< 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。
	FH_UINT32               maxrate_percent;    //!< 最大码率，为短期目标码率的百分比。推荐值200。。取值范围: [120-800]
	FH_SINT32               I_BitProp;          //!< I帧P帧的码率分配比例，推荐值5。
	FH_SINT32               P_BitProp;          //!< I帧P帧的码率分配比例，推荐值1。
}FH_VENC_RC_CHANGEPARAM;

typedef struct
{
	FH_UINT32             mode;                 //!< 0:不支持抽帧 1:单层跳帧 2:双层跳帧。取值范围: [0-2]
	FH_SINT32             svc_m;                //!< 第一层跳帧的间隔，mode = 1 or 2有效。取值范围: [0-15]
	FH_SINT32             svc_n;                //!< 第二层跳帧的间隔，mode = 2 有效。取值范围: [0-15]
}FH_VENC_REF_MODE;

typedef struct
{
	FH_UINT32 ref_pic_num;                      //!< 当前帧实际参考帧数量
	FH_UINT32 frm_type;                         //!< 当前帧类型, 目前仅支持P帧
	FH_SINT32 ref_pic_poc[MAX_REF_FRM_CNT];     //!< 参考帧POC, 目前仅支持前向参考
}FH_VENC_FRM_REF_CFG;

typedef struct
{
	FH_SINT32           gop_size;                 //!< 参考结构大小
	FH_VENC_FRM_REF_CFG gop_param[MAX_CUSTOM_GOP];//!< 帧的参考关系
}FH_VENC_CUSTOM_GOP_CFG;

typedef enum
{
	ENCPARAM_CMD_NONE                = 0,
	ENCPARAM_CMD_H264_LINE_RC        = 1,//!< 行级码控,适用于H264
	ENCPARAM_CMD_LINE_RC             = 1,//!< 行级码控,适用于H264&H265
	ENCPARAM_CMD_H264_SEARCH_RANGE   = 2,//!< 搜索范围,适用于H264
	ENCPARAM_CMD_H264_CHROMAQP_DELTA = 3,//!< 色度QP偏移,适用于H264
	ENCPARAM_CMD_H264_ENCODE_STYLE   = 4,//!< 编码风格,适用于H264
	ENCPARAM_CMD_TEXTUREQP_RANGE     = 5,//!< 纹理QP范围,适用于H264&H265
	ENCPARAM_CMD_RC_ADV_PARAM        = 6,//!< 高级码控参数,适用于H264&H265
	ENCPARAM_CMD_H265_LAMBDA         = 7,//!< 调整编码lambda参数,适用于H265
	ENCPARAM_CMD_LAMBDA              = 7,//!< 调整编码lambda参数,适用于H264&H265
	ENCPARAM_CMD_H265_CHROMAQP_DELTA = 8,//!< 色度QP偏移,适用于H265
	ENCPARAM_CMD_SMART_GOPMODE       = 9,//!< 智能下的参考关系
	ENCPARAM_CMD_BGMMODE             = 10,//!< 编码对BGM信息的使用配置
	ENCPARAM_CMD_ADV_BKGQP           = 11,//!<根据纹理基本调整背景QP
	ENCPARAM_CMD_BIGPFRM             = 12,//!<指定在GOP中插入大P帧的配置
	ENCPARAM_CMD_GOP                 = 13,//!<I帧间隔
	FH_ENCPARAM_CMD_DUMMY            = 0xffffffff,
}FH_ENCPARAM_CMD;

typedef struct
{
	FH_UINT32 linerc_en;        //!< 行级码控使能。取值范围: [0-1]
	FH_UINT32 linerc_iadd;      //!< 行级码控I帧QP增加范围。取值范围: [0-51]
	FH_UINT32 linerc_iminus;    //!< 行级码控I帧QP减少范围。取值范围: [0-51]
	FH_UINT32 linerc_padd;      //!< 行级码控P帧QP增加范围。取值范围: [0-51]
	FH_UINT32 linerc_pminus;    //!< 行级码控P帧QP减少范围。取值范围: [0-51]
}ENCPARAM_H264_LINE_RC;

typedef ENCPARAM_H264_LINE_RC ENCPARAM_LINE_RC;

typedef struct
{
	FH_UINT32 search_x;         //!< 编码搜索范围x,,范围为(-(i+1)*16,(i+1)*16-1).增大搜索范围会导致编码性能下降，默认0。取值范围: [0~3]
	FH_UINT32 search_y;         //!< 编码搜索范围y,,范围为(-(i+1)*16,(i+1)*16-1).增大搜索范围会导致编码性能下降，默认0。取值范围: [0~3]
}ENCPARAM_H264_SEARCH_RANGE;

typedef struct
{
	FH_UINT32 ilambda_weight; //!< I帧lambda权重,单位百分比。取值范围: [1~1000]
	FH_UINT32 plambda_weight; //!< P/B帧lambda权重,单位百分比。取值范围: [1~1000]
	FH_UINT32 rplambda_weight;//!< 刷新帧lambda权重,单位百分比。取值范围: [1~1000]
}ENCPARAM_H265_LAMBDA_ST;

typedef ENCPARAM_H265_LAMBDA_ST ENCPARAM_LAMBDA_ST;

typedef struct
{
	FH_SINT32 ChromaQPdelta;      //!< 色度Cb-QP相对与亮度QP的delta。取值范围: [-12~12]
	FH_SINT32 SecondChromaQPdelta;//!< 色度Cr-QP相对与亮度QP的delta,需Profile为High以上时支持。取值范围: [-12~12]
}ENCPARAM_H264_CHROMAQP_DELTA_ST;

typedef struct
{
	FH_SINT32 ChromaCbQPdelta;   //!< 色度Cb-QP相对与亮度QP的delta。取值范围: [-12~12]
	FH_SINT32 ChromaCrQPdelta;   //!< 色度Cr-QP相对与亮度QP的delta。取值范围: [-12~12]
}ENCPARAM_H265_CHROMAQP_DELTA_ST;

typedef enum
{
	ENCODE_STYLE_NORMAL = 0,
	ENCODE_STYLE_NIGHT = 1,
	ENCPARAM_H264_ENCODE_STYLE_DUMMY = 0xffffffff,
}ENCPARAM_H264_ENCODE_STYLE;

typedef struct
{
	FH_UINT32 text_minqp_minus; //!< 纹理映射的最小QP在原先RC的最小QP上减少的值。取值范围: [0-51]
	FH_UINT32 text_maxqp_add;   //!< 纹理映射的最大QP在原先RC的最大QP上的增加的值。取值范围: [0-51]
}ENCPARAM_TEXTUREQP_RANGE_ST;

typedef struct
{
	FH_SINT32 iqp_weight_tbit; //!< I帧QP分配,目标码率的影响权重，推荐值:1。
	FH_SINT32 iqp_weight_pqp;  //!< I帧QP分配,之前GOP PQP的影响权重，推荐值:2。
	FH_SINT32 pqp_limit;       //!< gop pqp在I帧QP上允许下降的幅度，推荐值:-3。
	FH_UINT32 vbuf_depth;      //!< 码控虚拟缓存深度,含义为平均每帧码率的倍数，推荐值:40。
	FH_UINT32 avbr_strength;   //!< AVBR强度,值越大,对同等局部运动时码率压制越强,但对应图像质量也会下降.默认值为0.。取值范围: [0-4]
}ENCPARAM_RC_ADV_PARAM;

typedef struct
{
	FH_UINT32 smart_gop_mode; //!< 智能下GOP模式,0:P帧双参考 1: P帧单参考(允许按按一定间隔使用双参考)。取值范围: [0-1]
	FH_UINT32 dualp_interval; //!< gop模式1下生效,0代表不使用双参考,其他代表使用双参考的P帧间隔。
}ENCPARAM_SMART_GOPMODE;

typedef struct
{
	FH_UINT32 use_bgm_info; //!< 编码是否允许使用BGM信息。普通编码下可用。。取值范围: [0-1]
	FH_UINT32 cycle_no_bgm; //!< 使用BGM时有效,指定一定帧间隔不使用BGM信息,0代表每一帧都使用。
	FH_UINT32 bgm_chn;      //!< 获取BGM信息的BGM通道号
}ENCPARAM_BGMMODE;

typedef struct
{
	FH_UINT32 idx;          //!< 所需设置的纹理索引。取值范围: [0-15]
	FH_UINT32 bkg_delta_qp; //!< 背景QP与前景QP之间的差值。取值范围: [0-51]
	FH_UINT32 bkg_max_qp;   //!< 背景QP的最大值。取值范围: [0-51]
}ENCPARAM_ADV_BKGQP;

typedef struct
{
	FH_UINT32 deltapqp_interval; //!< 智能非智能都有效。0代表不使用,其他代表使用特殊P帧间隔。
	FH_SINT32 deltapqp_delta;    //!< 允许用户按一定间隔对P帧QP加上一个指定DeltaQP，以取得更好的效果刷新。。取值范围: [-20-20]
	FH_SINT32 deltapqp_lambda;   //!< 允许用户按一定间隔对P帧QP重新定义一个lambda值，以取得更好的效果刷新。。取值范围: [1-1000]
}ENCPARAM_BIG_PFRM;

typedef struct
{
	FH_UINT32 i_interval; //!< I帧间隔
}ENCPARAM_GOP;

typedef union
{
	ENCPARAM_H264_LINE_RC           h264_line_rc;
	ENCPARAM_LINE_RC                line_rc;
	ENCPARAM_H264_SEARCH_RANGE      h264_search_range;
	ENCPARAM_H264_CHROMAQP_DELTA_ST h264_chromaqp;
	ENCPARAM_H265_CHROMAQP_DELTA_ST h265_chromaqp;
	ENCPARAM_H264_ENCODE_STYLE      h264_encodestyle;
	ENCPARAM_TEXTUREQP_RANGE_ST     textureqp_range;
	ENCPARAM_RC_ADV_PARAM           rc_adv;
	ENCPARAM_H265_LAMBDA_ST         h265_lambda;
	ENCPARAM_LAMBDA_ST              lambda;
	ENCPARAM_SMART_GOPMODE          gop_mode;
	ENCPARAM_BGMMODE                bgm_mode;
	ENCPARAM_ADV_BKGQP              adv_bkgqp;
	ENCPARAM_BIG_PFRM               bigpfrm;
	ENCPARAM_GOP                    gop;
}FH_ENC_PARAM_UNION;

typedef struct
{
	FH_UINT8 aspect_ratio_info_present_flag;//!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT8 aspect_ratio_idc;              //!< 详细含义参见H264/H265协议。取值范围: [0-255]
	FH_UINT8  overscan_info_present_flag;   //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT8  overscan_appropriate_flag;    //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT16 sar_width;                    //!< 详细含义参见H264/H265协议。取值范围: [0-65535]
	FH_UINT16 sar_height;                   //!< 详细含义参见H264/H265协议。取值范围: [0-65535]
}FH_VENC_VUI_ASPECT_RATIO;

typedef struct
{
	FH_UINT8  timing_info_present_flag;      //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT8  fixed_frame_rate_flag;         //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT32 num_units_in_tick;             //!< 详细含义参见H264/H265协议。取值范围: [1-(2^32-1)]
	FH_UINT32 time_scale;                    //!< 详细含义参见H264/H265协议。取值范围: [1-(2^32-1)]
}FH_VENC_VUI_H264_TIME_INFO;

typedef struct
{
	FH_UINT32 timing_info_present_flag;       //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT32 num_units_in_tick;              //!< 详细含义参见H264/H265协议。取值范围: [1-(2^32-1)]
	FH_UINT32 time_scale;                     //!< 详细含义参见H264/H265协议。取值范围: [1-(2^32-1)]
	FH_UINT32 num_ticks_poc_diff_one_minus1;  //!< 详细含义参见H264/H265协议。取值范围: [1-(2^32-2)]
}FH_VENC_VUI_H265_TIME_INFO;

typedef struct
{
	FH_UINT8  video_signal_type_present_flag;  //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT8  video_format;                    //!< 详细含义参见H264/H265协议。取值范围: [H264:0-7;H265:0-5]
	FH_UINT8  video_full_range_flag;           //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT8  colour_description_present_flag; //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_UINT8  colour_primaries;                //!< 详细含义参见H264/H265协议。取值范围: [0-255]
	FH_UINT8  transfer_characteristics;        //!< 详细含义参见H264/H265协议。取值范围: [0-255]
	FH_UINT8  matrix_coefficients;             //!< 详细含义参见H264/H265协议。取值范围: [0-255]
}FH_VENC_VUI_VIDEO_SIGNAL;

typedef struct
{
	FH_UINT8  bitstream_restriction_flag;     //!< 详细含义参见H264/H265协议。取值范围: [0-1]
}FH_VENC_VUI_BITSTREAM_RESTRIC;

typedef struct
{
	FH_UINT32             entropy_coding_mode; //!< 详细含义参见H264协议 0 : cavlc 1 : cabac baseline不支持cabac。取值范围: [0-1]
	FH_UINT32             cabac_init_idc;      //!< 详细含义参见H264协议。取值范围: [0-2]
}FH_H264_ENTROPY;

typedef struct
{
	FH_UINT32             deblocking_filter;   //!< 详细含义参见H264协议。取值范围: [0-1]
	FH_UINT32             disable_deblocking;  //!< 详细含义参见H264协议。取值范围: [0-2]
	FH_SINT32             slice_alpha;         //!< 详细含义参见H264协议。取值范围: [-6~+6]
	FH_SINT32             slice_beta;          //!< 详细含义参见H264协议。取值范围: [-6~+6]
}FH_H264_DBLK;

typedef struct
{
	FH_UINT32             enable;             //!< 使能。取值范围: [0-1]
	FH_UINT32             slicesplit;         //!< 多少个宏块行作为一个slice。最多不能多于16个slice。
}FH_H264_SLICE_SPLIT;

typedef struct
{
	FH_UINT32             enable;              //!< 使能。取值范围: [0-1]
	FH_UINT32             fresh_line;          //!< 刷新的Islice的行数
	FH_UINT32             fresh_step;          //!< 每帧刷新Islice的步进行数
}FH_H264_INTRA_FRESH;

typedef struct
{
	FH_SINT8                      vui_parameters_present_flag; //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_VENC_VUI_ASPECT_RATIO      vui_aspect_ratio;            //!< 详细含义参见H264/H265协议
	FH_VENC_VUI_H264_TIME_INFO    vui_time_info;               //!< 详细含义参见H264/H265协议
	FH_VENC_VUI_VIDEO_SIGNAL      vui_video_signal;            //!< 详细含义参见H264/H265协议
	FH_VENC_VUI_BITSTREAM_RESTRIC vui_bitstream_restric;       //!< 详细含义参见H264/H265协议
}FH_H264_VUI;

typedef struct
{
	FH_UINT32             disable_flag;     //!< 详细含义参见H265协议。取值范围: [0-1]
	FH_SINT32             beta_offset_div2; //!< 详细含义参见H265协议。取值范围: [-6~+6]
	FH_SINT32             tc_offset_div2;   //!< 详细含义参见H265协议。取值范围: [-6~+6]
}FH_H265_DBLK;

typedef struct
{
	FH_UINT32             debreath_en;      //!<呼吸效应使能。取值范围: [0-1]
	FH_UINT32             debreath_ratio;   //!<呼吸效应比率, 越小呼吸效应改善越明显，推荐值16。取值范围: [0-31]
}FH_DEBREATH;

typedef struct
{
	FH_UINT32             cabac_init_flag;  //!< 详细含义参见H265协议。取值范围: [0-1]
}FH_H265_ENTROPY;

typedef struct
{
	FH_UINT32             mode;            //!< 0 : 单slice 1 : 按行划分slice。取值范围: [0-1]
	FH_UINT32             slice_arg;       //!< Slice行数(64-aligned)，slice不能超过15个
}FH_H265_SLICE_SPLIT;

typedef struct
{
	FH_UINT32             mode;             //!< 0: 不使用帧内刷新 1:按行刷新 2:按列刷新 3:按CTU进行刷新。取值范围: [0-3]
	FH_UINT32             fresh_line;       //!< 刷新帧内编码的行数
	FH_UINT32             fresh_step;       //!< 每帧刷新Islice的步进行数
}FH_H265_INTRA_FRESH;

typedef struct
{
	FH_SINT8                      vui_parameters_present_flag; //!< 详细含义参见H264/H265协议。取值范围: [0-1]
	FH_VENC_VUI_ASPECT_RATIO      vui_aspect_ratio;            //!< 详细含义参见H264/H265协议
	FH_VENC_VUI_H265_TIME_INFO    vui_time_info;               //!< 详细含义参见H264/H265协议
	FH_VENC_VUI_VIDEO_SIGNAL      vui_video_signal;            //!< 详细含义参见H264/H265协议
	FH_VENC_VUI_BITSTREAM_RESTRIC vui_bitstream_restric;       //!< 详细含义参见H264/H265协议
}FH_H265_VUI;

typedef struct
{
	FH_UINT32 enable;           //!< 编码模式倾向性使能。取值范围: [0-１]
	FH_UINT32 skip_rate_offset; //!< 编码模式倾向性,越大越不倾向选择此模式。取值范围: [0-255]
	FH_UINT32 merge_rate_offset;//!< 编码模式倾向性,越大越不倾向选择此模式。取值范围: [0-255]
	FH_UINT32 cbf0_rate_offset; //!< 编码模式倾向性,越大越不倾向选择此模式。取值范围: [0-255]
	FH_UINT32 cbf1_rate_offset; //!< 编码模式倾向性,越大越不倾向选择此模式。取值范围: [0-255]
	FH_UINT32 intra_rate_offset;//!< 编码模式倾向性,越大越不倾向选择此模式。取值范围: [0-255]
}FH_ENC_BIAS_PARAM;

typedef struct
{
	FH_ENC_BIAS_PARAM fg_bias; //!< 前景CU模式倾向性调节
	FH_ENC_BIAS_PARAM bg_bias; //!< 背景CU模式倾向性调节
	FH_ENC_BIAS_PARAM frm_bias;//!< 帧级CU模式倾向性调节
}FH_ENC_BIAS;

typedef struct{
	FH_PHYADDR dumpPhyAddr;
	FH_VOID * dumpVirAddr;
	FH_UINT32 size;
}FH_INPUT_DUMP;

/***************************************************************************************

										IOC

****************************************************************************************/


typedef struct
{
	FH_UINT32 lumaAddr;
	FH_UINT32 chromaAddr;
	FH_UINT32 strideW;
	FH_UINT32 strideH;
}FH_VENC_EX_CONF;
typedef struct
{
	FH_UINT32 chan;
	FH_PHYADDR addr;
	FH_UINT32 size;
	FH_UINT32 timeout_ms;
	FH_UINT32 isidle;
}FH_ENC_CHECK_ADDR;

typedef struct
{
	FH_UINT32 request_type;
	FH_VENC_STREAM stVencstreamAttr;
}FH_VENC_STREAM_BLOCK;

typedef struct
{
	FH_UINT32 chan;
	FH_VENC_STREAM stVencstreamAttr;
	FH_UINT32 timeout_ms;
}FH_VENC_STREAM_TIMEOUT;

typedef struct
{
	FH_UINT32 chan;
	FH_ENC_FRAME stVencsubmitframe;
	FH_VPU_VO_MODE datamode;
}FH_VENC_SEND_STREAM;

typedef struct
{
	FH_UINT32 luma_addr;
	FH_UINT32 chroma_addr;
	FH_UINT32 stream_addr;
	FH_UINT32 stream_size;
}FH_VENC_PRE_ROLLING;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT32 workMode;
	FH_VENC_PRE_ROLLING stPreRollingAttr;
	FH_VENC_CHN_CONFIG stVencChnAttr;
}FH_VENC_SET_CHN_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_VENC_CHN_CONFIG stVencChnAttr;
}FH_VENC_GET_CHN_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_CHN_STATUS stVencStatus;
}FH_VENC_CHN_STATUS_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_VENC_STM_CONFIG stVencstreamCfg;
}FH_VENC_STR_CON_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_ROTATE_OPS stVencrotateinfo;
}FH_VENC_ROTATE_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_ROI_MAP stVencroimap;
}FH_VENC_ROIMAP_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_ROI stVencroiinfo;
}FH_VENC_ROI_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_VENC_RC_ATTR stVencrcattr;
}FH_VENC_RC_PARA_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_VENC_RC_CHANGEPARAM stRcparam;
}FH_VENC_RC_CHANGE_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_DROP_FRAME stVencdropattr;
}FH_VENC_DROP_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT32 bkg_delta_qp;
	FH_UINT32 bkg_max_qp;
}FH_VENC_BKGQP_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_ENCPARAM_CMD cmd;
	FH_ENC_PARAM_UNION unParam;
}FH_VENC_ENCPARAM_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_H264_ENTROPY stVencentropy;
}FH_VENC_H264ENTROPY_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_H264_DBLK stVencdblk;
}FH_VENC_H264DBLK_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_H264_SLICE_SPLIT stVencslicesplit;
}FH_VENC_H264SLICE_SPLIT_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_H265_ENTROPY stVencentropy;
}FH_VENC_H265ENTROPY_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_H265_DBLK stVencdblk;
}FH_VENC_H265DBLK_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_H265_SLICE_SPLIT stVencslicesplit;
}FH_VENC_H265SLICE_SPLIT_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_DEBREATH stDeBreath;
}FH_VENC_DEBREATH_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_ENC_BIAS stBias;
}FH_VENC_BIAS_ATTR;

typedef struct{
	FH_UINT32 scenarioId;
	FH_UINT32 clkThreshold;
	FH_UINT32 clk_rate[RATE_NUM];
	FH_UINT32 clk_slow_rate[RATE_NUM];
}FH_VENC_SCENARIO_ATTR;

typedef struct
{
	FH_UINT32 dtsGetEn;
	FH_UINT32 curScenarioId;
	FH_VENC_SCENARIO_ATTR ScenarioData[SCENARIO_NUM];
	FH_SINT8 workMode;
}FH_VENC_DTS_CLK_ATTR;

typedef struct
{
	FH_UINT32 strPhyAddr;
	FH_ADDR strVirAddr;
	FH_UINT32 strSize;
}FH_VENC_STREAM_ADDR;

typedef struct
{
	FH_SINT8 lvd;
	FH_SINT8 reserved[3];
}FH_VENC_SET_DRV_CONFIG;

typedef struct
{
	FH_SINT8 switchEnable[CHN_NUM];
	FH_VENC_CHN_CONFIG stVencChnAttr[CHN_NUM];
}FH_VENC_SET_SWITCH_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_INPUT_DUMP dumpAttr;
}FH_VENC_DUMP_ATTR;

typedef struct
{
	FH_UINT32 chan;
	FH_VENC_EX_CONF stVencExAttr;
}FH_VENC_EX_ATTR;
#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
