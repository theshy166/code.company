#include <common.h>
#include <mach/trace_debug.h>

u64 trace_flag __attribute__((section(".data"))) = 0;
u32 trace_arry[MAX_TRACEARRY_SIZE] __attribute__((section(".data")));

void write_trace_flag(u64 flag)
{
	trace_flag |= flag;

}


void write_trace_arry(u32 idx,u32 val)
{
	trace_arry[idx]= val;
}
