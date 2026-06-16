#include <common.h>
#include <pwm.h>
#include <asm/io.h>
#include <mach/hardware.h>

u32 pwm2_glb = 0;

void pwm2_init(void)
{
	//pwm2_out model
	writel(3, 0x2820009c);

	//config
	writel((~(1 << 2)) & readl(0x1C000000), 0x1C000000);
	writel((~(1 << 2)) & readl(0x1C000008), 0x1C000008);
	writel(0, 0x1C000010);	//disable all interrupt
	writel(1, 0x1C000300);


	//F =25MHz = 100MHz / 4
	writel(0x4, 0x1C000304);
	//duty 50%
	writel(0x4 >> 1, 0x1C000308);

	//enable
	writel((1 << 2)|readl(0x1C000000), 0x1C000000);
	writel((1 << 2)|readl(0x1C000008), 0x1C000008);
}

void pwm_hw_init(void)
{

	pwm2_glb = readl(0x2820009c);

	//clock enable
	writel((1 << 5)| readl(0xC0000AC), 0xC0000AC);
	writel((1 << 7)| readl(0xC0000B0), 0xC0000B0);

	//clock = 100MHz
	writel(2, 0x19F0028C);

	pwm2_init();

}

void close_upwm2(void)
{
	//close pwm2
	writel((~(1 << 2)) & readl(0x1C000000), 0x1C000000);
	writel((~(1 << 2)) & readl(0x1C000008), 0x1C000008);

	//clock = 24MHz
	writel(0, 0x19F0028C);

	//close clock
	writel((~(1 << 19)) & readl(0x18000000), 0x18000000);
	writel((~(1 << 21)) & readl(0x18000004), 0x18000004);

	// model
	writel(pwm2_glb, 0x2820009c);

}
