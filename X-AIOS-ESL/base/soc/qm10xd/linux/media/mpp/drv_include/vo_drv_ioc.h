#ifndef __FYVO_H__
#define __FYVO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "fh_vo_mpipara.h"
 #include "fh_vb_mpipara.h"
#include "drv_ioc_extension.h"
#define VO_QUERY_CHNS_INFO_MAX_CNT	3	//one vppu group max bind 3 path to vou chn
#define VPSS_LUMA_STAT_NUM		64

typedef struct {
	FY_U32				in_enMod;
	VO_LAYER			in_voLayer;
	VO_CHN				in_voChn;
	int				in_custom_group;
	int				in_custom_path;
	FY_S32				out_s32CntFree;
} VO_QCHN_ITEM_S;

typedef struct {
	FY_S32				s32Num;		//query chn cnt
	VO_QCHN_ITEM_S			vo_chn_info[VO_QUERY_CHNS_INFO_MAX_CNT];
} VO_QUERY_CHNS_INFO_S;

typedef struct fhVOU_EXPORT_FUNC_S
{
    FY_S32 (*pfnVouChnFrameSmartGet)(VB_UID_E enMod, VO_LAYER voLayer, VO_CHN voChn, VIDEO_FRAME_INFO_S * pstFrame, FY_S32 s32MilliSec);
    FY_S32 (*pfnVouChnFrameGet)(VB_UID_E enMod, VO_LAYER voLayer, VO_CHN voChn, VIDEO_FRAME_INFO_S * pstFrame);
    FY_S32 (*pfnVouChnFramePut)(VB_UID_E enMod, VO_LAYER voLayer, VO_CHN voChn, VIDEO_FRAME_INFO_S * pstFrame);
    FY_S32 (*pfnVouGuiOnOff)(VO_DEV voDev, GRAPHIC_LAYER voLayer, FY_S32 bEnable);
    FY_S32 (*pfnVouGuiShowHide)(VO_DEV voDev, GRAPHIC_LAYER voLayer, FY_S32 show);
    FY_S32 (*pfnVouGuiSetPos)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const POINT_S* pstPos);
    FY_S32 (*pfnVouGuiSetAlpha)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const ALPHA_S *pstAlpha);
    FY_S32 (*pfnVouGuiUpdate)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const VIDEO_FRAME_INFO_S *pstFrame, FY_S32 s32MilliSec);
    FY_S32 (*pfnVouGuiSetSize)(VO_DEV voDev, GRAPHIC_LAYER voLayer, const FH_SIZE* pstSize);
    FY_S32 (*pfnVouGuiCheckSwap)(VO_DEV voDev, GRAPHIC_LAYER voLayer, FY_BOOL *pbSwap);
    FY_S32 (*pfnVouQueryChnInfo)(VO_QUERY_CHNS_INFO_S *voinfoquery);
} VOU_EXPORT_FUNC_S;

/*************************** VO Driver IO Command Definition ****************************/

#define IOC_TYPE_VOU                   'O'

#define VOIO_BIND_DEV_LAYER             _IOW(IOC_TYPE_VOU,     0,   FY_S32)
#define VOIO_QUERY_CAPABILITY           _IOR(IOC_TYPE_VOU,     1,   VO_CAP_ATTR_S)
#define VOIO_ENABLE                     _IO(IOC_TYPE_VOU,      2)
#define VOIO_DISABLE                    _IO(IOC_TYPE_VOU,      3)
#define VOIO_SET_PUB_ATTR               _IOW(IOC_TYPE_VOU,     4,   VO_PUB_ATTR_S)
#define VOIO_GET_PUB_ATTR               _IOR(IOC_TYPE_VOU,     5,   VO_PUB_ATTR_S)

#define VOIO_SET_DEV_FRAME_RATE         _IOW(IOC_TYPE_VOU,     6,   FY_S32)
#define VOIO_GET_DEV_FRAME_RATE         _IOR(IOC_TYPE_VOU,     7,   FY_S32)

#define VOIO_SET_VTTH                   _IOW(IOC_TYPE_VOU,     8,   FY_S32)
#define VOIO_GET_VTTH                   _IOR(IOC_TYPE_VOU,     9,   FY_S32)

#define VOIO_ENALBE_RECV_FR_MATCH       _IO(IOC_TYPE_VOU,      10)
#define VOIO_DISALBE_RECV_FR_MATCH      _IO(IOC_TYPE_VOU,      11)

#define VOIO_SET_WBC_SOURCE             _IOW(IOC_TYPE_VOU,     12,  VO_WBC_SOURCE_S)
#define VOIO_GET_WBC_SOURCE             _IOR(IOC_TYPE_VOU,     13,  VO_WBC_SOURCE_S)
#define VOIO_ENABLE_WBC                 _IO(IOC_TYPE_VOU,      14)
#define VOIO_DISABLE_WBC                _IO(IOC_TYPE_VOU,      15)
#define VOIO_SET_WBC_ATTR               _IOW(IOC_TYPE_VOU,     16,  VO_WBC_ATTR_S)
#define VOIO_GET_WBC_ATTR               _IOR(IOC_TYPE_VOU,     17,  VO_WBC_ATTR_S)
#define VOIO_SET_WBC_MODE               _IOW(IOC_TYPE_VOU,     18,  VO_WBC_MODE_E)
#define VOIO_GET_WBC_MODE               _IOR(IOC_TYPE_VOU,     19,  VO_WBC_MODE_E)
#define VOIO_SET_WBC_DEPTH              _IOW(IOC_TYPE_VOU,     20,  FY_S32)
#define VOIO_GET_WBC_DEPTH              _IOR(IOC_TYPE_VOU,     21,  FY_S32)
#define VOIO_GET_WBC_FRAME              _IOWR(IOC_TYPE_VOU,    22,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_RELEASE_WBC_FRAME          _IOW(IOC_TYPE_VOU,     23,  VIDEO_FRAME_INFO_S)

#define VOIO_ENABLE_VIDEO_LAYER         _IO(IOC_TYPE_VOU,      24)
#define VOIO_DISABLE_VIDEO_LAYER        _IO(IOC_TYPE_VOU,      25)
#define VOIO_SET_VIDEO_LAYER_ATTR       _IOW(IOC_TYPE_VOU,     26,  VO_VIDEO_LAYER_ATTR_S)
#define VOIO_GET_VIDEO_LAYER_ATTR       _IOR(IOC_TYPE_VOU,     27,  VO_VIDEO_LAYER_ATTR_S)
#define VOIO_SET_COMPRESS_ATTR          _IOW(IOC_TYPE_VOU,     28,  VO_COMPRESS_ATTR_S)
#define VOIO_GET_COMPRESS_ATTR          _IOR(IOC_TYPE_VOU,     29,  VO_COMPRESS_ATTR_S)
#define VOIO_BIND_VIDEO_LAYER           _IO(IOC_TYPE_VOU,      30)
#define VOIO_UNBIND_VIDEO_LAYER         _IO(IOC_TYPE_VOU,      31)
#define VOIO_SET_PRIORITY               _IOW(IOC_TYPE_VOU,     32,  FY_S32)
#define VOIO_GET_PRIORITY               _IOR(IOC_TYPE_VOU,     33,  FY_S32)
#define VOIO_SET_CSC                    _IOW(IOC_TYPE_VOU,     34,  VO_CSC_S)
#define VOIO_GET_CSC                    _IOR(IOC_TYPE_VOU,     35,  VO_CSC_S)
#define VOIO_SET_DISP_BUFLEN            _IOW(IOC_TYPE_VOU,     36,  FY_S32)
#define VOIO_GET_DISP_BUFLEN            _IOR(IOC_TYPE_VOU,     37,  FY_S32)
#define VOIO_SET_PALY_TOLERATION        _IOW(IOC_TYPE_VOU,     38,  FY_S32)
#define VOIO_GET_PALY_TOLERATION        _IOR(IOC_TYPE_VOU,     39,  FY_S32)
#define VOIO_GET_SCREEN_FRAME           _IOWR(IOC_TYPE_VOU,    40,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_RELEASE_SCREEN_FRAME       _IOW(IOC_TYPE_VOU,     41,  VIDEO_FRAME_INFO_S)
#define VOIO_SET_PART_MODE              _IOW(IOC_TYPE_VOU,     42,  VO_PART_MODE_E)
#define VOIO_GET_PART_MODE              _IOR(IOC_TYPE_VOU,     43,  VO_PART_MODE_E)
#define VOIO_SET_ATTR_BEGIN             _IO(IOC_TYPE_VOU,      44)
#define VOIO_SET_ATTR_END               _IO(IOC_TYPE_VOU,      45)

#define VOIO_ENABLE_CHN                 _IO(IOC_TYPE_VOU,      48)
#define VOIO_DISABLE_CHN                _IO(IOC_TYPE_VOU,      49)
#define VOIO_SET_CHN_ATTR               _IOW(IOC_TYPE_VOU,     50,  VO_CHN_ATTR_S)
#define VOIO_GET_CHN_ATTR               _IOR(IOC_TYPE_VOU,     51,  VO_CHN_ATTR_S)
#define VOIO_SET_CHN_PARAM              _IOW(IOC_TYPE_VOU,     52,  VO_CHN_PARAM_S)
#define VOIO_GET_CHN_PARAM              _IOR(IOC_TYPE_VOU,     53,  VO_CHN_PARAM_S)
#define VOIO_SET_CHN_DISP_POS           _IOW(IOC_TYPE_VOU,     54,  POINT_S)
#define VOIO_GET_CHN_DISP_POS           _IOR(IOC_TYPE_VOU,     55,  POINT_S)
#define VOIO_SET_CHN_FIELD              _IOW(IOC_TYPE_VOU,     56,  VO_DISPLAY_FIELD_E)
#define VOIO_GET_CHN_FIELD              _IOR(IOC_TYPE_VOU,     57,  VO_DISPLAY_FIELD_E)
#define VOIO_SEND_FRAME                 _IOW(IOC_TYPE_VOU,     58,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_GET_CHN_FRAME              _IOWR(IOC_TYPE_VOU,    59,  VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_RELEASE_CHN_FRAME          _IOW(IOC_TYPE_VOU,     60,  VIDEO_FRAME_INFO_S)
#define VOIO_GET_CHN_FRAME_RATE         _IOR(IOC_TYPE_VOU,     61,  FY_S32)
#define VOIO_SET_CHN_FRAME_RATE         _IOW(IOC_TYPE_VOU,     62,  FY_S32)
#define VOIO_PAUSE_CHN                  _IO(IOC_TYPE_VOU,      63)
#define VOIO_RESUME_CHN                 _IO(IOC_TYPE_VOU,      64)
#define VOIO_STEP_CHN                   _IO(IOC_TYPE_VOU,      65)
#define VOIO_REFRESH_CHN                _IO(IOC_TYPE_VOU,      66)
#define VOIO_SET_ZOOMIN_WINDOW          _IOW(IOC_TYPE_VOU,     67,  VO_ZOOM_ATTR_S)
#define VOIO_GET_ZOOMIN_WINDOW          _IOR(IOC_TYPE_VOU,     68,  VO_ZOOM_ATTR_S)
#define VOIO_SHOW_CHN                   _IO(IOC_TYPE_VOU,      69)
#define VOIO_HIDE_CHN                   _IO(IOC_TYPE_VOU,      70)
#define VOIO_GET_CHN_PTS                _IOR(IOC_TYPE_VOU,     71,  FY_U64)
#define VOIO_QUERY_CHN_STAT             _IOR(IOC_TYPE_VOU,     72,  VO_QUERY_STATUS_S)
#define VOIO_CLEAR_CHN_BUFFER           _IOW(IOC_TYPE_VOU,     73,  FY_BOOL)
#define VOIO_SET_CHN_BORDER             _IOW(IOC_TYPE_VOU,     74,  VO_BORDER_S)
#define VOIO_GET_CHN_BORDER             _IOR(IOC_TYPE_VOU,     75,  VO_BORDER_S)


#define VOIO_SET_CHN_RECEIVE_THRESHOLD  _IOW(IOC_TYPE_VOU,     80,  FY_S32)
#define VOIO_GET_CHN_RECEIVE_THRESHOLD  _IOR(IOC_TYPE_VOU,     81,  FY_S32)
#define VOIO_GET_CHN_RG_LUMA            _IOWR(IOC_TYPE_VOU,    82,  VOIO_REGION_INFO_S)
#define VOIO_SET_CASCADE_ATTR           _IOW(IOC_TYPE_VOU,     83,  VO_CAS_ATTR_S)
#define VOIO_GET_CASCADE_ATTR           _IOR(IOC_TYPE_VOU,     84,  VO_CAS_ATTR_S)
#define VOIO_ENALBE_CASCADE_DEV         _IO(IOC_TYPE_VOU,      85)
#define VOIO_DISALBE_CASCADE_DEV        _IO(IOC_TYPE_VOU,      86)
#define VOIO_SET_CASCADE_PATTERN        _IOW(IOC_TYPE_VOU,     87,  FY_S32)
#define VOIO_GET_CASCADE_PATTERN        _IOR(IOC_TYPE_VOU,     88,  FY_S32)
#define VOIO_CASCADE_POS_BIND_CHN       _IOW(IOC_TYPE_VOU,     89,  FY_S32)
#define VOIO_CASCADE_POS_UNBIND_CHN     _IOW(IOC_TYPE_VOU,     90,  FY_S32)
#define VOIO_ENABLE_CASCADE             _IO(IOC_TYPE_VOU,      91)
#define VOIO_DISABLE_CASCADE            _IO(IOC_TYPE_VOU,      92)
#define VOIO_BIND_GRAPHIC_LAYER         _IOW(IOC_TYPE_VOU,     93,  FY_S32)
#define VOIO_UNBIND_GRAPHIC_LAYER       _IOW(IOC_TYPE_VOU,     94,  FY_S32)
#define VOIO_SET_GRAPHIC_LAYER_CSC      _IOW(IOC_TYPE_VOU,     95,  VO_CSC_S)
#define VOIO_GET_GRAPHIC_LAYER_CSC      _IOR(IOC_TYPE_VOU,     96,  VO_CSC_S)
#define VOIO_SET_VGA_PARAM              _IOW(IOC_TYPE_VOU,     97,  VO_VGA_PARAM_S)
#define VOIO_GET_VGA_PARAM              _IOR(IOC_TYPE_VOU,     98,  VO_VGA_PARAM_S)
#define VOIO_SET_HDMI_PARAM             _IOW(IOC_TYPE_VOU,     99,  VO_HDMI_PARAM_S)
#define VOIO_GET_HDMI_PARAM             _IOR(IOC_TYPE_VOU,     100, VO_HDMI_PARAM_S)
#define VOIO_QUERY_DEV_INTF_STATUS      _IOWR(IOC_TYPE_VOU,    101, VOIO_DEVINTF_STATUS_S)

#define VOIO_SET_CHN_CM                 _IOR(IOC_TYPE_VOU,     102, VO_CM_S)
#define VOIO_GET_CHN_CM                 _IOW(IOC_TYPE_VOU,     103, VO_CM_S)

#define VOIO_GFX_ENABLE                 _IO(IOC_TYPE_VOU,      104)
#define VOIO_GFX_DISABLE                _IO(IOC_TYPE_VOU,      105)
#define VOIO_GFX_SET_POS                _IOW(IOC_TYPE_VOU,     106, POINT_S)
#define VOIO_GFX_SET_ALPHA              _IOW(IOC_TYPE_VOU,     107, ALPHA_S)
#define VOIO_GFX_SHOW                   _IO(IOC_TYPE_VOU,      108)
#define VOIO_GFX_HIDE                   _IO(IOC_TYPE_VOU,      109)
#define VOIO_GFX_UPDATE                 _IOW(IOC_TYPE_VOU,     110, VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_GFX_SET_SIZE               _IOW(IOC_TYPE_VOU,     111, FH_SIZE)
#define VOIO_SET_HDMI_AUDIO             _IOW(IOC_TYPE_VOU,     112, VO_HDMI_AUDIO_S)
#define VOIO_GET_HDMI_AUDIO             _IOR(IOC_TYPE_VOU,     113, VO_HDMI_AUDIO_S)

#define VOIO_GET_GFX_FRAME              _IOWR(IOC_TYPE_VOU,    114, VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_PUT_GFX_FRAME              _IOW(IOC_TYPE_VOU,     115, VIDEO_FRAME_INFO_S)

#define VOIO_RESET_HW_ONCE              _IO(IOC_TYPE_VOU,      116)
#define VOIO_RESET_HW                   _IO(IOC_TYPE_VOU,      117)

#define VOIO_FILL_CHN                   _IOW(IOC_TYPE_VOU,     118, FY_U32)

#define VOIO_GET_AUTO_BUFFER            _IOWR(IOC_TYPE_VOU,    119, VOIO_VIDEO_FRAME_INFO_S)
#define VOIO_PUT_AUTO_BUFFER            _IOW(IOC_TYPE_VOU,     120, VIDEO_FRAME_INFO_S)

#define VOIO_TEST_MODE                  _IOWR(IOC_TYPE_VOU,    121, VO_TEST_PARAM_S)

#define VOIO_ENABLE_HDMI                _IO(IOC_TYPE_VOU,      122)
#define VOIO_DISABLE_HDMI               _IO(IOC_TYPE_VOU,      123)

#define VOIO_SET_Y2R_MATRIX             _IOW(IOC_TYPE_VOU,     124, VO_MATRIX_S)
#define VOIO_GET_Y2R_MATRIX             _IOWR(IOC_TYPE_VOU,    125, VO_MATRIX_S)

#define VOIO_GET_HDMI_EDID              _IOW(IOC_TYPE_VOU,     126, VO_HDMI_EDID_S)

#define VOIO_SET_HDMI_POL               _IOW(IOC_TYPE_VOU,     127, VO_HDMI_POL_S)
#define VOIO_GET_HDMI_POL               _IOWR(IOC_TYPE_VOU,    128, VO_HDMI_POL_S)

#define VOIO_SUSPEND                    _IO(IOC_TYPE_VOU,      129)
#define VOIO_RESUME                     _IO(IOC_TYPE_VOU,      130)

#define VOIO_SET_HDMI_FMT               _IOW(IOC_TYPE_VOU,     131, VO_HDMI_FMT_S)
#define VOIO_GET_HDMI_FMT               _IOWR(IOC_TYPE_VOU,    132, VO_HDMI_FMT_S)

#define VOIO_GET_HDMI_SINK_CAP          _IOWR(IOC_TYPE_VOU,    133, VO_HDMI_SINK_S)

#define VOIO_MEM_GET_FRAME          	_IOWR(IOC_TYPE_VOU,    134, VOIO_VIDEO_FRAME_INFO_S)

#define VOIO_MEM_PUT_FRAME          	_IOWR(IOC_TYPE_VOU,    135, VOIO_VIDEO_FRAME_INFO_S)

#define VOIO_SET_ADDR_CHECK		_IOR(IOC_TYPE_VOU,     136, FH_UINT32)
#define VOIO_GET_CHN_BUFINFO		_IOR(IOC_TYPE_VOU,     137, FH_UINT32)
#define VOIO_SET_CHN_BUF		_IOWR(IOC_TYPE_VOU,    138, FH_MEM_INFO)

#define VOIO_GET_CHN_BUFNUM		_IOWR(IOC_TYPE_VOU,    139, FH_UINT32)
#define VOIO_SET_NODE_DEPTH		_IOW(IOC_TYPE_VOU,     140,  FY_S32)

#define VOIO_GET_DISP_SIZE               _IOR(IOC_TYPE_VOU,   141,  USER_SYNC_INFO_S)

#define VOIO_LCM_SUSPEND                 _IO(IOC_TYPE_VOU,    142)
#define VOIO_LCM_RESUME                  _IO(IOC_TYPE_VOU,    143)
#define VOIO_LCM_CLOSE                   _IO(IOC_TYPE_VOU,    144)
#define VOIO_LCM_REOPEN                  _IO(IOC_TYPE_VOU,    145)
#define VOIO_LCM_IOCTRL                  _IO(IOC_TYPE_VOU,    146)

#define VOIO_ADV_IOC                    FY_DRV_ADV_IOC(IOC_TYPE_VOU)

/****************************************************************************************/

#pragma pack (4)
typedef struct fyVOCMD_VIDEO_FRAME_INFO_S
{
    VIDEO_FRAME_INFO_S stFrame;
    FY_S32 s32MilliSec;
} VOIO_VIDEO_FRAME_INFO_S;
#pragma pack ()

typedef struct fyVOCMD_REGION_INFO_S
{
	FY_S32 s32MilliSec;
    FY_U32 u32LumaData[VPSS_LUMA_STAT_NUM];
    FY_U32 u32RegionNum;       						/*count of the region*/
	RECT_S stRegion[VPSS_LUMA_STAT_NUM];         	/*region attribute*/
} VOIO_REGION_INFO_S;

typedef struct fyVOCMD_DEVINTF_STATUS_S
{
    VO_INTF_TYPE_E      enDevInter;
    VO_DEVINTF_STATUS_S stStatus;
} VOIO_DEVINTF_STATUS_S;


#define VOIO_NAME_MAX_NUM		8

#define VOIO_DEV_MAX_NUM		4
#define VOIO_LAY_MAX_NUM		16

#define VOIO_VID_NUM_PER_DEV		4
#define VOIO_GFX_NUM_PER_DEV		4

typedef struct {
	VO_LAYER			voLayer;
	FY_CHAR				acName[VOIO_NAME_MAX_NUM];		// VHD0/VHD1/VPIP
	FY_S32				s32CntX;
	FY_S32				s32CntY;
} VOIO_CAP_VID_ATTR_S;

typedef struct {
	GRAPHIC_LAYER			voGfx;
	FY_CHAR				acName[VOIO_NAME_MAX_NUM];		// OSD0/HC0
} VOIO_CAP_GFX_ATTR_S;

typedef struct {
	VO_DEV				voDev;
	FY_U32				u32WidthMax;
	FY_U32				u32HeightMax;
	FY_BOOL				bNeedSwap;
	FY_BOOL				bHasWBC;

	FY_CHAR				acName[VOIO_NAME_MAX_NUM];		// DHD0/DHD1/DSD0

	FY_S32				s32VidNum;
	VOIO_CAP_VID_ATTR_S		stVideo[VOIO_VID_NUM_PER_DEV];

	FY_S32				s32GfxNum;
	VOIO_CAP_GFX_ATTR_S		stGraphic[VOIO_GFX_NUM_PER_DEV];
} VOIO_CAP_DEV_ATTR_S;


typedef struct {
	FY_S32				s32DevNum;
	VOIO_CAP_DEV_ATTR_S		stDevices[VOIO_DEV_MAX_NUM];

	FY_S32				s32VidNum;
	FY_S32				s32DevByVideo[VOIO_LAY_MAX_NUM];	// dev id of all layers

	FY_S32				s32GfxNum;
	FY_S32				s32DevByGfx[VOIO_LAY_MAX_NUM];		// dev id of all osds
} VO_CAP_ATTR_S;

typedef struct fyVOCMD_CHAN_ADDR_STATUS_S
{
	FY_U32				u32Dev;
	FY_U32 				u32Chan;
	FH_PHYADDR 			u64Addr;
	FY_U32 				u32Size;
	FY_U32	 			u32MilliSec;
	FY_U32				u32Isidle;
} VOIO_CHAN_ADDR_STATUS_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __FYVO_H__ */

