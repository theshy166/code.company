#ifndef __SAMPLE_COMMON_ISP_H__
#define __SAMPLE_COMMON_ISP_H__
#include "common/qua_mm_common.h"

#define SAMPLE_SENSOR_FLAG_NORMAL    (0x00)
#define SAMPLE_SENSOR_FLAG_NIGHT     (0x02)
#define SAMPLE_SENSOR_FLAG_WDR       (0x01)
#define SAMPLE_SENSOR_FLAG_WDR_NIGHT (SAMPLE_SENSOR_FLAG_NIGHT | SAMPLE_SENSOR_FLAG_WDR)

struct dev_isp_info
{
    QUA_S32 enable;
    QUA_S32 channel;
    QUA_S32 isp_format;
    QUA_S32 isp_init_width;
    QUA_S32 isp_init_height;
    QUA_S32 isp_max_width;
    QUA_S32 isp_max_height;
    QUA_CHAR sensor_name[50];
    struct isp_sensor_if *sensor;
    QUA_BOOL bStop;
    QUA_BOOL running;
    QUA_S8 lut2dWorkMode;
};


extern QUA_S32 sample_common_isp_start(QUA_U32 dev_id);
extern QUA_S32 sample_common_isp_run(QUA_VOID);
extern QUA_S32 sample_common_isp_stop(QUA_VOID);

extern QUA_S32 sample_common_start_coolview(QUA_VOID *arg);
extern QUA_S32 sample_common_stop_coolview(QUA_U32 grpid);

#endif /*__SAMPLE_COMMON_ISP_H__*/
