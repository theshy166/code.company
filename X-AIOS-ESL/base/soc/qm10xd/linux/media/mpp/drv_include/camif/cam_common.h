#ifndef CAM_COMMON_H
#define CAM_COMMON_H
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
#define BIT(nr)        (1UL << (nr))
#endif

typedef unsigned int UINT32;
typedef int SINT32;
typedef unsigned short int UINT16;
typedef unsigned char UINT8;
typedef char CHAR;
typedef void VOID;
typedef _Bool BOOL;
typedef unsigned long long int UINT64;


#define CAM_SUCCESS  0
#define CAM_FAILURE  (-1)

typedef enum{
    CAM_PIXEL_FMT_RAW8,
    CAM_PIXEL_FMT_RAW10,
    CAM_PIXEL_FMT_RAW12,
    CAM_PIXEL_FMT_RAW16,
    CAM_PIXEL_FMT_NV12,
    CAM_PIXEL_FMT_NV21,
    CAM_PIXEL_FMT_YUV422,
    CAM_PIXEL_FMT_NUM,
}Cam_Pixel_Fmt_E;

typedef enum{
    CAM_BAYER_RGGB = 0,
    CAM_BAYER_GRBG = 1,
    CAM_BAYER_BGGR = 2,
    CAM_BAYER_GBRG = 3,
    CAM_BAYER_NUM,
}Cam_Bayer_Pattern_E;
#endif
