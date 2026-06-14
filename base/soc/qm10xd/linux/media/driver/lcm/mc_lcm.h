#ifndef _MC_VOU_H_
#define _MC_VOU_H_

#include "mol_lcm.h"

#if 0
struct vou_tasklet_info {
	struct tasklet_struct tasklet[VOU_DEV_IDX_NUM];
	spinlock_t lock[VOU_DEV_IDX_NUM];
	uint32_t irqBits[VOU_DEV_IDX_NUM];
};

struct mc_vou_t {
	struct device *dev;
	struct mutex mutex;
	vou_cfg_t context;
	int ref_cnt;
	struct vou_tasklet_info tasklet_info;
	struct dentry *debugfs_dir;
};
#endif

lcm_handle_t *get_lcm_handle(int lcm_id);

#endif
