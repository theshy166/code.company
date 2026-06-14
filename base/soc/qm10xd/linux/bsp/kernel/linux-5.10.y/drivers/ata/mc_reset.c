#include <linux/kernel.h>

#include <linux/blkdev.h>
#include <linux/export.h>
#include <linux/pci.h>
#include <scsi/scsi.h>

#include <linux/libata.h>

#include <trace/events/libata.h>
#include "libata.h"

#include "ahci.h"
#include <linux/regmap.h>

static void ahci_restore_feature_config(struct ata_host * host)
{
	struct ahci_host_priv *hpriv = host->private_data;
	void __iomem *mmio = hpriv->mmio;

	writel(hpriv->cap, mmio + HOST_CAP);
	if (hpriv->cap2)
		writel(hpriv->cap2, mmio + HOST_CAP2);
	writel(hpriv->port_map, mmio + HOST_PORTS_IMPL);
	(void) readl(mmio + HOST_PORTS_IMPL);	/* flush */
}

static void ahci_enable_ahci(void __iomem *mmio)
{
	int i;
	u32 tmp;

	/* turn on AHCI_EN */
	tmp = readl(mmio + HOST_CTL);
	if (tmp & HOST_AHCI_EN)
		return;

	/* Some controllers need AHCI_EN to be written multiple times.
	 * Try a few times before giving up.
	 */
	for (i = 0; i < 5; i++) {
		tmp |= HOST_AHCI_EN;
		writel(tmp, mmio + HOST_CTL);
		tmp = readl(mmio + HOST_CTL);	/* flush && sanity check */
		if (tmp & HOST_AHCI_EN)
			return;
		msleep(10);
	}
	WARN_ON(1);
}

static void sata_ctrl_reset_by_soft(struct ata_link *link){
	struct ahci_host_priv *hpriv = link->ap->host->private_data;

	regmap_update_bits(hpriv->vdu_ahb_mmio,hpriv->ctrl_rst[0],
			hpriv->ctrl_rst[2]<<hpriv->ctrl_rst[1],0x1<<hpriv->ctrl_rst[1]);
	msleep(1);
	regmap_update_bits(hpriv->vdu_ahb_mmio,hpriv->ctrl_rst[0],
			hpriv->ctrl_rst[2]<<hpriv->ctrl_rst[1],0x0<<hpriv->ctrl_rst[1]);
}

static void ahci_restore_controller_config(struct ata_link *link)
{
	struct ata_host * host = link->ap->host;
	struct ahci_host_priv *hpriv = link->ap->host->private_data;
	void __iomem *mmio = hpriv->mmio;
	u32 i;

	//1. make sure AHCI mode is enabled
	ahci_enable_ahci(mmio);

	//2. resore feature config
	ahci_restore_feature_config(host);

	//3. ahci init controller
	ahci_init_controller(host);

	//4.port cfg
	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];
		if (ata_port_is_dummy(ap))
			continue;
		ahci_port_resume(ap);
	}

}

void mc_reset_controller_sys(struct ata_link *link)
{
	sata_ctrl_reset_by_soft(link);
	ahci_restore_controller_config(link);

}

EXPORT_SYMBOL(mc_reset_controller_sys);

