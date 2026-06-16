#ifndef __FY_DRV_AIO_H__
#define __FY_DRV_AIO_H__

#include "fh_common.h"
#include "fh_errno.h"

#include "fy_comm_aio.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define MPI_AUDIO_LIB_VERSION      "1.0.0"


/*how mach channel in this MPI device*/
#define MAX_AUDIO_AI_CHN_NUM  16
#define MAX_AUDIO_AO_CHN_NUM  16


/*how many buffers in this audio channel*/
#define MAX_CACHE_NUM				16


#define DEV_NUM					2 /*  build-in codec max number  */
#define	ACW_VERION_ID				0x0002

/*audio chn 1-128*/
#define CHN(x)	(x-1)

#define AIO_MIN(a,b)	(a)<(b)?(a):(b)

#define DEV_I2S_MAX 1

#define MPI_AIO_IOCTL(fd,cmd,arg)		fd>0?ioctl(fd, cmd, arg):FY_FAILURE
#define MPI_AIO_CHECK_EQ(A,B) \
do{\
	if(A!=B)\
	{\
	    	fprintf((FILE *)stderr, "!!!!Error: FUNCTION:%s() LINE[%d], A=%d,b=%d\n",__FUNCTION__, __LINE__,A,B);\
	    	return FY_FAILURE;\
	}\
}while(0)

/****************************enum defined*******************************************/
typedef enum _G726_BPS_E
{
	G726_16K = 0,				/* G726 16kbps, see RFC3551.txt  4.5.4 G726-16 */
	G726_24K,				/* G726 24kbps, see RFC3551.txt  4.5.4 G726-24 (not surpport)*/
	G726_32K,				/* G726 32kbps, see RFC3551.txt  4.5.4 G726-32 */
	G726_40K,				/* G726 40kbps, see RFC3551.txt  4.5.4 G726-40 (not surpport)*/
	MEDIA_G726_16K,				/* G726 16kbps for ASF ... (not surpport)*/
	MEDIA_G726_24K,				/* G726 24kbps for ASF ... (not surpport)*/
	MEDIA_G726_32K,				/* G726 32kbps for ASF ... (not surpport)*/
	MEDIA_G726_40K,				/* G726 40kbps for ASF ... (not surpport)*/
	G726_BUTT
} G726_BPS_E;

typedef enum fyADPCM_TYPE_E
{
    /* see DVI4 diiffers in three respects from the IMA ADPCM at RFC3551.txt 4.5.1 DVI4 */

    ADPCM_TYPE_DVI4 = 0,    /* 32kbps ADPCM(DVI4) for RTP */
    ADPCM_TYPE_IMA,         /* 32kbps ADPCM(IMA),NOTICE:point num must be 161/241/321/481 */
    ADPCM_TYPE_ORG_DVI4,
    ADPCM_TYPE_BUTT
} ADPCM_TYPE_E;

typedef enum fyAUDIO_TYPE_PARAM_E{
	I2S_CHN_CMD = 0,
	I2S_CLK_DIV
}AUDIO_TYPE_PARAM_E;

typedef enum{
	DMIC_GET_PARAM=AC_CMD_BUTT+1,
	DMIC_SET_PARAM,
	DMIC_DEV_ENABLE,
	DMIC_DEV_DISABLE,
	DMIC_QUERY_DATA,
	DMIC_SET_FRMSIZE,
	DMIC_GET_FRAME,
	DMIC_CMD_BUTT
}IOC_DMIC_CMD;



typedef enum _EN_AIO_ERR_CODE_E
{
	AIO_ERR_VQE_ERR		= 65 ,		/*vqe error*/
} EN_AIO_ERR_CODE_E;

typedef enum _AUDIO_ALGO_E
{
	AUDIO_ALGO_SES_MIC1	= 0,		//!< FY_SES 算法单MIC
	AUDIO_ALGO_SES_MICN,			//!< FY_SES 算法多MIC
	AUDIO_ALGO_MPP_MIC1,			//!< MPP 算法，仅支持单MIC
	AUDIO_ALGO_MOD_BUTT,
} AUDIO_ALGO_E;


	
/***********************************************************************/

// for compatibility with old code.

#pragma pack (4)
typedef struct _AIO_CHN_PARAM_S
{
	FY_S32			s32Dev;
	FY_S32			s32Chn;
	AUDIO_CHN_PARAM_S		stParam;
}AIO_CHN_PARAM_S;
#pragma pack ()


/************misc defined**********/


typedef struct
{
    FY_U32		u32LogicId;
    FY_U32		u32ActureID;
}AI_CHN_ID;




/*****The carrier of information interaction between mpi layer and driver************/

struct buf_info {
	//FY_U64 seq_num; no support 64bit div
	FY_U32 seq_num;
	FY_U32 buf_index;
	FY_U64 pts_time;
};

//for ao
typedef struct _frame_sync_info
{
	FY_S32			sw_ptr;
	FY_S32			data_len;
	FY_U32			timeout_ms;
	struct buf_info buf_info;
}FRAME_SYNC_INFO;

//for ai
typedef struct _CHN_HEADER
{
	unsigned long long Chn_Header_L;
	unsigned long long Chn_Header_H;
} CHN_HEADER_S;


typedef struct _multi_frame_sync_info{
	CHN_HEADER_S ChnHeader;
	FRAME_SYNC_INFO FrmSyncInfo;

}MULTI_FRAME_SYNC_INFO;

typedef struct _multi_frame_sync_size{
	CHN_HEADER_S ChnHeader;
	FY_U32 size;

}MULTI_FRAME_SYNC_SIZE;



/**
**		abstraction of audio ai/ao device 
**
**
**
**   AIO_MOD_S               MPI_DEV_ATTR
┌─────────────────┐    ┌─────────────┐ 
│ ┌──MPI_DEV───┬──┼───►┼───stDevAttr─┤ 
│ └────────────┘  │    │┌──────────┐ │ 
│ ┌──MPI_DEV───┐  │    │┌──────────┐ │ 
│ └────────────┘  │    │└──stChnAttr │ 
│ ┌──MPI_DEV───┐  ┼    └─────────────┘ 
│ └────────────┘  │                    
└─────────────────┘                    

**
**
**
**
**/
typedef struct _AUDIO_CHN_S
{
	volatile FY_BOOL		bEnabled;
	FY_VOID*			pVirAddr;
	fy_paddr_t			u32PhyAddr;
	FY_U32			u32FrmNum;
	FY_U32			u32FrmSize;
	FY_U64			u64Pts;
	FY_U32			u32Seq;
	FY_S32			reserved[15];
}AUDIO_CHN_S;

typedef struct
{
    AUDIO_CHN_S		stChnInfo[MAX_CACHE_NUM];/*this chnl info, main part: real-time data buffer address for this channel */
    FY_U32		u32Vol[VOLUME_TYPE_MAX];
    AI_CHN_ID       stChnId;
    FY_BOOL         bMute;
}AIO_CHN_ATTR;



typedef struct _SW_SPLIT_INFO
{
	int chn_offset;
	int all_chn_size;
	int chnlen;
	int chncnt;
	int framelen;
	int chnmod_buf_size;
	int skip_div;
}SW_SPLIT_INFO;




typedef struct _AIO_DEV_ATTR
{
	FY_S32			s32DevFd; /*opened device fd*/
	AIO_ATTR_S			setParam;/*current set params in this device*/
	SW_SPLIT_INFO       sw_split_info;/*used to optimize to split channel data*/
}AIO_DEV_ATTR;


typedef struct
{
	FY_BOOL			bEnable;
	FY_BOOL			bVmmMem;
	FY_S32			s32DevId;/*device id*/
	AIO_DEV_ATTR	stDevAttr; /*device attribute*/
	AIO_CHN_ATTR	stChnAttr[MAX_AUDIO_AI_CHN_NUM]; /*how mach channel in this device,and channel attribute*/
}MPI_DEV_ATTR;


typedef struct
{
	FY_U32		u32LogLvl;
	MPI_DEV_ATTR		DevAttr[DEV_AUDIO_BUTT];
}AIO_MOD_S; /*all audio in or out device list*/


/* System define error code */
/** 无效的设备 	ID */
#define FY_ERR_AI_INVALID_DEVID     FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/** 无效的通道 	ID */
#define FY_ERR_AI_INVALID_CHNID     FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/** 非法传入参数 */
#define FY_ERR_AI_ILLEGAL_PARAM     FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/** 参数指针为空 */
#define FY_ERR_AI_NULL_PTR          FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/** 参数未配置 */
#define FY_ERR_AI_NOT_CONFIG        FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/** 不支持的参数/接口 */
#define FY_ERR_AI_NOT_SUPPORT       FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/** 无效的操作 */
#define FY_ERR_AI_NOT_PERM          FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/** 设备/通道未使能 */
#define FY_ERR_AI_NOT_ENABLED       FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/** 内存申请失败 */
#define FY_ERR_AI_NOMEM             FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/** 无内存 */
#define FY_ERR_AI_NOBUF             FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/** 缓存为空 */
#define FY_ERR_AI_BUF_EMPTY         FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/** 缓存已满 */
#define FY_ERR_AI_BUF_FULL          FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/** 系统未初始化 */
#define FY_ERR_AI_SYS_NOTREADY      FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
/** 设备忙 */
#define FY_ERR_AI_BUSY              FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
/** VQE配置出错 */
#define FY_ERR_AI_VQE_ERR       FH_DEF_ERR(FH_ID_AI, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

/** 无效的设备 	ID */
#define FY_ERR_AO_INVALID_DEVID     FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/** 无效的通道 	ID */
#define FY_ERR_AO_INVALID_CHNID     FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/** 非法传入参数 */
#define FY_ERR_AO_ILLEGAL_PARAM     FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/** 参数指针为空 */
#define FY_ERR_AO_NULL_PTR          FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/** 参数未配置 */
#define FY_ERR_AO_NOT_CONFIG        FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/** 不支持的参数/接口 */
#define FY_ERR_AO_NOT_SUPPORT       FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/** 无效的操作 */
#define FY_ERR_AO_NOT_PERM          FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/** 设备/通道未使能 */
#define FY_ERR_AO_NOT_ENABLED       FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/** 内存申请失败 */
#define FY_ERR_AO_NOMEM             FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/** 无内存 */
#define FY_ERR_AO_NOBUF             FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/** 缓存为空 */
#define FY_ERR_AO_BUF_EMPTY         FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/** 缓存已满 */
#define FY_ERR_AO_BUF_FULL          FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/** 系统未初始化 */
#define FY_ERR_AO_SYS_NOTREADY      FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
/** 设备忙 */
#define FY_ERR_AO_BUSY              FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
/** VQE配置出错 */
#define FY_ERR_AO_VQE_ERR       FH_DEF_ERR(FH_ID_AO, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __FY_COMM_AIO_H__ */
