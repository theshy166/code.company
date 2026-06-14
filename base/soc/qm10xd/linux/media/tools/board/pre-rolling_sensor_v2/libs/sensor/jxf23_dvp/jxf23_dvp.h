#ifndef JXF23_DVP_H_
#define JXF23_DVP_H_

//------------------1080P7.5-----------------------
#define FRAM_W_1080P7   2560
#define FRAM_H_1080P7   1125
#define ACTIVE_W_1080P7 1920
#define ACTIVE_H_1080P7 1080
#define PIC_IN_W_1080P7 1920
#define PIC_IN_H_1080P7 1080
#define OFFSET_X_1080P7 0
#define OFFSET_Y_1080P7 0

//------------------1080P15-----------------------
#define FRAM_W_1080P15   2560
#define FRAM_H_1080P15   2250
#define ACTIVE_W_1080P15 1920
#define ACTIVE_H_1080P15 1080
#define PIC_IN_W_1080P15 1920
#define PIC_IN_H_1080P15 1080
#define OFFSET_X_1080P15 0
#define OFFSET_Y_1080P15 0

//------------------1080P15-----------------------
#define FRAM_W_1080P15   2560
#define FRAM_H_1080P15   2250
#define ACTIVE_W_1080P15 1920
#define ACTIVE_H_1080P15 1080
#define PIC_IN_W_1080P15 1920
#define PIC_IN_H_1080P15 1080
#define OFFSET_X_1080P15 0
#define OFFSET_Y_1080P15 0

//------------------480P7-----------------------
// cis=27M pix=74.25M
#define FRAM_W_480P7   4800
#define FRAM_H_480P7   600
#define ACTIVE_W_480P7 640
#define ACTIVE_H_480P7 480
#define PIC_IN_W_480P7 640
#define PIC_IN_H_480P7 480
#define OFFSET_X_480P7 0
#define OFFSET_Y_480P7 0

//------------------720P2-----------------------
// 
#define FRAM_W_720P2   3000
#define FRAM_H_720P2   1000
#define ACTIVE_W_720P2 1280
#define ACTIVE_H_720P2 720
#define PIC_IN_W_720P2 1280
#define PIC_IN_H_720P2 720
#define OFFSET_X_720P2 0
#define OFFSET_Y_720P2 0

//------------------1080P25-----------------------
// cis=27M pix=74.25M
#define FRAM_W_1080P25   2880
#define FRAM_H_1080P25   1125
#define ACTIVE_W_1080P25 1920
#define ACTIVE_H_1080P25 1080
#define PIC_IN_W_1080P25 1920
#define PIC_IN_H_1080P25 1080
#define OFFSET_X_1080P25 0
#define OFFSET_Y_1080P25 0


//------------------1080P25-----------------------
// cis=27M pix=84.375M
#define FRAM_W_1080P30   2400
#define FRAM_H_1080P30   1125
#define ACTIVE_W_1080P30 1920
#define ACTIVE_H_1080P30 1080
#define PIC_IN_W_1080P30 1920
#define PIC_IN_H_1080P30 1080
#define OFFSET_X_1080P30 0
#define OFFSET_Y_1080P30 0

#include "cfg/jxf23_dvp_480p7.h"
#include "cfg/jxf23_dvp_720p2.h"
//------------------720p25-----------------------
#define FRAM_W_720P25   2400
#define FRAM_H_720P25   1125
#define ACTIVE_W_720P25 1280
#define ACTIVE_H_720P25 720
#define PIC_IN_W_720P25 1280
#define PIC_IN_H_720P25 720
#define OFFSET_X_720P25 0
#define OFFSET_Y_720P25 0
//------------------480P30-----------------------
#define FRAM_W_480P30   2400
#define FRAM_H_480P30   1125
#define ACTIVE_W_480P30 640
#define ACTIVE_H_480P30 480
#define PIC_IN_W_480P30 640
#define PIC_IN_H_480P30 480
#define OFFSET_X_480P30 0
#define OFFSET_Y_480P30 0
#include "cfg/jxf23_dvp_1080p7.h"
#include "cfg/jxf23_dvp_1080p15.h"
#include "cfg/jxf23_dvp_1080p25.h"
#include "cfg/jxf23_dvp_1080p30.h"
#include "cfg/jxf23_dvp_720p25.h"
#include "cfg/jxf23_dvp_480p30.h"

#define DEVICE_ID 0x80 >> 1

#endif /* JXF23_DVP_H_ */


