#ifndef __LCM_DRV_IOC_H__
#define __LCM_DRV_IOC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "fh_type.h"
#include "fh_lcm_mpipara.h"
#include "drv_ioc_extension.h"


typedef struct {
	LCM_SCREEN_ID_E				enScreenID;
	FY_VOID*					pstPrivate;
} LCM_PROBE_RESULT_S;

typedef struct {
	FY_S32 (*pfnLcmProbe)(LCM_PROBE_RESULT_S* pstResult);
	FY_S32 (*pfnLcmInit)(FY_VOID);
	FY_S32 (*pfnLcmDeinit)(FY_VOID);
	FY_S32 (*pfnLcmSuspend)(FY_VOID);
	FY_S32 (*pfnLcmResume)(FY_VOID);
} LCM_EXPORT_FUNC_S;


/*************************** LCM Driver IO Command Definition ***************************/

#define IOC_TYPE_LCM			'L'


#define LCM_IOC_CONFIG			_IOW(IOC_TYPE_LCM,	1,		LCM_ATTR_S)
#define LCM_IOC_ENABLE			_IO(IOC_TYPE_LCM,	2		)
#define LCM_IOC_DISABLE			_IO(IOC_TYPE_LCM,	3		)
#define LCM_IOC_SUSPEND			_IO(IOC_TYPE_LCM,	4		)
#define LCM_IOC_RESUME			_IO(IOC_TYPE_LCM,	5		)

#define LCM_IOC_DSI_SET_LP		_IOW(IOC_TYPE_LCM,	6,		FY_BOOL)
#define LCM_IOC_DSI_SET_HS		_IOW(IOC_TYPE_LCM,	7,		FY_BOOL)
#define LCM_IOC_DSI_CLK_ON		_IOW(IOC_TYPE_LCM,	8,		FY_BOOL)
#define LCM_IOC_DSI_CLK_OFF		_IOR(IOC_TYPE_LCM,	9,		FY_BOOL)
#define LCM_IOC_DSI_WRITE		_IOW(IOC_TYPE_LCM,	10,		LCM_CMD_S)
#define LCM_IOC_DSI_READ		_IOR(IOC_TYPE_LCM,	11,		LCM_CMD_S)

#define LCM_IOC_SPI_WRITE		_IOW(IOC_TYPE_LCM,	12,		LCM_CMD_S)
#define LCM_IOC_SPI_READ		_IOR(IOC_TYPE_LCM,	13,		LCM_CMD_S)

#define LCM_IOC_I2C_WRITE		_IOW(IOC_TYPE_LCM,	14,		LCM_CMD_S)
#define LCM_IOC_I2C_READ		_IOR(IOC_TYPE_LCM,	15,		LCM_CMD_S)

#define LCM_IOC_ADV 			FY_DRV_ADV_IOC(IOC_TYPE_LCM)

/****************************************************************************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LCM_DRV_IOC_H__ */

