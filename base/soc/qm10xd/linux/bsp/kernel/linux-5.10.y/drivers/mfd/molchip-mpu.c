#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/mfd/syscon.h>

#define MPU_CFG					0x0
#define MPU_REMAP_REGINE0_CFG	0x4
#define MPU_REMAP_REGINE1_CFG	0x8
#define MPU_REMAP_REGINE2_CFG	0xc
#define MPU_REMAP_REGINE3_CFG	0x10
#define MPU_REMAP_REGINE4_CFG	0x14
#define MPU_REMAP_REGINE5_CFG	0x18
#define MPU_REMAP_REGINE6_CFG	0x1c
#define MPU_REMAP_REGINE7_CFG	0x20
#define MPU_VIO_ADDR0			0x24
#define MPU_VIO_ADDR1			0x28
#define MPU_VIO_CMD				0x2c
#define DMC_MPU_RANGE0			0x30
#define DMC_MPU_RANGE1			0x34

#define DMC_MPU_ENABLE			BIT(0)
#define DMC_MPU_REMAP_EN		BIT(1)
#define DMC_MPU_SEL				BIT(2)
#define DMC_MPU_CLR				BIT(3)
#define DMC_MPU_IRQ				BIT(4)

#define MPU_CHN_NUM				3

static char mbuf[] = "Usage:\n\techo -s [-y/n] [mpu_port] [addr_range] [open/close]\nexample:\n\techo -s -n 0 0x15a000000 0x15a010000 close\n\t-y/n polarity\n\ty--address outside the specified range\n\tn--address inside the specified range\n\tmpu_port--it refers to chn num\n\topen/close--open or close the switch";
static char parameters[6][16];

struct molchip_mpu{
	struct device *dev;
	void __iomem *wr_mpubase[MPU_CHN_NUM];
	void __iomem *rd_mpubase[MPU_CHN_NUM];
	struct regmap *dwc_apb_reg;
	int irq;
	int mpu_irq_mask_regoff;
	spinlock_t lock;
};

static void mpu_init(struct molchip_mpu *mpu)
{
	int i,j;

	spin_lock(&mpu->lock);
	for(i=0;i<3;i++)
	{
		writel(DMC_MPU_CLR, mpu->wr_mpubase[i]);
		for(j=0;j<14;j++)
			writel(0, (mpu->wr_mpubase[i]+(j*4)));
	}

	for(i=0;i<3;i++)
	{
		writel(DMC_MPU_CLR, mpu->rd_mpubase[i]);
		for(j=0;j<14;j++)
			writel(0, (mpu->rd_mpubase[i]+(j*4)));
	}

	spin_unlock(&mpu->lock);
}

static void mpu_enable(struct molchip_mpu *mpu,int port)
{
	u32 val;

	spin_lock(&mpu->lock);

	val = readl(mpu->wr_mpubase[port] + MPU_CFG);
	val |= DMC_MPU_ENABLE;
	writel(val, mpu->wr_mpubase[port] + MPU_CFG);

	val = readl(mpu->rd_mpubase[port] + MPU_CFG);
	val |= DMC_MPU_ENABLE;
	writel(val, mpu->rd_mpubase[port] + MPU_CFG);

	regmap_update_bits(mpu->dwc_apb_reg, mpu->mpu_irq_mask_regoff,(1<<port), (1<<port));
	regmap_update_bits(mpu->dwc_apb_reg, mpu->mpu_irq_mask_regoff,(1<<(port+1)), (1<<(port+1)));

	spin_unlock(&mpu->lock);
}

static void mpu_disable(struct molchip_mpu *mpu,int port)
{
	u32 val;

	spin_lock(&mpu->lock);

	val = readl(mpu->wr_mpubase[port] + MPU_CFG);
	val &= ~DMC_MPU_ENABLE;
	writel(val, mpu->wr_mpubase[port] + MPU_CFG);

	val = readl(mpu->rd_mpubase[port] + MPU_CFG);
	val &= ~DMC_MPU_ENABLE;
	writel(val, mpu->rd_mpubase[port] + MPU_CFG);

	regmap_update_bits(mpu->dwc_apb_reg, mpu->mpu_irq_mask_regoff,(1<<port), 0);
	regmap_update_bits(mpu->dwc_apb_reg, mpu->mpu_irq_mask_regoff,(1<<(port+1)), 0);

	spin_unlock(&mpu->lock);
}


static void dmc_mpu_range_set(struct molchip_mpu *mpu,long range0,long range1,int mpu_port)
{
	spin_lock(&mpu->lock);
	writel( (unsigned int)(range0>>16), mpu ->wr_mpubase[mpu_port] + DMC_MPU_RANGE0);
	writel( (unsigned int)(range1>>16), mpu ->wr_mpubase[mpu_port] + DMC_MPU_RANGE1);

	writel( (unsigned int)(range0>>16), mpu ->rd_mpubase[mpu_port] + DMC_MPU_RANGE0);
	writel( (unsigned int)(range1>>16), mpu ->rd_mpubase[mpu_port] + DMC_MPU_RANGE1);
	spin_unlock(&mpu->lock);
}

static void dmc_mpu_match_select(struct molchip_mpu *mpu, int port, int flag)
{
	u32 val;

	spin_lock(&mpu->lock);
	if(flag) {
		val = readl(mpu->wr_mpubase[port] + MPU_CFG);
		val |= DMC_MPU_SEL;
		writel(val,mpu->wr_mpubase[port] + MPU_CFG);

		val = readl(mpu->rd_mpubase[port] + MPU_CFG);
		val |= DMC_MPU_SEL;
		writel(val,mpu->rd_mpubase[port] + MPU_CFG);
	}
	else {
		val = readl(mpu->wr_mpubase[port] + MPU_CFG);
		val &= ~DMC_MPU_SEL;
		writel(val,mpu->wr_mpubase[port] + MPU_CFG);

		val = readl(mpu->rd_mpubase[port] + MPU_CFG);
		val &= ~DMC_MPU_SEL;
		writel(val,mpu->rd_mpubase[port] + MPU_CFG);
	}

	spin_unlock(&mpu->lock);
}

static ssize_t show_reg(struct device *dev, struct device_attribute *attr,char *buf)
{
	return sprintf(buf, "%s\n", mbuf);
}

static ssize_t store_reg(struct device *dev ,struct device_attribute *attr,const char *buf,size_t len)
{
	u32 i,x=0,y=0;
	u32 range0_addr,range1_addr;
	int mpu_port=0;

	struct molchip_mpu *mpu=dev->driver_data;
	for (i = 0; buf[i] != 0; i++) {
		if ((x >= 6) || (y >= 16)) {
			dev_err(dev, "parameters greater than 6 or string is too long\n");
			return len;
		}
		if (buf[i] != ' ') {
			parameters[x][y++] = buf[i];
		} else {
			if (y) {
				parameters[x++][y] = 0;
			}
			y = 0;
		}
	}

	mpu_port = simple_strtol(parameters[2], NULL, 0);
	if(mpu_port>= MPU_CHN_NUM) {
		printk("invalid parameters\r\n");
		return -EINVAL;
	}

	range0_addr = simple_strtol(parameters[3], NULL, 0);
	range1_addr = simple_strtol(parameters[4], NULL, 0);

	printk("range0_addr--0x%x,range1_addr1--0x%x\r\n",range0_addr,range1_addr);

	if(parameters[1][1] == 'y') {
		dmc_mpu_range_set(mpu,range0_addr,range1_addr,mpu_port);
		dmc_mpu_match_select(mpu,mpu_port, 0);
	}
	else {
		dmc_mpu_range_set(mpu,range0_addr,range1_addr,mpu_port);
		dmc_mpu_match_select(mpu,mpu_port, 1);
	}

	if(parameters[5][0] == 'o')
		mpu_enable(mpu,mpu_port);
	else
		mpu_disable(mpu,mpu_port);

	return len;
}

static irqreturn_t mc_mpu_irq_handler(int irq, void *data)
{
	u32 val,addr;
	int i;

	struct molchip_mpu *mpu=platform_get_drvdata((struct platform_device *)data);
	for(i=0;i<MPU_CHN_NUM;i++)
	{
		val = readl(mpu->rd_mpubase[i]+MPU_CFG);
		if(!(val&DMC_MPU_IRQ))
			break;

		addr = readl(mpu->rd_mpubase[i]+MPU_VIO_ADDR0);
		printk("0x%x have read operation\r\n", addr);
		val |= DMC_MPU_CLR;
		writel(val, mpu->rd_mpubase[i]+MPU_CFG);

		val = readl(mpu->rd_mpubase[i]+MPU_CFG);
		val &= ~DMC_MPU_CLR;
		writel(val, mpu->rd_mpubase[i]+MPU_CFG);
	}

	for(i=0;i<MPU_CHN_NUM;i++)
	{
		val = readl(mpu->wr_mpubase[i]+MPU_CFG);
		if(!(val&DMC_MPU_IRQ))
			break;

		addr = readl(mpu->wr_mpubase[i]+MPU_VIO_ADDR0);
		printk("0x%x have write operation\r\n", addr);
		val |= DMC_MPU_CLR;
		writel(val, mpu->wr_mpubase[i]+MPU_CFG);

		val = readl(mpu->wr_mpubase[i]+MPU_CFG);
		val &= ~DMC_MPU_CLR;
		writel(val, mpu->wr_mpubase[i]+MPU_CFG);
	}
	return IRQ_HANDLED;
}

static DEVICE_ATTR(mpu, S_IWUSR|S_IRUSR, show_reg, store_reg); 

static int molchip_mpu_probe(struct platform_device *pdev)
{
	struct molchip_mpu *mpu;
	struct resource *res;
	struct device *dev = &pdev->dev;
	struct device_node *node =dev->of_node;
	int ret,i;

	ret = device_create_file(&pdev->dev, &dev_attr_mpu);
	if (ret) {
		dev_err(&pdev->dev, "failed to register attributes, ret=%d\n", ret);
		return ret;
	}

	mpu = devm_kzalloc(&pdev->dev, sizeof(*mpu),
				GFP_KERNEL);
	if (!mpu)
		return -ENOMEM;
	
	platform_set_drvdata(pdev, mpu);
	
	for(i=0 ; i < MPU_CHN_NUM;i++){
		res = platform_get_resource(pdev, IORESOURCE_MEM, i);
		mpu->wr_mpubase[i] = devm_ioremap_resource(dev, res);
		if (!res) {
		dev_err(&pdev->dev, "not provide mem resource\n");
		return -ENODEV;
		}
	}
	for(i=0 ; i < MPU_CHN_NUM;i++){
		res = platform_get_resource(pdev, IORESOURCE_MEM, MPU_CHN_NUM+i);
		mpu->rd_mpubase[i] = devm_ioremap_resource(dev, res);
		if (!res) {
		dev_err(&pdev->dev, "not provide mem resource\n");
		return -ENODEV;
		}
	}

	mpu->dwc_apb_reg = syscon_regmap_lookup_by_phandle(node,"molchip,dmc-syscon");
	if (WARN_ON(!mpu->dwc_apb_reg))
			return -ENOMEM;

	ret = of_property_read_u32(dev->of_node, "mpu-irq-mask-regoff", &mpu->mpu_irq_mask_regoff);
	if (ret < 0) {
		dev_err(&pdev->dev, "invalid mpu-irq-mask-regoff\n");
	}
	mpu->dev = &pdev->dev;
	mpu->irq = platform_get_irq(pdev, 0);
	if (mpu->irq <= 0)
		return -EINVAL;
	else {
		ret =devm_request_irq(&pdev->dev, mpu->irq,mc_mpu_irq_handler,
				     0, pdev->name, pdev);
		if (ret) {
			dev_err(&pdev->dev,
				"cannot register interrupt handler err=%d\n",
				ret);
			return ret;
		}
	}
	spin_lock_init(&mpu->lock);
	mpu_init(mpu);

	return 0;

}

static const struct of_device_id mc_mpu_match[] = {
	{ .compatible = "mc,mpu", },
	{}
};
MODULE_DEVICE_TABLE(of, mc_mpu_match);

static struct platform_driver mc_mpu_driver = {
	.driver = {
		.name = "mc,mpu",
		.of_match_table = mc_mpu_match,
	},
	.probe = molchip_mpu_probe,
};
module_platform_driver(mc_mpu_driver);
