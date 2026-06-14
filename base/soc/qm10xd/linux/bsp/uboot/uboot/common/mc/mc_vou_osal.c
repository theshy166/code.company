#include "mc_vou_osal.h"

uint32_t readl(uint32_t addr)
{
	uint32_t value = *(volatile uint32_t*)addr;
	return value;
}

void writel(uint32_t value, uint32_t addr)
{
	*(volatile uint32_t*)addr = value;
}

void writels(uint32_t value, uint32_t addr)
{
	uint32_t tmp = readl(addr);
	tmp |= value;
	writel(tmp, addr);
}

void writelc(uint32_t value, uint32_t addr)
{
	uint32_t tmp = readl(addr);
	tmp &= (~value);
	writel(tmp, addr);
}

void writelr(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
	uint32_t tmp = readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
	writel(tmp, addr);
}

/*
static uint8_t readb(uint32_t addr)
{
	uint8_t value = *(volatile uint8_t*)addr;
	return value;
}

static void writeb(uint8_t value, uint32_t addr)
{
	*(volatile uint8_t*)addr = value;
}

static void writebs(uint8_t value, uint32_t addr)
{
	uint8_t tmp = readb(addr);
	tmp |= value;
	writeb(tmp, addr);
}

static void writebc(uint8_t value, uint32_t addr)
{
	uint8_t tmp = readb(addr);
	tmp &= (~value);
	writeb(tmp, addr);
}
*/

