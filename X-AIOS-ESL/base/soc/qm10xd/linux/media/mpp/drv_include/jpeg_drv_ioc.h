#ifndef __JPEG_DRV_IO_H__
#define __JPEG_DRV_IO_H__

#include "fh_vdec_mpipara.h"
#include "fh_jpege_mpipara.h"
#include "drv_ioc_extension.h"

typedef struct
{
  FH_PHYADDR u32PhyAddr;
  FH_VOID* u32VirtAddr;
  FY_U32 u32BufSize;
  FY_U32 rsv;
}JPEGE_STRBUF_INFO_S;

typedef struct {
	FY_U32 Luma_Addr;//y data, phyaddr
	FY_U32 Chroma_Addr;//uv data, phyaddr
	FY_U32 JPG_Addr;//jpg data, phyaddr
	FY_U32 JPG_BufSize;
	FY_U16 Img_W;
	FY_U16 Img_H;
	FY_U16 ROI_X;
	FY_U16 ROI_Y;
	FY_U16 ROI_W;
	FY_U16 ROI_H;
	FY_U16 Stride;//Input Image Address Stride.
	FY_U8 Format;
	FY_U8 EncEn;
	FY_U8 DcmpEn;//TT_ecdc enable, encoder only.
	/*ByteMode: TT_ecdc compression ratio of a tile64x4, encoder only.
	00: 192byte 50%; 01: 224byte 60%; 02: 256byte 66%.
	In TT_ecdc disable, must be 00.*/
	FY_U8 EcsEn;
	FY_U16 NRST;//NRST = NMCU between two reset markers - 1
	FY_U32 Qfactor;//1--99
	FY_U8 ByteMode;
	/*ScanMode: 00: Block store; 10: CbCr; 11: CrCb.
	In decoder & TT_ecdc & block store, must be 00.*/
	FY_U8 ScanMode;
	/*RotateCfg: 00: 0; 01: 90; 10: 180; 11: 270.
	In decoder & TT_ecdc & block store, must be 00.*/
	FY_U8 RotateCfg;
	FY_U8 LumaQTbl[8][8];
	FY_U8 ChromaQTbl[8][8];
	FY_U32 timeout;//ms
	FY_U32 s32EncodeSize;//driver return encsize
}JPEGE_DMODE_PARA_S;

#define JPEGE_IOC_MAGIC      'J'

#define IOC_JPEGE_CREATE_CHAN                    _IOW(JPEGE_IOC_MAGIC,  0,  JPEGE_CHN_ATTR_S)
#define IOC_JPEGE_DESTROY_CHAN                   _IOW(JPEGE_IOC_MAGIC,  1,  FY_U32)
#define IOC_JPEGE_RESET_CHAN                     _IO(JPEGE_IOC_MAGIC,   2)
#define IOC_JPEGE_START_RECIEVE_PIC              _IOW(JPEGE_IOC_MAGIC,  3,  FY_U32)
#define IOC_JPEGE_START_RECIEVE_PIC_EX           _IOW(JPEGE_IOC_MAGIC,  4,  JPEGE_RECV_PIC_PARAM_S)
#define IOC_JPEGE_STOP_RECIEVE_PIC               _IOW(JPEGE_IOC_MAGIC,  5,  FY_U32)
#define IOC_JPEGE_QUERY                          _IOR(JPEGE_IOC_MAGIC,  6,  JPEGE_CHN_STAT_S)
#define IOC_JPEGE_SET_CHAN_ATTR                  _IOW(JPEGE_IOC_MAGIC,  7,  JPEGE_CHN_ATTR_S)
#define IOC_JPEGE_GET_CHAN_ATTR                  _IOWR(JPEGE_IOC_MAGIC, 8,  JPEGE_CHN_ATTR_S)
#define IOC_JPEGE_GET_STREAM                     _IOWR(JPEGE_IOC_MAGIC, 9,  JPEGE_STREAM_S)
#define IOC_JPEGE_RELEASE_STREAM                 _IOW(JPEGE_IOC_MAGIC,  10, JPEGE_STREAM_S)
#define IOC_JPEGE_SEND_FRAME                     _IOW(JPEGE_IOC_MAGIC,  11, VIDEO_FRAME_INFO_S)
#define IOC_JPEGE_SET_CHAN_PRIORITY              _IOWR(JPEGE_IOC_MAGIC, 12, FY_U32)
#define IOC_JPEGE_GET_CHAN_PRIORITY              _IOWR(JPEGE_IOC_MAGIC, 13, FY_U32)
#define IOC_JPEGE_SET_MAX_STREAM_CNT             _IOWR(JPEGE_IOC_MAGIC, 14, FY_U32)
#define IOC_JPEGE_GET_MAX_STREAM_CNT             _IOWR(JPEGE_IOC_MAGIC, 15, FY_U32)
#define IOC_JPEGE_GET_STREAM_BUF_INFO            _IOWR(JPEGE_IOC_MAGIC, 16, JPEGE_STRBUF_INFO_S)
#define IOC_JPEGE_SET_CHAN_PARAM                 _IOWR(JPEGE_IOC_MAGIC, 17, JPEGE_PARAM_S)
#define IOC_JPEGE_GET_CHAN_PARAM                 _IOWR(JPEGE_IOC_MAGIC, 18, JPEGE_PARAM_S)

#define IOC_JPEGE_BIND_CHAN                      _IOW(JPEGE_IOC_MAGIC,  20, FY_U32)
#define IOC_JPEGE_UNBIND_CHAN                    _IO(JPEGE_IOC_MAGIC,   21)

#define IOC_JPEGE_SET_ROTATE                     _IOW(JPEGE_IOC_MAGIC,  23, ROTATE_E)
#define IOC_JPEGE_GET_ROTATE                     _IOR(JPEGE_IOC_MAGIC,  24, ROTATE_E)
#define IOC_JPEGE_DIRECT_MODE                    _IOR(JPEGE_IOC_MAGIC,  25, JPEGE_DMODE_PARA_S)

#define IOC_JPEGE_SET_CHAN_CROP                  _IOWR(JPEGE_IOC_MAGIC, 26, JPEGE_CROP_CFG_S)
#define IOC_JPEGE_GET_CHAN_CROP                  _IOWR(JPEGE_IOC_MAGIC, 27, JPEGE_CROP_CFG_S)

#define IOC_JPEGE_SET_BUF_THRESHOLD              _IOWR(JPEGE_IOC_MAGIC, 28, FY_U32)
#define IOC_JPEGE_GET_BUF_THRESHOLD              _IOWR(JPEGE_IOC_MAGIC, 29, FY_U32)

#define IOC_JPEGE_GET_FRM_BUF_INFO               _IOWR(JPEGE_IOC_MAGIC, 30, FH_MEM_INFO)
#define IOC_JPEGE_REGISTER_FRM_BUF               _IOWR(JPEGE_IOC_MAGIC, 31, FH_MEM_INFO)
#define IOC_JPEGE_UNREGISTER_FRM_BUF             _IO(JPEGE_IOC_MAGIC, 32)

#define IOC_JPEGE_RELEASE_STREAM_EX              _IO(JPEGE_IOC_MAGIC, 33)
#define IOC_JPEGE_DIRECT_MODE2                   _IOR(JPEGE_IOC_MAGIC,  33, JPEGE_DMODE_PARA_S)

typedef struct jpegdiocsendstream_t
{
    VDEC_STREAM_S stream;
    FY_S32 s32MilliSec;
}JPEGD_IOC_SENDSTREAM_S;

typedef struct
{
    FY_S32 chan;
    JPEGD_IOC_SENDSTREAM_S stJpegdStream;
}JPEGD_SENDSTREAM_INFO_S;

typedef struct jpegdiocgetimage_t
{
    VIDEO_FRAME_INFO_S fameInfo;
    FY_S32 s32MilliSec;
}JPEGD_IOC_GETIMAGE_S;

typedef struct
{
    FY_S32 chan;
    JPEGD_IOC_GETIMAGE_S stJpegdGetImg;
}JPEGD_GETIMAGE_INFO_S;

typedef struct jpegiocoutbufinfo_t
{
	FY_U32 			size;
	FH_VOID 		*virAddr;
	FH_PHYADDR		u32PhyAddr;
}JPEGD_IOC_OUTBUFINFO_S;

#ifndef CONFIG_FH_SDK
typedef struct jpegdiocgetUsrData_t
{
    VDEC_USERDATA_S usrData;
    FY_S32 s32MilliSec;
}JPEGD_IOC_GETUSERDATA_S;
#endif

typedef struct _JPEGD_EXPORT_FUNC_S
{
    FY_VOID (*pfnReleaseImage) (MOD_ID_E enModId, FY_S32 s32DevId, FY_S32  s32ChnId, VIDEO_FRAME_INFO_S *pstFrameInfo);
} JPEGD_EXPORT_FUNC_S;

#define IOC_TYPE_JPEGD 'j'

#define IOC_JPEGD_BIND_CHAN             _IOW(IOC_TYPE_JPEGD,10,VDEC_CHN)
#define IOC_JPEGD_UNBIND_CHAN           _IO(IOC_TYPE_JPEGD,1)
#define IOC_JPEGD_CREATE_CHAN           _IOW(IOC_TYPE_JPEGD,0,VDEC_CHN_ATTR_S)
#define IOC_JPEGD_DESTROY_CHAN          _IO(IOC_TYPE_JPEGD,2)
#define IOC_JPEGD_GET_CHAN_ATTR         _IOR(IOC_TYPE_JPEGD,3,VDEC_CHN_ATTR_S)
#define IOC_JPEGD_START_RECV_STREAM     _IO(IOC_TYPE_JPEGD,7)
#define IOC_JPEGD_STOP_RECV_STREAM      _IO(IOC_TYPE_JPEGD,8)
#define IOC_JPEGD_QUERY                 _IOR(IOC_TYPE_JPEGD,6,VDEC_CHN_STAT_S)
#define IOC_JPEGD_RESET_CHAN            _IO(IOC_TYPE_JPEGD,9)
#define IOC_JPEGD_SET_CHAN_PARAM        _IOW(IOC_TYPE_JPEGD,13,VDEC_CHN_PARAM_S)
#define IOC_JPEGD_GET_CHAN_PARAM        _IOR(IOC_TYPE_JPEGD,14,VDEC_CHN_PARAM_S)
#ifndef CONFIG_FH_SDK
#define IOC_JPEGD_SET_PROTOCOL_PARAM    _IOW(IOC_TYPE_JPEGD,15,VDEC_PRTCL_PARAM_S)
#define IOC_JPEGD_GET_PROTOCOL_PARAM    _IOR(IOC_TYPE_JPEGD,16,VDEC_PRTCL_PARAM_S)
#endif
#define IOC_JPEGD_SEND_STREAM           _IOW(IOC_TYPE_JPEGD,4,JPEGD_SENDSTREAM_INFO_S)
#define IOC_JPEGD_GET_IMAGE             _IOWR(IOC_TYPE_JPEGD,20,JPEGD_GETIMAGE_INFO_S)
#define IOC_JPEGD_RELEASE_IMAGE         _IOW(IOC_TYPE_JPEGD,21, VIDEO_FRAME_INFO_S)
#ifndef CONFIG_FH_SDK
#define IOC_JPEGD_GET_USER_DATA         _IOWR(IOC_TYPE_JPEGD,17,JPEGD_IOC_GETUSERDATA_S)
#define IOC_JPEGD_RELEASE_USER_DATA     _IOW(IOC_TYPE_JPEGD,19,VDEC_USERDATA_S)
#endif
#define IOC_JPEGD_SET_ROTATE            _IOW(IOC_TYPE_JPEGD,29,ROTATE_E)
#define IOC_JPEGD_GET_ROTATE            _IOR(IOC_TYPE_JPEGD,30,ROTATE_E)
#define IOC_JPEGD_GET_CHAN_LUMA         _IOR(IOC_TYPE_JPEGD,31,VDEC_CHN_LUM_S)
#define IOC_JPEGD_SET_USER_PIC          _IOW(IOC_TYPE_JPEGD,26,VIDEO_FRAME_INFO_S)
#define IOC_JPEGD_ENABLE_USER_PIC       _IOW(IOC_TYPE_JPEGD,27, FY_BOOL)
#define IOC_JPEGD_DISABLE_USER_PIC      _IO(IOC_TYPE_JPEGD,28)
#define IOC_JPEGD_SET_DISPLAY_MODE      _IOW(IOC_TYPE_JPEGD,32,VIDEO_DISPLAY_MODE_E)
#define IOC_JPEGD_GET_DISPLAY_MODE      _IOR(IOC_TYPE_JPEGD,33,VIDEO_DISPLAY_MODE_E)
#define IOC_JPEGD_SET_CHAN_VBCNT        _IOW(IOC_TYPE_JPEGD,34,FY_U32)
#define IOC_JPEGD_GET_CHAN_VBCNT        _IOR(IOC_TYPE_JPEGD,35,FY_U32)
#define IOC_JPEGD_ATTACH_VBPOOL         _IOW(IOC_TYPE_JPEGD,36,VDEC_CHN_POOL_S)
#define IOC_JPEGD_DETACH_VBPOOL         _IO(IOC_TYPE_JPEGD,37)
#ifndef CONFIG_FH_SDK
#define IOC_JPEGD_SET_MOD_PARAM         _IOW(IOC_TYPE_JPEGD,38,VDEC_MOD_PARAM_S)
#define IOC_JPEGD_GET_MOD_PARAM         _IOR(IOC_TYPE_JPEGD,39,VDEC_MOD_PARAM_S)
#endif
#define IOC_JPEGD_SET_LOG_PARAM         _IOW(IOC_TYPE_JPEGD,40,FY_U32)
#define IOC_JPEGD_GET_FRM_BUF_INFO      _IOWR(IOC_TYPE_JPEGD, 41, VDEC_MEM_INFO_S)
#define IOC_JPEGD_REGISTER_FRM_BUF      _IOWR(IOC_TYPE_JPEGD, 42, FH_MEM_INFO)
#define IOC_JPEGD_UNREGISTER_FRM_BUF    _IO(IOC_TYPE_JPEGD, 43)
#define IOC_JPEGD_EXT                   FY_DRV_ADV_IOC(IOC_TYPE_JPEGD)

#define IOC_JPEGD_GET_OUTBUF_INFO       _IOR(IOC_TYPE_JPEGD,50,JPEGD_IOC_OUTBUFINFO_S)
#define IOC_JPEGD_SET_OUTBUF_VIR        _IOW(IOC_TYPE_JPEGD,51,JPEGD_IOC_OUTBUFINFO_S)

//The following are for fy01 and fy10 only
typedef struct _jpgd_buffer_t {
    unsigned int size; // input
    unsigned long phys_addr; //output
    unsigned long virt_addr; // should mmap at user space
    unsigned int  vbBlkHandle; //output
    int  poolid; // input
    char name[24]; //input
} jpgd_buffer_t;

typedef enum _jpegd_img_format_t {
	JPEGD_FMT_GRAY = 0,
	JPEGD_FMT_YUV,
	JPEGD_FMT_MAX,
}jpegd_img_format;

typedef struct DecParam_t {
	unsigned int JPG_Addr;//jpg data, phyaddr
	unsigned int JPG_Size;
	unsigned int Luma_Addr;//y data, phyaddr
	unsigned int Chroma_Addr;//uv data, phyaddr
	jpegd_img_format Format;
	unsigned int timeout;//ms
}DecParam_t;

#define IOC_JPEGD_ALLOCATE_PHYSICAL_MEMORY    _IOW(IOC_TYPE_JPEGD, 61, jpgd_buffer_t)
#define IOC_JPEGD_FREE_PHYSICALMEMORY         _IOW(IOC_TYPE_JPEGD, 62, jpgd_buffer_t)
#define IOC_JPEGD_GET_MOD_PARM                _IOR(IOC_TYPE_JPEGD, 63, FY_U32)
#define IOC_JPEGD_SET_MOD_PARM                _IOW(IOC_TYPE_JPEGD, 64, FY_U32)

#define IOC_JPEGD_DECODE_JPG                  _IOWR(IOC_TYPE_JPEGD, 65, DecParam_t)

#define IOC_JPEGD_SET_FRAMELOSS_ENABLE        _IOW(IOC_TYPE_JPEGD,  66, FH_BOOL)
#define IOC_JPEGD_SUSPEND                     _IO(IOC_TYPE_JPEGD,   67)
#define IOC_JPEGD_RESUME                      _IO(IOC_TYPE_JPEGD,   68)

#endif //__JPEG_DRV_IO_H__
