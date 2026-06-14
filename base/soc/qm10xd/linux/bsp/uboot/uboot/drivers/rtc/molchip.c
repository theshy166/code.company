/*
 * (C) Copyright 2001, 2002, 2003
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Date & Time support (no alarms) for molchip rtc
 *
 */

#include <common.h>
#include <command.h>
#include <dm.h>
#include <rtc.h>
#include <wait_bit.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>


/* Driver Private Data */

struct mc_rtc {
	struct udevice *dev;
	fdt_addr_t regs_addr;
	void __iomem	*regs;
	struct clk *rtc_apb_clk;
};
struct mc_rtc_drv_data {
	int (*clk_init) (struct mc_rtc *rtc);
};

#define RTC_TIMEOUT_MS 5

/*
 * RTC register addresses
 */
/* RTC wrapper APB address mapping */
#define RTC_INT_STATUS_OFFSET      0x0000
#define RTC_INT_EN_OFFSET          0x0004
#define RTC_DEBUG0_OFFSET          0x0008
#define RTC_DEBUG1_OFFSET          0x000C
#define RTC_DEBUG2_OFFSET          0x0010
#define RTC_CMD_OFFSET             0x0014
#define RTC_RD_DATA_OFFSET         0x0018
#define RTC_WR_DATA_OFFSET         0x001C
	
/* RTC core address mapping */
#define CORE_RTC_COUNTER_OFFSET    0x0000
#define CORE_RTC_OFFSET_OFFSET     0x0004
#define CORE_RTC_ALARM_CFG_OFFSET  0x0008
#define CORE_RTC_TEMP_INFO_OFFSET  0x000c
#define CORE_RTC_TEMP_CFG_OFFSET   0x0010
#define CORE_RTC_ANA_CFG_OFFSET    0x0014
#define CORE_RTC_INT_STATUS_OFFSET 0x0018
#define CORE_RTC_INT_EN_OFFSET     0x001c
#define CORE_RTC_DEBUG_OFFSET      0x0020
#define CORE_RTC_OFFSET_LUT_OFFSET 0x0024

/* RTC_INT_STATUS bit define */
#define RX_CRCERROR_INT              (0x1 << 0)
#define RX_COMERROR_INT              (0x1 << 1)
#define RX_LENERROR_INT              (0x1 << 2)
#define CNT_THL_INT                  (0x1 << 3)
#define CNT_THH_INT                  (0x1 << 4)
#define CORE_IDLE_INT                (0x1 << 5)
#define CORE_INT                    (0x1 << 6)
#define WRAPPER_BUSY                (0x1 << 8)
#define CORE_BUSY                   (0x1 << 16)
#define CORE_INT_MASK                (0x1 << 22)

/* RTC_INT_EN bit define */
#define RX_CRCERROR_EN               (0x1 << 0)
#define RX_COMERROR_EN               (0x1 << 1)
#define RX_LENERROR_EN               (0x1 << 2)
#define CNT_THL_EN                   (0x1 << 3)
#define CNT_THH_EN                   (0x1 << 4)
#define CORE_IDLE_EN                 (0x1 << 5)
#define CORE_INT_EN                  (0x1 << 6)

/* RTC_CMD bit define */
#define OPTYPE(x)                  ((x << 0) & 0x0000000F)
#define TARGETREG(x)               ((x << 4) & 0x000001F0)

/*RTC_CORE_ID_OFFSET bit define*/
#define  OFFSET_EN                     (1<<0)
#define  OFFSET_ATUTO                  (1<<1)
#define  OFFSET_IDX                    (1<<2)
#define  OFFSET_BK_EN                  (1<<8)
#define  OFFSET_BK_AUTO                (1<<9)
#define  OFFSET_BK_IDX                 (1<<10)
#define  OFFSET_CURRENT                (1<<16)

/* RTC_CORE_INT_EN define */
#define ALARM_INT_EN               (0x1 << 4)

#define ELAPSED_LEAP_YEARS(y) (((y - 1)/4) - ((y- 1)/100) + ((y + 299)/400) - 17)

enum rtc_core_reg_id {
	RTC_CORE_ID_COUNTER = 0,
	RTC_CORE_ID_OFFSET = 1,
	RTC_CORE_ID_ALARM_CFG = 2,
	RTC_CORE_ID_TEMP_INFO = 3,
	RTC_CORE_ID_TEMP_CFG = 4,
	RTC_CORE_ID_ANA_CFG = 5,
	RTC_CORE_ID_INT_STATUS = 6,
	RTC_CORE_ID_INT_EN = 7,
	RTC_CORE_ID_DEBUG = 8,
	RTC_CORE_ID_OFFSET_LUT0 = 9,
	RTC_CORE_ID_OFFSET_LUT1 = 10,
	RTC_CORE_ID_OFFSET_LUT2 = 11,
	RTC_CORE_ID_OFFSET_LUT3 = 12,
	RTC_CORE_ID_OFFSET_LUT4 = 13,
	RTC_CORE_ID_OFFSET_LUT5 = 14,
	RTC_CORE_ID_OFFSET_LUT6 = 15,
	RTC_CORE_ID_OFFSET_LUT7 = 16,
	RTC_CORE_ID_OFFSET_LUT8 = 17,
	RTC_CORE_ID_OFFSET_LUT9 = 18,
	RTC_CORE_ID_OFFSET_LUT10 = 19,
	RTC_CORE_ID_OFFSET_LUT11 = 20,
};

/*
 *Function: mc_rtc_poll_reg
 *Description: helper function to do poll and timeout
 *Input: register address and mask
 *Returns: 0 when the corresponding bit is 0 otherwise polling time out.
 */
static int mc_rtc_poll_reg(void __iomem * addr, u32 mask)
{
        int timeout = RTC_TIMEOUT_MS;

        do {
                if (!(readl(addr) & mask))
                        return 0;

                udelay(1000);
        } while (timeout -- > 0);

        return (readl(addr) & mask) ? 0 : -ETIMEDOUT;
}


/*
 *Function: mc_rtc_core_read
 *Description: Read the core register data
 *Input: mc_rtc struct and which register from core
 *Returns: RTC core register data.
 */
static u32 mc_rtc_core_read(struct mc_rtc *rtc, u32 id)
{
        int ret = 0;
        u32 regval = 0;
        writel(OPTYPE(1) | TARGETREG(id), rtc->regs + RTC_CMD_OFFSET);
        ret =
            mc_rtc_poll_reg(rtc->regs + RTC_INT_STATUS_OFFSET,
                            (CORE_BUSY | WRAPPER_BUSY));
		if (ret < 0) {
			printf("mc rtc read data poll timeout %d\n", ret);
		}
        regval = readl(rtc->regs + RTC_RD_DATA_OFFSET);

        return regval;
}
/*
 *Function: mc_rtc_core_write
 *Description: write data to the core register
 *Input: mc_rtc struct and write which register from core and the write data
 *Returns: none.
 */
static int mc_rtc_core_write(struct mc_rtc *rtc, u32 id, u32 data)
{
        int ret = 0;
        writel(data, rtc->regs + RTC_WR_DATA_OFFSET);
        ret =
            mc_rtc_poll_reg(rtc->regs + RTC_INT_STATUS_OFFSET,
                            (CORE_BUSY | WRAPPER_BUSY));
        if (ret)
                printf("mc rtc write data poll timeout %d\n", ret);
        writel(OPTYPE(2) | TARGETREG(id), rtc->regs + RTC_CMD_OFFSET);
        ret =
            mc_rtc_poll_reg(rtc->regs + RTC_INT_STATUS_OFFSET,
                            (CORE_BUSY | WRAPPER_BUSY));
		return ret;
}


#ifdef CONFIG_DM_RTC
static int molchip_rtc_set(struct udevice *dev, const struct rtc_time *tm)
{
	int ret;
	uint32_t reg_val, day, hour, min, sec;
	struct mc_rtc *mc_rtc = dev_get_priv(dev);

	printf("Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
	      tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_wday,
	      tm->tm_hour, tm->tm_min, tm->tm_sec);

	if (tm->tm_year < 1970 || tm->tm_year > 2069)
		printf("WARNING: year should be between 1970 and 2069!\n");

	sec = rtc_mktime(tm);
	day  = sec / (24 * 3600);
	sec  = sec % (24 * 3600);
	hour = sec / 3600;
	sec  = sec % 3600;
	min  = sec / 60;
	sec  = sec % 60;
	reg_val = (day << 17) | (hour << 12) | (min << 6) | sec;
	ret = mc_rtc_core_write(mc_rtc, RTC_CORE_ID_COUNTER, reg_val);
	
	if (ret < 0)
		return ret;

	debug("rtc %s\n",__func__);
	return 0;
}

static int molchip_rtc_get(struct udevice *dev, struct rtc_time *tm)
{
	//int ret;
	u32 day, hour, min, sec;
	u32 reg_time = 0;
	int tim = 0;
	struct mc_rtc *mc_rtc = dev_get_priv(dev);

	reg_time = mc_rtc_core_read(mc_rtc, RTC_CORE_ID_COUNTER);
	debug("raw timeval: 0x%x\n",reg_time);
	day = (reg_time >> 17) & 0x7FFF;
	hour = (reg_time >> 12) & 0x001F;
	min = (reg_time >> 6) & 0x003F;
	sec = (reg_time >> 0) & 0x003F;
	tim =
	    (int) ((day * 86400) + (hour * 3600) + (min * 60) + sec);
	rtc_to_tm(tim, tm);

	debug("Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
	      tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_wday,
	      tm->tm_hour, tm->tm_min, tm->tm_sec);
	debug("rtc %s\n",__func__);

	return 0;
}

static int molchip_rtc_reset(struct udevice *dev)
{
	int ret;
	struct rtc_time tmp = {
		.tm_year = 1970,
		.tm_mon = 1,
		.tm_mday = 1,
		.tm_hour = 0,
		.tm_min = 0,
		.tm_sec = 0,
	};


	ret = molchip_rtc_set(dev, &tmp);
	if (ret < 0)
		return ret;

	debug("RTC:   %4d-%02d-%02d %2d:%02d:%02d UTC\n",
	      tmp.tm_year, tmp.tm_mon, tmp.tm_mday,
	      tmp.tm_hour, tmp.tm_min, tmp.tm_sec);

	debug("rtc %s\n",__func__);

	return 0;
}
static int mc_rtc_clk_init_fy00(struct mc_rtc *rtc)
{

        int ret = 0;
		debug("fy00 rtc clk init\n");
		writel(readl(0x10000108) & ~ BIT(1), 0x10000108);
		writel(readl(0x10000200) | BIT(6), 0x10000200);
		
        return ret;
}
static const struct mc_rtc_drv_data fy00_data = {
        .clk_init = mc_rtc_clk_init_fy00,
};

static int molchip_probe(struct udevice *dev)
{
	struct mc_rtc *mc_rtc = dev_get_priv(dev);
	struct mc_rtc_drv_data * rtc_clk_init = (struct mc_rtc_drv_data *)dev->driver->of_match->data;
	debug("rtc %s\n",__func__);
	mc_rtc->regs_addr = devfdt_get_addr(dev);
	if (mc_rtc->regs_addr == FDT_ADDR_T_NONE) {
		error("devfdt_get_addr() failed");
		return -ENODEV;
	}
	mc_rtc->regs = (u32 *)mc_rtc->regs_addr;
	if (rtc_clk_init == NULL) {
		error("dev_get_driver_data() failed");
		return -ENODEV;
	}
	rtc_clk_init->clk_init(mc_rtc);

	//clr int
	mc_rtc_core_write(mc_rtc, RTC_CORE_ID_INT_STATUS, 0x00000000);
	mc_rtc_core_write(mc_rtc, RTC_CORE_ID_INT_EN, 0x00000000);

	return 0;
}
static int molchip_remove(struct udevice *dev)
{
	return 0;
}

static const struct rtc_ops molchip_rtc_ops = {
	.get = molchip_rtc_get,
	.set = molchip_rtc_set,
	.reset = molchip_rtc_reset,
};

static const struct udevice_id molchip_rtc_ids[] = {
	{ .compatible = "mc,fy00-rtc", .data = (ulong)&fy00_data},
	{ }
};

U_BOOT_DRIVER(rtc_molchip) = {
	.name	= "rtc-molchip",
	.id	= UCLASS_RTC,
	.probe	= molchip_probe,
	.remove = molchip_remove,
	.of_match = molchip_rtc_ids,
	.ops	= &molchip_rtc_ops,
	.priv_auto_alloc_size = sizeof(struct mc_rtc),
	.flags = DM_FLAG_PRE_RELOC,
};
#endif /* CONFIG_DM_RTC */
