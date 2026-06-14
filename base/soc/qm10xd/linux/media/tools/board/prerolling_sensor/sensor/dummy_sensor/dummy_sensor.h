#ifndef dummy_sensor_H_
#define dummy_sensor_H_

#define _NAME(n, s) n##_##s
#if MULTI_SENSOR
#define NAME(n) _NAME(n, DUMMY_SENSOR)
#else
#define NAME(n) n
#endif

//---------------------------720P25-------------------------------------------------------
#define FRAM_W_720P25				1800
#define FRAM_H_720P25				780
#define ACTIVE_W_720P25				1280
#define ACTIVE_H_720P25				720
#define PIC_IN_W_720P25				1280
#define PIC_IN_H_720P25				720
#define OFFSET_X_720P25				0
#define OFFSET_Y_720P25				0

//---------------------------720P30-------------------------------------------------------
#define FRAM_W_720P30				1540
#define FRAM_H_720P30				780
#define ACTIVE_W_720P30				1280
#define ACTIVE_H_720P30				714
#define PIC_IN_W_720P30				1280
#define PIC_IN_H_720P30				714
#define OFFSET_X_720P30				0
#define OFFSET_Y_720P30				0

//---------------------------1080P25-------------------------------------------------------
#define FRAM_W_1080P25				2640
#define FRAM_H_1080P25				1125
#define ACTIVE_W_1080P25			1920
#define ACTIVE_H_1080P25  			1080
#define PIC_IN_W_1080P25			1920
#define PIC_IN_H_1080P25			1080
#define OFFSET_X_1080P25			0
#define OFFSET_Y_1080P25			0

//------------------1080P30-----------------------
#define FRAM_W_1080P30				2200
#define FRAM_H_1080P30				1125
#define ACTIVE_W_1080P30			1920
#define ACTIVE_H_1080P30			1080
#define PIC_IN_W_1080P30			1920
#define PIC_IN_H_1080P30			1080
#define OFFSET_X_1080P30			0
#define OFFSET_Y_1080P30			0

//------------------4Kx2KP15-----------------------
#define FRAM_W_4Kx2KP15 			4096
#define FRAM_H_4Kx2KP15 			2160
#define ACTIVE_W_4Kx2KP15			3840
#define ACTIVE_H_4Kx2KP15			2160
#define PIC_IN_W_4Kx2KP15			3840
#define PIC_IN_H_4Kx2KP15			2160
#define OFFSET_X_4Kx2KP15			0
#define OFFSET_Y_4Kx2KP15			0

//------------------640X480P30-----------------------
#define FRAM_W_640X480P30 			800
#define FRAM_H_640X480P30 			540
#define ACTIVE_W_640X480P30			640
#define ACTIVE_H_640X480P30			480
#define PIC_IN_W_640X480P30			640
#define PIC_IN_H_640X480P30			480
#define OFFSET_X_640X480P30			0
#define OFFSET_Y_640X480P30			0

//------------------2304X1296P15-----------------------
#define FRAM_W_2304X1296P15 			2800
#define FRAM_H_2304X1296P15 			1600
#define ACTIVE_W_2304X1296P15		    2304
#define ACTIVE_H_2304X1296P15			1296
#define PIC_IN_W_2304X1296P15			2304
#define PIC_IN_H_2304X1296P15			1296
#define OFFSET_X_2304X1296P15			0
#define OFFSET_Y_2304X1296P15			0


//------------------3072X1728P15-----------------------
#define FRAM_W_3072X1728P15 			3800
#define FRAM_H_3072X1728P15 			1800
#define ACTIVE_W_3072X1728P15			3072
#define ACTIVE_H_3072X1728P15			1728
#define PIC_IN_W_3072X1728P15			3072
#define PIC_IN_H_3072X1728P15			1296
#define OFFSET_X_3072X1728P15			0
#define OFFSET_Y_3072X1728P15			0

//------------------2592X1440P15-----------------------
#define FRAM_W_2592X1440P15 			2800
#define FRAM_H_2592X1440P15 			1600
#define ACTIVE_W_2592X1440P15			2592
#define ACTIVE_H_2592X1440P15			1440
#define PIC_IN_W_2592X1440P15			2592
#define PIC_IN_H_2592X1440P15			1440
#define OFFSET_X_2592X1440P15			0
#define OFFSET_Y_2592X1440P15			0

//------------------3840X2160P15-----------------------
#define FRAM_W_3840X2160P15 			4000
#define FRAM_H_3840X2160P15 			2400
#define ACTIVE_W_3840X2160P15			3840
#define ACTIVE_H_3840X2160P15			2160
#define PIC_IN_W_3840X2160P15			3840
#define PIC_IN_H_3840X2160P15			2160
#define OFFSET_X_3840X2160P15			0
#define OFFSET_Y_3840X2160P15			0

#endif /* dummy_sensor_H_ */


