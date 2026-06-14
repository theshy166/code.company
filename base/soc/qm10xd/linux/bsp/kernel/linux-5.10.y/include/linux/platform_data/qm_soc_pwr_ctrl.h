#ifndef _QM_SOC_PWR_CTRL_H
#define _QM_SOC_PWR_CTRL_H

struct soc_power_ctrl_platdata {
	int pwr_gpio;
	unsigned power_on_level;
};

#endif