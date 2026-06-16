#include <linux/linkage.h>

#define MOL_DDR_COMPLE_START 	0x40000000
#define MOL_DDR_COMPLE_SIZE		0x300000

#define MOL_PORT_BUSY				0x3fc
#define MOL_SELFRFSH_FUN			 0x30
#define SELFRFSH_POS                 5
#define SELFRFSH_MASK                0x20

#define MOL_SELFRFSH_STATE			0x04
#define SELFRFSH_STATE_POS			4
#define SELFRFSH_STATE_MASK			0x30
#define MOL_SELFRFSH_VAL			0x20

#define MOL_CHECKSUM_INIT_VAL		0x6f6b
#define MOL_CHECKSUM_FAIL_VAL		0x6264

	.arch   armv7-a
	.text
	.align 3

ENTRY(molchip_sdram_self_refresh)
/*
*checksum before self-refresh enter
*/
	movw r5, 0x0
	movt r5, 0x1000
	mov r4, #0
	ldr r1, =MOL_DDR_COMPLE_START
    ldr r2, =MOL_DDR_COMPLE_SIZE

sum_loop_slp:
    ldr r3, [r1], #4
    add r4, r4, r3
    subs r2, #4
    bne sum_loop_slp
	str r4, [r5, #0x514]

/*
* retention enter dump
*/
       movw r1, 0x200
       movt r1, 0x2420
       mov r2, 0x1b0
       movw r3, 0x400
       movt r3, 0x2700
dump_ddr_enter_loop:
    ldr r4, [r1], #4
    str r4, [r3], #4
    subs r2, #4
    bne dump_ddr_enter_loop

/*
*self-refresh enter
*/
	movw r1, #0x0
	movt r1, #0x2410
wait_port_busy:
	ldr r2, [r1, #MOL_PORT_BUSY]
	cmp r2, #0
	bne wait_port_busy
	ldr	r2, [r1, #MOL_SELFRFSH_FUN]
	orr	r2, r2, #SELFRFSH_MASK
	str	r2, [r1, #MOL_SELFRFSH_FUN]
ck_lp:	ldr r4, [r1, #MOL_SELFRFSH_STATE]          
	and r4, r4, SELFRFSH_STATE_MASK
	cmp r4, #MOL_SELFRFSH_VAL
	bne ck_lp
	isb
	dmb
/*
*sys sleep cfg
*/
	add r0,r0,#0x1000       // r0 = config[0]
cfg_lp:
	ldr r1,[r0]            // r1 = config[0].addr
	cmp r1,#0x0
	beq unexpected_lp
	and r2, r1, #0x3
	cmp r2, #0x0
	bne polling_sts
	b set_reg
cntn:
	add r0,r0,#0xc
	b cfg_lp

set_reg:
	ldr r1,[r0]            // r1 = config[n].addr
	ldr r2,[r0,#0x4]       // r2 = config[n].val
	ldr r3,[r0,#0x8]       // r3 = config[n].mask
	ldr r4,[r1]            // r4 = data of config[0].addr
	mvn r5,r3              // r5 = ~mask
	and r4,r4,r5           // data &= ~mask
	and r2,r2,r3           // val &= mask
	orr r4,r4,r2           // data |= val
	str r4,[r1]            // config data to addr
	b cntn

polling_sts:
	ldr r1,[r0]            // r1 = config[n].addr
	ldr r2,[r0,#0x4]       // r2 = config[n].val
	ldr r3,[r0,#0x8]       // r3 = config[n].mask
	movw r6, #0xfffc
	movt r6, #0xffff
	and r1, r1, r6
	ldr r4,[r1]            // r4 = data of config[0].addr

polling_ck_lp:	

	mov r5,r3              // data &= mask
	and r4,r4,r5
	cmp r4,r2              // data == val ?
	bne polling_ck_lp
	b cntn

unexpected_lp:
/*
* change cpu-freq to 24M
*/
	movw r1, #0x0220
	movt r1, #0x0920
	mov r2, #0x0
	str r2, [r1]
	isb
	dmb
	dsb
	wfi

	movw r1, #0x0320
	movt r1, #0x1010
polling_pll_cnt_done:
	ldr r2, [r1]
	and r2, r2, #0x1
	cmp r2, #1
	bne polling_pll_cnt_done

/*
*ddr clk enable
*/
	movw r1, #0x300
	movt r1, #0x2400

	movw r2, #0x2001
	movt r2, #0x2c21
	
	ldr r3, [r1]
	orr r3, r3, r2
	str r3, [r1]

/*
*self-refresh exit
*/	
	movw r1, #0x0
	movt r1, #0x2410
	ldr	r2, [r1, #MOL_SELFRFSH_FUN]
	mvn r3, #SELFRFSH_MASK
	and	r2, r2, r3
	str	r2, [r1, #MOL_SELFRFSH_FUN]
ck_lp_exit:	ldr r4, [r1, #MOL_SELFRFSH_STATE]          
	and r4, r4, SELFRFSH_STATE_MASK
	cmp r4, #0
	bne ck_lp_exit
	isb
	dmb

	movw r1, #0x0220
	movt r1, #0x0920
	mov r2, #0x4
	str r2, [r1]

	ldr r1, =#MOL_DDR_COMPLE_SIZE
busy_loop:
	subs r1, #4
	bne busy_loop

/*
* retention exit dump
*/
	movw r1, 0x200
	movt r1, 0x2420
	mov r2, 0x1b0
	movw r3, 0x600
	movt r3, 0x2700
dump_ddr_exit_loop:
    ldr r4, [r1], #4
    str r4, [r3], #4
    subs r2, #4
    bne dump_ddr_exit_loop

/*
*checksum after exit self-refresh
*/
	movw r5, 0x0
	movt r5, 0x1000
	mov r4, #MOL_CHECKSUM_INIT_VAL
	str r4, [r5, #0x51c]
	mov r4, #0
	ldr r1, =MOL_DDR_COMPLE_START
    ldr r2, =MOL_DDR_COMPLE_SIZE
sum_loops_wake:
    ldr r3, [r1], #4
    add r4, r4, r3
    subs r2, #4
    bne sum_loops_wake
    str r4, [r5, #0x518]
    ldr r6, [r5, #0x514]
    cmp r6, r4
    beq cmp_s
    mov r4, #MOL_CHECKSUM_FAIL_VAL
    str r4, [r5, #0x51c]
    b .
cmp_s:
/*
*turn on mmu
*/
	mrc p15, 0, r1, c1, c0, 0
	orr r1, r1, #0x5
	mcr p15, 0, r1, c1, c0, 0
	
	bx lr

ENDPROC(molchip_sdram_self_refresh)
ENTRY(molchip_sdram_self_refresh_sz)
	.word	. - molchip_sdram_self_refresh

