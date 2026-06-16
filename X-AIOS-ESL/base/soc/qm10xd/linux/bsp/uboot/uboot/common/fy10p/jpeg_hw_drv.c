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

uint32_t JPEG_REGREAD(jpeg_context_t *handle, uint32_t offset)
{
	return JPEG_ReadReg(handle->jpeg_cfg_base + (offset));
}

void JPEG_REGWRITE(jpeg_context_t *handle, uint32_t offset, uint32_t val)
{
	uint32_t addr = 0;
	addr = handle->jpeg_cfg_base + (offset);
	JPEG_WriteReg(addr, val);
}

uint32_t JPEG_GetIntStatus(jpeg_context_t *handle)
{
	uint32_t IntStatus = 0;
	IntStatus = JPEG_REGREAD(handle, JPEG_INT_STATUS);
	return IntStatus;
}

uint32_t JPEG_GetSTMLenStatus(jpeg_context_t *handle)
{
	uint32_t STMLenStatus = 0;
	STMLenStatus = JPEG_REGREAD(handle, JPEG_PROC_STATUS);
	return STMLenStatus;
}

void JPEG_SetIntClear(jpeg_context_t *handle)
{
	JPEG_REGWRITE(handle, JPEG_INT_STATUS, (BIT(0) | BIT(1)));
}

void JPEG_SetIntEnable(jpeg_context_t *handle)
{
	JPEG_REGWRITE(handle, JPEG_INT_EN, (BIT(0) | BIT(1)));
}

void JPEG_SetEncDecStart(jpeg_context_t *handle)
{
	JPEG_REGWRITE(handle, JPEG_REG0, (BIT(0) | BIT(31)));
}

void JPEG_SetReg1(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG1, val);
}

void JPEG_SetNMCU(jpeg_context_t *handle, uint32_t NMCU)
{
	JPEG_REGWRITE(handle, JPEG_REG2, NMCU);
}

void JPEG_SetReg3(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG3, val);
}

void JPEG_SetReg4(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG4, val);
}

void JPEG_SetReg5(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG5, val);
}

void JPEG_SetReg6(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG6, val);
}

void JPEG_SetSTMAddr0(jpeg_context_t *handle, uint32_t STMAddr0)
{
	JPEG_REGWRITE(handle, JPEG_STM_ADDR0, STMAddr0);
}

void JPEG_SetSTMAddr1(jpeg_context_t *handle, uint32_t STMAddr1)
{
	JPEG_REGWRITE(handle, JPEG_STM_ADDR1, STMAddr1);
}

void JPEG_SetCurLumaOffset(jpeg_context_t *handle, uint32_t CurLumaOffset)
{
	JPEG_REGWRITE(handle, JPEG_REG8, CurLumaOffset);
}

void JPEG_SetCurChromaOffset(jpeg_context_t *handle, uint32_t CurChromaOffset)
{
	JPEG_REGWRITE(handle, JPEG_REG9, CurChromaOffset);
}

void JPEG_SetReg10(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG10, val);
}

void JPEG_SetBuff0Size(jpeg_context_t *handle, uint32_t Buff0Size)
{
	JPEG_REGWRITE(handle, JPEG_BUFF0_SIZE, Buff0Size);
}

void JPEG_SetBuff1Size(jpeg_context_t *handle, uint32_t Buff1Size)
{
	JPEG_REGWRITE(handle, JPEG_BUFF1_SIZE, Buff1Size);
}

void JPEG_SetReg11(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG11, val);
}

void JPEG_SetOutLengthMux(jpeg_context_t *handle, uint32_t val)
{
	JPEG_REGWRITE(handle, JPEG_REG12, ((val << 31) & BIT(31)));
}

uint32_t JPEG_GetOutLength(jpeg_context_t *handle)
{
	uint32_t val = 0;
	uint32_t OutLength = 0;
	val = JPEG_REGREAD(handle, JPEG_REG12);
	OutLength = val & 0xFFFFFF;
	return OutLength;
}

void JPEG_SetLumaQMem(jpeg_context_t *handle, uint32_t *LumaQTbl)
{
	JPEG_REGWRITE(handle, 0x80, *((uint32_t *)LumaQTbl + 0));
	JPEG_REGWRITE(handle, 0x84, *((uint32_t *)LumaQTbl + 1));
	JPEG_REGWRITE(handle, 0x88, *((uint32_t *)LumaQTbl + 2));
	JPEG_REGWRITE(handle, 0x8c, *((uint32_t *)LumaQTbl + 3));
	JPEG_REGWRITE(handle, 0x90, *((uint32_t *)LumaQTbl + 4));
	JPEG_REGWRITE(handle, 0x94, *((uint32_t *)LumaQTbl + 5));
	JPEG_REGWRITE(handle, 0x98, *((uint32_t *)LumaQTbl + 6));
	JPEG_REGWRITE(handle, 0x9c, *((uint32_t *)LumaQTbl + 7));
	JPEG_REGWRITE(handle, 0xa0, *((uint32_t *)LumaQTbl + 8));
	JPEG_REGWRITE(handle, 0xa4, *((uint32_t *)LumaQTbl + 9));
	JPEG_REGWRITE(handle, 0xa8, *((uint32_t *)LumaQTbl + 10));
	JPEG_REGWRITE(handle, 0xac, *((uint32_t *)LumaQTbl + 11));
	JPEG_REGWRITE(handle, 0xb0, *((uint32_t *)LumaQTbl + 12));
	JPEG_REGWRITE(handle, 0xb4, *((uint32_t *)LumaQTbl + 13));
	JPEG_REGWRITE(handle, 0xb8, *((uint32_t *)LumaQTbl + 14));
	JPEG_REGWRITE(handle, 0xbc, *((uint32_t *)LumaQTbl + 15));
}

void JPEG_SetChromaQMem(jpeg_context_t *handle, uint32_t *ChromaQTbl)
{
	JPEG_REGWRITE(handle, 0xc0, *((uint32_t *)ChromaQTbl + 0));
	JPEG_REGWRITE(handle, 0xc4, *((uint32_t *)ChromaQTbl + 1));
	JPEG_REGWRITE(handle, 0xc8, *((uint32_t *)ChromaQTbl + 2));
	JPEG_REGWRITE(handle, 0xcc, *((uint32_t *)ChromaQTbl + 3));
	JPEG_REGWRITE(handle, 0xd0, *((uint32_t *)ChromaQTbl + 4));
	JPEG_REGWRITE(handle, 0xd4, *((uint32_t *)ChromaQTbl + 5));
	JPEG_REGWRITE(handle, 0xd8, *((uint32_t *)ChromaQTbl + 6));
	JPEG_REGWRITE(handle, 0xdc, *((uint32_t *)ChromaQTbl + 7));
	JPEG_REGWRITE(handle, 0xe0, *((uint32_t *)ChromaQTbl + 8));
	JPEG_REGWRITE(handle, 0xe4, *((uint32_t *)ChromaQTbl + 9));
	JPEG_REGWRITE(handle, 0xe8, *((uint32_t *)ChromaQTbl + 10));
	JPEG_REGWRITE(handle, 0xec, *((uint32_t *)ChromaQTbl + 11));
	JPEG_REGWRITE(handle, 0xf0, *((uint32_t *)ChromaQTbl + 12));
	JPEG_REGWRITE(handle, 0xf4, *((uint32_t *)ChromaQTbl + 13));
	JPEG_REGWRITE(handle, 0xf8, *((uint32_t *)ChromaQTbl + 14));
	JPEG_REGWRITE(handle, 0xfc, *((uint32_t *)ChromaQTbl + 15));
}

void JPEG_SetQMem(jpeg_context_t *handle)
{
	JPEG_REGWRITE(handle, 0x80, 0x0807070a);
	JPEG_REGWRITE(handle, 0x84, 0x080a0607);
	JPEG_REGWRITE(handle, 0x88, 0x0a0b0808);
	JPEG_REGWRITE(handle, 0x8c, 0x180e0b0a);
	JPEG_REGWRITE(handle, 0x90, 0x0d0d0e10);
	JPEG_REGWRITE(handle, 0x94, 0x16151d0e);
	JPEG_REGWRITE(handle, 0x98, 0x1f231811);
	JPEG_REGWRITE(handle, 0x9c, 0x1f222425);
	JPEG_REGWRITE(handle, 0xa0, 0x2b262122);
	JPEG_REGWRITE(handle, 0xa4, 0x29262f37);
	JPEG_REGWRITE(handle, 0xa8, 0x22212934);
	JPEG_REGWRITE(handle, 0xac, 0x34314130);
	JPEG_REGWRITE(handle, 0xb0, 0x3e3e3b39);
	JPEG_REGWRITE(handle, 0xb4, 0x442e253e);
	JPEG_REGWRITE(handle, 0xb8, 0x483c4349);
	JPEG_REGWRITE(handle, 0xbc, 0x3b3e3d37);
	JPEG_REGWRITE(handle, 0xc0, 0x0e0b0b0a);
	JPEG_REGWRITE(handle, 0xc4, 0x101c0e0d);
	JPEG_REGWRITE(handle, 0xc8, 0x283b1c10);
	JPEG_REGWRITE(handle, 0xcc, 0x3b3b2822);
	JPEG_REGWRITE(handle, 0xd0, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xd4, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xd8, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xdc, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xe0, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xe4, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xe8, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xec, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xf0, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xf4, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xf8, 0x3b3b3b3b);
	JPEG_REGWRITE(handle, 0xfc, 0x3b3b3b3b);
}

void JPEG_SetHuffEnc(jpeg_context_t *handle)
{
	JPEG_REGWRITE(handle, 0x100, 0x01010100);
	JPEG_REGWRITE(handle, 0x104, 0x030b0204);
	JPEG_REGWRITE(handle, 0x108, 0x0678041a);
	JPEG_REGWRITE(handle, 0x10c, 0x09f607f8);
	JPEG_REGWRITE(handle, 0x110, 0x0f830f82);
	JPEG_REGWRITE(handle, 0x114, 0x041b030c);
	JPEG_REGWRITE(handle, 0x118, 0x08f60679);
	JPEG_REGWRITE(handle, 0x11c, 0x0f840af6);
	JPEG_REGWRITE(handle, 0x120, 0x0f860f85);
	JPEG_REGWRITE(handle, 0x124, 0x0f880f87);
	JPEG_REGWRITE(handle, 0x128, 0x07f9041c);
	JPEG_REGWRITE(handle, 0x12c, 0x0bf409f7);
	JPEG_REGWRITE(handle, 0x130, 0x0f8a0f89);
	JPEG_REGWRITE(handle, 0x134, 0x0f8c0f8b);
	JPEG_REGWRITE(handle, 0x138, 0x0f8e0f8d);
	JPEG_REGWRITE(handle, 0x13c, 0x08f7053a);
	JPEG_REGWRITE(handle, 0x140, 0x0f8f0bf5);
	JPEG_REGWRITE(handle, 0x144, 0x0f910f90);
	JPEG_REGWRITE(handle, 0x148, 0x0f930f92);
	JPEG_REGWRITE(handle, 0x14c, 0x0f950f94);
	JPEG_REGWRITE(handle, 0x150, 0x09f8053b);
	JPEG_REGWRITE(handle, 0x154, 0x0f970f96);
	JPEG_REGWRITE(handle, 0x158, 0x0f990f98);
	JPEG_REGWRITE(handle, 0x15c, 0x0f9b0f9a);
	JPEG_REGWRITE(handle, 0x160, 0x0f9d0f9c);
	JPEG_REGWRITE(handle, 0x164, 0x0af7067a);
	JPEG_REGWRITE(handle, 0x168, 0x0f9f0f9e);
	JPEG_REGWRITE(handle, 0x16c, 0x0fa10fa0);
	JPEG_REGWRITE(handle, 0x170, 0x0fa30fa2);
	JPEG_REGWRITE(handle, 0x174, 0x0fa50fa4);
	JPEG_REGWRITE(handle, 0x178, 0x0bf6067b);
	JPEG_REGWRITE(handle, 0x17c, 0x0fa70fa6);
	JPEG_REGWRITE(handle, 0x180, 0x0fa90fa8);
	JPEG_REGWRITE(handle, 0x184, 0x0fab0faa);
	JPEG_REGWRITE(handle, 0x188, 0x0fad0fac);
	JPEG_REGWRITE(handle, 0x18c, 0x0bf707fa);
	JPEG_REGWRITE(handle, 0x190, 0x0faf0fae);
	JPEG_REGWRITE(handle, 0x194, 0x0fb10fb0);
	JPEG_REGWRITE(handle, 0x198, 0x0fb30fb2);
	JPEG_REGWRITE(handle, 0x19c, 0x0fb50fb4);
	JPEG_REGWRITE(handle, 0x1a0, 0x0ec008f8);
	JPEG_REGWRITE(handle, 0x1a4, 0x0fb70fb6);
	JPEG_REGWRITE(handle, 0x1a8, 0x0fb90fb8);
	JPEG_REGWRITE(handle, 0x1ac, 0x0fbb0fba);
	JPEG_REGWRITE(handle, 0x1b0, 0x0fbd0fbc);
	JPEG_REGWRITE(handle, 0x1b4, 0x0fbe08f9);
	JPEG_REGWRITE(handle, 0x1b8, 0x0fc00fbf);
	JPEG_REGWRITE(handle, 0x1bc, 0x0fc20fc1);
	JPEG_REGWRITE(handle, 0x1c0, 0x0fc40fc3);
	JPEG_REGWRITE(handle, 0x1c4, 0x0fc60fc5);
	JPEG_REGWRITE(handle, 0x1c8, 0x0fc708fa);
	JPEG_REGWRITE(handle, 0x1cc, 0x0fc90fc8);
	JPEG_REGWRITE(handle, 0x1d0, 0x0fcb0fca);
	JPEG_REGWRITE(handle, 0x1d4, 0x0fcd0fcc);
	JPEG_REGWRITE(handle, 0x1d8, 0x0fcf0fce);
	JPEG_REGWRITE(handle, 0x1dc, 0x0fd009f9);
	JPEG_REGWRITE(handle, 0x1e0, 0x0fd20fd1);
	JPEG_REGWRITE(handle, 0x1e4, 0x0fd40fd3);
	JPEG_REGWRITE(handle, 0x1e8, 0x0fd60fd5);
	JPEG_REGWRITE(handle, 0x1ec, 0x0fd80fd7);
	JPEG_REGWRITE(handle, 0x1f0, 0x0fd909fa);
	JPEG_REGWRITE(handle, 0x1f4, 0x0fdb0fda);
	JPEG_REGWRITE(handle, 0x1f8, 0x0fdd0fdc);
	JPEG_REGWRITE(handle, 0x1fc, 0x0fdf0fde);
	JPEG_REGWRITE(handle, 0x200, 0x0fe10fe0);
	JPEG_REGWRITE(handle, 0x204, 0x0fe20af8);
	JPEG_REGWRITE(handle, 0x208, 0x0fe40fe3);
	JPEG_REGWRITE(handle, 0x20c, 0x0fe60fe5);
	JPEG_REGWRITE(handle, 0x210, 0x0fe80fe7);
	JPEG_REGWRITE(handle, 0x214, 0x0fea0fe9);
	JPEG_REGWRITE(handle, 0x218, 0x0fec0feb);
	JPEG_REGWRITE(handle, 0x21c, 0x0fee0fed);
	JPEG_REGWRITE(handle, 0x220, 0x0ff00fef);
	JPEG_REGWRITE(handle, 0x224, 0x0ff20ff1);
	JPEG_REGWRITE(handle, 0x228, 0x0ff40ff3);
	JPEG_REGWRITE(handle, 0x22c, 0x0ff60ff5);
	JPEG_REGWRITE(handle, 0x230, 0x0ff80ff7);
	JPEG_REGWRITE(handle, 0x234, 0x0ffa0ff9);
	JPEG_REGWRITE(handle, 0x238, 0x0ffc0ffb);
	JPEG_REGWRITE(handle, 0x23c, 0x0ffe0ffd);
	JPEG_REGWRITE(handle, 0x240, 0x0af9030a);
	JPEG_REGWRITE(handle, 0x244, 0x0fff0fff);
	JPEG_REGWRITE(handle, 0x248, 0x0fff0fff);
	JPEG_REGWRITE(handle, 0x24c, 0x0fff0fff);
	JPEG_REGWRITE(handle, 0x250, 0x0fd10fd0);
	JPEG_REGWRITE(handle, 0x254, 0x0fd30fd2);
	JPEG_REGWRITE(handle, 0x258, 0x0fd50fd4);
	JPEG_REGWRITE(handle, 0x25c, 0x0fd70fd6);
	JPEG_REGWRITE(handle, 0x260, 0x02040101);
	JPEG_REGWRITE(handle, 0x264, 0x0418030a);
	JPEG_REGWRITE(handle, 0x268, 0x05380419);
	JPEG_REGWRITE(handle, 0x26c, 0x08f40678);
	JPEG_REGWRITE(handle, 0x270, 0x0bf409f6);
	JPEG_REGWRITE(handle, 0x274, 0x0539030b);
	JPEG_REGWRITE(handle, 0x278, 0x08f507f6);
	JPEG_REGWRITE(handle, 0x27c, 0x0bf50af6);
	JPEG_REGWRITE(handle, 0x280, 0x0f890f88);
	JPEG_REGWRITE(handle, 0x284, 0x0f8b0f8a);
	JPEG_REGWRITE(handle, 0x288, 0x07f7041a);
	JPEG_REGWRITE(handle, 0x28c, 0x0bf609f7);
	JPEG_REGWRITE(handle, 0x290, 0x0f8c0ec2);
	JPEG_REGWRITE(handle, 0x294, 0x0f8e0f8d);
	JPEG_REGWRITE(handle, 0x298, 0x0f900f8f);
	JPEG_REGWRITE(handle, 0x29c, 0x07f8041b);
	JPEG_REGWRITE(handle, 0x2a0, 0x0bf709f8);
	JPEG_REGWRITE(handle, 0x2a4, 0x0f920f91);
	JPEG_REGWRITE(handle, 0x2a8, 0x0f940f93);
	JPEG_REGWRITE(handle, 0x2ac, 0x0f960f95);
	JPEG_REGWRITE(handle, 0x2b0, 0x08f6053a);
	JPEG_REGWRITE(handle, 0x2b4, 0x0f980f97);
	JPEG_REGWRITE(handle, 0x2b8, 0x0f9a0f99);
	JPEG_REGWRITE(handle, 0x2bc, 0x0f9c0f9b);
	JPEG_REGWRITE(handle, 0x2c0, 0x0f9e0f9d);
	JPEG_REGWRITE(handle, 0x2c4, 0x09f9053b);
	JPEG_REGWRITE(handle, 0x2c8, 0x0fa00f9f);
	JPEG_REGWRITE(handle, 0x2cc, 0x0fa20fa1);
	JPEG_REGWRITE(handle, 0x2d0, 0x0fa40fa3);
	JPEG_REGWRITE(handle, 0x2d4, 0x0fa60fa5);
	JPEG_REGWRITE(handle, 0x2d8, 0x0af70679);
	JPEG_REGWRITE(handle, 0x2dc, 0x0fa80fa7);
	JPEG_REGWRITE(handle, 0x2e0, 0x0faa0fa9);
	JPEG_REGWRITE(handle, 0x2e4, 0x0fac0fab);
	JPEG_REGWRITE(handle, 0x2e8, 0x0fae0fad);
	JPEG_REGWRITE(handle, 0x2ec, 0x0af8067a);
	JPEG_REGWRITE(handle, 0x2f0, 0x0fb00faf);
	JPEG_REGWRITE(handle, 0x2f4, 0x0fb20fb1);
	JPEG_REGWRITE(handle, 0x2f8, 0x0fb40fb3);
	JPEG_REGWRITE(handle, 0x2fc, 0x0fb60fb5);
	JPEG_REGWRITE(handle, 0x300, 0x0fb707f9);
	JPEG_REGWRITE(handle, 0x304, 0x0fb90fb8);
	JPEG_REGWRITE(handle, 0x308, 0x0fbb0fba);
	JPEG_REGWRITE(handle, 0x30c, 0x0fbd0fbc);
	JPEG_REGWRITE(handle, 0x310, 0x0fbf0fbe);
	JPEG_REGWRITE(handle, 0x314, 0x0fc008f7);
	JPEG_REGWRITE(handle, 0x318, 0x0fc20fc1);
	JPEG_REGWRITE(handle, 0x31c, 0x0fc40fc3);
	JPEG_REGWRITE(handle, 0x320, 0x0fc60fc5);
	JPEG_REGWRITE(handle, 0x324, 0x0fc80fc7);
	JPEG_REGWRITE(handle, 0x328, 0x0fc908f8);
	JPEG_REGWRITE(handle, 0x32c, 0x0fcb0fca);
	JPEG_REGWRITE(handle, 0x330, 0x0fcd0fcc);
	JPEG_REGWRITE(handle, 0x334, 0x0fcf0fce);
	JPEG_REGWRITE(handle, 0x338, 0x0fd10fd0);
	JPEG_REGWRITE(handle, 0x33c, 0x0fd208f9);
	JPEG_REGWRITE(handle, 0x340, 0x0fd40fd3);
	JPEG_REGWRITE(handle, 0x344, 0x0fd60fd5);
	JPEG_REGWRITE(handle, 0x348, 0x0fd80fd7);
	JPEG_REGWRITE(handle, 0x34c, 0x0fda0fd9);
	JPEG_REGWRITE(handle, 0x350, 0x0fdb08fa);
	JPEG_REGWRITE(handle, 0x354, 0x0fdd0fdc);
	JPEG_REGWRITE(handle, 0x358, 0x0fdf0fde);
	JPEG_REGWRITE(handle, 0x35c, 0x0fe10fe0);
	JPEG_REGWRITE(handle, 0x360, 0x0fe30fe2);
	JPEG_REGWRITE(handle, 0x364, 0x0fe40af9);
	JPEG_REGWRITE(handle, 0x368, 0x0fe60fe5);
	JPEG_REGWRITE(handle, 0x36c, 0x0fe80fe7);
	JPEG_REGWRITE(handle, 0x370, 0x0fea0fe9);
	JPEG_REGWRITE(handle, 0x374, 0x0fec0feb);
	JPEG_REGWRITE(handle, 0x378, 0x0fed0de0);
	JPEG_REGWRITE(handle, 0x37c, 0x0fef0fee);
	JPEG_REGWRITE(handle, 0x380, 0x0ff10ff0);
	JPEG_REGWRITE(handle, 0x384, 0x0ff30ff2);
	JPEG_REGWRITE(handle, 0x388, 0x0ff50ff4);
	JPEG_REGWRITE(handle, 0x38c, 0x0ff60ec3);
	JPEG_REGWRITE(handle, 0x390, 0x0ff80ff7);
	JPEG_REGWRITE(handle, 0x394, 0x0ffa0ff9);
	JPEG_REGWRITE(handle, 0x398, 0x0ffc0ffb);
	JPEG_REGWRITE(handle, 0x39c, 0x0ffe0ffd);
	JPEG_REGWRITE(handle, 0x3a0, 0x09fa0100);
	JPEG_REGWRITE(handle, 0x3a4, 0x0fff0fff);
	JPEG_REGWRITE(handle, 0x3a8, 0x0fff0fff);
	JPEG_REGWRITE(handle, 0x3ac, 0x0fff0fff);
	JPEG_REGWRITE(handle, 0x3b0, 0x0fd10fd0);
	JPEG_REGWRITE(handle, 0x3b4, 0x0fd30fd2);
	JPEG_REGWRITE(handle, 0x3b8, 0x0fd50fd4);
	JPEG_REGWRITE(handle, 0x3bc, 0x0fd70fd6);
	JPEG_REGWRITE(handle, 0x3c0, 0x02020100);
	JPEG_REGWRITE(handle, 0x3c4, 0x02040203);
	JPEG_REGWRITE(handle, 0x3c8, 0x02060205);
	JPEG_REGWRITE(handle, 0x3cc, 0x041e030e);
	JPEG_REGWRITE(handle, 0x3d0, 0x067e053e);
	JPEG_REGWRITE(handle, 0x3d4, 0x08fe07fe);
	JPEG_REGWRITE(handle, 0x3d8, 0x00000000);
	JPEG_REGWRITE(handle, 0x3dc, 0x00000000);
	JPEG_REGWRITE(handle, 0x3e0, 0x01010100);
	JPEG_REGWRITE(handle, 0x3e4, 0x02060102);
	JPEG_REGWRITE(handle, 0x3e8, 0x041e030e);
	JPEG_REGWRITE(handle, 0x3ec, 0x067e053e);
	JPEG_REGWRITE(handle, 0x3f0, 0x08fe07fe);
	JPEG_REGWRITE(handle, 0x3f4, 0x0afe09fe);
	JPEG_REGWRITE(handle, 0x3f8, 0x00000000);
	JPEG_REGWRITE(handle, 0x3fc, 0x00000000);
}

void JPEG_SetDHTMem(jpeg_context_t *handle)
{
	JPEG_REGWRITE(handle, 0x400, 0x01050100);
	JPEG_REGWRITE(handle, 0x404, 0x01010101);
	JPEG_REGWRITE(handle, 0x408, 0x00000001);
	JPEG_REGWRITE(handle, 0x40c, 0x00000000);
	JPEG_REGWRITE(handle, 0x410, 0x03020100);
	JPEG_REGWRITE(handle, 0x414, 0x07060504);
	JPEG_REGWRITE(handle, 0x418, 0x0b0a0908);
	JPEG_REGWRITE(handle, 0x41c, 0x03010200);
	JPEG_REGWRITE(handle, 0x420, 0x03040203);
	JPEG_REGWRITE(handle, 0x424, 0x04040505);
	JPEG_REGWRITE(handle, 0x428, 0x7d010000);
	JPEG_REGWRITE(handle, 0x42c, 0x00030201);
	JPEG_REGWRITE(handle, 0x430, 0x12051104);
	JPEG_REGWRITE(handle, 0x434, 0x06413121);
	JPEG_REGWRITE(handle, 0x438, 0x07615113);
	JPEG_REGWRITE(handle, 0x43c, 0x32147122);
	JPEG_REGWRITE(handle, 0x440, 0x08a19181);
	JPEG_REGWRITE(handle, 0x444, 0xc1b14223);
	JPEG_REGWRITE(handle, 0x448, 0xf0d15215);
	JPEG_REGWRITE(handle, 0x44c, 0x72623324);
	JPEG_REGWRITE(handle, 0x450, 0x160a0982);
	JPEG_REGWRITE(handle, 0x454, 0x1a191817);
	JPEG_REGWRITE(handle, 0x458, 0x28272625);
	JPEG_REGWRITE(handle, 0x45c, 0x35342a29);
	JPEG_REGWRITE(handle, 0x460, 0x39383736);
	JPEG_REGWRITE(handle, 0x464, 0x4544433a);
	JPEG_REGWRITE(handle, 0x468, 0x49484746);
	JPEG_REGWRITE(handle, 0x46c, 0x5554534a);
	JPEG_REGWRITE(handle, 0x470, 0x59585756);
	JPEG_REGWRITE(handle, 0x474, 0x6564635a);
	JPEG_REGWRITE(handle, 0x478, 0x69686766);
	JPEG_REGWRITE(handle, 0x47c, 0x7574736a);
	JPEG_REGWRITE(handle, 0x480, 0x79787776);
	JPEG_REGWRITE(handle, 0x484, 0x8584837a);
	JPEG_REGWRITE(handle, 0x488, 0x89888786);
	JPEG_REGWRITE(handle, 0x48c, 0x9493928a);
	JPEG_REGWRITE(handle, 0x490, 0x98979695);
	JPEG_REGWRITE(handle, 0x494, 0xa3a29a99);
	JPEG_REGWRITE(handle, 0x498, 0xa7a6a5a4);
	JPEG_REGWRITE(handle, 0x49c, 0xb2aaa9a8);
	JPEG_REGWRITE(handle, 0x4a0, 0xb6b5b4b3);
	JPEG_REGWRITE(handle, 0x4a4, 0xbab9b8b7);
	JPEG_REGWRITE(handle, 0x4a8, 0xc5c4c3c2);
	JPEG_REGWRITE(handle, 0x4ac, 0xc9c8c7c6);
	JPEG_REGWRITE(handle, 0x4b0, 0xd4d3d2ca);
	JPEG_REGWRITE(handle, 0x4b4, 0xd8d7d6d5);
	JPEG_REGWRITE(handle, 0x4b8, 0xe2e1dad9);
	JPEG_REGWRITE(handle, 0x4bc, 0xe6e5e4e3);
	JPEG_REGWRITE(handle, 0x4c0, 0xeae9e8e7);
	JPEG_REGWRITE(handle, 0x4c4, 0xf4f3f2f1);
	JPEG_REGWRITE(handle, 0x4c8, 0xf8f7f6f5);
	JPEG_REGWRITE(handle, 0x4cc, 0x0300faf9);
	JPEG_REGWRITE(handle, 0x4d0, 0x01010101);
	JPEG_REGWRITE(handle, 0x4d4, 0x01010101);
	JPEG_REGWRITE(handle, 0x4d8, 0x00000001);
	JPEG_REGWRITE(handle, 0x4dc, 0x01000000);
	JPEG_REGWRITE(handle, 0x4e0, 0x05040302);
	JPEG_REGWRITE(handle, 0x4e4, 0x09080706);
	JPEG_REGWRITE(handle, 0x4e8, 0x02000b0a);
	JPEG_REGWRITE(handle, 0x4ec, 0x04040201);
	JPEG_REGWRITE(handle, 0x4f0, 0x05070403);
	JPEG_REGWRITE(handle, 0x4f4, 0x01000404);
	JPEG_REGWRITE(handle, 0x4f8, 0x01007702);
	JPEG_REGWRITE(handle, 0x4fc, 0x04110302);
	JPEG_REGWRITE(handle, 0x500, 0x06312105);
	JPEG_REGWRITE(handle, 0x504, 0x07514112);
	JPEG_REGWRITE(handle, 0x508, 0x22137161);
	JPEG_REGWRITE(handle, 0x50c, 0x14088132);
	JPEG_REGWRITE(handle, 0x510, 0xb1a19142);
	JPEG_REGWRITE(handle, 0x514, 0x332309c1);
	JPEG_REGWRITE(handle, 0x518, 0x6215f052);
	JPEG_REGWRITE(handle, 0x51c, 0x160ad172);
	JPEG_REGWRITE(handle, 0x520, 0x25e13424);
	JPEG_REGWRITE(handle, 0x524, 0x191817f1);
	JPEG_REGWRITE(handle, 0x528, 0x2827261a);
	JPEG_REGWRITE(handle, 0x52c, 0x36352a29);
	JPEG_REGWRITE(handle, 0x530, 0x3a393837);
	JPEG_REGWRITE(handle, 0x534, 0x46454443);
	JPEG_REGWRITE(handle, 0x538, 0x4a494847);
	JPEG_REGWRITE(handle, 0x53c, 0x56555453);
	JPEG_REGWRITE(handle, 0x540, 0x5a595857);
	JPEG_REGWRITE(handle, 0x544, 0x66656463);
	JPEG_REGWRITE(handle, 0x548, 0x6a696867);
	JPEG_REGWRITE(handle, 0x54c, 0x76757473);
	JPEG_REGWRITE(handle, 0x550, 0x7a797877);
	JPEG_REGWRITE(handle, 0x554, 0x85848382);
	JPEG_REGWRITE(handle, 0x558, 0x89888786);
	JPEG_REGWRITE(handle, 0x55c, 0x9493928a);
	JPEG_REGWRITE(handle, 0x560, 0x98979695);
	JPEG_REGWRITE(handle, 0x564, 0xa3a29a99);
	JPEG_REGWRITE(handle, 0x568, 0xa7a6a5a4);
	JPEG_REGWRITE(handle, 0x56c, 0xb2aaa9a8);
	JPEG_REGWRITE(handle, 0x570, 0xb6b5b4b3);
	JPEG_REGWRITE(handle, 0x574, 0xbab9b8b7);
	JPEG_REGWRITE(handle, 0x578, 0xc5c4c3c2);
	JPEG_REGWRITE(handle, 0x57c, 0xc9c8c7c6);
	JPEG_REGWRITE(handle, 0x580, 0xd4d3d2ca);
	JPEG_REGWRITE(handle, 0x584, 0xd8d7d6d5);
	JPEG_REGWRITE(handle, 0x588, 0xe3e2dad9);
	JPEG_REGWRITE(handle, 0x58c, 0xe7e6e5e4);
	JPEG_REGWRITE(handle, 0x590, 0xf2eae9e8);
	JPEG_REGWRITE(handle, 0x594, 0xf6f5f4f3);
	JPEG_REGWRITE(handle, 0x598, 0xfaf9f8f7);
}


