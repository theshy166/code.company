#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mfd/syscon.h>
#include <linux/c2c.h>

struct c2c_data {
	void __iomem *c2c0_base;
	void __iomem *c2c1_base;
};

static struct c2c_data c2c;

void __iomem *get_c2c0_base(void)
{
	return c2c.c2c0_base;
}

void __iomem *get_c2c1_base(void)
{
	return c2c.c2c1_base;
}

static int c2c_probe(struct platform_device *pdev)
{
	struct resource *res;
	static struct c2c_data *c2c_p = &c2c;
	unsigned int id;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	of_property_read_u32(pdev->dev.of_node, "dev_id", &id);

	if(id == 0) {
		c2c_p->c2c0_base = ioremap(res->start, resource_size(res));
	} else if(id == 1) {
		c2c_p->c2c1_base = ioremap(res->start, resource_size(res));
	} else {
		return -EINVAL;
	}

	platform_set_drvdata(pdev, c2c_p);

	return 0;
}

static const struct of_device_id c2c_of_match[] = {
	{ .compatible = "molchip,c2c", },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, c2c_of_match);

static struct platform_driver c2c_driver = {
		.driver = {
		.name   = "c2c",
		.of_match_table = of_match_ptr(c2c_of_match),
	},
	.probe  = c2c_probe,
};

module_platform_driver(c2c_driver);

MODULE_DESCRIPTION("c2c driver for MOLCHIP");
MODULE_AUTHOR("www.molchip.com");
MODULE_LICENSE("GPL");
