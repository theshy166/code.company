#ifndef _HW_WAKEUP_INTERFACE_H_
#define _HW_WAKEUP_INTERFACE_H_
typedef void* HwWakeupHandle;
/*
	初始化
	handle 上下文
	return -1 失败 0 成功
*/
extern int hwWakeupInit(HwWakeupHandle *handle);
/*
	销毁
	handle 上下文
	return -1 失败 0 成功
*/
extern int hwWakeupDestroy(HwWakeupHandle handle);
/*
	处理
	handle 上下文
	音频数据，16000 单声道 16bits	480个点
	return -1 未唤醒
		0 小明小明
		1 打开拍照
		2 开始拍照
		3 打开录像
		4 开始录像
		5 停止录像
		6 打开日历
		7 打开天气
		8 拨打电话
		9 接听电话
		10 挂断电话
		11 上一张照片
		12 下一张照片
		13 增大音量
		14 降低音量
		15 减小音量
*/
extern int hwWakeup(HwWakeupHandle handle,short *data);
#endif
