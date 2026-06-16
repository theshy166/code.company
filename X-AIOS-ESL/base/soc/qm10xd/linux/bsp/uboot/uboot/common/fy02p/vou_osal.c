#include "vou_osal.h"

uint32_t readl(uint32_t addr)
{
	uint32_t value = 0;
	
	//printf("[0x%08x]: 0x%08x\n", (uint32_t)addr, value);
	value = *(volatile uint32_t*)addr;
	return value;
}

void writel(uint32_t value, uint32_t addr)
{
	//printk("writel(0x%08x, 0x%08x);\n", value, (uint32_t)addr);
	//printf("0x%08x = 0x%08x\n", (uint32_t)addr, value);
	//printf("data.set sd:0x%08x %%le %%long 0x%04x\n", addr, value);
	*(uint32_t*)addr = value;
//	printf("[%x]=0x%08x\n",addr,value);
}

uint8_t readb(uint32_t addr)
{
	uint8_t value = *(volatile uint8_t*)addr;
	return value;
}

void writeb(uint8_t value, uint32_t addr)
{
	*(volatile uint8_t*)addr = value;
//	printf("[%x]=0x%08x\n",addr,value);

}


void writelc(uint32_t value, uint32_t addr)
{
//	printk("writelc(0x%08x, 0x%08x);\n", value, (uint32_t)addr);
	uint32_t tmp = readl(addr);
	tmp &= (~value);
	writel(tmp, addr);
	//*(uint32_t*)addr = tmp;
}

void writels(uint32_t value, uint32_t addr)
{
//	printk("writels(0x%08x, 0x%08x);\n", value, (uint32_t)addr);
	uint32_t tmp = readl(addr);
	tmp |= value;
	writel(tmp, addr);
	//*(uint32_t*)addr = tmp;
}

void writelb(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	// 32bits n =32; 0xFFFFFFFF >> n; causes error
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
//	printf("&tmp=data.long(sd:0x%08x)\n", addr);
//	printk("writelr(0x%08x, 0x%08x, %d, %d);\n", value, (uint32_t)addr, start, bit);
//	printf("&tmp=&tmp&0x%08x\n", region);
//	printf("&tmp=&tmp|0x%08x\n", (~region) & (value << start));
//	printf("data.set sd:0x%08x %%le %%long &tmp\n", addr);
	uint32_t tmp = readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
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

void writebs(uint8_t value, uint32_t addr)
{
	
	uint8_t tmp = readb(addr);
	tmp |= value;
	writeb(tmp, addr);
}

void writebc(uint8_t value, uint32_t addr)
{
	uint8_t tmp = readb(addr);
	tmp &= (~value);
	writeb(tmp, addr);
}

uint32_t vou_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readl((uint32_t)(regbase->base + base_addr + offset));
}

void vou_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writel(value, (uint32_t)(regbase->base + base_addr + offset));
}

uint32_t topapb_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readl((uint32_t)(regbase->topapb + base_addr + offset));
}

void topapb_write_register_bit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit)
{
	writelb(value, (uint32_t)(regbase->topapb + base_addr + offset), start, bit);
}

uint32_t topclk_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readl((uint32_t)(regbase->topclk + base_addr + offset));
}

void topclk_write_register_bit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit)
{
	writelb(value, (uint32_t)(regbase->topclk + base_addr + offset), start, bit);
}

void topclk_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writel(value, (uint32_t)(regbase->topclk + base_addr + offset));
}

void topclk_write_register_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writels(value, (uint32_t)(regbase->topclk + base_addr + offset));
}

void topclk_write_register_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writelc(value, (uint32_t)(regbase->topclk + base_addr + offset));
}

uint32_t analog_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readl((uint32_t)(regbase->analog + base_addr + offset));
}

void analog_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writel(value, (uint32_t)(regbase->analog + base_addr + offset));
}

uint32_t cvbs_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readl((uint32_t)(regbase->cvbs + base_addr + offset));
}

void cvbs_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writel(value, (uint32_t)(regbase->cvbs + base_addr + offset));
}

uint32_t vioapb_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readl((uint32_t)(regbase->vioapb + base_addr + offset));
}

void vioapb_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writel(value, (uint32_t)(regbase->vioapb + base_addr + offset));
}

void vioapb_write_register_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writelc(value, (uint32_t)(regbase->vioapb + base_addr + offset));
}

void vioapb_write_register_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writels(value, (uint32_t)(regbase->vioapb + base_addr + offset));
}

void vioapb_write_register_bit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit)
{
	writelb(value, (uint32_t)(regbase->vioapb + base_addr + offset), start, bit);
}

uint8_t hdmi_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset)
{
	return readb((uint32_t)(regbase->hdmi + base_addr + offset));
}

void hdmi_write_register_byte(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint8_t value)
{
	writeb(value, (uint32_t)(regbase->hdmi + base_addr + offset));
}

void hdmi_write_register_clrbit(vou_reg_base_t *regbase,  uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writelc(value, (uint32_t)(regbase->hdmi + base_addr + offset));
}

void hdmi_write_register_byte_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint8_t value)
{
	writebs(value, (uint32_t)(regbase->hdmi + base_addr + offset));
}

void hdmi_write_register_byte_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint8_t value)
{
	writebc(value, (uint32_t)(regbase->hdmi + base_addr + offset));
}

void analog_write_register_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writels(value, (uint32_t)(regbase->analog + base_addr + offset));
}

void analog_write_register_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value)
{
	writelc(value, (uint32_t)(regbase->analog + base_addr + offset));
}


