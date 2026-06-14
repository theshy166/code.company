#include <linux/io.h>
#include <linux/slab.h>

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/regmap.h>
#include <linux/platform_device.h>
#include <linux/mfd/syscon.h>

#include <linux/molchip.h>

#include "molchip_pm.h"

struct mol_io_set {
	unsigned int addr;
	struct regmap * mmap;
};

static struct mol_io_set io_set[] = {
	{0x10200000, NULL},
	{0x25B00000, NULL},
	{0x0B100000, NULL},
};

#define EXTERNAL_PHY_WAKE_BIT	(BIT(31))
#define GET_GPIO_NUMBER(x)		((x >> 8) & 0xFF)

static struct regmap * get_regmap_by_address(unsigned int addr)
{
	int i;
	struct regmap * mmap = NULL;
	for (i = 0; i < ARRAY_SIZE(io_set); i++) {
		if (io_set[i].addr == addr) {
			mmap = io_set[i].mmap;
			break;
		}
	}
	return mmap;
}

#define IO_QUEUE_MAX_SIZE	0x100
struct io_cfg_queue {
	struct reg_info_t io_info[IO_QUEUE_MAX_SIZE];
	int front;
	int rear;
};

static struct io_cfg_queue io_queue;

static unsigned int sleep_io_cfg[] = {
	#include "soc_init_reg_list_io.h"
};

void initQueue(struct io_cfg_queue *Q) {
	Q->front = Q->rear = 0;
}

static bool isEmpty(struct io_cfg_queue *Q) {
	if(Q->front==Q->rear)
		return true;
	else
		return false;
}

static bool isFull(struct io_cfg_queue *Q) {
	if((Q->rear + 1) % IO_QUEUE_MAX_SIZE == Q->front)
		return true;
	else
		return false;
}

static bool enqueue(struct io_cfg_queue *Q, struct reg_info_t * reg) {
	if (isFull(Q)) {
		pr_err("Queue is full\n");
		return false;
	}  
	Q->io_info[Q->rear].regv = reg->regv;
	Q->io_info[Q->rear].reg_addr = reg->reg_addr;
	Q->rear = (Q->rear + 1) % IO_QUEUE_MAX_SIZE;
	return true;
}

static bool dequeue(struct io_cfg_queue *Q,struct reg_info_t * reg) {
	if (isEmpty(Q)) {
		pr_err("Queue is empty\n");
		return false;
	}
	reg->regv = Q->io_info[Q->front].regv;
	reg->reg_addr = Q->io_info[Q->front].reg_addr;
	Q->front = (Q->front + 1) % IO_QUEUE_MAX_SIZE;
	return true;
}

int mol_sleep_io_cfg_init(void)
{
	io_set[0].mmap = syscon_regmap_lookup_by_compatible("molchip,mc-pctl-syscfg");
	if (IS_ERR_OR_NULL(io_set[0].mmap)) {
		pr_err("failed to get syscon 0x10200000\n");
		goto err_out;
	}
	io_set[1].mmap = syscon_regmap_lookup_by_compatible("molchip,mc-pctl-a-syscfg");
	if (IS_ERR_OR_NULL(io_set[1].mmap)) {
		pr_err("failed to get syscon 0x25b00000\n");
		goto err_out;
	}
	io_set[2].mmap = syscon_regmap_lookup_by_compatible("molchip,mc-pctl-b-syscfg");
	if (IS_ERR_OR_NULL(io_set[0].mmap)) {
		pr_err("failed to get syscon 0x0b100000\n");
		goto err_out;
	}

	initQueue(&io_queue);

	return 0;
err_out:
	return -ENODEV;
}

static int sleep_io_save_state(unsigned int addr, unsigned int val)
{
	struct reg_info_t reg;
	bool ret;
	reg.regv = val;
	reg.reg_addr = addr;
	ret = enqueue(&io_queue, &reg);
	if (!ret)
		return -ENOMEM;
	return 0;
}

static int wakeup_io_restore(struct reg_info_t * reg)
{
	bool ret;
	ret = dequeue(&io_queue, reg);
	if (!ret)
		return -EINVAL;
	return 0;
}

int mol_sleep_io_func(void)
{
	struct reg_info_t *dbg = NULL;
	unsigned int cnt, i;
	struct regmap * mmap = NULL;
	unsigned int val;
	unsigned int base_addr = 0, off_addr = 0;
	int ret = 0;

	cnt = sleep_io_cfg[0];
	dbg = (struct reg_info_t *)&sleep_io_cfg[1];

	for (i = 0; i < cnt; i++, dbg++) {
// for external phy wakeup
		if (external_phy_wakeup_sys()) {
			if (dbg->regv & EXTERNAL_PHY_WAKE_BIT)
				continue;
		}
// for gpio wakeup
		if (gpio_is_wakeup_sys(GET_GPIO_NUMBER(dbg->regv)))
			continue;

		if (mol_ioremap_reuse(base_addr, dbg->reg_addr) == false) {
			base_addr = mol_ioremap_base_get(dbg->reg_addr);

			mmap = get_regmap_by_address(base_addr);
			if (mmap == NULL)
				return -ENOMEM;
		}

		off_addr = mol_ioremap_off_get(dbg->reg_addr);
		
		ret = regmap_read(mmap, off_addr, &val);
		if (ret) {
			pr_err("failed to get reg 0x%x\n", off_addr);
			break;
		}

		ret = sleep_io_save_state(dbg->reg_addr, val);
		if (ret)
			break;

		val &= ~dbg->mask;
		val |= dbg->regv;
		ret = regmap_write(mmap, off_addr, val);
		if (ret) {
			pr_err("failed to write reg 0x%x\n", off_addr);
			break;
		}
	}

	return ret;
}

int mol_wakeup_io_restore_state(void)
{
	struct reg_info_t reg;
	struct regmap * mmap = NULL;
	unsigned int base_addr = 0, off_addr = 0;
	int ret = 0;

	while(!isEmpty(&io_queue)) {
		ret = wakeup_io_restore(&reg);
		if (ret) {
			break;
		}

		if (mol_ioremap_reuse(base_addr, reg.reg_addr) == false) {
			base_addr = mol_ioremap_base_get(reg.reg_addr);

			mmap = get_regmap_by_address(base_addr);
			if (mmap == NULL)
				return -ENOMEM;
		}

		off_addr = mol_ioremap_off_get(reg.reg_addr);
		ret = regmap_write(mmap, off_addr, reg.regv);
		if (ret) {
			pr_err("failed to get reg 0x%x\n", off_addr);
			break;
		}
	}

	return ret;
}


