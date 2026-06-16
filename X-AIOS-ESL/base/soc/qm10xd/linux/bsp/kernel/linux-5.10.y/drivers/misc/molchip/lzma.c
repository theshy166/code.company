#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include "lzma_ioctl.h"
#include <linux/xc-clock-simple/clock.h>

#define DECODE_CTRL0		(0xC)
#define LZMA_DECODE_CNT		(0x28)
#define SRC_START_ADDR		(0x30)
#define SRC_END_ADDR		(0x34)
#define SRC_FILE_LENGTH		(0x38)
#define DES_BASE_ADDR		(0x40)
#define INT_DONE_MASK		(0x74)
#define LZMA_DMA_CTRL		(0x2C)
#define INT_DONE_STATUS		(0x70)
#define INT_DONE_CLEAR		(0x78)

//INT_DONE_STATUS
#define INT_DONE		BIT(0)
#define LZMA_DONE_CLEAR		BIT(0)

//DECODE_CTRL0
#define DECODE_START		BIT(0)		

//LZMA_DMA_CTRL
#define SRC_ROLL_BACKADDR_EN BIT(0)
#define RCH_DMACTL_BYPASS  (0x3<<4)

//DECODE_CTRL0
#define SFOT_RST_LZMA		BIT(8)

struct molchip_lzma_drv {
    struct mutex lz_mutex;
    void __iomem *lzma_base_va;
	struct clk			*clk_sel;
	int					max_freq;
	volatile bool condition_for_wakeup;
	volatile bool judge_result;
	struct miscdevice *my_misc_device;
};

static struct molchip_lzma_drv mol_lzma;

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);

static void  mol_lzma_write(unsigned int src, unsigned int dst,unsigned int zip_len)
{
    unsigned int  reg_val=0;

	reg_val = readl(mol_lzma.lzma_base_va + LZMA_DMA_CTRL);
	reg_val &= ~ SRC_ROLL_BACKADDR_EN;
	reg_val |= RCH_DMACTL_BYPASS;
	writel(reg_val, mol_lzma.lzma_base_va + LZMA_DMA_CTRL);

	reg_val = readl(mol_lzma.lzma_base_va + DECODE_CTRL0);
	reg_val |= SFOT_RST_LZMA;
	writel(reg_val, mol_lzma.lzma_base_va + DECODE_CTRL0);

	reg_val = readl(mol_lzma.lzma_base_va + DECODE_CTRL0);
	reg_val &= ~SFOT_RST_LZMA;
	writel(reg_val, mol_lzma.lzma_base_va + DECODE_CTRL0);

	writel(src >> 3, mol_lzma.lzma_base_va + SRC_START_ADDR);
	writel((src >> 3) + 0x40 - 1, mol_lzma.lzma_base_va + SRC_END_ADDR);
	writel(dst >> 3, mol_lzma.lzma_base_va + DES_BASE_ADDR);
	writel(zip_len, mol_lzma.lzma_base_va + SRC_FILE_LENGTH);
	writel(0xFFFFFFFF, mol_lzma.lzma_base_va + INT_DONE_MASK);
	
	reg_val = readl(mol_lzma.lzma_base_va + DECODE_CTRL0);
	reg_val |= DECODE_START;
	writel(reg_val, mol_lzma.lzma_base_va + DECODE_CTRL0);
	
	reg_val = readl(mol_lzma.lzma_base_va + DECODE_CTRL0);
	reg_val &= (~DECODE_START);
	writel(reg_val, mol_lzma.lzma_base_va + DECODE_CTRL0);

}

static irqreturn_t mc_lzma_irq_handler(int irq, void *dev_id)
{

	unsigned int val, v;
	unsigned int status = INT_DONE;
	struct molchip_lzma_drv *p_lzma = (struct molchip_lzma_drv *)platform_get_drvdata(dev_id);
	v = readl( mol_lzma.lzma_base_va + INT_DONE_STATUS);
	if ((v & status) == status) {
		val = readl( mol_lzma.lzma_base_va + INT_DONE_CLEAR);
		val |= LZMA_DONE_CLEAR;
		writel(val, mol_lzma.lzma_base_va +  INT_DONE_CLEAR);
	
		val = readl( mol_lzma.lzma_base_va + INT_DONE_CLEAR);
		val &= ~LZMA_DONE_CLEAR;
		writel(val, mol_lzma.lzma_base_va +  INT_DONE_CLEAR);

		p_lzma->judge_result = true;
	} else {
		writel(v, mol_lzma.lzma_base_va +  INT_DONE_CLEAR);
		
		val = readl(mol_lzma.lzma_base_va + DECODE_CTRL0);
		val |= SFOT_RST_LZMA;
		writel(val, mol_lzma.lzma_base_va + DECODE_CTRL0);

		val = readl(mol_lzma.lzma_base_va + DECODE_CTRL0);
		val &= ~SFOT_RST_LZMA;
		writel(val, mol_lzma.lzma_base_va + DECODE_CTRL0);

		writel(val, mol_lzma.lzma_base_va + DECODE_CTRL0);

		p_lzma->judge_result = false;
	}
	p_lzma->condition_for_wakeup = true;

	wake_up_interruptible(&wait_queue);

	return IRQ_HANDLED;
}

static int molchip_lzma_open(struct inode *inode, struct file *file)
{
	file->private_data = &mol_lzma;
    return 0;
}

static long lzma_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct custom_data data;
	struct molchip_lzma_drv *p_lzma = (struct molchip_lzma_drv *)file->private_data;

    switch (cmd) {
        case LZMA_DECODE_CMD:
			
            if (copy_from_user(&data, (void __user *)arg, sizeof(struct custom_data))) {
                return -EFAULT;
            }

            mutex_lock(&mol_lzma.lz_mutex);
	
            mol_lzma_write(data.src, data.dst, data.len);

            wait_event_interruptible(wait_queue, p_lzma->condition_for_wakeup);

			p_lzma->condition_for_wakeup = false;

            mutex_unlock(&mol_lzma.lz_mutex);

			if(p_lzma->judge_result == false)
				return -1;

            break;

        default:
            return -ENOTTY; 
    }

    return 0;
}

static const struct file_operations molchip_lzma_fops = {
    .owner = THIS_MODULE,
    .open = molchip_lzma_open,
    .unlocked_ioctl = lzma_ioctl,
};

static int molchip_lzma_probe(struct platform_device *pdev)
{
    int ret,irq;
    struct resource *res;

    ret = clk_mini_enable(CKG_LZMA_CORE_EN);
    if (ret) {
        dev_err(&pdev->dev, "Unable to enable ckg_lzma_en clock.\n");
        return ret;
    }
	
    ret = clk_mini_enable(CKG_IOE_PUB_EN);
    if (ret) {
        dev_err(&pdev->dev, "Unable to enable ckg_ioe_pub_en clock.\n");
        goto  clk_get_err;
    }

	ret = of_property_read_u32(pdev->dev.of_node, "lzma-max-frequency", &mol_lzma.max_freq);
	if (ret) {
		dev_err(&pdev->dev, "Execution environment unspecified\n");
		return ret;
	}

	ret = clk_mini_set_rate(CKG_MUX_LZMA_CORE,mol_lzma.max_freq);
	if (ret) {
			dev_err(&pdev->dev, "failed to set clk mux\r\n");
			goto clk_sel_err;
	}

	mutex_init(&mol_lzma.lz_mutex);

	platform_set_drvdata(pdev,&mol_lzma);

	irq = platform_get_irq(pdev, 0);

	ret = devm_request_irq(&pdev->dev, irq, mc_lzma_irq_handler, IRQF_SHARED,
					    "mc-lzma", pdev);
	if (ret) {
		dev_err(&pdev->dev,
			"cannot register lzma interrupt handler err=%d\n",
			ret);
		return ret;
	}

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
            dev_err(&pdev->dev, "not provide mem resource\n");
            return -ENODEV;
    }
	
    mol_lzma.lzma_base_va = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(mol_lzma.lzma_base_va))
            return PTR_ERR(mol_lzma.lzma_base_va);

	mol_lzma.judge_result = false;

    mol_lzma.my_misc_device = devm_kzalloc(&pdev->dev, sizeof(struct miscdevice), GFP_KERNEL);
    if (!mol_lzma.my_misc_device)
        return -ENOMEM;

    mol_lzma.my_misc_device->minor = MISC_DYNAMIC_MINOR;
    mol_lzma.my_misc_device->name = "molchip_lzma";
    mol_lzma.my_misc_device->fops = &molchip_lzma_fops;

    ret = misc_register(mol_lzma.my_misc_device);
    if (ret) {
        dev_err(&pdev->dev, "Failed to register misc device\n");
        return ret;
    }
	
	printk("molchip lzma drive register ok \r\n");
	
    return 0;
clk_sel_err:
	clk_mini_disable(CKG_LZMA_CORE_EN);
	clk_mini_disable(CKG_IOE_PUB_EN);
clk_get_err:
	clk_mini_disable(CKG_LZMA_CORE_EN);
	return ret;
	
}

static int molchip_lzma_remove(struct platform_device *pdev)
{
    misc_deregister(mol_lzma.my_misc_device);
    return 0;
}

static const struct of_device_id molchip_lzma_of_match[] = {
    { .compatible = "molchip,lzma" },
    {},
};

MODULE_DEVICE_TABLE(of, molchip_lzma_of_match);

static struct platform_driver molchip_lzma_driver = {
    .driver = {
        .name = "molchip-lzma-device",
        .of_match_table = molchip_lzma_of_match,
    },
    .probe = molchip_lzma_probe,
    .remove = molchip_lzma_remove,
};

module_platform_driver(molchip_lzma_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gilbert Shi");
MODULE_DESCRIPTION("Molchip Lzma Driver");
