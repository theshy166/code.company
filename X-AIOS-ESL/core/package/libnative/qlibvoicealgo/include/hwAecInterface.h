#ifndef _HW_AEC_INTERFACE_H_
#define _HW_AEC_INTERFACE_H_
typedef void *HwAecHandle;
/*
	初始化
	handle 上下文
*/
extern int hwAecInit(HwAecHandle *handle);
/*
	销毁
	handle 上下文
*/
extern int hwAecDestroy(HwAecHandle handle);
/*
	处理
	handle 上下文
	mic 麦克风音频数据，16000 单声道 32-float	1024个点
	ref 麦克风音频数据，16000 单声道 32-float	1024个点
	输出音频 麦克风音频数据，16000 单声道 32-float	1024个点
*/
extern int hwAecProcess(HwAecHandle handle,float *mic,float *ref,float *output);
#endif
