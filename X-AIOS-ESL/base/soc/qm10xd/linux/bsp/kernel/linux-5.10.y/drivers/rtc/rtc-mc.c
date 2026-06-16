/*
* Copyright (c) 2019-2025 Molchip Inc.
* Author: Molchip
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 1 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/rtc.h>
#include <linux/irqdomain.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/nvmem-consumer.h>
#include <linux/time.h>
#include <linux/xc-clock-simple/clock.h>

#define TSNSOR_SHOW_V2 1

#if TSNSOR_SHOW_V2
#define THM_CFG0 0x100
#define THM_CFG1 0x104
#define THM_CFG2 0x108

#define THM_DATA(val) ((val & GENMASK(11, 3)) >> 3) 
#define THM_EN  BIT(0)
#define THM_SOC BIT(1)
#endif

#define TSENSOR_LUT_NUM	12
#define USE_TSENSOR_OFFSET
#define USE_TSENSOR
#define  INTTER_TIMER_TEST
#define  RTC_INT_TEST
#define  TEMPERATURE_TEST
char *mc_rtc_efuse ;
char *mc_rtc_thm_efuse ;
#define  USE_TSENSOR_OFFSET
#ifdef USE_TSENSOR
#define LUT_COF             (86)
#define LUT_OFFSET          (0xf6)
#define THL_DATA_LOW        (0)
#define THH_DATA_HIGH       (48)
#define TSENSOR_CP_DEFAULT_OUT 0x8ac
#endif
#define TIMEOUT_MS		100
#define DELAY_INTERVAL_US	1500

#define TEMPERATURE_SENSOR_EB	        0x5757

#ifndef  INTTER_TIMER_TEST
struct timeval timestamp0;
struct timeval timestamp1;
struct timeval timestamp2;
struct timeval timestamp3;

static void rtc_get_timestamp(struct timeval *tv)
{
	struct timespec ts;

	ktime_get_ts(&ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}
#endif

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

unsigned int TSENSOR_LUT[TSENSOR_LUT_NUM] = {
#include "rtc-mc-tab.h"
};

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

struct mc_rtc {
	void __iomem *regs;
	struct device *dev;
	struct rtc_device *rtc_dev;
	int irq;
	struct mutex mc_rtc_mutex;
	struct work_struct mc_rtc_int_wq;
	struct regmap *analog_thm_top;
};

struct mc_rtc_drv_data {
	int	(*clk_init) (struct mc_rtc *rtc);
};

char *mc_rtc_calib_read(struct device *dev, const char *cname)
{
	struct nvmem_cell *cell;
	ssize_t data;
	char *ret;

	cell = nvmem_cell_get(dev, cname);
	if (IS_ERR(cell))
		return ERR_CAST(cell);

	ret = nvmem_cell_read(cell, &data);
	nvmem_cell_put(cell);

	return ret;
}

/*
 *Function: mc_rtc_poll_reg
 *Description: helper function to do poll and timeout
 *Input: register address and mask
 *Returns: 0 when the corresponding bit is 0 otherwise polling time out.
 */
static int mc_rtc_poll_reg(void __iomem * addr, u32 mask)
{
	unsigned long timeout = jiffies + msecs_to_jiffies(TIMEOUT_MS);

	do {
		if (!(readl_relaxed(addr) & mask))
			return 0;

		usleep_range(DELAY_INTERVAL_US, DELAY_INTERVAL_US + 50);
	} while (!time_after(jiffies, timeout));

	return (readl_relaxed(addr) & mask) ? 0 : -ETIMEDOUT;
}

/*
 *Function: rtc_time_from_reg
 *Description: Turn the data from core counter conversion to time
 *Input: data frome RTC_CORE_ID_COUNTER
 *Returns: RTC current time.
 */
static int rtc_time_from_reg(u32 data)
{
	u32 day, hour, min, sec, reg_time;
	time64_t rtc_time;

	reg_time = data;
	day = (reg_time >> 17) & 0x7FFF;
	hour = (reg_time >> 12) & 0x001F;
	min = (reg_time >> 6) & 0x003F;
	sec = (reg_time >> 0) & 0x003F;
	rtc_time =
	    (time64_t) ((day * 86400) + (hour * 3600) + (min * 60) + sec);

	return rtc_time;

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

	mutex_lock(&rtc->mc_rtc_mutex);
	writel_relaxed(OPTYPE(1) | TARGETREG(id), rtc->regs + RTC_CMD_OFFSET);
	ret =
	    mc_rtc_poll_reg(rtc->regs + RTC_INT_STATUS_OFFSET,
			    (CORE_BUSY | WRAPPER_BUSY));

	regval = readl_relaxed(rtc->regs + RTC_RD_DATA_OFFSET);
	mutex_unlock(&rtc->mc_rtc_mutex);

	return regval;
}

/*
 *Function: mc_rtc_core_write
 *Description: write data to the core register
 *Input: mc_rtc struct and write which register from core and the write data
 *Returns: none.
 */
static void mc_rtc_core_write(struct mc_rtc *rtc, u32 id, u32 data)
{
	int ret = 0;

	mutex_lock(&rtc->mc_rtc_mutex);
	writel_relaxed(data, rtc->regs + RTC_WR_DATA_OFFSET);
	ret =
	    mc_rtc_poll_reg(rtc->regs + RTC_INT_STATUS_OFFSET,
			    (CORE_BUSY | WRAPPER_BUSY));
	if (ret)
		pr_err("mc rtc write data poll timeout %d\n", ret);
	writel_relaxed(OPTYPE(2) | TARGETREG(id), rtc->regs + RTC_CMD_OFFSET);
	ret =
	    mc_rtc_poll_reg(rtc->regs + RTC_INT_STATUS_OFFSET,
			    (CORE_BUSY | WRAPPER_BUSY));

	mutex_unlock(&rtc->mc_rtc_mutex);
}

/*
 *Function: mc_get_rtc_time64
 *Description:get RTC timer
 *Input: mc_rtc struct parameter
 *Returns: the time from rtc.
 */
static time64_t mc_get_rtc_time64(struct mc_rtc *rtc)
{
	time64_t rtc_time = 0;
	u32 reg_time = 0;

	reg_time = mc_rtc_core_read(rtc, RTC_CORE_ID_COUNTER);
	rtc_time = rtc_time_from_reg(reg_time);

	return rtc_time;
}

/*
 *Function: mc_get_alm_time64
 *Description:get RTC alarme time
 *Input: mc_rtc struct parameter
 *Returns: the alarm time from rtc.
 */
static time64_t mc_get_alm_time64(struct mc_rtc *rtc)
{
	time64_t rtc_time = 0;
	u32 reg_time = 0;

	reg_time = mc_rtc_core_read(rtc, RTC_CORE_ID_ALARM_CFG);
	rtc_time = rtc_time_from_reg(reg_time);

	return rtc_time;
}

/*
 *Function: mc_rtc_irq_handler
 *Description:when alarm time out will come interruput
 *Input: irq number and point
 *Returns: IRQ_HANDLED.
 */
static irqreturn_t mc_rtc_irq_handler(int irq, void *data)
{
	u32 mc_rtc_data = 0;
	struct mc_rtc *rtc =
	    platform_get_drvdata((struct platform_device *)data);

#ifndef INTTER_TIMER_TEST
	rtc_get_timestamp(&timestamp0);
#endif
	mc_rtc_data = readl_relaxed(rtc->regs + RTC_INT_EN_OFFSET);
	writel_relaxed(mc_rtc_data & ~(CORE_INT), rtc->regs + RTC_INT_EN_OFFSET);

	schedule_work(&rtc->mc_rtc_int_wq);
#ifndef INTTER_TIMER_TEST
	rtc_get_timestamp(&timestamp3);
	printk
	    ("JOB_PROC: time0 (%lld, %lld), time1 (%lld, %lld),time2 (%lld, %lld),time3 (%lld, %lld), %lld\n",
	     (uint64_t) timestamp0.tv_sec, (uint64_t) timestamp0.tv_usec,
	     (uint64_t) timestamp1.tv_sec, (uint64_t) timestamp1.tv_usec,
	     (uint64_t) timestamp2.tv_sec, (uint64_t) timestamp2.tv_usec,
	     (uint64_t) timestamp3.tv_sec, (uint64_t) timestamp3.tv_usec,
	     (uint64_t) ((timestamp3.tv_sec * 1000000 + timestamp3.tv_usec) -
			 (timestamp0.tv_sec * 1000000 + timestamp0.tv_usec)));
#endif
	rtc_update_irq(rtc->rtc_dev, 1, RTC_IRQF | RTC_AF);

	return IRQ_HANDLED;

}

/*
 *Function: mc_rtc_core_handler
 *Description:RTC core int clean
 *Input: parameter point
 *Returns: none.
 */
static void mc_rtc_core_handler(struct work_struct *work)
{
	struct mc_rtc *rtc = container_of(work, struct mc_rtc, mc_rtc_int_wq);
	int mc_rtc_regdata = 0;

	/* clear CoreInt status */
	mc_rtc_core_write(rtc, RTC_CORE_ID_INT_STATUS, 0);
	/* clear WrapInt status */
	writel_relaxed(0, rtc->regs + RTC_INT_STATUS_OFFSET);
	mc_rtc_regdata = readl_relaxed(rtc->regs + RTC_INT_STATUS_OFFSET);
	writel_relaxed((mc_rtc_regdata | CORE_INT),
		       rtc->regs + RTC_INT_EN_OFFSET);

	//rtc_update_irq(rtc->rtc_dev, 1, RTC_IRQF | RTC_AF);
}

/*
 *Function: mc_rtc_read_time
 *Description:Read RTC timer
 *Input: parameter point
 *Returns: 0.
 */
static int mc_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);
	rtc_time64_to_tm(mc_get_rtc_time64(rtc), tm);

	return 0;
}

/*
 *Function: rtc_time_to_reg
 *Description: turn date for timer
 *Input: parameter point
 *Returns: current time to RTC count register.
 */
static int rtc_time_to_reg(struct rtc_time *tm)
{
	u32 reg_val, sec, min, hour, day;

	day = rtc_year_days(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
	day += (tm->tm_year - 70) * 365 + ELAPSED_LEAP_YEARS(tm->tm_year);
	hour = tm->tm_hour;
	min = tm->tm_min;
	sec = tm->tm_sec;
	reg_val = (day << 17) | (hour << 12) | (min << 6) | sec;

	return reg_val;

}

/*
 *Function: mc_rtc_set_time
 *Description: set timer for RTC
 *Input: parameter point
 *Returns: 0.
 */
static int mc_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);

	mc_rtc_core_write(rtc, RTC_CORE_ID_COUNTER, rtc_time_to_reg(tm));

	return 0;
}

/*
 *Function: mc_rtc_read_alarm
 *Description: read alarm timer
 *Input: parameter point
 *Returns: 0.
 */
static int mc_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);

	rtc_time64_to_tm(mc_get_alm_time64(rtc), &alm->time);

	return 0;
}

/*
 *Function: mc_rtc_set_alarm
 *Description: set alarm timer
 *Input: parameter point
 *Returns: 0.
 */
static int mc_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);

	mc_rtc_core_write(rtc, RTC_CORE_ID_ALARM_CFG,
			  rtc_time_to_reg(&alm->time));

	return 0;
}


/*
 *Function: mc_rtc_alarm_irq_enable
 *Description: enable alarm irq
 *Input: parameter point and data for enable
 *Returns: 0.
 */
static int mc_rtc_alarm_irq_enable(struct device *dev, unsigned int is_enabled)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);
	u32 reg_val = mc_rtc_core_read(rtc, RTC_CORE_ID_INT_EN);

	/* enable rtc wrapper core int */
	writel_relaxed(readl_relaxed(rtc->regs + RTC_INT_EN_OFFSET) |
		       CORE_INT_EN, rtc->regs + RTC_INT_EN_OFFSET);

	if (is_enabled) {
		/* enable rtc core sec and alarm int */
		mc_rtc_core_write(rtc, RTC_CORE_ID_INT_EN,
				  reg_val | ALARM_INT_EN);

		writel_relaxed(readl_relaxed(rtc->regs + RTC_INT_EN_OFFSET) &
			       ~CORE_INT_MASK, rtc->regs + RTC_INT_EN_OFFSET);
	} else {
		mc_rtc_core_write(rtc, RTC_CORE_ID_INT_EN,
				  reg_val & ~(ALARM_INT_EN));
		writel_relaxed(readl_relaxed(rtc->regs + RTC_INT_EN_OFFSET) |
			       CORE_INT_MASK, rtc->regs + RTC_INT_EN_OFFSET);
	}

	return 0;
}

static const struct rtc_class_ops mc_rtc_ops = {
	.read_time = mc_rtc_read_time,
	.set_time = mc_rtc_set_time,
	.read_alarm = mc_rtc_read_alarm,
	.set_alarm = mc_rtc_set_alarm,
	.alarm_irq_enable = mc_rtc_alarm_irq_enable,
};

/*
 *Function: rtc_reg_set_inprobe
 *Description: some register operations in probe
 *Input: parameter point
 *Returns: none.
 */
static void rtc_reg_set_inprobe(struct mc_rtc *rtc)
{

	int i = 0;

	mc_rtc_core_write(rtc, RTC_CORE_ID_OFFSET, 0x3);
	for (i = 0; i < TSENSOR_LUT_NUM; i++)
		mc_rtc_core_write(rtc, RTC_CORE_ID_OFFSET_LUT0 + i,
				  TSENSOR_LUT[i]);

	mc_rtc_core_write(rtc, RTC_CORE_ID_INT_STATUS, 0x00000000);
	mc_rtc_core_write(rtc, RTC_CORE_ID_INT_EN, 0x00000000);

}

//#if TSNSOR_SHOW_V2
/*
 *Function: show_mc_chip_temerature
 *Description: provide the chip temerature
 *Input: parameter point
 *Returns:chip temerature.
 */
static ssize_t show_mc_thm_temperature(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
    int tsensor_value = 0;
    int ret = 0;
    struct mc_rtc *rtc = dev_get_drvdata(dev);


    ret = regmap_read(rtc->analog_thm_top, THM_CFG1, &tsensor_value);
	if (ret < 0)
		dev_err(dev, "failed to get thm data\n");

    return sprintf(buf, "%d-%d\n", (int)THM_DATA(tsensor_value),(*((unsigned short*)mc_rtc_thm_efuse)) & 0x1ff);
}
//#else //!TSNSOR_SHOW_V2
/*
 *Function: show_mc_chip_temerature
 *Description: provide the chip temerature
 *Input: parameter point
 *Returns:chip temerature.
 */
static ssize_t show_mc_chip_temerature(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
#if 0
	unsigned long long temperature = 0;
	unsigned long long  offset_owern = 0;
	int tsensor_value = 0;
	int tsensor_value_judge = 0;
	static int slope = 0;
	static int rtc_precision_ctrl = 0;
	static int rtc_production_line_temp = 0;
	static int rtc_precision_frc_ctrl = 0;
	static int rtc_default_intercept = 0;
	int ret = 0;
	int dec_int = 0;
	int frc_int = 0;
	uint64_t temp_int64 = 0;

	struct mc_rtc *rtc = dev_get_drvdata(dev);

	if( 0 == slope ) {
		ret = of_property_read_u32(dev->of_node, "rtc-slope", &slope);
		if (ret) {
			dev_err(dev, "failed to get rtc-slope\n");
			return ret;
		}
	}

	if(0 == rtc_precision_ctrl){
		ret = of_property_read_u32(dev->of_node, "rtc-precision-ctrl", &rtc_precision_ctrl);
		if (ret) {
			dev_err(dev, "failed to get rtc-precision-ctrl\n");
			return ret;
		}
	}

	if(0 == rtc_production_line_temp){
		ret = of_property_read_u32(dev->of_node, "rtc-production-line-temp", &rtc_production_line_temp);
		if (ret) {
			dev_err(dev, "failed to get rtc-prroduction-line-temp\n");
			return ret;
		}
	}

	if(0 == rtc_precision_frc_ctrl){
		ret = of_property_read_u32(dev->of_node, "rtc-precision-frc-ctrl", &rtc_precision_frc_ctrl);
		if (ret) {
			dev_err(dev, "failed to get rtc-precision-frc-ctrl\n");
			return ret;
		}
	}

	if(0 == rtc_default_intercept){
		ret = of_property_read_u32(dev->of_node, "rtc-default-intercept", &rtc_default_intercept);
		if (ret) {
			dev_err(dev, "failed to get rtc-default-intercept\n");
			return ret;
		}
	}
	//printk(KERN_INFO "slope:%d rtc_precision_ctrl:%d rtc_production_line_temp:%d rtc_precision_frc_ctrl:%d rtc_default_intercept:%d\n",
	//slope, rtc_precision_ctrl, rtc_production_line_temp, rtc_precision_frc_ctrl, rtc_default_intercept);
	if(0 == (*((unsigned short*)mc_rtc_efuse)))
			*((unsigned short*)mc_rtc_efuse) = rtc_default_intercept;

	tsensor_value = mc_rtc_core_read(rtc, RTC_CORE_ID_TEMP_INFO);
	mc_rtc_core_write(rtc, RTC_CORE_ID_OFFSET, TEMPERATURE_SENSOR_EB);
	offset_owern = (*((unsigned short*)mc_rtc_efuse)) * slope  - rtc_production_line_temp * rtc_precision_ctrl;
	temperature = tsensor_value * slope  - offset_owern;

	temp_int64 = offset_owern;

	//printk(KERN_INFO "temp_int64:%lld slope:%d\n",temp_int64,slope);
	do_div(temp_int64, slope);
	//printk(KERN_INFO "2temp_int64:%lld slope:%d\n",temp_int64,slope);

	tsensor_value_judge = tsensor_value - temp_int64;

	if( tsensor_value_judge > 0){
		temp_int64 = temperature;
		do_div(temp_int64, rtc_precision_ctrl);
		dec_int = temp_int64;
		temp_int64 = (temperature - dec_int * rtc_precision_ctrl);
		do_div(temp_int64, rtc_precision_frc_ctrl);
		frc_int = temp_int64;
	}else{
		temp_int64 = (temperature * (-1));
		do_div(temp_int64, rtc_precision_ctrl);
		dec_int = temp_int64;

		temp_int64 = (temperature * ( -1 ) - dec_int * rtc_precision_ctrl);
		do_div(temp_int64, rtc_precision_frc_ctrl);
		frc_int = temp_int64;


	}

	if (tsensor_value == 2048) {
		return sprintf(buf, "efuse_data is %d raw data %d the temperature is : %d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, 50,0 );
	}else if(0 == tsensor_value){
		return sprintf(buf, "efuse_data is %d raw data %d the temperature is : %d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, 0,0 );
	}else if( tsensor_value_judge > 0) {
		if ((tsensor_value >= 1024) && (tsensor_value < 2048))
			return sprintf(buf, "efuse_data is %d raw data %d the temperature is : %d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, (dec_int - 30),frc_int );
		else if (tsensor_value < 1024)
			return sprintf(buf, "efuse_data is %d raw data %d the temperature is : %d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, (dec_int - 33),frc_int );
		else
			return sprintf(buf, "efuse_data is %d raw data %d the temperature is : %d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, dec_int,frc_int );
	}else{
		if ((tsensor_value >= 1024) && (tsensor_value < 2048))
			return sprintf(buf, "efuse_data is %d raw data %d the temperature is : -%d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, (dec_int-30),frc_int );
		else if (tsensor_value < 1024)
			return sprintf(buf, "efuse_data is %d raw data %d the temperature is : -%d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, (dec_int-33),frc_int );
		else
			return sprintf(buf, "efuse_data is %d raw data %d the temperature is : -%d.%02d \n" ,(*((unsigned short*)mc_rtc_efuse)),tsensor_value, dec_int,frc_int );
	}
#else
    int tsensor_value = 0;
    struct mc_rtc *rtc = dev_get_drvdata(dev);

    tsensor_value = mc_rtc_core_read(rtc, RTC_CORE_ID_TEMP_INFO);
    mc_rtc_core_write(rtc, RTC_CORE_ID_OFFSET, TEMPERATURE_SENSOR_EB);
    //return sprintf(buf, "raw data is： %d\n" ,tsensor_value);
    return sprintf(buf, "%d-%d\n", tsensor_value, *((unsigned short*)mc_rtc_efuse));

#endif
	return 0;
}
//#endif //TSNSOR_SHOW_V2

/* Non-standard name, might need revisiting */
static DEVICE_ATTR(mc_chip_temerature, S_IWUSR | S_IRUGO,
		   show_mc_chip_temerature, NULL);

static DEVICE_ATTR(mc_thm_temperature, S_IWUSR | S_IRUGO,
		   show_mc_thm_temperature, NULL);

static struct attribute *device_attrs[] = {
	&dev_attr_mc_chip_temerature.attr,
	&dev_attr_mc_thm_temperature.attr,
	NULL
};

static const struct attribute_group device_attr_group = {
	.attrs = device_attrs,
};

#ifdef USE_TSENSOR_OFFSET

/*
 *Function: mc_rtc_temp_cfg_thl_thh
 *Description: write some parameter for RTC_CORE_ID_TEMP_CFG register
 *Input: parameter point and the highest data and  lowest data
 *Returns: 0.
 */
static int mc_rtc_temp_cfg_thl_thh(struct mc_rtc *rtc, unsigned int thl,
				   unsigned int thh)
{
	unsigned int temp_cfg = 0;
	int status = 0;

	if (thl > 0x3f) {
		pr_err("thl invalid\n");
		return -1;
	}

	if (thh > 0x3f) {
		pr_err("thh invalid\n");
		return -1;
	}

	temp_cfg = mc_rtc_core_read(rtc, RTC_CORE_ID_TEMP_CFG);
	temp_cfg &= 0xf000ffff;
	temp_cfg |= (thl << 16);
	temp_cfg |= (thh << 22);

	mc_rtc_core_write(rtc, RTC_CORE_ID_TEMP_CFG, temp_cfg);

	return status;
}

/*
 *Function: mc_rtc_temp_cfg_coef_offset
 *Description: 48 offset,from 0 to 47，also means from -50 degrees Celsius to 85 degrees Celsius
 *Input: parameter point and the highest data and  lowest data
 *Returns: success return 0 otherwise rturn -1.
 */
static int mc_rtc_temp_cfg_coef_offset(struct mc_rtc *rtc, unsigned int coef,
				       unsigned int offset)
{
	unsigned int temp_cfg = 0;
	int status = 0;

	if (coef > 0xff) {
		pr_err("coef invalid\n");
		return -1;
	}

	if (offset > 0xffff) {
		pr_err("offset invalid\n");
		return -1;
	}

	temp_cfg = mc_rtc_core_read(rtc, RTC_CORE_ID_TEMP_CFG);
	temp_cfg &= 0xffff0000;
	temp_cfg |= coef;
	temp_cfg |= (offset << 8);

	mc_rtc_core_write(rtc, RTC_CORE_ID_TEMP_CFG, temp_cfg);

	return status;
}

#ifdef USE_TSENSOR

/*
 *Function: rtc_adjust
 *Description: RTC temperature compensation
 *Input: parameter point
 *Returns: none.
 */
static void rtc_adjust(struct mc_rtc *rtc,unsigned short data)
{
	int i = 0;
	int efuse_date = 0;
	int offset_index = 0;
	int temp_diff = 0;
	int tsensor_out_value_diff = 0;
	char offset_lut[THH_DATA_HIGH];

	for (i = 0; i < TSENSOR_LUT_NUM; i++) {
		offset_lut[i * 4] = TSENSOR_LUT[i] & 0xff;
		offset_lut[i * 4 + 1] = ((TSENSOR_LUT[i] >> 8) & 0xff);
		offset_lut[i * 4 + 2] = ((TSENSOR_LUT[i] >> 16) & 0xff);
		offset_lut[i * 4 + 3] = ((TSENSOR_LUT[i] >> 24) & 0xff);
	}

	for (i = 0; i < THH_DATA_HIGH - 1; i++) {
		if (offset_lut[i] > offset_lut[i + 1])
			offset_index = i + 1;
		else
			offset_lut[i + 1] = offset_lut[i];
	}
	#ifdef USE_TSENSOR_OFFSET
		efuse_date = data;
		printk(KERN_ERR "tsensor efusedata:0x%x\n",efuse_date);
		tsensor_out_value_diff = efuse_date - TSENSOR_CP_DEFAULT_OUT;
		temp_diff = tsensor_out_value_diff*LUT_COF/4096;
		if ((temp_diff < 0) || (temp_diff > 5))
			temp_diff = 0;

		mc_rtc_temp_cfg_coef_offset(rtc, LUT_COF, LUT_OFFSET - temp_diff);
	#else
		mc_rtc_temp_cfg_coef_offset(rtc, LUT_COF, LUT_OFFSET);
	#endif

	mc_rtc_temp_cfg_thl_thh(rtc, THL_DATA_LOW, THH_DATA_HIGH - 1);
	mc_rtc_core_write(rtc, RTC_CORE_ID_OFFSET,
			  (offset_index << 2) | (offset_index << 10) |
			  OFFSET_BK_EN | OFFSET_BK_AUTO);
}
#endif

#endif

static int mc_rtc_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct mc_rtc *rtc;
	int ret = 0;
#ifdef USE_TSENSOR
	int rtc_default_intercept = 0;
	struct device *dev = &pdev->dev;
#endif
	const struct mc_rtc_drv_data *params_data =
		of_device_get_match_data(&pdev->dev);

	rtc = devm_kzalloc(&pdev->dev, sizeof(struct mc_rtc), GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

	rtc->dev = &pdev->dev;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	rtc->regs = devm_ioremap_resource(&pdev->dev, res);

	if (IS_ERR(rtc->regs))
		return PTR_ERR(rtc->regs);

	if ((params_data) && (params_data->clk_init)) {
		ret = params_data->clk_init(rtc);
		if (ret) {
			dev_err(&pdev->dev, "clk init fail %d\n", ret);
			return ret;
		}
	}

	mutex_init(&rtc->mc_rtc_mutex);

	INIT_WORK(&rtc->mc_rtc_int_wq, mc_rtc_core_handler);
	rtc->irq = platform_get_irq(pdev, 0);
	rtc_reg_set_inprobe(rtc);

	if (rtc->irq <= 0)
		return -EINVAL;
	else {
		ret =
		    devm_request_irq(&pdev->dev, rtc->irq, mc_rtc_irq_handler,
				     0, pdev->name, pdev);
		if (ret) {
			dev_err(&pdev->dev,
				"cannot register interrupt handler err=%d\n",
				ret);
			return ret;
		}
	}

	platform_set_drvdata(pdev, rtc);

	//rtc->rtc_dev = rtc_device_register("mc-rtc", &pdev->dev,
	//				   &mc_rtc_ops, THIS_MODULE);
	rtc->rtc_dev = devm_rtc_device_register(&pdev->dev, "mc-rtc", &mc_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc->rtc_dev)) {
		dev_err(&pdev->dev, "register rtc device failed\n");
		ret = PTR_ERR(rtc->rtc_dev);
		return ret;
	}

#ifdef RTC_INT_TEST
	mc_rtc_alarm_irq_enable(rtc->dev, 1);
#endif

#ifdef USE_TSENSOR
	ret = of_property_read_u32(dev->of_node, "rtc-default-intercept", &rtc_default_intercept);
	if (ret) {
		dev_err(dev, "failed to get rtc-default-intercept\n");
		//return ret;
	}

	mc_rtc_efuse = mc_rtc_calib_read(&pdev->dev,"calib");
	printk(KERN_INFO "mc_rtc_efuse:%d \n",*((unsigned short*)mc_rtc_efuse));
	if (*((unsigned short*)mc_rtc_efuse) == 0) {
		if (rtc_default_intercept != 0)
			*((unsigned short*)mc_rtc_efuse) = rtc_default_intercept;
		else
			*((unsigned short*)mc_rtc_efuse) = 2220;
	}
	rtc_adjust(rtc,*((unsigned short*)mc_rtc_efuse));

	#if TSNSOR_SHOW_V2
	rtc->analog_thm_top = syscon_regmap_lookup_by_phandle(dev->of_node,
                                                "analog_thm_top");
	if (IS_ERR(rtc->analog_thm_top)) {
		dev_err(dev, "failed to map analog_thm_top\n");
		//return PTR_ERR(lp->apahb_base);
	}
	if(rtc->analog_thm_top != NULL) {
		regmap_write(rtc->analog_thm_top, THM_CFG0, (THM_EN | THM_SOC));
		regmap_write(rtc->analog_thm_top, THM_CFG2, 0x0);
	}
	mc_rtc_thm_efuse = mc_rtc_calib_read(&pdev->dev,"calib_thm");
	printk(KERN_INFO "mc_rtc_thm_efuse:%d \n",(*((unsigned short*)mc_rtc_thm_efuse))&0x1ff);
	
	#endif
#endif

#ifdef TEMPERATURE_TEST
#if 0
	if(sysfs_create_file(&(rtc->dev->kobj), &dev_attr_mc_chip_temerature.attr)){

	    return -1;
	}
#endif
	ret =  sysfs_create_group(&pdev->dev.kobj, &device_attr_group);
	if (ret) {
		dev_err(&pdev->dev, "sysfs_create_group aborting.\n");
	}

#endif

	return 0;
}

static int mc_rtc_remove(struct platform_device *pdev)
{
	struct mc_rtc *rtc = platform_get_drvdata(pdev);

	sysfs_remove_group(&pdev->dev.kobj, &device_attr_group);
	rtc_device_unregister(rtc->rtc_dev);
	free_irq(rtc->irq, rtc->rtc_dev);
	irq_dispose_mapping(rtc->irq);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int mc_rtc_suspend(struct device *dev)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(rtc->irq);

	return 0;
}

static int mc_rtc_resume(struct device *dev)
{
	struct mc_rtc *rtc = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(rtc->irq);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(mc_rtc_pm_ops, mc_rtc_suspend, mc_rtc_resume);


static int mc_rtc_clk_init_xc01(struct mc_rtc *rtc)
{

	int ret = 0;
	struct device	*dev = rtc->dev;

	ret = clk_mini_enable(CKG_APB_RTCT_EN);
	if (ret) {
		dev_err(dev, "failed to enable rtc_apb_clk\n");
		goto err;
	}

err:
	return ret;
}

static const struct mc_rtc_drv_data xc01_data = {
	.clk_init = mc_rtc_clk_init_xc01,
};

static const struct of_device_id mc_rtc_of_match[] = {
	{.compatible = "mc,xc01-rtc", .data = &xc01_data },
	{}
};

MODULE_DEVICE_TABLE(of, mc_rtc_of_match);

static struct platform_driver mc_rtc_driver = {
	.driver = {
		   .name = "mc-rtc",
		   .of_match_table = mc_rtc_of_match,
		   .pm = &mc_rtc_pm_ops,
		   },
	.probe = mc_rtc_probe,
	.remove = mc_rtc_remove,
};

module_platform_driver(mc_rtc_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Molchip.Inc");
MODULE_DESCRIPTION("RTC Driver for Molchip");
