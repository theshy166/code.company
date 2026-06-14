#include <common.h>


uint32_t qua_readl(uint32_t addr)
{
	uint32_t value = *(volatile uint32_t*)addr;
	return value;
}

void qua_writel(uint32_t value, uint32_t addr)
{
	*(volatile uint32_t*)addr = value;
}

void qua_writels(uint32_t value, uint32_t addr)
{
	uint32_t tmp = qua_readl(addr);
	tmp |= value;
	qua_writel(tmp, addr);
}

void qua_writelc(uint32_t value, uint32_t addr)
{
	uint32_t tmp = qua_readl(addr);
	tmp &= (~value);
	qua_writel(tmp, addr);
}

void qua_writelr(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
	uint32_t tmp = qua_readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
	qua_writel(tmp, addr);
}

int qua_gpio_request(unsigned gpio_num, const char *label) { return 0;}

void qua_gpio_free(unsigned gpio_num) {}

void qua_gpio_direction_output(unsigned int gpio_num)
{
	u8 group = gpio_num / 8;
	u8 offset = gpio_num % 8;
	unsigned long reg_base = 0x0B000000 + (group * 0x10000);
	u32 reg_val = 0;

	reg_val = qua_readl(reg_base + 0x4);
	reg_val |= BIT(offset);
	qua_writel(reg_val, reg_base + 0x4);//GIPO_DMSK 1: MASK

	reg_val = qua_readl(reg_base + 0x8);
	reg_val |= BIT(offset);
	qua_writel(reg_val, reg_base + 0x8);//GPIO_DIR 1:output 0:input
}

void qua_gpio_set_value(unsigned int gpio_num, unsigned int val)
{
	u8 group = gpio_num / 8;
	u8 offset = gpio_num % 8;
	unsigned long reg_base = 0x0B000000 + (group * 0x10000);
	u32 reg_val = 0;

	reg_val = qua_readl(reg_base + 0x0);
	if (val)
		reg_val |= BIT(offset);
	else
		reg_val &= (u32)(~(BIT(offset)));
	qua_writel(reg_val, reg_base + 0x0);//GPIO_DATA 1: high  0: low
}

u32 qua_gpio_get_value(unsigned int gpio_num)
{
	u8 group = gpio_num / 8;
	u8 offset = gpio_num % 8;
	unsigned long reg_base = 0x0B000000 + (group * 0x10000);
	u32 reg_val = 0;

	reg_val = qua_readl(reg_base + 0x0);
	reg_val &= (u32)(BIT(offset));
	return reg_val;
}
