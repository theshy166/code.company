#ifndef _RESAMPLE_API_H_
#define _RESAMPLE_API_H_

#include "fh_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#define RESAMP_RATION(x,y)	(x/y)

#define SAMPLES_48K10MS		480
#define BYTES_48K10MS		960
#define SIZE_RESAMPLER_STATE	6
#define	SIZE_ARRAY		8

#define PARAM_LSHIFT(a,b)	(a)<<(b)
#define PARAM_RSHIFT(a,b)	(a)>>(b)

#define LINE_RESAMPLE		FY_TRUE
#define RESAMPLE_NEW		FY_TRUE

#define INVAILD_STATUS		(-1)

typedef void NsHandle;

typedef enum _VQE_TYPE_E
{
	VQE_TYPE_ANR,
	VQE_TYPE_AGC,
	VQE_TYPE_AEC,
	VQE_TYPE_HPF,
	VQE_TYPE_MAX
}VQE_TYPE_E;

typedef struct {
	FY_S32			S_8_16[SIZE_ARRAY];
	FY_S32			S_16_12[SIZE_ARRAY];
	FY_S32			S_12_24[SIZE_ARRAY];
	FY_S32			S_24_48[SIZE_ARRAY];
} MPI_RESAMPTO48K;

typedef struct {
	FY_S32			S_8_16[SIZE_ARRAY];
	FY_S32			S_16_32[SIZE_ARRAY];
} MPI_RESAMPTO32K;

typedef struct __MPI_RESAMP_S
{
	FY_U8*			pAddr;
	FY_U32			u32Len;
	FY_U32			u32SampRate;
	FY_U32			u32Bit;
	FY_U32			u32TrcNum;
	FY_BOOL			bMsb;
	FY_BOOL			bAecFar;
	FY_VOID*		pAecHandle;
	FY_BOOL			bSigned;
	FY_U32			uLpass;
}MPI_RESAMP_S;

typedef struct __MPI_NS_S
{
	FY_VOID*		pHandle;
	FY_U32			bOpen;
	FY_U32			uParam1;
	FY_U32			uParam2;
}MPI_NS_S;

FY_S32 MPI_RESAMPLE_up(MPI_NS_S pNS[],MPI_RESAMP_S* pIn,MPI_RESAMP_S* pOut);

FY_S32 MPI_RESAMPLE_down(MPI_NS_S pNS[],MPI_RESAMP_S* pIn,MPI_RESAMP_S* pOut);

FY_S32 MPI_Ns_Create(NsHandle** NS_inst);

FY_S32 MPI_Ns_Free(NsHandle* NS_inst);

FY_S32 MPI_Ns_Init(NsHandle* NS_inst, FY_U32 fs);

FY_S32 MPI_Ns_set_policy(NsHandle* NS_inst, FY_S32 mode);

FY_S32 MPI_Ns_Process(NsHandle* NS_inst,
                     FY_S16* spframe,
                     FY_S16* spframe_H,
                     FY_S16* outframe,
                     FY_S16* outframe_H);

float MPI_Ns_prior_speech_probability(NsHandle* handle);
FY_S32 MPI_Resample_convertxxk_xxbits(MPI_RESAMP_S *pAtr_in, MPI_RESAMP_S* pAtr_out);
void MPI_DownsampleBy2(const FY_S16* in, const FY_S16 len, FY_S16* out, FY_S32* filtState);
void MPI_UpsampleBy2(const FY_S16* in, FY_S16 len, FY_S16* out, FY_S32* filtState);
FY_S32 MPI_change_data(MPI_RESAMP_S* pIn,MPI_RESAMP_S* pOut);
FY_S32 MPI_RESAMPLE_convert_xxbits(FY_CHAR* pIN,FY_U32 uInSize,FY_U32 uSize, MPI_RESAMP_S* pAtr_out);
FY_S32 MPI_RESAMPLE_convert_S32bits(FY_U8* pIN,FY_U32 uSize, MPI_RESAMP_S* pAtr_out);
FY_CHAR *MPI_AUDIO_CODEC_ShowVersion(FY_BOOL print_enable);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif
