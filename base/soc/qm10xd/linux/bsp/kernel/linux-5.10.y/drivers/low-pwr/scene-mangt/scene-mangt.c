#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/miscdevice.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/io.h>

#include "../low-power.h"
#include "../shared_mem_apcp.h"

static struct proc_dir_entry *pde1;
static void * share_mem_cp_ptr;

static const char *low_pwr_file = "low-power";
static const char *drvfile = "cpclk-info";
static const char *scene_file = "scene";

static atomic_t scene_sel;

unsigned int get_scene_sel(void)
{
	return atomic_read(&scene_sel);
}
EXPORT_SYMBOL(get_scene_sel);

static void set_scene_sel(u32 val)
{
	atomic_set(&scene_sel, val);
}

static long scene_mangt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
		case CMD_SEL_SCENARIO0:
			set_scene_sel(SCNEN_SCENARIO0);
			break;
		case CMD_SEL_SCENARIO1:
			set_scene_sel(SCNEN_SCENARIO1);
			break;
		case CMD_SEL_SCENARIO2:
			set_scene_sel(SCNEN_SCENARIO2);
			break;
	}
	return 0;
}

static const struct file_operations file_ops = {
	.unlocked_ioctl = scene_mangt_ioctl,
};

static struct miscdevice scene_mangt_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &file_ops,
	.name = "scene-mangt"
};

/* proc interface setup */
static void *pwr_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter;
	if (*pos == 0)
		return &counter;
	else {
		*pos = 0;
		return NULL;
	}
}

static void pwr_seq_stop(struct seq_file *s, void *v)
{
}

static void *pwr_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return NULL;
}

static int share_mem_remap(void)
{
	u32 val;
	void* addr_remap = ioremap(SHARE_MEM_REG, 4);
	val = readl(addr_remap);
	share_mem_cp_ptr = ioremap(val, 1024 * 16);
	if (!share_mem_cp_ptr) {
		iounmap(addr_remap);
		return -ENOMEM;
	}
	return 0;
}

static int pwr_file_seq_show(struct seq_file *sfile, void *v)
{
	int ret;
	int i;
	struct clk_info *clk;
	void *data = (void *)sfile;
	ret = share_mem_remap();
	if (ret)
		return ret;

	clk = (struct clk_info *)share_mem_cp_ptr;

	for (i = 0; i< 128; i++) {
		if (clk[i].ref_cnt == 0)
			continue;
		seq_printf(data, "==== %s ====\n", clk[i].file);
		seq_printf(data, "cnt: %d, rate: %d\n", clk[i].ref_cnt, clk[i].rate);
	}

	return 0;
}

static const struct seq_operations pwr_file_seq_ops = {
	.start	= pwr_seq_start,
	.next	= pwr_seq_next,
	.stop	= pwr_seq_stop,
	.show	= pwr_file_seq_show,
};

static int pwr_proc_file_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &pwr_file_seq_ops);
}

static struct proc_ops pwr_proc_file_ops = {
	.proc_open = pwr_proc_file_open,
	.proc_read	= seq_read,
};


static int scene_file_seq_show(struct seq_file *sfile, void *v)
{
	void *data = (void *)sfile;
	seq_printf(data, "scene: %d\n", get_scene_sel());
	return 0;
}

static const struct seq_operations scene_file_seq_ops = {
	.start	= pwr_seq_start,
	.next	= pwr_seq_next,
	.stop	= pwr_seq_stop,
	.show	= scene_file_seq_show,
};

static ssize_t scene_proc_file_write(struct file *file, const char __user *buf,
			size_t length, loff_t *ppos)
{
	char *buffer, *str;
	int err;
	int scene;

	if (!buf || length > PAGE_SIZE)
		return -EINVAL;
	buffer = (char *)__get_free_page(GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

	if (copy_from_user(buffer, buf, length))
		goto write_out;

	err = -EINVAL;
	if (length < PAGE_SIZE)
		buffer[length] = '\0';
	else if (buffer[PAGE_SIZE-1])
		goto write_out;

	str = buffer;
	scene = simple_strtoul(str, &str, 0);
	set_scene_sel(scene);

	free_page((unsigned long)buffer);
	return length;
write_out:
		free_page((unsigned long)buffer);
		return err;

}

static int scene_proc_file_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &scene_file_seq_ops);
}

static struct proc_ops scene_proc_file_ops = {
	.proc_open = scene_proc_file_open,
	.proc_read	= seq_read,
	.proc_write = scene_proc_file_write,
};

static int pwr_init_procfs(void)
{
	struct proc_dir_entry *entry;

	pde1 = proc_mkdir(low_pwr_file, NULL);
	if (!pde1)
		goto err;

	entry = proc_create(scene_file, 0, pde1, &scene_proc_file_ops);
	if (!entry)
		goto err;

	entry = proc_create(drvfile, 0, pde1, &pwr_proc_file_ops);
	if (!entry)
		goto err;

	return 0;

err:
	remove_proc_entry(drvfile, pde1);
	remove_proc_entry(scene_file, pde1);
	remove_proc_entry(low_pwr_file, NULL);
	return -ENOMEM;
}

static int scene_mangt_probe(struct platform_device *pdev)
{
	int ret;

	u32 val = 0;
	struct device *dev = &pdev->dev;
	ret = of_property_read_u32(dev->of_node, "default-scene", &val);
	if (ret)
		goto err;


	ret = misc_register(&scene_mangt_dev);
	if (ret)
		goto err;

	ret = share_mem_remap();
	if (ret)
		goto err;

	ret = pwr_init_procfs();
	if (ret)
		goto err;

	set_scene_sel(val);

	return ret;

err:
	if (share_mem_cp_ptr != NULL)
		iounmap(share_mem_cp_ptr);
	return ret;
}

static int scene_mangt_remove(struct platform_device *pdev)
{
	misc_deregister(&scene_mangt_dev);

	remove_proc_entry(drvfile, pde1);
	remove_proc_entry(scene_file, pde1);
	remove_proc_entry(low_pwr_file, NULL);

	return 0;
}

static const struct of_device_id scene_mangt_match[] = {
	{ .compatible = "scene-mangt,mc"},
	{},
};

static struct platform_driver scene_mangt_driver = {
	.driver = {
		.name   = "scene-mangt,mc",
		.of_match_table = scene_mangt_match,
	},
	.probe = scene_mangt_probe,
	.remove = scene_mangt_remove,
};

module_platform_driver(scene_mangt_driver);
MODULE_LICENSE("GPL v2");


