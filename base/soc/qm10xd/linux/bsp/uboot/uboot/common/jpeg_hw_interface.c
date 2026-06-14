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
#include <common.h>
#include <linux/stddef.h>
#include "jpeg_hw_drv.h"

struct EncParaInfo {
	uint8_t HD;
	uint8_t HA;
	uint8_t QT;
	uint8_t NBlock;
	uint8_t V;
	uint8_t H;
};

struct EncCurInfo {
	uint8_t Format;
	struct EncParaInfo ParaInfo[4];
};

struct EncCurInfo Cur_Info[] = {
	{
		.Format = JPEG_FMT_YUV,
		{
			{ 0, 0, 0, 3, 2, 2 },
			{ 1, 1, 1, 0, 1, 1 },
			{ 1, 1, 1, 0, 1, 1 },
			{ 0, 0, 0, 0, 0, 0 },
		},
	},
	{
		.Format = JPEG_FMT_GRAY,
		{
			{ 0, 0, 0, 0, 1, 1 },
			{ 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0 },
		},
	},
};

uint32_t JPEG_ReadReg(uint32_t addr)
{
	uint32_t value = *(volatile uint32_t*)addr;
	return value;
}

void JPEG_WriteReg(uint32_t addr, uint32_t val)
{
	*(volatile uint32_t*)addr = val;
}

int JPEG_CheckDecParam(jpeg_context_t *handle, struct DecParam *Dec_Param)
{
	int rtn = 0;

	if ((Dec_Param->JPG_Addr == 0) || (Dec_Param->Luma_Addr == 0) ||
		(((Dec_Param->JPG_Addr) & (0x3F)) != 0) || (((Dec_Param->Luma_Addr) & (0x3F)) != 0)) {
		printf("JPEG_CheckDecParam Err: JPG_Addr 0x%x, Luma_Addr 0x%x, need align 64 byte.\n",
			Dec_Param->JPG_Addr, Dec_Param->Luma_Addr);
		rtn = -1;
		goto exit;
	}

	if (Dec_Param->Format == JPEG_FMT_YUV) {
		if ((Dec_Param->Chroma_Addr == 0) || (((Dec_Param->Chroma_Addr) & (0x3F)) != 0)) {
			printf("JPEG_CheckDecParam Err: Chroma_Addr 0x%x, need align 64 byte.\n", Dec_Param->Chroma_Addr);
			rtn = -1;
			goto exit;
		}
	}

	if ((Dec_Param->Format != JPEG_FMT_YUV) && (Dec_Param->Format != JPEG_FMT_GRAY)) {
		printf("JPEG_CheckDecParam: Not support Format %d !\n", Dec_Param->Format);
		rtn = -1;
		goto exit;
	}

exit:
	return rtn;
}

int JPEG_SetDecParam(jpeg_context_t *handle, struct DecParam *Dec_Param)
{
	int rtn = 0;
	uint32_t reg = 0;
	uint8_t Nf = 0;

	rtn = JPEG_CheckDecParam(handle, Dec_Param);
	if (rtn) {
		printf("========JPEG_SetDecParam: Check DecParam fail !\n");
		return -1;
	}

	//reg8
	printf("JPEG_SetDecParam: JPG_Addr = 0x%08x\n", Dec_Param->JPG_Addr);
	JPEG_SetCurLumaOffset(handle, Dec_Param->JPG_Addr);

	//Buff0Size
	printf("JPEG_SetDecParam: JPG_Size = 0x%08x\n", Dec_Param->JPG_Size);
	JPEG_SetBuff0Size(handle, Dec_Param->JPG_Size);

	//STMAddr0
	printf("JPEG_SetDecParam: Luma_Addr = 0x%08x\n", Dec_Param->Luma_Addr);
	JPEG_SetSTMAddr0(handle, Dec_Param->Luma_Addr);

	//reg9
	if (Dec_Param->Format == JPEG_FMT_YUV) {
		printf("JPEG_SetDecParam: Chroma_Addr = 0x%08x\n", Dec_Param->Chroma_Addr);
		JPEG_SetCurChromaOffset(handle, Dec_Param->Chroma_Addr);
	}

	//reg1
	if (Dec_Param->Format == JPEG_FMT_YUV) {
		Nf = 2;
	} else if (Dec_Param->Format == JPEG_FMT_GRAY) {
		Nf = 0;
	}
	reg = Nf | (Dec_Param->DecEn << 3);


	printf("JPEG_SetDecParam: reg1 = 0x%08x\n", reg);
	JPEG_SetReg1(handle, reg);

	JPEG_SetReg11(handle, 0); //set scan mode to block

	return rtn;
}

static uint16_t read_short(uint8_t **in)
{
	uint8_t *ptr = *in;
	*in += 2;
	return (ptr[0] << 8) | ptr[1];
}

void JPEG_Fix_ImgSize(jpeg_context_t *handle)
{
	uint8_t *ptr = NULL;
	ptr = (uint8_t *)(handle->imgsize_ptr);
	if(ptr == NULL)
		return;
	ptr[0] = (handle->imgsize.height >> 8) & 0xFF;
	ptr[1] = handle->imgsize.height & 0xFF;
	ptr[2] = (handle->imgsize.width >> 8) & 0xFF;
	ptr[3] = handle->imgsize.width & 0xFF;
	//__cpuc_flush_dcache_area((void *)ptr, 4);
}

int JPEG_Dec_GetImgInfo(void *jpeg_handle, uint32_t JPG_Addr,  uint32_t JPG_size, struct ImgInfo *img_info)
{
	int rtn = 0;
	jpeg_context_t *handle = (jpeg_context_t *)jpeg_handle;
	uint8_t found = 0;
	uint8_t *jpg = NULL;
	uint8_t *ptr = NULL;
	uint8_t *end = NULL;
	uint16_t tag = 0, len = 0;
	uint16_t width = 0, height = 0;
	uint16_t offset = 0;
	handle->imgsize_ptr = 0;

	if ((JPG_Addr == 0) || (!img_info)) {
		printf("JPEG_Dec_GetImgInfo: JPG_Addr 0x%x, img_info %p !\n", JPG_Addr, img_info);
		rtn = -1;
		goto exit;
	}

	jpg = (uint8_t *)JPG_Addr;//handle->jpg_virtaddr;
	ptr = jpg;
	end = jpg + JPG_size;//length
	printf("JPEG_Dec_GetImgInfo: JPG_Addr 0x%08x, start = %p\n", JPG_Addr, jpg);
	tag = read_short(&ptr);
	if (tag != JPEG_IMAGE_START) {
		printf("JPEG_Dec_GetImgInfo: unsupported image format, not found SOI !\n");
		rtn = -1;
		goto exit;
	}

	while (ptr < (end - 5)) {
		tag = read_short(&ptr);
		len = read_short(&ptr);
		if (tag == JPEG_FRAME_START) {
			found = 1;
			break;
		}
		if (tag >= JPEG_MODULO_START && tag <= JPEG_MODULO_END) {
			continue;
		}
		if ((tag == JPEG_SCAN_START) || (tag == JPEG_IMAGE_END)) {
			rtn = -1;
			goto exit;
		}
		ptr += len - 2;//skip data segment
	}

	if (!found) {
		printf("JPEG_Dec_GetImgInfo: unsupported image format, not found SOF !\n");
		rtn = -1;
		goto exit;
	}

	++ptr;//skip image precision byte
	printf("JPEG_Dec_GetImgInfo: ptr = %p\n", ptr);
	offset = (uint32_t)ptr - (uint32_t)jpg;
	printf("JPEG_Dec_GetImgInfo: offset = 0x%x\n", offset);
	if(ptr > (end  -4)) {
		rtn = -1;
		goto exit;
	}

	handle->imgsize_ptr = (uint32_t)ptr;
	height = (ptr[0] << 8) | ptr[1];
	width = (ptr[2] << 8) | ptr[3];
	handle->imgsize.height = height;
	handle->imgsize.width = width;
	printf("JPEG_Dec_GetImgInfo: width = %d, height = %d\n", width, height);
	ptr[0] = (ALIGN_TO(height, 16) >> 8) & 0xFF;
	ptr[1] = ALIGN_TO(height, 16) & 0xFF;
	ptr[2] = (ALIGN_TO(width, 16) >> 8) & 0xFF;
	ptr[3] = ALIGN_TO(width, 16) & 0xFF;
	//__cpuc_flush_dcache_area((void *)ptr, 4);
	img_info->PicHeight = read_short(&ptr);
	img_info->PicWidth = read_short(&ptr);
	img_info->ClipHeight = img_info->PicHeight - height;
	img_info->ClipWidth = img_info->PicWidth - width;
	img_info->MaxHeight = 4095;
	img_info->MaxWidth = 4096;
	printf("JPEG_Get_ImgInfo: PicWidth = %d, PicHeight = %d, ClipWidth = %d, ClipHeight = %d, MaxWidth = %d, MaxHeight = %d\n",
		img_info->PicWidth, img_info->PicHeight, img_info->ClipWidth, img_info->ClipHeight, img_info->MaxWidth, img_info->MaxHeight);

	return rtn;

exit:
	printf("JPEG_Get_ImgInfo fail !\n");
	return rtn;
}

int JPEG_Dec(void *jpeg_handle, struct DecParam *Dec_Param)
{
	int rtn = 0;
	jpeg_context_t *handle = (jpeg_context_t *)jpeg_handle;

	if ((!handle) || (!Dec_Param)) {
		printf("JPEG_Dec: handle %p, Dec_Param %p !\n", handle, Dec_Param);
		return -1;
	}

	printf("JPEG_Dec\n");

#if 0
	rtn = JPEG_Module_Reset(handle);
	if (rtn) {
		printf("JPEG_Module_Reset fail !\n");
		rtn = -1;
		goto exit;
	}
#endif
	//JPEG_SetIntEnable(handle);
	rtn = JPEG_SetDecParam(handle, Dec_Param);
	if (rtn) {
		printf("========JPEG_Dec: JPEG_SetDecParam fail !\n");
		rtn = -1;
		goto exit;
	}

	JPEG_SetEncDecStart(handle);
	mdelay(Dec_Param->timeout);
	printf("========JPEG_Dec: done !\n");
	JPEG_Fix_ImgSize(handle);
	return rtn;
exit:
	return rtn;
}
jpeg_context_t jpeg_ctx;
#define JPEG_HW_BASE_ADDR       (0x26100000)
void jpeg_clk_on(int en)
{
	uint32_t rdata=0;
	if(en){
		rdata  = *(volatile uint32_t*)0x2400002c;
		JPEG_WriteReg(0x2400002c,rdata|(BIT(0)+BIT(2)+BIT(3)));
	}else{
		rdata  = *(volatile uint32_t*)0x2400002c;
		JPEG_WriteReg(0x2400002c,rdata&((~BIT(0)+BIT(2)+BIT(3))));
	}
}


unsigned int jpeg_decode_hw(void *jpg, void *y,uint32_t jpeg_size)
{
	struct DecParam decParam;
	struct ImgInfo img_info;
	unsigned int w,h;
	memset(&jpeg_ctx, 0x00, sizeof(jpeg_context_t));
	jpeg_ctx.init_cnt=1;
	jpeg_ctx.jpeg_cfg_base = JPEG_HW_BASE_ADDR;
	jpeg_ctx.imgsize_ptr = 0;
	JPEG_Dec_GetImgInfo(&jpeg_ctx, (uint32_t)(jpg), jpeg_size,&img_info);
	w = jpeg_ctx.imgsize.width;
	h = jpeg_ctx.imgsize.height;
	jpeg_clk_on(1);
	decParam.JPG_Addr = (uint32_t)(jpg);
	decParam.JPG_Size = jpeg_size;
	decParam.Luma_Addr =(uint32_t) y;
	decParam.Chroma_Addr = (uint32_t)y+ ALIGN_TO(w,16)*ALIGN_TO(h,16);
	decParam.Format = JPEG_FMT_YUV;
	decParam.DecEn = 1;
	decParam.timeout = 100;//ms

	JPEG_Dec(&jpeg_ctx,&decParam);
	//jpeg_clk_on(0);
	return 0;
}


