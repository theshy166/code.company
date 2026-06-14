
#ifndef _FH_COMM_DSP_H_
#define _FH_COMM_DSP_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include "fh_common.h"
#include "fh_errno.h"

/**********************************************************************
    PART I      - Macro
**********************************************************************/

typedef FH_SINT32 SVP_DSP_HANDLE;


/**********************************************************************
    PART II     - Structure and Enumeration
**********************************************************************/
typedef enum fhEN_SVP_DSP_ERR_CODE_E
{
    ERR_SVP_DSP_SYS_TIMEOUT    = 0x40,   /* SVP_DSP process timeout */
    ERR_SVP_DSP_QUERY_TIMEOUT  = 0x41,   /* SVP_DSP query timeout */
    ERR_SVP_DSP_OPEN_FILE      = 0x42,   /* SVP_DSP open file error*/
    ERR_SVP_DSP_READ_FILE      = 0x43,   /* SVP_DSP read file error*/

    ERR_SVP_DSP_BUTT
}EN_SVP_DSP_ERR_CODE_E;

/*SVP_DSP core id*/
typedef enum fhSVP_DSP_ID_E
{
    SVP_DSP_ID_0 = 0x0,

    SVP_DSP_ID_BUTT
}SVP_DSP_ID_E;

/*SVP_DSP memory type*/
typedef enum fhSVP_DSP_MEM_TYPE_E
{
    SVP_DSP_MEM_TYPE_SYS_DDR_DSP_0 = 0x0,
    SVP_DSP_MEM_TYPE_IRAM_DSP_0    = 0x1,
    SVP_DSP_MEM_TYPE_DRAM_0_DSP_0  = 0x2,
    SVP_DSP_MEM_TYPE_DRAM_1_DSP_0  = 0x3,

    SVP_DSP_MEM_TYPE_BUTT
}SVP_DSP_MEM_TYPE_E;

#if 0
typedef struct fhSVP_DSP_REQUEST_ENTITY_S {
	FH_UINT32 src;
	FH_UINT32 dst;
	FH_VOID *info;
}SVP_DSP_REQUEST_ENTITY_S;
#endif

typedef struct fhSVP_DSP_CFG_S {
	FH_FLOAT confid; // range[0,1]
}SVP_DSP_CFG_S;

typedef struct
{
    FH_UINT32   u32Width;           //输入图像数据的宽　| [ ]
    FH_UINT32   u32Height;          //输入图像数据的高　| [ ]
    FH_UINT32   u32Stride;          //输入图像的stride  | [ ]
    FH_MEM_INFO stSrcData[2];     //输入图像的地址信息　| [ ]
    FH_UINT32   u16Reserved;
}SVP_DSP_SRC_IMAGE_S;

typedef struct
{
    FH_MEM_INFO stDstData;       //输入图像的地址信息　| [ ]
}SVP_DSP_DST_IMAGE_S;

#if 1
/**********************************************************************
    PART III    - Error Code
**********************************************************************/
/*Invalid device ID*/
#define FH_ERR_SVP_DSP_INVALID_DEVID     FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/*Invalid channel ID*/
#define FH_ERR_SVP_DSP_INVALID_CHNID     FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/*At least one parameter is illegal. For example, an illegal enumeration value exists.*/
#define FH_ERR_SVP_DSP_ILLEGAL_PARAM     FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/*The channel exists.*/
#define FH_ERR_SVP_DSP_EXIST             FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
/*The UN exists.*/
#define FH_ERR_SVP_DSP_UNEXIST           FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/*A null point is used.*/
#define FH_ERR_SVP_DSP_NULL_PTR          FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/*Try to enable or initialize the system, device, or channel before configuring attributes.*/
#define FH_ERR_SVP_DSP_NOT_CONFIG        FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/*The operation is not supported currently.*/
#define FH_ERR_SVP_DSP_NOT_SURPPORT      FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/*The operation, changing static attributes for example, is not permitted.*/
#define FH_ERR_SVP_DSP_NOT_PERM          FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/*A failure caused by the malloc memory occurs.*/
#define FH_ERR_SVP_DSP_NOMEM             FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/*A failure caused by the malloc buffer occurs.*/
#define FH_ERR_SVP_DSP_NOBUF             FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/*The buffer is empty.*/
#define FH_ERR_SVP_DSP_BUF_EMPTY         FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/*No buffer is provided for storing new data.*/
#define FH_ERR_SVP_DSP_BUF_FULL          FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/*The system is not ready because it may be not initialized or loaded.
 *The error code is returned when a device file fails to be opened. */
#define FH_ERR_SVP_DSP_NOTREADY          FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
/*The source address or target address is incorrect during the operations such as calling copy_from_user or copy_to_user.*/
#define FH_ERR_SVP_DSP_BADADDR           FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR)
/*The resource is busy during the operations such as destroying a VENC channel without deregistering it.*/
#define FH_ERR_SVP_DSP_BUSY              FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
/*SVP_DSP process timeout*/
#define FH_ERR_SVP_DSP_SYS_TIMEOUT       FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, ERR_SVP_DSP_SYS_TIMEOUT)
/*SVP_DSP query timeout*/
#define FH_ERR_SVP_DSP_QUERY_TIMEOUT     FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, ERR_SVP_DSP_QUERY_TIMEOUT)
/*SVP_DSP open file error*/
#define FH_ERR_SVP_DSP_OPEN_FILE         FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, ERR_SVP_DSP_OPEN_FILE)
/*SVP_DSP read file error*/
#define FH_ERR_SVP_DSP_READ_FILE         FH_DEF_ERR(FH_OBJ_VDSP, EN_ERR_LEVEL_ERROR, ERR_SVP_DSP_READ_FILE)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif/*_FH_COMM_DSP_H_*/
