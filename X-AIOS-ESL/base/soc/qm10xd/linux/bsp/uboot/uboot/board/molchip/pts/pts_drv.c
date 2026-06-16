#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>

#define GET_REG(x)		__raw_readl(x)
#define SET_REG(a, v)		__raw_writel(v, a)

//extern void mc_printf(const char *format, ...);

void pts_init(void)
{
	int i;
	unsigned int reg, reg1;

	//mc_printf("Init PTS\r\n");

	/* init done? */
	reg = GET_REG(REG_CPU_SYS_CLK_CTRL);
	reg1 = GET_REG(REG_CEN_PERI_CLK_CTRL);
	if((reg & CKG_PTS_EN) && (reg1 & CKG_APB_PTS_EN))
		return;

	/* apb access pts register clk disable */
	reg = GET_REG(REG_CEN_PERI_CLK_CTRL);
	SET_REG(REG_CEN_PERI_CLK_CTRL, reg & (~CKG_APB_PTS_EN));

	/* pts ip clock disable */
	reg = GET_REG(REG_CPU_SYS_CLK_CTRL);
	SET_REG(REG_CPU_SYS_CLK_CTRL, reg & (~CKG_PTS_EN));

	/* set pts freq to 1Mhz */
	#if 1
	reg = (1 << CKG_PTS_SEL_SHIFT);
	reg |= (0x17 << CKG_PTS_DIV_SHIFT);
	#else
	reg = (2 << CKG_PTS_SEL_SHIFT);
	reg |= (0x31 << CKG_PTS_DIV_SHIFT);
	#endif
	SET_REG(REG_CKG_PTS_CFG, reg);

	/* pts ip clock enable */
	reg = GET_REG(REG_CPU_SYS_CLK_CTRL);
	SET_REG(REG_CPU_SYS_CLK_CTRL, reg | CKG_PTS_EN);

	/*pts reset */
	reg = GET_REG(REG_AP_PERI_SOFT_RST0);
	SET_REG(REG_AP_PERI_SOFT_RST0, reg | PTS_SOFT_RST);

	for (i = 0; i < 100; i++)
		;

	/*pts unreset */
	SET_REG(REG_AP_PERI_SOFT_RST0, reg & (~PTS_SOFT_RST));

	/* apb access pts register clk enable */
	reg = GET_REG(REG_CEN_PERI_CLK_CTRL);
	SET_REG(REG_CEN_PERI_CLK_CTRL, reg | CKG_APB_PTS_EN);
}

unsigned long long pts_get_value(void)
{
	unsigned long long val64;

	SET_REG(REG_PTS_BASE, 0);
	val64 = GET_REG(REG_PTS_RD_HIGH);
	val64 = val64 << 32;
	val64 = val64 | GET_REG(REG_PTS_RD_LOW);
	//mc_printf("pts_get_value_low:0x%x\r\n",(unsigned int)val64);
	//mc_printf("pts_get_value_high:0x%x\r\n",(unsigned int)(val64>>32));

	return val64;
}

unsigned long long pts_get_value_us(void)
{
	return pts_get_value();
}

unsigned long long pts_get_value_ms(void)
{
	return pts_get_value() / 1000;
}

int pts_udelay(unsigned long long  us_delay)
{
	unsigned long long curt_timer = 0;

	curt_timer = pts_get_value_us();
	while ((pts_get_value_us() - curt_timer ) < us_delay )
		;

	return 0;
}
