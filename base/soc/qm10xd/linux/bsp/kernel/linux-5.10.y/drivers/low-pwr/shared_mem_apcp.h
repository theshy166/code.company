#ifndef __SHARED_CLK_H_
#define __SHARED_CLK_H_

#define REG_SET_OFFSET			0x1000
#define REG_CLEAR_OFFSET		0x2000

#define VEU_CLK		0
#define VICAP_CLK	1

#define SHARE_MEM_REG		0x1c0000fc
/**
*	clk_mux_sel maybe shared by two or more than two IP;
**/
struct mc_rate_clock {
	unsigned int	address;
	unsigned int	offset;
	unsigned int	shift;
	unsigned int	width;

	unsigned int	curr_parent;
	unsigned int	parent_num;

	unsigned int	*parents_HZ;
};

struct mc_clock {
	unsigned int	id;
	unsigned int	address;
	unsigned int	offset;
	unsigned int	bit_idx;
	unsigned int	ref_cnt;

	struct mc_rate_clock rate;
};

struct clk_info {
	int id;
	int ref_cnt;
	unsigned int rate;
	const char file[32];

};

#endif
