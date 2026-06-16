#ifndef _TRACE_DEBUG_H_
#define _TRACE_DEBUG_H_

#define MAX_TRACEARRY_SIZE  0x20



#define UART_IDENTIFY_FAILED        (1ULL << 0)
#define NOR_ERASE_FAILED	(1ULL << 1)






void write_trace_flag(u64 flag);


void write_trace_arry(u32 idx,u32 val);



#endif
