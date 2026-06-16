#ifndef __LOW_POWER_H
#define __LOW_POWER_H

#define S_MAX_NUM	4
struct scene_mangt_priv {
	struct clk * clk_en;
	struct clk * clk_sel;
	u32 s0[S_MAX_NUM];
	u32 s1[S_MAX_NUM];
	u32 s2[S_MAX_NUM];
	u32 switch_enable;
	u32 switch_type;
};

#define SCNEN_SCENARIO0		0
#define SCNEN_SCENARIO1		1
#define SCNEN_SCENARIO2		2

#define CMD_SEL_SCENARIO0	_IOW('p', 1, int)
#define CMD_SEL_SCENARIO1	_IOW('p', 2, int)
#define CMD_SEL_SCENARIO2	_IOW('p', 3, int)

#define VEU_LEVEL_ID	0x1

unsigned int get_scene_sel(void);

#endif
