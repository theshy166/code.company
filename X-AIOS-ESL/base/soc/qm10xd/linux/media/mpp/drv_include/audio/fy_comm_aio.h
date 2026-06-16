#ifndef __FY_COMM_AIO_H__
#define __FY_COMM_AIO_H__

#include "fh_type.h"


#define DEFAULT_AI_CACHE_NUM			6
#define DEFAULT_AO_CACHE_NUM			6

//type
typedef FY_S32				AUDIO_DEV;
typedef FY_S32				AI_CHN;
typedef FY_S32				AO_CHN;
typedef FY_S32				AENC_CHN;
typedef FY_S32				ADEC_CHN;

/****************************enum defined*******************************************/

typedef enum {
	DEV_I2S0 = 0,				//!< I2S 设备号 0
	DEV_ACW0,				//!< 内置codec 设备号 0
	DEV_AUDIO_BUTT,
}AUDIO_DEVID;

typedef enum _AUDIO_IO_TYPE{
	SELECT_TYPE_AI,
	SELECT_TYPE_AO
}AUDIO_IO_TYPE;

typedef enum _AUDIO_SAMPLE_RATE_E
{
	AUDIO_SAMPLE_RATE_8000   = 8000,    	/* 8K samplerate*/
	AUDIO_SAMPLE_RATE_12000  = 12000,   	/* 12K samplerate*/
	AUDIO_SAMPLE_RATE_11025  = 11025,   	/* 11.025K samplerate*/
	AUDIO_SAMPLE_RATE_16000  = 16000,   	/* 16K samplerate*/
	AUDIO_SAMPLE_RATE_22050  = 22050,  	/* 22.050K samplerate*/
	AUDIO_SAMPLE_RATE_24000  = 24000,  	/* 24K samplerate*/
	AUDIO_SAMPLE_RATE_32000  = 32000,   	/* 32K samplerate*/
	AUDIO_SAMPLE_RATE_44100  = 44100,   	/* 44.1K samplerate*/
	AUDIO_SAMPLE_RATE_48000  = 48000,   	/* 48K samplerate*/
	AUDIO_SAMPLE_RATE_96000  = 96000,   	/* 96K samplerate*/
	AUDIO_SAMPLE_RATE_192000  = 192000,   	/* 192K samplerate*/
	AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E;


typedef enum volume_type
{
	VOLUME_TYPE_MIC,			//!< 内置codec MIC增益(仅MIC输入有效)
	VOLUME_TYPE_ANA,			//!< 内置codec 模拟增益(MIC/LINE IN输入/输出有效)
	VOLUME_TYPE_DIG,			//!< 内置codec 数字增益(MIC/LINE IN输入/输出有效)
	VOLUME_TYPE_VQE,			//!< VQE 模拟音量(仅 AUDIO_ALGO_MPP_MIC1 模式有效)
	VOLUME_TYPE_MAX
}VOLUME_TYPE_E;

typedef enum _AUDIO_TRACK_MODE_E
{
	AUDIO_TRACK_LEFT	= 0,
	AUDIO_TRACK_RIGHT	= 1,
	AUDIO_TRACK_MIX		= 2,
	AUDIO_TRACK_EXCHANGE	= 3,
	AUDIO_TRACK_LEFT_MUTE	= 4,
	AUDIO_TRACK_RIGHT_MUTE	= 5,
	AUDIO_TRACK_BOTH_MUTE	= 6,
	AUDIO_TRACK_BUTT
} AUDIO_TRACK_MODE_E;

typedef enum _AUDIO_RTX_MODE_E{
	I2S_RTX_IDLE = 0,
	I2S_RXF_MODE,
	I2S_TXF_MODE,
	I2S_RTX_MODE
}AUDIO_RTX_MODE_E;


typedef enum _AUDIO_BIT_WIDTH_E
{
	AUDIO_BIT_WIDTH_8	= 8,		/* 8bit width */
	AUDIO_BIT_WIDTH_16	= 16,		/* 16bit width*/
	AUDIO_BIT_WIDTH_20	= 20,		/* 20bit width*/
	AUDIO_BIT_WIDTH_24	= 24,		/* 24bit width*/
	AUDIO_BIT_WIDTH_32	= 32,		/* 32bit width*/
	AUDIO_BIT_WIDTH_BUTT,
} AUDIO_BIT_WIDTH_E;

typedef enum _AUDIO_CHN_NUM_E{
	AUDIO_CHN_NUM_1		= 0x1,
	AUDIO_CHN_NUM_2		= 0x2,
	AUDIO_CHN_NUM_4		= 0x4,
	AUDIO_CHN_NUM_8		= 0x8,
	AUDIO_CHN_NUM_16	= 0x10,
	AUDIO_CHN_NUM_20	= 0x14,
	AUDIO_CHN_NUM_32	= 0x20,
	AUDIO_CHN_NUM_64	= 0x40,
	AUDIO_CHN_NUM_96	= 0x60,
	AUDIO_CHN_NUM_128	= 0x80
}AUDIO_CHN_NUM_E;

typedef enum _AIO_MODE_E
{
	AIO_MODE_I2S_MASTER  = 0,		/* AIO I2S master mode */
	AIO_MODE_I2S_SLAVE,			/* AIO I2S slave mode */
	AIO_MODE_PCM_SLAVE_STD,			/* AIO PCM slave standard mode (not surpport)*/
	AIO_MODE_PCM_SLAVE_NSTD,		/* AIO PCM slave non-standard mode (not surpport)*/
	AIO_MODE_PCM_MASTER_STD,		/* AIO PCM master standard mode (not surpport)*/
	AIO_MODE_PCM_MASTER_NSTD,		/* AIO PCM master non-standard mode (not surpport)*/
	AIO_MODE_BUTT
} AIO_MODE_E;

typedef enum _AIO_SOUND_MODE_E
{
	AUDIO_SOUND_MODE_MONO   =0,		/*mono*/
	AUDIO_SOUND_MODE_STEREO =1,		/*stereo*/
	AUDIO_SOUND_MODE_BUTT
} AUDIO_SOUND_MODE_E;

enum i2s_mode{
	default_mode            = 0,
	mode_i2s1_master        = 1,		/* AIO I2S1 master mode */
	mode_adc1_master         = 2,		/* AIO AD1 master mode */
	mode_i2s1_i2s5_master   = 3,		/* AIO I2S1&5 master mode */
	mode_adc1_adc5_master     = 4		/* AIO AD1&5 master mode */
};

	
/*******************cmd**************************/

typedef enum{
	IOC_SET_PUBATTR,
	IOC_GET_PUBATTR,
	IOC_QUERY_CHN_STAT,
	IOC_CMD_BUTT
}IOC_I2S_CMD;

typedef enum{
	AC_INIT_CFG=IOC_CMD_BUTT+1,		/*  wrapper init cfg */
	AC_AI_CFG_CHN,				/*  AI config channel parameter */
	AC_AI_EN,				/*  enable AI  */
	AC_AI_DISABLE,				/*  disable AI  */
	AC_AI_PAUSE,				/*  pause AI  */
	AC_AI_RESUME,				/*  resume AI  */
	AC_AI_SET_VOL,				/*  AI set volume  */
	AC_AI_SET_DIG_VOL,			/*  AI set volume  */
	AC_AI_MICIN_SET_VOL,			/*  mic set volume  */
	AC_AI_SET_TRACK,			/*  AI set track mode  */
	AC_AI_GET_TRACK,			/*  AI get track mode  */
	AC_USING_EXTERNAL_CODEC,		/*  external api(reserved)  */
	AC_AO_CFG_CHN,				/*  AO config channel parameter */
	AC_AO_EN,				/*  enable AO  */
	AC_AO_DISABLE,				/*  disable AO  */
	AC_AO_PAUSE,				/*  pause AO  */
	AC_AO_RESUME,				/*  resume AO  */
	AC_AO_SET_VOL,				/*  AO set ANA volume  */
	AC_AO_SET_DIG_VOL,			/*  AO set digital volume  */
	AC_AO_SET_TRACK,			/*  AO set track mode  */
	AC_AO_GET_TRACK,			/*  AO get track mode  */
	AC_WORK_MODE,				/*  set workmode  */
	AC_EXT_INTF,				/*  reserved  */
	AC_GET_VERSION,				/*  codec get version  */
	AC_HPF_CTRL,				/*  high pass filter contrl  */
	AC_ALC_CTRL,				/*  automatic level control  */
	AC_NOTCE_CTRL,				/*  notch filter contrl  */
	AC_NOISE_GATE,				/*  noise gate contrl  */
	AC_DAC_LIMITER,				/*  dac limiter contrl  */
	AC_MUTE,				/*  codec output mute  */
	AC_IMCLK_CTRL,				/*  mclk contrl  */
	AC_EQ_CTRL,				/*  equalizer contrl  */
	AC_POWER_ON,				/*  codec power on  */
	AC_READ_SELECT,
	AC_READ_PTR_SYNC,
	AC_WRITE_SELECT,
	AC_WRITE_PTR_SYNC,
    AC_AI_CHN_NUM,
        AC_AI_GET_VOL,				/*  AI get volume  */
	AC_AI_GET_DIG_VOL,			/*  AI get volume  */
	AC_AI_MICIN_GET_VOL,		/*  mic get volume  */
	AC_AO_GET_VOL,				/*  AO get ANA volume  */
	AC_AO_GET_DIG_VOL,			/*  AO get digital volume  */
	AC_CMD_BUTT
}IOC_ACW_CMD;

/**************************************/
typedef struct _AUDIO_CHN_PARAM_S{
	FY_U32			u32FrmNum;
	FY_U32			u32FrmSize;
}AUDIO_CHN_PARAM_S;


/*****The carrier of information interaction between mpi layer and app for every frame************/
struct FRAME_STATE_INFO {
	FY_U64 seq_num;
	FY_U64 pts_time;
};
typedef struct _AUDIO_FRAME_S
{
	FY_VOID*			pVirAddr;		//!< 数据虚拟地址
	fy_paddr_t			u32PhyAddr;		//!< 数据物理地址
	FY_U32			u32Seq;			//!< 帧序号
	FY_U32			u32Len;			//!< 帧长度
	FY_U32			u32TimeOut;		//!< 超时时间
	FY_U64			u64TimeStamp;		//!< 时间戳
	AUDIO_SAMPLE_RATE_E		enFsRate;		//!< 采样率
	AUDIO_BIT_WIDTH_E		enBitwidth;		//!< 采样精度
	AUDIO_SOUND_MODE_E		enSoundmode;		//!< 声道模式
	fy_paddr_t			u32ExpID;		//!< 扩展参数
} AUDIO_FRAME_S;



enum i2s_dump_direct{
	i2s_dump_no = 0,
	i2s_dump_tx,
	i2s_dump_rx,
	i2s_dump_all,
};

enum i2s_time_analyse{
	i2s_time_no,
	i2s_time_tx,
	i2s_time_rx,
};

typedef struct _TIME_RECORD_LINE_S
{
	union {
		FY_U64 line_idx[0];
		struct {
			FY_U64 seqnum;
			FY_U64 select;
			FY_U64 getswptr;
			FY_U64 updateswptr;
			FY_U64 dmadone;
		}s;
	}line_s;
} TIME_RECORD_LINE;

/*****The attribute for this auido device************/
typedef struct _AIO_ATTR_S{
	FY_BOOL			enable;
	AUDIO_SAMPLE_RATE_E		fs_rate;		/* 8K:8000  16K:16000 32K:32000 48K:48000 */
	AUDIO_CHN_NUM_E			chn_num;		/* 2,4,8,16,20 channel */
	AUDIO_BIT_WIDTH_E		chn_width;		/* 8/16/32 bits */
	AIO_MODE_E			is_slave;		/* 0: Master  1: Slave */
	AUDIO_RTX_MODE_E		rxt_mode;		/* 1:rx 2:tx */
	AUDIO_SOUND_MODE_E		data_format;		/* 0:mono  1:stereo */
	AUDIO_IO_TYPE			io_type;		/* io type */
	FY_U32			u32DmaNum;		/* dma buffer num,default:3 */
	FY_U32			u32DmaSize;		/* single dma buffer size */
	FY_U64			u32Mclk;		/* only for i2s,default: 12000000*/
	enum i2s_mode     cascade_mode;

	//the below used for mpi
	fy_paddr_t	u32PhyAddr;
	FY_VOID*			pVirAddr;
	//capture data
	enum i2s_dump_direct dump_direct;
	fy_paddr_t      rx_dump_PhyAddr;
	FY_VOID*        rx_dump_pVirAddr;
	fy_paddr_t      tx_dump_PhyAddr;
	FY_VOID*        tx_dump_pVirAddr;

	//the below used to collect timestamp
	enum i2s_time_analyse time_analy_direc;
	fy_paddr_t      rx_time_PhyAddr;
	FY_VOID*        rx_time_pVirAddr;
	fy_paddr_t      tx_time_PhyAddr;
	FY_VOID*        tx_time_pVirAddr;
	FY_U32 total_size;
	FY_U32 cur_seq_num;
	
}AIO_ATTR_S;


typedef struct
{
	FY_S32			s32Chn;
	FY_U32			u32Vol;
	VOLUME_TYPE_E			stType;
}VOLUME_PARAM_S;


typedef struct _thread_param
{
	unsigned int uDevID;
	unsigned int uChnID;
	unsigned int uFrmSize;
    unsigned int chn_num;
	volatile FY_BOOL 		bThrdStart;
}thread_param;


/*callback defined for every frame*/
typedef struct _callback_param
{
	AUDIO_FRAME_S *pstFrm;
	FY_S32	buf_ptr;
	FY_U32  buf_len;
	void * priv;
}callback_param;

typedef void (*frame_callback)(callback_param*);

#endif
