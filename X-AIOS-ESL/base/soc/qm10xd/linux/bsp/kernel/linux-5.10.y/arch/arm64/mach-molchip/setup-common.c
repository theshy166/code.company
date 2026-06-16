#include <linux/device.h>
#include <asm/dma-mapping.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <asm/io.h>

extern struct dma_map_ops swiotlb_dma_ops;

void arch_setup_pdev_archdata(struct platform_device *pdev)
{
 	set_dma_ops(&pdev->dev, &swiotlb_dma_ops);
}

