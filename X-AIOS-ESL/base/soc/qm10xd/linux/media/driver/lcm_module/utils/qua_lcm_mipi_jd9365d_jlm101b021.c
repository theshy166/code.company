

#include "mol_lcm_local.h"
#include "dsi_com_param.h"
#include "mol_lcm.h"
#include <linux/delay.h>


#define REG_PIN_MUX_BASE	0x10200000
#define REG_LCD_DE_PIN		0x64
#define REG_LCD_HS_PIN		0x68
#define REG_LCD_VS_PIN		0x6C
#define REG_LCD_D0_PIN		0x74
#define REG_LCD_D1_PIN		0x78

#define REG_PIN_DRV_BASE1	0x0B100000
#define REG_LCD_DE_DRV		0x28
#define REG_LCD_HS_DRV		0x2C
#define REG_LCD_VS_DRV		0x30
#define REG_LCD_D0_DRV		0x38
#define REG_LCD_D1_DRV		0x3C

#define GPIO_NUM_INDEX(x,y)	(8*x + y)
#define GPIO_NUM_LCD_DE		GPIO_NUM_INDEX(3,3)
#define GPIO_NUM_LCD_HS		GPIO_NUM_INDEX(3,4)
#define GPIO_NUM_LCD_VS		GPIO_NUM_INDEX(3,5)
#define GPIO_NUM_LCD_D0		GPIO_NUM_INDEX(3,7)
#define GPIO_NUM_LCD_D1		GPIO_NUM_INDEX(4,0)

#define PIN_MUX_GPIO_ENABLE		(1)

extern unsigned int qua_lcm_readl(unsigned addr);
extern void qua_lcm_writel(unsigned int v, unsigned addr);

extern int  qua_lcm_gpio_request(unsigned gpio, const char *label);
extern void qua_lcm_gpio_free(unsigned gpio);
extern int  qua_lcm_gpio_get_value(unsigned gpio);
extern int  qua_lcm_gpio_direction_input(unsigned gpio);
extern void qua_lcm_gpio_set_value(unsigned gpio, int value);
extern int  qua_lcm_gpio_direction_output(unsigned gpio, int value);

static void __qua_lcm_mipi_jd9365d_hrst(unsigned int vci_pin_reg_oft, unsigned int rst_pin_reg_oft,
	unsigned int vci_pin_gpio_num, unsigned int rst_pin_gpio_num)
{
	void *_vpmuxbase = fhosal_ioremap_nocache(REG_PIN_MUX_BASE, 0x100);
	// volatile unsigned int *_pmux_reg = (volatile unsigned int *)_vpmuxbase;
	unsigned int pmux_reg = (unsigned int)_vpmuxbase;
	printk("%s e\n", __func__);
	/* power on rst timing gpio */
	qua_lcm_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + rst_pin_reg_oft);
	qua_lcm_gpio_request(rst_pin_gpio_num, "rst_gpio");
	qua_lcm_gpio_direction_output(rst_pin_gpio_num, 1);
	qua_lcm_gpio_set_value(rst_pin_gpio_num, 0);

	/* power on vci timing gpio, 0: on, 1: off */
	qua_lcm_writel(PIN_MUX_GPIO_ENABLE, pmux_reg + vci_pin_reg_oft);
	qua_lcm_gpio_request(vci_pin_gpio_num, "vci_gpio");
	qua_lcm_gpio_direction_output(vci_pin_gpio_num, 1);
	qua_lcm_gpio_set_value(vci_pin_gpio_num, 1);
	mdelay(500);
	qua_lcm_gpio_set_value(vci_pin_gpio_num, 0);
	mdelay(10);

	/* power on rst timing */
	// qua_lcm_gpio_set_value(rst_pin_gpio_num, 1);
	// udelay(5);
	// qua_lcm_gpio_set_value(rst_pin_gpio_num, 0);
	// udelay(10);
	qua_lcm_gpio_set_value(rst_pin_gpio_num, 1);
	mdelay(10);

	// qua_lcm_gpio_free(rst_pin_gpio_num);
	// qua_lcm_gpio_free(vci_pin_gpio_num);
	fhosal_iounmap(_vpmuxbase);
	printk("%s x\n", __func__);
}

int qua_lcm_mipi_jd9365d_hrst(int lcm_id)
{
	printk("%s e\n", __func__);
	printk("%s lcm_id=%d\n", __func__, lcm_id);
	if(lcm_id == 0)
		__qua_lcm_mipi_jd9365d_hrst(REG_LCD_D0_PIN, REG_LCD_DE_PIN, GPIO_NUM_LCD_D0, GPIO_NUM_LCD_DE);

	if(lcm_id == 1)
		__qua_lcm_mipi_jd9365d_hrst(REG_LCD_D1_PIN, REG_LCD_HS_PIN, GPIO_NUM_LCD_D1, GPIO_NUM_LCD_HS);
	printk("%s x\n", __func__);
	return 0;
}

#if 0
int32_t qua_lcm_mipi_jd9365d_hrst(void * param)
{
	// int32_t rtn = 0, val = 0, tmp = 0, _v = 0, i = 0, count = 3;
	unsigned long flags;
	void *_vpmuxbase = fhosal_ioremap_nocache(REG_PIN_MUX_BASE, 0x100);
	// void *_vpdrvbase = fhosal_ioremap_nocache(REG_PIN_DRV_BASE1,0x100);

	// volatile unsigned int *pdrv_reg = (volatile unsigned int *)_vpdrvbase;
	volatile unsigned int *pmux_reg = (volatile unsigned int *)_vpmuxbase;

	lcm_handle_t *lcmHandle = (lcm_handle_t *)param;

	lcmHandle->lcm_id = lcmHandle->lcm_probe.lcm_id;
	/* XAIN0_SADC, XAIN1_SADC */
	// qua_lcm_writel(0, (unsigned int)pmux_reg + 0xec);
	// qua_lcm_writel(0, (unsigned int)pmux_reg + 0xf0);

	if(lcmHandle->lcm_probe.lcm_id == 0)
	{
		qua_lcm_writel(1, (unsigned int)pmux_reg + REG_LCD_DE_PIN);
		// tmp = qua_lcm_readl((unsigned int)pdrv_reg + REG_LCD_DE_DRV);
		// printk("%s LCD_DE drv = 0x%x\n", __func__, tmp);
		// _v = ((tmp & 0xf0) | 0x04);
		// qua_lcm_writel(_v, (unsigned int)pdrv_reg + REG_LCD_DE_DRV);
		qua_lcm_gpio_request(GPIO_NUM_LCD_DE, 0);
		qua_lcm_gpio_direction_output(GPIO_NUM_LCD_DE, 1);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_DE, 0);

		/* power on vci timing gpio, 0: on, 1: off */
		qua_lcm_writel(PIN_MUX_GPIO_ENABLE, (unsigned int)pmux_reg + REG_LCD_D0_PIN);
		qua_lcm_gpio_request(GPIO_NUM_LCD_D0, 0);
		qua_lcm_gpio_direction_output(GPIO_NUM_LCD_D0, 1);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_D0, 1);
		mdelay(10);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_D0, 0);
		mdelay(20);

		/* power on rst timing */
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_DE, 1);
		udelay(10);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_DE, 0);
		udelay(10);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_DE, 1);
		mdelay(5);

		// printk("%s gpio3_3 val = 0x%x\n", __func__, val);
		// qua_lcm_writel(tmp, (unsigned int)pdrv_reg + REG_LCD_DE_DRV);
		// qua_lcm_gpio_free(GPIO_NUM_LCD_DE);
		// qua_lcm_writel(0, (unsigned int)pmux_reg + REG_LCD_DE_PIN);+
	}	// else {
	if(lcmHandle->lcm_probe.lcm_id == 1)
	{
		qua_lcm_writel(PIN_MUX_GPIO_ENABLE, (unsigned int)pmux_reg + GPIO_NUM_LCD_D1);
		qua_lcm_gpio_request(GPIO_NUM_LCD_D1, 0);
		qua_lcm_gpio_direction_output(GPIO_NUM_LCD_D1, 1);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_D1, 1);
		mdelay(10);
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_D1, 0);
		mdelay(20);

		qua_lcm_writel(1, (unsigned int)pmux_reg + REG_LCD_HS_PIN);
		// tmp = qua_lcm_readl((unsigned int)pdrv_reg + REG_LCD_VS_DRV);
		// printk("%s LCD_VS drv = 0x%x\n", __func__, tmp);
		// _v = ((tmp & 0xf0) | 0x04);
		// qua_lcm_writel(_v, (unsigned int)pdrv_reg + REG_LCD_VS_DRV);

		qua_lcm_gpio_request(GPIO_NUM_LCD_HS, 0);
		qua_lcm_gpio_direction_output(GPIO_NUM_LCD_HS, 1);

		local_irq_save(flags);
		preempt_disable();
		qua_lcm_gpio_set_value(GPIO_NUM_LCD_HS, 1);
		mdelay(10);
		// do{
			qua_lcm_gpio_set_value(GPIO_NUM_LCD_HS, 0);
			fhosal_udelay(10);
			qua_lcm_gpio_set_value(GPIO_NUM_LCD_HS, 1);
			fhosal_udelay(10);
			printk("%s gpio3_4 \n", __func__);
		// }while(1);
		preempt_enable();
		local_irq_restore(flags);
		// }while(1);
		// printk("%s gpio3_5 val = 0x%x\n", __func__, val);

		// qua_lcm_writel(tmp, (unsigned int)pdrv_reg + REG_LCD_VS_DRV);
		// qua_lcm_gpio_free(GPIO_NUM_LCD_VS);
		// qua_lcm_writel(0, (unsigned int)pmux_reg + REG_LCD_VS_PIN);
	}

	// fhosal_iounmap(_vpdrvbase);
	fhosal_iounmap(_vpmuxbase);

	return 0;
}
#endif
