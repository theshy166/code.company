/*
 * Copyright (C) 2019 MOLCHIP(Shanghai) Co., Ltd.
 * Author: Tiago Dong<tiago.dong@molchip.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 */

#ifndef __JPEG_DRV_H__
#define __JPEG_DRV_H__

typedef unsigned char		uint8;
typedef uint8				uint8_t;
typedef unsigned short		uint16;
typedef uint16				uint16_t;
typedef unsigned int		uint32;
typedef uint32				uint32_t;
typedef unsigned long int 	uint64;
typedef long long int 		int64;
typedef unsigned int		uint;
typedef signed char			int8;
typedef int8				int8_t;
typedef signed short		int16;
typedef int16				int16_t;
typedef signed int			int32;
typedef int32				int32_t;
//#define BIT(__n)       (1<<(__n))

#define ALIGN_TO(x, iAlign) ( (((unsigned int)(x)) + (iAlign) - 1) & (~((iAlign) - 1)) )

#define JPEG_CKG_EN						0x2C//0x30 // BIT23
#define JPEG_SOFT_RESET					0x30//x34 // BIT4
//#define JPEG_CLEAR					0x34 // BIT6

#define JPEG_VER_INFO					0x0000
#define JPEG_STM_INFO					0x0030

#define JPEG_INT_STATUS					0x0004
#define JPEG_INT_EN						0x0008

#define JPEG_PROC_STATUS				0x000C
#define JPEG_PROC_STATUS1				0x0048
#define JPEG_STM_ADDR0					0x0034
#define JPEG_STM_ADDR1					0x0038
#define JPEG_BUFF0_SIZE					0x0050
#define JPEG_BUFF1_SIZE					0x0054
#define JPEG_MB_RATE_CTRL				0x004C
#define JPEG_QMEM						0x0080
#define JPEG_HUFFENC					0x0100
#define JPEG_DHTMEM						0x0400

#define JPEG_REG0						0x0010
#define JPEG_REG1						0x0014
#define JPEG_REG2						0x0018
#define JPEG_REG3						0x001C
#define JPEG_REG4						0x0020
#define JPEG_REG5						0x0024
#define JPEG_REG6						0x0028
#define JPEG_REG7						0x002C
#define JPEG_REG8						0x003C
#define JPEG_REG9						0x0040
#define JPEG_REG10						0x0044
#define JPEG_REG11						0x0058
#define JPEG_REG12						0x005C
#define JPEG_REG13						0x0060
#define JPEG_REG14						0x0064

enum jpeg_img_format {
	JPEG_FMT_GRAY = 0,
	JPEG_FMT_YUV,
	JPEG_FMT_MAX,
};

struct ImgSize {
	uint16_t W;
	uint16_t H;
};

struct ImgRoi {
	uint16_t X;
	uint16_t Y;
	uint16_t W;
	uint16_t H;
};

struct ImgInfo {
	uint16_t PicWidth;
	uint16_t PicHeight;
	uint16_t ClipWidth;
	uint16_t ClipHeight;
	uint16_t MaxWidth;
	uint16_t MaxHeight;
};

struct DecParam {
	uint32_t JPG_Addr;//jpg data, phyaddr
	uint32_t JPG_Size;//jpg size
	uint32_t Luma_Addr;//y data, phyaddr
	uint32_t Chroma_Addr;//uv data, phyaddr
	uint8_t Format;
	uint8_t DecEn;
	uint32_t timeout;//ms
};

void *JPEG_Create_Handle(void);
void JPEG_Destroy_Handle(void *jpeg_handle);
int JPEG_Init(void *jpeg_handle);
int JPEG_Dec_GetImgInfo(void *jpeg_handle, uint32_t JPG_Addr, uint32_t JPG_size,struct ImgInfo *img_info);
int JPEG_Dec(void *jpeg_handle, struct DecParam *Dec_Param);
int JPEG_Deinit(void *jpeg_handle);


#define JPEG_LENGTH_MAX 0x1000000//buf_size

enum jpeg_tags {
	JPEG_IMAGE_START = 0xFFD8,
	JPEG_IMAGE_END = 0xFFD9,
	JPEG_SCAN_START = 0xFFDA,
	JPEG_MODULO_START = 0xFFD0,
	JPEG_MODULO_END = 0xFFD7,
	JPEG_FRAME_START = 0xFFC0,
	JPEG_FRAME_SOF1 = 0xFFC1,//some pic use this
};

struct jpeg_size {
	uint16_t width;
	uint16_t height;
};

typedef struct {
	uint32_t init_cnt;
	uint32_t jpeg_cfg_base;
	uint32_t imgsize_ptr;
	struct jpeg_size imgsize;
} jpeg_context_t;

uint32_t JPEG_ReadReg(uint32_t addr);
void JPEG_WriteReg(uint32_t addr, uint32_t val);

uint32_t JPEG_GetIntStatus(jpeg_context_t *handle);
void JPEG_SetIntClear(jpeg_context_t *handle);
void JPEG_SetReg1(jpeg_context_t *handle, uint32_t val);
void JPEG_SetNMCU(jpeg_context_t *handle, uint32_t NMCU);
void JPEG_SetReg3(jpeg_context_t *handle, uint32_t val);
void JPEG_SetReg4(jpeg_context_t *handle, uint32_t val);
void JPEG_SetReg5(jpeg_context_t *handle, uint32_t val);
void JPEG_SetReg6(jpeg_context_t *handle, uint32_t val);
void JPEG_SetSTMAddr0(jpeg_context_t *handle, uint32_t STMAddr0);
void JPEG_SetSTMAddr1(jpeg_context_t *handle, uint32_t STMAddr1);
void JPEG_SetBuff0Size(jpeg_context_t *handle, uint32_t Buff0Size);
void JPEG_SetBuff1Size(jpeg_context_t *handle, uint32_t Buff1Size);
void JPEG_SetCurLumaOffset(jpeg_context_t *handle, uint32_t CurLumaOffset);
void JPEG_SetCurChromaOffset(jpeg_context_t *handle, uint32_t CurChromaOffset);
void JPEG_SetReg10(jpeg_context_t *handle, uint32_t val);
void JPEG_SetReg11(jpeg_context_t *handle, uint32_t val);
void JPEG_SetOutLengthMux(jpeg_context_t *handle, uint32_t val);
uint32_t JPEG_GetOutLength(jpeg_context_t *handle);

void JPEG_SetLumaQMem(jpeg_context_t *handle, uint32_t *LumaQTbl);
void JPEG_SetChromaQMem(jpeg_context_t *handle, uint32_t *ChromaQTbl);
void JPEG_SetQMem(jpeg_context_t *handle);
void JPEG_SetDHTMem(jpeg_context_t *handle);
void JPEG_SetHuffEnc(jpeg_context_t *handle);

void JPEG_SetIntEnable(jpeg_context_t *handle);
void JPEG_SetEncDecStart(jpeg_context_t *handle);
uint32_t JPEG_GetSTMLenStatus(jpeg_context_t *handle);

void *JPEG_Create_Handle(void);
void JPEG_Destroy_Handle(void *jpeg_handle);

#endif /* __JPEG_DRV_H__ */

