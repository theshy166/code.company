#ifndef __FY_AO_MPI_H__
#define __FY_AO_MPI_H__

#include "fy_comm_aio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * @brief         配置音频输出设备属性S
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     cmd                音频输出命令号，默认传 IOC_CMD_BUTT
 * @param[in]     pstAttr            音频输出设备属性结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 音频输入设备属性为静态属性，必须在执行 FY_AO_Enable 前配置。
 *
 */
FY_S32 FY_AO_SetPubAttr(AUDIO_DEV AoDevId,FY_U32 cmd, FY_VOID *pstAttr);

/**
 * @brief         获取音频输出设备的属性
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     cmd                音频输出命令号，默认传 IOC_CMD_BUTT
 * @param[out]    pstAttr            音频输出设备属性结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 */
FY_S32 FY_AO_GetPubAttr(AUDIO_DEV AoDevId,FY_U32 cmd, FY_VOID *pstAttr);

/**
 * @brief         使能音频输出设备
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 */
FY_S32 FY_AO_Enable(AUDIO_DEV AoDevId);

/**
 * @brief         禁用音频输出设备
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 */
FY_S32 FY_AO_Disable(AUDIO_DEV AoDevId);

/**
 * @brief         获取音频输出设备句柄
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 */
FY_S32 FY_AO_GetFd(AUDIO_DEV AoDevId);

/**
 * @brief         使能音频输出设备通道
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_Enable 配置后生效。
 *
 */
FY_S32 FY_AO_EnableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

/**
 * @brief         禁用音频输出设备通道
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_Enable 配置后生效。
 *
 */
FY_S32 FY_AO_DisableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

/**
 * @brief         设置音频输出通道参数信息
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[in]     pFrmInfo           通道参数结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置前调用。
 *
 */
FY_S32 FY_AO_SetChnParam(AUDIO_DEV AoDevId, AO_CHN AoChn,AUDIO_CHN_PARAM_S* pFrmInfo);

/**
 * @brief         获取音频输入通道参数信息
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[out]    pFrmInfo           通道参数结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置后生效。
 *
 */
FY_S32 FY_AO_GetChnParam(AUDIO_DEV AoDevId, AO_CHN AoChn,AUDIO_CHN_PARAM_S* pFrmInfo);

/**
 * @brief         发送音频输出通道数据
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[in]     pstData            通道数据结构体指针
 * @param[in]     s32MilliSec        超时时间，-1 阻塞/0 立即返回
 *
 * @retval  >=0             成功
 * @retval  "<0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置后生效。
 *
 */
FY_S32 FY_AO_SendFrame(AUDIO_DEV AoDevId, AO_CHN AoChn, AUDIO_FRAME_S *pstData, FY_S32 s32MilliSec);

/**
 * @brief         发送音频输出通道数据
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[in]     pstData            通道数据结构体指针
 * @param[in]     s32MilliSec        超时时间，-1 阻塞/0 立即返回
 *
 * @retval  >=0             成功
 * @retval  "<0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置后生效。
 *
 */
FY_S32 FY_AO_SendFrameCallback(AUDIO_DEV AoDevId, AO_CHN AoChn, AUDIO_FRAME_S *pstData, FY_S32 s32MilliSec,
										frame_callback callback, callback_param* param);


/**
 * @brief         清除音频输出通道缓存数据
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[in]     pstData            通道数据结构体指针
 * @param[in]     s32MilliSec        超时时间，-1 阻塞/0 立即返回
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置后生效。
 *
 */
FY_S32 FY_AO_ClearChnBuf(AUDIO_DEV AoDevId ,AO_CHN AoChn);

/**
 * @brief         暂停音频输出通道
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AO_PauseChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

/**
 * @brief         恢复音频输出通道
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_EnableChn 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AO_ResumeChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

/**
 * @brief         设置音频输出设备音量
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     ptr                音量结构体指针，VOLUME_PARAM_S
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_Enable 配置后生效。
 *
 */
FY_S32 FY_AO_SetVolume(AUDIO_DEV AoDevId, VOLUME_PARAM_S* ptr);

/**
 * @brief         获取音频输出设备音量
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[out]    ptr                音量结构体指针，VOLUME_PARAM_S
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_Enable 配置后生效。
 *
 */
FY_S32 FY_AO_GetVolume(AUDIO_DEV AoDevId, VOLUME_PARAM_S* ptr);

/**
 * @brief         设置音频输出设备声道模式
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     enTrackMode        声道模式结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_Enable 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AO_SetTrackMode(AUDIO_DEV AoDevId, AUDIO_TRACK_MODE_E enTrackMode);

/**
 * @brief         获取音频输出设备声道模式
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[out]    enTrackMode        声道模式结构体指针
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 * @note
 *    - 在 FY_AO_Enable 配置后生效，需硬件支持。
 *
 */
FY_S32 FY_AO_GetTrackMode(AUDIO_DEV AoDevId, AUDIO_TRACK_MODE_E *penTrackMode);

/**
 * @brief         配置音频输出设备通道静音状态
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[in]     bEnable            静音状态
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 */
FY_S32 FY_AO_SetMute(AUDIO_DEV AoDevId, AO_CHN AoChn, FY_BOOL bEnable);

/**
 * @brief         获取音频输出设备通道静音状态
 *
 * @param[in]     AoDevId            音频输出设备号，参考 AUDIO_DEVID
 * @param[in]     AoChn              音频输出设备通道号，范围 0~chn_num
 * @param[in]     pEnable            静音状态
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 */
FY_S32 FY_AO_GetMute(AUDIO_DEV AoDevId, AO_CHN AoChn, FY_BOOL *pEnable);

/**
 * @brief         Ao进入低功耗状态
 *
 * @param[in]     AoDevId            音频输入设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ai_errcode} 。
 *
 *
 */
FY_S32 FY_AO_Suspend(AUDIO_DEV AoDevId);

/**
 * @brief         Ao从低功耗状态恢复
 *
 * @param[in]     AoDevId            音频输入设备号，参考 AUDIO_DEVID
 *
 * @retval  0             成功
 * @retval  "非0"          失败，其值参见 @namelink{错误码,ao_errcode} 。
 *
 *
 */
FY_S32 FY_AO_Resume(AUDIO_DEV AoDevId);


FY_VOID FY_AO_SYS_Init(FY_VOID);
FY_VOID FY_AO_SYS_DeInit(FY_VOID);


FY_S32 FY_AO_Ch2Mix(AUDIO_DEV AoDevId, AI_CHN AoChn, void* src_ptr, void* dst_ptr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_AO_H__ */

