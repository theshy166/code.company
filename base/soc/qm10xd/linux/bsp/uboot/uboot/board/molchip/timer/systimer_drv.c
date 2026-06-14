#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>




u32 get_sys_ticks(void)
{
	u32 ret;
	ret = __raw_readl(REG_SYS_TIMER_VALUE);

	return ret;

}

void delayms(uint ms)
{
	u32 v;
	u32 v1,v2;
	v1 = get_sys_ticks();
	do
	{
		v2 = get_sys_ticks();
		v = v2 - v1;
	}while(v < ms);

}
