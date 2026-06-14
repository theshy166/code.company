#ifndef _HW_FACE_INTERFACE_H_
#define _HW_FACE_INTERFACE_H_
typedef void *HwFaceHandle;
/*
        初始化
        handle 上下文
	return 0 - 成功 < 0 - 失败
*/
extern int hwFaceInit(HwFaceHandle *handle); 
/*
        销毁
        handle 上下文
	return 0 - 成功 < 0 - 失败
*/
extern int hwFaceDestroy(HwFaceHandle handle); 
/*
        处理
        handle 上下文
	return 0 - 成功 < 0 - 失败
	img 图像数据 (320 × 240 yuv)
	gesture 1 - palm 2 - ok
	faceret > 0 有脸
	faceXy 坐标 0.0 - 160.0 0.0 - 128.0
*/
extern int hwFace(HwFaceHandle handle,unsigned char *img,int *gesture,int *faceret,float *faceXy);
#endif
