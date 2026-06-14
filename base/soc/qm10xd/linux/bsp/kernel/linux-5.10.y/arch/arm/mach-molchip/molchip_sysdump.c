/*
 *
 *  Copyright (C) 2020  Kui.Wang  <kui.wang@molchip.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/profile.h>
#include <linux/timex.h>
#include <linux/sched.h>
#include <linux/clockchips.h>
#include <linux/platform_device.h>
#include <linux/smp.h>
#include <linux/reboot.h>
#include <linux/uaccess.h>
#include <asm/system_misc.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

struct molchip_sysdump {
	unsigned int magic_start;
	unsigned int buses_cnt;
	struct regmap **regm;
	unsigned int **regv;
	unsigned int magic_end;
};

static struct molchip_sysdump sysdump_info;

static struct of_device_id mc_sysdump_of_match[] = {
        { .compatible = "molchip,sysdump", },
        { /* end of table */ }
};
MODULE_DEVICE_TABLE(of, mc_sysdump_of_match);


int molchip_sysdump_sysreg_save(void)
{
	unsigned int i, cnt, start_ofs;

	for(i=0; i<sysdump_info.buses_cnt; i++) {
		cnt = 2;
		for(start_ofs=sysdump_info.regv[i][0]; start_ofs < sysdump_info.regv[i][1]; start_ofs += 4) {
			regmap_read(sysdump_info.regm[i], start_ofs,&sysdump_info.regv[i][cnt]);
			cnt += 1;
		}
	}

	return 0;
}

static int mc_sysdump_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np, *pp;
	unsigned int ofs[2],i;
	int error;

	np = dev->of_node;
	if(!np)
		return -EINVAL;

	sysdump_info.buses_cnt = of_get_child_count(np);
	if(sysdump_info.buses_cnt == 0) {
		dev_err(dev, "no sys regs to dump\n");
		return -EINVAL;
	} else {
		printk("sysdump buses cnt %d\r\n",sysdump_info.buses_cnt);
	}

	sysdump_info.regm = (struct regmap**)devm_kzalloc(dev, sysdump_info.buses_cnt * sizeof(struct regmap*), GFP_KERNEL);
	if(!sysdump_info.regm)
		return -ENOMEM;

	sysdump_info.regv = (unsigned int**)devm_kzalloc(dev, sysdump_info.buses_cnt * sizeof(unsigned int*), GFP_KERNEL);
	if(!sysdump_info.regv)
		return -ENOMEM;

	i = 0;
	for_each_child_of_node(np, pp) {
		error = of_property_read_u32_array(pp,"start_ofs,end_ofs",ofs,2);
		if(error) {
			dev_err(dev, "Inval start_ofs,end_ofs prop\n");
			return -EINVAL;
		}

		sysdump_info.regm[i] = syscon_regmap_lookup_by_phandle(pp,"regs_base");
		if(!sysdump_info.regm[i])
			return -EINVAL;

		sysdump_info.regv[i] = (unsigned int*)devm_kzalloc(dev, (ofs[1] - ofs[0])*sizeof(unsigned int) + (2 << 2), GFP_KERNEL);
		if(!sysdump_info.regv[i])
			return -ENOMEM;

		sysdump_info.regv[i][0] = ofs[0];
		sysdump_info.regv[i][1] = ofs[1];

		i++;
	}

	return 0;
}

/* Driver Structure */
static struct platform_driver mc_sysdump_driver = {
	.probe          = mc_sysdump_probe,
	.driver         = {
		.name   = "mc-sysdump",
		.of_match_table = mc_sysdump_of_match,
	},
};

module_platform_driver(mc_sysdump_driver);
