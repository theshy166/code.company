
u-boot:     file format elf32-littlearm


Disassembly of section .text:

81600000 <__image_copy_start>:
81600000:	b8 00 00 ea 14 f0 9f e5 14 f0 9f e5 14 f0 9f e5     ................
81600010:	14 f0 9f e5 14 f0 9f e5 14 f0 9f e5 14 f0 9f e5     ................

81600020 <_undefined_instruction>:
81600020:	81600060 	.word	0x81600060

81600024 <_software_interrupt>:
81600024:	816000c0 	.word	0x816000c0

81600028 <_prefetch_abort>:
81600028:	81600120 	.word	0x81600120

8160002c <_data_abort>:
8160002c:	81600180 	.word	0x81600180

81600030 <_not_used>:
81600030:	816001e0 	.word	0x816001e0

81600034 <_irq>:
81600034:	81600240 	.word	0x81600240

81600038 <_fiq>:
81600038:	816002a0 	.word	0x816002a0
8160003c:	deadbeef 	.word	0xdeadbeef

81600040 <IRQ_STACK_START_IN>:
81600040:	0badc0de 	.word	0x0badc0de
81600044:	e320f000 	nop	{0}
81600048:	e320f000 	nop	{0}
8160004c:	e320f000 	nop	{0}
81600050:	e320f000 	nop	{0}
81600054:	e320f000 	nop	{0}
81600058:	e320f000 	nop	{0}
8160005c:	e320f000 	nop	{0}

81600060 <undefined_instruction>:
81600060:	e51fd028 	ldr	sp, [pc, #-40]	; 81600040 <IRQ_STACK_START_IN>
81600064:	e58de000 	str	lr, [sp]
81600068:	e14fe000 	mrs	lr, SPSR
8160006c:	e58de004 	str	lr, [sp, #4]
81600070:	e3a0d013 	mov	sp, #19
81600074:	e169f00d 	msr	SPSR_fc, sp
81600078:	e1a0e00f 	mov	lr, pc
8160007c:	e1b0f00e 	movs	pc, lr
81600080:	e24dd048 	sub	sp, sp, #72	; 0x48
81600084:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
81600088:	e51f2050 	ldr	r2, [pc, #-80]	; 81600040 <IRQ_STACK_START_IN>
8160008c:	e892000c 	ldm	r2, {r2, r3}
81600090:	e28d0048 	add	r0, sp, #72	; 0x48
81600094:	e28d5034 	add	r5, sp, #52	; 0x34
81600098:	e1a0100e 	mov	r1, lr
8160009c:	e885000f 	stm	r5, {r0, r1, r2, r3}
816000a0:	e1a0000d 	mov	r0, sp
816000a4:	eb0003e6 	bl	81601044 <do_undefined_instruction>
816000a8:	e320f000 	nop	{0}
816000ac:	e320f000 	nop	{0}
816000b0:	e320f000 	nop	{0}
816000b4:	e320f000 	nop	{0}
816000b8:	e320f000 	nop	{0}
816000bc:	e320f000 	nop	{0}

816000c0 <software_interrupt>:
816000c0:	e51fd088 	ldr	sp, [pc, #-136]	; 81600040 <IRQ_STACK_START_IN>
816000c4:	e58de000 	str	lr, [sp]
816000c8:	e14fe000 	mrs	lr, SPSR
816000cc:	e58de004 	str	lr, [sp, #4]
816000d0:	e3a0d013 	mov	sp, #19
816000d4:	e169f00d 	msr	SPSR_fc, sp
816000d8:	e1a0e00f 	mov	lr, pc
816000dc:	e1b0f00e 	movs	pc, lr
816000e0:	e24dd048 	sub	sp, sp, #72	; 0x48
816000e4:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
816000e8:	e51f20b0 	ldr	r2, [pc, #-176]	; 81600040 <IRQ_STACK_START_IN>
816000ec:	e892000c 	ldm	r2, {r2, r3}
816000f0:	e28d0048 	add	r0, sp, #72	; 0x48
816000f4:	e28d5034 	add	r5, sp, #52	; 0x34
816000f8:	e1a0100e 	mov	r1, lr
816000fc:	e885000f 	stm	r5, {r0, r1, r2, r3}
81600100:	e1a0000d 	mov	r0, sp
81600104:	eb0003d9 	bl	81601070 <do_software_interrupt>
81600108:	e320f000 	nop	{0}
8160010c:	e320f000 	nop	{0}
81600110:	e320f000 	nop	{0}
81600114:	e320f000 	nop	{0}
81600118:	e320f000 	nop	{0}
8160011c:	e320f000 	nop	{0}

81600120 <prefetch_abort>:
81600120:	e51fd0e8 	ldr	sp, [pc, #-232]	; 81600040 <IRQ_STACK_START_IN>
81600124:	e58de000 	str	lr, [sp]
81600128:	e14fe000 	mrs	lr, SPSR
8160012c:	e58de004 	str	lr, [sp, #4]
81600130:	e3a0d013 	mov	sp, #19
81600134:	e169f00d 	msr	SPSR_fc, sp
81600138:	e1a0e00f 	mov	lr, pc
8160013c:	e1b0f00e 	movs	pc, lr
81600140:	e24dd048 	sub	sp, sp, #72	; 0x48
81600144:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
81600148:	e51f2110 	ldr	r2, [pc, #-272]	; 81600040 <IRQ_STACK_START_IN>
8160014c:	e892000c 	ldm	r2, {r2, r3}
81600150:	e28d0048 	add	r0, sp, #72	; 0x48
81600154:	e28d5034 	add	r5, sp, #52	; 0x34
81600158:	e1a0100e 	mov	r1, lr
8160015c:	e885000f 	stm	r5, {r0, r1, r2, r3}
81600160:	e1a0000d 	mov	r0, sp
81600164:	eb0003cc 	bl	8160109c <do_prefetch_abort>
81600168:	e320f000 	nop	{0}
8160016c:	e320f000 	nop	{0}
81600170:	e320f000 	nop	{0}
81600174:	e320f000 	nop	{0}
81600178:	e320f000 	nop	{0}
8160017c:	e320f000 	nop	{0}

81600180 <data_abort>:
81600180:	e51fd148 	ldr	sp, [pc, #-328]	; 81600040 <IRQ_STACK_START_IN>
81600184:	e58de000 	str	lr, [sp]
81600188:	e14fe000 	mrs	lr, SPSR
8160018c:	e58de004 	str	lr, [sp, #4]
81600190:	e3a0d013 	mov	sp, #19
81600194:	e169f00d 	msr	SPSR_fc, sp
81600198:	e1a0e00f 	mov	lr, pc
8160019c:	e1b0f00e 	movs	pc, lr
816001a0:	e24dd048 	sub	sp, sp, #72	; 0x48
816001a4:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
816001a8:	e51f2170 	ldr	r2, [pc, #-368]	; 81600040 <IRQ_STACK_START_IN>
816001ac:	e892000c 	ldm	r2, {r2, r3}
816001b0:	e28d0048 	add	r0, sp, #72	; 0x48
816001b4:	e28d5034 	add	r5, sp, #52	; 0x34
816001b8:	e1a0100e 	mov	r1, lr
816001bc:	e885000f 	stm	r5, {r0, r1, r2, r3}
816001c0:	e1a0000d 	mov	r0, sp
816001c4:	eb0003bf 	bl	816010c8 <do_data_abort>
816001c8:	e320f000 	nop	{0}
816001cc:	e320f000 	nop	{0}
816001d0:	e320f000 	nop	{0}
816001d4:	e320f000 	nop	{0}
816001d8:	e320f000 	nop	{0}
816001dc:	e320f000 	nop	{0}

816001e0 <not_used>:
816001e0:	e51fd1a8 	ldr	sp, [pc, #-424]	; 81600040 <IRQ_STACK_START_IN>
816001e4:	e58de000 	str	lr, [sp]
816001e8:	e14fe000 	mrs	lr, SPSR
816001ec:	e58de004 	str	lr, [sp, #4]
816001f0:	e3a0d013 	mov	sp, #19
816001f4:	e169f00d 	msr	SPSR_fc, sp
816001f8:	e1a0e00f 	mov	lr, pc
816001fc:	e1b0f00e 	movs	pc, lr
81600200:	e24dd048 	sub	sp, sp, #72	; 0x48
81600204:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
81600208:	e51f21d0 	ldr	r2, [pc, #-464]	; 81600040 <IRQ_STACK_START_IN>
8160020c:	e892000c 	ldm	r2, {r2, r3}
81600210:	e28d0048 	add	r0, sp, #72	; 0x48
81600214:	e28d5034 	add	r5, sp, #52	; 0x34
81600218:	e1a0100e 	mov	r1, lr
8160021c:	e885000f 	stm	r5, {r0, r1, r2, r3}
81600220:	e1a0000d 	mov	r0, sp
81600224:	eb0003b2 	bl	816010f4 <do_not_used>
81600228:	e320f000 	nop	{0}
8160022c:	e320f000 	nop	{0}
81600230:	e320f000 	nop	{0}
81600234:	e320f000 	nop	{0}
81600238:	e320f000 	nop	{0}
8160023c:	e320f000 	nop	{0}

81600240 <irq>:
81600240:	e51fd208 	ldr	sp, [pc, #-520]	; 81600040 <IRQ_STACK_START_IN>
81600244:	e58de000 	str	lr, [sp]
81600248:	e14fe000 	mrs	lr, SPSR
8160024c:	e58de004 	str	lr, [sp, #4]
81600250:	e3a0d013 	mov	sp, #19
81600254:	e169f00d 	msr	SPSR_fc, sp
81600258:	e1a0e00f 	mov	lr, pc
8160025c:	e1b0f00e 	movs	pc, lr
81600260:	e24dd048 	sub	sp, sp, #72	; 0x48
81600264:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
81600268:	e51f2230 	ldr	r2, [pc, #-560]	; 81600040 <IRQ_STACK_START_IN>
8160026c:	e892000c 	ldm	r2, {r2, r3}
81600270:	e28d0048 	add	r0, sp, #72	; 0x48
81600274:	e28d5034 	add	r5, sp, #52	; 0x34
81600278:	e1a0100e 	mov	r1, lr
8160027c:	e885000f 	stm	r5, {r0, r1, r2, r3}
81600280:	e1a0000d 	mov	r0, sp
81600284:	eb0003b0 	bl	8160114c <do_irq>
81600288:	e320f000 	nop	{0}
8160028c:	e320f000 	nop	{0}
81600290:	e320f000 	nop	{0}
81600294:	e320f000 	nop	{0}
81600298:	e320f000 	nop	{0}
8160029c:	e320f000 	nop	{0}

816002a0 <fiq>:
816002a0:	e51fd268 	ldr	sp, [pc, #-616]	; 81600040 <IRQ_STACK_START_IN>
816002a4:	e58de000 	str	lr, [sp]
816002a8:	e14fe000 	mrs	lr, SPSR
816002ac:	e58de004 	str	lr, [sp, #4]
816002b0:	e3a0d013 	mov	sp, #19
816002b4:	e169f00d 	msr	SPSR_fc, sp
816002b8:	e1a0e00f 	mov	lr, pc
816002bc:	e1b0f00e 	movs	pc, lr
816002c0:	e24dd048 	sub	sp, sp, #72	; 0x48
816002c4:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
816002c8:	e51f2290 	ldr	r2, [pc, #-656]	; 81600040 <IRQ_STACK_START_IN>
816002cc:	e892000c 	ldm	r2, {r2, r3}
816002d0:	e28d0048 	add	r0, sp, #72	; 0x48
816002d4:	e28d5034 	add	r5, sp, #52	; 0x34
816002d8:	e1a0100e 	mov	r1, lr
816002dc:	e885000f 	stm	r5, {r0, r1, r2, r3}
816002e0:	e1a0000d 	mov	r0, sp
816002e4:	eb00038d 	bl	81601120 <do_fiq>

816002e8 <reset>:
816002e8:	ea000016 	b	81600348 <save_boot_params>

816002ec <save_boot_params_ret>:
816002ec:	ee100f31 	mrc	15, 0, r0, cr0, cr1, {1}
816002f0:	e2000a0f 	and	r0, r0, #61440	; 0xf000
816002f4:	e3500a01 	cmp	r0, #4096	; 0x1000
816002f8:	0a000013 	beq	8160034c <switch_to_hypervisor>

816002fc <switch_to_hypervisor_ret>:
816002fc:	e10f0000 	mrs	r0, CPSR
81600300:	e200101f 	and	r1, r0, #31
81600304:	e331001a 	teq	r1, #26
81600308:	13c0001f 	bicne	r0, r0, #31
8160030c:	13800013 	orrne	r0, r0, #19
81600310:	e38000c0 	orr	r0, r0, #192	; 0xc0
81600314:	e129f000 	msr	CPSR_fc, r0
81600318:	ee110f10 	mrc	15, 0, r0, cr1, cr0, {0}
8160031c:	e3c00a02 	bic	r0, r0, #8192	; 0x2000
81600320:	ee010f10 	mcr	15, 0, r0, cr1, cr0, {0}
81600324:	e59f007c 	ldr	r0, [pc, #124]	; 816003a8 <cpu_init_crit+0x4>
81600328:	ee0c0f10 	mcr	15, 0, r0, cr12, cr0, {0}
8160032c:	eb000007 	bl	81600350 <cpu_init_cp15>
81600330:	eb00001b 	bl	816003a4 <cpu_init_crit>
81600334:	eb0000b9 	bl	81600620 <_main>

81600338 <c_runtime_cpu_setup>:
81600338:	ee070f15 	mcr	15, 0, r0, cr7, cr5, {0}
8160033c:	ee070f9a 	mcr	15, 0, r0, cr7, cr10, {4}
81600340:	ee070f95 	mcr	15, 0, r0, cr7, cr5, {4}
81600344:	e12fff1e 	bx	lr

81600348 <save_boot_params>:
81600348:	eaffffe7 	b	816002ec <save_boot_params_ret>

8160034c <switch_to_hypervisor>:
8160034c:	eaffffea 	b	816002fc <switch_to_hypervisor_ret>

81600350 <cpu_init_cp15>:
81600350:	e3a00000 	mov	r0, #0
81600354:	ee080f17 	mcr	15, 0, r0, cr8, cr7, {0}
81600358:	ee070f15 	mcr	15, 0, r0, cr7, cr5, {0}
8160035c:	ee070fd5 	mcr	15, 0, r0, cr7, cr5, {6}
81600360:	ee070f9a 	mcr	15, 0, r0, cr7, cr10, {4}
81600364:	ee070f95 	mcr	15, 0, r0, cr7, cr5, {4}
81600368:	ee110f10 	mrc	15, 0, r0, cr1, cr0, {0}
8160036c:	e3c00a02 	bic	r0, r0, #8192	; 0x2000
81600370:	e3c00007 	bic	r0, r0, #7
81600374:	e3800002 	orr	r0, r0, #2
81600378:	e3800b02 	orr	r0, r0, #2048	; 0x800
8160037c:	e3800a01 	orr	r0, r0, #4096	; 0x1000
81600380:	ee010f10 	mcr	15, 0, r0, cr1, cr0, {0}
81600384:	e1a0500e 	mov	r5, lr
81600388:	ee101f10 	mrc	15, 0, r1, cr0, cr0, {0}
8160038c:	e1a03a21 	lsr	r3, r1, #20
81600390:	e203300f 	and	r3, r3, #15
81600394:	e201400f 	and	r4, r1, #15
81600398:	e1a02203 	lsl	r2, r3, #4
8160039c:	e1842002 	orr	r2, r4, r2
816003a0:	e1a0f005 	mov	pc, r5

816003a4 <cpu_init_crit>:
816003a4:	ea000465 	b	81601540 <lowlevel_init>
816003a8:	81600000 	.word	0x81600000

816003ac <__v7_flush_dcache_all>:
816003ac:	f57ff05f 	dmb	sy
816003b0:	ee300f30 	mrc	15, 1, r0, cr0, cr0, {1}
816003b4:	e1a03ba0 	lsr	r3, r0, #23
816003b8:	e213300e 	ands	r3, r3, #14
816003bc:	0a00001a 	beq	8160042c <finished>

816003c0 <start_flush_levels>:
816003c0:	e3a0a000 	mov	sl, #0

816003c4 <flush_levels>:
816003c4:	e08a20aa 	add	r2, sl, sl, lsr #1
816003c8:	e1a01230 	lsr	r1, r0, r2
816003cc:	e2011007 	and	r1, r1, #7
816003d0:	e3510002 	cmp	r1, #2
816003d4:	ba000011 	blt	81600420 <skip>
816003d8:	ee40af10 	mcr	15, 2, sl, cr0, cr0, {0}
816003dc:	f57ff06f 	isb	sy
816003e0:	ee301f10 	mrc	15, 1, r1, cr0, cr0, {0}
816003e4:	e2012007 	and	r2, r1, #7
816003e8:	e2822004 	add	r2, r2, #4
816003ec:	e30043ff 	movw	r4, #1023	; 0x3ff
816003f0:	e01441a1 	ands	r4, r4, r1, lsr #3
816003f4:	e16f5f14 	clz	r5, r4
816003f8:	e3077fff 	movw	r7, #32767	; 0x7fff
816003fc:	e01776a1 	ands	r7, r7, r1, lsr #13

81600400 <loop1>:
81600400:	e1a09007 	mov	r9, r7

81600404 <loop2>:
81600404:	e18ab514 	orr	fp, sl, r4, lsl r5
81600408:	e18bb219 	orr	fp, fp, r9, lsl r2
8160040c:	ee07bf5e 	mcr	15, 0, fp, cr7, cr14, {2}
81600410:	e2599001 	subs	r9, r9, #1
81600414:	aafffffa 	bge	81600404 <loop2>
81600418:	e2544001 	subs	r4, r4, #1
8160041c:	aafffff7 	bge	81600400 <loop1>

81600420 <skip>:
81600420:	e28aa002 	add	sl, sl, #2
81600424:	e153000a 	cmp	r3, sl
81600428:	caffffe5 	bgt	816003c4 <flush_levels>

8160042c <finished>:
8160042c:	e3a0a000 	mov	sl, #0
81600430:	ee40af10 	mcr	15, 2, sl, cr0, cr0, {0}
81600434:	f57ff04e 	dsb	st
81600438:	f57ff06f 	isb	sy
8160043c:	e12fff1e 	bx	lr

81600440 <v7_flush_dcache_all>:
81600440:	e92d4eb0 	push	{r4, r5, r7, r9, sl, fp, lr}
81600444:	ebffffd8 	bl	816003ac <__v7_flush_dcache_all>
81600448:	e8bd4eb0 	pop	{r4, r5, r7, r9, sl, fp, lr}
8160044c:	e12fff1e 	bx	lr

81600450 <__v7_invalidate_dcache_all>:
81600450:	f57ff05f 	dmb	sy
81600454:	ee300f30 	mrc	15, 1, r0, cr0, cr0, {1}
81600458:	e1a03ba0 	lsr	r3, r0, #23
8160045c:	e213300e 	ands	r3, r3, #14
81600460:	0a00001a 	beq	816004d0 <inval_finished>
81600464:	e3a0a000 	mov	sl, #0

81600468 <inval_levels>:
81600468:	e08a20aa 	add	r2, sl, sl, lsr #1
8160046c:	e1a01230 	lsr	r1, r0, r2
81600470:	e2011007 	and	r1, r1, #7
81600474:	e3510002 	cmp	r1, #2
81600478:	ba000011 	blt	816004c4 <inval_skip>
8160047c:	ee40af10 	mcr	15, 2, sl, cr0, cr0, {0}
81600480:	f57ff06f 	isb	sy
81600484:	ee301f10 	mrc	15, 1, r1, cr0, cr0, {0}
81600488:	e2012007 	and	r2, r1, #7
8160048c:	e2822004 	add	r2, r2, #4
81600490:	e30043ff 	movw	r4, #1023	; 0x3ff
81600494:	e01441a1 	ands	r4, r4, r1, lsr #3
81600498:	e16f5f14 	clz	r5, r4
8160049c:	e3077fff 	movw	r7, #32767	; 0x7fff
816004a0:	e01776a1 	ands	r7, r7, r1, lsr #13

816004a4 <inval_loop1>:
816004a4:	e1a09007 	mov	r9, r7

816004a8 <inval_loop2>:
816004a8:	e18ab514 	orr	fp, sl, r4, lsl r5
816004ac:	e18bb219 	orr	fp, fp, r9, lsl r2
816004b0:	ee07bf56 	mcr	15, 0, fp, cr7, cr6, {2}
816004b4:	e2599001 	subs	r9, r9, #1
816004b8:	aafffffa 	bge	816004a8 <inval_loop2>
816004bc:	e2544001 	subs	r4, r4, #1
816004c0:	aafffff7 	bge	816004a4 <inval_loop1>

816004c4 <inval_skip>:
816004c4:	e28aa002 	add	sl, sl, #2
816004c8:	e153000a 	cmp	r3, sl
816004cc:	caffffe5 	bgt	81600468 <inval_levels>

816004d0 <inval_finished>:
816004d0:	e3a0a000 	mov	sl, #0
816004d4:	ee40af10 	mcr	15, 2, sl, cr0, cr0, {0}
816004d8:	f57ff04e 	dsb	st
816004dc:	f57ff06f 	isb	sy
816004e0:	e12fff1e 	bx	lr

816004e4 <v7_invalidate_dcache_all>:
816004e4:	e92d4eb0 	push	{r4, r5, r7, r9, sl, fp, lr}
816004e8:	ebffffd8 	bl	81600450 <__v7_invalidate_dcache_all>
816004ec:	e8bd4eb0 	pop	{r4, r5, r7, r9, sl, fp, lr}
816004f0:	e12fff1e 	bx	lr

816004f4 <invalidate_icache_all>:
816004f4:	e3a03000 	mov	r3, #0
816004f8:	ee073f15 	mcr	15, 0, r3, cr7, cr5, {0}
816004fc:	ee073fd5 	mcr	15, 0, r3, cr7, cr5, {6}
81600500:	f57ff04f 	dsb	sy
81600504:	f57ff06f 	isb	sy
81600508:	e12fff1e 	bx	lr

8160050c <v7_outer_cache_enable>:
8160050c:	e12fff1e 	bx	lr

81600510 <v7_outer_cache_disable>:
81600510:	e12fff1e 	bx	lr

81600514 <v7_outer_cache_flush_all>:
81600514:	e12fff1e 	bx	lr

81600518 <flush_dcache_all>:
81600518:	e92d4010 	push	{r4, lr}
8160051c:	ebffffc7 	bl	81600440 <v7_flush_dcache_all>
81600520:	e8bd4010 	pop	{r4, lr}
81600524:	eafffffa 	b	81600514 <v7_outer_cache_flush_all>

81600528 <v7_outer_cache_inval_all>:
81600528:	e12fff1e 	bx	lr

8160052c <invalidate_dcache_all>:
8160052c:	e92d4010 	push	{r4, lr}
81600530:	ebffffeb 	bl	816004e4 <v7_invalidate_dcache_all>
81600534:	e8bd4010 	pop	{r4, lr}
81600538:	eafffffa 	b	81600528 <v7_outer_cache_inval_all>

8160053c <arm_init_before_mmu>:
8160053c:	e92d4010 	push	{r4, lr}
81600540:	ebfffff1 	bl	8160050c <v7_outer_cache_enable>
81600544:	ebfffff8 	bl	8160052c <invalidate_dcache_all>
81600548:	e3a03000 	mov	r3, #0
8160054c:	ee083f17 	mcr	15, 0, r3, cr8, cr7, {0}
81600550:	ee083f16 	mcr	15, 0, r3, cr8, cr6, {0}
81600554:	ee083f15 	mcr	15, 0, r3, cr8, cr5, {0}
81600558:	f57ff04f 	dsb	sy
8160055c:	f57ff06f 	isb	sy
81600560:	e8bd8010 	pop	{r4, pc}

81600564 <v7_outer_cache_flush_range>:
81600564:	e12fff1e 	bx	lr

81600568 <flush_dcache_range>:
81600568:	e92d4070 	push	{r4, r5, r6, lr}
8160056c:	e1a04000 	mov	r4, r0
81600570:	e1a05001 	mov	r5, r1
81600574:	eb00030e 	bl	816011b4 <check_cache_range>
81600578:	ee303f10 	mrc	15, 1, r3, cr0, cr0, {0}
8160057c:	e2033007 	and	r3, r3, #7
81600580:	e3a02001 	mov	r2, #1
81600584:	e2833004 	add	r3, r3, #4
81600588:	e1a02312 	lsl	r2, r2, r3
8160058c:	e2623000 	rsb	r3, r2, #0
81600590:	e0033004 	and	r3, r3, r4
81600594:	e1550003 	cmp	r5, r3
81600598:	8a000004 	bhi	816005b0 <flush_dcache_range+0x48>
8160059c:	f57ff04f 	dsb	sy
816005a0:	e1a01005 	mov	r1, r5
816005a4:	e1a00004 	mov	r0, r4
816005a8:	e8bd4070 	pop	{r4, r5, r6, lr}
816005ac:	eaffffec 	b	81600564 <v7_outer_cache_flush_range>
816005b0:	ee073f3e 	mcr	15, 0, r3, cr7, cr14, {1}
816005b4:	e0833002 	add	r3, r3, r2
816005b8:	eafffff5 	b	81600594 <flush_dcache_range+0x2c>

816005bc <cpu_cache_initialization>:
816005bc:	e12fff1e 	bx	lr

816005c0 <cleanup_before_linux_select>:
816005c0:	e92d4010 	push	{r4, lr}
816005c4:	e1a04000 	mov	r4, r0
816005c8:	eb00023a 	bl	81600eb8 <disable_interrupts>
816005cc:	e3140001 	tst	r4, #1
816005d0:	0a000007 	beq	816005f4 <cleanup_before_linux_select+0x34>
816005d4:	eb0003a0 	bl	8160145c <dcache_disable>
816005d8:	ebffffcc 	bl	81600510 <v7_outer_cache_disable>
816005dc:	ebffffd2 	bl	8160052c <invalidate_dcache_all>
816005e0:	eb000399 	bl	8160144c <icache_disable>
816005e4:	ebffffc2 	bl	816004f4 <invalidate_icache_all>
816005e8:	ebfffff3 	bl	816005bc <cpu_cache_initialization>
816005ec:	e3a00000 	mov	r0, #0
816005f0:	e8bd8010 	pop	{r4, pc}
816005f4:	eb000394 	bl	8160144c <icache_disable>
816005f8:	ebffffbd 	bl	816004f4 <invalidate_icache_all>
816005fc:	ebffffc5 	bl	81600518 <flush_dcache_all>
81600600:	ebffffbb 	bl	816004f4 <invalidate_icache_all>
81600604:	eb00038e 	bl	81601444 <icache_enable>
81600608:	eafffff6 	b	816005e8 <cleanup_before_linux_select+0x28>

8160060c <cleanup_before_linux>:
8160060c:	e3a00003 	mov	r0, #3
81600610:	eaffffea 	b	816005c0 <cleanup_before_linux_select>
	...

81600620 <_main>:
81600620:	e59f0074 	ldr	r0, [pc, #116]	; 8160069c <here+0x30>
81600624:	e3c00007 	bic	r0, r0, #7
81600628:	e1a0d000 	mov	sp, r0
8160062c:	eb001012 	bl	8160467c <board_init_f_alloc_reserve>
81600630:	e1a0d000 	mov	sp, r0
81600634:	e1a09000 	mov	r9, r0
81600638:	eb001012 	bl	81604688 <board_init_f_init_reserve>
8160063c:	e3a00000 	mov	r0, #0
81600640:	eb0011b3 	bl	81604d14 <board_init_f>
81600644:	e599003c 	ldr	r0, [r9, #60]	; 0x3c
81600648:	e3c00007 	bic	r0, r0, #7
8160064c:	e1a0d000 	mov	sp, r0
81600650:	e5999000 	ldr	r9, [r9]
81600654:	e24990a8 	sub	r9, r9, #168	; 0xa8
81600658:	e28fe00c 	add	lr, pc, #12
8160065c:	e5990040 	ldr	r0, [r9, #64]	; 0x40
81600660:	e08ee000 	add	lr, lr, r0
81600664:	e599002c 	ldr	r0, [r9, #44]	; 0x2c
81600668:	ea000139 	b	81600b54 <relocate_code>

8160066c <here>:
8160066c:	eb000135 	bl	81600b48 <relocate_vectors>
81600670:	ebffff30 	bl	81600338 <c_runtime_cpu_setup>
81600674:	e59f0024 	ldr	r0, [pc, #36]	; 816006a0 <here+0x34>
81600678:	e59f3024 	ldr	r3, [pc, #36]	; 816006a4 <here+0x38>
8160067c:	e3a01000 	mov	r1, #0
81600680:	e0532000 	subs	r2, r3, r0
81600684:	eb00000d 	bl	816006c0 <memset>
81600688:	eb001175 	bl	81604c64 <coloured_LED_init>
8160068c:	eb001175 	bl	81604c68 <red_led_on>
81600690:	e1a00009 	mov	r0, r9
81600694:	e599102c 	ldr	r1, [r9, #44]	; 0x2c
81600698:	e59ff008 	ldr	pc, [pc, #8]	; 816006a8 <here+0x3c>
8160069c:	80000f50 	.word	0x80000f50
816006a0:	81619460 	.word	0x81619460
816006a4:	81619ea0 	.word	0x81619ea0
816006a8:	81604e8c 	.word	0x81604e8c
	...

816006c0 <memset>:
816006c0:	e2103003 	ands	r3, r0, #3
816006c4:	e1a0c000 	mov	ip, r0
816006c8:	1a00002b 	bne	8160077c <memset+0xbc>
816006cc:	e1811401 	orr	r1, r1, r1, lsl #8
816006d0:	e1811801 	orr	r1, r1, r1, lsl #16
816006d4:	e1a03001 	mov	r3, r1
816006d8:	e3520010 	cmp	r2, #16
816006dc:	ba00001c 	blt	81600754 <memset+0x94>
816006e0:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816006e4:	e1a04001 	mov	r4, r1
816006e8:	e1a05001 	mov	r5, r1
816006ec:	e1a06001 	mov	r6, r1
816006f0:	e1a07001 	mov	r7, r1
816006f4:	e1a08001 	mov	r8, r1
816006f8:	e1a0e001 	mov	lr, r1
816006fc:	e3520060 	cmp	r2, #96	; 0x60
81600700:	c31c001f 	tstgt	ip, #31
81600704:	da000008 	ble	8160072c <memset+0x6c>
81600708:	e20c801f 	and	r8, ip, #31
8160070c:	e2688020 	rsb	r8, r8, #32
81600710:	e0422008 	sub	r2, r2, r8
81600714:	e1b08e08 	lsls	r8, r8, #28
81600718:	28ac00f0 	stmiacs	ip!, {r4, r5, r6, r7}
8160071c:	48ac0030 	stmiami	ip!, {r4, r5}
81600720:	e3180101 	tst	r8, #1073741824	; 0x40000000
81600724:	e1a08001 	mov	r8, r1
81600728:	148c1004 	strne	r1, [ip], #4
8160072c:	e2522040 	subs	r2, r2, #64	; 0x40
81600730:	a8ac41fa 	stmiage	ip!, {r1, r3, r4, r5, r6, r7, r8, lr}
81600734:	a8ac41fa 	stmiage	ip!, {r1, r3, r4, r5, r6, r7, r8, lr}
81600738:	cafffffb 	bgt	8160072c <memset+0x6c>
8160073c:	08bd81f0 	popeq	{r4, r5, r6, r7, r8, pc}
81600740:	e3120020 	tst	r2, #32
81600744:	18ac41fa 	stmiane	ip!, {r1, r3, r4, r5, r6, r7, r8, lr}
81600748:	e3120010 	tst	r2, #16
8160074c:	18ac00f0 	stmiane	ip!, {r4, r5, r6, r7}
81600750:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
81600754:	e3120008 	tst	r2, #8
81600758:	18ac000a 	stmiane	ip!, {r1, r3}
8160075c:	e3120004 	tst	r2, #4
81600760:	148c1004 	strne	r1, [ip], #4
81600764:	e3120002 	tst	r2, #2
81600768:	14cc1001 	strbne	r1, [ip], #1
8160076c:	14cc1001 	strbne	r1, [ip], #1
81600770:	e3120001 	tst	r2, #1
81600774:	14cc1001 	strbne	r1, [ip], #1
81600778:	e12fff1e 	bx	lr
8160077c:	e2522004 	subs	r2, r2, #4
81600780:	bafffff7 	blt	81600764 <memset+0xa4>
81600784:	e3530002 	cmp	r3, #2
81600788:	b4cc1001 	strblt	r1, [ip], #1
8160078c:	d4cc1001 	strble	r1, [ip], #1
81600790:	e4cc1001 	strb	r1, [ip], #1
81600794:	e0822003 	add	r2, r2, r3
81600798:	eaffffcb 	b	816006cc <memset+0xc>

8160079c <memcpy>:
8160079c:	e1500001 	cmp	r0, r1
816007a0:	01a0f00e 	moveq	pc, lr
816007a4:	e92d4011 	push	{r0, r4, lr}
816007a8:	e2522004 	subs	r2, r2, #4
816007ac:	ba000033 	blt	81600880 <memcpy+0xe4>
816007b0:	e210c003 	ands	ip, r0, #3
816007b4:	f5d1f000 	pld	[r1]
816007b8:	1a000038 	bne	816008a0 <memcpy+0x104>
816007bc:	e211c003 	ands	ip, r1, #3
816007c0:	1a000042 	bne	816008d0 <memcpy+0x134>
816007c4:	e252201c 	subs	r2, r2, #28
816007c8:	e92d01e0 	push	{r5, r6, r7, r8}
816007cc:	ba000013 	blt	81600820 <memcpy+0x84>
816007d0:	e210c01f 	ands	ip, r0, #31
816007d4:	e26c3020 	rsb	r3, ip, #32
816007d8:	10d34002 	sbcsne	r4, r3, r2
816007dc:	2a000003 	bcs	816007f0 <memcpy+0x54>
816007e0:	e28f4048 	add	r4, pc, #72	; 0x48
816007e4:	e0522003 	subs	r2, r2, r3
816007e8:	e084f00c 	add	pc, r4, ip
816007ec:	f5d1f000 	pld	[r1]
816007f0:	e2522060 	subs	r2, r2, #96	; 0x60
816007f4:	f5d1f01c 	pld	[r1, #28]
816007f8:	ba000002 	blt	81600808 <memcpy+0x6c>
816007fc:	f5d1f03c 	pld	[r1, #60]	; 0x3c
81600800:	f5d1f05c 	pld	[r1, #92]	; 0x5c
81600804:	f5d1f07c 	pld	[r1, #124]	; 0x7c
81600808:	e8b151f8 	ldm	r1!, {r3, r4, r5, r6, r7, r8, ip, lr}
8160080c:	e2522020 	subs	r2, r2, #32
81600810:	e8a051f8 	stmia	r0!, {r3, r4, r5, r6, r7, r8, ip, lr}
81600814:	aafffffa 	bge	81600804 <memcpy+0x68>
81600818:	e3720060 	cmn	r2, #96	; 0x60
8160081c:	aafffff9 	bge	81600808 <memcpy+0x6c>
81600820:	e212c01c 	ands	ip, r2, #28
81600824:	e26cc020 	rsb	ip, ip, #32
81600828:	108ff00c 	addne	pc, pc, ip
8160082c:	ea000012 	b	8160087c <memcpy+0xe0>
81600830:	e320f000 	nop	{0}
81600834:	e4913004 	ldr	r3, [r1], #4
81600838:	e4914004 	ldr	r4, [r1], #4
8160083c:	e4915004 	ldr	r5, [r1], #4
81600840:	e4916004 	ldr	r6, [r1], #4
81600844:	e4917004 	ldr	r7, [r1], #4
81600848:	e4918004 	ldr	r8, [r1], #4
8160084c:	e491e004 	ldr	lr, [r1], #4
81600850:	e08ff00c 	add	pc, pc, ip
81600854:	e320f000 	nop	{0}
81600858:	e320f000 	nop	{0}
8160085c:	e4803004 	str	r3, [r0], #4
81600860:	e4804004 	str	r4, [r0], #4
81600864:	e4805004 	str	r5, [r0], #4
81600868:	e4806004 	str	r6, [r0], #4
8160086c:	e4807004 	str	r7, [r0], #4
81600870:	e4808004 	str	r8, [r0], #4
81600874:	e480e004 	str	lr, [r0], #4
81600878:	2affffdc 	bcs	816007f0 <memcpy+0x54>
8160087c:	e8bd01e0 	pop	{r5, r6, r7, r8}
81600880:	e1b02f82 	lsls	r2, r2, #31
81600884:	14d13001 	ldrbne	r3, [r1], #1
81600888:	24d14001 	ldrbcs	r4, [r1], #1
8160088c:	24d1c001 	ldrbcs	ip, [r1], #1
81600890:	14c03001 	strbne	r3, [r0], #1
81600894:	24c04001 	strbcs	r4, [r0], #1
81600898:	24c0c001 	strbcs	ip, [r0], #1
8160089c:	e8bd8011 	pop	{r0, r4, pc}
816008a0:	e26cc004 	rsb	ip, ip, #4
816008a4:	e35c0002 	cmp	ip, #2
816008a8:	c4d13001 	ldrbgt	r3, [r1], #1
816008ac:	a4d14001 	ldrbge	r4, [r1], #1
816008b0:	e4d1e001 	ldrb	lr, [r1], #1
816008b4:	c4c03001 	strbgt	r3, [r0], #1
816008b8:	a4c04001 	strbge	r4, [r0], #1
816008bc:	e052200c 	subs	r2, r2, ip
816008c0:	e4c0e001 	strb	lr, [r0], #1
816008c4:	baffffed 	blt	81600880 <memcpy+0xe4>
816008c8:	e211c003 	ands	ip, r1, #3
816008cc:	0affffbc 	beq	816007c4 <memcpy+0x28>
816008d0:	e3c11003 	bic	r1, r1, #3
816008d4:	e35c0002 	cmp	ip, #2
816008d8:	e491e004 	ldr	lr, [r1], #4
816008dc:	0a000033 	beq	816009b0 <memcpy+0x214>
816008e0:	ca000065 	bgt	81600a7c <memcpy+0x2e0>
816008e4:	e252201c 	subs	r2, r2, #28
816008e8:	ba000024 	blt	81600980 <memcpy+0x1e4>
816008ec:	e210c01f 	ands	ip, r0, #31
816008f0:	e26cc020 	rsb	ip, ip, #32
816008f4:	10dc4002 	sbcsne	r4, ip, r2
816008f8:	3042200c 	subcc	r2, r2, ip
816008fc:	3a000021 	bcc	81600988 <memcpy+0x1ec>
81600900:	e92d03e0 	push	{r5, r6, r7, r8, r9}
81600904:	f5d1f000 	pld	[r1]
81600908:	e2522060 	subs	r2, r2, #96	; 0x60
8160090c:	f5d1f01c 	pld	[r1, #28]
81600910:	ba000002 	blt	81600920 <memcpy+0x184>
81600914:	f5d1f03c 	pld	[r1, #60]	; 0x3c
81600918:	f5d1f05c 	pld	[r1, #92]	; 0x5c
8160091c:	f5d1f07c 	pld	[r1, #124]	; 0x7c
81600920:	e8b100f0 	ldm	r1!, {r4, r5, r6, r7}
81600924:	e1a0342e 	lsr	r3, lr, #8
81600928:	e2522020 	subs	r2, r2, #32
8160092c:	e8b15300 	ldm	r1!, {r8, r9, ip, lr}
81600930:	e1833c04 	orr	r3, r3, r4, lsl #24
81600934:	e1a04424 	lsr	r4, r4, #8
81600938:	e1844c05 	orr	r4, r4, r5, lsl #24
8160093c:	e1a05425 	lsr	r5, r5, #8
81600940:	e1855c06 	orr	r5, r5, r6, lsl #24
81600944:	e1a06426 	lsr	r6, r6, #8
81600948:	e1866c07 	orr	r6, r6, r7, lsl #24
8160094c:	e1a07427 	lsr	r7, r7, #8
81600950:	e1877c08 	orr	r7, r7, r8, lsl #24
81600954:	e1a08428 	lsr	r8, r8, #8
81600958:	e1888c09 	orr	r8, r8, r9, lsl #24
8160095c:	e1a09429 	lsr	r9, r9, #8
81600960:	e1899c0c 	orr	r9, r9, ip, lsl #24
81600964:	e1a0c42c 	lsr	ip, ip, #8
81600968:	e18ccc0e 	orr	ip, ip, lr, lsl #24
8160096c:	e8a013f8 	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
81600970:	aaffffe9 	bge	8160091c <memcpy+0x180>
81600974:	e3720060 	cmn	r2, #96	; 0x60
81600978:	aaffffe8 	bge	81600920 <memcpy+0x184>
8160097c:	e8bd03e0 	pop	{r5, r6, r7, r8, r9}
81600980:	e212c01c 	ands	ip, r2, #28
81600984:	0a000007 	beq	816009a8 <memcpy+0x20c>
81600988:	e1a0342e 	lsr	r3, lr, #8
8160098c:	e491e004 	ldr	lr, [r1], #4
81600990:	e25cc004 	subs	ip, ip, #4
81600994:	e1833c0e 	orr	r3, r3, lr, lsl #24
81600998:	e4803004 	str	r3, [r0], #4
8160099c:	cafffff9 	bgt	81600988 <memcpy+0x1ec>
816009a0:	e3520000 	cmp	r2, #0
816009a4:	aaffffd5 	bge	81600900 <memcpy+0x164>
816009a8:	e2411003 	sub	r1, r1, #3
816009ac:	eaffffb3 	b	81600880 <memcpy+0xe4>
816009b0:	e252201c 	subs	r2, r2, #28
816009b4:	ba000024 	blt	81600a4c <memcpy+0x2b0>
816009b8:	e210c01f 	ands	ip, r0, #31
816009bc:	e26cc020 	rsb	ip, ip, #32
816009c0:	10dc4002 	sbcsne	r4, ip, r2
816009c4:	3042200c 	subcc	r2, r2, ip
816009c8:	3a000021 	bcc	81600a54 <memcpy+0x2b8>
816009cc:	e92d03e0 	push	{r5, r6, r7, r8, r9}
816009d0:	f5d1f000 	pld	[r1]
816009d4:	e2522060 	subs	r2, r2, #96	; 0x60
816009d8:	f5d1f01c 	pld	[r1, #28]
816009dc:	ba000002 	blt	816009ec <memcpy+0x250>
816009e0:	f5d1f03c 	pld	[r1, #60]	; 0x3c
816009e4:	f5d1f05c 	pld	[r1, #92]	; 0x5c
816009e8:	f5d1f07c 	pld	[r1, #124]	; 0x7c
816009ec:	e8b100f0 	ldm	r1!, {r4, r5, r6, r7}
816009f0:	e1a0382e 	lsr	r3, lr, #16
816009f4:	e2522020 	subs	r2, r2, #32
816009f8:	e8b15300 	ldm	r1!, {r8, r9, ip, lr}
816009fc:	e1833804 	orr	r3, r3, r4, lsl #16
81600a00:	e1a04824 	lsr	r4, r4, #16
81600a04:	e1844805 	orr	r4, r4, r5, lsl #16
81600a08:	e1a05825 	lsr	r5, r5, #16
81600a0c:	e1855806 	orr	r5, r5, r6, lsl #16
81600a10:	e1a06826 	lsr	r6, r6, #16
81600a14:	e1866807 	orr	r6, r6, r7, lsl #16
81600a18:	e1a07827 	lsr	r7, r7, #16
81600a1c:	e1877808 	orr	r7, r7, r8, lsl #16
81600a20:	e1a08828 	lsr	r8, r8, #16
81600a24:	e1888809 	orr	r8, r8, r9, lsl #16
81600a28:	e1a09829 	lsr	r9, r9, #16
81600a2c:	e189980c 	orr	r9, r9, ip, lsl #16
81600a30:	e1a0c82c 	lsr	ip, ip, #16
81600a34:	e18cc80e 	orr	ip, ip, lr, lsl #16
81600a38:	e8a013f8 	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
81600a3c:	aaffffe9 	bge	816009e8 <memcpy+0x24c>
81600a40:	e3720060 	cmn	r2, #96	; 0x60
81600a44:	aaffffe8 	bge	816009ec <memcpy+0x250>
81600a48:	e8bd03e0 	pop	{r5, r6, r7, r8, r9}
81600a4c:	e212c01c 	ands	ip, r2, #28
81600a50:	0a000007 	beq	81600a74 <memcpy+0x2d8>
81600a54:	e1a0382e 	lsr	r3, lr, #16
81600a58:	e491e004 	ldr	lr, [r1], #4
81600a5c:	e25cc004 	subs	ip, ip, #4
81600a60:	e183380e 	orr	r3, r3, lr, lsl #16
81600a64:	e4803004 	str	r3, [r0], #4
81600a68:	cafffff9 	bgt	81600a54 <memcpy+0x2b8>
81600a6c:	e3520000 	cmp	r2, #0
81600a70:	aaffffd5 	bge	816009cc <memcpy+0x230>
81600a74:	e2411002 	sub	r1, r1, #2
81600a78:	eaffff80 	b	81600880 <memcpy+0xe4>
81600a7c:	e252201c 	subs	r2, r2, #28
81600a80:	ba000024 	blt	81600b18 <memcpy+0x37c>
81600a84:	e210c01f 	ands	ip, r0, #31
81600a88:	e26cc020 	rsb	ip, ip, #32
81600a8c:	10dc4002 	sbcsne	r4, ip, r2
81600a90:	3042200c 	subcc	r2, r2, ip
81600a94:	3a000021 	bcc	81600b20 <memcpy+0x384>
81600a98:	e92d03e0 	push	{r5, r6, r7, r8, r9}
81600a9c:	f5d1f000 	pld	[r1]
81600aa0:	e2522060 	subs	r2, r2, #96	; 0x60
81600aa4:	f5d1f01c 	pld	[r1, #28]
81600aa8:	ba000002 	blt	81600ab8 <memcpy+0x31c>
81600aac:	f5d1f03c 	pld	[r1, #60]	; 0x3c
81600ab0:	f5d1f05c 	pld	[r1, #92]	; 0x5c
81600ab4:	f5d1f07c 	pld	[r1, #124]	; 0x7c
81600ab8:	e8b100f0 	ldm	r1!, {r4, r5, r6, r7}
81600abc:	e1a03c2e 	lsr	r3, lr, #24
81600ac0:	e2522020 	subs	r2, r2, #32
81600ac4:	e8b15300 	ldm	r1!, {r8, r9, ip, lr}
81600ac8:	e1833404 	orr	r3, r3, r4, lsl #8
81600acc:	e1a04c24 	lsr	r4, r4, #24
81600ad0:	e1844405 	orr	r4, r4, r5, lsl #8
81600ad4:	e1a05c25 	lsr	r5, r5, #24
81600ad8:	e1855406 	orr	r5, r5, r6, lsl #8
81600adc:	e1a06c26 	lsr	r6, r6, #24
81600ae0:	e1866407 	orr	r6, r6, r7, lsl #8
81600ae4:	e1a07c27 	lsr	r7, r7, #24
81600ae8:	e1877408 	orr	r7, r7, r8, lsl #8
81600aec:	e1a08c28 	lsr	r8, r8, #24
81600af0:	e1888409 	orr	r8, r8, r9, lsl #8
81600af4:	e1a09c29 	lsr	r9, r9, #24
81600af8:	e189940c 	orr	r9, r9, ip, lsl #8
81600afc:	e1a0cc2c 	lsr	ip, ip, #24
81600b00:	e18cc40e 	orr	ip, ip, lr, lsl #8
81600b04:	e8a013f8 	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
81600b08:	aaffffe9 	bge	81600ab4 <memcpy+0x318>
81600b0c:	e3720060 	cmn	r2, #96	; 0x60
81600b10:	aaffffe8 	bge	81600ab8 <memcpy+0x31c>
81600b14:	e8bd03e0 	pop	{r5, r6, r7, r8, r9}
81600b18:	e212c01c 	ands	ip, r2, #28
81600b1c:	0a000007 	beq	81600b40 <memcpy+0x3a4>
81600b20:	e1a03c2e 	lsr	r3, lr, #24
81600b24:	e491e004 	ldr	lr, [r1], #4
81600b28:	e25cc004 	subs	ip, ip, #4
81600b2c:	e183340e 	orr	r3, r3, lr, lsl #8
81600b30:	e4803004 	str	r3, [r0], #4
81600b34:	cafffff9 	bgt	81600b20 <memcpy+0x384>
81600b38:	e3520000 	cmp	r2, #0
81600b3c:	aaffffd5 	bge	81600a98 <memcpy+0x2fc>
81600b40:	e2411001 	sub	r1, r1, #1
81600b44:	eaffff4d 	b	81600880 <memcpy+0xe4>

81600b48 <relocate_vectors>:
81600b48:	e599002c 	ldr	r0, [r9, #44]	; 0x2c
81600b4c:	ee0c0f10 	mcr	15, 0, r0, cr12, cr0, {0}
81600b50:	e12fff1e 	bx	lr

81600b54 <relocate_code>:
81600b54:	e59f104c 	ldr	r1, [pc, #76]	; 81600ba8 <relocate_done+0x4>
81600b58:	e0504001 	subs	r4, r0, r1
81600b5c:	0a000010 	beq	81600ba4 <relocate_done>
81600b60:	e59f2044 	ldr	r2, [pc, #68]	; 81600bac <relocate_done+0x8>

81600b64 <copy_loop>:
81600b64:	e8b10c00 	ldm	r1!, {sl, fp}
81600b68:	e8a00c00 	stmia	r0!, {sl, fp}
81600b6c:	e1510002 	cmp	r1, r2
81600b70:	3afffffb 	bcc	81600b64 <copy_loop>
81600b74:	e59f2034 	ldr	r2, [pc, #52]	; 81600bb0 <relocate_done+0xc>
81600b78:	e59f3034 	ldr	r3, [pc, #52]	; 81600bb4 <relocate_done+0x10>

81600b7c <fixloop>:
81600b7c:	e8b20003 	ldm	r2!, {r0, r1}
81600b80:	e20110ff 	and	r1, r1, #255	; 0xff
81600b84:	e3510017 	cmp	r1, #23
81600b88:	1a000003 	bne	81600b9c <fixnext>
81600b8c:	e0800004 	add	r0, r0, r4
81600b90:	e5901000 	ldr	r1, [r0]
81600b94:	e0811004 	add	r1, r1, r4
81600b98:	e5801000 	str	r1, [r0]

81600b9c <fixnext>:
81600b9c:	e1520003 	cmp	r2, r3
81600ba0:	3afffff5 	bcc	81600b7c <fixloop>

81600ba4 <relocate_done>:
81600ba4:	e12fff1e 	bx	lr
81600ba8:	81600000 	.word	0x81600000
81600bac:	81619460 	.word	0x81619460
81600bb0:	81619460 	.word	0x81619460
81600bb4:	8161c468 	.word	0x8161c468

81600bb8 <arch_fixup_fdt>:
81600bb8:	e92d401f 	push	{r0, r1, r2, r3, r4, lr}
81600bbc:	e3a03000 	mov	r3, #0
81600bc0:	e5992000 	ldr	r2, [r9]
81600bc4:	e5921048 	ldr	r1, [r2, #72]	; 0x48
81600bc8:	e592204c 	ldr	r2, [r2, #76]	; 0x4c
81600bcc:	e88d000a 	stm	sp, {r1, r3}
81600bd0:	e1a0100d 	mov	r1, sp
81600bd4:	e1cd20f8 	strd	r2, [sp, #8]
81600bd8:	e3a03001 	mov	r3, #1
81600bdc:	e28d2008 	add	r2, sp, #8
81600be0:	eb001526 	bl	81606080 <fdt_fixup_memory_banks>
81600be4:	e28dd014 	add	sp, sp, #20
81600be8:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

81600bec <boot_prep_linux>:
81600bec:	e92d4010 	push	{r4, lr}
81600bf0:	e1a04000 	mov	r4, r0
81600bf4:	e59f0030 	ldr	r0, [pc, #48]	; 81600c2c <boot_prep_linux+0x40>
81600bf8:	eb000e66 	bl	81604598 <env_get>
81600bfc:	e5943070 	ldr	r3, [r4, #112]	; 0x70
81600c00:	e3530000 	cmp	r3, #0
81600c04:	0a000006 	beq	81600c24 <boot_prep_linux+0x38>
81600c08:	e1a00004 	mov	r0, r4
81600c0c:	eb001dfe 	bl	8160840c <image_setup_linux>
81600c10:	e3500000 	cmp	r0, #0
81600c14:	159f0014 	ldrne	r0, [pc, #20]	; 81600c30 <boot_prep_linux+0x44>
81600c18:	08bd8010 	popeq	{r4, pc}
81600c1c:	eb004696 	bl	8161267c <printf>
81600c20:	eb0042cf 	bl	81611764 <hang>
81600c24:	e59f0008 	ldr	r0, [pc, #8]	; 81600c34 <boot_prep_linux+0x48>
81600c28:	eafffffb 	b	81600c1c <boot_prep_linux+0x30>
81600c2c:	81614ff6 	.word	0x81614ff6
81600c30:	816145d6 	.word	0x816145d6
81600c34:	816145f6 	.word	0x816145f6

81600c38 <arch_lmb_reserve>:
81600c38:	e5992000 	ldr	r2, [r9]
81600c3c:	e5993000 	ldr	r3, [r9]
81600c40:	e1a0100d 	mov	r1, sp
81600c44:	e5922048 	ldr	r2, [r2, #72]	; 0x48
81600c48:	e2411a01 	sub	r1, r1, #4096	; 0x1000
81600c4c:	e593304c 	ldr	r3, [r3, #76]	; 0x4c
81600c50:	e0822003 	add	r2, r2, r3
81600c54:	e0422001 	sub	r2, r2, r1
81600c58:	ea003787 	b	8160ea7c <lmb_reserve>

81600c5c <board_quiesce_devices>:
81600c5c:	e12fff1e 	bx	lr

81600c60 <boot_jump_linux>:
81600c60:	e5993000 	ldr	r3, [r9]
81600c64:	e92d4073 	push	{r0, r1, r4, r5, r6, lr}
81600c68:	e1a05000 	mov	r5, r0
81600c6c:	e5933040 	ldr	r3, [r3, #64]	; 0x40
81600c70:	e1a04001 	mov	r4, r1
81600c74:	e5906060 	ldr	r6, [r0, #96]	; 0x60
81600c78:	e59f0094 	ldr	r0, [pc, #148]	; 81600d14 <boot_jump_linux+0xb4>
81600c7c:	e58d3004 	str	r3, [sp, #4]
81600c80:	eb000e44 	bl	81604598 <env_get>
81600c84:	e3500000 	cmp	r0, #0
81600c88:	0a000007 	beq	81600cac <boot_jump_linux+0x4c>
81600c8c:	e28d2004 	add	r2, sp, #4
81600c90:	e3a01010 	mov	r1, #16
81600c94:	eb0046eb 	bl	81612848 <strict_strtoul>
81600c98:	e3500000 	cmp	r0, #0
81600c9c:	ba00001a 	blt	81600d0c <boot_jump_linux+0xac>
81600ca0:	e59d1004 	ldr	r1, [sp, #4]
81600ca4:	e59f006c 	ldr	r0, [pc, #108]	; 81600d18 <boot_jump_linux+0xb8>
81600ca8:	eb004673 	bl	8161267c <printf>
81600cac:	e3a0000f 	mov	r0, #15
81600cb0:	e2044c02 	and	r4, r4, #512	; 0x200
81600cb4:	eb000e76 	bl	81604694 <show_boot_progress>
81600cb8:	e59f305c 	ldr	r3, [pc, #92]	; 81600d1c <boot_jump_linux+0xbc>
81600cbc:	e3540000 	cmp	r4, #0
81600cc0:	e59f1058 	ldr	r1, [pc, #88]	; 81600d20 <boot_jump_linux+0xc0>
81600cc4:	e59f0058 	ldr	r0, [pc, #88]	; 81600d24 <boot_jump_linux+0xc4>
81600cc8:	01a01003 	moveq	r1, r3
81600ccc:	eb00466a 	bl	8161267c <printf>
81600cd0:	e3a000ab 	mov	r0, #171	; 0xab
81600cd4:	eb000e6e 	bl	81604694 <show_boot_progress>
81600cd8:	ebffffdf 	bl	81600c5c <board_quiesce_devices>
81600cdc:	ebfffe4a 	bl	8160060c <cleanup_before_linux>
81600ce0:	e5953070 	ldr	r3, [r5, #112]	; 0x70
81600ce4:	e3530000 	cmp	r3, #0
81600ce8:	1595206c 	ldrne	r2, [r5, #108]	; 0x6c
81600cec:	01a03009 	moveq	r3, r9
81600cf0:	05933000 	ldreq	r3, [r3]
81600cf4:	05932044 	ldreq	r2, [r3, #68]	; 0x44
81600cf8:	e3540000 	cmp	r4, #0
81600cfc:	1a000002 	bne	81600d0c <boot_jump_linux+0xac>
81600d00:	e59d1004 	ldr	r1, [sp, #4]
81600d04:	e1a00004 	mov	r0, r4
81600d08:	e12fff36 	blx	r6
81600d0c:	e28dd008 	add	sp, sp, #8
81600d10:	e8bd8070 	pop	{r4, r5, r6, pc}
81600d14:	81614591 	.word	0x81614591
81600d18:	81614598 	.word	0x81614598
81600d1c:	8161623b 	.word	0x8161623b
81600d20:	8161457a 	.word	0x8161457a
81600d24:	816145bd 	.word	0x816145bd

81600d28 <do_bootm_linux>:
81600d28:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81600d2c:	e21070c0 	ands	r7, r0, #192	; 0xc0
81600d30:	13e06000 	mvnne	r6, #0
81600d34:	1a000006 	bne	81600d54 <do_bootm_linux+0x2c>
81600d38:	e2106c01 	ands	r6, r0, #256	; 0x100
81600d3c:	e1a05003 	mov	r5, r3
81600d40:	e1a04000 	mov	r4, r0
81600d44:	0a000004 	beq	81600d5c <do_bootm_linux+0x34>
81600d48:	e1a00003 	mov	r0, r3
81600d4c:	e1a06007 	mov	r6, r7
81600d50:	ebffffa5 	bl	81600bec <boot_prep_linux>
81600d54:	e1a00006 	mov	r0, r6
81600d58:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81600d5c:	e3100c06 	tst	r0, #1536	; 0x600
81600d60:	0a000003 	beq	81600d74 <do_bootm_linux+0x4c>
81600d64:	e1a01004 	mov	r1, r4
81600d68:	e1a00005 	mov	r0, r5
81600d6c:	ebffffbb 	bl	81600c60 <boot_jump_linux>
81600d70:	eafffff7 	b	81600d54 <do_bootm_linux+0x2c>
81600d74:	e1a00003 	mov	r0, r3
81600d78:	ebffff9b 	bl	81600bec <boot_prep_linux>
81600d7c:	eafffff8 	b	81600d64 <do_bootm_linux+0x3c>

81600d80 <boot_prep_vxworks>:
81600d80:	e590306c 	ldr	r3, [r0, #108]	; 0x6c
81600d84:	e3530000 	cmp	r3, #0
81600d88:	0a00000d 	beq	81600dc4 <boot_prep_vxworks+0x44>
81600d8c:	e92d4010 	push	{r4, lr}
81600d90:	e1a04000 	mov	r4, r0
81600d94:	e59f102c 	ldr	r1, [pc, #44]	; 81600dc8 <boot_prep_vxworks+0x48>
81600d98:	e1a00003 	mov	r0, r3
81600d9c:	eb003a66 	bl	8160f73c <fdt_path_offset>
81600da0:	e3500000 	cmp	r0, #0
81600da4:	aa000005 	bge	81600dc0 <boot_prep_vxworks+0x40>
81600da8:	e594006c 	ldr	r0, [r4, #108]	; 0x6c
81600dac:	ebffff81 	bl	81600bb8 <arch_fixup_fdt>
81600db0:	e3500000 	cmp	r0, #0
81600db4:	0a000001 	beq	81600dc0 <boot_prep_vxworks+0x40>
81600db8:	e59f000c 	ldr	r0, [pc, #12]	; 81600dcc <boot_prep_vxworks+0x4c>
81600dbc:	eb0017f4 	bl	81606d94 <puts>
81600dc0:	e8bd4010 	pop	{r4, lr}
81600dc4:	eafffe10 	b	8160060c <cleanup_before_linux>
81600dc8:	81614627 	.word	0x81614627
81600dcc:	8161462f 	.word	0x8161462f

81600dd0 <boot_jump_vxworks>:
81600dd0:	e5903060 	ldr	r3, [r0, #96]	; 0x60
81600dd4:	e590006c 	ldr	r0, [r0, #108]	; 0x6c
81600dd8:	e12fff13 	bx	r3

81600ddc <bootz_setup>:
81600ddc:	e92d4010 	push	{r4, lr}
81600de0:	e5d0c025 	ldrb	ip, [r0, #37]	; 0x25
81600de4:	e5d03024 	ldrb	r3, [r0, #36]	; 0x24
81600de8:	e183340c 	orr	r3, r3, ip, lsl #8
81600dec:	e5d0c026 	ldrb	ip, [r0, #38]	; 0x26
81600df0:	e183380c 	orr	r3, r3, ip, lsl #16
81600df4:	e5d0c027 	ldrb	ip, [r0, #39]	; 0x27
81600df8:	e1833c0c 	orr	r3, r3, ip, lsl #24
81600dfc:	e59fc06c 	ldr	ip, [pc, #108]	; 81600e70 <bootz_setup+0x94>
81600e00:	e153000c 	cmp	r3, ip
81600e04:	0a000003 	beq	81600e18 <bootz_setup+0x3c>
81600e08:	e59f0064 	ldr	r0, [pc, #100]	; 81600e74 <bootz_setup+0x98>
81600e0c:	eb0017e0 	bl	81606d94 <puts>
81600e10:	e3a00001 	mov	r0, #1
81600e14:	e8bd8010 	pop	{r4, pc}
81600e18:	e5d0c029 	ldrb	ip, [r0, #41]	; 0x29
81600e1c:	e5d03028 	ldrb	r3, [r0, #40]	; 0x28
81600e20:	e183340c 	orr	r3, r3, ip, lsl #8
81600e24:	e5d0c02a 	ldrb	ip, [r0, #42]	; 0x2a
81600e28:	e183380c 	orr	r3, r3, ip, lsl #16
81600e2c:	e5d0c02b 	ldrb	ip, [r0, #43]	; 0x2b
81600e30:	e1833c0c 	orr	r3, r3, ip, lsl #24
81600e34:	e5813000 	str	r3, [r1]
81600e38:	e5d0c02d 	ldrb	ip, [r0, #45]	; 0x2d
81600e3c:	e5d0302c 	ldrb	r3, [r0, #44]	; 0x2c
81600e40:	e183340c 	orr	r3, r3, ip, lsl #8
81600e44:	e5d0c02e 	ldrb	ip, [r0, #46]	; 0x2e
81600e48:	e183380c 	orr	r3, r3, ip, lsl #16
81600e4c:	e5d0c02f 	ldrb	ip, [r0, #47]	; 0x2f
81600e50:	e1833c0c 	orr	r3, r3, ip, lsl #24
81600e54:	e5823000 	str	r3, [r2]
81600e58:	e5912000 	ldr	r2, [r1]
81600e5c:	e1a01000 	mov	r1, r0
81600e60:	e59f0010 	ldr	r0, [pc, #16]	; 81600e78 <bootz_setup+0x9c>
81600e64:	eb004604 	bl	8161267c <printf>
81600e68:	e3a00000 	mov	r0, #0
81600e6c:	e8bd8010 	pop	{r4, pc}
81600e70:	016f2818 	.word	0x016f2818
81600e74:	8161488b 	.word	0x8161488b
81600e78:	816148a8 	.word	0x816148a8

81600e7c <arch_reserve_stacks>:
81600e7c:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81600e80:	e3a00000 	mov	r0, #0
81600e84:	e5892038 	str	r2, [r9, #56]	; 0x38
81600e88:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81600e8c:	e2422010 	sub	r2, r2, #16
81600e90:	e589203c 	str	r2, [r9, #60]	; 0x3c
81600e94:	e12fff1e 	bx	lr

81600e98 <interrupt_init>:
81600e98:	e59f2010 	ldr	r2, [pc, #16]	; 81600eb0 <interrupt_init+0x18>
81600e9c:	e3a00000 	mov	r0, #0
81600ea0:	e5993038 	ldr	r3, [r9, #56]	; 0x38
81600ea4:	e2833008 	add	r3, r3, #8
81600ea8:	e5823000 	str	r3, [r2]
81600eac:	e12fff1e 	bx	lr
81600eb0:	81600040 	.word	0x81600040

81600eb4 <enable_interrupts>:
81600eb4:	e12fff1e 	bx	lr

81600eb8 <disable_interrupts>:
81600eb8:	e3a00000 	mov	r0, #0
81600ebc:	e12fff1e 	bx	lr

81600ec0 <bad_mode>:
81600ec0:	e92d4010 	push	{r4, lr}
81600ec4:	e59f0000 	ldr	r0, [pc]	; 81600ecc <bad_mode+0xc>
81600ec8:	eb00462b 	bl	8161277c <panic>
81600ecc:	816142c4 	.word	0x816142c4

81600ed0 <show_regs>:
81600ed0:	e92d4070 	push	{r4, r5, r6, lr}
81600ed4:	e24dd088 	sub	sp, sp, #136	; 0x88
81600ed8:	e1a04000 	mov	r4, r0
81600edc:	e3a02080 	mov	r2, #128	; 0x80
81600ee0:	e59f112c 	ldr	r1, [pc, #300]	; 81601014 <show_regs+0x144>
81600ee4:	e28d0008 	add	r0, sp, #8
81600ee8:	ebfffe2b 	bl	8160079c <memcpy>
81600eec:	e5942038 	ldr	r2, [r4, #56]	; 0x38
81600ef0:	e594103c 	ldr	r1, [r4, #60]	; 0x3c
81600ef4:	e59f011c 	ldr	r0, [pc, #284]	; 81601018 <show_regs+0x148>
81600ef8:	e5945040 	ldr	r5, [r4, #64]	; 0x40
81600efc:	eb0045de 	bl	8161267c <printf>
81600f00:	e205620f 	and	r6, r5, #-268435456	; 0xf0000000
81600f04:	e5992004 	ldr	r2, [r9, #4]
81600f08:	e3120001 	tst	r2, #1
81600f0c:	0a000007 	beq	81600f30 <show_regs+0x60>
81600f10:	e5991040 	ldr	r1, [r9, #64]	; 0x40
81600f14:	e5940038 	ldr	r0, [r4, #56]	; 0x38
81600f18:	e5992040 	ldr	r2, [r9, #64]	; 0x40
81600f1c:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
81600f20:	e0402002 	sub	r2, r0, r2
81600f24:	e59f00f0 	ldr	r0, [pc, #240]	; 8160101c <show_regs+0x14c>
81600f28:	e0431001 	sub	r1, r3, r1
81600f2c:	eb0045d2 	bl	8161267c <printf>
81600f30:	e594302c 	ldr	r3, [r4, #44]	; 0x2c
81600f34:	e5942030 	ldr	r2, [r4, #48]	; 0x30
81600f38:	e5941034 	ldr	r1, [r4, #52]	; 0x34
81600f3c:	e59f00dc 	ldr	r0, [pc, #220]	; 81601020 <show_regs+0x150>
81600f40:	eb0045cd 	bl	8161267c <printf>
81600f44:	e5943020 	ldr	r3, [r4, #32]
81600f48:	e5942024 	ldr	r2, [r4, #36]	; 0x24
81600f4c:	e5941028 	ldr	r1, [r4, #40]	; 0x28
81600f50:	e59f00cc 	ldr	r0, [pc, #204]	; 81601024 <show_regs+0x154>
81600f54:	eb0045c8 	bl	8161267c <printf>
81600f58:	e5943010 	ldr	r3, [r4, #16]
81600f5c:	e5942018 	ldr	r2, [r4, #24]
81600f60:	e594101c 	ldr	r1, [r4, #28]
81600f64:	e58d3000 	str	r3, [sp]
81600f68:	e59f00b8 	ldr	r0, [pc, #184]	; 81601028 <show_regs+0x158>
81600f6c:	e5943014 	ldr	r3, [r4, #20]
81600f70:	eb0045c1 	bl	8161267c <printf>
81600f74:	e5943000 	ldr	r3, [r4]
81600f78:	e594100c 	ldr	r1, [r4, #12]
81600f7c:	e5942008 	ldr	r2, [r4, #8]
81600f80:	e58d3000 	str	r3, [sp]
81600f84:	e59f00a0 	ldr	r0, [pc, #160]	; 8160102c <show_regs+0x15c>
81600f88:	e5943004 	ldr	r3, [r4, #4]
81600f8c:	eb0045ba 	bl	8161267c <printf>
81600f90:	e3560000 	cmp	r6, #0
81600f94:	b3a0104e 	movlt	r1, #78	; 0x4e
81600f98:	a3a0106e 	movge	r1, #110	; 0x6e
81600f9c:	e3150101 	tst	r5, #1073741824	; 0x40000000
81600fa0:	13a0205a 	movne	r2, #90	; 0x5a
81600fa4:	03a0207a 	moveq	r2, #122	; 0x7a
81600fa8:	e3150202 	tst	r5, #536870912	; 0x20000000
81600fac:	13a03043 	movne	r3, #67	; 0x43
81600fb0:	03a03063 	moveq	r3, #99	; 0x63
81600fb4:	e3150201 	tst	r5, #268435456	; 0x10000000
81600fb8:	13a00056 	movne	r0, #86	; 0x56
81600fbc:	03a00076 	moveq	r0, #118	; 0x76
81600fc0:	e58d0000 	str	r0, [sp]
81600fc4:	e59f0064 	ldr	r0, [pc, #100]	; 81601030 <show_regs+0x160>
81600fc8:	eb0045ab 	bl	8161267c <printf>
81600fcc:	e5943040 	ldr	r3, [r4, #64]	; 0x40
81600fd0:	e59f205c 	ldr	r2, [pc, #92]	; 81601034 <show_regs+0x164>
81600fd4:	e28dc088 	add	ip, sp, #136	; 0x88
81600fd8:	e59f0058 	ldr	r0, [pc, #88]	; 81601038 <show_regs+0x168>
81600fdc:	e3130080 	tst	r3, #128	; 0x80
81600fe0:	11a01002 	movne	r1, r2
81600fe4:	01a01000 	moveq	r1, r0
81600fe8:	e3130040 	tst	r3, #64	; 0x40
81600fec:	e203301f 	and	r3, r3, #31
81600ff0:	01a02000 	moveq	r2, r0
81600ff4:	e59f0040 	ldr	r0, [pc, #64]	; 8160103c <show_regs+0x16c>
81600ff8:	e08c3103 	add	r3, ip, r3, lsl #2
81600ffc:	e59fc03c 	ldr	ip, [pc, #60]	; 81601040 <show_regs+0x170>
81601000:	e58dc000 	str	ip, [sp]
81601004:	e5133080 	ldr	r3, [r3, #-128]	; 0xffffff80
81601008:	eb00459b 	bl	8161267c <printf>
8160100c:	e28dd088 	add	sp, sp, #136	; 0x88
81601010:	e8bd8070 	pop	{r4, r5, r6, pc}
81601014:	81612ccc 	.word	0x81612ccc
81601018:	8161676c 	.word	0x8161676c
8160101c:	81616766 	.word	0x81616766
81601020:	8161678e 	.word	0x8161678e
81601024:	816167b2 	.word	0x816167b2
81601028:	816167d6 	.word	0x816167d6
8160102c:	81616806 	.word	0x81616806
81601030:	81616836 	.word	0x81616836
81601034:	81614b83 	.word	0x81614b83
81601038:	81617a49 	.word	0x81617a49
8160103c:	81616846 	.word	0x81616846
81601040:	8161623b 	.word	0x8161623b

81601044 <do_undefined_instruction>:
81601044:	e92d4010 	push	{r4, lr}
81601048:	e1a04000 	mov	r4, r0
8160104c:	e59f0018 	ldr	r0, [pc, #24]	; 8160106c <do_undefined_instruction+0x28>
81601050:	eb004589 	bl	8161267c <printf>
81601054:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
81601058:	e1a00004 	mov	r0, r4
8160105c:	e2433004 	sub	r3, r3, #4
81601060:	e584303c 	str	r3, [r4, #60]	; 0x3c
81601064:	ebffff99 	bl	81600ed0 <show_regs>
81601068:	ebffff94 	bl	81600ec0 <bad_mode>
8160106c:	81615900 	.word	0x81615900

81601070 <do_software_interrupt>:
81601070:	e92d4010 	push	{r4, lr}
81601074:	e1a04000 	mov	r4, r0
81601078:	e59f0018 	ldr	r0, [pc, #24]	; 81601098 <do_software_interrupt+0x28>
8160107c:	eb00457e 	bl	8161267c <printf>
81601080:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
81601084:	e1a00004 	mov	r0, r4
81601088:	e2433004 	sub	r3, r3, #4
8160108c:	e584303c 	str	r3, [r4, #60]	; 0x3c
81601090:	ebffff8e 	bl	81600ed0 <show_regs>
81601094:	ebffff89 	bl	81600ec0 <bad_mode>
81601098:	816158ce 	.word	0x816158ce

8160109c <do_prefetch_abort>:
8160109c:	e92d4010 	push	{r4, lr}
816010a0:	e1a04000 	mov	r4, r0
816010a4:	e59f0018 	ldr	r0, [pc, #24]	; 816010c4 <do_prefetch_abort+0x28>
816010a8:	eb004573 	bl	8161267c <printf>
816010ac:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
816010b0:	e1a00004 	mov	r0, r4
816010b4:	e2433008 	sub	r3, r3, #8
816010b8:	e584303c 	str	r3, [r4, #60]	; 0x3c
816010bc:	ebffff83 	bl	81600ed0 <show_regs>
816010c0:	ebffff7e 	bl	81600ec0 <bad_mode>
816010c4:	816158af 	.word	0x816158af

816010c8 <do_data_abort>:
816010c8:	e92d4010 	push	{r4, lr}
816010cc:	e1a04000 	mov	r4, r0
816010d0:	e59f0018 	ldr	r0, [pc, #24]	; 816010f0 <do_data_abort+0x28>
816010d4:	eb004568 	bl	8161267c <printf>
816010d8:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
816010dc:	e1a00004 	mov	r0, r4
816010e0:	e2433008 	sub	r3, r3, #8
816010e4:	e584303c 	str	r3, [r4, #60]	; 0x3c
816010e8:	ebffff78 	bl	81600ed0 <show_regs>
816010ec:	ebffff73 	bl	81600ec0 <bad_mode>
816010f0:	816150d8 	.word	0x816150d8

816010f4 <do_not_used>:
816010f4:	e92d4010 	push	{r4, lr}
816010f8:	e1a04000 	mov	r4, r0
816010fc:	e59f0018 	ldr	r0, [pc, #24]	; 8160111c <do_not_used+0x28>
81601100:	eb00455d 	bl	8161267c <printf>
81601104:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
81601108:	e1a00004 	mov	r0, r4
8160110c:	e2433008 	sub	r3, r3, #8
81601110:	e584303c 	str	r3, [r4, #60]	; 0x3c
81601114:	ebffff6d 	bl	81600ed0 <show_regs>
81601118:	ebffff68 	bl	81600ec0 <bad_mode>
8160111c:	816158a5 	.word	0x816158a5

81601120 <do_fiq>:
81601120:	e92d4010 	push	{r4, lr}
81601124:	e1a04000 	mov	r4, r0
81601128:	e59f0018 	ldr	r0, [pc, #24]	; 81601148 <do_fiq+0x28>
8160112c:	eb004552 	bl	8161267c <printf>
81601130:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
81601134:	e1a00004 	mov	r0, r4
81601138:	e2433008 	sub	r3, r3, #8
8160113c:	e584303c 	str	r3, [r4, #60]	; 0x3c
81601140:	ebffff62 	bl	81600ed0 <show_regs>
81601144:	ebffff5d 	bl	81600ec0 <bad_mode>
81601148:	81615779 	.word	0x81615779

8160114c <do_irq>:
8160114c:	e92d4010 	push	{r4, lr}
81601150:	e1a04000 	mov	r4, r0
81601154:	e59f0018 	ldr	r0, [pc, #24]	; 81601174 <do_irq+0x28>
81601158:	eb004547 	bl	8161267c <printf>
8160115c:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
81601160:	e1a00004 	mov	r0, r4
81601164:	e2433008 	sub	r3, r3, #8
81601168:	e584303c 	str	r3, [r4, #60]	; 0x3c
8160116c:	ebffff57 	bl	81600ed0 <show_regs>
81601170:	ebffff52 	bl	81600ec0 <bad_mode>
81601174:	8161577e 	.word	0x8161577e

81601178 <reset_misc>:
81601178:	e12fff1e 	bx	lr

8160117c <do_reset>:
8160117c:	e92d4010 	push	{r4, lr}
81601180:	e59f0020 	ldr	r0, [pc, #32]	; 816011a8 <do_reset+0x2c>
81601184:	eb001702 	bl	81606d94 <puts>
81601188:	e30c0350 	movw	r0, #50000	; 0xc350
8160118c:	eb004286 	bl	81611bac <udelay>
81601190:	ebffff48 	bl	81600eb8 <disable_interrupts>
81601194:	ebfffff7 	bl	81601178 <reset_misc>
81601198:	e3a00000 	mov	r0, #0
8160119c:	eb0000e6 	bl	8160153c <reset_cpu>
816011a0:	e3a00000 	mov	r0, #0
816011a4:	e8bd8010 	pop	{r4, pc}
816011a8:	816158bf 	.word	0x816158bf

816011ac <flush_cache>:
816011ac:	e0801001 	add	r1, r0, r1
816011b0:	eafffcec 	b	81600568 <flush_dcache_range>

816011b4 <check_cache_range>:
816011b4:	e1803001 	orr	r3, r0, r1
816011b8:	e313003f 	tst	r3, #63	; 0x3f
816011bc:	0a000006 	beq	816011dc <check_cache_range+0x28>
816011c0:	e1a02001 	mov	r2, r1
816011c4:	e1a01000 	mov	r1, r0
816011c8:	e92d4010 	push	{r4, lr}
816011cc:	e59f0010 	ldr	r0, [pc, #16]	; 816011e4 <check_cache_range+0x30>
816011d0:	eb004529 	bl	8161267c <printf>
816011d4:	e3a00000 	mov	r0, #0
816011d8:	e8bd8010 	pop	{r4, pc}
816011dc:	e3a00001 	mov	r0, #1
816011e0:	e12fff1e 	bx	lr
816011e4:	816148d3 	.word	0x816148d3

816011e8 <cache_disable>:
816011e8:	e92d4070 	push	{r4, r5, r6, lr}
816011ec:	e1a05000 	mov	r5, r0
816011f0:	e10f3000 	mrs	r3, CPSR
816011f4:	e203301f 	and	r3, r3, #31
816011f8:	e353001a 	cmp	r3, #26
816011fc:	1a000015 	bne	81601258 <cache_disable+0x70>
81601200:	ee913f10 	mrc	15, 4, r3, cr1, cr0, {0}
81601204:	e3550004 	cmp	r5, #4
81601208:	1a000002 	bne	81601218 <cache_disable+0x30>
8160120c:	e3130004 	tst	r3, #4
81601210:	08bd8070 	popeq	{r4, r5, r6, pc}
81601214:	e3a05005 	mov	r5, #5
81601218:	e10f3000 	mrs	r3, CPSR
8160121c:	e203301f 	and	r3, r3, #31
81601220:	e353001a 	cmp	r3, #26
81601224:	1a00000d 	bne	81601260 <cache_disable+0x78>
81601228:	ee914f10 	mrc	15, 4, r4, cr1, cr0, {0}
8160122c:	e3550005 	cmp	r5, #5
81601230:	1a000000 	bne	81601238 <cache_disable+0x50>
81601234:	ebfffcb7 	bl	81600518 <flush_dcache_all>
81601238:	e1c44005 	bic	r4, r4, r5
8160123c:	e10f3000 	mrs	r3, CPSR
81601240:	e203301f 	and	r3, r3, #31
81601244:	e353001a 	cmp	r3, #26
81601248:	1a000006 	bne	81601268 <cache_disable+0x80>
8160124c:	ee814f10 	mcr	15, 4, r4, cr1, cr0, {0}
81601250:	f57ff06f 	isb	sy
81601254:	e8bd8070 	pop	{r4, r5, r6, pc}
81601258:	ee113f10 	mrc	15, 0, r3, cr1, cr0, {0}
8160125c:	eaffffe8 	b	81601204 <cache_disable+0x1c>
81601260:	ee114f10 	mrc	15, 0, r4, cr1, cr0, {0}
81601264:	eafffff0 	b	8160122c <cache_disable+0x44>
81601268:	ee014f10 	mcr	15, 0, r4, cr1, cr0, {0}
8160126c:	eafffff7 	b	81601250 <cache_disable+0x68>

81601270 <arm_init_domains>:
81601270:	e12fff1e 	bx	lr

81601274 <set_section_dcache>:
81601274:	e59910a0 	ldr	r1, [r9, #160]	; 0xa0
81601278:	e1822a80 	orr	r2, r2, r0, lsl #21
8160127c:	e3822d11 	orr	r2, r2, #1088	; 0x440
81601280:	e081c180 	add	ip, r1, r0, lsl #3
81601284:	e7812180 	str	r2, [r1, r0, lsl #3]
81601288:	e58c3004 	str	r3, [ip, #4]
8160128c:	e12fff1e 	bx	lr

81601290 <dram_bank_mmu_setup>:
81601290:	e92d4070 	push	{r4, r5, r6, lr}
81601294:	e5995000 	ldr	r5, [r9]
81601298:	e0855180 	add	r5, r5, r0, lsl #3
8160129c:	e5954048 	ldr	r4, [r5, #72]	; 0x48
816012a0:	e1a04aa4 	lsr	r4, r4, #21
816012a4:	e595304c 	ldr	r3, [r5, #76]	; 0x4c
816012a8:	e5952048 	ldr	r2, [r5, #72]	; 0x48
816012ac:	e1a03aa3 	lsr	r3, r3, #21
816012b0:	e0833aa2 	add	r3, r3, r2, lsr #21
816012b4:	e1530004 	cmp	r3, r4
816012b8:	98bd8070 	popls	{r4, r5, r6, pc}
816012bc:	e1a00004 	mov	r0, r4
816012c0:	e3a02009 	mov	r2, #9
816012c4:	e3a03000 	mov	r3, #0
816012c8:	e2844001 	add	r4, r4, #1
816012cc:	ebffffe8 	bl	81601274 <set_section_dcache>
816012d0:	eafffff3 	b	816012a4 <dram_bank_mmu_setup+0x14>

816012d4 <cache_enable>:
816012d4:	e3500004 	cmp	r0, #4
816012d8:	e92d4070 	push	{r4, r5, r6, lr}
816012dc:	e1a05000 	mov	r5, r0
816012e0:	1a000038 	bne	816013c8 <cache_enable+0xf4>
816012e4:	e10f3000 	mrs	r3, CPSR
816012e8:	e203301f 	and	r3, r3, #31
816012ec:	e353001a 	cmp	r3, #26
816012f0:	1a000041 	bne	816013fc <cache_enable+0x128>
816012f4:	ee914f10 	mrc	15, 4, r4, cr1, cr0, {0}
816012f8:	e2144001 	ands	r4, r4, #1
816012fc:	1a000031 	bne	816013c8 <cache_enable+0xf4>
81601300:	ebfffc8d 	bl	8160053c <arm_init_before_mmu>
81601304:	e1a00004 	mov	r0, r4
81601308:	e3a02001 	mov	r2, #1
8160130c:	e3a03501 	mov	r3, #4194304	; 0x400000
81601310:	e2844001 	add	r4, r4, #1
81601314:	ebffffd6 	bl	81601274 <set_section_dcache>
81601318:	e3540b02 	cmp	r4, #2048	; 0x800
8160131c:	1afffff8 	bne	81601304 <cache_enable+0x30>
81601320:	e3a00000 	mov	r0, #0
81601324:	ebffffd9 	bl	81601290 <dram_bank_mmu_setup>
81601328:	e3a03000 	mov	r3, #0
8160132c:	e1a00003 	mov	r0, r3
81601330:	e1a0c009 	mov	ip, r9
81601334:	e59c20a0 	ldr	r2, [ip, #160]	; 0xa0
81601338:	e59c10a0 	ldr	r1, [ip, #160]	; 0xa0
8160133c:	e0822003 	add	r2, r2, r3
81601340:	e0811483 	add	r1, r1, r3, lsl #9
81601344:	e2833008 	add	r3, r3, #8
81601348:	e2822901 	add	r2, r2, #16384	; 0x4000
8160134c:	e3530020 	cmp	r3, #32
81601350:	e3811003 	orr	r1, r1, #3
81601354:	e5820004 	str	r0, [r2, #4]
81601358:	e5821000 	str	r1, [r2]
8160135c:	1afffff4 	bne	81601334 <cache_enable+0x60>
81601360:	e10f3000 	mrs	r3, CPSR
81601364:	e203301f 	and	r3, r3, #31
81601368:	e59f20cc 	ldr	r2, [pc, #204]	; 8160143c <cache_enable+0x168>
8160136c:	e353001a 	cmp	r3, #26
81601370:	e59f30c8 	ldr	r3, [pc, #200]	; 81601440 <cache_enable+0x16c>
81601374:	1a000022 	bne	81601404 <cache_enable+0x130>
81601378:	ee823f50 	mcr	15, 4, r3, cr2, cr0, {2}
8160137c:	e59930a0 	ldr	r3, [r9, #160]	; 0xa0
81601380:	e2833901 	add	r3, r3, #16384	; 0x4000
81601384:	ec403f42 	mcrr	15, 4, r3, r0, cr2
81601388:	ee8a2f12 	mcr	15, 4, r2, cr10, cr2, {0}
8160138c:	e3e03000 	mvn	r3, #0
81601390:	ee033f10 	mcr	15, 0, r3, cr3, cr0, {0}
81601394:	ebffffb5 	bl	81601270 <arm_init_domains>
81601398:	e10f3000 	mrs	r3, CPSR
8160139c:	e203301f 	and	r3, r3, #31
816013a0:	e353001a 	cmp	r3, #26
816013a4:	1a00001c 	bne	8160141c <cache_enable+0x148>
816013a8:	ee913f10 	mrc	15, 4, r3, cr1, cr0, {0}
816013ac:	e3833001 	orr	r3, r3, #1
816013b0:	e10f2000 	mrs	r2, CPSR
816013b4:	e202201f 	and	r2, r2, #31
816013b8:	e352001a 	cmp	r2, #26
816013bc:	1a000018 	bne	81601424 <cache_enable+0x150>
816013c0:	ee813f10 	mcr	15, 4, r3, cr1, cr0, {0}
816013c4:	f57ff06f 	isb	sy
816013c8:	e10f3000 	mrs	r3, CPSR
816013cc:	e203301f 	and	r3, r3, #31
816013d0:	e353001a 	cmp	r3, #26
816013d4:	1a000014 	bne	8160142c <cache_enable+0x158>
816013d8:	ee913f10 	mrc	15, 4, r3, cr1, cr0, {0}
816013dc:	e1855003 	orr	r5, r5, r3
816013e0:	e10f3000 	mrs	r3, CPSR
816013e4:	e203301f 	and	r3, r3, #31
816013e8:	e353001a 	cmp	r3, #26
816013ec:	1a000010 	bne	81601434 <cache_enable+0x160>
816013f0:	ee815f10 	mcr	15, 4, r5, cr1, cr0, {0}
816013f4:	f57ff06f 	isb	sy
816013f8:	e8bd8070 	pop	{r4, r5, r6, pc}
816013fc:	ee114f10 	mrc	15, 0, r4, cr1, cr0, {0}
81601400:	eaffffbc 	b	816012f8 <cache_enable+0x24>
81601404:	ee023f50 	mcr	15, 0, r3, cr2, cr0, {2}
81601408:	e59930a0 	ldr	r3, [r9, #160]	; 0xa0
8160140c:	e2833901 	add	r3, r3, #16384	; 0x4000
81601410:	ec403f02 	mcrr	15, 0, r3, r0, cr2
81601414:	ee0a2f12 	mcr	15, 0, r2, cr10, cr2, {0}
81601418:	eaffffdb 	b	8160138c <cache_enable+0xb8>
8160141c:	ee113f10 	mrc	15, 0, r3, cr1, cr0, {0}
81601420:	eaffffe1 	b	816013ac <cache_enable+0xd8>
81601424:	ee013f10 	mcr	15, 0, r3, cr1, cr0, {0}
81601428:	eaffffe5 	b	816013c4 <cache_enable+0xf0>
8160142c:	ee113f10 	mrc	15, 0, r3, cr1, cr0, {0}
81601430:	eaffffe9 	b	816013dc <cache_enable+0x108>
81601434:	ee015f10 	mcr	15, 0, r5, cr1, cr0, {0}
81601438:	eaffffed 	b	816013f4 <cache_enable+0x120>
8160143c:	ffcc8800 	.word	0xffcc8800
81601440:	80000f00 	.word	0x80000f00

81601444 <icache_enable>:
81601444:	e3a00a01 	mov	r0, #4096	; 0x1000
81601448:	eaffffa1 	b	816012d4 <cache_enable>

8160144c <icache_disable>:
8160144c:	e3a00a01 	mov	r0, #4096	; 0x1000
81601450:	eaffff64 	b	816011e8 <cache_disable>

81601454 <dcache_enable>:
81601454:	e3a00004 	mov	r0, #4
81601458:	eaffff9d 	b	816012d4 <cache_enable>

8160145c <dcache_disable>:
8160145c:	e3a00004 	mov	r0, #4
81601460:	eaffff60 	b	816011e8 <cache_disable>

81601464 <dcache_status>:
81601464:	e10f3000 	mrs	r3, CPSR
81601468:	e203301f 	and	r3, r3, #31
8160146c:	e353001a 	cmp	r3, #26
81601470:	1a000002 	bne	81601480 <dcache_status+0x1c>
81601474:	ee910f10 	mrc	15, 4, r0, cr1, cr0, {0}
81601478:	e7e00150 	ubfx	r0, r0, #2, #1
8160147c:	e12fff1e 	bx	lr
81601480:	ee110f10 	mrc	15, 0, r0, cr1, cr0, {0}
81601484:	eafffffb 	b	81601478 <dcache_status+0x14>

81601488 <arch_cpu_init>:
81601488:	e3a00000 	mov	r0, #0
8160148c:	e12fff1e 	bx	lr

81601490 <enable_caches>:
81601490:	e12fff1e 	bx	lr

81601494 <board_init>:
81601494:	e5992000 	ldr	r2, [r9]
81601498:	e59f1050 	ldr	r1, [pc, #80]	; 816014f0 <board_init+0x5c>
8160149c:	e92d4010 	push	{r4, lr}
816014a0:	e3a04000 	mov	r4, #0
816014a4:	e5821044 	str	r1, [r2, #68]	; 0x44
816014a8:	e3a01e8e 	mov	r1, #2272	; 0x8e0
816014ac:	e5992000 	ldr	r2, [r9]
816014b0:	e5821040 	str	r1, [r2, #64]	; 0x40
816014b4:	e5894004 	str	r4, [r9, #4]
816014b8:	ebffffe1 	bl	81601444 <icache_enable>
816014bc:	f57ff05f 	dmb	sy
816014c0:	e59f302c 	ldr	r3, [pc, #44]	; 816014f4 <board_init+0x60>
816014c4:	e3e02000 	mvn	r2, #0
816014c8:	e5832000 	str	r2, [r3]
816014cc:	f57ff05f 	dmb	sy
816014d0:	e5832004 	str	r2, [r3, #4]
816014d4:	e5932008 	ldr	r2, [r3, #8]
816014d8:	f57ff05f 	dmb	sy
816014dc:	e3822082 	orr	r2, r2, #130	; 0x82
816014e0:	f57ff05f 	dmb	sy
816014e4:	e5832008 	str	r2, [r3, #8]
816014e8:	e1a00004 	mov	r0, r4
816014ec:	e8bd8010 	pop	{r4, pc}
816014f0:	80002000 	.word	0x80002000
816014f4:	13110000 	.word	0x13110000

816014f8 <dram_init>:
816014f8:	e92d4010 	push	{r4, lr}
816014fc:	e3a01402 	mov	r1, #33554432	; 0x2000000
81601500:	e3a00102 	mov	r0, #-2147483648	; 0x80000000
81601504:	eb001d55 	bl	81608a60 <get_ram_size>
81601508:	e5890030 	str	r0, [r9, #48]	; 0x30
8160150c:	e3a00000 	mov	r0, #0
81601510:	e8bd8010 	pop	{r4, pc}

81601514 <dram_init_banksize>:
81601514:	e5993000 	ldr	r3, [r9]
81601518:	e3a00102 	mov	r0, #-2147483648	; 0x80000000
8160151c:	e92d4010 	push	{r4, lr}
81601520:	e3a01402 	mov	r1, #33554432	; 0x2000000
81601524:	e5830048 	str	r0, [r3, #72]	; 0x48
81601528:	eb001d4c 	bl	81608a60 <get_ram_size>
8160152c:	e5993000 	ldr	r3, [r9]
81601530:	e583004c 	str	r0, [r3, #76]	; 0x4c
81601534:	e3a00000 	mov	r0, #0
81601538:	e8bd8010 	pop	{r4, pc}

8160153c <reset_cpu>:
8160153c:	e12fff1e 	bx	lr

81601540 <lowlevel_init>:
81601540:	e12fff1e 	bx	lr

81601544 <do_go_exec>:
81601544:	e1a03000 	mov	r3, r0
81601548:	e1a00001 	mov	r0, r1
8160154c:	e1a01002 	mov	r1, r2
81601550:	e12fff13 	bx	r3

81601554 <do_go>:
81601554:	e3520001 	cmp	r2, #1
81601558:	e92d4070 	push	{r4, r5, r6, lr}
8160155c:	d3e04000 	mvnle	r4, #0
81601560:	da000012 	ble	816015b0 <do_go+0x5c>
81601564:	e1a04003 	mov	r4, r3
81601568:	e1a05002 	mov	r5, r2
8160156c:	e5b40004 	ldr	r0, [r4, #4]!
81601570:	e3a02010 	mov	r2, #16
81601574:	e3a01000 	mov	r1, #0
81601578:	eb004486 	bl	81612798 <simple_strtoul>
8160157c:	e1a01000 	mov	r1, r0
81601580:	e1a06000 	mov	r6, r0
81601584:	e59f002c 	ldr	r0, [pc, #44]	; 816015b8 <do_go+0x64>
81601588:	eb00443b 	bl	8161267c <printf>
8160158c:	e1a02004 	mov	r2, r4
81601590:	e2451001 	sub	r1, r5, #1
81601594:	e1a00006 	mov	r0, r6
81601598:	ebffffe9 	bl	81601544 <do_go_exec>
8160159c:	e2904000 	adds	r4, r0, #0
816015a0:	e1a01000 	mov	r1, r0
816015a4:	e59f0010 	ldr	r0, [pc, #16]	; 816015bc <do_go+0x68>
816015a8:	13a04001 	movne	r4, #1
816015ac:	eb004432 	bl	8161267c <printf>
816015b0:	e1a00004 	mov	r0, r4
816015b4:	e8bd8070 	pop	{r4, r5, r6, pc}
816015b8:	816157ba 	.word	0x816157ba
816015bc:	816157e2 	.word	0x816157e2

816015c0 <do_bootm>:
816015c0:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
816015c4:	e2428001 	sub	r8, r2, #1
816015c8:	e3580000 	cmp	r8, #0
816015cc:	e24dd01c 	sub	sp, sp, #28
816015d0:	e1a05000 	mov	r5, r0
816015d4:	e1a06001 	mov	r6, r1
816015d8:	e283a004 	add	sl, r3, #4
816015dc:	da00002d 	ble	81601698 <do_bootm+0xd8>
816015e0:	e28d1014 	add	r1, sp, #20
816015e4:	e5930004 	ldr	r0, [r3, #4]
816015e8:	e1a07002 	mov	r7, r2
816015ec:	e3a02010 	mov	r2, #16
816015f0:	e1a04003 	mov	r4, r3
816015f4:	eb004467 	bl	81612798 <simple_strtoul>
816015f8:	e59d3014 	ldr	r3, [sp, #20]
816015fc:	e5d33000 	ldrb	r3, [r3]
81601600:	e253103a 	subs	r1, r3, #58	; 0x3a
81601604:	13a01001 	movne	r1, #1
81601608:	e3530000 	cmp	r3, #0
8160160c:	03a01000 	moveq	r1, #0
81601610:	e3530023 	cmp	r3, #35	; 0x23
81601614:	03a01000 	moveq	r1, #0
81601618:	12011001 	andne	r1, r1, #1
8160161c:	e3510000 	cmp	r1, #0
81601620:	0a00001c 	beq	81601698 <do_bootm+0xd8>
81601624:	e3a02009 	mov	r2, #9
81601628:	e59f108c 	ldr	r1, [pc, #140]	; 816016bc <do_bootm+0xfc>
8160162c:	e5940004 	ldr	r0, [r4, #4]
81601630:	eb001df2 	bl	81608e00 <find_cmd_tbl>
81601634:	e2472002 	sub	r2, r7, #2
81601638:	e2843008 	add	r3, r4, #8
8160163c:	e3500000 	cmp	r0, #0
81601640:	0a00000a 	beq	81601670 <do_bootm+0xb0>
81601644:	e590100c 	ldr	r1, [r0, #12]
81601648:	e59f0070 	ldr	r0, [pc, #112]	; 816016c0 <do_bootm+0x100>
8160164c:	e3510001 	cmp	r1, #1
81601650:	03a01007 	moveq	r1, #7
81601654:	e3110001 	tst	r1, #1
81601658:	1a000007 	bne	8160167c <do_bootm+0xbc>
8160165c:	e590c08c 	ldr	ip, [r0, #140]	; 0x8c
81601660:	e151000c 	cmp	r1, ip
81601664:	ca000004 	bgt	8160167c <do_bootm+0xbc>
81601668:	e59f0054 	ldr	r0, [pc, #84]	; 816016c4 <do_bootm+0x104>
8160166c:	eb004402 	bl	8161267c <printf>
81601670:	e3e00000 	mvn	r0, #0
81601674:	e28dd01c 	add	sp, sp, #28
81601678:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
8160167c:	e3a0c000 	mov	ip, #0
81601680:	e58d1000 	str	r1, [sp]
81601684:	e98d1001 	stmib	sp, {r0, ip}
81601688:	e1a01006 	mov	r1, r6
8160168c:	e1a00005 	mov	r0, r5
81601690:	eb000e7d 	bl	8160508c <do_bootm_states>
81601694:	eafffff6 	b	81601674 <do_bootm+0xb4>
81601698:	e3a03001 	mov	r3, #1
8160169c:	e58d3008 	str	r3, [sp, #8]
816016a0:	e59f3018 	ldr	r3, [pc, #24]	; 816016c0 <do_bootm+0x100>
816016a4:	e1a02008 	mov	r2, r8
816016a8:	e58d3004 	str	r3, [sp, #4]
816016ac:	e300371f 	movw	r3, #1823	; 0x71f
816016b0:	e58d3000 	str	r3, [sp]
816016b4:	e1a0300a 	mov	r3, sl
816016b8:	eafffff2 	b	81601688 <do_bootm+0xc8>
816016bc:	81617ffc 	.word	0x81617ffc
816016c0:	81619880 	.word	0x81619880
816016c4:	81614c03 	.word	0x81614c03

816016c8 <image_info>:
816016c8:	e92d4070 	push	{r4, r5, r6, lr}
816016cc:	e1a01000 	mov	r1, r0
816016d0:	e1a04000 	mov	r4, r0
816016d4:	e59f0094 	ldr	r0, [pc, #148]	; 81601770 <image_info+0xa8>
816016d8:	eb0043e7 	bl	8161267c <printf>
816016dc:	e1a00004 	mov	r0, r4
816016e0:	eb001a4c 	bl	81608018 <genimg_get_format>
816016e4:	e3500001 	cmp	r0, #1
816016e8:	1a00001c 	bne	81601760 <image_info+0x98>
816016ec:	e1a05000 	mov	r5, r0
816016f0:	e59f007c 	ldr	r0, [pc, #124]	; 81601774 <image_info+0xac>
816016f4:	eb0015a6 	bl	81606d94 <puts>
816016f8:	e5942000 	ldr	r2, [r4]
816016fc:	e59f3074 	ldr	r3, [pc, #116]	; 81601778 <image_info+0xb0>
81601700:	e1520003 	cmp	r2, r3
81601704:	159f0070 	ldrne	r0, [pc, #112]	; 8160177c <image_info+0xb4>
81601708:	1a00000d 	bne	81601744 <image_info+0x7c>
8160170c:	e1a00004 	mov	r0, r4
81601710:	eb001919 	bl	81607b7c <image_check_hcrc>
81601714:	e3500000 	cmp	r0, #0
81601718:	059f0060 	ldreq	r0, [pc, #96]	; 81601780 <image_info+0xb8>
8160171c:	0a000008 	beq	81601744 <image_info+0x7c>
81601720:	e1a00004 	mov	r0, r4
81601724:	eb0019c6 	bl	81607e44 <image_print_contents>
81601728:	e59f0054 	ldr	r0, [pc, #84]	; 81601784 <image_info+0xbc>
8160172c:	eb001598 	bl	81606d94 <puts>
81601730:	e1a00004 	mov	r0, r4
81601734:	eb001923 	bl	81607bc8 <image_check_dcrc>
81601738:	e3500000 	cmp	r0, #0
8160173c:	1a000003 	bne	81601750 <image_info+0x88>
81601740:	e59f0040 	ldr	r0, [pc, #64]	; 81601788 <image_info+0xc0>
81601744:	eb001592 	bl	81606d94 <puts>
81601748:	e1a00005 	mov	r0, r5
8160174c:	e8bd8070 	pop	{r4, r5, r6, pc}
81601750:	e59f0034 	ldr	r0, [pc, #52]	; 8160178c <image_info+0xc4>
81601754:	e3a05000 	mov	r5, #0
81601758:	eb00158d 	bl	81606d94 <puts>
8160175c:	eafffff9 	b	81601748 <image_info+0x80>
81601760:	e59f0028 	ldr	r0, [pc, #40]	; 81601790 <image_info+0xc8>
81601764:	e3a05001 	mov	r5, #1
81601768:	eb001589 	bl	81606d94 <puts>
8160176c:	eafffff5 	b	81601748 <image_info+0x80>
81601770:	81616031 	.word	0x81616031
81601774:	81616052 	.word	0x81616052
81601778:	56190527 	.word	0x56190527
8160177c:	81616069 	.word	0x81616069
81601780:	8161607e 	.word	0x8161607e
81601784:	81616096 	.word	0x81616096
81601788:	816160b1 	.word	0x816160b1
8160178c:	816160c2 	.word	0x816160c2
81601790:	816160c6 	.word	0x816160c6

81601794 <do_iminfo>:
81601794:	e3520001 	cmp	r2, #1
81601798:	ca000002 	bgt	816017a8 <do_iminfo+0x14>
8160179c:	e59f3040 	ldr	r3, [pc, #64]	; 816017e4 <do_iminfo+0x50>
816017a0:	e5930000 	ldr	r0, [r3]
816017a4:	eaffffc7 	b	816016c8 <image_info>
816017a8:	e92d4070 	push	{r4, r5, r6, lr}
816017ac:	e2836004 	add	r6, r3, #4
816017b0:	e0834102 	add	r4, r3, r2, lsl #2
816017b4:	e3a05000 	mov	r5, #0
816017b8:	e3a02010 	mov	r2, #16
816017bc:	e3a01000 	mov	r1, #0
816017c0:	e4960004 	ldr	r0, [r6], #4
816017c4:	eb0043f3 	bl	81612798 <simple_strtoul>
816017c8:	ebffffbe 	bl	816016c8 <image_info>
816017cc:	e3500000 	cmp	r0, #0
816017d0:	13a05001 	movne	r5, #1
816017d4:	e1560004 	cmp	r6, r4
816017d8:	1afffff6 	bne	816017b8 <do_iminfo+0x24>
816017dc:	e1a00005 	mov	r0, r5
816017e0:	e8bd8070 	pop	{r4, r5, r6, pc}
816017e4:	816190ac 	.word	0x816190ac

816017e8 <do_help>:
816017e8:	e92d4007 	push	{r0, r1, r2, lr}
816017ec:	e59fc02c 	ldr	ip, [pc, #44]	; 81601820 <do_help+0x38>
816017f0:	e59fe02c 	ldr	lr, [pc, #44]	; 81601824 <do_help+0x3c>
816017f4:	e88d000c 	stm	sp, {r2, r3}
816017f8:	e1a03001 	mov	r3, r1
816017fc:	e04cc00e 	sub	ip, ip, lr
81601800:	e59f1020 	ldr	r1, [pc, #32]	; 81601828 <do_help+0x40>
81601804:	e1a02000 	mov	r2, r0
81601808:	e1a0000e 	mov	r0, lr
8160180c:	e1a0c14c 	asr	ip, ip, #2
81601810:	e0010c91 	mul	r1, r1, ip
81601814:	eb001dc3 	bl	81608f28 <_do_help>
81601818:	e28dd00c 	add	sp, sp, #12
8160181c:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
81601820:	81619420 	.word	0x81619420
81601824:	8161912c 	.word	0x8161912c
81601828:	b6db6db7 	.word	0xb6db6db7

8160182c <do_version>:
8160182c:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
81601830:	e24dd0cc 	sub	sp, sp, #204	; 0xcc
81601834:	e3a020c8 	mov	r2, #200	; 0xc8
81601838:	e3a00000 	mov	r0, #0
8160183c:	e1a0100d 	mov	r1, sp
81601840:	eb003e8c 	bl	81611278 <display_options_get_banner>
81601844:	eb00438c 	bl	8161267c <printf>
81601848:	e59f0014 	ldr	r0, [pc, #20]	; 81601864 <do_version+0x38>
8160184c:	eb001550 	bl	81606d94 <puts>
81601850:	e59f0010 	ldr	r0, [pc, #16]	; 81601868 <do_version+0x3c>
81601854:	eb00154e 	bl	81606d94 <puts>
81601858:	e3a00000 	mov	r0, #0
8160185c:	e28dd0cc 	add	sp, sp, #204	; 0xcc
81601860:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
81601864:	81615917 	.word	0x81615917
81601868:	81615994 	.word	0x81615994

8160186c <source>:
8160186c:	e92d4070 	push	{r4, r5, r6, lr}
81601870:	e1a04000 	mov	r4, r0
81601874:	e59f00a8 	ldr	r0, [pc, #168]	; 81601924 <source+0xb8>
81601878:	eb0026ea 	bl	8160b428 <env_get_yesno>
8160187c:	e1a05000 	mov	r5, r0
81601880:	e1a00004 	mov	r0, r4
81601884:	eb0019e3 	bl	81608018 <genimg_get_format>
81601888:	e3500001 	cmp	r0, #1
8160188c:	1a000020 	bne	81601914 <source+0xa8>
81601890:	e5942000 	ldr	r2, [r4]
81601894:	e59f308c 	ldr	r3, [pc, #140]	; 81601928 <source+0xbc>
81601898:	e1520003 	cmp	r2, r3
8160189c:	159f0088 	ldrne	r0, [pc, #136]	; 8160192c <source+0xc0>
816018a0:	1a00001c 	bne	81601918 <source+0xac>
816018a4:	e1a00004 	mov	r0, r4
816018a8:	eb0018b3 	bl	81607b7c <image_check_hcrc>
816018ac:	e3500000 	cmp	r0, #0
816018b0:	059f0078 	ldreq	r0, [pc, #120]	; 81601930 <source+0xc4>
816018b4:	0a000017 	beq	81601918 <source+0xac>
816018b8:	e3550000 	cmp	r5, #0
816018bc:	0a000004 	beq	816018d4 <source+0x68>
816018c0:	e1a00004 	mov	r0, r4
816018c4:	eb0018bf 	bl	81607bc8 <image_check_dcrc>
816018c8:	e3500000 	cmp	r0, #0
816018cc:	059f0060 	ldreq	r0, [pc, #96]	; 81601934 <source+0xc8>
816018d0:	0a000010 	beq	81601918 <source+0xac>
816018d4:	e5d4301e 	ldrb	r3, [r4, #30]
816018d8:	e3530006 	cmp	r3, #6
816018dc:	159f0054 	ldrne	r0, [pc, #84]	; 81601938 <source+0xcc>
816018e0:	1a00000c 	bne	81601918 <source+0xac>
816018e4:	e5941040 	ldr	r1, [r4, #64]	; 0x40
816018e8:	e2840040 	add	r0, r4, #64	; 0x40
816018ec:	e6bf1f31 	rev	r1, r1
816018f0:	e3510000 	cmp	r1, #0
816018f4:	059f0040 	ldreq	r0, [pc, #64]	; 8160193c <source+0xd0>
816018f8:	0a000006 	beq	81601918 <source+0xac>
816018fc:	e5902000 	ldr	r2, [r0]
81601900:	e2800004 	add	r0, r0, #4
81601904:	e3520000 	cmp	r2, #0
81601908:	1afffffb 	bne	816018fc <source+0x90>
8160190c:	e8bd4070 	pop	{r4, r5, r6, lr}
81601910:	ea001cf7 	b	81608cf4 <run_command_list>
81601914:	e59f0024 	ldr	r0, [pc, #36]	; 81601940 <source+0xd4>
81601918:	eb00151d 	bl	81606d94 <puts>
8160191c:	e3a00001 	mov	r0, #1
81601920:	e8bd8070 	pop	{r4, r5, r6, pc}
81601924:	81616865 	.word	0x81616865
81601928:	56190527 	.word	0x56190527
8160192c:	8161686c 	.word	0x8161686c
81601930:	8161687e 	.word	0x8161687e
81601934:	8161688e 	.word	0x8161688e
81601938:	8161689c 	.word	0x8161689c
8160193c:	816168ac 	.word	0x816168ac
81601940:	816168ba 	.word	0x816168ba

81601944 <do_source>:
81601944:	e3520001 	cmp	r2, #1
81601948:	e92d4010 	push	{r4, lr}
8160194c:	d59f4030 	ldrle	r4, [pc, #48]	; 81601984 <do_source+0x40>
81601950:	da000004 	ble	81601968 <do_source+0x24>
81601954:	e3a02010 	mov	r2, #16
81601958:	e3a01000 	mov	r1, #0
8160195c:	e5930004 	ldr	r0, [r3, #4]
81601960:	eb00438c 	bl	81612798 <simple_strtoul>
81601964:	e1a04000 	mov	r4, r0
81601968:	e1a01004 	mov	r1, r4
8160196c:	e59f0014 	ldr	r0, [pc, #20]	; 81601988 <do_source+0x44>
81601970:	eb004341 	bl	8161267c <printf>
81601974:	e1a00004 	mov	r0, r4
81601978:	e3a01000 	mov	r1, #0
8160197c:	e8bd4010 	pop	{r4, lr}
81601980:	eaffffb9 	b	8160186c <source>
81601984:	80008000 	.word	0x80008000
81601988:	816158e2 	.word	0x816158e2

8160198c <print_num>:
8160198c:	e1a02001 	mov	r2, r1
81601990:	e1a01000 	mov	r1, r0
81601994:	e59f0000 	ldr	r0, [pc]	; 8160199c <print_num+0x10>
81601998:	ea004337 	b	8161267c <printf>
8160199c:	816166e2 	.word	0x816166e2

816019a0 <do_bdinfo>:
816019a0:	e92d4010 	push	{r4, lr}
816019a4:	e5994000 	ldr	r4, [r9]
816019a8:	e59f00a8 	ldr	r0, [pc, #168]	; 81601a58 <do_bdinfo+0xb8>
816019ac:	e5941040 	ldr	r1, [r4, #64]	; 0x40
816019b0:	ebfffff5 	bl	8160198c <print_num>
816019b4:	e5941044 	ldr	r1, [r4, #68]	; 0x44
816019b8:	e59f009c 	ldr	r0, [pc, #156]	; 81601a5c <do_bdinfo+0xbc>
816019bc:	ebfffff2 	bl	8160198c <print_num>
816019c0:	e594304c 	ldr	r3, [r4, #76]	; 0x4c
816019c4:	e3530000 	cmp	r3, #0
816019c8:	0a000008 	beq	816019f0 <do_bdinfo+0x50>
816019cc:	e3a01000 	mov	r1, #0
816019d0:	e59f0088 	ldr	r0, [pc, #136]	; 81601a60 <do_bdinfo+0xc0>
816019d4:	ebffffec 	bl	8160198c <print_num>
816019d8:	e5941048 	ldr	r1, [r4, #72]	; 0x48
816019dc:	e59f0080 	ldr	r0, [pc, #128]	; 81601a64 <do_bdinfo+0xc4>
816019e0:	ebffffe9 	bl	8160198c <print_num>
816019e4:	e594104c 	ldr	r1, [r4, #76]	; 0x4c
816019e8:	e59f0078 	ldr	r0, [pc, #120]	; 81601a68 <do_bdinfo+0xc8>
816019ec:	ebffffe6 	bl	8160198c <print_num>
816019f0:	e5991008 	ldr	r1, [r9, #8]
816019f4:	e59f0070 	ldr	r0, [pc, #112]	; 81601a6c <do_bdinfo+0xcc>
816019f8:	eb00431f 	bl	8161267c <printf>
816019fc:	e59f006c 	ldr	r0, [pc, #108]	; 81601a70 <do_bdinfo+0xd0>
81601a00:	e59910a0 	ldr	r1, [r9, #160]	; 0xa0
81601a04:	ebffffe0 	bl	8160198c <print_num>
81601a08:	e59f0064 	ldr	r0, [pc, #100]	; 81601a74 <do_bdinfo+0xd4>
81601a0c:	e599102c 	ldr	r1, [r9, #44]	; 0x2c
81601a10:	ebffffdd 	bl	8160198c <print_num>
81601a14:	e59f005c 	ldr	r0, [pc, #92]	; 81601a78 <do_bdinfo+0xd8>
81601a18:	e5991040 	ldr	r1, [r9, #64]	; 0x40
81601a1c:	ebffffda 	bl	8160198c <print_num>
81601a20:	e59f0054 	ldr	r0, [pc, #84]	; 81601a7c <do_bdinfo+0xdc>
81601a24:	e5991038 	ldr	r1, [r9, #56]	; 0x38
81601a28:	ebffffd7 	bl	8160198c <print_num>
81601a2c:	e59f004c 	ldr	r0, [pc, #76]	; 81601a80 <do_bdinfo+0xe0>
81601a30:	e599103c 	ldr	r1, [r9, #60]	; 0x3c
81601a34:	ebffffd4 	bl	8160198c <print_num>
81601a38:	e5992048 	ldr	r2, [r9, #72]	; 0x48
81601a3c:	e3520000 	cmp	r2, #0
81601a40:	0a000002 	beq	81601a50 <do_bdinfo+0xb0>
81601a44:	e5991048 	ldr	r1, [r9, #72]	; 0x48
81601a48:	e59f0034 	ldr	r0, [pc, #52]	; 81601a84 <do_bdinfo+0xe4>
81601a4c:	eb00430a 	bl	8161267c <printf>
81601a50:	e3a00000 	mov	r0, #0
81601a54:	e8bd8010 	pop	{r4, pc}
81601a58:	81614b21 	.word	0x81614b21
81601a5c:	81614b2d 	.word	0x81614b2d
81601a60:	81614b39 	.word	0x81614b39
81601a64:	81614b43 	.word	0x81614b43
81601a68:	81614b4c 	.word	0x81614b4c
81601a6c:	81614b54 	.word	0x81614b54
81601a70:	81614b6a 	.word	0x81614b6a
81601a74:	81614b73 	.word	0x81614b73
81601a78:	81614b7d 	.word	0x81614b7d
81601a7c:	81614b87 	.word	0x81614b87
81601a80:	81614b8e 	.word	0x81614b8e
81601a84:	81614b98 	.word	0x81614b98

81601a88 <do_bootz>:
81601a88:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
81601a8c:	e2425001 	sub	r5, r2, #1
81601a90:	e59f40e0 	ldr	r4, [pc, #224]	; 81601b78 <do_bootz+0xf0>
81601a94:	e24dd01c 	sub	sp, sp, #28
81601a98:	e2836004 	add	r6, r3, #4
81601a9c:	e1a08003 	mov	r8, r3
81601aa0:	e1a02005 	mov	r2, r5
81601aa4:	e3a03001 	mov	r3, #1
81601aa8:	e58d3008 	str	r3, [sp, #8]
81601aac:	e1a0a000 	mov	sl, r0
81601ab0:	e88d0018 	stm	sp, {r3, r4}
81601ab4:	e1a03006 	mov	r3, r6
81601ab8:	e1a07001 	mov	r7, r1
81601abc:	eb000d72 	bl	8160508c <do_bootm_states>
81601ac0:	e3550000 	cmp	r5, #0
81601ac4:	1a00000b 	bne	81601af8 <do_bootz+0x70>
81601ac8:	e59f30ac 	ldr	r3, [pc, #172]	; 81601b7c <do_bootz+0xf4>
81601acc:	e5933000 	ldr	r3, [r3]
81601ad0:	e5843060 	str	r3, [r4, #96]	; 0x60
81601ad4:	e28d2014 	add	r2, sp, #20
81601ad8:	e28d1010 	add	r1, sp, #16
81601adc:	e5940060 	ldr	r0, [r4, #96]	; 0x60
81601ae0:	ebfffcbd 	bl	81600ddc <bootz_setup>
81601ae4:	e3500000 	cmp	r0, #0
81601ae8:	0a000008 	beq	81601b10 <do_bootz+0x88>
81601aec:	e3a00001 	mov	r0, #1
81601af0:	e28dd01c 	add	sp, sp, #28
81601af4:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
81601af8:	e3a02010 	mov	r2, #16
81601afc:	e3a01000 	mov	r1, #0
81601b00:	e5980004 	ldr	r0, [r8, #4]
81601b04:	eb004323 	bl	81612798 <simple_strtoul>
81601b08:	e5840060 	str	r0, [r4, #96]	; 0x60
81601b0c:	eafffff0 	b	81601ad4 <do_bootz+0x4c>
81601b10:	e59d3010 	ldr	r3, [sp, #16]
81601b14:	e59d2014 	ldr	r2, [sp, #20]
81601b18:	e5941060 	ldr	r1, [r4, #96]	; 0x60
81601b1c:	e0422003 	sub	r2, r2, r3
81601b20:	e59f0058 	ldr	r0, [pc, #88]	; 81601b80 <do_bootz+0xf8>
81601b24:	eb0033d4 	bl	8160ea7c <lmb_reserve>
81601b28:	e1a02006 	mov	r2, r6
81601b2c:	e1a01005 	mov	r1, r5
81601b30:	e1a00007 	mov	r0, r7
81601b34:	eb000cd9 	bl	81604ea0 <bootm_find_images>
81601b38:	e3500000 	cmp	r0, #0
81601b3c:	1affffea 	bne	81601aec <do_bootz+0x64>
81601b40:	eb000d50 	bl	81605088 <bootm_disable_interrupts>
81601b44:	e3a03005 	mov	r3, #5
81601b48:	e58d4004 	str	r4, [sp, #4]
81601b4c:	e1a02005 	mov	r2, r5
81601b50:	e5c4305e 	strb	r3, [r4, #94]	; 0x5e
81601b54:	e1a01007 	mov	r1, r7
81601b58:	e3a03001 	mov	r3, #1
81601b5c:	e1a0000a 	mov	r0, sl
81601b60:	e58d3008 	str	r3, [sp, #8]
81601b64:	e3a03e71 	mov	r3, #1808	; 0x710
81601b68:	e58d3000 	str	r3, [sp]
81601b6c:	e1a03006 	mov	r3, r6
81601b70:	eb000d45 	bl	8160508c <do_bootm_states>
81601b74:	eaffffdd 	b	81601af0 <do_bootz+0x68>
81601b78:	81619880 	.word	0x81619880
81601b7c:	816190ac 	.word	0x816190ac
81601b80:	81619910 	.word	0x81619910

81601b84 <do_echo>:
81601b84:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81601b88:	e3a07001 	mov	r7, #1
81601b8c:	e59f80a0 	ldr	r8, [pc, #160]	; 81601c34 <do_echo+0xb0>
81601b90:	e1a0a002 	mov	sl, r2
81601b94:	e1a0b003 	mov	fp, r3
81601b98:	e1a05007 	mov	r5, r7
81601b9c:	e155000a 	cmp	r5, sl
81601ba0:	ba000005 	blt	81601bbc <do_echo+0x38>
81601ba4:	e3570000 	cmp	r7, #0
81601ba8:	0a000001 	beq	81601bb4 <do_echo+0x30>
81601bac:	e3a0000a 	mov	r0, #10
81601bb0:	eb00146d 	bl	81606d6c <putc>
81601bb4:	e3a00000 	mov	r0, #0
81601bb8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81601bbc:	e3550001 	cmp	r5, #1
81601bc0:	e79b4105 	ldr	r4, [fp, r5, lsl #2]
81601bc4:	da000001 	ble	81601bd0 <do_echo+0x4c>
81601bc8:	e3a00020 	mov	r0, #32
81601bcc:	eb001466 	bl	81606d6c <putc>
81601bd0:	e1a01008 	mov	r1, r8
81601bd4:	e1a00004 	mov	r0, r4
81601bd8:	eb003f95 	bl	81611a34 <strstr>
81601bdc:	e2506000 	subs	r6, r0, #0
81601be0:	0a000010 	beq	81601c28 <do_echo+0xa4>
81601be4:	e3a07000 	mov	r7, #0
81601be8:	e1a00004 	mov	r0, r4
81601bec:	e5c67000 	strb	r7, [r6]
81601bf0:	e1a04006 	mov	r4, r6
81601bf4:	eb001466 	bl	81606d94 <puts>
81601bf8:	e3a0305c 	mov	r3, #92	; 0x5c
81601bfc:	e1a01008 	mov	r1, r8
81601c00:	e4c43002 	strb	r3, [r4], #2
81601c04:	e1a00004 	mov	r0, r4
81601c08:	eb003f89 	bl	81611a34 <strstr>
81601c0c:	e2506000 	subs	r6, r0, #0
81601c10:	1afffff4 	bne	81601be8 <do_echo+0x64>
81601c14:	e1a00004 	mov	r0, r4
81601c18:	e1a07006 	mov	r7, r6
81601c1c:	eb00145c 	bl	81606d94 <puts>
81601c20:	e2855001 	add	r5, r5, #1
81601c24:	eaffffdc 	b	81601b9c <do_echo+0x18>
81601c28:	e1a00004 	mov	r0, r4
81601c2c:	eb001458 	bl	81606d94 <puts>
81601c30:	eafffffa 	b	81601c20 <do_echo+0x9c>
81601c34:	816150e4 	.word	0x816150e4

81601c38 <load_elf_image_shdr>:
81601c38:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81601c3c:	e1a07000 	mov	r7, r0
81601c40:	e3a05000 	mov	r5, #0
81601c44:	e3a06028 	mov	r6, #40	; 0x28
81601c48:	e1d733b0 	ldrh	r3, [r7, #48]	; 0x30
81601c4c:	e1550003 	cmp	r5, r3
81601c50:	ba000001 	blt	81601c5c <load_elf_image_shdr+0x24>
81601c54:	e5970018 	ldr	r0, [r7, #24]
81601c58:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81601c5c:	e5974020 	ldr	r4, [r7, #32]
81601c60:	e0874004 	add	r4, r7, r4
81601c64:	e0244596 	mla	r4, r6, r5, r4
81601c68:	e5943008 	ldr	r3, [r4, #8]
81601c6c:	e3130002 	tst	r3, #2
81601c70:	0a00000d 	beq	81601cac <load_elf_image_shdr+0x74>
81601c74:	e594000c 	ldr	r0, [r4, #12]
81601c78:	e3500000 	cmp	r0, #0
81601c7c:	0a00000a 	beq	81601cac <load_elf_image_shdr+0x74>
81601c80:	e5942014 	ldr	r2, [r4, #20]
81601c84:	e3520000 	cmp	r2, #0
81601c88:	0a000007 	beq	81601cac <load_elf_image_shdr+0x74>
81601c8c:	e5943004 	ldr	r3, [r4, #4]
81601c90:	e3530008 	cmp	r3, #8
81601c94:	1a000006 	bne	81601cb4 <load_elf_image_shdr+0x7c>
81601c98:	e3a01000 	mov	r1, #0
81601c9c:	ebfffa87 	bl	816006c0 <memset>
81601ca0:	e5941014 	ldr	r1, [r4, #20]
81601ca4:	e594000c 	ldr	r0, [r4, #12]
81601ca8:	ebfffd3f 	bl	816011ac <flush_cache>
81601cac:	e2855001 	add	r5, r5, #1
81601cb0:	eaffffe4 	b	81601c48 <load_elf_image_shdr+0x10>
81601cb4:	e5941010 	ldr	r1, [r4, #16]
81601cb8:	e0871001 	add	r1, r7, r1
81601cbc:	ebfffab6 	bl	8160079c <memcpy>
81601cc0:	eafffff6 	b	81601ca0 <load_elf_image_shdr+0x68>

81601cc4 <valid_elf_image>:
81601cc4:	e92d4010 	push	{r4, lr}
81601cc8:	e1a01000 	mov	r1, r0
81601ccc:	e5902000 	ldr	r2, [r0]
81601cd0:	e59f302c 	ldr	r3, [pc, #44]	; 81601d04 <valid_elf_image+0x40>
81601cd4:	e1520003 	cmp	r2, r3
81601cd8:	159f0028 	ldrne	r0, [pc, #40]	; 81601d08 <valid_elf_image+0x44>
81601cdc:	1a000003 	bne	81601cf0 <valid_elf_image+0x2c>
81601ce0:	e1d131b0 	ldrh	r3, [r1, #16]
81601ce4:	e3530002 	cmp	r3, #2
81601ce8:	0a000003 	beq	81601cfc <valid_elf_image+0x38>
81601cec:	e59f0018 	ldr	r0, [pc, #24]	; 81601d0c <valid_elf_image+0x48>
81601cf0:	eb004261 	bl	8161267c <printf>
81601cf4:	e3a00000 	mov	r0, #0
81601cf8:	e8bd8010 	pop	{r4, pc}
81601cfc:	e3a00001 	mov	r0, #1
81601d00:	e8bd8010 	pop	{r4, pc}
81601d04:	464c457f 	.word	0x464c457f
81601d08:	81617b82 	.word	0x81617b82
81601d0c:	81617ba6 	.word	0x81617ba6

81601d10 <do_bootelf>:
81601d10:	e92d4dff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, sl, fp, lr}
81601d14:	e1a0a002 	mov	sl, r2
81601d18:	e59f01a8 	ldr	r0, [pc, #424]	; 81601ec8 <do_bootelf+0x1b8>
81601d1c:	e1a06003 	mov	r6, r3
81601d20:	eb000a1c 	bl	81604598 <env_get>
81601d24:	e24a5001 	sub	r5, sl, #1
81601d28:	e3550000 	cmp	r5, #0
81601d2c:	e2867004 	add	r7, r6, #4
81601d30:	e1a08000 	mov	r8, r0
81601d34:	da000042 	ble	81601e44 <do_bootelf+0x134>
81601d38:	e5964004 	ldr	r4, [r6, #4]
81601d3c:	e5d43000 	ldrb	r3, [r4]
81601d40:	e353002d 	cmp	r3, #45	; 0x2d
81601d44:	13a04000 	movne	r4, #0
81601d48:	1a000009 	bne	81601d74 <do_bootelf+0x64>
81601d4c:	e5d41001 	ldrb	r1, [r4, #1]
81601d50:	e3510073 	cmp	r1, #115	; 0x73
81601d54:	13510070 	cmpne	r1, #112	; 0x70
81601d58:	03a01001 	moveq	r1, #1
81601d5c:	13a01000 	movne	r1, #0
81601d60:	1a000035 	bne	81601e3c <do_bootelf+0x12c>
81601d64:	e24a5002 	sub	r5, sl, #2
81601d68:	e2867008 	add	r7, r6, #8
81601d6c:	e3550000 	cmp	r5, #0
81601d70:	da000007 	ble	81601d94 <do_bootelf+0x84>
81601d74:	e5970000 	ldr	r0, [r7]
81601d78:	e28d200c 	add	r2, sp, #12
81601d7c:	e3a01010 	mov	r1, #16
81601d80:	eb0042b0 	bl	81612848 <strict_strtoul>
81601d84:	e3700016 	cmn	r0, #22
81601d88:	12455001 	subne	r5, r5, #1
81601d8c:	12877004 	addne	r7, r7, #4
81601d90:	1a000002 	bne	81601da0 <do_bootelf+0x90>
81601d94:	e59f3130 	ldr	r3, [pc, #304]	; 81601ecc <do_bootelf+0x1bc>
81601d98:	e5933000 	ldr	r3, [r3]
81601d9c:	e58d300c 	str	r3, [sp, #12]
81601da0:	e59d000c 	ldr	r0, [sp, #12]
81601da4:	ebffffc6 	bl	81601cc4 <valid_elf_image>
81601da8:	e3500000 	cmp	r0, #0
81601dac:	03a04001 	moveq	r4, #1
81601db0:	0a00001e 	beq	81601e30 <do_bootelf+0x120>
81601db4:	e3540000 	cmp	r4, #0
81601db8:	e59d600c 	ldr	r6, [sp, #12]
81601dbc:	0a000037 	beq	81601ea0 <do_bootelf+0x190>
81601dc0:	e5d43001 	ldrb	r3, [r4, #1]
81601dc4:	e3530070 	cmp	r3, #112	; 0x70
81601dc8:	1a000034 	bne	81601ea0 <do_bootelf+0x190>
81601dcc:	e596b01c 	ldr	fp, [r6, #28]
81601dd0:	e3a0a000 	mov	sl, #0
81601dd4:	e086300b 	add	r3, r6, fp
81601dd8:	e58d3004 	str	r3, [sp, #4]
81601ddc:	e59d3004 	ldr	r3, [sp, #4]
81601de0:	e083428a 	add	r4, r3, sl, lsl #5
81601de4:	e1d632bc 	ldrh	r3, [r6, #44]	; 0x2c
81601de8:	e15a0003 	cmp	sl, r3
81601dec:	ba000016 	blt	81601e4c <do_bootelf+0x13c>
81601df0:	e5963018 	ldr	r3, [r6, #24]
81601df4:	e58d300c 	str	r3, [sp, #12]
81601df8:	e3580000 	cmp	r8, #0
81601dfc:	1a00002b 	bne	81601eb0 <do_bootelf+0x1a0>
81601e00:	e59d100c 	ldr	r1, [sp, #12]
81601e04:	e59f00c4 	ldr	r0, [pc, #196]	; 81601ed0 <do_bootelf+0x1c0>
81601e08:	eb00421b 	bl	8161267c <printf>
81601e0c:	e1a01007 	mov	r1, r7
81601e10:	e1a00005 	mov	r0, r5
81601e14:	e59d300c 	ldr	r3, [sp, #12]
81601e18:	e12fff33 	blx	r3
81601e1c:	e2904000 	adds	r4, r0, #0
81601e20:	e1a01000 	mov	r1, r0
81601e24:	e59f00a8 	ldr	r0, [pc, #168]	; 81601ed4 <do_bootelf+0x1c4>
81601e28:	13a04001 	movne	r4, #1
81601e2c:	eb004212 	bl	8161267c <printf>
81601e30:	e1a00004 	mov	r0, r4
81601e34:	e28dd010 	add	sp, sp, #16
81601e38:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81601e3c:	e1a04001 	mov	r4, r1
81601e40:	eaffffcb 	b	81601d74 <do_bootelf+0x64>
81601e44:	e3a04000 	mov	r4, #0
81601e48:	eaffffd1 	b	81601d94 <do_bootelf+0x84>
81601e4c:	e5942010 	ldr	r2, [r4, #16]
81601e50:	e594b00c 	ldr	fp, [r4, #12]
81601e54:	e3520000 	cmp	r2, #0
81601e58:	0a000003 	beq	81601e6c <do_bootelf+0x15c>
81601e5c:	e5941004 	ldr	r1, [r4, #4]
81601e60:	e1a0000b 	mov	r0, fp
81601e64:	e0861001 	add	r1, r6, r1
81601e68:	ebfffa4b 	bl	8160079c <memcpy>
81601e6c:	e5940010 	ldr	r0, [r4, #16]
81601e70:	e5942014 	ldr	r2, [r4, #20]
81601e74:	e1500002 	cmp	r0, r2
81601e78:	0a000003 	beq	81601e8c <do_bootelf+0x17c>
81601e7c:	e0422000 	sub	r2, r2, r0
81601e80:	e3a01000 	mov	r1, #0
81601e84:	e08b0000 	add	r0, fp, r0
81601e88:	ebfffa0c 	bl	816006c0 <memset>
81601e8c:	e5941010 	ldr	r1, [r4, #16]
81601e90:	e1a0000b 	mov	r0, fp
81601e94:	ebfffcc4 	bl	816011ac <flush_cache>
81601e98:	e28aa001 	add	sl, sl, #1
81601e9c:	eaffffce 	b	81601ddc <do_bootelf+0xcc>
81601ea0:	e1a00006 	mov	r0, r6
81601ea4:	ebffff63 	bl	81601c38 <load_elf_image_shdr>
81601ea8:	e58d000c 	str	r0, [sp, #12]
81601eac:	eaffffd1 	b	81601df8 <do_bootelf+0xe8>
81601eb0:	e59f1020 	ldr	r1, [pc, #32]	; 81601ed8 <do_bootelf+0x1c8>
81601eb4:	e1a00008 	mov	r0, r8
81601eb8:	eb003e60 	bl	81611840 <strcmp>
81601ebc:	e2504000 	subs	r4, r0, #0
81601ec0:	1affffce 	bne	81601e00 <do_bootelf+0xf0>
81601ec4:	eaffffd9 	b	81601e30 <do_bootelf+0x120>
81601ec8:	81614ba7 	.word	0x81614ba7
81601ecc:	816190ac 	.word	0x816190ac
81601ed0:	81614bb1 	.word	0x81614bb1
81601ed4:	81614bd9 	.word	0x81614bd9
81601ed8:	81614c00 	.word	0x81614c00

81601edc <do_bootvx>:
81601edc:	e3520001 	cmp	r2, #1
81601ee0:	e92d40f0 	push	{r4, r5, r6, r7, lr}
81601ee4:	e24dd084 	sub	sp, sp, #132	; 0x84
81601ee8:	d59f3270 	ldrle	r3, [pc, #624]	; 81602160 <do_bootvx+0x284>
81601eec:	d5935000 	ldrle	r5, [r3]
81601ef0:	da000004 	ble	81601f08 <do_bootvx+0x2c>
81601ef4:	e3a02010 	mov	r2, #16
81601ef8:	e3a01000 	mov	r1, #0
81601efc:	e5930004 	ldr	r0, [r3, #4]
81601f00:	eb004224 	bl	81612798 <simple_strtoul>
81601f04:	e1a05000 	mov	r5, r0
81601f08:	e59f0254 	ldr	r0, [pc, #596]	; 81602164 <do_bootvx+0x288>
81601f0c:	eb0013a0 	bl	81606d94 <puts>
81601f10:	e59f0250 	ldr	r0, [pc, #592]	; 81602168 <do_bootvx+0x28c>
81601f14:	eb00099f 	bl	81604598 <env_get>
81601f18:	e3500000 	cmp	r0, #0
81601f1c:	1a000013 	bne	81601f70 <do_bootvx+0x94>
81601f20:	e59f0244 	ldr	r0, [pc, #580]	; 8160216c <do_bootvx+0x290>
81601f24:	eb0041d4 	bl	8161267c <printf>
81601f28:	e1a00005 	mov	r0, r5
81601f2c:	ebffff64 	bl	81601cc4 <valid_elf_image>
81601f30:	e3500000 	cmp	r0, #0
81601f34:	0a000086 	beq	81602154 <do_bootvx+0x278>
81601f38:	e1a00005 	mov	r0, r5
81601f3c:	ebffff3d 	bl	81601c38 <load_elf_image_shdr>
81601f40:	e1a05000 	mov	r5, r0
81601f44:	e1a01005 	mov	r1, r5
81601f48:	e59f0220 	ldr	r0, [pc, #544]	; 81602170 <do_bootvx+0x294>
81601f4c:	eb0041ca 	bl	8161267c <printf>
81601f50:	ebfffd41 	bl	8160145c <dcache_disable>
81601f54:	e3a00000 	mov	r0, #0
81601f58:	e12fff35 	blx	r5
81601f5c:	e59f0210 	ldr	r0, [pc, #528]	; 81602174 <do_bootvx+0x298>
81601f60:	eb00138b 	bl	81606d94 <puts>
81601f64:	e3a00001 	mov	r0, #1
81601f68:	e28dd084 	add	sp, sp, #132	; 0x84
81601f6c:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81601f70:	e3a02010 	mov	r2, #16
81601f74:	e3a01000 	mov	r1, #0
81601f78:	eb004206 	bl	81612798 <simple_strtoul>
81601f7c:	e1a06000 	mov	r6, r0
81601f80:	e59f01f0 	ldr	r0, [pc, #496]	; 81602178 <do_bootvx+0x29c>
81601f84:	eb000983 	bl	81604598 <env_get>
81601f88:	e2504000 	subs	r4, r0, #0
81601f8c:	0a000012 	beq	81601fdc <do_bootvx+0x100>
81601f90:	eb003e4f 	bl	816118d4 <strlen>
81601f94:	e1a01004 	mov	r1, r4
81601f98:	e35000ff 	cmp	r0, #255	; 0xff
81601f9c:	21a02000 	movcs	r2, r0
81601fa0:	33a020ff 	movcc	r2, #255	; 0xff
81601fa4:	e1a00006 	mov	r0, r6
81601fa8:	ebfff9fb 	bl	8160079c <memcpy>
81601fac:	e1a00004 	mov	r0, r4
81601fb0:	eb003e47 	bl	816118d4 <strlen>
81601fb4:	e35000ff 	cmp	r0, #255	; 0xff
81601fb8:	21a01000 	movcs	r1, r0
81601fbc:	33a010ff 	movcc	r1, #255	; 0xff
81601fc0:	e1a00006 	mov	r0, r6
81601fc4:	ebfffc78 	bl	816011ac <flush_cache>
81601fc8:	e1a02006 	mov	r2, r6
81601fcc:	e1a01006 	mov	r1, r6
81601fd0:	e59f01a4 	ldr	r0, [pc, #420]	; 8160217c <do_bootvx+0x2a0>
81601fd4:	eb0041a8 	bl	8161267c <printf>
81601fd8:	eaffffd2 	b	81601f28 <do_bootvx+0x4c>
81601fdc:	e59f019c 	ldr	r0, [pc, #412]	; 81602180 <do_bootvx+0x2a4>
81601fe0:	eb00096c 	bl	81604598 <env_get>
81601fe4:	e2504000 	subs	r4, r0, #0
81601fe8:	0a00004e 	beq	81602128 <do_bootvx+0x24c>
81601fec:	e1a01004 	mov	r1, r4
81601ff0:	e1a0000d 	mov	r0, sp
81601ff4:	eb003e02 	bl	81611804 <strcpy>
81601ff8:	e1a00004 	mov	r0, r4
81601ffc:	eb003e34 	bl	816118d4 <strlen>
81602000:	e1a04000 	mov	r4, r0
81602004:	e59f0178 	ldr	r0, [pc, #376]	; 81602184 <do_bootvx+0x2a8>
81602008:	eb000962 	bl	81604598 <env_get>
8160200c:	e08d3004 	add	r3, sp, r4
81602010:	e2502000 	subs	r2, r0, #0
81602014:	0a000046 	beq	81602134 <do_bootvx+0x258>
81602018:	e59f1168 	ldr	r1, [pc, #360]	; 81602188 <do_bootvx+0x2ac>
8160201c:	e1a00003 	mov	r0, r3
81602020:	eb00418b 	bl	81612654 <sprintf>
81602024:	e0844000 	add	r4, r4, r0
81602028:	e59f015c 	ldr	r0, [pc, #348]	; 8160218c <do_bootvx+0x2b0>
8160202c:	eb000959 	bl	81604598 <env_get>
81602030:	e2502000 	subs	r2, r0, #0
81602034:	0a000010 	beq	8160207c <do_bootvx+0x1a0>
81602038:	e59f1150 	ldr	r1, [pc, #336]	; 81602190 <do_bootvx+0x2b4>
8160203c:	e08d0004 	add	r0, sp, r4
81602040:	eb004183 	bl	81612654 <sprintf>
81602044:	e0844000 	add	r4, r4, r0
81602048:	e59f0144 	ldr	r0, [pc, #324]	; 81602194 <do_bootvx+0x2b8>
8160204c:	eb000951 	bl	81604598 <env_get>
81602050:	e08d7004 	add	r7, sp, r4
81602054:	e3500000 	cmp	r0, #0
81602058:	0a000039 	beq	81602144 <do_bootvx+0x268>
8160205c:	e59f0130 	ldr	r0, [pc, #304]	; 81602194 <do_bootvx+0x2b8>
81602060:	eb00094c 	bl	81604598 <env_get>
81602064:	eb0032df 	bl	8160ebe8 <string_to_ip>
81602068:	e59f1128 	ldr	r1, [pc, #296]	; 81602198 <do_bootvx+0x2bc>
8160206c:	e6bf2f30 	rev	r2, r0
81602070:	e1a00007 	mov	r0, r7
81602074:	eb004176 	bl	81612654 <sprintf>
81602078:	e0844000 	add	r4, r4, r0
8160207c:	e59f0118 	ldr	r0, [pc, #280]	; 8160219c <do_bootvx+0x2c0>
81602080:	eb000944 	bl	81604598 <env_get>
81602084:	e2502000 	subs	r2, r0, #0
81602088:	0a000003 	beq	8160209c <do_bootvx+0x1c0>
8160208c:	e08d0004 	add	r0, sp, r4
81602090:	e59f1108 	ldr	r1, [pc, #264]	; 816021a0 <do_bootvx+0x2c4>
81602094:	eb00416e 	bl	81612654 <sprintf>
81602098:	e0844000 	add	r4, r4, r0
8160209c:	e59f0100 	ldr	r0, [pc, #256]	; 816021a4 <do_bootvx+0x2c8>
816020a0:	eb00093c 	bl	81604598 <env_get>
816020a4:	e2502000 	subs	r2, r0, #0
816020a8:	0a000003 	beq	816020bc <do_bootvx+0x1e0>
816020ac:	e08d0004 	add	r0, sp, r4
816020b0:	e59f10f0 	ldr	r1, [pc, #240]	; 816021a8 <do_bootvx+0x2cc>
816020b4:	eb004166 	bl	81612654 <sprintf>
816020b8:	e0844000 	add	r4, r4, r0
816020bc:	e59f00e8 	ldr	r0, [pc, #232]	; 816021ac <do_bootvx+0x2d0>
816020c0:	eb000934 	bl	81604598 <env_get>
816020c4:	e2502000 	subs	r2, r0, #0
816020c8:	0a000003 	beq	816020dc <do_bootvx+0x200>
816020cc:	e08d0004 	add	r0, sp, r4
816020d0:	e59f10d8 	ldr	r1, [pc, #216]	; 816021b0 <do_bootvx+0x2d4>
816020d4:	eb00415e 	bl	81612654 <sprintf>
816020d8:	e0844000 	add	r4, r4, r0
816020dc:	e59f00d0 	ldr	r0, [pc, #208]	; 816021b4 <do_bootvx+0x2d8>
816020e0:	eb00092c 	bl	81604598 <env_get>
816020e4:	e2507000 	subs	r7, r0, #0
816020e8:	0a000004 	beq	81602100 <do_bootvx+0x224>
816020ec:	e1a01007 	mov	r1, r7
816020f0:	e08d0004 	add	r0, sp, r4
816020f4:	eb003dc2 	bl	81611804 <strcpy>
816020f8:	e1a00007 	mov	r0, r7
816020fc:	eb003df4 	bl	816118d4 <strlen>
81602100:	e1a0000d 	mov	r0, sp
81602104:	eb003df2 	bl	816118d4 <strlen>
81602108:	e1a0100d 	mov	r1, sp
8160210c:	e35000ff 	cmp	r0, #255	; 0xff
81602110:	21a02000 	movcs	r2, r0
81602114:	33a020ff 	movcc	r2, #255	; 0xff
81602118:	e1a00006 	mov	r0, r6
8160211c:	ebfff99e 	bl	8160079c <memcpy>
81602120:	e1a0000d 	mov	r0, sp
81602124:	eaffffa1 	b	81601fb0 <do_bootvx+0xd4>
81602128:	e59f0088 	ldr	r0, [pc, #136]	; 816021b8 <do_bootvx+0x2dc>
8160212c:	eb004152 	bl	8161267c <printf>
81602130:	eaffffb3 	b	81602004 <do_bootvx+0x128>
81602134:	e59f1080 	ldr	r1, [pc, #128]	; 816021bc <do_bootvx+0x2e0>
81602138:	e1a00003 	mov	r0, r3
8160213c:	eb004144 	bl	81612654 <sprintf>
81602140:	eaffffb7 	b	81602024 <do_bootvx+0x148>
81602144:	e59f1074 	ldr	r1, [pc, #116]	; 816021c0 <do_bootvx+0x2e4>
81602148:	e1a00007 	mov	r0, r7
8160214c:	eb004140 	bl	81612654 <sprintf>
81602150:	eaffffc8 	b	81602078 <do_bootvx+0x19c>
81602154:	e59f0068 	ldr	r0, [pc, #104]	; 816021c4 <do_bootvx+0x2e8>
81602158:	eb00130d 	bl	81606d94 <puts>
8160215c:	eaffff78 	b	81601f44 <do_bootvx+0x68>
81602160:	816190ac 	.word	0x816190ac
81602164:	81614ef9 	.word	0x81614ef9
81602168:	81614f27 	.word	0x81614f27
8160216c:	81614f30 	.word	0x81614f30
81602170:	81615046 	.word	0x81615046
81602174:	8161506a 	.word	0x8161506a
81602178:	81614ff6 	.word	0x81614ff6
8160217c:	81614fff 	.word	0x81614fff
81602180:	81614f5b 	.word	0x81614f5b
81602184:	81614f89 	.word	0x81614f89
81602188:	81614f92 	.word	0x81614f92
8160218c:	81614fa9 	.word	0x81614fa9
81602190:	81614fb0 	.word	0x81614fb0
81602194:	81614fb5 	.word	0x81614fb5
81602198:	81614fbd 	.word	0x81614fbd
8160219c:	81614fc4 	.word	0x81614fc4
816021a0:	81614fcd 	.word	0x81614fcd
816021a4:	81614fd3 	.word	0x81614fd3
816021a8:	81614fdd 	.word	0x81614fdd
816021ac:	81614fe3 	.word	0x81614fe3
816021b0:	81614fec 	.word	0x81614fec
816021b4:	81614ff3 	.word	0x81614ff3
816021b8:	81614f63 	.word	0x81614f63
816021bc:	81614f9b 	.word	0x81614f9b
816021c0:	81616122 	.word	0x81616122
816021c4:	81615020 	.word	0x81615020

816021c8 <fdt_valid>:
816021c8:	e92d4070 	push	{r4, r5, r6, lr}
816021cc:	e5904000 	ldr	r4, [r0]
816021d0:	e3540000 	cmp	r4, #0
816021d4:	1a000003 	bne	816021e8 <fdt_valid+0x20>
816021d8:	e59f0090 	ldr	r0, [pc, #144]	; 81602270 <fdt_valid+0xa8>
816021dc:	eb004126 	bl	8161267c <printf>
816021e0:	e1a00004 	mov	r0, r4
816021e4:	e8bd8070 	pop	{r4, r5, r6, pc}
816021e8:	e1a05000 	mov	r5, r0
816021ec:	e1a00004 	mov	r0, r4
816021f0:	eb0032de 	bl	8160ed70 <fdt_check_header>
816021f4:	e2506000 	subs	r6, r0, #0
816021f8:	aa00001a 	bge	81602268 <fdt_valid+0xa0>
816021fc:	eb0037c7 	bl	81610120 <fdt_strerror>
81602200:	e1a01000 	mov	r1, r0
81602204:	e59f0068 	ldr	r0, [pc, #104]	; 81602274 <fdt_valid+0xac>
81602208:	eb00411b 	bl	8161267c <printf>
8160220c:	e376000a 	cmn	r6, #10
81602210:	1a00000f 	bne	81602254 <fdt_valid+0x8c>
81602214:	e5941014 	ldr	r1, [r4, #20]
81602218:	e6bf1f31 	rev	r1, r1
8160221c:	e351000f 	cmp	r1, #15
81602220:	8a000002 	bhi	81602230 <fdt_valid+0x68>
81602224:	e3a02010 	mov	r2, #16
81602228:	e59f0048 	ldr	r0, [pc, #72]	; 81602278 <fdt_valid+0xb0>
8160222c:	eb004112 	bl	8161267c <printf>
81602230:	e5943018 	ldr	r3, [r4, #24]
81602234:	e6bf3f33 	rev	r3, r3
81602238:	e3530011 	cmp	r3, #17
8160223c:	9a000004 	bls	81602254 <fdt_valid+0x8c>
81602240:	e5941014 	ldr	r1, [r4, #20]
81602244:	e3a02011 	mov	r2, #17
81602248:	e59f002c 	ldr	r0, [pc, #44]	; 8160227c <fdt_valid+0xb4>
8160224c:	e6bf1f31 	rev	r1, r1
81602250:	eb004109 	bl	8161267c <printf>
81602254:	e59f0024 	ldr	r0, [pc, #36]	; 81602280 <fdt_valid+0xb8>
81602258:	eb004107 	bl	8161267c <printf>
8160225c:	e3a00000 	mov	r0, #0
81602260:	e5850000 	str	r0, [r5]
81602264:	e8bd8070 	pop	{r4, r5, r6, pc}
81602268:	e3a00001 	mov	r0, #1
8160226c:	e8bd8070 	pop	{r4, r5, r6, pc}
81602270:	81615de6 	.word	0x81615de6
81602274:	81615e11 	.word	0x81615e11
81602278:	81615e2f 	.word	0x81615e2f
8160227c:	81615e47 	.word	0x81615e47
81602280:	8161623a 	.word	0x8161623a

81602284 <is_printable_string>:
81602284:	e3510000 	cmp	r1, #0
81602288:	0a00001c 	beq	81602300 <is_printable_string+0x7c>
8160228c:	e0803001 	add	r3, r0, r1
81602290:	e5533001 	ldrb	r3, [r3, #-1]
81602294:	e3530000 	cmp	r3, #0
81602298:	1353000a 	cmpne	r3, #10
8160229c:	059fc064 	ldreq	ip, [pc, #100]	; 81602308 <is_printable_string+0x84>
816022a0:	0a000009 	beq	816022cc <is_printable_string+0x48>
816022a4:	e3a01000 	mov	r1, #0
816022a8:	ea000014 	b	81602300 <is_printable_string+0x7c>
816022ac:	e3510000 	cmp	r1, #0
816022b0:	da00000d 	ble	816022ec <is_printable_string+0x68>
816022b4:	e3510001 	cmp	r1, #1
816022b8:	0a000010 	beq	81602300 <is_printable_string+0x7c>
816022bc:	e5d03000 	ldrb	r3, [r0]
816022c0:	e3530000 	cmp	r3, #0
816022c4:	0afffff6 	beq	816022a4 <is_printable_string+0x20>
816022c8:	e2411001 	sub	r1, r1, #1
816022cc:	e4d02001 	ldrb	r2, [r0], #1
816022d0:	e3520000 	cmp	r2, #0
816022d4:	0afffff4 	beq	816022ac <is_printable_string+0x28>
816022d8:	e7dc3002 	ldrb	r3, [ip, r2]
816022dc:	e21330b7 	ands	r3, r3, #183	; 0xb7
816022e0:	0affffef 	beq	816022a4 <is_printable_string+0x20>
816022e4:	e3510000 	cmp	r1, #0
816022e8:	cafffff6 	bgt	816022c8 <is_printable_string+0x44>
816022ec:	e2911000 	adds	r1, r1, #0
816022f0:	13a01001 	movne	r1, #1
816022f4:	e3520000 	cmp	r2, #0
816022f8:	13a01001 	movne	r1, #1
816022fc:	e2211001 	eor	r1, r1, #1
81602300:	e1a00001 	mov	r0, r1
81602304:	e12fff1e 	bx	lr
81602308:	816140e1 	.word	0x816140e1

8160230c <print_data>:
8160230c:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81602310:	e2515000 	subs	r5, r1, #0
81602314:	08bd8df0 	popeq	{r4, r5, r6, r7, r8, sl, fp, pc}
81602318:	e1a06000 	mov	r6, r0
8160231c:	ebffffd8 	bl	81602284 <is_printable_string>
81602320:	e2504000 	subs	r4, r0, #0
81602324:	0a000017 	beq	81602388 <print_data+0x7c>
81602328:	e59f0120 	ldr	r0, [pc, #288]	; 81602450 <print_data+0x144>
8160232c:	e3a04000 	mov	r4, #0
81602330:	eb001297 	bl	81606d94 <puts>
81602334:	e59f7118 	ldr	r7, [pc, #280]	; 81602454 <print_data+0x148>
81602338:	e1540005 	cmp	r4, r5
8160233c:	ba000002 	blt	8160234c <print_data+0x40>
81602340:	e59f0108 	ldr	r0, [pc, #264]	; 81602450 <print_data+0x144>
81602344:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
81602348:	ea001291 	b	81606d94 <puts>
8160234c:	e3540000 	cmp	r4, #0
81602350:	da000001 	ble	8160235c <print_data+0x50>
81602354:	e1a00007 	mov	r0, r7
81602358:	eb00128d 	bl	81606d94 <puts>
8160235c:	e1a00006 	mov	r0, r6
81602360:	e2844001 	add	r4, r4, #1
81602364:	eb00128a 	bl	81606d94 <puts>
81602368:	e1a00006 	mov	r0, r6
8160236c:	eb003d58 	bl	816118d4 <strlen>
81602370:	e0844000 	add	r4, r4, r0
81602374:	e1a00006 	mov	r0, r6
81602378:	eb003d55 	bl	816118d4 <strlen>
8160237c:	e2800001 	add	r0, r0, #1
81602380:	e0866000 	add	r6, r6, r0
81602384:	eaffffeb 	b	81602338 <print_data+0x2c>
81602388:	e3150003 	tst	r5, #3
8160238c:	1a00001d 	bne	81602408 <print_data+0xfc>
81602390:	e3550040 	cmp	r5, #64	; 0x40
81602394:	da000004 	ble	816023ac <print_data+0xa0>
81602398:	e1a02005 	mov	r2, r5
8160239c:	e1a01006 	mov	r1, r6
816023a0:	e59f00b0 	ldr	r0, [pc, #176]	; 81602458 <print_data+0x14c>
816023a4:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
816023a8:	ea0040b3 	b	8161267c <printf>
816023ac:	e59f00a8 	ldr	r0, [pc, #168]	; 8160245c <print_data+0x150>
816023b0:	eb0040b1 	bl	8161267c <printf>
816023b4:	e2853003 	add	r3, r5, #3
816023b8:	e3550000 	cmp	r5, #0
816023bc:	e59f809c 	ldr	r8, [pc, #156]	; 81602460 <print_data+0x154>
816023c0:	e59fa09c 	ldr	sl, [pc, #156]	; 81602464 <print_data+0x158>
816023c4:	b1a05003 	movlt	r5, r3
816023c8:	e1a05145 	asr	r5, r5, #2
816023cc:	e2457001 	sub	r7, r5, #1
816023d0:	e1540005 	cmp	r4, r5
816023d4:	ba000002 	blt	816023e4 <print_data+0xd8>
816023d8:	e59f0088 	ldr	r0, [pc, #136]	; 81602468 <print_data+0x15c>
816023dc:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
816023e0:	ea0040a5 	b	8161267c <printf>
816023e4:	e1540007 	cmp	r4, r7
816023e8:	e7961104 	ldr	r1, [r6, r4, lsl #2]
816023ec:	e59f0078 	ldr	r0, [pc, #120]	; 8160246c <print_data+0x160>
816023f0:	e2844001 	add	r4, r4, #1
816023f4:	a1a02008 	movge	r2, r8
816023f8:	b1a0200a 	movlt	r2, sl
816023fc:	e6bf1f31 	rev	r1, r1
81602400:	eb00409d 	bl	8161267c <printf>
81602404:	eafffff1 	b	816023d0 <print_data+0xc4>
81602408:	e3550040 	cmp	r5, #64	; 0x40
8160240c:	caffffe1 	bgt	81602398 <print_data+0x8c>
81602410:	e59f0058 	ldr	r0, [pc, #88]	; 81602470 <print_data+0x164>
81602414:	eb004098 	bl	8161267c <printf>
81602418:	e59f7040 	ldr	r7, [pc, #64]	; 81602460 <print_data+0x154>
8160241c:	e59f8040 	ldr	r8, [pc, #64]	; 81602464 <print_data+0x158>
81602420:	e1550004 	cmp	r5, r4
81602424:	d59f0048 	ldrle	r0, [pc, #72]	; 81602474 <print_data+0x168>
81602428:	daffffeb 	ble	816023dc <print_data+0xd0>
8160242c:	e2452001 	sub	r2, r5, #1
81602430:	e7d61004 	ldrb	r1, [r6, r4]
81602434:	e1540002 	cmp	r4, r2
81602438:	e59f0038 	ldr	r0, [pc, #56]	; 81602478 <print_data+0x16c>
8160243c:	e2844001 	add	r4, r4, #1
81602440:	a1a02007 	movge	r2, r7
81602444:	b1a02008 	movlt	r2, r8
81602448:	eb00408b 	bl	8161267c <printf>
8160244c:	eafffff3 	b	81602420 <print_data+0x114>
81602450:	8161668f 	.word	0x8161668f
81602454:	8161668c 	.word	0x8161668c
81602458:	81616691 	.word	0x81616691
8160245c:	816166a1 	.word	0x816166a1
81602460:	8161623b 	.word	0x8161623b
81602464:	81616122 	.word	0x81616122
81602468:	81617c0d 	.word	0x81617c0d
8160246c:	816166a3 	.word	0x816166a3
81602470:	816166ac 	.word	0x816166ac
81602474:	81617000 	.word	0x81617000
81602478:	816166ae 	.word	0x816166ae

8160247c <set_working_fdt_addr>:
8160247c:	e59f300c 	ldr	r3, [pc, #12]	; 81602490 <set_working_fdt_addr+0x14>
81602480:	e1a01000 	mov	r1, r0
81602484:	e5830000 	str	r0, [r3]
81602488:	e59f0004 	ldr	r0, [pc, #4]	; 81602494 <set_working_fdt_addr+0x18>
8160248c:	ea000755 	b	816041e8 <env_set_hex>
81602490:	81619860 	.word	0x81619860
81602494:	8161675e 	.word	0x8161675e

81602498 <do_fdt>:
81602498:	e3520001 	cmp	r2, #1
8160249c:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816024a0:	e24dd048 	sub	sp, sp, #72	; 0x48
816024a4:	ca000001 	bgt	816024b0 <do_fdt+0x18>
816024a8:	e3e06000 	mvn	r6, #0
816024ac:	ea00001a 	b	8160251c <do_fdt+0x84>
816024b0:	e1a08002 	mov	r8, r2
816024b4:	e59f1d94 	ldr	r1, [pc, #3476]	; 81603250 <do_fdt+0xdb8>
816024b8:	e3a02002 	mov	r2, #2
816024bc:	e5930004 	ldr	r0, [r3, #4]
816024c0:	e1a07003 	mov	r7, r3
816024c4:	eb003ce7 	bl	81611868 <strncmp>
816024c8:	e2506000 	subs	r6, r0, #0
816024cc:	1a000037 	bne	816025b0 <do_fdt+0x118>
816024d0:	e2588002 	subs	r8, r8, #2
816024d4:	1a000013 	bne	81602528 <do_fdt+0x90>
816024d8:	e59f3d74 	ldr	r3, [pc, #3444]	; 81603254 <do_fdt+0xdbc>
816024dc:	e5933000 	ldr	r3, [r3]
816024e0:	e3530000 	cmp	r3, #0
816024e4:	e58d3018 	str	r3, [sp, #24]
816024e8:	1a000001 	bne	816024f4 <do_fdt+0x5c>
816024ec:	e3a06001 	mov	r6, #1
816024f0:	ea000009 	b	8160251c <do_fdt+0x84>
816024f4:	e28d0018 	add	r0, sp, #24
816024f8:	ebffff32 	bl	816021c8 <fdt_valid>
816024fc:	e3500000 	cmp	r0, #0
81602500:	0afffff9 	beq	816024ec <do_fdt+0x54>
81602504:	e1a01006 	mov	r1, r6
81602508:	e59f0d48 	ldr	r0, [pc, #3400]	; 81603258 <do_fdt+0xdc0>
8160250c:	eb00083f 	bl	81604610 <env_get_hex>
81602510:	e1a01000 	mov	r1, r0
81602514:	e59f0d40 	ldr	r0, [pc, #3392]	; 8160325c <do_fdt+0xdc4>
81602518:	eb004057 	bl	8161267c <printf>
8160251c:	e1a00006 	mov	r0, r6
81602520:	e28dd048 	add	sp, sp, #72	; 0x48
81602524:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81602528:	e3a02010 	mov	r2, #16
8160252c:	e1a01006 	mov	r1, r6
81602530:	e5970008 	ldr	r0, [r7, #8]
81602534:	eb004097 	bl	81612798 <simple_strtoul>
81602538:	e1a04000 	mov	r4, r0
8160253c:	e28d0048 	add	r0, sp, #72	; 0x48
81602540:	e5204030 	str	r4, [r0, #-48]!	; 0xffffffd0
81602544:	ebffff1f 	bl	816021c8 <fdt_valid>
81602548:	e3500000 	cmp	r0, #0
8160254c:	0affffe6 	beq	816024ec <do_fdt+0x54>
81602550:	e1a00004 	mov	r0, r4
81602554:	ebffffc8 	bl	8160247c <set_working_fdt_addr>
81602558:	e3580001 	cmp	r8, #1
8160255c:	0affffee 	beq	8160251c <do_fdt+0x84>
81602560:	e3a02010 	mov	r2, #16
81602564:	e1a01006 	mov	r1, r6
81602568:	e597000c 	ldr	r0, [r7, #12]
8160256c:	eb004089 	bl	81612798 <simple_strtoul>
81602570:	e59d1018 	ldr	r1, [sp, #24]
81602574:	e5912004 	ldr	r2, [r1, #4]
81602578:	e6bf2f32 	rev	r2, r2
8160257c:	e1500002 	cmp	r0, r2
81602580:	31a01000 	movcc	r1, r0
81602584:	359f0cd4 	ldrcc	r0, [pc, #3284]	; 81603260 <do_fdt+0xdc8>
81602588:	3a0001b6 	bcc	81602c68 <do_fdt+0x7d0>
8160258c:	e1a02000 	mov	r2, r0
81602590:	e1a00001 	mov	r0, r1
81602594:	eb00367d 	bl	8160ff90 <fdt_open_into>
81602598:	e3500000 	cmp	r0, #0
8160259c:	0affffde 	beq	8160251c <do_fdt+0x84>
816025a0:	eb0036de 	bl	81610120 <fdt_strerror>
816025a4:	e1a01000 	mov	r1, r0
816025a8:	e59f0cb4 	ldr	r0, [pc, #3252]	; 81603264 <do_fdt+0xdcc>
816025ac:	eaffffd9 	b	81602518 <do_fdt+0x80>
816025b0:	e59f5c9c 	ldr	r5, [pc, #3228]	; 81603254 <do_fdt+0xdbc>
816025b4:	e5953000 	ldr	r3, [r5]
816025b8:	e1a04005 	mov	r4, r5
816025bc:	e3530000 	cmp	r3, #0
816025c0:	1a000002 	bne	816025d0 <do_fdt+0x138>
816025c4:	e59f0c9c 	ldr	r0, [pc, #3228]	; 81603268 <do_fdt+0xdd0>
816025c8:	eb0011f1 	bl	81606d94 <puts>
816025cc:	eaffffc6 	b	816024ec <do_fdt+0x54>
816025d0:	e3a02002 	mov	r2, #2
816025d4:	e59f1c90 	ldr	r1, [pc, #3216]	; 8160326c <do_fdt+0xdd4>
816025d8:	e5970004 	ldr	r0, [r7, #4]
816025dc:	eb003ca1 	bl	81611868 <strncmp>
816025e0:	e2506000 	subs	r6, r0, #0
816025e4:	1a00002e 	bne	816026a4 <do_fdt+0x20c>
816025e8:	e3580003 	cmp	r8, #3
816025ec:	daffffad 	ble	816024a8 <do_fdt+0x10>
816025f0:	e3a02010 	mov	r2, #16
816025f4:	e1a01006 	mov	r1, r6
816025f8:	e5970008 	ldr	r0, [r7, #8]
816025fc:	eb004065 	bl	81612798 <simple_strtoul>
81602600:	e5850000 	str	r0, [r5]
81602604:	e1a00005 	mov	r0, r5
81602608:	ebfffeee 	bl	816021c8 <fdt_valid>
8160260c:	e3500000 	cmp	r0, #0
81602610:	0affffb5 	beq	816024ec <do_fdt+0x54>
81602614:	e3a02010 	mov	r2, #16
81602618:	e1a01006 	mov	r1, r6
8160261c:	e597000c 	ldr	r0, [r7, #12]
81602620:	eb00405c 	bl	81612798 <simple_strtoul>
81602624:	e3580004 	cmp	r8, #4
81602628:	05953000 	ldreq	r3, [r5]
8160262c:	05933004 	ldreq	r3, [r3, #4]
81602630:	e1a0a000 	mov	sl, r0
81602634:	06bf3f33 	reveq	r3, r3
81602638:	0a00000d 	beq	81602674 <do_fdt+0x1dc>
8160263c:	e3a02010 	mov	r2, #16
81602640:	e1a01006 	mov	r1, r6
81602644:	e5970010 	ldr	r0, [r7, #16]
81602648:	eb004052 	bl	81612798 <simple_strtoul>
8160264c:	e5952000 	ldr	r2, [r5]
81602650:	e5922004 	ldr	r2, [r2, #4]
81602654:	e6bf2f32 	rev	r2, r2
81602658:	e1500002 	cmp	r0, r2
8160265c:	e1a01000 	mov	r1, r0
81602660:	e1a03000 	mov	r3, r0
81602664:	2a000002 	bcs	81602674 <do_fdt+0x1dc>
81602668:	e59f0c00 	ldr	r0, [pc, #3072]	; 81603270 <do_fdt+0xdd8>
8160266c:	eb004002 	bl	8161267c <printf>
81602670:	eaffff9d 	b	816024ec <do_fdt+0x54>
81602674:	e1a02003 	mov	r2, r3
81602678:	e1a0100a 	mov	r1, sl
8160267c:	e5940000 	ldr	r0, [r4]
81602680:	eb003642 	bl	8160ff90 <fdt_open_into>
81602684:	e2506000 	subs	r6, r0, #0
81602688:	0584a000 	streq	sl, [r4]
8160268c:	0affffa2 	beq	8160251c <do_fdt+0x84>
81602690:	eb0036a2 	bl	81610120 <fdt_strerror>
81602694:	e1a01000 	mov	r1, r0
81602698:	e59f0bc4 	ldr	r0, [pc, #3012]	; 81603264 <do_fdt+0xdcc>
8160269c:	eb003ff6 	bl	8161267c <printf>
816026a0:	eaffff91 	b	816024ec <do_fdt+0x54>
816026a4:	e3a02002 	mov	r2, #2
816026a8:	e59f1bc4 	ldr	r1, [pc, #3012]	; 81603274 <do_fdt+0xddc>
816026ac:	e5970004 	ldr	r0, [r7, #4]
816026b0:	eb003c6c 	bl	81611868 <strncmp>
816026b4:	e3500000 	cmp	r0, #0
816026b8:	1a000016 	bne	81602718 <do_fdt+0x280>
816026bc:	e3580003 	cmp	r8, #3
816026c0:	daffff78 	ble	816024a8 <do_fdt+0x10>
816026c4:	e5971008 	ldr	r1, [r7, #8]
816026c8:	e5950000 	ldr	r0, [r5]
816026cc:	e597400c 	ldr	r4, [r7, #12]
816026d0:	eb003419 	bl	8160f73c <fdt_path_offset>
816026d4:	e2501000 	subs	r1, r0, #0
816026d8:	aa000003 	bge	816026ec <do_fdt+0x254>
816026dc:	eb00368f 	bl	81610120 <fdt_strerror>
816026e0:	e1a01000 	mov	r1, r0
816026e4:	e59f0b8c 	ldr	r0, [pc, #2956]	; 81603278 <do_fdt+0xde0>
816026e8:	eaffffeb 	b	8160269c <do_fdt+0x204>
816026ec:	e1a02004 	mov	r2, r4
816026f0:	e5950000 	ldr	r0, [r5]
816026f4:	eb003605 	bl	8160ff10 <fdt_add_subnode>
816026f8:	e3500000 	cmp	r0, #0
816026fc:	ba000001 	blt	81602708 <do_fdt+0x270>
81602700:	e3a06000 	mov	r6, #0
81602704:	eaffff84 	b	8160251c <do_fdt+0x84>
81602708:	eb003684 	bl	81610120 <fdt_strerror>
8160270c:	e1a01000 	mov	r1, r0
81602710:	e59f0b64 	ldr	r0, [pc, #2916]	; 8160327c <do_fdt+0xde4>
81602714:	eaffffe0 	b	8160269c <do_fdt+0x204>
81602718:	e5970004 	ldr	r0, [r7, #4]
8160271c:	e5d03000 	ldrb	r3, [r0]
81602720:	e3530073 	cmp	r3, #115	; 0x73
81602724:	1a00009b 	bne	81602998 <do_fdt+0x500>
81602728:	e3580003 	cmp	r8, #3
8160272c:	daffff5d 	ble	816024a8 <do_fdt+0x10>
81602730:	e5971008 	ldr	r1, [r7, #8]
81602734:	e5950000 	ldr	r0, [r5]
81602738:	e597a00c 	ldr	sl, [r7, #12]
8160273c:	eb0033fe 	bl	8160f73c <fdt_path_offset>
81602740:	e2503000 	subs	r3, r0, #0
81602744:	e58d3008 	str	r3, [sp, #8]
81602748:	baffffe3 	blt	816026dc <do_fdt+0x244>
8160274c:	e3580004 	cmp	r8, #4
81602750:	03a03000 	moveq	r3, #0
81602754:	058d3010 	streq	r3, [sp, #16]
81602758:	0a000071 	beq	81602924 <do_fdt+0x48c>
8160275c:	e1a0200a 	mov	r2, sl
81602760:	e59d1008 	ldr	r1, [sp, #8]
81602764:	e28d3010 	add	r3, sp, #16
81602768:	e5950000 	ldr	r0, [r5]
8160276c:	eb003399 	bl	8160f5d8 <fdt_getprop>
81602770:	e59d2010 	ldr	r2, [sp, #16]
81602774:	e3520b01 	cmp	r2, #1024	; 0x400
81602778:	e1a01000 	mov	r1, r0
8160277c:	c1a01002 	movgt	r1, r2
81602780:	c59f0af8 	ldrgt	r0, [pc, #2808]	; 81603280 <do_fdt+0xde8>
81602784:	caffffc4 	bgt	8160269c <do_fdt+0x204>
81602788:	e3510000 	cmp	r1, #0
8160278c:	0a000001 	beq	81602798 <do_fdt+0x300>
81602790:	e59f0aec 	ldr	r0, [pc, #2796]	; 81603284 <do_fdt+0xdec>
81602794:	ebfff800 	bl	8160079c <memcpy>
81602798:	e5973010 	ldr	r3, [r7, #16]
8160279c:	e3a05000 	mov	r5, #0
816027a0:	e58d5010 	str	r5, [sp, #16]
816027a4:	e2876010 	add	r6, r7, #16
816027a8:	e2488004 	sub	r8, r8, #4
816027ac:	e58d3018 	str	r3, [sp, #24]
816027b0:	e5d32000 	ldrb	r2, [r3]
816027b4:	e352003c 	cmp	r2, #60	; 0x3c
816027b8:	1a00002f 	bne	8160287c <do_fdt+0x3e4>
816027bc:	e59fbac0 	ldr	fp, [pc, #2752]	; 81603284 <do_fdt+0xdec>
816027c0:	e2832001 	add	r2, r3, #1
816027c4:	e28d3048 	add	r3, sp, #72	; 0x48
816027c8:	e5232030 	str	r2, [r3, #-48]!	; 0xffffffd0
816027cc:	e59d7018 	ldr	r7, [sp, #24]
816027d0:	e5d71000 	ldrb	r1, [r7]
816027d4:	e351003e 	cmp	r1, #62	; 0x3e
816027d8:	0a000051 	beq	81602924 <do_fdt+0x48c>
816027dc:	e1580005 	cmp	r8, r5
816027e0:	ca000001 	bgt	816027ec <do_fdt+0x354>
816027e4:	e59f0a9c 	ldr	r0, [pc, #2716]	; 81603288 <do_fdt+0xdf0>
816027e8:	eaffffab 	b	8160269c <do_fdt+0x204>
816027ec:	e3510000 	cmp	r1, #0
816027f0:	02855001 	addeq	r5, r5, #1
816027f4:	07962105 	ldreq	r2, [r6, r5, lsl #2]
816027f8:	058d2018 	streq	r2, [sp, #24]
816027fc:	0afffff2 	beq	816027cc <do_fdt+0x334>
81602800:	e3a02000 	mov	r2, #0
81602804:	e1a01003 	mov	r1, r3
81602808:	e1a00007 	mov	r0, r7
8160280c:	e58d300c 	str	r3, [sp, #12]
81602810:	eb003fe0 	bl	81612798 <simple_strtoul>
81602814:	e5d72000 	ldrb	r2, [r7]
81602818:	e59d300c 	ldr	r3, [sp, #12]
8160281c:	e28bb004 	add	fp, fp, #4
81602820:	e352003f 	cmp	r2, #63	; 0x3f
81602824:	059d2018 	ldreq	r2, [sp, #24]
81602828:	02822001 	addeq	r2, r2, #1
8160282c:	058d2018 	streq	r2, [sp, #24]
81602830:	e59d2010 	ldr	r2, [sp, #16]
81602834:	e2822004 	add	r2, r2, #4
81602838:	e58d2010 	str	r2, [sp, #16]
8160283c:	e59d2018 	ldr	r2, [sp, #24]
81602840:	e0422007 	sub	r2, r2, r7
81602844:	16bf0f30 	revne	r0, r0
81602848:	150b0004 	strne	r0, [fp, #-4]
8160284c:	e3520000 	cmp	r2, #0
81602850:	ca000004 	bgt	81602868 <do_fdt+0x3d0>
81602854:	e1a01007 	mov	r1, r7
81602858:	e59f0a2c 	ldr	r0, [pc, #2604]	; 8160328c <do_fdt+0xdf4>
8160285c:	eaffff8e 	b	8160269c <do_fdt+0x204>
81602860:	e2822001 	add	r2, r2, #1
81602864:	e58d2018 	str	r2, [sp, #24]
81602868:	e59d2018 	ldr	r2, [sp, #24]
8160286c:	e5d21000 	ldrb	r1, [r2]
81602870:	e3510020 	cmp	r1, #32
81602874:	0afffff9 	beq	81602860 <do_fdt+0x3c8>
81602878:	eaffffd3 	b	816027cc <do_fdt+0x334>
8160287c:	e352005b 	cmp	r2, #91	; 0x5b
81602880:	159f79fc 	ldrne	r7, [pc, #2556]	; 81603284 <do_fdt+0xdec>
81602884:	1a000033 	bne	81602958 <do_fdt+0x4c0>
81602888:	e28db048 	add	fp, sp, #72	; 0x48
8160288c:	e2833001 	add	r3, r3, #1
81602890:	e59f79ec 	ldr	r7, [pc, #2540]	; 81603284 <do_fdt+0xdec>
81602894:	e52b3030 	str	r3, [fp, #-48]!	; 0xffffffd0
81602898:	e59f39f0 	ldr	r3, [pc, #2544]	; 81603290 <do_fdt+0xdf8>
8160289c:	e59d2018 	ldr	r2, [sp, #24]
816028a0:	e5d22000 	ldrb	r2, [r2]
816028a4:	e352005d 	cmp	r2, #93	; 0x5d
816028a8:	1a000002 	bne	816028b8 <do_fdt+0x420>
816028ac:	ea000018 	b	81602914 <do_fdt+0x47c>
816028b0:	e2800001 	add	r0, r0, #1
816028b4:	e58d0018 	str	r0, [sp, #24]
816028b8:	e59d0018 	ldr	r0, [sp, #24]
816028bc:	e5d02000 	ldrb	r2, [r0]
816028c0:	e3520020 	cmp	r2, #32
816028c4:	0afffff9 	beq	816028b0 <do_fdt+0x418>
816028c8:	e3520000 	cmp	r2, #0
816028cc:	02855001 	addeq	r5, r5, #1
816028d0:	07962105 	ldreq	r2, [r6, r5, lsl #2]
816028d4:	058d2018 	streq	r2, [sp, #24]
816028d8:	0a00000b 	beq	8160290c <do_fdt+0x474>
816028dc:	e7d32002 	ldrb	r2, [r3, r2]
816028e0:	e3120044 	tst	r2, #68	; 0x44
816028e4:	0a00000a 	beq	81602914 <do_fdt+0x47c>
816028e8:	e3a02010 	mov	r2, #16
816028ec:	e1a0100b 	mov	r1, fp
816028f0:	eb003fa8 	bl	81612798 <simple_strtoul>
816028f4:	e59f3994 	ldr	r3, [pc, #2452]	; 81603290 <do_fdt+0xdf8>
816028f8:	e59d2010 	ldr	r2, [sp, #16]
816028fc:	e2877001 	add	r7, r7, #1
81602900:	e2822001 	add	r2, r2, #1
81602904:	e58d2010 	str	r2, [sp, #16]
81602908:	e5470001 	strb	r0, [r7, #-1]
8160290c:	e1580005 	cmp	r8, r5
81602910:	caffffe1 	bgt	8160289c <do_fdt+0x404>
81602914:	e59d3018 	ldr	r3, [sp, #24]
81602918:	e5d31000 	ldrb	r1, [r3]
8160291c:	e351005d 	cmp	r1, #93	; 0x5d
81602920:	1affffaf 	bne	816027e4 <do_fdt+0x34c>
81602924:	e59d3010 	ldr	r3, [sp, #16]
81602928:	e1a0200a 	mov	r2, sl
8160292c:	e59d1008 	ldr	r1, [sp, #8]
81602930:	e58d3000 	str	r3, [sp]
81602934:	e59f3948 	ldr	r3, [pc, #2376]	; 81603284 <do_fdt+0xdec>
81602938:	e5940000 	ldr	r0, [r4]
8160293c:	eb0034e3 	bl	8160fcd0 <fdt_setprop>
81602940:	e3500000 	cmp	r0, #0
81602944:	aaffff6d 	bge	81602700 <do_fdt+0x268>
81602948:	eb0035f4 	bl	81610120 <fdt_strerror>
8160294c:	e1a01000 	mov	r1, r0
81602950:	e59f093c 	ldr	r0, [pc, #2364]	; 81603294 <do_fdt+0xdfc>
81602954:	eaffff50 	b	8160269c <do_fdt+0x204>
81602958:	e59d0018 	ldr	r0, [sp, #24]
8160295c:	e2855001 	add	r5, r5, #1
81602960:	eb003bdb 	bl	816118d4 <strlen>
81602964:	e59d1018 	ldr	r1, [sp, #24]
81602968:	e280b001 	add	fp, r0, #1
8160296c:	e1a00007 	mov	r0, r7
81602970:	eb003ba3 	bl	81611804 <strcpy>
81602974:	e5b63004 	ldr	r3, [r6, #4]!
81602978:	e59d0010 	ldr	r0, [sp, #16]
8160297c:	e1580005 	cmp	r8, r5
81602980:	e087700b 	add	r7, r7, fp
81602984:	e080000b 	add	r0, r0, fp
81602988:	e58d3018 	str	r3, [sp, #24]
8160298c:	e58d0010 	str	r0, [sp, #16]
81602990:	cafffff0 	bgt	81602958 <do_fdt+0x4c0>
81602994:	eaffffe2 	b	81602924 <do_fdt+0x48c>
81602998:	e3530067 	cmp	r3, #103	; 0x67
8160299c:	1a000091 	bne	81602be8 <do_fdt+0x750>
816029a0:	e3580004 	cmp	r8, #4
816029a4:	e3a03000 	mov	r3, #0
816029a8:	e58d3010 	str	r3, [sp, #16]
816029ac:	dafffebd 	ble	816024a8 <do_fdt+0x10>
816029b0:	e3580005 	cmp	r8, #5
816029b4:	e5976008 	ldr	r6, [r7, #8]
816029b8:	1a000002 	bne	816029c8 <do_fdt+0x530>
816029bc:	e5d63000 	ldrb	r3, [r6]
816029c0:	e3530073 	cmp	r3, #115	; 0x73
816029c4:	1afffeb7 	bne	816024a8 <do_fdt+0x10>
816029c8:	e5971010 	ldr	r1, [r7, #16]
816029cc:	e5940000 	ldr	r0, [r4]
816029d0:	e597500c 	ldr	r5, [r7, #12]
816029d4:	e597a014 	ldr	sl, [r7, #20]
816029d8:	eb003357 	bl	8160f73c <fdt_path_offset>
816029dc:	e250b000 	subs	fp, r0, #0
816029e0:	baffff3d 	blt	816026dc <do_fdt+0x244>
816029e4:	e5d62000 	ldrb	r2, [r6]
816029e8:	e5940000 	ldr	r0, [r4]
816029ec:	e352006e 	cmp	r2, #110	; 0x6e
816029f0:	0a000002 	beq	81602a00 <do_fdt+0x568>
816029f4:	e3580005 	cmp	r8, #5
816029f8:	03520073 	cmpeq	r2, #115	; 0x73
816029fc:	1a000036 	bne	81602adc <do_fdt+0x644>
81602a00:	e1a0100b 	mov	r1, fp
81602a04:	e28d8048 	add	r8, sp, #72	; 0x48
81602a08:	eb00337e 	bl	8160f808 <fdt_node_depth>
81602a0c:	e1a0100b 	mov	r1, fp
81602a10:	e5280030 	str	r0, [r8, #-48]!	; 0xffffffd0
81602a14:	e1a0a000 	mov	sl, r0
81602a18:	e1a02008 	mov	r2, r8
81602a1c:	e5940000 	ldr	r0, [r4]
81602a20:	eb003167 	bl	8160efc4 <fdt_next_node>
81602a24:	e5d63000 	ldrb	r3, [r6]
81602a28:	e353006e 	cmp	r3, #110	; 0x6e
81602a2c:	13e03000 	mvnne	r3, #0
81602a30:	e1a0b000 	mov	fp, r0
81602a34:	1a000004 	bne	81602a4c <do_fdt+0x5b4>
81602a38:	e3a02010 	mov	r2, #16
81602a3c:	e3a01000 	mov	r1, #0
81602a40:	e5970014 	ldr	r0, [r7, #20]
81602a44:	eb003f53 	bl	81612798 <simple_strtoul>
81602a48:	e1a03000 	mov	r3, r0
81602a4c:	e3e07000 	mvn	r7, #0
81602a50:	e28a2001 	add	r2, sl, #1
81602a54:	e58d2008 	str	r2, [sp, #8]
81602a58:	e59d2018 	ldr	r2, [sp, #24]
81602a5c:	e152000a 	cmp	r2, sl
81602a60:	da000015 	ble	81602abc <do_fdt+0x624>
81602a64:	e59d1008 	ldr	r1, [sp, #8]
81602a68:	e5940000 	ldr	r0, [r4]
81602a6c:	e1510002 	cmp	r1, r2
81602a70:	e5d62000 	ldrb	r2, [r6]
81602a74:	02877001 	addeq	r7, r7, #1
81602a78:	e352006e 	cmp	r2, #110	; 0x6e
81602a7c:	01570003 	cmpeq	r7, r3
81602a80:	1a000006 	bne	81602aa0 <do_fdt+0x608>
81602a84:	e1a0100b 	mov	r1, fp
81602a88:	e3a02000 	mov	r2, #0
81602a8c:	eb003238 	bl	8160f374 <fdt_get_name>
81602a90:	e1a01000 	mov	r1, r0
81602a94:	e1a00005 	mov	r0, r5
81602a98:	eb0005b2 	bl	81604168 <env_set>
81602a9c:	eaffff17 	b	81602700 <do_fdt+0x268>
81602aa0:	e1a0100b 	mov	r1, fp
81602aa4:	e1a02008 	mov	r2, r8
81602aa8:	e58d300c 	str	r3, [sp, #12]
81602aac:	eb003144 	bl	8160efc4 <fdt_next_node>
81602ab0:	e59d300c 	ldr	r3, [sp, #12]
81602ab4:	e250b000 	subs	fp, r0, #0
81602ab8:	aaffffe6 	bge	81602a58 <do_fdt+0x5c0>
81602abc:	e5d63000 	ldrb	r3, [r6]
81602ac0:	e3530073 	cmp	r3, #115	; 0x73
81602ac4:	159f07cc 	ldrne	r0, [pc, #1996]	; 81603298 <do_fdt+0xe00>
81602ac8:	1a0000a2 	bne	81602d58 <do_fdt+0x8c0>
81602acc:	e2871001 	add	r1, r7, #1
81602ad0:	e1a00005 	mov	r0, r5
81602ad4:	eb0005bb 	bl	816041c8 <env_set_ulong>
81602ad8:	eaffff08 	b	81602700 <do_fdt+0x268>
81602adc:	e1a0200a 	mov	r2, sl
81602ae0:	e1a0100b 	mov	r1, fp
81602ae4:	e28d3010 	add	r3, sp, #16
81602ae8:	eb0032ba 	bl	8160f5d8 <fdt_getprop>
81602aec:	e59d4010 	ldr	r4, [sp, #16]
81602af0:	e3540000 	cmp	r4, #0
81602af4:	059f17a0 	ldreq	r1, [pc, #1952]	; 8160329c <do_fdt+0xe04>
81602af8:	e1a0a000 	mov	sl, r0
81602afc:	0affffe4 	beq	81602a94 <do_fdt+0x5fc>
81602b00:	c3a03001 	movgt	r3, #1
81602b04:	d3a03000 	movle	r3, #0
81602b08:	e3500000 	cmp	r0, #0
81602b0c:	03a03000 	moveq	r3, #0
81602b10:	e3530000 	cmp	r3, #0
81602b14:	0a00002e 	beq	81602bd4 <do_fdt+0x73c>
81602b18:	e5d63000 	ldrb	r3, [r6]
81602b1c:	e3530076 	cmp	r3, #118	; 0x76
81602b20:	1a00001f 	bne	81602ba4 <do_fdt+0x70c>
81602b24:	e1a01004 	mov	r1, r4
81602b28:	ebfffdd5 	bl	81602284 <is_printable_string>
81602b2c:	e2506000 	subs	r6, r0, #0
81602b30:	11a0100a 	movne	r1, sl
81602b34:	1affffd6 	bne	81602a94 <do_fdt+0x5fc>
81602b38:	e3540004 	cmp	r4, #4
81602b3c:	1a000008 	bne	81602b64 <do_fdt+0x6cc>
81602b40:	e59a2000 	ldr	r2, [sl]
81602b44:	e28d0018 	add	r0, sp, #24
81602b48:	e59f1750 	ldr	r1, [pc, #1872]	; 816032a0 <do_fdt+0xe08>
81602b4c:	e6bf2f32 	rev	r2, r2
81602b50:	eb003ebf 	bl	81612654 <sprintf>
81602b54:	e28d1018 	add	r1, sp, #24
81602b58:	e1a00005 	mov	r0, r5
81602b5c:	eb000581 	bl	81604168 <env_set>
81602b60:	eafffe6d 	b	8160251c <do_fdt+0x84>
81602b64:	e7e13054 	ubfx	r3, r4, #0, #2
81602b68:	e3540014 	cmp	r4, #20
81602b6c:	d3530000 	cmple	r3, #0
81602b70:	159f072c 	ldrne	r0, [pc, #1836]	; 816032a4 <do_fdt+0xe0c>
81602b74:	1a000077 	bne	81602d58 <do_fdt+0x8c0>
81602b78:	e59f8728 	ldr	r8, [pc, #1832]	; 816032a8 <do_fdt+0xe10>
81602b7c:	e1a07006 	mov	r7, r6
81602b80:	e28d3018 	add	r3, sp, #24
81602b84:	e79a2007 	ldr	r2, [sl, r7]
81602b88:	e0830087 	add	r0, r3, r7, lsl #1
81602b8c:	e1a01008 	mov	r1, r8
81602b90:	e2877004 	add	r7, r7, #4
81602b94:	eb003eae 	bl	81612654 <sprintf>
81602b98:	e1540007 	cmp	r4, r7
81602b9c:	cafffff7 	bgt	81602b80 <do_fdt+0x6e8>
81602ba0:	eaffffeb 	b	81602b54 <do_fdt+0x6bc>
81602ba4:	e3530061 	cmp	r3, #97	; 0x61
81602ba8:	01a02000 	moveq	r2, r0
81602bac:	059f16f8 	ldreq	r1, [pc, #1784]	; 816032ac <do_fdt+0xe14>
81602bb0:	0a000003 	beq	81602bc4 <do_fdt+0x72c>
81602bb4:	e3530073 	cmp	r3, #115	; 0x73
81602bb8:	1afffe3a 	bne	816024a8 <do_fdt+0x10>
81602bbc:	e59f16dc 	ldr	r1, [pc, #1756]	; 816032a0 <do_fdt+0xe08>
81602bc0:	e1a02004 	mov	r2, r4
81602bc4:	e28d0018 	add	r0, sp, #24
81602bc8:	eb003ea1 	bl	81612654 <sprintf>
81602bcc:	e28d1018 	add	r1, sp, #24
81602bd0:	eaffffaf 	b	81602a94 <do_fdt+0x5fc>
81602bd4:	e1a00004 	mov	r0, r4
81602bd8:	eb003550 	bl	81610120 <fdt_strerror>
81602bdc:	e1a01000 	mov	r1, r0
81602be0:	e59f06c8 	ldr	r0, [pc, #1736]	; 816032b0 <do_fdt+0xe18>
81602be4:	eafffeac 	b	8160269c <do_fdt+0x204>
81602be8:	e243506c 	sub	r5, r3, #108	; 0x6c
81602bec:	e21550fb 	ands	r5, r5, #251	; 0xfb
81602bf0:	1a000093 	bne	81602e44 <do_fdt+0x9ac>
81602bf4:	e353006c 	cmp	r3, #108	; 0x6c
81602bf8:	03a06001 	moveq	r6, #1
81602bfc:	13a06020 	movne	r6, #32
81602c00:	e3580002 	cmp	r8, #2
81602c04:	059fa6a8 	ldreq	sl, [pc, #1704]	; 816032b4 <do_fdt+0xe1c>
81602c08:	0a000003 	beq	81602c1c <do_fdt+0x784>
81602c0c:	e3580003 	cmp	r8, #3
81602c10:	e597a008 	ldr	sl, [r7, #8]
81602c14:	1597500c 	ldrne	r5, [r7, #12]
81602c18:	03a05000 	moveq	r5, #0
81602c1c:	e1a0100a 	mov	r1, sl
81602c20:	e5940000 	ldr	r0, [r4]
81602c24:	eb0032c4 	bl	8160f73c <fdt_path_offset>
81602c28:	e2507000 	subs	r7, r0, #0
81602c2c:	bafffeaa 	blt	816026dc <do_fdt+0x244>
81602c30:	e3550000 	cmp	r5, #0
81602c34:	e5940000 	ldr	r0, [r4]
81602c38:	0a00001c 	beq	81602cb0 <do_fdt+0x818>
81602c3c:	e28d3018 	add	r3, sp, #24
81602c40:	e1a02005 	mov	r2, r5
81602c44:	e1a01007 	mov	r1, r7
81602c48:	eb003262 	bl	8160f5d8 <fdt_getprop>
81602c4c:	e59d6018 	ldr	r6, [sp, #24]
81602c50:	e3560000 	cmp	r6, #0
81602c54:	e1a04000 	mov	r4, r0
81602c58:	1a000004 	bne	81602c70 <do_fdt+0x7d8>
81602c5c:	e59f0654 	ldr	r0, [pc, #1620]	; 816032b8 <do_fdt+0xe20>
81602c60:	e1a02005 	mov	r2, r5
81602c64:	e1a0100a 	mov	r1, sl
81602c68:	eb003e83 	bl	8161267c <printf>
81602c6c:	eafffe2a 	b	8160251c <do_fdt+0x84>
81602c70:	c3a03001 	movgt	r3, #1
81602c74:	d3a03000 	movle	r3, #0
81602c78:	e3500000 	cmp	r0, #0
81602c7c:	03a03000 	moveq	r3, #0
81602c80:	e3530000 	cmp	r3, #0
81602c84:	01a00006 	moveq	r0, r6
81602c88:	0affffd2 	beq	81602bd8 <do_fdt+0x740>
81602c8c:	e1a01005 	mov	r1, r5
81602c90:	e59f0624 	ldr	r0, [pc, #1572]	; 816032bc <do_fdt+0xe24>
81602c94:	eb003e78 	bl	8161267c <printf>
81602c98:	e59d1018 	ldr	r1, [sp, #24]
81602c9c:	e1a00004 	mov	r0, r4
81602ca0:	ebfffd99 	bl	8160230c <print_data>
81602ca4:	e59f0614 	ldr	r0, [pc, #1556]	; 816032c0 <do_fdt+0xe28>
81602ca8:	eb003e73 	bl	8161267c <printf>
81602cac:	eafffe93 	b	81602700 <do_fdt+0x268>
81602cb0:	e59f860c 	ldr	r8, [pc, #1548]	; 816032c4 <do_fdt+0xe2c>
81602cb4:	e59fa60c 	ldr	sl, [pc, #1548]	; 816032c8 <do_fdt+0xe30>
81602cb8:	e1a01007 	mov	r1, r7
81602cbc:	e28d2010 	add	r2, sp, #16
81602cc0:	e5940000 	ldr	r0, [r4]
81602cc4:	eb003061 	bl	8160ee50 <fdt_next_tag>
81602cc8:	e2403001 	sub	r3, r0, #1
81602ccc:	e1a01000 	mov	r1, r0
81602cd0:	e3530008 	cmp	r3, #8
81602cd4:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
81602cd8:	ea000055 	b	81602e34 <do_fdt+0x99c>
81602cdc:	81602d00 	.word	0x81602d00
81602ce0:	81602d60 	.word	0x81602d60
81602ce4:	81602d94 	.word	0x81602d94
81602ce8:	81602e20 	.word	0x81602e20
81602cec:	81602e34 	.word	0x81602e34
81602cf0:	81602e34 	.word	0x81602e34
81602cf4:	81602e34 	.word	0x81602e34
81602cf8:	81602e34 	.word	0x81602e34
81602cfc:	816024ec 	.word	0x816024ec
81602d00:	e3a02000 	mov	r2, #0
81602d04:	e1a01007 	mov	r1, r7
81602d08:	e5940000 	ldr	r0, [r4]
81602d0c:	eb003198 	bl	8160f374 <fdt_get_name>
81602d10:	e1560005 	cmp	r6, r5
81602d14:	ba00000b 	blt	81602d48 <do_fdt+0x8b0>
81602d18:	e59f35ac 	ldr	r3, [pc, #1452]	; 816032cc <do_fdt+0xe34>
81602d1c:	e3500000 	cmp	r0, #0
81602d20:	e2651020 	rsb	r1, r5, #32
81602d24:	e0881001 	add	r1, r8, r1
81602d28:	01a00003 	moveq	r0, r3
81602d2c:	e59f359c 	ldr	r3, [pc, #1436]	; 816032d0 <do_fdt+0xe38>
81602d30:	e5d02000 	ldrb	r2, [r0]
81602d34:	e3520000 	cmp	r2, #0
81602d38:	11a02000 	movne	r2, r0
81602d3c:	01a02003 	moveq	r2, r3
81602d40:	e59f058c 	ldr	r0, [pc, #1420]	; 816032d4 <do_fdt+0xe3c>
81602d44:	eb003e4c 	bl	8161267c <printf>
81602d48:	e2855001 	add	r5, r5, #1
81602d4c:	e355001f 	cmp	r5, #31
81602d50:	da00000b 	ble	81602d84 <do_fdt+0x8ec>
81602d54:	e59f057c 	ldr	r0, [pc, #1404]	; 816032d8 <do_fdt+0xe40>
81602d58:	eb003e47 	bl	8161267c <printf>
81602d5c:	eafffde2 	b	816024ec <do_fdt+0x54>
81602d60:	e2455001 	sub	r5, r5, #1
81602d64:	e1560005 	cmp	r6, r5
81602d68:	ba000005 	blt	81602d84 <do_fdt+0x8ec>
81602d6c:	e2651020 	rsb	r1, r5, #32
81602d70:	e59f0564 	ldr	r0, [pc, #1380]	; 816032dc <do_fdt+0xe44>
81602d74:	e0881001 	add	r1, r8, r1
81602d78:	eb003e3f 	bl	8161267c <printf>
81602d7c:	e3550000 	cmp	r5, #0
81602d80:	0afffe5e 	beq	81602700 <do_fdt+0x268>
81602d84:	e3750001 	cmn	r5, #1
81602d88:	e59d7010 	ldr	r7, [sp, #16]
81602d8c:	1affffc9 	bne	81602cb8 <do_fdt+0x820>
81602d90:	eafffe5a 	b	81602700 <do_fdt+0x268>
81602d94:	e3a0200c 	mov	r2, #12
81602d98:	e1a01007 	mov	r1, r7
81602d9c:	e5940000 	ldr	r0, [r4]
81602da0:	eb00300f 	bl	8160ede4 <fdt_offset_ptr>
81602da4:	e5901008 	ldr	r1, [r0, #8]
81602da8:	e1a07000 	mov	r7, r0
81602dac:	e5940000 	ldr	r0, [r4]
81602db0:	e6bf1f31 	rev	r1, r1
81602db4:	eb003101 	bl	8160f1c0 <fdt_string>
81602db8:	e5973004 	ldr	r3, [r7, #4]
81602dbc:	e1a02000 	mov	r2, r0
81602dc0:	e6bf0f33 	rev	r0, r3
81602dc4:	e3500000 	cmp	r0, #0
81602dc8:	e58d0018 	str	r0, [sp, #24]
81602dcc:	baffff81 	blt	81602bd8 <do_fdt+0x740>
81602dd0:	1a000006 	bne	81602df0 <do_fdt+0x958>
81602dd4:	e1560005 	cmp	r6, r5
81602dd8:	baffffe9 	blt	81602d84 <do_fdt+0x8ec>
81602ddc:	e2651020 	rsb	r1, r5, #32
81602de0:	e59f04f8 	ldr	r0, [pc, #1272]	; 816032e0 <do_fdt+0xe48>
81602de4:	e0881001 	add	r1, r8, r1
81602de8:	eb003e23 	bl	8161267c <printf>
81602dec:	eaffffe4 	b	81602d84 <do_fdt+0x8ec>
81602df0:	e1560005 	cmp	r6, r5
81602df4:	baffffe2 	blt	81602d84 <do_fdt+0x8ec>
81602df8:	e2651020 	rsb	r1, r5, #32
81602dfc:	e59f04e0 	ldr	r0, [pc, #1248]	; 816032e4 <do_fdt+0xe4c>
81602e00:	e0881001 	add	r1, r8, r1
81602e04:	eb003e1c 	bl	8161267c <printf>
81602e08:	e287000c 	add	r0, r7, #12
81602e0c:	e59d1018 	ldr	r1, [sp, #24]
81602e10:	ebfffd3d 	bl	8160230c <print_data>
81602e14:	e59f04cc 	ldr	r0, [pc, #1228]	; 816032e8 <do_fdt+0xe50>
81602e18:	eb003e17 	bl	8161267c <printf>
81602e1c:	eaffffd8 	b	81602d84 <do_fdt+0x8ec>
81602e20:	e2651020 	rsb	r1, r5, #32
81602e24:	e1a0000a 	mov	r0, sl
81602e28:	e0881001 	add	r1, r8, r1
81602e2c:	eb003e12 	bl	8161267c <printf>
81602e30:	eaffffd3 	b	81602d84 <do_fdt+0x8ec>
81602e34:	e1560005 	cmp	r6, r5
81602e38:	bafffdab 	blt	816024ec <do_fdt+0x54>
81602e3c:	e59f04a8 	ldr	r0, [pc, #1192]	; 816032ec <do_fdt+0xe54>
81602e40:	eafffe15 	b	8160269c <do_fdt+0x204>
81602e44:	e3a02002 	mov	r2, #2
81602e48:	e59f14a0 	ldr	r1, [pc, #1184]	; 816032f0 <do_fdt+0xe58>
81602e4c:	eb003a85 	bl	81611868 <strncmp>
81602e50:	e3500000 	cmp	r0, #0
81602e54:	1a000017 	bne	81602eb8 <do_fdt+0xa20>
81602e58:	e5971008 	ldr	r1, [r7, #8]
81602e5c:	e5940000 	ldr	r0, [r4]
81602e60:	eb003235 	bl	8160f73c <fdt_path_offset>
81602e64:	e2501000 	subs	r1, r0, #0
81602e68:	b1a00001 	movlt	r0, r1
81602e6c:	bafffe1a 	blt	816026dc <do_fdt+0x244>
81602e70:	e3580003 	cmp	r8, #3
81602e74:	e5940000 	ldr	r0, [r4]
81602e78:	da000007 	ble	81602e9c <do_fdt+0xa04>
81602e7c:	e597200c 	ldr	r2, [r7, #12]
81602e80:	eb0033cc 	bl	8160fdb8 <fdt_delprop>
81602e84:	e2506000 	subs	r6, r0, #0
81602e88:	aafffe1c 	bge	81602700 <do_fdt+0x268>
81602e8c:	eb0034a3 	bl	81610120 <fdt_strerror>
81602e90:	e1a01000 	mov	r1, r0
81602e94:	e59f0458 	ldr	r0, [pc, #1112]	; 816032f4 <do_fdt+0xe5c>
81602e98:	eafffd9e 	b	81602518 <do_fdt+0x80>
81602e9c:	eb003427 	bl	8160ff40 <fdt_del_node>
81602ea0:	e2506000 	subs	r6, r0, #0
81602ea4:	aafffe15 	bge	81602700 <do_fdt+0x268>
81602ea8:	eb00349c 	bl	81610120 <fdt_strerror>
81602eac:	e1a01000 	mov	r1, r0
81602eb0:	e59f0440 	ldr	r0, [pc, #1088]	; 816032f8 <do_fdt+0xe60>
81602eb4:	eafffd97 	b	81602518 <do_fdt+0x80>
81602eb8:	e5970004 	ldr	r0, [r7, #4]
81602ebc:	e5d03000 	ldrb	r3, [r0]
81602ec0:	e3530068 	cmp	r3, #104	; 0x68
81602ec4:	1a00003e 	bne	81602fc4 <do_fdt+0xb2c>
81602ec8:	e5943000 	ldr	r3, [r4]
81602ecc:	e59f0428 	ldr	r0, [pc, #1064]	; 816032fc <do_fdt+0xe64>
81602ed0:	e5931000 	ldr	r1, [r3]
81602ed4:	e5935014 	ldr	r5, [r3, #20]
81602ed8:	e6bf1f31 	rev	r1, r1
81602edc:	eb003de6 	bl	8161267c <printf>
81602ee0:	e5943000 	ldr	r3, [r4]
81602ee4:	e59f0414 	ldr	r0, [pc, #1044]	; 81603300 <do_fdt+0xe68>
81602ee8:	e6bf5f35 	rev	r5, r5
81602eec:	e5932004 	ldr	r2, [r3, #4]
81602ef0:	e6bf2f32 	rev	r2, r2
81602ef4:	e1a01002 	mov	r1, r2
81602ef8:	eb003ddf 	bl	8161267c <printf>
81602efc:	e5943000 	ldr	r3, [r4]
81602f00:	e59f03fc 	ldr	r0, [pc, #1020]	; 81603304 <do_fdt+0xe6c>
81602f04:	e5931008 	ldr	r1, [r3, #8]
81602f08:	e6bf1f31 	rev	r1, r1
81602f0c:	eb003dda 	bl	8161267c <printf>
81602f10:	e5943000 	ldr	r3, [r4]
81602f14:	e59f03ec 	ldr	r0, [pc, #1004]	; 81603308 <do_fdt+0xe70>
81602f18:	e593100c 	ldr	r1, [r3, #12]
81602f1c:	e6bf1f31 	rev	r1, r1
81602f20:	eb003dd5 	bl	8161267c <printf>
81602f24:	e5943000 	ldr	r3, [r4]
81602f28:	e59f03dc 	ldr	r0, [pc, #988]	; 8160330c <do_fdt+0xe74>
81602f2c:	e5931010 	ldr	r1, [r3, #16]
81602f30:	e6bf1f31 	rev	r1, r1
81602f34:	eb003dd0 	bl	8161267c <printf>
81602f38:	e1a01005 	mov	r1, r5
81602f3c:	e59f03cc 	ldr	r0, [pc, #972]	; 81603310 <do_fdt+0xe78>
81602f40:	eb003dcd 	bl	8161267c <printf>
81602f44:	e5943000 	ldr	r3, [r4]
81602f48:	e59f03c4 	ldr	r0, [pc, #964]	; 81603314 <do_fdt+0xe7c>
81602f4c:	e5931018 	ldr	r1, [r3, #24]
81602f50:	e6bf1f31 	rev	r1, r1
81602f54:	eb003dc8 	bl	8161267c <printf>
81602f58:	e3550001 	cmp	r5, #1
81602f5c:	9a000012 	bls	81602fac <do_fdt+0xb14>
81602f60:	e5943000 	ldr	r3, [r4]
81602f64:	e59f03ac 	ldr	r0, [pc, #940]	; 81603318 <do_fdt+0xe80>
81602f68:	e593101c 	ldr	r1, [r3, #28]
81602f6c:	e6bf1f31 	rev	r1, r1
81602f70:	eb003dc1 	bl	8161267c <printf>
81602f74:	e3550002 	cmp	r5, #2
81602f78:	0a00000b 	beq	81602fac <do_fdt+0xb14>
81602f7c:	e5943000 	ldr	r3, [r4]
81602f80:	e59f0394 	ldr	r0, [pc, #916]	; 8160331c <do_fdt+0xe84>
81602f84:	e5931020 	ldr	r1, [r3, #32]
81602f88:	e6bf1f31 	rev	r1, r1
81602f8c:	eb003dba 	bl	8161267c <printf>
81602f90:	e3550010 	cmp	r5, #16
81602f94:	9a000004 	bls	81602fac <do_fdt+0xb14>
81602f98:	e5943000 	ldr	r3, [r4]
81602f9c:	e59f037c 	ldr	r0, [pc, #892]	; 81603320 <do_fdt+0xe88>
81602fa0:	e5931024 	ldr	r1, [r3, #36]	; 0x24
81602fa4:	e6bf1f31 	rev	r1, r1
81602fa8:	eb003db3 	bl	8161267c <printf>
81602fac:	e5940000 	ldr	r0, [r4]
81602fb0:	eb00309f 	bl	8160f234 <fdt_num_mem_rsv>
81602fb4:	e1a01000 	mov	r1, r0
81602fb8:	e59f0364 	ldr	r0, [pc, #868]	; 81603324 <do_fdt+0xe8c>
81602fbc:	eb003dae 	bl	8161267c <printf>
81602fc0:	eaffff37 	b	81602ca4 <do_fdt+0x80c>
81602fc4:	e3a02003 	mov	r2, #3
81602fc8:	e59f1358 	ldr	r1, [pc, #856]	; 81603328 <do_fdt+0xe90>
81602fcc:	eb003a25 	bl	81611868 <strncmp>
81602fd0:	e2506000 	subs	r6, r0, #0
81602fd4:	1a000007 	bne	81602ff8 <do_fdt+0xb60>
81602fd8:	e3a02010 	mov	r2, #16
81602fdc:	e1a01006 	mov	r1, r6
81602fe0:	e5970008 	ldr	r0, [r7, #8]
81602fe4:	eb003deb 	bl	81612798 <simple_strtoul>
81602fe8:	e5943000 	ldr	r3, [r4]
81602fec:	e6bf0f30 	rev	r0, r0
81602ff0:	e583001c 	str	r0, [r3, #28]
81602ff4:	eafffd48 	b	8160251c <do_fdt+0x84>
81602ff8:	e3a02002 	mov	r2, #2
81602ffc:	e59f1328 	ldr	r1, [pc, #808]	; 8160332c <do_fdt+0xe94>
81603000:	e5970004 	ldr	r0, [r7, #4]
81603004:	eb003a17 	bl	81611868 <strncmp>
81603008:	e2505000 	subs	r5, r0, #0
8160300c:	1a000010 	bne	81603054 <do_fdt+0xbbc>
81603010:	e3a02010 	mov	r2, #16
81603014:	e1a01005 	mov	r1, r5
81603018:	e5970008 	ldr	r0, [r7, #8]
8160301c:	eb003e4b 	bl	81612950 <simple_strtoull>
81603020:	e3a02010 	mov	r2, #16
81603024:	e1a0a000 	mov	sl, r0
81603028:	e1a0b001 	mov	fp, r1
8160302c:	e597000c 	ldr	r0, [r7, #12]
81603030:	e1a01005 	mov	r1, r5
81603034:	eb003e45 	bl	81612950 <simple_strtoull>
81603038:	e1a0200a 	mov	r2, sl
8160303c:	e1a0300b 	mov	r3, fp
81603040:	e1cd00f0 	strd	r0, [sp]
81603044:	e5940000 	ldr	r0, [r4]
81603048:	eb000c82 	bl	81606258 <fdt_fixup_memory>
8160304c:	e0006fc0 	and	r6, r0, r0, asr #31
81603050:	eafffd31 	b	8160251c <do_fdt+0x84>
81603054:	e3a02002 	mov	r2, #2
81603058:	e59f12d0 	ldr	r1, [pc, #720]	; 81603330 <do_fdt+0xe98>
8160305c:	e5970004 	ldr	r0, [r7, #4]
81603060:	eb003a00 	bl	81611868 <strncmp>
81603064:	e2505000 	subs	r5, r0, #0
81603068:	1a000049 	bne	81603194 <do_fdt+0xcfc>
8160306c:	e5973008 	ldr	r3, [r7, #8]
81603070:	e5d33000 	ldrb	r3, [r3]
81603074:	e3530070 	cmp	r3, #112	; 0x70
81603078:	1a00001f 	bne	816030fc <do_fdt+0xc64>
8160307c:	e5940000 	ldr	r0, [r4]
81603080:	eb00306b 	bl	8160f234 <fdt_num_mem_rsv>
81603084:	e59f82a8 	ldr	r8, [pc, #680]	; 81603334 <do_fdt+0xe9c>
81603088:	e1a07000 	mov	r7, r0
8160308c:	e59f02a4 	ldr	r0, [pc, #676]	; 81603338 <do_fdt+0xea0>
81603090:	eb003d79 	bl	8161267c <printf>
81603094:	e59f02a0 	ldr	r0, [pc, #672]	; 8160333c <do_fdt+0xea4>
81603098:	eb003d77 	bl	8161267c <printf>
8160309c:	e1550007 	cmp	r5, r7
816030a0:	aafffd96 	bge	81602700 <do_fdt+0x268>
816030a4:	e28d3018 	add	r3, sp, #24
816030a8:	e28d2010 	add	r2, sp, #16
816030ac:	e1a01005 	mov	r1, r5
816030b0:	e5940000 	ldr	r0, [r4]
816030b4:	eb003046 	bl	8160f1d4 <fdt_get_mem_rsv>
816030b8:	e2506000 	subs	r6, r0, #0
816030bc:	aa000003 	bge	816030d0 <do_fdt+0xc38>
816030c0:	eb003416 	bl	81610120 <fdt_strerror>
816030c4:	e1a01000 	mov	r1, r0
816030c8:	e59f0270 	ldr	r0, [pc, #624]	; 81603340 <do_fdt+0xea8>
816030cc:	eafffd11 	b	81602518 <do_fdt+0x80>
816030d0:	e59d3018 	ldr	r3, [sp, #24]
816030d4:	e1a01005 	mov	r1, r5
816030d8:	e59d2014 	ldr	r2, [sp, #20]
816030dc:	e1a00008 	mov	r0, r8
816030e0:	e2855001 	add	r5, r5, #1
816030e4:	e58d3004 	str	r3, [sp, #4]
816030e8:	e59d301c 	ldr	r3, [sp, #28]
816030ec:	e58d3000 	str	r3, [sp]
816030f0:	e59d3010 	ldr	r3, [sp, #16]
816030f4:	eb003d60 	bl	8161267c <printf>
816030f8:	eaffffe7 	b	8160309c <do_fdt+0xc04>
816030fc:	e3530061 	cmp	r3, #97	; 0x61
81603100:	1a000014 	bne	81603158 <do_fdt+0xcc0>
81603104:	e3a02010 	mov	r2, #16
81603108:	e1a01005 	mov	r1, r5
8160310c:	e597000c 	ldr	r0, [r7, #12]
81603110:	eb003e0e 	bl	81612950 <simple_strtoull>
81603114:	e3a02010 	mov	r2, #16
81603118:	e1a0a000 	mov	sl, r0
8160311c:	e1a0b001 	mov	fp, r1
81603120:	e5970010 	ldr	r0, [r7, #16]
81603124:	e1a01005 	mov	r1, r5
81603128:	eb003e08 	bl	81612950 <simple_strtoull>
8160312c:	e1a0200a 	mov	r2, sl
81603130:	e1a0300b 	mov	r3, fp
81603134:	e1cd00f0 	strd	r0, [sp]
81603138:	e5940000 	ldr	r0, [r4]
8160313c:	eb0032ab 	bl	8160fbf0 <fdt_add_mem_rsv>
81603140:	e2506000 	subs	r6, r0, #0
81603144:	aafffd6d 	bge	81602700 <do_fdt+0x268>
81603148:	eb0033f4 	bl	81610120 <fdt_strerror>
8160314c:	e1a01000 	mov	r1, r0
81603150:	e59f01ec 	ldr	r0, [pc, #492]	; 81603344 <do_fdt+0xeac>
81603154:	eafffcef 	b	81602518 <do_fdt+0x80>
81603158:	e3530064 	cmp	r3, #100	; 0x64
8160315c:	1afffcd1 	bne	816024a8 <do_fdt+0x10>
81603160:	e3a02010 	mov	r2, #16
81603164:	e1a01005 	mov	r1, r5
81603168:	e597000c 	ldr	r0, [r7, #12]
8160316c:	eb003d89 	bl	81612798 <simple_strtoul>
81603170:	e1a01000 	mov	r1, r0
81603174:	e5940000 	ldr	r0, [r4]
81603178:	eb0032be 	bl	8160fc78 <fdt_del_mem_rsv>
8160317c:	e2506000 	subs	r6, r0, #0
81603180:	aafffd5e 	bge	81602700 <do_fdt+0x268>
81603184:	eb0033e5 	bl	81610120 <fdt_strerror>
81603188:	e1a01000 	mov	r1, r0
8160318c:	e59f01b4 	ldr	r0, [pc, #436]	; 81603348 <do_fdt+0xeb0>
81603190:	eafffce0 	b	81602518 <do_fdt+0x80>
81603194:	e3a02003 	mov	r2, #3
81603198:	e59f11ac 	ldr	r1, [pc, #428]	; 8160334c <do_fdt+0xeb4>
8160319c:	e5970004 	ldr	r0, [r7, #4]
816031a0:	eb0039b0 	bl	81611868 <strncmp>
816031a4:	e2506000 	subs	r6, r0, #0
816031a8:	1a000017 	bne	8160320c <do_fdt+0xd74>
816031ac:	e2483002 	sub	r3, r8, #2
816031b0:	e3d33002 	bics	r3, r3, #2
816031b4:	1afffcbb 	bne	816024a8 <do_fdt+0x10>
816031b8:	e3580004 	cmp	r8, #4
816031bc:	11a07006 	movne	r7, r6
816031c0:	11a05006 	movne	r5, r6
816031c4:	1a000009 	bne	816031f0 <do_fdt+0xd58>
816031c8:	e3a02010 	mov	r2, #16
816031cc:	e1a01006 	mov	r1, r6
816031d0:	e5970008 	ldr	r0, [r7, #8]
816031d4:	eb003d6f 	bl	81612798 <simple_strtoul>
816031d8:	e3a02010 	mov	r2, #16
816031dc:	e1a01006 	mov	r1, r6
816031e0:	e1a05000 	mov	r5, r0
816031e4:	e597000c 	ldr	r0, [r7, #12]
816031e8:	eb003d6a 	bl	81612798 <simple_strtoul>
816031ec:	e1a07000 	mov	r7, r0
816031f0:	e5940000 	ldr	r0, [r4]
816031f4:	eb000b62 	bl	81605f84 <fdt_chosen>
816031f8:	e1a02007 	mov	r2, r7
816031fc:	e1a01005 	mov	r1, r5
81603200:	e5940000 	ldr	r0, [r4]
81603204:	eb000aea 	bl	81605db4 <fdt_initrd>
81603208:	eafffcc3 	b	8160251c <do_fdt+0x84>
8160320c:	e3a02002 	mov	r2, #2
81603210:	e59f1138 	ldr	r1, [pc, #312]	; 81603350 <do_fdt+0xeb8>
81603214:	e5970004 	ldr	r0, [r7, #4]
81603218:	eb003992 	bl	81611868 <strncmp>
8160321c:	e2506000 	subs	r6, r0, #0
81603220:	1afffca0 	bne	816024a8 <do_fdt+0x10>
81603224:	e3580002 	cmp	r8, #2
81603228:	01a01006 	moveq	r1, r6
8160322c:	0a000004 	beq	81603244 <do_fdt+0xdac>
81603230:	e1a01006 	mov	r1, r6
81603234:	e3a02010 	mov	r2, #16
81603238:	e5970008 	ldr	r0, [r7, #8]
8160323c:	eb003d55 	bl	81612798 <simple_strtoul>
81603240:	e1a01000 	mov	r1, r0
81603244:	e5940000 	ldr	r0, [r4]
81603248:	eb000c78 	bl	81606430 <fdt_shrink_to_minimum>
8160324c:	eafffcb2 	b	8160251c <do_fdt+0x84>
81603250:	81617496 	.word	0x81617496
81603254:	81619860 	.word	0x81619860
81603258:	8161675e 	.word	0x8161675e
8160325c:	816152fe 	.word	0x816152fe
81603260:	81615320 	.word	0x81615320
81603264:	8161534f 	.word	0x8161534f
81603268:	8161536b 	.word	0x8161536b
8160326c:	816153db 	.word	0x816153db
81603270:	816153de 	.word	0x816153de
81603274:	81615411 	.word	0x81615411
81603278:	81615414 	.word	0x81615414
8160327c:	8161543a 	.word	0x8161543a
81603280:	81615458 	.word	0x81615458
81603284:	81619460 	.word	0x81619460
81603288:	8161549f 	.word	0x8161549f
8160328c:	8161547e 	.word	0x8161547e
81603290:	816140e1 	.word	0x816140e1
81603294:	816154ba 	.word	0x816154ba
81603298:	816154d4 	.word	0x816154d4
8160329c:	8161623b 	.word	0x8161623b
816032a0:	816154eb 	.word	0x816154eb
816032a4:	816154f2 	.word	0x816154f2
816032a8:	8161654f 	.word	0x8161654f
816032ac:	8161550c 	.word	0x8161550c
816032b0:	81615511 	.word	0x81615511
816032b4:	81618958 	.word	0x81618958
816032b8:	8161552b 	.word	0x8161552b
816032bc:	81615565 	.word	0x81615565
816032c0:	8161623a 	.word	0x8161623a
816032c4:	8161895a 	.word	0x8161895a
816032c8:	8161556b 	.word	0x8161556b
816032cc:	816152e5 	.word	0x816152e5
816032d0:	816152fc 	.word	0x816152fc
816032d4:	81615532 	.word	0x81615532
816032d8:	8161553a 	.word	0x8161553a
816032dc:	81615556 	.word	0x81615556
816032e0:	8161555c 	.word	0x8161555c
816032e4:	81615563 	.word	0x81615563
816032e8:	81615560 	.word	0x81615560
816032ec:	81615578 	.word	0x81615578
816032f0:	816177f7 	.word	0x816177f7
816032f4:	8161558c 	.word	0x8161558c
816032f8:	816155a7 	.word	0x816155a7
816032fc:	816155c3 	.word	0x816155c3
81603300:	816155d2 	.word	0x816155d2
81603304:	816155e9 	.word	0x816155e9
81603308:	816155ff 	.word	0x816155ff
8160330c:	81615616 	.word	0x81615616
81603310:	8161562d 	.word	0x8161562d
81603314:	8161563b 	.word	0x8161563b
81603318:	81615652 	.word	0x81615652
8160331c:	81615669 	.word	0x81615669
81603320:	81615680 	.word	0x81615680
81603324:	81615697 	.word	0x81615697
81603328:	816156ae 	.word	0x816156ae
8160332c:	81614fe9 	.word	0x81614fe9
81603330:	816156b2 	.word	0x816156b2
81603334:	81615721 	.word	0x81615721
81603338:	816156b5 	.word	0x816156b5
8160333c:	816156d0 	.word	0x816156d0
81603340:	81615702 	.word	0x81615702
81603344:	8161573b 	.word	0x8161573b
81603348:	8161575a 	.word	0x8161575a
8160334c:	81616d6e 	.word	0x81616d6e
81603350:	81617012 	.word	0x81617012

81603354 <do_mem_mw>:
81603354:	e2421003 	sub	r1, r2, #3
81603358:	e3510001 	cmp	r1, #1
8160335c:	8a000027 	bhi	81603400 <do_mem_mw+0xac>
81603360:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81603364:	e3a01004 	mov	r1, #4
81603368:	e5930000 	ldr	r0, [r3]
8160336c:	e1a05003 	mov	r5, r3
81603370:	e1a06002 	mov	r6, r2
81603374:	eb00189e 	bl	816095f4 <cmd_get_data_size>
81603378:	e2507000 	subs	r7, r0, #0
8160337c:	da000021 	ble	81603408 <do_mem_mw+0xb4>
81603380:	e3a02010 	mov	r2, #16
81603384:	e3a01000 	mov	r1, #0
81603388:	e5950004 	ldr	r0, [r5, #4]
8160338c:	eb003d01 	bl	81612798 <simple_strtoul>
81603390:	e59f3078 	ldr	r3, [pc, #120]	; 81603410 <do_mem_mw+0xbc>
81603394:	e3a02010 	mov	r2, #16
81603398:	e3a01000 	mov	r1, #0
8160339c:	e5934000 	ldr	r4, [r3]
816033a0:	e0804004 	add	r4, r0, r4
816033a4:	e5950008 	ldr	r0, [r5, #8]
816033a8:	eb003cfa 	bl	81612798 <simple_strtoul>
816033ac:	e3560004 	cmp	r6, #4
816033b0:	e1a08000 	mov	r8, r0
816033b4:	13a00001 	movne	r0, #1
816033b8:	1a000003 	bne	816033cc <do_mem_mw+0x78>
816033bc:	e3a02010 	mov	r2, #16
816033c0:	e3a01000 	mov	r1, #0
816033c4:	e595000c 	ldr	r0, [r5, #12]
816033c8:	eb003cf2 	bl	81612798 <simple_strtoul>
816033cc:	e1a03004 	mov	r3, r4
816033d0:	e2500001 	subs	r0, r0, #1
816033d4:	2a000001 	bcs	816033e0 <do_mem_mw+0x8c>
816033d8:	e3a00000 	mov	r0, #0
816033dc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
816033e0:	e3570004 	cmp	r7, #4
816033e4:	05838000 	streq	r8, [r3]
816033e8:	0a000002 	beq	816033f8 <do_mem_mw+0xa4>
816033ec:	e3570002 	cmp	r7, #2
816033f0:	01c380b0 	strheq	r8, [r3]
816033f4:	15c38000 	strbne	r8, [r3]
816033f8:	e0833007 	add	r3, r3, r7
816033fc:	eafffff3 	b	816033d0 <do_mem_mw+0x7c>
81603400:	e3e00000 	mvn	r0, #0
81603404:	e12fff1e 	bx	lr
81603408:	e3a00001 	mov	r0, #1
8160340c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81603410:	81619864 	.word	0x81619864

81603414 <do_mem_loop>:
81603414:	e3520002 	cmp	r2, #2
81603418:	da000034 	ble	816034f0 <do_mem_loop+0xdc>
8160341c:	e92d4070 	push	{r4, r5, r6, lr}
81603420:	e3a01004 	mov	r1, #4
81603424:	e5930000 	ldr	r0, [r3]
81603428:	e1a05003 	mov	r5, r3
8160342c:	eb001870 	bl	816095f4 <cmd_get_data_size>
81603430:	e2506000 	subs	r6, r0, #0
81603434:	ba00002f 	blt	816034f8 <do_mem_loop+0xe4>
81603438:	e3a02010 	mov	r2, #16
8160343c:	e3a01000 	mov	r1, #0
81603440:	e5950004 	ldr	r0, [r5, #4]
81603444:	eb003cd3 	bl	81612798 <simple_strtoul>
81603448:	e3a02010 	mov	r2, #16
8160344c:	e3a01000 	mov	r1, #0
81603450:	e1a04000 	mov	r4, r0
81603454:	e5950008 	ldr	r0, [r5, #8]
81603458:	eb003cce 	bl	81612798 <simple_strtoul>
8160345c:	e3500001 	cmp	r0, #1
81603460:	1a000009 	bne	8160348c <do_mem_loop+0x78>
81603464:	e3560004 	cmp	r6, #4
81603468:	1a000001 	bne	81603474 <do_mem_loop+0x60>
8160346c:	e5943000 	ldr	r3, [r4]
81603470:	eafffffd 	b	8160346c <do_mem_loop+0x58>
81603474:	e3560002 	cmp	r6, #2
81603478:	1a000001 	bne	81603484 <do_mem_loop+0x70>
8160347c:	e1d430b0 	ldrh	r3, [r4]
81603480:	eafffffd 	b	8160347c <do_mem_loop+0x68>
81603484:	e5d43000 	ldrb	r3, [r4]
81603488:	eafffffd 	b	81603484 <do_mem_loop+0x70>
8160348c:	e3560004 	cmp	r6, #4
81603490:	1a000006 	bne	816034b0 <do_mem_loop+0x9c>
81603494:	e1a02004 	mov	r2, r4
81603498:	e1a03000 	mov	r3, r0
8160349c:	e2533001 	subs	r3, r3, #1
816034a0:	3afffffb 	bcc	81603494 <do_mem_loop+0x80>
816034a4:	e5921000 	ldr	r1, [r2]
816034a8:	e2822004 	add	r2, r2, #4
816034ac:	eafffffa 	b	8160349c <do_mem_loop+0x88>
816034b0:	e3560002 	cmp	r6, #2
816034b4:	10840000 	addne	r0, r4, r0
816034b8:	1a000006 	bne	816034d8 <do_mem_loop+0xc4>
816034bc:	e1a02004 	mov	r2, r4
816034c0:	e1a03000 	mov	r3, r0
816034c4:	e2533001 	subs	r3, r3, #1
816034c8:	3afffffb 	bcc	816034bc <do_mem_loop+0xa8>
816034cc:	e1d210b0 	ldrh	r1, [r2]
816034d0:	e2822002 	add	r2, r2, #2
816034d4:	eafffffa 	b	816034c4 <do_mem_loop+0xb0>
816034d8:	e1a03004 	mov	r3, r4
816034dc:	e1530000 	cmp	r3, r0
816034e0:	0afffffc 	beq	816034d8 <do_mem_loop+0xc4>
816034e4:	e5d32000 	ldrb	r2, [r3]
816034e8:	e2833001 	add	r3, r3, #1
816034ec:	eafffffa 	b	816034dc <do_mem_loop+0xc8>
816034f0:	e3e00000 	mvn	r0, #0
816034f4:	e12fff1e 	bx	lr
816034f8:	e3a00001 	mov	r0, #1
816034fc:	e8bd8070 	pop	{r4, r5, r6, pc}

81603500 <do_mem_md>:
81603500:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
81603504:	e3520001 	cmp	r2, #1
81603508:	e59f70c8 	ldr	r7, [pc, #200]	; 816035d8 <do_mem_md+0xd8>
8160350c:	d3e00000 	mvnle	r0, #0
81603510:	e5978000 	ldr	r8, [r7]
81603514:	da000018 	ble	8160357c <do_mem_md+0x7c>
81603518:	e2115001 	ands	r5, r1, #1
8160351c:	e1a0b003 	mov	fp, r3
81603520:	e1a0a002 	mov	sl, r2
81603524:	0a000016 	beq	81603584 <do_mem_md+0x84>
81603528:	e59f30ac 	ldr	r3, [pc, #172]	; 816035dc <do_mem_md+0xdc>
8160352c:	e5934000 	ldr	r4, [r3]
81603530:	e59f30a8 	ldr	r3, [pc, #168]	; 816035e0 <do_mem_md+0xe0>
81603534:	e5936000 	ldr	r6, [r3]
81603538:	e1a01006 	mov	r1, r6
8160353c:	e3a00010 	mov	r0, #16
81603540:	eb003d9d 	bl	81612bbc <__aeabi_idiv>
81603544:	e0050698 	mul	r5, r8, r6
81603548:	e1a03008 	mov	r3, r8
8160354c:	e1a01004 	mov	r1, r4
81603550:	e1a02006 	mov	r2, r6
81603554:	e58d0000 	str	r0, [sp]
81603558:	e1a00004 	mov	r0, r4
8160355c:	eb0037a8 	bl	81611404 <print_buffer>
81603560:	e59f3074 	ldr	r3, [pc, #116]	; 816035dc <do_mem_md+0xdc>
81603564:	e0844005 	add	r4, r4, r5
81603568:	e3a00000 	mov	r0, #0
8160356c:	e5878000 	str	r8, [r7]
81603570:	e5834000 	str	r4, [r3]
81603574:	e59f3064 	ldr	r3, [pc, #100]	; 816035e0 <do_mem_md+0xe0>
81603578:	e5836000 	str	r6, [r3]
8160357c:	e28dd008 	add	sp, sp, #8
81603580:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81603584:	e3a01004 	mov	r1, #4
81603588:	e5930000 	ldr	r0, [r3]
8160358c:	eb001818 	bl	816095f4 <cmd_get_data_size>
81603590:	e2506000 	subs	r6, r0, #0
81603594:	b3a00001 	movlt	r0, #1
81603598:	bafffff7 	blt	8160357c <do_mem_md+0x7c>
8160359c:	e3a02010 	mov	r2, #16
816035a0:	e1a01005 	mov	r1, r5
816035a4:	e59b0004 	ldr	r0, [fp, #4]
816035a8:	eb003c7a 	bl	81612798 <simple_strtoul>
816035ac:	e59f3030 	ldr	r3, [pc, #48]	; 816035e4 <do_mem_md+0xe4>
816035b0:	e35a0002 	cmp	sl, #2
816035b4:	e5934000 	ldr	r4, [r3]
816035b8:	e0804004 	add	r4, r0, r4
816035bc:	0affffdd 	beq	81603538 <do_mem_md+0x38>
816035c0:	e3a02010 	mov	r2, #16
816035c4:	e1a01005 	mov	r1, r5
816035c8:	e59b0008 	ldr	r0, [fp, #8]
816035cc:	eb003c71 	bl	81612798 <simple_strtoul>
816035d0:	e1a08000 	mov	r8, r0
816035d4:	eaffffd7 	b	81603538 <do_mem_md+0x38>
816035d8:	8161897c 	.word	0x8161897c
816035dc:	81619868 	.word	0x81619868
816035e0:	8161986c 	.word	0x8161986c
816035e4:	81619864 	.word	0x81619864

816035e8 <do_mem_cmp>:
816035e8:	e3520004 	cmp	r2, #4
816035ec:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816035f0:	13e04000 	mvnne	r4, #0
816035f4:	e24dd018 	sub	sp, sp, #24
816035f8:	1a000042 	bne	81603708 <do_mem_cmp+0x120>
816035fc:	e1a01002 	mov	r1, r2
81603600:	e5930000 	ldr	r0, [r3]
81603604:	e1a08003 	mov	r8, r3
81603608:	eb0017f9 	bl	816095f4 <cmd_get_data_size>
8160360c:	e2504000 	subs	r4, r0, #0
81603610:	b3a04001 	movlt	r4, #1
81603614:	ba00003b 	blt	81603708 <do_mem_cmp+0x120>
81603618:	e3540008 	cmp	r4, #8
8160361c:	059f50fc 	ldreq	r5, [pc, #252]	; 81603720 <do_mem_cmp+0x138>
81603620:	0a000006 	beq	81603640 <do_mem_cmp+0x58>
81603624:	e3540004 	cmp	r4, #4
81603628:	059f50f4 	ldreq	r5, [pc, #244]	; 81603724 <do_mem_cmp+0x13c>
8160362c:	0a000003 	beq	81603640 <do_mem_cmp+0x58>
81603630:	e59f30f0 	ldr	r3, [pc, #240]	; 81603728 <do_mem_cmp+0x140>
81603634:	e3540002 	cmp	r4, #2
81603638:	e59f50ec 	ldr	r5, [pc, #236]	; 8160372c <do_mem_cmp+0x144>
8160363c:	11a05003 	movne	r5, r3
81603640:	e3a02010 	mov	r2, #16
81603644:	e3a01000 	mov	r1, #0
81603648:	e5980004 	ldr	r0, [r8, #4]
8160364c:	eb003c51 	bl	81612798 <simple_strtoul>
81603650:	e59f70d8 	ldr	r7, [pc, #216]	; 81603730 <do_mem_cmp+0x148>
81603654:	e3a02010 	mov	r2, #16
81603658:	e3a01000 	mov	r1, #0
8160365c:	e5976000 	ldr	r6, [r7]
81603660:	e0806006 	add	r6, r0, r6
81603664:	e5980008 	ldr	r0, [r8, #8]
81603668:	eb003c4a 	bl	81612798 <simple_strtoul>
8160366c:	e5977000 	ldr	r7, [r7]
81603670:	e3a02010 	mov	r2, #16
81603674:	e3a01000 	mov	r1, #0
81603678:	e0807007 	add	r7, r0, r7
8160367c:	e598000c 	ldr	r0, [r8, #12]
81603680:	eb003c44 	bl	81612798 <simple_strtoul>
81603684:	e047c006 	sub	ip, r7, r6
81603688:	e1a02006 	mov	r2, r6
8160368c:	e3a08000 	mov	r8, #0
81603690:	e1580000 	cmp	r8, r0
81603694:	e082100c 	add	r1, r2, ip
81603698:	03a04000 	moveq	r4, #0
8160369c:	0a000015 	beq	816036f8 <do_mem_cmp+0x110>
816036a0:	e3540004 	cmp	r4, #4
816036a4:	05923000 	ldreq	r3, [r2]
816036a8:	05911000 	ldreq	r1, [r1]
816036ac:	0a000004 	beq	816036c4 <do_mem_cmp+0xdc>
816036b0:	e3540002 	cmp	r4, #2
816036b4:	01d230b0 	ldrheq	r3, [r2]
816036b8:	01d110b0 	ldrheq	r1, [r1]
816036bc:	15d23000 	ldrbne	r3, [r2]
816036c0:	15d11000 	ldrbne	r1, [r1]
816036c4:	e1530001 	cmp	r3, r1
816036c8:	0a000011 	beq	81603714 <do_mem_cmp+0x12c>
816036cc:	e0422006 	sub	r2, r2, r6
816036d0:	e58d1010 	str	r1, [sp, #16]
816036d4:	e58d400c 	str	r4, [sp, #12]
816036d8:	e0877002 	add	r7, r7, r2
816036dc:	e1a01005 	mov	r1, r5
816036e0:	e0862002 	add	r2, r6, r2
816036e4:	e88d00a8 	stm	sp, {r3, r5, r7}
816036e8:	e1a03004 	mov	r3, r4
816036ec:	e59f0040 	ldr	r0, [pc, #64]	; 81603734 <do_mem_cmp+0x14c>
816036f0:	e3a04001 	mov	r4, #1
816036f4:	eb003be0 	bl	8161267c <printf>
816036f8:	e1a02005 	mov	r2, r5
816036fc:	e1a01008 	mov	r1, r8
81603700:	e59f0030 	ldr	r0, [pc, #48]	; 81603738 <do_mem_cmp+0x150>
81603704:	eb003bdc 	bl	8161267c <printf>
81603708:	e1a00004 	mov	r0, r4
8160370c:	e28dd018 	add	sp, sp, #24
81603710:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81603714:	e0822004 	add	r2, r2, r4
81603718:	e2888001 	add	r8, r8, #1
8160371c:	eaffffdb 	b	81603690 <do_mem_cmp+0xa8>
81603720:	81615820 	.word	0x81615820
81603724:	81615827 	.word	0x81615827
81603728:	8161582c 	.word	0x8161582c
8160372c:	81615831 	.word	0x81615831
81603730:	81619864 	.word	0x81619864
81603734:	8161583a 	.word	0x8161583a
81603738:	8161586c 	.word	0x8161586c

8160373c <do_mem_base>:
8160373c:	e3520001 	cmp	r2, #1
81603740:	e92d4010 	push	{r4, lr}
81603744:	e59f4028 	ldr	r4, [pc, #40]	; 81603774 <do_mem_base+0x38>
81603748:	da000004 	ble	81603760 <do_mem_base+0x24>
8160374c:	e3a02010 	mov	r2, #16
81603750:	e3a01000 	mov	r1, #0
81603754:	e5930004 	ldr	r0, [r3, #4]
81603758:	eb003c0e 	bl	81612798 <simple_strtoul>
8160375c:	e5840000 	str	r0, [r4]
81603760:	e5941000 	ldr	r1, [r4]
81603764:	e59f000c 	ldr	r0, [pc, #12]	; 81603778 <do_mem_base+0x3c>
81603768:	eb003bc3 	bl	8161267c <printf>
8160376c:	e3a00000 	mov	r0, #0
81603770:	e8bd8010 	pop	{r4, pc}
81603774:	81619864 	.word	0x81619864
81603778:	81615809 	.word	0x81615809

8160377c <do_mem_cp>:
8160377c:	e3520004 	cmp	r2, #4
81603780:	1a000023 	bne	81603814 <do_mem_cp+0x98>
81603784:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81603788:	e1a01002 	mov	r1, r2
8160378c:	e5930000 	ldr	r0, [r3]
81603790:	e1a04003 	mov	r4, r3
81603794:	eb001796 	bl	816095f4 <cmd_get_data_size>
81603798:	e250a000 	subs	sl, r0, #0
8160379c:	ba000014 	blt	816037f4 <do_mem_cp+0x78>
816037a0:	e3a02010 	mov	r2, #16
816037a4:	e3a01000 	mov	r1, #0
816037a8:	e5940004 	ldr	r0, [r4, #4]
816037ac:	eb003bf9 	bl	81612798 <simple_strtoul>
816037b0:	e59f5064 	ldr	r5, [pc, #100]	; 8160381c <do_mem_cp+0xa0>
816037b4:	e3a02010 	mov	r2, #16
816037b8:	e3a01000 	mov	r1, #0
816037bc:	e5958000 	ldr	r8, [r5]
816037c0:	e1a07000 	mov	r7, r0
816037c4:	e5940008 	ldr	r0, [r4, #8]
816037c8:	eb003bf2 	bl	81612798 <simple_strtoul>
816037cc:	e3a02010 	mov	r2, #16
816037d0:	e3a01000 	mov	r1, #0
816037d4:	e5955000 	ldr	r5, [r5]
816037d8:	e1a06000 	mov	r6, r0
816037dc:	e594000c 	ldr	r0, [r4, #12]
816037e0:	eb003bec 	bl	81612798 <simple_strtoul>
816037e4:	e2502000 	subs	r2, r0, #0
816037e8:	1a000003 	bne	816037fc <do_mem_cp+0x80>
816037ec:	e59f002c 	ldr	r0, [pc, #44]	; 81603820 <do_mem_cp+0xa4>
816037f0:	eb000d67 	bl	81606d94 <puts>
816037f4:	e3a00001 	mov	r0, #1
816037f8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816037fc:	e0020a92 	mul	r2, r2, sl
81603800:	e0881007 	add	r1, r8, r7
81603804:	e0850006 	add	r0, r5, r6
81603808:	ebfff3e3 	bl	8160079c <memcpy>
8160380c:	e3a00000 	mov	r0, #0
81603810:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81603814:	e3e00000 	mvn	r0, #0
81603818:	e12fff1e 	bx	lr
8160381c:	81619864 	.word	0x81619864
81603820:	8161588e 	.word	0x8161588e

81603824 <do_mem_crc>:
81603824:	e3520002 	cmp	r2, #2
81603828:	e92d4007 	push	{r0, r1, r2, lr}
8160382c:	d3e00000 	mvnle	r0, #0
81603830:	da00000a 	ble	81603860 <do_mem_crc+0x3c>
81603834:	e1a0e003 	mov	lr, r3
81603838:	e1a0c002 	mov	ip, r2
8160383c:	e1a03001 	mov	r3, r1
81603840:	e1a02000 	mov	r2, r0
81603844:	e28ee004 	add	lr, lr, #4
81603848:	e24cc001 	sub	ip, ip, #1
8160384c:	e58de004 	str	lr, [sp, #4]
81603850:	e3a01000 	mov	r1, #0
81603854:	e58dc000 	str	ip, [sp]
81603858:	e59f0008 	ldr	r0, [pc, #8]	; 81603868 <do_mem_crc+0x44>
8160385c:	eb00041b 	bl	816048d0 <hash_command>
81603860:	e28dd00c 	add	sp, sp, #12
81603864:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
81603868:	8161589f 	.word	0x8161589f

8160386c <mod_mem.isra.0>:
8160386c:	e3520002 	cmp	r2, #2
81603870:	e92d45f7 	push	{r0, r1, r2, r4, r5, r6, r7, r8, sl, lr}
81603874:	13e00000 	mvnne	r0, #0
81603878:	1a00004c 	bne	816039b0 <mod_mem.isra.0+0x144>
8160387c:	e2114001 	ands	r4, r1, #1
81603880:	e1a07003 	mov	r7, r3
81603884:	e1a06000 	mov	r6, r0
81603888:	0a000022 	beq	81603918 <mod_mem.isra.0+0xac>
8160388c:	e59f3124 	ldr	r3, [pc, #292]	; 816039b8 <mod_mem.isra.0+0x14c>
81603890:	e5934000 	ldr	r4, [r3]
81603894:	e59f3120 	ldr	r3, [pc, #288]	; 816039bc <mod_mem.isra.0+0x150>
81603898:	e5935000 	ldr	r5, [r3]
8160389c:	e59f811c 	ldr	r8, [pc, #284]	; 816039c0 <mod_mem.isra.0+0x154>
816038a0:	e59fa11c 	ldr	sl, [pc, #284]	; 816039c4 <mod_mem.isra.0+0x158>
816038a4:	e1a01004 	mov	r1, r4
816038a8:	e1a00008 	mov	r0, r8
816038ac:	eb003b72 	bl	8161267c <printf>
816038b0:	e3550004 	cmp	r5, #4
816038b4:	e1a07005 	mov	r7, r5
816038b8:	05941000 	ldreq	r1, [r4]
816038bc:	059f0104 	ldreq	r0, [pc, #260]	; 816039c8 <mod_mem.isra.0+0x15c>
816038c0:	0a000004 	beq	816038d8 <mod_mem.isra.0+0x6c>
816038c4:	e3550002 	cmp	r5, #2
816038c8:	01d410b0 	ldrheq	r1, [r4]
816038cc:	11a0000a 	movne	r0, sl
816038d0:	059f00f4 	ldreq	r0, [pc, #244]	; 816039cc <mod_mem.isra.0+0x160>
816038d4:	15d41000 	ldrbne	r1, [r4]
816038d8:	eb003b67 	bl	8161267c <printf>
816038dc:	e59f00ec 	ldr	r0, [pc, #236]	; 816039d0 <mod_mem.isra.0+0x164>
816038e0:	eb000bc0 	bl	816067e8 <cli_readline>
816038e4:	e3500000 	cmp	r0, #0
816038e8:	0a000028 	beq	81603990 <mod_mem.isra.0+0x124>
816038ec:	e3500001 	cmp	r0, #1
816038f0:	e59f70dc 	ldr	r7, [pc, #220]	; 816039d4 <mod_mem.isra.0+0x168>
816038f4:	1a000015 	bne	81603950 <mod_mem.isra.0+0xe4>
816038f8:	e5d73000 	ldrb	r3, [r7]
816038fc:	e353002d 	cmp	r3, #45	; 0x2d
81603900:	1a000012 	bne	81603950 <mod_mem.isra.0+0xe4>
81603904:	e3560000 	cmp	r6, #0
81603908:	12657000 	rsbne	r7, r5, #0
8160390c:	0affffe4 	beq	816038a4 <mod_mem.isra.0+0x38>
81603910:	e0844007 	add	r4, r4, r7
81603914:	eaffffe2 	b	816038a4 <mod_mem.isra.0+0x38>
81603918:	e3a01004 	mov	r1, #4
8160391c:	e5930000 	ldr	r0, [r3]
81603920:	eb001733 	bl	816095f4 <cmd_get_data_size>
81603924:	e2505000 	subs	r5, r0, #0
81603928:	b3a00001 	movlt	r0, #1
8160392c:	ba00001f 	blt	816039b0 <mod_mem.isra.0+0x144>
81603930:	e1a01004 	mov	r1, r4
81603934:	e3a02010 	mov	r2, #16
81603938:	e5970004 	ldr	r0, [r7, #4]
8160393c:	eb003b95 	bl	81612798 <simple_strtoul>
81603940:	e59f3090 	ldr	r3, [pc, #144]	; 816039d8 <mod_mem.isra.0+0x16c>
81603944:	e5934000 	ldr	r4, [r3]
81603948:	e0804004 	add	r4, r0, r4
8160394c:	eaffffd2 	b	8160389c <mod_mem.isra.0+0x30>
81603950:	e3a02010 	mov	r2, #16
81603954:	e28d1004 	add	r1, sp, #4
81603958:	e59f0074 	ldr	r0, [pc, #116]	; 816039d4 <mod_mem.isra.0+0x168>
8160395c:	eb003b8d 	bl	81612798 <simple_strtoul>
81603960:	e59d3004 	ldr	r3, [sp, #4]
81603964:	e1530007 	cmp	r3, r7
81603968:	0a00000b 	beq	8160399c <mod_mem.isra.0+0x130>
8160396c:	e3550004 	cmp	r5, #4
81603970:	05840000 	streq	r0, [r4]
81603974:	0a000002 	beq	81603984 <mod_mem.isra.0+0x118>
81603978:	e3550002 	cmp	r5, #2
8160397c:	01c400b0 	strheq	r0, [r4]
81603980:	15c40000 	strbne	r0, [r4]
81603984:	e3560000 	cmp	r6, #0
81603988:	10844005 	addne	r4, r4, r5
8160398c:	eaffffc4 	b	816038a4 <mod_mem.isra.0+0x38>
81603990:	e3560000 	cmp	r6, #0
81603994:	0affffc2 	beq	816038a4 <mod_mem.isra.0+0x38>
81603998:	eaffffdc 	b	81603910 <mod_mem.isra.0+0xa4>
8160399c:	e59f3014 	ldr	r3, [pc, #20]	; 816039b8 <mod_mem.isra.0+0x14c>
816039a0:	e3a00000 	mov	r0, #0
816039a4:	e5834000 	str	r4, [r3]
816039a8:	e59f300c 	ldr	r3, [pc, #12]	; 816039bc <mod_mem.isra.0+0x150>
816039ac:	e5835000 	str	r5, [r3]
816039b0:	e28dd00c 	add	sp, sp, #12
816039b4:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
816039b8:	81619870 	.word	0x81619870
816039bc:	81619874 	.word	0x81619874
816039c0:	81616547 	.word	0x81616547
816039c4:	8161655a 	.word	0x8161655a
816039c8:	8161654e 	.word	0x8161654e
816039cc:	81616554 	.word	0x81616554
816039d0:	81616560 	.word	0x81616560
816039d4:	816199b0 	.word	0x816199b0
816039d8:	81619864 	.word	0x81619864

816039dc <do_mem_mm>:
816039dc:	e3a00001 	mov	r0, #1
816039e0:	eaffffa1 	b	8160386c <mod_mem.isra.0>

816039e4 <do_mem_nm>:
816039e4:	e3a00000 	mov	r0, #0
816039e8:	eaffff9f 	b	8160386c <mod_mem.isra.0>

816039ec <do_env_delete>:
816039ec:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816039f0:	e3a07004 	mov	r7, #4
816039f4:	e3520001 	cmp	r2, #1
816039f8:	e1a06003 	mov	r6, r3
816039fc:	e1a05002 	mov	r5, r2
81603a00:	da000003 	ble	81603a14 <do_env_delete+0x28>
81603a04:	e5b31004 	ldr	r1, [r3, #4]!
81603a08:	e5d10000 	ldrb	r0, [r1]
81603a0c:	e350002d 	cmp	r0, #45	; 0x2d
81603a10:	0a00000d 	beq	81603a4c <do_env_delete+0x60>
81603a14:	e59f2068 	ldr	r2, [pc, #104]	; 81603a84 <do_env_delete+0x98>
81603a18:	e3a04000 	mov	r4, #0
81603a1c:	e59f8064 	ldr	r8, [pc, #100]	; 81603a88 <do_env_delete+0x9c>
81603a20:	e5923000 	ldr	r3, [r2]
81603a24:	e2833001 	add	r3, r3, #1
81603a28:	e5823000 	str	r3, [r2]
81603a2c:	e2455001 	sub	r5, r5, #1
81603a30:	e3550000 	cmp	r5, #0
81603a34:	ca000009 	bgt	81603a60 <do_env_delete+0x74>
81603a38:	e1a00004 	mov	r0, r4
81603a3c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81603a40:	e3500066 	cmp	r0, #102	; 0x66
81603a44:	e3a07006 	mov	r7, #6
81603a48:	1a00000b 	bne	81603a7c <do_env_delete+0x90>
81603a4c:	e5f10001 	ldrb	r0, [r1, #1]!
81603a50:	e3500000 	cmp	r0, #0
81603a54:	1afffff9 	bne	81603a40 <do_env_delete+0x54>
81603a58:	e2422001 	sub	r2, r2, #1
81603a5c:	eaffffe4 	b	816039f4 <do_env_delete+0x8>
81603a60:	e1a02007 	mov	r2, r7
81603a64:	e1a01008 	mov	r1, r8
81603a68:	e5b60004 	ldr	r0, [r6, #4]!
81603a6c:	eb0033c5 	bl	81610988 <hdelete_r>
81603a70:	e3500000 	cmp	r0, #0
81603a74:	03a04001 	moveq	r4, #1
81603a78:	eaffffeb 	b	81603a2c <do_env_delete+0x40>
81603a7c:	e3e04000 	mvn	r4, #0
81603a80:	eaffffec 	b	81603a38 <do_env_delete+0x4c>
81603a84:	81618c1c 	.word	0x81618c1c
81603a88:	816190f0 	.word	0x816190f0

81603a8c <do_env>:
81603a8c:	e3520001 	cmp	r2, #1
81603a90:	da000014 	ble	81603ae8 <do_env+0x5c>
81603a94:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81603a98:	e1a06001 	mov	r6, r1
81603a9c:	e1a05000 	mov	r5, r0
81603aa0:	e2424001 	sub	r4, r2, #1
81603aa4:	e59f1044 	ldr	r1, [pc, #68]	; 81603af0 <do_env+0x64>
81603aa8:	e3a02007 	mov	r2, #7
81603aac:	e5930004 	ldr	r0, [r3, #4]
81603ab0:	e2837004 	add	r7, r3, #4
81603ab4:	eb0014d1 	bl	81608e00 <find_cmd_tbl>
81603ab8:	e3500000 	cmp	r0, #0
81603abc:	0a000007 	beq	81603ae0 <do_env+0x54>
81603ac0:	e590e00c 	ldr	lr, [r0, #12]
81603ac4:	e1a03007 	mov	r3, r7
81603ac8:	e1a02004 	mov	r2, r4
81603acc:	e1a01006 	mov	r1, r6
81603ad0:	e1a00005 	mov	r0, r5
81603ad4:	e1a0c00e 	mov	ip, lr
81603ad8:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
81603adc:	e12fff1c 	bx	ip
81603ae0:	e3e00000 	mvn	r0, #0
81603ae4:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81603ae8:	e3e00000 	mvn	r0, #0
81603aec:	e12fff1e 	bx	lr
81603af0:	81618980 	.word	0x81618980

81603af4 <env_print>:
81603af4:	e92d4030 	push	{r4, r5, lr}
81603af8:	e2505000 	subs	r5, r0, #0
81603afc:	e24dd02c 	sub	sp, sp, #44	; 0x2c
81603b00:	e3a03000 	mov	r3, #0
81603b04:	e1a02001 	mov	r2, r1
81603b08:	e58d3010 	str	r3, [sp, #16]
81603b0c:	0a000014 	beq	81603b64 <env_print+0x70>
81603b10:	e59f2094 	ldr	r2, [pc, #148]	; 81603bac <env_print+0xb8>
81603b14:	e58d301c 	str	r3, [sp, #28]
81603b18:	e58d3000 	str	r3, [sp]
81603b1c:	e28d3028 	add	r3, sp, #40	; 0x28
81603b20:	e58d2008 	str	r2, [sp, #8]
81603b24:	e28d2014 	add	r2, sp, #20
81603b28:	e58d5018 	str	r5, [sp, #24]
81603b2c:	e58d100c 	str	r1, [sp, #12]
81603b30:	e58d2004 	str	r2, [sp, #4]
81603b34:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
81603b38:	eb003271 	bl	81610504 <hsearch_r>
81603b3c:	e59d4014 	ldr	r4, [sp, #20]
81603b40:	e3540000 	cmp	r4, #0
81603b44:	0a000003 	beq	81603b58 <env_print+0x64>
81603b48:	e8940006 	ldm	r4, {r1, r2}
81603b4c:	e59f005c 	ldr	r0, [pc, #92]	; 81603bb0 <env_print+0xbc>
81603b50:	eb003ac9 	bl	8161267c <printf>
81603b54:	e1a04000 	mov	r4, r0
81603b58:	e1a00004 	mov	r0, r4
81603b5c:	e28dd02c 	add	sp, sp, #44	; 0x2c
81603b60:	e8bd8030 	pop	{r4, r5, pc}
81603b64:	e58d5008 	str	r5, [sp, #8]
81603b68:	e28d3010 	add	r3, sp, #16
81603b6c:	e58d5004 	str	r5, [sp, #4]
81603b70:	e3a0100a 	mov	r1, #10
81603b74:	e58d5000 	str	r5, [sp]
81603b78:	e59f002c 	ldr	r0, [pc, #44]	; 81603bac <env_print+0xb8>
81603b7c:	eb0033bd 	bl	81610a78 <hexport_r>
81603b80:	e2504000 	subs	r4, r0, #0
81603b84:	da000004 	ble	81603b9c <env_print+0xa8>
81603b88:	e59d0010 	ldr	r0, [sp, #16]
81603b8c:	eb000c80 	bl	81606d94 <puts>
81603b90:	e59d0010 	ldr	r0, [sp, #16]
81603b94:	eb000da7 	bl	81607238 <free>
81603b98:	eaffffee 	b	81603b58 <env_print+0x64>
81603b9c:	e59f0010 	ldr	r0, [pc, #16]	; 81603bb4 <env_print+0xc0>
81603ba0:	e1a04005 	mov	r4, r5
81603ba4:	eb003ab4 	bl	8161267c <printf>
81603ba8:	eaffffea 	b	81603b58 <env_print+0x64>
81603bac:	816190f0 	.word	0x816190f0
81603bb0:	81615b58 	.word	0x81615b58
81603bb4:	81615b5f 	.word	0x81615b5f

81603bb8 <do_env_print>:
81603bb8:	e3520001 	cmp	r2, #1
81603bbc:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81603bc0:	e1a04002 	mov	r4, r2
81603bc4:	e1a06003 	mov	r6, r3
81603bc8:	da00000d 	ble	81603c04 <do_env_print+0x4c>
81603bcc:	e5933004 	ldr	r3, [r3, #4]
81603bd0:	e5d32000 	ldrb	r2, [r3]
81603bd4:	e352002d 	cmp	r2, #45	; 0x2d
81603bd8:	1a000005 	bne	81603bf4 <do_env_print+0x3c>
81603bdc:	e5d33001 	ldrb	r3, [r3, #1]
81603be0:	e3530061 	cmp	r3, #97	; 0x61
81603be4:	02444001 	subeq	r4, r4, #1
81603be8:	02866004 	addeq	r6, r6, #4
81603bec:	03a01000 	moveq	r1, #0
81603bf0:	0a000004 	beq	81603c08 <do_env_print+0x50>
81603bf4:	e3a07000 	mov	r7, #0
81603bf8:	e3a05001 	mov	r5, #1
81603bfc:	e59f806c 	ldr	r8, [pc, #108]	; 81603c70 <do_env_print+0xb8>
81603c00:	ea000016 	b	81603c60 <do_env_print+0xa8>
81603c04:	e3a01008 	mov	r1, #8
81603c08:	e3540001 	cmp	r4, #1
81603c0c:	1afffff8 	bne	81603bf4 <do_env_print+0x3c>
81603c10:	e3a00000 	mov	r0, #0
81603c14:	ebffffb6 	bl	81603af4 <env_print>
81603c18:	e2501000 	subs	r1, r0, #0
81603c1c:	0a000003 	beq	81603c30 <do_env_print+0x78>
81603c20:	e3072ffc 	movw	r2, #32764	; 0x7ffc
81603c24:	e59f0048 	ldr	r0, [pc, #72]	; 81603c74 <do_env_print+0xbc>
81603c28:	eb003a93 	bl	8161267c <printf>
81603c2c:	e3a04000 	mov	r4, #0
81603c30:	e1a00004 	mov	r0, r4
81603c34:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81603c38:	e3a01000 	mov	r1, #0
81603c3c:	e7960105 	ldr	r0, [r6, r5, lsl #2]
81603c40:	ebffffab 	bl	81603af4 <env_print>
81603c44:	e3500000 	cmp	r0, #0
81603c48:	1a000003 	bne	81603c5c <do_env_print+0xa4>
81603c4c:	e7961105 	ldr	r1, [r6, r5, lsl #2]
81603c50:	e1a00008 	mov	r0, r8
81603c54:	eb003a88 	bl	8161267c <printf>
81603c58:	e2877001 	add	r7, r7, #1
81603c5c:	e2855001 	add	r5, r5, #1
81603c60:	e1550004 	cmp	r5, r4
81603c64:	bafffff3 	blt	81603c38 <do_env_print+0x80>
81603c68:	e1a04007 	mov	r4, r7
81603c6c:	eaffffef 	b	81603c30 <do_env_print+0x78>
81603c70:	816152c9 	.word	0x816152c9
81603c74:	816152a8 	.word	0x816152a8

81603c78 <do_env_import>:
81603c78:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81603c7c:	e3a06000 	mov	r6, #0
81603c80:	e24dd018 	sub	sp, sp, #24
81603c84:	e1a04002 	mov	r4, r2
81603c88:	e1a0b003 	mov	fp, r3
81603c8c:	e1a0a006 	mov	sl, r6
81603c90:	e1a02006 	mov	r2, r6
81603c94:	e1a05006 	mov	r5, r6
81603c98:	e3a0800a 	mov	r8, #10
81603c9c:	e2441001 	sub	r1, r4, #1
81603ca0:	e2514000 	subs	r4, r1, #0
81603ca4:	da00003e 	ble	81603da4 <do_env_import+0x12c>
81603ca8:	e5bb0004 	ldr	r0, [fp, #4]!
81603cac:	e5d01000 	ldrb	r1, [r0]
81603cb0:	e351002d 	cmp	r1, #45	; 0x2d
81603cb4:	0a000030 	beq	81603d7c <do_env_import+0x104>
81603cb8:	e3520000 	cmp	r2, #0
81603cbc:	1a000001 	bne	81603cc8 <do_env_import+0x50>
81603cc0:	e59f0204 	ldr	r0, [pc, #516]	; 81603ecc <do_env_import+0x254>
81603cc4:	eb003a6c 	bl	8161267c <printf>
81603cc8:	e358000a 	cmp	r8, #10
81603ccc:	13560000 	cmpne	r6, #0
81603cd0:	e3a02010 	mov	r2, #16
81603cd4:	e3a01000 	mov	r1, #0
81603cd8:	e59b0000 	ldr	r0, [fp]
81603cdc:	13a06000 	movne	r6, #0
81603ce0:	eb003aac 	bl	81612798 <simple_strtoul>
81603ce4:	e3540002 	cmp	r4, #2
81603ce8:	e1a07000 	mov	r7, r0
81603cec:	1a00003a 	bne	81603ddc <do_env_import+0x164>
81603cf0:	e3a02010 	mov	r2, #16
81603cf4:	e3a01000 	mov	r1, #0
81603cf8:	e59b0004 	ldr	r0, [fp, #4]
81603cfc:	eb003aa5 	bl	81612798 <simple_strtoul>
81603d00:	e1a04000 	mov	r4, r0
81603d04:	e3550000 	cmp	r5, #0
81603d08:	0a000051 	beq	81603e54 <do_env_import+0x1dc>
81603d0c:	e1a01007 	mov	r1, r7
81603d10:	e2444004 	sub	r4, r4, #4
81603d14:	e2877004 	add	r7, r7, #4
81603d18:	e3a02004 	mov	r2, #4
81603d1c:	e28d0014 	add	r0, sp, #20
81603d20:	ebfff29d 	bl	8160079c <memcpy>
81603d24:	e1a02004 	mov	r2, r4
81603d28:	e1a01007 	mov	r1, r7
81603d2c:	e3a00000 	mov	r0, #0
81603d30:	eb003653 	bl	81611684 <crc32>
81603d34:	e59d3014 	ldr	r3, [sp, #20]
81603d38:	e1500003 	cmp	r0, r3
81603d3c:	0a000044 	beq	81603e54 <do_env_import+0x1dc>
81603d40:	e59f0188 	ldr	r0, [pc, #392]	; 81603ed0 <do_env_import+0x258>
81603d44:	eb000c12 	bl	81606d94 <puts>
81603d48:	e1a00005 	mov	r0, r5
81603d4c:	e28dd018 	add	sp, sp, #24
81603d50:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81603d54:	e3510072 	cmp	r1, #114	; 0x72
81603d58:	0a00001d 	beq	81603dd4 <do_env_import+0x15c>
81603d5c:	e3510074 	cmp	r1, #116	; 0x74
81603d60:	1a00000f 	bne	81603da4 <do_env_import+0x12c>
81603d64:	e3520000 	cmp	r2, #0
81603d68:	1a000053 	bne	81603ebc <do_env_import+0x244>
81603d6c:	e3a02001 	mov	r2, #1
81603d70:	e3a0800a 	mov	r8, #10
81603d74:	ea000000 	b	81603d7c <do_env_import+0x104>
81603d78:	e3a0a001 	mov	sl, #1
81603d7c:	e5f01001 	ldrb	r1, [r0, #1]!
81603d80:	e3510000 	cmp	r1, #0
81603d84:	0affffc4 	beq	81603c9c <do_env_import+0x24>
81603d88:	e3510064 	cmp	r1, #100	; 0x64
81603d8c:	0afffff9 	beq	81603d78 <do_env_import+0x100>
81603d90:	8affffef 	bhi	81603d54 <do_env_import+0xdc>
81603d94:	e3510062 	cmp	r1, #98	; 0x62
81603d98:	0a000003 	beq	81603dac <do_env_import+0x134>
81603d9c:	e3510063 	cmp	r1, #99	; 0x63
81603da0:	0a000006 	beq	81603dc0 <do_env_import+0x148>
81603da4:	e3e05000 	mvn	r5, #0
81603da8:	eaffffe6 	b	81603d48 <do_env_import+0xd0>
81603dac:	e3520000 	cmp	r2, #0
81603db0:	1a000041 	bne	81603ebc <do_env_import+0x244>
81603db4:	e3a02001 	mov	r2, #1
81603db8:	e3a08000 	mov	r8, #0
81603dbc:	eaffffee 	b	81603d7c <do_env_import+0x104>
81603dc0:	e3520000 	cmp	r2, #0
81603dc4:	1a00003c 	bne	81603ebc <do_env_import+0x244>
81603dc8:	e3a02001 	mov	r2, #1
81603dcc:	e1a05002 	mov	r5, r2
81603dd0:	eafffff8 	b	81603db8 <do_env_import+0x140>
81603dd4:	e3a06001 	mov	r6, #1
81603dd8:	eaffffe7 	b	81603d7c <do_env_import+0x104>
81603ddc:	e2444001 	sub	r4, r4, #1
81603de0:	e3550000 	cmp	r5, #0
81603de4:	e16f4f14 	clz	r4, r4
81603de8:	e1a042a4 	lsr	r4, r4, #5
81603dec:	03a04000 	moveq	r4, #0
81603df0:	e3540000 	cmp	r4, #0
81603df4:	0a000003 	beq	81603e08 <do_env_import+0x190>
81603df8:	e59f00d4 	ldr	r0, [pc, #212]	; 81603ed4 <do_env_import+0x25c>
81603dfc:	eb000be4 	bl	81606d94 <puts>
81603e00:	e3a05001 	mov	r5, #1
81603e04:	eaffffcf 	b	81603d48 <do_env_import+0xd0>
81603e08:	e7d72004 	ldrb	r2, [r7, r4]
81603e0c:	e0873004 	add	r3, r7, r4
81603e10:	e1520008 	cmp	r2, r8
81603e14:	1a000002 	bne	81603e24 <do_env_import+0x1ac>
81603e18:	e5d33001 	ldrb	r3, [r3, #1]
81603e1c:	e3530000 	cmp	r3, #0
81603e20:	0a000005 	beq	81603e3c <do_env_import+0x1c4>
81603e24:	e2844001 	add	r4, r4, #1
81603e28:	e3540601 	cmp	r4, #1048576	; 0x100000
81603e2c:	1afffff5 	bne	81603e08 <do_env_import+0x190>
81603e30:	e1a01004 	mov	r1, r4
81603e34:	e59f009c 	ldr	r0, [pc, #156]	; 81603ed8 <do_env_import+0x260>
81603e38:	eb003a0f 	bl	8161267c <printf>
81603e3c:	e2844002 	add	r4, r4, #2
81603e40:	e59f0094 	ldr	r0, [pc, #148]	; 81603edc <do_env_import+0x264>
81603e44:	e1a02004 	mov	r2, r4
81603e48:	e1a01004 	mov	r1, r4
81603e4c:	eb003a0a 	bl	8161267c <printf>
81603e50:	eaffffab 	b	81603d04 <do_env_import+0x8c>
81603e54:	e3a05000 	mov	r5, #0
81603e58:	e58d6004 	str	r6, [sp, #4]
81603e5c:	e58d500c 	str	r5, [sp, #12]
81603e60:	e22aa001 	eor	sl, sl, #1
81603e64:	e58d5008 	str	r5, [sp, #8]
81603e68:	e1a03008 	mov	r3, r8
81603e6c:	e58da000 	str	sl, [sp]
81603e70:	e1a02004 	mov	r2, r4
81603e74:	e1a01007 	mov	r1, r7
81603e78:	e59f0060 	ldr	r0, [pc, #96]	; 81603ee0 <do_env_import+0x268>
81603e7c:	eb0033c3 	bl	81610d90 <himport_r>
81603e80:	e1500005 	cmp	r0, r5
81603e84:	1a000008 	bne	81603eac <do_env_import+0x234>
81603e88:	e59f3054 	ldr	r3, [pc, #84]	; 81603ee4 <do_env_import+0x26c>
81603e8c:	e59f1054 	ldr	r1, [pc, #84]	; 81603ee8 <do_env_import+0x270>
81603e90:	e59f2054 	ldr	r2, [pc, #84]	; 81603eec <do_env_import+0x274>
81603e94:	e58d3000 	str	r3, [sp]
81603e98:	e300343d 	movw	r3, #1085	; 0x43d
81603e9c:	e5911000 	ldr	r1, [r1]
81603ea0:	e59f0048 	ldr	r0, [pc, #72]	; 81603ef0 <do_env_import+0x278>
81603ea4:	eb0039f4 	bl	8161267c <printf>
81603ea8:	eaffffd4 	b	81603e00 <do_env_import+0x188>
81603eac:	e5993004 	ldr	r3, [r9, #4]
81603eb0:	e3833080 	orr	r3, r3, #128	; 0x80
81603eb4:	e5893004 	str	r3, [r9, #4]
81603eb8:	eaffffa2 	b	81603d48 <do_env_import+0xd0>
81603ebc:	e5931000 	ldr	r1, [r3]
81603ec0:	e59f002c 	ldr	r0, [pc, #44]	; 81603ef4 <do_env_import+0x27c>
81603ec4:	eb0039ec 	bl	8161267c <printf>
81603ec8:	eaffffcc 	b	81603e00 <do_env_import+0x188>
81603ecc:	81615156 	.word	0x81615156
81603ed0:	816151d8 	.word	0x816151d8
81603ed4:	8161517d 	.word	0x8161517d
81603ed8:	81615273 	.word	0x81615273
81603edc:	816151b0 	.word	0x816151b0
81603ee0:	816190f0 	.word	0x816190f0
81603ee4:	816140a4 	.word	0x816140a4
81603ee8:	81619e84 	.word	0x81619e84
81603eec:	816151fa 	.word	0x816151fa
81603ef0:	81615207 	.word	0x81615207
81603ef4:	81615244 	.word	0x81615244

81603ef8 <do_env_default>:
81603ef8:	e3a0c000 	mov	ip, #0
81603efc:	e92d4010 	push	{r4, lr}
81603f00:	e2422001 	sub	r2, r2, #1
81603f04:	e3520000 	cmp	r2, #0
81603f08:	da000003 	ble	81603f1c <do_env_default+0x24>
81603f0c:	e5b31004 	ldr	r1, [r3, #4]!
81603f10:	e5d1e000 	ldrb	lr, [r1]
81603f14:	e35e002d 	cmp	lr, #45	; 0x2d
81603f18:	0a000009 	beq	81603f44 <do_env_default+0x4c>
81603f1c:	e3520000 	cmp	r2, #0
81603f20:	01a0100c 	moveq	r1, ip
81603f24:	13a01000 	movne	r1, #0
81603f28:	e3510000 	cmp	r1, #0
81603f2c:	0a00000d 	beq	81603f68 <do_env_default+0x70>
81603f30:	e59f0058 	ldr	r0, [pc, #88]	; 81603f90 <do_env_default+0x98>
81603f34:	eb001d56 	bl	8160b494 <set_default_env>
81603f38:	e3a00000 	mov	r0, #0
81603f3c:	e8bd8010 	pop	{r4, pc}
81603f40:	e3a0c001 	mov	ip, #1
81603f44:	e5f1e001 	ldrb	lr, [r1, #1]!
81603f48:	e35e0000 	cmp	lr, #0
81603f4c:	0affffeb 	beq	81603f00 <do_env_default+0x8>
81603f50:	e35e0061 	cmp	lr, #97	; 0x61
81603f54:	0afffff9 	beq	81603f40 <do_env_default+0x48>
81603f58:	e35e0066 	cmp	lr, #102	; 0x66
81603f5c:	0afffff8 	beq	81603f44 <do_env_default+0x4c>
81603f60:	e8bd4010 	pop	{r4, lr}
81603f64:	ea0013d8 	b	81608ecc <cmd_usage>
81603f68:	e22cc001 	eor	ip, ip, #1
81603f6c:	e3520000 	cmp	r2, #0
81603f70:	d3a0c000 	movle	ip, #0
81603f74:	c20cc001 	andgt	ip, ip, #1
81603f78:	e35c0000 	cmp	ip, #0
81603f7c:	0afffff7 	beq	81603f60 <do_env_default+0x68>
81603f80:	e1a01003 	mov	r1, r3
81603f84:	e1a00002 	mov	r0, r2
81603f88:	eb001d74 	bl	8160b560 <set_default_vars>
81603f8c:	eaffffe9 	b	81603f38 <do_env_default+0x40>
81603f90:	816150e7 	.word	0x816150e7

81603f94 <_do_env_set.isra.0>:
81603f94:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
81603f98:	e24dd02c 	sub	sp, sp, #44	; 0x2c
81603f9c:	e1a08000 	mov	r8, r0
81603fa0:	e1a07002 	mov	r7, r2
81603fa4:	e1a06001 	mov	r6, r1
81603fa8:	e3580001 	cmp	r8, #1
81603fac:	e2862004 	add	r2, r6, #4
81603fb0:	e5965004 	ldr	r5, [r6, #4]
81603fb4:	da000002 	ble	81603fc4 <_do_env_set.isra.0+0x30>
81603fb8:	e5d53000 	ldrb	r3, [r5]
81603fbc:	e353002d 	cmp	r3, #45	; 0x2d
81603fc0:	0a00000a 	beq	81603ff0 <_do_env_set.isra.0+0x5c>
81603fc4:	e3a0103d 	mov	r1, #61	; 0x3d
81603fc8:	e1a00005 	mov	r0, r5
81603fcc:	eb003634 	bl	816118a4 <strchr>
81603fd0:	e2504000 	subs	r4, r0, #0
81603fd4:	0a00000e 	beq	81604014 <_do_env_set.isra.0+0x80>
81603fd8:	e59f0150 	ldr	r0, [pc, #336]	; 81604130 <_do_env_set.isra.0+0x19c>
81603fdc:	e1a01005 	mov	r1, r5
81603fe0:	eb0039a5 	bl	8161267c <printf>
81603fe4:	e3a00001 	mov	r0, #1
81603fe8:	e28dd02c 	add	sp, sp, #44	; 0x2c
81603fec:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
81603ff0:	e2488001 	sub	r8, r8, #1
81603ff4:	e1a06002 	mov	r6, r2
81603ff8:	e5f53001 	ldrb	r3, [r5, #1]!
81603ffc:	e3530000 	cmp	r3, #0
81604000:	0affffe8 	beq	81603fa8 <_do_env_set.isra.0+0x14>
81604004:	e3530066 	cmp	r3, #102	; 0x66
81604008:	1a000046 	bne	81604128 <_do_env_set.isra.0+0x194>
8160400c:	e3877002 	orr	r7, r7, #2
81604010:	eafffff8 	b	81603ff8 <_do_env_set.isra.0+0x64>
81604014:	e59f2118 	ldr	r2, [pc, #280]	; 81604134 <_do_env_set.isra.0+0x1a0>
81604018:	e3580002 	cmp	r8, #2
8160401c:	e5923000 	ldr	r3, [r2]
81604020:	e2833001 	add	r3, r3, #1
81604024:	e5823000 	str	r3, [r2]
81604028:	da000005 	ble	81604044 <_do_env_set.isra.0+0xb0>
8160402c:	e5963008 	ldr	r3, [r6, #8]
81604030:	e3530000 	cmp	r3, #0
81604034:	1286a008 	addne	sl, r6, #8
81604038:	10866108 	addne	r6, r6, r8, lsl #2
8160403c:	11a0800a 	movne	r8, sl
81604040:	1a000006 	bne	81604060 <_do_env_set.isra.0+0xcc>
81604044:	e1a02007 	mov	r2, r7
81604048:	e59f10e8 	ldr	r1, [pc, #232]	; 81604138 <_do_env_set.isra.0+0x1a4>
8160404c:	e1a00005 	mov	r0, r5
81604050:	eb00324c 	bl	81610988 <hdelete_r>
81604054:	e16f0f10 	clz	r0, r0
81604058:	e1a002a0 	lsr	r0, r0, #5
8160405c:	eaffffe1 	b	81603fe8 <_do_env_set.isra.0+0x54>
81604060:	e4980004 	ldr	r0, [r8], #4
81604064:	e2844001 	add	r4, r4, #1
81604068:	eb003619 	bl	816118d4 <strlen>
8160406c:	e1580006 	cmp	r8, r6
81604070:	e0844000 	add	r4, r4, r0
81604074:	1afffff9 	bne	81604060 <_do_env_set.isra.0+0xcc>
81604078:	e1a00004 	mov	r0, r4
8160407c:	eb000cf8 	bl	81607464 <malloc>
81604080:	e2506000 	subs	r6, r0, #0
81604084:	01a01004 	moveq	r1, r4
81604088:	059f00ac 	ldreq	r0, [pc, #172]	; 8160413c <_do_env_set.isra.0+0x1a8>
8160408c:	0affffd3 	beq	81603fe0 <_do_env_set.isra.0+0x4c>
81604090:	e1a03006 	mov	r3, r6
81604094:	e3a0c020 	mov	ip, #32
81604098:	e49a2004 	ldr	r2, [sl], #4
8160409c:	e2422001 	sub	r2, r2, #1
816040a0:	e5f21001 	ldrb	r1, [r2, #1]!
816040a4:	e1a00003 	mov	r0, r3
816040a8:	e3510000 	cmp	r1, #0
816040ac:	e4c31001 	strb	r1, [r3], #1
816040b0:	1afffffa 	bne	816040a0 <_do_env_set.isra.0+0x10c>
816040b4:	e15a0008 	cmp	sl, r8
816040b8:	e5c0c000 	strb	ip, [r0]
816040bc:	1afffff5 	bne	81604098 <_do_env_set.isra.0+0x104>
816040c0:	e1560003 	cmp	r6, r3
816040c4:	15431001 	strbne	r1, [r3, #-1]
816040c8:	e59f3068 	ldr	r3, [pc, #104]	; 81604138 <_do_env_set.isra.0+0x1a4>
816040cc:	e3a04001 	mov	r4, #1
816040d0:	e58d5018 	str	r5, [sp, #24]
816040d4:	e58d601c 	str	r6, [sp, #28]
816040d8:	e58d3008 	str	r3, [sp, #8]
816040dc:	e28d3014 	add	r3, sp, #20
816040e0:	e58d3004 	str	r3, [sp, #4]
816040e4:	e28d3028 	add	r3, sp, #40	; 0x28
816040e8:	e58d700c 	str	r7, [sp, #12]
816040ec:	e58d4000 	str	r4, [sp]
816040f0:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
816040f4:	eb003102 	bl	81610504 <hsearch_r>
816040f8:	e1a00006 	mov	r0, r6
816040fc:	eb000c4d 	bl	81607238 <free>
81604100:	e59d3014 	ldr	r3, [sp, #20]
81604104:	e3530000 	cmp	r3, #0
81604108:	13a00000 	movne	r0, #0
8160410c:	1affffb5 	bne	81603fe8 <_do_env_set.isra.0+0x54>
81604110:	e59f3028 	ldr	r3, [pc, #40]	; 81604140 <_do_env_set.isra.0+0x1ac>
81604114:	e1a01005 	mov	r1, r5
81604118:	e59f0024 	ldr	r0, [pc, #36]	; 81604144 <_do_env_set.isra.0+0x1b0>
8160411c:	e5932000 	ldr	r2, [r3]
81604120:	eb003955 	bl	8161267c <printf>
81604124:	eaffffae 	b	81603fe4 <_do_env_set.isra.0+0x50>
81604128:	e3e00000 	mvn	r0, #0
8160412c:	eaffffad 	b	81603fe8 <_do_env_set.isra.0+0x54>
81604130:	816141e1 	.word	0x816141e1
81604134:	81618c1c 	.word	0x81618c1c
81604138:	816190f0 	.word	0x816190f0
8160413c:	81614217 	.word	0x81614217
81604140:	81619e84 	.word	0x81619e84
81604144:	81614231 	.word	0x81614231

81604148 <do_env_set>:
81604148:	e3520001 	cmp	r2, #1
8160414c:	e1a00002 	mov	r0, r2
81604150:	da000002 	ble	81604160 <do_env_set+0x18>
81604154:	e3a02004 	mov	r2, #4
81604158:	e1a01003 	mov	r1, r3
8160415c:	eaffff8c 	b	81603f94 <_do_env_set.isra.0>
81604160:	e3e00000 	mvn	r0, #0
81604164:	e12fff1e 	bx	lr

81604168 <env_set>:
81604168:	e92d401f 	push	{r0, r1, r2, r3, r4, lr}
8160416c:	e59f3050 	ldr	r3, [pc, #80]	; 816041c4 <env_set+0x5c>
81604170:	e98d0003 	stmib	sp, {r0, r1}
81604174:	e58d3000 	str	r3, [sp]
81604178:	e3a03000 	mov	r3, #0
8160417c:	e58d300c 	str	r3, [sp, #12]
81604180:	e5993004 	ldr	r3, [r9, #4]
81604184:	e3130080 	tst	r3, #128	; 0x80
81604188:	03a00001 	moveq	r0, #1
8160418c:	0a00000a 	beq	816041bc <env_set+0x54>
81604190:	e3510000 	cmp	r1, #0
81604194:	e3a02c02 	mov	r2, #512	; 0x200
81604198:	0a000004 	beq	816041b0 <env_set+0x48>
8160419c:	e5d13000 	ldrb	r3, [r1]
816041a0:	e3530000 	cmp	r3, #0
816041a4:	11a0100d 	movne	r1, sp
816041a8:	13a00003 	movne	r0, #3
816041ac:	1a000001 	bne	816041b8 <env_set+0x50>
816041b0:	e1a0100d 	mov	r1, sp
816041b4:	e3a00002 	mov	r0, #2
816041b8:	ebffff75 	bl	81603f94 <_do_env_set.isra.0>
816041bc:	e28dd014 	add	sp, sp, #20
816041c0:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
816041c4:	81615b84 	.word	0x81615b84

816041c8 <env_set_ulong>:
816041c8:	e92d4010 	push	{r4, lr}
816041cc:	e1a04000 	mov	r4, r0
816041d0:	e1a00001 	mov	r0, r1
816041d4:	eb003946 	bl	816126f4 <simple_itoa>
816041d8:	e1a01000 	mov	r1, r0
816041dc:	e1a00004 	mov	r0, r4
816041e0:	e8bd4010 	pop	{r4, lr}
816041e4:	eaffffdf 	b	81604168 <env_set>

816041e8 <env_set_hex>:
816041e8:	e92d4010 	push	{r4, lr}
816041ec:	e24dd018 	sub	sp, sp, #24
816041f0:	e1a02001 	mov	r2, r1
816041f4:	e1a04000 	mov	r4, r0
816041f8:	e59f1018 	ldr	r1, [pc, #24]	; 81604218 <env_set_hex+0x30>
816041fc:	e28d0004 	add	r0, sp, #4
81604200:	eb003913 	bl	81612654 <sprintf>
81604204:	e28d1004 	add	r1, sp, #4
81604208:	e1a00004 	mov	r0, r4
8160420c:	ebffffd5 	bl	81604168 <env_set>
81604210:	e28dd018 	add	sp, sp, #24
81604214:	e8bd8010 	pop	{r4, pc}
81604218:	81615b8b 	.word	0x81615b8b

8160421c <do_env_export>:
8160421c:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81604220:	e1a04003 	mov	r4, r3
81604224:	e5933000 	ldr	r3, [r3]
81604228:	e3a06000 	mov	r6, #0
8160422c:	e24dd040 	sub	sp, sp, #64	; 0x40
81604230:	e1a0b000 	mov	fp, r0
81604234:	e1a05002 	mov	r5, r2
81604238:	e1a0a006 	mov	sl, r6
8160423c:	e3a0700a 	mov	r7, #10
81604240:	e1a08006 	mov	r8, r6
81604244:	e2451001 	sub	r1, r5, #1
81604248:	e3510000 	cmp	r1, #0
8160424c:	da00002a 	ble	816042fc <do_env_export+0xe0>
81604250:	e5940004 	ldr	r0, [r4, #4]
81604254:	e284c004 	add	ip, r4, #4
81604258:	e5d02000 	ldrb	r2, [r0]
8160425c:	e352002d 	cmp	r2, #45	; 0x2d
81604260:	0a000033 	beq	81604334 <do_env_export+0x118>
81604264:	e3a02010 	mov	r2, #16
81604268:	e3a01000 	mov	r1, #0
8160426c:	eb003949 	bl	81612798 <simple_strtoul>
81604270:	e3580000 	cmp	r8, #0
81604274:	e58d001c 	str	r0, [sp, #28]
81604278:	0a000002 	beq	81604288 <do_env_export+0x6c>
8160427c:	e1a02008 	mov	r2, r8
81604280:	e3a01000 	mov	r1, #0
81604284:	ebfff10d 	bl	816006c0 <memset>
81604288:	e3570000 	cmp	r7, #0
8160428c:	e2455002 	sub	r5, r5, #2
81604290:	e2844008 	add	r4, r4, #8
81604294:	0a000048 	beq	816043bc <do_env_export+0x1a0>
81604298:	e3a02050 	mov	r2, #80	; 0x50
8160429c:	e58d4008 	str	r4, [sp, #8]
816042a0:	e58d5004 	str	r5, [sp, #4]
816042a4:	e28d301c 	add	r3, sp, #28
816042a8:	e58d8000 	str	r8, [sp]
816042ac:	e1a01007 	mov	r1, r7
816042b0:	e59f018c 	ldr	r0, [pc, #396]	; 81604444 <do_env_export+0x228>
816042b4:	eb0031ef 	bl	81610a78 <hexport_r>
816042b8:	e2502000 	subs	r2, r0, #0
816042bc:	aa000036 	bge	8160439c <do_env_export+0x180>
816042c0:	e59f3180 	ldr	r3, [pc, #384]	; 81604448 <do_env_export+0x22c>
816042c4:	e58d3000 	str	r3, [sp]
816042c8:	e30033a2 	movw	r3, #930	; 0x3a2
816042cc:	e59f1178 	ldr	r1, [pc, #376]	; 8160444c <do_env_export+0x230>
816042d0:	e59f2178 	ldr	r2, [pc, #376]	; 81604450 <do_env_export+0x234>
816042d4:	e59f0178 	ldr	r0, [pc, #376]	; 81604454 <do_env_export+0x238>
816042d8:	e5911000 	ldr	r1, [r1]
816042dc:	eb0038e6 	bl	8161267c <printf>
816042e0:	e3a00001 	mov	r0, #1
816042e4:	ea000021 	b	81604370 <do_env_export+0x154>
816042e8:	e3520063 	cmp	r2, #99	; 0x63
816042ec:	0a000016 	beq	8160434c <do_env_export+0x130>
816042f0:	8a000003 	bhi	81604304 <do_env_export+0xe8>
816042f4:	e3520062 	cmp	r2, #98	; 0x62
816042f8:	0a000009 	beq	81604324 <do_env_export+0x108>
816042fc:	e3e00000 	mvn	r0, #0
81604300:	ea00001a 	b	81604370 <do_env_export+0x154>
81604304:	e3520073 	cmp	r2, #115	; 0x73
81604308:	0a000013 	beq	8160435c <do_env_export+0x140>
8160430c:	e3520074 	cmp	r2, #116	; 0x74
81604310:	1afffff9 	bne	816042fc <do_env_export+0xe0>
81604314:	e3560000 	cmp	r6, #0
81604318:	1a000045 	bne	81604434 <do_env_export+0x218>
8160431c:	e3a0700a 	mov	r7, #10
81604320:	ea000002 	b	81604330 <do_env_export+0x114>
81604324:	e3560000 	cmp	r6, #0
81604328:	1a000041 	bne	81604434 <do_env_export+0x218>
8160432c:	e1a07006 	mov	r7, r6
81604330:	e3a06001 	mov	r6, #1
81604334:	e5f02001 	ldrb	r2, [r0, #1]!
81604338:	e3520000 	cmp	r2, #0
8160433c:	1affffe9 	bne	816042e8 <do_env_export+0xcc>
81604340:	e1a0400c 	mov	r4, ip
81604344:	e1a05001 	mov	r5, r1
81604348:	eaffffbd 	b	81604244 <do_env_export+0x28>
8160434c:	e3560000 	cmp	r6, #0
81604350:	1a000037 	bne	81604434 <do_env_export+0x218>
81604354:	e3a0a001 	mov	sl, #1
81604358:	eafffff3 	b	8160432c <do_env_export+0x110>
8160435c:	e2455002 	sub	r5, r5, #2
81604360:	e3550000 	cmp	r5, #0
81604364:	ca000003 	bgt	81604378 <do_env_export+0x15c>
81604368:	e1a0000b 	mov	r0, fp
8160436c:	eb0012d6 	bl	81608ecc <cmd_usage>
81604370:	e28dd040 	add	sp, sp, #64	; 0x40
81604374:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81604378:	e3a02010 	mov	r2, #16
8160437c:	e3a01000 	mov	r1, #0
81604380:	e59c0004 	ldr	r0, [ip, #4]
81604384:	e2844008 	add	r4, r4, #8
81604388:	e58d3014 	str	r3, [sp, #20]
8160438c:	eb003901 	bl	81612798 <simple_strtoul>
81604390:	e59d3014 	ldr	r3, [sp, #20]
81604394:	e1a08000 	mov	r8, r0
81604398:	eaffffa9 	b	81604244 <do_env_export+0x28>
8160439c:	e59f10b4 	ldr	r1, [pc, #180]	; 81604458 <do_env_export+0x23c>
816043a0:	e28d0020 	add	r0, sp, #32
816043a4:	eb0038aa 	bl	81612654 <sprintf>
816043a8:	e28d1020 	add	r1, sp, #32
816043ac:	e59f00a8 	ldr	r0, [pc, #168]	; 8160445c <do_env_export+0x240>
816043b0:	ebffff6c 	bl	81604168 <env_set>
816043b4:	e3a00000 	mov	r0, #0
816043b8:	eaffffec 	b	81604370 <do_env_export+0x154>
816043bc:	e59d601c 	ldr	r6, [sp, #28]
816043c0:	e35a0000 	cmp	sl, #0
816043c4:	e58d4008 	str	r4, [sp, #8]
816043c8:	e3a02050 	mov	r2, #80	; 0x50
816043cc:	12863004 	addne	r3, r6, #4
816043d0:	e58d5004 	str	r5, [sp, #4]
816043d4:	158d3020 	strne	r3, [sp, #32]
816043d8:	e3074ffc 	movw	r4, #32764	; 0x7ffc
816043dc:	e28d3020 	add	r3, sp, #32
816043e0:	e58d4000 	str	r4, [sp]
816043e4:	e3a01000 	mov	r1, #0
816043e8:	e59f0054 	ldr	r0, [pc, #84]	; 81604444 <do_env_export+0x228>
816043ec:	058d6020 	streq	r6, [sp, #32]
816043f0:	eb0031a0 	bl	81610a78 <hexport_r>
816043f4:	e2505000 	subs	r5, r0, #0
816043f8:	b59f3048 	ldrlt	r3, [pc, #72]	; 81604448 <do_env_export+0x22c>
816043fc:	b58d3000 	strlt	r3, [sp]
81604400:	b30033b6 	movwlt	r3, #950	; 0x3b6
81604404:	baffffb0 	blt	816042cc <do_env_export+0xb0>
81604408:	e35a0000 	cmp	sl, #0
8160440c:	0a000004 	beq	81604424 <do_env_export+0x208>
81604410:	e1a02004 	mov	r2, r4
81604414:	e2861004 	add	r1, r6, #4
81604418:	e3a00000 	mov	r0, #0
8160441c:	eb003498 	bl	81611684 <crc32>
81604420:	e5860000 	str	r0, [r6]
81604424:	e2851004 	add	r1, r5, #4
81604428:	e59f002c 	ldr	r0, [pc, #44]	; 8160445c <do_env_export+0x240>
8160442c:	ebffff6d 	bl	816041e8 <env_set_hex>
81604430:	eaffffdf 	b	816043b4 <do_env_export+0x198>
81604434:	e1a01003 	mov	r1, r3
81604438:	e59f0020 	ldr	r0, [pc, #32]	; 81604460 <do_env_export+0x244>
8160443c:	eb00388e 	bl	8161267c <printf>
81604440:	eaffffa6 	b	816042e0 <do_env_export+0xc4>
81604444:	816190f0 	.word	0x816190f0
81604448:	81614096 	.word	0x81614096
8160444c:	81619e84 	.word	0x81619e84
81604450:	816151fa 	.word	0x816151fa
81604454:	8161510c 	.word	0x8161510c
81604458:	81615149 	.word	0x81615149
8160445c:	8161514d 	.word	0x8161514d
81604460:	81615244 	.word	0x81615244

81604464 <envmatch>:
81604464:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81604468:	e2504000 	subs	r4, r0, #0
8160446c:	11a05001 	movne	r5, r1
81604470:	1a000005 	bne	8160448c <envmatch+0x28>
81604474:	e3e06000 	mvn	r6, #0
81604478:	ea000012 	b	816044c8 <envmatch+0x64>
8160447c:	e5543001 	ldrb	r3, [r4, #-1]
81604480:	e1a05006 	mov	r5, r6
81604484:	e353003d 	cmp	r3, #61	; 0x3d
81604488:	0a00000e 	beq	816044c8 <envmatch+0x64>
8160448c:	e1a07004 	mov	r7, r4
81604490:	e1a00005 	mov	r0, r5
81604494:	e5d78000 	ldrb	r8, [r7]
81604498:	eb001c88 	bl	8160b6c0 <env_get_char>
8160449c:	e2844001 	add	r4, r4, #1
816044a0:	e2856001 	add	r6, r5, #1
816044a4:	e1580000 	cmp	r8, r0
816044a8:	0afffff3 	beq	8160447c <envmatch+0x18>
816044ac:	e5d73000 	ldrb	r3, [r7]
816044b0:	e3530000 	cmp	r3, #0
816044b4:	1affffee 	bne	81604474 <envmatch+0x10>
816044b8:	e1a00005 	mov	r0, r5
816044bc:	eb001c7f 	bl	8160b6c0 <env_get_char>
816044c0:	e350003d 	cmp	r0, #61	; 0x3d
816044c4:	1affffea 	bne	81604474 <envmatch+0x10>
816044c8:	e1a00006 	mov	r0, r6
816044cc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

816044d0 <env_get_f>:
816044d0:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816044d4:	e1a07000 	mov	r7, r0
816044d8:	e1a06001 	mov	r6, r1
816044dc:	e1a08002 	mov	r8, r2
816044e0:	e3a05000 	mov	r5, #0
816044e4:	e1a00005 	mov	r0, r5
816044e8:	eb001c74 	bl	8160b6c0 <env_get_char>
816044ec:	e3500000 	cmp	r0, #0
816044f0:	1a00001c 	bne	81604568 <env_get_f+0x98>
816044f4:	e3e05000 	mvn	r5, #0
816044f8:	ea000023 	b	8160458c <env_get_f+0xbc>
816044fc:	e3540902 	cmp	r4, #32768	; 0x8000
81604500:	aafffffb 	bge	816044f4 <env_get_f+0x24>
81604504:	e2844001 	add	r4, r4, #1
81604508:	e1a00004 	mov	r0, r4
8160450c:	eb001c6b 	bl	8160b6c0 <env_get_char>
81604510:	e3500000 	cmp	r0, #0
81604514:	1afffff8 	bne	816044fc <env_get_f+0x2c>
81604518:	e1a01005 	mov	r1, r5
8160451c:	e1a00007 	mov	r0, r7
81604520:	ebffffcf 	bl	81604464 <envmatch>
81604524:	e3500000 	cmp	r0, #0
81604528:	ba000010 	blt	81604570 <env_get_f+0xa0>
8160452c:	e086b008 	add	fp, r6, r8
81604530:	e040a006 	sub	sl, r0, r6
81604534:	e1a04006 	mov	r4, r6
81604538:	e154000b 	cmp	r4, fp
8160453c:	e08a0004 	add	r0, sl, r4
81604540:	e0445006 	sub	r5, r4, r6
81604544:	1a00000b 	bne	81604578 <env_get_f+0xa8>
81604548:	e3550000 	cmp	r5, #0
8160454c:	e1a02007 	mov	r2, r7
81604550:	e1a01008 	mov	r1, r8
81604554:	e59f0038 	ldr	r0, [pc, #56]	; 81604594 <env_get_f+0xc4>
81604558:	13a03000 	movne	r3, #0
8160455c:	15443001 	strbne	r3, [r4, #-1]
81604560:	eb003845 	bl	8161267c <printf>
81604564:	ea000008 	b	8160458c <env_get_f+0xbc>
81604568:	e1a04005 	mov	r4, r5
8160456c:	eaffffe5 	b	81604508 <env_get_f+0x38>
81604570:	e2845001 	add	r5, r4, #1
81604574:	eaffffda 	b	816044e4 <env_get_f+0x14>
81604578:	eb001c50 	bl	8160b6c0 <env_get_char>
8160457c:	e6ef0070 	uxtb	r0, r0
81604580:	e4c40001 	strb	r0, [r4], #1
81604584:	e3500000 	cmp	r0, #0
81604588:	1affffea 	bne	81604538 <env_get_f+0x68>
8160458c:	e1a00005 	mov	r0, r5
81604590:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81604594:	81615b28 	.word	0x81615b28

81604598 <env_get>:
81604598:	e5993004 	ldr	r3, [r9, #4]
8160459c:	e92d4010 	push	{r4, lr}
816045a0:	e2134080 	ands	r4, r3, #128	; 0x80
816045a4:	e24dd028 	sub	sp, sp, #40	; 0x28
816045a8:	0a000010 	beq	816045f0 <env_get+0x58>
816045ac:	e59f2058 	ldr	r2, [pc, #88]	; 8160460c <env_get+0x74>
816045b0:	e3a03000 	mov	r3, #0
816045b4:	e58d301c 	str	r3, [sp, #28]
816045b8:	e58d300c 	str	r3, [sp, #12]
816045bc:	e58d3000 	str	r3, [sp]
816045c0:	e28d3028 	add	r3, sp, #40	; 0x28
816045c4:	e58d2008 	str	r2, [sp, #8]
816045c8:	e28d2014 	add	r2, sp, #20
816045cc:	e58d0018 	str	r0, [sp, #24]
816045d0:	e58d2004 	str	r2, [sp, #4]
816045d4:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
816045d8:	eb002fc9 	bl	81610504 <hsearch_r>
816045dc:	e59d0014 	ldr	r0, [sp, #20]
816045e0:	e3500000 	cmp	r0, #0
816045e4:	15900004 	ldrne	r0, [r0, #4]
816045e8:	e28dd028 	add	sp, sp, #40	; 0x28
816045ec:	e8bd8010 	pop	{r4, pc}
816045f0:	e3a02020 	mov	r2, #32
816045f4:	e2891058 	add	r1, r9, #88	; 0x58
816045f8:	ebffffb4 	bl	816044d0 <env_get_f>
816045fc:	e3500000 	cmp	r0, #0
81604600:	c2890058 	addgt	r0, r9, #88	; 0x58
81604604:	d1a00004 	movle	r0, r4
81604608:	eafffff6 	b	816045e8 <env_get+0x50>
8160460c:	816190f0 	.word	0x816190f0

81604610 <env_get_hex>:
81604610:	e92d4037 	push	{r0, r1, r2, r4, r5, lr}
81604614:	e1a05001 	mov	r5, r1
81604618:	ebffffde 	bl	81604598 <env_get>
8160461c:	e2504000 	subs	r4, r0, #0
81604620:	1a000002 	bne	81604630 <env_get_hex+0x20>
81604624:	e1a00005 	mov	r0, r5
81604628:	e28dd00c 	add	sp, sp, #12
8160462c:	e8bd8030 	pop	{r4, r5, pc}
81604630:	e3a02010 	mov	r2, #16
81604634:	e28d1004 	add	r1, sp, #4
81604638:	eb003856 	bl	81612798 <simple_strtoul>
8160463c:	e59d3004 	ldr	r3, [sp, #4]
81604640:	e1530004 	cmp	r3, r4
81604644:	1afffff7 	bne	81604628 <env_get_hex+0x18>
81604648:	eafffff5 	b	81604624 <env_get_hex+0x14>

8160464c <env_get_ulong>:
8160464c:	e92d4070 	push	{r4, r5, r6, lr}
81604650:	e1a05001 	mov	r5, r1
81604654:	e1a04002 	mov	r4, r2
81604658:	ebffffce 	bl	81604598 <env_get>
8160465c:	e3500000 	cmp	r0, #0
81604660:	0a000003 	beq	81604674 <env_get_ulong+0x28>
81604664:	e1a02005 	mov	r2, r5
81604668:	e3a01000 	mov	r1, #0
8160466c:	e8bd4070 	pop	{r4, r5, r6, lr}
81604670:	ea003848 	b	81612798 <simple_strtoul>
81604674:	e1a00004 	mov	r0, r4
81604678:	e8bd8070 	pop	{r4, r5, r6, pc}

8160467c <board_init_f_alloc_reserve>:
8160467c:	e24000a8 	sub	r0, r0, #168	; 0xa8
81604680:	e3c0000f 	bic	r0, r0, #15
81604684:	e12fff1e 	bx	lr

81604688 <board_init_f_init_reserve>:
81604688:	e3a020a8 	mov	r2, #168	; 0xa8
8160468c:	e3a01000 	mov	r1, #0
81604690:	eafff00a 	b	816006c0 <memset>

81604694 <show_boot_progress>:
81604694:	e12fff1e 	bx	lr
81604698:	e12fff1e 	bx	lr

8160469c <main_loop>:
8160469c:	e92d4010 	push	{r4, lr}
816046a0:	e3a000ac 	mov	r0, #172	; 0xac
816046a4:	ebfffffa 	bl	81604694 <show_boot_progress>
816046a8:	eb0011d3 	bl	81608dfc <cli_init>
816046ac:	eb0000b3 	bl	81604980 <bootdelay_process>
816046b0:	e3a02000 	mov	r2, #0
816046b4:	e3e01000 	mvn	r1, #0
816046b8:	e59f0004 	ldr	r0, [pc, #4]	; 816046c4 <main_loop+0x28>
816046bc:	eb00118c 	bl	81608cf4 <run_command_list>
816046c0:	eafffffe 	b	816046c0 <main_loop+0x24>
816046c4:	81618c20 	.word	0x81618c20

816046c8 <mdelay>:
816046c8:	e92d4010 	push	{r4, lr}
816046cc:	e1a04000 	mov	r4, r0
816046d0:	e2544001 	subs	r4, r4, #1
816046d4:	38bd8010 	popcc	{r4, pc}
816046d8:	e3a00ffa 	mov	r0, #1000	; 0x3e8
816046dc:	eb003532 	bl	81611bac <udelay>
816046e0:	eafffffa 	b	816046d0 <mdelay+0x8>

816046e4 <dummy>:
816046e4:	e12fff1e 	bx	lr

816046e8 <get_version>:
816046e8:	e3a00009 	mov	r0, #9
816046ec:	e12fff1e 	bx	lr

816046f0 <jumptable_init>:
816046f0:	e92d4010 	push	{r4, lr}
816046f4:	e3a00084 	mov	r0, #132	; 0x84
816046f8:	e1a04009 	mov	r4, r9
816046fc:	eb000b58 	bl	81607464 <malloc>
81604700:	e59f1168 	ldr	r1, [pc, #360]	; 81604870 <jumptable_init+0x180>
81604704:	e5840054 	str	r0, [r4, #84]	; 0x54
81604708:	e5992054 	ldr	r2, [r9, #84]	; 0x54
8160470c:	e59f0160 	ldr	r0, [pc, #352]	; 81604874 <jumptable_init+0x184>
81604710:	e5821000 	str	r1, [r2]
81604714:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604718:	e59f1158 	ldr	r1, [pc, #344]	; 81604878 <jumptable_init+0x188>
8160471c:	e5821004 	str	r1, [r2, #4]
81604720:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604724:	e59f1150 	ldr	r1, [pc, #336]	; 8160487c <jumptable_init+0x18c>
81604728:	e5821008 	str	r1, [r2, #8]
8160472c:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604730:	e59f1148 	ldr	r1, [pc, #328]	; 81604880 <jumptable_init+0x190>
81604734:	e582100c 	str	r1, [r2, #12]
81604738:	e5992054 	ldr	r2, [r9, #84]	; 0x54
8160473c:	e59f1140 	ldr	r1, [pc, #320]	; 81604884 <jumptable_init+0x194>
81604740:	e5821010 	str	r1, [r2, #16]
81604744:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604748:	e59f1138 	ldr	r1, [pc, #312]	; 81604888 <jumptable_init+0x198>
8160474c:	e5821014 	str	r1, [r2, #20]
81604750:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604754:	e59f2130 	ldr	r2, [pc, #304]	; 8160488c <jumptable_init+0x19c>
81604758:	e5812018 	str	r2, [r1, #24]
8160475c:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604760:	e581201c 	str	r2, [r1, #28]
81604764:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604768:	e5810020 	str	r0, [r1, #32]
8160476c:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604770:	e59f0118 	ldr	r0, [pc, #280]	; 81604890 <jumptable_init+0x1a0>
81604774:	e5810024 	str	r0, [r1, #36]	; 0x24
81604778:	e5991054 	ldr	r1, [r9, #84]	; 0x54
8160477c:	e59f0110 	ldr	r0, [pc, #272]	; 81604894 <jumptable_init+0x1a4>
81604780:	e5810028 	str	r0, [r1, #40]	; 0x28
81604784:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604788:	e59f0108 	ldr	r0, [pc, #264]	; 81604898 <jumptable_init+0x1a8>
8160478c:	e581002c 	str	r0, [r1, #44]	; 0x2c
81604790:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604794:	e59f0100 	ldr	r0, [pc, #256]	; 8160489c <jumptable_init+0x1ac>
81604798:	e5810030 	str	r0, [r1, #48]	; 0x30
8160479c:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047a0:	e59f00f8 	ldr	r0, [pc, #248]	; 816048a0 <jumptable_init+0x1b0>
816047a4:	e5810034 	str	r0, [r1, #52]	; 0x34
816047a8:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047ac:	e59f00f0 	ldr	r0, [pc, #240]	; 816048a4 <jumptable_init+0x1b4>
816047b0:	e5810038 	str	r0, [r1, #56]	; 0x38
816047b4:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047b8:	e59f00e8 	ldr	r0, [pc, #232]	; 816048a8 <jumptable_init+0x1b8>
816047bc:	e581003c 	str	r0, [r1, #60]	; 0x3c
816047c0:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047c4:	e59f00e0 	ldr	r0, [pc, #224]	; 816048ac <jumptable_init+0x1bc>
816047c8:	e5810040 	str	r0, [r1, #64]	; 0x40
816047cc:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047d0:	e59f00d8 	ldr	r0, [pc, #216]	; 816048b0 <jumptable_init+0x1c0>
816047d4:	e5810044 	str	r0, [r1, #68]	; 0x44
816047d8:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047dc:	e59f00d0 	ldr	r0, [pc, #208]	; 816048b4 <jumptable_init+0x1c4>
816047e0:	e5810048 	str	r0, [r1, #72]	; 0x48
816047e4:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047e8:	e59f00c8 	ldr	r0, [pc, #200]	; 816048b8 <jumptable_init+0x1c8>
816047ec:	e581004c 	str	r0, [r1, #76]	; 0x4c
816047f0:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047f4:	e5812050 	str	r2, [r1, #80]	; 0x50
816047f8:	e5991054 	ldr	r1, [r9, #84]	; 0x54
816047fc:	e5812054 	str	r2, [r1, #84]	; 0x54
81604800:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604804:	e5812058 	str	r2, [r1, #88]	; 0x58
81604808:	e5991054 	ldr	r1, [r9, #84]	; 0x54
8160480c:	e581205c 	str	r2, [r1, #92]	; 0x5c
81604810:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604814:	e5812060 	str	r2, [r1, #96]	; 0x60
81604818:	e5991054 	ldr	r1, [r9, #84]	; 0x54
8160481c:	e5812064 	str	r2, [r1, #100]	; 0x64
81604820:	e5991054 	ldr	r1, [r9, #84]	; 0x54
81604824:	e5812068 	str	r2, [r1, #104]	; 0x68
81604828:	e5991054 	ldr	r1, [r9, #84]	; 0x54
8160482c:	e581206c 	str	r2, [r1, #108]	; 0x6c
81604830:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604834:	e59f1080 	ldr	r1, [pc, #128]	; 816048bc <jumptable_init+0x1cc>
81604838:	e5821070 	str	r1, [r2, #112]	; 0x70
8160483c:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604840:	e59f1078 	ldr	r1, [pc, #120]	; 816048c0 <jumptable_init+0x1d0>
81604844:	e5821074 	str	r1, [r2, #116]	; 0x74
81604848:	e5992054 	ldr	r2, [r9, #84]	; 0x54
8160484c:	e59f1070 	ldr	r1, [pc, #112]	; 816048c4 <jumptable_init+0x1d4>
81604850:	e5821078 	str	r1, [r2, #120]	; 0x78
81604854:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81604858:	e59f1068 	ldr	r1, [pc, #104]	; 816048c8 <jumptable_init+0x1d8>
8160485c:	e582107c 	str	r1, [r2, #124]	; 0x7c
81604860:	e5993054 	ldr	r3, [r9, #84]	; 0x54
81604864:	e59f2060 	ldr	r2, [pc, #96]	; 816048cc <jumptable_init+0x1dc>
81604868:	e5832080 	str	r2, [r3, #128]	; 0x80
8160486c:	e8bd8010 	pop	{r4, pc}
81604870:	816046e8 	.word	0x816046e8
81604874:	81607464 	.word	0x81607464
81604878:	81606d00 	.word	0x81606d00
8160487c:	81606d2c 	.word	0x81606d2c
81604880:	81606d6c 	.word	0x81606d6c
81604884:	81606d94 	.word	0x81606d94
81604888:	8161267c 	.word	0x8161267c
8160488c:	816046e4 	.word	0x816046e4
81604890:	81607238 	.word	0x81607238
81604894:	81611bac 	.word	0x81611bac
81604898:	81611b90 	.word	0x81611b90
8160489c:	816126c0 	.word	0x816126c0
816048a0:	8160117c 	.word	0x8160117c
816048a4:	81604598 	.word	0x81604598
816048a8:	81604168 	.word	0x81604168
816048ac:	81612798 	.word	0x81612798
816048b0:	81612848 	.word	0x81612848
816048b4:	816128c0 	.word	0x816128c0
816048b8:	81611840 	.word	0x81611840
816048bc:	816128e4 	.word	0x816128e4
816048c0:	81612a24 	.word	0x81612a24
816048c4:	81611804 	.word	0x81611804
816048c8:	816046c8 	.word	0x816046c8
816048cc:	816006c0 	.word	0x816006c0

816048d0 <hash_command>:
816048d0:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816048d4:	e59d7020 	ldr	r7, [sp, #32]
816048d8:	e59d6024 	ldr	r6, [sp, #36]	; 0x24
816048dc:	e3570001 	cmp	r7, #1
816048e0:	da000023 	ble	81604974 <hash_command+0xa4>
816048e4:	e3570002 	cmp	r7, #2
816048e8:	13a04000 	movne	r4, #0
816048ec:	02014001 	andeq	r4, r1, #1
816048f0:	e3540000 	cmp	r4, #0
816048f4:	1a00001e 	bne	81604974 <hash_command+0xa4>
816048f8:	e3a02010 	mov	r2, #16
816048fc:	e1a01004 	mov	r1, r4
81604900:	e5960000 	ldr	r0, [r6]
81604904:	eb0037a3 	bl	81612798 <simple_strtoul>
81604908:	e3a02010 	mov	r2, #16
8160490c:	e1a01004 	mov	r1, r4
81604910:	e1a08000 	mov	r8, r0
81604914:	e5960004 	ldr	r0, [r6, #4]
81604918:	eb00379e 	bl	81612798 <simple_strtoul>
8160491c:	e3a03801 	mov	r3, #65536	; 0x10000
81604920:	e1a01008 	mov	r1, r8
81604924:	e1a02000 	mov	r2, r0
81604928:	e1a05000 	mov	r5, r0
8160492c:	e1a00004 	mov	r0, r4
81604930:	eb003358 	bl	81611698 <crc32_wd>
81604934:	e0882005 	add	r2, r8, r5
81604938:	e2422001 	sub	r2, r2, #1
8160493c:	e1a01008 	mov	r1, r8
81604940:	e1a03000 	mov	r3, r0
81604944:	e1a0a000 	mov	sl, r0
81604948:	e59f002c 	ldr	r0, [pc, #44]	; 8160497c <hash_command+0xac>
8160494c:	eb00374a 	bl	8161267c <printf>
81604950:	e3570002 	cmp	r7, #2
81604954:	0a000004 	beq	8160496c <hash_command+0x9c>
81604958:	e3a02010 	mov	r2, #16
8160495c:	e1a01004 	mov	r1, r4
81604960:	e5960008 	ldr	r0, [r6, #8]
81604964:	eb00378b 	bl	81612798 <simple_strtoul>
81604968:	e580a000 	str	sl, [r0]
8160496c:	e1a00004 	mov	r0, r4
81604970:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81604974:	e3e00000 	mvn	r0, #0
81604978:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160497c:	81615f43 	.word	0x81615f43

81604980 <bootdelay_process>:
81604980:	e92d4010 	push	{r4, lr}
81604984:	e59f0034 	ldr	r0, [pc, #52]	; 816049c0 <bootdelay_process+0x40>
81604988:	ebffff02 	bl	81604598 <env_get>
8160498c:	e2504000 	subs	r4, r0, #0
81604990:	0a000003 	beq	816049a4 <bootdelay_process+0x24>
81604994:	e3a0200a 	mov	r2, #10
81604998:	e3a01000 	mov	r1, #0
8160499c:	eb0037c7 	bl	816128c0 <simple_strtol>
816049a0:	e1a04000 	mov	r4, r0
816049a4:	e59f0018 	ldr	r0, [pc, #24]	; 816049c4 <bootdelay_process+0x44>
816049a8:	ebfffefa 	bl	81604598 <env_get>
816049ac:	e1a03009 	mov	r3, r9
816049b0:	e5933048 	ldr	r3, [r3, #72]	; 0x48
816049b4:	e59f300c 	ldr	r3, [pc, #12]	; 816049c8 <bootdelay_process+0x48>
816049b8:	e5834000 	str	r4, [r3]
816049bc:	e8bd8010 	pop	{r4, pc}
816049c0:	81614774 	.word	0x81614774
816049c4:	8161477e 	.word	0x8161477e
816049c8:	81619878 	.word	0x81619878

816049cc <display_text_info>:
816049cc:	e3a00000 	mov	r0, #0
816049d0:	e12fff1e 	bx	lr

816049d4 <setup_mon_len>:
816049d4:	e59f1010 	ldr	r1, [pc, #16]	; 816049ec <setup_mon_len+0x18>
816049d8:	e3a00000 	mov	r0, #0
816049dc:	e59f300c 	ldr	r3, [pc, #12]	; 816049f0 <setup_mon_len+0x1c>
816049e0:	e0433001 	sub	r3, r3, r1
816049e4:	e5893034 	str	r3, [r9, #52]	; 0x34
816049e8:	e12fff1e 	bx	lr
816049ec:	81600000 	.word	0x81600000
816049f0:	81619ea0 	.word	0x81619ea0

816049f4 <reserve_round_4k>:
816049f4:	e599302c 	ldr	r3, [r9, #44]	; 0x2c
816049f8:	e3a00000 	mov	r0, #0
816049fc:	e3c33eff 	bic	r3, r3, #4080	; 0xff0
81604a00:	e3c3300f 	bic	r3, r3, #15
81604a04:	e589302c 	str	r3, [r9, #44]	; 0x2c
81604a08:	e12fff1e 	bx	lr

81604a0c <reserve_mmu>:
81604a0c:	e3a02a05 	mov	r2, #20480	; 0x5000
81604a10:	e58920a4 	str	r2, [r9, #164]	; 0xa4
81604a14:	e59910a4 	ldr	r1, [r9, #164]	; 0xa4
81604a18:	e3a00000 	mov	r0, #0
81604a1c:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
81604a20:	e0422001 	sub	r2, r2, r1
81604a24:	e589202c 	str	r2, [r9, #44]	; 0x2c
81604a28:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
81604a2c:	e1a02822 	lsr	r2, r2, #16
81604a30:	e1a02802 	lsl	r2, r2, #16
81604a34:	e589202c 	str	r2, [r9, #44]	; 0x2c
81604a38:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
81604a3c:	e58920a0 	str	r2, [r9, #160]	; 0xa0
81604a40:	e12fff1e 	bx	lr

81604a44 <reserve_uboot>:
81604a44:	e5991034 	ldr	r1, [r9, #52]	; 0x34
81604a48:	e3a00000 	mov	r0, #0
81604a4c:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
81604a50:	e0422001 	sub	r2, r2, r1
81604a54:	e589202c 	str	r2, [r9, #44]	; 0x2c
81604a58:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
81604a5c:	e3c22eff 	bic	r2, r2, #4080	; 0xff0
81604a60:	e3c2200f 	bic	r2, r2, #15
81604a64:	e589202c 	str	r2, [r9, #44]	; 0x2c
81604a68:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
81604a6c:	e589203c 	str	r2, [r9, #60]	; 0x3c
81604a70:	e12fff1e 	bx	lr

81604a74 <reserve_malloc>:
81604a74:	e599303c 	ldr	r3, [r9, #60]	; 0x3c
81604a78:	e3a00000 	mov	r0, #0
81604a7c:	e2433803 	sub	r3, r3, #196608	; 0x30000
81604a80:	e589303c 	str	r3, [r9, #60]	; 0x3c
81604a84:	e12fff1e 	bx	lr

81604a88 <reserve_global_data>:
81604a88:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604a8c:	e3a00000 	mov	r0, #0
81604a90:	e24220a8 	sub	r2, r2, #168	; 0xa8
81604a94:	e589203c 	str	r2, [r9, #60]	; 0x3c
81604a98:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604a9c:	e5892044 	str	r2, [r9, #68]	; 0x44
81604aa0:	e12fff1e 	bx	lr

81604aa4 <reserve_fdt>:
81604aa4:	e5992048 	ldr	r2, [r9, #72]	; 0x48
81604aa8:	e3520000 	cmp	r2, #0
81604aac:	0a00000d 	beq	81604ae8 <reserve_fdt+0x44>
81604ab0:	e5992048 	ldr	r2, [r9, #72]	; 0x48
81604ab4:	e5922004 	ldr	r2, [r2, #4]
81604ab8:	e6bf2f32 	rev	r2, r2
81604abc:	e2822a01 	add	r2, r2, #4096	; 0x1000
81604ac0:	e282201f 	add	r2, r2, #31
81604ac4:	e3c2201f 	bic	r2, r2, #31
81604ac8:	e5892050 	str	r2, [r9, #80]	; 0x50
81604acc:	e5991050 	ldr	r1, [r9, #80]	; 0x50
81604ad0:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604ad4:	e0422001 	sub	r2, r2, r1
81604ad8:	e589203c 	str	r2, [r9, #60]	; 0x3c
81604adc:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604ae0:	e5991050 	ldr	r1, [r9, #80]	; 0x50
81604ae4:	e589204c 	str	r2, [r9, #76]	; 0x4c
81604ae8:	e3a00000 	mov	r0, #0
81604aec:	e12fff1e 	bx	lr

81604af0 <setup_reloc>:
81604af0:	e5993004 	ldr	r3, [r9, #4]
81604af4:	e3130b02 	tst	r3, #2048	; 0x800
81604af8:	1a00000a 	bne	81604b28 <setup_reloc+0x38>
81604afc:	e599302c 	ldr	r3, [r9, #44]	; 0x2c
81604b00:	e1a01009 	mov	r1, r9
81604b04:	e92d4010 	push	{r4, lr}
81604b08:	e283347e 	add	r3, r3, #2113929216	; 0x7e000000
81604b0c:	e283360a 	add	r3, r3, #10485760	; 0xa00000
81604b10:	e3a020a8 	mov	r2, #168	; 0xa8
81604b14:	e5893040 	str	r3, [r9, #64]	; 0x40
81604b18:	e5990044 	ldr	r0, [r9, #68]	; 0x44
81604b1c:	ebffef1e 	bl	8160079c <memcpy>
81604b20:	e3a00000 	mov	r0, #0
81604b24:	e8bd8010 	pop	{r4, pc}
81604b28:	e3a00000 	mov	r0, #0
81604b2c:	e12fff1e 	bx	lr

81604b30 <reloc_fdt>:
81604b30:	e5992004 	ldr	r2, [r9, #4]
81604b34:	e3120b02 	tst	r2, #2048	; 0x800
81604b38:	1a00000b 	bne	81604b6c <reloc_fdt+0x3c>
81604b3c:	e599204c 	ldr	r2, [r9, #76]	; 0x4c
81604b40:	e3520000 	cmp	r2, #0
81604b44:	0a000008 	beq	81604b6c <reloc_fdt+0x3c>
81604b48:	e599004c 	ldr	r0, [r9, #76]	; 0x4c
81604b4c:	e92d4010 	push	{r4, lr}
81604b50:	e5991048 	ldr	r1, [r9, #72]	; 0x48
81604b54:	e5992050 	ldr	r2, [r9, #80]	; 0x50
81604b58:	ebffef0f 	bl	8160079c <memcpy>
81604b5c:	e3a00000 	mov	r0, #0
81604b60:	e599204c 	ldr	r2, [r9, #76]	; 0x4c
81604b64:	e5892048 	str	r2, [r9, #72]	; 0x48
81604b68:	e8bd8010 	pop	{r4, pc}
81604b6c:	e3a00000 	mov	r0, #0
81604b70:	e12fff1e 	bx	lr

81604b74 <reserve_board>:
81604b74:	e5991000 	ldr	r1, [r9]
81604b78:	e3510000 	cmp	r1, #0
81604b7c:	1a00000a 	bne	81604bac <reserve_board+0x38>
81604b80:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604b84:	e92d4010 	push	{r4, lr}
81604b88:	e2422050 	sub	r2, r2, #80	; 0x50
81604b8c:	e589203c 	str	r2, [r9, #60]	; 0x3c
81604b90:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604b94:	e5892000 	str	r2, [r9]
81604b98:	e3a02050 	mov	r2, #80	; 0x50
81604b9c:	e5990000 	ldr	r0, [r9]
81604ba0:	ebffeec6 	bl	816006c0 <memset>
81604ba4:	e3a00000 	mov	r0, #0
81604ba8:	e8bd8010 	pop	{r4, pc}
81604bac:	e3a00000 	mov	r0, #0
81604bb0:	e12fff1e 	bx	lr

81604bb4 <announce_dram_init>:
81604bb4:	e92d4010 	push	{r4, lr}
81604bb8:	e59f0008 	ldr	r0, [pc, #8]	; 81604bc8 <announce_dram_init+0x14>
81604bbc:	eb000874 	bl	81606d94 <puts>
81604bc0:	e3a00000 	mov	r0, #0
81604bc4:	e8bd8010 	pop	{r4, pc}
81604bc8:	816142bc 	.word	0x816142bc

81604bcc <init_baud_rate>:
81604bcc:	e92d4010 	push	{r4, lr}
81604bd0:	e3a0100a 	mov	r1, #10
81604bd4:	e59f2014 	ldr	r2, [pc, #20]	; 81604bf0 <init_baud_rate+0x24>
81604bd8:	e1a04009 	mov	r4, r9
81604bdc:	e59f0010 	ldr	r0, [pc, #16]	; 81604bf4 <init_baud_rate+0x28>
81604be0:	ebfffe99 	bl	8160464c <env_get_ulong>
81604be4:	e5840008 	str	r0, [r4, #8]
81604be8:	e3a00000 	mov	r0, #0
81604bec:	e8bd8010 	pop	{r4, pc}
81604bf0:	0001c200 	.word	0x0001c200
81604bf4:	816163e4 	.word	0x816163e4

81604bf8 <initf_bootstage>:
81604bf8:	e92d4010 	push	{r4, lr}
81604bfc:	e3a000a4 	mov	r0, #164	; 0xa4
81604c00:	ebfffea3 	bl	81604694 <show_boot_progress>
81604c04:	e3a00000 	mov	r0, #0
81604c08:	e8bd8010 	pop	{r4, pc}

81604c0c <arch_cpu_init_dm>:
81604c0c:	e3a00000 	mov	r0, #0
81604c10:	e12fff1e 	bx	lr

81604c14 <mach_cpu_init>:
81604c14:	e3a00000 	mov	r0, #0
81604c18:	e12fff1e 	bx	lr

81604c1c <reserve_video>:
81604c1c:	e3a00000 	mov	r0, #0
81604c20:	e12fff1e 	bx	lr

81604c24 <reserve_trace>:
81604c24:	e3a00000 	mov	r0, #0
81604c28:	e12fff1e 	bx	lr

81604c2c <setup_machine>:
81604c2c:	e3a00000 	mov	r0, #0
81604c30:	e12fff1e 	bx	lr

81604c34 <reserve_bootstage>:
81604c34:	e3a00000 	mov	r0, #0
81604c38:	e12fff1e 	bx	lr

81604c3c <display_new_sp>:
81604c3c:	e3a00000 	mov	r0, #0
81604c40:	e12fff1e 	bx	lr

81604c44 <reloc_bootstage>:
81604c44:	e3a00000 	mov	r0, #0
81604c48:	e12fff1e 	bx	lr

81604c4c <initf_console_record>:
81604c4c:	e3a00000 	mov	r0, #0
81604c50:	e12fff1e 	bx	lr

81604c54 <initf_dm>:
81604c54:	e3a00000 	mov	r0, #0
81604c58:	e12fff1e 	bx	lr

81604c5c <reserve_arch>:
81604c5c:	e3a00000 	mov	r0, #0
81604c60:	e12fff1e 	bx	lr

81604c64 <coloured_LED_init>:
81604c64:	e12fff1e 	bx	lr

81604c68 <red_led_on>:
81604c68:	e12fff1e 	bx	lr

81604c6c <board_add_ram_info>:
81604c6c:	e12fff1e 	bx	lr

81604c70 <show_dram_config>:
81604c70:	e5993000 	ldr	r3, [r9]
81604c74:	e92d4010 	push	{r4, lr}
81604c78:	e3a04000 	mov	r4, #0
81604c7c:	e593004c 	ldr	r0, [r3, #76]	; 0x4c
81604c80:	e1a01004 	mov	r1, r4
81604c84:	e59f2018 	ldr	r2, [pc, #24]	; 81604ca4 <show_dram_config+0x34>
81604c88:	eb00318b 	bl	816112bc <print_size>
81604c8c:	e1a00004 	mov	r0, r4
81604c90:	ebfffff5 	bl	81604c6c <board_add_ram_info>
81604c94:	e3a0000a 	mov	r0, #10
81604c98:	eb000833 	bl	81606d6c <putc>
81604c9c:	e1a00004 	mov	r0, r4
81604ca0:	e8bd8010 	pop	{r4, pc}
81604ca4:	8161623b 	.word	0x8161623b

81604ca8 <board_get_usable_ram_top>:
81604ca8:	e5992028 	ldr	r2, [r9, #40]	; 0x28
81604cac:	e3520000 	cmp	r2, #0
81604cb0:	b5990028 	ldrlt	r0, [r9, #40]	; 0x28
81604cb4:	a3a00000 	movge	r0, #0
81604cb8:	e12fff1e 	bx	lr

81604cbc <setup_dest_addr>:
81604cbc:	e92d4010 	push	{r4, lr}
81604cc0:	e3a02102 	mov	r2, #-2147483648	; 0x80000000
81604cc4:	e5892028 	str	r2, [r9, #40]	; 0x28
81604cc8:	eb000f9c 	bl	81608b40 <get_effective_memsize>
81604ccc:	e5993028 	ldr	r3, [r9, #40]	; 0x28
81604cd0:	e1a04009 	mov	r4, r9
81604cd4:	e0800003 	add	r0, r0, r3
81604cd8:	e5890028 	str	r0, [r9, #40]	; 0x28
81604cdc:	e5990034 	ldr	r0, [r9, #52]	; 0x34
81604ce0:	ebfffff0 	bl	81604ca8 <board_get_usable_ram_top>
81604ce4:	e5840028 	str	r0, [r4, #40]	; 0x28
81604ce8:	e3a00000 	mov	r0, #0
81604cec:	e5992028 	ldr	r2, [r9, #40]	; 0x28
81604cf0:	e589202c 	str	r2, [r9, #44]	; 0x2c
81604cf4:	e8bd8010 	pop	{r4, pc}

81604cf8 <reserve_stacks>:
81604cf8:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604cfc:	e2422010 	sub	r2, r2, #16
81604d00:	e589203c 	str	r2, [r9, #60]	; 0x3c
81604d04:	e599203c 	ldr	r2, [r9, #60]	; 0x3c
81604d08:	e3c2200f 	bic	r2, r2, #15
81604d0c:	e589203c 	str	r2, [r9, #60]	; 0x3c
81604d10:	eafff059 	b	81600e7c <arch_reserve_stacks>

81604d14 <board_init_f>:
81604d14:	e92d4010 	push	{r4, lr}
81604d18:	e3a02000 	mov	r2, #0
81604d1c:	e5890004 	str	r0, [r9, #4]
81604d20:	e589201c 	str	r2, [r9, #28]
81604d24:	e59f000c 	ldr	r0, [pc, #12]	; 81604d38 <board_init_f+0x24>
81604d28:	eb002693 	bl	8160e77c <initcall_run_list>
81604d2c:	e3500000 	cmp	r0, #0
81604d30:	08bd8010 	popeq	{r4, pc}
81604d34:	eb00328a 	bl	81611764 <hang>
81604d38:	81613244 	.word	0x81613244

81604d3c <bootstage_relocate>:
81604d3c:	e3a00000 	mov	r0, #0
81604d40:	e12fff1e 	bx	lr

81604d44 <initr_trace>:
81604d44:	e3a00000 	mov	r0, #0
81604d48:	e12fff1e 	bx	lr

81604d4c <initr_reloc>:
81604d4c:	e5993004 	ldr	r3, [r9, #4]
81604d50:	e3a00000 	mov	r0, #0
81604d54:	e3833c02 	orr	r3, r3, #512	; 0x200
81604d58:	e3833001 	orr	r3, r3, #1
81604d5c:	e5893004 	str	r3, [r9, #4]
81604d60:	e12fff1e 	bx	lr

81604d64 <initr_reloc_global_data>:
81604d64:	e59f2014 	ldr	r2, [pc, #20]	; 81604d80 <initr_reloc_global_data+0x1c>
81604d68:	e3a00000 	mov	r0, #0
81604d6c:	e59f1010 	ldr	r1, [pc, #16]	; 81604d84 <initr_reloc_global_data+0x20>
81604d70:	e59f3010 	ldr	r3, [pc, #16]	; 81604d88 <initr_reloc_global_data+0x24>
81604d74:	e0433001 	sub	r3, r3, r1
81604d78:	e5823000 	str	r3, [r2]
81604d7c:	e12fff1e 	bx	lr
81604d80:	8161987c 	.word	0x8161987c
81604d84:	81600000 	.word	0x81600000
81604d88:	8161c468 	.word	0x8161c468

81604d8c <run_main_loop>:
81604d8c:	e92d4010 	push	{r4, lr}
81604d90:	ebfffe41 	bl	8160469c <main_loop>
81604d94:	eafffffd 	b	81604d90 <run_main_loop+0x4>

81604d98 <initr_enable_interrupts>:
81604d98:	e92d4010 	push	{r4, lr}
81604d9c:	ebfff044 	bl	81600eb4 <enable_interrupts>
81604da0:	e3a00000 	mov	r0, #0
81604da4:	e8bd8010 	pop	{r4, pc}

81604da8 <initr_jumptable>:
81604da8:	e92d4010 	push	{r4, lr}
81604dac:	ebfffe4f 	bl	816046f0 <jumptable_init>
81604db0:	e3a00000 	mov	r0, #0
81604db4:	e8bd8010 	pop	{r4, pc}

81604db8 <initr_env>:
81604db8:	e92d4010 	push	{r4, lr}
81604dbc:	eb0019f6 	bl	8160b59c <env_relocate>
81604dc0:	e3a01010 	mov	r1, #16
81604dc4:	e59f4014 	ldr	r4, [pc, #20]	; 81604de0 <initr_env+0x28>
81604dc8:	e59f0014 	ldr	r0, [pc, #20]	; 81604de4 <initr_env+0x2c>
81604dcc:	e5942000 	ldr	r2, [r4]
81604dd0:	ebfffe1d 	bl	8160464c <env_get_ulong>
81604dd4:	e5840000 	str	r0, [r4]
81604dd8:	e3a00000 	mov	r0, #0
81604ddc:	e8bd8010 	pop	{r4, pc}
81604de0:	816190ac 	.word	0x816190ac
81604de4:	81617340 	.word	0x81617340

81604de8 <initr_mmc>:
81604de8:	e92d4010 	push	{r4, lr}
81604dec:	e59f0010 	ldr	r0, [pc, #16]	; 81604e04 <initr_mmc+0x1c>
81604df0:	eb0007e7 	bl	81606d94 <puts>
81604df4:	e5990000 	ldr	r0, [r9]
81604df8:	eb001781 	bl	8160ac04 <mmc_initialize>
81604dfc:	e3a00000 	mov	r0, #0
81604e00:	e8bd8010 	pop	{r4, pc}
81604e04:	8161641e 	.word	0x8161641e

81604e08 <initr_serial>:
81604e08:	e92d4010 	push	{r4, lr}
81604e0c:	eb001878 	bl	8160aff4 <serial_initialize>
81604e10:	e3a00000 	mov	r0, #0
81604e14:	e8bd8010 	pop	{r4, pc}

81604e18 <initr_bootstage>:
81604e18:	e92d4010 	push	{r4, lr}
81604e1c:	e3a000a5 	mov	r0, #165	; 0xa5
81604e20:	ebfffe1b 	bl	81604694 <show_boot_progress>
81604e24:	e3a00000 	mov	r0, #0
81604e28:	e8bd8010 	pop	{r4, pc}

81604e2c <initr_malloc>:
81604e2c:	e599002c 	ldr	r0, [r9, #44]	; 0x2c
81604e30:	e3a01803 	mov	r1, #196608	; 0x30000
81604e34:	e92d4010 	push	{r4, lr}
81604e38:	e2400803 	sub	r0, r0, #196608	; 0x30000
81604e3c:	eb0008bc 	bl	81607134 <mem_malloc_init>
81604e40:	e3a00000 	mov	r0, #0
81604e44:	e8bd8010 	pop	{r4, pc}

81604e48 <initr_caches>:
81604e48:	e92d4010 	push	{r4, lr}
81604e4c:	ebfff18f 	bl	81601490 <enable_caches>
81604e50:	e3a00000 	mov	r0, #0
81604e54:	e8bd8010 	pop	{r4, pc}

81604e58 <initr_announce>:
81604e58:	e3a00000 	mov	r0, #0
81604e5c:	e12fff1e 	bx	lr

81604e60 <initr_barrier>:
81604e60:	e3a00000 	mov	r0, #0
81604e64:	e12fff1e 	bx	lr

81604e68 <initr_console_record>:
81604e68:	e3a00000 	mov	r0, #0
81604e6c:	e12fff1e 	bx	lr

81604e70 <power_init_board>:
81604e70:	e3a00000 	mov	r0, #0
81604e74:	e12fff1e 	bx	lr

81604e78 <cpu_secondary_init_r>:
81604e78:	e12fff1e 	bx	lr

81604e7c <initr_secondary_cpu>:
81604e7c:	e92d4010 	push	{r4, lr}
81604e80:	ebfffffc 	bl	81604e78 <cpu_secondary_init_r>
81604e84:	e3a00000 	mov	r0, #0
81604e88:	e8bd8010 	pop	{r4, pc}

81604e8c <board_init_r>:
81604e8c:	e92d4010 	push	{r4, lr}
81604e90:	e59f0004 	ldr	r0, [pc, #4]	; 81604e9c <board_init_r+0x10>
81604e94:	eb002638 	bl	8160e77c <initcall_run_list>
81604e98:	eb003231 	bl	81611764 <hang>
81604e9c:	81618c40 	.word	0x81618c40

81604ea0 <bootm_find_images>:
81604ea0:	e92d40f0 	push	{r4, r5, r6, r7, lr}
81604ea4:	e24dd014 	sub	sp, sp, #20
81604ea8:	e59f408c 	ldr	r4, [pc, #140]	; 81604f3c <bootm_find_images+0x9c>
81604eac:	e1a05001 	mov	r5, r1
81604eb0:	e1a06002 	mov	r6, r2
81604eb4:	e1a07000 	mov	r7, r0
81604eb8:	e2843068 	add	r3, r4, #104	; 0x68
81604ebc:	e1a01006 	mov	r1, r6
81604ec0:	e58d3004 	str	r3, [sp, #4]
81604ec4:	e1a02004 	mov	r2, r4
81604ec8:	e2843064 	add	r3, r4, #100	; 0x64
81604ecc:	e1a00005 	mov	r0, r5
81604ed0:	e58d3000 	str	r3, [sp]
81604ed4:	e3a03002 	mov	r3, #2
81604ed8:	eb000c5e 	bl	81608058 <boot_get_ramdisk>
81604edc:	e3500000 	cmp	r0, #0
81604ee0:	159f0058 	ldrne	r0, [pc, #88]	; 81604f40 <bootm_find_images+0xa0>
81604ee4:	1a00000c 	bne	81604f1c <bootm_find_images+0x7c>
81604ee8:	e2843070 	add	r3, r4, #112	; 0x70
81604eec:	e1a01005 	mov	r1, r5
81604ef0:	e58d3008 	str	r3, [sp, #8]
81604ef4:	e1a02006 	mov	r2, r6
81604ef8:	e284306c 	add	r3, r4, #108	; 0x6c
81604efc:	e58d4000 	str	r4, [sp]
81604f00:	e58d3004 	str	r3, [sp, #4]
81604f04:	e1a00007 	mov	r0, r7
81604f08:	e3a03002 	mov	r3, #2
81604f0c:	eb000dea 	bl	816086bc <boot_get_fdt>
81604f10:	e2505000 	subs	r5, r0, #0
81604f14:	0a000005 	beq	81604f30 <bootm_find_images+0x90>
81604f18:	e59f0024 	ldr	r0, [pc, #36]	; 81604f44 <bootm_find_images+0xa4>
81604f1c:	eb00079c 	bl	81606d94 <puts>
81604f20:	e3a05001 	mov	r5, #1
81604f24:	e1a00005 	mov	r0, r5
81604f28:	e28dd014 	add	sp, sp, #20
81604f2c:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81604f30:	e594006c 	ldr	r0, [r4, #108]	; 0x6c
81604f34:	ebfff550 	bl	8160247c <set_working_fdt_addr>
81604f38:	eafffff9 	b	81604f24 <bootm_find_images+0x84>
81604f3c:	81619880 	.word	0x81619880
81604f40:	81614842 	.word	0x81614842
81604f44:	81614867 	.word	0x81614867

81604f48 <bootm_decomp_image>:
81604f48:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81604f4c:	e1a05000 	mov	r5, r0
81604f50:	e59d7028 	ldr	r7, [sp, #40]	; 0x28
81604f54:	e6ef0073 	uxtb	r0, r3
81604f58:	e1a08002 	mov	r8, r2
81604f5c:	e1a04001 	mov	r4, r1
81604f60:	e59d6024 	ldr	r6, [sp, #36]	; 0x24
81604f64:	e5871000 	str	r1, [r7]
81604f68:	eb000ba9 	bl	81607e14 <genimg_get_type_name>
81604f6c:	e3550000 	cmp	r5, #0
81604f70:	e1a02000 	mov	r2, r0
81604f74:	1a00000e 	bne	81604fb4 <bootm_decomp_image+0x6c>
81604f78:	e59f30e4 	ldr	r3, [pc, #228]	; 81605064 <bootm_decomp_image+0x11c>
81604f7c:	e1540008 	cmp	r4, r8
81604f80:	e59f10e0 	ldr	r1, [pc, #224]	; 81605068 <bootm_decomp_image+0x120>
81604f84:	e59f00e0 	ldr	r0, [pc, #224]	; 8160506c <bootm_decomp_image+0x124>
81604f88:	01a01003 	moveq	r1, r3
81604f8c:	eb0035ba 	bl	8161267c <printf>
81604f90:	e1540008 	cmp	r4, r8
81604f94:	1a000010 	bne	81604fdc <bootm_decomp_image+0x94>
81604f98:	e59d3020 	ldr	r3, [sp, #32]
81604f9c:	e59f00cc 	ldr	r0, [pc, #204]	; 81605070 <bootm_decomp_image+0x128>
81604fa0:	e0834004 	add	r4, r3, r4
81604fa4:	e5874000 	str	r4, [r7]
81604fa8:	eb000779 	bl	81606d94 <puts>
81604fac:	e3a00000 	mov	r0, #0
81604fb0:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81604fb4:	e1a01000 	mov	r1, r0
81604fb8:	e59f00b4 	ldr	r0, [pc, #180]	; 81605074 <bootm_decomp_image+0x12c>
81604fbc:	eb0035ae 	bl	8161267c <printf>
81604fc0:	e3550001 	cmp	r5, #1
81604fc4:	0a00000d 	beq	81605000 <bootm_decomp_image+0xb8>
81604fc8:	e1a01005 	mov	r1, r5
81604fcc:	e59f00a4 	ldr	r0, [pc, #164]	; 81605078 <bootm_decomp_image+0x130>
81604fd0:	eb0035a9 	bl	8161267c <printf>
81604fd4:	e3e00002 	mvn	r0, #2
81604fd8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81604fdc:	e59d2020 	ldr	r2, [sp, #32]
81604fe0:	e1520006 	cmp	r2, r6
81604fe4:	83a08001 	movhi	r8, #1
81604fe8:	8a00000b 	bhi	8160501c <bootm_decomp_image+0xd4>
81604fec:	e3a03801 	mov	r3, #65536	; 0x10000
81604ff0:	e59d101c 	ldr	r1, [sp, #28]
81604ff4:	e59d0018 	ldr	r0, [sp, #24]
81604ff8:	eb000b59 	bl	81607d64 <memmove_wd>
81604ffc:	eaffffe5 	b	81604f98 <bootm_decomp_image+0x50>
81605000:	e28d3020 	add	r3, sp, #32
81605004:	e59d201c 	ldr	r2, [sp, #28]
81605008:	e1a01006 	mov	r1, r6
8160500c:	e59d0018 	ldr	r0, [sp, #24]
81605010:	eb0025ac 	bl	8160e6c8 <gunzip>
81605014:	e2508000 	subs	r8, r0, #0
81605018:	0affffde 	beq	81604f98 <bootm_decomp_image+0x50>
8160501c:	e6ef0075 	uxtb	r0, r5
81605020:	e59d4020 	ldr	r4, [sp, #32]
81605024:	eb000b80 	bl	81607e2c <genimg_get_comp_name>
81605028:	e1540006 	cmp	r4, r6
8160502c:	e1a01000 	mov	r1, r0
81605030:	3a000007 	bcc	81605054 <bootm_decomp_image+0x10c>
81605034:	e59f0040 	ldr	r0, [pc, #64]	; 8160507c <bootm_decomp_image+0x134>
81605038:	eb00358f 	bl	8161267c <printf>
8160503c:	e59f003c 	ldr	r0, [pc, #60]	; 81605080 <bootm_decomp_image+0x138>
81605040:	eb00358d 	bl	8161267c <printf>
81605044:	e3e00005 	mvn	r0, #5
81605048:	ebfffd91 	bl	81604694 <show_boot_progress>
8160504c:	e3e00000 	mvn	r0, #0
81605050:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81605054:	e1a02008 	mov	r2, r8
81605058:	e59f0024 	ldr	r0, [pc, #36]	; 81605084 <bootm_decomp_image+0x13c>
8160505c:	eb003586 	bl	8161267c <printf>
81605060:	eafffff5 	b	8160503c <bootm_decomp_image+0xf4>
81605064:	81614786 	.word	0x81614786
81605068:	8161478a 	.word	0x8161478a
8160506c:	81614792 	.word	0x81614792
81605070:	816160c2 	.word	0x816160c2
81605074:	816147a0 	.word	0x816147a0
81605078:	816147b9 	.word	0x816147b9
8160507c:	816147dc 	.word	0x816147dc
81605080:	81614825 	.word	0x81614825
81605084:	8161480c 	.word	0x8161480c

81605088 <bootm_disable_interrupts>:
81605088:	eaffef8a 	b	81600eb8 <disable_interrupts>

8160508c <do_bootm_states>:
8160508c:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81605090:	e24dd038 	sub	sp, sp, #56	; 0x38
81605094:	e59d405c 	ldr	r4, [sp, #92]	; 0x5c
81605098:	e59d7058 	ldr	r7, [sp, #88]	; 0x58
8160509c:	e58d301c 	str	r3, [sp, #28]
816050a0:	e3170001 	tst	r7, #1
816050a4:	e594308c 	ldr	r3, [r4, #140]	; 0x8c
816050a8:	e58d0020 	str	r0, [sp, #32]
816050ac:	e1833007 	orr	r3, r3, r7
816050b0:	e58d1024 	str	r1, [sp, #36]	; 0x24
816050b4:	e58d2018 	str	r2, [sp, #24]
816050b8:	e584308c 	str	r3, [r4, #140]	; 0x8c
816050bc:	0a000018 	beq	81605124 <do_bootm_states+0x98>
816050c0:	e59f5680 	ldr	r5, [pc, #1664]	; 81605748 <do_bootm_states+0x6bc>
816050c4:	e3a02e13 	mov	r2, #304	; 0x130
816050c8:	e3a01000 	mov	r1, #0
816050cc:	e1a00005 	mov	r0, r5
816050d0:	ebffed7a 	bl	816006c0 <memset>
816050d4:	e59f0670 	ldr	r0, [pc, #1648]	; 8160574c <do_bootm_states+0x6c0>
816050d8:	eb0018d2 	bl	8160b428 <env_get_yesno>
816050dc:	e5850088 	str	r0, [r5, #136]	; 0x88
816050e0:	e2850090 	add	r0, r5, #144	; 0x90
816050e4:	eb00261f 	bl	8160e968 <lmb_init>
816050e8:	eb000aeb 	bl	81607c9c <env_get_bootm_low>
816050ec:	e1a06000 	mov	r6, r0
816050f0:	eb000af5 	bl	81607ccc <env_get_bootm_size>
816050f4:	e1a01006 	mov	r1, r6
816050f8:	e1a02000 	mov	r2, r0
816050fc:	e2850090 	add	r0, r5, #144	; 0x90
81605100:	eb002622 	bl	8160e990 <lmb_add>
81605104:	e2850090 	add	r0, r5, #144	; 0x90
81605108:	ebffeeca 	bl	81600c38 <arch_lmb_reserve>
8160510c:	e2850090 	add	r0, r5, #144	; 0x90
81605110:	eb0026b3 	bl	8160ebe4 <board_lmb_reserve>
81605114:	e3a000aa 	mov	r0, #170	; 0xaa
81605118:	ebfffd5d 	bl	81604694 <show_boot_progress>
8160511c:	e3a03001 	mov	r3, #1
81605120:	e585308c 	str	r3, [r5, #140]	; 0x8c
81605124:	e3170002 	tst	r7, #2
81605128:	0a000052 	beq	81605278 <do_bootm_states+0x1ec>
8160512c:	e59d3018 	ldr	r3, [sp, #24]
81605130:	e3a00000 	mov	r0, #0
81605134:	e28d2034 	add	r2, sp, #52	; 0x34
81605138:	e28d1030 	add	r1, sp, #48	; 0x30
8160513c:	e1530000 	cmp	r3, r0
81605140:	e58d0030 	str	r0, [sp, #48]	; 0x30
81605144:	e58d0034 	str	r0, [sp, #52]	; 0x34
81605148:	e3a0a000 	mov	sl, #0
8160514c:	c59d301c 	ldrgt	r3, [sp, #28]
81605150:	c5930000 	ldrgt	r0, [r3]
81605154:	eb000ba6 	bl	81607ff4 <genimg_get_kernel_addr_fit>
81605158:	e1a05000 	mov	r5, r0
8160515c:	e3a00001 	mov	r0, #1
81605160:	ebfffd4b 	bl	81604694 <show_boot_progress>
81605164:	e1a00005 	mov	r0, r5
81605168:	eb000bb7 	bl	8160804c <genimg_get_image>
8160516c:	e59f55d4 	ldr	r5, [pc, #1492]	; 81605748 <do_bootm_states+0x6bc>
81605170:	e585a054 	str	sl, [r5, #84]	; 0x54
81605174:	e585a050 	str	sl, [r5, #80]	; 0x50
81605178:	e1a08000 	mov	r8, r0
8160517c:	e1a06000 	mov	r6, r0
81605180:	eb000ba4 	bl	81608018 <genimg_get_format>
81605184:	e3500001 	cmp	r0, #1
81605188:	1a0000bb 	bne	8160547c <do_bootm_states+0x3f0>
8160518c:	e1a01008 	mov	r1, r8
81605190:	e59f05b8 	ldr	r0, [pc, #1464]	; 81605750 <do_bootm_states+0x6c4>
81605194:	eb003538 	bl	8161267c <printf>
81605198:	e5982000 	ldr	r2, [r8]
8160519c:	e59f35b0 	ldr	r3, [pc, #1456]	; 81605754 <do_bootm_states+0x6c8>
816051a0:	e595b088 	ldr	fp, [r5, #136]	; 0x88
816051a4:	e1520003 	cmp	r2, r3
816051a8:	0a000064 	beq	81605340 <do_bootm_states+0x2b4>
816051ac:	e59f05a4 	ldr	r0, [pc, #1444]	; 81605758 <do_bootm_states+0x6cc>
816051b0:	eb0006f7 	bl	81606d94 <puts>
816051b4:	e3e00000 	mvn	r0, #0
816051b8:	ebfffd35 	bl	81604694 <show_boot_progress>
816051bc:	e3a06000 	mov	r6, #0
816051c0:	e5953054 	ldr	r3, [r5, #84]	; 0x54
816051c4:	e3530000 	cmp	r3, #0
816051c8:	059f058c 	ldreq	r0, [pc, #1420]	; 8160575c <do_bootm_states+0x6d0>
816051cc:	0a0000b1 	beq	81605498 <do_bootm_states+0x40c>
816051d0:	e1a00006 	mov	r0, r6
816051d4:	eb000b8f 	bl	81608018 <genimg_get_format>
816051d8:	e3500001 	cmp	r0, #1
816051dc:	e1a08000 	mov	r8, r0
816051e0:	1a0000ab 	bne	81605494 <do_bootm_states+0x408>
816051e4:	e5d6301e 	ldrb	r3, [r6, #30]
816051e8:	e5c5305d 	strb	r3, [r5, #93]	; 0x5d
816051ec:	e5d6301f 	ldrb	r3, [r6, #31]
816051f0:	e5c5305c 	strb	r3, [r5, #92]	; 0x5c
816051f4:	e5d6301c 	ldrb	r3, [r6, #28]
816051f8:	e5c5305e 	strb	r3, [r5, #94]	; 0x5e
816051fc:	e596300c 	ldr	r3, [r6, #12]
81605200:	e6bf2f33 	rev	r2, r3
81605204:	e2863040 	add	r3, r6, #64	; 0x40
81605208:	e0833002 	add	r3, r3, r2
8160520c:	e585304c 	str	r3, [r5, #76]	; 0x4c
81605210:	e5963010 	ldr	r3, [r6, #16]
81605214:	e6bf3f33 	rev	r3, r3
81605218:	e5853058 	str	r3, [r5, #88]	; 0x58
8160521c:	e5d6301d 	ldrb	r3, [r6, #29]
81605220:	e3530018 	cmp	r3, #24
81605224:	13530003 	cmpne	r3, #3
81605228:	e5c5305f 	strb	r3, [r5, #95]	; 0x5f
8160522c:	1a00009e 	bne	816054ac <do_bootm_states+0x420>
81605230:	e59f2528 	ldr	r2, [pc, #1320]	; 81605760 <do_bootm_states+0x6d4>
81605234:	e28d3034 	add	r3, sp, #52	; 0x34
81605238:	e3a01003 	mov	r1, #3
8160523c:	e2420060 	sub	r0, r2, #96	; 0x60
81605240:	eb000c6f 	bl	81608404 <boot_get_setup>
81605244:	e2903002 	adds	r3, r0, #2
81605248:	13a03001 	movne	r3, #1
8160524c:	e0133fa0 	ands	r3, r3, r0, lsr #31
81605250:	159f050c 	ldrne	r0, [pc, #1292]	; 81605764 <do_bootm_states+0x6d8>
81605254:	1a00009b 	bne	816054c8 <do_bootm_states+0x43c>
81605258:	e5d5305d 	ldrb	r3, [r5, #93]	; 0x5d
8160525c:	e5856048 	str	r6, [r5, #72]	; 0x48
81605260:	e353000e 	cmp	r3, #14
81605264:	05952050 	ldreq	r2, [r5, #80]	; 0x50
81605268:	05953060 	ldreq	r3, [r5, #96]	; 0x60
8160526c:	05852058 	streq	r2, [r5, #88]	; 0x58
81605270:	00833002 	addeq	r3, r3, r2
81605274:	05853060 	streq	r3, [r5, #96]	; 0x60
81605278:	e3170004 	tst	r7, #4
8160527c:	1a000093 	bne	816054d0 <do_bootm_states+0x444>
81605280:	e2175008 	ands	r5, r7, #8
81605284:	1a0000a3 	bne	81605518 <do_bootm_states+0x48c>
81605288:	e3170010 	tst	r7, #16
8160528c:	0a00000f 	beq	816052d0 <do_bootm_states+0x244>
81605290:	e5941064 	ldr	r1, [r4, #100]	; 0x64
81605294:	e2843078 	add	r3, r4, #120	; 0x78
81605298:	e5942068 	ldr	r2, [r4, #104]	; 0x68
8160529c:	e2840090 	add	r0, r4, #144	; 0x90
816052a0:	e58d3000 	str	r3, [sp]
816052a4:	e2843074 	add	r3, r4, #116	; 0x74
816052a8:	e0422001 	sub	r2, r2, r1
816052ac:	eb000c08 	bl	816082d4 <boot_ramdisk_high>
816052b0:	e2508000 	subs	r8, r0, #0
816052b4:	1a000079 	bne	816054a0 <do_bootm_states+0x414>
816052b8:	e5941074 	ldr	r1, [r4, #116]	; 0x74
816052bc:	e59f04a4 	ldr	r0, [pc, #1188]	; 81605768 <do_bootm_states+0x6dc>
816052c0:	ebfffbc8 	bl	816041e8 <env_set_hex>
816052c4:	e5941078 	ldr	r1, [r4, #120]	; 0x78
816052c8:	e59f049c 	ldr	r0, [pc, #1180]	; 8160576c <do_bootm_states+0x6e0>
816052cc:	ebfffbc5 	bl	816041e8 <env_set_hex>
816052d0:	e3170020 	tst	r7, #32
816052d4:	1a0000df 	bne	81605658 <do_bootm_states+0x5cc>
816052d8:	e5d4005e 	ldrb	r0, [r4, #94]	; 0x5e
816052dc:	eb000258 	bl	81605c44 <bootm_os_get_boot_func>
816052e0:	e3170d1f 	tst	r7, #1984	; 0x7c0
816052e4:	13a03001 	movne	r3, #1
816052e8:	03a03000 	moveq	r3, #0
816052ec:	e3500000 	cmp	r0, #0
816052f0:	e1a06000 	mov	r6, r0
816052f4:	13a03000 	movne	r3, #0
816052f8:	e3530000 	cmp	r3, #0
816052fc:	1a0000e0 	bne	81605684 <do_bootm_states+0x5f8>
81605300:	e3170040 	tst	r7, #64	; 0x40
81605304:	1a0000ea 	bne	816056b4 <do_bootm_states+0x628>
81605308:	e3170080 	tst	r7, #128	; 0x80
8160530c:	1a0000f1 	bne	816056d8 <do_bootm_states+0x64c>
81605310:	e3170c01 	tst	r7, #256	; 0x100
81605314:	1a0000f7 	bne	816056f8 <do_bootm_states+0x66c>
81605318:	e2178b01 	ands	r8, r7, #1024	; 0x400
8160531c:	0a000105 	beq	81605738 <do_bootm_states+0x6ac>
81605320:	e58d6000 	str	r6, [sp]
81605324:	e1a03004 	mov	r3, r4
81605328:	e3a02b01 	mov	r2, #1024	; 0x400
8160532c:	e59d101c 	ldr	r1, [sp, #28]
81605330:	e59d0018 	ldr	r0, [sp, #24]
81605334:	eb00022c 	bl	81605bec <boot_selected_os>
81605338:	e1a08000 	mov	r8, r0
8160533c:	ea000090 	b	81605584 <do_bootm_states+0x4f8>
81605340:	e3a00002 	mov	r0, #2
81605344:	ebfffcd2 	bl	81604694 <show_boot_progress>
81605348:	e1a00008 	mov	r0, r8
8160534c:	eb000a0a 	bl	81607b7c <image_check_hcrc>
81605350:	e250a000 	subs	sl, r0, #0
81605354:	1a000003 	bne	81605368 <do_bootm_states+0x2dc>
81605358:	e59f0410 	ldr	r0, [pc, #1040]	; 81605770 <do_bootm_states+0x6e4>
8160535c:	eb00068c 	bl	81606d94 <puts>
81605360:	e3e00001 	mvn	r0, #1
81605364:	eaffff93 	b	816051b8 <do_bootm_states+0x12c>
81605368:	e3a00003 	mov	r0, #3
8160536c:	ebfffcc8 	bl	81604694 <show_boot_progress>
81605370:	e1a00008 	mov	r0, r8
81605374:	eb000ab2 	bl	81607e44 <image_print_contents>
81605378:	e35b0000 	cmp	fp, #0
8160537c:	0a00000b 	beq	816053b0 <do_bootm_states+0x324>
81605380:	e59f03ec 	ldr	r0, [pc, #1004]	; 81605774 <do_bootm_states+0x6e8>
81605384:	eb000682 	bl	81606d94 <puts>
81605388:	e1a00008 	mov	r0, r8
8160538c:	eb000a0d 	bl	81607bc8 <image_check_dcrc>
81605390:	e250a000 	subs	sl, r0, #0
81605394:	1a000003 	bne	816053a8 <do_bootm_states+0x31c>
81605398:	e59f03d8 	ldr	r0, [pc, #984]	; 81605778 <do_bootm_states+0x6ec>
8160539c:	eb0034b6 	bl	8161267c <printf>
816053a0:	e3e00002 	mvn	r0, #2
816053a4:	eaffff83 	b	816051b8 <do_bootm_states+0x12c>
816053a8:	e59f03cc 	ldr	r0, [pc, #972]	; 8160577c <do_bootm_states+0x6f0>
816053ac:	eb000678 	bl	81606d94 <puts>
816053b0:	e3a00004 	mov	r0, #4
816053b4:	ebfffcb6 	bl	81604694 <show_boot_progress>
816053b8:	e5d8101d 	ldrb	r1, [r8, #29]
816053bc:	e3510002 	cmp	r1, #2
816053c0:	0a000003 	beq	816053d4 <do_bootm_states+0x348>
816053c4:	e59f03b4 	ldr	r0, [pc, #948]	; 81605780 <do_bootm_states+0x6f4>
816053c8:	eb0034ab 	bl	8161267c <printf>
816053cc:	e3e00003 	mvn	r0, #3
816053d0:	eaffff78 	b	816051b8 <do_bootm_states+0x12c>
816053d4:	e3580000 	cmp	r8, #0
816053d8:	01a06008 	moveq	r6, r8
816053dc:	0affff77 	beq	816051c0 <do_bootm_states+0x134>
816053e0:	e3a00005 	mov	r0, #5
816053e4:	ebfffcaa 	bl	81604694 <show_boot_progress>
816053e8:	e5d8301e 	ldrb	r3, [r8, #30]
816053ec:	e3530002 	cmp	r3, #2
816053f0:	0a000002 	beq	81605400 <do_bootm_states+0x374>
816053f4:	8a000007 	bhi	81605418 <do_bootm_states+0x38c>
816053f8:	e3530001 	cmp	r3, #1
816053fc:	1a000018 	bne	81605464 <do_bootm_states+0x3d8>
81605400:	e2883040 	add	r3, r8, #64	; 0x40
81605404:	e5853050 	str	r3, [r5, #80]	; 0x50
81605408:	e598300c 	ldr	r3, [r8, #12]
8160540c:	e6bf3f33 	rev	r3, r3
81605410:	e5853054 	str	r3, [r5, #84]	; 0x54
81605414:	ea000008 	b	8160543c <do_bootm_states+0x3b0>
81605418:	e3530004 	cmp	r3, #4
8160541c:	0a000001 	beq	81605428 <do_bootm_states+0x39c>
81605420:	e353000e 	cmp	r3, #14
81605424:	eafffff4 	b	816053fc <do_bootm_states+0x370>
81605428:	e59f3354 	ldr	r3, [pc, #852]	; 81605784 <do_bootm_states+0x6f8>
8160542c:	e3a01000 	mov	r1, #0
81605430:	e1a00008 	mov	r0, r8
81605434:	e2432004 	sub	r2, r3, #4
81605438:	eb0009f7 	bl	81607c1c <image_multi_getimg>
8160543c:	e3a02040 	mov	r2, #64	; 0x40
81605440:	e1a01008 	mov	r1, r8
81605444:	e59f033c 	ldr	r0, [pc, #828]	; 81605788 <do_bootm_states+0x6fc>
81605448:	eb003158 	bl	816119b0 <memmove>
8160544c:	e3a00006 	mov	r0, #6
81605450:	e3a03001 	mov	r3, #1
81605454:	e5858000 	str	r8, [r5]
81605458:	e5853044 	str	r3, [r5, #68]	; 0x44
8160545c:	ebfffc8c 	bl	81604694 <show_boot_progress>
81605460:	eaffff56 	b	816051c0 <do_bootm_states+0x134>
81605464:	e59d3020 	ldr	r3, [sp, #32]
81605468:	e59f031c 	ldr	r0, [pc, #796]	; 8160578c <do_bootm_states+0x700>
8160546c:	e5931000 	ldr	r1, [r3]
81605470:	eb003481 	bl	8161267c <printf>
81605474:	e3e00004 	mvn	r0, #4
81605478:	eaffff4e 	b	816051b8 <do_bootm_states+0x12c>
8160547c:	e59d3020 	ldr	r3, [sp, #32]
81605480:	e59f0308 	ldr	r0, [pc, #776]	; 81605790 <do_bootm_states+0x704>
81605484:	e5931000 	ldr	r1, [r3]
81605488:	eb00347b 	bl	8161267c <printf>
8160548c:	e3e0006f 	mvn	r0, #111	; 0x6f
81605490:	eaffff48 	b	816051b8 <do_bootm_states+0x12c>
81605494:	e59f02f8 	ldr	r0, [pc, #760]	; 81605794 <do_bootm_states+0x708>
81605498:	eb00063d 	bl	81606d94 <puts>
8160549c:	e3a08001 	mov	r8, #1
816054a0:	e1a00008 	mov	r0, r8
816054a4:	e28dd038 	add	sp, sp, #56	; 0x38
816054a8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816054ac:	e5953044 	ldr	r3, [r5, #68]	; 0x44
816054b0:	e3530000 	cmp	r3, #0
816054b4:	15953018 	ldrne	r3, [r5, #24]
816054b8:	16bf3f33 	revne	r3, r3
816054bc:	15853060 	strne	r3, [r5, #96]	; 0x60
816054c0:	1affff64 	bne	81605258 <do_bootm_states+0x1cc>
816054c4:	e59f02cc 	ldr	r0, [pc, #716]	; 81605798 <do_bootm_states+0x70c>
816054c8:	eb000631 	bl	81606d94 <puts>
816054cc:	eafffff3 	b	816054a0 <do_bootm_states+0x414>
816054d0:	e59f2270 	ldr	r2, [pc, #624]	; 81605748 <do_bootm_states+0x6bc>
816054d4:	e5d2105d 	ldrb	r1, [r2, #93]	; 0x5d
816054d8:	e2413002 	sub	r3, r1, #2
816054dc:	e20330fd 	and	r3, r3, #253	; 0xfd
816054e0:	e3530000 	cmp	r3, #0
816054e4:	1351000e 	cmpne	r1, #14
816054e8:	1affff64 	bne	81605280 <do_bootm_states+0x1f4>
816054ec:	e5d2305e 	ldrb	r3, [r2, #94]	; 0x5e
816054f0:	e353000e 	cmp	r3, #14
816054f4:	13530005 	cmpne	r3, #5
816054f8:	1affff60 	bne	81605280 <do_bootm_states+0x1f4>
816054fc:	e59d201c 	ldr	r2, [sp, #28]
81605500:	e59d1018 	ldr	r1, [sp, #24]
81605504:	e59d0024 	ldr	r0, [sp, #36]	; 0x24
81605508:	ebfffe64 	bl	81604ea0 <bootm_find_images>
8160550c:	e2508000 	subs	r8, r0, #0
81605510:	1affffe2 	bne	816054a0 <do_bootm_states+0x414>
81605514:	eaffff59 	b	81605280 <do_bootm_states+0x1f4>
81605518:	ebffee66 	bl	81600eb8 <disable_interrupts>
8160551c:	e5943048 	ldr	r3, [r4, #72]	; 0x48
81605520:	e594a050 	ldr	sl, [r4, #80]	; 0x50
81605524:	e28d2034 	add	r2, sp, #52	; 0x34
81605528:	e5946058 	ldr	r6, [r4, #88]	; 0x58
8160552c:	e58d3028 	str	r3, [sp, #40]	; 0x28
81605530:	e594304c 	ldr	r3, [r4, #76]	; 0x4c
81605534:	e5d4b05c 	ldrb	fp, [r4, #92]	; 0x5c
81605538:	e1a01006 	mov	r1, r6
8160553c:	e58d302c 	str	r3, [sp, #44]	; 0x2c
81605540:	e5d4305d 	ldrb	r3, [r4, #93]	; 0x5d
81605544:	e58d2010 	str	r2, [sp, #16]
81605548:	e3a02502 	mov	r2, #8388608	; 0x800000
8160554c:	e58d200c 	str	r2, [sp, #12]
81605550:	e5942054 	ldr	r2, [r4, #84]	; 0x54
81605554:	e88d0440 	stm	sp, {r6, sl}
81605558:	e58d2008 	str	r2, [sp, #8]
8160555c:	e1a0200a 	mov	r2, sl
81605560:	e1a05000 	mov	r5, r0
81605564:	e1a0000b 	mov	r0, fp
81605568:	ebfffe76 	bl	81604f48 <bootm_decomp_image>
8160556c:	e2508000 	subs	r8, r0, #0
81605570:	0a00000b 	beq	816055a4 <do_bootm_states+0x518>
81605574:	e3e00005 	mvn	r0, #5
81605578:	ebfffc45 	bl	81604694 <show_boot_progress>
8160557c:	e3780002 	cmn	r8, #2
81605580:	0affff40 	beq	81605288 <do_bootm_states+0x1fc>
81605584:	e3550000 	cmp	r5, #0
81605588:	0a000000 	beq	81605590 <do_bootm_states+0x504>
8160558c:	ebffee48 	bl	81600eb4 <enable_interrupts>
81605590:	e3780003 	cmn	r8, #3
81605594:	1a00005f 	bne	81605718 <do_bootm_states+0x68c>
81605598:	e3e00006 	mvn	r0, #6
8160559c:	ebfffc3c 	bl	81604694 <show_boot_progress>
816055a0:	eaffffbe 	b	816054a0 <do_bootm_states+0x414>
816055a4:	e59d1034 	ldr	r1, [sp, #52]	; 0x34
816055a8:	e04aa006 	sub	sl, sl, r6
816055ac:	e1a00006 	mov	r0, r6
816055b0:	e16faf1a 	clz	sl, sl
816055b4:	e281103f 	add	r1, r1, #63	; 0x3f
816055b8:	e0411006 	sub	r1, r1, r6
816055bc:	e1a0a2aa 	lsr	sl, sl, #5
816055c0:	e3c1103f 	bic	r1, r1, #63	; 0x3f
816055c4:	ebffeef8 	bl	816011ac <flush_cache>
816055c8:	e3a00007 	mov	r0, #7
816055cc:	ebfffc30 	bl	81604694 <show_boot_progress>
816055d0:	e35b0000 	cmp	fp, #0
816055d4:	e59d302c 	ldr	r3, [sp, #44]	; 0x2c
816055d8:	01a0b00a 	moveq	fp, sl
816055dc:	13a0b000 	movne	fp, #0
816055e0:	e22bb001 	eor	fp, fp, #1
816055e4:	e59d2034 	ldr	r2, [sp, #52]	; 0x34
816055e8:	e1530006 	cmp	r3, r6
816055ec:	93a06000 	movls	r6, #0
816055f0:	820b6001 	andhi	r6, fp, #1
816055f4:	e3560000 	cmp	r6, #0
816055f8:	0a000011 	beq	81605644 <do_bootm_states+0x5b8>
816055fc:	e59d3028 	ldr	r3, [sp, #40]	; 0x28
81605600:	e1530002 	cmp	r3, r2
81605604:	2a00000e 	bcs	81605644 <do_bootm_states+0x5b8>
81605608:	e5943044 	ldr	r3, [r4, #68]	; 0x44
8160560c:	e3530000 	cmp	r3, #0
81605610:	0a000005 	beq	8160562c <do_bootm_states+0x5a0>
81605614:	e5d43022 	ldrb	r3, [r4, #34]	; 0x22
81605618:	e3530004 	cmp	r3, #4
8160561c:	1affff19 	bne	81605288 <do_bootm_states+0x1fc>
81605620:	e59f0174 	ldr	r0, [pc, #372]	; 8160579c <do_bootm_states+0x710>
81605624:	eb0005da 	bl	81606d94 <puts>
81605628:	eaffff16 	b	81605288 <do_bootm_states+0x1fc>
8160562c:	e59f016c 	ldr	r0, [pc, #364]	; 816057a0 <do_bootm_states+0x714>
81605630:	e3e08000 	mvn	r8, #0
81605634:	eb0005d6 	bl	81606d94 <puts>
81605638:	e3e00073 	mvn	r0, #115	; 0x73
8160563c:	ebfffc14 	bl	81604694 <show_boot_progress>
81605640:	eaffffcf 	b	81605584 <do_bootm_states+0x4f8>
81605644:	e5941058 	ldr	r1, [r4, #88]	; 0x58
81605648:	e2840090 	add	r0, r4, #144	; 0x90
8160564c:	e0422001 	sub	r2, r2, r1
81605650:	eb002509 	bl	8160ea7c <lmb_reserve>
81605654:	eaffff0b 	b	81605288 <do_bootm_states+0x1fc>
81605658:	e2846090 	add	r6, r4, #144	; 0x90
8160565c:	e594106c 	ldr	r1, [r4, #108]	; 0x6c
81605660:	e1a00006 	mov	r0, r6
81605664:	eb000b8c 	bl	8160849c <boot_fdt_add_mem_rsv_regions>
81605668:	e2842070 	add	r2, r4, #112	; 0x70
8160566c:	e284106c 	add	r1, r4, #108	; 0x6c
81605670:	e1a00006 	mov	r0, r6
81605674:	eb000bab 	bl	81608528 <boot_relocate_fdt>
81605678:	e2508000 	subs	r8, r0, #0
8160567c:	0affff15 	beq	816052d8 <do_bootm_states+0x24c>
81605680:	eaffff86 	b	816054a0 <do_bootm_states+0x414>
81605684:	e3550000 	cmp	r5, #0
81605688:	0a000000 	beq	81605690 <do_bootm_states+0x604>
8160568c:	ebffee08 	bl	81600eb4 <enable_interrupts>
81605690:	e5d4005e 	ldrb	r0, [r4, #94]	; 0x5e
81605694:	eb0009d2 	bl	81607de4 <genimg_get_os_name>
81605698:	e5d4205e 	ldrb	r2, [r4, #94]	; 0x5e
8160569c:	e1a01000 	mov	r1, r0
816056a0:	e59f00fc 	ldr	r0, [pc, #252]	; 816057a4 <do_bootm_states+0x718>
816056a4:	eb0033f4 	bl	8161267c <printf>
816056a8:	e3e00007 	mvn	r0, #7
816056ac:	ebfffbf8 	bl	81604694 <show_boot_progress>
816056b0:	eaffff79 	b	8160549c <do_bootm_states+0x410>
816056b4:	e1a03004 	mov	r3, r4
816056b8:	e59d201c 	ldr	r2, [sp, #28]
816056bc:	e59d1018 	ldr	r1, [sp, #24]
816056c0:	e3a00040 	mov	r0, #64	; 0x40
816056c4:	e12fff36 	blx	r6
816056c8:	e2508000 	subs	r8, r0, #0
816056cc:	0affff0d 	beq	81605308 <do_bootm_states+0x27c>
816056d0:	e59f00d0 	ldr	r0, [pc, #208]	; 816057a8 <do_bootm_states+0x71c>
816056d4:	eaffff7b 	b	816054c8 <do_bootm_states+0x43c>
816056d8:	e1a03004 	mov	r3, r4
816056dc:	e59d201c 	ldr	r2, [sp, #28]
816056e0:	e59d1018 	ldr	r1, [sp, #24]
816056e4:	e3a00080 	mov	r0, #128	; 0x80
816056e8:	e12fff36 	blx	r6
816056ec:	e2508000 	subs	r8, r0, #0
816056f0:	0affff06 	beq	81605310 <do_bootm_states+0x284>
816056f4:	eafffff5 	b	816056d0 <do_bootm_states+0x644>
816056f8:	e1a03004 	mov	r3, r4
816056fc:	e59d201c 	ldr	r2, [sp, #28]
81605700:	e59d1018 	ldr	r1, [sp, #24]
81605704:	e3a00c01 	mov	r0, #256	; 0x100
81605708:	e12fff36 	blx	r6
8160570c:	e2508000 	subs	r8, r0, #0
81605710:	0affff00 	beq	81605318 <do_bootm_states+0x28c>
81605714:	eaffffed 	b	816056d0 <do_bootm_states+0x644>
81605718:	e3780001 	cmn	r8, #1
8160571c:	1affff5f 	bne	816054a0 <do_bootm_states+0x414>
81605720:	e59d301c 	ldr	r3, [sp, #28]
81605724:	e59d2018 	ldr	r2, [sp, #24]
81605728:	e59d1024 	ldr	r1, [sp, #36]	; 0x24
8160572c:	e59d0020 	ldr	r0, [sp, #32]
81605730:	ebffee91 	bl	8160117c <do_reset>
81605734:	eaffff59 	b	816054a0 <do_bootm_states+0x414>
81605738:	e3550000 	cmp	r5, #0
8160573c:	0affff57 	beq	816054a0 <do_bootm_states+0x414>
81605740:	ebffeddb 	bl	81600eb4 <enable_interrupts>
81605744:	eaffff55 	b	816054a0 <do_bootm_states+0x414>
81605748:	81619880 	.word	0x81619880
8160574c:	81616865 	.word	0x81616865
81605750:	81614cf2 	.word	0x81614cf2
81605754:	56190527 	.word	0x56190527
81605758:	8161606c 	.word	0x8161606c
8160575c:	81614d87 	.word	0x81614d87
81605760:	816198e0 	.word	0x816198e0
81605764:	81614dca 	.word	0x81614dca
81605768:	81614e99 	.word	0x81614e99
8160576c:	81614ea6 	.word	0x81614ea6
81605770:	81616081 	.word	0x81616081
81605774:	81616096 	.word	0x81616096
81605778:	816160b4 	.word	0x816160b4
8160577c:	816160c2 	.word	0x816160c2
81605780:	81614d24 	.word	0x81614d24
81605784:	816198d4 	.word	0x816198d4
81605788:	81619884 	.word	0x81619884
8160578c:	81614d43 	.word	0x81614d43
81605790:	81614d64 	.word	0x81614d64
81605794:	81614da7 	.word	0x81614da7
81605798:	81614df4 	.word	0x81614df4
8160579c:	81614e18 	.word	0x81614e18
816057a0:	81614e52 	.word	0x81614e52
816057a4:	81614eb1 	.word	0x81614eb1
816057a8:	81614edf 	.word	0x81614edf

816057ac <copy_args>:
816057ac:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816057b0:	e1a05000 	mov	r5, r0
816057b4:	e1a07001 	mov	r7, r1
816057b8:	e1a06002 	mov	r6, r2
816057bc:	e1a08003 	mov	r8, r3
816057c0:	e3a04000 	mov	r4, #0
816057c4:	e1540007 	cmp	r4, r7
816057c8:	a8bd81f0 	popge	{r4, r5, r6, r7, r8, pc}
816057cc:	e3540000 	cmp	r4, #0
816057d0:	c5c58000 	strbgt	r8, [r5]
816057d4:	c2855001 	addgt	r5, r5, #1
816057d8:	e7961104 	ldr	r1, [r6, r4, lsl #2]
816057dc:	e1a00005 	mov	r0, r5
816057e0:	eb003007 	bl	81611804 <strcpy>
816057e4:	e7960104 	ldr	r0, [r6, r4, lsl #2]
816057e8:	eb003039 	bl	816118d4 <strlen>
816057ec:	e2844001 	add	r4, r4, #1
816057f0:	e0855000 	add	r5, r5, r0
816057f4:	eafffff2 	b	816057c4 <copy_args+0x18>

816057f8 <do_bootm_rtems>:
816057f8:	e3500b01 	cmp	r0, #1024	; 0x400
816057fc:	1a00000a 	bne	8160582c <do_bootm_rtems+0x34>
81605800:	e92d4010 	push	{r4, lr}
81605804:	e5934060 	ldr	r4, [r3, #96]	; 0x60
81605808:	e59f0024 	ldr	r0, [pc, #36]	; 81605834 <do_bootm_rtems+0x3c>
8160580c:	e1a01004 	mov	r1, r4
81605810:	eb003399 	bl	8161267c <printf>
81605814:	e3a0000f 	mov	r0, #15
81605818:	ebfffb9d 	bl	81604694 <show_boot_progress>
8160581c:	e5990000 	ldr	r0, [r9]
81605820:	e12fff34 	blx	r4
81605824:	e3a00001 	mov	r0, #1
81605828:	e8bd8010 	pop	{r4, pc}
8160582c:	e3a00000 	mov	r0, #0
81605830:	e12fff1e 	bx	lr
81605834:	81614cb9 	.word	0x81614cb9

81605838 <do_bootm_plan9>:
81605838:	e3500b01 	cmp	r0, #1024	; 0x400
8160583c:	1a000021 	bne	816058c8 <do_bootm_plan9+0x90>
81605840:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81605844:	e1a05003 	mov	r5, r3
81605848:	e59f0080 	ldr	r0, [pc, #128]	; 816058d0 <do_bootm_plan9+0x98>
8160584c:	e1a06002 	mov	r6, r2
81605850:	e1a04001 	mov	r4, r1
81605854:	ebfffb4f 	bl	81604598 <env_get>
81605858:	e3500000 	cmp	r0, #0
8160585c:	0a000009 	beq	81605888 <do_bootm_plan9+0x50>
81605860:	e3a02010 	mov	r2, #16
81605864:	e3a01000 	mov	r1, #0
81605868:	eb0033ca 	bl	81612798 <simple_strtoul>
8160586c:	e3540000 	cmp	r4, #0
81605870:	e1a07000 	mov	r7, r0
81605874:	da00000c 	ble	816058ac <do_bootm_plan9+0x74>
81605878:	e3a0300a 	mov	r3, #10
8160587c:	e1a02006 	mov	r2, r6
81605880:	e1a01004 	mov	r1, r4
81605884:	ebffffc8 	bl	816057ac <copy_args>
81605888:	e5954060 	ldr	r4, [r5, #96]	; 0x60
8160588c:	e59f0040 	ldr	r0, [pc, #64]	; 816058d4 <do_bootm_plan9+0x9c>
81605890:	e1a01004 	mov	r1, r4
81605894:	eb003378 	bl	8161267c <printf>
81605898:	e3a0000f 	mov	r0, #15
8160589c:	ebfffb7c 	bl	81604694 <show_boot_progress>
816058a0:	e12fff34 	blx	r4
816058a4:	e3a00001 	mov	r0, #1
816058a8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
816058ac:	e59f0024 	ldr	r0, [pc, #36]	; 816058d8 <do_bootm_plan9+0xa0>
816058b0:	ebfffb38 	bl	81604598 <env_get>
816058b4:	e2501000 	subs	r1, r0, #0
816058b8:	0afffff2 	beq	81605888 <do_bootm_plan9+0x50>
816058bc:	e1a00007 	mov	r0, r7
816058c0:	eb002fcf 	bl	81611804 <strcpy>
816058c4:	eaffffef 	b	81605888 <do_bootm_plan9+0x50>
816058c8:	e3a00000 	mov	r0, #0
816058cc:	e12fff1e 	bx	lr
816058d0:	81614c76 	.word	0x81614c76
816058d4:	81614c7f 	.word	0x81614c7f
816058d8:	81614ff6 	.word	0x81614ff6

816058dc <do_bootm_standalone>:
816058dc:	e92d4070 	push	{r4, r5, r6, lr}
816058e0:	e1a05001 	mov	r5, r1
816058e4:	e59f0048 	ldr	r0, [pc, #72]	; 81605934 <do_bootm_standalone+0x58>
816058e8:	e1a06002 	mov	r6, r2
816058ec:	e1a04003 	mov	r4, r3
816058f0:	ebfffb28 	bl	81604598 <env_get>
816058f4:	e3500000 	cmp	r0, #0
816058f8:	0a000008 	beq	81605920 <do_bootm_standalone+0x44>
816058fc:	e59f1034 	ldr	r1, [pc, #52]	; 81605938 <do_bootm_standalone+0x5c>
81605900:	eb002fce 	bl	81611840 <strcmp>
81605904:	e3500000 	cmp	r0, #0
81605908:	1a000004 	bne	81605920 <do_bootm_standalone+0x44>
8160590c:	e5941054 	ldr	r1, [r4, #84]	; 0x54
81605910:	e59f0024 	ldr	r0, [pc, #36]	; 8160593c <do_bootm_standalone+0x60>
81605914:	ebfffa33 	bl	816041e8 <env_set_hex>
81605918:	e3a00000 	mov	r0, #0
8160591c:	e8bd8070 	pop	{r4, r5, r6, pc}
81605920:	e5943060 	ldr	r3, [r4, #96]	; 0x60
81605924:	e1a01006 	mov	r1, r6
81605928:	e1a00005 	mov	r0, r5
8160592c:	e12fff33 	blx	r3
81605930:	eafffff8 	b	81605918 <do_bootm_standalone+0x3c>
81605934:	81614ba7 	.word	0x81614ba7
81605938:	81614c00 	.word	0x81614c00
8160593c:	8161514d 	.word	0x8161514d

81605940 <do_bootm_qnxelf>:
81605940:	e3500b01 	cmp	r0, #1024	; 0x400
81605944:	1a000018 	bne	816059ac <do_bootm_qnxelf+0x6c>
81605948:	e92d4030 	push	{r4, r5, lr}
8160594c:	e24dd01c 	sub	sp, sp, #28
81605950:	e28d5008 	add	r5, sp, #8
81605954:	e1a04002 	mov	r4, r2
81605958:	e59f1054 	ldr	r1, [pc, #84]	; 816059b4 <do_bootm_qnxelf+0x74>
8160595c:	e5932060 	ldr	r2, [r3, #96]	; 0x60
81605960:	e1a00005 	mov	r0, r5
81605964:	eb00333a 	bl	81612654 <sprintf>
81605968:	e5943000 	ldr	r3, [r4]
8160596c:	e88d0028 	stm	sp, {r3, r5}
81605970:	ebffeebb 	bl	81601464 <dcache_status>
81605974:	e2504000 	subs	r4, r0, #0
81605978:	0a000000 	beq	81605980 <do_bootm_qnxelf+0x40>
8160597c:	ebffeeb6 	bl	8160145c <dcache_disable>
81605980:	e3a01000 	mov	r1, #0
81605984:	e1a0300d 	mov	r3, sp
81605988:	e3a02002 	mov	r2, #2
8160598c:	e1a00001 	mov	r0, r1
81605990:	ebfff0de 	bl	81601d10 <do_bootelf>
81605994:	e3540000 	cmp	r4, #0
81605998:	0a000000 	beq	816059a0 <do_bootm_qnxelf+0x60>
8160599c:	ebffeeac 	bl	81601454 <dcache_enable>
816059a0:	e3a00001 	mov	r0, #1
816059a4:	e28dd01c 	add	sp, sp, #28
816059a8:	e8bd8030 	pop	{r4, r5, pc}
816059ac:	e3a00000 	mov	r0, #0
816059b0:	e12fff1e 	bx	lr
816059b4:	81615b8b 	.word	0x81615b8b

816059b8 <do_bootm_netbsd>:
816059b8:	e3500b01 	cmp	r0, #1024	; 0x400
816059bc:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
816059c0:	13a00000 	movne	r0, #0
816059c4:	1a000024 	bne	81605a5c <do_bootm_netbsd+0xa4>
816059c8:	e1a04003 	mov	r4, r3
816059cc:	e5d33022 	ldrb	r3, [r3, #34]	; 0x22
816059d0:	e1a06002 	mov	r6, r2
816059d4:	e1a05001 	mov	r5, r1
816059d8:	e3530004 	cmp	r3, #4
816059dc:	0a000020 	beq	81605a64 <do_bootm_netbsd+0xac>
816059e0:	e3a08000 	mov	r8, #0
816059e4:	e3550000 	cmp	r5, #0
816059e8:	da000027 	ble	81605a8c <do_bootm_netbsd+0xd4>
816059ec:	e086b105 	add	fp, r6, r5, lsl #2
816059f0:	e1a0a006 	mov	sl, r6
816059f4:	e3a07000 	mov	r7, #0
816059f8:	e49a0004 	ldr	r0, [sl], #4
816059fc:	eb002fb4 	bl	816118d4 <strlen>
81605a00:	e15a000b 	cmp	sl, fp
81605a04:	e2800001 	add	r0, r0, #1
81605a08:	e0877000 	add	r7, r7, r0
81605a0c:	1afffff9 	bne	816059f8 <do_bootm_netbsd+0x40>
81605a10:	e1a00007 	mov	r0, r7
81605a14:	eb000692 	bl	81607464 <malloc>
81605a18:	e3a03020 	mov	r3, #32
81605a1c:	e1a02006 	mov	r2, r6
81605a20:	e1a01005 	mov	r1, r5
81605a24:	e1a07000 	mov	r7, r0
81605a28:	ebffff5f 	bl	816057ac <copy_args>
81605a2c:	e5944060 	ldr	r4, [r4, #96]	; 0x60
81605a30:	e59f006c 	ldr	r0, [pc, #108]	; 81605aa4 <do_bootm_netbsd+0xec>
81605a34:	e1a01004 	mov	r1, r4
81605a38:	eb00330f 	bl	8161267c <printf>
81605a3c:	e3a0000f 	mov	r0, #15
81605a40:	ebfffb13 	bl	81604694 <show_boot_progress>
81605a44:	e1a03007 	mov	r3, r7
81605a48:	e59f2058 	ldr	r2, [pc, #88]	; 81605aa8 <do_bootm_netbsd+0xf0>
81605a4c:	e1a01008 	mov	r1, r8
81605a50:	e5990000 	ldr	r0, [r9]
81605a54:	e12fff34 	blx	r4
81605a58:	e3a00001 	mov	r0, #1
81605a5c:	e28dd008 	add	sp, sp, #8
81605a60:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81605a64:	e5948000 	ldr	r8, [r4]
81605a68:	e28d3004 	add	r3, sp, #4
81605a6c:	e1a0200d 	mov	r2, sp
81605a70:	e3a01001 	mov	r1, #1
81605a74:	e1a00008 	mov	r0, r8
81605a78:	eb000867 	bl	81607c1c <image_multi_getimg>
81605a7c:	e59d3004 	ldr	r3, [sp, #4]
81605a80:	e3530000 	cmp	r3, #0
81605a84:	0affffd5 	beq	816059e0 <do_bootm_netbsd+0x28>
81605a88:	eaffffd5 	b	816059e4 <do_bootm_netbsd+0x2c>
81605a8c:	e59f0018 	ldr	r0, [pc, #24]	; 81605aac <do_bootm_netbsd+0xf4>
81605a90:	ebfffac0 	bl	81604598 <env_get>
81605a94:	e59f700c 	ldr	r7, [pc, #12]	; 81605aa8 <do_bootm_netbsd+0xf0>
81605a98:	e3500000 	cmp	r0, #0
81605a9c:	11a07000 	movne	r7, r0
81605aa0:	eaffffe1 	b	81605a2c <do_bootm_netbsd+0x74>
81605aa4:	81614c2d 	.word	0x81614c2d
81605aa8:	8161623b 	.word	0x8161623b
81605aac:	81614ff6 	.word	0x81614ff6

81605ab0 <do_bootvx_fdt>:
81605ab0:	e92d407f 	push	{r0, r1, r2, r3, r4, r5, r6, lr}
81605ab4:	e1a04000 	mov	r4, r0
81605ab8:	e590106c 	ldr	r1, [r0, #108]	; 0x6c
81605abc:	e5903070 	ldr	r3, [r0, #112]	; 0x70
81605ac0:	e3510000 	cmp	r1, #0
81605ac4:	e58d300c 	str	r3, [sp, #12]
81605ac8:	0a000029 	beq	81605b74 <do_bootvx_fdt+0xc4>
81605acc:	e2805090 	add	r5, r0, #144	; 0x90
81605ad0:	e1a00005 	mov	r0, r5
81605ad4:	eb000a70 	bl	8160849c <boot_fdt_add_mem_rsv_regions>
81605ad8:	e1a00005 	mov	r0, r5
81605adc:	e28d200c 	add	r2, sp, #12
81605ae0:	e284106c 	add	r1, r4, #108	; 0x6c
81605ae4:	eb000a8f 	bl	81608528 <boot_relocate_fdt>
81605ae8:	e2505000 	subs	r5, r0, #0
81605aec:	1a00001e 	bne	81605b6c <do_bootvx_fdt+0xbc>
81605af0:	e594006c 	ldr	r0, [r4, #108]	; 0x6c
81605af4:	eb0001df 	bl	81606278 <fdt_fixup_ethernet>
81605af8:	e59f20a8 	ldr	r2, [pc, #168]	; 81605ba8 <do_bootvx_fdt+0xf8>
81605afc:	e1a01005 	mov	r1, r5
81605b00:	e594006c 	ldr	r0, [r4, #108]	; 0x6c
81605b04:	eb002901 	bl	8160ff10 <fdt_add_subnode>
81605b08:	e3700002 	cmn	r0, #2
81605b0c:	13500000 	cmpne	r0, #0
81605b10:	ba000010 	blt	81605b58 <do_bootvx_fdt+0xa8>
81605b14:	e59f0090 	ldr	r0, [pc, #144]	; 81605bac <do_bootvx_fdt+0xfc>
81605b18:	ebfffa9e 	bl	81604598 <env_get>
81605b1c:	e2506000 	subs	r6, r0, #0
81605b20:	0a000013 	beq	81605b74 <do_bootvx_fdt+0xc4>
81605b24:	e594506c 	ldr	r5, [r4, #108]	; 0x6c
81605b28:	eb002f69 	bl	816118d4 <strlen>
81605b2c:	e3a03001 	mov	r3, #1
81605b30:	e59f2074 	ldr	r2, [pc, #116]	; 81605bac <do_bootvx_fdt+0xfc>
81605b34:	e58d3004 	str	r3, [sp, #4]
81605b38:	e59f1070 	ldr	r1, [pc, #112]	; 81605bb0 <do_bootvx_fdt+0x100>
81605b3c:	e0800003 	add	r0, r0, r3
81605b40:	e1a03006 	mov	r3, r6
81605b44:	e58d0000 	str	r0, [sp]
81605b48:	e1a00005 	mov	r0, r5
81605b4c:	eb000040 	bl	81605c54 <fdt_find_and_setprop>
81605b50:	e3500000 	cmp	r0, #0
81605b54:	aa000006 	bge	81605b74 <do_bootvx_fdt+0xc4>
81605b58:	eb002970 	bl	81610120 <fdt_strerror>
81605b5c:	e59f1050 	ldr	r1, [pc, #80]	; 81605bb4 <do_bootvx_fdt+0x104>
81605b60:	e1a02000 	mov	r2, r0
81605b64:	e59f004c 	ldr	r0, [pc, #76]	; 81605bb8 <do_bootvx_fdt+0x108>
81605b68:	eb0032c3 	bl	8161267c <printf>
81605b6c:	e28dd010 	add	sp, sp, #16
81605b70:	e8bd8070 	pop	{r4, r5, r6, pc}
81605b74:	e1a00004 	mov	r0, r4
81605b78:	ebffec80 	bl	81600d80 <boot_prep_vxworks>
81605b7c:	e3a0000f 	mov	r0, #15
81605b80:	ebfffac3 	bl	81604694 <show_boot_progress>
81605b84:	e594206c 	ldr	r2, [r4, #108]	; 0x6c
81605b88:	e5941060 	ldr	r1, [r4, #96]	; 0x60
81605b8c:	e59f0028 	ldr	r0, [pc, #40]	; 81605bbc <do_bootvx_fdt+0x10c>
81605b90:	eb0032b9 	bl	8161267c <printf>
81605b94:	e1a00004 	mov	r0, r4
81605b98:	ebffec8c 	bl	81600dd0 <boot_jump_vxworks>
81605b9c:	e59f001c 	ldr	r0, [pc, #28]	; 81605bc0 <do_bootvx_fdt+0x110>
81605ba0:	eb00047b 	bl	81606d94 <puts>
81605ba4:	eafffff0 	b	81605b6c <do_bootvx_fdt+0xbc>
81605ba8:	81615082 	.word	0x81615082
81605bac:	81614ff6 	.word	0x81614ff6
81605bb0:	81615081 	.word	0x81615081
81605bb4:	816140b2 	.word	0x816140b2
81605bb8:	81615089 	.word	0x81615089
81605bbc:	8161509c 	.word	0x8161509c
81605bc0:	8161506a 	.word	0x8161506a

81605bc4 <do_bootm_vxworks>:
81605bc4:	e3500b01 	cmp	r0, #1024	; 0x400
81605bc8:	1a000004 	bne	81605be0 <do_bootm_vxworks+0x1c>
81605bcc:	e1a00003 	mov	r0, r3
81605bd0:	e92d4010 	push	{r4, lr}
81605bd4:	ebffffb5 	bl	81605ab0 <do_bootvx_fdt>
81605bd8:	e3a00001 	mov	r0, #1
81605bdc:	e8bd8010 	pop	{r4, pc}
81605be0:	e3a00000 	mov	r0, #0
81605be4:	e12fff1e 	bx	lr

81605be8 <arch_preboot_os>:
81605be8:	e12fff1e 	bx	lr

81605bec <boot_selected_os>:
81605bec:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81605bf0:	e1a06000 	mov	r6, r0
81605bf4:	e1a04002 	mov	r4, r2
81605bf8:	e1a07001 	mov	r7, r1
81605bfc:	e1a05003 	mov	r5, r3
81605c00:	ebfffff8 	bl	81605be8 <arch_preboot_os>
81605c04:	e1a03005 	mov	r3, r5
81605c08:	e1a01006 	mov	r1, r6
81605c0c:	e1a02007 	mov	r2, r7
81605c10:	e1a00004 	mov	r0, r4
81605c14:	e59d6018 	ldr	r6, [sp, #24]
81605c18:	e12fff36 	blx	r6
81605c1c:	e5d5305d 	ldrb	r3, [r5, #93]	; 0x5d
81605c20:	e3540c02 	cmp	r4, #512	; 0x200
81605c24:	13530001 	cmpne	r3, #1
81605c28:	0a000003 	beq	81605c3c <boot_selected_os+0x50>
81605c2c:	e3e00008 	mvn	r0, #8
81605c30:	ebfffa97 	bl	81604694 <show_boot_progress>
81605c34:	e3e00000 	mvn	r0, #0
81605c38:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81605c3c:	e3a00000 	mov	r0, #0
81605c40:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

81605c44 <bootm_os_get_boot_func>:
81605c44:	e59f3004 	ldr	r3, [pc, #4]	; 81605c50 <bootm_os_get_boot_func+0xc>
81605c48:	e7930100 	ldr	r0, [r3, r0, lsl #2]
81605c4c:	e12fff1e 	bx	lr
81605c50:	81612d4c 	.word	0x81612d4c

81605c54 <fdt_find_and_setprop>:
81605c54:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81605c58:	e1a05000 	mov	r5, r0
81605c5c:	e1a06002 	mov	r6, r2
81605c60:	e1a08003 	mov	r8, r3
81605c64:	e59da020 	ldr	sl, [sp, #32]
81605c68:	e59d7024 	ldr	r7, [sp, #36]	; 0x24
81605c6c:	eb0026b2 	bl	8160f73c <fdt_path_offset>
81605c70:	e2504000 	subs	r4, r0, #0
81605c74:	ba000010 	blt	81605cbc <fdt_find_and_setprop+0x68>
81605c78:	e3570000 	cmp	r7, #0
81605c7c:	0a000006 	beq	81605c9c <fdt_find_and_setprop+0x48>
81605c80:	e58da020 	str	sl, [sp, #32]
81605c84:	e1a03008 	mov	r3, r8
81605c88:	e1a02006 	mov	r2, r6
81605c8c:	e1a01004 	mov	r1, r4
81605c90:	e1a00005 	mov	r0, r5
81605c94:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
81605c98:	ea00280c 	b	8160fcd0 <fdt_setprop>
81605c9c:	e1a03007 	mov	r3, r7
81605ca0:	e1a02006 	mov	r2, r6
81605ca4:	e1a01004 	mov	r1, r4
81605ca8:	e1a00005 	mov	r0, r5
81605cac:	eb002620 	bl	8160f534 <fdt_get_property>
81605cb0:	e3500000 	cmp	r0, #0
81605cb4:	1afffff1 	bne	81605c80 <fdt_find_and_setprop+0x2c>
81605cb8:	e1a04000 	mov	r4, r0
81605cbc:	e1a00004 	mov	r0, r4
81605cc0:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}

81605cc4 <fdt_find_or_add_subnode>:
81605cc4:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81605cc8:	e1a06000 	mov	r6, r0
81605ccc:	e1a07001 	mov	r7, r1
81605cd0:	e1a05002 	mov	r5, r2
81605cd4:	eb00259a 	bl	8160f344 <fdt_subnode_offset>
81605cd8:	e3700001 	cmn	r0, #1
81605cdc:	1a000003 	bne	81605cf0 <fdt_find_or_add_subnode+0x2c>
81605ce0:	e1a02005 	mov	r2, r5
81605ce4:	e1a01007 	mov	r1, r7
81605ce8:	e1a00006 	mov	r0, r6
81605cec:	eb002887 	bl	8160ff10 <fdt_add_subnode>
81605cf0:	e3500000 	cmp	r0, #0
81605cf4:	e1a04000 	mov	r4, r0
81605cf8:	aa000005 	bge	81605d14 <fdt_find_or_add_subnode+0x50>
81605cfc:	eb002907 	bl	81610120 <fdt_strerror>
81605d00:	e1a02005 	mov	r2, r5
81605d04:	e59f1010 	ldr	r1, [pc, #16]	; 81605d1c <fdt_find_or_add_subnode+0x58>
81605d08:	e1a03000 	mov	r3, r0
81605d0c:	e59f000c 	ldr	r0, [pc, #12]	; 81605d20 <fdt_find_or_add_subnode+0x5c>
81605d10:	eb003259 	bl	8161267c <printf>
81605d14:	e1a00004 	mov	r0, r4
81605d18:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81605d1c:	816140c0 	.word	0x816140c0
81605d20:	81615c04 	.word	0x81615c04

81605d24 <fdt_root>:
81605d24:	e92d4037 	push	{r0, r1, r2, r4, r5, lr}
81605d28:	e1a05000 	mov	r5, r0
81605d2c:	eb00240f 	bl	8160ed70 <fdt_check_header>
81605d30:	e2504000 	subs	r4, r0, #0
81605d34:	aa000004 	bge	81605d4c <fdt_root+0x28>
81605d38:	eb0028f8 	bl	81610120 <fdt_strerror>
81605d3c:	e1a01000 	mov	r1, r0
81605d40:	e59f005c 	ldr	r0, [pc, #92]	; 81605da4 <fdt_root+0x80>
81605d44:	eb00324c 	bl	8161267c <printf>
81605d48:	ea000004 	b	81605d60 <fdt_root+0x3c>
81605d4c:	e59f0054 	ldr	r0, [pc, #84]	; 81605da8 <fdt_root+0x84>
81605d50:	ebfffa10 	bl	81604598 <env_get>
81605d54:	e2504000 	subs	r4, r0, #0
81605d58:	1a000003 	bne	81605d6c <fdt_root+0x48>
81605d5c:	e3a04000 	mov	r4, #0
81605d60:	e1a00004 	mov	r0, r4
81605d64:	e28dd00c 	add	sp, sp, #12
81605d68:	e8bd8030 	pop	{r4, r5, pc}
81605d6c:	eb002ed8 	bl	816118d4 <strlen>
81605d70:	e1a03004 	mov	r3, r4
81605d74:	e59f2030 	ldr	r2, [pc, #48]	; 81605dac <fdt_root+0x88>
81605d78:	e3a01000 	mov	r1, #0
81605d7c:	e2800001 	add	r0, r0, #1
81605d80:	e58d0000 	str	r0, [sp]
81605d84:	e1a00005 	mov	r0, r5
81605d88:	eb0027d0 	bl	8160fcd0 <fdt_setprop>
81605d8c:	e2504000 	subs	r4, r0, #0
81605d90:	aafffff1 	bge	81605d5c <fdt_root+0x38>
81605d94:	eb0028e1 	bl	81610120 <fdt_strerror>
81605d98:	e1a01000 	mov	r1, r0
81605d9c:	e59f000c 	ldr	r0, [pc, #12]	; 81605db0 <fdt_root+0x8c>
81605da0:	eaffffe7 	b	81605d44 <fdt_root+0x20>
81605da4:	81615d5b 	.word	0x81615d5b
81605da8:	81615d69 	.word	0x81615d69
81605dac:	81615d71 	.word	0x81615d71
81605db0:	81615d7f 	.word	0x81615d7f

81605db4 <fdt_initrd>:
81605db4:	e1510002 	cmp	r1, r2
81605db8:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81605dbc:	e24dd028 	sub	sp, sp, #40	; 0x28
81605dc0:	1a000003 	bne	81605dd4 <fdt_initrd+0x20>
81605dc4:	e3a04000 	mov	r4, #0
81605dc8:	e1a00004 	mov	r0, r4
81605dcc:	e28dd028 	add	sp, sp, #40	; 0x28
81605dd0:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81605dd4:	e1a06002 	mov	r6, r2
81605dd8:	e1a07001 	mov	r7, r1
81605ddc:	e59f2188 	ldr	r2, [pc, #392]	; 81605f6c <fdt_initrd+0x1b8>
81605de0:	e3a01000 	mov	r1, #0
81605de4:	e1a05000 	mov	r5, r0
81605de8:	ebffffb5 	bl	81605cc4 <fdt_find_or_add_subnode>
81605dec:	e2504000 	subs	r4, r0, #0
81605df0:	bafffff4 	blt	81605dc8 <fdt_initrd+0x14>
81605df4:	e1a00005 	mov	r0, r5
81605df8:	e3a08000 	mov	r8, #0
81605dfc:	eb00250c 	bl	8160f234 <fdt_num_mem_rsv>
81605e00:	e1a0a007 	mov	sl, r7
81605e04:	e3a0b000 	mov	fp, #0
81605e08:	e58d000c 	str	r0, [sp, #12]
81605e0c:	e59d300c 	ldr	r3, [sp, #12]
81605e10:	e1580003 	cmp	r8, r3
81605e14:	aa00000b 	bge	81605e48 <fdt_initrd+0x94>
81605e18:	e28d3018 	add	r3, sp, #24
81605e1c:	e28d2010 	add	r2, sp, #16
81605e20:	e1a01008 	mov	r1, r8
81605e24:	e1a00005 	mov	r0, r5
81605e28:	eb0024e9 	bl	8160f1d4 <fdt_get_mem_rsv>
81605e2c:	e1cd21d0 	ldrd	r2, [sp, #16]
81605e30:	e153000b 	cmp	r3, fp
81605e34:	0152000a 	cmpeq	r2, sl
81605e38:	1a000011 	bne	81605e84 <fdt_initrd+0xd0>
81605e3c:	e1a01008 	mov	r1, r8
81605e40:	e1a00005 	mov	r0, r5
81605e44:	eb00278b 	bl	8160fc78 <fdt_del_mem_rsv>
81605e48:	e0463007 	sub	r3, r6, r7
81605e4c:	e1a0200a 	mov	r2, sl
81605e50:	e3a08000 	mov	r8, #0
81605e54:	e1a00005 	mov	r0, r5
81605e58:	e88d0108 	stm	sp, {r3, r8}
81605e5c:	e1a0300b 	mov	r3, fp
81605e60:	eb002762 	bl	8160fbf0 <fdt_add_mem_rsv>
81605e64:	e250a000 	subs	sl, r0, #0
81605e68:	aa000007 	bge	81605e8c <fdt_initrd+0xd8>
81605e6c:	eb0028ab 	bl	81610120 <fdt_strerror>
81605e70:	e1a0400a 	mov	r4, sl
81605e74:	e1a01000 	mov	r1, r0
81605e78:	e59f00f0 	ldr	r0, [pc, #240]	; 81605f70 <fdt_initrd+0x1bc>
81605e7c:	eb0031fe 	bl	8161267c <printf>
81605e80:	eaffffd0 	b	81605dc8 <fdt_initrd+0x14>
81605e84:	e2888001 	add	r8, r8, #1
81605e88:	eaffffdf 	b	81605e0c <fdt_initrd+0x58>
81605e8c:	e1a01008 	mov	r1, r8
81605e90:	e1a00005 	mov	r0, r5
81605e94:	eb0028c8 	bl	816101bc <fdt_address_cells>
81605e98:	e6bf7f37 	rev	r7, r7
81605e9c:	e3500002 	cmp	r0, #2
81605ea0:	1a000011 	bne	81605eec <fdt_initrd+0x138>
81605ea4:	e3a0a008 	mov	sl, #8
81605ea8:	e28d3020 	add	r3, sp, #32
81605eac:	e58da000 	str	sl, [sp]
81605eb0:	e1a01004 	mov	r1, r4
81605eb4:	e59f20b8 	ldr	r2, [pc, #184]	; 81605f74 <fdt_initrd+0x1c0>
81605eb8:	e1a00005 	mov	r0, r5
81605ebc:	e58d7024 	str	r7, [sp, #36]	; 0x24
81605ec0:	e58d8020 	str	r8, [sp, #32]
81605ec4:	eb002781 	bl	8160fcd0 <fdt_setprop>
81605ec8:	e2507000 	subs	r7, r0, #0
81605ecc:	aa000021 	bge	81605f58 <fdt_initrd+0x1a4>
81605ed0:	e1a00007 	mov	r0, r7
81605ed4:	e1a04007 	mov	r4, r7
81605ed8:	eb002890 	bl	81610120 <fdt_strerror>
81605edc:	e1a01000 	mov	r1, r0
81605ee0:	e59f0090 	ldr	r0, [pc, #144]	; 81605f78 <fdt_initrd+0x1c4>
81605ee4:	eb0031e4 	bl	8161267c <printf>
81605ee8:	eaffffb6 	b	81605dc8 <fdt_initrd+0x14>
81605eec:	e3a08004 	mov	r8, #4
81605ef0:	e28d3020 	add	r3, sp, #32
81605ef4:	e58d8000 	str	r8, [sp]
81605ef8:	e1a01004 	mov	r1, r4
81605efc:	e59f2070 	ldr	r2, [pc, #112]	; 81605f74 <fdt_initrd+0x1c0>
81605f00:	e1a00005 	mov	r0, r5
81605f04:	e58d7020 	str	r7, [sp, #32]
81605f08:	eb002770 	bl	8160fcd0 <fdt_setprop>
81605f0c:	e2507000 	subs	r7, r0, #0
81605f10:	a58d8000 	strge	r8, [sp]
81605f14:	a6bf6f36 	revge	r6, r6
81605f18:	a58d6020 	strge	r6, [sp, #32]
81605f1c:	baffffeb 	blt	81605ed0 <fdt_initrd+0x11c>
81605f20:	e1a01004 	mov	r1, r4
81605f24:	e28d3020 	add	r3, sp, #32
81605f28:	e59f204c 	ldr	r2, [pc, #76]	; 81605f7c <fdt_initrd+0x1c8>
81605f2c:	e1a00005 	mov	r0, r5
81605f30:	eb002766 	bl	8160fcd0 <fdt_setprop>
81605f34:	e1a04000 	mov	r4, r0
81605f38:	e3540000 	cmp	r4, #0
81605f3c:	aaffffa0 	bge	81605dc4 <fdt_initrd+0x10>
81605f40:	e1a00004 	mov	r0, r4
81605f44:	eb002875 	bl	81610120 <fdt_strerror>
81605f48:	e1a01000 	mov	r1, r0
81605f4c:	e59f002c 	ldr	r0, [pc, #44]	; 81605f80 <fdt_initrd+0x1cc>
81605f50:	eb0031c9 	bl	8161267c <printf>
81605f54:	eaffff9b 	b	81605dc8 <fdt_initrd+0x14>
81605f58:	e6bf6f36 	rev	r6, r6
81605f5c:	e58d8020 	str	r8, [sp, #32]
81605f60:	e58d6024 	str	r6, [sp, #36]	; 0x24
81605f64:	e58da000 	str	sl, [sp]
81605f68:	eaffffec 	b	81605f20 <fdt_initrd+0x16c>
81605f6c:	81615082 	.word	0x81615082
81605f70:	81615ccb 	.word	0x81615ccb
81605f74:	81615cdb 	.word	0x81615cdb
81605f78:	81615cee 	.word	0x81615cee
81605f7c:	81615d4a 	.word	0x81615d4a
81605f80:	81615d1d 	.word	0x81615d1d

81605f84 <fdt_chosen>:
81605f84:	e92d4073 	push	{r0, r1, r4, r5, r6, lr}
81605f88:	e1a05000 	mov	r5, r0
81605f8c:	eb002377 	bl	8160ed70 <fdt_check_header>
81605f90:	e2504000 	subs	r4, r0, #0
81605f94:	aa000004 	bge	81605fac <fdt_chosen+0x28>
81605f98:	eb002860 	bl	81610120 <fdt_strerror>
81605f9c:	e1a01000 	mov	r1, r0
81605fa0:	e59f0074 	ldr	r0, [pc, #116]	; 8160601c <fdt_chosen+0x98>
81605fa4:	eb0031b4 	bl	8161267c <printf>
81605fa8:	ea00000a 	b	81605fd8 <fdt_chosen+0x54>
81605fac:	e59f206c 	ldr	r2, [pc, #108]	; 81606020 <fdt_chosen+0x9c>
81605fb0:	e3a01000 	mov	r1, #0
81605fb4:	e1a00005 	mov	r0, r5
81605fb8:	ebffff41 	bl	81605cc4 <fdt_find_or_add_subnode>
81605fbc:	e2504000 	subs	r4, r0, #0
81605fc0:	ba000004 	blt	81605fd8 <fdt_chosen+0x54>
81605fc4:	e59f0058 	ldr	r0, [pc, #88]	; 81606024 <fdt_chosen+0xa0>
81605fc8:	ebfff972 	bl	81604598 <env_get>
81605fcc:	e2506000 	subs	r6, r0, #0
81605fd0:	1a000003 	bne	81605fe4 <fdt_chosen+0x60>
81605fd4:	e3a04000 	mov	r4, #0
81605fd8:	e1a00004 	mov	r0, r4
81605fdc:	e28dd008 	add	sp, sp, #8
81605fe0:	e8bd8070 	pop	{r4, r5, r6, pc}
81605fe4:	eb002e3a 	bl	816118d4 <strlen>
81605fe8:	e1a01004 	mov	r1, r4
81605fec:	e1a03006 	mov	r3, r6
81605ff0:	e59f202c 	ldr	r2, [pc, #44]	; 81606024 <fdt_chosen+0xa0>
81605ff4:	e2800001 	add	r0, r0, #1
81605ff8:	e58d0000 	str	r0, [sp]
81605ffc:	e1a00005 	mov	r0, r5
81606000:	eb002732 	bl	8160fcd0 <fdt_setprop>
81606004:	e2504000 	subs	r4, r0, #0
81606008:	aafffff1 	bge	81605fd4 <fdt_chosen+0x50>
8160600c:	eb002843 	bl	81610120 <fdt_strerror>
81606010:	e1a01000 	mov	r1, r0
81606014:	e59f000c 	ldr	r0, [pc, #12]	; 81606028 <fdt_chosen+0xa4>
81606018:	eaffffe1 	b	81605fa4 <fdt_chosen+0x20>
8160601c:	81615ba2 	.word	0x81615ba2
81606020:	81615082 	.word	0x81615082
81606024:	81614ff6 	.word	0x81614ff6
81606028:	81615bb2 	.word	0x81615bb2

8160602c <do_fixup_by_path>:
8160602c:	e92d4037 	push	{r0, r1, r2, r4, r5, lr}
81606030:	e1a04001 	mov	r4, r1
81606034:	e59dc01c 	ldr	ip, [sp, #28]
81606038:	e1a05002 	mov	r5, r2
8160603c:	e58dc004 	str	ip, [sp, #4]
81606040:	e59dc018 	ldr	ip, [sp, #24]
81606044:	e58dc000 	str	ip, [sp]
81606048:	ebffff01 	bl	81605c54 <fdt_find_and_setprop>
8160604c:	e3500000 	cmp	r0, #0
81606050:	0a000007 	beq	81606074 <do_fixup_by_path+0x48>
81606054:	eb002831 	bl	81610120 <fdt_strerror>
81606058:	e1a02005 	mov	r2, r5
8160605c:	e1a01004 	mov	r1, r4
81606060:	e1a03000 	mov	r3, r0
81606064:	e59f0010 	ldr	r0, [pc, #16]	; 8160607c <do_fixup_by_path+0x50>
81606068:	e28dd00c 	add	sp, sp, #12
8160606c:	e8bd4030 	pop	{r4, r5, lr}
81606070:	ea003181 	b	8161267c <printf>
81606074:	e28dd00c 	add	sp, sp, #12
81606078:	e8bd8030 	pop	{r4, r5, pc}
8160607c:	81615791 	.word	0x81615791

81606080 <fdt_fixup_memory_banks>:
81606080:	e3530001 	cmp	r3, #1
81606084:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81606088:	e1a05003 	mov	r5, r3
8160608c:	e24dd020 	sub	sp, sp, #32
81606090:	da000008 	ble	816060b8 <fdt_fixup_memory_banks+0x38>
81606094:	e3a03001 	mov	r3, #1
81606098:	e1a02005 	mov	r2, r5
8160609c:	e59f1198 	ldr	r1, [pc, #408]	; 8160623c <fdt_fixup_memory_banks+0x1bc>
816060a0:	e3e04000 	mvn	r4, #0
816060a4:	e59f0194 	ldr	r0, [pc, #404]	; 81606240 <fdt_fixup_memory_banks+0x1c0>
816060a8:	eb003173 	bl	8161267c <printf>
816060ac:	e1a00004 	mov	r0, r4
816060b0:	e28dd020 	add	sp, sp, #32
816060b4:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816060b8:	e1a06000 	mov	r6, r0
816060bc:	e1a07001 	mov	r7, r1
816060c0:	e1a08002 	mov	r8, r2
816060c4:	eb002329 	bl	8160ed70 <fdt_check_header>
816060c8:	e2504000 	subs	r4, r0, #0
816060cc:	aa000005 	bge	816060e8 <fdt_fixup_memory_banks+0x68>
816060d0:	eb002812 	bl	81610120 <fdt_strerror>
816060d4:	e59f1160 	ldr	r1, [pc, #352]	; 8160623c <fdt_fixup_memory_banks+0x1bc>
816060d8:	e1a02000 	mov	r2, r0
816060dc:	e59f0160 	ldr	r0, [pc, #352]	; 81606244 <fdt_fixup_memory_banks+0x1c4>
816060e0:	eb003165 	bl	8161267c <printf>
816060e4:	eafffff0 	b	816060ac <fdt_fixup_memory_banks+0x2c>
816060e8:	e59f2158 	ldr	r2, [pc, #344]	; 81606248 <fdt_fixup_memory_banks+0x1c8>
816060ec:	e3a01000 	mov	r1, #0
816060f0:	e1a00006 	mov	r0, r6
816060f4:	ebfffef2 	bl	81605cc4 <fdt_find_or_add_subnode>
816060f8:	e2504000 	subs	r4, r0, #0
816060fc:	baffffea 	blt	816060ac <fdt_fixup_memory_banks+0x2c>
81606100:	e3a03007 	mov	r3, #7
81606104:	e59f2140 	ldr	r2, [pc, #320]	; 8160624c <fdt_fixup_memory_banks+0x1cc>
81606108:	e58d3000 	str	r3, [sp]
8160610c:	e1a01004 	mov	r1, r4
81606110:	e59f3130 	ldr	r3, [pc, #304]	; 81606248 <fdt_fixup_memory_banks+0x1c8>
81606114:	e1a00006 	mov	r0, r6
81606118:	eb0026ec 	bl	8160fcd0 <fdt_setprop>
8160611c:	e250a000 	subs	sl, r0, #0
81606120:	aa000006 	bge	81606140 <fdt_fixup_memory_banks+0xc0>
81606124:	eb0027fd 	bl	81610120 <fdt_strerror>
81606128:	e59f111c 	ldr	r1, [pc, #284]	; 8160624c <fdt_fixup_memory_banks+0x1cc>
8160612c:	e1a0400a 	mov	r4, sl
81606130:	e1a02000 	mov	r2, r0
81606134:	e59f0114 	ldr	r0, [pc, #276]	; 81606250 <fdt_fixup_memory_banks+0x1d0>
81606138:	eb00314f 	bl	8161267c <printf>
8160613c:	eaffffda 	b	816060ac <fdt_fixup_memory_banks+0x2c>
81606140:	e3550000 	cmp	r5, #0
81606144:	1a000001 	bne	81606150 <fdt_fixup_memory_banks+0xd0>
81606148:	e3a04000 	mov	r4, #0
8160614c:	eaffffd6 	b	816060ac <fdt_fixup_memory_banks+0x2c>
81606150:	e3a01000 	mov	r1, #0
81606154:	e1a00006 	mov	r0, r6
81606158:	eb002817 	bl	816101bc <fdt_address_cells>
8160615c:	e3a01000 	mov	r1, #0
81606160:	e1a0a000 	mov	sl, r0
81606164:	e1a00006 	mov	r0, r6
81606168:	eb002826 	bl	81610208 <fdt_size_cells>
8160616c:	e28d2010 	add	r2, sp, #16
81606170:	e1a0c10a 	lsl	ip, sl, #2
81606174:	e3a03000 	mov	r3, #0
81606178:	e1a01100 	lsl	r1, r0, #2
8160617c:	e081e00c 	add	lr, r1, ip
81606180:	e58de008 	str	lr, [sp, #8]
81606184:	e1550003 	cmp	r5, r3
81606188:	ca00000f 	bgt	816061cc <fdt_fixup_memory_banks+0x14c>
8160618c:	e081100c 	add	r1, r1, ip
81606190:	e1c55fc5 	bic	r5, r5, r5, asr #31
81606194:	e28d3010 	add	r3, sp, #16
81606198:	e59f20b4 	ldr	r2, [pc, #180]	; 81606254 <fdt_fixup_memory_banks+0x1d4>
8160619c:	e0050591 	mul	r5, r1, r5
816061a0:	e1a00006 	mov	r0, r6
816061a4:	e1a01004 	mov	r1, r4
816061a8:	e58d5000 	str	r5, [sp]
816061ac:	eb0026c7 	bl	8160fcd0 <fdt_setprop>
816061b0:	e2504000 	subs	r4, r0, #0
816061b4:	aaffffe3 	bge	81606148 <fdt_fixup_memory_banks+0xc8>
816061b8:	eb0027d8 	bl	81610120 <fdt_strerror>
816061bc:	e59f1090 	ldr	r1, [pc, #144]	; 81606254 <fdt_fixup_memory_banks+0x1d4>
816061c0:	e1a02000 	mov	r2, r0
816061c4:	e59f0084 	ldr	r0, [pc, #132]	; 81606250 <fdt_fixup_memory_banks+0x1d0>
816061c8:	eaffffc4 	b	816060e0 <fdt_fixup_memory_banks+0x60>
816061cc:	e35a0002 	cmp	sl, #2
816061d0:	e797e183 	ldr	lr, [r7, r3, lsl #3]
816061d4:	0087b183 	addeq	fp, r7, r3, lsl #3
816061d8:	e6bfef3e 	rev	lr, lr
816061dc:	0797e183 	ldreq	lr, [r7, r3, lsl #3]
816061e0:	059bb004 	ldreq	fp, [fp, #4]
816061e4:	1582e000 	strne	lr, [r2]
816061e8:	06bfef3e 	reveq	lr, lr
816061ec:	06bfbf3b 	reveq	fp, fp
816061f0:	08824800 	stmeq	r2, {fp, lr}
816061f4:	e3500002 	cmp	r0, #2
816061f8:	0088b183 	addeq	fp, r8, r3, lsl #3
816061fc:	e082e00c 	add	lr, r2, ip
81606200:	e58de00c 	str	lr, [sp, #12]
81606204:	059bb004 	ldreq	fp, [fp, #4]
81606208:	e798e183 	ldr	lr, [r8, r3, lsl #3]
8160620c:	06bfbf3b 	reveq	fp, fp
81606210:	e6bfef3e 	rev	lr, lr
81606214:	0798e183 	ldreq	lr, [r8, r3, lsl #3]
81606218:	0782b00c 	streq	fp, [r2, ip]
8160621c:	0082b00c 	addeq	fp, r2, ip
81606220:	06bfef3e 	reveq	lr, lr
81606224:	1782e00c 	strne	lr, [r2, ip]
81606228:	058be004 	streq	lr, [fp, #4]
8160622c:	e2833001 	add	r3, r3, #1
81606230:	e59de008 	ldr	lr, [sp, #8]
81606234:	e082200e 	add	r2, r2, lr
81606238:	eaffffd1 	b	81606184 <fdt_fixup_memory_banks+0x104>
8160623c:	8161406f 	.word	0x8161406f
81606240:	81615c46 	.word	0x81615c46
81606244:	81615c08 	.word	0x81615c08
81606248:	81616d66 	.word	0x81616d66
8160624c:	81615c9c 	.word	0x81615c9c
81606250:	81615ca8 	.word	0x81615ca8
81606254:	81615cc7 	.word	0x81615cc7

81606258 <fdt_fixup_memory>:
81606258:	e92d4007 	push	{r0, r1, r2, lr}
8160625c:	e28d1008 	add	r1, sp, #8
81606260:	e16120f8 	strd	r2, [r1, #-8]!
81606264:	e3a03001 	mov	r3, #1
81606268:	e28d2010 	add	r2, sp, #16
8160626c:	ebffff83 	bl	81606080 <fdt_fixup_memory_banks>
81606270:	e28dd00c 	add	sp, sp, #12
81606274:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

81606278 <fdt_fixup_ethernet>:
81606278:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160627c:	e24dd030 	sub	sp, sp, #48	; 0x30
81606280:	e59f1190 	ldr	r1, [pc, #400]	; 81606418 <fdt_fixup_ethernet+0x1a0>
81606284:	e1a04000 	mov	r4, r0
81606288:	eb00252b 	bl	8160f73c <fdt_path_offset>
8160628c:	e3500000 	cmp	r0, #0
81606290:	ba00005e 	blt	81606410 <fdt_fixup_ethernet+0x198>
81606294:	e59fb17c 	ldr	fp, [pc, #380]	; 81606418 <fdt_fixup_ethernet+0x1a0>
81606298:	e3a06000 	mov	r6, #0
8160629c:	e59f8178 	ldr	r8, [pc, #376]	; 8160641c <fdt_fixup_ethernet+0x1a4>
816062a0:	e1a0100b 	mov	r1, fp
816062a4:	e1a00004 	mov	r0, r4
816062a8:	eb002523 	bl	8160f73c <fdt_path_offset>
816062ac:	e3a05000 	mov	r5, #0
816062b0:	e1a01000 	mov	r1, r0
816062b4:	e1a00004 	mov	r0, r4
816062b8:	eb00244a 	bl	8160f3e8 <fdt_first_property_offset>
816062bc:	e1a01000 	mov	r1, r0
816062c0:	e1550006 	cmp	r5, r6
816062c4:	1a00001b 	bne	81606338 <fdt_fixup_ethernet+0xc0>
816062c8:	e3510000 	cmp	r1, #0
816062cc:	ba00004f 	blt	81606410 <fdt_fixup_ethernet+0x198>
816062d0:	e3a03000 	mov	r3, #0
816062d4:	e28d2014 	add	r2, sp, #20
816062d8:	e1a00004 	mov	r0, r4
816062dc:	eb0024ab 	bl	8160f590 <fdt_getprop_by_offset>
816062e0:	e3a02008 	mov	r2, #8
816062e4:	e1a01008 	mov	r1, r8
816062e8:	e1a0a000 	mov	sl, r0
816062ec:	e59d0014 	ldr	r0, [sp, #20]
816062f0:	eb002d5c 	bl	81611868 <strncmp>
816062f4:	e2505000 	subs	r5, r0, #0
816062f8:	1a00000c 	bne	81606330 <fdt_fixup_ethernet+0xb8>
816062fc:	e1a01008 	mov	r1, r8
81606300:	e59d0014 	ldr	r0, [sp, #20]
81606304:	eb002d4d 	bl	81611840 <strcmp>
81606308:	e3500000 	cmp	r0, #0
8160630c:	1a00000e 	bne	8160634c <fdt_fixup_ethernet+0xd4>
81606310:	e59f1108 	ldr	r1, [pc, #264]	; 81606420 <fdt_fixup_ethernet+0x1a8>
81606314:	e28d0020 	add	r0, sp, #32
81606318:	eb002d39 	bl	81611804 <strcpy>
8160631c:	e28d0020 	add	r0, sp, #32
81606320:	ebfff89c 	bl	81604598 <env_get>
81606324:	e2503000 	subs	r3, r0, #0
81606328:	128d7018 	addne	r7, sp, #24
8160632c:	1a000013 	bne	81606380 <fdt_fixup_ethernet+0x108>
81606330:	e2866001 	add	r6, r6, #1
81606334:	eaffffd9 	b	816062a0 <fdt_fixup_ethernet+0x28>
81606338:	e1a00004 	mov	r0, r4
8160633c:	e2855001 	add	r5, r5, #1
81606340:	eb002430 	bl	8160f408 <fdt_next_property_offset>
81606344:	e1a01000 	mov	r1, r0
81606348:	eaffffdc 	b	816062c0 <fdt_fixup_ethernet+0x48>
8160634c:	e59d0014 	ldr	r0, [sp, #20]
81606350:	eb0031f0 	bl	81612b18 <trailing_strtol>
81606354:	e3700001 	cmn	r0, #1
81606358:	e1a02000 	mov	r2, r0
8160635c:	0afffff3 	beq	81606330 <fdt_fixup_ethernet+0xb8>
81606360:	e3500000 	cmp	r0, #0
81606364:	0affffe9 	beq	81606310 <fdt_fixup_ethernet+0x98>
81606368:	e59f10b4 	ldr	r1, [pc, #180]	; 81606424 <fdt_fixup_ethernet+0x1ac>
8160636c:	e28d0020 	add	r0, sp, #32
81606370:	eb0030b7 	bl	81612654 <sprintf>
81606374:	eaffffe8 	b	8160631c <fdt_fixup_ethernet+0xa4>
81606378:	e3530000 	cmp	r3, #0
8160637c:	0a000021 	beq	81606408 <fdt_fixup_ethernet+0x190>
81606380:	e3a02010 	mov	r2, #16
81606384:	e1a00003 	mov	r0, r3
81606388:	e08d1002 	add	r1, sp, r2
8160638c:	e58d300c 	str	r3, [sp, #12]
81606390:	eb003100 	bl	81612798 <simple_strtoul>
81606394:	e59d300c 	ldr	r3, [sp, #12]
81606398:	e6ef0070 	uxtb	r0, r0
8160639c:	e3530000 	cmp	r3, #0
816063a0:	e7c50007 	strb	r0, [r5, r7]
816063a4:	0a000003 	beq	816063b8 <fdt_fixup_ethernet+0x140>
816063a8:	e59d3010 	ldr	r3, [sp, #16]
816063ac:	e5d32000 	ldrb	r2, [r3]
816063b0:	e3520000 	cmp	r2, #0
816063b4:	12833001 	addne	r3, r3, #1
816063b8:	e2855001 	add	r5, r5, #1
816063bc:	e3550006 	cmp	r5, #6
816063c0:	1affffec 	bne	81606378 <fdt_fixup_ethernet+0x100>
816063c4:	e1a0100a 	mov	r1, sl
816063c8:	e1a00004 	mov	r0, r4
816063cc:	e3a03000 	mov	r3, #0
816063d0:	e58d5000 	str	r5, [sp]
816063d4:	e58d3004 	str	r3, [sp, #4]
816063d8:	e1a03007 	mov	r3, r7
816063dc:	e59f2044 	ldr	r2, [pc, #68]	; 81606428 <fdt_fixup_ethernet+0x1b0>
816063e0:	ebffff11 	bl	8160602c <do_fixup_by_path>
816063e4:	e3a03001 	mov	r3, #1
816063e8:	e58d5000 	str	r5, [sp]
816063ec:	e1a0100a 	mov	r1, sl
816063f0:	e58d3004 	str	r3, [sp, #4]
816063f4:	e1a00004 	mov	r0, r4
816063f8:	e1a03007 	mov	r3, r7
816063fc:	e59f2028 	ldr	r2, [pc, #40]	; 8160642c <fdt_fixup_ethernet+0x1b4>
81606400:	ebffff09 	bl	8160602c <do_fixup_by_path>
81606404:	eaffffc9 	b	81606330 <fdt_fixup_ethernet+0xb8>
81606408:	e1a00003 	mov	r0, r3
8160640c:	eaffffe2 	b	8160639c <fdt_fixup_ethernet+0x124>
81606410:	e28dd030 	add	sp, sp, #48	; 0x30
81606414:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81606418:	81615c10 	.word	0x81615c10
8160641c:	81615c19 	.word	0x81615c19
81606420:	81615c22 	.word	0x81615c22
81606424:	81615c2a 	.word	0x81615c2a
81606428:	81615c3a 	.word	0x81615c3a
8160642c:	81615c34 	.word	0x81615c34

81606430 <fdt_shrink_to_minimum>:
81606430:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
81606434:	e2508000 	subs	r8, r0, #0
81606438:	e24dd01c 	sub	sp, sp, #28
8160643c:	01a00008 	moveq	r0, r8
81606440:	0a00002b 	beq	816064f4 <fdt_shrink_to_minimum+0xc4>
81606444:	e1a05001 	mov	r5, r1
81606448:	eb002379 	bl	8160f234 <fdt_num_mem_rsv>
8160644c:	e3a04000 	mov	r4, #0
81606450:	e1a06008 	mov	r6, r8
81606454:	e3a07000 	mov	r7, #0
81606458:	e1a0a000 	mov	sl, r0
8160645c:	e154000a 	cmp	r4, sl
81606460:	aa00000b 	bge	81606494 <fdt_shrink_to_minimum+0x64>
81606464:	e28d3010 	add	r3, sp, #16
81606468:	e28d2008 	add	r2, sp, #8
8160646c:	e1a01004 	mov	r1, r4
81606470:	e1a00008 	mov	r0, r8
81606474:	eb002356 	bl	8160f1d4 <fdt_get_mem_rsv>
81606478:	e1cd20d8 	ldrd	r2, [sp, #8]
8160647c:	e1530007 	cmp	r3, r7
81606480:	01520006 	cmpeq	r2, r6
81606484:	1a00001c 	bne	816064fc <fdt_shrink_to_minimum+0xcc>
81606488:	e1a01004 	mov	r1, r4
8160648c:	e1a00008 	mov	r0, r8
81606490:	eb0025f8 	bl	8160fc78 <fdt_del_mem_rsv>
81606494:	e598400c 	ldr	r4, [r8, #12]
81606498:	e2851d41 	add	r1, r5, #4160	; 0x1040
8160649c:	e281100f 	add	r1, r1, #15
816064a0:	e7eb0058 	ubfx	r0, r8, #0, #12
816064a4:	e6bf2f34 	rev	r2, r4
816064a8:	e5984020 	ldr	r4, [r8, #32]
816064ac:	e6bf3f34 	rev	r3, r4
816064b0:	e0814002 	add	r4, r1, r2
816064b4:	e0844003 	add	r4, r4, r3
816064b8:	e1a02006 	mov	r2, r6
816064bc:	e0844000 	add	r4, r4, r0
816064c0:	e3c44eff 	bic	r4, r4, #4080	; 0xff0
816064c4:	e3c4400f 	bic	r4, r4, #15
816064c8:	e0444000 	sub	r4, r4, r0
816064cc:	e1a00008 	mov	r0, r8
816064d0:	e6bf3f34 	rev	r3, r4
816064d4:	e5883004 	str	r3, [r8, #4]
816064d8:	e3a03000 	mov	r3, #0
816064dc:	e58d4000 	str	r4, [sp]
816064e0:	e58d3004 	str	r3, [sp, #4]
816064e4:	e1a03007 	mov	r3, r7
816064e8:	eb0025c0 	bl	8160fbf0 <fdt_add_mem_rsv>
816064ec:	e3500000 	cmp	r0, #0
816064f0:	a1a00004 	movge	r0, r4
816064f4:	e28dd01c 	add	sp, sp, #28
816064f8:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
816064fc:	e2844001 	add	r4, r4, #1
81606500:	eaffffd5 	b	8160645c <fdt_shrink_to_minimum+0x2c>

81606504 <delete_char.part.0>:
81606504:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81606508:	e1a07003 	mov	r7, r3
8160650c:	e5513001 	ldrb	r3, [r1, #-1]
81606510:	e1a04002 	mov	r4, r2
81606514:	e59d8020 	ldr	r8, [sp, #32]
81606518:	e2416001 	sub	r6, r1, #1
8160651c:	e3530009 	cmp	r3, #9
81606520:	01a05000 	moveq	r5, r0
81606524:	059fa0a0 	ldreq	sl, [pc, #160]	; 816065cc <delete_char.part.0+0xc8>
81606528:	0a00000a 	beq	81606558 <delete_char.part.0+0x54>
8160652c:	e59f0098 	ldr	r0, [pc, #152]	; 816065cc <delete_char.part.0+0xc8>
81606530:	eb000217 	bl	81606d94 <puts>
81606534:	e5943000 	ldr	r3, [r4]
81606538:	e2433001 	sub	r3, r3, #1
8160653c:	e5843000 	str	r3, [r4]
81606540:	ea00000a 	b	81606570 <delete_char.part.0+0x6c>
81606544:	e1a0000a 	mov	r0, sl
81606548:	eb000211 	bl	81606d94 <puts>
8160654c:	e5943000 	ldr	r3, [r4]
81606550:	e2433001 	sub	r3, r3, #1
81606554:	e5843000 	str	r3, [r4]
81606558:	e5943000 	ldr	r3, [r4]
8160655c:	e1530008 	cmp	r3, r8
81606560:	cafffff7 	bgt	81606544 <delete_char.part.0+0x40>
81606564:	e59f8064 	ldr	r8, [pc, #100]	; 816065d0 <delete_char.part.0+0xcc>
81606568:	e1560005 	cmp	r6, r5
8160656c:	8a000004 	bhi	81606584 <delete_char.part.0+0x80>
81606570:	e5973000 	ldr	r3, [r7]
81606574:	e1a00006 	mov	r0, r6
81606578:	e2433001 	sub	r3, r3, #1
8160657c:	e5873000 	str	r3, [r7]
81606580:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81606584:	e5d53000 	ldrb	r3, [r5]
81606588:	e5940000 	ldr	r0, [r4]
8160658c:	e3530009 	cmp	r3, #9
81606590:	1a000008 	bne	816065b8 <delete_char.part.0+0xb4>
81606594:	e2000007 	and	r0, r0, #7
81606598:	e0880000 	add	r0, r8, r0
8160659c:	eb0001fc 	bl	81606d94 <puts>
816065a0:	e5943000 	ldr	r3, [r4]
816065a4:	e3c33007 	bic	r3, r3, #7
816065a8:	e2833008 	add	r3, r3, #8
816065ac:	e5843000 	str	r3, [r4]
816065b0:	e2855001 	add	r5, r5, #1
816065b4:	eaffffeb 	b	81606568 <delete_char.part.0+0x64>
816065b8:	e2800001 	add	r0, r0, #1
816065bc:	e5840000 	str	r0, [r4]
816065c0:	e5d50000 	ldrb	r0, [r5]
816065c4:	eb0001e8 	bl	81606d6c <putc>
816065c8:	eafffff8 	b	816065b0 <delete_char.part.0+0xac>
816065cc:	81615b8f 	.word	0x81615b8f
816065d0:	81617b79 	.word	0x81617b79

816065d4 <cli_readline_into_buffer>:
816065d4:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816065d8:	e2508000 	subs	r8, r0, #0
816065dc:	e24dd018 	sub	sp, sp, #24
816065e0:	e3a03000 	mov	r3, #0
816065e4:	e1a06001 	mov	r6, r1
816065e8:	01a07008 	moveq	r7, r8
816065ec:	e58d3010 	str	r3, [sp, #16]
816065f0:	0a000003 	beq	81606604 <cli_readline_into_buffer+0x30>
816065f4:	eb002cb6 	bl	816118d4 <strlen>
816065f8:	e1a07000 	mov	r7, r0
816065fc:	e1a00008 	mov	r0, r8
81606600:	eb0001e3 	bl	81606d94 <puts>
81606604:	e59fa1cc 	ldr	sl, [pc, #460]	; 816067d8 <cli_readline_into_buffer+0x204>
81606608:	e1a04006 	mov	r4, r6
8160660c:	e59fb1c8 	ldr	fp, [pc, #456]	; 816067dc <cli_readline_into_buffer+0x208>
81606610:	e58d7014 	str	r7, [sp, #20]
81606614:	eb0001b9 	bl	81606d00 <getc>
81606618:	e6ef5070 	uxtb	r5, r0
8160661c:	e355000a 	cmp	r5, #10
81606620:	0a00002a 	beq	816066d0 <cli_readline_into_buffer+0xfc>
81606624:	ca000023 	bgt	816066b8 <cli_readline_into_buffer+0xe4>
81606628:	e3550003 	cmp	r5, #3
8160662c:	0a00003d 	beq	81606728 <cli_readline_into_buffer+0x154>
81606630:	e3550008 	cmp	r5, #8
81606634:	0a000030 	beq	816066fc <cli_readline_into_buffer+0x128>
81606638:	e3550000 	cmp	r5, #0
8160663c:	0afffff4 	beq	81606614 <cli_readline_into_buffer+0x40>
81606640:	e59d3010 	ldr	r3, [sp, #16]
81606644:	e35300fd 	cmp	r3, #253	; 0xfd
81606648:	ca00005f 	bgt	816067cc <cli_readline_into_buffer+0x1f8>
8160664c:	e3550009 	cmp	r5, #9
81606650:	1a000054 	bne	816067a8 <cli_readline_into_buffer+0x1d4>
81606654:	e3a03000 	mov	r3, #0
81606658:	e28d2010 	add	r2, sp, #16
8160665c:	e5c43000 	strb	r3, [r4]
81606660:	e1a0100a 	mov	r1, sl
81606664:	e28d3014 	add	r3, sp, #20
81606668:	e1a00008 	mov	r0, r8
8160666c:	eb000ab0 	bl	81609134 <cmd_auto_complete>
81606670:	e3500000 	cmp	r0, #0
81606674:	159d4010 	ldrne	r4, [sp, #16]
81606678:	10864004 	addne	r4, r6, r4
8160667c:	1affffe4 	bne	81606614 <cli_readline_into_buffer+0x40>
81606680:	e59d0014 	ldr	r0, [sp, #20]
81606684:	e2000007 	and	r0, r0, #7
81606688:	e08b0000 	add	r0, fp, r0
8160668c:	eb0001c0 	bl	81606d94 <puts>
81606690:	e59d3014 	ldr	r3, [sp, #20]
81606694:	e3c33007 	bic	r3, r3, #7
81606698:	e2833008 	add	r3, r3, #8
8160669c:	e58d3014 	str	r3, [sp, #20]
816066a0:	e5c45000 	strb	r5, [r4]
816066a4:	e2844001 	add	r4, r4, #1
816066a8:	e59d3010 	ldr	r3, [sp, #16]
816066ac:	e2833001 	add	r3, r3, #1
816066b0:	e58d3010 	str	r3, [sp, #16]
816066b4:	eaffffd6 	b	81606614 <cli_readline_into_buffer+0x40>
816066b8:	e3550015 	cmp	r5, #21
816066bc:	059f411c 	ldreq	r4, [pc, #284]	; 816067e0 <cli_readline_into_buffer+0x20c>
816066c0:	0a000021 	beq	8160674c <cli_readline_into_buffer+0x178>
816066c4:	ca000008 	bgt	816066ec <cli_readline_into_buffer+0x118>
816066c8:	e355000d 	cmp	r5, #13
816066cc:	1affffdb 	bne	81606640 <cli_readline_into_buffer+0x6c>
816066d0:	e3a03000 	mov	r3, #0
816066d4:	e59f0108 	ldr	r0, [pc, #264]	; 816067e4 <cli_readline_into_buffer+0x210>
816066d8:	e5c43000 	strb	r3, [r4]
816066dc:	eb0001ac 	bl	81606d94 <puts>
816066e0:	e0440006 	sub	r0, r4, r6
816066e4:	e28dd018 	add	sp, sp, #24
816066e8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816066ec:	e3550017 	cmp	r5, #23
816066f0:	0a00001c 	beq	81606768 <cli_readline_into_buffer+0x194>
816066f4:	e355007f 	cmp	r5, #127	; 0x7f
816066f8:	1affffd0 	bne	81606640 <cli_readline_into_buffer+0x6c>
816066fc:	e59d3010 	ldr	r3, [sp, #16]
81606700:	e3530000 	cmp	r3, #0
81606704:	0affffc2 	beq	81606614 <cli_readline_into_buffer+0x40>
81606708:	e1a01004 	mov	r1, r4
8160670c:	e58d7000 	str	r7, [sp]
81606710:	e28d3010 	add	r3, sp, #16
81606714:	e28d2014 	add	r2, sp, #20
81606718:	e1a00006 	mov	r0, r6
8160671c:	ebffff78 	bl	81606504 <delete_char.part.0>
81606720:	e1a04000 	mov	r4, r0
81606724:	eaffffba 	b	81606614 <cli_readline_into_buffer+0x40>
81606728:	e3a03000 	mov	r3, #0
8160672c:	e3e00000 	mvn	r0, #0
81606730:	e5c63000 	strb	r3, [r6]
81606734:	eaffffea 	b	816066e4 <cli_readline_into_buffer+0x110>
81606738:	e1a00004 	mov	r0, r4
8160673c:	eb000194 	bl	81606d94 <puts>
81606740:	e59d3014 	ldr	r3, [sp, #20]
81606744:	e2433001 	sub	r3, r3, #1
81606748:	e58d3014 	str	r3, [sp, #20]
8160674c:	e59d3014 	ldr	r3, [sp, #20]
81606750:	e1530007 	cmp	r3, r7
81606754:	cafffff7 	bgt	81606738 <cli_readline_into_buffer+0x164>
81606758:	e3a03000 	mov	r3, #0
8160675c:	e1a04006 	mov	r4, r6
81606760:	e58d3010 	str	r3, [sp, #16]
81606764:	eaffffaa 	b	81606614 <cli_readline_into_buffer+0x40>
81606768:	e59d3010 	ldr	r3, [sp, #16]
8160676c:	e3530000 	cmp	r3, #0
81606770:	0affffa7 	beq	81606614 <cli_readline_into_buffer+0x40>
81606774:	e28d3010 	add	r3, sp, #16
81606778:	e1a01004 	mov	r1, r4
8160677c:	e58d7000 	str	r7, [sp]
81606780:	e28d2014 	add	r2, sp, #20
81606784:	e1a00006 	mov	r0, r6
81606788:	ebffff5d 	bl	81606504 <delete_char.part.0>
8160678c:	e59d3010 	ldr	r3, [sp, #16]
81606790:	e3530000 	cmp	r3, #0
81606794:	e1a04000 	mov	r4, r0
81606798:	daffff9d 	ble	81606614 <cli_readline_into_buffer+0x40>
8160679c:	e5d03000 	ldrb	r3, [r0]
816067a0:	e3530020 	cmp	r3, #32
816067a4:	eafffff1 	b	81606770 <cli_readline_into_buffer+0x19c>
816067a8:	e59d3014 	ldr	r3, [sp, #20]
816067ac:	e28d000c 	add	r0, sp, #12
816067b0:	e5cd500c 	strb	r5, [sp, #12]
816067b4:	e2833001 	add	r3, r3, #1
816067b8:	e58d3014 	str	r3, [sp, #20]
816067bc:	e3a03000 	mov	r3, #0
816067c0:	e5cd300d 	strb	r3, [sp, #13]
816067c4:	eb000172 	bl	81606d94 <puts>
816067c8:	eaffffb4 	b	816066a0 <cli_readline_into_buffer+0xcc>
816067cc:	e3a00007 	mov	r0, #7
816067d0:	eb000165 	bl	81606d6c <putc>
816067d4:	eaffff8e 	b	81606614 <cli_readline_into_buffer+0x40>
816067d8:	816199b0 	.word	0x816199b0
816067dc:	81617b79 	.word	0x81617b79
816067e0:	81615b8f 	.word	0x81615b8f
816067e4:	81614908 	.word	0x81614908

816067e8 <cli_readline>:
816067e8:	e59f1008 	ldr	r1, [pc, #8]	; 816067f8 <cli_readline+0x10>
816067ec:	e3a02000 	mov	r2, #0
816067f0:	e5c12000 	strb	r2, [r1]
816067f4:	eaffff76 	b	816065d4 <cli_readline_into_buffer>
816067f8:	816199b0 	.word	0x816199b0

816067fc <cli_simple_parse_line>:
816067fc:	e92d4010 	push	{r4, lr}
81606800:	e1a0c001 	mov	ip, r1
81606804:	e3a04000 	mov	r4, #0
81606808:	e1a02000 	mov	r2, r0
8160680c:	e4d03001 	ldrb	r3, [r0], #1
81606810:	e3530009 	cmp	r3, #9
81606814:	13530020 	cmpne	r3, #32
81606818:	0afffffa 	beq	81606808 <cli_simple_parse_line+0xc>
8160681c:	e3530000 	cmp	r3, #0
81606820:	1a000002 	bne	81606830 <cli_simple_parse_line+0x34>
81606824:	e58c3000 	str	r3, [ip]
81606828:	e1a00004 	mov	r0, r4
8160682c:	e8bd8010 	pop	{r4, pc}
81606830:	e2844001 	add	r4, r4, #1
81606834:	e48c2004 	str	r2, [ip], #4
81606838:	e1a0e002 	mov	lr, r2
8160683c:	e4d23001 	ldrb	r3, [r2], #1
81606840:	e3530000 	cmp	r3, #0
81606844:	0a00000c 	beq	8160687c <cli_simple_parse_line+0x80>
81606848:	e3530020 	cmp	r3, #32
8160684c:	13530009 	cmpne	r3, #9
81606850:	e1a00002 	mov	r0, r2
81606854:	13a03001 	movne	r3, #1
81606858:	03a03000 	moveq	r3, #0
8160685c:	1afffff5 	bne	81606838 <cli_simple_parse_line+0x3c>
81606860:	e3540010 	cmp	r4, #16
81606864:	e5ce3000 	strb	r3, [lr]
81606868:	1affffe6 	bne	81606808 <cli_simple_parse_line+0xc>
8160686c:	e1a01004 	mov	r1, r4
81606870:	e59f000c 	ldr	r0, [pc, #12]	; 81606884 <cli_simple_parse_line+0x88>
81606874:	eb002f80 	bl	8161267c <printf>
81606878:	eaffffea 	b	81606828 <cli_simple_parse_line+0x2c>
8160687c:	e7813104 	str	r3, [r1, r4, lsl #2]
81606880:	eaffffe8 	b	81606828 <cli_simple_parse_line+0x2c>
81606884:	8161490b 	.word	0x8161490b

81606888 <cli_simple_process_macros>:
81606888:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160688c:	e24ddc01 	sub	sp, sp, #256	; 0x100
81606890:	e1a04001 	mov	r4, r1
81606894:	e1a07000 	mov	r7, r0
81606898:	eb002c0d 	bl	816118d4 <strlen>
8160689c:	e3a03000 	mov	r3, #0
816068a0:	e3a06c01 	mov	r6, #256	; 0x100
816068a4:	e1a0b003 	mov	fp, r3
816068a8:	e1a02003 	mov	r2, r3
816068ac:	e1a01003 	mov	r1, r3
816068b0:	e3500000 	cmp	r0, #0
816068b4:	13560000 	cmpne	r6, #0
816068b8:	1a000006 	bne	816068d8 <cli_simple_process_macros+0x50>
816068bc:	e3560000 	cmp	r6, #0
816068c0:	05446001 	strbeq	r6, [r4, #-1]
816068c4:	0a000001 	beq	816068d0 <cli_simple_process_macros+0x48>
816068c8:	e3a03000 	mov	r3, #0
816068cc:	e5c43000 	strb	r3, [r4]
816068d0:	e28ddc01 	add	sp, sp, #256	; 0x100
816068d4:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816068d8:	e3530003 	cmp	r3, #3
816068dc:	e287a001 	add	sl, r7, #1
816068e0:	e5d75000 	ldrb	r5, [r7]
816068e4:	e2408001 	sub	r8, r0, #1
816068e8:	0a00005c 	beq	81606a60 <cli_simple_process_macros+0x1d8>
816068ec:	e252205c 	subs	r2, r2, #92	; 0x5c
816068f0:	13a02001 	movne	r2, #1
816068f4:	e355005c 	cmp	r5, #92	; 0x5c
816068f8:	13a0c000 	movne	ip, #0
816068fc:	0202c001 	andeq	ip, r2, #1
81606900:	e35c0000 	cmp	ip, #0
81606904:	0a00000b 	beq	81606938 <cli_simple_process_macros+0xb0>
81606908:	e3580000 	cmp	r8, #0
8160690c:	e2400002 	sub	r0, r0, #2
81606910:	0affffec 	beq	816068c8 <cli_simple_process_macros+0x40>
81606914:	e3530001 	cmp	r3, #1
81606918:	e287a002 	add	sl, r7, #2
8160691c:	e5d75001 	ldrb	r5, [r7, #1]
81606920:	0a000019 	beq	8160698c <cli_simple_process_macros+0x104>
81606924:	e3530002 	cmp	r3, #2
81606928:	0a000029 	beq	816069d4 <cli_simple_process_macros+0x14c>
8160692c:	e1a08000 	mov	r8, r0
81606930:	e3a02000 	mov	r2, #0
81606934:	ea00000a 	b	81606964 <cli_simple_process_macros+0xdc>
81606938:	e3530001 	cmp	r3, #1
8160693c:	0a000013 	beq	81606990 <cli_simple_process_macros+0x108>
81606940:	e3530002 	cmp	r3, #2
81606944:	0a000023 	beq	816069d8 <cli_simple_process_macros+0x150>
81606948:	e3550027 	cmp	r5, #39	; 0x27
8160694c:	13a03000 	movne	r3, #0
81606950:	02023001 	andeq	r3, r2, #1
81606954:	e3530000 	cmp	r3, #0
81606958:	13a03003 	movne	r3, #3
8160695c:	13a05027 	movne	r5, #39	; 0x27
81606960:	1a000017 	bne	816069c4 <cli_simple_process_macros+0x13c>
81606964:	e3550024 	cmp	r5, #36	; 0x24
81606968:	13a03000 	movne	r3, #0
8160696c:	02023001 	andeq	r3, r2, #1
81606970:	e3530000 	cmp	r3, #0
81606974:	13a03001 	movne	r3, #1
81606978:	1a000011 	bne	816069c4 <cli_simple_process_macros+0x13c>
8160697c:	e5c45000 	strb	r5, [r4]
81606980:	e2466001 	sub	r6, r6, #1
81606984:	e2844001 	add	r4, r4, #1
81606988:	ea00000d 	b	816069c4 <cli_simple_process_macros+0x13c>
8160698c:	e1a08000 	mov	r8, r0
81606990:	e355007b 	cmp	r5, #123	; 0x7b
81606994:	13550028 	cmpne	r5, #40	; 0x28
81606998:	03a03001 	moveq	r3, #1
8160699c:	13a03000 	movne	r3, #0
816069a0:	0a000036 	beq	81606a80 <cli_simple_process_macros+0x1f8>
816069a4:	e3560001 	cmp	r6, #1
816069a8:	e3a02024 	mov	r2, #36	; 0x24
816069ac:	15c45001 	strbne	r5, [r4, #1]
816069b0:	12466002 	subne	r6, r6, #2
816069b4:	e5c42000 	strb	r2, [r4]
816069b8:	02844001 	addeq	r4, r4, #1
816069bc:	12844002 	addne	r4, r4, #2
816069c0:	01a06003 	moveq	r6, r3
816069c4:	e1a00008 	mov	r0, r8
816069c8:	e1a02005 	mov	r2, r5
816069cc:	e1a0700a 	mov	r7, sl
816069d0:	eaffffb6 	b	816068b0 <cli_simple_process_macros+0x28>
816069d4:	e1a08000 	mov	r8, r0
816069d8:	e355007d 	cmp	r5, #125	; 0x7d
816069dc:	13550029 	cmpne	r5, #41	; 0x29
816069e0:	004a300b 	subeq	r3, sl, fp
816069e4:	02430001 	subeq	r0, r3, #1
816069e8:	03a02000 	moveq	r2, #0
816069ec:	0a000004 	beq	81606a04 <cli_simple_process_macros+0x17c>
816069f0:	e3a03002 	mov	r3, #2
816069f4:	eafffff2 	b	816069c4 <cli_simple_process_macros+0x13c>
816069f8:	e7dbc002 	ldrb	ip, [fp, r2]
816069fc:	e7cdc002 	strb	ip, [sp, r2]
81606a00:	e2822001 	add	r2, r2, #1
81606a04:	e1520000 	cmp	r2, r0
81606a08:	bafffffa 	blt	816069f8 <cli_simple_process_macros+0x170>
81606a0c:	e3530001 	cmp	r3, #1
81606a10:	e28d2c01 	add	r2, sp, #256	; 0x100
81606a14:	e1a0000d 	mov	r0, sp
81606a18:	b3a03001 	movlt	r3, #1
81606a1c:	e0823003 	add	r3, r2, r3
81606a20:	e5431101 	strb	r1, [r3, #-257]	; 0xfffffeff
81606a24:	ebfff6db 	bl	81604598 <env_get>
81606a28:	e3a01000 	mov	r1, #0
81606a2c:	e3500000 	cmp	r0, #0
81606a30:	12400001 	subne	r0, r0, #1
81606a34:	1a000003 	bne	81606a48 <cli_simple_process_macros+0x1c0>
81606a38:	e3a03000 	mov	r3, #0
81606a3c:	eaffffe0 	b	816069c4 <cli_simple_process_macros+0x13c>
81606a40:	e2466001 	sub	r6, r6, #1
81606a44:	e4c43001 	strb	r3, [r4], #1
81606a48:	e5f03001 	ldrb	r3, [r0, #1]!
81606a4c:	e3530000 	cmp	r3, #0
81606a50:	0afffff8 	beq	81606a38 <cli_simple_process_macros+0x1b0>
81606a54:	e3560000 	cmp	r6, #0
81606a58:	1afffff8 	bne	81606a40 <cli_simple_process_macros+0x1b8>
81606a5c:	eafffff5 	b	81606a38 <cli_simple_process_macros+0x1b0>
81606a60:	e2450027 	sub	r0, r5, #39	; 0x27
81606a64:	e352005c 	cmp	r2, #92	; 0x5c
81606a68:	e16f0f10 	clz	r0, r0
81606a6c:	e1a002a0 	lsr	r0, r0, #5
81606a70:	03a00000 	moveq	r0, #0
81606a74:	e3500000 	cmp	r0, #0
81606a78:	1affffee 	bne	81606a38 <cli_simple_process_macros+0x1b0>
81606a7c:	eaffffbe 	b	8160697c <cli_simple_process_macros+0xf4>
81606a80:	e1a0b00a 	mov	fp, sl
81606a84:	eaffffd9 	b	816069f0 <cli_simple_process_macros+0x168>

81606a88 <cli_simple_run_command>:
81606a88:	e92d40f0 	push	{r4, r5, r6, r7, lr}
81606a8c:	e24ddf95 	sub	sp, sp, #596	; 0x254
81606a90:	e1a04000 	mov	r4, r0
81606a94:	e3a03001 	mov	r3, #1
81606a98:	e1a06001 	mov	r6, r1
81606a9c:	e58d3008 	str	r3, [sp, #8]
81606aa0:	eb0000d4 	bl	81606df8 <clear_ctrlc>
81606aa4:	e3540000 	cmp	r4, #0
81606aa8:	1a000002 	bne	81606ab8 <cli_simple_run_command+0x30>
81606aac:	e3e00000 	mvn	r0, #0
81606ab0:	e28ddf95 	add	sp, sp, #596	; 0x254
81606ab4:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81606ab8:	e5d43000 	ldrb	r3, [r4]
81606abc:	e3530000 	cmp	r3, #0
81606ac0:	0afffff9 	beq	81606aac <cli_simple_run_command+0x24>
81606ac4:	e1a00004 	mov	r0, r4
81606ac8:	eb002b81 	bl	816118d4 <strlen>
81606acc:	e35000ff 	cmp	r0, #255	; 0xff
81606ad0:	9a000002 	bls	81606ae0 <cli_simple_run_command+0x58>
81606ad4:	e59f00f8 	ldr	r0, [pc, #248]	; 81606bd4 <cli_simple_run_command+0x14c>
81606ad8:	eb0000ad 	bl	81606d94 <puts>
81606adc:	eafffff2 	b	81606aac <cli_simple_run_command+0x24>
81606ae0:	e1a01004 	mov	r1, r4
81606ae4:	e28d0050 	add	r0, sp, #80	; 0x50
81606ae8:	eb002b45 	bl	81611804 <strcpy>
81606aec:	e3a05000 	mov	r5, #0
81606af0:	e28d0050 	add	r0, sp, #80	; 0x50
81606af4:	e1a07005 	mov	r7, r5
81606af8:	e5d03000 	ldrb	r3, [r0]
81606afc:	e3530000 	cmp	r3, #0
81606b00:	1a00002b 	bne	81606bb4 <cli_simple_run_command+0x12c>
81606b04:	e3550000 	cmp	r5, #0
81606b08:	059d0008 	ldreq	r0, [sp, #8]
81606b0c:	0affffe7 	beq	81606ab0 <cli_simple_run_command+0x28>
81606b10:	eaffffe5 	b	81606aac <cli_simple_run_command+0x24>
81606b14:	e35c0027 	cmp	ip, #39	; 0x27
81606b18:	1a000002 	bne	81606b28 <cli_simple_run_command+0xa0>
81606b1c:	e5541001 	ldrb	r1, [r4, #-1]
81606b20:	e351005c 	cmp	r1, #92	; 0x5c
81606b24:	12222001 	eorne	r2, r2, #1
81606b28:	e24c303b 	sub	r3, ip, #59	; 0x3b
81606b2c:	e1540000 	cmp	r4, r0
81606b30:	e16f3f13 	clz	r3, r3
81606b34:	e2221001 	eor	r1, r2, #1
81606b38:	e1a032a3 	lsr	r3, r3, #5
81606b3c:	03a03000 	moveq	r3, #0
81606b40:	e1110003 	tst	r1, r3
81606b44:	e2843001 	add	r3, r4, #1
81606b48:	0a000002 	beq	81606b58 <cli_simple_run_command+0xd0>
81606b4c:	e5541001 	ldrb	r1, [r4, #-1]
81606b50:	e351005c 	cmp	r1, #92	; 0x5c
81606b54:	1a00001b 	bne	81606bc8 <cli_simple_run_command+0x140>
81606b58:	e1a04003 	mov	r4, r3
81606b5c:	e5d4c000 	ldrb	ip, [r4]
81606b60:	e35c0000 	cmp	ip, #0
81606b64:	1affffea 	bne	81606b14 <cli_simple_run_command+0x8c>
81606b68:	e28d1e15 	add	r1, sp, #336	; 0x150
81606b6c:	ebffff45 	bl	81606888 <cli_simple_process_macros>
81606b70:	e28d100c 	add	r1, sp, #12
81606b74:	e28d0e15 	add	r0, sp, #336	; 0x150
81606b78:	ebffff1f 	bl	816067fc <cli_simple_parse_line>
81606b7c:	e2501000 	subs	r1, r0, #0
81606b80:	0a00000e 	beq	81606bc0 <cli_simple_run_command+0x138>
81606b84:	e28d3008 	add	r3, sp, #8
81606b88:	e28d200c 	add	r2, sp, #12
81606b8c:	e58d7000 	str	r7, [sp]
81606b90:	e1a00006 	mov	r0, r6
81606b94:	eb000aaa 	bl	81609644 <cmd_process>
81606b98:	e3500000 	cmp	r0, #0
81606b9c:	13e05000 	mvnne	r5, #0
81606ba0:	eb000090 	bl	81606de8 <had_ctrlc>
81606ba4:	e3500000 	cmp	r0, #0
81606ba8:	1affffbf 	bne	81606aac <cli_simple_run_command+0x24>
81606bac:	e1a00004 	mov	r0, r4
81606bb0:	eaffffd0 	b	81606af8 <cli_simple_run_command+0x70>
81606bb4:	e1a04000 	mov	r4, r0
81606bb8:	e3a02000 	mov	r2, #0
81606bbc:	eaffffe6 	b	81606b5c <cli_simple_run_command+0xd4>
81606bc0:	e3e05000 	mvn	r5, #0
81606bc4:	eafffff8 	b	81606bac <cli_simple_run_command+0x124>
81606bc8:	e5c47000 	strb	r7, [r4]
81606bcc:	e1a04003 	mov	r4, r3
81606bd0:	eaffffe4 	b	81606b68 <cli_simple_run_command+0xe0>
81606bd4:	8161492a 	.word	0x8161492a

81606bd8 <cli_simple_run_command_list>:
81606bd8:	e92d4070 	push	{r4, r5, r6, lr}
81606bdc:	e1a04000 	mov	r4, r0
81606be0:	e3a05000 	mov	r5, #0
81606be4:	e4d41001 	ldrb	r1, [r4], #1
81606be8:	e3510000 	cmp	r1, #0
81606bec:	1a000004 	bne	81606c04 <cli_simple_run_command_list+0x2c>
81606bf0:	e5d03000 	ldrb	r3, [r0]
81606bf4:	e3530000 	cmp	r3, #0
81606bf8:	1a00000f 	bne	81606c3c <cli_simple_run_command_list+0x64>
81606bfc:	e1a00003 	mov	r0, r3
81606c00:	e8bd8070 	pop	{r4, r5, r6, pc}
81606c04:	e351000a 	cmp	r1, #10
81606c08:	1afffff5 	bne	81606be4 <cli_simple_run_command_list+0xc>
81606c0c:	e5445001 	strb	r5, [r4, #-1]
81606c10:	e5d03000 	ldrb	r3, [r0]
81606c14:	e3530000 	cmp	r3, #0
81606c18:	1a000001 	bne	81606c24 <cli_simple_run_command_list+0x4c>
81606c1c:	e1a00004 	mov	r0, r4
81606c20:	eaffffef 	b	81606be4 <cli_simple_run_command_list+0xc>
81606c24:	e3a01000 	mov	r1, #0
81606c28:	ebffff96 	bl	81606a88 <cli_simple_run_command>
81606c2c:	e3500000 	cmp	r0, #0
81606c30:	aafffff9 	bge	81606c1c <cli_simple_run_command_list+0x44>
81606c34:	e3a00001 	mov	r0, #1
81606c38:	e8bd8070 	pop	{r4, r5, r6, pc}
81606c3c:	ebffff91 	bl	81606a88 <cli_simple_run_command>
81606c40:	e1a00fa0 	lsr	r0, r0, #31
81606c44:	e8bd8070 	pop	{r4, r5, r6, pc}

81606c48 <console_setfile>:
81606c48:	e92d4070 	push	{r4, r5, r6, lr}
81606c4c:	e2515000 	subs	r5, r1, #0
81606c50:	0a000022 	beq	81606ce0 <console_setfile+0x98>
81606c54:	e3500002 	cmp	r0, #2
81606c58:	8a000020 	bhi	81606ce0 <console_setfile+0x98>
81606c5c:	e5953028 	ldr	r3, [r5, #40]	; 0x28
81606c60:	e1a04000 	mov	r4, r0
81606c64:	e3530000 	cmp	r3, #0
81606c68:	0a000013 	beq	81606cbc <console_setfile+0x74>
81606c6c:	e1a00005 	mov	r0, r5
81606c70:	e12fff33 	blx	r3
81606c74:	e3500000 	cmp	r0, #0
81606c78:	b8bd8070 	poplt	{r4, r5, r6, pc}
81606c7c:	e59f3064 	ldr	r3, [pc, #100]	; 81606ce8 <console_setfile+0xa0>
81606c80:	e3540000 	cmp	r4, #0
81606c84:	e7835104 	str	r5, [r3, r4, lsl #2]
81606c88:	0a00000d 	beq	81606cc4 <console_setfile+0x7c>
81606c8c:	e3540001 	cmp	r4, #1
81606c90:	18bd8070 	popne	{r4, r5, r6, pc}
81606c94:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81606c98:	e59f104c 	ldr	r1, [pc, #76]	; 81606cec <console_setfile+0xa4>
81606c9c:	e582100c 	str	r1, [r2, #12]
81606ca0:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81606ca4:	e59f1044 	ldr	r1, [pc, #68]	; 81606cf0 <console_setfile+0xa8>
81606ca8:	e5993054 	ldr	r3, [r9, #84]	; 0x54
81606cac:	e5821010 	str	r1, [r2, #16]
81606cb0:	e59f203c 	ldr	r2, [pc, #60]	; 81606cf4 <console_setfile+0xac>
81606cb4:	e5832014 	str	r2, [r3, #20]
81606cb8:	e8bd8070 	pop	{r4, r5, r6, pc}
81606cbc:	e1a00003 	mov	r0, r3
81606cc0:	eaffffed 	b	81606c7c <console_setfile+0x34>
81606cc4:	e5992054 	ldr	r2, [r9, #84]	; 0x54
81606cc8:	e59f1028 	ldr	r1, [pc, #40]	; 81606cf8 <console_setfile+0xb0>
81606ccc:	e5993054 	ldr	r3, [r9, #84]	; 0x54
81606cd0:	e5821004 	str	r1, [r2, #4]
81606cd4:	e59f2020 	ldr	r2, [pc, #32]	; 81606cfc <console_setfile+0xb4>
81606cd8:	e5832008 	str	r2, [r3, #8]
81606cdc:	e8bd8070 	pop	{r4, r5, r6, pc}
81606ce0:	e3e00000 	mvn	r0, #0
81606ce4:	e8bd8070 	pop	{r4, r5, r6, pc}
81606ce8:	81619b48 	.word	0x81619b48
81606cec:	81606d6c 	.word	0x81606d6c
81606cf0:	81606d94 	.word	0x81606d94
81606cf4:	8161267c 	.word	0x8161267c
81606cf8:	81606d00 	.word	0x81606d00
81606cfc:	81606d2c 	.word	0x81606d2c

81606d00 <getc>:
81606d00:	e3a00000 	mov	r0, #0
81606d04:	e12fff1e 	bx	lr

81606d08 <ftstc>:
81606d08:	e3500002 	cmp	r0, #2
81606d0c:	ca000003 	bgt	81606d20 <ftstc+0x18>
81606d10:	e59f3010 	ldr	r3, [pc, #16]	; 81606d28 <ftstc+0x20>
81606d14:	e7930100 	ldr	r0, [r3, r0, lsl #2]
81606d18:	e5903038 	ldr	r3, [r0, #56]	; 0x38
81606d1c:	e12fff13 	bx	r3
81606d20:	e3e00000 	mvn	r0, #0
81606d24:	e12fff1e 	bx	lr
81606d28:	81619b48 	.word	0x81619b48

81606d2c <tstc>:
81606d2c:	e599001c 	ldr	r0, [r9, #28]
81606d30:	e3500000 	cmp	r0, #0
81606d34:	012fff1e 	bxeq	lr
81606d38:	e5993004 	ldr	r3, [r9, #4]
81606d3c:	e3130002 	tst	r3, #2
81606d40:	0a000001 	beq	81606d4c <tstc+0x20>
81606d44:	e3a00000 	mov	r0, #0
81606d48:	eaffffee 	b	81606d08 <ftstc>
81606d4c:	ea001126 	b	8160b1ec <serial_tstc>

81606d50 <fputc>:
81606d50:	e3500002 	cmp	r0, #2
81606d54:	c12fff1e 	bxgt	lr
81606d58:	e59f3008 	ldr	r3, [pc, #8]	; 81606d68 <fputc+0x18>
81606d5c:	e7930100 	ldr	r0, [r3, r0, lsl #2]
81606d60:	e5903030 	ldr	r3, [r0, #48]	; 0x30
81606d64:	e12fff13 	bx	r3
81606d68:	81619b48 	.word	0x81619b48

81606d6c <putc>:
81606d6c:	e599201c 	ldr	r2, [r9, #28]
81606d70:	e1a01000 	mov	r1, r0
81606d74:	e3520000 	cmp	r2, #0
81606d78:	012fff1e 	bxeq	lr
81606d7c:	e5993004 	ldr	r3, [r9, #4]
81606d80:	e3130002 	tst	r3, #2
81606d84:	0a000001 	beq	81606d90 <putc+0x24>
81606d88:	e3a00001 	mov	r0, #1
81606d8c:	eaffffef 	b	81606d50 <fputc>
81606d90:	ea00111a 	b	8160b200 <serial_putc>

81606d94 <puts>:
81606d94:	e92d4010 	push	{r4, lr}
81606d98:	e2404001 	sub	r4, r0, #1
81606d9c:	e5f40001 	ldrb	r0, [r4, #1]!
81606da0:	e3500000 	cmp	r0, #0
81606da4:	08bd8010 	popeq	{r4, pc}
81606da8:	ebffffef 	bl	81606d6c <putc>
81606dac:	eafffffa 	b	81606d9c <puts+0x8>

81606db0 <ctrlc>:
81606db0:	e59f302c 	ldr	r3, [pc, #44]	; 81606de4 <ctrlc+0x34>
81606db4:	e5933000 	ldr	r3, [r3]
81606db8:	e3530000 	cmp	r3, #0
81606dbc:	1a000006 	bne	81606ddc <ctrlc+0x2c>
81606dc0:	e599301c 	ldr	r3, [r9, #28]
81606dc4:	e3530000 	cmp	r3, #0
81606dc8:	0a000003 	beq	81606ddc <ctrlc+0x2c>
81606dcc:	e92d4010 	push	{r4, lr}
81606dd0:	ebffffd5 	bl	81606d2c <tstc>
81606dd4:	e3a00000 	mov	r0, #0
81606dd8:	e8bd8010 	pop	{r4, pc}
81606ddc:	e3a00000 	mov	r0, #0
81606de0:	e12fff1e 	bx	lr
81606de4:	81619ab4 	.word	0x81619ab4

81606de8 <had_ctrlc>:
81606de8:	e59f3004 	ldr	r3, [pc, #4]	; 81606df4 <had_ctrlc+0xc>
81606dec:	e5930000 	ldr	r0, [r3]
81606df0:	e12fff1e 	bx	lr
81606df4:	81619ab8 	.word	0x81619ab8

81606df8 <clear_ctrlc>:
81606df8:	e59f3008 	ldr	r3, [pc, #8]	; 81606e08 <clear_ctrlc+0x10>
81606dfc:	e3a02000 	mov	r2, #0
81606e00:	e5832000 	str	r2, [r3]
81606e04:	e12fff1e 	bx	lr
81606e08:	81619ab8 	.word	0x81619ab8

81606e0c <search_device>:
81606e0c:	e92d4010 	push	{r4, lr}
81606e10:	e1a04000 	mov	r4, r0
81606e14:	e1a00001 	mov	r0, r1
81606e18:	eb000753 	bl	81608b6c <stdio_get_by_name>
81606e1c:	e3500000 	cmp	r0, #0
81606e20:	08bd8010 	popeq	{r4, pc}
81606e24:	e5903000 	ldr	r3, [r0]
81606e28:	e1140003 	tst	r4, r3
81606e2c:	03a00000 	moveq	r0, #0
81606e30:	e8bd8010 	pop	{r4, pc}

81606e34 <console_assign>:
81606e34:	e92d4010 	push	{r4, lr}
81606e38:	e2504000 	subs	r4, r0, #0
81606e3c:	0a000005 	beq	81606e58 <console_assign+0x24>
81606e40:	ba000002 	blt	81606e50 <console_assign+0x1c>
81606e44:	e3540002 	cmp	r4, #2
81606e48:	d3a00002 	movle	r0, #2
81606e4c:	da000002 	ble	81606e5c <console_assign+0x28>
81606e50:	e3e00000 	mvn	r0, #0
81606e54:	e8bd8010 	pop	{r4, pc}
81606e58:	e3a00001 	mov	r0, #1
81606e5c:	ebffffea 	bl	81606e0c <search_device>
81606e60:	e2501000 	subs	r1, r0, #0
81606e64:	0afffff9 	beq	81606e50 <console_assign+0x1c>
81606e68:	e1a00004 	mov	r0, r4
81606e6c:	e8bd4010 	pop	{r4, lr}
81606e70:	eaffff74 	b	81606c48 <console_setfile>

81606e74 <on_console>:
81606e74:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81606e78:	e1a07001 	mov	r7, r1
81606e7c:	e59f1098 	ldr	r1, [pc, #152]	; 81606f1c <on_console+0xa8>
81606e80:	e1a05000 	mov	r5, r0
81606e84:	e1a04002 	mov	r4, r2
81606e88:	e1a06003 	mov	r6, r3
81606e8c:	eb002a6b 	bl	81611840 <strcmp>
81606e90:	e3500000 	cmp	r0, #0
81606e94:	0a00000d 	beq	81606ed0 <on_console+0x5c>
81606e98:	e59f1080 	ldr	r1, [pc, #128]	; 81606f20 <on_console+0xac>
81606e9c:	e1a00005 	mov	r0, r5
81606ea0:	eb002a66 	bl	81611840 <strcmp>
81606ea4:	e3500000 	cmp	r0, #0
81606ea8:	03a00001 	moveq	r0, #1
81606eac:	0a000007 	beq	81606ed0 <on_console+0x5c>
81606eb0:	e59f106c 	ldr	r1, [pc, #108]	; 81606f24 <on_console+0xb0>
81606eb4:	e1a00005 	mov	r0, r5
81606eb8:	eb002a60 	bl	81611840 <strcmp>
81606ebc:	e3500000 	cmp	r0, #0
81606ec0:	03a00002 	moveq	r0, #2
81606ec4:	0a000001 	beq	81606ed0 <on_console+0x5c>
81606ec8:	e3a00000 	mov	r0, #0
81606ecc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81606ed0:	e5993004 	ldr	r3, [r9, #4]
81606ed4:	e3130002 	tst	r3, #2
81606ed8:	0afffffa 	beq	81606ec8 <on_console+0x54>
81606edc:	e3540001 	cmp	r4, #1
81606ee0:	0a000006 	beq	81606f00 <on_console+0x8c>
81606ee4:	3a000001 	bcc	81606ef0 <on_console+0x7c>
81606ee8:	e3540002 	cmp	r4, #2
81606eec:	1afffff5 	bne	81606ec8 <on_console+0x54>
81606ef0:	e1a01007 	mov	r1, r7
81606ef4:	ebffffce 	bl	81606e34 <console_assign>
81606ef8:	e1a00fa0 	lsr	r0, r0, #31
81606efc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81606f00:	e3160002 	tst	r6, #2
81606f04:	1a000002 	bne	81606f14 <on_console+0xa0>
81606f08:	e1a01005 	mov	r1, r5
81606f0c:	e59f0014 	ldr	r0, [pc, #20]	; 81606f28 <on_console+0xb4>
81606f10:	eb002dd9 	bl	8161267c <printf>
81606f14:	e1a00004 	mov	r0, r4
81606f18:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81606f1c:	8161791e 	.word	0x8161791e
81606f20:	81617924 	.word	0x81617924
81606f24:	8161792b 	.word	0x8161792b
81606f28:	81615a93 	.word	0x81615a93

81606f2c <console_init_f>:
81606f2c:	e3a02001 	mov	r2, #1
81606f30:	e3a00000 	mov	r0, #0
81606f34:	e589201c 	str	r2, [r9, #28]
81606f38:	e12fff1e 	bx	lr

81606f3c <stdio_print_current_devices>:
81606f3c:	e92d4010 	push	{r4, lr}
81606f40:	e59f4084 	ldr	r4, [pc, #132]	; 81606fcc <stdio_print_current_devices+0x90>
81606f44:	e59f0084 	ldr	r0, [pc, #132]	; 81606fd0 <stdio_print_current_devices+0x94>
81606f48:	ebffff91 	bl	81606d94 <puts>
81606f4c:	e5941000 	ldr	r1, [r4]
81606f50:	e3510000 	cmp	r1, #0
81606f54:	1a000010 	bne	81606f9c <stdio_print_current_devices+0x60>
81606f58:	e59f0074 	ldr	r0, [pc, #116]	; 81606fd4 <stdio_print_current_devices+0x98>
81606f5c:	ebffff8c 	bl	81606d94 <puts>
81606f60:	e59f0070 	ldr	r0, [pc, #112]	; 81606fd8 <stdio_print_current_devices+0x9c>
81606f64:	ebffff8a 	bl	81606d94 <puts>
81606f68:	e5941004 	ldr	r1, [r4, #4]
81606f6c:	e3510000 	cmp	r1, #0
81606f70:	1a00000d 	bne	81606fac <stdio_print_current_devices+0x70>
81606f74:	e59f0060 	ldr	r0, [pc, #96]	; 81606fdc <stdio_print_current_devices+0xa0>
81606f78:	ebffff85 	bl	81606d94 <puts>
81606f7c:	e59f005c 	ldr	r0, [pc, #92]	; 81606fe0 <stdio_print_current_devices+0xa4>
81606f80:	ebffff83 	bl	81606d94 <puts>
81606f84:	e5941008 	ldr	r1, [r4, #8]
81606f88:	e3510000 	cmp	r1, #0
81606f8c:	1a00000a 	bne	81606fbc <stdio_print_current_devices+0x80>
81606f90:	e59f004c 	ldr	r0, [pc, #76]	; 81606fe4 <stdio_print_current_devices+0xa8>
81606f94:	e8bd4010 	pop	{r4, lr}
81606f98:	eaffff7d 	b	81606d94 <puts>
81606f9c:	e2811008 	add	r1, r1, #8
81606fa0:	e59f0040 	ldr	r0, [pc, #64]	; 81606fe8 <stdio_print_current_devices+0xac>
81606fa4:	eb002db4 	bl	8161267c <printf>
81606fa8:	eaffffec 	b	81606f60 <stdio_print_current_devices+0x24>
81606fac:	e2811008 	add	r1, r1, #8
81606fb0:	e59f0030 	ldr	r0, [pc, #48]	; 81606fe8 <stdio_print_current_devices+0xac>
81606fb4:	eb002db0 	bl	8161267c <printf>
81606fb8:	eaffffef 	b	81606f7c <stdio_print_current_devices+0x40>
81606fbc:	e2811008 	add	r1, r1, #8
81606fc0:	e59f0020 	ldr	r0, [pc, #32]	; 81606fe8 <stdio_print_current_devices+0xac>
81606fc4:	e8bd4010 	pop	{r4, lr}
81606fc8:	ea002dab 	b	8161267c <printf>
81606fcc:	81619b48 	.word	0x81619b48
81606fd0:	816168ea 	.word	0x816168ea
81606fd4:	816168f2 	.word	0x816168f2
81606fd8:	8161690f 	.word	0x8161690f
81606fdc:	81616917 	.word	0x81616917
81606fe0:	81616935 	.word	0x81616935
81606fe4:	8161693d 	.word	0x8161693d
81606fe8:	81616688 	.word	0x81616688

81606fec <console_init_r>:
81606fec:	e92d4070 	push	{r4, r5, r6, lr}
81606ff0:	eb0006da 	bl	81608b60 <stdio_get_list>
81606ff4:	e3a04000 	mov	r4, #0
81606ff8:	e1a05004 	mov	r5, r4
81606ffc:	e5903000 	ldr	r3, [r0]
81607000:	e1530000 	cmp	r3, r0
81607004:	e593c000 	ldr	ip, [r3]
81607008:	1a000016 	bne	81607068 <console_init_r+0x7c>
8160700c:	e3540000 	cmp	r4, #0
81607010:	1a000023 	bne	816070a4 <console_init_r+0xb8>
81607014:	e3550000 	cmp	r5, #0
81607018:	0a000002 	beq	81607028 <console_init_r+0x3c>
8160701c:	e1a01005 	mov	r1, r5
81607020:	e3a00000 	mov	r0, #0
81607024:	ebffff07 	bl	81606c48 <console_setfile>
81607028:	ebffffc3 	bl	81606f3c <stdio_print_current_devices>
8160702c:	e59f608c 	ldr	r6, [pc, #140]	; 816070c0 <console_init_r+0xd4>
81607030:	e59f508c 	ldr	r5, [pc, #140]	; 816070c4 <console_init_r+0xd8>
81607034:	e3a04000 	mov	r4, #0
81607038:	e7961104 	ldr	r1, [r6, r4, lsl #2]
8160703c:	e7950104 	ldr	r0, [r5, r4, lsl #2]
81607040:	e2844001 	add	r4, r4, #1
81607044:	e2811008 	add	r1, r1, #8
81607048:	ebfff446 	bl	81604168 <env_set>
8160704c:	e3540003 	cmp	r4, #3
81607050:	1afffff8 	bne	81607038 <console_init_r+0x4c>
81607054:	e5993004 	ldr	r3, [r9, #4]
81607058:	e3a00000 	mov	r0, #0
8160705c:	e3833002 	orr	r3, r3, #2
81607060:	e5893004 	str	r3, [r9, #4]
81607064:	e8bd8070 	pop	{r4, r5, r6, pc}
81607068:	e5131044 	ldr	r1, [r3, #-68]	; 0xffffffbc
8160706c:	e3550000 	cmp	r5, #0
81607070:	e2432044 	sub	r2, r3, #68	; 0x44
81607074:	02013001 	andeq	r3, r1, #1
81607078:	13a03000 	movne	r3, #0
8160707c:	e3530000 	cmp	r3, #0
81607080:	e16f3f14 	clz	r3, r4
81607084:	e1a032a3 	lsr	r3, r3, #5
81607088:	11a05002 	movne	r5, r2
8160708c:	e01330a1 	ands	r3, r3, r1, lsr #1
81607090:	e1a0300c 	mov	r3, ip
81607094:	11a04002 	movne	r4, r2
81607098:	e3550000 	cmp	r5, #0
8160709c:	13540000 	cmpne	r4, #0
816070a0:	0affffd6 	beq	81607000 <console_init_r+0x14>
816070a4:	e1a01004 	mov	r1, r4
816070a8:	e3a00001 	mov	r0, #1
816070ac:	ebfffee5 	bl	81606c48 <console_setfile>
816070b0:	e1a01004 	mov	r1, r4
816070b4:	e3a00002 	mov	r0, #2
816070b8:	ebfffee2 	bl	81606c48 <console_setfile>
816070bc:	eaffffd4 	b	81607014 <console_init_r+0x28>
816070c0:	81619b48 	.word	0x81619b48
816070c4:	816190b0 	.word	0x816190b0

816070c8 <sbrk>:
816070c8:	e59f3058 	ldr	r3, [pc, #88]	; 81607128 <sbrk+0x60>
816070cc:	e3500000 	cmp	r0, #0
816070d0:	e92d4070 	push	{r4, r5, r6, lr}
816070d4:	e5934000 	ldr	r4, [r3]
816070d8:	e1a05003 	mov	r5, r3
816070dc:	e0806004 	add	r6, r0, r4
816070e0:	aa000003 	bge	816070f4 <sbrk+0x2c>
816070e4:	e2602000 	rsb	r2, r0, #0
816070e8:	e3a01000 	mov	r1, #0
816070ec:	e1a00006 	mov	r0, r6
816070f0:	ebffe572 	bl	816006c0 <memset>
816070f4:	e59f3030 	ldr	r3, [pc, #48]	; 8160712c <sbrk+0x64>
816070f8:	e5933000 	ldr	r3, [r3]
816070fc:	e1530006 	cmp	r3, r6
81607100:	8a000006 	bhi	81607120 <sbrk+0x58>
81607104:	e59f3024 	ldr	r3, [pc, #36]	; 81607130 <sbrk+0x68>
81607108:	e5933000 	ldr	r3, [r3]
8160710c:	e1530006 	cmp	r3, r6
81607110:	3a000002 	bcc	81607120 <sbrk+0x58>
81607114:	e5856000 	str	r6, [r5]
81607118:	e1a00004 	mov	r0, r4
8160711c:	e8bd8070 	pop	{r4, r5, r6, pc}
81607120:	e3e00000 	mvn	r0, #0
81607124:	e8bd8070 	pop	{r4, r5, r6, pc}
81607128:	81619aec 	.word	0x81619aec
8160712c:	81619af4 	.word	0x81619af4
81607130:	81619af0 	.word	0x81619af0

81607134 <mem_malloc_init>:
81607134:	e1a02001 	mov	r2, r1
81607138:	e59f101c 	ldr	r1, [pc, #28]	; 8160715c <mem_malloc_init+0x28>
8160713c:	e080c002 	add	ip, r0, r2
81607140:	e5810000 	str	r0, [r1]
81607144:	e59f1014 	ldr	r1, [pc, #20]	; 81607160 <mem_malloc_init+0x2c>
81607148:	e581c000 	str	ip, [r1]
8160714c:	e59f1010 	ldr	r1, [pc, #16]	; 81607164 <mem_malloc_init+0x30>
81607150:	e5810000 	str	r0, [r1]
81607154:	e3a01000 	mov	r1, #0
81607158:	eaffe558 	b	816006c0 <memset>
8160715c:	81619af4 	.word	0x81619af4
81607160:	81619af0 	.word	0x81619af0
81607164:	81619aec 	.word	0x81619aec

81607168 <malloc_trim>:
81607168:	e92d4070 	push	{r4, r5, r6, lr}
8160716c:	e2600efe 	rsb	r0, r0, #4064	; 0xfe0
81607170:	e59f60b4 	ldr	r6, [pc, #180]	; 8160722c <malloc_trim+0xc4>
81607174:	e280000f 	add	r0, r0, #15
81607178:	e5963008 	ldr	r3, [r6, #8]
8160717c:	e5935004 	ldr	r5, [r3, #4]
81607180:	e3c55003 	bic	r5, r5, #3
81607184:	e0804005 	add	r4, r0, r5
81607188:	e3c44eff 	bic	r4, r4, #4080	; 0xff0
8160718c:	e3c4400f 	bic	r4, r4, #15
81607190:	e2444a01 	sub	r4, r4, #4096	; 0x1000
81607194:	e3540a01 	cmp	r4, #4096	; 0x1000
81607198:	aa000001 	bge	816071a4 <malloc_trim+0x3c>
8160719c:	e3a00000 	mov	r0, #0
816071a0:	e8bd8070 	pop	{r4, r5, r6, pc}
816071a4:	e3a00000 	mov	r0, #0
816071a8:	ebffffc6 	bl	816070c8 <sbrk>
816071ac:	e5963008 	ldr	r3, [r6, #8]
816071b0:	e0833005 	add	r3, r3, r5
816071b4:	e1500003 	cmp	r0, r3
816071b8:	1afffff7 	bne	8160719c <malloc_trim+0x34>
816071bc:	e2640000 	rsb	r0, r4, #0
816071c0:	ebffffc0 	bl	816070c8 <sbrk>
816071c4:	e3700001 	cmn	r0, #1
816071c8:	1a00000d 	bne	81607204 <malloc_trim+0x9c>
816071cc:	e3a00000 	mov	r0, #0
816071d0:	ebffffbc 	bl	816070c8 <sbrk>
816071d4:	e5962008 	ldr	r2, [r6, #8]
816071d8:	e0403002 	sub	r3, r0, r2
816071dc:	e353000f 	cmp	r3, #15
816071e0:	daffffed 	ble	8160719c <malloc_trim+0x34>
816071e4:	e59f1044 	ldr	r1, [pc, #68]	; 81607230 <malloc_trim+0xc8>
816071e8:	e3833001 	orr	r3, r3, #1
816071ec:	e5823004 	str	r3, [r2, #4]
816071f0:	e5911000 	ldr	r1, [r1]
816071f4:	e0400001 	sub	r0, r0, r1
816071f8:	e59f1034 	ldr	r1, [pc, #52]	; 81607234 <malloc_trim+0xcc>
816071fc:	e5810000 	str	r0, [r1]
81607200:	eaffffe5 	b	8160719c <malloc_trim+0x34>
81607204:	e5963008 	ldr	r3, [r6, #8]
81607208:	e0455004 	sub	r5, r5, r4
8160720c:	e59f2020 	ldr	r2, [pc, #32]	; 81607234 <malloc_trim+0xcc>
81607210:	e3855001 	orr	r5, r5, #1
81607214:	e3a00001 	mov	r0, #1
81607218:	e5835004 	str	r5, [r3, #4]
8160721c:	e5923000 	ldr	r3, [r2]
81607220:	e0434004 	sub	r4, r3, r4
81607224:	e5824000 	str	r4, [r2]
81607228:	e8bd8070 	pop	{r4, r5, r6, pc}
8160722c:	81618c9c 	.word	0x81618c9c
81607230:	816190a4 	.word	0x816190a4
81607234:	81619abc 	.word	0x81619abc

81607238 <free>:
81607238:	e3500000 	cmp	r0, #0
8160723c:	012fff1e 	bxeq	lr
81607240:	e92d4030 	push	{r4, r5, lr}
81607244:	e2403008 	sub	r3, r0, #8
81607248:	e5102004 	ldr	r2, [r0, #-4]
8160724c:	e59fc200 	ldr	ip, [pc, #512]	; 81607454 <free+0x21c>
81607250:	e3c21001 	bic	r1, r2, #1
81607254:	e2022001 	and	r2, r2, #1
81607258:	e0834001 	add	r4, r3, r1
8160725c:	e59ce008 	ldr	lr, [ip, #8]
81607260:	e5945004 	ldr	r5, [r4, #4]
81607264:	e15e0004 	cmp	lr, r4
81607268:	e3c55003 	bic	r5, r5, #3
8160726c:	1a000014 	bne	816072c4 <free+0x8c>
81607270:	e3520000 	cmp	r2, #0
81607274:	e0811005 	add	r1, r1, r5
81607278:	1a000006 	bne	81607298 <free+0x60>
8160727c:	e5102008 	ldr	r2, [r0, #-8]
81607280:	e0433002 	sub	r3, r3, r2
81607284:	e0811002 	add	r1, r1, r2
81607288:	e5930008 	ldr	r0, [r3, #8]
8160728c:	e593200c 	ldr	r2, [r3, #12]
81607290:	e580200c 	str	r2, [r0, #12]
81607294:	e5820008 	str	r0, [r2, #8]
81607298:	e3812001 	orr	r2, r1, #1
8160729c:	e5832004 	str	r2, [r3, #4]
816072a0:	e58c3008 	str	r3, [ip, #8]
816072a4:	e59f31ac 	ldr	r3, [pc, #428]	; 81607458 <free+0x220>
816072a8:	e5933000 	ldr	r3, [r3]
816072ac:	e1530001 	cmp	r3, r1
816072b0:	88bd8030 	pophi	{r4, r5, pc}
816072b4:	e59f31a0 	ldr	r3, [pc, #416]	; 8160745c <free+0x224>
816072b8:	e8bd4030 	pop	{r4, r5, lr}
816072bc:	e5930000 	ldr	r0, [r3]
816072c0:	eaffffa8 	b	81607168 <malloc_trim>
816072c4:	e3520000 	cmp	r2, #0
816072c8:	e5845004 	str	r5, [r4, #4]
816072cc:	13a02000 	movne	r2, #0
816072d0:	1a000009 	bne	816072fc <free+0xc4>
816072d4:	e5100008 	ldr	r0, [r0, #-8]
816072d8:	e28ce008 	add	lr, ip, #8
816072dc:	e0433000 	sub	r3, r3, r0
816072e0:	e0811000 	add	r1, r1, r0
816072e4:	e5930008 	ldr	r0, [r3, #8]
816072e8:	e150000e 	cmp	r0, lr
816072ec:	1593e00c 	ldrne	lr, [r3, #12]
816072f0:	03a02001 	moveq	r2, #1
816072f4:	1580e00c 	strne	lr, [r0, #12]
816072f8:	158e0008 	strne	r0, [lr, #8]
816072fc:	e0840005 	add	r0, r4, r5
81607300:	e5900004 	ldr	r0, [r0, #4]
81607304:	e3100001 	tst	r0, #1
81607308:	1a00000b 	bne	8160733c <free+0x104>
8160730c:	e3520000 	cmp	r2, #0
81607310:	e0811005 	add	r1, r1, r5
81607314:	e5940008 	ldr	r0, [r4, #8]
81607318:	1a00001d 	bne	81607394 <free+0x15c>
8160731c:	e59fe13c 	ldr	lr, [pc, #316]	; 81607460 <free+0x228>
81607320:	e150000e 	cmp	r0, lr
81607324:	1a00001a 	bne	81607394 <free+0x15c>
81607328:	e3a02001 	mov	r2, #1
8160732c:	e58c3014 	str	r3, [ip, #20]
81607330:	e58c3010 	str	r3, [ip, #16]
81607334:	e583000c 	str	r0, [r3, #12]
81607338:	e5830008 	str	r0, [r3, #8]
8160733c:	e3520000 	cmp	r2, #0
81607340:	e3810001 	orr	r0, r1, #1
81607344:	e5830004 	str	r0, [r3, #4]
81607348:	e7831001 	str	r1, [r3, r1]
8160734c:	18bd8030 	popne	{r4, r5, pc}
81607350:	e3510c02 	cmp	r1, #512	; 0x200
81607354:	2a000012 	bcs	816073a4 <free+0x16c>
81607358:	e1a011a1 	lsr	r1, r1, #3
8160735c:	e59c2004 	ldr	r2, [ip, #4]
81607360:	e3a0e001 	mov	lr, #1
81607364:	e1a00141 	asr	r0, r1, #2
81607368:	e081100e 	add	r1, r1, lr
8160736c:	e182201e 	orr	r2, r2, lr, lsl r0
81607370:	e58c2004 	str	r2, [ip, #4]
81607374:	e79c0181 	ldr	r0, [ip, r1, lsl #3]
81607378:	e08c2181 	add	r2, ip, r1, lsl #3
8160737c:	e2422008 	sub	r2, r2, #8
81607380:	e583200c 	str	r2, [r3, #12]
81607384:	e5830008 	str	r0, [r3, #8]
81607388:	e78c3181 	str	r3, [ip, r1, lsl #3]
8160738c:	e580300c 	str	r3, [r0, #12]
81607390:	e8bd8030 	pop	{r4, r5, pc}
81607394:	e594e00c 	ldr	lr, [r4, #12]
81607398:	e580e00c 	str	lr, [r0, #12]
8160739c:	e58e0008 	str	r0, [lr, #8]
816073a0:	eaffffe5 	b	8160733c <free+0x104>
816073a4:	e1a024a1 	lsr	r2, r1, #9
816073a8:	e3520004 	cmp	r2, #4
816073ac:	91a02321 	lsrls	r2, r1, #6
816073b0:	92822038 	addls	r2, r2, #56	; 0x38
816073b4:	9a00000f 	bls	816073f8 <free+0x1c0>
816073b8:	e3520014 	cmp	r2, #20
816073bc:	9282205b 	addls	r2, r2, #91	; 0x5b
816073c0:	9a00000c 	bls	816073f8 <free+0x1c0>
816073c4:	e3520054 	cmp	r2, #84	; 0x54
816073c8:	91a02621 	lsrls	r2, r1, #12
816073cc:	9282206e 	addls	r2, r2, #110	; 0x6e
816073d0:	9a000008 	bls	816073f8 <free+0x1c0>
816073d4:	e3520f55 	cmp	r2, #340	; 0x154
816073d8:	91a027a1 	lsrls	r2, r1, #15
816073dc:	92822077 	addls	r2, r2, #119	; 0x77
816073e0:	9a000004 	bls	816073f8 <free+0x1c0>
816073e4:	e3000554 	movw	r0, #1364	; 0x554
816073e8:	e1520000 	cmp	r2, r0
816073ec:	91a02921 	lsrls	r2, r1, #18
816073f0:	9282207c 	addls	r2, r2, #124	; 0x7c
816073f4:	83a0207e 	movhi	r2, #126	; 0x7e
816073f8:	e2820001 	add	r0, r2, #1
816073fc:	e08ce182 	add	lr, ip, r2, lsl #3
81607400:	e79c0180 	ldr	r0, [ip, r0, lsl #3]
81607404:	e15e0000 	cmp	lr, r0
81607408:	1a00000b 	bne	8160743c <free+0x204>
8160740c:	e59c1004 	ldr	r1, [ip, #4]
81607410:	e1a02142 	asr	r2, r2, #2
81607414:	e3a04001 	mov	r4, #1
81607418:	e1812214 	orr	r2, r1, r4, lsl r2
8160741c:	e58c2004 	str	r2, [ip, #4]
81607420:	e583e00c 	str	lr, [r3, #12]
81607424:	e5830008 	str	r0, [r3, #8]
81607428:	e58e3008 	str	r3, [lr, #8]
8160742c:	eaffffd6 	b	8160738c <free+0x154>
81607430:	e5900008 	ldr	r0, [r0, #8]
81607434:	e15e0000 	cmp	lr, r0
81607438:	0a000003 	beq	8160744c <free+0x214>
8160743c:	e5902004 	ldr	r2, [r0, #4]
81607440:	e3c22003 	bic	r2, r2, #3
81607444:	e1520001 	cmp	r2, r1
81607448:	8afffff8 	bhi	81607430 <free+0x1f8>
8160744c:	e590e00c 	ldr	lr, [r0, #12]
81607450:	eafffff2 	b	81607420 <free+0x1e8>
81607454:	81618c9c 	.word	0x81618c9c
81607458:	816190a8 	.word	0x816190a8
8160745c:	81619af8 	.word	0x81619af8
81607460:	81618ca4 	.word	0x81618ca4

81607464 <malloc>:
81607464:	e59f35d8 	ldr	r3, [pc, #1496]	; 81607a44 <malloc+0x5e0>
81607468:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160746c:	e5933000 	ldr	r3, [r3]
81607470:	e3530000 	cmp	r3, #0
81607474:	1a000006 	bne	81607494 <malloc+0x30>
81607478:	e59f35c8 	ldr	r3, [pc, #1480]	; 81607a48 <malloc+0x5e4>
8160747c:	e5933000 	ldr	r3, [r3]
81607480:	e3530000 	cmp	r3, #0
81607484:	1a000002 	bne	81607494 <malloc+0x30>
81607488:	e3a00000 	mov	r0, #0
8160748c:	e28dd008 	add	sp, sp, #8
81607490:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81607494:	e3500000 	cmp	r0, #0
81607498:	bafffffa 	blt	81607488 <malloc+0x24>
8160749c:	e280400b 	add	r4, r0, #11
816074a0:	e59f65a4 	ldr	r6, [pc, #1444]	; 81607a4c <malloc+0x5e8>
816074a4:	e3540016 	cmp	r4, #22
816074a8:	da000019 	ble	81607514 <malloc+0xb0>
816074ac:	e3c44007 	bic	r4, r4, #7
816074b0:	e3540f7e 	cmp	r4, #504	; 0x1f8
816074b4:	2a000018 	bcs	8160751c <malloc+0xb8>
816074b8:	e2842008 	add	r2, r4, #8
816074bc:	e1a031a4 	lsr	r3, r4, #3
816074c0:	e0862002 	add	r2, r6, r2
816074c4:	e2421008 	sub	r1, r2, #8
816074c8:	e5920004 	ldr	r0, [r2, #4]
816074cc:	e1500001 	cmp	r0, r1
816074d0:	1a000003 	bne	816074e4 <malloc+0x80>
816074d4:	e592000c 	ldr	r0, [r2, #12]
816074d8:	e1520000 	cmp	r2, r0
816074dc:	02833002 	addeq	r3, r3, #2
816074e0:	0a000031 	beq	816075ac <malloc+0x148>
816074e4:	e5903004 	ldr	r3, [r0, #4]
816074e8:	e590200c 	ldr	r2, [r0, #12]
816074ec:	e5901008 	ldr	r1, [r0, #8]
816074f0:	e3c33003 	bic	r3, r3, #3
816074f4:	e0803003 	add	r3, r0, r3
816074f8:	e581200c 	str	r2, [r1, #12]
816074fc:	e5821008 	str	r1, [r2, #8]
81607500:	e5932004 	ldr	r2, [r3, #4]
81607504:	e3822001 	orr	r2, r2, #1
81607508:	e5832004 	str	r2, [r3, #4]
8160750c:	e2800008 	add	r0, r0, #8
81607510:	eaffffdd 	b	8160748c <malloc+0x28>
81607514:	e3a04010 	mov	r4, #16
81607518:	eaffffe6 	b	816074b8 <malloc+0x54>
8160751c:	e1b034a4 	lsrs	r3, r4, #9
81607520:	03a0303f 	moveq	r3, #63	; 0x3f
81607524:	0a000013 	beq	81607578 <malloc+0x114>
81607528:	e3530004 	cmp	r3, #4
8160752c:	91a03324 	lsrls	r3, r4, #6
81607530:	92833038 	addls	r3, r3, #56	; 0x38
81607534:	9a00000f 	bls	81607578 <malloc+0x114>
81607538:	e3530014 	cmp	r3, #20
8160753c:	9283305b 	addls	r3, r3, #91	; 0x5b
81607540:	9a00000c 	bls	81607578 <malloc+0x114>
81607544:	e3530054 	cmp	r3, #84	; 0x54
81607548:	91a03624 	lsrls	r3, r4, #12
8160754c:	9283306e 	addls	r3, r3, #110	; 0x6e
81607550:	9a000008 	bls	81607578 <malloc+0x114>
81607554:	e3530f55 	cmp	r3, #340	; 0x154
81607558:	91a037a4 	lsrls	r3, r4, #15
8160755c:	92833077 	addls	r3, r3, #119	; 0x77
81607560:	9a000004 	bls	81607578 <malloc+0x114>
81607564:	e3002554 	movw	r2, #1364	; 0x554
81607568:	e1530002 	cmp	r3, r2
8160756c:	91a03924 	lsrls	r3, r4, #18
81607570:	9283307c 	addls	r3, r3, #124	; 0x7c
81607574:	83a0307e 	movhi	r3, #126	; 0x7e
81607578:	e2832001 	add	r2, r3, #1
8160757c:	e0862182 	add	r2, r6, r2, lsl #3
81607580:	e242e008 	sub	lr, r2, #8
81607584:	e5920004 	ldr	r0, [r2, #4]
81607588:	e150000e 	cmp	r0, lr
8160758c:	0a000005 	beq	816075a8 <malloc+0x144>
81607590:	e5902004 	ldr	r2, [r0, #4]
81607594:	e3c22003 	bic	r2, r2, #3
81607598:	e042c004 	sub	ip, r2, r4
8160759c:	e35c000f 	cmp	ip, #15
816075a0:	da000016 	ble	81607600 <malloc+0x19c>
816075a4:	e2433001 	sub	r3, r3, #1
816075a8:	e2833001 	add	r3, r3, #1
816075ac:	e59f249c 	ldr	r2, [pc, #1180]	; 81607a50 <malloc+0x5ec>
816075b0:	e5960010 	ldr	r0, [r6, #16]
816075b4:	e1500002 	cmp	r0, r2
816075b8:	e1a01002 	mov	r1, r2
816075bc:	0a000030 	beq	81607684 <malloc+0x220>
816075c0:	e590e004 	ldr	lr, [r0, #4]
816075c4:	e3cee003 	bic	lr, lr, #3
816075c8:	e04ec004 	sub	ip, lr, r4
816075cc:	e35c000f 	cmp	ip, #15
816075d0:	da000013 	ble	81607624 <malloc+0x1c0>
816075d4:	e0803004 	add	r3, r0, r4
816075d8:	e3844001 	orr	r4, r4, #1
816075dc:	e5804004 	str	r4, [r0, #4]
816075e0:	e5863014 	str	r3, [r6, #20]
816075e4:	e5863010 	str	r3, [r6, #16]
816075e8:	e583200c 	str	r2, [r3, #12]
816075ec:	e5832008 	str	r2, [r3, #8]
816075f0:	e38c2001 	orr	r2, ip, #1
816075f4:	e5832004 	str	r2, [r3, #4]
816075f8:	e780c00e 	str	ip, [r0, lr]
816075fc:	eaffffc2 	b	8160750c <malloc+0xa8>
81607600:	e35c0000 	cmp	ip, #0
81607604:	e590100c 	ldr	r1, [r0, #12]
81607608:	a5903008 	ldrge	r3, [r0, #8]
8160760c:	a583100c 	strge	r1, [r3, #12]
81607610:	a5813008 	strge	r3, [r1, #8]
81607614:	a0803002 	addge	r3, r0, r2
81607618:	aaffffb8 	bge	81607500 <malloc+0x9c>
8160761c:	e1a00001 	mov	r0, r1
81607620:	eaffffd8 	b	81607588 <malloc+0x124>
81607624:	e35c0000 	cmp	ip, #0
81607628:	e5862014 	str	r2, [r6, #20]
8160762c:	e5862010 	str	r2, [r6, #16]
81607630:	a080e00e 	addge	lr, r0, lr
81607634:	a59e3004 	ldrge	r3, [lr, #4]
81607638:	a3833001 	orrge	r3, r3, #1
8160763c:	a58e3004 	strge	r3, [lr, #4]
81607640:	aaffffb1 	bge	8160750c <malloc+0xa8>
81607644:	e35e0c02 	cmp	lr, #512	; 0x200
81607648:	e596c004 	ldr	ip, [r6, #4]
8160764c:	2a00006e 	bcs	8160780c <malloc+0x3a8>
81607650:	e1a0e1ae 	lsr	lr, lr, #3
81607654:	e3a02001 	mov	r2, #1
81607658:	e1a0514e 	asr	r5, lr, #2
8160765c:	e08ee002 	add	lr, lr, r2
81607660:	e18cc512 	orr	ip, ip, r2, lsl r5
81607664:	e586c004 	str	ip, [r6, #4]
81607668:	e796c18e 	ldr	ip, [r6, lr, lsl #3]
8160766c:	e086218e 	add	r2, r6, lr, lsl #3
81607670:	e2422008 	sub	r2, r2, #8
81607674:	e580200c 	str	r2, [r0, #12]
81607678:	e580c008 	str	ip, [r0, #8]
8160767c:	e786018e 	str	r0, [r6, lr, lsl #3]
81607680:	e58c000c 	str	r0, [ip, #12]
81607684:	e3a00001 	mov	r0, #1
81607688:	e1a02143 	asr	r2, r3, #2
8160768c:	e1a02210 	lsl	r2, r0, r2
81607690:	e5960004 	ldr	r0, [r6, #4]
81607694:	e1500002 	cmp	r0, r2
81607698:	3a00001e 	bcc	81607718 <malloc+0x2b4>
8160769c:	e1100002 	tst	r0, r2
816076a0:	1a000004 	bne	816076b8 <malloc+0x254>
816076a4:	e3c33003 	bic	r3, r3, #3
816076a8:	e1a02082 	lsl	r2, r2, #1
816076ac:	e2833004 	add	r3, r3, #4
816076b0:	e1100002 	tst	r0, r2
816076b4:	0afffffb 	beq	816076a8 <malloc+0x244>
816076b8:	e086e183 	add	lr, r6, r3, lsl #3
816076bc:	e1a05003 	mov	r5, r3
816076c0:	e1a0700e 	mov	r7, lr
816076c4:	e597000c 	ldr	r0, [r7, #12]
816076c8:	e1500007 	cmp	r0, r7
816076cc:	1a00007a 	bne	816078bc <malloc+0x458>
816076d0:	e2855001 	add	r5, r5, #1
816076d4:	e2877008 	add	r7, r7, #8
816076d8:	e3150003 	tst	r5, #3
816076dc:	1afffff8 	bne	816076c4 <malloc+0x260>
816076e0:	e3130003 	tst	r3, #3
816076e4:	1a000095 	bne	81607940 <malloc+0x4dc>
816076e8:	e5963004 	ldr	r3, [r6, #4]
816076ec:	e1c33002 	bic	r3, r3, r2
816076f0:	e5863004 	str	r3, [r6, #4]
816076f4:	e1b02082 	lsls	r2, r2, #1
816076f8:	e5960004 	ldr	r0, [r6, #4]
816076fc:	13a03001 	movne	r3, #1
81607700:	03a03000 	moveq	r3, #0
81607704:	e1500002 	cmp	r0, r2
81607708:	33a03000 	movcc	r3, #0
8160770c:	22033001 	andcs	r3, r3, #1
81607710:	e3530000 	cmp	r3, #0
81607714:	1a000094 	bne	8160796c <malloc+0x508>
81607718:	e5963008 	ldr	r3, [r6, #8]
8160771c:	e5938004 	ldr	r8, [r3, #4]
81607720:	e3c88003 	bic	r8, r8, #3
81607724:	e0482004 	sub	r2, r8, r4
81607728:	e352000f 	cmp	r2, #15
8160772c:	ca00002e 	bgt	816077ec <malloc+0x388>
81607730:	e59f231c 	ldr	r2, [pc, #796]	; 81607a54 <malloc+0x5f0>
81607734:	e59f731c 	ldr	r7, [pc, #796]	; 81607a58 <malloc+0x5f4>
81607738:	e58d3004 	str	r3, [sp, #4]
8160773c:	e5925000 	ldr	r5, [r2]
81607740:	e5972000 	ldr	r2, [r7]
81607744:	e0845005 	add	r5, r4, r5
81607748:	e3720001 	cmn	r2, #1
8160774c:	12855a01 	addne	r5, r5, #4096	; 0x1000
81607750:	02855010 	addeq	r5, r5, #16
81607754:	1285500f 	addne	r5, r5, #15
81607758:	13c55eff 	bicne	r5, r5, #4080	; 0xff0
8160775c:	13c5500f 	bicne	r5, r5, #15
81607760:	e1a00005 	mov	r0, r5
81607764:	ebfffe57 	bl	816070c8 <sbrk>
81607768:	e3700001 	cmn	r0, #1
8160776c:	e1a0a000 	mov	sl, r0
81607770:	0a000017 	beq	816077d4 <malloc+0x370>
81607774:	e59d3004 	ldr	r3, [sp, #4]
81607778:	e0832008 	add	r2, r3, r8
8160777c:	e1530006 	cmp	r3, r6
81607780:	11520000 	cmpne	r2, r0
81607784:	8a000012 	bhi	816077d4 <malloc+0x370>
81607788:	e59fb2cc 	ldr	fp, [pc, #716]	; 81607a5c <malloc+0x5f8>
8160778c:	e1520000 	cmp	r2, r0
81607790:	e59b1000 	ldr	r1, [fp]
81607794:	e0851001 	add	r1, r5, r1
81607798:	e58b1000 	str	r1, [fp]
8160779c:	1a000074 	bne	81607974 <malloc+0x510>
816077a0:	e5963008 	ldr	r3, [r6, #8]
816077a4:	e0885005 	add	r5, r8, r5
816077a8:	e3855001 	orr	r5, r5, #1
816077ac:	e5835004 	str	r5, [r3, #4]
816077b0:	e59f22a8 	ldr	r2, [pc, #680]	; 81607a60 <malloc+0x5fc>
816077b4:	e59b3000 	ldr	r3, [fp]
816077b8:	e5921000 	ldr	r1, [r2]
816077bc:	e1530001 	cmp	r3, r1
816077c0:	85823000 	strhi	r3, [r2]
816077c4:	e59f2298 	ldr	r2, [pc, #664]	; 81607a64 <malloc+0x600>
816077c8:	e5921000 	ldr	r1, [r2]
816077cc:	e1530001 	cmp	r3, r1
816077d0:	85823000 	strhi	r3, [r2]
816077d4:	e5963008 	ldr	r3, [r6, #8]
816077d8:	e5932004 	ldr	r2, [r3, #4]
816077dc:	e3c22003 	bic	r2, r2, #3
816077e0:	e0422004 	sub	r2, r2, r4
816077e4:	e352000f 	cmp	r2, #15
816077e8:	daffff26 	ble	81607488 <malloc+0x24>
816077ec:	e5960008 	ldr	r0, [r6, #8]
816077f0:	e3843001 	orr	r3, r4, #1
816077f4:	e3822001 	orr	r2, r2, #1
816077f8:	e0804004 	add	r4, r0, r4
816077fc:	e5803004 	str	r3, [r0, #4]
81607800:	e5864008 	str	r4, [r6, #8]
81607804:	e5842004 	str	r2, [r4, #4]
81607808:	eaffff3f 	b	8160750c <malloc+0xa8>
8160780c:	e1a024ae 	lsr	r2, lr, #9
81607810:	e3520004 	cmp	r2, #4
81607814:	91a0232e 	lsrls	r2, lr, #6
81607818:	92822038 	addls	r2, r2, #56	; 0x38
8160781c:	9a00000f 	bls	81607860 <malloc+0x3fc>
81607820:	e3520014 	cmp	r2, #20
81607824:	9282205b 	addls	r2, r2, #91	; 0x5b
81607828:	9a00000c 	bls	81607860 <malloc+0x3fc>
8160782c:	e3520054 	cmp	r2, #84	; 0x54
81607830:	91a0262e 	lsrls	r2, lr, #12
81607834:	9282206e 	addls	r2, r2, #110	; 0x6e
81607838:	9a000008 	bls	81607860 <malloc+0x3fc>
8160783c:	e3520f55 	cmp	r2, #340	; 0x154
81607840:	91a027ae 	lsrls	r2, lr, #15
81607844:	92822077 	addls	r2, r2, #119	; 0x77
81607848:	9a000004 	bls	81607860 <malloc+0x3fc>
8160784c:	e3005554 	movw	r5, #1364	; 0x554
81607850:	e1520005 	cmp	r2, r5
81607854:	91a0292e 	lsrls	r2, lr, #18
81607858:	9282207c 	addls	r2, r2, #124	; 0x7c
8160785c:	83a0207e 	movhi	r2, #126	; 0x7e
81607860:	e2825001 	add	r5, r2, #1
81607864:	e0867182 	add	r7, r6, r2, lsl #3
81607868:	e7965185 	ldr	r5, [r6, r5, lsl #3]
8160786c:	e1570005 	cmp	r7, r5
81607870:	1a00000b 	bne	816078a4 <malloc+0x440>
81607874:	e1a02142 	asr	r2, r2, #2
81607878:	e3a0e001 	mov	lr, #1
8160787c:	e18c221e 	orr	r2, ip, lr, lsl r2
81607880:	e5862004 	str	r2, [r6, #4]
81607884:	e580700c 	str	r7, [r0, #12]
81607888:	e5805008 	str	r5, [r0, #8]
8160788c:	e5870008 	str	r0, [r7, #8]
81607890:	e585000c 	str	r0, [r5, #12]
81607894:	eaffff7a 	b	81607684 <malloc+0x220>
81607898:	e5955008 	ldr	r5, [r5, #8]
8160789c:	e1570005 	cmp	r7, r5
816078a0:	0a000003 	beq	816078b4 <malloc+0x450>
816078a4:	e5952004 	ldr	r2, [r5, #4]
816078a8:	e3c22003 	bic	r2, r2, #3
816078ac:	e152000e 	cmp	r2, lr
816078b0:	8afffff8 	bhi	81607898 <malloc+0x434>
816078b4:	e595700c 	ldr	r7, [r5, #12]
816078b8:	eafffff1 	b	81607884 <malloc+0x420>
816078bc:	e590c004 	ldr	ip, [r0, #4]
816078c0:	e3ccc003 	bic	ip, ip, #3
816078c4:	e04c8004 	sub	r8, ip, r4
816078c8:	e358000f 	cmp	r8, #15
816078cc:	da00000e 	ble	8160790c <malloc+0x4a8>
816078d0:	e590200c 	ldr	r2, [r0, #12]
816078d4:	e0803004 	add	r3, r0, r4
816078d8:	e590e008 	ldr	lr, [r0, #8]
816078dc:	e3844001 	orr	r4, r4, #1
816078e0:	e5804004 	str	r4, [r0, #4]
816078e4:	e58e200c 	str	r2, [lr, #12]
816078e8:	e582e008 	str	lr, [r2, #8]
816078ec:	e3882001 	orr	r2, r8, #1
816078f0:	e5863014 	str	r3, [r6, #20]
816078f4:	e5863010 	str	r3, [r6, #16]
816078f8:	e583100c 	str	r1, [r3, #12]
816078fc:	e5831008 	str	r1, [r3, #8]
81607900:	e5832004 	str	r2, [r3, #4]
81607904:	e780800c 	str	r8, [r0, ip]
81607908:	eafffeff 	b	8160750c <malloc+0xa8>
8160790c:	e3580000 	cmp	r8, #0
81607910:	ba000008 	blt	81607938 <malloc+0x4d4>
81607914:	e080c00c 	add	ip, r0, ip
81607918:	e59c3004 	ldr	r3, [ip, #4]
8160791c:	e3833001 	orr	r3, r3, #1
81607920:	e58c3004 	str	r3, [ip, #4]
81607924:	e590300c 	ldr	r3, [r0, #12]
81607928:	e5b02008 	ldr	r2, [r0, #8]!
8160792c:	e582300c 	str	r3, [r2, #12]
81607930:	e5832008 	str	r2, [r3, #8]
81607934:	eafffed4 	b	8160748c <malloc+0x28>
81607938:	e590000c 	ldr	r0, [r0, #12]
8160793c:	eaffff61 	b	816076c8 <malloc+0x264>
81607940:	e24e0008 	sub	r0, lr, #8
81607944:	e59ee000 	ldr	lr, [lr]
81607948:	e2433001 	sub	r3, r3, #1
8160794c:	e15e0000 	cmp	lr, r0
81607950:	0affff62 	beq	816076e0 <malloc+0x27c>
81607954:	eaffff66 	b	816076f4 <malloc+0x290>
81607958:	e2833004 	add	r3, r3, #4
8160795c:	e1a02082 	lsl	r2, r2, #1
81607960:	e1100002 	tst	r0, r2
81607964:	0afffffb 	beq	81607958 <malloc+0x4f4>
81607968:	eaffff52 	b	816076b8 <malloc+0x254>
8160796c:	e1a03005 	mov	r3, r5
81607970:	eafffffa 	b	81607960 <malloc+0x4fc>
81607974:	e5970000 	ldr	r0, [r7]
81607978:	e58d3004 	str	r3, [sp, #4]
8160797c:	e3700001 	cmn	r0, #1
81607980:	0587a000 	streq	sl, [r7]
81607984:	104a2002 	subne	r2, sl, r2
81607988:	10822001 	addne	r2, r2, r1
8160798c:	158b2000 	strne	r2, [fp]
81607990:	e21a2007 	ands	r2, sl, #7
81607994:	12622008 	rsbne	r2, r2, #8
81607998:	108aa002 	addne	sl, sl, r2
8160799c:	e08a7005 	add	r7, sl, r5
816079a0:	e0422007 	sub	r2, r2, r7
816079a4:	e2877eff 	add	r7, r7, #4080	; 0xff0
816079a8:	e287700f 	add	r7, r7, #15
816079ac:	e3c77eff 	bic	r7, r7, #4080	; 0xff0
816079b0:	e3c7700f 	bic	r7, r7, #15
816079b4:	e0877002 	add	r7, r7, r2
816079b8:	e1a00007 	mov	r0, r7
816079bc:	ebfffdc1 	bl	816070c8 <sbrk>
816079c0:	e3700001 	cmn	r0, #1
816079c4:	0affff82 	beq	816077d4 <malloc+0x370>
816079c8:	e59d3004 	ldr	r3, [sp, #4]
816079cc:	e040000a 	sub	r0, r0, sl
816079d0:	e59b2000 	ldr	r2, [fp]
816079d4:	e1530006 	cmp	r3, r6
816079d8:	e586a008 	str	sl, [r6, #8]
816079dc:	e0822007 	add	r2, r2, r7
816079e0:	e0807007 	add	r7, r0, r7
816079e4:	e3877001 	orr	r7, r7, #1
816079e8:	e58b2000 	str	r2, [fp]
816079ec:	e58a7004 	str	r7, [sl, #4]
816079f0:	0affff6e 	beq	816077b0 <malloc+0x34c>
816079f4:	e358000f 	cmp	r8, #15
816079f8:	95963008 	ldrls	r3, [r6, #8]
816079fc:	93a02001 	movls	r2, #1
81607a00:	95832004 	strls	r2, [r3, #4]
81607a04:	9affff72 	bls	816077d4 <malloc+0x370>
81607a08:	e5932004 	ldr	r2, [r3, #4]
81607a0c:	e248800c 	sub	r8, r8, #12
81607a10:	e3c88007 	bic	r8, r8, #7
81607a14:	e3a01005 	mov	r1, #5
81607a18:	e2022001 	and	r2, r2, #1
81607a1c:	e358000f 	cmp	r8, #15
81607a20:	e1822008 	orr	r2, r2, r8
81607a24:	e5832004 	str	r2, [r3, #4]
81607a28:	e0832008 	add	r2, r3, r8
81607a2c:	e5821004 	str	r1, [r2, #4]
81607a30:	e5821008 	str	r1, [r2, #8]
81607a34:	9affff5d 	bls	816077b0 <malloc+0x34c>
81607a38:	e2830008 	add	r0, r3, #8
81607a3c:	ebfffdfd 	bl	81607238 <free>
81607a40:	eaffff5a 	b	816077b0 <malloc+0x34c>
81607a44:	81619af4 	.word	0x81619af4
81607a48:	81619af0 	.word	0x81619af0
81607a4c:	81618c9c 	.word	0x81618c9c
81607a50:	81618ca4 	.word	0x81618ca4
81607a54:	81619af8 	.word	0x81619af8
81607a58:	816190a4 	.word	0x816190a4
81607a5c:	81619abc 	.word	0x81619abc
81607a60:	81619ae4 	.word	0x81619ae4
81607a64:	81619ae8 	.word	0x81619ae8

81607a68 <calloc>:
81607a68:	e59f30c0 	ldr	r3, [pc, #192]	; 81607b30 <calloc+0xc8>
81607a6c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81607a70:	e1a07000 	mov	r7, r0
81607a74:	e5938008 	ldr	r8, [r3, #8]
81607a78:	e0000091 	mul	r0, r1, r0
81607a7c:	e5982004 	ldr	r2, [r8, #4]
81607a80:	e3c25003 	bic	r5, r2, #3
81607a84:	ebfffe76 	bl	81607464 <malloc>
81607a88:	e16f1f10 	clz	r1, r0
81607a8c:	e1a012a1 	lsr	r1, r1, #5
81607a90:	e1911fa7 	orrs	r1, r1, r7, lsr #31
81607a94:	13a04000 	movne	r4, #0
81607a98:	1a000020 	bne	81607b20 <calloc+0xb8>
81607a9c:	e2402008 	sub	r2, r0, #8
81607aa0:	e5103004 	ldr	r3, [r0, #-4]
81607aa4:	e0486002 	sub	r6, r8, r2
81607aa8:	e1a04000 	mov	r4, r0
81607aac:	e3c33003 	bic	r3, r3, #3
81607ab0:	e16f6f16 	clz	r6, r6
81607ab4:	e1550003 	cmp	r5, r3
81607ab8:	e1a062a6 	lsr	r6, r6, #5
81607abc:	23a06000 	movcs	r6, #0
81607ac0:	e3560000 	cmp	r6, #0
81607ac4:	01a05003 	moveq	r5, r3
81607ac8:	e2452004 	sub	r2, r5, #4
81607acc:	e3520024 	cmp	r2, #36	; 0x24
81607ad0:	8a000014 	bhi	81607b28 <calloc+0xc0>
81607ad4:	e3520013 	cmp	r2, #19
81607ad8:	91a03000 	movls	r3, r0
81607adc:	9a00000b 	bls	81607b10 <calloc+0xa8>
81607ae0:	e352001b 	cmp	r2, #27
81607ae4:	e5801000 	str	r1, [r0]
81607ae8:	e5801004 	str	r1, [r0, #4]
81607aec:	92803008 	addls	r3, r0, #8
81607af0:	9a000006 	bls	81607b10 <calloc+0xa8>
81607af4:	e3520024 	cmp	r2, #36	; 0x24
81607af8:	e5801008 	str	r1, [r0, #8]
81607afc:	12803010 	addne	r3, r0, #16
81607b00:	02803018 	addeq	r3, r0, #24
81607b04:	e580100c 	str	r1, [r0, #12]
81607b08:	05801010 	streq	r1, [r0, #16]
81607b0c:	05801014 	streq	r1, [r0, #20]
81607b10:	e3a02000 	mov	r2, #0
81607b14:	e5832000 	str	r2, [r3]
81607b18:	e5832004 	str	r2, [r3, #4]
81607b1c:	e5832008 	str	r2, [r3, #8]
81607b20:	e1a00004 	mov	r0, r4
81607b24:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81607b28:	ebffe2e4 	bl	816006c0 <memset>
81607b2c:	eafffffb 	b	81607b20 <calloc+0xb8>
81607b30:	81618c9c 	.word	0x81618c9c

81607b34 <initf_malloc>:
81607b34:	e3a00000 	mov	r0, #0
81607b38:	e12fff1e 	bx	lr

81607b3c <on_loadaddr>:
81607b3c:	e3520000 	cmp	r2, #0
81607b40:	0a000001 	beq	81607b4c <on_loadaddr+0x10>
81607b44:	e3520002 	cmp	r2, #2
81607b48:	1a000008 	bne	81607b70 <on_loadaddr+0x34>
81607b4c:	e92d4010 	push	{r4, lr}
81607b50:	e1a00001 	mov	r0, r1
81607b54:	e3a02010 	mov	r2, #16
81607b58:	e3a01000 	mov	r1, #0
81607b5c:	eb002b0d 	bl	81612798 <simple_strtoul>
81607b60:	e59f3010 	ldr	r3, [pc, #16]	; 81607b78 <on_loadaddr+0x3c>
81607b64:	e5830000 	str	r0, [r3]
81607b68:	e3a00000 	mov	r0, #0
81607b6c:	e8bd8010 	pop	{r4, pc}
81607b70:	e3a00000 	mov	r0, #0
81607b74:	e12fff1e 	bx	lr
81607b78:	816190ac 	.word	0x816190ac

81607b7c <image_check_hcrc>:
81607b7c:	e92d4010 	push	{r4, lr}
81607b80:	e24dd040 	sub	sp, sp, #64	; 0x40
81607b84:	e1a04000 	mov	r4, r0
81607b88:	e1a01000 	mov	r1, r0
81607b8c:	e3a02040 	mov	r2, #64	; 0x40
81607b90:	e1a0000d 	mov	r0, sp
81607b94:	eb002785 	bl	816119b0 <memmove>
81607b98:	e3a00000 	mov	r0, #0
81607b9c:	e3a02040 	mov	r2, #64	; 0x40
81607ba0:	e1a0100d 	mov	r1, sp
81607ba4:	e58d0004 	str	r0, [sp, #4]
81607ba8:	eb0026b5 	bl	81611684 <crc32>
81607bac:	e5943004 	ldr	r3, [r4, #4]
81607bb0:	e6bf3f33 	rev	r3, r3
81607bb4:	e0400003 	sub	r0, r0, r3
81607bb8:	e16f0f10 	clz	r0, r0
81607bbc:	e1a002a0 	lsr	r0, r0, #5
81607bc0:	e28dd040 	add	sp, sp, #64	; 0x40
81607bc4:	e8bd8010 	pop	{r4, pc}

81607bc8 <image_check_dcrc>:
81607bc8:	e590200c 	ldr	r2, [r0, #12]
81607bcc:	e2801040 	add	r1, r0, #64	; 0x40
81607bd0:	e92d4010 	push	{r4, lr}
81607bd4:	e3a03801 	mov	r3, #65536	; 0x10000
81607bd8:	e1a04000 	mov	r4, r0
81607bdc:	e6bf2f32 	rev	r2, r2
81607be0:	e3a00000 	mov	r0, #0
81607be4:	eb0026ab 	bl	81611698 <crc32_wd>
81607be8:	e5943018 	ldr	r3, [r4, #24]
81607bec:	e6bf3f33 	rev	r3, r3
81607bf0:	e0400003 	sub	r0, r0, r3
81607bf4:	e16f0f10 	clz	r0, r0
81607bf8:	e1a002a0 	lsr	r0, r0, #5
81607bfc:	e8bd8010 	pop	{r4, pc}

81607c00 <image_multi_count>:
81607c00:	e2803040 	add	r3, r0, #64	; 0x40
81607c04:	e3a00000 	mov	r0, #0
81607c08:	e4932004 	ldr	r2, [r3], #4
81607c0c:	e3520000 	cmp	r2, #0
81607c10:	012fff1e 	bxeq	lr
81607c14:	e2800001 	add	r0, r0, #1
81607c18:	eafffffa 	b	81607c08 <image_multi_count+0x8>

81607c1c <image_multi_getimg>:
81607c1c:	e92d4070 	push	{r4, r5, r6, lr}
81607c20:	e1a06000 	mov	r6, r0
81607c24:	e1a04002 	mov	r4, r2
81607c28:	e1a05003 	mov	r5, r3
81607c2c:	ebfffff3 	bl	81607c00 <image_multi_count>
81607c30:	e1500001 	cmp	r0, r1
81607c34:	9a000014 	bls	81607c8c <image_multi_getimg+0x70>
81607c38:	e286c040 	add	ip, r6, #64	; 0x40
81607c3c:	e3a0e000 	mov	lr, #0
81607c40:	e79c2101 	ldr	r2, [ip, r1, lsl #2]
81607c44:	e6bf2f32 	rev	r2, r2
81607c48:	e5852000 	str	r2, [r5]
81607c4c:	e1a0500c 	mov	r5, ip
81607c50:	e1a0200e 	mov	r2, lr
81607c54:	e1520001 	cmp	r2, r1
81607c58:	1a000004 	bne	81607c70 <image_multi_getimg+0x54>
81607c5c:	e2800001 	add	r0, r0, #1
81607c60:	e08cc100 	add	ip, ip, r0, lsl #2
81607c64:	e08cc00e 	add	ip, ip, lr
81607c68:	e584c000 	str	ip, [r4]
81607c6c:	e8bd8070 	pop	{r4, r5, r6, pc}
81607c70:	e4953004 	ldr	r3, [r5], #4
81607c74:	e2822001 	add	r2, r2, #1
81607c78:	e6bf3f33 	rev	r3, r3
81607c7c:	e2833003 	add	r3, r3, #3
81607c80:	e3c33003 	bic	r3, r3, #3
81607c84:	e08ee003 	add	lr, lr, r3
81607c88:	eafffff1 	b	81607c54 <image_multi_getimg+0x38>
81607c8c:	e3a02000 	mov	r2, #0
81607c90:	e5852000 	str	r2, [r5]
81607c94:	e5842000 	str	r2, [r4]
81607c98:	e8bd8070 	pop	{r4, r5, r6, pc}

81607c9c <env_get_bootm_low>:
81607c9c:	e92d4010 	push	{r4, lr}
81607ca0:	e59f0020 	ldr	r0, [pc, #32]	; 81607cc8 <env_get_bootm_low+0x2c>
81607ca4:	ebfff23b 	bl	81604598 <env_get>
81607ca8:	e3500000 	cmp	r0, #0
81607cac:	0a000003 	beq	81607cc0 <env_get_bootm_low+0x24>
81607cb0:	e3a02010 	mov	r2, #16
81607cb4:	e3a01000 	mov	r1, #0
81607cb8:	e8bd4010 	pop	{r4, lr}
81607cbc:	ea002ab5 	b	81612798 <simple_strtoul>
81607cc0:	e3a00102 	mov	r0, #-2147483648	; 0x80000000
81607cc4:	e8bd8010 	pop	{r4, pc}
81607cc8:	81615b05 	.word	0x81615b05

81607ccc <env_get_bootm_size>:
81607ccc:	e92d4070 	push	{r4, r5, r6, lr}
81607cd0:	e59f0054 	ldr	r0, [pc, #84]	; 81607d2c <env_get_bootm_size+0x60>
81607cd4:	ebfff22f 	bl	81604598 <env_get>
81607cd8:	e2506000 	subs	r6, r0, #0
81607cdc:	0a000003 	beq	81607cf0 <env_get_bootm_size+0x24>
81607ce0:	e3a02010 	mov	r2, #16
81607ce4:	e3a01000 	mov	r1, #0
81607ce8:	eb002b18 	bl	81612950 <simple_strtoull>
81607cec:	e8bd8070 	pop	{r4, r5, r6, pc}
81607cf0:	e5992000 	ldr	r2, [r9]
81607cf4:	e5993000 	ldr	r3, [r9]
81607cf8:	e59f0030 	ldr	r0, [pc, #48]	; 81607d30 <env_get_bootm_size+0x64>
81607cfc:	e5924048 	ldr	r4, [r2, #72]	; 0x48
81607d00:	e593504c 	ldr	r5, [r3, #76]	; 0x4c
81607d04:	ebfff223 	bl	81604598 <env_get>
81607d08:	e3500000 	cmp	r0, #0
81607d0c:	01a00004 	moveq	r0, r4
81607d10:	0a000002 	beq	81607d20 <env_get_bootm_size+0x54>
81607d14:	e3a02010 	mov	r2, #16
81607d18:	e1a01006 	mov	r1, r6
81607d1c:	eb002b0b 	bl	81612950 <simple_strtoull>
81607d20:	e0844005 	add	r4, r4, r5
81607d24:	e0440000 	sub	r0, r4, r0
81607d28:	e8bd8070 	pop	{r4, r5, r6, pc}
81607d2c:	81615b1d 	.word	0x81615b1d
81607d30:	81615b05 	.word	0x81615b05

81607d34 <env_get_bootm_mapsize>:
81607d34:	e92d4010 	push	{r4, lr}
81607d38:	e59f0020 	ldr	r0, [pc, #32]	; 81607d60 <env_get_bootm_mapsize+0x2c>
81607d3c:	ebfff215 	bl	81604598 <env_get>
81607d40:	e3500000 	cmp	r0, #0
81607d44:	0a000003 	beq	81607d58 <env_get_bootm_mapsize+0x24>
81607d48:	e3a02010 	mov	r2, #16
81607d4c:	e3a01000 	mov	r1, #0
81607d50:	eb002afe 	bl	81612950 <simple_strtoull>
81607d54:	e8bd8010 	pop	{r4, pc}
81607d58:	e8bd4010 	pop	{r4, lr}
81607d5c:	eaffffda 	b	81607ccc <env_get_bootm_size>
81607d60:	81615b0f 	.word	0x81615b0f

81607d64 <memmove_wd>:
81607d64:	e1510000 	cmp	r1, r0
81607d68:	012fff1e 	bxeq	lr
81607d6c:	ea00270f 	b	816119b0 <memmove>

81607d70 <genimg_print_size>:
81607d70:	e92d4010 	push	{r4, lr}
81607d74:	e1a01000 	mov	r1, r0
81607d78:	e1a04000 	mov	r4, r0
81607d7c:	e59f0014 	ldr	r0, [pc, #20]	; 81607d98 <genimg_print_size+0x28>
81607d80:	eb002a3d 	bl	8161267c <printf>
81607d84:	e1a00004 	mov	r0, r4
81607d88:	e59f200c 	ldr	r2, [pc, #12]	; 81607d9c <genimg_print_size+0x2c>
81607d8c:	e3a01000 	mov	r1, #0
81607d90:	e8bd4010 	pop	{r4, lr}
81607d94:	ea002548 	b	816112bc <print_size>
81607d98:	81615eba 	.word	0x81615eba
81607d9c:	8161623a 	.word	0x8161623a

81607da0 <get_table_entry>:
81607da0:	e5903000 	ldr	r3, [r0]
81607da4:	e3530000 	cmp	r3, #0
81607da8:	aa000001 	bge	81607db4 <get_table_entry+0x14>
81607dac:	e3a00000 	mov	r0, #0
81607db0:	e12fff1e 	bx	lr
81607db4:	e1530001 	cmp	r3, r1
81607db8:	012fff1e 	bxeq	lr
81607dbc:	e280000c 	add	r0, r0, #12
81607dc0:	eafffff6 	b	81607da0 <get_table_entry>

81607dc4 <get_table_entry_name>:
81607dc4:	e92d4010 	push	{r4, lr}
81607dc8:	e1a04001 	mov	r4, r1
81607dcc:	e1a01002 	mov	r1, r2
81607dd0:	ebfffff2 	bl	81607da0 <get_table_entry>
81607dd4:	e3500000 	cmp	r0, #0
81607dd8:	15904008 	ldrne	r4, [r0, #8]
81607ddc:	e1a00004 	mov	r0, r4
81607de0:	e8bd8010 	pop	{r4, pc}

81607de4 <genimg_get_os_name>:
81607de4:	e1a02000 	mov	r2, r0
81607de8:	e59f1004 	ldr	r1, [pc, #4]	; 81607df4 <genimg_get_os_name+0x10>
81607dec:	e59f0004 	ldr	r0, [pc, #4]	; 81607df8 <genimg_get_os_name+0x14>
81607df0:	eafffff3 	b	81607dc4 <get_table_entry_name>
81607df4:	81615ea1 	.word	0x81615ea1
81607df8:	816134a8 	.word	0x816134a8

81607dfc <genimg_get_arch_name>:
81607dfc:	e1a02000 	mov	r2, r0
81607e00:	e59f1004 	ldr	r1, [pc, #4]	; 81607e0c <genimg_get_arch_name+0x10>
81607e04:	e59f0004 	ldr	r0, [pc, #4]	; 81607e10 <genimg_get_arch_name+0x14>
81607e08:	eaffffed 	b	81607dc4 <get_table_entry_name>
81607e0c:	81615e78 	.word	0x81615e78
81607e10:	8161331c 	.word	0x8161331c

81607e14 <genimg_get_type_name>:
81607e14:	e1a02000 	mov	r2, r0
81607e18:	e59f1004 	ldr	r1, [pc, #4]	; 81607e24 <genimg_get_type_name+0x10>
81607e1c:	e59f0004 	ldr	r0, [pc, #4]	; 81607e28 <genimg_get_type_name+0x14>
81607e20:	eaffffe7 	b	81607dc4 <get_table_entry_name>
81607e24:	81615eac 	.word	0x81615eac
81607e28:	81613520 	.word	0x81613520

81607e2c <genimg_get_comp_name>:
81607e2c:	e1a02000 	mov	r2, r0
81607e30:	e59f1004 	ldr	r1, [pc, #4]	; 81607e3c <genimg_get_comp_name+0x10>
81607e34:	e59f0004 	ldr	r0, [pc, #4]	; 81607e40 <genimg_get_comp_name+0x14>
81607e38:	eaffffe1 	b	81607dc4 <get_table_entry_name>
81607e3c:	81615e8d 	.word	0x81615e8d
81607e40:	81613454 	.word	0x81613454

81607e44 <image_print_contents>:
81607e44:	e92d41ff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
81607e48:	e1a04000 	mov	r4, r0
81607e4c:	e2803020 	add	r3, r0, #32
81607e50:	e3a02020 	mov	r2, #32
81607e54:	e59f116c 	ldr	r1, [pc, #364]	; 81607fc8 <image_print_contents+0x184>
81607e58:	e59f016c 	ldr	r0, [pc, #364]	; 81607fcc <image_print_contents+0x188>
81607e5c:	eb002a06 	bl	8161267c <printf>
81607e60:	e59f1160 	ldr	r1, [pc, #352]	; 81607fc8 <image_print_contents+0x184>
81607e64:	e59f0164 	ldr	r0, [pc, #356]	; 81607fd0 <image_print_contents+0x18c>
81607e68:	eb002a03 	bl	8161267c <printf>
81607e6c:	e5d4001c 	ldrb	r0, [r4, #28]
81607e70:	ebffffdb 	bl	81607de4 <genimg_get_os_name>
81607e74:	e1a06000 	mov	r6, r0
81607e78:	e5d4001d 	ldrb	r0, [r4, #29]
81607e7c:	ebffffde 	bl	81607dfc <genimg_get_arch_name>
81607e80:	e1a05000 	mov	r5, r0
81607e84:	e5d4001e 	ldrb	r0, [r4, #30]
81607e88:	ebffffe1 	bl	81607e14 <genimg_get_type_name>
81607e8c:	e1a07000 	mov	r7, r0
81607e90:	e5d4001f 	ldrb	r0, [r4, #31]
81607e94:	ebffffe4 	bl	81607e2c <genimg_get_comp_name>
81607e98:	e1a03007 	mov	r3, r7
81607e9c:	e1a02006 	mov	r2, r6
81607ea0:	e1a01005 	mov	r1, r5
81607ea4:	e58d0000 	str	r0, [sp]
81607ea8:	e59f0124 	ldr	r0, [pc, #292]	; 81607fd4 <image_print_contents+0x190>
81607eac:	eb0029f2 	bl	8161267c <printf>
81607eb0:	e59f1110 	ldr	r1, [pc, #272]	; 81607fc8 <image_print_contents+0x184>
81607eb4:	e59f011c 	ldr	r0, [pc, #284]	; 81607fd8 <image_print_contents+0x194>
81607eb8:	eb0029ef 	bl	8161267c <printf>
81607ebc:	e594000c 	ldr	r0, [r4, #12]
81607ec0:	e6bf0f30 	rev	r0, r0
81607ec4:	ebffffa9 	bl	81607d70 <genimg_print_size>
81607ec8:	e5942010 	ldr	r2, [r4, #16]
81607ecc:	e59f10f4 	ldr	r1, [pc, #244]	; 81607fc8 <image_print_contents+0x184>
81607ed0:	e6bf2f32 	rev	r2, r2
81607ed4:	e59f0100 	ldr	r0, [pc, #256]	; 81607fdc <image_print_contents+0x198>
81607ed8:	eb0029e7 	bl	8161267c <printf>
81607edc:	e5942014 	ldr	r2, [r4, #20]
81607ee0:	e59f10e0 	ldr	r1, [pc, #224]	; 81607fc8 <image_print_contents+0x184>
81607ee4:	e6bf2f32 	rev	r2, r2
81607ee8:	e59f00f0 	ldr	r0, [pc, #240]	; 81607fe0 <image_print_contents+0x19c>
81607eec:	eb0029e2 	bl	8161267c <printf>
81607ef0:	e5d4301e 	ldrb	r3, [r4, #30]
81607ef4:	e20320fd 	and	r2, r3, #253	; 0xfd
81607ef8:	e3520004 	cmp	r2, #4
81607efc:	1a000025 	bne	81607f98 <image_print_contents+0x154>
81607f00:	e1a00004 	mov	r0, r4
81607f04:	e59f60bc 	ldr	r6, [pc, #188]	; 81607fc8 <image_print_contents+0x184>
81607f08:	ebffff3c 	bl	81607c00 <image_multi_count>
81607f0c:	e59f10b4 	ldr	r1, [pc, #180]	; 81607fc8 <image_print_contents+0x184>
81607f10:	e59f80cc 	ldr	r8, [pc, #204]	; 81607fe4 <image_print_contents+0x1a0>
81607f14:	e3a05000 	mov	r5, #0
81607f18:	e1a07000 	mov	r7, r0
81607f1c:	e59f00c4 	ldr	r0, [pc, #196]	; 81607fe8 <image_print_contents+0x1a4>
81607f20:	eb0029d5 	bl	8161267c <printf>
81607f24:	e1550007 	cmp	r5, r7
81607f28:	1a000001 	bne	81607f34 <image_print_contents+0xf0>
81607f2c:	e28dd010 	add	sp, sp, #16
81607f30:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81607f34:	e28d300c 	add	r3, sp, #12
81607f38:	e28d2008 	add	r2, sp, #8
81607f3c:	e1a01005 	mov	r1, r5
81607f40:	e1a00004 	mov	r0, r4
81607f44:	ebffff34 	bl	81607c1c <image_multi_getimg>
81607f48:	e1a02005 	mov	r2, r5
81607f4c:	e1a01006 	mov	r1, r6
81607f50:	e1a00008 	mov	r0, r8
81607f54:	eb0029c8 	bl	8161267c <printf>
81607f58:	e59d000c 	ldr	r0, [sp, #12]
81607f5c:	ebffff83 	bl	81607d70 <genimg_print_size>
81607f60:	e5d4201e 	ldrb	r2, [r4, #30]
81607f64:	e3550000 	cmp	r5, #0
81607f68:	e2423006 	sub	r3, r2, #6
81607f6c:	e16f3f13 	clz	r3, r3
81607f70:	e1a032a3 	lsr	r3, r3, #5
81607f74:	03a03000 	moveq	r3, #0
81607f78:	e3530000 	cmp	r3, #0
81607f7c:	0a000003 	beq	81607f90 <image_print_contents+0x14c>
81607f80:	e59d2008 	ldr	r2, [sp, #8]
81607f84:	e1a01006 	mov	r1, r6
81607f88:	e59f005c 	ldr	r0, [pc, #92]	; 81607fec <image_print_contents+0x1a8>
81607f8c:	eb0029ba 	bl	8161267c <printf>
81607f90:	e2855001 	add	r5, r5, #1
81607f94:	eaffffe2 	b	81607f24 <image_print_contents+0xe0>
81607f98:	e353001f 	cmp	r3, #31
81607f9c:	1affffe2 	bne	81607f2c <image_print_contents+0xe8>
81607fa0:	e594200c 	ldr	r2, [r4, #12]
81607fa4:	e5941010 	ldr	r1, [r4, #16]
81607fa8:	e6bf2f32 	rev	r2, r2
81607fac:	e59f003c 	ldr	r0, [pc, #60]	; 81607ff0 <image_print_contents+0x1ac>
81607fb0:	e6bf1f31 	rev	r1, r1
81607fb4:	e2422d7e 	sub	r2, r2, #8064	; 0x1f80
81607fb8:	e2422020 	sub	r2, r2, #32
81607fbc:	e2411040 	sub	r1, r1, #64	; 0x40
81607fc0:	eb0029ad 	bl	8161267c <printf>
81607fc4:	eaffffd8 	b	81607f2c <image_print_contents+0xe8>
81607fc8:	81616120 	.word	0x81616120
81607fcc:	816160dd 	.word	0x816160dd
81607fd0:	816160f3 	.word	0x816160f3
81607fd4:	81616104 	.word	0x81616104
81607fd8:	81616113 	.word	0x81616113
81607fdc:	81616124 	.word	0x81616124
81607fe0:	8161613a 	.word	0x8161613a
81607fe4:	8161615d 	.word	0x8161615d
81607fe8:	81616150 	.word	0x81616150
81607fec:	8161616d 	.word	0x8161616d
81607ff0:	81616185 	.word	0x81616185

81607ff4 <genimg_get_kernel_addr_fit>:
81607ff4:	e3500000 	cmp	r0, #0
81607ff8:	0a000002 	beq	81608008 <genimg_get_kernel_addr_fit+0x14>
81607ffc:	e3a02010 	mov	r2, #16
81608000:	e3a01000 	mov	r1, #0
81608004:	ea0029e3 	b	81612798 <simple_strtoul>
81608008:	e59f3004 	ldr	r3, [pc, #4]	; 81608014 <genimg_get_kernel_addr_fit+0x20>
8160800c:	e5930000 	ldr	r0, [r3]
81608010:	e12fff1e 	bx	lr
81608014:	816190ac 	.word	0x816190ac

81608018 <genimg_get_format>:
81608018:	e5902000 	ldr	r2, [r0]
8160801c:	e59f3024 	ldr	r3, [pc, #36]	; 81608048 <genimg_get_format+0x30>
81608020:	e1520003 	cmp	r2, r3
81608024:	0a000005 	beq	81608040 <genimg_get_format+0x28>
81608028:	e92d4010 	push	{r4, lr}
8160802c:	eb001b4f 	bl	8160ed70 <fdt_check_header>
81608030:	e3500000 	cmp	r0, #0
81608034:	13a00000 	movne	r0, #0
81608038:	03a00002 	moveq	r0, #2
8160803c:	e8bd8010 	pop	{r4, pc}
81608040:	e3a00001 	mov	r0, #1
81608044:	e12fff1e 	bx	lr
81608048:	56190527 	.word	0x56190527

8160804c <genimg_get_image>:
8160804c:	e12fff1e 	bx	lr

81608050 <genimg_has_config>:
81608050:	e3a00000 	mov	r0, #0
81608054:	e12fff1e 	bx	lr

81608058 <boot_get_ramdisk>:
81608058:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160805c:	e1a08003 	mov	r8, r3
81608060:	e59db02c 	ldr	fp, [sp, #44]	; 0x2c
81608064:	e3500001 	cmp	r0, #1
81608068:	e59d3028 	ldr	r3, [sp, #40]	; 0x28
8160806c:	e3a07000 	mov	r7, #0
81608070:	e1a06002 	mov	r6, r2
81608074:	e5837000 	str	r7, [r3]
81608078:	e58b7000 	str	r7, [fp]
8160807c:	da000073 	ble	81608250 <boot_get_ramdisk+0x1f8>
81608080:	e591a004 	ldr	sl, [r1, #4]
81608084:	e15a0007 	cmp	sl, r7
81608088:	0a000070 	beq	81608250 <boot_get_ramdisk+0x1f8>
8160808c:	e59f1214 	ldr	r1, [pc, #532]	; 816082a8 <boot_get_ramdisk+0x250>
81608090:	e1a0000a 	mov	r0, sl
81608094:	eb0025e9 	bl	81611840 <strcmp>
81608098:	e3500000 	cmp	r0, #0
8160809c:	058d0000 	streq	r0, [sp]
816080a0:	058d0004 	streq	r0, [sp, #4]
816080a4:	0a000013 	beq	816080f8 <boot_get_ramdisk+0xa0>
816080a8:	e3a02010 	mov	r2, #16
816080ac:	e1a01007 	mov	r1, r7
816080b0:	e1a0000a 	mov	r0, sl
816080b4:	eb0029b7 	bl	81612798 <simple_strtoul>
816080b8:	e1a04000 	mov	r4, r0
816080bc:	ebffffd5 	bl	81608018 <genimg_get_format>
816080c0:	e3500001 	cmp	r0, #1
816080c4:	e1a05000 	mov	r5, r0
816080c8:	0a000014 	beq	81608120 <boot_get_ramdisk+0xc8>
816080cc:	e3a0103a 	mov	r1, #58	; 0x3a
816080d0:	e1a0000a 	mov	r0, sl
816080d4:	eb0025f2 	bl	816118a4 <strchr>
816080d8:	e3500000 	cmp	r0, #0
816080dc:	0a000057 	beq	81608240 <boot_get_ramdisk+0x1e8>
816080e0:	e3a02010 	mov	r2, #16
816080e4:	e1a01007 	mov	r1, r7
816080e8:	e2800001 	add	r0, r0, #1
816080ec:	eb0029a9 	bl	81612798 <simple_strtoul>
816080f0:	e58d4000 	str	r4, [sp]
816080f4:	e58d0004 	str	r0, [sp, #4]
816080f8:	e59d5000 	ldr	r5, [sp]
816080fc:	e3550000 	cmp	r5, #0
81608100:	0a000014 	beq	81608158 <boot_get_ramdisk+0x100>
81608104:	e59d3028 	ldr	r3, [sp, #40]	; 0x28
81608108:	e5835000 	str	r5, [r3]
8160810c:	e59d3004 	ldr	r3, [sp, #4]
81608110:	e0835005 	add	r5, r3, r5
81608114:	e58b5000 	str	r5, [fp]
81608118:	e3a05000 	mov	r5, #0
8160811c:	ea00000d 	b	81608158 <boot_get_ramdisk+0x100>
81608120:	e1a01004 	mov	r1, r4
81608124:	e59f0180 	ldr	r0, [pc, #384]	; 816082ac <boot_get_ramdisk+0x254>
81608128:	eb002953 	bl	8161267c <printf>
8160812c:	e3a00009 	mov	r0, #9
81608130:	ebfff157 	bl	81604694 <show_boot_progress>
81608134:	e5942000 	ldr	r2, [r4]
81608138:	e59f3170 	ldr	r3, [pc, #368]	; 816082b0 <boot_get_ramdisk+0x258>
8160813c:	e5966088 	ldr	r6, [r6, #136]	; 0x88
81608140:	e1520003 	cmp	r2, r3
81608144:	0a000006 	beq	81608164 <boot_get_ramdisk+0x10c>
81608148:	e59f0164 	ldr	r0, [pc, #356]	; 816082b4 <boot_get_ramdisk+0x25c>
8160814c:	ebfffb10 	bl	81606d94 <puts>
81608150:	e3e00009 	mvn	r0, #9
81608154:	ebfff14e 	bl	81604694 <show_boot_progress>
81608158:	e1a00005 	mov	r0, r5
8160815c:	e28dd008 	add	sp, sp, #8
81608160:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81608164:	e1a00004 	mov	r0, r4
81608168:	ebfffe83 	bl	81607b7c <image_check_hcrc>
8160816c:	e3500000 	cmp	r0, #0
81608170:	1a000003 	bne	81608184 <boot_get_ramdisk+0x12c>
81608174:	e59f013c 	ldr	r0, [pc, #316]	; 816082b8 <boot_get_ramdisk+0x260>
81608178:	ebfffb05 	bl	81606d94 <puts>
8160817c:	e3e0000a 	mvn	r0, #10
81608180:	eafffff3 	b	81608154 <boot_get_ramdisk+0xfc>
81608184:	e3a0000a 	mov	r0, #10
81608188:	ebfff141 	bl	81604694 <show_boot_progress>
8160818c:	e1a00004 	mov	r0, r4
81608190:	ebffff2b 	bl	81607e44 <image_print_contents>
81608194:	e3560000 	cmp	r6, #0
81608198:	0a00000b 	beq	816081cc <boot_get_ramdisk+0x174>
8160819c:	e59f0118 	ldr	r0, [pc, #280]	; 816082bc <boot_get_ramdisk+0x264>
816081a0:	ebfffafb 	bl	81606d94 <puts>
816081a4:	e1a00004 	mov	r0, r4
816081a8:	ebfffe86 	bl	81607bc8 <image_check_dcrc>
816081ac:	e3500000 	cmp	r0, #0
816081b0:	1a000003 	bne	816081c4 <boot_get_ramdisk+0x16c>
816081b4:	e59f0104 	ldr	r0, [pc, #260]	; 816082c0 <boot_get_ramdisk+0x268>
816081b8:	ebfffaf5 	bl	81606d94 <puts>
816081bc:	e3e0000b 	mvn	r0, #11
816081c0:	eaffffe3 	b	81608154 <boot_get_ramdisk+0xfc>
816081c4:	e59f00f8 	ldr	r0, [pc, #248]	; 816082c4 <boot_get_ramdisk+0x26c>
816081c8:	ebfffaf1 	bl	81606d94 <puts>
816081cc:	e3a0000b 	mov	r0, #11
816081d0:	ebfff12f 	bl	81604694 <show_boot_progress>
816081d4:	e5d4301c 	ldrb	r3, [r4, #28]
816081d8:	e3530005 	cmp	r3, #5
816081dc:	1a000008 	bne	81608204 <boot_get_ramdisk+0x1ac>
816081e0:	e5d4301d 	ldrb	r3, [r4, #29]
816081e4:	e1580003 	cmp	r8, r3
816081e8:	0a000002 	beq	816081f8 <boot_get_ramdisk+0x1a0>
816081ec:	e3530002 	cmp	r3, #2
816081f0:	03580016 	cmpeq	r8, #22
816081f4:	1a000002 	bne	81608204 <boot_get_ramdisk+0x1ac>
816081f8:	e5d4301e 	ldrb	r3, [r4, #30]
816081fc:	e3530003 	cmp	r3, #3
81608200:	0a000006 	beq	81608220 <boot_get_ramdisk+0x1c8>
81608204:	e1a00008 	mov	r0, r8
81608208:	ebfffefb 	bl	81607dfc <genimg_get_arch_name>
8160820c:	e1a01000 	mov	r1, r0
81608210:	e59f00b0 	ldr	r0, [pc, #176]	; 816082c8 <boot_get_ramdisk+0x270>
81608214:	eb002918 	bl	8161267c <printf>
81608218:	e3e0000c 	mvn	r0, #12
8160821c:	eaffffcc 	b	81608154 <boot_get_ramdisk+0xfc>
81608220:	e3540000 	cmp	r4, #0
81608224:	0affffcb 	beq	81608158 <boot_get_ramdisk+0x100>
81608228:	e2843040 	add	r3, r4, #64	; 0x40
8160822c:	e58d3000 	str	r3, [sp]
81608230:	e594300c 	ldr	r3, [r4, #12]
81608234:	e6bf3f33 	rev	r3, r3
81608238:	e58d3004 	str	r3, [sp, #4]
8160823c:	eaffffad 	b	816080f8 <boot_get_ramdisk+0xa0>
81608240:	e59f0084 	ldr	r0, [pc, #132]	; 816082cc <boot_get_ramdisk+0x274>
81608244:	e3a05001 	mov	r5, #1
81608248:	ebfffad1 	bl	81606d94 <puts>
8160824c:	eaffffc1 	b	81608158 <boot_get_ramdisk+0x100>
81608250:	e5963044 	ldr	r3, [r6, #68]	; 0x44
81608254:	e3530000 	cmp	r3, #0
81608258:	0a00000d 	beq	81608294 <boot_get_ramdisk+0x23c>
8160825c:	e5d63022 	ldrb	r3, [r6, #34]	; 0x22
81608260:	e3530004 	cmp	r3, #4
81608264:	1a00000a 	bne	81608294 <boot_get_ramdisk+0x23c>
81608268:	e3a0000d 	mov	r0, #13
8160826c:	ebfff108 	bl	81604694 <show_boot_progress>
81608270:	e5961000 	ldr	r1, [r6]
81608274:	e59f0054 	ldr	r0, [pc, #84]	; 816082d0 <boot_get_ramdisk+0x278>
81608278:	eb0028ff 	bl	8161267c <printf>
8160827c:	e28d3004 	add	r3, sp, #4
81608280:	e1a0200d 	mov	r2, sp
81608284:	e3a01001 	mov	r1, #1
81608288:	e5960000 	ldr	r0, [r6]
8160828c:	ebfffe62 	bl	81607c1c <image_multi_getimg>
81608290:	eaffff98 	b	816080f8 <boot_get_ramdisk+0xa0>
81608294:	e3a0000e 	mov	r0, #14
81608298:	ebfff0fd 	bl	81604694 <show_boot_progress>
8160829c:	e3a03000 	mov	r3, #0
816082a0:	e58d3000 	str	r3, [sp]
816082a4:	eaffffe3 	b	81608238 <boot_get_ramdisk+0x1e0>
816082a8:	816144c1 	.word	0x816144c1
816082ac:	816144c3 	.word	0x816144c3
816082b0:	56190527 	.word	0x56190527
816082b4:	8161606c 	.word	0x8161606c
816082b8:	81616081 	.word	0x81616081
816082bc:	81616096 	.word	0x81616096
816082c0:	816160b4 	.word	0x816160b4
816082c4:	816160c2 	.word	0x816160c2
816082c8:	816144fb 	.word	0x816144fb
816082cc:	81614516 	.word	0x81614516
816082d0:	81614532 	.word	0x81614532

816082d4 <boot_ramdisk_high>:
816082d4:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816082d8:	e1a0a000 	mov	sl, r0
816082dc:	e59f0110 	ldr	r0, [pc, #272]	; 816083f4 <boot_ramdisk_high+0x120>
816082e0:	e1a06001 	mov	r6, r1
816082e4:	e1a07002 	mov	r7, r2
816082e8:	e1a04003 	mov	r4, r3
816082ec:	e59d8020 	ldr	r8, [sp, #32]
816082f0:	ebfff0a8 	bl	81604598 <env_get>
816082f4:	e3500000 	cmp	r0, #0
816082f8:	0a00000a 	beq	81608328 <boot_ramdisk_high+0x54>
816082fc:	e3a02010 	mov	r2, #16
81608300:	e3a01000 	mov	r1, #0
81608304:	eb002923 	bl	81612798 <simple_strtoul>
81608308:	e3700001 	cmn	r0, #1
8160830c:	1a000009 	bne	81608338 <boot_ramdisk_high+0x64>
81608310:	e3560000 	cmp	r6, #0
81608314:	1a000018 	bne	8160837c <boot_ramdisk_high+0xa8>
81608318:	e3a00000 	mov	r0, #0
8160831c:	e5840000 	str	r0, [r4]
81608320:	e5880000 	str	r0, [r8]
81608324:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81608328:	ebfffe81 	bl	81607d34 <env_get_bootm_mapsize>
8160832c:	e1a05000 	mov	r5, r0
81608330:	ebfffe59 	bl	81607c9c <env_get_bootm_low>
81608334:	e0850000 	add	r0, r5, r0
81608338:	e3560000 	cmp	r6, #0
8160833c:	0afffff5 	beq	81608318 <boot_ramdisk_high+0x44>
81608340:	e3500000 	cmp	r0, #0
81608344:	0a000015 	beq	816083a0 <boot_ramdisk_high+0xcc>
81608348:	e1a03000 	mov	r3, r0
8160834c:	e3a02a01 	mov	r2, #4096	; 0x1000
81608350:	e1a01007 	mov	r1, r7
81608354:	e1a0000a 	mov	r0, sl
81608358:	eb001a12 	bl	8160eba8 <lmb_alloc_base>
8160835c:	e5840000 	str	r0, [r4]
81608360:	e5943000 	ldr	r3, [r4]
81608364:	e3530000 	cmp	r3, #0
81608368:	1a000011 	bne	816083b4 <boot_ramdisk_high+0xe0>
8160836c:	e59f0084 	ldr	r0, [pc, #132]	; 816083f8 <boot_ramdisk_high+0x124>
81608370:	ebfffa87 	bl	81606d94 <puts>
81608374:	e3e00000 	mvn	r0, #0
81608378:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160837c:	e5846000 	str	r6, [r4]
81608380:	e0863007 	add	r3, r6, r7
81608384:	e1a02007 	mov	r2, r7
81608388:	e5883000 	str	r3, [r8]
8160838c:	e1a01006 	mov	r1, r6
81608390:	e1a0000a 	mov	r0, sl
81608394:	eb0019b8 	bl	8160ea7c <lmb_reserve>
81608398:	e3a00000 	mov	r0, #0
8160839c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816083a0:	e3a02a01 	mov	r2, #4096	; 0x1000
816083a4:	e1a01007 	mov	r1, r7
816083a8:	e1a0000a 	mov	r0, sl
816083ac:	eb001a0a 	bl	8160ebdc <lmb_alloc>
816083b0:	eaffffe9 	b	8160835c <boot_ramdisk_high+0x88>
816083b4:	e3a0000c 	mov	r0, #12
816083b8:	ebfff0b5 	bl	81604694 <show_boot_progress>
816083bc:	e5942000 	ldr	r2, [r4]
816083c0:	e59f0034 	ldr	r0, [pc, #52]	; 816083fc <boot_ramdisk_high+0x128>
816083c4:	e0872002 	add	r2, r7, r2
816083c8:	e5882000 	str	r2, [r8]
816083cc:	e5941000 	ldr	r1, [r4]
816083d0:	eb0028a9 	bl	8161267c <printf>
816083d4:	e5940000 	ldr	r0, [r4]
816083d8:	e3a03801 	mov	r3, #65536	; 0x10000
816083dc:	e1a02007 	mov	r2, r7
816083e0:	e1a01006 	mov	r1, r6
816083e4:	ebfffe5e 	bl	81607d64 <memmove_wd>
816083e8:	e59f0010 	ldr	r0, [pc, #16]	; 81608400 <boot_ramdisk_high+0x12c>
816083ec:	ebfffa68 	bl	81606d94 <puts>
816083f0:	eaffffe8 	b	81608398 <boot_ramdisk_high+0xc4>
816083f4:	81614651 	.word	0x81614651
816083f8:	8161465d 	.word	0x8161465d
816083fc:	81614679 	.word	0x81614679
81608400:	816160c2 	.word	0x816160c2

81608404 <boot_get_setup>:
81608404:	e3e00001 	mvn	r0, #1
81608408:	e12fff1e 	bx	lr

8160840c <image_setup_linux>:
8160840c:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
81608410:	e1a06000 	mov	r6, r0
81608414:	e5903070 	ldr	r3, [r0, #112]	; 0x70
81608418:	e2805090 	add	r5, r0, #144	; 0x90
8160841c:	e5b6106c 	ldr	r1, [r6, #108]!	; 0x6c
81608420:	e28d7008 	add	r7, sp, #8
81608424:	e1a04000 	mov	r4, r0
81608428:	e1a00005 	mov	r0, r5
8160842c:	e5273004 	str	r3, [r7, #-4]!
81608430:	eb000019 	bl	8160849c <boot_fdt_add_mem_rsv_regions>
81608434:	e1a02007 	mov	r2, r7
81608438:	e1a01006 	mov	r1, r6
8160843c:	e1a00005 	mov	r0, r5
81608440:	eb000038 	bl	81608528 <boot_relocate_fdt>
81608444:	e3500000 	cmp	r0, #0
81608448:	1a000006 	bne	81608468 <image_setup_linux+0x5c>
8160844c:	e59d2004 	ldr	r2, [sp, #4]
81608450:	e3520000 	cmp	r2, #0
81608454:	0a000003 	beq	81608468 <image_setup_linux+0x5c>
81608458:	e1a03005 	mov	r3, r5
8160845c:	e594106c 	ldr	r1, [r4, #108]	; 0x6c
81608460:	e1a00004 	mov	r0, r4
81608464:	eb000139 	bl	81608950 <image_setup_libfdt>
81608468:	e28dd00c 	add	sp, sp, #12
8160846c:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}

81608470 <fdt_error>:
81608470:	e92d4010 	push	{r4, lr}
81608474:	e1a04000 	mov	r4, r0
81608478:	e59f0014 	ldr	r0, [pc, #20]	; 81608494 <fdt_error+0x24>
8160847c:	ebfffa44 	bl	81606d94 <puts>
81608480:	e1a00004 	mov	r0, r4
81608484:	ebfffa42 	bl	81606d94 <puts>
81608488:	e59f0008 	ldr	r0, [pc, #8]	; 81608498 <fdt_error+0x28>
8160848c:	e8bd4010 	pop	{r4, lr}
81608490:	eafffa3f 	b	81606d94 <puts>
81608494:	81615bd7 	.word	0x81615bd7
81608498:	81615bdf 	.word	0x81615bdf

8160849c <boot_fdt_add_mem_rsv_regions>:
8160849c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816084a0:	e1a07000 	mov	r7, r0
816084a4:	e24dd018 	sub	sp, sp, #24
816084a8:	e1a00001 	mov	r0, r1
816084ac:	e1a05001 	mov	r5, r1
816084b0:	eb001a2e 	bl	8160ed70 <fdt_check_header>
816084b4:	e2504000 	subs	r4, r0, #0
816084b8:	1a000005 	bne	816084d4 <boot_fdt_add_mem_rsv_regions+0x38>
816084bc:	e1a00005 	mov	r0, r5
816084c0:	e59f805c 	ldr	r8, [pc, #92]	; 81608524 <boot_fdt_add_mem_rsv_regions+0x88>
816084c4:	eb001b5a 	bl	8160f234 <fdt_num_mem_rsv>
816084c8:	e1a06000 	mov	r6, r0
816084cc:	e1540006 	cmp	r4, r6
816084d0:	ba000001 	blt	816084dc <boot_fdt_add_mem_rsv_regions+0x40>
816084d4:	e28dd018 	add	sp, sp, #24
816084d8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
816084dc:	e28d3010 	add	r3, sp, #16
816084e0:	e28d2008 	add	r2, sp, #8
816084e4:	e1a01004 	mov	r1, r4
816084e8:	e1a00005 	mov	r0, r5
816084ec:	eb001b38 	bl	8160f1d4 <fdt_get_mem_rsv>
816084f0:	e3500000 	cmp	r0, #0
816084f4:	1a000008 	bne	8160851c <boot_fdt_add_mem_rsv_regions+0x80>
816084f8:	e1cd21d0 	ldrd	r2, [sp, #16]
816084fc:	e1a00008 	mov	r0, r8
81608500:	e1cd20f0 	strd	r2, [sp]
81608504:	e1cd20d8 	ldrd	r2, [sp, #8]
81608508:	eb00285b 	bl	8161267c <printf>
8160850c:	e59d2010 	ldr	r2, [sp, #16]
81608510:	e59d1008 	ldr	r1, [sp, #8]
81608514:	e1a00007 	mov	r0, r7
81608518:	eb001957 	bl	8160ea7c <lmb_reserve>
8160851c:	e2844001 	add	r4, r4, #1
81608520:	eaffffe9 	b	816084cc <boot_fdt_add_mem_rsv_regions+0x30>
81608524:	816142d7 	.word	0x816142d7

81608528 <boot_relocate_fdt>:
81608528:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160852c:	e5926000 	ldr	r6, [r2]
81608530:	e3560000 	cmp	r6, #0
81608534:	0a00001d 	beq	816085b0 <boot_relocate_fdt+0x88>
81608538:	e5917000 	ldr	r7, [r1]
8160853c:	e1a05000 	mov	r5, r0
81608540:	e1a0a002 	mov	sl, r2
81608544:	e1a0b001 	mov	fp, r1
81608548:	e1a00007 	mov	r0, r7
8160854c:	eb001a07 	bl	8160ed70 <fdt_check_header>
81608550:	e2506000 	subs	r6, r0, #0
81608554:	159f0140 	ldrne	r0, [pc, #320]	; 8160869c <boot_relocate_fdt+0x174>
81608558:	1a00002d 	bne	81608614 <boot_relocate_fdt+0xec>
8160855c:	e59f013c 	ldr	r0, [pc, #316]	; 816086a0 <boot_relocate_fdt+0x178>
81608560:	e59a4000 	ldr	r4, [sl]
81608564:	ebfff00b 	bl	81604598 <env_get>
81608568:	e2848a03 	add	r8, r4, #12288	; 0x3000
8160856c:	e3500000 	cmp	r0, #0
81608570:	0a00002e 	beq	81608630 <boot_relocate_fdt+0x108>
81608574:	e3a02010 	mov	r2, #16
81608578:	e1a01006 	mov	r1, r6
8160857c:	eb002885 	bl	81612798 <simple_strtoul>
81608580:	e3700001 	cmn	r0, #1
81608584:	e1a03000 	mov	r3, r0
81608588:	1a00000b 	bne	816085bc <boot_relocate_fdt+0x94>
8160858c:	e1a02008 	mov	r2, r8
81608590:	e1a01007 	mov	r1, r7
81608594:	e1a00005 	mov	r0, r5
81608598:	eb001937 	bl	8160ea7c <lmb_reserve>
8160859c:	e3570000 	cmp	r7, #0
816085a0:	1a00002c 	bne	81608658 <boot_relocate_fdt+0x130>
816085a4:	e59f00f8 	ldr	r0, [pc, #248]	; 816086a4 <boot_relocate_fdt+0x17c>
816085a8:	ebfff9f9 	bl	81606d94 <puts>
816085ac:	e3a06001 	mov	r6, #1
816085b0:	e1a00006 	mov	r0, r6
816085b4:	e28dd008 	add	sp, sp, #8
816085b8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816085bc:	e3530000 	cmp	r3, #0
816085c0:	e3a02a01 	mov	r2, #4096	; 0x1000
816085c4:	e1a01008 	mov	r1, r8
816085c8:	e1a00005 	mov	r0, r5
816085cc:	0a000012 	beq	8160861c <boot_relocate_fdt+0xf4>
816085d0:	eb001974 	bl	8160eba8 <lmb_alloc_base>
816085d4:	e2505000 	subs	r5, r0, #0
816085d8:	059f00c8 	ldreq	r0, [pc, #200]	; 816086a8 <boot_relocate_fdt+0x180>
816085dc:	0afffff1 	beq	816085a8 <boot_relocate_fdt+0x80>
816085e0:	e2842dbf 	add	r2, r4, #12224	; 0x2fc0
816085e4:	e1a01005 	mov	r1, r5
816085e8:	e282203f 	add	r2, r2, #63	; 0x3f
816085ec:	e59f00b8 	ldr	r0, [pc, #184]	; 816086ac <boot_relocate_fdt+0x184>
816085f0:	e0852002 	add	r2, r5, r2
816085f4:	eb002820 	bl	8161267c <printf>
816085f8:	e1a02008 	mov	r2, r8
816085fc:	e1a01005 	mov	r1, r5
81608600:	e1a00007 	mov	r0, r7
81608604:	eb001e61 	bl	8160ff90 <fdt_open_into>
81608608:	e3500000 	cmp	r0, #0
8160860c:	0a00001f 	beq	81608690 <boot_relocate_fdt+0x168>
81608610:	e59f0098 	ldr	r0, [pc, #152]	; 816086b0 <boot_relocate_fdt+0x188>
81608614:	ebffff95 	bl	81608470 <fdt_error>
81608618:	eaffffe3 	b	816085ac <boot_relocate_fdt+0x84>
8160861c:	eb00196e 	bl	8160ebdc <lmb_alloc>
81608620:	e3500000 	cmp	r0, #0
81608624:	e1a05000 	mov	r5, r0
81608628:	0affffdd 	beq	816085a4 <boot_relocate_fdt+0x7c>
8160862c:	eaffffeb 	b	816085e0 <boot_relocate_fdt+0xb8>
81608630:	ebfffdbf 	bl	81607d34 <env_get_bootm_mapsize>
81608634:	e58d0004 	str	r0, [sp, #4]
81608638:	ebfffd97 	bl	81607c9c <env_get_bootm_low>
8160863c:	e59d3004 	ldr	r3, [sp, #4]
81608640:	e3a02a01 	mov	r2, #4096	; 0x1000
81608644:	e1a01008 	mov	r1, r8
81608648:	e0833000 	add	r3, r3, r0
8160864c:	e1a00005 	mov	r0, r5
81608650:	eb001954 	bl	8160eba8 <lmb_alloc_base>
81608654:	eafffff1 	b	81608620 <boot_relocate_fdt+0xf8>
81608658:	e2842dbf 	add	r2, r4, #12224	; 0x2fc0
8160865c:	e6bf3f38 	rev	r3, r8
81608660:	e282203f 	add	r2, r2, #63	; 0x3f
81608664:	e5873004 	str	r3, [r7, #4]
81608668:	e0872002 	add	r2, r7, r2
8160866c:	e1a01007 	mov	r1, r7
81608670:	e59f003c 	ldr	r0, [pc, #60]	; 816086b4 <boot_relocate_fdt+0x18c>
81608674:	e1a05007 	mov	r5, r7
81608678:	eb0027ff 	bl	8161267c <printf>
8160867c:	e58b5000 	str	r5, [fp]
81608680:	e1a00005 	mov	r0, r5
81608684:	e58a8000 	str	r8, [sl]
81608688:	ebffe77b 	bl	8160247c <set_working_fdt_addr>
8160868c:	eaffffc7 	b	816085b0 <boot_relocate_fdt+0x88>
81608690:	e59f0020 	ldr	r0, [pc, #32]	; 816086b8 <boot_relocate_fdt+0x190>
81608694:	ebfff9be 	bl	81606d94 <puts>
81608698:	eafffff7 	b	8160867c <boot_relocate_fdt+0x154>
8160869c:	816146a5 	.word	0x816146a5
816086a0:	816146b8 	.word	0x816146b8
816086a4:	816146ed 	.word	0x816146ed
816086a8:	816146c1 	.word	0x816146c1
816086ac:	8161473a 	.word	0x8161473a
816086b0:	81614764 	.word	0x81614764
816086b4:	8161470d 	.word	0x8161470d
816086b8:	816160c2 	.word	0x816160c2

816086bc <boot_get_fdt>:
816086bc:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
816086c0:	e3510002 	cmp	r1, #2
816086c4:	e59da02c 	ldr	sl, [sp, #44]	; 0x2c
816086c8:	e3a03000 	mov	r3, #0
816086cc:	e59d8030 	ldr	r8, [sp, #48]	; 0x30
816086d0:	e59d4028 	ldr	r4, [sp, #40]	; 0x28
816086d4:	e58a3000 	str	r3, [sl]
816086d8:	e5883000 	str	r3, [r8]
816086dc:	da000002 	ble	816086ec <boot_get_fdt+0x30>
816086e0:	e5927008 	ldr	r7, [r2, #8]
816086e4:	e1570003 	cmp	r7, r3
816086e8:	1a000004 	bne	81608700 <boot_get_fdt+0x44>
816086ec:	e1a00004 	mov	r0, r4
816086f0:	ebfffe56 	bl	81608050 <genimg_has_config>
816086f4:	e2506000 	subs	r6, r0, #0
816086f8:	0a00005d 	beq	81608874 <boot_get_fdt+0x1b8>
816086fc:	e3a07000 	mov	r7, #0
81608700:	e3a02010 	mov	r2, #16
81608704:	e3a01000 	mov	r1, #0
81608708:	e1a00007 	mov	r0, r7
8160870c:	eb002821 	bl	81612798 <simple_strtoul>
81608710:	ebfffe4d 	bl	8160804c <genimg_get_image>
81608714:	e1a05000 	mov	r5, r0
81608718:	ebfffe3e 	bl	81608018 <genimg_get_format>
8160871c:	e3500001 	cmp	r0, #1
81608720:	e1a06000 	mov	r6, r0
81608724:	0a000005 	beq	81608740 <boot_get_fdt+0x84>
81608728:	e3500002 	cmp	r0, #2
8160872c:	0a00004b 	beq	81608860 <boot_get_fdt+0x1a4>
81608730:	e59f01d0 	ldr	r0, [pc, #464]	; 81608908 <boot_get_fdt+0x24c>
81608734:	ebfff996 	bl	81606d94 <puts>
81608738:	e3a06001 	mov	r6, #1
8160873c:	ea000032 	b	8160880c <boot_get_fdt+0x150>
81608740:	e1a01005 	mov	r1, r5
81608744:	e59f01c0 	ldr	r0, [pc, #448]	; 8160890c <boot_get_fdt+0x250>
81608748:	eb0027cb 	bl	8161267c <printf>
8160874c:	e1a00005 	mov	r0, r5
81608750:	ebfffdbb 	bl	81607e44 <image_print_contents>
81608754:	e59f01b4 	ldr	r0, [pc, #436]	; 81608910 <boot_get_fdt+0x254>
81608758:	ebfff98d 	bl	81606d94 <puts>
8160875c:	e1a00005 	mov	r0, r5
81608760:	ebfffd05 	bl	81607b7c <image_check_hcrc>
81608764:	e3500000 	cmp	r0, #0
81608768:	059f01a4 	ldreq	r0, [pc, #420]	; 81608914 <boot_get_fdt+0x258>
8160876c:	0a000025 	beq	81608808 <boot_get_fdt+0x14c>
81608770:	e1a00005 	mov	r0, r5
81608774:	ebfffd13 	bl	81607bc8 <image_check_dcrc>
81608778:	e3500000 	cmp	r0, #0
8160877c:	059f0194 	ldreq	r0, [pc, #404]	; 81608918 <boot_get_fdt+0x25c>
81608780:	0a000020 	beq	81608808 <boot_get_fdt+0x14c>
81608784:	e59f0190 	ldr	r0, [pc, #400]	; 8160891c <boot_get_fdt+0x260>
81608788:	ebfff981 	bl	81606d94 <puts>
8160878c:	e5d5301e 	ldrb	r3, [r5, #30]
81608790:	e3530008 	cmp	r3, #8
81608794:	159f0184 	ldrne	r0, [pc, #388]	; 81608920 <boot_get_fdt+0x264>
81608798:	1a00001a 	bne	81608808 <boot_get_fdt+0x14c>
8160879c:	e5d5301f 	ldrb	r3, [r5, #31]
816087a0:	e3530000 	cmp	r3, #0
816087a4:	159f0178 	ldrne	r0, [pc, #376]	; 81608924 <boot_get_fdt+0x268>
816087a8:	1a000016 	bne	81608808 <boot_get_fdt+0x14c>
816087ac:	e285b040 	add	fp, r5, #64	; 0x40
816087b0:	e1a0000b 	mov	r0, fp
816087b4:	eb00196d 	bl	8160ed70 <fdt_check_header>
816087b8:	e3500000 	cmp	r0, #0
816087bc:	159f0164 	ldrne	r0, [pc, #356]	; 81608928 <boot_get_fdt+0x26c>
816087c0:	1a000010 	bne	81608808 <boot_get_fdt+0x14c>
816087c4:	e3550000 	cmp	r5, #0
816087c8:	0a00000f 	beq	8160880c <boot_get_fdt+0x150>
816087cc:	e5954010 	ldr	r4, [r5, #16]
816087d0:	e595200c 	ldr	r2, [r5, #12]
816087d4:	e6bf4f34 	rev	r4, r4
816087d8:	e154000b 	cmp	r4, fp
816087dc:	11550004 	cmpne	r5, r4
816087e0:	e6bf2f32 	rev	r2, r2
816087e4:	03a06001 	moveq	r6, #1
816087e8:	13a06000 	movne	r6, #0
816087ec:	0a000011 	beq	81608838 <boot_get_fdt+0x17c>
816087f0:	e082300b 	add	r3, r2, fp
816087f4:	e0841002 	add	r1, r4, r2
816087f8:	e1540003 	cmp	r4, r3
816087fc:	31550001 	cmpcc	r5, r1
81608800:	2a000009 	bcs	8160882c <boot_get_fdt+0x170>
81608804:	e59f0120 	ldr	r0, [pc, #288]	; 8160892c <boot_get_fdt+0x270>
81608808:	ebffff18 	bl	81608470 <fdt_error>
8160880c:	e3a03000 	mov	r3, #0
81608810:	e1570003 	cmp	r7, r3
81608814:	02066001 	andeq	r6, r6, #1
81608818:	e58a3000 	str	r3, [sl]
8160881c:	13a06000 	movne	r6, #0
81608820:	e5883000 	str	r3, [r8]
81608824:	e2260001 	eor	r0, r6, #1
81608828:	ea00000a 	b	81608858 <boot_get_fdt+0x19c>
8160882c:	e1a0100b 	mov	r1, fp
81608830:	e1a00004 	mov	r0, r4
81608834:	eb00245d 	bl	816119b0 <memmove>
81608838:	e1a01004 	mov	r1, r4
8160883c:	e59f00ec 	ldr	r0, [pc, #236]	; 81608930 <boot_get_fdt+0x274>
81608840:	eb00278d 	bl	8161267c <printf>
81608844:	e5943004 	ldr	r3, [r4, #4]
81608848:	e3a00000 	mov	r0, #0
8160884c:	e58a4000 	str	r4, [sl]
81608850:	e6bf3f33 	rev	r3, r3
81608854:	e5883000 	str	r3, [r8]
81608858:	e28dd008 	add	sp, sp, #8
8160885c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81608860:	e1a01005 	mov	r1, r5
81608864:	e59f00c8 	ldr	r0, [pc, #200]	; 81608934 <boot_get_fdt+0x278>
81608868:	eb002783 	bl	8161267c <printf>
8160886c:	e1a04005 	mov	r4, r5
81608870:	eafffff0 	b	81608838 <boot_get_fdt+0x17c>
81608874:	e5943044 	ldr	r3, [r4, #68]	; 0x44
81608878:	e3530000 	cmp	r3, #0
8160887c:	0a00001f 	beq	81608900 <boot_get_fdt+0x244>
81608880:	e5d43022 	ldrb	r3, [r4, #34]	; 0x22
81608884:	e3530004 	cmp	r3, #4
81608888:	1a00001c 	bne	81608900 <boot_get_fdt+0x244>
8160888c:	e5941000 	ldr	r1, [r4]
81608890:	e59f00a0 	ldr	r0, [pc, #160]	; 81608938 <boot_get_fdt+0x27c>
81608894:	eb002778 	bl	8161267c <printf>
81608898:	e28d3004 	add	r3, sp, #4
8160889c:	e1a0200d 	mov	r2, sp
816088a0:	e3a01002 	mov	r1, #2
816088a4:	e5940000 	ldr	r0, [r4]
816088a8:	ebfffcdb 	bl	81607c1c <image_multi_getimg>
816088ac:	e59d3004 	ldr	r3, [sp, #4]
816088b0:	e3530000 	cmp	r3, #0
816088b4:	0a000011 	beq	81608900 <boot_get_fdt+0x244>
816088b8:	e59d4000 	ldr	r4, [sp]
816088bc:	e59f0078 	ldr	r0, [pc, #120]	; 8160893c <boot_get_fdt+0x280>
816088c0:	e1a01004 	mov	r1, r4
816088c4:	eb00276c 	bl	8161267c <printf>
816088c8:	e1a00004 	mov	r0, r4
816088cc:	eb001927 	bl	8160ed70 <fdt_check_header>
816088d0:	e3500000 	cmp	r0, #0
816088d4:	159f0064 	ldrne	r0, [pc, #100]	; 81608940 <boot_get_fdt+0x284>
816088d8:	1a000005 	bne	816088f4 <boot_get_fdt+0x238>
816088dc:	e5943004 	ldr	r3, [r4, #4]
816088e0:	e59d2004 	ldr	r2, [sp, #4]
816088e4:	e6bf3f33 	rev	r3, r3
816088e8:	e1520003 	cmp	r2, r3
816088ec:	0affffd4 	beq	81608844 <boot_get_fdt+0x188>
816088f0:	e59f004c 	ldr	r0, [pc, #76]	; 81608944 <boot_get_fdt+0x288>
816088f4:	ebfffedd 	bl	81608470 <fdt_error>
816088f8:	e3a07000 	mov	r7, #0
816088fc:	eaffffc2 	b	8160880c <boot_get_fdt+0x150>
81608900:	e1a07006 	mov	r7, r6
81608904:	eaffff8b 	b	81608738 <boot_get_fdt+0x7c>
81608908:	816143ec 	.word	0x816143ec
8160890c:	8161430c 	.word	0x8161430c
81608910:	81616096 	.word	0x81616096
81608914:	81614341 	.word	0x81614341
81608918:	8161435d 	.word	0x8161435d
8160891c:	816160c2 	.word	0x816160c2
81608920:	81614372 	.word	0x81614372
81608924:	81614386 	.word	0x81614386
81608928:	8161439b 	.word	0x8161439b
8160892c:	816143b4 	.word	0x816143b4
81608930:	81614421 	.word	0x81614421
81608934:	816143c4 	.word	0x816143c4
81608938:	8161444a 	.word	0x8161444a
8160893c:	81614488 	.word	0x81614488
81608940:	816146a5 	.word	0x816146a5
81608944:	816144aa 	.word	0x816144aa

81608948 <ft_verify_fdt>:
81608948:	e3a00001 	mov	r0, #1
8160894c:	e12fff1e 	bx	lr

81608950 <image_setup_libfdt>:
81608950:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81608954:	e1a06000 	mov	r6, r0
81608958:	e1a00001 	mov	r0, r1
8160895c:	e1a04001 	mov	r4, r1
81608960:	e1a07003 	mov	r7, r3
81608964:	ebfff4ee 	bl	81605d24 <fdt_root>
81608968:	e3500000 	cmp	r0, #0
8160896c:	b59f00dc 	ldrlt	r0, [pc, #220]	; 81608a50 <image_setup_libfdt+0x100>
81608970:	ba000004 	blt	81608988 <image_setup_libfdt+0x38>
81608974:	e1a00004 	mov	r0, r4
81608978:	ebfff581 	bl	81605f84 <fdt_chosen>
8160897c:	e3500000 	cmp	r0, #0
81608980:	aa000005 	bge	8160899c <image_setup_libfdt+0x4c>
81608984:	e59f00c8 	ldr	r0, [pc, #200]	; 81608a54 <image_setup_libfdt+0x104>
81608988:	eb00273b 	bl	8161267c <printf>
8160898c:	e3e05000 	mvn	r5, #0
81608990:	e59f00c0 	ldr	r0, [pc, #192]	; 81608a58 <image_setup_libfdt+0x108>
81608994:	eb002738 	bl	8161267c <printf>
81608998:	ea00002a 	b	81608a48 <image_setup_libfdt+0xf8>
8160899c:	e1a00004 	mov	r0, r4
816089a0:	ebffe084 	bl	81600bb8 <arch_fixup_fdt>
816089a4:	e3500000 	cmp	r0, #0
816089a8:	b59f00ac 	ldrlt	r0, [pc, #172]	; 81608a5c <image_setup_libfdt+0x10c>
816089ac:	bafffff5 	blt	81608988 <image_setup_libfdt+0x38>
816089b0:	e1a00004 	mov	r0, r4
816089b4:	ebfff62f 	bl	81606278 <fdt_fixup_ethernet>
816089b8:	e3570000 	cmp	r7, #0
816089bc:	0a000004 	beq	816089d4 <image_setup_libfdt+0x84>
816089c0:	e5942004 	ldr	r2, [r4, #4]
816089c4:	e1a01004 	mov	r1, r4
816089c8:	e1a00007 	mov	r0, r7
816089cc:	e6bf2f32 	rev	r2, r2
816089d0:	eb0017ef 	bl	8160e994 <lmb_free>
816089d4:	e3a01000 	mov	r1, #0
816089d8:	e1a00004 	mov	r0, r4
816089dc:	ebfff693 	bl	81606430 <fdt_shrink_to_minimum>
816089e0:	e2505000 	subs	r5, r0, #0
816089e4:	baffffe9 	blt	81608990 <image_setup_libfdt+0x40>
816089e8:	e5963074 	ldr	r3, [r6, #116]	; 0x74
816089ec:	e3530000 	cmp	r3, #0
816089f0:	0a000005 	beq	81608a0c <image_setup_libfdt+0xbc>
816089f4:	e5963078 	ldr	r3, [r6, #120]	; 0x78
816089f8:	e3530000 	cmp	r3, #0
816089fc:	12852080 	addne	r2, r5, #128	; 0x80
81608a00:	16bf3f32 	revne	r3, r2
81608a04:	15843004 	strne	r3, [r4, #4]
81608a08:	1a000000 	bne	81608a10 <image_setup_libfdt+0xc0>
81608a0c:	e1a02005 	mov	r2, r5
81608a10:	e3570000 	cmp	r7, #0
81608a14:	0a000002 	beq	81608a24 <image_setup_libfdt+0xd4>
81608a18:	e1a01004 	mov	r1, r4
81608a1c:	e1a00007 	mov	r0, r7
81608a20:	eb001815 	bl	8160ea7c <lmb_reserve>
81608a24:	e5962078 	ldr	r2, [r6, #120]	; 0x78
81608a28:	e1a00004 	mov	r0, r4
81608a2c:	e5961074 	ldr	r1, [r6, #116]	; 0x74
81608a30:	ebfff4df 	bl	81605db4 <fdt_initrd>
81608a34:	e1a00004 	mov	r0, r4
81608a38:	ebffffc2 	bl	81608948 <ft_verify_fdt>
81608a3c:	e3500000 	cmp	r0, #0
81608a40:	13a05000 	movne	r5, #0
81608a44:	0affffd1 	beq	81608990 <image_setup_libfdt+0x40>
81608a48:	e1a00005 	mov	r0, r5
81608a4c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81608a50:	816161ad 	.word	0x816161ad
81608a54:	816161cc 	.word	0x816161cc
81608a58:	81616216 	.word	0x81616216
81608a5c:	816161ef 	.word	0x816161ef

81608a60 <get_ram_size>:
81608a60:	e92d4030 	push	{r4, r5, lr}
81608a64:	e1a0e000 	mov	lr, r0
81608a68:	e24dd084 	sub	sp, sp, #132	; 0x84
81608a6c:	e1a04121 	lsr	r4, r1, #2
81608a70:	e1a001a1 	lsr	r0, r1, #3
81608a74:	e3a03000 	mov	r3, #0
81608a78:	e3500000 	cmp	r0, #0
81608a7c:	e1a05003 	mov	r5, r3
81608a80:	1a00000f 	bne	81608ac4 <get_ram_size+0x64>
81608a84:	e59ec000 	ldr	ip, [lr]
81608a88:	e28d2080 	add	r2, sp, #128	; 0x80
81608a8c:	e0822103 	add	r2, r2, r3, lsl #2
81608a90:	e58e0000 	str	r0, [lr]
81608a94:	e502c080 	str	ip, [r2, #-128]	; 0xffffff80
81608a98:	e59e2000 	ldr	r2, [lr]
81608a9c:	e3520000 	cmp	r2, #0
81608aa0:	158ec000 	strne	ip, [lr]
81608aa4:	108d3103 	addne	r3, sp, r3, lsl #2
81608aa8:	13a02001 	movne	r2, #1
81608aac:	1a00000e 	bne	81608aec <get_ram_size+0x8c>
81608ab0:	e3a03001 	mov	r3, #1
81608ab4:	e1540003 	cmp	r4, r3
81608ab8:	8a00000f 	bhi	81608afc <get_ram_size+0x9c>
81608abc:	e1a00001 	mov	r0, r1
81608ac0:	ea00000b 	b	81608af4 <get_ram_size+0x94>
81608ac4:	e79e2100 	ldr	r2, [lr, r0, lsl #2]
81608ac8:	e78d2103 	str	r2, [sp, r3, lsl #2]
81608acc:	e1e02000 	mvn	r2, r0
81608ad0:	e2833001 	add	r3, r3, #1
81608ad4:	e78e2100 	str	r2, [lr, r0, lsl #2]
81608ad8:	e1a000c0 	asr	r0, r0, #1
81608adc:	eaffffe5 	b	81608a78 <get_ram_size+0x18>
81608ae0:	e5331004 	ldr	r1, [r3, #-4]!
81608ae4:	e78e1102 	str	r1, [lr, r2, lsl #2]
81608ae8:	e1a02082 	lsl	r2, r2, #1
81608aec:	e1540002 	cmp	r4, r2
81608af0:	8afffffa 	bhi	81608ae0 <get_ram_size+0x80>
81608af4:	e28dd084 	add	sp, sp, #132	; 0x84
81608af8:	e8bd8030 	pop	{r4, r5, pc}
81608afc:	e2455001 	sub	r5, r5, #1
81608b00:	e79e0103 	ldr	r0, [lr, r3, lsl #2]
81608b04:	e1a0c103 	lsl	ip, r3, #2
81608b08:	e79d2105 	ldr	r2, [sp, r5, lsl #2]
81608b0c:	e78e2103 	str	r2, [lr, r3, lsl #2]
81608b10:	e1e02003 	mvn	r2, r3
81608b14:	e1520000 	cmp	r2, r0
81608b18:	e1a03083 	lsl	r3, r3, #1
81608b1c:	0affffe4 	beq	81608ab4 <get_ram_size+0x54>
81608b20:	e08d2105 	add	r2, sp, r5, lsl #2
81608b24:	e1a0000c 	mov	r0, ip
81608b28:	e1540003 	cmp	r4, r3
81608b2c:	9afffff0 	bls	81608af4 <get_ram_size+0x94>
81608b30:	e5321004 	ldr	r1, [r2, #-4]!
81608b34:	e78e1103 	str	r1, [lr, r3, lsl #2]
81608b38:	e1a03083 	lsl	r3, r3, #1
81608b3c:	eafffff9 	b	81608b28 <get_ram_size+0xc8>

81608b40 <get_effective_memsize>:
81608b40:	e5990030 	ldr	r0, [r9, #48]	; 0x30
81608b44:	e12fff1e 	bx	lr

81608b48 <stdio_serial_tstc>:
81608b48:	ea0009a7 	b	8160b1ec <serial_tstc>

81608b4c <stdio_serial_getc>:
81608b4c:	ea0009a1 	b	8160b1d8 <serial_getc>

81608b50 <stdio_serial_puts>:
81608b50:	e1a00001 	mov	r0, r1
81608b54:	ea0009b0 	b	8160b21c <serial_puts>

81608b58 <stdio_serial_putc>:
81608b58:	e1a00001 	mov	r0, r1
81608b5c:	ea0009a7 	b	8160b200 <serial_putc>

81608b60 <stdio_get_list>:
81608b60:	e59f0000 	ldr	r0, [pc]	; 81608b68 <stdio_get_list+0x8>
81608b64:	e12fff1e 	bx	lr
81608b68:	81619b40 	.word	0x81619b40

81608b6c <stdio_get_by_name>:
81608b6c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81608b70:	e2507000 	subs	r7, r0, #0
81608b74:	159f6038 	ldrne	r6, [pc, #56]	; 81608bb4 <stdio_get_by_name+0x48>
81608b78:	15b64044 	ldrne	r4, [r6, #68]!	; 0x44
81608b7c:	1a000002 	bne	81608b8c <stdio_get_by_name+0x20>
81608b80:	e3a00000 	mov	r0, #0
81608b84:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81608b88:	e5944000 	ldr	r4, [r4]
81608b8c:	e1540006 	cmp	r4, r6
81608b90:	0afffffa 	beq	81608b80 <stdio_get_by_name+0x14>
81608b94:	e1a01007 	mov	r1, r7
81608b98:	e244003c 	sub	r0, r4, #60	; 0x3c
81608b9c:	eb002327 	bl	81611840 <strcmp>
81608ba0:	e2445044 	sub	r5, r4, #68	; 0x44
81608ba4:	e3500000 	cmp	r0, #0
81608ba8:	1afffff6 	bne	81608b88 <stdio_get_by_name+0x1c>
81608bac:	e1a00005 	mov	r0, r5
81608bb0:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81608bb4:	81619afc 	.word	0x81619afc

81608bb8 <stdio_clone>:
81608bb8:	e92d4070 	push	{r4, r5, r6, lr}
81608bbc:	e2504000 	subs	r4, r0, #0
81608bc0:	0a000008 	beq	81608be8 <stdio_clone+0x30>
81608bc4:	e3a0104c 	mov	r1, #76	; 0x4c
81608bc8:	e3a00001 	mov	r0, #1
81608bcc:	ebfffba5 	bl	81607a68 <calloc>
81608bd0:	e1a05004 	mov	r5, r4
81608bd4:	e2504000 	subs	r4, r0, #0
81608bd8:	0a000002 	beq	81608be8 <stdio_clone+0x30>
81608bdc:	e3a0204c 	mov	r2, #76	; 0x4c
81608be0:	e1a01005 	mov	r1, r5
81608be4:	ebffdeec 	bl	8160079c <memcpy>
81608be8:	e1a00004 	mov	r0, r4
81608bec:	e8bd8070 	pop	{r4, r5, r6, pc}

81608bf0 <stdio_register_dev>:
81608bf0:	e92d4010 	push	{r4, lr}
81608bf4:	e1a04001 	mov	r4, r1
81608bf8:	ebffffee 	bl	81608bb8 <stdio_clone>
81608bfc:	e3500000 	cmp	r0, #0
81608c00:	0a00000b 	beq	81608c34 <stdio_register_dev+0x44>
81608c04:	e59f3030 	ldr	r3, [pc, #48]	; 81608c3c <stdio_register_dev+0x4c>
81608c08:	e2802044 	add	r2, r0, #68	; 0x44
81608c0c:	e3540000 	cmp	r4, #0
81608c10:	e2833044 	add	r3, r3, #68	; 0x44
81608c14:	e5931004 	ldr	r1, [r3, #4]
81608c18:	e5832004 	str	r2, [r3, #4]
81608c1c:	e5803044 	str	r3, [r0, #68]	; 0x44
81608c20:	e5801048 	str	r1, [r0, #72]	; 0x48
81608c24:	e5812000 	str	r2, [r1]
81608c28:	15840000 	strne	r0, [r4]
81608c2c:	e3a00000 	mov	r0, #0
81608c30:	e8bd8010 	pop	{r4, pc}
81608c34:	e3e00012 	mvn	r0, #18
81608c38:	e8bd8010 	pop	{r4, pc}
81608c3c:	81619afc 	.word	0x81619afc

81608c40 <stdio_register>:
81608c40:	e3a01000 	mov	r1, #0
81608c44:	eaffffe9 	b	81608bf0 <stdio_register_dev>

81608c48 <stdio_init_tables>:
81608c48:	e59f3010 	ldr	r3, [pc, #16]	; 81608c60 <stdio_init_tables+0x18>
81608c4c:	e3a00000 	mov	r0, #0
81608c50:	e2832044 	add	r2, r3, #68	; 0x44
81608c54:	e5832044 	str	r2, [r3, #68]	; 0x44
81608c58:	e5832048 	str	r2, [r3, #72]	; 0x48
81608c5c:	e12fff1e 	bx	lr
81608c60:	81619afc 	.word	0x81619afc

81608c64 <stdio_add_devices>:
81608c64:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
81608c68:	e24dd054 	sub	sp, sp, #84	; 0x54
81608c6c:	e3a0204c 	mov	r2, #76	; 0x4c
81608c70:	e3a01000 	mov	r1, #0
81608c74:	e28d0004 	add	r0, sp, #4
81608c78:	ebffde90 	bl	816006c0 <memset>
81608c7c:	e59f1044 	ldr	r1, [pc, #68]	; 81608cc8 <stdio_add_devices+0x64>
81608c80:	e28d000c 	add	r0, sp, #12
81608c84:	eb0022de 	bl	81611804 <strcpy>
81608c88:	e3a03003 	mov	r3, #3
81608c8c:	e58d3004 	str	r3, [sp, #4]
81608c90:	e28d0004 	add	r0, sp, #4
81608c94:	e59f3030 	ldr	r3, [pc, #48]	; 81608ccc <stdio_add_devices+0x68>
81608c98:	e58d3034 	str	r3, [sp, #52]	; 0x34
81608c9c:	e59f302c 	ldr	r3, [pc, #44]	; 81608cd0 <stdio_add_devices+0x6c>
81608ca0:	e58d3038 	str	r3, [sp, #56]	; 0x38
81608ca4:	e59f3028 	ldr	r3, [pc, #40]	; 81608cd4 <stdio_add_devices+0x70>
81608ca8:	e58d3040 	str	r3, [sp, #64]	; 0x40
81608cac:	e59f3024 	ldr	r3, [pc, #36]	; 81608cd8 <stdio_add_devices+0x74>
81608cb0:	e58d303c 	str	r3, [sp, #60]	; 0x3c
81608cb4:	ebffffe1 	bl	81608c40 <stdio_register>
81608cb8:	eb000891 	bl	8160af04 <serial_stdio_init>
81608cbc:	e3a00000 	mov	r0, #0
81608cc0:	e28dd054 	add	sp, sp, #84	; 0x54
81608cc4:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
81608cc8:	816168e3 	.word	0x816168e3
81608ccc:	81608b58 	.word	0x81608b58
81608cd0:	81608b50 	.word	0x81608b50
81608cd4:	81608b4c 	.word	0x81608b4c
81608cd8:	81608b48 	.word	0x81608b48

81608cdc <run_command>:
81608cdc:	e92d4010 	push	{r4, lr}
81608ce0:	ebfff768 	bl	81606a88 <cli_simple_run_command>
81608ce4:	e3700001 	cmn	r0, #1
81608ce8:	13a00000 	movne	r0, #0
81608cec:	03a00001 	moveq	r0, #1
81608cf0:	e8bd8010 	pop	{r4, pc}

81608cf4 <run_command_list>:
81608cf4:	e3710001 	cmn	r1, #1
81608cf8:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81608cfc:	e1a05000 	mov	r5, r0
81608d00:	e1a07002 	mov	r7, r2
81608d04:	11a04001 	movne	r4, r1
81608d08:	1a000006 	bne	81608d28 <run_command_list+0x34>
81608d0c:	eb0022f0 	bl	816118d4 <strlen>
81608d10:	e3a0100a 	mov	r1, #10
81608d14:	e1a04000 	mov	r4, r0
81608d18:	e1a00005 	mov	r0, r5
81608d1c:	eb0022e0 	bl	816118a4 <strchr>
81608d20:	e3500000 	cmp	r0, #0
81608d24:	0a000015 	beq	81608d80 <run_command_list+0x8c>
81608d28:	e2840001 	add	r0, r4, #1
81608d2c:	ebfff9cc 	bl	81607464 <malloc>
81608d30:	e2506000 	subs	r6, r0, #0
81608d34:	03a06001 	moveq	r6, #1
81608d38:	0a00000e 	beq	81608d78 <run_command_list+0x84>
81608d3c:	e1a02004 	mov	r2, r4
81608d40:	e1a01005 	mov	r1, r5
81608d44:	ebffde94 	bl	8160079c <memcpy>
81608d48:	e1a05006 	mov	r5, r6
81608d4c:	e3a03000 	mov	r3, #0
81608d50:	e7c63004 	strb	r3, [r6, r4]
81608d54:	e3a04001 	mov	r4, #1
81608d58:	e1a01007 	mov	r1, r7
81608d5c:	e1a00005 	mov	r0, r5
81608d60:	ebfff79c 	bl	81606bd8 <cli_simple_run_command_list>
81608d64:	e3540000 	cmp	r4, #0
81608d68:	e1a06000 	mov	r6, r0
81608d6c:	0a000001 	beq	81608d78 <run_command_list+0x84>
81608d70:	e1a00005 	mov	r0, r5
81608d74:	ebfff92f 	bl	81607238 <free>
81608d78:	e1a00006 	mov	r0, r6
81608d7c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81608d80:	e1a04000 	mov	r4, r0
81608d84:	eafffff3 	b	81608d58 <run_command_list+0x64>

81608d88 <do_run>:
81608d88:	e3520001 	cmp	r2, #1
81608d8c:	da000017 	ble	81608df0 <do_run+0x68>
81608d90:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81608d94:	e2834004 	add	r4, r3, #4
81608d98:	e3815004 	orr	r5, r1, #4
81608d9c:	e1a06002 	mov	r6, r2
81608da0:	e3a07001 	mov	r7, #1
81608da4:	e1a08004 	mov	r8, r4
81608da8:	e2844004 	add	r4, r4, #4
81608dac:	e5980000 	ldr	r0, [r8]
81608db0:	ebffedf8 	bl	81604598 <env_get>
81608db4:	e3500000 	cmp	r0, #0
81608db8:	1a000004 	bne	81608dd0 <do_run+0x48>
81608dbc:	e5981000 	ldr	r1, [r8]
81608dc0:	e59f0030 	ldr	r0, [pc, #48]	; 81608df8 <do_run+0x70>
81608dc4:	eb00262c 	bl	8161267c <printf>
81608dc8:	e3a00001 	mov	r0, #1
81608dcc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81608dd0:	e1a01005 	mov	r1, r5
81608dd4:	ebffffc0 	bl	81608cdc <run_command>
81608dd8:	e3500000 	cmp	r0, #0
81608ddc:	1afffff9 	bne	81608dc8 <do_run+0x40>
81608de0:	e2877001 	add	r7, r7, #1
81608de4:	e1560007 	cmp	r6, r7
81608de8:	1affffed 	bne	81608da4 <do_run+0x1c>
81608dec:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81608df0:	e3e00000 	mvn	r0, #0
81608df4:	e12fff1e 	bx	lr
81608df8:	816152c9 	.word	0x816152c9

81608dfc <cli_init>:
81608dfc:	e12fff1e 	bx	lr

81608e00 <find_cmd_tbl>:
81608e00:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81608e04:	e250a000 	subs	sl, r0, #0
81608e08:	1a000002 	bne	81608e18 <find_cmd_tbl+0x18>
81608e0c:	e3a06000 	mov	r6, #0
81608e10:	e1a00006 	mov	r0, r6
81608e14:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81608e18:	e1a04001 	mov	r4, r1
81608e1c:	e3a0102e 	mov	r1, #46	; 0x2e
81608e20:	e1a08002 	mov	r8, r2
81608e24:	eb00229e 	bl	816118a4 <strchr>
81608e28:	e3500000 	cmp	r0, #0
81608e2c:	1040500a 	subne	r5, r0, sl
81608e30:	1a000002 	bne	81608e40 <find_cmd_tbl+0x40>
81608e34:	e1a0000a 	mov	r0, sl
81608e38:	eb0022a5 	bl	816118d4 <strlen>
81608e3c:	e1a05000 	mov	r5, r0
81608e40:	e3a0301c 	mov	r3, #28
81608e44:	e1a06004 	mov	r6, r4
81608e48:	e0284893 	mla	r8, r3, r8, r4
81608e4c:	e3a07000 	mov	r7, #0
81608e50:	e1580004 	cmp	r8, r4
81608e54:	1a000002 	bne	81608e64 <find_cmd_tbl+0x64>
81608e58:	e3570001 	cmp	r7, #1
81608e5c:	0affffeb 	beq	81608e10 <find_cmd_tbl+0x10>
81608e60:	eaffffe9 	b	81608e0c <find_cmd_tbl+0xc>
81608e64:	e1a02005 	mov	r2, r5
81608e68:	e5941000 	ldr	r1, [r4]
81608e6c:	e1a0000a 	mov	r0, sl
81608e70:	eb00227c 	bl	81611868 <strncmp>
81608e74:	e3500000 	cmp	r0, #0
81608e78:	1a000005 	bne	81608e94 <find_cmd_tbl+0x94>
81608e7c:	e5940000 	ldr	r0, [r4]
81608e80:	eb002293 	bl	816118d4 <strlen>
81608e84:	e1550000 	cmp	r5, r0
81608e88:	0a000003 	beq	81608e9c <find_cmd_tbl+0x9c>
81608e8c:	e2877001 	add	r7, r7, #1
81608e90:	e1a06004 	mov	r6, r4
81608e94:	e284401c 	add	r4, r4, #28
81608e98:	eaffffec 	b	81608e50 <find_cmd_tbl+0x50>
81608e9c:	e1a06004 	mov	r6, r4
81608ea0:	eaffffda 	b	81608e10 <find_cmd_tbl+0x10>

81608ea4 <find_cmd>:
81608ea4:	e59f1014 	ldr	r1, [pc, #20]	; 81608ec0 <find_cmd+0x1c>
81608ea8:	e59f3014 	ldr	r3, [pc, #20]	; 81608ec4 <find_cmd+0x20>
81608eac:	e59f2014 	ldr	r2, [pc, #20]	; 81608ec8 <find_cmd+0x24>
81608eb0:	e0433001 	sub	r3, r3, r1
81608eb4:	e1a03143 	asr	r3, r3, #2
81608eb8:	e0020392 	mul	r2, r2, r3
81608ebc:	eaffffcf 	b	81608e00 <find_cmd_tbl>
81608ec0:	8161912c 	.word	0x8161912c
81608ec4:	81619420 	.word	0x81619420
81608ec8:	b6db6db7 	.word	0xb6db6db7

81608ecc <cmd_usage>:
81608ecc:	e92d4010 	push	{r4, lr}
81608ed0:	e1a04000 	mov	r4, r0
81608ed4:	e5902010 	ldr	r2, [r0, #16]
81608ed8:	e5901000 	ldr	r1, [r0]
81608edc:	e59f0038 	ldr	r0, [pc, #56]	; 81608f1c <cmd_usage+0x50>
81608ee0:	eb0025e5 	bl	8161267c <printf>
81608ee4:	e5941000 	ldr	r1, [r4]
81608ee8:	e59f0030 	ldr	r0, [pc, #48]	; 81608f20 <cmd_usage+0x54>
81608eec:	eb0025e2 	bl	8161267c <printf>
81608ef0:	e5940014 	ldr	r0, [r4, #20]
81608ef4:	e3500000 	cmp	r0, #0
81608ef8:	1a000003 	bne	81608f0c <cmd_usage+0x40>
81608efc:	e59f0020 	ldr	r0, [pc, #32]	; 81608f24 <cmd_usage+0x58>
81608f00:	ebfff7a3 	bl	81606d94 <puts>
81608f04:	e3a00001 	mov	r0, #1
81608f08:	e8bd8010 	pop	{r4, pc}
81608f0c:	ebfff7a0 	bl	81606d94 <puts>
81608f10:	e3a0000a 	mov	r0, #10
81608f14:	ebfff794 	bl	81606d6c <putc>
81608f18:	eafffff9 	b	81608f04 <cmd_usage+0x38>
81608f1c:	8161496a 	.word	0x8161496a
81608f20:	81614974 	.word	0x81614974
81608f24:	8161497f 	.word	0x8161497f

81608f28 <_do_help>:
81608f28:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81608f2c:	e28db01c 	add	fp, sp, #28
81608f30:	e24dd010 	sub	sp, sp, #16
81608f34:	e99b0088 	ldmib	fp, {r3, r7}
81608f38:	e1a05000 	mov	r5, r0
81608f3c:	e1a04001 	mov	r4, r1
81608f40:	e3530001 	cmp	r3, #1
81608f44:	1a000052 	bne	81609094 <_do_help+0x16c>
81608f48:	e1a03101 	lsl	r3, r1, #2
81608f4c:	e50bd020 	str	sp, [fp, #-32]	; 0xffffffe0
81608f50:	e283300a 	add	r3, r3, #10
81608f54:	e2416001 	sub	r6, r1, #1
81608f58:	e3c33007 	bic	r3, r3, #7
81608f5c:	e3a0101c 	mov	r1, #28
81608f60:	e04dd003 	sub	sp, sp, r3
81608f64:	e3a03000 	mov	r3, #0
81608f68:	e1a0800d 	mov	r8, sp
81608f6c:	e1530004 	cmp	r3, r4
81608f70:	e0225391 	mla	r2, r1, r3, r5
81608f74:	ba000021 	blt	81609000 <_do_help+0xd8>
81608f78:	e3560000 	cmp	r6, #0
81608f7c:	da000014 	ble	81608fd4 <_do_help+0xac>
81608f80:	e3a07000 	mov	r7, #0
81608f84:	e1a05008 	mov	r5, r8
81608f88:	e1a0a007 	mov	sl, r7
81608f8c:	e5953000 	ldr	r3, [r5]
81608f90:	e28aa001 	add	sl, sl, #1
81608f94:	e5b52004 	ldr	r2, [r5, #4]!
81608f98:	e5930000 	ldr	r0, [r3]
81608f9c:	e5921000 	ldr	r1, [r2]
81608fa0:	e50b2028 	str	r2, [fp, #-40]	; 0xffffffd8
81608fa4:	e50b3024 	str	r3, [fp, #-36]	; 0xffffffdc
81608fa8:	eb002224 	bl	81611840 <strcmp>
81608fac:	e3500000 	cmp	r0, #0
81608fb0:	c2877001 	addgt	r7, r7, #1
81608fb4:	c51b2028 	ldrgt	r2, [fp, #-40]	; 0xffffffd8
81608fb8:	c51b3024 	ldrgt	r3, [fp, #-36]	; 0xffffffdc
81608fbc:	c5052004 	strgt	r2, [r5, #-4]
81608fc0:	c5853000 	strgt	r3, [r5]
81608fc4:	e15a0006 	cmp	sl, r6
81608fc8:	1affffef 	bne	81608f8c <_do_help+0x64>
81608fcc:	e3570000 	cmp	r7, #0
81608fd0:	1a00000d 	bne	8160900c <_do_help+0xe4>
81608fd4:	e59f60c8 	ldr	r6, [pc, #200]	; 816090a4 <_do_help+0x17c>
81608fd8:	e3a05000 	mov	r5, #0
81608fdc:	e1550004 	cmp	r5, r4
81608fe0:	ba00000b 	blt	81609014 <_do_help+0xec>
81608fe4:	e3a03000 	mov	r3, #0
81608fe8:	e58b3004 	str	r3, [fp, #4]
81608fec:	e51bd020 	ldr	sp, [fp, #-32]	; 0xffffffe0
81608ff0:	e59b6004 	ldr	r6, [fp, #4]
81608ff4:	e1a00006 	mov	r0, r6
81608ff8:	e24bd01c 	sub	sp, fp, #28
81608ffc:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81609000:	e7882103 	str	r2, [r8, r3, lsl #2]
81609004:	e2833001 	add	r3, r3, #1
81609008:	eaffffd7 	b	81608f6c <_do_help+0x44>
8160900c:	e2466001 	sub	r6, r6, #1
81609010:	eaffffd8 	b	81608f78 <_do_help+0x50>
81609014:	e7987105 	ldr	r7, [r8, r5, lsl #2]
81609018:	e597a010 	ldr	sl, [r7, #16]
8160901c:	ebfff763 	bl	81606db0 <ctrlc>
81609020:	e3500000 	cmp	r0, #0
81609024:	1afffff0 	bne	81608fec <_do_help+0xc4>
81609028:	e35a0000 	cmp	sl, #0
8160902c:	0a000004 	beq	81609044 <_do_help+0x11c>
81609030:	e1a0300a 	mov	r3, sl
81609034:	e5972000 	ldr	r2, [r7]
81609038:	e3a01008 	mov	r1, #8
8160903c:	e1a00006 	mov	r0, r6
81609040:	eb00258d 	bl	8161267c <printf>
81609044:	e2855001 	add	r5, r5, #1
81609048:	eaffffe3 	b	81608fdc <_do_help+0xb4>
8160904c:	e1a02004 	mov	r2, r4
81609050:	e1a01005 	mov	r1, r5
81609054:	e797010a 	ldr	r0, [r7, sl, lsl #2]
81609058:	ebffff68 	bl	81608e00 <find_cmd_tbl>
8160905c:	e3500000 	cmp	r0, #0
81609060:	0a000006 	beq	81609080 <_do_help+0x158>
81609064:	ebffff98 	bl	81608ecc <cmd_usage>
81609068:	e1866000 	orr	r6, r6, r0
8160906c:	e28aa001 	add	sl, sl, #1
81609070:	e59b3004 	ldr	r3, [fp, #4]
81609074:	e15a0003 	cmp	sl, r3
81609078:	bafffff3 	blt	8160904c <_do_help+0x124>
8160907c:	eaffffdc 	b	81608ff4 <_do_help+0xcc>
81609080:	e797110a 	ldr	r1, [r7, sl, lsl #2]
81609084:	e1a00008 	mov	r0, r8
81609088:	eb00257b 	bl	8161267c <printf>
8160908c:	e3a06001 	mov	r6, #1
81609090:	eafffff5 	b	8160906c <_do_help+0x144>
81609094:	e3a06000 	mov	r6, #0
81609098:	e3a0a001 	mov	sl, #1
8160909c:	e59f8004 	ldr	r8, [pc, #4]	; 816090a8 <_do_help+0x180>
816090a0:	eafffff2 	b	81609070 <_do_help+0x148>
816090a4:	8161425d 	.word	0x8161425d
816090a8:	81614267 	.word	0x81614267

816090ac <var_complete>:
816090ac:	e1a0c002 	mov	ip, r2
816090b0:	e31c00df 	tst	ip, #223	; 0xdf
816090b4:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
816090b8:	e1a0e001 	mov	lr, r1
816090bc:	e1a01003 	mov	r1, r3
816090c0:	124c3009 	subne	r3, ip, #9
816090c4:	116f3f13 	clzne	r3, r3
816090c8:	03a03001 	moveq	r3, #1
816090cc:	e59d2004 	ldr	r2, [sp, #4]
816090d0:	11a032a3 	lsrne	r3, r3, #5
816090d4:	e3500001 	cmp	r0, #1
816090d8:	13a0c000 	movne	ip, #0
816090dc:	0203c001 	andeq	ip, r3, #1
816090e0:	e35c0000 	cmp	ip, #0
816090e4:	0a000005 	beq	81609100 <var_complete+0x54>
816090e8:	e59f303c 	ldr	r3, [pc, #60]	; 8160912c <var_complete+0x80>
816090ec:	e59f003c 	ldr	r0, [pc, #60]	; 81609130 <var_complete+0x84>
816090f0:	e58d3004 	str	r3, [sp, #4]
816090f4:	e3a03c02 	mov	r3, #512	; 0x200
816090f8:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
816090fc:	ea00092b 	b	8160b5b0 <env_complete>
81609100:	e2233001 	eor	r3, r3, #1
81609104:	e3500002 	cmp	r0, #2
81609108:	13a00000 	movne	r0, #0
8160910c:	02030001 	andeq	r0, r3, #1
81609110:	e3500000 	cmp	r0, #0
81609114:	049df004 	popeq	{pc}		; (ldreq pc, [sp], #4)
81609118:	e59f300c 	ldr	r3, [pc, #12]	; 8160912c <var_complete+0x80>
8160911c:	e59e0004 	ldr	r0, [lr, #4]
81609120:	e58d3004 	str	r3, [sp, #4]
81609124:	e3a03c02 	mov	r3, #512	; 0x200
81609128:	eafffff2 	b	816090f8 <var_complete+0x4c>
8160912c:	81619c54 	.word	0x81619c54
81609130:	8161623b 	.word	0x8161623b

81609134 <cmd_auto_complete>:
81609134:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81609138:	e24dd0b8 	sub	sp, sp, #184	; 0xb8
8160913c:	e1a0a001 	mov	sl, r1
81609140:	e59f1488 	ldr	r1, [pc, #1160]	; 816095d0 <cmd_auto_complete+0x49c>
81609144:	e58d3010 	str	r3, [sp, #16]
81609148:	e5923000 	ldr	r3, [r2]
8160914c:	e58d001c 	str	r0, [sp, #28]
81609150:	e58d200c 	str	r2, [sp, #12]
81609154:	e58d3014 	str	r3, [sp, #20]
81609158:	e59d3010 	ldr	r3, [sp, #16]
8160915c:	e5933000 	ldr	r3, [r3]
81609160:	e58d3018 	str	r3, [sp, #24]
81609164:	eb0021b5 	bl	81611840 <strcmp>
81609168:	e2506000 	subs	r6, r0, #0
8160916c:	0a000002 	beq	8160917c <cmd_auto_complete+0x48>
81609170:	e3a00000 	mov	r0, #0
81609174:	e28dd0b8 	add	sp, sp, #184	; 0xb8
81609178:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160917c:	e1a0000a 	mov	r0, sl
81609180:	eb0021d3 	bl	816118d4 <strlen>
81609184:	e1a0100a 	mov	r1, sl
81609188:	e2503000 	subs	r3, r0, #0
8160918c:	e58d3008 	str	r3, [sp, #8]
81609190:	c08a3003 	addgt	r3, sl, r3
81609194:	e59f0438 	ldr	r0, [pc, #1080]	; 816095d4 <cmd_auto_complete+0x4a0>
81609198:	c5536001 	ldrbgt	r6, [r3, #-1]
8160919c:	eb002198 	bl	81611804 <strcpy>
816091a0:	e59f142c 	ldr	r1, [pc, #1068]	; 816095d4 <cmd_auto_complete+0x4a0>
816091a4:	e28dc024 	add	ip, sp, #36	; 0x24
816091a8:	e3a03000 	mov	r3, #0
816091ac:	e1a05003 	mov	r5, r3
816091b0:	e1a02001 	mov	r2, r1
816091b4:	ea000000 	b	816091bc <cmd_auto_complete+0x88>
816091b8:	e1a02000 	mov	r2, r0
816091bc:	e1a00002 	mov	r0, r2
816091c0:	e4d01001 	ldrb	r1, [r0], #1
816091c4:	e3510009 	cmp	r1, #9
816091c8:	13510020 	cmpne	r1, #32
816091cc:	0afffff9 	beq	816091b8 <cmd_auto_complete+0x84>
816091d0:	e3510000 	cmp	r1, #0
816091d4:	0a00000f 	beq	81609218 <cmd_auto_complete+0xe4>
816091d8:	e2835001 	add	r5, r3, #1
816091dc:	e78c2103 	str	r2, [ip, r3, lsl #2]
816091e0:	e1a00002 	mov	r0, r2
816091e4:	e4d23001 	ldrb	r3, [r2], #1
816091e8:	e3530000 	cmp	r3, #0
816091ec:	0a000009 	beq	81609218 <cmd_auto_complete+0xe4>
816091f0:	e3530020 	cmp	r3, #32
816091f4:	13530009 	cmpne	r3, #9
816091f8:	e1a01002 	mov	r1, r2
816091fc:	13a03001 	movne	r3, #1
81609200:	03a03000 	moveq	r3, #0
81609204:	1afffff5 	bne	816091e0 <cmd_auto_complete+0xac>
81609208:	e3550010 	cmp	r5, #16
8160920c:	e5c03000 	strb	r3, [r0]
81609210:	e1a03005 	mov	r3, r5
81609214:	1affffe4 	bne	816091ac <cmd_auto_complete+0x78>
81609218:	e28d30b8 	add	r3, sp, #184	; 0xb8
8160921c:	e3a02000 	mov	r2, #0
81609220:	e0833105 	add	r3, r3, r5, lsl #2
81609224:	e1550002 	cmp	r5, r2
81609228:	e59f83a8 	ldr	r8, [pc, #936]	; 816095d8 <cmd_auto_complete+0x4a4>
8160922c:	e5032094 	str	r2, [r3, #-148]	; 0xffffff6c
81609230:	e58d2068 	str	r2, [sp, #104]	; 0x68
81609234:	0a00002c 	beq	816092ec <cmd_auto_complete+0x1b8>
81609238:	e31600df 	tst	r6, #223	; 0xdf
8160923c:	e59d7024 	ldr	r7, [sp, #36]	; 0x24
81609240:	03a03001 	moveq	r3, #1
81609244:	13a03000 	movne	r3, #0
81609248:	e3550001 	cmp	r5, #1
8160924c:	c3833001 	orrgt	r3, r3, #1
81609250:	e3530000 	cmp	r3, #0
81609254:	1a000001 	bne	81609260 <cmd_auto_complete+0x12c>
81609258:	e3560009 	cmp	r6, #9
8160925c:	1a000063 	bne	816093f0 <cmd_auto_complete+0x2bc>
81609260:	e1a00007 	mov	r0, r7
81609264:	ebffff0e 	bl	81608ea4 <find_cmd>
81609268:	e3500000 	cmp	r0, #0
8160926c:	0a000002 	beq	8160927c <cmd_auto_complete+0x148>
81609270:	e5904018 	ldr	r4, [r0, #24]
81609274:	e3540000 	cmp	r4, #0
81609278:	1a000020 	bne	81609300 <cmd_auto_complete+0x1cc>
8160927c:	e3a03000 	mov	r3, #0
81609280:	e58d3068 	str	r3, [sp, #104]	; 0x68
81609284:	e3550001 	cmp	r5, #1
81609288:	caffffb8 	bgt	81609170 <cmd_auto_complete+0x3c>
8160928c:	ea000012 	b	816092dc <cmd_auto_complete+0x1a8>
81609290:	e7922001 	ldr	r2, [r2, r1]
81609294:	e78e2103 	str	r2, [lr, r3, lsl #2]
81609298:	e2833001 	add	r3, r3, #1
8160929c:	e002039c 	mul	r2, ip, r3
816092a0:	e1a04003 	mov	r4, r3
816092a4:	e0810002 	add	r0, r1, r2
816092a8:	e1580000 	cmp	r8, r0
816092ac:	0a000004 	beq	816092c4 <cmd_auto_complete+0x190>
816092b0:	e3540012 	cmp	r4, #18
816092b4:	1afffff5 	bne	81609290 <cmd_auto_complete+0x15c>
816092b8:	e59f331c 	ldr	r3, [pc, #796]	; 816095dc <cmd_auto_complete+0x4a8>
816092bc:	e3a04013 	mov	r4, #19
816092c0:	e58d30b0 	str	r3, [sp, #176]	; 0xb0
816092c4:	e28d30b8 	add	r3, sp, #184	; 0xb8
816092c8:	e3a02000 	mov	r2, #0
816092cc:	e0833104 	add	r3, r3, r4, lsl #2
816092d0:	e1540002 	cmp	r4, r2
816092d4:	e5032050 	str	r2, [r3, #-80]	; 0xffffffb0
816092d8:	1a000012 	bne	81609328 <cmd_auto_complete+0x1f4>
816092dc:	e3a00007 	mov	r0, #7
816092e0:	ebfff6a1 	bl	81606d6c <putc>
816092e4:	e3a00001 	mov	r0, #1
816092e8:	eaffffa1 	b	81609174 <cmd_auto_complete+0x40>
816092ec:	e1a03005 	mov	r3, r5
816092f0:	e3a0c01c 	mov	ip, #28
816092f4:	e59f12e4 	ldr	r1, [pc, #740]	; 816095e0 <cmd_auto_complete+0x4ac>
816092f8:	e28de068 	add	lr, sp, #104	; 0x68
816092fc:	eaffffe6 	b	8160929c <cmd_auto_complete+0x168>
81609300:	e28d3068 	add	r3, sp, #104	; 0x68
81609304:	e1a02006 	mov	r2, r6
81609308:	e58d3000 	str	r3, [sp]
8160930c:	e28d1024 	add	r1, sp, #36	; 0x24
81609310:	e3a03014 	mov	r3, #20
81609314:	e1a00005 	mov	r0, r5
81609318:	e12fff34 	blx	r4
8160931c:	e1a04000 	mov	r4, r0
81609320:	e3540000 	cmp	r4, #0
81609324:	0affffd6 	beq	81609284 <cmd_auto_complete+0x150>
81609328:	e3540001 	cmp	r4, #1
8160932c:	1a00005a 	bne	8160949c <cmd_auto_complete+0x368>
81609330:	e28d30b8 	add	r3, sp, #184	; 0xb8
81609334:	e59f82a8 	ldr	r8, [pc, #680]	; 816095e4 <cmd_auto_complete+0x4b0>
81609338:	e0835105 	add	r5, r3, r5, lsl #2
8160933c:	e5150098 	ldr	r0, [r5, #-152]	; 0xffffff68
81609340:	eb002163 	bl	816118d4 <strlen>
81609344:	e59d7068 	ldr	r7, [sp, #104]	; 0x68
81609348:	e0877000 	add	r7, r7, r0
8160934c:	e1a00007 	mov	r0, r7
81609350:	eb00215f 	bl	816118d4 <strlen>
81609354:	e1a06000 	mov	r6, r0
81609358:	e3570000 	cmp	r7, #0
8160935c:	0a000077 	beq	81609540 <cmd_auto_complete+0x40c>
81609360:	e59d2014 	ldr	r2, [sp, #20]
81609364:	e0863004 	add	r3, r6, r4
81609368:	e0825003 	add	r5, r2, r3
8160936c:	e35500fd 	cmp	r5, #253	; 0xfd
81609370:	caffffd9 	bgt	816092dc <cmd_auto_complete+0x1a8>
81609374:	e59d2008 	ldr	r2, [sp, #8]
81609378:	e1a01007 	mov	r1, r7
8160937c:	e08a2002 	add	r2, sl, r2
81609380:	e2420001 	sub	r0, r2, #1
81609384:	e041c007 	sub	ip, r1, r7
81609388:	e156000c 	cmp	r6, ip
8160938c:	ca000068 	bgt	81609534 <cmd_auto_complete+0x400>
81609390:	e3580000 	cmp	r8, #0
81609394:	e1c66fc6 	bic	r6, r6, r6, asr #31
81609398:	12044001 	andne	r4, r4, #1
8160939c:	e0820006 	add	r0, r2, r6
816093a0:	03a04000 	moveq	r4, #0
816093a4:	e3540000 	cmp	r4, #0
816093a8:	12800001 	addne	r0, r0, #1
816093ac:	15d81000 	ldrbne	r1, [r8]
816093b0:	17c21006 	strbne	r1, [r2, r6]
816093b4:	e3a02000 	mov	r2, #0
816093b8:	e5c02000 	strb	r2, [r0]
816093bc:	e0400003 	sub	r0, r0, r3
816093c0:	e59d2018 	ldr	r2, [sp, #24]
816093c4:	e0824003 	add	r4, r2, r3
816093c8:	ebfff671 	bl	81606d94 <puts>
816093cc:	e3580000 	cmp	r8, #0
816093d0:	1a000001 	bne	816093dc <cmd_auto_complete+0x2a8>
816093d4:	e3a00007 	mov	r0, #7
816093d8:	ebfff663 	bl	81606d6c <putc>
816093dc:	e59d300c 	ldr	r3, [sp, #12]
816093e0:	e5835000 	str	r5, [r3]
816093e4:	e59d3010 	ldr	r3, [sp, #16]
816093e8:	e5834000 	str	r4, [r3]
816093ec:	eaffffbc 	b	816092e4 <cmd_auto_complete+0x1b0>
816093f0:	e3a0102e 	mov	r1, #46	; 0x2e
816093f4:	e1a00007 	mov	r0, r7
816093f8:	eb002129 	bl	816118a4 <strchr>
816093fc:	e3500000 	cmp	r0, #0
81609400:	1040b007 	subne	fp, r0, r7
81609404:	1a000002 	bne	81609414 <cmd_auto_complete+0x2e0>
81609408:	e1a00007 	mov	r0, r7
8160940c:	eb002130 	bl	816118d4 <strlen>
81609410:	e1a0b000 	mov	fp, r0
81609414:	e59f61cc 	ldr	r6, [pc, #460]	; 816095e8 <cmd_auto_complete+0x4b4>
81609418:	e3a04000 	mov	r4, #0
8160941c:	ea000004 	b	81609434 <cmd_auto_complete+0x300>
81609420:	e516201c 	ldr	r2, [r6, #-28]	; 0xffffffe4
81609424:	e28d1068 	add	r1, sp, #104	; 0x68
81609428:	e7812104 	str	r2, [r1, r4, lsl #2]
8160942c:	e1a04003 	mov	r4, r3
81609430:	e286601c 	add	r6, r6, #28
81609434:	e246301c 	sub	r3, r6, #28
81609438:	e1580003 	cmp	r8, r3
8160943c:	0a000011 	beq	81609488 <cmd_auto_complete+0x354>
81609440:	e516001c 	ldr	r0, [r6, #-28]	; 0xffffffe4
81609444:	eb002122 	bl	816118d4 <strlen>
81609448:	e150000b 	cmp	r0, fp
8160944c:	bafffff7 	blt	81609430 <cmd_auto_complete+0x2fc>
81609450:	e1a0200b 	mov	r2, fp
81609454:	e516101c 	ldr	r1, [r6, #-28]	; 0xffffffe4
81609458:	e1a00007 	mov	r0, r7
8160945c:	eb002163 	bl	816119f0 <memcmp>
81609460:	e3500000 	cmp	r0, #0
81609464:	1afffff1 	bne	81609430 <cmd_auto_complete+0x2fc>
81609468:	e3540011 	cmp	r4, #17
8160946c:	e28d3068 	add	r3, sp, #104	; 0x68
81609470:	e0832104 	add	r2, r3, r4, lsl #2
81609474:	e2843001 	add	r3, r4, #1
81609478:	daffffe8 	ble	81609420 <cmd_auto_complete+0x2ec>
8160947c:	e59f1158 	ldr	r1, [pc, #344]	; 816095dc <cmd_auto_complete+0x4a8>
81609480:	e1a04003 	mov	r4, r3
81609484:	e5821000 	str	r1, [r2]
81609488:	e28d30b8 	add	r3, sp, #184	; 0xb8
8160948c:	e3a02000 	mov	r2, #0
81609490:	e0833104 	add	r3, r3, r4, lsl #2
81609494:	e5032050 	str	r2, [r3, #-80]	; 0xffffffb0
81609498:	eaffffa0 	b	81609320 <cmd_auto_complete+0x1ec>
8160949c:	da000027 	ble	81609540 <cmd_auto_complete+0x40c>
816094a0:	e59d7068 	ldr	r7, [sp, #104]	; 0x68
816094a4:	e3570000 	cmp	r7, #0
816094a8:	0a000024 	beq	81609540 <cmd_auto_complete+0x40c>
816094ac:	e1a00007 	mov	r0, r7
816094b0:	eb002107 	bl	816118d4 <strlen>
816094b4:	e28d206c 	add	r2, sp, #108	; 0x6c
816094b8:	e1a06000 	mov	r6, r0
816094bc:	e4924004 	ldr	r4, [r2], #4
816094c0:	e3540000 	cmp	r4, #0
816094c4:	0a00000d 	beq	81609500 <cmd_auto_complete+0x3cc>
816094c8:	e2444001 	sub	r4, r4, #1
816094cc:	e1a03007 	mov	r3, r7
816094d0:	ea000005 	b	816094ec <cmd_auto_complete+0x3b8>
816094d4:	e5f4c001 	ldrb	ip, [r4, #1]!
816094d8:	e2831001 	add	r1, r3, #1
816094dc:	e5d30000 	ldrb	r0, [r3]
816094e0:	e15c0000 	cmp	ip, r0
816094e4:	1a000003 	bne	816094f8 <cmd_auto_complete+0x3c4>
816094e8:	e1a03001 	mov	r3, r1
816094ec:	e0431007 	sub	r1, r3, r7
816094f0:	e1510006 	cmp	r1, r6
816094f4:	bafffff6 	blt	816094d4 <cmd_auto_complete+0x3a0>
816094f8:	e0436007 	sub	r6, r3, r7
816094fc:	eaffffee 	b	816094bc <cmd_auto_complete+0x388>
81609500:	e3560000 	cmp	r6, #0
81609504:	0a00000d 	beq	81609540 <cmd_auto_complete+0x40c>
81609508:	e28d30b8 	add	r3, sp, #184	; 0xb8
8160950c:	e0835105 	add	r5, r3, r5, lsl #2
81609510:	e5150098 	ldr	r0, [r5, #-152]	; 0xffffff68
81609514:	eb0020ee 	bl	816118d4 <strlen>
81609518:	e0466000 	sub	r6, r6, r0
8160951c:	e3560000 	cmp	r6, #0
81609520:	da000006 	ble	81609540 <cmd_auto_complete+0x40c>
81609524:	e59d7068 	ldr	r7, [sp, #104]	; 0x68
81609528:	e1a08004 	mov	r8, r4
8160952c:	e0877000 	add	r7, r7, r0
81609530:	eaffff88 	b	81609358 <cmd_auto_complete+0x224>
81609534:	e4d1c001 	ldrb	ip, [r1], #1
81609538:	e5e0c001 	strb	ip, [r0, #1]!
8160953c:	eaffff90 	b	81609384 <cmd_auto_complete+0x250>
81609540:	e59f00a4 	ldr	r0, [pc, #164]	; 816095ec <cmd_auto_complete+0x4b8>
81609544:	e28d5064 	add	r5, sp, #100	; 0x64
81609548:	eb0020e1 	bl	816118d4 <strlen>
8160954c:	e59fb09c 	ldr	fp, [pc, #156]	; 816095f0 <cmd_auto_complete+0x4bc>
81609550:	e3a0404e 	mov	r4, #78	; 0x4e
81609554:	e1a08000 	mov	r8, r0
81609558:	e59f0084 	ldr	r0, [pc, #132]	; 816095e4 <cmd_auto_complete+0x4b0>
8160955c:	eb0020dc 	bl	816118d4 <strlen>
81609560:	e1a06000 	mov	r6, r0
81609564:	e5b50004 	ldr	r0, [r5, #4]!
81609568:	e3500000 	cmp	r0, #0
8160956c:	1a000006 	bne	8160958c <cmd_auto_complete+0x458>
81609570:	e59f0078 	ldr	r0, [pc, #120]	; 816095f0 <cmd_auto_complete+0x4bc>
81609574:	eb002440 	bl	8161267c <printf>
81609578:	e59d001c 	ldr	r0, [sp, #28]
8160957c:	ebfff604 	bl	81606d94 <puts>
81609580:	e1a0000a 	mov	r0, sl
81609584:	ebfff602 	bl	81606d94 <puts>
81609588:	eaffff55 	b	816092e4 <cmd_auto_complete+0x1b0>
8160958c:	eb0020d0 	bl	816118d4 <strlen>
81609590:	e0867000 	add	r7, r6, r0
81609594:	e0873004 	add	r3, r7, r4
81609598:	e353004d 	cmp	r3, #77	; 0x4d
8160959c:	da000008 	ble	816095c4 <cmd_auto_complete+0x490>
816095a0:	e1a0000b 	mov	r0, fp
816095a4:	e0484006 	sub	r4, r8, r6
816095a8:	ebfff5f9 	bl	81606d94 <puts>
816095ac:	e59f0038 	ldr	r0, [pc, #56]	; 816095ec <cmd_auto_complete+0x4b8>
816095b0:	ebfff5f7 	bl	81606d94 <puts>
816095b4:	e5950000 	ldr	r0, [r5]
816095b8:	e0844007 	add	r4, r4, r7
816095bc:	ebfff5f4 	bl	81606d94 <puts>
816095c0:	eaffffe7 	b	81609564 <cmd_auto_complete+0x430>
816095c4:	e59f0018 	ldr	r0, [pc, #24]	; 816095e4 <cmd_auto_complete+0x4b0>
816095c8:	ebfff5f1 	bl	81606d94 <puts>
816095cc:	eafffff8 	b	816095b4 <cmd_auto_complete+0x480>
816095d0:	81614940 	.word	0x81614940
816095d4:	81619b54 	.word	0x81619b54
816095d8:	81619420 	.word	0x81619420
816095dc:	816145f2 	.word	0x816145f2
816095e0:	8161912c 	.word	0x8161912c
816095e4:	81616122 	.word	0x81616122
816095e8:	81619148 	.word	0x81619148
816095ec:	81616121 	.word	0x81616121
816095f0:	8161623a 	.word	0x8161623a

816095f4 <cmd_get_data_size>:
816095f4:	e92d4070 	push	{r4, r5, r6, lr}
816095f8:	e1a04000 	mov	r4, r0
816095fc:	e1a05001 	mov	r5, r1
81609600:	eb0020b3 	bl	816118d4 <strlen>
81609604:	e3500002 	cmp	r0, #2
81609608:	da00000a 	ble	81609638 <cmd_get_data_size+0x44>
8160960c:	e0840000 	add	r0, r4, r0
81609610:	e5503002 	ldrb	r3, [r0, #-2]
81609614:	e353002e 	cmp	r3, #46	; 0x2e
81609618:	1a000006 	bne	81609638 <cmd_get_data_size+0x44>
8160961c:	e5503001 	ldrb	r3, [r0, #-1]
81609620:	e2433062 	sub	r3, r3, #98	; 0x62
81609624:	e6ef3073 	uxtb	r3, r3
81609628:	e3530015 	cmp	r3, #21
8160962c:	959f200c 	ldrls	r2, [pc, #12]	; 81609640 <cmd_get_data_size+0x4c>
81609630:	83e05000 	mvnhi	r5, #0
81609634:	919250d3 	ldrsbls	r5, [r2, r3]
81609638:	e1a00005 	mov	r0, r5
8160963c:	e8bd8070 	pop	{r4, r5, r6, pc}
81609640:	81614059 	.word	0x81614059

81609644 <cmd_process>:
81609644:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81609648:	e1a0b000 	mov	fp, r0
8160964c:	e5920000 	ldr	r0, [r2]
81609650:	e1a08001 	mov	r8, r1
81609654:	e1a06002 	mov	r6, r2
81609658:	e1a07003 	mov	r7, r3
8160965c:	e59d5020 	ldr	r5, [sp, #32]
81609660:	ebfffe0f 	bl	81608ea4 <find_cmd>
81609664:	e2504000 	subs	r4, r0, #0
81609668:	1a000005 	bne	81609684 <cmd_process+0x40>
8160966c:	e5961000 	ldr	r1, [r6]
81609670:	e3a06001 	mov	r6, #1
81609674:	e59f0080 	ldr	r0, [pc, #128]	; 816096fc <cmd_process+0xb8>
81609678:	eb0023ff 	bl	8161267c <printf>
8160967c:	e1a00006 	mov	r0, r6
81609680:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81609684:	e5943004 	ldr	r3, [r4, #4]
81609688:	e1530008 	cmp	r3, r8
8160968c:	aa000002 	bge	8160969c <cmd_process+0x58>
81609690:	e1a00004 	mov	r0, r4
81609694:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
81609698:	eafffe0b 	b	81608ecc <cmd_usage>
8160969c:	e3550000 	cmp	r5, #0
816096a0:	0a000002 	beq	816096b0 <cmd_process+0x6c>
816096a4:	e3a00000 	mov	r0, #0
816096a8:	eb002138 	bl	81611b90 <get_timer>
816096ac:	e5850000 	str	r0, [r5]
816096b0:	e1a03006 	mov	r3, r6
816096b4:	e594a00c 	ldr	sl, [r4, #12]
816096b8:	e1a02008 	mov	r2, r8
816096bc:	e1a0100b 	mov	r1, fp
816096c0:	e1a00004 	mov	r0, r4
816096c4:	e12fff3a 	blx	sl
816096c8:	e3550000 	cmp	r5, #0
816096cc:	e1a06000 	mov	r6, r0
816096d0:	0a000002 	beq	816096e0 <cmd_process+0x9c>
816096d4:	e5950000 	ldr	r0, [r5]
816096d8:	eb00212c 	bl	81611b90 <get_timer>
816096dc:	e5850000 	str	r0, [r5]
816096e0:	e5973000 	ldr	r3, [r7]
816096e4:	e3760001 	cmn	r6, #1
816096e8:	e5942008 	ldr	r2, [r4, #8]
816096ec:	e0033002 	and	r3, r3, r2
816096f0:	e5873000 	str	r3, [r7]
816096f4:	1affffe0 	bne	8160967c <cmd_process+0x38>
816096f8:	eaffffe4 	b	81609690 <cmd_process+0x4c>
816096fc:	81614947 	.word	0x81614947

81609700 <mmc_set_ios>:
81609700:	e5903008 	ldr	r3, [r0, #8]
81609704:	e5933004 	ldr	r3, [r3, #4]
81609708:	e5933004 	ldr	r3, [r3, #4]
8160970c:	e3530000 	cmp	r3, #0
81609710:	012fff1e 	bxeq	lr
81609714:	e12fff13 	bx	r3

81609718 <mmc_set_capacity>:
81609718:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
8160971c:	e3510007 	cmp	r1, #7
81609720:	979ff101 	ldrls	pc, [pc, r1, lsl #2]
81609724:	ea000025 	b	816097c0 <mmc_set_capacity+0xa8>
81609728:	81609748 	.word	0x81609748
8160972c:	81609798 	.word	0x81609798
81609730:	81609798 	.word	0x81609798
81609734:	816097a0 	.word	0x816097a0
81609738:	816097a8 	.word	0x816097a8
8160973c:	816097a8 	.word	0x816097a8
81609740:	816097a8 	.word	0x816097a8
81609744:	816097a8 	.word	0x816097a8
81609748:	e1c029d0 	ldrd	r2, [r0, #144]	; 0x90
8160974c:	e1a04000 	mov	r4, r0
81609750:	e1c028f8 	strd	r2, [r0, #136]	; 0x88
81609754:	e594308c 	ldr	r3, [r4, #140]	; 0x8c
81609758:	e3a07000 	mov	r7, #0
8160975c:	e5900088 	ldr	r0, [r0, #136]	; 0x88
81609760:	e5941068 	ldr	r1, [r4, #104]	; 0x68
81609764:	e1a06003 	mov	r6, r3
81609768:	e88d0009 	stm	sp, {r0, r3}
8160976c:	e1963007 	orrs	r3, r6, r7
81609770:	1a00000f 	bne	816097b4 <mmc_set_capacity+0x9c>
81609774:	eb0024e9 	bl	81612b20 <__aeabi_uidiv>
81609778:	e88d0081 	stm	sp, {r0, r7}
8160977c:	e1a00004 	mov	r0, r4
81609780:	e59d5000 	ldr	r5, [sp]
81609784:	eb00056a 	bl	8160ad34 <mmc_get_blk_desc>
81609788:	e5805010 	str	r5, [r0, #16]
8160978c:	e3a00000 	mov	r0, #0
81609790:	e28dd00c 	add	sp, sp, #12
81609794:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81609798:	e1c029d8 	ldrd	r2, [r0, #152]	; 0x98
8160979c:	eaffffea 	b	8160974c <mmc_set_capacity+0x34>
816097a0:	e1c02ad0 	ldrd	r2, [r0, #160]	; 0xa0
816097a4:	eaffffe8 	b	8160974c <mmc_set_capacity+0x34>
816097a8:	e0801181 	add	r1, r0, r1, lsl #3
816097ac:	e1c128d8 	ldrd	r2, [r1, #136]	; 0x88
816097b0:	eaffffe5 	b	8160974c <mmc_set_capacity+0x34>
816097b4:	e1a0000d 	mov	r0, sp
816097b8:	eb001fb7 	bl	8161169c <__div64_32>
816097bc:	eaffffee 	b	8160977c <mmc_set_capacity+0x64>
816097c0:	e3e00000 	mvn	r0, #0
816097c4:	eafffff1 	b	81609790 <mmc_set_capacity+0x78>

816097c8 <board_mmc_getcd>:
816097c8:	e3e00000 	mvn	r0, #0
816097cc:	e12fff1e 	bx	lr

816097d0 <mmc_send_cmd>:
816097d0:	e5903008 	ldr	r3, [r0, #8]
816097d4:	e5933004 	ldr	r3, [r3, #4]
816097d8:	e5933000 	ldr	r3, [r3]
816097dc:	e12fff13 	bx	r3

816097e0 <mmc_send_ext_csd>:
816097e0:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
816097e4:	e24dd034 	sub	sp, sp, #52	; 0x34
816097e8:	e3a02c02 	mov	r2, #512	; 0x200
816097ec:	e3a03008 	mov	r3, #8
816097f0:	e58d1004 	str	r1, [sp, #4]
816097f4:	e28d1014 	add	r1, sp, #20
816097f8:	e58d2010 	str	r2, [sp, #16]
816097fc:	e28d2004 	add	r2, sp, #4
81609800:	e1cd31b4 	strh	r3, [sp, #20]
81609804:	e3a03015 	mov	r3, #21
81609808:	e58d3018 	str	r3, [sp, #24]
8160980c:	e3a03000 	mov	r3, #0
81609810:	e58d301c 	str	r3, [sp, #28]
81609814:	e3a03001 	mov	r3, #1
81609818:	e58d300c 	str	r3, [sp, #12]
8160981c:	e58d3008 	str	r3, [sp, #8]
81609820:	ebffffea 	bl	816097d0 <mmc_send_cmd>
81609824:	e28dd034 	add	sp, sp, #52	; 0x34
81609828:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

8160982c <mmc_go_idle>:
8160982c:	e92d4010 	push	{r4, lr}
81609830:	e1a04000 	mov	r4, r0
81609834:	e24dd020 	sub	sp, sp, #32
81609838:	e3a00ffa 	mov	r0, #1000	; 0x3e8
8160983c:	eb0020da 	bl	81611bac <udelay>
81609840:	e3a02000 	mov	r2, #0
81609844:	e1a00004 	mov	r0, r4
81609848:	e28d1004 	add	r1, sp, #4
8160984c:	e1cd20b4 	strh	r2, [sp, #4]
81609850:	e58d200c 	str	r2, [sp, #12]
81609854:	e58d2008 	str	r2, [sp, #8]
81609858:	ebffffdc 	bl	816097d0 <mmc_send_cmd>
8160985c:	e2504000 	subs	r4, r0, #0
81609860:	1a000001 	bne	8160986c <mmc_go_idle+0x40>
81609864:	e3a00e7d 	mov	r0, #2000	; 0x7d0
81609868:	eb0020cf 	bl	81611bac <udelay>
8160986c:	e1a00004 	mov	r0, r4
81609870:	e28dd020 	add	sp, sp, #32
81609874:	e8bd8010 	pop	{r4, pc}

81609878 <mmc_send_op_cond_iter>:
81609878:	e92d4010 	push	{r4, lr}
8160987c:	e24dd020 	sub	sp, sp, #32
81609880:	e3a03001 	mov	r3, #1
81609884:	e1a04000 	mov	r4, r0
81609888:	e1cd30b4 	strh	r3, [sp, #4]
8160988c:	e58d3008 	str	r3, [sp, #8]
81609890:	e3a03000 	mov	r3, #0
81609894:	e1510003 	cmp	r1, r3
81609898:	e58d300c 	str	r3, [sp, #12]
8160989c:	0a000009 	beq	816098c8 <mmc_send_op_cond_iter+0x50>
816098a0:	e5901008 	ldr	r1, [r0, #8]
816098a4:	e5902028 	ldr	r2, [r0, #40]	; 0x28
816098a8:	e59f303c 	ldr	r3, [pc, #60]	; 816098ec <mmc_send_op_cond_iter+0x74>
816098ac:	e591100c 	ldr	r1, [r1, #12]
816098b0:	e0033002 	and	r3, r3, r2
816098b4:	e2022206 	and	r2, r2, #1610612736	; 0x60000000
816098b8:	e0033001 	and	r3, r3, r1
816098bc:	e3822101 	orr	r2, r2, #1073741824	; 0x40000000
816098c0:	e1833002 	orr	r3, r3, r2
816098c4:	e58d300c 	str	r3, [sp, #12]
816098c8:	e3a02000 	mov	r2, #0
816098cc:	e28d1004 	add	r1, sp, #4
816098d0:	e1a00004 	mov	r0, r4
816098d4:	ebffffbd 	bl	816097d0 <mmc_send_cmd>
816098d8:	e3500000 	cmp	r0, #0
816098dc:	059d3010 	ldreq	r3, [sp, #16]
816098e0:	05843028 	streq	r3, [r4, #40]	; 0x28
816098e4:	e28dd020 	add	sp, sp, #32
816098e8:	e8bd8010 	pop	{r4, pc}
816098ec:	007fff80 	.word	0x007fff80

816098f0 <mmc_send_status>:
816098f0:	e92d40f0 	push	{r4, r5, r6, r7, lr}
816098f4:	e24dd024 	sub	sp, sp, #36	; 0x24
816098f8:	e3a0300d 	mov	r3, #13
816098fc:	e59f70ac 	ldr	r7, [pc, #172]	; 816099b0 <mmc_send_status+0xc0>
81609900:	e1cd30b4 	strh	r3, [sp, #4]
81609904:	e3a03015 	mov	r3, #21
81609908:	e58d3008 	str	r3, [sp, #8]
8160990c:	e1a06000 	mov	r6, r0
81609910:	e1d035bc 	ldrh	r3, [r0, #92]	; 0x5c
81609914:	e1a04001 	mov	r4, r1
81609918:	e3a05005 	mov	r5, #5
8160991c:	e1a03803 	lsl	r3, r3, #16
81609920:	e58d300c 	str	r3, [sp, #12]
81609924:	e3a02000 	mov	r2, #0
81609928:	e28d1004 	add	r1, sp, #4
8160992c:	e1a00006 	mov	r0, r6
81609930:	ebffffa6 	bl	816097d0 <mmc_send_cmd>
81609934:	e3500000 	cmp	r0, #0
81609938:	1a00000c 	bne	81609970 <mmc_send_status+0x80>
8160993c:	e59d1010 	ldr	r1, [sp, #16]
81609940:	e3110c01 	tst	r1, #256	; 0x100
81609944:	0a000002 	beq	81609954 <mmc_send_status+0x64>
81609948:	e2013c1e 	and	r3, r1, #7680	; 0x1e00
8160994c:	e3530c0e 	cmp	r3, #3584	; 0xe00
81609950:	1a00000f 	bne	81609994 <mmc_send_status+0xa4>
81609954:	e1110007 	tst	r1, r7
81609958:	0a000006 	beq	81609978 <mmc_send_status+0x88>
8160995c:	e59f0050 	ldr	r0, [pc, #80]	; 816099b4 <mmc_send_status+0xc4>
81609960:	eb002345 	bl	8161267c <printf>
81609964:	e3e00045 	mvn	r0, #69	; 0x45
81609968:	e28dd024 	add	sp, sp, #36	; 0x24
8160996c:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81609970:	e2555001 	subs	r5, r5, #1
81609974:	4afffffb 	bmi	81609968 <mmc_send_status+0x78>
81609978:	e2444001 	sub	r4, r4, #1
8160997c:	e2843001 	add	r3, r4, #1
81609980:	e3530000 	cmp	r3, #0
81609984:	da000002 	ble	81609994 <mmc_send_status+0xa4>
81609988:	e3a00ffa 	mov	r0, #1000	; 0x3e8
8160998c:	eb002086 	bl	81611bac <udelay>
81609990:	eaffffe3 	b	81609924 <mmc_send_status+0x34>
81609994:	e3540000 	cmp	r4, #0
81609998:	c3a00000 	movgt	r0, #0
8160999c:	cafffff1 	bgt	81609968 <mmc_send_status+0x78>
816099a0:	e59f0010 	ldr	r0, [pc, #16]	; 816099b8 <mmc_send_status+0xc8>
816099a4:	eb002334 	bl	8161267c <printf>
816099a8:	e3e0006d 	mvn	r0, #109	; 0x6d
816099ac:	eaffffed 	b	81609968 <mmc_send_status+0x78>
816099b0:	fdf94080 	.word	0xfdf94080
816099b4:	81616498 	.word	0x81616498
816099b8:	816164ae 	.word	0x816164ae

816099bc <mmc_switch>:
816099bc:	e92d4030 	push	{r4, r5, lr}
816099c0:	e1a03403 	lsl	r3, r3, #8
816099c4:	e24dd024 	sub	sp, sp, #36	; 0x24
816099c8:	e3833403 	orr	r3, r3, #50331648	; 0x3000000
816099cc:	e1a05000 	mov	r5, r0
816099d0:	e3a04003 	mov	r4, #3
816099d4:	e3a01006 	mov	r1, #6
816099d8:	e1833802 	orr	r3, r3, r2, lsl #16
816099dc:	e1cd10b4 	strh	r1, [sp, #4]
816099e0:	e3a0101d 	mov	r1, #29
816099e4:	e58d300c 	str	r3, [sp, #12]
816099e8:	e58d1008 	str	r1, [sp, #8]
816099ec:	e3a02000 	mov	r2, #0
816099f0:	e28d1004 	add	r1, sp, #4
816099f4:	e1a00005 	mov	r0, r5
816099f8:	ebffff74 	bl	816097d0 <mmc_send_cmd>
816099fc:	e3500000 	cmp	r0, #0
81609a00:	1a000004 	bne	81609a18 <mmc_switch+0x5c>
81609a04:	e3a01ffa 	mov	r1, #1000	; 0x3e8
81609a08:	e1a00005 	mov	r0, r5
81609a0c:	ebffffb7 	bl	816098f0 <mmc_send_status>
81609a10:	e28dd024 	add	sp, sp, #36	; 0x24
81609a14:	e8bd8030 	pop	{r4, r5, pc}
81609a18:	e2544001 	subs	r4, r4, #1
81609a1c:	1afffff2 	bne	816099ec <mmc_switch+0x30>
81609a20:	eafffffa 	b	81609a10 <mmc_switch+0x54>

81609a24 <mmc_switch_part>:
81609a24:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81609a28:	e6ef7071 	uxtb	r7, r1
81609a2c:	e5d03061 	ldrb	r3, [r0, #97]	; 0x61
81609a30:	e2072007 	and	r2, r7, #7
81609a34:	e1a06001 	mov	r6, r1
81609a38:	e3a01001 	mov	r1, #1
81609a3c:	e3c33007 	bic	r3, r3, #7
81609a40:	e1a05000 	mov	r5, r0
81609a44:	e1833002 	orr	r3, r3, r2
81609a48:	e3a020b3 	mov	r2, #179	; 0xb3
81609a4c:	ebffffda 	bl	816099bc <mmc_switch>
81609a50:	e2504000 	subs	r4, r0, #0
81609a54:	0a000002 	beq	81609a64 <mmc_switch_part+0x40>
81609a58:	e3560000 	cmp	r6, #0
81609a5c:	03740013 	cmneq	r4, #19
81609a60:	1a000006 	bne	81609a80 <mmc_switch_part+0x5c>
81609a64:	e1a01006 	mov	r1, r6
81609a68:	e1a00005 	mov	r0, r5
81609a6c:	ebffff29 	bl	81609718 <mmc_set_capacity>
81609a70:	e1a04000 	mov	r4, r0
81609a74:	e1a00005 	mov	r0, r5
81609a78:	eb0004ad 	bl	8160ad34 <mmc_get_blk_desc>
81609a7c:	e5c0700b 	strb	r7, [r0, #11]
81609a80:	e1a00004 	mov	r0, r4
81609a84:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

81609a88 <mmc_getcd>:
81609a88:	e92d4010 	push	{r4, lr}
81609a8c:	e1a04000 	mov	r4, r0
81609a90:	ebffff4c 	bl	816097c8 <board_mmc_getcd>
81609a94:	e3500000 	cmp	r0, #0
81609a98:	a8bd8010 	popge	{r4, pc}
81609a9c:	e5943008 	ldr	r3, [r4, #8]
81609aa0:	e5933004 	ldr	r3, [r3, #4]
81609aa4:	e593300c 	ldr	r3, [r3, #12]
81609aa8:	e3530000 	cmp	r3, #0
81609aac:	0a000002 	beq	81609abc <mmc_getcd+0x34>
81609ab0:	e1a00004 	mov	r0, r4
81609ab4:	e8bd4010 	pop	{r4, lr}
81609ab8:	e12fff13 	bx	r3
81609abc:	e3a00001 	mov	r0, #1
81609ac0:	e8bd8010 	pop	{r4, pc}

81609ac4 <mmc_set_clock>:
81609ac4:	e590c008 	ldr	ip, [r0, #8]
81609ac8:	e59c3014 	ldr	r3, [ip, #20]
81609acc:	e1510003 	cmp	r1, r3
81609ad0:	21a01003 	movcs	r1, r3
81609ad4:	e59c3010 	ldr	r3, [ip, #16]
81609ad8:	e1530001 	cmp	r3, r1
81609adc:	25803020 	strcs	r3, [r0, #32]
81609ae0:	35801020 	strcc	r1, [r0, #32]
81609ae4:	eaffff05 	b	81609700 <mmc_set_ios>

81609ae8 <mmc_startup>:
81609ae8:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81609aec:	e24dde59 	sub	sp, sp, #1424	; 0x590
81609af0:	e28d503c 	add	r5, sp, #60	; 0x3c
81609af4:	e3a02000 	mov	r2, #0
81609af8:	e3a03002 	mov	r3, #2
81609afc:	e1a04000 	mov	r4, r0
81609b00:	e1a01005 	mov	r1, r5
81609b04:	e1cd33bc 	strh	r3, [sp, #60]	; 0x3c
81609b08:	e58d2044 	str	r2, [sp, #68]	; 0x44
81609b0c:	e3a03007 	mov	r3, #7
81609b10:	e58d3040 	str	r3, [sp, #64]	; 0x40
81609b14:	ebffff2d 	bl	816097d0 <mmc_send_cmd>
81609b18:	e2507000 	subs	r7, r0, #0
81609b1c:	1a0002ef 	bne	8160a6e0 <mmc_startup+0xbf8>
81609b20:	e3a02010 	mov	r2, #16
81609b24:	e28d1048 	add	r1, sp, #72	; 0x48
81609b28:	e284004c 	add	r0, r4, #76	; 0x4c
81609b2c:	ebffdb1a 	bl	8160079c <memcpy>
81609b30:	e3a03003 	mov	r3, #3
81609b34:	e1cd33bc 	strh	r3, [sp, #60]	; 0x3c
81609b38:	e1a02007 	mov	r2, r7
81609b3c:	e1d435bc 	ldrh	r3, [r4, #92]	; 0x5c
81609b40:	e1a01005 	mov	r1, r5
81609b44:	e1a00004 	mov	r0, r4
81609b48:	e1a03803 	lsl	r3, r3, #16
81609b4c:	e58d3044 	str	r3, [sp, #68]	; 0x44
81609b50:	e3a03015 	mov	r3, #21
81609b54:	e58d3040 	str	r3, [sp, #64]	; 0x40
81609b58:	ebffff1c 	bl	816097d0 <mmc_send_cmd>
81609b5c:	e2507000 	subs	r7, r0, #0
81609b60:	1a0002de 	bne	8160a6e0 <mmc_startup+0xbf8>
81609b64:	e594300c 	ldr	r3, [r4, #12]
81609b68:	e3a02000 	mov	r2, #0
81609b6c:	e28d103c 	add	r1, sp, #60	; 0x3c
81609b70:	e1a00004 	mov	r0, r4
81609b74:	e3530000 	cmp	r3, #0
81609b78:	b59d3048 	ldrlt	r3, [sp, #72]	; 0x48
81609b7c:	b1a03823 	lsrlt	r3, r3, #16
81609b80:	b1c435bc 	strhlt	r3, [r4, #92]	; 0x5c
81609b84:	e3a03009 	mov	r3, #9
81609b88:	e1cd33bc 	strh	r3, [sp, #60]	; 0x3c
81609b8c:	e3a03007 	mov	r3, #7
81609b90:	e58d3040 	str	r3, [sp, #64]	; 0x40
81609b94:	e1d435bc 	ldrh	r3, [r4, #92]	; 0x5c
81609b98:	e1a03803 	lsl	r3, r3, #16
81609b9c:	e58d3044 	str	r3, [sp, #68]	; 0x44
81609ba0:	ebffff0a 	bl	816097d0 <mmc_send_cmd>
81609ba4:	e2507000 	subs	r7, r0, #0
81609ba8:	1a0002cc 	bne	8160a6e0 <mmc_startup+0xbf8>
81609bac:	e594100c 	ldr	r1, [r4, #12]
81609bb0:	e59d3048 	ldr	r3, [sp, #72]	; 0x48
81609bb4:	e59de04c 	ldr	lr, [sp, #76]	; 0x4c
81609bb8:	e3510101 	cmp	r1, #1073741824	; 0x40000000
81609bbc:	e59dc050 	ldr	ip, [sp, #80]	; 0x50
81609bc0:	e59d2054 	ldr	r2, [sp, #84]	; 0x54
81609bc4:	e584303c 	str	r3, [r4, #60]	; 0x3c
81609bc8:	e584e040 	str	lr, [r4, #64]	; 0x40
81609bcc:	e584c044 	str	ip, [r4, #68]	; 0x44
81609bd0:	e5842048 	str	r2, [r4, #72]	; 0x48
81609bd4:	1a00000a 	bne	81609c04 <mmc_startup+0x11c>
81609bd8:	e7e31d53 	ubfx	r1, r3, #26, #4
81609bdc:	e2411001 	sub	r1, r1, #1
81609be0:	e3510003 	cmp	r1, #3
81609be4:	979ff101 	ldrls	pc, [pc, r1, lsl #2]
81609be8:	ea000003 	b	81609bfc <mmc_startup+0x114>
81609bec:	81609e2c 	.word	0x81609e2c
81609bf0:	81609e34 	.word	0x81609e34
81609bf4:	81609e3c 	.word	0x81609e3c
81609bf8:	81609e44 	.word	0x81609e44
81609bfc:	e59f1bd4 	ldr	r1, [pc, #3028]	; 8160a7d8 <mmc_startup+0xcf0>
81609c00:	e584100c 	str	r1, [r4, #12]
81609c04:	e59f0bd0 	ldr	r0, [pc, #3024]	; 8160a7dc <mmc_startup+0xcf4>
81609c08:	e7e311d3 	ubfx	r1, r3, #3, #4
81609c0c:	e2033007 	and	r3, r3, #7
81609c10:	e594500c 	ldr	r5, [r4, #12]
81609c14:	e7d01001 	ldrb	r1, [r0, r1]
81609c18:	e3550000 	cmp	r5, #0
81609c1c:	e59f0bbc 	ldr	r0, [pc, #3004]	; 8160a7e0 <mmc_startup+0xcf8>
81609c20:	a7e32b52 	ubfxge	r2, r2, #22, #4
81609c24:	e7903103 	ldr	r3, [r0, r3, lsl #2]
81609c28:	e7e0065e 	ubfx	r0, lr, #12, #1
81609c2c:	e5840030 	str	r0, [r4, #48]	; 0x30
81609c30:	e0030193 	mul	r3, r3, r1
81609c34:	e7e3185e 	ubfx	r1, lr, #16, #4
81609c38:	e5843064 	str	r3, [r4, #100]	; 0x64
81609c3c:	e3a03001 	mov	r3, #1
81609c40:	e1a01113 	lsl	r1, r3, r1
81609c44:	a1a03213 	lslge	r3, r3, r2
81609c48:	a584306c 	strge	r3, [r4, #108]	; 0x6c
81609c4c:	e5943018 	ldr	r3, [r4, #24]
81609c50:	e1a07fc1 	asr	r7, r1, #31
81609c54:	b584106c 	strlt	r1, [r4, #108]	; 0x6c
81609c58:	e3530000 	cmp	r3, #0
81609c5c:	e5841068 	str	r1, [r4, #104]	; 0x68
81609c60:	11a0280e 	lslne	r2, lr, #16
81609c64:	03002ffc 	movweq	r2, #4092	; 0xffc
81609c68:	1202283f 	andne	r2, r2, #4128768	; 0x3f0000
81609c6c:	0002210e 	andeq	r2, r2, lr, lsl #2
81609c70:	1182282c 	orrne	r2, r2, ip, lsr #16
81609c74:	01822f2c 	orreq	r2, r2, ip, lsr #30
81609c78:	13a0c008 	movne	ip, #8
81609c7c:	07e2c7dc 	ubfxeq	ip, ip, #15, #3
81609c80:	13a03000 	movne	r3, #0
81609c84:	03a03000 	moveq	r3, #0
81609c88:	e2922001 	adds	r2, r2, #1
81609c8c:	e2a33000 	adc	r3, r3, #0
81609c90:	e28ce002 	add	lr, ip, #2
81609c94:	e24cc01e 	sub	ip, ip, #30
81609c98:	e26e5020 	rsb	r5, lr, #32
81609c9c:	e1a03e13 	lsl	r3, r3, lr
81609ca0:	e3510c02 	cmp	r1, #512	; 0x200
81609ca4:	e1833c12 	orr	r3, r3, r2, lsl ip
81609ca8:	e1833532 	orr	r3, r3, r2, lsr r5
81609cac:	e1a02e12 	lsl	r2, r2, lr
81609cb0:	e00c0792 	mul	ip, r2, r7
81609cb4:	e02cc391 	mla	ip, r1, r3, ip
81609cb8:	e0832192 	umull	r2, r3, r2, r1
81609cbc:	e08c3003 	add	r3, ip, r3
81609cc0:	e1c429f0 	strd	r2, [r4, #144]	; 0x90
81609cc4:	e3a03000 	mov	r3, #0
81609cc8:	e3a02000 	mov	r2, #0
81609ccc:	e1c429f8 	strd	r2, [r4, #152]	; 0x98
81609cd0:	e1c42af0 	strd	r2, [r4, #160]	; 0xa0
81609cd4:	e1c42af8 	strd	r2, [r4, #168]	; 0xa8
81609cd8:	e1c42bf0 	strd	r2, [r4, #176]	; 0xb0
81609cdc:	e1c42bf8 	strd	r2, [r4, #184]	; 0xb8
81609ce0:	e1c42cf0 	strd	r2, [r4, #192]	; 0xc0
81609ce4:	c3a03c02 	movgt	r3, #512	; 0x200
81609ce8:	c5843068 	strgt	r3, [r4, #104]	; 0x68
81609cec:	e594306c 	ldr	r3, [r4, #108]	; 0x6c
81609cf0:	e3530c02 	cmp	r3, #512	; 0x200
81609cf4:	83a03c02 	movhi	r3, #512	; 0x200
81609cf8:	8584306c 	strhi	r3, [r4, #108]	; 0x6c
81609cfc:	e3500000 	cmp	r0, #0
81609d00:	0a00000f 	beq	81609d44 <mmc_startup+0x25c>
81609d04:	e594302c 	ldr	r3, [r4, #44]	; 0x2c
81609d08:	e3730001 	cmn	r3, #1
81609d0c:	0a00000c 	beq	81609d44 <mmc_startup+0x25c>
81609d10:	e3a02004 	mov	r2, #4
81609d14:	e28d103c 	add	r1, sp, #60	; 0x3c
81609d18:	e1cd23bc 	strh	r2, [sp, #60]	; 0x3c
81609d1c:	e1a00004 	mov	r0, r4
81609d20:	e3a02000 	mov	r2, #0
81609d24:	e1a03803 	lsl	r3, r3, #16
81609d28:	e58d2040 	str	r2, [sp, #64]	; 0x40
81609d2c:	e58d3044 	str	r3, [sp, #68]	; 0x44
81609d30:	ebfffea6 	bl	816097d0 <mmc_send_cmd>
81609d34:	e3500000 	cmp	r0, #0
81609d38:	0a000001 	beq	81609d44 <mmc_startup+0x25c>
81609d3c:	e59f0aa0 	ldr	r0, [pc, #2720]	; 8160a7e4 <mmc_startup+0xcfc>
81609d40:	eb00224d 	bl	8161267c <printf>
81609d44:	e3a03007 	mov	r3, #7
81609d48:	e1cd33bc 	strh	r3, [sp, #60]	; 0x3c
81609d4c:	e3a03015 	mov	r3, #21
81609d50:	e58d3040 	str	r3, [sp, #64]	; 0x40
81609d54:	e1d435bc 	ldrh	r3, [r4, #92]	; 0x5c
81609d58:	e3a02000 	mov	r2, #0
81609d5c:	e28d103c 	add	r1, sp, #60	; 0x3c
81609d60:	e1a00004 	mov	r0, r4
81609d64:	e1a03803 	lsl	r3, r3, #16
81609d68:	e58d3044 	str	r3, [sp, #68]	; 0x44
81609d6c:	ebfffe97 	bl	816097d0 <mmc_send_cmd>
81609d70:	e2507000 	subs	r7, r0, #0
81609d74:	1a000259 	bne	8160a6e0 <mmc_startup+0xbf8>
81609d78:	e3a03001 	mov	r3, #1
81609d7c:	e5843070 	str	r3, [r4, #112]	; 0x70
81609d80:	e3e03000 	mvn	r3, #0
81609d84:	e5c43061 	strb	r3, [r4, #97]	; 0x61
81609d88:	e594300c 	ldr	r3, [r4, #12]
81609d8c:	e28d5f53 	add	r5, sp, #332	; 0x14c
81609d90:	e59f2a50 	ldr	r2, [pc, #2640]	; 8160a7e8 <mmc_startup+0xd00>
81609d94:	e2855003 	add	r5, r5, #3
81609d98:	e28334bf 	add	r3, r3, #-1090519040	; 0xbf000000
81609d9c:	e3c5503f 	bic	r5, r5, #63	; 0x3f
81609da0:	e283373f 	add	r3, r3, #16515072	; 0xfc0000
81609da4:	e1530002 	cmp	r3, r2
81609da8:	8a0000ac 	bhi	8160a060 <mmc_startup+0x578>
81609dac:	e1a01005 	mov	r1, r5
81609db0:	e1a00004 	mov	r0, r4
81609db4:	ebfffe89 	bl	816097e0 <mmc_send_ext_csd>
81609db8:	e2507000 	subs	r7, r0, #0
81609dbc:	1a000247 	bne	8160a6e0 <mmc_startup+0xbf8>
81609dc0:	e5d520c0 	ldrb	r2, [r5, #192]	; 0xc0
81609dc4:	e3520001 	cmp	r2, #1
81609dc8:	9a00000b 	bls	81609dfc <mmc_startup+0x314>
81609dcc:	e59530d4 	ldr	r3, [r5, #212]	; 0xd4
81609dd0:	e1a01fc3 	asr	r1, r3, #31
81609dd4:	e1a01481 	lsl	r1, r1, #9
81609dd8:	e1811ba3 	orr	r1, r1, r3, lsr #23
81609ddc:	e1a03483 	lsl	r3, r3, #9
81609de0:	e1a07a21 	lsr	r7, r1, #20
81609de4:	e1a06a23 	lsr	r6, r3, #20
81609de8:	e3570000 	cmp	r7, #0
81609dec:	e1866601 	orr	r6, r6, r1, lsl #12
81609df0:	03560b02 	cmpeq	r6, #2048	; 0x800
81609df4:	85843090 	strhi	r3, [r4, #144]	; 0x90
81609df8:	85841094 	strhi	r1, [r4, #148]	; 0x94
81609dfc:	e2423001 	sub	r3, r2, #1
81609e00:	e3530007 	cmp	r3, #7
81609e04:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
81609e08:	ea000011 	b	81609e54 <mmc_startup+0x36c>
81609e0c:	81609e4c 	.word	0x81609e4c
81609e10:	8160a11c 	.word	0x8160a11c
81609e14:	8160a124 	.word	0x8160a124
81609e18:	81609e54 	.word	0x81609e54
81609e1c:	8160a12c 	.word	0x8160a12c
81609e20:	8160a134 	.word	0x8160a134
81609e24:	8160a13c 	.word	0x8160a13c
81609e28:	8160a144 	.word	0x8160a144
81609e2c:	e59f19b8 	ldr	r1, [pc, #2488]	; 8160a7ec <mmc_startup+0xd04>
81609e30:	eaffff72 	b	81609c00 <mmc_startup+0x118>
81609e34:	e59f19b4 	ldr	r1, [pc, #2484]	; 8160a7f0 <mmc_startup+0xd08>
81609e38:	eaffff70 	b	81609c00 <mmc_startup+0x118>
81609e3c:	e59f19b0 	ldr	r1, [pc, #2480]	; 8160a7f4 <mmc_startup+0xd0c>
81609e40:	eaffff6e 	b	81609c00 <mmc_startup+0x118>
81609e44:	e59f19ac 	ldr	r1, [pc, #2476]	; 8160a7f8 <mmc_startup+0xd10>
81609e48:	eaffff6c 	b	81609c00 <mmc_startup+0x118>
81609e4c:	e59f39a8 	ldr	r3, [pc, #2472]	; 8160a7fc <mmc_startup+0xd14>
81609e50:	e584300c 	str	r3, [r4, #12]
81609e54:	e5d530a0 	ldrb	r3, [r5, #160]	; 0xa0
81609e58:	e5d5609b 	ldrb	r6, [r5, #155]	; 0x9b
81609e5c:	e3130001 	tst	r3, #1
81609e60:	e5c4305e 	strb	r3, [r4, #94]	; 0x5e
81609e64:	e2068001 	and	r8, r6, #1
81609e68:	e5d520e2 	ldrb	r2, [r5, #226]	; 0xe2
81609e6c:	1a000001 	bne	81609e78 <mmc_startup+0x390>
81609e70:	e3520000 	cmp	r2, #0
81609e74:	0a000001 	beq	81609e80 <mmc_startup+0x398>
81609e78:	e5d510b3 	ldrb	r1, [r5, #179]	; 0xb3
81609e7c:	e5c41061 	strb	r1, [r4, #97]	; 0x61
81609e80:	e3580000 	cmp	r8, #0
81609e84:	0a000002 	beq	81609e94 <mmc_startup+0x3ac>
81609e88:	e3130002 	tst	r3, #2
81609e8c:	15d5309c 	ldrbne	r3, [r5, #156]	; 0x9c
81609e90:	15c4305f 	strbne	r3, [r4, #95]	; 0x5f
81609e94:	e1a02882 	lsl	r2, r2, #17
81609e98:	e285c091 	add	ip, r5, #145	; 0x91
81609e9c:	e28470a8 	add	r7, r4, #168	; 0xa8
81609ea0:	e3a0e000 	mov	lr, #0
81609ea4:	e1a03fc2 	asr	r3, r2, #31
81609ea8:	e1c429f8 	strd	r2, [r4, #152]	; 0x98
81609eac:	e5d520a8 	ldrb	r2, [r5, #168]	; 0xa8
81609eb0:	e1a02882 	lsl	r2, r2, #17
81609eb4:	e1a03fc2 	asr	r3, r2, #31
81609eb8:	e1c42af0 	strd	r2, [r4, #160]	; 0xa0
81609ebc:	e285309d 	add	r3, r5, #157	; 0x9d
81609ec0:	e58d3014 	str	r3, [sp, #20]
81609ec4:	e5dc2000 	ldrb	r2, [ip]
81609ec8:	e55c3001 	ldrb	r3, [ip, #-1]
81609ecc:	e1a02802 	lsl	r2, r2, #16
81609ed0:	e0822403 	add	r2, r2, r3, lsl #8
81609ed4:	e55c3002 	ldrb	r3, [ip, #-2]
81609ed8:	e0922003 	adds	r2, r2, r3
81609edc:	13a0e001 	movne	lr, #1
81609ee0:	e3580000 	cmp	r8, #0
81609ee4:	0a000009 	beq	81609f10 <mmc_startup+0x428>
81609ee8:	e5d530e0 	ldrb	r3, [r5, #224]	; 0xe0
81609eec:	e5d500dd 	ldrb	r0, [r5, #221]	; 0xdd
81609ef0:	e0c32293 	smull	r2, r3, r3, r2
81609ef4:	e08ba290 	umull	sl, fp, r0, r2
81609ef8:	e02bb390 	mla	fp, r0, r3, fp
81609efc:	e1a0398b 	lsl	r3, fp, #19
81609f00:	e18336aa 	orr	r3, r3, sl, lsr #13
81609f04:	e1a0a98a 	lsl	sl, sl, #19
81609f08:	e5873004 	str	r3, [r7, #4]
81609f0c:	e587a000 	str	sl, [r7]
81609f10:	e59d3014 	ldr	r3, [sp, #20]
81609f14:	e28cc003 	add	ip, ip, #3
81609f18:	e2877008 	add	r7, r7, #8
81609f1c:	e15c0003 	cmp	ip, r3
81609f20:	1affffe7 	bne	81609ec4 <mmc_startup+0x3dc>
81609f24:	e3580000 	cmp	r8, #0
81609f28:	0a000022 	beq	81609fb8 <mmc_startup+0x4d0>
81609f2c:	e5d5208d 	ldrb	r2, [r5, #141]	; 0x8d
81609f30:	e3a0e001 	mov	lr, #1
81609f34:	e5d5308e 	ldrb	r3, [r5, #142]	; 0x8e
81609f38:	e5d500dd 	ldrb	r0, [r5, #221]	; 0xdd
81609f3c:	e1a02402 	lsl	r2, r2, #8
81609f40:	e0822803 	add	r2, r2, r3, lsl #16
81609f44:	e5d5308c 	ldrb	r3, [r5, #140]	; 0x8c
81609f48:	e0823003 	add	r3, r2, r3
81609f4c:	e5d520e0 	ldrb	r2, [r5, #224]	; 0xe0
81609f50:	e0c32392 	smull	r2, r3, r2, r3
81609f54:	e08ba290 	umull	sl, fp, r0, r2
81609f58:	e02bb390 	mla	fp, r0, r3, fp
81609f5c:	e1a0398b 	lsl	r3, fp, #19
81609f60:	e18336aa 	orr	r3, r3, sl, lsr #13
81609f64:	e1a0a98a 	lsl	sl, sl, #19
81609f68:	e58430d4 	str	r3, [r4, #212]	; 0xd4
81609f6c:	e584a0d0 	str	sl, [r4, #208]	; 0xd0
81609f70:	e5d5308a 	ldrb	r3, [r5, #138]	; 0x8a
81609f74:	e5d5208b 	ldrb	r2, [r5, #139]	; 0x8b
81609f78:	e1a03803 	lsl	r3, r3, #16
81609f7c:	e0833c02 	add	r3, r3, r2, lsl #24
81609f80:	e5d52089 	ldrb	r2, [r5, #137]	; 0x89
81609f84:	e0833402 	add	r3, r3, r2, lsl #8
81609f88:	e5d52088 	ldrb	r2, [r5, #136]	; 0x88
81609f8c:	e0833002 	add	r3, r3, r2
81609f90:	e5942018 	ldr	r2, [r4, #24]
81609f94:	e1a01fc3 	asr	r1, r3, #31
81609f98:	e3520000 	cmp	r2, #0
81609f9c:	e1a00003 	mov	r0, r3
81609fa0:	e1c40cf8 	strd	r0, [r4, #200]	; 0xc8
81609fa4:	11a02481 	lslne	r2, r1, #9
81609fa8:	11822ba3 	orrne	r2, r2, r3, lsr #23
81609fac:	11a03483 	lslne	r3, r3, #9
81609fb0:	158420cc 	strne	r2, [r4, #204]	; 0xcc
81609fb4:	158430c8 	strne	r3, [r4, #200]	; 0xc8
81609fb8:	e5d530a0 	ldrb	r3, [r5, #160]	; 0xa0
81609fbc:	e3130001 	tst	r3, #1
81609fc0:	0a000002 	beq	81609fd0 <mmc_startup+0x4e8>
81609fc4:	e5d5309c 	ldrb	r3, [r5, #156]	; 0x9c
81609fc8:	e313001f 	tst	r3, #31
81609fcc:	1a000001 	bne	81609fd8 <mmc_startup+0x4f0>
81609fd0:	e35e0000 	cmp	lr, #0
81609fd4:	0a000008 	beq	81609ffc <mmc_startup+0x514>
81609fd8:	e3a03001 	mov	r3, #1
81609fdc:	e3a020af 	mov	r2, #175	; 0xaf
81609fe0:	e1a01003 	mov	r1, r3
81609fe4:	e1a00004 	mov	r0, r4
81609fe8:	ebfffe73 	bl	816099bc <mmc_switch>
81609fec:	e2507000 	subs	r7, r0, #0
81609ff0:	1a0001ba 	bne	8160a6e0 <mmc_startup+0xbf8>
81609ff4:	e3a03001 	mov	r3, #1
81609ff8:	e5c530af 	strb	r3, [r5, #175]	; 0xaf
81609ffc:	e5d530af 	ldrb	r3, [r5, #175]	; 0xaf
8160a000:	e5d520e0 	ldrb	r2, [r5, #224]	; 0xe0
8160a004:	e3130001 	tst	r3, #1
8160a008:	0a00004f 	beq	8160a14c <mmc_startup+0x664>
8160a00c:	e1a03502 	lsl	r3, r2, #10
8160a010:	e5843070 	str	r3, [r4, #112]	; 0x70
8160a014:	e5943018 	ldr	r3, [r4, #24]
8160a018:	e3530000 	cmp	r3, #0
8160a01c:	12066001 	andne	r6, r6, #1
8160a020:	03a06000 	moveq	r6, #0
8160a024:	e3560000 	cmp	r6, #0
8160a028:	0a000006 	beq	8160a048 <mmc_startup+0x560>
8160a02c:	e59530d4 	ldr	r3, [r5, #212]	; 0xd4
8160a030:	e1a01fc3 	asr	r1, r3, #31
8160a034:	e1a01481 	lsl	r1, r1, #9
8160a038:	e1811ba3 	orr	r1, r1, r3, lsr #23
8160a03c:	e1a03483 	lsl	r3, r3, #9
8160a040:	e5841094 	str	r1, [r4, #148]	; 0x94
8160a044:	e5843090 	str	r3, [r4, #144]	; 0x90
8160a048:	e5d530dd 	ldrb	r3, [r5, #221]	; 0xdd
8160a04c:	e1a02502 	lsl	r2, r2, #10
8160a050:	e0020293 	mul	r2, r3, r2
8160a054:	e5842074 	str	r2, [r4, #116]	; 0x74
8160a058:	e5d530a7 	ldrb	r3, [r5, #167]	; 0xa7
8160a05c:	e5c43060 	strb	r3, [r4, #96]	; 0x60
8160a060:	e1a00004 	mov	r0, r4
8160a064:	eb000332 	bl	8160ad34 <mmc_get_blk_desc>
8160a068:	e5d0100b 	ldrb	r1, [r0, #11]
8160a06c:	e1a00004 	mov	r0, r4
8160a070:	ebfffda8 	bl	81609718 <mmc_set_capacity>
8160a074:	e2507000 	subs	r7, r0, #0
8160a078:	1a000198 	bne	8160a6e0 <mmc_startup+0xbf8>
8160a07c:	e594300c 	ldr	r3, [r4, #12]
8160a080:	e3530000 	cmp	r3, #0
8160a084:	aa0000dc 	bge	8160a3fc <mmc_startup+0x914>
8160a088:	e3a03037 	mov	r3, #55	; 0x37
8160a08c:	e1cd35b8 	strh	r3, [sp, #88]	; 0x58
8160a090:	e1d435bc 	ldrh	r3, [r4, #92]	; 0x5c
8160a094:	e1a02007 	mov	r2, r7
8160a098:	e5847024 	str	r7, [r4, #36]	; 0x24
8160a09c:	e28d1058 	add	r1, sp, #88	; 0x58
8160a0a0:	e1a00004 	mov	r0, r4
8160a0a4:	e3a08015 	mov	r8, #21
8160a0a8:	e1a03803 	lsl	r3, r3, #16
8160a0ac:	e58d805c 	str	r8, [sp, #92]	; 0x5c
8160a0b0:	e58d3060 	str	r3, [sp, #96]	; 0x60
8160a0b4:	ebfffdc5 	bl	816097d0 <mmc_send_cmd>
8160a0b8:	e3500000 	cmp	r0, #0
8160a0bc:	1a000014 	bne	8160a114 <mmc_startup+0x62c>
8160a0c0:	e28d60cf 	add	r6, sp, #207	; 0xcf
8160a0c4:	e58d805c 	str	r8, [sp, #92]	; 0x5c
8160a0c8:	e3c6603f 	bic	r6, r6, #63	; 0x3f
8160a0cc:	e3a08004 	mov	r8, #4
8160a0d0:	e3a0a008 	mov	sl, #8
8160a0d4:	e3a03033 	mov	r3, #51	; 0x33
8160a0d8:	e58d7060 	str	r7, [sp, #96]	; 0x60
8160a0dc:	e1cd35b8 	strh	r3, [sp, #88]	; 0x58
8160a0e0:	e28d201c 	add	r2, sp, #28
8160a0e4:	e28d1058 	add	r1, sp, #88	; 0x58
8160a0e8:	e1a00004 	mov	r0, r4
8160a0ec:	e3a03001 	mov	r3, #1
8160a0f0:	e58d601c 	str	r6, [sp, #28]
8160a0f4:	e58da028 	str	sl, [sp, #40]	; 0x28
8160a0f8:	e58d3024 	str	r3, [sp, #36]	; 0x24
8160a0fc:	e58d3020 	str	r3, [sp, #32]
8160a100:	ebfffdb2 	bl	816097d0 <mmc_send_cmd>
8160a104:	e3500000 	cmp	r0, #0
8160a108:	0a000016 	beq	8160a168 <mmc_startup+0x680>
8160a10c:	e2588001 	subs	r8, r8, #1
8160a110:	1afffff2 	bne	8160a0e0 <mmc_startup+0x5f8>
8160a114:	e1a07000 	mov	r7, r0
8160a118:	ea000170 	b	8160a6e0 <mmc_startup+0xbf8>
8160a11c:	e59f36dc 	ldr	r3, [pc, #1756]	; 8160a800 <mmc_startup+0xd18>
8160a120:	eaffff4a 	b	81609e50 <mmc_startup+0x368>
8160a124:	e59f36d8 	ldr	r3, [pc, #1752]	; 8160a804 <mmc_startup+0xd1c>
8160a128:	eaffff48 	b	81609e50 <mmc_startup+0x368>
8160a12c:	e59f36d4 	ldr	r3, [pc, #1748]	; 8160a808 <mmc_startup+0xd20>
8160a130:	eaffff46 	b	81609e50 <mmc_startup+0x368>
8160a134:	e59f36d0 	ldr	r3, [pc, #1744]	; 8160a80c <mmc_startup+0xd24>
8160a138:	eaffff44 	b	81609e50 <mmc_startup+0x368>
8160a13c:	e59f36cc 	ldr	r3, [pc, #1740]	; 8160a810 <mmc_startup+0xd28>
8160a140:	eaffff42 	b	81609e50 <mmc_startup+0x368>
8160a144:	e59f36c8 	ldr	r3, [pc, #1736]	; 8160a814 <mmc_startup+0xd2c>
8160a148:	eaffff40 	b	81609e50 <mmc_startup+0x368>
8160a14c:	e5943044 	ldr	r3, [r4, #68]	; 0x44
8160a150:	e7e41553 	ubfx	r1, r3, #10, #5
8160a154:	e7e432d3 	ubfx	r3, r3, #5, #5
8160a158:	e2833001 	add	r3, r3, #1
8160a15c:	e0233391 	mla	r3, r1, r3, r3
8160a160:	e5843070 	str	r3, [r4, #112]	; 0x70
8160a164:	eaffffb7 	b	8160a048 <mmc_startup+0x560>
8160a168:	e5963000 	ldr	r3, [r6]
8160a16c:	e6bf3f33 	rev	r3, r3
8160a170:	e5843034 	str	r3, [r4, #52]	; 0x34
8160a174:	e5962004 	ldr	r2, [r6, #4]
8160a178:	e6bf2f32 	rev	r2, r2
8160a17c:	e5842038 	str	r2, [r4, #56]	; 0x38
8160a180:	e7e32c53 	ubfx	r2, r3, #24, #4
8160a184:	e3520001 	cmp	r2, #1
8160a188:	059f2688 	ldreq	r2, [pc, #1672]	; 8160a818 <mmc_startup+0xd30>
8160a18c:	0a000005 	beq	8160a1a8 <mmc_startup+0x6c0>
8160a190:	e3520002 	cmp	r2, #2
8160a194:	159f2680 	ldrne	r2, [pc, #1664]	; 8160a81c <mmc_startup+0xd34>
8160a198:	1a000002 	bne	8160a1a8 <mmc_startup+0x6c0>
8160a19c:	e3130902 	tst	r3, #32768	; 0x8000
8160a1a0:	059f2678 	ldreq	r2, [pc, #1656]	; 8160a820 <mmc_startup+0xd38>
8160a1a4:	159f2678 	ldrne	r2, [pc, #1656]	; 8160a824 <mmc_startup+0xd3c>
8160a1a8:	e3130701 	tst	r3, #262144	; 0x40000
8160a1ac:	e584200c 	str	r2, [r4, #12]
8160a1b0:	e594200c 	ldr	r2, [r4, #12]
8160a1b4:	15943024 	ldrne	r3, [r4, #36]	; 0x24
8160a1b8:	13833004 	orrne	r3, r3, #4
8160a1bc:	15843024 	strne	r3, [r4, #36]	; 0x24
8160a1c0:	e59f3654 	ldr	r3, [pc, #1620]	; 8160a81c <mmc_startup+0xd34>
8160a1c4:	e1520003 	cmp	r2, r3
8160a1c8:	1a00004b 	bne	8160a2fc <mmc_startup+0x814>
8160a1cc:	e5943008 	ldr	r3, [r4, #8]
8160a1d0:	e5942024 	ldr	r2, [r4, #36]	; 0x24
8160a1d4:	e5933008 	ldr	r3, [r3, #8]
8160a1d8:	e0033002 	and	r3, r3, r2
8160a1dc:	e594200c 	ldr	r2, [r4, #12]
8160a1e0:	e5843024 	str	r3, [r4, #36]	; 0x24
8160a1e4:	e3520000 	cmp	r2, #0
8160a1e8:	aa0000d4 	bge	8160a540 <mmc_startup+0xa58>
8160a1ec:	e3130004 	tst	r3, #4
8160a1f0:	0a00001c 	beq	8160a268 <mmc_startup+0x780>
8160a1f4:	e1d435bc 	ldrh	r3, [r4, #92]	; 0x5c
8160a1f8:	e28d503c 	add	r5, sp, #60	; 0x3c
8160a1fc:	e3a02037 	mov	r2, #55	; 0x37
8160a200:	e1a00004 	mov	r0, r4
8160a204:	e1cd23bc 	strh	r2, [sp, #60]	; 0x3c
8160a208:	e1a01005 	mov	r1, r5
8160a20c:	e3a02000 	mov	r2, #0
8160a210:	e1a03803 	lsl	r3, r3, #16
8160a214:	e3a06015 	mov	r6, #21
8160a218:	e58d3044 	str	r3, [sp, #68]	; 0x44
8160a21c:	e58d6040 	str	r6, [sp, #64]	; 0x40
8160a220:	ebfffd6a 	bl	816097d0 <mmc_send_cmd>
8160a224:	e2502000 	subs	r2, r0, #0
8160a228:	11a07002 	movne	r7, r2
8160a22c:	1a00012b 	bne	8160a6e0 <mmc_startup+0xbf8>
8160a230:	e1a01005 	mov	r1, r5
8160a234:	e1a00004 	mov	r0, r4
8160a238:	e3a03006 	mov	r3, #6
8160a23c:	e58d6040 	str	r6, [sp, #64]	; 0x40
8160a240:	e1cd33bc 	strh	r3, [sp, #60]	; 0x3c
8160a244:	e3a03002 	mov	r3, #2
8160a248:	e58d3044 	str	r3, [sp, #68]	; 0x44
8160a24c:	ebfffd5f 	bl	816097d0 <mmc_send_cmd>
8160a250:	e3500000 	cmp	r0, #0
8160a254:	1affffae 	bne	8160a114 <mmc_startup+0x62c>
8160a258:	e3a03004 	mov	r3, #4
8160a25c:	e1a00004 	mov	r0, r4
8160a260:	e584301c 	str	r3, [r4, #28]
8160a264:	ebfffd25 	bl	81609700 <mmc_set_ios>
8160a268:	e3a03037 	mov	r3, #55	; 0x37
8160a26c:	e1cd39b0 	strh	r3, [sp, #144]	; 0x90
8160a270:	e1d435bc 	ldrh	r3, [r4, #92]	; 0x5c
8160a274:	e3a02000 	mov	r2, #0
8160a278:	e28d1090 	add	r1, sp, #144	; 0x90
8160a27c:	e1a00004 	mov	r0, r4
8160a280:	e3a08015 	mov	r8, #21
8160a284:	e58d8094 	str	r8, [sp, #148]	; 0x94
8160a288:	e1a03803 	lsl	r3, r3, #16
8160a28c:	e58d3098 	str	r3, [sp, #152]	; 0x98
8160a290:	ebfffd4e 	bl	816097d0 <mmc_send_cmd>
8160a294:	e3500000 	cmp	r0, #0
8160a298:	1affff9d 	bne	8160a114 <mmc_startup+0x62c>
8160a29c:	e28d5fe3 	add	r5, sp, #908	; 0x38c
8160a2a0:	e58d8094 	str	r8, [sp, #148]	; 0x94
8160a2a4:	e2855003 	add	r5, r5, #3
8160a2a8:	e3a08004 	mov	r8, #4
8160a2ac:	e3c5503f 	bic	r5, r5, #63	; 0x3f
8160a2b0:	e3a0a040 	mov	sl, #64	; 0x40
8160a2b4:	e3a0300d 	mov	r3, #13
8160a2b8:	e58d0098 	str	r0, [sp, #152]	; 0x98
8160a2bc:	e1a06005 	mov	r6, r5
8160a2c0:	e1cd39b0 	strh	r3, [sp, #144]	; 0x90
8160a2c4:	e28d2074 	add	r2, sp, #116	; 0x74
8160a2c8:	e28d1090 	add	r1, sp, #144	; 0x90
8160a2cc:	e1a00004 	mov	r0, r4
8160a2d0:	e3a03001 	mov	r3, #1
8160a2d4:	e58d5074 	str	r5, [sp, #116]	; 0x74
8160a2d8:	e58da080 	str	sl, [sp, #128]	; 0x80
8160a2dc:	e58d307c 	str	r3, [sp, #124]	; 0x7c
8160a2e0:	e58d3078 	str	r3, [sp, #120]	; 0x78
8160a2e4:	ebfffd39 	bl	816097d0 <mmc_send_cmd>
8160a2e8:	e3500000 	cmp	r0, #0
8160a2ec:	0a00006a 	beq	8160a49c <mmc_startup+0x9b4>
8160a2f0:	e2588001 	subs	r8, r8, #1
8160a2f4:	1afffff2 	bne	8160a2c4 <mmc_startup+0x7dc>
8160a2f8:	eaffff85 	b	8160a114 <mmc_startup+0x62c>
8160a2fc:	e28d6fe3 	add	r6, sp, #908	; 0x38c
8160a300:	e3a08005 	mov	r8, #5
8160a304:	e2866003 	add	r6, r6, #3
8160a308:	e3c6603f 	bic	r6, r6, #63	; 0x3f
8160a30c:	e2588001 	subs	r8, r8, #1
8160a310:	0a000016 	beq	8160a370 <mmc_startup+0x888>
8160a314:	e3a03006 	mov	r3, #6
8160a318:	e1cd37b4 	strh	r3, [sp, #116]	; 0x74
8160a31c:	e3a03015 	mov	r3, #21
8160a320:	e58d3078 	str	r3, [sp, #120]	; 0x78
8160a324:	e59f34fc 	ldr	r3, [pc, #1276]	; 8160a828 <mmc_startup+0xd40>
8160a328:	e28d2e59 	add	r2, sp, #1424	; 0x590
8160a32c:	e28d1074 	add	r1, sp, #116	; 0x74
8160a330:	e1a00004 	mov	r0, r4
8160a334:	e58d602c 	str	r6, [sp, #44]	; 0x2c
8160a338:	e58d307c 	str	r3, [sp, #124]	; 0x7c
8160a33c:	e3a03040 	mov	r3, #64	; 0x40
8160a340:	e58d3038 	str	r3, [sp, #56]	; 0x38
8160a344:	e3a03001 	mov	r3, #1
8160a348:	e5223560 	str	r3, [r2, #-1376]!	; 0xfffffaa0
8160a34c:	e2422004 	sub	r2, r2, #4
8160a350:	e58d3034 	str	r3, [sp, #52]	; 0x34
8160a354:	ebfffd1d 	bl	816097d0 <mmc_send_cmd>
8160a358:	e3500000 	cmp	r0, #0
8160a35c:	1affff6c 	bne	8160a114 <mmc_startup+0x62c>
8160a360:	e596301c 	ldr	r3, [r6, #28]
8160a364:	e6bf3f33 	rev	r3, r3
8160a368:	e3130802 	tst	r3, #131072	; 0x20000
8160a36c:	1affffe6 	bne	8160a30c <mmc_startup+0x824>
8160a370:	e596300c 	ldr	r3, [r6, #12]
8160a374:	e6bf3f33 	rev	r3, r3
8160a378:	e3130802 	tst	r3, #131072	; 0x20000
8160a37c:	0affff92 	beq	8160a1cc <mmc_startup+0x6e4>
8160a380:	e5943008 	ldr	r3, [r4, #8]
8160a384:	e5933008 	ldr	r3, [r3, #8]
8160a388:	e2033003 	and	r3, r3, #3
8160a38c:	e3530003 	cmp	r3, #3
8160a390:	1affff8d 	bne	8160a1cc <mmc_startup+0x6e4>
8160a394:	e3a03006 	mov	r3, #6
8160a398:	e1cd37b4 	strh	r3, [sp, #116]	; 0x74
8160a39c:	e3a03015 	mov	r3, #21
8160a3a0:	e58d3078 	str	r3, [sp, #120]	; 0x78
8160a3a4:	e59f3480 	ldr	r3, [pc, #1152]	; 8160a82c <mmc_startup+0xd44>
8160a3a8:	e28d2e59 	add	r2, sp, #1424	; 0x590
8160a3ac:	e28d1074 	add	r1, sp, #116	; 0x74
8160a3b0:	e1a00004 	mov	r0, r4
8160a3b4:	e58d602c 	str	r6, [sp, #44]	; 0x2c
8160a3b8:	e58d307c 	str	r3, [sp, #124]	; 0x7c
8160a3bc:	e3a03040 	mov	r3, #64	; 0x40
8160a3c0:	e58d3038 	str	r3, [sp, #56]	; 0x38
8160a3c4:	e3a03001 	mov	r3, #1
8160a3c8:	e5223560 	str	r3, [r2, #-1376]!	; 0xfffffaa0
8160a3cc:	e2422004 	sub	r2, r2, #4
8160a3d0:	e58d3034 	str	r3, [sp, #52]	; 0x34
8160a3d4:	ebfffcfd 	bl	816097d0 <mmc_send_cmd>
8160a3d8:	e3500000 	cmp	r0, #0
8160a3dc:	1affff4c 	bne	8160a114 <mmc_startup+0x62c>
8160a3e0:	e5962010 	ldr	r2, [r6, #16]
8160a3e4:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160a3e8:	e202200f 	and	r2, r2, #15
8160a3ec:	e3520001 	cmp	r2, #1
8160a3f0:	1affff75 	bne	8160a1cc <mmc_startup+0x6e4>
8160a3f4:	e3833001 	orr	r3, r3, #1
8160a3f8:	ea000025 	b	8160a494 <mmc_startup+0x9ac>
8160a3fc:	e59f242c 	ldr	r2, [pc, #1068]	; 8160a830 <mmc_startup+0xd48>
8160a400:	e1530002 	cmp	r3, r2
8160a404:	95847024 	strls	r7, [r4, #36]	; 0x24
8160a408:	9affff6f 	bls	8160a1cc <mmc_startup+0x6e4>
8160a40c:	e28d6fe3 	add	r6, sp, #908	; 0x38c
8160a410:	e3a0300c 	mov	r3, #12
8160a414:	e2866003 	add	r6, r6, #3
8160a418:	e5843024 	str	r3, [r4, #36]	; 0x24
8160a41c:	e3c6603f 	bic	r6, r6, #63	; 0x3f
8160a420:	e1a00004 	mov	r0, r4
8160a424:	e1a01006 	mov	r1, r6
8160a428:	ebfffcec 	bl	816097e0 <mmc_send_ext_csd>
8160a42c:	e3500000 	cmp	r0, #0
8160a430:	1affff37 	bne	8160a114 <mmc_startup+0x62c>
8160a434:	e3a03001 	mov	r3, #1
8160a438:	e3a020b9 	mov	r2, #185	; 0xb9
8160a43c:	e1a01003 	mov	r1, r3
8160a440:	e1a00004 	mov	r0, r4
8160a444:	e5d680c4 	ldrb	r8, [r6, #196]	; 0xc4
8160a448:	ebfffd5b 	bl	816099bc <mmc_switch>
8160a44c:	e3500000 	cmp	r0, #0
8160a450:	1affff2f 	bne	8160a114 <mmc_startup+0x62c>
8160a454:	e1a01006 	mov	r1, r6
8160a458:	e1a00004 	mov	r0, r4
8160a45c:	ebfffcdf 	bl	816097e0 <mmc_send_ext_csd>
8160a460:	e3500000 	cmp	r0, #0
8160a464:	1affff2a 	bne	8160a114 <mmc_startup+0x62c>
8160a468:	e5d630b9 	ldrb	r3, [r6, #185]	; 0xb9
8160a46c:	e3530000 	cmp	r3, #0
8160a470:	0affff55 	beq	8160a1cc <mmc_startup+0x6e4>
8160a474:	e3180002 	tst	r8, #2
8160a478:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160a47c:	0affffdc 	beq	8160a3f4 <mmc_startup+0x90c>
8160a480:	e3180004 	tst	r8, #4
8160a484:	13833020 	orrne	r3, r3, #32
8160a488:	15843024 	strne	r3, [r4, #36]	; 0x24
8160a48c:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160a490:	e3833003 	orr	r3, r3, #3
8160a494:	e5843024 	str	r3, [r4, #36]	; 0x24
8160a498:	eaffff4b 	b	8160a1cc <mmc_startup+0x6e4>
8160a49c:	e2852040 	add	r2, r5, #64	; 0x40
8160a4a0:	e5953000 	ldr	r3, [r5]
8160a4a4:	e6bf3f33 	rev	r3, r3
8160a4a8:	e4853004 	str	r3, [r5], #4
8160a4ac:	e1550002 	cmp	r5, r2
8160a4b0:	1afffffa 	bne	8160a4a0 <mmc_startup+0x9b8>
8160a4b4:	e5961008 	ldr	r1, [r6, #8]
8160a4b8:	e7e33651 	ubfx	r3, r1, #12, #4
8160a4bc:	e3530009 	cmp	r3, #9
8160a4c0:	9a000003 	bls	8160a4d4 <mmc_startup+0x9ec>
8160a4c4:	e594000c 	ldr	r0, [r4, #12]
8160a4c8:	e59f2354 	ldr	r2, [pc, #852]	; 8160a824 <mmc_startup+0xd3c>
8160a4cc:	e1500002 	cmp	r0, r2
8160a4d0:	1a000014 	bne	8160a528 <mmc_startup+0xa40>
8160a4d4:	e59f2358 	ldr	r2, [pc, #856]	; 8160a834 <mmc_startup+0xd4c>
8160a4d8:	e1a01401 	lsl	r1, r1, #8
8160a4dc:	e6ff1071 	uxth	r1, r1
8160a4e0:	e7923103 	ldr	r3, [r2, r3, lsl #2]
8160a4e4:	e5843078 	str	r3, [r4, #120]	; 0x78
8160a4e8:	e596500c 	ldr	r5, [r6, #12]
8160a4ec:	e7e50955 	ubfx	r0, r5, #18, #6
8160a4f0:	e1911c25 	orrs	r1, r1, r5, lsr #24
8160a4f4:	13a03001 	movne	r3, #1
8160a4f8:	03a03000 	moveq	r3, #0
8160a4fc:	e3500000 	cmp	r0, #0
8160a500:	03a03000 	moveq	r3, #0
8160a504:	e3530000 	cmp	r3, #0
8160a508:	0a000006 	beq	8160a528 <mmc_startup+0xa40>
8160a50c:	e3a06ffa 	mov	r6, #1000	; 0x3e8
8160a510:	e7e15855 	ubfx	r5, r5, #16, #2
8160a514:	e0000096 	mul	r0, r6, r0
8160a518:	eb002180 	bl	81612b20 <__aeabi_uidiv>
8160a51c:	e0050596 	mul	r5, r6, r5
8160a520:	e5845080 	str	r5, [r4, #128]	; 0x80
8160a524:	e584007c 	str	r0, [r4, #124]	; 0x7c
8160a528:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160a52c:	e3130001 	tst	r3, #1
8160a530:	159f3300 	ldrne	r3, [pc, #768]	; 8160a838 <mmc_startup+0xd50>
8160a534:	059f3300 	ldreq	r3, [pc, #768]	; 8160a83c <mmc_startup+0xd54>
8160a538:	e5843064 	str	r3, [r4, #100]	; 0x64
8160a53c:	ea000014 	b	8160a594 <mmc_startup+0xaac>
8160a540:	e59f32e8 	ldr	r3, [pc, #744]	; 8160a830 <mmc_startup+0xd48>
8160a544:	e1520003 	cmp	r2, r3
8160a548:	9a000011 	bls	8160a594 <mmc_startup+0xaac>
8160a54c:	e28d6fe3 	add	r6, sp, #908	; 0x38c
8160a550:	e3a08000 	mov	r8, #0
8160a554:	e2866003 	add	r6, r6, #3
8160a558:	e1a00008 	mov	r0, r8
8160a55c:	e3c6603f 	bic	r6, r6, #63	; 0x3f
8160a560:	e59f32d8 	ldr	r3, [pc, #728]	; 8160a840 <mmc_startup+0xd58>
8160a564:	e1a0b108 	lsl	fp, r8, #2
8160a568:	e59f22d4 	ldr	r2, [pc, #724]	; 8160a844 <mmc_startup+0xd5c>
8160a56c:	e7933108 	ldr	r3, [r3, r8, lsl #2]
8160a570:	e3530000 	cmp	r3, #0
8160a574:	e792a103 	ldr	sl, [r2, r3, lsl #2]
8160a578:	1a00005b 	bne	8160a6ec <mmc_startup+0xc04>
8160a57c:	e594201c 	ldr	r2, [r4, #28]
8160a580:	e3520001 	cmp	r2, #1
8160a584:	1a000058 	bne	8160a6ec <mmc_startup+0xc04>
8160a588:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160a58c:	e3130001 	tst	r3, #1
8160a590:	1a000089 	bne	8160a7bc <mmc_startup+0xcd4>
8160a594:	e5941064 	ldr	r1, [r4, #100]	; 0x64
8160a598:	e1a00004 	mov	r0, r4
8160a59c:	ebfffd48 	bl	81609ac4 <mmc_set_clock>
8160a5a0:	e5943150 	ldr	r3, [r4, #336]	; 0x150
8160a5a4:	e1a00004 	mov	r0, r4
8160a5a8:	e3a06000 	mov	r6, #0
8160a5ac:	e3530000 	cmp	r3, #0
8160a5b0:	13a03c02 	movne	r3, #512	; 0x200
8160a5b4:	15843068 	strne	r3, [r4, #104]	; 0x68
8160a5b8:	1584306c 	strne	r3, [r4, #108]	; 0x6c
8160a5bc:	eb0001dc 	bl	8160ad34 <mmc_get_blk_desc>
8160a5c0:	e59f3280 	ldr	r3, [pc, #640]	; 8160a848 <mmc_startup+0xd60>
8160a5c4:	e59f2280 	ldr	r2, [pc, #640]	; 8160a84c <mmc_startup+0xd64>
8160a5c8:	e1c060ba 	strh	r6, [r0, #10]
8160a5cc:	e1a05000 	mov	r5, r0
8160a5d0:	e5c0600c 	strb	r6, [r0, #12]
8160a5d4:	e5941068 	ldr	r1, [r4, #104]	; 0x68
8160a5d8:	e594c08c 	ldr	ip, [r4, #140]	; 0x8c
8160a5dc:	e1110003 	tst	r1, r3
8160a5e0:	e5801014 	str	r1, [r0, #20]
8160a5e4:	13a03001 	movne	r3, #1
8160a5e8:	01a03006 	moveq	r3, r6
8160a5ec:	e1110002 	tst	r1, r2
8160a5f0:	01a02006 	moveq	r2, r6
8160a5f4:	13a02002 	movne	r2, #2
8160a5f8:	e0833002 	add	r3, r3, r2
8160a5fc:	e59f224c 	ldr	r2, [pc, #588]	; 8160a850 <mmc_startup+0xd68>
8160a600:	e1110002 	tst	r1, r2
8160a604:	01a02006 	moveq	r2, r6
8160a608:	13a02004 	movne	r2, #4
8160a60c:	e0833002 	add	r3, r3, r2
8160a610:	e3c128ff 	bic	r2, r1, #16711680	; 0xff0000
8160a614:	e3c220ff 	bic	r2, r2, #255	; 0xff
8160a618:	e1520006 	cmp	r2, r6
8160a61c:	01a02006 	moveq	r2, r6
8160a620:	13a02008 	movne	r2, #8
8160a624:	e0833002 	add	r3, r3, r2
8160a628:	e1a02821 	lsr	r2, r1, #16
8160a62c:	e1a02802 	lsl	r2, r2, #16
8160a630:	e1520006 	cmp	r2, r6
8160a634:	01a02006 	moveq	r2, r6
8160a638:	13a02010 	movne	r2, #16
8160a63c:	e0833002 	add	r3, r3, r2
8160a640:	e5803018 	str	r3, [r0, #24]
8160a644:	e5940088 	ldr	r0, [r4, #136]	; 0x88
8160a648:	e1a0200c 	mov	r2, ip
8160a64c:	e1a03006 	mov	r3, r6
8160a650:	e1923003 	orrs	r3, r2, r3
8160a654:	e58dc094 	str	ip, [sp, #148]	; 0x94
8160a658:	e58d0090 	str	r0, [sp, #144]	; 0x90
8160a65c:	1a00005a 	bne	8160a7cc <mmc_startup+0xce4>
8160a660:	eb00212e 	bl	81612b20 <__aeabi_uidiv>
8160a664:	e58d6094 	str	r6, [sp, #148]	; 0x94
8160a668:	e58d0090 	str	r0, [sp, #144]	; 0x90
8160a66c:	e59d3090 	ldr	r3, [sp, #144]	; 0x90
8160a670:	e285001c 	add	r0, r5, #28
8160a674:	e1d415ba 	ldrh	r1, [r4, #90]	; 0x5a
8160a678:	e5d4204f 	ldrb	r2, [r4, #79]	; 0x4f
8160a67c:	e5853010 	str	r3, [r5, #16]
8160a680:	e1d435b4 	ldrh	r3, [r4, #84]	; 0x54
8160a684:	e58d1000 	str	r1, [sp]
8160a688:	e59f11c4 	ldr	r1, [pc, #452]	; 8160a854 <mmc_startup+0xd6c>
8160a68c:	eb001ff0 	bl	81612654 <sprintf>
8160a690:	e5943050 	ldr	r3, [r4, #80]	; 0x50
8160a694:	e5d41057 	ldrb	r1, [r4, #87]	; 0x57
8160a698:	e2850045 	add	r0, r5, #69	; 0x45
8160a69c:	e5d4204c 	ldrb	r2, [r4, #76]	; 0x4c
8160a6a0:	e58d100c 	str	r1, [sp, #12]
8160a6a4:	e6ef1073 	uxtb	r1, r3
8160a6a8:	e58d1008 	str	r1, [sp, #8]
8160a6ac:	e7e71453 	ubfx	r1, r3, #8, #8
8160a6b0:	e58d1004 	str	r1, [sp, #4]
8160a6b4:	e7e71853 	ubfx	r1, r3, #16, #8
8160a6b8:	e1a03c23 	lsr	r3, r3, #24
8160a6bc:	e58d1000 	str	r1, [sp]
8160a6c0:	e59f1190 	ldr	r1, [pc, #400]	; 8160a858 <mmc_startup+0xd70>
8160a6c4:	eb001fe2 	bl	81612654 <sprintf>
8160a6c8:	e5942054 	ldr	r2, [r4, #84]	; 0x54
8160a6cc:	e59f1188 	ldr	r1, [pc, #392]	; 8160a85c <mmc_startup+0xd74>
8160a6d0:	e285005a 	add	r0, r5, #90	; 0x5a
8160a6d4:	e7e33852 	ubfx	r3, r2, #16, #4
8160a6d8:	e7e32a52 	ubfx	r2, r2, #20, #4
8160a6dc:	eb001fdc 	bl	81612654 <sprintf>
8160a6e0:	e1a00007 	mov	r0, r7
8160a6e4:	e28dde59 	add	sp, sp, #1424	; 0x590
8160a6e8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160a6ec:	e5942024 	ldr	r2, [r4, #36]	; 0x24
8160a6f0:	e1da2002 	bics	r2, sl, r2
8160a6f4:	1a000017 	bne	8160a758 <mmc_startup+0xc70>
8160a6f8:	e6ef3073 	uxtb	r3, r3
8160a6fc:	e3a020b7 	mov	r2, #183	; 0xb7
8160a700:	e3a01001 	mov	r1, #1
8160a704:	e1a00004 	mov	r0, r4
8160a708:	ebfffcab 	bl	816099bc <mmc_switch>
8160a70c:	e3500000 	cmp	r0, #0
8160a710:	1a000010 	bne	8160a758 <mmc_startup+0xc70>
8160a714:	e59f3144 	ldr	r3, [pc, #324]	; 8160a860 <mmc_startup+0xd78>
8160a718:	e1a00004 	mov	r0, r4
8160a71c:	e7e0a2da 	ubfx	sl, sl, #5, #1
8160a720:	e584a150 	str	sl, [r4, #336]	; 0x150
8160a724:	e79b3003 	ldr	r3, [fp, r3]
8160a728:	e584301c 	str	r3, [r4, #28]
8160a72c:	ebfffbf3 	bl	81609700 <mmc_set_ios>
8160a730:	e1a01006 	mov	r1, r6
8160a734:	e1a00004 	mov	r0, r4
8160a738:	ebfffc28 	bl	816097e0 <mmc_send_ext_csd>
8160a73c:	e3500000 	cmp	r0, #0
8160a740:	1a000004 	bne	8160a758 <mmc_startup+0xc70>
8160a744:	e5d520a0 	ldrb	r2, [r5, #160]	; 0xa0
8160a748:	e5d630a0 	ldrb	r3, [r6, #160]	; 0xa0
8160a74c:	e1520003 	cmp	r2, r3
8160a750:	0a000006 	beq	8160a770 <mmc_startup+0xc88>
8160a754:	e3e00049 	mvn	r0, #73	; 0x49
8160a758:	e2888001 	add	r8, r8, #1
8160a75c:	e3580005 	cmp	r8, #5
8160a760:	1affff7e 	bne	8160a560 <mmc_startup+0xa78>
8160a764:	e3500000 	cmp	r0, #0
8160a768:	1afffe69 	bne	8160a114 <mmc_startup+0x62c>
8160a76c:	eaffff85 	b	8160a588 <mmc_startup+0xaa0>
8160a770:	e5d520dd 	ldrb	r2, [r5, #221]	; 0xdd
8160a774:	e5d630dd 	ldrb	r3, [r6, #221]	; 0xdd
8160a778:	e1520003 	cmp	r2, r3
8160a77c:	1afffff4 	bne	8160a754 <mmc_startup+0xc6c>
8160a780:	e5d520c0 	ldrb	r2, [r5, #192]	; 0xc0
8160a784:	e5d630c0 	ldrb	r3, [r6, #192]	; 0xc0
8160a788:	e1520003 	cmp	r2, r3
8160a78c:	1afffff0 	bne	8160a754 <mmc_startup+0xc6c>
8160a790:	e5d520e0 	ldrb	r2, [r5, #224]	; 0xe0
8160a794:	e5d630e0 	ldrb	r3, [r6, #224]	; 0xe0
8160a798:	e1520003 	cmp	r2, r3
8160a79c:	1affffec 	bne	8160a754 <mmc_startup+0xc6c>
8160a7a0:	e3a02004 	mov	r2, #4
8160a7a4:	e28610d4 	add	r1, r6, #212	; 0xd4
8160a7a8:	e28500d4 	add	r0, r5, #212	; 0xd4
8160a7ac:	eb001c8f 	bl	816119f0 <memcmp>
8160a7b0:	e3500000 	cmp	r0, #0
8160a7b4:	1affffe6 	bne	8160a754 <mmc_startup+0xc6c>
8160a7b8:	eaffff72 	b	8160a588 <mmc_startup+0xaa0>
8160a7bc:	e3130002 	tst	r3, #2
8160a7c0:	159f309c 	ldrne	r3, [pc, #156]	; 8160a864 <mmc_startup+0xd7c>
8160a7c4:	059f309c 	ldreq	r3, [pc, #156]	; 8160a868 <mmc_startup+0xd80>
8160a7c8:	eaffff5a 	b	8160a538 <mmc_startup+0xa50>
8160a7cc:	e28d0090 	add	r0, sp, #144	; 0x90
8160a7d0:	eb001bb1 	bl	8161169c <__div64_32>
8160a7d4:	eaffffa4 	b	8160a66c <mmc_startup+0xb84>
8160a7d8:	40010200 	.word	0x40010200
8160a7dc:	81616564 	.word	0x81616564
8160a7e0:	816131ec 	.word	0x816131ec
8160a7e4:	81616509 	.word	0x81616509
8160a7e8:	3ffbffff 	.word	0x3ffbffff
8160a7ec:	40010400 	.word	0x40010400
8160a7f0:	40020200 	.word	0x40020200
8160a7f4:	40030000 	.word	0x40030000
8160a7f8:	40040000 	.word	0x40040000
8160a7fc:	40040100 	.word	0x40040100
8160a800:	40040200 	.word	0x40040200
8160a804:	40040300 	.word	0x40040300
8160a808:	40040401 	.word	0x40040401
8160a80c:	40040500 	.word	0x40040500
8160a810:	40050000 	.word	0x40050000
8160a814:	40050100 	.word	0x40050100
8160a818:	80010a00 	.word	0x80010a00
8160a81c:	80010000 	.word	0x80010000
8160a820:	80020000 	.word	0x80020000
8160a824:	80030000 	.word	0x80030000
8160a828:	00fffff1 	.word	0x00fffff1
8160a82c:	80fffff1 	.word	0x80fffff1
8160a830:	4003ffff 	.word	0x4003ffff
8160a834:	816132dc 	.word	0x816132dc
8160a838:	02faf080 	.word	0x02faf080
8160a83c:	017d7840 	.word	0x017d7840
8160a840:	816131bc 	.word	0x816131bc
8160a844:	816131d0 	.word	0x816131d0
8160a848:	aaaaaaaa 	.word	0xaaaaaaaa
8160a84c:	cccccccc 	.word	0xcccccccc
8160a850:	f0f0f0f0 	.word	0xf0f0f0f0
8160a854:	8161651e 	.word	0x8161651e
8160a858:	81616534 	.word	0x81616534
8160a85c:	81616541 	.word	0x81616541
8160a860:	816136a0 	.word	0x816136a0
8160a864:	03197500 	.word	0x03197500
8160a868:	018cba80 	.word	0x018cba80

8160a86c <board_mmc_power_init>:
8160a86c:	e12fff1e 	bx	lr

8160a870 <mmc_start_init>:
8160a870:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160a874:	e24dd020 	sub	sp, sp, #32
8160a878:	e1a04000 	mov	r4, r0
8160a87c:	ebfffc81 	bl	81609a88 <mmc_getcd>
8160a880:	e3500000 	cmp	r0, #0
8160a884:	0a000004 	beq	8160a89c <mmc_start_init+0x2c>
8160a888:	e5943008 	ldr	r3, [r4, #8]
8160a88c:	e5933004 	ldr	r3, [r3, #4]
8160a890:	e5933008 	ldr	r3, [r3, #8]
8160a894:	e3530000 	cmp	r3, #0
8160a898:	1a000007 	bne	8160a8bc <mmc_start_init+0x4c>
8160a89c:	e3a03000 	mov	r3, #0
8160a8a0:	e59f0210 	ldr	r0, [pc, #528]	; 8160aab8 <mmc_start_init+0x248>
8160a8a4:	e5843014 	str	r3, [r4, #20]
8160a8a8:	e3e0507a 	mvn	r5, #122	; 0x7a
8160a8ac:	eb001f72 	bl	8161267c <printf>
8160a8b0:	e1a00005 	mov	r0, r5
8160a8b4:	e28dd020 	add	sp, sp, #32
8160a8b8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160a8bc:	e5943014 	ldr	r3, [r4, #20]
8160a8c0:	e3530000 	cmp	r3, #0
8160a8c4:	13a05000 	movne	r5, #0
8160a8c8:	1afffff8 	bne	8160a8b0 <mmc_start_init+0x40>
8160a8cc:	ebffffe6 	bl	8160a86c <board_mmc_power_init>
8160a8d0:	e5943008 	ldr	r3, [r4, #8]
8160a8d4:	e1a00004 	mov	r0, r4
8160a8d8:	e5933004 	ldr	r3, [r3, #4]
8160a8dc:	e5933008 	ldr	r3, [r3, #8]
8160a8e0:	e12fff33 	blx	r3
8160a8e4:	e2505000 	subs	r5, r0, #0
8160a8e8:	1afffff0 	bne	8160a8b0 <mmc_start_init+0x40>
8160a8ec:	e5845150 	str	r5, [r4, #336]	; 0x150
8160a8f0:	e1a00004 	mov	r0, r4
8160a8f4:	e3a05001 	mov	r5, #1
8160a8f8:	e584501c 	str	r5, [r4, #28]
8160a8fc:	ebfffb7f 	bl	81609700 <mmc_set_ios>
8160a900:	e1a01005 	mov	r1, r5
8160a904:	e1a00004 	mov	r0, r4
8160a908:	ebfffc6d 	bl	81609ac4 <mmc_set_clock>
8160a90c:	e1a00004 	mov	r0, r4
8160a910:	ebfffbc5 	bl	8160982c <mmc_go_idle>
8160a914:	e2505000 	subs	r5, r0, #0
8160a918:	1affffe4 	bne	8160a8b0 <mmc_start_init+0x40>
8160a91c:	e1a00004 	mov	r0, r4
8160a920:	eb000103 	bl	8160ad34 <mmc_get_blk_desc>
8160a924:	e3a03008 	mov	r3, #8
8160a928:	e28d1004 	add	r1, sp, #4
8160a92c:	e5c0500b 	strb	r5, [r0, #11]
8160a930:	e1a00004 	mov	r0, r4
8160a934:	e1cd30b4 	strh	r3, [sp, #4]
8160a938:	e5943008 	ldr	r3, [r4, #8]
8160a93c:	e593200c 	ldr	r2, [r3, #12]
8160a940:	e59f3174 	ldr	r3, [pc, #372]	; 8160aabc <mmc_start_init+0x24c>
8160a944:	e0033002 	and	r3, r3, r2
8160a948:	e1a02005 	mov	r2, r5
8160a94c:	e3530000 	cmp	r3, #0
8160a950:	e30031aa 	movw	r3, #426	; 0x1aa
8160a954:	03a030aa 	moveq	r3, #170	; 0xaa
8160a958:	e58d300c 	str	r3, [sp, #12]
8160a95c:	e3a03015 	mov	r3, #21
8160a960:	e58d3008 	str	r3, [sp, #8]
8160a964:	ebfffb99 	bl	816097d0 <mmc_send_cmd>
8160a968:	e3500000 	cmp	r0, #0
8160a96c:	1a000003 	bne	8160a980 <mmc_start_init+0x110>
8160a970:	e5dd3010 	ldrb	r3, [sp, #16]
8160a974:	e35300aa 	cmp	r3, #170	; 0xaa
8160a978:	059f3140 	ldreq	r3, [pc, #320]	; 8160aac0 <mmc_start_init+0x250>
8160a97c:	0584300c 	streq	r3, [r4, #12]
8160a980:	e59f7134 	ldr	r7, [pc, #308]	; 8160aabc <mmc_start_init+0x24c>
8160a984:	e30063e9 	movw	r6, #1001	; 0x3e9
8160a988:	e59f8130 	ldr	r8, [pc, #304]	; 8160aac0 <mmc_start_init+0x250>
8160a98c:	e3a02000 	mov	r2, #0
8160a990:	e28d1004 	add	r1, sp, #4
8160a994:	e1a00004 	mov	r0, r4
8160a998:	e3a03037 	mov	r3, #55	; 0x37
8160a99c:	e58d200c 	str	r2, [sp, #12]
8160a9a0:	e1cd30b4 	strh	r3, [sp, #4]
8160a9a4:	e3a03015 	mov	r3, #21
8160a9a8:	e58d3008 	str	r3, [sp, #8]
8160a9ac:	ebfffb87 	bl	816097d0 <mmc_send_cmd>
8160a9b0:	e3500000 	cmp	r0, #0
8160a9b4:	1a000026 	bne	8160aa54 <mmc_start_init+0x1e4>
8160a9b8:	e3a03029 	mov	r3, #41	; 0x29
8160a9bc:	e1cd30b4 	strh	r3, [sp, #4]
8160a9c0:	e3a03001 	mov	r3, #1
8160a9c4:	e58d3008 	str	r3, [sp, #8]
8160a9c8:	e5943008 	ldr	r3, [r4, #8]
8160a9cc:	e28d1004 	add	r1, sp, #4
8160a9d0:	e594200c 	ldr	r2, [r4, #12]
8160a9d4:	e1a00004 	mov	r0, r4
8160a9d8:	e593300c 	ldr	r3, [r3, #12]
8160a9dc:	e1520008 	cmp	r2, r8
8160a9e0:	e3a02000 	mov	r2, #0
8160a9e4:	e0033007 	and	r3, r3, r7
8160a9e8:	e58d300c 	str	r3, [sp, #12]
8160a9ec:	03833101 	orreq	r3, r3, #1073741824	; 0x40000000
8160a9f0:	058d300c 	streq	r3, [sp, #12]
8160a9f4:	ebfffb75 	bl	816097d0 <mmc_send_cmd>
8160a9f8:	e3500000 	cmp	r0, #0
8160a9fc:	1a000014 	bne	8160aa54 <mmc_start_init+0x1e4>
8160aa00:	e59d3010 	ldr	r3, [sp, #16]
8160aa04:	e3530000 	cmp	r3, #0
8160aa08:	ba000004 	blt	8160aa20 <mmc_start_init+0x1b0>
8160aa0c:	e2566001 	subs	r6, r6, #1
8160aa10:	0a000026 	beq	8160aab0 <mmc_start_init+0x240>
8160aa14:	e3a00ffa 	mov	r0, #1000	; 0x3e8
8160aa18:	eb001c63 	bl	81611bac <udelay>
8160aa1c:	eaffffda 	b	8160a98c <mmc_start_init+0x11c>
8160aa20:	e59f2098 	ldr	r2, [pc, #152]	; 8160aac0 <mmc_start_init+0x250>
8160aa24:	e594100c 	ldr	r1, [r4, #12]
8160aa28:	e5843028 	str	r3, [r4, #40]	; 0x28
8160aa2c:	e7e03f53 	ubfx	r3, r3, #30, #1
8160aa30:	e1510002 	cmp	r1, r2
8160aa34:	e5843018 	str	r3, [r4, #24]
8160aa38:	12422801 	subne	r2, r2, #65536	; 0x10000
8160aa3c:	e3a03000 	mov	r3, #0
8160aa40:	1584200c 	strne	r2, [r4, #12]
8160aa44:	e1c435bc 	strh	r3, [r4, #92]	; 0x5c
8160aa48:	e3a03001 	mov	r3, #1
8160aa4c:	e5c4314d 	strb	r3, [r4, #333]	; 0x14d
8160aa50:	eaffff96 	b	8160a8b0 <mmc_start_init+0x40>
8160aa54:	e370006e 	cmn	r0, #110	; 0x6e
8160aa58:	11a05000 	movne	r5, r0
8160aa5c:	1affff93 	bne	8160a8b0 <mmc_start_init+0x40>
8160aa60:	e1a00004 	mov	r0, r4
8160aa64:	ebfffb70 	bl	8160982c <mmc_go_idle>
8160aa68:	e3a01000 	mov	r1, #0
8160aa6c:	e1a00004 	mov	r0, r4
8160aa70:	ebfffb80 	bl	81609878 <mmc_send_op_cond_iter>
8160aa74:	e3500000 	cmp	r0, #0
8160aa78:	1a00000a 	bne	8160aaa8 <mmc_start_init+0x238>
8160aa7c:	e5943028 	ldr	r3, [r4, #40]	; 0x28
8160aa80:	e3530000 	cmp	r3, #0
8160aa84:	aa000002 	bge	8160aa94 <mmc_start_init+0x224>
8160aa88:	e3a03001 	mov	r3, #1
8160aa8c:	e5c4314c 	strb	r3, [r4, #332]	; 0x14c
8160aa90:	eaffffec 	b	8160aa48 <mmc_start_init+0x1d8>
8160aa94:	e3a01001 	mov	r1, #1
8160aa98:	e1a00004 	mov	r0, r4
8160aa9c:	ebfffb75 	bl	81609878 <mmc_send_op_cond_iter>
8160aaa0:	e3500000 	cmp	r0, #0
8160aaa4:	0afffff7 	beq	8160aa88 <mmc_start_init+0x218>
8160aaa8:	e59f0014 	ldr	r0, [pc, #20]	; 8160aac4 <mmc_start_init+0x254>
8160aaac:	eb001ef2 	bl	8161267c <printf>
8160aab0:	e3e0505e 	mvn	r5, #94	; 0x5e
8160aab4:	eaffff7d 	b	8160a8b0 <mmc_start_init+0x40>
8160aab8:	816164ca 	.word	0x816164ca
8160aabc:	00ff8000 	.word	0x00ff8000
8160aac0:	80020000 	.word	0x80020000
8160aac4:	816164e0 	.word	0x816164e0

8160aac8 <mmc_init>:
8160aac8:	e5903014 	ldr	r3, [r0, #20]
8160aacc:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160aad0:	e3530000 	cmp	r3, #0
8160aad4:	13a05000 	movne	r5, #0
8160aad8:	1a00001f 	bne	8160ab5c <mmc_init+0x94>
8160aadc:	e1a04000 	mov	r4, r0
8160aae0:	e1a00003 	mov	r0, r3
8160aae4:	eb001c29 	bl	81611b90 <get_timer>
8160aae8:	e5d4314d 	ldrb	r3, [r4, #333]	; 0x14d
8160aaec:	e3530000 	cmp	r3, #0
8160aaf0:	e1a06000 	mov	r6, r0
8160aaf4:	0a00000d 	beq	8160ab30 <mmc_init+0x68>
8160aaf8:	e5d4314c 	ldrb	r3, [r4, #332]	; 0x14c
8160aafc:	e3a05000 	mov	r5, #0
8160ab00:	e5c4514d 	strb	r5, [r4, #333]	; 0x14d
8160ab04:	e1530005 	cmp	r3, r5
8160ab08:	1a000015 	bne	8160ab64 <mmc_init+0x9c>
8160ab0c:	e1a00004 	mov	r0, r4
8160ab10:	ebfffbf4 	bl	81609ae8 <mmc_startup>
8160ab14:	e2505000 	subs	r5, r0, #0
8160ab18:	03a03001 	moveq	r3, #1
8160ab1c:	05843014 	streq	r3, [r4, #20]
8160ab20:	0a00000d 	beq	8160ab5c <mmc_init+0x94>
8160ab24:	e3a03000 	mov	r3, #0
8160ab28:	e5843014 	str	r3, [r4, #20]
8160ab2c:	ea000003 	b	8160ab40 <mmc_init+0x78>
8160ab30:	e1a00004 	mov	r0, r4
8160ab34:	ebffff4d 	bl	8160a870 <mmc_start_init>
8160ab38:	e2505000 	subs	r5, r0, #0
8160ab3c:	0affffed 	beq	8160aaf8 <mmc_init+0x30>
8160ab40:	e1a00006 	mov	r0, r6
8160ab44:	eb001c11 	bl	81611b90 <get_timer>
8160ab48:	e1a02005 	mov	r2, r5
8160ab4c:	e59f1098 	ldr	r1, [pc, #152]	; 8160abec <mmc_init+0x124>
8160ab50:	e1a03000 	mov	r3, r0
8160ab54:	e59f0094 	ldr	r0, [pc, #148]	; 8160abf0 <mmc_init+0x128>
8160ab58:	eb001ec7 	bl	8161267c <printf>
8160ab5c:	e1a00005 	mov	r0, r5
8160ab60:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160ab64:	e5943028 	ldr	r3, [r4, #40]	; 0x28
8160ab68:	e5c4514c 	strb	r5, [r4, #332]	; 0x14c
8160ab6c:	e3530000 	cmp	r3, #0
8160ab70:	ba000013 	blt	8160abc4 <mmc_init+0xfc>
8160ab74:	e1a00004 	mov	r0, r4
8160ab78:	ebfffb2b 	bl	8160982c <mmc_go_idle>
8160ab7c:	e1a00005 	mov	r0, r5
8160ab80:	eb001c02 	bl	81611b90 <get_timer>
8160ab84:	e1a07000 	mov	r7, r0
8160ab88:	e3a01001 	mov	r1, #1
8160ab8c:	e1a00004 	mov	r0, r4
8160ab90:	ebfffb38 	bl	81609878 <mmc_send_op_cond_iter>
8160ab94:	e2505000 	subs	r5, r0, #0
8160ab98:	1affffe1 	bne	8160ab24 <mmc_init+0x5c>
8160ab9c:	e5943028 	ldr	r3, [r4, #40]	; 0x28
8160aba0:	e3530000 	cmp	r3, #0
8160aba4:	ba000006 	blt	8160abc4 <mmc_init+0xfc>
8160aba8:	e1a00007 	mov	r0, r7
8160abac:	eb001bf7 	bl	81611b90 <get_timer>
8160abb0:	e3500ffa 	cmp	r0, #1000	; 0x3e8
8160abb4:	8a00000a 	bhi	8160abe4 <mmc_init+0x11c>
8160abb8:	e3a00064 	mov	r0, #100	; 0x64
8160abbc:	eb001bfa 	bl	81611bac <udelay>
8160abc0:	eafffff0 	b	8160ab88 <mmc_init+0xc0>
8160abc4:	e3a03101 	mov	r3, #1073741824	; 0x40000000
8160abc8:	e584300c 	str	r3, [r4, #12]
8160abcc:	e5943028 	ldr	r3, [r4, #40]	; 0x28
8160abd0:	e7e03f53 	ubfx	r3, r3, #30, #1
8160abd4:	e5843018 	str	r3, [r4, #24]
8160abd8:	e3a03001 	mov	r3, #1
8160abdc:	e1c435bc 	strh	r3, [r4, #92]	; 0x5c
8160abe0:	eaffffc9 	b	8160ab0c <mmc_init+0x44>
8160abe4:	e3e0505e 	mvn	r5, #94	; 0x5e
8160abe8:	eaffffcd 	b	8160ab24 <mmc_init+0x5c>
8160abec:	816140d8 	.word	0x816140d8
8160abf0:	81616486 	.word	0x81616486

8160abf4 <cpu_mmc_init>:
8160abf4:	e3e00000 	mvn	r0, #0
8160abf8:	e12fff1e 	bx	lr

8160abfc <board_mmc_init>:
8160abfc:	e3e00000 	mvn	r0, #0
8160ac00:	e12fff1e 	bx	lr

8160ac04 <mmc_initialize>:
8160ac04:	e59f3050 	ldr	r3, [pc, #80]	; 8160ac5c <mmc_initialize+0x58>
8160ac08:	e5932000 	ldr	r2, [r3]
8160ac0c:	e3520000 	cmp	r2, #0
8160ac10:	1a00000f 	bne	8160ac54 <mmc_initialize+0x50>
8160ac14:	e92d4010 	push	{r4, lr}
8160ac18:	e1a04000 	mov	r4, r0
8160ac1c:	e3a02001 	mov	r2, #1
8160ac20:	e5832000 	str	r2, [r3]
8160ac24:	eb000052 	bl	8160ad74 <mmc_list_init>
8160ac28:	e1a00004 	mov	r0, r4
8160ac2c:	ebfffff2 	bl	8160abfc <board_mmc_init>
8160ac30:	e3500000 	cmp	r0, #0
8160ac34:	aa000001 	bge	8160ac40 <mmc_initialize+0x3c>
8160ac38:	e1a00004 	mov	r0, r4
8160ac3c:	ebffffec 	bl	8160abf4 <cpu_mmc_init>
8160ac40:	e3a0002c 	mov	r0, #44	; 0x2c
8160ac44:	eb000053 	bl	8160ad98 <print_mmc_devices>
8160ac48:	eb00003b 	bl	8160ad3c <mmc_do_preinit>
8160ac4c:	e3a00000 	mov	r0, #0
8160ac50:	e8bd8010 	pop	{r4, pc}
8160ac54:	e3a00000 	mov	r0, #0
8160ac58:	e12fff1e 	bx	lr
8160ac5c:	81619e54 	.word	0x81619e54

8160ac60 <find_mmc_device>:
8160ac60:	e59f2040 	ldr	r2, [pc, #64]	; 8160aca8 <find_mmc_device+0x48>
8160ac64:	e5923000 	ldr	r3, [r2]
8160ac68:	e1530002 	cmp	r3, r2
8160ac6c:	e5931000 	ldr	r1, [r3]
8160ac70:	1a000006 	bne	8160ac90 <find_mmc_device+0x30>
8160ac74:	e1a01000 	mov	r1, r0
8160ac78:	e59f002c 	ldr	r0, [pc, #44]	; 8160acac <find_mmc_device+0x4c>
8160ac7c:	e92d4010 	push	{r4, lr}
8160ac80:	eb001e7d 	bl	8161267c <printf>
8160ac84:	e3a03000 	mov	r3, #0
8160ac88:	e1a00003 	mov	r0, r3
8160ac8c:	e8bd8010 	pop	{r4, pc}
8160ac90:	e593c0dc 	ldr	ip, [r3, #220]	; 0xdc
8160ac94:	e15c0000 	cmp	ip, r0
8160ac98:	11a03001 	movne	r3, r1
8160ac9c:	1afffff1 	bne	8160ac68 <find_mmc_device+0x8>
8160aca0:	e1a00003 	mov	r0, r3
8160aca4:	e12fff1e 	bx	lr
8160aca8:	81619e58 	.word	0x81619e58
8160acac:	81615e5f 	.word	0x81615e5f

8160acb0 <mmc_select_hwpartp>:
8160acb0:	e92d4010 	push	{r4, lr}
8160acb4:	e1a04001 	mov	r4, r1
8160acb8:	e5900004 	ldr	r0, [r0, #4]
8160acbc:	ebffffe7 	bl	8160ac60 <find_mmc_device>
8160acc0:	e2503000 	subs	r3, r0, #0
8160acc4:	0a000008 	beq	8160acec <mmc_select_hwpartp+0x3c>
8160acc8:	e5d320e3 	ldrb	r2, [r3, #227]	; 0xe3
8160accc:	e1520004 	cmp	r2, r4
8160acd0:	0a000007 	beq	8160acf4 <mmc_select_hwpartp+0x44>
8160acd4:	e5d33061 	ldrb	r3, [r3, #97]	; 0x61
8160acd8:	e35300ff 	cmp	r3, #255	; 0xff
8160acdc:	0a000006 	beq	8160acfc <mmc_select_hwpartp+0x4c>
8160ace0:	e1a01004 	mov	r1, r4
8160ace4:	e8bd4010 	pop	{r4, lr}
8160ace8:	eafffb4d 	b	81609a24 <mmc_switch_part>
8160acec:	e3e00012 	mvn	r0, #18
8160acf0:	e8bd8010 	pop	{r4, pc}
8160acf4:	e3a00000 	mov	r0, #0
8160acf8:	e8bd8010 	pop	{r4, pc}
8160acfc:	e3e0007b 	mvn	r0, #123	; 0x7b
8160ad00:	e8bd8010 	pop	{r4, pc}

8160ad04 <mmc_get_dev>:
8160ad04:	e92d4070 	push	{r4, r5, r6, lr}
8160ad08:	e1a05001 	mov	r5, r1
8160ad0c:	ebffffd3 	bl	8160ac60 <find_mmc_device>
8160ad10:	e2504000 	subs	r4, r0, #0
8160ad14:	0a000004 	beq	8160ad2c <mmc_get_dev+0x28>
8160ad18:	ebffff6a 	bl	8160aac8 <mmc_init>
8160ad1c:	e3500000 	cmp	r0, #0
8160ad20:	028440d8 	addeq	r4, r4, #216	; 0xd8
8160ad24:	05854000 	streq	r4, [r5]
8160ad28:	e8bd8070 	pop	{r4, r5, r6, pc}
8160ad2c:	e3e00012 	mvn	r0, #18
8160ad30:	e8bd8070 	pop	{r4, r5, r6, pc}

8160ad34 <mmc_get_blk_desc>:
8160ad34:	e28000d8 	add	r0, r0, #216	; 0xd8
8160ad38:	e12fff1e 	bx	lr

8160ad3c <mmc_do_preinit>:
8160ad3c:	e59f302c 	ldr	r3, [pc, #44]	; 8160ad70 <mmc_do_preinit+0x34>
8160ad40:	e92d4070 	push	{r4, r5, r6, lr}
8160ad44:	e5934000 	ldr	r4, [r3]
8160ad48:	e1a05003 	mov	r5, r3
8160ad4c:	e1540005 	cmp	r4, r5
8160ad50:	08bd8070 	popeq	{r4, r5, r6, pc}
8160ad54:	e5d4314e 	ldrb	r3, [r4, #334]	; 0x14e
8160ad58:	e3530000 	cmp	r3, #0
8160ad5c:	0a000001 	beq	8160ad68 <mmc_do_preinit+0x2c>
8160ad60:	e1a00004 	mov	r0, r4
8160ad64:	ebfffec1 	bl	8160a870 <mmc_start_init>
8160ad68:	e5944000 	ldr	r4, [r4]
8160ad6c:	eafffff6 	b	8160ad4c <mmc_do_preinit+0x10>
8160ad70:	81619e58 	.word	0x81619e58

8160ad74 <mmc_list_init>:
8160ad74:	e59f3014 	ldr	r3, [pc, #20]	; 8160ad90 <mmc_list_init+0x1c>
8160ad78:	e3a02000 	mov	r2, #0
8160ad7c:	e5833000 	str	r3, [r3]
8160ad80:	e5833004 	str	r3, [r3, #4]
8160ad84:	e59f3008 	ldr	r3, [pc, #8]	; 8160ad94 <mmc_list_init+0x20>
8160ad88:	e5832000 	str	r2, [r3]
8160ad8c:	e12fff1e 	bx	lr
8160ad90:	81619e58 	.word	0x81619e58
8160ad94:	816190bc 	.word	0x816190bc

8160ad98 <print_mmc_devices>:
8160ad98:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160ad9c:	e1a07000 	mov	r7, r0
8160ada0:	e59f6090 	ldr	r6, [pc, #144]	; 8160ae38 <print_mmc_devices+0xa0>
8160ada4:	e59f8090 	ldr	r8, [pc, #144]	; 8160ae3c <print_mmc_devices+0xa4>
8160ada8:	e59fa090 	ldr	sl, [pc, #144]	; 8160ae40 <print_mmc_devices+0xa8>
8160adac:	e5964000 	ldr	r4, [r6]
8160adb0:	e1540006 	cmp	r4, r6
8160adb4:	1a000002 	bne	8160adc4 <print_mmc_devices+0x2c>
8160adb8:	e59f0084 	ldr	r0, [pc, #132]	; 8160ae44 <print_mmc_devices+0xac>
8160adbc:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
8160adc0:	ea001e2d 	b	8161267c <printf>
8160adc4:	e5945014 	ldr	r5, [r4, #20]
8160adc8:	e3550000 	cmp	r5, #0
8160adcc:	0a000003 	beq	8160ade0 <print_mmc_devices+0x48>
8160add0:	e594500c 	ldr	r5, [r4, #12]
8160add4:	e3550000 	cmp	r5, #0
8160add8:	b1a05008 	movlt	r5, r8
8160addc:	a1a0500a 	movge	r5, sl
8160ade0:	e5943008 	ldr	r3, [r4, #8]
8160ade4:	e59420dc 	ldr	r2, [r4, #220]	; 0xdc
8160ade8:	e59f0058 	ldr	r0, [pc, #88]	; 8160ae48 <print_mmc_devices+0xb0>
8160adec:	e5931000 	ldr	r1, [r3]
8160adf0:	eb001e21 	bl	8161267c <printf>
8160adf4:	e3550000 	cmp	r5, #0
8160adf8:	0a000002 	beq	8160ae08 <print_mmc_devices+0x70>
8160adfc:	e1a01005 	mov	r1, r5
8160ae00:	e59f0044 	ldr	r0, [pc, #68]	; 8160ae4c <print_mmc_devices+0xb4>
8160ae04:	eb001e1c 	bl	8161267c <printf>
8160ae08:	e5943000 	ldr	r3, [r4]
8160ae0c:	e1530006 	cmp	r3, r6
8160ae10:	0a000006 	beq	8160ae30 <print_mmc_devices+0x98>
8160ae14:	e1a01007 	mov	r1, r7
8160ae18:	e59f0030 	ldr	r0, [pc, #48]	; 8160ae50 <print_mmc_devices+0xb8>
8160ae1c:	eb001e16 	bl	8161267c <printf>
8160ae20:	e357000a 	cmp	r7, #10
8160ae24:	0a000001 	beq	8160ae30 <print_mmc_devices+0x98>
8160ae28:	e59f0024 	ldr	r0, [pc, #36]	; 8160ae54 <print_mmc_devices+0xbc>
8160ae2c:	ebffefd8 	bl	81606d94 <puts>
8160ae30:	e5944000 	ldr	r4, [r4]
8160ae34:	eaffffdd 	b	8160adb0 <print_mmc_devices+0x18>
8160ae38:	81619e58 	.word	0x81619e58
8160ae3c:	8161778e 	.word	0x8161778e
8160ae40:	816166d0 	.word	0x816166d0
8160ae44:	8161623a 	.word	0x8161623a
8160ae48:	816166d5 	.word	0x816166d5
8160ae4c:	816166dc 	.word	0x816166dc
8160ae50:	81616843 	.word	0x81616843
8160ae54:	81616122 	.word	0x81616122

8160ae58 <serial_stub_start>:
8160ae58:	e5903040 	ldr	r3, [r0, #64]	; 0x40
8160ae5c:	e5933010 	ldr	r3, [r3, #16]
8160ae60:	e12fff13 	bx	r3

8160ae64 <serial_stub_stop>:
8160ae64:	e5903040 	ldr	r3, [r0, #64]	; 0x40
8160ae68:	e5933014 	ldr	r3, [r3, #20]
8160ae6c:	e12fff13 	bx	r3

8160ae70 <serial_stub_putc>:
8160ae70:	e5903040 	ldr	r3, [r0, #64]	; 0x40
8160ae74:	e1a00001 	mov	r0, r1
8160ae78:	e5933024 	ldr	r3, [r3, #36]	; 0x24
8160ae7c:	e12fff13 	bx	r3

8160ae80 <serial_stub_puts>:
8160ae80:	e5903040 	ldr	r3, [r0, #64]	; 0x40
8160ae84:	e1a00001 	mov	r0, r1
8160ae88:	e5933028 	ldr	r3, [r3, #40]	; 0x28
8160ae8c:	e12fff13 	bx	r3

8160ae90 <serial_stub_getc>:
8160ae90:	e5903040 	ldr	r3, [r0, #64]	; 0x40
8160ae94:	e593301c 	ldr	r3, [r3, #28]
8160ae98:	e12fff13 	bx	r3

8160ae9c <serial_stub_tstc>:
8160ae9c:	e5903040 	ldr	r3, [r0, #64]	; 0x40
8160aea0:	e5933020 	ldr	r3, [r3, #32]
8160aea4:	e12fff13 	bx	r3

8160aea8 <get_current>:
8160aea8:	e5993004 	ldr	r3, [r9, #4]
8160aeac:	e3130001 	tst	r3, #1
8160aeb0:	1a000005 	bne	8160aecc <get_current+0x24>
8160aeb4:	e92d4010 	push	{r4, lr}
8160aeb8:	eb000157 	bl	8160b41c <default_serial_console>
8160aebc:	e3500000 	cmp	r0, #0
8160aec0:	18bd8010 	popne	{r4, pc}
8160aec4:	e59f0014 	ldr	r0, [pc, #20]	; 8160aee0 <get_current+0x38>
8160aec8:	eb001e2b 	bl	8161277c <panic>
8160aecc:	e59f3010 	ldr	r3, [pc, #16]	; 8160aee4 <get_current+0x3c>
8160aed0:	e5930000 	ldr	r0, [r3]
8160aed4:	e3500000 	cmp	r0, #0
8160aed8:	112fff1e 	bxne	lr
8160aedc:	eafffff4 	b	8160aeb4 <get_current+0xc>
8160aee0:	81615ec6 	.word	0x81615ec6
8160aee4:	81619e60 	.word	0x81619e60

8160aee8 <amirix_serial_initialize>:
8160aee8:	e12fff1e 	bx	lr

8160aeec <serial_register>:
8160aeec:	e59f300c 	ldr	r3, [pc, #12]	; 8160af00 <serial_register+0x14>
8160aef0:	e5932000 	ldr	r2, [r3]
8160aef4:	e5830000 	str	r0, [r3]
8160aef8:	e580202c 	str	r2, [r0, #44]	; 0x2c
8160aefc:	e12fff1e 	bx	lr
8160af00:	81619e64 	.word	0x81619e64

8160af04 <serial_stdio_init>:
8160af04:	e59f3080 	ldr	r3, [pc, #128]	; 8160af8c <serial_stdio_init+0x88>
8160af08:	e92d4070 	push	{r4, r5, r6, lr}
8160af0c:	e24dd050 	sub	sp, sp, #80	; 0x50
8160af10:	e5934000 	ldr	r4, [r3]
8160af14:	e59f5074 	ldr	r5, [pc, #116]	; 8160af90 <serial_stdio_init+0x8c>
8160af18:	e59f6074 	ldr	r6, [pc, #116]	; 8160af94 <serial_stdio_init+0x90>
8160af1c:	e3540000 	cmp	r4, #0
8160af20:	1a000001 	bne	8160af2c <serial_stdio_init+0x28>
8160af24:	e28dd050 	add	sp, sp, #80	; 0x50
8160af28:	e8bd8070 	pop	{r4, r5, r6, pc}
8160af2c:	e3a0204c 	mov	r2, #76	; 0x4c
8160af30:	e3a01000 	mov	r1, #0
8160af34:	e28d0004 	add	r0, sp, #4
8160af38:	ebffd5e0 	bl	816006c0 <memset>
8160af3c:	e1a01004 	mov	r1, r4
8160af40:	e28d000c 	add	r0, sp, #12
8160af44:	eb001a2e 	bl	81611804 <strcpy>
8160af48:	e3a03003 	mov	r3, #3
8160af4c:	e58d3004 	str	r3, [sp, #4]
8160af50:	e28d0004 	add	r0, sp, #4
8160af54:	e59f303c 	ldr	r3, [pc, #60]	; 8160af98 <serial_stdio_init+0x94>
8160af58:	e58d4044 	str	r4, [sp, #68]	; 0x44
8160af5c:	e58d502c 	str	r5, [sp, #44]	; 0x2c
8160af60:	e58d3034 	str	r3, [sp, #52]	; 0x34
8160af64:	e59f3030 	ldr	r3, [pc, #48]	; 8160af9c <serial_stdio_init+0x98>
8160af68:	e58d6030 	str	r6, [sp, #48]	; 0x30
8160af6c:	e58d3038 	str	r3, [sp, #56]	; 0x38
8160af70:	e59f3028 	ldr	r3, [pc, #40]	; 8160afa0 <serial_stdio_init+0x9c>
8160af74:	e58d3040 	str	r3, [sp, #64]	; 0x40
8160af78:	e59f3024 	ldr	r3, [pc, #36]	; 8160afa4 <serial_stdio_init+0xa0>
8160af7c:	e58d303c 	str	r3, [sp, #60]	; 0x3c
8160af80:	ebfff72e 	bl	81608c40 <stdio_register>
8160af84:	e594402c 	ldr	r4, [r4, #44]	; 0x2c
8160af88:	eaffffe3 	b	8160af1c <serial_stdio_init+0x18>
8160af8c:	81619e64 	.word	0x81619e64
8160af90:	8160ae58 	.word	0x8160ae58
8160af94:	8160ae64 	.word	0x8160ae64
8160af98:	8160ae70 	.word	0x8160ae70
8160af9c:	8160ae80 	.word	0x8160ae80
8160afa0:	8160ae90 	.word	0x8160ae90
8160afa4:	8160ae9c 	.word	0x8160ae9c

8160afa8 <serial_assign>:
8160afa8:	e59f303c 	ldr	r3, [pc, #60]	; 8160afec <serial_assign+0x44>
8160afac:	e92d4070 	push	{r4, r5, r6, lr}
8160afb0:	e1a05000 	mov	r5, r0
8160afb4:	e5934000 	ldr	r4, [r3]
8160afb8:	e3540000 	cmp	r4, #0
8160afbc:	1a000001 	bne	8160afc8 <serial_assign+0x20>
8160afc0:	e3e00015 	mvn	r0, #21
8160afc4:	e8bd8070 	pop	{r4, r5, r6, pc}
8160afc8:	e1a01005 	mov	r1, r5
8160afcc:	e1a00004 	mov	r0, r4
8160afd0:	eb001a1a 	bl	81611840 <strcmp>
8160afd4:	e3500000 	cmp	r0, #0
8160afd8:	1594402c 	ldrne	r4, [r4, #44]	; 0x2c
8160afdc:	1afffff5 	bne	8160afb8 <serial_assign+0x10>
8160afe0:	e59f3008 	ldr	r3, [pc, #8]	; 8160aff0 <serial_assign+0x48>
8160afe4:	e5834000 	str	r4, [r3]
8160afe8:	e8bd8070 	pop	{r4, r5, r6, pc}
8160afec:	81619e64 	.word	0x81619e64
8160aff0:	81619e60 	.word	0x81619e60

8160aff4 <serial_initialize>:
8160aff4:	e92d4010 	push	{r4, lr}
8160aff8:	ebffffba 	bl	8160aee8 <amirix_serial_initialize>
8160affc:	ebffffb9 	bl	8160aee8 <amirix_serial_initialize>
8160b000:	ebffffb8 	bl	8160aee8 <amirix_serial_initialize>
8160b004:	ebffffb7 	bl	8160aee8 <amirix_serial_initialize>
8160b008:	ebffffb6 	bl	8160aee8 <amirix_serial_initialize>
8160b00c:	ebffffb5 	bl	8160aee8 <amirix_serial_initialize>
8160b010:	ebffffb4 	bl	8160aee8 <amirix_serial_initialize>
8160b014:	ebffffb3 	bl	8160aee8 <amirix_serial_initialize>
8160b018:	ebffffb2 	bl	8160aee8 <amirix_serial_initialize>
8160b01c:	ebffffb1 	bl	8160aee8 <amirix_serial_initialize>
8160b020:	ebffffb0 	bl	8160aee8 <amirix_serial_initialize>
8160b024:	ebffffaf 	bl	8160aee8 <amirix_serial_initialize>
8160b028:	ebffffae 	bl	8160aee8 <amirix_serial_initialize>
8160b02c:	ebffffad 	bl	8160aee8 <amirix_serial_initialize>
8160b030:	ebffffac 	bl	8160aee8 <amirix_serial_initialize>
8160b034:	ebffffab 	bl	8160aee8 <amirix_serial_initialize>
8160b038:	ebffffaa 	bl	8160aee8 <amirix_serial_initialize>
8160b03c:	ebffffa9 	bl	8160aee8 <amirix_serial_initialize>
8160b040:	ebffffa8 	bl	8160aee8 <amirix_serial_initialize>
8160b044:	ebffffa7 	bl	8160aee8 <amirix_serial_initialize>
8160b048:	ebffffa6 	bl	8160aee8 <amirix_serial_initialize>
8160b04c:	ebffffa5 	bl	8160aee8 <amirix_serial_initialize>
8160b050:	ebffffa4 	bl	8160aee8 <amirix_serial_initialize>
8160b054:	ebffffa3 	bl	8160aee8 <amirix_serial_initialize>
8160b058:	ebffffa2 	bl	8160aee8 <amirix_serial_initialize>
8160b05c:	ebffffa1 	bl	8160aee8 <amirix_serial_initialize>
8160b060:	ebffffa0 	bl	8160aee8 <amirix_serial_initialize>
8160b064:	ebffff9f 	bl	8160aee8 <amirix_serial_initialize>
8160b068:	ebffff9e 	bl	8160aee8 <amirix_serial_initialize>
8160b06c:	ebffff9d 	bl	8160aee8 <amirix_serial_initialize>
8160b070:	ebffff9c 	bl	8160aee8 <amirix_serial_initialize>
8160b074:	ebffff9b 	bl	8160aee8 <amirix_serial_initialize>
8160b078:	ebffff9a 	bl	8160aee8 <amirix_serial_initialize>
8160b07c:	ebffff99 	bl	8160aee8 <amirix_serial_initialize>
8160b080:	ebffff98 	bl	8160aee8 <amirix_serial_initialize>
8160b084:	ebffff97 	bl	8160aee8 <amirix_serial_initialize>
8160b088:	ebffff96 	bl	8160aee8 <amirix_serial_initialize>
8160b08c:	ebffff95 	bl	8160aee8 <amirix_serial_initialize>
8160b090:	ebffff94 	bl	8160aee8 <amirix_serial_initialize>
8160b094:	ebffff93 	bl	8160aee8 <amirix_serial_initialize>
8160b098:	ebffff92 	bl	8160aee8 <amirix_serial_initialize>
8160b09c:	ebffff91 	bl	8160aee8 <amirix_serial_initialize>
8160b0a0:	eb0000da 	bl	8160b410 <fh_serial_initialize>
8160b0a4:	eb0000dc 	bl	8160b41c <default_serial_console>
8160b0a8:	e8bd4010 	pop	{r4, lr}
8160b0ac:	eaffffbd 	b	8160afa8 <serial_assign>

8160b0b0 <serial_init>:
8160b0b0:	e5993004 	ldr	r3, [r9, #4]
8160b0b4:	e92d4010 	push	{r4, lr}
8160b0b8:	e3833c01 	orr	r3, r3, #256	; 0x100
8160b0bc:	e5893004 	str	r3, [r9, #4]
8160b0c0:	ebffff78 	bl	8160aea8 <get_current>
8160b0c4:	e8bd4010 	pop	{r4, lr}
8160b0c8:	e5903010 	ldr	r3, [r0, #16]
8160b0cc:	e12fff13 	bx	r3

8160b0d0 <serial_setbrg>:
8160b0d0:	e92d4010 	push	{r4, lr}
8160b0d4:	ebffff73 	bl	8160aea8 <get_current>
8160b0d8:	e8bd4010 	pop	{r4, lr}
8160b0dc:	e5903018 	ldr	r3, [r0, #24]
8160b0e0:	e12fff13 	bx	r3

8160b0e4 <on_baudrate>:
8160b0e4:	e3520001 	cmp	r2, #1
8160b0e8:	e92d4070 	push	{r4, r5, r6, lr}
8160b0ec:	e1a04002 	mov	r4, r2
8160b0f0:	0a000031 	beq	8160b1bc <on_baudrate+0xd8>
8160b0f4:	e1a00001 	mov	r0, r1
8160b0f8:	e1a05003 	mov	r5, r3
8160b0fc:	3a000001 	bcc	8160b108 <on_baudrate+0x24>
8160b100:	e3520002 	cmp	r2, #2
8160b104:	1a000006 	bne	8160b124 <on_baudrate+0x40>
8160b108:	e3a0200a 	mov	r2, #10
8160b10c:	e3a01000 	mov	r1, #0
8160b110:	eb001da0 	bl	81612798 <simple_strtoul>
8160b114:	e5993008 	ldr	r3, [r9, #8]
8160b118:	e1a04000 	mov	r4, r0
8160b11c:	e1500003 	cmp	r0, r3
8160b120:	1a000001 	bne	8160b12c <on_baudrate+0x48>
8160b124:	e3a00000 	mov	r0, #0
8160b128:	e8bd8070 	pop	{r4, r5, r6, pc}
8160b12c:	e3500c4b 	cmp	r0, #19200	; 0x4b00
8160b130:	13500d96 	cmpne	r0, #9600	; 0x2580
8160b134:	e59f308c 	ldr	r3, [pc, #140]	; 8160b1c8 <on_baudrate+0xe4>
8160b138:	03a02001 	moveq	r2, #1
8160b13c:	13a02000 	movne	r2, #0
8160b140:	e3500c96 	cmp	r0, #38400	; 0x9600
8160b144:	03822001 	orreq	r2, r2, #1
8160b148:	e3500ce1 	cmp	r0, #57600	; 0xe100
8160b14c:	03822001 	orreq	r2, r2, #1
8160b150:	e1500003 	cmp	r0, r3
8160b154:	03822001 	orreq	r2, r2, #1
8160b158:	e3520000 	cmp	r2, #0
8160b15c:	1a000006 	bne	8160b17c <on_baudrate+0x98>
8160b160:	e3150002 	tst	r5, #2
8160b164:	1a000002 	bne	8160b174 <on_baudrate+0x90>
8160b168:	e1a01000 	mov	r1, r0
8160b16c:	e59f0058 	ldr	r0, [pc, #88]	; 8160b1cc <on_baudrate+0xe8>
8160b170:	eb001d41 	bl	8161267c <printf>
8160b174:	e3a00001 	mov	r0, #1
8160b178:	e8bd8070 	pop	{r4, r5, r6, pc}
8160b17c:	e2155004 	ands	r5, r5, #4
8160b180:	0a000004 	beq	8160b198 <on_baudrate+0xb4>
8160b184:	e1a01000 	mov	r1, r0
8160b188:	e59f0040 	ldr	r0, [pc, #64]	; 8160b1d0 <on_baudrate+0xec>
8160b18c:	eb001d3a 	bl	8161267c <printf>
8160b190:	e30c0350 	movw	r0, #50000	; 0xc350
8160b194:	eb001a84 	bl	81611bac <udelay>
8160b198:	e5894008 	str	r4, [r9, #8]
8160b19c:	ebffffcb 	bl	8160b0d0 <serial_setbrg>
8160b1a0:	e30c0350 	movw	r0, #50000	; 0xc350
8160b1a4:	eb001a80 	bl	81611bac <udelay>
8160b1a8:	e3550000 	cmp	r5, #0
8160b1ac:	0affffdc 	beq	8160b124 <on_baudrate+0x40>
8160b1b0:	ebffeed2 	bl	81606d00 <getc>
8160b1b4:	e350000d 	cmp	r0, #13
8160b1b8:	eafffffb 	b	8160b1ac <on_baudrate+0xc8>
8160b1bc:	e59f0010 	ldr	r0, [pc, #16]	; 8160b1d4 <on_baudrate+0xf0>
8160b1c0:	eb001d2d 	bl	8161267c <printf>
8160b1c4:	eaffffea 	b	8160b174 <on_baudrate+0x90>
8160b1c8:	0001c200 	.word	0x0001c200
8160b1cc:	8161657a 	.word	0x8161657a
8160b1d0:	8161659c 	.word	0x8161659c
8160b1d4:	816165ce 	.word	0x816165ce

8160b1d8 <serial_getc>:
8160b1d8:	e92d4010 	push	{r4, lr}
8160b1dc:	ebffff31 	bl	8160aea8 <get_current>
8160b1e0:	e8bd4010 	pop	{r4, lr}
8160b1e4:	e590301c 	ldr	r3, [r0, #28]
8160b1e8:	e12fff13 	bx	r3

8160b1ec <serial_tstc>:
8160b1ec:	e92d4010 	push	{r4, lr}
8160b1f0:	ebffff2c 	bl	8160aea8 <get_current>
8160b1f4:	e8bd4010 	pop	{r4, lr}
8160b1f8:	e5903020 	ldr	r3, [r0, #32]
8160b1fc:	e12fff13 	bx	r3

8160b200 <serial_putc>:
8160b200:	e92d4010 	push	{r4, lr}
8160b204:	e1a04000 	mov	r4, r0
8160b208:	ebffff26 	bl	8160aea8 <get_current>
8160b20c:	e5903024 	ldr	r3, [r0, #36]	; 0x24
8160b210:	e1a00004 	mov	r0, r4
8160b214:	e8bd4010 	pop	{r4, lr}
8160b218:	e12fff13 	bx	r3

8160b21c <serial_puts>:
8160b21c:	e92d4010 	push	{r4, lr}
8160b220:	e1a04000 	mov	r4, r0
8160b224:	ebffff1f 	bl	8160aea8 <get_current>
8160b228:	e5903028 	ldr	r3, [r0, #40]	; 0x28
8160b22c:	e1a00004 	mov	r0, r4
8160b230:	e8bd4010 	pop	{r4, lr}
8160b234:	e12fff13 	bx	r3

8160b238 <default_serial_puts>:
8160b238:	e92d4070 	push	{r4, r5, r6, lr}
8160b23c:	e1a04000 	mov	r4, r0
8160b240:	ebffff18 	bl	8160aea8 <get_current>
8160b244:	e2444001 	sub	r4, r4, #1
8160b248:	e1a05000 	mov	r5, r0
8160b24c:	e5f40001 	ldrb	r0, [r4, #1]!
8160b250:	e3500000 	cmp	r0, #0
8160b254:	08bd8070 	popeq	{r4, r5, r6, pc}
8160b258:	e5953024 	ldr	r3, [r5, #36]	; 0x24
8160b25c:	e12fff33 	blx	r3
8160b260:	eafffff9 	b	8160b24c <default_serial_puts+0x14>

8160b264 <fh_serial_setbrg>:
8160b264:	e12fff1e 	bx	lr

8160b268 <Uart_Disable_Irq>:
8160b268:	e5902000 	ldr	r2, [r0]
8160b26c:	e3a00000 	mov	r0, #0
8160b270:	e5923004 	ldr	r3, [r2, #4]
8160b274:	e1c31001 	bic	r1, r3, r1
8160b278:	e5821004 	str	r1, [r2, #4]
8160b27c:	e12fff1e 	bx	lr

8160b280 <Uart_Read_Data>:
8160b280:	e0812002 	add	r2, r1, r2
8160b284:	e1510002 	cmp	r1, r2
8160b288:	1a000001 	bne	8160b294 <Uart_Read_Data+0x14>
8160b28c:	e3a00000 	mov	r0, #0
8160b290:	e12fff1e 	bx	lr
8160b294:	e5903000 	ldr	r3, [r0]
8160b298:	e5933000 	ldr	r3, [r3]
8160b29c:	e4c13001 	strb	r3, [r1], #1
8160b2a0:	eafffff7 	b	8160b284 <Uart_Read_Data+0x4>

8160b2a4 <Uart_Write_Data>:
8160b2a4:	e3510000 	cmp	r1, #0
8160b2a8:	0a000004 	beq	8160b2c0 <Uart_Write_Data+0x1c>
8160b2ac:	e0812002 	add	r2, r1, r2
8160b2b0:	e1520001 	cmp	r2, r1
8160b2b4:	1a000003 	bne	8160b2c8 <Uart_Write_Data+0x24>
8160b2b8:	e3a00000 	mov	r0, #0
8160b2bc:	e12fff1e 	bx	lr
8160b2c0:	e3a00001 	mov	r0, #1
8160b2c4:	e12fff1e 	bx	lr
8160b2c8:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
8160b2cc:	e5903000 	ldr	r3, [r0]
8160b2d0:	e283e07c 	add	lr, r3, #124	; 0x7c
8160b2d4:	e59ec000 	ldr	ip, [lr]
8160b2d8:	e31c0002 	tst	ip, #2
8160b2dc:	0afffffc 	beq	8160b2d4 <Uart_Write_Data+0x30>
8160b2e0:	e4d1c001 	ldrb	ip, [r1], #1
8160b2e4:	e1520001 	cmp	r2, r1
8160b2e8:	e583c000 	str	ip, [r3]
8160b2ec:	1afffff6 	bne	8160b2cc <Uart_Write_Data+0x28>
8160b2f0:	e3a00000 	mov	r0, #0
8160b2f4:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

8160b2f8 <Uart_Init>:
8160b2f8:	e92d4010 	push	{r4, lr}
8160b2fc:	e3a0100f 	mov	r1, #15
8160b300:	ebffffd8 	bl	8160b268 <Uart_Disable_Irq>
8160b304:	e3a00000 	mov	r0, #0
8160b308:	e8bd8010 	pop	{r4, pc}

8160b30c <fh_serial_init>:
8160b30c:	e59f0038 	ldr	r0, [pc, #56]	; 8160b34c <fh_serial_init+0x40>
8160b310:	e59f3038 	ldr	r3, [pc, #56]	; 8160b350 <fh_serial_init+0x44>
8160b314:	e92d4010 	push	{r4, lr}
8160b318:	e5803000 	str	r3, [r0]
8160b31c:	e3a03010 	mov	r3, #16
8160b320:	e5803004 	str	r3, [r0, #4]
8160b324:	e3a03003 	mov	r3, #3
8160b328:	e5803008 	str	r3, [r0, #8]
8160b32c:	e3a03041 	mov	r3, #65	; 0x41
8160b330:	e580300c 	str	r3, [r0, #12]
8160b334:	ebffffef 	bl	8160b2f8 <Uart_Init>
8160b338:	e59f3014 	ldr	r3, [pc, #20]	; 8160b354 <fh_serial_init+0x48>
8160b33c:	e3a02001 	mov	r2, #1
8160b340:	e3a00000 	mov	r0, #0
8160b344:	e5c32000 	strb	r2, [r3]
8160b348:	e8bd8010 	pop	{r4, pc}
8160b34c:	81619e68 	.word	0x81619e68
8160b350:	13030000 	.word	0x13030000
8160b354:	81619e78 	.word	0x81619e78

8160b358 <fh_serial_getc>:
8160b358:	e59f302c 	ldr	r3, [pc, #44]	; 8160b38c <fh_serial_getc+0x34>
8160b35c:	e92d4007 	push	{r0, r1, r2, lr}
8160b360:	e5d33000 	ldrb	r3, [r3]
8160b364:	e3530000 	cmp	r3, #0
8160b368:	1a000000 	bne	8160b370 <fh_serial_getc+0x18>
8160b36c:	ebffffe6 	bl	8160b30c <fh_serial_init>
8160b370:	e3a02001 	mov	r2, #1
8160b374:	e28d1007 	add	r1, sp, #7
8160b378:	e59f0010 	ldr	r0, [pc, #16]	; 8160b390 <fh_serial_getc+0x38>
8160b37c:	ebffffbf 	bl	8160b280 <Uart_Read_Data>
8160b380:	e5dd0007 	ldrb	r0, [sp, #7]
8160b384:	e28dd00c 	add	sp, sp, #12
8160b388:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8160b38c:	81619e78 	.word	0x81619e78
8160b390:	81619e68 	.word	0x81619e68

8160b394 <fh_serial_putc>:
8160b394:	e59f3040 	ldr	r3, [pc, #64]	; 8160b3dc <fh_serial_putc+0x48>
8160b398:	e92d4007 	push	{r0, r1, r2, lr}
8160b39c:	e5d33000 	ldrb	r3, [r3]
8160b3a0:	e5cd0007 	strb	r0, [sp, #7]
8160b3a4:	e3530000 	cmp	r3, #0
8160b3a8:	1a000000 	bne	8160b3b0 <fh_serial_putc+0x1c>
8160b3ac:	ebffffd6 	bl	8160b30c <fh_serial_init>
8160b3b0:	e3a02001 	mov	r2, #1
8160b3b4:	e28d1007 	add	r1, sp, #7
8160b3b8:	e59f0020 	ldr	r0, [pc, #32]	; 8160b3e0 <fh_serial_putc+0x4c>
8160b3bc:	ebffffb8 	bl	8160b2a4 <Uart_Write_Data>
8160b3c0:	e5dd3007 	ldrb	r3, [sp, #7]
8160b3c4:	e353000a 	cmp	r3, #10
8160b3c8:	1a000001 	bne	8160b3d4 <fh_serial_putc+0x40>
8160b3cc:	e3a0000d 	mov	r0, #13
8160b3d0:	ebffff8a 	bl	8160b200 <serial_putc>
8160b3d4:	e28dd00c 	add	sp, sp, #12
8160b3d8:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8160b3dc:	81619e78 	.word	0x81619e78
8160b3e0:	81619e68 	.word	0x81619e68

8160b3e4 <fh_serial_tstc>:
8160b3e4:	e59f3020 	ldr	r3, [pc, #32]	; 8160b40c <fh_serial_tstc+0x28>
8160b3e8:	e5d33000 	ldrb	r3, [r3]
8160b3ec:	e3530000 	cmp	r3, #0
8160b3f0:	1a000003 	bne	8160b404 <fh_serial_tstc+0x20>
8160b3f4:	e92d4010 	push	{r4, lr}
8160b3f8:	ebffffc3 	bl	8160b30c <fh_serial_init>
8160b3fc:	e3a00001 	mov	r0, #1
8160b400:	e8bd8010 	pop	{r4, pc}
8160b404:	e3a00001 	mov	r0, #1
8160b408:	e12fff1e 	bx	lr
8160b40c:	81619e78 	.word	0x81619e78

8160b410 <fh_serial_initialize>:
8160b410:	e59f0000 	ldr	r0, [pc]	; 8160b418 <fh_serial_initialize+0x8>
8160b414:	eafffeb4 	b	8160aeec <serial_register>
8160b418:	816190c0 	.word	0x816190c0

8160b41c <default_serial_console>:
8160b41c:	e59f0000 	ldr	r0, [pc]	; 8160b424 <default_serial_console+0x8>
8160b420:	e12fff1e 	bx	lr
8160b424:	816190c0 	.word	0x816190c0

8160b428 <env_get_yesno>:
8160b428:	e92d4010 	push	{r4, lr}
8160b42c:	ebffe459 	bl	81604598 <env_get>
8160b430:	e3500000 	cmp	r0, #0
8160b434:	0a000008 	beq	8160b45c <env_get_yesno+0x34>
8160b438:	e5d03000 	ldrb	r3, [r0]
8160b43c:	e20300df 	and	r0, r3, #223	; 0xdf
8160b440:	e3500059 	cmp	r0, #89	; 0x59
8160b444:	13500054 	cmpne	r0, #84	; 0x54
8160b448:	03a00001 	moveq	r0, #1
8160b44c:	13a00000 	movne	r0, #0
8160b450:	e3530031 	cmp	r3, #49	; 0x31
8160b454:	03800001 	orreq	r0, r0, #1
8160b458:	e8bd8010 	pop	{r4, pc}
8160b45c:	e3e00000 	mvn	r0, #0
8160b460:	e8bd8010 	pop	{r4, pc}

8160b464 <env_get_default>:
8160b464:	e92d4070 	push	{r4, r5, r6, lr}
8160b468:	e5995024 	ldr	r5, [r9, #36]	; 0x24
8160b46c:	e5994004 	ldr	r4, [r9, #4]
8160b470:	e5992004 	ldr	r2, [r9, #4]
8160b474:	e3c22080 	bic	r2, r2, #128	; 0x80
8160b478:	e5892004 	str	r2, [r9, #4]
8160b47c:	e3a02000 	mov	r2, #0
8160b480:	e5892024 	str	r2, [r9, #36]	; 0x24
8160b484:	ebffe443 	bl	81604598 <env_get>
8160b488:	e5895024 	str	r5, [r9, #36]	; 0x24
8160b48c:	e5894004 	str	r4, [r9, #4]
8160b490:	e8bd8070 	pop	{r4, r5, r6, pc}

8160b494 <set_default_env>:
8160b494:	e92d401f 	push	{r0, r1, r2, r3, r4, lr}
8160b498:	e2504000 	subs	r4, r0, #0
8160b49c:	0a000024 	beq	8160b534 <set_default_env+0xa0>
8160b4a0:	e5d43000 	ldrb	r3, [r4]
8160b4a4:	e3530021 	cmp	r3, #33	; 0x21
8160b4a8:	1a00001e 	bne	8160b528 <set_default_env+0x94>
8160b4ac:	e2841001 	add	r1, r4, #1
8160b4b0:	e59f0088 	ldr	r0, [pc, #136]	; 8160b540 <set_default_env+0xac>
8160b4b4:	eb001c70 	bl	8161267c <printf>
8160b4b8:	e3a04000 	mov	r4, #0
8160b4bc:	e3a03000 	mov	r3, #0
8160b4c0:	e58d4000 	str	r4, [sp]
8160b4c4:	e58d300c 	str	r3, [sp, #12]
8160b4c8:	e300215d 	movw	r2, #349	; 0x15d
8160b4cc:	e58d3008 	str	r3, [sp, #8]
8160b4d0:	e58d3004 	str	r3, [sp, #4]
8160b4d4:	e59f1068 	ldr	r1, [pc, #104]	; 8160b544 <set_default_env+0xb0>
8160b4d8:	e59f0068 	ldr	r0, [pc, #104]	; 8160b548 <set_default_env+0xb4>
8160b4dc:	eb00162b 	bl	81610d90 <himport_r>
8160b4e0:	e3500000 	cmp	r0, #0
8160b4e4:	1a000007 	bne	8160b508 <set_default_env+0x74>
8160b4e8:	e59f305c 	ldr	r3, [pc, #92]	; 8160b54c <set_default_env+0xb8>
8160b4ec:	e59f105c 	ldr	r1, [pc, #92]	; 8160b550 <set_default_env+0xbc>
8160b4f0:	e59f205c 	ldr	r2, [pc, #92]	; 8160b554 <set_default_env+0xc0>
8160b4f4:	e58d3000 	str	r3, [sp]
8160b4f8:	e3a03057 	mov	r3, #87	; 0x57
8160b4fc:	e5911000 	ldr	r1, [r1]
8160b500:	e59f0050 	ldr	r0, [pc, #80]	; 8160b558 <set_default_env+0xc4>
8160b504:	eb001c5c 	bl	8161267c <printf>
8160b508:	e5992004 	ldr	r2, [r9, #4]
8160b50c:	e3822080 	orr	r2, r2, #128	; 0x80
8160b510:	e5892004 	str	r2, [r9, #4]
8160b514:	e5992004 	ldr	r2, [r9, #4]
8160b518:	e3822a02 	orr	r2, r2, #8192	; 0x2000
8160b51c:	e5892004 	str	r2, [r9, #4]
8160b520:	e28dd010 	add	sp, sp, #16
8160b524:	e8bd8010 	pop	{r4, pc}
8160b528:	ebffee19 	bl	81606d94 <puts>
8160b52c:	e3a04004 	mov	r4, #4
8160b530:	eaffffe1 	b	8160b4bc <set_default_env+0x28>
8160b534:	e59f0020 	ldr	r0, [pc, #32]	; 8160b55c <set_default_env+0xc8>
8160b538:	ebffee15 	bl	81606d94 <puts>
8160b53c:	eaffffde 	b	8160b4bc <set_default_env+0x28>
8160b540:	81616707 	.word	0x81616707
8160b544:	816149a0 	.word	0x816149a0
8160b548:	816190f0 	.word	0x816190f0
8160b54c:	81614086 	.word	0x81614086
8160b550:	81619e84 	.word	0x81619e84
8160b554:	81616751 	.word	0x81616751
8160b558:	81615207 	.word	0x81615207
8160b55c:	81616735 	.word	0x81616735

8160b560 <set_default_vars>:
8160b560:	e92d401f 	push	{r0, r1, r2, r3, r4, lr}
8160b564:	e3a03000 	mov	r3, #0
8160b568:	e58d100c 	str	r1, [sp, #12]
8160b56c:	e3a02005 	mov	r2, #5
8160b570:	e58d0008 	str	r0, [sp, #8]
8160b574:	e58d2000 	str	r2, [sp]
8160b578:	e300215d 	movw	r2, #349	; 0x15d
8160b57c:	e58d3004 	str	r3, [sp, #4]
8160b580:	e59f100c 	ldr	r1, [pc, #12]	; 8160b594 <set_default_vars+0x34>
8160b584:	e59f000c 	ldr	r0, [pc, #12]	; 8160b598 <set_default_vars+0x38>
8160b588:	eb001600 	bl	81610d90 <himport_r>
8160b58c:	e28dd014 	add	sp, sp, #20
8160b590:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8160b594:	816149a0 	.word	0x816149a0
8160b598:	816190f0 	.word	0x816190f0

8160b59c <env_relocate>:
8160b59c:	e5990024 	ldr	r0, [r9, #36]	; 0x24
8160b5a0:	e3500000 	cmp	r0, #0
8160b5a4:	1a000000 	bne	8160b5ac <env_relocate+0x10>
8160b5a8:	eaffffb9 	b	8160b494 <set_default_env>
8160b5ac:	ea000058 	b	8160b714 <env_load>

8160b5b0 <env_complete>:
8160b5b0:	e92d4dff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, sl, fp, lr}
8160b5b4:	e3a05000 	mov	r5, #0
8160b5b8:	e59d7030 	ldr	r7, [sp, #48]	; 0x30
8160b5bc:	e1a0b000 	mov	fp, r0
8160b5c0:	e1a06002 	mov	r6, r2
8160b5c4:	e1a0a003 	mov	sl, r3
8160b5c8:	e1a08002 	mov	r8, r2
8160b5cc:	e1a04005 	mov	r4, r5
8160b5d0:	e58d1000 	str	r1, [sp]
8160b5d4:	e5825000 	str	r5, [r2]
8160b5d8:	e1a01005 	mov	r1, r5
8160b5dc:	e59f309c 	ldr	r3, [pc, #156]	; 8160b680 <env_complete+0xd0>
8160b5e0:	e28d200c 	add	r2, sp, #12
8160b5e4:	e1a0000b 	mov	r0, fp
8160b5e8:	eb00139e 	bl	81610468 <hmatch_r>
8160b5ec:	e2505000 	subs	r5, r0, #0
8160b5f0:	1a00000d 	bne	8160b62c <env_complete+0x7c>
8160b5f4:	e59f3088 	ldr	r3, [pc, #136]	; 8160b684 <env_complete+0xd4>
8160b5f8:	e1a01004 	mov	r1, r4
8160b5fc:	e1a00006 	mov	r0, r6
8160b600:	e3a02004 	mov	r2, #4
8160b604:	eb000d9b 	bl	8160ec78 <qsort>
8160b608:	e3550000 	cmp	r5, #0
8160b60c:	12844001 	addne	r4, r4, #1
8160b610:	159f3070 	ldrne	r3, [pc, #112]	; 8160b688 <env_complete+0xd8>
8160b614:	e1a00004 	mov	r0, r4
8160b618:	15883000 	strne	r3, [r8]
8160b61c:	e3a03000 	mov	r3, #0
8160b620:	e7863104 	str	r3, [r6, r4, lsl #2]
8160b624:	e28dd010 	add	sp, sp, #16
8160b628:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160b62c:	e59d300c 	ldr	r3, [sp, #12]
8160b630:	e5930000 	ldr	r0, [r3]
8160b634:	eb0018a6 	bl	816118d4 <strlen>
8160b638:	e59d2000 	ldr	r2, [sp]
8160b63c:	e2422002 	sub	r2, r2, #2
8160b640:	e2803001 	add	r3, r0, #1
8160b644:	e15a0003 	cmp	sl, r3
8160b648:	a1520004 	cmpge	r2, r4
8160b64c:	daffffe8 	ble	8160b5f4 <env_complete+0x44>
8160b650:	e59d100c 	ldr	r1, [sp, #12]
8160b654:	e1a02003 	mov	r2, r3
8160b658:	e4887004 	str	r7, [r8], #4
8160b65c:	e1a00007 	mov	r0, r7
8160b660:	e58d3004 	str	r3, [sp, #4]
8160b664:	e2844001 	add	r4, r4, #1
8160b668:	e5911000 	ldr	r1, [r1]
8160b66c:	ebffd44a 	bl	8160079c <memcpy>
8160b670:	e59d3004 	ldr	r3, [sp, #4]
8160b674:	e0877003 	add	r7, r7, r3
8160b678:	e04aa003 	sub	sl, sl, r3
8160b67c:	eaffffd5 	b	8160b5d8 <env_complete+0x28>
8160b680:	816190f0 	.word	0x816190f0
8160b684:	8160ed64 	.word	0x8160ed64
8160b688:	816145f2 	.word	0x816145f2

8160b68c <env_driver_lookup_default>:
8160b68c:	e59f0024 	ldr	r0, [pc, #36]	; 8160b6b8 <env_driver_lookup_default+0x2c>
8160b690:	e59f3024 	ldr	r3, [pc, #36]	; 8160b6bc <env_driver_lookup_default+0x30>
8160b694:	e1500003 	cmp	r0, r3
8160b698:	1a000001 	bne	8160b6a4 <env_driver_lookup_default+0x18>
8160b69c:	e3a00000 	mov	r0, #0
8160b6a0:	e12fff1e 	bx	lr
8160b6a4:	e5902004 	ldr	r2, [r0, #4]
8160b6a8:	e352000c 	cmp	r2, #12
8160b6ac:	012fff1e 	bxeq	lr
8160b6b0:	e2800018 	add	r0, r0, #24
8160b6b4:	eafffff6 	b	8160b694 <env_driver_lookup_default+0x8>
8160b6b8:	81619448 	.word	0x81619448
8160b6bc:	81619460 	.word	0x81619460

8160b6c0 <env_get_char>:
8160b6c0:	e92d4010 	push	{r4, lr}
8160b6c4:	e1a01000 	mov	r1, r0
8160b6c8:	ebffffef 	bl	8160b68c <env_driver_lookup_default>
8160b6cc:	e5993024 	ldr	r3, [r9, #36]	; 0x24
8160b6d0:	e3530000 	cmp	r3, #0
8160b6d4:	059f3034 	ldreq	r3, [pc, #52]	; 8160b710 <env_get_char+0x50>
8160b6d8:	0a000005 	beq	8160b6f4 <env_get_char+0x34>
8160b6dc:	e3500000 	cmp	r0, #0
8160b6e0:	0a000008 	beq	8160b708 <env_get_char+0x48>
8160b6e4:	e5903008 	ldr	r3, [r0, #8]
8160b6e8:	e3530000 	cmp	r3, #0
8160b6ec:	1a000002 	bne	8160b6fc <env_get_char+0x3c>
8160b6f0:	e5993020 	ldr	r3, [r9, #32]
8160b6f4:	e7d10003 	ldrb	r0, [r1, r3]
8160b6f8:	e8bd8010 	pop	{r4, pc}
8160b6fc:	e1a00001 	mov	r0, r1
8160b700:	e8bd4010 	pop	{r4, lr}
8160b704:	e12fff13 	bx	r3
8160b708:	e3e00012 	mvn	r0, #18
8160b70c:	e8bd8010 	pop	{r4, pc}
8160b710:	816149a0 	.word	0x816149a0

8160b714 <env_load>:
8160b714:	e92d4010 	push	{r4, lr}
8160b718:	ebffffdb 	bl	8160b68c <env_driver_lookup_default>
8160b71c:	e3500000 	cmp	r0, #0
8160b720:	0a000004 	beq	8160b738 <env_load+0x24>
8160b724:	e590000c 	ldr	r0, [r0, #12]
8160b728:	e3500000 	cmp	r0, #0
8160b72c:	08bd8010 	popeq	{r4, pc}
8160b730:	e8bd4010 	pop	{r4, lr}
8160b734:	e12fff10 	bx	r0
8160b738:	e3e00012 	mvn	r0, #18
8160b73c:	e8bd8010 	pop	{r4, pc}

8160b740 <env_init>:
8160b740:	e92d4010 	push	{r4, lr}
8160b744:	ebffffd0 	bl	8160b68c <env_driver_lookup_default>
8160b748:	e3500000 	cmp	r0, #0
8160b74c:	0a00000c 	beq	8160b784 <env_init+0x44>
8160b750:	e5903014 	ldr	r3, [r0, #20]
8160b754:	e3530000 	cmp	r3, #0
8160b758:	1a000005 	bne	8160b774 <env_init+0x34>
8160b75c:	e59f2028 	ldr	r2, [pc, #40]	; 8160b78c <env_init+0x4c>
8160b760:	e3a00000 	mov	r0, #0
8160b764:	e5892020 	str	r2, [r9, #32]
8160b768:	e3a02001 	mov	r2, #1
8160b76c:	e5892024 	str	r2, [r9, #36]	; 0x24
8160b770:	e8bd8010 	pop	{r4, pc}
8160b774:	e12fff33 	blx	r3
8160b778:	e3700002 	cmn	r0, #2
8160b77c:	18bd8010 	popne	{r4, pc}
8160b780:	eafffff5 	b	8160b75c <env_init+0x1c>
8160b784:	e3e00012 	mvn	r0, #18
8160b788:	e8bd8010 	pop	{r4, pc}
8160b78c:	816149a0 	.word	0x816149a0

8160b790 <env_attr_walk>:
8160b790:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160b794:	e2504000 	subs	r4, r0, #0
8160b798:	e1a08001 	mov	r8, r1
8160b79c:	e1a0a002 	mov	sl, r2
8160b7a0:	03a05001 	moveq	r5, #1
8160b7a4:	0a00000d 	beq	8160b7e0 <env_attr_walk+0x50>
8160b7a8:	e3a0102c 	mov	r1, #44	; 0x2c
8160b7ac:	e1a00004 	mov	r0, r4
8160b7b0:	eb00183b 	bl	816118a4 <strchr>
8160b7b4:	e2506000 	subs	r6, r0, #0
8160b7b8:	1a000025 	bne	8160b854 <env_attr_walk+0xc4>
8160b7bc:	e1a00004 	mov	r0, r4
8160b7c0:	eb001843 	bl	816118d4 <strlen>
8160b7c4:	e2505000 	subs	r5, r0, #0
8160b7c8:	0a00001f 	beq	8160b84c <env_attr_walk+0xbc>
8160b7cc:	e2850001 	add	r0, r5, #1
8160b7d0:	ebffef23 	bl	81607464 <malloc>
8160b7d4:	e2507000 	subs	r7, r0, #0
8160b7d8:	1a000002 	bne	8160b7e8 <env_attr_walk+0x58>
8160b7dc:	e3e0500b 	mvn	r5, #11
8160b7e0:	e1a00005 	mov	r0, r5
8160b7e4:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160b7e8:	e1a01004 	mov	r1, r4
8160b7ec:	eb001804 	bl	81611804 <strcpy>
8160b7f0:	e3a0103a 	mov	r1, #58	; 0x3a
8160b7f4:	e1a00007 	mov	r0, r7
8160b7f8:	eb001829 	bl	816118a4 <strchr>
8160b7fc:	e2504000 	subs	r4, r0, #0
8160b800:	0a000004 	beq	8160b818 <env_attr_walk+0x88>
8160b804:	e3a03000 	mov	r3, #0
8160b808:	e2840001 	add	r0, r4, #1
8160b80c:	e5c43000 	strb	r3, [r4]
8160b810:	eb0017e4 	bl	816117a8 <strim>
8160b814:	e1a04000 	mov	r4, r0
8160b818:	e1a00007 	mov	r0, r7
8160b81c:	eb0017e1 	bl	816117a8 <strim>
8160b820:	e1a05000 	mov	r5, r0
8160b824:	eb00182a 	bl	816118d4 <strlen>
8160b828:	e3500000 	cmp	r0, #0
8160b82c:	0a000014 	beq	8160b884 <env_attr_walk+0xf4>
8160b830:	e1a00005 	mov	r0, r5
8160b834:	e1a0200a 	mov	r2, sl
8160b838:	e1a01004 	mov	r1, r4
8160b83c:	e12fff38 	blx	r8
8160b840:	e2505000 	subs	r5, r0, #0
8160b844:	0a00000e 	beq	8160b884 <env_attr_walk+0xf4>
8160b848:	e1a00007 	mov	r0, r7
8160b84c:	ebffee79 	bl	81607238 <free>
8160b850:	eaffffe2 	b	8160b7e0 <env_attr_walk+0x50>
8160b854:	e0565004 	subs	r5, r6, r4
8160b858:	0a000010 	beq	8160b8a0 <env_attr_walk+0x110>
8160b85c:	e2850001 	add	r0, r5, #1
8160b860:	ebffeeff 	bl	81607464 <malloc>
8160b864:	e2507000 	subs	r7, r0, #0
8160b868:	0affffdb 	beq	8160b7dc <env_attr_walk+0x4c>
8160b86c:	e1a02005 	mov	r2, r5
8160b870:	e1a01004 	mov	r1, r4
8160b874:	eb0017e8 	bl	8161181c <strncpy>
8160b878:	e3a03000 	mov	r3, #0
8160b87c:	e7c73005 	strb	r3, [r7, r5]
8160b880:	eaffffda 	b	8160b7f0 <env_attr_walk+0x60>
8160b884:	e1a00007 	mov	r0, r7
8160b888:	e2864001 	add	r4, r6, #1
8160b88c:	ebffee69 	bl	81607238 <free>
8160b890:	e3560000 	cmp	r6, #0
8160b894:	1affffc3 	bne	8160b7a8 <env_attr_walk+0x18>
8160b898:	e1a05006 	mov	r5, r6
8160b89c:	eaffffcf 	b	8160b7e0 <env_attr_walk+0x50>
8160b8a0:	e1a00005 	mov	r0, r5
8160b8a4:	e2864001 	add	r4, r6, #1
8160b8a8:	ebffee62 	bl	81607238 <free>
8160b8ac:	eaffffbd 	b	8160b7a8 <env_attr_walk+0x18>

8160b8b0 <env_attr_lookup>:
8160b8b0:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160b8b4:	e1a06002 	mov	r6, r2
8160b8b8:	e16f2f12 	clz	r2, r2
8160b8bc:	e3500000 	cmp	r0, #0
8160b8c0:	e1a022a2 	lsr	r2, r2, #5
8160b8c4:	03a04001 	moveq	r4, #1
8160b8c8:	11a04002 	movne	r4, r2
8160b8cc:	e3540000 	cmp	r4, #0
8160b8d0:	13e00015 	mvnne	r0, #21
8160b8d4:	1a000054 	bne	8160ba2c <env_attr_lookup+0x17c>
8160b8d8:	e5d13000 	ldrb	r3, [r1]
8160b8dc:	e1a05000 	mov	r5, r0
8160b8e0:	e1a07001 	mov	r7, r1
8160b8e4:	e3530000 	cmp	r3, #0
8160b8e8:	1a00001d 	bne	8160b964 <env_attr_lookup+0xb4>
8160b8ec:	eb0017f8 	bl	816118d4 <strlen>
8160b8f0:	e1a04005 	mov	r4, r5
8160b8f4:	e1a0b000 	mov	fp, r0
8160b8f8:	e084400b 	add	r4, r4, fp
8160b8fc:	e4d42001 	ldrb	r2, [r4], #1
8160b900:	e3520020 	cmp	r2, #32
8160b904:	0afffffc 	beq	8160b8fc <env_attr_lookup+0x4c>
8160b908:	e352003a 	cmp	r2, #58	; 0x3a
8160b90c:	13a05000 	movne	r5, #0
8160b910:	1a000010 	bne	8160b958 <env_attr_lookup+0xa8>
8160b914:	e5d43000 	ldrb	r3, [r4]
8160b918:	e2842001 	add	r2, r4, #1
8160b91c:	e3530020 	cmp	r3, #32
8160b920:	0a000043 	beq	8160ba34 <env_attr_lookup+0x184>
8160b924:	e59f1110 	ldr	r1, [pc, #272]	; 8160ba3c <env_attr_lookup+0x18c>
8160b928:	e1a00004 	mov	r0, r4
8160b92c:	eb00180a 	bl	8161195c <strpbrk>
8160b930:	e3500000 	cmp	r0, #0
8160b934:	10405004 	subne	r5, r0, r4
8160b938:	1a000002 	bne	8160b948 <env_attr_lookup+0x98>
8160b93c:	e1a00004 	mov	r0, r4
8160b940:	eb0017e3 	bl	816118d4 <strlen>
8160b944:	e1a05000 	mov	r5, r0
8160b948:	e1a02005 	mov	r2, r5
8160b94c:	e1a01004 	mov	r1, r4
8160b950:	e1a00006 	mov	r0, r6
8160b954:	ebffd390 	bl	8160079c <memcpy>
8160b958:	e3a00000 	mov	r0, #0
8160b95c:	e7c60005 	strb	r0, [r6, r5]
8160b960:	ea000031 	b	8160ba2c <env_attr_lookup+0x17c>
8160b964:	e2408001 	sub	r8, r0, #1
8160b968:	e1a02000 	mov	r2, r0
8160b96c:	e1a0b004 	mov	fp, r4
8160b970:	e1a01007 	mov	r1, r7
8160b974:	e1a00002 	mov	r0, r2
8160b978:	eb00182d 	bl	81611a34 <strstr>
8160b97c:	e250a000 	subs	sl, r0, #0
8160b980:	0a000026 	beq	8160ba20 <env_attr_lookup+0x170>
8160b984:	e1a00007 	mov	r0, r7
8160b988:	e24a3001 	sub	r3, sl, #1
8160b98c:	e58d3004 	str	r3, [sp, #4]
8160b990:	eb0017cf 	bl	816118d4 <strlen>
8160b994:	e59d3004 	ldr	r3, [sp, #4]
8160b998:	e5d31000 	ldrb	r1, [r3]
8160b99c:	e3510020 	cmp	r1, #32
8160b9a0:	0a00001a 	beq	8160ba10 <env_attr_lookup+0x160>
8160b9a4:	e2400001 	sub	r0, r0, #1
8160b9a8:	e08a0000 	add	r0, sl, r0
8160b9ac:	e5f0c001 	ldrb	ip, [r0, #1]!
8160b9b0:	e35c0020 	cmp	ip, #32
8160b9b4:	0afffffc 	beq	8160b9ac <env_attr_lookup+0xfc>
8160b9b8:	e351002c 	cmp	r1, #44	; 0x2c
8160b9bc:	1155000a 	cmpne	r5, sl
8160b9c0:	e28a2001 	add	r2, sl, #1
8160b9c4:	0a000001 	beq	8160b9d0 <env_attr_lookup+0x120>
8160b9c8:	e1530008 	cmp	r3, r8
8160b9cc:	1affffe7 	bne	8160b970 <env_attr_lookup+0xc0>
8160b9d0:	e35c003a 	cmp	ip, #58	; 0x3a
8160b9d4:	135c002c 	cmpne	ip, #44	; 0x2c
8160b9d8:	13a03001 	movne	r3, #1
8160b9dc:	03a03000 	moveq	r3, #0
8160b9e0:	e35c0000 	cmp	ip, #0
8160b9e4:	11a0c003 	movne	ip, r3
8160b9e8:	03a0c000 	moveq	ip, #0
8160b9ec:	e35c0000 	cmp	ip, #0
8160b9f0:	1affffde 	bne	8160b970 <env_attr_lookup+0xc0>
8160b9f4:	e1a00007 	mov	r0, r7
8160b9f8:	e58d2004 	str	r2, [sp, #4]
8160b9fc:	eb0017b4 	bl	816118d4 <strlen>
8160ba00:	e1a0400a 	mov	r4, sl
8160ba04:	e59d2004 	ldr	r2, [sp, #4]
8160ba08:	e1a0b000 	mov	fp, r0
8160ba0c:	eaffffd7 	b	8160b970 <env_attr_lookup+0xc0>
8160ba10:	e1550003 	cmp	r5, r3
8160ba14:	8affffe2 	bhi	8160b9a4 <env_attr_lookup+0xf4>
8160ba18:	e2433001 	sub	r3, r3, #1
8160ba1c:	eaffffdd 	b	8160b998 <env_attr_lookup+0xe8>
8160ba20:	e3540000 	cmp	r4, #0
8160ba24:	03e00001 	mvneq	r0, #1
8160ba28:	1affffb2 	bne	8160b8f8 <env_attr_lookup+0x48>
8160ba2c:	e28dd008 	add	sp, sp, #8
8160ba30:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160ba34:	e1a04002 	mov	r4, r2
8160ba38:	eaffffb5 	b	8160b914 <env_attr_lookup+0x64>
8160ba3c:	81614afd 	.word	0x81614afd

8160ba40 <clear_callback>:
8160ba40:	e3a03000 	mov	r3, #0
8160ba44:	e5803008 	str	r3, [r0, #8]
8160ba48:	e1a00003 	mov	r0, r3
8160ba4c:	e12fff1e 	bx	lr

8160ba50 <find_env_callback>:
8160ba50:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160ba54:	e2507000 	subs	r7, r0, #0
8160ba58:	e59f6044 	ldr	r6, [pc, #68]	; 8160baa4 <find_env_callback+0x54>
8160ba5c:	e59f5044 	ldr	r5, [pc, #68]	; 8160baa8 <find_env_callback+0x58>
8160ba60:	13a04000 	movne	r4, #0
8160ba64:	e0455006 	sub	r5, r5, r6
8160ba68:	11a051c5 	asrne	r5, r5, #3
8160ba6c:	1a000008 	bne	8160ba94 <find_env_callback+0x44>
8160ba70:	e3a08000 	mov	r8, #0
8160ba74:	e1a00008 	mov	r0, r8
8160ba78:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160ba7c:	e7961184 	ldr	r1, [r6, r4, lsl #3]
8160ba80:	e1a00007 	mov	r0, r7
8160ba84:	eb00176d 	bl	81611840 <strcmp>
8160ba88:	e3500000 	cmp	r0, #0
8160ba8c:	0afffff8 	beq	8160ba74 <find_env_callback+0x24>
8160ba90:	e2844001 	add	r4, r4, #1
8160ba94:	e1550004 	cmp	r5, r4
8160ba98:	e0868184 	add	r8, r6, r4, lsl #3
8160ba9c:	cafffff6 	bgt	8160ba7c <find_env_callback+0x2c>
8160baa0:	eafffff2 	b	8160ba70 <find_env_callback+0x20>
8160baa4:	81619420 	.word	0x81619420
8160baa8:	81619448 	.word	0x81619448

8160baac <on_callbacks>:
8160baac:	e92d4070 	push	{r4, r5, r6, lr}
8160bab0:	e1a05001 	mov	r5, r1
8160bab4:	e59f4030 	ldr	r4, [pc, #48]	; 8160baec <on_callbacks+0x40>
8160bab8:	e59f1030 	ldr	r1, [pc, #48]	; 8160baf0 <on_callbacks+0x44>
8160babc:	e59f0030 	ldr	r0, [pc, #48]	; 8160baf4 <on_callbacks+0x48>
8160bac0:	eb0015af 	bl	81611184 <hwalk_r>
8160bac4:	e1a01004 	mov	r1, r4
8160bac8:	e3a02000 	mov	r2, #0
8160bacc:	e59f0024 	ldr	r0, [pc, #36]	; 8160baf8 <on_callbacks+0x4c>
8160bad0:	ebffff2e 	bl	8160b790 <env_attr_walk>
8160bad4:	e3a02000 	mov	r2, #0
8160bad8:	e1a01004 	mov	r1, r4
8160badc:	e1a00005 	mov	r0, r5
8160bae0:	ebffff2a 	bl	8160b790 <env_attr_walk>
8160bae4:	e3a00000 	mov	r0, #0
8160bae8:	e8bd8070 	pop	{r4, r5, r6, pc}
8160baec:	8160bafc 	.word	0x8160bafc
8160baf0:	8160ba40 	.word	0x8160ba40
8160baf4:	816190f0 	.word	0x816190f0
8160baf8:	816165ee 	.word	0x816165ee

8160bafc <set_callback>:
8160bafc:	e92d4010 	push	{r4, lr}
8160bb00:	e24dd028 	sub	sp, sp, #40	; 0x28
8160bb04:	e59f2088 	ldr	r2, [pc, #136]	; 8160bb94 <set_callback+0x98>
8160bb08:	e3a03000 	mov	r3, #0
8160bb0c:	e58d301c 	str	r3, [sp, #28]
8160bb10:	e1a04001 	mov	r4, r1
8160bb14:	e58d3020 	str	r3, [sp, #32]
8160bb18:	e58d300c 	str	r3, [sp, #12]
8160bb1c:	e58d3000 	str	r3, [sp]
8160bb20:	e28d3028 	add	r3, sp, #40	; 0x28
8160bb24:	e58d2008 	str	r2, [sp, #8]
8160bb28:	e28d2014 	add	r2, sp, #20
8160bb2c:	e58d0018 	str	r0, [sp, #24]
8160bb30:	e58d2004 	str	r2, [sp, #4]
8160bb34:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
8160bb38:	eb001271 	bl	81610504 <hsearch_r>
8160bb3c:	e59d3014 	ldr	r3, [sp, #20]
8160bb40:	e3530000 	cmp	r3, #0
8160bb44:	0a000004 	beq	8160bb5c <set_callback+0x60>
8160bb48:	e3540000 	cmp	r4, #0
8160bb4c:	1a000005 	bne	8160bb68 <set_callback+0x6c>
8160bb50:	e59d3014 	ldr	r3, [sp, #20]
8160bb54:	e3a02000 	mov	r2, #0
8160bb58:	e5832008 	str	r2, [r3, #8]
8160bb5c:	e3a00000 	mov	r0, #0
8160bb60:	e28dd028 	add	sp, sp, #40	; 0x28
8160bb64:	e8bd8010 	pop	{r4, pc}
8160bb68:	e1a00004 	mov	r0, r4
8160bb6c:	eb001758 	bl	816118d4 <strlen>
8160bb70:	e3500000 	cmp	r0, #0
8160bb74:	0afffff5 	beq	8160bb50 <set_callback+0x54>
8160bb78:	e1a00004 	mov	r0, r4
8160bb7c:	ebffffb3 	bl	8160ba50 <find_env_callback>
8160bb80:	e3500000 	cmp	r0, #0
8160bb84:	0afffff4 	beq	8160bb5c <set_callback+0x60>
8160bb88:	e5902004 	ldr	r2, [r0, #4]
8160bb8c:	e59d3014 	ldr	r3, [sp, #20]
8160bb90:	eafffff0 	b	8160bb58 <set_callback+0x5c>
8160bb94:	816190f0 	.word	0x816190f0

8160bb98 <env_callback_init>:
8160bb98:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160bb9c:	e3a04000 	mov	r4, #0
8160bba0:	e59f80a4 	ldr	r8, [pc, #164]	; 8160bc4c <env_callback_init+0xb4>
8160bba4:	e24ddc01 	sub	sp, sp, #256	; 0x100
8160bba8:	e5906000 	ldr	r6, [r0]
8160bbac:	e1a05000 	mov	r5, r0
8160bbb0:	e3a020fc 	mov	r2, #252	; 0xfc
8160bbb4:	e1a01004 	mov	r1, r4
8160bbb8:	e28d0004 	add	r0, sp, #4
8160bbbc:	e58d4000 	str	r4, [sp]
8160bbc0:	ebffd2be 	bl	816006c0 <memset>
8160bbc4:	e5983000 	ldr	r3, [r8]
8160bbc8:	e59f7080 	ldr	r7, [pc, #128]	; 8160bc50 <env_callback_init+0xb8>
8160bbcc:	e1530004 	cmp	r3, r4
8160bbd0:	0a000003 	beq	8160bbe4 <env_callback_init+0x4c>
8160bbd4:	e59f0078 	ldr	r0, [pc, #120]	; 8160bc54 <env_callback_init+0xbc>
8160bbd8:	ebffe26e 	bl	81604598 <env_get>
8160bbdc:	e5884000 	str	r4, [r8]
8160bbe0:	e5870000 	str	r0, [r7]
8160bbe4:	e5970000 	ldr	r0, [r7]
8160bbe8:	e3500000 	cmp	r0, #0
8160bbec:	1a000010 	bne	8160bc34 <env_callback_init+0x9c>
8160bbf0:	e1a0200d 	mov	r2, sp
8160bbf4:	e1a01006 	mov	r1, r6
8160bbf8:	e59f0058 	ldr	r0, [pc, #88]	; 8160bc58 <env_callback_init+0xc0>
8160bbfc:	ebffff2b 	bl	8160b8b0 <env_attr_lookup>
8160bc00:	e3500000 	cmp	r0, #0
8160bc04:	1a000008 	bne	8160bc2c <env_callback_init+0x94>
8160bc08:	e1a0000d 	mov	r0, sp
8160bc0c:	eb001730 	bl	816118d4 <strlen>
8160bc10:	e3500000 	cmp	r0, #0
8160bc14:	0a000004 	beq	8160bc2c <env_callback_init+0x94>
8160bc18:	e1a0000d 	mov	r0, sp
8160bc1c:	ebffff8b 	bl	8160ba50 <find_env_callback>
8160bc20:	e3500000 	cmp	r0, #0
8160bc24:	15903004 	ldrne	r3, [r0, #4]
8160bc28:	15853008 	strne	r3, [r5, #8]
8160bc2c:	e28ddc01 	add	sp, sp, #256	; 0x100
8160bc30:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160bc34:	e1a0200d 	mov	r2, sp
8160bc38:	e1a01006 	mov	r1, r6
8160bc3c:	ebffff1b 	bl	8160b8b0 <env_attr_lookup>
8160bc40:	e3500000 	cmp	r0, #0
8160bc44:	1affffe9 	bne	8160bbf0 <env_callback_init+0x58>
8160bc48:	eaffffee 	b	8160bc08 <env_callback_init+0x70>
8160bc4c:	81619100 	.word	0x81619100
8160bc50:	81619e7c 	.word	0x81619e7c
8160bc54:	816159c6 	.word	0x816159c6
8160bc58:	816165ee 	.word	0x816165ee

8160bc5c <clear_flags>:
8160bc5c:	e3a03000 	mov	r3, #0
8160bc60:	e580300c 	str	r3, [r0, #12]
8160bc64:	e1a00003 	mov	r0, r3
8160bc68:	e12fff1e 	bx	lr

8160bc6c <on_flags>:
8160bc6c:	e92d4070 	push	{r4, r5, r6, lr}
8160bc70:	e1a05001 	mov	r5, r1
8160bc74:	e59f4030 	ldr	r4, [pc, #48]	; 8160bcac <on_flags+0x40>
8160bc78:	e59f1030 	ldr	r1, [pc, #48]	; 8160bcb0 <on_flags+0x44>
8160bc7c:	e59f0030 	ldr	r0, [pc, #48]	; 8160bcb4 <on_flags+0x48>
8160bc80:	eb00153f 	bl	81611184 <hwalk_r>
8160bc84:	e1a01004 	mov	r1, r4
8160bc88:	e3a02000 	mov	r2, #0
8160bc8c:	e59f0024 	ldr	r0, [pc, #36]	; 8160bcb8 <on_flags+0x4c>
8160bc90:	ebfffebe 	bl	8160b790 <env_attr_walk>
8160bc94:	e3a02000 	mov	r2, #0
8160bc98:	e1a01004 	mov	r1, r4
8160bc9c:	e1a00005 	mov	r0, r5
8160bca0:	ebfffeba 	bl	8160b790 <env_attr_walk>
8160bca4:	e3a00000 	mov	r0, #0
8160bca8:	e8bd8070 	pop	{r4, r5, r6, pc}
8160bcac:	8160bd5c 	.word	0x8160bd5c
8160bcb0:	8160bc5c 	.word	0x8160bc5c
8160bcb4:	816190f0 	.word	0x816190f0
8160bcb8:	81616672 	.word	0x81616672

8160bcbc <env_flags_parse_vartype>:
8160bcbc:	e92d4070 	push	{r4, r5, r6, lr}
8160bcc0:	e1a05000 	mov	r5, r0
8160bcc4:	eb001702 	bl	816118d4 <strlen>
8160bcc8:	e3500000 	cmp	r0, #0
8160bccc:	08bd8070 	popeq	{r4, r5, r6, pc}
8160bcd0:	e59f602c 	ldr	r6, [pc, #44]	; 8160bd04 <env_flags_parse_vartype+0x48>
8160bcd4:	e5d51000 	ldrb	r1, [r5]
8160bcd8:	e1a00006 	mov	r0, r6
8160bcdc:	eb0016f0 	bl	816118a4 <strchr>
8160bce0:	e2504000 	subs	r4, r0, #0
8160bce4:	0a000001 	beq	8160bcf0 <env_flags_parse_vartype+0x34>
8160bce8:	e0440006 	sub	r0, r4, r6
8160bcec:	e8bd8070 	pop	{r4, r5, r6, pc}
8160bcf0:	e5d51000 	ldrb	r1, [r5]
8160bcf4:	e59f000c 	ldr	r0, [pc, #12]	; 8160bd08 <env_flags_parse_vartype+0x4c>
8160bcf8:	eb001a5f 	bl	8161267c <printf>
8160bcfc:	e1a00004 	mov	r0, r4
8160bd00:	e8bd8070 	pop	{r4, r5, r6, pc}
8160bd04:	81615b00 	.word	0x81615b00
8160bd08:	81615a15 	.word	0x81615a15

8160bd0c <env_flags_parse_varaccess>:
8160bd0c:	e92d4070 	push	{r4, r5, r6, lr}
8160bd10:	e1a05000 	mov	r5, r0
8160bd14:	eb0016ee 	bl	816118d4 <strlen>
8160bd18:	e3500001 	cmp	r0, #1
8160bd1c:	9a00000a 	bls	8160bd4c <env_flags_parse_varaccess+0x40>
8160bd20:	e59f602c 	ldr	r6, [pc, #44]	; 8160bd54 <env_flags_parse_varaccess+0x48>
8160bd24:	e5d51001 	ldrb	r1, [r5, #1]
8160bd28:	e1a00006 	mov	r0, r6
8160bd2c:	eb0016dc 	bl	816118a4 <strchr>
8160bd30:	e2504000 	subs	r4, r0, #0
8160bd34:	0a000001 	beq	8160bd40 <env_flags_parse_varaccess+0x34>
8160bd38:	e0440006 	sub	r0, r4, r6
8160bd3c:	e8bd8070 	pop	{r4, r5, r6, pc}
8160bd40:	e5d51001 	ldrb	r1, [r5, #1]
8160bd44:	e59f000c 	ldr	r0, [pc, #12]	; 8160bd58 <env_flags_parse_varaccess+0x4c>
8160bd48:	eb001a4b 	bl	8161267c <printf>
8160bd4c:	e3a00000 	mov	r0, #0
8160bd50:	e8bd8070 	pop	{r4, r5, r6, pc}
8160bd54:	81615afb 	.word	0x81615afb
8160bd58:	816159d8 	.word	0x816159d8

8160bd5c <set_flags>:
8160bd5c:	e92d4070 	push	{r4, r5, r6, lr}
8160bd60:	e24dd028 	sub	sp, sp, #40	; 0x28
8160bd64:	e59f2098 	ldr	r2, [pc, #152]	; 8160be04 <set_flags+0xa8>
8160bd68:	e3a03000 	mov	r3, #0
8160bd6c:	e58d301c 	str	r3, [sp, #28]
8160bd70:	e1a04001 	mov	r4, r1
8160bd74:	e58d3020 	str	r3, [sp, #32]
8160bd78:	e58d300c 	str	r3, [sp, #12]
8160bd7c:	e58d3000 	str	r3, [sp]
8160bd80:	e28d3028 	add	r3, sp, #40	; 0x28
8160bd84:	e58d2008 	str	r2, [sp, #8]
8160bd88:	e28d2014 	add	r2, sp, #20
8160bd8c:	e58d0018 	str	r0, [sp, #24]
8160bd90:	e58d2004 	str	r2, [sp, #4]
8160bd94:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
8160bd98:	eb0011d9 	bl	81610504 <hsearch_r>
8160bd9c:	e59d3014 	ldr	r3, [sp, #20]
8160bda0:	e3530000 	cmp	r3, #0
8160bda4:	0a000004 	beq	8160bdbc <set_flags+0x60>
8160bda8:	e3540000 	cmp	r4, #0
8160bdac:	1a000005 	bne	8160bdc8 <set_flags+0x6c>
8160bdb0:	e59d3014 	ldr	r3, [sp, #20]
8160bdb4:	e3a02000 	mov	r2, #0
8160bdb8:	e583200c 	str	r2, [r3, #12]
8160bdbc:	e3a00000 	mov	r0, #0
8160bdc0:	e28dd028 	add	sp, sp, #40	; 0x28
8160bdc4:	e8bd8070 	pop	{r4, r5, r6, pc}
8160bdc8:	e1a00004 	mov	r0, r4
8160bdcc:	eb0016c0 	bl	816118d4 <strlen>
8160bdd0:	e3500000 	cmp	r0, #0
8160bdd4:	0afffff5 	beq	8160bdb0 <set_flags+0x54>
8160bdd8:	e1a00004 	mov	r0, r4
8160bddc:	e59d5014 	ldr	r5, [sp, #20]
8160bde0:	ebffffb5 	bl	8160bcbc <env_flags_parse_vartype>
8160bde4:	e2006007 	and	r6, r0, #7
8160bde8:	e1a00004 	mov	r0, r4
8160bdec:	ebffffc6 	bl	8160bd0c <env_flags_parse_varaccess>
8160bdf0:	e59f3010 	ldr	r3, [pc, #16]	; 8160be08 <set_flags+0xac>
8160bdf4:	e7930100 	ldr	r0, [r3, r0, lsl #2]
8160bdf8:	e1800006 	orr	r0, r0, r6
8160bdfc:	e585000c 	str	r0, [r5, #12]
8160be00:	eaffffed 	b	8160bdbc <set_flags+0x60>
8160be04:	816190f0 	.word	0x816190f0
8160be08:	816131ac 	.word	0x816131ac

8160be0c <env_flags_init>:
8160be0c:	e59f30b8 	ldr	r3, [pc, #184]	; 8160becc <env_flags_init+0xc0>
8160be10:	e92d41f3 	push	{r0, r1, r4, r5, r6, r7, r8, lr}
8160be14:	e3a04000 	mov	r4, #0
8160be18:	e5d33000 	ldrb	r3, [r3]
8160be1c:	e1a05000 	mov	r5, r0
8160be20:	e59f80a8 	ldr	r8, [pc, #168]	; 8160bed0 <env_flags_init+0xc4>
8160be24:	e5906000 	ldr	r6, [r0]
8160be28:	e5cd3004 	strb	r3, [sp, #4]
8160be2c:	e5983000 	ldr	r3, [r8]
8160be30:	e5cd4005 	strb	r4, [sp, #5]
8160be34:	e1530004 	cmp	r3, r4
8160be38:	e5cd4006 	strb	r4, [sp, #6]
8160be3c:	e59f7090 	ldr	r7, [pc, #144]	; 8160bed4 <env_flags_init+0xc8>
8160be40:	0a000003 	beq	8160be54 <env_flags_init+0x48>
8160be44:	e59f008c 	ldr	r0, [pc, #140]	; 8160bed8 <env_flags_init+0xcc>
8160be48:	ebffe1d2 	bl	81604598 <env_get>
8160be4c:	e5884000 	str	r4, [r8]
8160be50:	e5870000 	str	r0, [r7]
8160be54:	e5970000 	ldr	r0, [r7]
8160be58:	e3500000 	cmp	r0, #0
8160be5c:	1a000014 	bne	8160beb4 <env_flags_init+0xa8>
8160be60:	e28d2004 	add	r2, sp, #4
8160be64:	e1a01006 	mov	r1, r6
8160be68:	e59f006c 	ldr	r0, [pc, #108]	; 8160bedc <env_flags_init+0xd0>
8160be6c:	ebfffe8f 	bl	8160b8b0 <env_attr_lookup>
8160be70:	e3500000 	cmp	r0, #0
8160be74:	1a00000c 	bne	8160beac <env_flags_init+0xa0>
8160be78:	e28d0004 	add	r0, sp, #4
8160be7c:	eb001694 	bl	816118d4 <strlen>
8160be80:	e3500000 	cmp	r0, #0
8160be84:	0a000008 	beq	8160beac <env_flags_init+0xa0>
8160be88:	e28d0004 	add	r0, sp, #4
8160be8c:	ebffff8a 	bl	8160bcbc <env_flags_parse_vartype>
8160be90:	e2004007 	and	r4, r0, #7
8160be94:	e28d0004 	add	r0, sp, #4
8160be98:	ebffff9b 	bl	8160bd0c <env_flags_parse_varaccess>
8160be9c:	e59f303c 	ldr	r3, [pc, #60]	; 8160bee0 <env_flags_init+0xd4>
8160bea0:	e7930100 	ldr	r0, [r3, r0, lsl #2]
8160bea4:	e1800004 	orr	r0, r0, r4
8160bea8:	e585000c 	str	r0, [r5, #12]
8160beac:	e28dd008 	add	sp, sp, #8
8160beb0:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160beb4:	e28d2004 	add	r2, sp, #4
8160beb8:	e1a01006 	mov	r1, r6
8160bebc:	ebfffe7b 	bl	8160b8b0 <env_attr_lookup>
8160bec0:	e3500000 	cmp	r0, #0
8160bec4:	1affffe5 	bne	8160be60 <env_flags_init+0x54>
8160bec8:	eaffffea 	b	8160be78 <env_flags_init+0x6c>
8160becc:	81614056 	.word	0x81614056
8160bed0:	81619104 	.word	0x81619104
8160bed4:	81619e80 	.word	0x81619e80
8160bed8:	816159d1 	.word	0x816159d1
8160bedc:	81616672 	.word	0x81616672
8160bee0:	816131ac 	.word	0x816131ac

8160bee4 <env_flags_validate>:
8160bee4:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160bee8:	e2526000 	subs	r6, r2, #0
8160beec:	e59f21e8 	ldr	r2, [pc, #488]	; 8160c0dc <env_flags_validate+0x1f8>
8160bef0:	15905004 	ldrne	r5, [r0, #4]
8160bef4:	01a05006 	moveq	r5, r6
8160bef8:	e3510000 	cmp	r1, #0
8160befc:	e5904000 	ldr	r4, [r0]
8160bf00:	01a01002 	moveq	r1, r2
8160bf04:	e3560001 	cmp	r6, #1
8160bf08:	0a000011 	beq	8160bf54 <env_flags_validate+0x70>
8160bf0c:	e590e00c 	ldr	lr, [r0, #12]
8160bf10:	e20ee007 	and	lr, lr, #7
8160bf14:	e24e2001 	sub	r2, lr, #1
8160bf18:	e3520003 	cmp	r2, #3
8160bf1c:	979ff102 	ldrls	pc, [pc, r2, lsl #2]
8160bf20:	ea00000b 	b	8160bf54 <env_flags_validate+0x70>
8160bf24:	8160bf34 	.word	0x8160bf34
8160bf28:	8160bf64 	.word	0x8160bf64
8160bf2c:	8160bfec 	.word	0x8160bfec
8160bf30:	8160bfc8 	.word	0x8160bfc8
8160bf34:	e59f81a4 	ldr	r8, [pc, #420]	; 8160c0e0 <env_flags_validate+0x1fc>
8160bf38:	e241c001 	sub	ip, r1, #1
8160bf3c:	e5fc2001 	ldrb	r2, [ip, #1]!
8160bf40:	e7d87002 	ldrb	r7, [r8, r2]
8160bf44:	e3170004 	tst	r7, #4
8160bf48:	1afffffb 	bne	8160bf3c <env_flags_validate+0x58>
8160bf4c:	e3520000 	cmp	r2, #0
8160bf50:	1a00001c 	bne	8160bfc8 <env_flags_validate+0xe4>
8160bf54:	e2133002 	ands	r3, r3, #2
8160bf58:	0a000035 	beq	8160c034 <env_flags_validate+0x150>
8160bf5c:	e3a03000 	mov	r3, #0
8160bf60:	ea00001f 	b	8160bfe4 <env_flags_validate+0x100>
8160bf64:	e5d1c000 	ldrb	ip, [r1]
8160bf68:	e35c0030 	cmp	ip, #48	; 0x30
8160bf6c:	1a000004 	bne	8160bf84 <env_flags_validate+0xa0>
8160bf70:	e5d12001 	ldrb	r2, [r1, #1]
8160bf74:	e20220df 	and	r2, r2, #223	; 0xdf
8160bf78:	e3520058 	cmp	r2, #88	; 0x58
8160bf7c:	02812002 	addeq	r2, r1, #2
8160bf80:	0a000000 	beq	8160bf88 <env_flags_validate+0xa4>
8160bf84:	e1a02001 	mov	r2, r1
8160bf88:	e59fb150 	ldr	fp, [pc, #336]	; 8160c0e0 <env_flags_validate+0x1fc>
8160bf8c:	e1a08002 	mov	r8, r2
8160bf90:	e4d27001 	ldrb	r7, [r2], #1
8160bf94:	e7dba007 	ldrb	sl, [fp, r7]
8160bf98:	e31a0044 	tst	sl, #68	; 0x44
8160bf9c:	1afffffa 	bne	8160bf8c <env_flags_validate+0xa8>
8160bfa0:	e3570000 	cmp	r7, #0
8160bfa4:	1a000007 	bne	8160bfc8 <env_flags_validate+0xe4>
8160bfa8:	e2812002 	add	r2, r1, #2
8160bfac:	e1580002 	cmp	r8, r2
8160bfb0:	035c0030 	cmpeq	ip, #48	; 0x30
8160bfb4:	1affffe6 	bne	8160bf54 <env_flags_validate+0x70>
8160bfb8:	e5d12001 	ldrb	r2, [r1, #1]
8160bfbc:	e20220df 	and	r2, r2, #223	; 0xdf
8160bfc0:	e3520058 	cmp	r2, #88	; 0x58
8160bfc4:	1affffe2 	bne	8160bf54 <env_flags_validate+0x70>
8160bfc8:	e59f3114 	ldr	r3, [pc, #276]	; 8160c0e4 <env_flags_validate+0x200>
8160bfcc:	e1a02001 	mov	r2, r1
8160bfd0:	e59f0110 	ldr	r0, [pc, #272]	; 8160c0e8 <env_flags_validate+0x204>
8160bfd4:	e1a01004 	mov	r1, r4
8160bfd8:	e7d3300e 	ldrb	r3, [r3, lr]
8160bfdc:	eb0019a6 	bl	8161267c <printf>
8160bfe0:	e3e03000 	mvn	r3, #0
8160bfe4:	e1a00003 	mov	r0, r3
8160bfe8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160bfec:	e5d12000 	ldrb	r2, [r1]
8160bff0:	e202c0df 	and	ip, r2, #223	; 0xdf
8160bff4:	e2427030 	sub	r7, r2, #48	; 0x30
8160bff8:	e35c0054 	cmp	ip, #84	; 0x54
8160bffc:	135c0059 	cmpne	ip, #89	; 0x59
8160c000:	e20220d7 	and	r2, r2, #215	; 0xd7
8160c004:	13a0c001 	movne	ip, #1
8160c008:	03a0c000 	moveq	ip, #0
8160c00c:	e3570001 	cmp	r7, #1
8160c010:	93a0c000 	movls	ip, #0
8160c014:	820cc001 	andhi	ip, ip, #1
8160c018:	e3520046 	cmp	r2, #70	; 0x46
8160c01c:	03a0c000 	moveq	ip, #0
8160c020:	120cc001 	andne	ip, ip, #1
8160c024:	e35c0000 	cmp	ip, #0
8160c028:	1affffe6 	bne	8160bfc8 <env_flags_validate+0xe4>
8160c02c:	e5d12001 	ldrb	r2, [r1, #1]
8160c030:	eaffffc5 	b	8160bf4c <env_flags_validate+0x68>
8160c034:	e3560001 	cmp	r6, #1
8160c038:	0a000008 	beq	8160c060 <env_flags_validate+0x17c>
8160c03c:	3a000020 	bcc	8160c0c4 <env_flags_validate+0x1e0>
8160c040:	e3560002 	cmp	r6, #2
8160c044:	1affffe6 	bne	8160bfe4 <env_flags_validate+0x100>
8160c048:	e590300c 	ldr	r3, [r0, #12]
8160c04c:	e3130020 	tst	r3, #32
8160c050:	0a00000a 	beq	8160c080 <env_flags_validate+0x19c>
8160c054:	e1a01004 	mov	r1, r4
8160c058:	e59f008c 	ldr	r0, [pc, #140]	; 8160c0ec <env_flags_validate+0x208>
8160c05c:	ea000004 	b	8160c074 <env_flags_validate+0x190>
8160c060:	e590300c 	ldr	r3, [r0, #12]
8160c064:	e3130008 	tst	r3, #8
8160c068:	11a01004 	movne	r1, r4
8160c06c:	159f007c 	ldrne	r0, [pc, #124]	; 8160c0f0 <env_flags_validate+0x20c>
8160c070:	0affffb9 	beq	8160bf5c <env_flags_validate+0x78>
8160c074:	eb001980 	bl	8161267c <printf>
8160c078:	e3a03001 	mov	r3, #1
8160c07c:	eaffffd8 	b	8160bfe4 <env_flags_validate+0x100>
8160c080:	e3130040 	tst	r3, #64	; 0x40
8160c084:	0affffb4 	beq	8160bf5c <env_flags_validate+0x78>
8160c088:	e1a00004 	mov	r0, r4
8160c08c:	e59f6048 	ldr	r6, [pc, #72]	; 8160c0dc <env_flags_validate+0x1f8>
8160c090:	ebfffcf3 	bl	8160b464 <env_get_default>
8160c094:	e1a01005 	mov	r1, r5
8160c098:	e3500000 	cmp	r0, #0
8160c09c:	11a06000 	movne	r6, r0
8160c0a0:	e59f004c 	ldr	r0, [pc, #76]	; 8160c0f4 <env_flags_validate+0x210>
8160c0a4:	e1a02006 	mov	r2, r6
8160c0a8:	eb001973 	bl	8161267c <printf>
8160c0ac:	e1a01006 	mov	r1, r6
8160c0b0:	e1a00005 	mov	r0, r5
8160c0b4:	eb0015e1 	bl	81611840 <strcmp>
8160c0b8:	e3500000 	cmp	r0, #0
8160c0bc:	0affffa6 	beq	8160bf5c <env_flags_validate+0x78>
8160c0c0:	eaffffe3 	b	8160c054 <env_flags_validate+0x170>
8160c0c4:	e590300c 	ldr	r3, [r0, #12]
8160c0c8:	e3130010 	tst	r3, #16
8160c0cc:	0affffa2 	beq	8160bf5c <env_flags_validate+0x78>
8160c0d0:	e1a01004 	mov	r1, r4
8160c0d4:	e59f001c 	ldr	r0, [pc, #28]	; 8160c0f8 <env_flags_validate+0x214>
8160c0d8:	eaffffe5 	b	8160c074 <env_flags_validate+0x190>
8160c0dc:	8161623b 	.word	0x8161623b
8160c0e0:	816140e1 	.word	0x816140e1
8160c0e4:	81615b00 	.word	0x81615b00
8160c0e8:	81615a49 	.word	0x81615a49
8160c0ec:	81615aa6 	.word	0x81615aa6
8160c0f0:	81615a89 	.word	0x81615a89
8160c0f4:	81615ac6 	.word	0x81615ac6
8160c0f8:	81615ade 	.word	0x81615ade

8160c0fc <env_nowhere_init>:
8160c0fc:	e59f200c 	ldr	r2, [pc, #12]	; 8160c110 <env_nowhere_init+0x14>
8160c100:	e3a00000 	mov	r0, #0
8160c104:	e5892020 	str	r2, [r9, #32]
8160c108:	e5890024 	str	r0, [r9, #36]	; 0x24
8160c10c:	e12fff1e 	bx	lr
8160c110:	816149a0 	.word	0x816149a0

8160c114 <zcalloc>:
8160c114:	e0000192 	mul	r0, r2, r1
8160c118:	eaffecd1 	b	81607464 <malloc>

8160c11c <zcfree>:
8160c11c:	e1a00001 	mov	r0, r1
8160c120:	eaffec44 	b	81607238 <free>

8160c124 <inflate_fast>:
8160c124:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160c128:	e1a0a000 	mov	sl, r0
8160c12c:	e590b01c 	ldr	fp, [r0, #28]
8160c130:	e24dd040 	sub	sp, sp, #64	; 0x40
8160c134:	e5902000 	ldr	r2, [r0]
8160c138:	e5900004 	ldr	r0, [r0, #4]
8160c13c:	e2422001 	sub	r2, r2, #1
8160c140:	e59be054 	ldr	lr, [fp, #84]	; 0x54
8160c144:	e2403005 	sub	r3, r0, #5
8160c148:	e59bc038 	ldr	ip, [fp, #56]	; 0x38
8160c14c:	e0923003 	adds	r3, r2, r3
8160c150:	e58d3008 	str	r3, [sp, #8]
8160c154:	23a03001 	movcs	r3, #1
8160c158:	33a03000 	movcc	r3, #0
8160c15c:	e3500005 	cmp	r0, #5
8160c160:	93a03000 	movls	r3, #0
8160c164:	82033001 	andhi	r3, r3, #1
8160c168:	e59a0010 	ldr	r0, [sl, #16]
8160c16c:	e3530000 	cmp	r3, #0
8160c170:	e0401001 	sub	r1, r0, r1
8160c174:	11e03002 	mvnne	r3, r2
8160c178:	158a3004 	strne	r3, [sl, #4]
8160c17c:	12433005 	subne	r3, r3, #5
8160c180:	10823003 	addne	r3, r2, r3
8160c184:	158d3008 	strne	r3, [sp, #8]
8160c188:	e59a300c 	ldr	r3, [sl, #12]
8160c18c:	e2433001 	sub	r3, r3, #1
8160c190:	e0831001 	add	r1, r3, r1
8160c194:	e58d1028 	str	r1, [sp, #40]	; 0x28
8160c198:	e2401c01 	sub	r1, r0, #256	; 0x100
8160c19c:	e59b004c 	ldr	r0, [fp, #76]	; 0x4c
8160c1a0:	e2411001 	sub	r1, r1, #1
8160c1a4:	e0831001 	add	r1, r3, r1
8160c1a8:	e58d100c 	str	r1, [sp, #12]
8160c1ac:	e59b1028 	ldr	r1, [fp, #40]	; 0x28
8160c1b0:	e58d0010 	str	r0, [sp, #16]
8160c1b4:	e59b0050 	ldr	r0, [fp, #80]	; 0x50
8160c1b8:	e58d1020 	str	r1, [sp, #32]
8160c1bc:	e59b102c 	ldr	r1, [fp, #44]	; 0x2c
8160c1c0:	e58d0014 	str	r0, [sp, #20]
8160c1c4:	e3e00000 	mvn	r0, #0
8160c1c8:	e1e0ee10 	mvn	lr, r0, lsl lr
8160c1cc:	e58de024 	str	lr, [sp, #36]	; 0x24
8160c1d0:	e58d102c 	str	r1, [sp, #44]	; 0x2c
8160c1d4:	e59b1030 	ldr	r1, [fp, #48]	; 0x30
8160c1d8:	e59be058 	ldr	lr, [fp, #88]	; 0x58
8160c1dc:	e58d001c 	str	r0, [sp, #28]
8160c1e0:	e58d1004 	str	r1, [sp, #4]
8160c1e4:	e59b1034 	ldr	r1, [fp, #52]	; 0x34
8160c1e8:	e1e0ee10 	mvn	lr, r0, lsl lr
8160c1ec:	e59d0020 	ldr	r0, [sp, #32]
8160c1f0:	e58de034 	str	lr, [sp, #52]	; 0x34
8160c1f4:	e59de004 	ldr	lr, [sp, #4]
8160c1f8:	e58d1030 	str	r1, [sp, #48]	; 0x30
8160c1fc:	e080000e 	add	r0, r0, lr
8160c200:	e59b103c 	ldr	r1, [fp, #60]	; 0x3c
8160c204:	e58d003c 	str	r0, [sp, #60]	; 0x3c
8160c208:	e351000e 	cmp	r1, #14
8160c20c:	8a000007 	bhi	8160c230 <inflate_fast+0x10c>
8160c210:	e5d20001 	ldrb	r0, [r2, #1]
8160c214:	e281e008 	add	lr, r1, #8
8160c218:	e5d24002 	ldrb	r4, [r2, #2]
8160c21c:	e2822002 	add	r2, r2, #2
8160c220:	e1a00110 	lsl	r0, r0, r1
8160c224:	e2811010 	add	r1, r1, #16
8160c228:	e0800e14 	add	r0, r0, r4, lsl lr
8160c22c:	e08cc000 	add	ip, ip, r0
8160c230:	e59d0024 	ldr	r0, [sp, #36]	; 0x24
8160c234:	e59de010 	ldr	lr, [sp, #16]
8160c238:	e000000c 	and	r0, r0, ip
8160c23c:	e08e4100 	add	r4, lr, r0, lsl #2
8160c240:	e7de0100 	ldrb	r0, [lr, r0, lsl #2]
8160c244:	e5d4e001 	ldrb	lr, [r4, #1]
8160c248:	e1d440b2 	ldrh	r4, [r4, #2]
8160c24c:	e3500000 	cmp	r0, #0
8160c250:	e1a0ce3c 	lsr	ip, ip, lr
8160c254:	e041100e 	sub	r1, r1, lr
8160c258:	1a000002 	bne	8160c268 <inflate_fast+0x144>
8160c25c:	e5c34001 	strb	r4, [r3, #1]
8160c260:	e2833001 	add	r3, r3, #1
8160c264:	ea00007f 	b	8160c468 <inflate_fast+0x344>
8160c268:	e3100010 	tst	r0, #16
8160c26c:	0a0000f6 	beq	8160c64c <inflate_fast+0x528>
8160c270:	e210000f 	ands	r0, r0, #15
8160c274:	e1a0e004 	mov	lr, r4
8160c278:	0a000009 	beq	8160c2a4 <inflate_fast+0x180>
8160c27c:	e1510000 	cmp	r1, r0
8160c280:	35d2e001 	ldrbcc	lr, [r2, #1]
8160c284:	32822001 	addcc	r2, r2, #1
8160c288:	308cc11e 	addcc	ip, ip, lr, lsl r1
8160c28c:	e59de01c 	ldr	lr, [sp, #28]
8160c290:	32811008 	addcc	r1, r1, #8
8160c294:	e0411000 	sub	r1, r1, r0
8160c298:	e1cce01e 	bic	lr, ip, lr, lsl r0
8160c29c:	e1a0c03c 	lsr	ip, ip, r0
8160c2a0:	e084e00e 	add	lr, r4, lr
8160c2a4:	e351000e 	cmp	r1, #14
8160c2a8:	8a000007 	bhi	8160c2cc <inflate_fast+0x1a8>
8160c2ac:	e5d20001 	ldrb	r0, [r2, #1]
8160c2b0:	e2814008 	add	r4, r1, #8
8160c2b4:	e5d25002 	ldrb	r5, [r2, #2]
8160c2b8:	e2822002 	add	r2, r2, #2
8160c2bc:	e1a00110 	lsl	r0, r0, r1
8160c2c0:	e2811010 	add	r1, r1, #16
8160c2c4:	e0800415 	add	r0, r0, r5, lsl r4
8160c2c8:	e08cc000 	add	ip, ip, r0
8160c2cc:	e59d0034 	ldr	r0, [sp, #52]	; 0x34
8160c2d0:	e59d4014 	ldr	r4, [sp, #20]
8160c2d4:	e00c0000 	and	r0, ip, r0
8160c2d8:	e0845100 	add	r5, r4, r0, lsl #2
8160c2dc:	e7d40100 	ldrb	r0, [r4, r0, lsl #2]
8160c2e0:	e5d54001 	ldrb	r4, [r5, #1]
8160c2e4:	e1d550b2 	ldrh	r5, [r5, #2]
8160c2e8:	e3100010 	tst	r0, #16
8160c2ec:	e1a0c43c 	lsr	ip, ip, r4
8160c2f0:	e0411004 	sub	r1, r1, r4
8160c2f4:	0a0000c7 	beq	8160c618 <inflate_fast+0x4f4>
8160c2f8:	e200000f 	and	r0, r0, #15
8160c2fc:	e1510000 	cmp	r1, r0
8160c300:	2a000009 	bcs	8160c32c <inflate_fast+0x208>
8160c304:	e5d24001 	ldrb	r4, [r2, #1]
8160c308:	e08cc114 	add	ip, ip, r4, lsl r1
8160c30c:	e2814008 	add	r4, r1, #8
8160c310:	e1500004 	cmp	r0, r4
8160c314:	92822001 	addls	r2, r2, #1
8160c318:	82811010 	addhi	r1, r1, #16
8160c31c:	85d26002 	ldrbhi	r6, [r2, #2]
8160c320:	82822002 	addhi	r2, r2, #2
8160c324:	91a01004 	movls	r1, r4
8160c328:	808cc416 	addhi	ip, ip, r6, lsl r4
8160c32c:	e59d401c 	ldr	r4, [sp, #28]
8160c330:	e0411000 	sub	r1, r1, r0
8160c334:	e1cc4014 	bic	r4, ip, r4, lsl r0
8160c338:	e1a0c03c 	lsr	ip, ip, r0
8160c33c:	e59d0028 	ldr	r0, [sp, #40]	; 0x28
8160c340:	e0845005 	add	r5, r4, r5
8160c344:	e0434000 	sub	r4, r3, r0
8160c348:	e1550004 	cmp	r5, r4
8160c34c:	9a000084 	bls	8160c564 <inflate_fast+0x440>
8160c350:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
8160c354:	e0454004 	sub	r4, r5, r4
8160c358:	e1500004 	cmp	r0, r4
8160c35c:	2a000020 	bcs	8160c3e4 <inflate_fast+0x2c0>
8160c360:	e59f0324 	ldr	r0, [pc, #804]	; 8160c68c <inflate_fast+0x568>
8160c364:	e58a0018 	str	r0, [sl, #24]
8160c368:	e3a0001b 	mov	r0, #27
8160c36c:	e58b0000 	str	r0, [fp]
8160c370:	e04221a1 	sub	r2, r2, r1, lsr #3
8160c374:	e3e00000 	mvn	r0, #0
8160c378:	e2011007 	and	r1, r1, #7
8160c37c:	e1ccc110 	bic	ip, ip, r0, lsl r1
8160c380:	e2820001 	add	r0, r2, #1
8160c384:	e58a0000 	str	r0, [sl]
8160c388:	e2830001 	add	r0, r3, #1
8160c38c:	e58a000c 	str	r0, [sl, #12]
8160c390:	e59d0008 	ldr	r0, [sp, #8]
8160c394:	e1500002 	cmp	r0, r2
8160c398:	80402002 	subhi	r2, r0, r2
8160c39c:	959d0008 	ldrls	r0, [sp, #8]
8160c3a0:	82822005 	addhi	r2, r2, #5
8160c3a4:	90422000 	subls	r2, r2, r0
8160c3a8:	e59d000c 	ldr	r0, [sp, #12]
8160c3ac:	92622005 	rsbls	r2, r2, #5
8160c3b0:	e58a2004 	str	r2, [sl, #4]
8160c3b4:	e59d200c 	ldr	r2, [sp, #12]
8160c3b8:	e1530002 	cmp	r3, r2
8160c3bc:	e3002101 	movw	r2, #257	; 0x101
8160c3c0:	30403003 	subcc	r3, r0, r3
8160c3c4:	20433000 	subcs	r3, r3, r0
8160c3c8:	30833002 	addcc	r3, r3, r2
8160c3cc:	20423003 	subcs	r3, r2, r3
8160c3d0:	e58a3010 	str	r3, [sl, #16]
8160c3d4:	e58bc038 	str	ip, [fp, #56]	; 0x38
8160c3d8:	e58b103c 	str	r1, [fp, #60]	; 0x3c
8160c3dc:	e28dd040 	add	sp, sp, #64	; 0x40
8160c3e0:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160c3e4:	e59d0030 	ldr	r0, [sp, #48]	; 0x30
8160c3e8:	e2406001 	sub	r6, r0, #1
8160c3ec:	e59d0004 	ldr	r0, [sp, #4]
8160c3f0:	e3500000 	cmp	r0, #0
8160c3f4:	1a000021 	bne	8160c480 <inflate_fast+0x35c>
8160c3f8:	e59d0020 	ldr	r0, [sp, #32]
8160c3fc:	e15e0004 	cmp	lr, r4
8160c400:	e0400004 	sub	r0, r0, r4
8160c404:	e0860000 	add	r0, r6, r0
8160c408:	9a000009 	bls	8160c434 <inflate_fast+0x310>
8160c40c:	e59d8020 	ldr	r8, [sp, #32]
8160c410:	e04ee004 	sub	lr, lr, r4
8160c414:	e1a07003 	mov	r7, r3
8160c418:	e0866008 	add	r6, r6, r8
8160c41c:	e5f08001 	ldrb	r8, [r0, #1]!
8160c420:	e1500006 	cmp	r0, r6
8160c424:	e5e78001 	strb	r8, [r7, #1]!
8160c428:	1afffffb 	bne	8160c41c <inflate_fast+0x2f8>
8160c42c:	e0833004 	add	r3, r3, r4
8160c430:	e0430005 	sub	r0, r3, r5
8160c434:	e1a04003 	mov	r4, r3
8160c438:	e35e0002 	cmp	lr, #2
8160c43c:	e1a03004 	mov	r3, r4
8160c440:	8a00003f 	bhi	8160c544 <inflate_fast+0x420>
8160c444:	e35e0000 	cmp	lr, #0
8160c448:	0a000006 	beq	8160c468 <inflate_fast+0x344>
8160c44c:	e5d04001 	ldrb	r4, [r0, #1]
8160c450:	e35e0002 	cmp	lr, #2
8160c454:	e5c34001 	strb	r4, [r3, #1]
8160c458:	05d00002 	ldrbeq	r0, [r0, #2]
8160c45c:	05c30002 	strbeq	r0, [r3, #2]
8160c460:	02833002 	addeq	r3, r3, #2
8160c464:	1affff7d 	bne	8160c260 <inflate_fast+0x13c>
8160c468:	e59d0008 	ldr	r0, [sp, #8]
8160c46c:	e59de00c 	ldr	lr, [sp, #12]
8160c470:	e1520000 	cmp	r2, r0
8160c474:	3153000e 	cmpcc	r3, lr
8160c478:	3affff62 	bcc	8160c208 <inflate_fast+0xe4>
8160c47c:	eaffffbb 	b	8160c370 <inflate_fast+0x24c>
8160c480:	e59d0004 	ldr	r0, [sp, #4]
8160c484:	e1500004 	cmp	r0, r4
8160c488:	2a00001f 	bcs	8160c50c <inflate_fast+0x3e8>
8160c48c:	e59d7004 	ldr	r7, [sp, #4]
8160c490:	e59d003c 	ldr	r0, [sp, #60]	; 0x3c
8160c494:	e0447007 	sub	r7, r4, r7
8160c498:	e58d7018 	str	r7, [sp, #24]
8160c49c:	e15e0007 	cmp	lr, r7
8160c4a0:	e0400004 	sub	r0, r0, r4
8160c4a4:	e0860000 	add	r0, r6, r0
8160c4a8:	9affffe1 	bls	8160c434 <inflate_fast+0x310>
8160c4ac:	e59d8018 	ldr	r8, [sp, #24]
8160c4b0:	e04ee007 	sub	lr, lr, r7
8160c4b4:	e1a07003 	mov	r7, r3
8160c4b8:	e0808008 	add	r8, r0, r8
8160c4bc:	e58d8038 	str	r8, [sp, #56]	; 0x38
8160c4c0:	e5f08001 	ldrb	r8, [r0, #1]!
8160c4c4:	e5e78001 	strb	r8, [r7, #1]!
8160c4c8:	e59d8038 	ldr	r8, [sp, #56]	; 0x38
8160c4cc:	e1500008 	cmp	r0, r8
8160c4d0:	1afffffa 	bne	8160c4c0 <inflate_fast+0x39c>
8160c4d4:	e59d0018 	ldr	r0, [sp, #24]
8160c4d8:	e0837000 	add	r7, r3, r0
8160c4dc:	e59d0004 	ldr	r0, [sp, #4]
8160c4e0:	e150000e 	cmp	r0, lr
8160c4e4:	21a00006 	movcs	r0, r6
8160c4e8:	21a03007 	movcs	r3, r7
8160c4ec:	2affffd0 	bcs	8160c434 <inflate_fast+0x310>
8160c4f0:	e04ee000 	sub	lr, lr, r0
8160c4f4:	e0870000 	add	r0, r7, r0
8160c4f8:	e5f68001 	ldrb	r8, [r6, #1]!
8160c4fc:	e5e78001 	strb	r8, [r7, #1]!
8160c500:	e1500007 	cmp	r0, r7
8160c504:	1afffffb 	bne	8160c4f8 <inflate_fast+0x3d4>
8160c508:	eaffffc7 	b	8160c42c <inflate_fast+0x308>
8160c50c:	e59d0004 	ldr	r0, [sp, #4]
8160c510:	e15e0004 	cmp	lr, r4
8160c514:	e0400004 	sub	r0, r0, r4
8160c518:	e0860000 	add	r0, r6, r0
8160c51c:	9affffc4 	bls	8160c434 <inflate_fast+0x310>
8160c520:	e59d8004 	ldr	r8, [sp, #4]
8160c524:	e04ee004 	sub	lr, lr, r4
8160c528:	e1a07003 	mov	r7, r3
8160c52c:	e0866008 	add	r6, r6, r8
8160c530:	e5f08001 	ldrb	r8, [r0, #1]!
8160c534:	e1500006 	cmp	r0, r6
8160c538:	e5e78001 	strb	r8, [r7, #1]!
8160c53c:	1afffffb 	bne	8160c530 <inflate_fast+0x40c>
8160c540:	eaffffb9 	b	8160c42c <inflate_fast+0x308>
8160c544:	e5d03001 	ldrb	r3, [r0, #1]
8160c548:	e24ee003 	sub	lr, lr, #3
8160c54c:	e5c43001 	strb	r3, [r4, #1]
8160c550:	e5d03002 	ldrb	r3, [r0, #2]
8160c554:	e5c43002 	strb	r3, [r4, #2]
8160c558:	e5f03003 	ldrb	r3, [r0, #3]!
8160c55c:	e5e43003 	strb	r3, [r4, #3]!
8160c560:	eaffffb4 	b	8160c438 <inflate_fast+0x314>
8160c564:	e3130001 	tst	r3, #1
8160c568:	e0437005 	sub	r7, r3, r5
8160c56c:	024ee001 	subeq	lr, lr, #1
8160c570:	05d70001 	ldrbeq	r0, [r7, #1]
8160c574:	02877001 	addeq	r7, r7, #1
8160c578:	e1a040ae 	lsr	r4, lr, #1
8160c57c:	05c30001 	strbeq	r0, [r3, #1]
8160c580:	02833001 	addeq	r3, r3, #1
8160c584:	e3550002 	cmp	r5, #2
8160c588:	e2430001 	sub	r0, r3, #1
8160c58c:	9a000015 	bls	8160c5e8 <inflate_fast+0x4c4>
8160c590:	e2477001 	sub	r7, r7, #1
8160c594:	e1a08000 	mov	r8, r0
8160c598:	e58d4018 	str	r4, [sp, #24]
8160c59c:	e1a06007 	mov	r6, r7
8160c5a0:	e5d65003 	ldrb	r5, [r6, #3]
8160c5a4:	e1a03005 	mov	r3, r5
8160c5a8:	e5f65002 	ldrb	r5, [r6, #2]!
8160c5ac:	e1855403 	orr	r5, r5, r3, lsl #8
8160c5b0:	e59d3018 	ldr	r3, [sp, #24]
8160c5b4:	e1e850b2 	strh	r5, [r8, #2]!
8160c5b8:	e2533001 	subs	r3, r3, #1
8160c5bc:	e58d3018 	str	r3, [sp, #24]
8160c5c0:	1afffff6 	bne	8160c5a0 <inflate_fast+0x47c>
8160c5c4:	e1a04084 	lsl	r4, r4, #1
8160c5c8:	e2844001 	add	r4, r4, #1
8160c5cc:	e0803004 	add	r3, r0, r4
8160c5d0:	e0877004 	add	r7, r7, r4
8160c5d4:	e31e0001 	tst	lr, #1
8160c5d8:	0affffa2 	beq	8160c468 <inflate_fast+0x344>
8160c5dc:	e5d70001 	ldrb	r0, [r7, #1]
8160c5e0:	e5c30001 	strb	r0, [r3, #1]
8160c5e4:	eaffff1d 	b	8160c260 <inflate_fast+0x13c>
8160c5e8:	e15330b1 	ldrh	r3, [r3, #-1]
8160c5ec:	e3550001 	cmp	r5, #1
8160c5f0:	e1a06000 	mov	r6, r0
8160c5f4:	03c350ff 	biceq	r5, r3, #255	; 0xff
8160c5f8:	01853423 	orreq	r3, r5, r3, lsr #8
8160c5fc:	e1a05004 	mov	r5, r4
8160c600:	e2555001 	subs	r5, r5, #1
8160c604:	e1e630b2 	strh	r3, [r6, #2]!
8160c608:	1afffffc 	bne	8160c600 <inflate_fast+0x4dc>
8160c60c:	e0803084 	add	r3, r0, r4, lsl #1
8160c610:	e2833001 	add	r3, r3, #1
8160c614:	eaffffee 	b	8160c5d4 <inflate_fast+0x4b0>
8160c618:	e3100040 	tst	r0, #64	; 0x40
8160c61c:	1a000008 	bne	8160c644 <inflate_fast+0x520>
8160c620:	e59d401c 	ldr	r4, [sp, #28]
8160c624:	e1cc0014 	bic	r0, ip, r4, lsl r0
8160c628:	e0805005 	add	r5, r0, r5
8160c62c:	e59d0014 	ldr	r0, [sp, #20]
8160c630:	e0806105 	add	r6, r0, r5, lsl #2
8160c634:	e7d00105 	ldrb	r0, [r0, r5, lsl #2]
8160c638:	e5d64001 	ldrb	r4, [r6, #1]
8160c63c:	e1d650b2 	ldrh	r5, [r6, #2]
8160c640:	eaffff28 	b	8160c2e8 <inflate_fast+0x1c4>
8160c644:	e59f0044 	ldr	r0, [pc, #68]	; 8160c690 <inflate_fast+0x56c>
8160c648:	eaffff45 	b	8160c364 <inflate_fast+0x240>
8160c64c:	e3100040 	tst	r0, #64	; 0x40
8160c650:	1a000008 	bne	8160c678 <inflate_fast+0x554>
8160c654:	e59de01c 	ldr	lr, [sp, #28]
8160c658:	e1cc001e 	bic	r0, ip, lr, lsl r0
8160c65c:	e0804004 	add	r4, r0, r4
8160c660:	e59d0010 	ldr	r0, [sp, #16]
8160c664:	e0805104 	add	r5, r0, r4, lsl #2
8160c668:	e7d00104 	ldrb	r0, [r0, r4, lsl #2]
8160c66c:	e5d5e001 	ldrb	lr, [r5, #1]
8160c670:	e1d540b2 	ldrh	r4, [r5, #2]
8160c674:	eafffef4 	b	8160c24c <inflate_fast+0x128>
8160c678:	e3100020 	tst	r0, #32
8160c67c:	13a0000b 	movne	r0, #11
8160c680:	059f000c 	ldreq	r0, [pc, #12]	; 8160c694 <inflate_fast+0x570>
8160c684:	0affff36 	beq	8160c364 <inflate_fast+0x240>
8160c688:	eaffff37 	b	8160c36c <inflate_fast+0x248>
8160c68c:	81616394 	.word	0x81616394
8160c690:	816163b2 	.word	0x816163b2
8160c694:	816163c8 	.word	0x816163c8

8160c698 <inflate_table>:
8160c698:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160c69c:	e24dd080 	sub	sp, sp, #128	; 0x80
8160c6a0:	e59dc0a4 	ldr	ip, [sp, #164]	; 0xa4
8160c6a4:	e58d300c 	str	r3, [sp, #12]
8160c6a8:	e3a03000 	mov	r3, #0
8160c6ac:	e1a0e003 	mov	lr, r3
8160c6b0:	e58d1034 	str	r1, [sp, #52]	; 0x34
8160c6b4:	e1a01083 	lsl	r1, r3, #1
8160c6b8:	e2833001 	add	r3, r3, #1
8160c6bc:	e28d4040 	add	r4, sp, #64	; 0x40
8160c6c0:	e3530010 	cmp	r3, #16
8160c6c4:	e184e0b1 	strh	lr, [r4, r1]
8160c6c8:	1afffff9 	bne	8160c6b4 <inflate_table+0x1c>
8160c6cc:	e59d3034 	ldr	r3, [sp, #52]	; 0x34
8160c6d0:	e3a04000 	mov	r4, #0
8160c6d4:	e2437002 	sub	r7, r3, #2
8160c6d8:	e1a0e007 	mov	lr, r7
8160c6dc:	e1540002 	cmp	r4, r2
8160c6e0:	1a000017 	bne	8160c744 <inflate_table+0xac>
8160c6e4:	e3a0100f 	mov	r1, #15
8160c6e8:	e1a03081 	lsl	r3, r1, #1
8160c6ec:	e28d2040 	add	r2, sp, #64	; 0x40
8160c6f0:	e19230b3 	ldrh	r3, [r2, r3]
8160c6f4:	e3530000 	cmp	r3, #0
8160c6f8:	1a0000de 	bne	8160ca78 <inflate_table+0x3e0>
8160c6fc:	e2511001 	subs	r1, r1, #1
8160c700:	1afffff8 	bne	8160c6e8 <inflate_table+0x50>
8160c704:	e59d300c 	ldr	r3, [sp, #12]
8160c708:	e3a02d05 	mov	r2, #320	; 0x140
8160c70c:	e59dc00c 	ldr	ip, [sp, #12]
8160c710:	e5933000 	ldr	r3, [r3]
8160c714:	e2830008 	add	r0, r3, #8
8160c718:	e1c320b0 	strh	r2, [r3]
8160c71c:	e1c310b2 	strh	r1, [r3, #2]
8160c720:	e58c0000 	str	r0, [ip]
8160c724:	e1a00001 	mov	r0, r1
8160c728:	e1c320b4 	strh	r2, [r3, #4]
8160c72c:	e59d20a0 	ldr	r2, [sp, #160]	; 0xa0
8160c730:	e1c310b6 	strh	r1, [r3, #6]
8160c734:	e3a03001 	mov	r3, #1
8160c738:	e5823000 	str	r3, [r2]
8160c73c:	e28dd080 	add	sp, sp, #128	; 0x80
8160c740:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160c744:	e1fe30b2 	ldrh	r3, [lr, #2]!
8160c748:	e28d1080 	add	r1, sp, #128	; 0x80
8160c74c:	e2844001 	add	r4, r4, #1
8160c750:	e0813083 	add	r3, r1, r3, lsl #1
8160c754:	e15314b0 	ldrh	r1, [r3, #-64]	; 0xffffffc0
8160c758:	e2811001 	add	r1, r1, #1
8160c75c:	e14314b0 	strh	r1, [r3, #-64]	; 0xffffffc0
8160c760:	eaffffdd 	b	8160c6dc <inflate_table+0x44>
8160c764:	e1520001 	cmp	r2, r1
8160c768:	21a02001 	movcs	r2, r1
8160c76c:	e1520003 	cmp	r2, r3
8160c770:	31a02003 	movcc	r2, r3
8160c774:	ea0000cb 	b	8160caa8 <inflate_table+0x410>
8160c778:	e1f7e0b2 	ldrh	lr, [r7, #2]!
8160c77c:	e35e0000 	cmp	lr, #0
8160c780:	0a000006 	beq	8160c7a0 <inflate_table+0x108>
8160c784:	e28d6080 	add	r6, sp, #128	; 0x80
8160c788:	e086e08e 	add	lr, r6, lr, lsl #1
8160c78c:	e15e62b0 	ldrh	r6, [lr, #-32]	; 0xffffffe0
8160c790:	e2868001 	add	r8, r6, #1
8160c794:	e14e82b0 	strh	r8, [lr, #-32]	; 0xffffffe0
8160c798:	e1a06086 	lsl	r6, r6, #1
8160c79c:	e18c50b6 	strh	r5, [ip, r6]
8160c7a0:	e2855001 	add	r5, r5, #1
8160c7a4:	ea0000de 	b	8160cb24 <inflate_table+0x48c>
8160c7a8:	e3a0e013 	mov	lr, #19
8160c7ac:	e58dc024 	str	ip, [sp, #36]	; 0x24
8160c7b0:	e58dc020 	str	ip, [sp, #32]
8160c7b4:	e58de028 	str	lr, [sp, #40]	; 0x28
8160c7b8:	ea0000e9 	b	8160cb64 <inflate_table+0x4cc>
8160c7bc:	e59de00c 	ldr	lr, [sp, #12]
8160c7c0:	e3e06000 	mvn	r6, #0
8160c7c4:	e58dc014 	str	ip, [sp, #20]
8160c7c8:	e1a08005 	mov	r8, r5
8160c7cc:	e1a0c004 	mov	ip, r4
8160c7d0:	e58d003c 	str	r0, [sp, #60]	; 0x3c
8160c7d4:	e59eb000 	ldr	fp, [lr]
8160c7d8:	e1a0e215 	lsl	lr, r5, r2
8160c7dc:	e24ee001 	sub	lr, lr, #1
8160c7e0:	e58de02c 	str	lr, [sp, #44]	; 0x2c
8160c7e4:	e1a0e002 	mov	lr, r2
8160c7e8:	e1a0a00b 	mov	sl, fp
8160c7ec:	e043000c 	sub	r0, r3, ip
8160c7f0:	e6ef0070 	uxtb	r0, r0
8160c7f4:	e58d0008 	str	r0, [sp, #8]
8160c7f8:	e59d0014 	ldr	r0, [sp, #20]
8160c7fc:	e0d050b2 	ldrh	r5, [r0], #2
8160c800:	e58d0014 	str	r0, [sp, #20]
8160c804:	e59d0028 	ldr	r0, [sp, #40]	; 0x28
8160c808:	e1550000 	cmp	r5, r0
8160c80c:	ba000040 	blt	8160c914 <inflate_table+0x27c>
8160c810:	da000042 	ble	8160c920 <inflate_table+0x288>
8160c814:	e59d0024 	ldr	r0, [sp, #36]	; 0x24
8160c818:	e1a05085 	lsl	r5, r5, #1
8160c81c:	e7d00005 	ldrb	r0, [r0, r5]
8160c820:	e58d0010 	str	r0, [sp, #16]
8160c824:	e59d0020 	ldr	r0, [sp, #32]
8160c828:	e19050b5 	ldrh	r5, [r0, r5]
8160c82c:	e043000c 	sub	r0, r3, ip
8160c830:	e1a07018 	lsl	r7, r8, r0
8160c834:	e58d7030 	str	r7, [sp, #48]	; 0x30
8160c838:	e1a07e18 	lsl	r7, r8, lr
8160c83c:	e58d701c 	str	r7, [sp, #28]
8160c840:	e3e07003 	mvn	r7, #3
8160c844:	e1a00017 	lsl	r0, r7, r0
8160c848:	e59d7030 	ldr	r7, [sp, #48]	; 0x30
8160c84c:	e58d0038 	str	r0, [sp, #56]	; 0x38
8160c850:	e1a00e18 	lsl	r0, r8, lr
8160c854:	e0800c34 	add	r0, r0, r4, lsr ip
8160c858:	e0400007 	sub	r0, r0, r7
8160c85c:	e08a0100 	add	r0, sl, r0, lsl #2
8160c860:	e58d0000 	str	r0, [sp]
8160c864:	e59d0000 	ldr	r0, [sp]
8160c868:	e59d7010 	ldr	r7, [sp, #16]
8160c86c:	e1c050b2 	strh	r5, [r0, #2]
8160c870:	e5c07000 	strb	r7, [r0]
8160c874:	e59d7008 	ldr	r7, [sp, #8]
8160c878:	e5c07001 	strb	r7, [r0, #1]
8160c87c:	e59d7038 	ldr	r7, [sp, #56]	; 0x38
8160c880:	e0800007 	add	r0, r0, r7
8160c884:	e59d7030 	ldr	r7, [sp, #48]	; 0x30
8160c888:	e58d0000 	str	r0, [sp]
8160c88c:	e59d001c 	ldr	r0, [sp, #28]
8160c890:	e0507007 	subs	r7, r0, r7
8160c894:	e58d701c 	str	r7, [sp, #28]
8160c898:	1afffff1 	bne	8160c864 <inflate_table+0x1cc>
8160c89c:	e2430001 	sub	r0, r3, #1
8160c8a0:	e1a00018 	lsl	r0, r8, r0
8160c8a4:	e1140000 	tst	r4, r0
8160c8a8:	1a00001f 	bne	8160c92c <inflate_table+0x294>
8160c8ac:	e3500000 	cmp	r0, #0
8160c8b0:	12405001 	subne	r5, r0, #1
8160c8b4:	10044005 	andne	r4, r4, r5
8160c8b8:	10800004 	addne	r0, r0, r4
8160c8bc:	e28d4080 	add	r4, sp, #128	; 0x80
8160c8c0:	e0845083 	add	r5, r4, r3, lsl #1
8160c8c4:	e15544b0 	ldrh	r4, [r5, #-64]	; 0xffffffc0
8160c8c8:	e2444001 	sub	r4, r4, #1
8160c8cc:	e6ff4074 	uxth	r4, r4
8160c8d0:	e14544b0 	strh	r4, [r5, #-64]	; 0xffffffc0
8160c8d4:	e3540000 	cmp	r4, #0
8160c8d8:	1a00001a 	bne	8160c948 <inflate_table+0x2b0>
8160c8dc:	e1530001 	cmp	r3, r1
8160c8e0:	1a000013 	bne	8160c934 <inflate_table+0x29c>
8160c8e4:	e6ef5072 	uxtb	r5, r2
8160c8e8:	e3a07040 	mov	r7, #64	; 0x40
8160c8ec:	e3a08001 	mov	r8, #1
8160c8f0:	e3500000 	cmp	r0, #0
8160c8f4:	1a000040 	bne	8160c9fc <inflate_table+0x364>
8160c8f8:	e59d3004 	ldr	r3, [sp, #4]
8160c8fc:	e59d100c 	ldr	r1, [sp, #12]
8160c900:	e08b3103 	add	r3, fp, r3, lsl #2
8160c904:	e5813000 	str	r3, [r1]
8160c908:	e59d30a0 	ldr	r3, [sp, #160]	; 0xa0
8160c90c:	e5832000 	str	r2, [r3]
8160c910:	eaffff89 	b	8160c73c <inflate_table+0xa4>
8160c914:	e3a00000 	mov	r0, #0
8160c918:	e58d0010 	str	r0, [sp, #16]
8160c91c:	eaffffc2 	b	8160c82c <inflate_table+0x194>
8160c920:	e3a05000 	mov	r5, #0
8160c924:	e3a00060 	mov	r0, #96	; 0x60
8160c928:	eafffffa 	b	8160c918 <inflate_table+0x280>
8160c92c:	e1a000a0 	lsr	r0, r0, #1
8160c930:	eaffffdb 	b	8160c8a4 <inflate_table+0x20c>
8160c934:	e59d3014 	ldr	r3, [sp, #20]
8160c938:	e59d4034 	ldr	r4, [sp, #52]	; 0x34
8160c93c:	e1d330b0 	ldrh	r3, [r3]
8160c940:	e1a03083 	lsl	r3, r3, #1
8160c944:	e19430b3 	ldrh	r3, [r4, r3]
8160c948:	e1530002 	cmp	r3, r2
8160c94c:	9a000025 	bls	8160c9e8 <inflate_table+0x350>
8160c950:	e59d402c 	ldr	r4, [sp, #44]	; 0x2c
8160c954:	e0004004 	and	r4, r0, r4
8160c958:	e1540006 	cmp	r4, r6
8160c95c:	0a000021 	beq	8160c9e8 <inflate_table+0x350>
8160c960:	e3a05004 	mov	r5, #4
8160c964:	e35c0000 	cmp	ip, #0
8160c968:	e08aae15 	add	sl, sl, r5, lsl lr
8160c96c:	e28d5040 	add	r5, sp, #64	; 0x40
8160c970:	01a0c002 	moveq	ip, r2
8160c974:	e0855083 	add	r5, r5, r3, lsl #1
8160c978:	e043e00c 	sub	lr, r3, ip
8160c97c:	e2455002 	sub	r5, r5, #2
8160c980:	e1a07e18 	lsl	r7, r8, lr
8160c984:	e08e600c 	add	r6, lr, ip
8160c988:	e1560001 	cmp	r6, r1
8160c98c:	2a000003 	bcs	8160c9a0 <inflate_table+0x308>
8160c990:	e1f560b2 	ldrh	r6, [r5, #2]!
8160c994:	e0477006 	sub	r7, r7, r6
8160c998:	e3570000 	cmp	r7, #0
8160c99c:	ca000013 	bgt	8160c9f0 <inflate_table+0x358>
8160c9a0:	e59d5004 	ldr	r5, [sp, #4]
8160c9a4:	e59d703c 	ldr	r7, [sp, #60]	; 0x3c
8160c9a8:	e0855e18 	add	r5, r5, r8, lsl lr
8160c9ac:	e58d5004 	str	r5, [sp, #4]
8160c9b0:	e59d6004 	ldr	r6, [sp, #4]
8160c9b4:	e59d5018 	ldr	r5, [sp, #24]
8160c9b8:	e1560007 	cmp	r6, r7
8160c9bc:	93a05000 	movls	r5, #0
8160c9c0:	82055001 	andhi	r5, r5, #1
8160c9c4:	e3550000 	cmp	r5, #0
8160c9c8:	1a000073 	bne	8160cb9c <inflate_table+0x504>
8160c9cc:	e08b6104 	add	r6, fp, r4, lsl #2
8160c9d0:	e04a500b 	sub	r5, sl, fp
8160c9d4:	e7cbe104 	strb	lr, [fp, r4, lsl #2]
8160c9d8:	e1a05145 	asr	r5, r5, #2
8160c9dc:	e5c62001 	strb	r2, [r6, #1]
8160c9e0:	e1c650b2 	strh	r5, [r6, #2]
8160c9e4:	e1a06004 	mov	r6, r4
8160c9e8:	e1a04000 	mov	r4, r0
8160c9ec:	eaffff7e 	b	8160c7ec <inflate_table+0x154>
8160c9f0:	e28ee001 	add	lr, lr, #1
8160c9f4:	e1a07087 	lsl	r7, r7, #1
8160c9f8:	eaffffe1 	b	8160c984 <inflate_table+0x2ec>
8160c9fc:	e35c0000 	cmp	ip, #0
8160ca00:	0a000008 	beq	8160ca28 <inflate_table+0x390>
8160ca04:	e59d102c 	ldr	r1, [sp, #44]	; 0x2c
8160ca08:	e59de008 	ldr	lr, [sp, #8]
8160ca0c:	e0001001 	and	r1, r0, r1
8160ca10:	e1510006 	cmp	r1, r6
8160ca14:	11a0a00b 	movne	sl, fp
8160ca18:	13a0c000 	movne	ip, #0
8160ca1c:	11a03002 	movne	r3, r2
8160ca20:	11a0e005 	movne	lr, r5
8160ca24:	e58de008 	str	lr, [sp, #8]
8160ca28:	e1a0ec30 	lsr	lr, r0, ip
8160ca2c:	e08a110e 	add	r1, sl, lr, lsl #2
8160ca30:	e7ca710e 	strb	r7, [sl, lr, lsl #2]
8160ca34:	e59de008 	ldr	lr, [sp, #8]
8160ca38:	e1c140b2 	strh	r4, [r1, #2]
8160ca3c:	e5c1e001 	strb	lr, [r1, #1]
8160ca40:	e243e001 	sub	lr, r3, #1
8160ca44:	e1a0ee18 	lsl	lr, r8, lr
8160ca48:	e110000e 	tst	r0, lr
8160ca4c:	1a000005 	bne	8160ca68 <inflate_table+0x3d0>
8160ca50:	e35e0000 	cmp	lr, #0
8160ca54:	124e1001 	subne	r1, lr, #1
8160ca58:	10000001 	andne	r0, r0, r1
8160ca5c:	108ee000 	addne	lr, lr, r0
8160ca60:	e1a0000e 	mov	r0, lr
8160ca64:	eaffffa1 	b	8160c8f0 <inflate_table+0x258>
8160ca68:	e1a0e0ae 	lsr	lr, lr, #1
8160ca6c:	eafffff5 	b	8160ca48 <inflate_table+0x3b0>
8160ca70:	e3e00000 	mvn	r0, #0
8160ca74:	eaffff30 	b	8160c73c <inflate_table+0xa4>
8160ca78:	e59d30a0 	ldr	r3, [sp, #160]	; 0xa0
8160ca7c:	e5932000 	ldr	r2, [r3]
8160ca80:	e3a03001 	mov	r3, #1
8160ca84:	e1a0e083 	lsl	lr, r3, #1
8160ca88:	e28d5040 	add	r5, sp, #64	; 0x40
8160ca8c:	e195e0be 	ldrh	lr, [r5, lr]
8160ca90:	e35e0000 	cmp	lr, #0
8160ca94:	1affff32 	bne	8160c764 <inflate_table+0xcc>
8160ca98:	e2833001 	add	r3, r3, #1
8160ca9c:	e3530010 	cmp	r3, #16
8160caa0:	1afffff7 	bne	8160ca84 <inflate_table+0x3ec>
8160caa4:	e1a02003 	mov	r2, r3
8160caa8:	e3a05000 	mov	r5, #0
8160caac:	e3a0e001 	mov	lr, #1
8160cab0:	e28d6040 	add	r6, sp, #64	; 0x40
8160cab4:	e0866005 	add	r6, r6, r5
8160cab8:	e1d660b2 	ldrh	r6, [r6, #2]
8160cabc:	e076e08e 	rsbs	lr, r6, lr, lsl #1
8160cac0:	4affffea 	bmi	8160ca70 <inflate_table+0x3d8>
8160cac4:	e2855002 	add	r5, r5, #2
8160cac8:	e355001e 	cmp	r5, #30
8160cacc:	1afffff7 	bne	8160cab0 <inflate_table+0x418>
8160cad0:	e35e0000 	cmp	lr, #0
8160cad4:	0a000005 	beq	8160caf0 <inflate_table+0x458>
8160cad8:	e251e001 	subs	lr, r1, #1
8160cadc:	13a0e001 	movne	lr, #1
8160cae0:	e3500000 	cmp	r0, #0
8160cae4:	03a0e001 	moveq	lr, #1
8160cae8:	e35e0000 	cmp	lr, #0
8160caec:	1affffdf 	bne	8160ca70 <inflate_table+0x3d8>
8160caf0:	e28de080 	add	lr, sp, #128	; 0x80
8160caf4:	e3a05000 	mov	r5, #0
8160caf8:	e16e51be 	strh	r5, [lr, #-30]!	; 0xffffffe2
8160cafc:	e3a05002 	mov	r5, #2
8160cb00:	e28d8040 	add	r8, sp, #64	; 0x40
8160cb04:	e1de60b0 	ldrh	r6, [lr]
8160cb08:	e19880b5 	ldrh	r8, [r8, r5]
8160cb0c:	e2855002 	add	r5, r5, #2
8160cb10:	e355001e 	cmp	r5, #30
8160cb14:	e0866008 	add	r6, r6, r8
8160cb18:	e1ee60b2 	strh	r6, [lr, #2]!
8160cb1c:	1afffff7 	bne	8160cb00 <inflate_table+0x468>
8160cb20:	e3a05000 	mov	r5, #0
8160cb24:	e1540005 	cmp	r4, r5
8160cb28:	1affff12 	bne	8160c778 <inflate_table+0xe0>
8160cb2c:	e3500000 	cmp	r0, #0
8160cb30:	0affff1c 	beq	8160c7a8 <inflate_table+0x110>
8160cb34:	e3500001 	cmp	r0, #1
8160cb38:	e59f4064 	ldr	r4, [pc, #100]	; 8160cba4 <inflate_table+0x50c>
8160cb3c:	03a0ec01 	moveq	lr, #256	; 0x100
8160cb40:	13e0e000 	mvnne	lr, #0
8160cb44:	e58de028 	str	lr, [sp, #40]	; 0x28
8160cb48:	e59fe058 	ldr	lr, [pc, #88]	; 8160cba8 <inflate_table+0x510>
8160cb4c:	01a0e004 	moveq	lr, r4
8160cb50:	e59f4054 	ldr	r4, [pc, #84]	; 8160cbac <inflate_table+0x514>
8160cb54:	e58de024 	str	lr, [sp, #36]	; 0x24
8160cb58:	e59fe050 	ldr	lr, [pc, #80]	; 8160cbb0 <inflate_table+0x518>
8160cb5c:	01a0e004 	moveq	lr, r4
8160cb60:	e58de020 	str	lr, [sp, #32]
8160cb64:	e3a05001 	mov	r5, #1
8160cb68:	e0400005 	sub	r0, r0, r5
8160cb6c:	e16f0f10 	clz	r0, r0
8160cb70:	e1a0e215 	lsl	lr, r5, r2
8160cb74:	e58de004 	str	lr, [sp, #4]
8160cb78:	e1a002a0 	lsr	r0, r0, #5
8160cb7c:	e58d0018 	str	r0, [sp, #24]
8160cb80:	e59d4018 	ldr	r4, [sp, #24]
8160cb84:	e30005af 	movw	r0, #1455	; 0x5af
8160cb88:	e15e0000 	cmp	lr, r0
8160cb8c:	93a04000 	movls	r4, #0
8160cb90:	82044001 	andhi	r4, r4, #1
8160cb94:	e3540000 	cmp	r4, #0
8160cb98:	0affff07 	beq	8160c7bc <inflate_table+0x124>
8160cb9c:	e3a00001 	mov	r0, #1
8160cba0:	eafffee5 	b	8160c73c <inflate_table+0xa4>
8160cba4:	81613df0 	.word	0x81613df0
8160cba8:	816136f4 	.word	0x816136f4
8160cbac:	816135b2 	.word	0x816135b2
8160cbb0:	816136b4 	.word	0x816136b4

8160cbb4 <inflateReset>:
8160cbb4:	e2502000 	subs	r2, r0, #0
8160cbb8:	0a000019 	beq	8160cc24 <inflateReset+0x70>
8160cbbc:	e592301c 	ldr	r3, [r2, #28]
8160cbc0:	e3530000 	cmp	r3, #0
8160cbc4:	0a000016 	beq	8160cc24 <inflateReset+0x70>
8160cbc8:	e3a00000 	mov	r0, #0
8160cbcc:	e3a01001 	mov	r1, #1
8160cbd0:	e583001c 	str	r0, [r3, #28]
8160cbd4:	e5820014 	str	r0, [r2, #20]
8160cbd8:	e5820008 	str	r0, [r2, #8]
8160cbdc:	e5820018 	str	r0, [r2, #24]
8160cbe0:	e5821034 	str	r1, [r2, #52]	; 0x34
8160cbe4:	e3a02902 	mov	r2, #32768	; 0x8000
8160cbe8:	e5830000 	str	r0, [r3]
8160cbec:	e5832014 	str	r2, [r3, #20]
8160cbf0:	e2832e53 	add	r2, r3, #1328	; 0x530
8160cbf4:	e5830004 	str	r0, [r3, #4]
8160cbf8:	e583000c 	str	r0, [r3, #12]
8160cbfc:	e5830020 	str	r0, [r3, #32]
8160cc00:	e5830028 	str	r0, [r3, #40]	; 0x28
8160cc04:	e583002c 	str	r0, [r3, #44]	; 0x2c
8160cc08:	e5830030 	str	r0, [r3, #48]	; 0x30
8160cc0c:	e5830038 	str	r0, [r3, #56]	; 0x38
8160cc10:	e583003c 	str	r0, [r3, #60]	; 0x3c
8160cc14:	e583206c 	str	r2, [r3, #108]	; 0x6c
8160cc18:	e5832050 	str	r2, [r3, #80]	; 0x50
8160cc1c:	e583204c 	str	r2, [r3, #76]	; 0x4c
8160cc20:	e12fff1e 	bx	lr
8160cc24:	e3e00001 	mvn	r0, #1
8160cc28:	e12fff1e 	bx	lr

8160cc2c <inflateInit2_>:
8160cc2c:	e3520000 	cmp	r2, #0
8160cc30:	0a000037 	beq	8160cd14 <inflateInit2_+0xe8>
8160cc34:	e5d22000 	ldrb	r2, [r2]
8160cc38:	e353003c 	cmp	r3, #60	; 0x3c
8160cc3c:	03520031 	cmpeq	r2, #49	; 0x31
8160cc40:	13a02001 	movne	r2, #1
8160cc44:	03a02000 	moveq	r2, #0
8160cc48:	1a000031 	bne	8160cd14 <inflateInit2_+0xe8>
8160cc4c:	e3500000 	cmp	r0, #0
8160cc50:	e92d4070 	push	{r4, r5, r6, lr}
8160cc54:	e1a04000 	mov	r4, r0
8160cc58:	0a000025 	beq	8160ccf4 <inflateInit2_+0xc8>
8160cc5c:	e5903020 	ldr	r3, [r0, #32]
8160cc60:	e1a05001 	mov	r5, r1
8160cc64:	e5802018 	str	r2, [r0, #24]
8160cc68:	e3a01001 	mov	r1, #1
8160cc6c:	e3530000 	cmp	r3, #0
8160cc70:	05803028 	streq	r3, [r0, #40]	; 0x28
8160cc74:	059f20a8 	ldreq	r2, [pc, #168]	; 8160cd24 <inflateInit2_+0xf8>
8160cc78:	e5903024 	ldr	r3, [r0, #36]	; 0x24
8160cc7c:	05802020 	streq	r2, [r0, #32]
8160cc80:	e3530000 	cmp	r3, #0
8160cc84:	e3022530 	movw	r2, #9520	; 0x2530
8160cc88:	059f3098 	ldreq	r3, [pc, #152]	; 8160cd28 <inflateInit2_+0xfc>
8160cc8c:	05803024 	streq	r3, [r0, #36]	; 0x24
8160cc90:	e5903020 	ldr	r3, [r0, #32]
8160cc94:	e5900028 	ldr	r0, [r0, #40]	; 0x28
8160cc98:	e12fff33 	blx	r3
8160cc9c:	e2501000 	subs	r1, r0, #0
8160cca0:	0a00001d 	beq	8160cd1c <inflateInit2_+0xf0>
8160cca4:	e3550000 	cmp	r5, #0
8160cca8:	e584101c 	str	r1, [r4, #28]
8160ccac:	b2655000 	rsblt	r5, r5, #0
8160ccb0:	b3a03000 	movlt	r3, #0
8160ccb4:	b5813008 	strlt	r3, [r1, #8]
8160ccb8:	ba000004 	blt	8160ccd0 <inflateInit2_+0xa4>
8160ccbc:	e355002f 	cmp	r5, #47	; 0x2f
8160ccc0:	e1a03245 	asr	r3, r5, #4
8160ccc4:	d205500f 	andle	r5, r5, #15
8160ccc8:	e2833001 	add	r3, r3, #1
8160cccc:	e5813008 	str	r3, [r1, #8]
8160ccd0:	e2453008 	sub	r3, r5, #8
8160ccd4:	e3530007 	cmp	r3, #7
8160ccd8:	9a000007 	bls	8160ccfc <inflateInit2_+0xd0>
8160ccdc:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160cce0:	e3a02000 	mov	r2, #0
8160cce4:	e5940028 	ldr	r0, [r4, #40]	; 0x28
8160cce8:	e12fff33 	blx	r3
8160ccec:	e3a03000 	mov	r3, #0
8160ccf0:	e584301c 	str	r3, [r4, #28]
8160ccf4:	e3e00001 	mvn	r0, #1
8160ccf8:	e8bd8070 	pop	{r4, r5, r6, pc}
8160ccfc:	e5815024 	str	r5, [r1, #36]	; 0x24
8160cd00:	e3a03000 	mov	r3, #0
8160cd04:	e1a00004 	mov	r0, r4
8160cd08:	e5813034 	str	r3, [r1, #52]	; 0x34
8160cd0c:	e8bd4070 	pop	{r4, r5, r6, lr}
8160cd10:	eaffffa7 	b	8160cbb4 <inflateReset>
8160cd14:	e3e00005 	mvn	r0, #5
8160cd18:	e12fff1e 	bx	lr
8160cd1c:	e3e00003 	mvn	r0, #3
8160cd20:	e8bd8070 	pop	{r4, r5, r6, pc}
8160cd24:	8160c114 	.word	0x8160c114
8160cd28:	8160c11c 	.word	0x8160c11c

8160cd2c <inflateEnd>:
8160cd2c:	e92d4010 	push	{r4, lr}
8160cd30:	e2504000 	subs	r4, r0, #0
8160cd34:	0a000013 	beq	8160cd88 <inflateEnd+0x5c>
8160cd38:	e594201c 	ldr	r2, [r4, #28]
8160cd3c:	e3520000 	cmp	r2, #0
8160cd40:	0a000010 	beq	8160cd88 <inflateEnd+0x5c>
8160cd44:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160cd48:	e3530000 	cmp	r3, #0
8160cd4c:	0a00000d 	beq	8160cd88 <inflateEnd+0x5c>
8160cd50:	e5921034 	ldr	r1, [r2, #52]	; 0x34
8160cd54:	e5940028 	ldr	r0, [r4, #40]	; 0x28
8160cd58:	e3510000 	cmp	r1, #0
8160cd5c:	0a000001 	beq	8160cd68 <inflateEnd+0x3c>
8160cd60:	e3a02000 	mov	r2, #0
8160cd64:	e12fff33 	blx	r3
8160cd68:	e5940028 	ldr	r0, [r4, #40]	; 0x28
8160cd6c:	e3a02000 	mov	r2, #0
8160cd70:	e5943024 	ldr	r3, [r4, #36]	; 0x24
8160cd74:	e594101c 	ldr	r1, [r4, #28]
8160cd78:	e12fff33 	blx	r3
8160cd7c:	e3a00000 	mov	r0, #0
8160cd80:	e584001c 	str	r0, [r4, #28]
8160cd84:	e8bd8010 	pop	{r4, pc}
8160cd88:	e3e00001 	mvn	r0, #1
8160cd8c:	e8bd8010 	pop	{r4, pc}

8160cd90 <adler32>:
8160cd90:	e3520001 	cmp	r2, #1
8160cd94:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160cd98:	e1a06001 	mov	r6, r1
8160cd9c:	e24dd020 	sub	sp, sp, #32
8160cda0:	e1a05820 	lsr	r5, r0, #16
8160cda4:	e6ff4070 	uxth	r4, r0
8160cda8:	1a00000b 	bne	8160cddc <adler32+0x4c>
8160cdac:	e5d10000 	ldrb	r0, [r1]
8160cdb0:	e30f3ff0 	movw	r3, #65520	; 0xfff0
8160cdb4:	e0804004 	add	r4, r0, r4
8160cdb8:	e1540003 	cmp	r4, r3
8160cdbc:	82444cff 	subhi	r4, r4, #65280	; 0xff00
8160cdc0:	824440f1 	subhi	r4, r4, #241	; 0xf1
8160cdc4:	e0845005 	add	r5, r4, r5
8160cdc8:	e1550003 	cmp	r5, r3
8160cdcc:	82455cff 	subhi	r5, r5, #65280	; 0xff00
8160cdd0:	824550f1 	subhi	r5, r5, #241	; 0xf1
8160cdd4:	e1840805 	orr	r0, r4, r5, lsl #16
8160cdd8:	ea00002f 	b	8160ce9c <adler32+0x10c>
8160cddc:	e3510000 	cmp	r1, #0
8160cde0:	03a00001 	moveq	r0, #1
8160cde4:	0a00002c 	beq	8160ce9c <adler32+0x10c>
8160cde8:	e352000f 	cmp	r2, #15
8160cdec:	e1a07002 	mov	r7, r2
8160cdf0:	90817002 	addls	r7, r1, r2
8160cdf4:	9a00001e 	bls	8160ce74 <adler32+0xe4>
8160cdf8:	e30135af 	movw	r3, #5551	; 0x15af
8160cdfc:	e1570003 	cmp	r7, r3
8160ce00:	8a000027 	bhi	8160cea4 <adler32+0x114>
8160ce04:	e3570000 	cmp	r7, #0
8160ce08:	0afffff1 	beq	8160cdd4 <adler32+0x44>
8160ce0c:	e1a02227 	lsr	r2, r7, #4
8160ce10:	e286c010 	add	ip, r6, #16
8160ce14:	e1a03006 	mov	r3, r6
8160ce18:	e1a0e202 	lsl	lr, r2, #4
8160ce1c:	e08cc00e 	add	ip, ip, lr
8160ce20:	e2833010 	add	r3, r3, #16
8160ce24:	e15c0003 	cmp	ip, r3
8160ce28:	1a000073 	bne	8160cffc <adler32+0x26c>
8160ce2c:	e3e0300f 	mvn	r3, #15
8160ce30:	e086600e 	add	r6, r6, lr
8160ce34:	e0227293 	mla	r2, r3, r2, r7
8160ce38:	e0862002 	add	r2, r6, r2
8160ce3c:	e1560002 	cmp	r6, r2
8160ce40:	1a00009e 	bne	8160d0c0 <adler32+0x330>
8160ce44:	e1a00004 	mov	r0, r4
8160ce48:	e30f1ff1 	movw	r1, #65521	; 0xfff1
8160ce4c:	eb00178d 	bl	81612c88 <__aeabi_uidivmod>
8160ce50:	e1a00005 	mov	r0, r5
8160ce54:	e1a04001 	mov	r4, r1
8160ce58:	e30f1ff1 	movw	r1, #65521	; 0xfff1
8160ce5c:	eb001789 	bl	81612c88 <__aeabi_uidivmod>
8160ce60:	e1a05001 	mov	r5, r1
8160ce64:	eaffffda 	b	8160cdd4 <adler32+0x44>
8160ce68:	e4d63001 	ldrb	r3, [r6], #1
8160ce6c:	e0844003 	add	r4, r4, r3
8160ce70:	e0855004 	add	r5, r5, r4
8160ce74:	e1560007 	cmp	r6, r7
8160ce78:	1afffffa 	bne	8160ce68 <adler32+0xd8>
8160ce7c:	e30f3ff0 	movw	r3, #65520	; 0xfff0
8160ce80:	e1540003 	cmp	r4, r3
8160ce84:	82444cff 	subhi	r4, r4, #65280	; 0xff00
8160ce88:	e30f1ff1 	movw	r1, #65521	; 0xfff1
8160ce8c:	e1a00005 	mov	r0, r5
8160ce90:	824440f1 	subhi	r4, r4, #241	; 0xf1
8160ce94:	eb00177b 	bl	81612c88 <__aeabi_uidivmod>
8160ce98:	e1840801 	orr	r0, r4, r1, lsl #16
8160ce9c:	e28dd020 	add	sp, sp, #32
8160cea0:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160cea4:	e2477d56 	sub	r7, r7, #5504	; 0x1580
8160cea8:	e286c010 	add	ip, r6, #16
8160ceac:	e2477030 	sub	r7, r7, #48	; 0x30
8160ceb0:	e2863d57 	add	r3, r6, #5568	; 0x15c0
8160ceb4:	e58d301c 	str	r3, [sp, #28]
8160ceb8:	e55c3010 	ldrb	r3, [ip, #-16]
8160cebc:	e28cc010 	add	ip, ip, #16
8160cec0:	e55c001f 	ldrb	r0, [ip, #-31]	; 0xffffffe1
8160cec4:	e55c101e 	ldrb	r1, [ip, #-30]	; 0xffffffe2
8160cec8:	e0833004 	add	r3, r3, r4
8160cecc:	e55c201d 	ldrb	r2, [ip, #-29]	; 0xffffffe3
8160ced0:	e0800003 	add	r0, r0, r3
8160ced4:	e0811000 	add	r1, r1, r0
8160ced8:	e0833000 	add	r3, r3, r0
8160cedc:	e0822001 	add	r2, r2, r1
8160cee0:	e58d2000 	str	r2, [sp]
8160cee4:	e59de000 	ldr	lr, [sp]
8160cee8:	e0833001 	add	r3, r3, r1
8160ceec:	e55c201c 	ldrb	r2, [ip, #-28]	; 0xffffffe4
8160cef0:	e59d1000 	ldr	r1, [sp]
8160cef4:	e082200e 	add	r2, r2, lr
8160cef8:	e58d2004 	str	r2, [sp, #4]
8160cefc:	e59de004 	ldr	lr, [sp, #4]
8160cf00:	e0833001 	add	r3, r3, r1
8160cf04:	e55c201b 	ldrb	r2, [ip, #-27]	; 0xffffffe5
8160cf08:	e59d1004 	ldr	r1, [sp, #4]
8160cf0c:	e082200e 	add	r2, r2, lr
8160cf10:	e58d2008 	str	r2, [sp, #8]
8160cf14:	e59de008 	ldr	lr, [sp, #8]
8160cf18:	e0833001 	add	r3, r3, r1
8160cf1c:	e55c201a 	ldrb	r2, [ip, #-26]	; 0xffffffe6
8160cf20:	e55cb018 	ldrb	fp, [ip, #-24]	; 0xffffffe8
8160cf24:	e082200e 	add	r2, r2, lr
8160cf28:	e58d200c 	str	r2, [sp, #12]
8160cf2c:	e59de00c 	ldr	lr, [sp, #12]
8160cf30:	e55c2019 	ldrb	r2, [ip, #-25]	; 0xffffffe7
8160cf34:	e59d1008 	ldr	r1, [sp, #8]
8160cf38:	e082200e 	add	r2, r2, lr
8160cf3c:	e58d2010 	str	r2, [sp, #16]
8160cf40:	e0833001 	add	r3, r3, r1
8160cf44:	e08b2002 	add	r2, fp, r2
8160cf48:	e59d100c 	ldr	r1, [sp, #12]
8160cf4c:	e55cb017 	ldrb	fp, [ip, #-23]	; 0xffffffe9
8160cf50:	e58d2014 	str	r2, [sp, #20]
8160cf54:	e0833001 	add	r3, r3, r1
8160cf58:	e08b2002 	add	r2, fp, r2
8160cf5c:	e59d1010 	ldr	r1, [sp, #16]
8160cf60:	e55cb016 	ldrb	fp, [ip, #-22]	; 0xffffffea
8160cf64:	e55ca015 	ldrb	sl, [ip, #-21]	; 0xffffffeb
8160cf68:	e0833001 	add	r3, r3, r1
8160cf6c:	e55c8014 	ldrb	r8, [ip, #-20]	; 0xffffffec
8160cf70:	e08bb002 	add	fp, fp, r2
8160cf74:	e55ce013 	ldrb	lr, [ip, #-19]	; 0xffffffed
8160cf78:	e08aa00b 	add	sl, sl, fp
8160cf7c:	e55c4011 	ldrb	r4, [ip, #-17]	; 0xffffffef
8160cf80:	e088800a 	add	r8, r8, sl
8160cf84:	e58d2018 	str	r2, [sp, #24]
8160cf88:	e08ee008 	add	lr, lr, r8
8160cf8c:	e55c2012 	ldrb	r2, [ip, #-18]	; 0xffffffee
8160cf90:	e59d1014 	ldr	r1, [sp, #20]
8160cf94:	e082200e 	add	r2, r2, lr
8160cf98:	e0833001 	add	r3, r3, r1
8160cf9c:	e59d1018 	ldr	r1, [sp, #24]
8160cfa0:	e0844002 	add	r4, r4, r2
8160cfa4:	e0833001 	add	r3, r3, r1
8160cfa8:	e083300b 	add	r3, r3, fp
8160cfac:	e083a00a 	add	sl, r3, sl
8160cfb0:	e59d301c 	ldr	r3, [sp, #28]
8160cfb4:	e08a8008 	add	r8, sl, r8
8160cfb8:	e088e00e 	add	lr, r8, lr
8160cfbc:	e153000c 	cmp	r3, ip
8160cfc0:	e08e2002 	add	r2, lr, r2
8160cfc4:	e0822004 	add	r2, r2, r4
8160cfc8:	e0855002 	add	r5, r5, r2
8160cfcc:	1affffb9 	bne	8160ceb8 <adler32+0x128>
8160cfd0:	e1a00004 	mov	r0, r4
8160cfd4:	e30f1ff1 	movw	r1, #65521	; 0xfff1
8160cfd8:	eb00172a 	bl	81612c88 <__aeabi_uidivmod>
8160cfdc:	e1a00005 	mov	r0, r5
8160cfe0:	e2866d56 	add	r6, r6, #5504	; 0x1580
8160cfe4:	e2866030 	add	r6, r6, #48	; 0x30
8160cfe8:	e1a04001 	mov	r4, r1
8160cfec:	e30f1ff1 	movw	r1, #65521	; 0xfff1
8160cff0:	eb001724 	bl	81612c88 <__aeabi_uidivmod>
8160cff4:	e1a05001 	mov	r5, r1
8160cff8:	eaffff7e 	b	8160cdf8 <adler32+0x68>
8160cffc:	e5530010 	ldrb	r0, [r3, #-16]
8160d000:	e553100f 	ldrb	r1, [r3, #-15]
8160d004:	e0804004 	add	r4, r0, r4
8160d008:	e0811004 	add	r1, r1, r4
8160d00c:	e0844001 	add	r4, r4, r1
8160d010:	e0845005 	add	r5, r4, r5
8160d014:	e553400e 	ldrb	r4, [r3, #-14]
8160d018:	e0841001 	add	r1, r4, r1
8160d01c:	e553400d 	ldrb	r4, [r3, #-13]
8160d020:	e0855001 	add	r5, r5, r1
8160d024:	e0844001 	add	r4, r4, r1
8160d028:	e553100c 	ldrb	r1, [r3, #-12]
8160d02c:	e0855004 	add	r5, r5, r4
8160d030:	e0814004 	add	r4, r1, r4
8160d034:	e553100b 	ldrb	r1, [r3, #-11]
8160d038:	e0855004 	add	r5, r5, r4
8160d03c:	e0811004 	add	r1, r1, r4
8160d040:	e553400a 	ldrb	r4, [r3, #-10]
8160d044:	e0855001 	add	r5, r5, r1
8160d048:	e0841001 	add	r1, r4, r1
8160d04c:	e5534009 	ldrb	r4, [r3, #-9]
8160d050:	e0855001 	add	r5, r5, r1
8160d054:	e0844001 	add	r4, r4, r1
8160d058:	e5531008 	ldrb	r1, [r3, #-8]
8160d05c:	e0855004 	add	r5, r5, r4
8160d060:	e0814004 	add	r4, r1, r4
8160d064:	e5531007 	ldrb	r1, [r3, #-7]
8160d068:	e0855004 	add	r5, r5, r4
8160d06c:	e0811004 	add	r1, r1, r4
8160d070:	e5534006 	ldrb	r4, [r3, #-6]
8160d074:	e0855001 	add	r5, r5, r1
8160d078:	e0841001 	add	r1, r4, r1
8160d07c:	e5534005 	ldrb	r4, [r3, #-5]
8160d080:	e0855001 	add	r5, r5, r1
8160d084:	e0844001 	add	r4, r4, r1
8160d088:	e5531004 	ldrb	r1, [r3, #-4]
8160d08c:	e0855004 	add	r5, r5, r4
8160d090:	e0814004 	add	r4, r1, r4
8160d094:	e5531003 	ldrb	r1, [r3, #-3]
8160d098:	e0855004 	add	r5, r5, r4
8160d09c:	e0811004 	add	r1, r1, r4
8160d0a0:	e5534002 	ldrb	r4, [r3, #-2]
8160d0a4:	e0855001 	add	r5, r5, r1
8160d0a8:	e0841001 	add	r1, r4, r1
8160d0ac:	e5534001 	ldrb	r4, [r3, #-1]
8160d0b0:	e0855001 	add	r5, r5, r1
8160d0b4:	e0844001 	add	r4, r4, r1
8160d0b8:	e0855004 	add	r5, r5, r4
8160d0bc:	eaffff57 	b	8160ce20 <adler32+0x90>
8160d0c0:	e4d63001 	ldrb	r3, [r6], #1
8160d0c4:	e0844003 	add	r4, r4, r3
8160d0c8:	e0855004 	add	r5, r5, r4
8160d0cc:	eaffff5a 	b	8160ce3c <adler32+0xac>

8160d0d0 <inflate>:
8160d0d0:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160d0d4:	e250a000 	subs	sl, r0, #0
8160d0d8:	e24dd030 	sub	sp, sp, #48	; 0x30
8160d0dc:	0a000523 	beq	8160e570 <inflate+0x14a0>
8160d0e0:	e59a401c 	ldr	r4, [sl, #28]
8160d0e4:	e3540000 	cmp	r4, #0
8160d0e8:	0a000520 	beq	8160e570 <inflate+0x14a0>
8160d0ec:	e59ab000 	ldr	fp, [sl]
8160d0f0:	e59a3004 	ldr	r3, [sl, #4]
8160d0f4:	e35b0000 	cmp	fp, #0
8160d0f8:	e58d3020 	str	r3, [sp, #32]
8160d0fc:	1a000001 	bne	8160d108 <inflate+0x38>
8160d100:	e3530000 	cmp	r3, #0
8160d104:	1a000519 	bne	8160e570 <inflate+0x14a0>
8160d108:	e5943000 	ldr	r3, [r4]
8160d10c:	e5945038 	ldr	r5, [r4, #56]	; 0x38
8160d110:	e353000b 	cmp	r3, #11
8160d114:	e594603c 	ldr	r6, [r4, #60]	; 0x3c
8160d118:	e59d7020 	ldr	r7, [sp, #32]
8160d11c:	03a0300c 	moveq	r3, #12
8160d120:	05843000 	streq	r3, [r4]
8160d124:	e59a300c 	ldr	r3, [sl, #12]
8160d128:	e58d1024 	str	r1, [sp, #36]	; 0x24
8160d12c:	e58d3014 	str	r3, [sp, #20]
8160d130:	e59a3010 	ldr	r3, [sl, #16]
8160d134:	e58d300c 	str	r3, [sp, #12]
8160d138:	e58d3018 	str	r3, [sp, #24]
8160d13c:	e3a03000 	mov	r3, #0
8160d140:	e58d3010 	str	r3, [sp, #16]
8160d144:	e5943000 	ldr	r3, [r4]
8160d148:	e353001c 	cmp	r3, #28
8160d14c:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
8160d150:	ea000506 	b	8160e570 <inflate+0x14a0>
8160d154:	8160d1d4 	.word	0x8160d1d4
8160d158:	8160d328 	.word	0x8160d328
8160d15c:	8160d394 	.word	0x8160d394
8160d160:	8160d3f4 	.word	0x8160d3f4
8160d164:	8160d450 	.word	0x8160d450
8160d168:	8160d514 	.word	0x8160d514
8160d16c:	8160d5c8 	.word	0x8160d5c8
8160d170:	8160d71c 	.word	0x8160d71c
8160d174:	8160d7c4 	.word	0x8160d7c4
8160d178:	8160d86c 	.word	0x8160d86c
8160d17c:	8160d8ac 	.word	0x8160d8ac
8160d180:	8160d900 	.word	0x8160d900
8160d184:	8160d90c 	.word	0x8160d90c
8160d188:	8160d9b8 	.word	0x8160d9b8
8160d18c:	8160d9fc 	.word	0x8160d9fc
8160d190:	8160da9c 	.word	0x8160da9c
8160d194:	8160dafc 	.word	0x8160dafc
8160d198:	8160dbc0 	.word	0x8160dbc0
8160d19c:	8160dd34 	.word	0x8160dd34
8160d1a0:	8160df8c 	.word	0x8160df8c
8160d1a4:	8160dfa0 	.word	0x8160dfa0
8160d1a8:	8160e0bc 	.word	0x8160e0bc
8160d1ac:	8160e118 	.word	0x8160e118
8160d1b0:	8160e244 	.word	0x8160e244
8160d1b4:	8160e27c 	.word	0x8160e27c
8160d1b8:	8160e290 	.word	0x8160e290
8160d1bc:	8160e2b0 	.word	0x8160e2b0
8160d1c0:	8160d1c8 	.word	0x8160d1c8
8160d1c4:	8160d6f8 	.word	0x8160d6f8
8160d1c8:	e3e03002 	mvn	r3, #2
8160d1cc:	e58d3010 	str	r3, [sp, #16]
8160d1d0:	ea000124 	b	8160d668 <inflate+0x598>
8160d1d4:	e5942008 	ldr	r2, [r4, #8]
8160d1d8:	e3520000 	cmp	r2, #0
8160d1dc:	03a0300c 	moveq	r3, #12
8160d1e0:	0a00002f 	beq	8160d2a4 <inflate+0x1d4>
8160d1e4:	e356000f 	cmp	r6, #15
8160d1e8:	9a000015 	bls	8160d244 <inflate+0x174>
8160d1ec:	e3083b1f 	movw	r3, #35615	; 0x8b1f
8160d1f0:	e0453003 	sub	r3, r5, r3
8160d1f4:	e16f3f13 	clz	r3, r3
8160d1f8:	e1a032a3 	lsr	r3, r3, #5
8160d1fc:	e01330a2 	ands	r3, r3, r2, lsr #1
8160d200:	0a000016 	beq	8160d260 <inflate+0x190>
8160d204:	e3a02000 	mov	r2, #0
8160d208:	e3a06000 	mov	r6, #0
8160d20c:	e1a01002 	mov	r1, r2
8160d210:	e1a00002 	mov	r0, r2
8160d214:	eb00111a 	bl	81611684 <crc32>
8160d218:	e59f3fd0 	ldr	r3, [pc, #4048]	; 8160e1f0 <inflate+0x1120>
8160d21c:	e28d1030 	add	r1, sp, #48	; 0x30
8160d220:	e3a02002 	mov	r2, #2
8160d224:	e1a05006 	mov	r5, r6
8160d228:	e16130b4 	strh	r3, [r1, #-4]!
8160d22c:	e5840018 	str	r0, [r4, #24]
8160d230:	eb001113 	bl	81611684 <crc32>
8160d234:	e3a03001 	mov	r3, #1
8160d238:	e5843000 	str	r3, [r4]
8160d23c:	e5840018 	str	r0, [r4, #24]
8160d240:	eaffffbf 	b	8160d144 <inflate+0x74>
8160d244:	e3570000 	cmp	r7, #0
8160d248:	0a000106 	beq	8160d668 <inflate+0x598>
8160d24c:	e4db3001 	ldrb	r3, [fp], #1
8160d250:	e2477001 	sub	r7, r7, #1
8160d254:	e0855613 	add	r5, r5, r3, lsl r6
8160d258:	e2866008 	add	r6, r6, #8
8160d25c:	eaffffe0 	b	8160d1e4 <inflate+0x114>
8160d260:	e5843010 	str	r3, [r4, #16]
8160d264:	e5943020 	ldr	r3, [r4, #32]
8160d268:	e3530000 	cmp	r3, #0
8160d26c:	13e01000 	mvnne	r1, #0
8160d270:	15831030 	strne	r1, [r3, #48]	; 0x30
8160d274:	e3120001 	tst	r2, #1
8160d278:	0a000006 	beq	8160d298 <inflate+0x1c8>
8160d27c:	e1a00405 	lsl	r0, r5, #8
8160d280:	e3a0101f 	mov	r1, #31
8160d284:	e6ff0070 	uxth	r0, r0
8160d288:	e0800425 	add	r0, r0, r5, lsr #8
8160d28c:	eb00167d 	bl	81612c88 <__aeabi_uidivmod>
8160d290:	e3510000 	cmp	r1, #0
8160d294:	0a000004 	beq	8160d2ac <inflate+0x1dc>
8160d298:	e59f3f54 	ldr	r3, [pc, #3924]	; 8160e1f4 <inflate+0x1124>
8160d29c:	e58a3018 	str	r3, [sl, #24]
8160d2a0:	e3a0301b 	mov	r3, #27
8160d2a4:	e5843000 	str	r3, [r4]
8160d2a8:	eaffffa5 	b	8160d144 <inflate+0x74>
8160d2ac:	e205300f 	and	r3, r5, #15
8160d2b0:	e3530008 	cmp	r3, #8
8160d2b4:	0a000001 	beq	8160d2c0 <inflate+0x1f0>
8160d2b8:	e59f3f38 	ldr	r3, [pc, #3896]	; 8160e1f8 <inflate+0x1128>
8160d2bc:	eafffff6 	b	8160d29c <inflate+0x1cc>
8160d2c0:	e1a05225 	lsr	r5, r5, #4
8160d2c4:	e5942024 	ldr	r2, [r4, #36]	; 0x24
8160d2c8:	e205300f 	and	r3, r5, #15
8160d2cc:	e2833008 	add	r3, r3, #8
8160d2d0:	e1520003 	cmp	r2, r3
8160d2d4:	32466004 	subcc	r6, r6, #4
8160d2d8:	359f3f1c 	ldrcc	r3, [pc, #3868]	; 8160e1fc <inflate+0x112c>
8160d2dc:	3affffee 	bcc	8160d29c <inflate+0x1cc>
8160d2e0:	e3150c02 	tst	r5, #512	; 0x200
8160d2e4:	e3a02001 	mov	r2, #1
8160d2e8:	e1a03312 	lsl	r3, r2, r3
8160d2ec:	e5842018 	str	r2, [r4, #24]
8160d2f0:	e5843014 	str	r3, [r4, #20]
8160d2f4:	e1a05001 	mov	r5, r1
8160d2f8:	e58a2034 	str	r2, [sl, #52]	; 0x34
8160d2fc:	03a0300b 	moveq	r3, #11
8160d300:	13a03009 	movne	r3, #9
8160d304:	e1a06001 	mov	r6, r1
8160d308:	e5843000 	str	r3, [r4]
8160d30c:	eaffff8c 	b	8160d144 <inflate+0x74>
8160d310:	e3570000 	cmp	r7, #0
8160d314:	0a0000d3 	beq	8160d668 <inflate+0x598>
8160d318:	e4db3001 	ldrb	r3, [fp], #1
8160d31c:	e2477001 	sub	r7, r7, #1
8160d320:	e0855613 	add	r5, r5, r3, lsl r6
8160d324:	e2866008 	add	r6, r6, #8
8160d328:	e356000f 	cmp	r6, #15
8160d32c:	9afffff7 	bls	8160d310 <inflate+0x240>
8160d330:	e6ef3075 	uxtb	r3, r5
8160d334:	e5845010 	str	r5, [r4, #16]
8160d338:	e3530008 	cmp	r3, #8
8160d33c:	1affffdd 	bne	8160d2b8 <inflate+0x1e8>
8160d340:	e3150a0e 	tst	r5, #57344	; 0xe000
8160d344:	159f3eb4 	ldrne	r3, [pc, #3764]	; 8160e200 <inflate+0x1130>
8160d348:	1affffd3 	bne	8160d29c <inflate+0x1cc>
8160d34c:	e5943020 	ldr	r3, [r4, #32]
8160d350:	e3530000 	cmp	r3, #0
8160d354:	17e02455 	ubfxne	r2, r5, #8, #1
8160d358:	15832000 	strne	r2, [r3]
8160d35c:	e3150c02 	tst	r5, #512	; 0x200
8160d360:	0a000007 	beq	8160d384 <inflate+0x2b4>
8160d364:	e3a02002 	mov	r2, #2
8160d368:	e28d102c 	add	r1, sp, #44	; 0x2c
8160d36c:	e5940018 	ldr	r0, [r4, #24]
8160d370:	e5cd502c 	strb	r5, [sp, #44]	; 0x2c
8160d374:	e1a05425 	lsr	r5, r5, #8
8160d378:	e5cd502d 	strb	r5, [sp, #45]	; 0x2d
8160d37c:	eb0010c0 	bl	81611684 <crc32>
8160d380:	e5840018 	str	r0, [r4, #24]
8160d384:	e3a06000 	mov	r6, #0
8160d388:	e3a03002 	mov	r3, #2
8160d38c:	e1a05006 	mov	r5, r6
8160d390:	e5843000 	str	r3, [r4]
8160d394:	e356001f 	cmp	r6, #31
8160d398:	9a000043 	bls	8160d4ac <inflate+0x3dc>
8160d39c:	e5943020 	ldr	r3, [r4, #32]
8160d3a0:	e3530000 	cmp	r3, #0
8160d3a4:	15835004 	strne	r5, [r3, #4]
8160d3a8:	e5943010 	ldr	r3, [r4, #16]
8160d3ac:	e3130c02 	tst	r3, #512	; 0x200
8160d3b0:	0a00000b 	beq	8160d3e4 <inflate+0x314>
8160d3b4:	e3a02004 	mov	r2, #4
8160d3b8:	e28d102c 	add	r1, sp, #44	; 0x2c
8160d3bc:	e5940018 	ldr	r0, [r4, #24]
8160d3c0:	e1a03425 	lsr	r3, r5, #8
8160d3c4:	e5cd502c 	strb	r5, [sp, #44]	; 0x2c
8160d3c8:	e5cd302d 	strb	r3, [sp, #45]	; 0x2d
8160d3cc:	e1a03825 	lsr	r3, r5, #16
8160d3d0:	e1a05c25 	lsr	r5, r5, #24
8160d3d4:	e5cd302e 	strb	r3, [sp, #46]	; 0x2e
8160d3d8:	e5cd502f 	strb	r5, [sp, #47]	; 0x2f
8160d3dc:	eb0010a8 	bl	81611684 <crc32>
8160d3e0:	e5840018 	str	r0, [r4, #24]
8160d3e4:	e3a06000 	mov	r6, #0
8160d3e8:	e3a03003 	mov	r3, #3
8160d3ec:	e1a05006 	mov	r5, r6
8160d3f0:	e5843000 	str	r3, [r4]
8160d3f4:	e356000f 	cmp	r6, #15
8160d3f8:	9a000032 	bls	8160d4c8 <inflate+0x3f8>
8160d3fc:	e5943020 	ldr	r3, [r4, #32]
8160d400:	e3530000 	cmp	r3, #0
8160d404:	16ef2075 	uxtbne	r2, r5
8160d408:	15832008 	strne	r2, [r3, #8]
8160d40c:	11a02425 	lsrne	r2, r5, #8
8160d410:	1583200c 	strne	r2, [r3, #12]
8160d414:	e5943010 	ldr	r3, [r4, #16]
8160d418:	e3130c02 	tst	r3, #512	; 0x200
8160d41c:	0a000007 	beq	8160d440 <inflate+0x370>
8160d420:	e3a02002 	mov	r2, #2
8160d424:	e28d102c 	add	r1, sp, #44	; 0x2c
8160d428:	e5940018 	ldr	r0, [r4, #24]
8160d42c:	e5cd502c 	strb	r5, [sp, #44]	; 0x2c
8160d430:	e1a05425 	lsr	r5, r5, #8
8160d434:	e5cd502d 	strb	r5, [sp, #45]	; 0x2d
8160d438:	eb001091 	bl	81611684 <crc32>
8160d43c:	e5840018 	str	r0, [r4, #24]
8160d440:	e3a06000 	mov	r6, #0
8160d444:	e3a03004 	mov	r3, #4
8160d448:	e1a05006 	mov	r5, r6
8160d44c:	e5843000 	str	r3, [r4]
8160d450:	e5943010 	ldr	r3, [r4, #16]
8160d454:	e2132b01 	ands	r2, r3, #1024	; 0x400
8160d458:	0a000028 	beq	8160d500 <inflate+0x430>
8160d45c:	e356000f 	cmp	r6, #15
8160d460:	9a00001f 	bls	8160d4e4 <inflate+0x414>
8160d464:	e5942020 	ldr	r2, [r4, #32]
8160d468:	e5845040 	str	r5, [r4, #64]	; 0x40
8160d46c:	e3520000 	cmp	r2, #0
8160d470:	15825014 	strne	r5, [r2, #20]
8160d474:	e2133c02 	ands	r3, r3, #512	; 0x200
8160d478:	01a05003 	moveq	r5, r3
8160d47c:	0a000008 	beq	8160d4a4 <inflate+0x3d4>
8160d480:	e3a02002 	mov	r2, #2
8160d484:	e28d102c 	add	r1, sp, #44	; 0x2c
8160d488:	e5940018 	ldr	r0, [r4, #24]
8160d48c:	e5cd502c 	strb	r5, [sp, #44]	; 0x2c
8160d490:	e1a05425 	lsr	r5, r5, #8
8160d494:	e5cd502d 	strb	r5, [sp, #45]	; 0x2d
8160d498:	eb001079 	bl	81611684 <crc32>
8160d49c:	e3a05000 	mov	r5, #0
8160d4a0:	e5840018 	str	r0, [r4, #24]
8160d4a4:	e1a06005 	mov	r6, r5
8160d4a8:	ea000017 	b	8160d50c <inflate+0x43c>
8160d4ac:	e3570000 	cmp	r7, #0
8160d4b0:	0a00006c 	beq	8160d668 <inflate+0x598>
8160d4b4:	e4db3001 	ldrb	r3, [fp], #1
8160d4b8:	e2477001 	sub	r7, r7, #1
8160d4bc:	e0855613 	add	r5, r5, r3, lsl r6
8160d4c0:	e2866008 	add	r6, r6, #8
8160d4c4:	eaffffb2 	b	8160d394 <inflate+0x2c4>
8160d4c8:	e3570000 	cmp	r7, #0
8160d4cc:	0a000065 	beq	8160d668 <inflate+0x598>
8160d4d0:	e4db3001 	ldrb	r3, [fp], #1
8160d4d4:	e2477001 	sub	r7, r7, #1
8160d4d8:	e0855613 	add	r5, r5, r3, lsl r6
8160d4dc:	e2866008 	add	r6, r6, #8
8160d4e0:	eaffffc3 	b	8160d3f4 <inflate+0x324>
8160d4e4:	e3570000 	cmp	r7, #0
8160d4e8:	0a00005e 	beq	8160d668 <inflate+0x598>
8160d4ec:	e4db2001 	ldrb	r2, [fp], #1
8160d4f0:	e2477001 	sub	r7, r7, #1
8160d4f4:	e0855612 	add	r5, r5, r2, lsl r6
8160d4f8:	e2866008 	add	r6, r6, #8
8160d4fc:	eaffffd6 	b	8160d45c <inflate+0x38c>
8160d500:	e5943020 	ldr	r3, [r4, #32]
8160d504:	e3530000 	cmp	r3, #0
8160d508:	15832010 	strne	r2, [r3, #16]
8160d50c:	e3a03005 	mov	r3, #5
8160d510:	e5843000 	str	r3, [r4]
8160d514:	e5943010 	ldr	r3, [r4, #16]
8160d518:	e3130b01 	tst	r3, #1024	; 0x400
8160d51c:	0a000025 	beq	8160d5b8 <inflate+0x4e8>
8160d520:	e5942040 	ldr	r2, [r4, #64]	; 0x40
8160d524:	e1570002 	cmp	r7, r2
8160d528:	31a08007 	movcc	r8, r7
8160d52c:	21a08002 	movcs	r8, r2
8160d530:	e3580000 	cmp	r8, #0
8160d534:	0a00001c 	beq	8160d5ac <inflate+0x4dc>
8160d538:	e5941020 	ldr	r1, [r4, #32]
8160d53c:	e3510000 	cmp	r1, #0
8160d540:	0a00000c 	beq	8160d578 <inflate+0x4a8>
8160d544:	e5910010 	ldr	r0, [r1, #16]
8160d548:	e3500000 	cmp	r0, #0
8160d54c:	0a000009 	beq	8160d578 <inflate+0x4a8>
8160d550:	e5913014 	ldr	r3, [r1, #20]
8160d554:	e0433002 	sub	r3, r3, r2
8160d558:	e5912018 	ldr	r2, [r1, #24]
8160d55c:	e0881003 	add	r1, r8, r3
8160d560:	e0800003 	add	r0, r0, r3
8160d564:	e1510002 	cmp	r1, r2
8160d568:	e1a0100b 	mov	r1, fp
8160d56c:	80422003 	subhi	r2, r2, r3
8160d570:	91a02008 	movls	r2, r8
8160d574:	ebffcc88 	bl	8160079c <memcpy>
8160d578:	e5943010 	ldr	r3, [r4, #16]
8160d57c:	e3130c02 	tst	r3, #512	; 0x200
8160d580:	0a000004 	beq	8160d598 <inflate+0x4c8>
8160d584:	e1a02008 	mov	r2, r8
8160d588:	e1a0100b 	mov	r1, fp
8160d58c:	e5940018 	ldr	r0, [r4, #24]
8160d590:	eb00103b 	bl	81611684 <crc32>
8160d594:	e5840018 	str	r0, [r4, #24]
8160d598:	e5943040 	ldr	r3, [r4, #64]	; 0x40
8160d59c:	e0477008 	sub	r7, r7, r8
8160d5a0:	e08bb008 	add	fp, fp, r8
8160d5a4:	e0438008 	sub	r8, r3, r8
8160d5a8:	e5848040 	str	r8, [r4, #64]	; 0x40
8160d5ac:	e5943040 	ldr	r3, [r4, #64]	; 0x40
8160d5b0:	e3530000 	cmp	r3, #0
8160d5b4:	1a00002b 	bne	8160d668 <inflate+0x598>
8160d5b8:	e3a03000 	mov	r3, #0
8160d5bc:	e5843040 	str	r3, [r4, #64]	; 0x40
8160d5c0:	e3a03006 	mov	r3, #6
8160d5c4:	e5843000 	str	r3, [r4]
8160d5c8:	e5943010 	ldr	r3, [r4, #16]
8160d5cc:	e2133b02 	ands	r3, r3, #2048	; 0x800
8160d5d0:	0a00004a 	beq	8160d700 <inflate+0x630>
8160d5d4:	e3570000 	cmp	r7, #0
8160d5d8:	0a000022 	beq	8160d668 <inflate+0x598>
8160d5dc:	e24b1001 	sub	r1, fp, #1
8160d5e0:	e3a08000 	mov	r8, #0
8160d5e4:	e5942020 	ldr	r2, [r4, #32]
8160d5e8:	e2888001 	add	r8, r8, #1
8160d5ec:	e5f13001 	ldrb	r3, [r1, #1]!
8160d5f0:	e3520000 	cmp	r2, #0
8160d5f4:	e58d301c 	str	r3, [sp, #28]
8160d5f8:	0a000009 	beq	8160d624 <inflate+0x554>
8160d5fc:	e592001c 	ldr	r0, [r2, #28]
8160d600:	e3500000 	cmp	r0, #0
8160d604:	0a000006 	beq	8160d624 <inflate+0x554>
8160d608:	e5922020 	ldr	r2, [r2, #32]
8160d60c:	e5943040 	ldr	r3, [r4, #64]	; 0x40
8160d610:	e1530002 	cmp	r3, r2
8160d614:	32832001 	addcc	r2, r3, #1
8160d618:	35842040 	strcc	r2, [r4, #64]	; 0x40
8160d61c:	359d201c 	ldrcc	r2, [sp, #28]
8160d620:	37c02003 	strbcc	r2, [r0, r3]
8160d624:	e59d301c 	ldr	r3, [sp, #28]
8160d628:	e3530000 	cmp	r3, #0
8160d62c:	11570008 	cmpne	r7, r8
8160d630:	8affffeb 	bhi	8160d5e4 <inflate+0x514>
8160d634:	e5943010 	ldr	r3, [r4, #16]
8160d638:	e3130c02 	tst	r3, #512	; 0x200
8160d63c:	0a000004 	beq	8160d654 <inflate+0x584>
8160d640:	e1a02008 	mov	r2, r8
8160d644:	e1a0100b 	mov	r1, fp
8160d648:	e5940018 	ldr	r0, [r4, #24]
8160d64c:	eb00100c 	bl	81611684 <crc32>
8160d650:	e5840018 	str	r0, [r4, #24]
8160d654:	e59d301c 	ldr	r3, [sp, #28]
8160d658:	e0477008 	sub	r7, r7, r8
8160d65c:	e08bb008 	add	fp, fp, r8
8160d660:	e3530000 	cmp	r3, #0
8160d664:	0a000028 	beq	8160d70c <inflate+0x63c>
8160d668:	e59d3014 	ldr	r3, [sp, #20]
8160d66c:	e58ab000 	str	fp, [sl]
8160d670:	e58a7004 	str	r7, [sl, #4]
8160d674:	e58a300c 	str	r3, [sl, #12]
8160d678:	e59d300c 	ldr	r3, [sp, #12]
8160d67c:	e58a3010 	str	r3, [sl, #16]
8160d680:	e5943028 	ldr	r3, [r4, #40]	; 0x28
8160d684:	e5845038 	str	r5, [r4, #56]	; 0x38
8160d688:	e3530000 	cmp	r3, #0
8160d68c:	e584603c 	str	r6, [r4, #60]	; 0x3c
8160d690:	1a000009 	bne	8160d6bc <inflate+0x5ec>
8160d694:	e5940000 	ldr	r0, [r4]
8160d698:	e59d200c 	ldr	r2, [sp, #12]
8160d69c:	e3500017 	cmp	r0, #23
8160d6a0:	e59d1018 	ldr	r1, [sp, #24]
8160d6a4:	83a03000 	movhi	r3, #0
8160d6a8:	93a03001 	movls	r3, #1
8160d6ac:	e1520001 	cmp	r2, r1
8160d6b0:	03a03000 	moveq	r3, #0
8160d6b4:	e3530000 	cmp	r3, #0
8160d6b8:	0a00035b 	beq	8160e42c <inflate+0x135c>
8160d6bc:	e59a501c 	ldr	r5, [sl, #28]
8160d6c0:	e5953034 	ldr	r3, [r5, #52]	; 0x34
8160d6c4:	e3530000 	cmp	r3, #0
8160d6c8:	1a000341 	bne	8160e3d4 <inflate+0x1304>
8160d6cc:	e5951024 	ldr	r1, [r5, #36]	; 0x24
8160d6d0:	e3a02001 	mov	r2, #1
8160d6d4:	e59a3020 	ldr	r3, [sl, #32]
8160d6d8:	e1a01112 	lsl	r1, r2, r1
8160d6dc:	e59a0028 	ldr	r0, [sl, #40]	; 0x28
8160d6e0:	e12fff33 	blx	r3
8160d6e4:	e3500000 	cmp	r0, #0
8160d6e8:	e5850034 	str	r0, [r5, #52]	; 0x34
8160d6ec:	1a000338 	bne	8160e3d4 <inflate+0x1304>
8160d6f0:	e3a0301c 	mov	r3, #28
8160d6f4:	e5843000 	str	r3, [r4]
8160d6f8:	e3e03003 	mvn	r3, #3
8160d6fc:	ea000076 	b	8160d8dc <inflate+0x80c>
8160d700:	e5942020 	ldr	r2, [r4, #32]
8160d704:	e3520000 	cmp	r2, #0
8160d708:	1582301c 	strne	r3, [r2, #28]
8160d70c:	e3a03000 	mov	r3, #0
8160d710:	e5843040 	str	r3, [r4, #64]	; 0x40
8160d714:	e3a03007 	mov	r3, #7
8160d718:	e5843000 	str	r3, [r4]
8160d71c:	e5943010 	ldr	r3, [r4, #16]
8160d720:	e2133a01 	ands	r3, r3, #4096	; 0x1000
8160d724:	0a00003f 	beq	8160d828 <inflate+0x758>
8160d728:	e3570000 	cmp	r7, #0
8160d72c:	0affffcd 	beq	8160d668 <inflate+0x598>
8160d730:	e24b1001 	sub	r1, fp, #1
8160d734:	e3a08000 	mov	r8, #0
8160d738:	e5942020 	ldr	r2, [r4, #32]
8160d73c:	e2888001 	add	r8, r8, #1
8160d740:	e5f13001 	ldrb	r3, [r1, #1]!
8160d744:	e3520000 	cmp	r2, #0
8160d748:	e58d301c 	str	r3, [sp, #28]
8160d74c:	0a000009 	beq	8160d778 <inflate+0x6a8>
8160d750:	e5920024 	ldr	r0, [r2, #36]	; 0x24
8160d754:	e3500000 	cmp	r0, #0
8160d758:	0a000006 	beq	8160d778 <inflate+0x6a8>
8160d75c:	e5922028 	ldr	r2, [r2, #40]	; 0x28
8160d760:	e5943040 	ldr	r3, [r4, #64]	; 0x40
8160d764:	e1530002 	cmp	r3, r2
8160d768:	32832001 	addcc	r2, r3, #1
8160d76c:	35842040 	strcc	r2, [r4, #64]	; 0x40
8160d770:	359d201c 	ldrcc	r2, [sp, #28]
8160d774:	37c02003 	strbcc	r2, [r0, r3]
8160d778:	e59d301c 	ldr	r3, [sp, #28]
8160d77c:	e3530000 	cmp	r3, #0
8160d780:	11570008 	cmpne	r7, r8
8160d784:	8affffeb 	bhi	8160d738 <inflate+0x668>
8160d788:	e5943010 	ldr	r3, [r4, #16]
8160d78c:	e3130c02 	tst	r3, #512	; 0x200
8160d790:	0a000004 	beq	8160d7a8 <inflate+0x6d8>
8160d794:	e1a02008 	mov	r2, r8
8160d798:	e1a0100b 	mov	r1, fp
8160d79c:	e5940018 	ldr	r0, [r4, #24]
8160d7a0:	eb000fb7 	bl	81611684 <crc32>
8160d7a4:	e5840018 	str	r0, [r4, #24]
8160d7a8:	e59d301c 	ldr	r3, [sp, #28]
8160d7ac:	e0477008 	sub	r7, r7, r8
8160d7b0:	e08bb008 	add	fp, fp, r8
8160d7b4:	e3530000 	cmp	r3, #0
8160d7b8:	1affffaa 	bne	8160d668 <inflate+0x598>
8160d7bc:	e3a03008 	mov	r3, #8
8160d7c0:	e5843000 	str	r3, [r4]
8160d7c4:	e5943010 	ldr	r3, [r4, #16]
8160d7c8:	e3130c02 	tst	r3, #512	; 0x200
8160d7cc:	0a000007 	beq	8160d7f0 <inflate+0x720>
8160d7d0:	e356000f 	cmp	r6, #15
8160d7d4:	9a000017 	bls	8160d838 <inflate+0x768>
8160d7d8:	e1d421b8 	ldrh	r2, [r4, #24]
8160d7dc:	e1520005 	cmp	r2, r5
8160d7e0:	159f3a1c 	ldrne	r3, [pc, #2588]	; 8160e204 <inflate+0x1134>
8160d7e4:	1afffeac 	bne	8160d29c <inflate+0x1cc>
8160d7e8:	e3a06000 	mov	r6, #0
8160d7ec:	e1a05006 	mov	r5, r6
8160d7f0:	e5942020 	ldr	r2, [r4, #32]
8160d7f4:	e3520000 	cmp	r2, #0
8160d7f8:	17e034d3 	ubfxne	r3, r3, #9, #1
8160d7fc:	1582302c 	strne	r3, [r2, #44]	; 0x2c
8160d800:	13a03001 	movne	r3, #1
8160d804:	15823030 	strne	r3, [r2, #48]	; 0x30
8160d808:	e3a02000 	mov	r2, #0
8160d80c:	e1a01002 	mov	r1, r2
8160d810:	e1a00002 	mov	r0, r2
8160d814:	eb000f9a 	bl	81611684 <crc32>
8160d818:	e5840018 	str	r0, [r4, #24]
8160d81c:	e58a0034 	str	r0, [sl, #52]	; 0x34
8160d820:	e3a0300b 	mov	r3, #11
8160d824:	eafffe9e 	b	8160d2a4 <inflate+0x1d4>
8160d828:	e5942020 	ldr	r2, [r4, #32]
8160d82c:	e3520000 	cmp	r2, #0
8160d830:	15823024 	strne	r3, [r2, #36]	; 0x24
8160d834:	eaffffe0 	b	8160d7bc <inflate+0x6ec>
8160d838:	e3570000 	cmp	r7, #0
8160d83c:	0affff89 	beq	8160d668 <inflate+0x598>
8160d840:	e4db2001 	ldrb	r2, [fp], #1
8160d844:	e2477001 	sub	r7, r7, #1
8160d848:	e0855612 	add	r5, r5, r2, lsl r6
8160d84c:	e2866008 	add	r6, r6, #8
8160d850:	eaffffde 	b	8160d7d0 <inflate+0x700>
8160d854:	e3570000 	cmp	r7, #0
8160d858:	0affff82 	beq	8160d668 <inflate+0x598>
8160d85c:	e4db3001 	ldrb	r3, [fp], #1
8160d860:	e2477001 	sub	r7, r7, #1
8160d864:	e0855613 	add	r5, r5, r3, lsl r6
8160d868:	e2866008 	add	r6, r6, #8
8160d86c:	e356001f 	cmp	r6, #31
8160d870:	9afffff7 	bls	8160d854 <inflate+0x784>
8160d874:	e1a03c05 	lsl	r3, r5, #24
8160d878:	e1a02425 	lsr	r2, r5, #8
8160d87c:	e0833c25 	add	r3, r3, r5, lsr #24
8160d880:	e2022cff 	and	r2, r2, #65280	; 0xff00
8160d884:	e1a05405 	lsl	r5, r5, #8
8160d888:	e0833002 	add	r3, r3, r2
8160d88c:	e20558ff 	and	r5, r5, #16711680	; 0xff0000
8160d890:	e0835005 	add	r5, r3, r5
8160d894:	e5845018 	str	r5, [r4, #24]
8160d898:	e58a5034 	str	r5, [sl, #52]	; 0x34
8160d89c:	e3a05000 	mov	r5, #0
8160d8a0:	e1a06005 	mov	r6, r5
8160d8a4:	e3a0300a 	mov	r3, #10
8160d8a8:	e5843000 	str	r3, [r4]
8160d8ac:	e594300c 	ldr	r3, [r4, #12]
8160d8b0:	e3530000 	cmp	r3, #0
8160d8b4:	1a00000c 	bne	8160d8ec <inflate+0x81c>
8160d8b8:	e59d3014 	ldr	r3, [sp, #20]
8160d8bc:	e58ab000 	str	fp, [sl]
8160d8c0:	e58a7004 	str	r7, [sl, #4]
8160d8c4:	e58a300c 	str	r3, [sl, #12]
8160d8c8:	e59d300c 	ldr	r3, [sp, #12]
8160d8cc:	e58a3010 	str	r3, [sl, #16]
8160d8d0:	e3a03002 	mov	r3, #2
8160d8d4:	e5845038 	str	r5, [r4, #56]	; 0x38
8160d8d8:	e584603c 	str	r6, [r4, #60]	; 0x3c
8160d8dc:	e58d3010 	str	r3, [sp, #16]
8160d8e0:	e59d0010 	ldr	r0, [sp, #16]
8160d8e4:	e28dd030 	add	sp, sp, #48	; 0x30
8160d8e8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160d8ec:	e3a03001 	mov	r3, #1
8160d8f0:	e5843018 	str	r3, [r4, #24]
8160d8f4:	e58a3034 	str	r3, [sl, #52]	; 0x34
8160d8f8:	e3a0300b 	mov	r3, #11
8160d8fc:	e5843000 	str	r3, [r4]
8160d900:	e59d3024 	ldr	r3, [sp, #36]	; 0x24
8160d904:	e3530005 	cmp	r3, #5
8160d908:	0affff56 	beq	8160d668 <inflate+0x598>
8160d90c:	e5943004 	ldr	r3, [r4, #4]
8160d910:	e3530000 	cmp	r3, #0
8160d914:	0a00000a 	beq	8160d944 <inflate+0x874>
8160d918:	e2063007 	and	r3, r6, #7
8160d91c:	e3c66007 	bic	r6, r6, #7
8160d920:	e1a05335 	lsr	r5, r5, r3
8160d924:	e3a03018 	mov	r3, #24
8160d928:	eafffe5d 	b	8160d2a4 <inflate+0x1d4>
8160d92c:	e3570000 	cmp	r7, #0
8160d930:	0affff4c 	beq	8160d668 <inflate+0x598>
8160d934:	e4db3001 	ldrb	r3, [fp], #1
8160d938:	e2477001 	sub	r7, r7, #1
8160d93c:	e0855613 	add	r5, r5, r3, lsl r6
8160d940:	e2866008 	add	r6, r6, #8
8160d944:	e3560002 	cmp	r6, #2
8160d948:	9afffff7 	bls	8160d92c <inflate+0x85c>
8160d94c:	e2053001 	and	r3, r5, #1
8160d950:	e5843004 	str	r3, [r4, #4]
8160d954:	e7e130d5 	ubfx	r3, r5, #1, #2
8160d958:	e3530002 	cmp	r3, #2
8160d95c:	03a0300f 	moveq	r3, #15
8160d960:	0a00000c 	beq	8160d998 <inflate+0x8c8>
8160d964:	e3530003 	cmp	r3, #3
8160d968:	0a00000e 	beq	8160d9a8 <inflate+0x8d8>
8160d96c:	e3530001 	cmp	r3, #1
8160d970:	059f3890 	ldreq	r3, [pc, #2192]	; 8160e208 <inflate+0x1138>
8160d974:	13a0300d 	movne	r3, #13
8160d978:	0584304c 	streq	r3, [r4, #76]	; 0x4c
8160d97c:	03a03009 	moveq	r3, #9
8160d980:	05843054 	streq	r3, [r4, #84]	; 0x54
8160d984:	059f3880 	ldreq	r3, [pc, #2176]	; 8160e20c <inflate+0x113c>
8160d988:	05843050 	streq	r3, [r4, #80]	; 0x50
8160d98c:	03a03005 	moveq	r3, #5
8160d990:	05843058 	streq	r3, [r4, #88]	; 0x58
8160d994:	03a03012 	moveq	r3, #18
8160d998:	e5843000 	str	r3, [r4]
8160d99c:	e1a051a5 	lsr	r5, r5, #3
8160d9a0:	e2466003 	sub	r6, r6, #3
8160d9a4:	eafffde6 	b	8160d144 <inflate+0x74>
8160d9a8:	e59f3860 	ldr	r3, [pc, #2144]	; 8160e210 <inflate+0x1140>
8160d9ac:	e58a3018 	str	r3, [sl, #24]
8160d9b0:	e3a0301b 	mov	r3, #27
8160d9b4:	eafffff7 	b	8160d998 <inflate+0x8c8>
8160d9b8:	e2063007 	and	r3, r6, #7
8160d9bc:	e3c66007 	bic	r6, r6, #7
8160d9c0:	e1a05335 	lsr	r5, r5, r3
8160d9c4:	e356001f 	cmp	r6, #31
8160d9c8:	9a000026 	bls	8160da68 <inflate+0x998>
8160d9cc:	e1a03825 	lsr	r3, r5, #16
8160d9d0:	e6ff2075 	uxth	r2, r5
8160d9d4:	e2233cff 	eor	r3, r3, #65280	; 0xff00
8160d9d8:	e22330ff 	eor	r3, r3, #255	; 0xff
8160d9dc:	e1520003 	cmp	r2, r3
8160d9e0:	159f382c 	ldrne	r3, [pc, #2092]	; 8160e214 <inflate+0x1144>
8160d9e4:	1afffe2c 	bne	8160d29c <inflate+0x1cc>
8160d9e8:	e3a06000 	mov	r6, #0
8160d9ec:	e3a0300e 	mov	r3, #14
8160d9f0:	e1a05006 	mov	r5, r6
8160d9f4:	e5842040 	str	r2, [r4, #64]	; 0x40
8160d9f8:	e5843000 	str	r3, [r4]
8160d9fc:	e5942040 	ldr	r2, [r4, #64]	; 0x40
8160da00:	e3520000 	cmp	r2, #0
8160da04:	0affff85 	beq	8160d820 <inflate+0x750>
8160da08:	e59d300c 	ldr	r3, [sp, #12]
8160da0c:	e1530002 	cmp	r3, r2
8160da10:	31a02003 	movcc	r2, r3
8160da14:	e1570002 	cmp	r7, r2
8160da18:	31a08007 	movcc	r8, r7
8160da1c:	21a08002 	movcs	r8, r2
8160da20:	e3580000 	cmp	r8, #0
8160da24:	0affff0f 	beq	8160d668 <inflate+0x598>
8160da28:	e1a02008 	mov	r2, r8
8160da2c:	e1a0100b 	mov	r1, fp
8160da30:	e59d0014 	ldr	r0, [sp, #20]
8160da34:	e0477008 	sub	r7, r7, r8
8160da38:	ebffcb57 	bl	8160079c <memcpy>
8160da3c:	e59d300c 	ldr	r3, [sp, #12]
8160da40:	e5942040 	ldr	r2, [r4, #64]	; 0x40
8160da44:	e08bb008 	add	fp, fp, r8
8160da48:	e0433008 	sub	r3, r3, r8
8160da4c:	e58d300c 	str	r3, [sp, #12]
8160da50:	e59d3014 	ldr	r3, [sp, #20]
8160da54:	e0422008 	sub	r2, r2, r8
8160da58:	e5842040 	str	r2, [r4, #64]	; 0x40
8160da5c:	e0833008 	add	r3, r3, r8
8160da60:	e58d3014 	str	r3, [sp, #20]
8160da64:	eafffdb6 	b	8160d144 <inflate+0x74>
8160da68:	e3570000 	cmp	r7, #0
8160da6c:	0afffefd 	beq	8160d668 <inflate+0x598>
8160da70:	e4db3001 	ldrb	r3, [fp], #1
8160da74:	e2477001 	sub	r7, r7, #1
8160da78:	e0855613 	add	r5, r5, r3, lsl r6
8160da7c:	e2866008 	add	r6, r6, #8
8160da80:	eaffffcf 	b	8160d9c4 <inflate+0x8f4>
8160da84:	e3570000 	cmp	r7, #0
8160da88:	0afffef6 	beq	8160d668 <inflate+0x598>
8160da8c:	e4db3001 	ldrb	r3, [fp], #1
8160da90:	e2477001 	sub	r7, r7, #1
8160da94:	e0855613 	add	r5, r5, r3, lsl r6
8160da98:	e2866008 	add	r6, r6, #8
8160da9c:	e356000d 	cmp	r6, #13
8160daa0:	9afffff7 	bls	8160da84 <inflate+0x9b4>
8160daa4:	e205301f 	and	r3, r5, #31
8160daa8:	e7e422d5 	ubfx	r2, r5, #5, #5
8160daac:	e2833c01 	add	r3, r3, #256	; 0x100
8160dab0:	e2822001 	add	r2, r2, #1
8160dab4:	e2833001 	add	r3, r3, #1
8160dab8:	e7e31555 	ubfx	r1, r5, #10, #4
8160dabc:	e2811004 	add	r1, r1, #4
8160dac0:	e584105c 	str	r1, [r4, #92]	; 0x5c
8160dac4:	e300111e 	movw	r1, #286	; 0x11e
8160dac8:	e352001e 	cmp	r2, #30
8160dacc:	91530001 	cmpls	r3, r1
8160dad0:	e5843060 	str	r3, [r4, #96]	; 0x60
8160dad4:	e1a05725 	lsr	r5, r5, #14
8160dad8:	93a03000 	movls	r3, #0
8160dadc:	83a03001 	movhi	r3, #1
8160dae0:	95843068 	strls	r3, [r4, #104]	; 0x68
8160dae4:	e246600e 	sub	r6, r6, #14
8160dae8:	93a03010 	movls	r3, #16
8160daec:	e5842064 	str	r2, [r4, #100]	; 0x64
8160daf0:	859f3720 	ldrhi	r3, [pc, #1824]	; 8160e218 <inflate+0x1148>
8160daf4:	95843000 	strls	r3, [r4]
8160daf8:	8afffde7 	bhi	8160d29c <inflate+0x1cc>
8160dafc:	e594205c 	ldr	r2, [r4, #92]	; 0x5c
8160db00:	ea000011 	b	8160db4c <inflate+0xa7c>
8160db04:	e3570000 	cmp	r7, #0
8160db08:	0afffed6 	beq	8160d668 <inflate+0x598>
8160db0c:	e4db1001 	ldrb	r1, [fp], #1
8160db10:	e2477001 	sub	r7, r7, #1
8160db14:	e0855611 	add	r5, r5, r1, lsl r6
8160db18:	e2866008 	add	r6, r6, #8
8160db1c:	e3560002 	cmp	r6, #2
8160db20:	9afffff7 	bls	8160db04 <inflate+0xa34>
8160db24:	e2831001 	add	r1, r3, #1
8160db28:	e5841068 	str	r1, [r4, #104]	; 0x68
8160db2c:	e59f16e8 	ldr	r1, [pc, #1768]	; 8160e21c <inflate+0x114c>
8160db30:	e1a03083 	lsl	r3, r3, #1
8160db34:	e2466003 	sub	r6, r6, #3
8160db38:	e19130b3 	ldrh	r3, [r1, r3]
8160db3c:	e2051007 	and	r1, r5, #7
8160db40:	e1a051a5 	lsr	r5, r5, #3
8160db44:	e0843083 	add	r3, r4, r3, lsl #1
8160db48:	e1c317b0 	strh	r1, [r3, #112]	; 0x70
8160db4c:	e5943068 	ldr	r3, [r4, #104]	; 0x68
8160db50:	e1530002 	cmp	r3, r2
8160db54:	3afffff0 	bcc	8160db1c <inflate+0xa4c>
8160db58:	e3a02000 	mov	r2, #0
8160db5c:	e5943068 	ldr	r3, [r4, #104]	; 0x68
8160db60:	e3530012 	cmp	r3, #18
8160db64:	9a00001a 	bls	8160dbd4 <inflate+0xb04>
8160db68:	e2843e53 	add	r3, r4, #1328	; 0x530
8160db6c:	e3a02013 	mov	r2, #19
8160db70:	e584306c 	str	r3, [r4, #108]	; 0x6c
8160db74:	e2841070 	add	r1, r4, #112	; 0x70
8160db78:	e584304c 	str	r3, [r4, #76]	; 0x4c
8160db7c:	e3a00000 	mov	r0, #0
8160db80:	e3a03007 	mov	r3, #7
8160db84:	e5843054 	str	r3, [r4, #84]	; 0x54
8160db88:	e2843e2f 	add	r3, r4, #752	; 0x2f0
8160db8c:	e58d3004 	str	r3, [sp, #4]
8160db90:	e2843054 	add	r3, r4, #84	; 0x54
8160db94:	e58d3000 	str	r3, [sp]
8160db98:	e284306c 	add	r3, r4, #108	; 0x6c
8160db9c:	ebfffabd 	bl	8160c698 <inflate_table>
8160dba0:	e2503000 	subs	r3, r0, #0
8160dba4:	e58d3010 	str	r3, [sp, #16]
8160dba8:	159f3670 	ldrne	r3, [pc, #1648]	; 8160e220 <inflate+0x1150>
8160dbac:	1afffdba 	bne	8160d29c <inflate+0x1cc>
8160dbb0:	e59d3010 	ldr	r3, [sp, #16]
8160dbb4:	e5843068 	str	r3, [r4, #104]	; 0x68
8160dbb8:	e3a03011 	mov	r3, #17
8160dbbc:	e5843000 	str	r3, [r4]
8160dbc0:	e594e060 	ldr	lr, [r4, #96]	; 0x60
8160dbc4:	e3e08002 	mvn	r8, #2
8160dbc8:	e5941064 	ldr	r1, [r4, #100]	; 0x64
8160dbcc:	e08e1001 	add	r1, lr, r1
8160dbd0:	ea00007a 	b	8160ddc0 <inflate+0xcf0>
8160dbd4:	e2831001 	add	r1, r3, #1
8160dbd8:	e5841068 	str	r1, [r4, #104]	; 0x68
8160dbdc:	e59f1638 	ldr	r1, [pc, #1592]	; 8160e21c <inflate+0x114c>
8160dbe0:	e1a03083 	lsl	r3, r3, #1
8160dbe4:	e19130b3 	ldrh	r3, [r1, r3]
8160dbe8:	e2833038 	add	r3, r3, #56	; 0x38
8160dbec:	e1a03083 	lsl	r3, r3, #1
8160dbf0:	e18420b3 	strh	r2, [r4, r3]
8160dbf4:	eaffffd8 	b	8160db5c <inflate+0xa8c>
8160dbf8:	e1d000b2 	ldrh	r0, [r0, #2]
8160dbfc:	e350000f 	cmp	r0, #15
8160dc00:	9a00025c 	bls	8160e578 <inflate+0x14a8>
8160dc04:	e3500010 	cmp	r0, #16
8160dc08:	02830002 	addeq	r0, r3, #2
8160dc0c:	0a000013 	beq	8160dc60 <inflate+0xb90>
8160dc10:	e3500011 	cmp	r0, #17
8160dc14:	02830003 	addeq	r0, r3, #3
8160dc18:	12830007 	addne	r0, r3, #7
8160dc1c:	1a000089 	bne	8160de48 <inflate+0xd78>
8160dc20:	e1500006 	cmp	r0, r6
8160dc24:	8a00007a 	bhi	8160de14 <inflate+0xd44>
8160dc28:	e1a05335 	lsr	r5, r5, r3
8160dc2c:	e0483003 	sub	r3, r8, r3
8160dc30:	e2050007 	and	r0, r5, #7
8160dc34:	e2800003 	add	r0, r0, #3
8160dc38:	e1a051a5 	lsr	r5, r5, #3
8160dc3c:	e0866003 	add	r6, r6, r3
8160dc40:	e3a0c000 	mov	ip, #0
8160dc44:	ea000056 	b	8160dda4 <inflate+0xcd4>
8160dc48:	e3570000 	cmp	r7, #0
8160dc4c:	0afffe85 	beq	8160d668 <inflate+0x598>
8160dc50:	e4dbc001 	ldrb	ip, [fp], #1
8160dc54:	e2477001 	sub	r7, r7, #1
8160dc58:	e085561c 	add	r5, r5, ip, lsl r6
8160dc5c:	e2866008 	add	r6, r6, #8
8160dc60:	e1500006 	cmp	r0, r6
8160dc64:	8afffff7 	bhi	8160dc48 <inflate+0xb78>
8160dc68:	e3520000 	cmp	r2, #0
8160dc6c:	e1a05335 	lsr	r5, r5, r3
8160dc70:	e0466003 	sub	r6, r6, r3
8160dc74:	1a000044 	bne	8160dd8c <inflate+0xcbc>
8160dc78:	e59f35a4 	ldr	r3, [pc, #1444]	; 8160e224 <inflate+0x1154>
8160dc7c:	e58a3018 	str	r3, [sl, #24]
8160dc80:	e3a0301b 	mov	r3, #27
8160dc84:	e5843000 	str	r3, [r4]
8160dc88:	e5943000 	ldr	r3, [r4]
8160dc8c:	e353001b 	cmp	r3, #27
8160dc90:	0afffd2b 	beq	8160d144 <inflate+0x74>
8160dc94:	e2848070 	add	r8, r4, #112	; 0x70
8160dc98:	e2843e53 	add	r3, r4, #1328	; 0x530
8160dc9c:	e1a0200e 	mov	r2, lr
8160dca0:	e584306c 	str	r3, [r4, #108]	; 0x6c
8160dca4:	e584304c 	str	r3, [r4, #76]	; 0x4c
8160dca8:	e1a01008 	mov	r1, r8
8160dcac:	e3a03009 	mov	r3, #9
8160dcb0:	e3a00001 	mov	r0, #1
8160dcb4:	e5843054 	str	r3, [r4, #84]	; 0x54
8160dcb8:	e2843e2f 	add	r3, r4, #752	; 0x2f0
8160dcbc:	e58d3010 	str	r3, [sp, #16]
8160dcc0:	e58d3004 	str	r3, [sp, #4]
8160dcc4:	e2843054 	add	r3, r4, #84	; 0x54
8160dcc8:	e58d3000 	str	r3, [sp]
8160dccc:	e284306c 	add	r3, r4, #108	; 0x6c
8160dcd0:	ebfffa70 	bl	8160c698 <inflate_table>
8160dcd4:	e2503000 	subs	r3, r0, #0
8160dcd8:	e58d3010 	str	r3, [sp, #16]
8160dcdc:	159f3544 	ldrne	r3, [pc, #1348]	; 8160e228 <inflate+0x1158>
8160dce0:	1afffd6d 	bne	8160d29c <inflate+0x1cc>
8160dce4:	e594306c 	ldr	r3, [r4, #108]	; 0x6c
8160dce8:	e3a00002 	mov	r0, #2
8160dcec:	e5941060 	ldr	r1, [r4, #96]	; 0x60
8160dcf0:	e5843050 	str	r3, [r4, #80]	; 0x50
8160dcf4:	e3a03006 	mov	r3, #6
8160dcf8:	e0881081 	add	r1, r8, r1, lsl #1
8160dcfc:	e5843058 	str	r3, [r4, #88]	; 0x58
8160dd00:	e2843e2f 	add	r3, r4, #752	; 0x2f0
8160dd04:	e58d3004 	str	r3, [sp, #4]
8160dd08:	e2843058 	add	r3, r4, #88	; 0x58
8160dd0c:	e58d3000 	str	r3, [sp]
8160dd10:	e284306c 	add	r3, r4, #108	; 0x6c
8160dd14:	e5942064 	ldr	r2, [r4, #100]	; 0x64
8160dd18:	ebfffa5e 	bl	8160c698 <inflate_table>
8160dd1c:	e2503000 	subs	r3, r0, #0
8160dd20:	e58d3010 	str	r3, [sp, #16]
8160dd24:	159f3500 	ldrne	r3, [pc, #1280]	; 8160e22c <inflate+0x115c>
8160dd28:	1afffd5b 	bne	8160d29c <inflate+0x1cc>
8160dd2c:	e3a03012 	mov	r3, #18
8160dd30:	e5843000 	str	r3, [r4]
8160dd34:	e59d200c 	ldr	r2, [sp, #12]
8160dd38:	e3003101 	movw	r3, #257	; 0x101
8160dd3c:	e3570005 	cmp	r7, #5
8160dd40:	81520003 	cmphi	r2, r3
8160dd44:	8a00004f 	bhi	8160de88 <inflate+0xdb8>
8160dd48:	e5943054 	ldr	r3, [r4, #84]	; 0x54
8160dd4c:	e3e02000 	mvn	r2, #0
8160dd50:	e594c04c 	ldr	ip, [r4, #76]	; 0x4c
8160dd54:	e1e03312 	mvn	r3, r2, lsl r3
8160dd58:	e0030005 	and	r0, r3, r5
8160dd5c:	e08c0100 	add	r0, ip, r0, lsl #2
8160dd60:	e5d02001 	ldrb	r2, [r0, #1]
8160dd64:	e1520006 	cmp	r2, r6
8160dd68:	e1a01002 	mov	r1, r2
8160dd6c:	9a000059 	bls	8160ded8 <inflate+0xe08>
8160dd70:	e3570000 	cmp	r7, #0
8160dd74:	0afffe3b 	beq	8160d668 <inflate+0x598>
8160dd78:	e4db2001 	ldrb	r2, [fp], #1
8160dd7c:	e2477001 	sub	r7, r7, #1
8160dd80:	e0855612 	add	r5, r5, r2, lsl r6
8160dd84:	e2866008 	add	r6, r6, #8
8160dd88:	eafffff2 	b	8160dd58 <inflate+0xc88>
8160dd8c:	e0843082 	add	r3, r4, r2, lsl #1
8160dd90:	e2050003 	and	r0, r5, #3
8160dd94:	e2800003 	add	r0, r0, #3
8160dd98:	e1a05125 	lsr	r5, r5, #2
8160dd9c:	e1d3c6be 	ldrh	ip, [r3, #110]	; 0x6e
8160dda0:	e2466002 	sub	r6, r6, #2
8160dda4:	e0822000 	add	r2, r2, r0
8160dda8:	e1520001 	cmp	r2, r1
8160ddac:	8affffb1 	bhi	8160dc78 <inflate+0xba8>
8160ddb0:	e2400001 	sub	r0, r0, #1
8160ddb4:	e5943068 	ldr	r3, [r4, #104]	; 0x68
8160ddb8:	e3700001 	cmn	r0, #1
8160ddbc:	1a00002c 	bne	8160de74 <inflate+0xda4>
8160ddc0:	e5942068 	ldr	r2, [r4, #104]	; 0x68
8160ddc4:	e1520001 	cmp	r2, r1
8160ddc8:	2affffae 	bcs	8160dc88 <inflate+0xbb8>
8160ddcc:	e594304c 	ldr	r3, [r4, #76]	; 0x4c
8160ddd0:	e594c054 	ldr	ip, [r4, #84]	; 0x54
8160ddd4:	e58d301c 	str	r3, [sp, #28]
8160ddd8:	e3e03000 	mvn	r3, #0
8160dddc:	e1e0cc13 	mvn	ip, r3, lsl ip
8160dde0:	e59d301c 	ldr	r3, [sp, #28]
8160dde4:	e00c0005 	and	r0, ip, r5
8160dde8:	e0830100 	add	r0, r3, r0, lsl #2
8160ddec:	e5d03001 	ldrb	r3, [r0, #1]
8160ddf0:	e1530006 	cmp	r3, r6
8160ddf4:	9affff7f 	bls	8160dbf8 <inflate+0xb28>
8160ddf8:	e3570000 	cmp	r7, #0
8160ddfc:	0afffe19 	beq	8160d668 <inflate+0x598>
8160de00:	e4db3001 	ldrb	r3, [fp], #1
8160de04:	e2477001 	sub	r7, r7, #1
8160de08:	e0855613 	add	r5, r5, r3, lsl r6
8160de0c:	e2866008 	add	r6, r6, #8
8160de10:	eafffff2 	b	8160dde0 <inflate+0xd10>
8160de14:	e3570000 	cmp	r7, #0
8160de18:	0afffe12 	beq	8160d668 <inflate+0x598>
8160de1c:	e4dbc001 	ldrb	ip, [fp], #1
8160de20:	e2477001 	sub	r7, r7, #1
8160de24:	e085561c 	add	r5, r5, ip, lsl r6
8160de28:	e2866008 	add	r6, r6, #8
8160de2c:	eaffff7b 	b	8160dc20 <inflate+0xb50>
8160de30:	e3570000 	cmp	r7, #0
8160de34:	0afffe0b 	beq	8160d668 <inflate+0x598>
8160de38:	e4dbc001 	ldrb	ip, [fp], #1
8160de3c:	e2477001 	sub	r7, r7, #1
8160de40:	e085561c 	add	r5, r5, ip, lsl r6
8160de44:	e2866008 	add	r6, r6, #8
8160de48:	e1500006 	cmp	r0, r6
8160de4c:	8afffff7 	bhi	8160de30 <inflate+0xd60>
8160de50:	e1a05335 	lsr	r5, r5, r3
8160de54:	e26334ff 	rsb	r3, r3, #-16777216	; 0xff000000
8160de58:	e28338ff 	add	r3, r3, #16711680	; 0xff0000
8160de5c:	e205007f 	and	r0, r5, #127	; 0x7f
8160de60:	e2833cff 	add	r3, r3, #65280	; 0xff00
8160de64:	e280000b 	add	r0, r0, #11
8160de68:	e1a053a5 	lsr	r5, r5, #7
8160de6c:	e28330f9 	add	r3, r3, #249	; 0xf9
8160de70:	eaffff71 	b	8160dc3c <inflate+0xb6c>
8160de74:	e2832001 	add	r2, r3, #1
8160de78:	e0843083 	add	r3, r4, r3, lsl #1
8160de7c:	e5842068 	str	r2, [r4, #104]	; 0x68
8160de80:	e1c3c7b0 	strh	ip, [r3, #112]	; 0x70
8160de84:	eaffffc9 	b	8160ddb0 <inflate+0xce0>
8160de88:	e59d3014 	ldr	r3, [sp, #20]
8160de8c:	e1a0000a 	mov	r0, sl
8160de90:	e58ab000 	str	fp, [sl]
8160de94:	e58a7004 	str	r7, [sl, #4]
8160de98:	e58a300c 	str	r3, [sl, #12]
8160de9c:	e59d300c 	ldr	r3, [sp, #12]
8160dea0:	e59d1018 	ldr	r1, [sp, #24]
8160dea4:	e58a3010 	str	r3, [sl, #16]
8160dea8:	e5845038 	str	r5, [r4, #56]	; 0x38
8160deac:	e584603c 	str	r6, [r4, #60]	; 0x3c
8160deb0:	ebfff89b 	bl	8160c124 <inflate_fast>
8160deb4:	e59a300c 	ldr	r3, [sl, #12]
8160deb8:	e59ab000 	ldr	fp, [sl]
8160debc:	e59a7004 	ldr	r7, [sl, #4]
8160dec0:	e58d3014 	str	r3, [sp, #20]
8160dec4:	e59a3010 	ldr	r3, [sl, #16]
8160dec8:	e5945038 	ldr	r5, [r4, #56]	; 0x38
8160decc:	e594603c 	ldr	r6, [r4, #60]	; 0x3c
8160ded0:	e58d300c 	str	r3, [sp, #12]
8160ded4:	eafffc9a 	b	8160d144 <inflate+0x74>
8160ded8:	e5d03000 	ldrb	r3, [r0]
8160dedc:	e1d000b2 	ldrh	r0, [r0, #2]
8160dee0:	e3530000 	cmp	r3, #0
8160dee4:	0a000019 	beq	8160df50 <inflate+0xe80>
8160dee8:	e31300f0 	tst	r3, #240	; 0xf0
8160deec:	1a000017 	bne	8160df50 <inflate+0xe80>
8160def0:	e0833002 	add	r3, r3, r2
8160def4:	e3e0e000 	mvn	lr, #0
8160def8:	e1e0331e 	mvn	r3, lr, lsl r3
8160defc:	e58d301c 	str	r3, [sp, #28]
8160df00:	e59d301c 	ldr	r3, [sp, #28]
8160df04:	e0033005 	and	r3, r3, r5
8160df08:	e0803233 	add	r3, r0, r3, lsr r2
8160df0c:	e08c8103 	add	r8, ip, r3, lsl #2
8160df10:	e7dc3103 	ldrb	r3, [ip, r3, lsl #2]
8160df14:	e5d81001 	ldrb	r1, [r8, #1]
8160df18:	e1d880b2 	ldrh	r8, [r8, #2]
8160df1c:	e081e002 	add	lr, r1, r2
8160df20:	e15e0006 	cmp	lr, r6
8160df24:	9a000006 	bls	8160df44 <inflate+0xe74>
8160df28:	e3570000 	cmp	r7, #0
8160df2c:	0afffdcd 	beq	8160d668 <inflate+0x598>
8160df30:	e4db3001 	ldrb	r3, [fp], #1
8160df34:	e2477001 	sub	r7, r7, #1
8160df38:	e0855613 	add	r5, r5, r3, lsl r6
8160df3c:	e2866008 	add	r6, r6, #8
8160df40:	eaffffee 	b	8160df00 <inflate+0xe30>
8160df44:	e1a05235 	lsr	r5, r5, r2
8160df48:	e0466002 	sub	r6, r6, r2
8160df4c:	e1a00008 	mov	r0, r8
8160df50:	e3530000 	cmp	r3, #0
8160df54:	e1a05135 	lsr	r5, r5, r1
8160df58:	e0466001 	sub	r6, r6, r1
8160df5c:	e5840040 	str	r0, [r4, #64]	; 0x40
8160df60:	03a03017 	moveq	r3, #23
8160df64:	0afffcce 	beq	8160d2a4 <inflate+0x1d4>
8160df68:	e3130020 	tst	r3, #32
8160df6c:	1afffe2b 	bne	8160d820 <inflate+0x750>
8160df70:	e3130040 	tst	r3, #64	; 0x40
8160df74:	159f32b4 	ldrne	r3, [pc, #692]	; 8160e230 <inflate+0x1160>
8160df78:	1afffcc7 	bne	8160d29c <inflate+0x1cc>
8160df7c:	e203300f 	and	r3, r3, #15
8160df80:	e5843048 	str	r3, [r4, #72]	; 0x48
8160df84:	e3a03013 	mov	r3, #19
8160df88:	e5843000 	str	r3, [r4]
8160df8c:	e5943048 	ldr	r3, [r4, #72]	; 0x48
8160df90:	e3530000 	cmp	r3, #0
8160df94:	1a000018 	bne	8160dffc <inflate+0xf2c>
8160df98:	e3a03014 	mov	r3, #20
8160df9c:	e5843000 	str	r3, [r4]
8160dfa0:	e5943058 	ldr	r3, [r4, #88]	; 0x58
8160dfa4:	e3e02000 	mvn	r2, #0
8160dfa8:	e594c050 	ldr	ip, [r4, #80]	; 0x50
8160dfac:	e1e03312 	mvn	r3, r2, lsl r3
8160dfb0:	e0030005 	and	r0, r3, r5
8160dfb4:	e08c0100 	add	r0, ip, r0, lsl #2
8160dfb8:	e5d02001 	ldrb	r2, [r0, #1]
8160dfbc:	e1520006 	cmp	r2, r6
8160dfc0:	e1a01002 	mov	r1, r2
8160dfc4:	9a000016 	bls	8160e024 <inflate+0xf54>
8160dfc8:	e3570000 	cmp	r7, #0
8160dfcc:	0afffda5 	beq	8160d668 <inflate+0x598>
8160dfd0:	e4db2001 	ldrb	r2, [fp], #1
8160dfd4:	e2477001 	sub	r7, r7, #1
8160dfd8:	e0855612 	add	r5, r5, r2, lsl r6
8160dfdc:	e2866008 	add	r6, r6, #8
8160dfe0:	eafffff2 	b	8160dfb0 <inflate+0xee0>
8160dfe4:	e3570000 	cmp	r7, #0
8160dfe8:	0afffd9e 	beq	8160d668 <inflate+0x598>
8160dfec:	e4db2001 	ldrb	r2, [fp], #1
8160dff0:	e2477001 	sub	r7, r7, #1
8160dff4:	e0855612 	add	r5, r5, r2, lsl r6
8160dff8:	e2866008 	add	r6, r6, #8
8160dffc:	e1530006 	cmp	r3, r6
8160e000:	8afffff7 	bhi	8160dfe4 <inflate+0xf14>
8160e004:	e3e02000 	mvn	r2, #0
8160e008:	e0466003 	sub	r6, r6, r3
8160e00c:	e1c51312 	bic	r1, r5, r2, lsl r3
8160e010:	e5942040 	ldr	r2, [r4, #64]	; 0x40
8160e014:	e1a05335 	lsr	r5, r5, r3
8160e018:	e0822001 	add	r2, r2, r1
8160e01c:	e5842040 	str	r2, [r4, #64]	; 0x40
8160e020:	eaffffdc 	b	8160df98 <inflate+0xec8>
8160e024:	e5d03000 	ldrb	r3, [r0]
8160e028:	e1d000b2 	ldrh	r0, [r0, #2]
8160e02c:	e31300f0 	tst	r3, #240	; 0xf0
8160e030:	1a000017 	bne	8160e094 <inflate+0xfc4>
8160e034:	e0833002 	add	r3, r3, r2
8160e038:	e3e0e000 	mvn	lr, #0
8160e03c:	e1e0331e 	mvn	r3, lr, lsl r3
8160e040:	e58d301c 	str	r3, [sp, #28]
8160e044:	e59d301c 	ldr	r3, [sp, #28]
8160e048:	e0033005 	and	r3, r3, r5
8160e04c:	e0803233 	add	r3, r0, r3, lsr r2
8160e050:	e08c8103 	add	r8, ip, r3, lsl #2
8160e054:	e7dc3103 	ldrb	r3, [ip, r3, lsl #2]
8160e058:	e5d81001 	ldrb	r1, [r8, #1]
8160e05c:	e1d880b2 	ldrh	r8, [r8, #2]
8160e060:	e081e002 	add	lr, r1, r2
8160e064:	e15e0006 	cmp	lr, r6
8160e068:	9a000006 	bls	8160e088 <inflate+0xfb8>
8160e06c:	e3570000 	cmp	r7, #0
8160e070:	0afffd7c 	beq	8160d668 <inflate+0x598>
8160e074:	e4db3001 	ldrb	r3, [fp], #1
8160e078:	e2477001 	sub	r7, r7, #1
8160e07c:	e0855613 	add	r5, r5, r3, lsl r6
8160e080:	e2866008 	add	r6, r6, #8
8160e084:	eaffffee 	b	8160e044 <inflate+0xf74>
8160e088:	e1a05235 	lsr	r5, r5, r2
8160e08c:	e0466002 	sub	r6, r6, r2
8160e090:	e1a00008 	mov	r0, r8
8160e094:	e3130040 	tst	r3, #64	; 0x40
8160e098:	e1a05135 	lsr	r5, r5, r1
8160e09c:	e0466001 	sub	r6, r6, r1
8160e0a0:	159f318c 	ldrne	r3, [pc, #396]	; 8160e234 <inflate+0x1164>
8160e0a4:	1afffc7c 	bne	8160d29c <inflate+0x1cc>
8160e0a8:	e203300f 	and	r3, r3, #15
8160e0ac:	e5840044 	str	r0, [r4, #68]	; 0x44
8160e0b0:	e5843048 	str	r3, [r4, #72]	; 0x48
8160e0b4:	e3a03015 	mov	r3, #21
8160e0b8:	e5843000 	str	r3, [r4]
8160e0bc:	e5943048 	ldr	r3, [r4, #72]	; 0x48
8160e0c0:	e3530000 	cmp	r3, #0
8160e0c4:	0a000008 	beq	8160e0ec <inflate+0x101c>
8160e0c8:	e1530006 	cmp	r3, r6
8160e0cc:	8a000040 	bhi	8160e1d4 <inflate+0x1104>
8160e0d0:	e3e02000 	mvn	r2, #0
8160e0d4:	e0466003 	sub	r6, r6, r3
8160e0d8:	e1c51312 	bic	r1, r5, r2, lsl r3
8160e0dc:	e5942044 	ldr	r2, [r4, #68]	; 0x44
8160e0e0:	e1a05335 	lsr	r5, r5, r3
8160e0e4:	e0822001 	add	r2, r2, r1
8160e0e8:	e5842044 	str	r2, [r4, #68]	; 0x44
8160e0ec:	e59d2018 	ldr	r2, [sp, #24]
8160e0f0:	e594302c 	ldr	r3, [r4, #44]	; 0x2c
8160e0f4:	e0823003 	add	r3, r2, r3
8160e0f8:	e59d200c 	ldr	r2, [sp, #12]
8160e0fc:	e0433002 	sub	r3, r3, r2
8160e100:	e5942044 	ldr	r2, [r4, #68]	; 0x44
8160e104:	e1520003 	cmp	r2, r3
8160e108:	859f3128 	ldrhi	r3, [pc, #296]	; 8160e238 <inflate+0x1168>
8160e10c:	8afffc62 	bhi	8160d29c <inflate+0x1cc>
8160e110:	e3a03016 	mov	r3, #22
8160e114:	e5843000 	str	r3, [r4]
8160e118:	e59d300c 	ldr	r3, [sp, #12]
8160e11c:	e3530000 	cmp	r3, #0
8160e120:	0afffd50 	beq	8160d668 <inflate+0x598>
8160e124:	e59d3018 	ldr	r3, [sp, #24]
8160e128:	e59d200c 	ldr	r2, [sp, #12]
8160e12c:	e5941040 	ldr	r1, [r4, #64]	; 0x40
8160e130:	e0432002 	sub	r2, r3, r2
8160e134:	e5943044 	ldr	r3, [r4, #68]	; 0x44
8160e138:	e1530002 	cmp	r3, r2
8160e13c:	959d2014 	ldrls	r2, [sp, #20]
8160e140:	90422003 	subls	r2, r2, r3
8160e144:	91a03001 	movls	r3, r1
8160e148:	9a00000b 	bls	8160e17c <inflate+0x10ac>
8160e14c:	e5940030 	ldr	r0, [r4, #48]	; 0x30
8160e150:	e0433002 	sub	r3, r3, r2
8160e154:	e594c034 	ldr	ip, [r4, #52]	; 0x34
8160e158:	e1500003 	cmp	r0, r3
8160e15c:	30433000 	subcc	r3, r3, r0
8160e160:	20400003 	subcs	r0, r0, r3
8160e164:	35942028 	ldrcc	r2, [r4, #40]	; 0x28
8160e168:	208c2000 	addcs	r2, ip, r0
8160e16c:	30422003 	subcc	r2, r2, r3
8160e170:	308c2002 	addcc	r2, ip, r2
8160e174:	e1530001 	cmp	r3, r1
8160e178:	21a03001 	movcs	r3, r1
8160e17c:	e59d000c 	ldr	r0, [sp, #12]
8160e180:	e1500003 	cmp	r0, r3
8160e184:	31a03000 	movcc	r3, r0
8160e188:	e0411003 	sub	r1, r1, r3
8160e18c:	e5841040 	str	r1, [r4, #64]	; 0x40
8160e190:	e0400003 	sub	r0, r0, r3
8160e194:	e59d1014 	ldr	r1, [sp, #20]
8160e198:	e58d000c 	str	r0, [sp, #12]
8160e19c:	e0820003 	add	r0, r2, r3
8160e1a0:	e2411001 	sub	r1, r1, #1
8160e1a4:	e4d2c001 	ldrb	ip, [r2], #1
8160e1a8:	e1520000 	cmp	r2, r0
8160e1ac:	e5e1c001 	strb	ip, [r1, #1]!
8160e1b0:	1afffffb 	bne	8160e1a4 <inflate+0x10d4>
8160e1b4:	e59d2014 	ldr	r2, [sp, #20]
8160e1b8:	e0823003 	add	r3, r2, r3
8160e1bc:	e58d3014 	str	r3, [sp, #20]
8160e1c0:	e5943040 	ldr	r3, [r4, #64]	; 0x40
8160e1c4:	e3530000 	cmp	r3, #0
8160e1c8:	03a03012 	moveq	r3, #18
8160e1cc:	0afffc34 	beq	8160d2a4 <inflate+0x1d4>
8160e1d0:	eafffbdb 	b	8160d144 <inflate+0x74>
8160e1d4:	e3570000 	cmp	r7, #0
8160e1d8:	0afffd22 	beq	8160d668 <inflate+0x598>
8160e1dc:	e4db2001 	ldrb	r2, [fp], #1
8160e1e0:	e2477001 	sub	r7, r7, #1
8160e1e4:	e0855612 	add	r5, r5, r2, lsl r6
8160e1e8:	e2866008 	add	r6, r6, #8
8160e1ec:	eaffffb5 	b	8160e0c8 <inflate+0xff8>
8160e1f0:	ffff8b1f 	.word	0xffff8b1f
8160e1f4:	8161623c 	.word	0x8161623c
8160e1f8:	81616253 	.word	0x81616253
8160e1fc:	8161626e 	.word	0x8161626e
8160e200:	81616282 	.word	0x81616282
8160e204:	8161629b 	.word	0x8161629b
8160e208:	816137f2 	.word	0x816137f2
8160e20c:	81613734 	.word	0x81613734
8160e210:	816162af 	.word	0x816162af
8160e214:	816162c2 	.word	0x816162c2
8160e218:	816162df 	.word	0x816162df
8160e21c:	81614030 	.word	0x81614030
8160e220:	81616303 	.word	0x81616303
8160e224:	8161631c 	.word	0x8161631c
8160e228:	81616336 	.word	0x81616336
8160e22c:	81616352 	.word	0x81616352
8160e230:	816163c8 	.word	0x816163c8
8160e234:	816163b2 	.word	0x816163b2
8160e238:	81616394 	.word	0x81616394
8160e23c:	81616368 	.word	0x81616368
8160e240:	8161637d 	.word	0x8161637d
8160e244:	e59d300c 	ldr	r3, [sp, #12]
8160e248:	e3530000 	cmp	r3, #0
8160e24c:	0afffd05 	beq	8160d668 <inflate+0x598>
8160e250:	e59d2014 	ldr	r2, [sp, #20]
8160e254:	e5943040 	ldr	r3, [r4, #64]	; 0x40
8160e258:	e5c23000 	strb	r3, [r2]
8160e25c:	e59d300c 	ldr	r3, [sp, #12]
8160e260:	e2433001 	sub	r3, r3, #1
8160e264:	e58d300c 	str	r3, [sp, #12]
8160e268:	e3a03012 	mov	r3, #18
8160e26c:	e5843000 	str	r3, [r4]
8160e270:	e2823001 	add	r3, r2, #1
8160e274:	e58d3014 	str	r3, [sp, #20]
8160e278:	eafffbb1 	b	8160d144 <inflate+0x74>
8160e27c:	e5943008 	ldr	r3, [r4, #8]
8160e280:	e3530000 	cmp	r3, #0
8160e284:	1a000011 	bne	8160e2d0 <inflate+0x1200>
8160e288:	e3a03019 	mov	r3, #25
8160e28c:	e5843000 	str	r3, [r4]
8160e290:	e5943008 	ldr	r3, [r4, #8]
8160e294:	e3530000 	cmp	r3, #0
8160e298:	0a000002 	beq	8160e2a8 <inflate+0x11d8>
8160e29c:	e5943010 	ldr	r3, [r4, #16]
8160e2a0:	e3530000 	cmp	r3, #0
8160e2a4:	1a000041 	bne	8160e3b0 <inflate+0x12e0>
8160e2a8:	e3a0301a 	mov	r3, #26
8160e2ac:	e5843000 	str	r3, [r4]
8160e2b0:	e3a03001 	mov	r3, #1
8160e2b4:	eafffbc4 	b	8160d1cc <inflate+0xfc>
8160e2b8:	e3570000 	cmp	r7, #0
8160e2bc:	0afffce9 	beq	8160d668 <inflate+0x598>
8160e2c0:	e4db3001 	ldrb	r3, [fp], #1
8160e2c4:	e2477001 	sub	r7, r7, #1
8160e2c8:	e0855613 	add	r5, r5, r3, lsl r6
8160e2cc:	e2866008 	add	r6, r6, #8
8160e2d0:	e356001f 	cmp	r6, #31
8160e2d4:	9afffff7 	bls	8160e2b8 <inflate+0x11e8>
8160e2d8:	e59d3018 	ldr	r3, [sp, #24]
8160e2dc:	e59d200c 	ldr	r2, [sp, #12]
8160e2e0:	e0432002 	sub	r2, r3, r2
8160e2e4:	e59a3014 	ldr	r3, [sl, #20]
8160e2e8:	e3520000 	cmp	r2, #0
8160e2ec:	e0833002 	add	r3, r3, r2
8160e2f0:	e58a3014 	str	r3, [sl, #20]
8160e2f4:	e594301c 	ldr	r3, [r4, #28]
8160e2f8:	e0833002 	add	r3, r3, r2
8160e2fc:	e584301c 	str	r3, [r4, #28]
8160e300:	0a000008 	beq	8160e328 <inflate+0x1258>
8160e304:	e59d3014 	ldr	r3, [sp, #20]
8160e308:	e5940018 	ldr	r0, [r4, #24]
8160e30c:	e0431002 	sub	r1, r3, r2
8160e310:	e5943010 	ldr	r3, [r4, #16]
8160e314:	e3530000 	cmp	r3, #0
8160e318:	0a000017 	beq	8160e37c <inflate+0x12ac>
8160e31c:	eb000cd8 	bl	81611684 <crc32>
8160e320:	e5840018 	str	r0, [r4, #24]
8160e324:	e58a0034 	str	r0, [sl, #52]	; 0x34
8160e328:	e5943010 	ldr	r3, [r4, #16]
8160e32c:	e3530000 	cmp	r3, #0
8160e330:	01a02425 	lsreq	r2, r5, #8
8160e334:	01a03c05 	lsleq	r3, r5, #24
8160e338:	02022cff 	andeq	r2, r2, #65280	; 0xff00
8160e33c:	00833c25 	addeq	r3, r3, r5, lsr #24
8160e340:	00833002 	addeq	r3, r3, r2
8160e344:	01a02405 	lsleq	r2, r5, #8
8160e348:	020228ff 	andeq	r2, r2, #16711680	; 0xff0000
8160e34c:	11a03005 	movne	r3, r5
8160e350:	00833002 	addeq	r3, r3, r2
8160e354:	e5942018 	ldr	r2, [r4, #24]
8160e358:	e1520003 	cmp	r2, r3
8160e35c:	0a000008 	beq	8160e384 <inflate+0x12b4>
8160e360:	e51f312c 	ldr	r3, [pc, #-300]	; 8160e23c <inflate+0x116c>
8160e364:	e58a3018 	str	r3, [sl, #24]
8160e368:	e3a0301b 	mov	r3, #27
8160e36c:	e5843000 	str	r3, [r4]
8160e370:	e59d300c 	ldr	r3, [sp, #12]
8160e374:	e58d3018 	str	r3, [sp, #24]
8160e378:	eafffb71 	b	8160d144 <inflate+0x74>
8160e37c:	ebfffa83 	bl	8160cd90 <adler32>
8160e380:	eaffffe6 	b	8160e320 <inflate+0x1250>
8160e384:	e59d300c 	ldr	r3, [sp, #12]
8160e388:	e3a06000 	mov	r6, #0
8160e38c:	e1a05006 	mov	r5, r6
8160e390:	e58d3018 	str	r3, [sp, #24]
8160e394:	eaffffbb 	b	8160e288 <inflate+0x11b8>
8160e398:	e3570000 	cmp	r7, #0
8160e39c:	0afffcb1 	beq	8160d668 <inflate+0x598>
8160e3a0:	e4db3001 	ldrb	r3, [fp], #1
8160e3a4:	e2477001 	sub	r7, r7, #1
8160e3a8:	e0855613 	add	r5, r5, r3, lsl r6
8160e3ac:	e2866008 	add	r6, r6, #8
8160e3b0:	e356001f 	cmp	r6, #31
8160e3b4:	9afffff7 	bls	8160e398 <inflate+0x12c8>
8160e3b8:	e594301c 	ldr	r3, [r4, #28]
8160e3bc:	e1530005 	cmp	r3, r5
8160e3c0:	151f3188 	ldrne	r3, [pc, #-392]	; 8160e240 <inflate+0x1170>
8160e3c4:	1afffbb4 	bne	8160d29c <inflate+0x1cc>
8160e3c8:	e3a06000 	mov	r6, #0
8160e3cc:	e1a05006 	mov	r5, r6
8160e3d0:	eaffffb4 	b	8160e2a8 <inflate+0x11d8>
8160e3d4:	e5953028 	ldr	r3, [r5, #40]	; 0x28
8160e3d8:	e59a7010 	ldr	r7, [sl, #16]
8160e3dc:	e3530000 	cmp	r3, #0
8160e3e0:	e5950034 	ldr	r0, [r5, #52]	; 0x34
8160e3e4:	05853030 	streq	r3, [r5, #48]	; 0x30
8160e3e8:	05951024 	ldreq	r1, [r5, #36]	; 0x24
8160e3ec:	03a02001 	moveq	r2, #1
8160e3f0:	0585302c 	streq	r3, [r5, #44]	; 0x2c
8160e3f4:	e59d3018 	ldr	r3, [sp, #24]
8160e3f8:	01a02112 	lsleq	r2, r2, r1
8160e3fc:	05852028 	streq	r2, [r5, #40]	; 0x28
8160e400:	e5952028 	ldr	r2, [r5, #40]	; 0x28
8160e404:	e0437007 	sub	r7, r3, r7
8160e408:	e59a100c 	ldr	r1, [sl, #12]
8160e40c:	e1570002 	cmp	r7, r2
8160e410:	3a000037 	bcc	8160e4f4 <inflate+0x1424>
8160e414:	e0411002 	sub	r1, r1, r2
8160e418:	ebffc8df 	bl	8160079c <memcpy>
8160e41c:	e3a03000 	mov	r3, #0
8160e420:	e5853030 	str	r3, [r5, #48]	; 0x30
8160e424:	e5953028 	ldr	r3, [r5, #40]	; 0x28
8160e428:	e585302c 	str	r3, [r5, #44]	; 0x2c
8160e42c:	e59a6004 	ldr	r6, [sl, #4]
8160e430:	e59d3020 	ldr	r3, [sp, #32]
8160e434:	e59a5010 	ldr	r5, [sl, #16]
8160e438:	e0436006 	sub	r6, r3, r6
8160e43c:	e59d3018 	ldr	r3, [sp, #24]
8160e440:	e0435005 	sub	r5, r3, r5
8160e444:	e59a3008 	ldr	r3, [sl, #8]
8160e448:	e0833006 	add	r3, r3, r6
8160e44c:	e58a3008 	str	r3, [sl, #8]
8160e450:	e59a3014 	ldr	r3, [sl, #20]
8160e454:	e0833005 	add	r3, r3, r5
8160e458:	e58a3014 	str	r3, [sl, #20]
8160e45c:	e594301c 	ldr	r3, [r4, #28]
8160e460:	e0833005 	add	r3, r3, r5
8160e464:	e584301c 	str	r3, [r4, #28]
8160e468:	e5943008 	ldr	r3, [r4, #8]
8160e46c:	e3530000 	cmp	r3, #0
8160e470:	13550000 	cmpne	r5, #0
8160e474:	0a000009 	beq	8160e4a0 <inflate+0x13d0>
8160e478:	e5943010 	ldr	r3, [r4, #16]
8160e47c:	e1a02005 	mov	r2, r5
8160e480:	e59a100c 	ldr	r1, [sl, #12]
8160e484:	e3530000 	cmp	r3, #0
8160e488:	e5940018 	ldr	r0, [r4, #24]
8160e48c:	e0411005 	sub	r1, r1, r5
8160e490:	0a000034 	beq	8160e568 <inflate+0x1498>
8160e494:	eb000c7a 	bl	81611684 <crc32>
8160e498:	e5840018 	str	r0, [r4, #24]
8160e49c:	e58a0034 	str	r0, [sl, #52]	; 0x34
8160e4a0:	e5942004 	ldr	r2, [r4, #4]
8160e4a4:	e1865005 	orr	r5, r6, r5
8160e4a8:	e594303c 	ldr	r3, [r4, #60]	; 0x3c
8160e4ac:	e3520000 	cmp	r2, #0
8160e4b0:	03a02000 	moveq	r2, #0
8160e4b4:	13a02040 	movne	r2, #64	; 0x40
8160e4b8:	e0833002 	add	r3, r3, r2
8160e4bc:	e5942000 	ldr	r2, [r4]
8160e4c0:	e352000b 	cmp	r2, #11
8160e4c4:	13a02000 	movne	r2, #0
8160e4c8:	03a02080 	moveq	r2, #128	; 0x80
8160e4cc:	e0833002 	add	r3, r3, r2
8160e4d0:	e58a302c 	str	r3, [sl, #44]	; 0x2c
8160e4d4:	e59d3024 	ldr	r3, [sp, #36]	; 0x24
8160e4d8:	e3550000 	cmp	r5, #0
8160e4dc:	13530004 	cmpne	r3, #4
8160e4e0:	1afffcfe 	bne	8160d8e0 <inflate+0x810>
8160e4e4:	e59d3010 	ldr	r3, [sp, #16]
8160e4e8:	e3530000 	cmp	r3, #0
8160e4ec:	03e03004 	mvneq	r3, #4
8160e4f0:	eafffcf9 	b	8160d8dc <inflate+0x80c>
8160e4f4:	e5953030 	ldr	r3, [r5, #48]	; 0x30
8160e4f8:	e0411007 	sub	r1, r1, r7
8160e4fc:	e0422003 	sub	r2, r2, r3
8160e500:	e0800003 	add	r0, r0, r3
8160e504:	e1570002 	cmp	r7, r2
8160e508:	31a06007 	movcc	r6, r7
8160e50c:	21a06002 	movcs	r6, r2
8160e510:	e1a02006 	mov	r2, r6
8160e514:	ebffc8a0 	bl	8160079c <memcpy>
8160e518:	e0577006 	subs	r7, r7, r6
8160e51c:	0a000006 	beq	8160e53c <inflate+0x146c>
8160e520:	e59a100c 	ldr	r1, [sl, #12]
8160e524:	e1a02007 	mov	r2, r7
8160e528:	e5950034 	ldr	r0, [r5, #52]	; 0x34
8160e52c:	e0411007 	sub	r1, r1, r7
8160e530:	ebffc899 	bl	8160079c <memcpy>
8160e534:	e5857030 	str	r7, [r5, #48]	; 0x30
8160e538:	eaffffb9 	b	8160e424 <inflate+0x1354>
8160e53c:	e5953030 	ldr	r3, [r5, #48]	; 0x30
8160e540:	e595202c 	ldr	r2, [r5, #44]	; 0x2c
8160e544:	e5951028 	ldr	r1, [r5, #40]	; 0x28
8160e548:	e0863003 	add	r3, r6, r3
8160e54c:	e5853030 	str	r3, [r5, #48]	; 0x30
8160e550:	e1530001 	cmp	r3, r1
8160e554:	05857030 	streq	r7, [r5, #48]	; 0x30
8160e558:	e1510002 	cmp	r1, r2
8160e55c:	80866002 	addhi	r6, r6, r2
8160e560:	8585602c 	strhi	r6, [r5, #44]	; 0x2c
8160e564:	eaffffb0 	b	8160e42c <inflate+0x135c>
8160e568:	ebfffa08 	bl	8160cd90 <adler32>
8160e56c:	eaffffc9 	b	8160e498 <inflate+0x13c8>
8160e570:	e3e03001 	mvn	r3, #1
8160e574:	eafffcd8 	b	8160d8dc <inflate+0x80c>
8160e578:	e1a05335 	lsr	r5, r5, r3
8160e57c:	e0466003 	sub	r6, r6, r3
8160e580:	e2823001 	add	r3, r2, #1
8160e584:	e0842082 	add	r2, r4, r2, lsl #1
8160e588:	e5843068 	str	r3, [r4, #104]	; 0x68
8160e58c:	e1c207b0 	strh	r0, [r2, #112]	; 0x70
8160e590:	eafffe0a 	b	8160ddc0 <inflate+0xcf0>

8160e594 <gzalloc>:
8160e594:	e0020291 	mul	r2, r1, r2
8160e598:	e282000f 	add	r0, r2, #15
8160e59c:	e3c0000f 	bic	r0, r0, #15
8160e5a0:	eaffe3af 	b	81607464 <malloc>

8160e5a4 <gzfree>:
8160e5a4:	e1a00001 	mov	r0, r1
8160e5a8:	eaffe322 	b	81607238 <free>

8160e5ac <zunzip>:
8160e5ac:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160e5b0:	e1a05003 	mov	r5, r3
8160e5b4:	e59f30f8 	ldr	r3, [pc, #248]	; 8160e6b4 <zunzip+0x108>
8160e5b8:	e24dd040 	sub	sp, sp, #64	; 0x40
8160e5bc:	e1a06000 	mov	r6, r0
8160e5c0:	e1a08001 	mov	r8, r1
8160e5c4:	e1a0b002 	mov	fp, r2
8160e5c8:	e3e0100e 	mvn	r1, #14
8160e5cc:	e58d3024 	str	r3, [sp, #36]	; 0x24
8160e5d0:	e28d0004 	add	r0, sp, #4
8160e5d4:	e59f30dc 	ldr	r3, [pc, #220]	; 8160e6b8 <zunzip+0x10c>
8160e5d8:	e59f20dc 	ldr	r2, [pc, #220]	; 8160e6bc <zunzip+0x110>
8160e5dc:	e59d7060 	ldr	r7, [sp, #96]	; 0x60
8160e5e0:	e58d3028 	str	r3, [sp, #40]	; 0x28
8160e5e4:	e3a0303c 	mov	r3, #60	; 0x3c
8160e5e8:	e59da064 	ldr	sl, [sp, #100]	; 0x64
8160e5ec:	ebfff98e 	bl	8160cc2c <inflateInit2_>
8160e5f0:	e2504000 	subs	r4, r0, #0
8160e5f4:	0a000006 	beq	8160e614 <zunzip+0x68>
8160e5f8:	e1a01004 	mov	r1, r4
8160e5fc:	e59f00bc 	ldr	r0, [pc, #188]	; 8160e6c0 <zunzip+0x114>
8160e600:	eb00101d 	bl	8161267c <printf>
8160e604:	e3e04000 	mvn	r4, #0
8160e608:	e1a00004 	mov	r0, r4
8160e60c:	e28dd040 	add	sp, sp, #64	; 0x40
8160e610:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160e614:	e5953000 	ldr	r3, [r5]
8160e618:	e08bb00a 	add	fp, fp, sl
8160e61c:	e58d6010 	str	r6, [sp, #16]
8160e620:	e043300a 	sub	r3, r3, sl
8160e624:	e58db004 	str	fp, [sp, #4]
8160e628:	e58d3008 	str	r3, [sp, #8]
8160e62c:	e58d8014 	str	r8, [sp, #20]
8160e630:	e3a01004 	mov	r1, #4
8160e634:	e08d0001 	add	r0, sp, r1
8160e638:	ebfffaa4 	bl	8160d0d0 <inflate>
8160e63c:	e2473001 	sub	r3, r7, #1
8160e640:	e16f3f13 	clz	r3, r3
8160e644:	e1a032a3 	lsr	r3, r3, #5
8160e648:	e3500001 	cmp	r0, #1
8160e64c:	e1a01000 	mov	r1, r0
8160e650:	03a03000 	moveq	r3, #0
8160e654:	e3530000 	cmp	r3, #0
8160e658:	0a000012 	beq	8160e6a8 <zunzip+0xfc>
8160e65c:	e59d3008 	ldr	r3, [sp, #8]
8160e660:	e3530000 	cmp	r3, #0
8160e664:	1a000008 	bne	8160e68c <zunzip+0xe0>
8160e668:	e59f0054 	ldr	r0, [pc, #84]	; 8160e6c4 <zunzip+0x118>
8160e66c:	e3e04000 	mvn	r4, #0
8160e670:	eb001001 	bl	8161267c <printf>
8160e674:	e59d0010 	ldr	r0, [sp, #16]
8160e678:	e0400006 	sub	r0, r0, r6
8160e67c:	e5850000 	str	r0, [r5]
8160e680:	e28d0004 	add	r0, sp, #4
8160e684:	ebfff9a8 	bl	8160cd2c <inflateEnd>
8160e688:	eaffffde 	b	8160e608 <zunzip+0x5c>
8160e68c:	e2903005 	adds	r3, r0, #5
8160e690:	e59d2014 	ldr	r2, [sp, #20]
8160e694:	13a03001 	movne	r3, #1
8160e698:	e3520000 	cmp	r2, #0
8160e69c:	03a03001 	moveq	r3, #1
8160e6a0:	e3530000 	cmp	r3, #0
8160e6a4:	1affffef 	bne	8160e668 <zunzip+0xbc>
8160e6a8:	e3710005 	cmn	r1, #5
8160e6ac:	0affffdf 	beq	8160e630 <zunzip+0x84>
8160e6b0:	eaffffef 	b	8160e674 <zunzip+0xc8>
8160e6b4:	8160e594 	.word	0x8160e594
8160e6b8:	8160e5a4 	.word	0x8160e5a4
8160e6bc:	81617c0f 	.word	0x81617c0f
8160e6c0:	81617c15 	.word	0x81617c15
8160e6c4:	81617c38 	.word	0x81617c38

8160e6c8 <gunzip>:
8160e6c8:	e5d2c002 	ldrb	ip, [r2, #2]
8160e6cc:	e92d4037 	push	{r0, r1, r2, r4, r5, lr}
8160e6d0:	e35c0008 	cmp	ip, #8
8160e6d4:	e5d2e003 	ldrb	lr, [r2, #3]
8160e6d8:	1a000001 	bne	8160e6e4 <gunzip+0x1c>
8160e6dc:	e31e00e0 	tst	lr, #224	; 0xe0
8160e6e0:	0a000004 	beq	8160e6f8 <gunzip+0x30>
8160e6e4:	e59f0088 	ldr	r0, [pc, #136]	; 8160e774 <gunzip+0xac>
8160e6e8:	ebffe1a9 	bl	81606d94 <puts>
8160e6ec:	e3e00000 	mvn	r0, #0
8160e6f0:	e28dd00c 	add	sp, sp, #12
8160e6f4:	e8bd8030 	pop	{r4, r5, pc}
8160e6f8:	e31e0004 	tst	lr, #4
8160e6fc:	15d2c00a 	ldrbne	ip, [r2, #10]
8160e700:	03a0c00a 	moveq	ip, #10
8160e704:	15d2400b 	ldrbne	r4, [r2, #11]
8160e708:	128cc00c 	addne	ip, ip, #12
8160e70c:	108cc404 	addne	ip, ip, r4, lsl #8
8160e710:	e31e0008 	tst	lr, #8
8160e714:	0a000004 	beq	8160e72c <gunzip+0x64>
8160e718:	e082400c 	add	r4, r2, ip
8160e71c:	e4d45001 	ldrb	r5, [r4], #1
8160e720:	e28cc001 	add	ip, ip, #1
8160e724:	e3550000 	cmp	r5, #0
8160e728:	1afffffb 	bne	8160e71c <gunzip+0x54>
8160e72c:	e31e0010 	tst	lr, #16
8160e730:	0a000004 	beq	8160e748 <gunzip+0x80>
8160e734:	e082400c 	add	r4, r2, ip
8160e738:	e4d45001 	ldrb	r5, [r4], #1
8160e73c:	e28cc001 	add	ip, ip, #1
8160e740:	e3550000 	cmp	r5, #0
8160e744:	1afffffb 	bne	8160e738 <gunzip+0x70>
8160e748:	e31e0002 	tst	lr, #2
8160e74c:	e593e000 	ldr	lr, [r3]
8160e750:	128cc002 	addne	ip, ip, #2
8160e754:	e15e000c 	cmp	lr, ip
8160e758:	959f0018 	ldrls	r0, [pc, #24]	; 8160e778 <gunzip+0xb0>
8160e75c:	9affffe1 	bls	8160e6e8 <gunzip+0x20>
8160e760:	e58dc004 	str	ip, [sp, #4]
8160e764:	e3a0c001 	mov	ip, #1
8160e768:	e58dc000 	str	ip, [sp]
8160e76c:	ebffff8e 	bl	8160e5ac <zunzip>
8160e770:	eaffffde 	b	8160e6f0 <gunzip+0x28>
8160e774:	81615edb 	.word	0x81615edb
8160e778:	81615ef4 	.word	0x81615ef4

8160e77c <initcall_run_list>:
8160e77c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160e780:	e1a06000 	mov	r6, r0
8160e784:	e1a05000 	mov	r5, r0
8160e788:	e1a07005 	mov	r7, r5
8160e78c:	e4950004 	ldr	r0, [r5], #4
8160e790:	e3500000 	cmp	r0, #0
8160e794:	08bd81f0 	popeq	{r4, r5, r6, r7, r8, pc}
8160e798:	e5994004 	ldr	r4, [r9, #4]
8160e79c:	e1a03009 	mov	r3, r9
8160e7a0:	e2144001 	ands	r4, r4, #1
8160e7a4:	15994040 	ldrne	r4, [r9, #64]	; 0x40
8160e7a8:	e5933004 	ldr	r3, [r3, #4]
8160e7ac:	e12fff30 	blx	r0
8160e7b0:	e2503000 	subs	r3, r0, #0
8160e7b4:	0afffff3 	beq	8160e788 <initcall_run_list+0xc>
8160e7b8:	e5972000 	ldr	r2, [r7]
8160e7bc:	e1a01006 	mov	r1, r6
8160e7c0:	e59f000c 	ldr	r0, [pc, #12]	; 8160e7d4 <initcall_run_list+0x58>
8160e7c4:	e0422004 	sub	r2, r2, r4
8160e7c8:	eb000fab 	bl	8161267c <printf>
8160e7cc:	e3e00000 	mvn	r0, #0
8160e7d0:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160e7d4:	816163ed 	.word	0x816163ed

8160e7d8 <lmb_add_region>:
8160e7d8:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160e7dc:	e590e000 	ldr	lr, [r0]
8160e7e0:	e35e0001 	cmp	lr, #1
8160e7e4:	1a000002 	bne	8160e7f4 <lmb_add_region+0x1c>
8160e7e8:	e590300c 	ldr	r3, [r0, #12]
8160e7ec:	e3530000 	cmp	r3, #0
8160e7f0:	0a000006 	beq	8160e810 <lmb_add_region+0x38>
8160e7f4:	e0816002 	add	r6, r1, r2
8160e7f8:	e1a05000 	mov	r5, r0
8160e7fc:	e3a03000 	mov	r3, #0
8160e800:	e15e0003 	cmp	lr, r3
8160e804:	1a000005 	bne	8160e820 <lmb_add_region+0x48>
8160e808:	e3a0c000 	mov	ip, #0
8160e80c:	ea000011 	b	8160e858 <lmb_add_region+0x80>
8160e810:	e5801008 	str	r1, [r0, #8]
8160e814:	e580200c 	str	r2, [r0, #12]
8160e818:	e3a00000 	mov	r0, #0
8160e81c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160e820:	e5954008 	ldr	r4, [r5, #8]
8160e824:	e595c00c 	ldr	ip, [r5, #12]
8160e828:	e1510004 	cmp	r1, r4
8160e82c:	0152000c 	cmpeq	r2, ip
8160e830:	0afffff8 	beq	8160e818 <lmb_add_region+0x40>
8160e834:	e1540006 	cmp	r4, r6
8160e838:	1a00001d 	bne	8160e8b4 <lmb_add_region+0xdc>
8160e83c:	e080c183 	add	ip, r0, r3, lsl #3
8160e840:	e0466002 	sub	r6, r6, r2
8160e844:	e59c400c 	ldr	r4, [ip, #12]
8160e848:	e58c6008 	str	r6, [ip, #8]
8160e84c:	e0844002 	add	r4, r4, r2
8160e850:	e58c400c 	str	r4, [ip, #12]
8160e854:	e3a0c001 	mov	ip, #1
8160e858:	e24e6001 	sub	r6, lr, #1
8160e85c:	e1530006 	cmp	r3, r6
8160e860:	2a000023 	bcs	8160e8f4 <lmb_add_region+0x11c>
8160e864:	e2837001 	add	r7, r3, #1
8160e868:	e0803183 	add	r3, r0, r3, lsl #3
8160e86c:	e283400c 	add	r4, r3, #12
8160e870:	e593a008 	ldr	sl, [r3, #8]
8160e874:	e8940130 	ldm	r4, {r4, r5, r8}
8160e878:	e08ab004 	add	fp, sl, r4
8160e87c:	e155000b 	cmp	r5, fp
8160e880:	0a000002 	beq	8160e890 <lmb_add_region+0xb8>
8160e884:	e0855008 	add	r5, r5, r8
8160e888:	e15a0005 	cmp	sl, r5
8160e88c:	1a000018 	bne	8160e8f4 <lmb_add_region+0x11c>
8160e890:	e0844008 	add	r4, r4, r8
8160e894:	e583400c 	str	r4, [r3, #12]
8160e898:	e1560007 	cmp	r6, r7
8160e89c:	e2833008 	add	r3, r3, #8
8160e8a0:	8a00000d 	bhi	8160e8dc <lmb_add_region+0x104>
8160e8a4:	e28cc001 	add	ip, ip, #1
8160e8a8:	e5806000 	str	r6, [r0]
8160e8ac:	e1a0000c 	mov	r0, ip
8160e8b0:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160e8b4:	e084400c 	add	r4, r4, ip
8160e8b8:	e2855008 	add	r5, r5, #8
8160e8bc:	e1510004 	cmp	r1, r4
8160e8c0:	e2834001 	add	r4, r3, #1
8160e8c4:	00804184 	addeq	r4, r0, r4, lsl #3
8160e8c8:	0082c00c 	addeq	ip, r2, ip
8160e8cc:	0584c004 	streq	ip, [r4, #4]
8160e8d0:	0affffdf 	beq	8160e854 <lmb_add_region+0x7c>
8160e8d4:	e1a03004 	mov	r3, r4
8160e8d8:	eaffffc8 	b	8160e800 <lmb_add_region+0x28>
8160e8dc:	e5932010 	ldr	r2, [r3, #16]
8160e8e0:	e2877001 	add	r7, r7, #1
8160e8e4:	e5832008 	str	r2, [r3, #8]
8160e8e8:	e5932014 	ldr	r2, [r3, #20]
8160e8ec:	e583200c 	str	r2, [r3, #12]
8160e8f0:	eaffffe8 	b	8160e898 <lmb_add_region+0xc0>
8160e8f4:	e35c0000 	cmp	ip, #0
8160e8f8:	1affffeb 	bne	8160e8ac <lmb_add_region+0xd4>
8160e8fc:	e35e0007 	cmp	lr, #7
8160e900:	9080c18e 	addls	ip, r0, lr, lsl #3
8160e904:	91a0300e 	movls	r3, lr
8160e908:	9a000009 	bls	8160e934 <lmb_add_region+0x15c>
8160e90c:	e3e00000 	mvn	r0, #0
8160e910:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160e914:	e41c4008 	ldr	r4, [ip], #-8
8160e918:	e2435001 	sub	r5, r3, #1
8160e91c:	e1540001 	cmp	r4, r1
8160e920:	9a000006 	bls	8160e940 <lmb_add_region+0x168>
8160e924:	e59c300c 	ldr	r3, [ip, #12]
8160e928:	e58c4010 	str	r4, [ip, #16]
8160e92c:	e58c3014 	str	r3, [ip, #20]
8160e930:	e1a03005 	mov	r3, r5
8160e934:	e3530000 	cmp	r3, #0
8160e938:	1afffff5 	bne	8160e914 <lmb_add_region+0x13c>
8160e93c:	ea000002 	b	8160e94c <lmb_add_region+0x174>
8160e940:	e0803183 	add	r3, r0, r3, lsl #3
8160e944:	e5831008 	str	r1, [r3, #8]
8160e948:	e583200c 	str	r2, [r3, #12]
8160e94c:	e5903008 	ldr	r3, [r0, #8]
8160e950:	e28ee001 	add	lr, lr, #1
8160e954:	e580e000 	str	lr, [r0]
8160e958:	e1530001 	cmp	r3, r1
8160e95c:	85801008 	strhi	r1, [r0, #8]
8160e960:	8580200c 	strhi	r2, [r0, #12]
8160e964:	eaffffab 	b	8160e818 <lmb_add_region+0x40>

8160e968 <lmb_init>:
8160e968:	e3a03000 	mov	r3, #0
8160e96c:	e3a02001 	mov	r2, #1
8160e970:	e5803008 	str	r3, [r0, #8]
8160e974:	e580300c 	str	r3, [r0, #12]
8160e978:	e880000c 	stm	r0, {r2, r3}
8160e97c:	e5803058 	str	r3, [r0, #88]	; 0x58
8160e980:	e580305c 	str	r3, [r0, #92]	; 0x5c
8160e984:	e5802050 	str	r2, [r0, #80]	; 0x50
8160e988:	e5803054 	str	r3, [r0, #84]	; 0x54
8160e98c:	e12fff1e 	bx	lr

8160e990 <lmb_add>:
8160e990:	eaffff90 	b	8160e7d8 <lmb_add_region>

8160e994 <lmb_free>:
8160e994:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160e998:	e1a0c001 	mov	ip, r1
8160e99c:	e5906050 	ldr	r6, [r0, #80]	; 0x50
8160e9a0:	e0811002 	add	r1, r1, r2
8160e9a4:	e1a0e000 	mov	lr, r0
8160e9a8:	e3a03000 	mov	r3, #0
8160e9ac:	e1530006 	cmp	r3, r6
8160e9b0:	e1a07003 	mov	r7, r3
8160e9b4:	1a000001 	bne	8160e9c0 <lmb_free+0x2c>
8160e9b8:	e3e00000 	mvn	r0, #0
8160e9bc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160e9c0:	e59e5058 	ldr	r5, [lr, #88]	; 0x58
8160e9c4:	e28ee008 	add	lr, lr, #8
8160e9c8:	e59e4054 	ldr	r4, [lr, #84]	; 0x54
8160e9cc:	e15c0005 	cmp	ip, r5
8160e9d0:	e0854004 	add	r4, r5, r4
8160e9d4:	33a08000 	movcc	r8, #0
8160e9d8:	23a08001 	movcs	r8, #1
8160e9dc:	e1510004 	cmp	r1, r4
8160e9e0:	83a08000 	movhi	r8, #0
8160e9e4:	e3580000 	cmp	r8, #0
8160e9e8:	02833001 	addeq	r3, r3, #1
8160e9ec:	0affffee 	beq	8160e9ac <lmb_free+0x18>
8160e9f0:	e15c0005 	cmp	ip, r5
8160e9f4:	01540001 	cmpeq	r4, r1
8160e9f8:	e1a03183 	lsl	r3, r3, #3
8160e9fc:	e0803003 	add	r3, r0, r3
8160ea00:	02462001 	subeq	r2, r6, #1
8160ea04:	03a0e001 	moveq	lr, #1
8160ea08:	13a0e000 	movne	lr, #0
8160ea0c:	0a00000d 	beq	8160ea48 <lmb_free+0xb4>
8160ea10:	e15c0005 	cmp	ip, r5
8160ea14:	05831058 	streq	r1, [r3, #88]	; 0x58
8160ea18:	0a000001 	beq	8160ea24 <lmb_free+0x90>
8160ea1c:	e1540001 	cmp	r4, r1
8160ea20:	1a00000e 	bne	8160ea60 <lmb_free+0xcc>
8160ea24:	e593105c 	ldr	r1, [r3, #92]	; 0x5c
8160ea28:	e0412002 	sub	r2, r1, r2
8160ea2c:	e583205c 	str	r2, [r3, #92]	; 0x5c
8160ea30:	ea000008 	b	8160ea58 <lmb_free+0xc4>
8160ea34:	e5931058 	ldr	r1, [r3, #88]	; 0x58
8160ea38:	e2877001 	add	r7, r7, #1
8160ea3c:	e5831050 	str	r1, [r3, #80]	; 0x50
8160ea40:	e593105c 	ldr	r1, [r3, #92]	; 0x5c
8160ea44:	e5831054 	str	r1, [r3, #84]	; 0x54
8160ea48:	e1570002 	cmp	r7, r2
8160ea4c:	e2833008 	add	r3, r3, #8
8160ea50:	3afffff7 	bcc	8160ea34 <lmb_free+0xa0>
8160ea54:	e5802050 	str	r2, [r0, #80]	; 0x50
8160ea58:	e3a00000 	mov	r0, #0
8160ea5c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160ea60:	e5932058 	ldr	r2, [r3, #88]	; 0x58
8160ea64:	e2800050 	add	r0, r0, #80	; 0x50
8160ea68:	e04cc002 	sub	ip, ip, r2
8160ea6c:	e0442001 	sub	r2, r4, r1
8160ea70:	e583c05c 	str	ip, [r3, #92]	; 0x5c
8160ea74:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
8160ea78:	eaffff56 	b	8160e7d8 <lmb_add_region>

8160ea7c <lmb_reserve>:
8160ea7c:	e2800050 	add	r0, r0, #80	; 0x50
8160ea80:	eaffff54 	b	8160e7d8 <lmb_add_region>

8160ea84 <__lmb_alloc_base>:
8160ea84:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160ea88:	e2628000 	rsb	r8, r2, #0
8160ea8c:	e590e000 	ldr	lr, [r0]
8160ea90:	e24e7001 	sub	r7, lr, #1
8160ea94:	e080e18e 	add	lr, r0, lr, lsl #3
8160ea98:	e3570000 	cmp	r7, #0
8160ea9c:	ba00002a 	blt	8160eb4c <__lmb_alloc_base+0xc8>
8160eaa0:	e59e4004 	ldr	r4, [lr, #4]
8160eaa4:	e1540001 	cmp	r4, r1
8160eaa8:	3a000012 	bcc	8160eaf8 <__lmb_alloc_base+0x74>
8160eaac:	e59e5000 	ldr	r5, [lr]
8160eab0:	e3530000 	cmp	r3, #0
8160eab4:	00854004 	addeq	r4, r5, r4
8160eab8:	0a000038 	beq	8160eba0 <__lmb_alloc_base+0x11c>
8160eabc:	e1550003 	cmp	r5, r3
8160eac0:	2a00000c 	bcs	8160eaf8 <__lmb_alloc_base+0x74>
8160eac4:	e0954004 	adds	r4, r5, r4
8160eac8:	23e04000 	mvncs	r4, #0
8160eacc:	e1540003 	cmp	r4, r3
8160ead0:	90614004 	rsbls	r4, r1, r4
8160ead4:	80614003 	rsbhi	r4, r1, r3
8160ead8:	e0044008 	and	r4, r4, r8
8160eadc:	e1540005 	cmp	r4, r5
8160eae0:	33a0c000 	movcc	ip, #0
8160eae4:	23a0c001 	movcs	ip, #1
8160eae8:	e3540000 	cmp	r4, #0
8160eaec:	03a0c000 	moveq	ip, #0
8160eaf0:	e35c0000 	cmp	ip, #0
8160eaf4:	1a000002 	bne	8160eb04 <__lmb_alloc_base+0x80>
8160eaf8:	e2477001 	sub	r7, r7, #1
8160eafc:	e24ee008 	sub	lr, lr, #8
8160eb00:	eaffffe4 	b	8160ea98 <__lmb_alloc_base+0x14>
8160eb04:	e590c050 	ldr	ip, [r0, #80]	; 0x50
8160eb08:	e1a06000 	mov	r6, r0
8160eb0c:	e084a001 	add	sl, r4, r1
8160eb10:	e58da004 	str	sl, [sp, #4]
8160eb14:	e58dc000 	str	ip, [sp]
8160eb18:	e3a0c000 	mov	ip, #0
8160eb1c:	e59da000 	ldr	sl, [sp]
8160eb20:	e15c000a 	cmp	ip, sl
8160eb24:	1a00000c 	bne	8160eb5c <__lmb_alloc_base+0xd8>
8160eb28:	e2423001 	sub	r3, r2, #1
8160eb2c:	e2622000 	rsb	r2, r2, #0
8160eb30:	e0831001 	add	r1, r3, r1
8160eb34:	e2800050 	add	r0, r0, #80	; 0x50
8160eb38:	e0022001 	and	r2, r2, r1
8160eb3c:	e1a01004 	mov	r1, r4
8160eb40:	ebffff24 	bl	8160e7d8 <lmb_add_region>
8160eb44:	e3500000 	cmp	r0, #0
8160eb48:	aa000000 	bge	8160eb50 <__lmb_alloc_base+0xcc>
8160eb4c:	e3a04000 	mov	r4, #0
8160eb50:	e1a00004 	mov	r0, r4
8160eb54:	e28dd008 	add	sp, sp, #8
8160eb58:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160eb5c:	e596b058 	ldr	fp, [r6, #88]	; 0x58
8160eb60:	e596a05c 	ldr	sl, [r6, #92]	; 0x5c
8160eb64:	e08ba00a 	add	sl, fp, sl
8160eb68:	e154000a 	cmp	r4, sl
8160eb6c:	2a000002 	bcs	8160eb7c <__lmb_alloc_base+0xf8>
8160eb70:	e59da004 	ldr	sl, [sp, #4]
8160eb74:	e15b000a 	cmp	fp, sl
8160eb78:	3a000002 	bcc	8160eb88 <__lmb_alloc_base+0x104>
8160eb7c:	e28cc001 	add	ip, ip, #1
8160eb80:	e2866008 	add	r6, r6, #8
8160eb84:	eaffffe4 	b	8160eb1c <__lmb_alloc_base+0x98>
8160eb88:	e35c0000 	cmp	ip, #0
8160eb8c:	baffffe5 	blt	8160eb28 <__lmb_alloc_base+0xa4>
8160eb90:	e28cc00b 	add	ip, ip, #11
8160eb94:	e790418c 	ldr	r4, [r0, ip, lsl #3]
8160eb98:	e1510004 	cmp	r1, r4
8160eb9c:	8affffd5 	bhi	8160eaf8 <__lmb_alloc_base+0x74>
8160eba0:	e0444001 	sub	r4, r4, r1
8160eba4:	eaffffcb 	b	8160ead8 <__lmb_alloc_base+0x54>

8160eba8 <lmb_alloc_base>:
8160eba8:	e92d4070 	push	{r4, r5, r6, lr}
8160ebac:	e1a05001 	mov	r5, r1
8160ebb0:	e1a06003 	mov	r6, r3
8160ebb4:	ebffffb2 	bl	8160ea84 <__lmb_alloc_base>
8160ebb8:	e2504000 	subs	r4, r0, #0
8160ebbc:	1a000003 	bne	8160ebd0 <lmb_alloc_base+0x28>
8160ebc0:	e1a02006 	mov	r2, r6
8160ebc4:	e1a01005 	mov	r1, r5
8160ebc8:	e59f0008 	ldr	r0, [pc, #8]	; 8160ebd8 <lmb_alloc_base+0x30>
8160ebcc:	eb000eaa 	bl	8161267c <printf>
8160ebd0:	e1a00004 	mov	r0, r4
8160ebd4:	e8bd8070 	pop	{r4, r5, r6, pc}
8160ebd8:	81616426 	.word	0x81616426

8160ebdc <lmb_alloc>:
8160ebdc:	e3a03000 	mov	r3, #0
8160ebe0:	eafffff0 	b	8160eba8 <lmb_alloc_base>

8160ebe4 <board_lmb_reserve>:
8160ebe4:	e12fff1e 	bx	lr

8160ebe8 <string_to_ip>:
8160ebe8:	e92d4073 	push	{r0, r1, r4, r5, r6, lr}
8160ebec:	e2504000 	subs	r4, r0, #0
8160ebf0:	13a05000 	movne	r5, #0
8160ebf4:	11a06005 	movne	r6, r5
8160ebf8:	1a000002 	bne	8160ec08 <string_to_ip+0x20>
8160ebfc:	e3a00000 	mov	r0, #0
8160ec00:	e28dd008 	add	sp, sp, #8
8160ec04:	e8bd8070 	pop	{r4, r5, r6, pc}
8160ec08:	e3540000 	cmp	r4, #0
8160ec0c:	0a000017 	beq	8160ec70 <string_to_ip+0x88>
8160ec10:	e3a0200a 	mov	r2, #10
8160ec14:	e28d1004 	add	r1, sp, #4
8160ec18:	e1a00004 	mov	r0, r4
8160ec1c:	eb000edd 	bl	81612798 <simple_strtoul>
8160ec20:	e35000ff 	cmp	r0, #255	; 0xff
8160ec24:	8afffff4 	bhi	8160ebfc <string_to_ip+0x14>
8160ec28:	e3560003 	cmp	r6, #3
8160ec2c:	0a000003 	beq	8160ec40 <string_to_ip+0x58>
8160ec30:	e59d3004 	ldr	r3, [sp, #4]
8160ec34:	e5d33000 	ldrb	r3, [r3]
8160ec38:	e353002e 	cmp	r3, #46	; 0x2e
8160ec3c:	1affffee 	bne	8160ebfc <string_to_ip+0x14>
8160ec40:	e3540000 	cmp	r4, #0
8160ec44:	e1805405 	orr	r5, r0, r5, lsl #8
8160ec48:	0a000003 	beq	8160ec5c <string_to_ip+0x74>
8160ec4c:	e59d4004 	ldr	r4, [sp, #4]
8160ec50:	e5d43000 	ldrb	r3, [r4]
8160ec54:	e3530000 	cmp	r3, #0
8160ec58:	12844001 	addne	r4, r4, #1
8160ec5c:	e2866001 	add	r6, r6, #1
8160ec60:	e3560004 	cmp	r6, #4
8160ec64:	1affffe7 	bne	8160ec08 <string_to_ip+0x20>
8160ec68:	e6bf0f35 	rev	r0, r5
8160ec6c:	eaffffe3 	b	8160ec00 <string_to_ip+0x18>
8160ec70:	e1a00004 	mov	r0, r4
8160ec74:	eaffffeb 	b	8160ec28 <string_to_ip+0x40>

8160ec78 <qsort>:
8160ec78:	e3520000 	cmp	r2, #0
8160ec7c:	13510001 	cmpne	r1, #1
8160ec80:	912fff1e 	bxls	lr
8160ec84:	e59fc0d4 	ldr	ip, [pc, #212]	; 8160ed60 <qsort+0xe8>
8160ec88:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160ec8c:	e2414001 	sub	r4, r1, #1
8160ec90:	e24dd018 	sub	sp, sp, #24
8160ec94:	e0854c94 	umull	r4, r5, r4, ip
8160ec98:	e3a04000 	mov	r4, #0
8160ec9c:	e58dc008 	str	ip, [sp, #8]
8160eca0:	e1a0e0a5 	lsr	lr, r5, #1
8160eca4:	e0844084 	add	r4, r4, r4, lsl #1
8160eca8:	e2844001 	add	r4, r4, #1
8160ecac:	e15e0004 	cmp	lr, r4
8160ecb0:	8afffffb 	bhi	8160eca4 <qsort+0x2c>
8160ecb4:	e1a05002 	mov	r5, r2
8160ecb8:	e1a07000 	mov	r7, r0
8160ecbc:	e0040294 	mul	r4, r4, r2
8160ecc0:	e0020192 	mul	r2, r2, r1
8160ecc4:	e58d2004 	str	r2, [sp, #4]
8160ecc8:	e0852000 	add	r2, r5, r0
8160eccc:	e58d200c 	str	r2, [sp, #12]
8160ecd0:	e1a0a004 	mov	sl, r4
8160ecd4:	e0842007 	add	r2, r4, r7
8160ecd8:	e1a0600a 	mov	r6, sl
8160ecdc:	e0466004 	sub	r6, r6, r4
8160ece0:	e58d3010 	str	r3, [sp, #16]
8160ece4:	e082b006 	add	fp, r2, r6
8160ece8:	e0868007 	add	r8, r6, r7
8160ecec:	e58d2014 	str	r2, [sp, #20]
8160ecf0:	e1a0100b 	mov	r1, fp
8160ecf4:	e1a00008 	mov	r0, r8
8160ecf8:	e12fff33 	blx	r3
8160ecfc:	e59d3010 	ldr	r3, [sp, #16]
8160ed00:	e3500000 	cmp	r0, #0
8160ed04:	da00000a 	ble	8160ed34 <qsort+0xbc>
8160ed08:	e59d200c 	ldr	r2, [sp, #12]
8160ed0c:	e0821006 	add	r1, r2, r6
8160ed10:	e59d2014 	ldr	r2, [sp, #20]
8160ed14:	e5dbc000 	ldrb	ip, [fp]
8160ed18:	e5d80000 	ldrb	r0, [r8]
8160ed1c:	e4c8c001 	strb	ip, [r8], #1
8160ed20:	e1580001 	cmp	r8, r1
8160ed24:	e4cb0001 	strb	r0, [fp], #1
8160ed28:	1afffff9 	bne	8160ed14 <qsort+0x9c>
8160ed2c:	e1540006 	cmp	r4, r6
8160ed30:	9affffe9 	bls	8160ecdc <qsort+0x64>
8160ed34:	e59d2004 	ldr	r2, [sp, #4]
8160ed38:	e08aa005 	add	sl, sl, r5
8160ed3c:	e152000a 	cmp	r2, sl
8160ed40:	8affffe3 	bhi	8160ecd4 <qsort+0x5c>
8160ed44:	e59d2008 	ldr	r2, [sp, #8]
8160ed48:	e0444005 	sub	r4, r4, r5
8160ed4c:	e0810294 	umull	r0, r1, r4, r2
8160ed50:	e1b040a1 	lsrs	r4, r1, #1
8160ed54:	1affffdd 	bne	8160ecd0 <qsort+0x58>
8160ed58:	e28dd018 	add	sp, sp, #24
8160ed5c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160ed60:	aaaaaaab 	.word	0xaaaaaaab

8160ed64 <strcmp_compar>:
8160ed64:	e5911000 	ldr	r1, [r1]
8160ed68:	e5900000 	ldr	r0, [r0]
8160ed6c:	ea000ab3 	b	81611840 <strcmp>

8160ed70 <fdt_check_header>:
8160ed70:	e5903000 	ldr	r3, [r0]
8160ed74:	e59f2060 	ldr	r2, [pc, #96]	; 8160eddc <fdt_check_header+0x6c>
8160ed78:	e6bf3f33 	rev	r3, r3
8160ed7c:	e1530002 	cmp	r3, r2
8160ed80:	1a000009 	bne	8160edac <fdt_check_header+0x3c>
8160ed84:	e5903014 	ldr	r3, [r0, #20]
8160ed88:	e6bf3f33 	rev	r3, r3
8160ed8c:	e353000f 	cmp	r3, #15
8160ed90:	9a00000d 	bls	8160edcc <fdt_check_header+0x5c>
8160ed94:	e5903018 	ldr	r3, [r0, #24]
8160ed98:	e6bf3f33 	rev	r3, r3
8160ed9c:	e3530012 	cmp	r3, #18
8160eda0:	33a00000 	movcc	r0, #0
8160eda4:	23e00009 	mvncs	r0, #9
8160eda8:	e12fff1e 	bx	lr
8160edac:	e59f202c 	ldr	r2, [pc, #44]	; 8160ede0 <fdt_check_header+0x70>
8160edb0:	e1530002 	cmp	r3, r2
8160edb4:	1a000006 	bne	8160edd4 <fdt_check_header+0x64>
8160edb8:	e5903024 	ldr	r3, [r0, #36]	; 0x24
8160edbc:	e3530000 	cmp	r3, #0
8160edc0:	13a00000 	movne	r0, #0
8160edc4:	03e00006 	mvneq	r0, #6
8160edc8:	e12fff1e 	bx	lr
8160edcc:	e3e00009 	mvn	r0, #9
8160edd0:	e12fff1e 	bx	lr
8160edd4:	e3e00008 	mvn	r0, #8
8160edd8:	e12fff1e 	bx	lr
8160eddc:	d00dfeed 	.word	0xd00dfeed
8160ede0:	2ff20112 	.word	0x2ff20112

8160ede4 <fdt_offset_ptr>:
8160ede4:	e5903008 	ldr	r3, [r0, #8]
8160ede8:	e6bf3f33 	rev	r3, r3
8160edec:	e0913003 	adds	r3, r1, r3
8160edf0:	2a000004 	bcs	8160ee08 <fdt_offset_ptr+0x24>
8160edf4:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
8160edf8:	e093e002 	adds	lr, r3, r2
8160edfc:	3a000003 	bcc	8160ee10 <fdt_offset_ptr+0x2c>
8160ee00:	e3a00000 	mov	r0, #0
8160ee04:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8160ee08:	e3a00000 	mov	r0, #0
8160ee0c:	e12fff1e 	bx	lr
8160ee10:	e590c004 	ldr	ip, [r0, #4]
8160ee14:	e6bfcf3c 	rev	ip, ip
8160ee18:	e15e000c 	cmp	lr, ip
8160ee1c:	8afffff7 	bhi	8160ee00 <fdt_offset_ptr+0x1c>
8160ee20:	e590c014 	ldr	ip, [r0, #20]
8160ee24:	e6bfcf3c 	rev	ip, ip
8160ee28:	e35c0010 	cmp	ip, #16
8160ee2c:	9a000005 	bls	8160ee48 <fdt_offset_ptr+0x64>
8160ee30:	e0911002 	adds	r1, r1, r2
8160ee34:	2afffff1 	bcs	8160ee00 <fdt_offset_ptr+0x1c>
8160ee38:	e5902024 	ldr	r2, [r0, #36]	; 0x24
8160ee3c:	e6bf2f32 	rev	r2, r2
8160ee40:	e1510002 	cmp	r1, r2
8160ee44:	8affffed 	bhi	8160ee00 <fdt_offset_ptr+0x1c>
8160ee48:	e0800003 	add	r0, r0, r3
8160ee4c:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

8160ee50 <fdt_next_tag>:
8160ee50:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8160ee54:	e3e03007 	mvn	r3, #7
8160ee58:	e1a07002 	mov	r7, r2
8160ee5c:	e5823000 	str	r3, [r2]
8160ee60:	e3a02004 	mov	r2, #4
8160ee64:	e1a08000 	mov	r8, r0
8160ee68:	e1a06001 	mov	r6, r1
8160ee6c:	ebffffdc 	bl	8160ede4 <fdt_offset_ptr>
8160ee70:	e3500000 	cmp	r0, #0
8160ee74:	1a000001 	bne	8160ee80 <fdt_next_tag+0x30>
8160ee78:	e3a05009 	mov	r5, #9
8160ee7c:	ea000025 	b	8160ef18 <fdt_next_tag+0xc8>
8160ee80:	e5905000 	ldr	r5, [r0]
8160ee84:	e3e0300a 	mvn	r3, #10
8160ee88:	e5873000 	str	r3, [r7]
8160ee8c:	e2864004 	add	r4, r6, #4
8160ee90:	e6bf5f35 	rev	r5, r5
8160ee94:	e2453001 	sub	r3, r5, #1
8160ee98:	e3530008 	cmp	r3, #8
8160ee9c:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
8160eea0:	eafffff4 	b	8160ee78 <fdt_next_tag+0x28>
8160eea4:	8160eec8 	.word	0x8160eec8
8160eea8:	8160eef4 	.word	0x8160eef4
8160eeac:	8160ef20 	.word	0x8160ef20
8160eeb0:	8160eef4 	.word	0x8160eef4
8160eeb4:	8160ee78 	.word	0x8160ee78
8160eeb8:	8160ee78 	.word	0x8160ee78
8160eebc:	8160ee78 	.word	0x8160ee78
8160eec0:	8160ee78 	.word	0x8160ee78
8160eec4:	8160eef4 	.word	0x8160eef4
8160eec8:	e3a02001 	mov	r2, #1
8160eecc:	e1a01004 	mov	r1, r4
8160eed0:	e1a00008 	mov	r0, r8
8160eed4:	e284a001 	add	sl, r4, #1
8160eed8:	ebffffc1 	bl	8160ede4 <fdt_offset_ptr>
8160eedc:	e3500000 	cmp	r0, #0
8160eee0:	0affffe4 	beq	8160ee78 <fdt_next_tag+0x28>
8160eee4:	e5d03000 	ldrb	r3, [r0]
8160eee8:	e1a0400a 	mov	r4, sl
8160eeec:	e3530000 	cmp	r3, #0
8160eef0:	1afffff4 	bne	8160eec8 <fdt_next_tag+0x78>
8160eef4:	e0442006 	sub	r2, r4, r6
8160eef8:	e1a01006 	mov	r1, r6
8160eefc:	e1a00008 	mov	r0, r8
8160ef00:	ebffffb7 	bl	8160ede4 <fdt_offset_ptr>
8160ef04:	e3500000 	cmp	r0, #0
8160ef08:	12844003 	addne	r4, r4, #3
8160ef0c:	13c44003 	bicne	r4, r4, #3
8160ef10:	15874000 	strne	r4, [r7]
8160ef14:	0affffd7 	beq	8160ee78 <fdt_next_tag+0x28>
8160ef18:	e1a00005 	mov	r0, r5
8160ef1c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160ef20:	e3a02004 	mov	r2, #4
8160ef24:	e1a01004 	mov	r1, r4
8160ef28:	e1a00008 	mov	r0, r8
8160ef2c:	ebffffac 	bl	8160ede4 <fdt_offset_ptr>
8160ef30:	e3500000 	cmp	r0, #0
8160ef34:	0affffcf 	beq	8160ee78 <fdt_next_tag+0x28>
8160ef38:	e5904000 	ldr	r4, [r0]
8160ef3c:	e6bf3f34 	rev	r3, r4
8160ef40:	e286400c 	add	r4, r6, #12
8160ef44:	e0844003 	add	r4, r4, r3
8160ef48:	eaffffe9 	b	8160eef4 <fdt_next_tag+0xa4>

8160ef4c <_fdt_check_node_offset>:
8160ef4c:	e2513000 	subs	r3, r1, #0
8160ef50:	e92d4007 	push	{r0, r1, r2, lr}
8160ef54:	e58d1004 	str	r1, [sp, #4]
8160ef58:	aa000002 	bge	8160ef68 <_fdt_check_node_offset+0x1c>
8160ef5c:	e3e00003 	mvn	r0, #3
8160ef60:	e28dd00c 	add	sp, sp, #12
8160ef64:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8160ef68:	e3130003 	tst	r3, #3
8160ef6c:	1afffffa 	bne	8160ef5c <_fdt_check_node_offset+0x10>
8160ef70:	e28d2004 	add	r2, sp, #4
8160ef74:	ebffffb5 	bl	8160ee50 <fdt_next_tag>
8160ef78:	e3500001 	cmp	r0, #1
8160ef7c:	1afffff6 	bne	8160ef5c <_fdt_check_node_offset+0x10>
8160ef80:	e59d0004 	ldr	r0, [sp, #4]
8160ef84:	eafffff5 	b	8160ef60 <_fdt_check_node_offset+0x14>

8160ef88 <_fdt_check_prop_offset>:
8160ef88:	e2513000 	subs	r3, r1, #0
8160ef8c:	e92d4007 	push	{r0, r1, r2, lr}
8160ef90:	e58d1004 	str	r1, [sp, #4]
8160ef94:	aa000002 	bge	8160efa4 <_fdt_check_prop_offset+0x1c>
8160ef98:	e3e00003 	mvn	r0, #3
8160ef9c:	e28dd00c 	add	sp, sp, #12
8160efa0:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8160efa4:	e3130003 	tst	r3, #3
8160efa8:	1afffffa 	bne	8160ef98 <_fdt_check_prop_offset+0x10>
8160efac:	e28d2004 	add	r2, sp, #4
8160efb0:	ebffffa6 	bl	8160ee50 <fdt_next_tag>
8160efb4:	e3500003 	cmp	r0, #3
8160efb8:	1afffff6 	bne	8160ef98 <_fdt_check_prop_offset+0x10>
8160efbc:	e59d0004 	ldr	r0, [sp, #4]
8160efc0:	eafffff5 	b	8160ef9c <_fdt_check_prop_offset+0x14>

8160efc4 <fdt_next_node>:
8160efc4:	e3a03000 	mov	r3, #0
8160efc8:	e1510003 	cmp	r1, r3
8160efcc:	e92d4073 	push	{r0, r1, r4, r5, r6, lr}
8160efd0:	e1a06000 	mov	r6, r0
8160efd4:	e1a05002 	mov	r5, r2
8160efd8:	e58d3004 	str	r3, [sp, #4]
8160efdc:	ba000004 	blt	8160eff4 <fdt_next_node+0x30>
8160efe0:	ebffffd9 	bl	8160ef4c <_fdt_check_node_offset>
8160efe4:	e3500000 	cmp	r0, #0
8160efe8:	e1a04000 	mov	r4, r0
8160efec:	e58d0004 	str	r0, [sp, #4]
8160eff0:	ba00000e 	blt	8160f030 <fdt_next_node+0x6c>
8160eff4:	e59d4004 	ldr	r4, [sp, #4]
8160eff8:	e28d2004 	add	r2, sp, #4
8160effc:	e1a00006 	mov	r0, r6
8160f000:	e1a01004 	mov	r1, r4
8160f004:	ebffff91 	bl	8160ee50 <fdt_next_tag>
8160f008:	e3500002 	cmp	r0, #2
8160f00c:	0a00000a 	beq	8160f03c <fdt_next_node+0x78>
8160f010:	e3500009 	cmp	r0, #9
8160f014:	0a000011 	beq	8160f060 <fdt_next_node+0x9c>
8160f018:	e3500001 	cmp	r0, #1
8160f01c:	1afffff4 	bne	8160eff4 <fdt_next_node+0x30>
8160f020:	e3550000 	cmp	r5, #0
8160f024:	15953000 	ldrne	r3, [r5]
8160f028:	12833001 	addne	r3, r3, #1
8160f02c:	15853000 	strne	r3, [r5]
8160f030:	e1a00004 	mov	r0, r4
8160f034:	e28dd008 	add	sp, sp, #8
8160f038:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f03c:	e3550000 	cmp	r5, #0
8160f040:	0affffeb 	beq	8160eff4 <fdt_next_node+0x30>
8160f044:	e5953000 	ldr	r3, [r5]
8160f048:	e2433001 	sub	r3, r3, #1
8160f04c:	e5853000 	str	r3, [r5]
8160f050:	e3530000 	cmp	r3, #0
8160f054:	aaffffe6 	bge	8160eff4 <fdt_next_node+0x30>
8160f058:	e59d4004 	ldr	r4, [sp, #4]
8160f05c:	eafffff3 	b	8160f030 <fdt_next_node+0x6c>
8160f060:	e59d4004 	ldr	r4, [sp, #4]
8160f064:	e3540000 	cmp	r4, #0
8160f068:	a3e04000 	mvnge	r4, #0
8160f06c:	aaffffef 	bge	8160f030 <fdt_next_node+0x6c>
8160f070:	e3550000 	cmp	r5, #0
8160f074:	03740008 	cmneq	r4, #8
8160f078:	03e04000 	mvneq	r4, #0
8160f07c:	eaffffeb 	b	8160f030 <fdt_next_node+0x6c>

8160f080 <_fdt_find_string>:
8160f080:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160f084:	e1a04000 	mov	r4, r0
8160f088:	e1a00002 	mov	r0, r2
8160f08c:	e1a05001 	mov	r5, r1
8160f090:	e1a07002 	mov	r7, r2
8160f094:	eb000a0e 	bl	816118d4 <strlen>
8160f098:	e2806001 	add	r6, r0, #1
8160f09c:	e0455006 	sub	r5, r5, r6
8160f0a0:	e0845005 	add	r5, r4, r5
8160f0a4:	e1540005 	cmp	r4, r5
8160f0a8:	9a000002 	bls	8160f0b8 <_fdt_find_string+0x38>
8160f0ac:	e3a04000 	mov	r4, #0
8160f0b0:	e1a00004 	mov	r0, r4
8160f0b4:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160f0b8:	e1a02006 	mov	r2, r6
8160f0bc:	e1a01007 	mov	r1, r7
8160f0c0:	e1a00004 	mov	r0, r4
8160f0c4:	eb000a49 	bl	816119f0 <memcmp>
8160f0c8:	e3500000 	cmp	r0, #0
8160f0cc:	0afffff7 	beq	8160f0b0 <_fdt_find_string+0x30>
8160f0d0:	e2844001 	add	r4, r4, #1
8160f0d4:	eafffff2 	b	8160f0a4 <_fdt_find_string+0x24>

8160f0d8 <fdt_move>:
8160f0d8:	e92d4070 	push	{r4, r5, r6, lr}
8160f0dc:	e1a05000 	mov	r5, r0
8160f0e0:	e1a06001 	mov	r6, r1
8160f0e4:	e1a01002 	mov	r1, r2
8160f0e8:	ebffff20 	bl	8160ed70 <fdt_check_header>
8160f0ec:	e2504000 	subs	r4, r0, #0
8160f0f0:	1a000007 	bne	8160f114 <fdt_move+0x3c>
8160f0f4:	e5953004 	ldr	r3, [r5, #4]
8160f0f8:	e6bf2f33 	rev	r2, r3
8160f0fc:	e1510002 	cmp	r1, r2
8160f100:	33e04002 	mvncc	r4, #2
8160f104:	3a000002 	bcc	8160f114 <fdt_move+0x3c>
8160f108:	e1a01005 	mov	r1, r5
8160f10c:	e1a00006 	mov	r0, r6
8160f110:	eb000a26 	bl	816119b0 <memmove>
8160f114:	e1a00004 	mov	r0, r4
8160f118:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f11c <fdt_path_next_separator>:
8160f11c:	e92d4070 	push	{r4, r5, r6, lr}
8160f120:	e1a02001 	mov	r2, r1
8160f124:	e1a05001 	mov	r5, r1
8160f128:	e3a0102f 	mov	r1, #47	; 0x2f
8160f12c:	e1a06000 	mov	r6, r0
8160f130:	eb000a57 	bl	81611a94 <memchr>
8160f134:	e1a02005 	mov	r2, r5
8160f138:	e3a0103a 	mov	r1, #58	; 0x3a
8160f13c:	e1a04000 	mov	r4, r0
8160f140:	e1a00006 	mov	r0, r6
8160f144:	eb000a52 	bl	81611a94 <memchr>
8160f148:	e3540000 	cmp	r4, #0
8160f14c:	13500000 	cmpne	r0, #0
8160f150:	0a000002 	beq	8160f160 <fdt_path_next_separator+0x44>
8160f154:	e1500004 	cmp	r0, r4
8160f158:	21a00004 	movcs	r0, r4
8160f15c:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f160:	e3540000 	cmp	r4, #0
8160f164:	11a00004 	movne	r0, r4
8160f168:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f16c <_nextprop>:
8160f16c:	e92d4037 	push	{r0, r1, r2, r4, r5, lr}
8160f170:	e1a05000 	mov	r5, r0
8160f174:	e1a04001 	mov	r4, r1
8160f178:	e28d2004 	add	r2, sp, #4
8160f17c:	e1a01004 	mov	r1, r4
8160f180:	e1a00005 	mov	r0, r5
8160f184:	ebffff31 	bl	8160ee50 <fdt_next_tag>
8160f188:	e3500003 	cmp	r0, #3
8160f18c:	0a000004 	beq	8160f1a4 <_nextprop+0x38>
8160f190:	e3500009 	cmp	r0, #9
8160f194:	e59d4004 	ldr	r4, [sp, #4]
8160f198:	1a000004 	bne	8160f1b0 <_nextprop+0x44>
8160f19c:	e3540000 	cmp	r4, #0
8160f1a0:	a3e0400a 	mvnge	r4, #10
8160f1a4:	e1a00004 	mov	r0, r4
8160f1a8:	e28dd00c 	add	sp, sp, #12
8160f1ac:	e8bd8030 	pop	{r4, r5, pc}
8160f1b0:	e3500004 	cmp	r0, #4
8160f1b4:	0affffef 	beq	8160f178 <_nextprop+0xc>
8160f1b8:	e3e04000 	mvn	r4, #0
8160f1bc:	eafffff8 	b	8160f1a4 <_nextprop+0x38>

8160f1c0 <fdt_string>:
8160f1c0:	e590300c 	ldr	r3, [r0, #12]
8160f1c4:	e6bf3f33 	rev	r3, r3
8160f1c8:	e0811003 	add	r1, r1, r3
8160f1cc:	e0800001 	add	r0, r0, r1
8160f1d0:	e12fff1e 	bx	lr

8160f1d4 <fdt_get_mem_rsv>:
8160f1d4:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160f1d8:	e1a05000 	mov	r5, r0
8160f1dc:	e1a07001 	mov	r7, r1
8160f1e0:	e1a06002 	mov	r6, r2
8160f1e4:	e1a04003 	mov	r4, r3
8160f1e8:	ebfffee0 	bl	8160ed70 <fdt_check_header>
8160f1ec:	e3500000 	cmp	r0, #0
8160f1f0:	18bd81f0 	popne	{r4, r5, r6, r7, r8, pc}
8160f1f4:	e595c010 	ldr	ip, [r5, #16]
8160f1f8:	e6bfcf3c 	rev	ip, ip
8160f1fc:	e08c1207 	add	r1, ip, r7, lsl #4
8160f200:	e085c001 	add	ip, r5, r1
8160f204:	e7951001 	ldr	r1, [r5, r1]
8160f208:	e59c3004 	ldr	r3, [ip, #4]
8160f20c:	e6bf1f31 	rev	r1, r1
8160f210:	e5861004 	str	r1, [r6, #4]
8160f214:	e6bf3f33 	rev	r3, r3
8160f218:	e5863000 	str	r3, [r6]
8160f21c:	e59c2008 	ldr	r2, [ip, #8]
8160f220:	e59c100c 	ldr	r1, [ip, #12]
8160f224:	e6bf2f32 	rev	r2, r2
8160f228:	e6bf1f31 	rev	r1, r1
8160f22c:	e8840006 	stm	r4, {r1, r2}
8160f230:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

8160f234 <fdt_num_mem_rsv>:
8160f234:	e5901010 	ldr	r1, [r0, #16]
8160f238:	e3a03000 	mov	r3, #0
8160f23c:	e92d4030 	push	{r4, r5, lr}
8160f240:	e6bf1f31 	rev	r1, r1
8160f244:	e0812203 	add	r2, r1, r3, lsl #4
8160f248:	e0802002 	add	r2, r0, r2
8160f24c:	e592c008 	ldr	ip, [r2, #8]
8160f250:	e592200c 	ldr	r2, [r2, #12]
8160f254:	e6bf5f3c 	rev	r5, ip
8160f258:	e6bf4f32 	rev	r4, r2
8160f25c:	e1942005 	orrs	r2, r4, r5
8160f260:	1a000001 	bne	8160f26c <fdt_num_mem_rsv+0x38>
8160f264:	e1a00003 	mov	r0, r3
8160f268:	e8bd8030 	pop	{r4, r5, pc}
8160f26c:	e2833001 	add	r3, r3, #1
8160f270:	eafffff3 	b	8160f244 <fdt_num_mem_rsv+0x10>

8160f274 <fdt_subnode_offset_namelen>:
8160f274:	e92d45f7 	push	{r0, r1, r2, r4, r5, r6, r7, r8, sl, lr}
8160f278:	e1a08001 	mov	r8, r1
8160f27c:	e1a05003 	mov	r5, r3
8160f280:	e1a06000 	mov	r6, r0
8160f284:	e1a07002 	mov	r7, r2
8160f288:	ebfffeb8 	bl	8160ed70 <fdt_check_header>
8160f28c:	e2504000 	subs	r4, r0, #0
8160f290:	058d4004 	streq	r4, [sp, #4]
8160f294:	0285a001 	addeq	sl, r5, #1
8160f298:	01a04008 	moveq	r4, r8
8160f29c:	0a000009 	beq	8160f2c8 <fdt_subnode_offset_namelen+0x54>
8160f2a0:	e1a00004 	mov	r0, r4
8160f2a4:	e28dd00c 	add	sp, sp, #12
8160f2a8:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
8160f2ac:	e3530001 	cmp	r3, #1
8160f2b0:	0a00000b 	beq	8160f2e4 <fdt_subnode_offset_namelen+0x70>
8160f2b4:	e1a01004 	mov	r1, r4
8160f2b8:	e28d2004 	add	r2, sp, #4
8160f2bc:	e1a00006 	mov	r0, r6
8160f2c0:	ebffff3f 	bl	8160efc4 <fdt_next_node>
8160f2c4:	e1a04000 	mov	r4, r0
8160f2c8:	e59d3004 	ldr	r3, [sp, #4]
8160f2cc:	e3540000 	cmp	r4, #0
8160f2d0:	a3530000 	cmpge	r3, #0
8160f2d4:	aafffff4 	bge	8160f2ac <fdt_subnode_offset_namelen+0x38>
8160f2d8:	e3530000 	cmp	r3, #0
8160f2dc:	b3e04000 	mvnlt	r4, #0
8160f2e0:	eaffffee 	b	8160f2a0 <fdt_subnode_offset_namelen+0x2c>
8160f2e4:	e1a0200a 	mov	r2, sl
8160f2e8:	e2841004 	add	r1, r4, #4
8160f2ec:	e1a00006 	mov	r0, r6
8160f2f0:	ebfffebb 	bl	8160ede4 <fdt_offset_ptr>
8160f2f4:	e2508000 	subs	r8, r0, #0
8160f2f8:	0affffed 	beq	8160f2b4 <fdt_subnode_offset_namelen+0x40>
8160f2fc:	e1a02005 	mov	r2, r5
8160f300:	e1a01007 	mov	r1, r7
8160f304:	eb0009b9 	bl	816119f0 <memcmp>
8160f308:	e3500000 	cmp	r0, #0
8160f30c:	1affffe8 	bne	8160f2b4 <fdt_subnode_offset_namelen+0x40>
8160f310:	e7d83005 	ldrb	r3, [r8, r5]
8160f314:	e3530000 	cmp	r3, #0
8160f318:	0affffe0 	beq	8160f2a0 <fdt_subnode_offset_namelen+0x2c>
8160f31c:	e1a02005 	mov	r2, r5
8160f320:	e3a01040 	mov	r1, #64	; 0x40
8160f324:	e1a00007 	mov	r0, r7
8160f328:	eb0009d9 	bl	81611a94 <memchr>
8160f32c:	e3500000 	cmp	r0, #0
8160f330:	1affffdf 	bne	8160f2b4 <fdt_subnode_offset_namelen+0x40>
8160f334:	e7d83005 	ldrb	r3, [r8, r5]
8160f338:	e3530040 	cmp	r3, #64	; 0x40
8160f33c:	1affffdc 	bne	8160f2b4 <fdt_subnode_offset_namelen+0x40>
8160f340:	eaffffd6 	b	8160f2a0 <fdt_subnode_offset_namelen+0x2c>

8160f344 <fdt_subnode_offset>:
8160f344:	e92d4070 	push	{r4, r5, r6, lr}
8160f348:	e1a05000 	mov	r5, r0
8160f34c:	e1a00002 	mov	r0, r2
8160f350:	e1a06001 	mov	r6, r1
8160f354:	e1a04002 	mov	r4, r2
8160f358:	eb00095d 	bl	816118d4 <strlen>
8160f35c:	e1a02004 	mov	r2, r4
8160f360:	e1a01006 	mov	r1, r6
8160f364:	e1a03000 	mov	r3, r0
8160f368:	e1a00005 	mov	r0, r5
8160f36c:	e8bd4070 	pop	{r4, r5, r6, lr}
8160f370:	eaffffbf 	b	8160f274 <fdt_subnode_offset_namelen>

8160f374 <fdt_get_name>:
8160f374:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160f378:	e1a04000 	mov	r4, r0
8160f37c:	e1a06001 	mov	r6, r1
8160f380:	e1a05002 	mov	r5, r2
8160f384:	e5907008 	ldr	r7, [r0, #8]
8160f388:	ebfffe78 	bl	8160ed70 <fdt_check_header>
8160f38c:	e3500000 	cmp	r0, #0
8160f390:	1a00000f 	bne	8160f3d4 <fdt_get_name+0x60>
8160f394:	e1a01006 	mov	r1, r6
8160f398:	e1a00004 	mov	r0, r4
8160f39c:	ebfffeea 	bl	8160ef4c <_fdt_check_node_offset>
8160f3a0:	e3500000 	cmp	r0, #0
8160f3a4:	ba00000a 	blt	8160f3d4 <fdt_get_name+0x60>
8160f3a8:	e6bf7f37 	rev	r7, r7
8160f3ac:	e3550000 	cmp	r5, #0
8160f3b0:	e0866007 	add	r6, r6, r7
8160f3b4:	e0844006 	add	r4, r4, r6
8160f3b8:	e2844004 	add	r4, r4, #4
8160f3bc:	0a000002 	beq	8160f3cc <fdt_get_name+0x58>
8160f3c0:	e1a00004 	mov	r0, r4
8160f3c4:	eb000942 	bl	816118d4 <strlen>
8160f3c8:	e5850000 	str	r0, [r5]
8160f3cc:	e1a00004 	mov	r0, r4
8160f3d0:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160f3d4:	e3550000 	cmp	r5, #0
8160f3d8:	15850000 	strne	r0, [r5]
8160f3dc:	13a04000 	movne	r4, #0
8160f3e0:	01a04005 	moveq	r4, r5
8160f3e4:	eafffff8 	b	8160f3cc <fdt_get_name+0x58>

8160f3e8 <fdt_first_property_offset>:
8160f3e8:	e92d4010 	push	{r4, lr}
8160f3ec:	e1a04000 	mov	r4, r0
8160f3f0:	ebfffed5 	bl	8160ef4c <_fdt_check_node_offset>
8160f3f4:	e2501000 	subs	r1, r0, #0
8160f3f8:	b8bd8010 	poplt	{r4, pc}
8160f3fc:	e1a00004 	mov	r0, r4
8160f400:	e8bd4010 	pop	{r4, lr}
8160f404:	eaffff58 	b	8160f16c <_nextprop>

8160f408 <fdt_next_property_offset>:
8160f408:	e92d4010 	push	{r4, lr}
8160f40c:	e1a04000 	mov	r4, r0
8160f410:	ebfffedc 	bl	8160ef88 <_fdt_check_prop_offset>
8160f414:	e2501000 	subs	r1, r0, #0
8160f418:	b8bd8010 	poplt	{r4, pc}
8160f41c:	e1a00004 	mov	r0, r4
8160f420:	e8bd4010 	pop	{r4, lr}
8160f424:	eaffff50 	b	8160f16c <_nextprop>

8160f428 <fdt_get_property_by_offset>:
8160f428:	e92d4070 	push	{r4, r5, r6, lr}
8160f42c:	e1a04000 	mov	r4, r0
8160f430:	e1a06001 	mov	r6, r1
8160f434:	e1a05002 	mov	r5, r2
8160f438:	ebfffed2 	bl	8160ef88 <_fdt_check_prop_offset>
8160f43c:	e3500000 	cmp	r0, #0
8160f440:	aa000003 	bge	8160f454 <fdt_get_property_by_offset+0x2c>
8160f444:	e3550000 	cmp	r5, #0
8160f448:	15850000 	strne	r0, [r5]
8160f44c:	e3a00000 	mov	r0, #0
8160f450:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f454:	e5940008 	ldr	r0, [r4, #8]
8160f458:	e3550000 	cmp	r5, #0
8160f45c:	e6bf0f30 	rev	r0, r0
8160f460:	e0860000 	add	r0, r6, r0
8160f464:	e0840000 	add	r0, r4, r0
8160f468:	15903004 	ldrne	r3, [r0, #4]
8160f46c:	16bf3f33 	revne	r3, r3
8160f470:	15853000 	strne	r3, [r5]
8160f474:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f478 <fdt_get_property_namelen>:
8160f478:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160f47c:	e1a08003 	mov	r8, r3
8160f480:	e1a07000 	mov	r7, r0
8160f484:	e1a0a002 	mov	sl, r2
8160f488:	e59d6028 	ldr	r6, [sp, #40]	; 0x28
8160f48c:	ebffffd5 	bl	8160f3e8 <fdt_first_property_offset>
8160f490:	e288b001 	add	fp, r8, #1
8160f494:	e1a04000 	mov	r4, r0
8160f498:	e3540000 	cmp	r4, #0
8160f49c:	aa000004 	bge	8160f4b4 <fdt_get_property_namelen+0x3c>
8160f4a0:	e3560000 	cmp	r6, #0
8160f4a4:	15864000 	strne	r4, [r6]
8160f4a8:	13a05000 	movne	r5, #0
8160f4ac:	01a05006 	moveq	r5, r6
8160f4b0:	ea00001a 	b	8160f520 <fdt_get_property_namelen+0xa8>
8160f4b4:	e1a02006 	mov	r2, r6
8160f4b8:	e1a01004 	mov	r1, r4
8160f4bc:	e1a00007 	mov	r0, r7
8160f4c0:	ebffffd8 	bl	8160f428 <fdt_get_property_by_offset>
8160f4c4:	e2505000 	subs	r5, r0, #0
8160f4c8:	0a000017 	beq	8160f52c <fdt_get_property_namelen+0xb4>
8160f4cc:	e5951008 	ldr	r1, [r5, #8]
8160f4d0:	e1a00007 	mov	r0, r7
8160f4d4:	e6bf1f31 	rev	r1, r1
8160f4d8:	ebffff38 	bl	8160f1c0 <fdt_string>
8160f4dc:	e1a0100b 	mov	r1, fp
8160f4e0:	e58d0004 	str	r0, [sp, #4]
8160f4e4:	eb000903 	bl	816118f8 <strnlen>
8160f4e8:	e59d3004 	ldr	r3, [sp, #4]
8160f4ec:	e1500008 	cmp	r0, r8
8160f4f0:	0a000004 	beq	8160f508 <fdt_get_property_namelen+0x90>
8160f4f4:	e1a01004 	mov	r1, r4
8160f4f8:	e1a00007 	mov	r0, r7
8160f4fc:	ebffffc1 	bl	8160f408 <fdt_next_property_offset>
8160f500:	e1a04000 	mov	r4, r0
8160f504:	eaffffe3 	b	8160f498 <fdt_get_property_namelen+0x20>
8160f508:	e1a02008 	mov	r2, r8
8160f50c:	e1a0100a 	mov	r1, sl
8160f510:	e1a00003 	mov	r0, r3
8160f514:	eb000935 	bl	816119f0 <memcmp>
8160f518:	e3500000 	cmp	r0, #0
8160f51c:	1afffff4 	bne	8160f4f4 <fdt_get_property_namelen+0x7c>
8160f520:	e1a00005 	mov	r0, r5
8160f524:	e28dd008 	add	sp, sp, #8
8160f528:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
8160f52c:	e3e0400c 	mvn	r4, #12
8160f530:	eaffffda 	b	8160f4a0 <fdt_get_property_namelen+0x28>

8160f534 <fdt_get_property>:
8160f534:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
8160f538:	e1a05000 	mov	r5, r0
8160f53c:	e1a00002 	mov	r0, r2
8160f540:	e1a07003 	mov	r7, r3
8160f544:	e1a06001 	mov	r6, r1
8160f548:	e1a04002 	mov	r4, r2
8160f54c:	eb0008e0 	bl	816118d4 <strlen>
8160f550:	e58d7000 	str	r7, [sp]
8160f554:	e1a02004 	mov	r2, r4
8160f558:	e1a01006 	mov	r1, r6
8160f55c:	e1a03000 	mov	r3, r0
8160f560:	e1a00005 	mov	r0, r5
8160f564:	ebffffc3 	bl	8160f478 <fdt_get_property_namelen>
8160f568:	e28dd00c 	add	sp, sp, #12
8160f56c:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}

8160f570 <fdt_getprop_namelen>:
8160f570:	e92d4007 	push	{r0, r1, r2, lr}
8160f574:	e59dc010 	ldr	ip, [sp, #16]
8160f578:	e58dc000 	str	ip, [sp]
8160f57c:	ebffffbd 	bl	8160f478 <fdt_get_property_namelen>
8160f580:	e3500000 	cmp	r0, #0
8160f584:	1280000c 	addne	r0, r0, #12
8160f588:	e28dd00c 	add	sp, sp, #12
8160f58c:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

8160f590 <fdt_getprop_by_offset>:
8160f590:	e92d4070 	push	{r4, r5, r6, lr}
8160f594:	e1a04002 	mov	r4, r2
8160f598:	e1a02003 	mov	r2, r3
8160f59c:	e1a05000 	mov	r5, r0
8160f5a0:	ebffffa0 	bl	8160f428 <fdt_get_property_by_offset>
8160f5a4:	e2502000 	subs	r2, r0, #0
8160f5a8:	0a000008 	beq	8160f5d0 <fdt_getprop_by_offset+0x40>
8160f5ac:	e3540000 	cmp	r4, #0
8160f5b0:	0a000004 	beq	8160f5c8 <fdt_getprop_by_offset+0x38>
8160f5b4:	e5921008 	ldr	r1, [r2, #8]
8160f5b8:	e1a00005 	mov	r0, r5
8160f5bc:	e6bf1f31 	rev	r1, r1
8160f5c0:	ebfffefe 	bl	8160f1c0 <fdt_string>
8160f5c4:	e5840000 	str	r0, [r4]
8160f5c8:	e282000c 	add	r0, r2, #12
8160f5cc:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f5d0:	e1a00002 	mov	r0, r2
8160f5d4:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f5d8 <fdt_getprop>:
8160f5d8:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
8160f5dc:	e1a05000 	mov	r5, r0
8160f5e0:	e1a00002 	mov	r0, r2
8160f5e4:	e1a07003 	mov	r7, r3
8160f5e8:	e1a06001 	mov	r6, r1
8160f5ec:	e1a04002 	mov	r4, r2
8160f5f0:	eb0008b7 	bl	816118d4 <strlen>
8160f5f4:	e58d7000 	str	r7, [sp]
8160f5f8:	e1a02004 	mov	r2, r4
8160f5fc:	e1a01006 	mov	r1, r6
8160f600:	e1a03000 	mov	r3, r0
8160f604:	e1a00005 	mov	r0, r5
8160f608:	ebffffd8 	bl	8160f570 <fdt_getprop_namelen>
8160f60c:	e28dd00c 	add	sp, sp, #12
8160f610:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}

8160f614 <fdt_get_alias_namelen>:
8160f614:	e92d4073 	push	{r0, r1, r4, r5, r6, lr}
8160f618:	e1a05001 	mov	r5, r1
8160f61c:	e59f1030 	ldr	r1, [pc, #48]	; 8160f654 <fdt_get_alias_namelen+0x40>
8160f620:	e1a04000 	mov	r4, r0
8160f624:	e1a06002 	mov	r6, r2
8160f628:	eb000043 	bl	8160f73c <fdt_path_offset>
8160f62c:	e2501000 	subs	r1, r0, #0
8160f630:	e3a00000 	mov	r0, #0
8160f634:	ba000004 	blt	8160f64c <fdt_get_alias_namelen+0x38>
8160f638:	e58d0000 	str	r0, [sp]
8160f63c:	e1a03006 	mov	r3, r6
8160f640:	e1a02005 	mov	r2, r5
8160f644:	e1a00004 	mov	r0, r4
8160f648:	ebffffc8 	bl	8160f570 <fdt_getprop_namelen>
8160f64c:	e28dd008 	add	sp, sp, #8
8160f650:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f654:	81615c10 	.word	0x81615c10

8160f658 <fdt_path_offset_namelen>:
8160f658:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160f65c:	e1a08000 	mov	r8, r0
8160f660:	e1a04001 	mov	r4, r1
8160f664:	e1a05002 	mov	r5, r2
8160f668:	ebfffdc0 	bl	8160ed70 <fdt_check_header>
8160f66c:	e2506000 	subs	r6, r0, #0
8160f670:	1a00002f 	bne	8160f734 <fdt_path_offset_namelen+0xdc>
8160f674:	e5d43000 	ldrb	r3, [r4]
8160f678:	e0847005 	add	r7, r4, r5
8160f67c:	e353002f 	cmp	r3, #47	; 0x2f
8160f680:	0a000010 	beq	8160f6c8 <fdt_path_offset_namelen+0x70>
8160f684:	e1a01005 	mov	r1, r5
8160f688:	e1a00004 	mov	r0, r4
8160f68c:	ebfffea2 	bl	8160f11c <fdt_path_next_separator>
8160f690:	e1a01004 	mov	r1, r4
8160f694:	e3500000 	cmp	r0, #0
8160f698:	11a05000 	movne	r5, r0
8160f69c:	01a05007 	moveq	r5, r7
8160f6a0:	e1a00008 	mov	r0, r8
8160f6a4:	e0452004 	sub	r2, r5, r4
8160f6a8:	ebffffd9 	bl	8160f614 <fdt_get_alias_namelen>
8160f6ac:	e2501000 	subs	r1, r0, #0
8160f6b0:	03e06004 	mvneq	r6, #4
8160f6b4:	0a00001e 	beq	8160f734 <fdt_path_offset_namelen+0xdc>
8160f6b8:	e1a00008 	mov	r0, r8
8160f6bc:	e1a04005 	mov	r4, r5
8160f6c0:	eb00001d 	bl	8160f73c <fdt_path_offset>
8160f6c4:	e1a06000 	mov	r6, r0
8160f6c8:	e5d43000 	ldrb	r3, [r4]
8160f6cc:	e3530000 	cmp	r3, #0
8160f6d0:	11540007 	cmpne	r4, r7
8160f6d4:	2a000016 	bcs	8160f734 <fdt_path_offset_namelen+0xdc>
8160f6d8:	e1a05004 	mov	r5, r4
8160f6dc:	ea000000 	b	8160f6e4 <fdt_path_offset_namelen+0x8c>
8160f6e0:	e1a05002 	mov	r5, r2
8160f6e4:	e1a02005 	mov	r2, r5
8160f6e8:	e4d23001 	ldrb	r3, [r2], #1
8160f6ec:	e353002f 	cmp	r3, #47	; 0x2f
8160f6f0:	0afffffa 	beq	8160f6e0 <fdt_path_offset_namelen+0x88>
8160f6f4:	e353003a 	cmp	r3, #58	; 0x3a
8160f6f8:	13530000 	cmpne	r3, #0
8160f6fc:	0a00000c 	beq	8160f734 <fdt_path_offset_namelen+0xdc>
8160f700:	e0471005 	sub	r1, r7, r5
8160f704:	e1a00005 	mov	r0, r5
8160f708:	ebfffe83 	bl	8160f11c <fdt_path_next_separator>
8160f70c:	e1a01006 	mov	r1, r6
8160f710:	e1a02005 	mov	r2, r5
8160f714:	e3500000 	cmp	r0, #0
8160f718:	11a04000 	movne	r4, r0
8160f71c:	01a04007 	moveq	r4, r7
8160f720:	e1a00008 	mov	r0, r8
8160f724:	e0443005 	sub	r3, r4, r5
8160f728:	ebfffed1 	bl	8160f274 <fdt_subnode_offset_namelen>
8160f72c:	e2506000 	subs	r6, r0, #0
8160f730:	aaffffe4 	bge	8160f6c8 <fdt_path_offset_namelen+0x70>
8160f734:	e1a00006 	mov	r0, r6
8160f738:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

8160f73c <fdt_path_offset>:
8160f73c:	e92d4070 	push	{r4, r5, r6, lr}
8160f740:	e1a05000 	mov	r5, r0
8160f744:	e1a00001 	mov	r0, r1
8160f748:	e1a04001 	mov	r4, r1
8160f74c:	eb000860 	bl	816118d4 <strlen>
8160f750:	e1a01004 	mov	r1, r4
8160f754:	e1a02000 	mov	r2, r0
8160f758:	e1a00005 	mov	r0, r5
8160f75c:	e8bd4070 	pop	{r4, r5, r6, lr}
8160f760:	eaffffbc 	b	8160f658 <fdt_path_offset_namelen>

8160f764 <fdt_supernode_atdepth_offset>:
8160f764:	e92d45f7 	push	{r0, r1, r2, r4, r5, r6, r7, r8, sl, lr}
8160f768:	e1a0a000 	mov	sl, r0
8160f76c:	e1a07001 	mov	r7, r1
8160f770:	e1a05002 	mov	r5, r2
8160f774:	e1a08003 	mov	r8, r3
8160f778:	ebfffd7c 	bl	8160ed70 <fdt_check_header>
8160f77c:	e3500000 	cmp	r0, #0
8160f780:	1a000010 	bne	8160f7c8 <fdt_supernode_atdepth_offset+0x64>
8160f784:	e3550000 	cmp	r5, #0
8160f788:	a28d6008 	addge	r6, sp, #8
8160f78c:	a3e0400c 	mvnge	r4, #12
8160f790:	a5260004 	strge	r0, [r6, #-4]!
8160f794:	aa000011 	bge	8160f7e0 <fdt_supernode_atdepth_offset+0x7c>
8160f798:	e3e00000 	mvn	r0, #0
8160f79c:	ea000009 	b	8160f7c8 <fdt_supernode_atdepth_offset+0x64>
8160f7a0:	e59d3004 	ldr	r3, [sp, #4]
8160f7a4:	e1550003 	cmp	r5, r3
8160f7a8:	01a04000 	moveq	r4, r0
8160f7ac:	e1570000 	cmp	r7, r0
8160f7b0:	1a000006 	bne	8160f7d0 <fdt_supernode_atdepth_offset+0x6c>
8160f7b4:	e3580000 	cmp	r8, #0
8160f7b8:	15883000 	strne	r3, [r8]
8160f7bc:	e1550003 	cmp	r5, r3
8160f7c0:	c3e04000 	mvngt	r4, #0
8160f7c4:	e1a00004 	mov	r0, r4
8160f7c8:	e28dd00c 	add	sp, sp, #12
8160f7cc:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
8160f7d0:	e1a01000 	mov	r1, r0
8160f7d4:	e1a02006 	mov	r2, r6
8160f7d8:	e1a0000a 	mov	r0, sl
8160f7dc:	ebfffdf8 	bl	8160efc4 <fdt_next_node>
8160f7e0:	e3500000 	cmp	r0, #0
8160f7e4:	a1570000 	cmpge	r7, r0
8160f7e8:	aaffffec 	bge	8160f7a0 <fdt_supernode_atdepth_offset+0x3c>
8160f7ec:	e3700001 	cmn	r0, #1
8160f7f0:	a3e04003 	mvnge	r4, #3
8160f7f4:	aafffff2 	bge	8160f7c4 <fdt_supernode_atdepth_offset+0x60>
8160f7f8:	e3700004 	cmn	r0, #4
8160f7fc:	11a04000 	movne	r4, r0
8160f800:	03e0400a 	mvneq	r4, #10
8160f804:	eaffffee 	b	8160f7c4 <fdt_supernode_atdepth_offset+0x60>

8160f808 <fdt_node_depth>:
8160f808:	e92d4007 	push	{r0, r1, r2, lr}
8160f80c:	e3a02000 	mov	r2, #0
8160f810:	e28d3004 	add	r3, sp, #4
8160f814:	ebffffd2 	bl	8160f764 <fdt_supernode_atdepth_offset>
8160f818:	e3500000 	cmp	r0, #0
8160f81c:	059d0004 	ldreq	r0, [sp, #4]
8160f820:	0a000000 	beq	8160f828 <fdt_node_depth+0x20>
8160f824:	a3e0000c 	mvnge	r0, #12
8160f828:	e28dd00c 	add	sp, sp, #12
8160f82c:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

8160f830 <_fdt_blocks_misordered>:
8160f830:	e5903010 	ldr	r3, [r0, #16]
8160f834:	e6bf3f33 	rev	r3, r3
8160f838:	e3530027 	cmp	r3, #39	; 0x27
8160f83c:	9a000012 	bls	8160f88c <_fdt_blocks_misordered+0x5c>
8160f840:	e590c008 	ldr	ip, [r0, #8]
8160f844:	e0811003 	add	r1, r1, r3
8160f848:	e6bfcf3c 	rev	ip, ip
8160f84c:	e151000c 	cmp	r1, ip
8160f850:	8a00000d 	bhi	8160f88c <_fdt_blocks_misordered+0x5c>
8160f854:	e590300c 	ldr	r3, [r0, #12]
8160f858:	e082c00c 	add	ip, r2, ip
8160f85c:	e6bf3f33 	rev	r3, r3
8160f860:	e15c0003 	cmp	ip, r3
8160f864:	8a000008 	bhi	8160f88c <_fdt_blocks_misordered+0x5c>
8160f868:	e5902004 	ldr	r2, [r0, #4]
8160f86c:	e5900020 	ldr	r0, [r0, #32]
8160f870:	e6bf2f32 	rev	r2, r2
8160f874:	e6bf0f30 	rev	r0, r0
8160f878:	e0830000 	add	r0, r3, r0
8160f87c:	e1500002 	cmp	r0, r2
8160f880:	93a00000 	movls	r0, #0
8160f884:	83a00001 	movhi	r0, #1
8160f888:	e12fff1e 	bx	lr
8160f88c:	e3a00001 	mov	r0, #1
8160f890:	e12fff1e 	bx	lr

8160f894 <_fdt_splice>:
8160f894:	e92d4070 	push	{r4, r5, r6, lr}
8160f898:	e3520000 	cmp	r2, #0
8160f89c:	e590c00c 	ldr	ip, [r0, #12]
8160f8a0:	e590e020 	ldr	lr, [r0, #32]
8160f8a4:	e6bfcf3c 	rev	ip, ip
8160f8a8:	e6bfef3e 	rev	lr, lr
8160f8ac:	e08cc00e 	add	ip, ip, lr
8160f8b0:	e080c00c 	add	ip, r0, ip
8160f8b4:	ba000018 	blt	8160f91c <_fdt_splice+0x88>
8160f8b8:	e1510000 	cmp	r1, r0
8160f8bc:	e0815002 	add	r5, r1, r2
8160f8c0:	23a04000 	movcs	r4, #0
8160f8c4:	33a04001 	movcc	r4, #1
8160f8c8:	e155000c 	cmp	r5, ip
8160f8cc:	83844001 	orrhi	r4, r4, #1
8160f8d0:	e3540000 	cmp	r4, #0
8160f8d4:	1a000010 	bne	8160f91c <_fdt_splice+0x88>
8160f8d8:	e0436002 	sub	r6, r3, r2
8160f8dc:	e08c6006 	add	r6, ip, r6
8160f8e0:	e1560000 	cmp	r6, r0
8160f8e4:	3a00000c 	bcc	8160f91c <_fdt_splice+0x88>
8160f8e8:	e590e004 	ldr	lr, [r0, #4]
8160f8ec:	e6bfef3e 	rev	lr, lr
8160f8f0:	e080e00e 	add	lr, r0, lr
8160f8f4:	e156000e 	cmp	r6, lr
8160f8f8:	8a000009 	bhi	8160f924 <_fdt_splice+0x90>
8160f8fc:	e04cc001 	sub	ip, ip, r1
8160f900:	e1a00001 	mov	r0, r1
8160f904:	e04c2002 	sub	r2, ip, r2
8160f908:	e1a01005 	mov	r1, r5
8160f90c:	e0800003 	add	r0, r0, r3
8160f910:	eb000826 	bl	816119b0 <memmove>
8160f914:	e1a00004 	mov	r0, r4
8160f918:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f91c:	e3e00003 	mvn	r0, #3
8160f920:	e8bd8070 	pop	{r4, r5, r6, pc}
8160f924:	e3e00002 	mvn	r0, #2
8160f928:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f92c <_fdt_splice_mem_rsv>:
8160f92c:	e92d4070 	push	{r4, r5, r6, lr}
8160f930:	e1a06002 	mov	r6, r2
8160f934:	e1a04003 	mov	r4, r3
8160f938:	e1a02202 	lsl	r2, r2, #4
8160f93c:	e1a03203 	lsl	r3, r3, #4
8160f940:	e1a05000 	mov	r5, r0
8160f944:	ebffffd2 	bl	8160f894 <_fdt_splice>
8160f948:	e3500000 	cmp	r0, #0
8160f94c:	18bd8070 	popne	{r4, r5, r6, pc}
8160f950:	e5953008 	ldr	r3, [r5, #8]
8160f954:	e0444006 	sub	r4, r4, r6
8160f958:	e6bf3f33 	rev	r3, r3
8160f95c:	e1a04204 	lsl	r4, r4, #4
8160f960:	e0843003 	add	r3, r4, r3
8160f964:	e6bf3f33 	rev	r3, r3
8160f968:	e5853008 	str	r3, [r5, #8]
8160f96c:	e595300c 	ldr	r3, [r5, #12]
8160f970:	e6bf3f33 	rev	r3, r3
8160f974:	e0844003 	add	r4, r4, r3
8160f978:	e6bf4f34 	rev	r4, r4
8160f97c:	e585400c 	str	r4, [r5, #12]
8160f980:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f984 <_fdt_splice_struct>:
8160f984:	e92d4070 	push	{r4, r5, r6, lr}
8160f988:	e1a05000 	mov	r5, r0
8160f98c:	e1a06002 	mov	r6, r2
8160f990:	e1a04003 	mov	r4, r3
8160f994:	ebffffbe 	bl	8160f894 <_fdt_splice>
8160f998:	e3500000 	cmp	r0, #0
8160f99c:	18bd8070 	popne	{r4, r5, r6, pc}
8160f9a0:	e5952024 	ldr	r2, [r5, #36]	; 0x24
8160f9a4:	e0443006 	sub	r3, r4, r6
8160f9a8:	e6bf2f32 	rev	r2, r2
8160f9ac:	e0832002 	add	r2, r3, r2
8160f9b0:	e6bf2f32 	rev	r2, r2
8160f9b4:	e5852024 	str	r2, [r5, #36]	; 0x24
8160f9b8:	e595200c 	ldr	r2, [r5, #12]
8160f9bc:	e6bf2f32 	rev	r2, r2
8160f9c0:	e0833002 	add	r3, r3, r2
8160f9c4:	e6bf3f33 	rev	r3, r3
8160f9c8:	e585300c 	str	r3, [r5, #12]
8160f9cc:	e8bd8070 	pop	{r4, r5, r6, pc}

8160f9d0 <_fdt_packblocks>:
8160f9d0:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160f9d4:	e1a04001 	mov	r4, r1
8160f9d8:	e5901010 	ldr	r1, [r0, #16]
8160f9dc:	e1a05000 	mov	r5, r0
8160f9e0:	e2827028 	add	r7, r2, #40	; 0x28
8160f9e4:	e1a06003 	mov	r6, r3
8160f9e8:	e6bf1f31 	rev	r1, r1
8160f9ec:	e0878003 	add	r8, r7, r3
8160f9f0:	e0801001 	add	r1, r0, r1
8160f9f4:	e2840028 	add	r0, r4, #40	; 0x28
8160f9f8:	eb0007ec 	bl	816119b0 <memmove>
8160f9fc:	e3a0330a 	mov	r3, #671088640	; 0x28000000
8160fa00:	e5843010 	str	r3, [r4, #16]
8160fa04:	e1a02006 	mov	r2, r6
8160fa08:	e5951008 	ldr	r1, [r5, #8]
8160fa0c:	e0840007 	add	r0, r4, r7
8160fa10:	e6bf6f36 	rev	r6, r6
8160fa14:	e6bf7f37 	rev	r7, r7
8160fa18:	e6bf1f31 	rev	r1, r1
8160fa1c:	e0851001 	add	r1, r5, r1
8160fa20:	eb0007e2 	bl	816119b0 <memmove>
8160fa24:	e5847008 	str	r7, [r4, #8]
8160fa28:	e5846024 	str	r6, [r4, #36]	; 0x24
8160fa2c:	e0840008 	add	r0, r4, r8
8160fa30:	e595100c 	ldr	r1, [r5, #12]
8160fa34:	e6bf8f38 	rev	r8, r8
8160fa38:	e5952020 	ldr	r2, [r5, #32]
8160fa3c:	e6bf1f31 	rev	r1, r1
8160fa40:	e6bf2f32 	rev	r2, r2
8160fa44:	e0851001 	add	r1, r5, r1
8160fa48:	eb0007d8 	bl	816119b0 <memmove>
8160fa4c:	e584800c 	str	r8, [r4, #12]
8160fa50:	e5953020 	ldr	r3, [r5, #32]
8160fa54:	e5843020 	str	r3, [r4, #32]
8160fa58:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

8160fa5c <_fdt_find_add_string>:
8160fa5c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160fa60:	e1a04000 	mov	r4, r0
8160fa64:	e590500c 	ldr	r5, [r0, #12]
8160fa68:	e1a08001 	mov	r8, r1
8160fa6c:	e6bf5f35 	rev	r5, r5
8160fa70:	e0805005 	add	r5, r0, r5
8160fa74:	e1a00001 	mov	r0, r1
8160fa78:	eb000795 	bl	816118d4 <strlen>
8160fa7c:	e5941020 	ldr	r1, [r4, #32]
8160fa80:	e1a02008 	mov	r2, r8
8160fa84:	e6bf1f31 	rev	r1, r1
8160fa88:	e1a07000 	mov	r7, r0
8160fa8c:	e1a00005 	mov	r0, r5
8160fa90:	ebfffd7a 	bl	8160f080 <_fdt_find_string>
8160fa94:	e2502000 	subs	r2, r0, #0
8160fa98:	0a000001 	beq	8160faa4 <_fdt_find_add_string+0x48>
8160fa9c:	e0420005 	sub	r0, r2, r5
8160faa0:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8160faa4:	e5946020 	ldr	r6, [r4, #32]
8160faa8:	e2877001 	add	r7, r7, #1
8160faac:	e594100c 	ldr	r1, [r4, #12]
8160fab0:	e1a00004 	mov	r0, r4
8160fab4:	e6bf6f36 	rev	r6, r6
8160fab8:	e1a03007 	mov	r3, r7
8160fabc:	e6bf1f31 	rev	r1, r1
8160fac0:	e0861001 	add	r1, r6, r1
8160fac4:	e0841001 	add	r1, r4, r1
8160fac8:	ebffff71 	bl	8160f894 <_fdt_splice>
8160facc:	e3500000 	cmp	r0, #0
8160fad0:	18bd81f0 	popne	{r4, r5, r6, r7, r8, pc}
8160fad4:	e5943020 	ldr	r3, [r4, #32]
8160fad8:	e1a02007 	mov	r2, r7
8160fadc:	e1a01008 	mov	r1, r8
8160fae0:	e0850006 	add	r0, r5, r6
8160fae4:	e6bf3f33 	rev	r3, r3
8160fae8:	e0873003 	add	r3, r7, r3
8160faec:	e6bf3f33 	rev	r3, r3
8160faf0:	e5843020 	str	r3, [r4, #32]
8160faf4:	ebffc328 	bl	8160079c <memcpy>
8160faf8:	e1a00006 	mov	r0, r6
8160fafc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

8160fb00 <_fdt_add_property>:
8160fb00:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160fb04:	e1a06000 	mov	r6, r0
8160fb08:	e1a04002 	mov	r4, r2
8160fb0c:	e1a05003 	mov	r5, r3
8160fb10:	e59d7018 	ldr	r7, [sp, #24]
8160fb14:	ebfffd0c 	bl	8160ef4c <_fdt_check_node_offset>
8160fb18:	e2508000 	subs	r8, r0, #0
8160fb1c:	ba000019 	blt	8160fb88 <_fdt_add_property+0x88>
8160fb20:	e1a01004 	mov	r1, r4
8160fb24:	e1a00006 	mov	r0, r6
8160fb28:	ebffffcb 	bl	8160fa5c <_fdt_find_add_string>
8160fb2c:	e2504000 	subs	r4, r0, #0
8160fb30:	b1a08004 	movlt	r8, r4
8160fb34:	ba000013 	blt	8160fb88 <_fdt_add_property+0x88>
8160fb38:	e5961008 	ldr	r1, [r6, #8]
8160fb3c:	e2853003 	add	r3, r5, #3
8160fb40:	e3c33003 	bic	r3, r3, #3
8160fb44:	e3a02000 	mov	r2, #0
8160fb48:	e6bf1f31 	rev	r1, r1
8160fb4c:	e283300c 	add	r3, r3, #12
8160fb50:	e0881001 	add	r1, r8, r1
8160fb54:	e1a00006 	mov	r0, r6
8160fb58:	e0861001 	add	r1, r6, r1
8160fb5c:	e5871000 	str	r1, [r7]
8160fb60:	ebffff87 	bl	8160f984 <_fdt_splice_struct>
8160fb64:	e2508000 	subs	r8, r0, #0
8160fb68:	1a000006 	bne	8160fb88 <_fdt_add_property+0x88>
8160fb6c:	e5973000 	ldr	r3, [r7]
8160fb70:	e3a02403 	mov	r2, #50331648	; 0x3000000
8160fb74:	e6bf4f34 	rev	r4, r4
8160fb78:	e6bf5f35 	rev	r5, r5
8160fb7c:	e5832000 	str	r2, [r3]
8160fb80:	e5834008 	str	r4, [r3, #8]
8160fb84:	e5835004 	str	r5, [r3, #4]
8160fb88:	e1a00008 	mov	r0, r8
8160fb8c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

8160fb90 <_fdt_rw_check_header>:
8160fb90:	e92d4070 	push	{r4, r5, r6, lr}
8160fb94:	e1a04000 	mov	r4, r0
8160fb98:	ebfffc74 	bl	8160ed70 <fdt_check_header>
8160fb9c:	e3500000 	cmp	r0, #0
8160fba0:	18bd8070 	popne	{r4, r5, r6, pc}
8160fba4:	e5945014 	ldr	r5, [r4, #20]
8160fba8:	e6bf5f35 	rev	r5, r5
8160fbac:	e3550010 	cmp	r5, #16
8160fbb0:	9a00000a 	bls	8160fbe0 <_fdt_rw_check_header+0x50>
8160fbb4:	e5942024 	ldr	r2, [r4, #36]	; 0x24
8160fbb8:	e3a01010 	mov	r1, #16
8160fbbc:	e1a00004 	mov	r0, r4
8160fbc0:	e6bf2f32 	rev	r2, r2
8160fbc4:	ebffff19 	bl	8160f830 <_fdt_blocks_misordered>
8160fbc8:	e3500000 	cmp	r0, #0
8160fbcc:	1a000005 	bne	8160fbe8 <_fdt_rw_check_header+0x58>
8160fbd0:	e3550011 	cmp	r5, #17
8160fbd4:	13a03411 	movne	r3, #285212672	; 0x11000000
8160fbd8:	15843014 	strne	r3, [r4, #20]
8160fbdc:	e8bd8070 	pop	{r4, r5, r6, pc}
8160fbe0:	e3e00009 	mvn	r0, #9
8160fbe4:	e8bd8070 	pop	{r4, r5, r6, pc}
8160fbe8:	e3e0000b 	mvn	r0, #11
8160fbec:	e8bd8070 	pop	{r4, r5, r6, pc}

8160fbf0 <fdt_add_mem_rsv>:
8160fbf0:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160fbf4:	e1a0a002 	mov	sl, r2
8160fbf8:	e1a05000 	mov	r5, r0
8160fbfc:	e1a0b003 	mov	fp, r3
8160fc00:	e59d7028 	ldr	r7, [sp, #40]	; 0x28
8160fc04:	e59d802c 	ldr	r8, [sp, #44]	; 0x2c
8160fc08:	ebffffe0 	bl	8160fb90 <_fdt_rw_check_header>
8160fc0c:	e2502000 	subs	r2, r0, #0
8160fc10:	1a000015 	bne	8160fc6c <fdt_add_mem_rsv+0x7c>
8160fc14:	e1a00005 	mov	r0, r5
8160fc18:	e58d2004 	str	r2, [sp, #4]
8160fc1c:	ebfffd84 	bl	8160f234 <fdt_num_mem_rsv>
8160fc20:	e5954010 	ldr	r4, [r5, #16]
8160fc24:	e59d2004 	ldr	r2, [sp, #4]
8160fc28:	e3a03001 	mov	r3, #1
8160fc2c:	e6bf4f34 	rev	r4, r4
8160fc30:	e0844200 	add	r4, r4, r0, lsl #4
8160fc34:	e1a00005 	mov	r0, r5
8160fc38:	e0856004 	add	r6, r5, r4
8160fc3c:	e1a01006 	mov	r1, r6
8160fc40:	ebffff39 	bl	8160f92c <_fdt_splice_mem_rsv>
8160fc44:	e2502000 	subs	r2, r0, #0
8160fc48:	1a000007 	bne	8160fc6c <fdt_add_mem_rsv+0x7c>
8160fc4c:	e6bfaf3a 	rev	sl, sl
8160fc50:	e6bfbf3b 	rev	fp, fp
8160fc54:	e6bf7f37 	rev	r7, r7
8160fc58:	e785b004 	str	fp, [r5, r4]
8160fc5c:	e6bf8f38 	rev	r8, r8
8160fc60:	e586a004 	str	sl, [r6, #4]
8160fc64:	e5868008 	str	r8, [r6, #8]
8160fc68:	e586700c 	str	r7, [r6, #12]
8160fc6c:	e1a00002 	mov	r0, r2
8160fc70:	e28dd008 	add	sp, sp, #8
8160fc74:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}

8160fc78 <fdt_del_mem_rsv>:
8160fc78:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
8160fc7c:	e1a05000 	mov	r5, r0
8160fc80:	e1a07001 	mov	r7, r1
8160fc84:	e5904010 	ldr	r4, [r0, #16]
8160fc88:	ebffffc0 	bl	8160fb90 <_fdt_rw_check_header>
8160fc8c:	e2506000 	subs	r6, r0, #0
8160fc90:	1a00000c 	bne	8160fcc8 <fdt_del_mem_rsv+0x50>
8160fc94:	e1a00005 	mov	r0, r5
8160fc98:	ebfffd65 	bl	8160f234 <fdt_num_mem_rsv>
8160fc9c:	e1500007 	cmp	r0, r7
8160fca0:	da000007 	ble	8160fcc4 <fdt_del_mem_rsv+0x4c>
8160fca4:	e6bf4f34 	rev	r4, r4
8160fca8:	e1a03006 	mov	r3, r6
8160fcac:	e0841207 	add	r1, r4, r7, lsl #4
8160fcb0:	e1a00005 	mov	r0, r5
8160fcb4:	e0851001 	add	r1, r5, r1
8160fcb8:	e3a02001 	mov	r2, #1
8160fcbc:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
8160fcc0:	eaffff19 	b	8160f92c <_fdt_splice_mem_rsv>
8160fcc4:	e3e06000 	mvn	r6, #0
8160fcc8:	e1a00006 	mov	r0, r6
8160fccc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

8160fcd0 <fdt_setprop>:
8160fcd0:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
8160fcd4:	e24dd014 	sub	sp, sp, #20
8160fcd8:	e1a06000 	mov	r6, r0
8160fcdc:	e1a07001 	mov	r7, r1
8160fce0:	e1a08002 	mov	r8, r2
8160fce4:	e1a0a003 	mov	sl, r3
8160fce8:	e59d4030 	ldr	r4, [sp, #48]	; 0x30
8160fcec:	ebffffa7 	bl	8160fb90 <_fdt_rw_check_header>
8160fcf0:	e2505000 	subs	r5, r0, #0
8160fcf4:	1a00001c 	bne	8160fd6c <fdt_setprop+0x9c>
8160fcf8:	e1a02008 	mov	r2, r8
8160fcfc:	e28d300c 	add	r3, sp, #12
8160fd00:	e1a01007 	mov	r1, r7
8160fd04:	e1a00006 	mov	r0, r6
8160fd08:	ebfffe09 	bl	8160f534 <fdt_get_property>
8160fd0c:	e59d200c 	ldr	r2, [sp, #12]
8160fd10:	e3500000 	cmp	r0, #0
8160fd14:	e58d0008 	str	r0, [sp, #8]
8160fd18:	1a000016 	bne	8160fd78 <fdt_setprop+0xa8>
8160fd1c:	e3720001 	cmn	r2, #1
8160fd20:	e1a00002 	mov	r0, r2
8160fd24:	1a000006 	bne	8160fd44 <fdt_setprop+0x74>
8160fd28:	e28d3008 	add	r3, sp, #8
8160fd2c:	e1a02008 	mov	r2, r8
8160fd30:	e58d3000 	str	r3, [sp]
8160fd34:	e1a01007 	mov	r1, r7
8160fd38:	e1a03004 	mov	r3, r4
8160fd3c:	e1a00006 	mov	r0, r6
8160fd40:	ebffff6e 	bl	8160fb00 <_fdt_add_property>
8160fd44:	e3500000 	cmp	r0, #0
8160fd48:	1a000018 	bne	8160fdb0 <fdt_setprop+0xe0>
8160fd4c:	e3540000 	cmp	r4, #0
8160fd50:	01a05004 	moveq	r5, r4
8160fd54:	0a000004 	beq	8160fd6c <fdt_setprop+0x9c>
8160fd58:	e59d0008 	ldr	r0, [sp, #8]
8160fd5c:	e1a02004 	mov	r2, r4
8160fd60:	e1a0100a 	mov	r1, sl
8160fd64:	e280000c 	add	r0, r0, #12
8160fd68:	ebffc28b 	bl	8160079c <memcpy>
8160fd6c:	e1a00005 	mov	r0, r5
8160fd70:	e28dd014 	add	sp, sp, #20
8160fd74:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
8160fd78:	e2843003 	add	r3, r4, #3
8160fd7c:	e2822003 	add	r2, r2, #3
8160fd80:	e3c33003 	bic	r3, r3, #3
8160fd84:	e3c22003 	bic	r2, r2, #3
8160fd88:	e280100c 	add	r1, r0, #12
8160fd8c:	e1a00006 	mov	r0, r6
8160fd90:	ebfffefb 	bl	8160f984 <_fdt_splice_struct>
8160fd94:	e3500000 	cmp	r0, #0
8160fd98:	06bf2f34 	reveq	r2, r4
8160fd9c:	059d3008 	ldreq	r3, [sp, #8]
8160fda0:	05832004 	streq	r2, [r3, #4]
8160fda4:	0affffe8 	beq	8160fd4c <fdt_setprop+0x7c>
8160fda8:	e3700001 	cmn	r0, #1
8160fdac:	0affffdd 	beq	8160fd28 <fdt_setprop+0x58>
8160fdb0:	e1a05000 	mov	r5, r0
8160fdb4:	eaffffec 	b	8160fd6c <fdt_setprop+0x9c>

8160fdb8 <fdt_delprop>:
8160fdb8:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
8160fdbc:	e1a05000 	mov	r5, r0
8160fdc0:	e1a06001 	mov	r6, r1
8160fdc4:	e1a07002 	mov	r7, r2
8160fdc8:	ebffff70 	bl	8160fb90 <_fdt_rw_check_header>
8160fdcc:	e2504000 	subs	r4, r0, #0
8160fdd0:	1a00000f 	bne	8160fe14 <fdt_delprop+0x5c>
8160fdd4:	e1a02007 	mov	r2, r7
8160fdd8:	e1a01006 	mov	r1, r6
8160fddc:	e28d3004 	add	r3, sp, #4
8160fde0:	e1a00005 	mov	r0, r5
8160fde4:	ebfffdd2 	bl	8160f534 <fdt_get_property>
8160fde8:	e59d2004 	ldr	r2, [sp, #4]
8160fdec:	e2501000 	subs	r1, r0, #0
8160fdf0:	01a04002 	moveq	r4, r2
8160fdf4:	0a000006 	beq	8160fe14 <fdt_delprop+0x5c>
8160fdf8:	e2822003 	add	r2, r2, #3
8160fdfc:	e1a03004 	mov	r3, r4
8160fe00:	e3c22003 	bic	r2, r2, #3
8160fe04:	e1a00005 	mov	r0, r5
8160fe08:	e282200c 	add	r2, r2, #12
8160fe0c:	ebfffedc 	bl	8160f984 <_fdt_splice_struct>
8160fe10:	e1a04000 	mov	r4, r0
8160fe14:	e1a00004 	mov	r0, r4
8160fe18:	e28dd00c 	add	sp, sp, #12
8160fe1c:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}

8160fe20 <fdt_add_subnode_namelen>:
8160fe20:	e92d4df3 	push	{r0, r1, r4, r5, r6, r7, r8, sl, fp, lr}
8160fe24:	e1a04000 	mov	r4, r0
8160fe28:	e1a05001 	mov	r5, r1
8160fe2c:	e1a0b002 	mov	fp, r2
8160fe30:	e1a0a003 	mov	sl, r3
8160fe34:	ebffff55 	bl	8160fb90 <_fdt_rw_check_header>
8160fe38:	e3500000 	cmp	r0, #0
8160fe3c:	1a000031 	bne	8160ff08 <fdt_add_subnode_namelen+0xe8>
8160fe40:	e1a0300a 	mov	r3, sl
8160fe44:	e1a0200b 	mov	r2, fp
8160fe48:	e1a01005 	mov	r1, r5
8160fe4c:	e1a00004 	mov	r0, r4
8160fe50:	ebfffd07 	bl	8160f274 <fdt_subnode_offset_namelen>
8160fe54:	e3500000 	cmp	r0, #0
8160fe58:	a3e00001 	mvnge	r0, #1
8160fe5c:	aa000029 	bge	8160ff08 <fdt_add_subnode_namelen+0xe8>
8160fe60:	e3700001 	cmn	r0, #1
8160fe64:	1a000027 	bne	8160ff08 <fdt_add_subnode_namelen+0xe8>
8160fe68:	e28d2004 	add	r2, sp, #4
8160fe6c:	e1a01005 	mov	r1, r5
8160fe70:	e1a00004 	mov	r0, r4
8160fe74:	ebfffbf5 	bl	8160ee50 <fdt_next_tag>
8160fe78:	e59d8004 	ldr	r8, [sp, #4]
8160fe7c:	e28d2004 	add	r2, sp, #4
8160fe80:	e1a00004 	mov	r0, r4
8160fe84:	e1a01008 	mov	r1, r8
8160fe88:	ebfffbf0 	bl	8160ee50 <fdt_next_tag>
8160fe8c:	e2400003 	sub	r0, r0, #3
8160fe90:	e3500001 	cmp	r0, #1
8160fe94:	9afffff7 	bls	8160fe78 <fdt_add_subnode_namelen+0x58>
8160fe98:	e5945008 	ldr	r5, [r4, #8]
8160fe9c:	e28a7004 	add	r7, sl, #4
8160fea0:	e3c77003 	bic	r7, r7, #3
8160fea4:	e3a02000 	mov	r2, #0
8160fea8:	e6bf5f35 	rev	r5, r5
8160feac:	e2873008 	add	r3, r7, #8
8160feb0:	e0885005 	add	r5, r8, r5
8160feb4:	e1a00004 	mov	r0, r4
8160feb8:	e0846005 	add	r6, r4, r5
8160febc:	e1a01006 	mov	r1, r6
8160fec0:	ebfffeaf 	bl	8160f984 <_fdt_splice_struct>
8160fec4:	e3500000 	cmp	r0, #0
8160fec8:	1a00000e 	bne	8160ff08 <fdt_add_subnode_namelen+0xe8>
8160fecc:	e3a03401 	mov	r3, #16777216	; 0x1000000
8160fed0:	e7843005 	str	r3, [r4, r5]
8160fed4:	e2864004 	add	r4, r6, #4
8160fed8:	e1a01000 	mov	r1, r0
8160fedc:	e1a02007 	mov	r2, r7
8160fee0:	e0866007 	add	r6, r6, r7
8160fee4:	e1a00004 	mov	r0, r4
8160fee8:	ebffc1f4 	bl	816006c0 <memset>
8160feec:	e1a0200a 	mov	r2, sl
8160fef0:	e1a0100b 	mov	r1, fp
8160fef4:	e1a00004 	mov	r0, r4
8160fef8:	ebffc227 	bl	8160079c <memcpy>
8160fefc:	e1a00008 	mov	r0, r8
8160ff00:	e3a03402 	mov	r3, #33554432	; 0x2000000
8160ff04:	e5863004 	str	r3, [r6, #4]
8160ff08:	e28dd008 	add	sp, sp, #8
8160ff0c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}

8160ff10 <fdt_add_subnode>:
8160ff10:	e92d4070 	push	{r4, r5, r6, lr}
8160ff14:	e1a05000 	mov	r5, r0
8160ff18:	e1a00002 	mov	r0, r2
8160ff1c:	e1a06001 	mov	r6, r1
8160ff20:	e1a04002 	mov	r4, r2
8160ff24:	eb00066a 	bl	816118d4 <strlen>
8160ff28:	e1a02004 	mov	r2, r4
8160ff2c:	e1a01006 	mov	r1, r6
8160ff30:	e1a03000 	mov	r3, r0
8160ff34:	e1a00005 	mov	r0, r5
8160ff38:	e8bd4070 	pop	{r4, r5, r6, lr}
8160ff3c:	eaffffb7 	b	8160fe20 <fdt_add_subnode_namelen>

8160ff40 <fdt_del_node>:
8160ff40:	e92d4070 	push	{r4, r5, r6, lr}
8160ff44:	e1a04000 	mov	r4, r0
8160ff48:	e1a05001 	mov	r5, r1
8160ff4c:	ebffff0f 	bl	8160fb90 <_fdt_rw_check_header>
8160ff50:	e3500000 	cmp	r0, #0
8160ff54:	18bd8070 	popne	{r4, r5, r6, pc}
8160ff58:	e1a01005 	mov	r1, r5
8160ff5c:	e1a00004 	mov	r0, r4
8160ff60:	eb000084 	bl	81610178 <_fdt_node_end_offset>
8160ff64:	e3500000 	cmp	r0, #0
8160ff68:	b8bd8070 	poplt	{r4, r5, r6, pc}
8160ff6c:	e5941008 	ldr	r1, [r4, #8]
8160ff70:	e0402005 	sub	r2, r0, r5
8160ff74:	e3a03000 	mov	r3, #0
8160ff78:	e1a00004 	mov	r0, r4
8160ff7c:	e6bf1f31 	rev	r1, r1
8160ff80:	e0851001 	add	r1, r5, r1
8160ff84:	e0841001 	add	r1, r4, r1
8160ff88:	e8bd4070 	pop	{r4, r5, r6, lr}
8160ff8c:	eafffe7c 	b	8160f984 <_fdt_splice_struct>

8160ff90 <fdt_open_into>:
8160ff90:	e92d4dff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, sl, fp, lr}
8160ff94:	e1a05000 	mov	r5, r0
8160ff98:	e1a04001 	mov	r4, r1
8160ff9c:	e1a06002 	mov	r6, r2
8160ffa0:	e5907004 	ldr	r7, [r0, #4]
8160ffa4:	ebfffb71 	bl	8160ed70 <fdt_check_header>
8160ffa8:	e250a000 	subs	sl, r0, #0
8160ffac:	1a00002d 	bne	81610068 <fdt_open_into+0xd8>
8160ffb0:	e1a00005 	mov	r0, r5
8160ffb4:	e6bf7f37 	rev	r7, r7
8160ffb8:	ebfffc9d 	bl	8160f234 <fdt_num_mem_rsv>
8160ffbc:	e5953014 	ldr	r3, [r5, #20]
8160ffc0:	e6bf3f33 	rev	r3, r3
8160ffc4:	e3530010 	cmp	r3, #16
8160ffc8:	85953024 	ldrhi	r3, [r5, #36]	; 0x24
8160ffcc:	86bf3f33 	revhi	r3, r3
8160ffd0:	858d300c 	strhi	r3, [sp, #12]
8160ffd4:	e2800001 	add	r0, r0, #1
8160ffd8:	e1a0b200 	lsl	fp, r0, #4
8160ffdc:	8a00000b 	bhi	81610010 <fdt_open_into+0x80>
8160ffe0:	e28d8010 	add	r8, sp, #16
8160ffe4:	e528a004 	str	sl, [r8, #-4]!
8160ffe8:	e1a02008 	mov	r2, r8
8160ffec:	e59d100c 	ldr	r1, [sp, #12]
8160fff0:	e1a00005 	mov	r0, r5
8160fff4:	ebfffb95 	bl	8160ee50 <fdt_next_tag>
8160fff8:	e3500009 	cmp	r0, #9
8160fffc:	1afffff9 	bne	8160ffe8 <fdt_open_into+0x58>
81610000:	e59d300c 	ldr	r3, [sp, #12]
81610004:	e3530000 	cmp	r3, #0
81610008:	b1a0a003 	movlt	sl, r3
8161000c:	ba000015 	blt	81610068 <fdt_open_into+0xd8>
81610010:	e59d300c 	ldr	r3, [sp, #12]
81610014:	e1a0100b 	mov	r1, fp
81610018:	e1a00005 	mov	r0, r5
8161001c:	e1a02003 	mov	r2, r3
81610020:	e58d3004 	str	r3, [sp, #4]
81610024:	ebfffe01 	bl	8160f830 <_fdt_blocks_misordered>
81610028:	e59d3004 	ldr	r3, [sp, #4]
8161002c:	e3500000 	cmp	r0, #0
81610030:	1a00000f 	bne	81610074 <fdt_open_into+0xe4>
81610034:	e1a02006 	mov	r2, r6
81610038:	e1a01004 	mov	r1, r4
8161003c:	e1a00005 	mov	r0, r5
81610040:	ebfffc24 	bl	8160f0d8 <fdt_move>
81610044:	e250a000 	subs	sl, r0, #0
81610048:	1a000006 	bne	81610068 <fdt_open_into+0xd8>
8161004c:	e3a03411 	mov	r3, #285212672	; 0x11000000
81610050:	e5843014 	str	r3, [r4, #20]
81610054:	e59d300c 	ldr	r3, [sp, #12]
81610058:	e6bf6f36 	rev	r6, r6
8161005c:	e5846004 	str	r6, [r4, #4]
81610060:	e6bf3f33 	rev	r3, r3
81610064:	e5843024 	str	r3, [r4, #36]	; 0x24
81610068:	e1a0000a 	mov	r0, sl
8161006c:	e28dd010 	add	sp, sp, #16
81610070:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81610074:	e5958020 	ldr	r8, [r5, #32]
81610078:	e6bf2f38 	rev	r2, r8
8161007c:	e2838028 	add	r8, r3, #40	; 0x28
81610080:	e088800b 	add	r8, r8, fp
81610084:	e0888002 	add	r8, r8, r2
81610088:	e1580006 	cmp	r8, r6
8161008c:	ca00000d 	bgt	816100c8 <fdt_open_into+0x138>
81610090:	e0841008 	add	r1, r4, r8
81610094:	e0857007 	add	r7, r5, r7
81610098:	e1550001 	cmp	r5, r1
8161009c:	23a02000 	movcs	r2, #0
816100a0:	33a02001 	movcc	r2, #1
816100a4:	e1570004 	cmp	r7, r4
816100a8:	93a02000 	movls	r2, #0
816100ac:	e3520000 	cmp	r2, #0
816100b0:	01a07004 	moveq	r7, r4
816100b4:	0a000005 	beq	816100d0 <fdt_open_into+0x140>
816100b8:	e0872008 	add	r2, r7, r8
816100bc:	e0841006 	add	r1, r4, r6
816100c0:	e1520001 	cmp	r2, r1
816100c4:	9a000001 	bls	816100d0 <fdt_open_into+0x140>
816100c8:	e3e0a002 	mvn	sl, #2
816100cc:	eaffffe5 	b	81610068 <fdt_open_into+0xd8>
816100d0:	e1a0200b 	mov	r2, fp
816100d4:	e1a01007 	mov	r1, r7
816100d8:	e1a00005 	mov	r0, r5
816100dc:	e6bf6f36 	rev	r6, r6
816100e0:	ebfffe3a 	bl	8160f9d0 <_fdt_packblocks>
816100e4:	e1a02008 	mov	r2, r8
816100e8:	e1a01007 	mov	r1, r7
816100ec:	e1a00004 	mov	r0, r4
816100f0:	eb00062e 	bl	816119b0 <memmove>
816100f4:	e59f3020 	ldr	r3, [pc, #32]	; 8161011c <fdt_open_into+0x18c>
816100f8:	e5846004 	str	r6, [r4, #4]
816100fc:	e5843000 	str	r3, [r4]
81610100:	e3a03411 	mov	r3, #285212672	; 0x11000000
81610104:	e5843014 	str	r3, [r4, #20]
81610108:	e3a03201 	mov	r3, #268435456	; 0x10000000
8161010c:	e5843018 	str	r3, [r4, #24]
81610110:	e595301c 	ldr	r3, [r5, #28]
81610114:	e584301c 	str	r3, [r4, #28]
81610118:	eaffffd2 	b	81610068 <fdt_open_into+0xd8>
8161011c:	edfe0dd0 	.word	0xedfe0dd0

81610120 <fdt_strerror>:
81610120:	e3500000 	cmp	r0, #0
81610124:	ca000009 	bgt	81610150 <fdt_strerror+0x30>
81610128:	0a00000a 	beq	81610158 <fdt_strerror+0x38>
8161012c:	e3700012 	cmn	r0, #18
81610130:	9a00000a 	bls	81610160 <fdt_strerror+0x40>
81610134:	e59f302c 	ldr	r3, [pc, #44]	; 81610168 <fdt_strerror+0x48>
81610138:	e2600000 	rsb	r0, r0, #0
8161013c:	e7930100 	ldr	r0, [r3, r0, lsl #2]
81610140:	e59f3024 	ldr	r3, [pc, #36]	; 8161016c <fdt_strerror+0x4c>
81610144:	e3500000 	cmp	r0, #0
81610148:	01a00003 	moveq	r0, r3
8161014c:	e12fff1e 	bx	lr
81610150:	e59f0018 	ldr	r0, [pc, #24]	; 81610170 <fdt_strerror+0x50>
81610154:	e12fff1e 	bx	lr
81610158:	e59f0014 	ldr	r0, [pc, #20]	; 81610174 <fdt_strerror+0x54>
8161015c:	e12fff1e 	bx	lr
81610160:	e59f0004 	ldr	r0, [pc, #4]	; 8161016c <fdt_strerror+0x4c>
81610164:	e12fff1e 	bx	lr
81610168:	816131fc 	.word	0x816131fc
8161016c:	81615dd6 	.word	0x81615dd6
81610170:	81615db5 	.word	0x81615db5
81610174:	81615dcb 	.word	0x81615dcb

81610178 <_fdt_node_end_offset>:
81610178:	e92d4037 	push	{r0, r1, r2, r4, r5, lr}
8161017c:	e28d4008 	add	r4, sp, #8
81610180:	e1a05000 	mov	r5, r0
81610184:	e3a03000 	mov	r3, #0
81610188:	e5243004 	str	r3, [r4, #-4]!
8161018c:	e59d3004 	ldr	r3, [sp, #4]
81610190:	e3530000 	cmp	r3, #0
81610194:	a3510000 	cmpge	r1, #0
81610198:	aa000002 	bge	816101a8 <_fdt_node_end_offset+0x30>
8161019c:	e1a00001 	mov	r0, r1
816101a0:	e28dd00c 	add	sp, sp, #12
816101a4:	e8bd8030 	pop	{r4, r5, pc}
816101a8:	e1a02004 	mov	r2, r4
816101ac:	e1a00005 	mov	r0, r5
816101b0:	ebfffb83 	bl	8160efc4 <fdt_next_node>
816101b4:	e1a01000 	mov	r1, r0
816101b8:	eafffff3 	b	8161018c <_fdt_node_end_offset+0x14>

816101bc <fdt_address_cells>:
816101bc:	e92d4007 	push	{r0, r1, r2, lr}
816101c0:	e28d3004 	add	r3, sp, #4
816101c4:	e59f2038 	ldr	r2, [pc, #56]	; 81610204 <fdt_address_cells+0x48>
816101c8:	ebfffd02 	bl	8160f5d8 <fdt_getprop>
816101cc:	e3500000 	cmp	r0, #0
816101d0:	03a00002 	moveq	r0, #2
816101d4:	0a000008 	beq	816101fc <fdt_address_cells+0x40>
816101d8:	e59d3004 	ldr	r3, [sp, #4]
816101dc:	e3530004 	cmp	r3, #4
816101e0:	13e0000d 	mvnne	r0, #13
816101e4:	1a000004 	bne	816101fc <fdt_address_cells+0x40>
816101e8:	e5900000 	ldr	r0, [r0]
816101ec:	e6bf0f30 	rev	r0, r0
816101f0:	e2403001 	sub	r3, r0, #1
816101f4:	e3530004 	cmp	r3, #4
816101f8:	23e0000d 	mvncs	r0, #13
816101fc:	e28dd00c 	add	sp, sp, #12
81610200:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
81610204:	81615b93 	.word	0x81615b93

81610208 <fdt_size_cells>:
81610208:	e92d4007 	push	{r0, r1, r2, lr}
8161020c:	e28d3004 	add	r3, sp, #4
81610210:	e59f2034 	ldr	r2, [pc, #52]	; 8161024c <fdt_size_cells+0x44>
81610214:	ebfffcef 	bl	8160f5d8 <fdt_getprop>
81610218:	e3500000 	cmp	r0, #0
8161021c:	03a00002 	moveq	r0, #2
81610220:	0a000007 	beq	81610244 <fdt_size_cells+0x3c>
81610224:	e59d3004 	ldr	r3, [sp, #4]
81610228:	e3530004 	cmp	r3, #4
8161022c:	13e0000d 	mvnne	r0, #13
81610230:	1a000003 	bne	81610244 <fdt_size_cells+0x3c>
81610234:	e5900000 	ldr	r0, [r0]
81610238:	e6bf0f30 	rev	r0, r0
8161023c:	e3500005 	cmp	r0, #5
81610240:	23e0000d 	mvncs	r0, #13
81610244:	e28dd00c 	add	sp, sp, #12
81610248:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
8161024c:	81615da9 	.word	0x81615da9

81610250 <cmpkey>:
81610250:	e5912000 	ldr	r2, [r1]
81610254:	e5903000 	ldr	r3, [r0]
81610258:	e5921000 	ldr	r1, [r2]
8161025c:	e5930000 	ldr	r0, [r3]
81610260:	ea000576 	b	81611840 <strcmp>

81610264 <_hdelete.isra.0>:
81610264:	e92d4070 	push	{r4, r5, r6, lr}
81610268:	e1a04001 	mov	r4, r1
8161026c:	e1a05000 	mov	r5, r0
81610270:	e5910000 	ldr	r0, [r1]
81610274:	e1a06002 	mov	r6, r2
81610278:	ebffdbee 	bl	81607238 <free>
8161027c:	e5940004 	ldr	r0, [r4, #4]
81610280:	ebffdbec 	bl	81607238 <free>
81610284:	e3a03000 	mov	r3, #0
81610288:	e5843008 	str	r3, [r4, #8]
8161028c:	e584300c 	str	r3, [r4, #12]
81610290:	e3a03014 	mov	r3, #20
81610294:	e0020693 	mul	r2, r3, r6
81610298:	e5951000 	ldr	r1, [r5]
8161029c:	e3e03000 	mvn	r3, #0
816102a0:	e7813002 	str	r3, [r1, r2]
816102a4:	e5953008 	ldr	r3, [r5, #8]
816102a8:	e2433001 	sub	r3, r3, #1
816102ac:	e5853008 	str	r3, [r5, #8]
816102b0:	e8bd8070 	pop	{r4, r5, r6, pc}

816102b4 <drop_var_from_set.part.1>:
816102b4:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816102b8:	e3a05000 	mov	r5, #0
816102bc:	e1a08000 	mov	r8, r0
816102c0:	e1a07001 	mov	r7, r1
816102c4:	e1a06002 	mov	r6, r2
816102c8:	e1a04005 	mov	r4, r5
816102cc:	e1540007 	cmp	r4, r7
816102d0:	ba000001 	blt	816102dc <drop_var_from_set.part.1+0x28>
816102d4:	e1a00005 	mov	r0, r5
816102d8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
816102dc:	e7961104 	ldr	r1, [r6, r4, lsl #2]
816102e0:	e3510000 	cmp	r1, #0
816102e4:	0a000004 	beq	816102fc <drop_var_from_set.part.1+0x48>
816102e8:	e1a00008 	mov	r0, r8
816102ec:	eb000553 	bl	81611840 <strcmp>
816102f0:	e3500000 	cmp	r0, #0
816102f4:	07860104 	streq	r0, [r6, r4, lsl #2]
816102f8:	03a05001 	moveq	r5, #1
816102fc:	e2844001 	add	r4, r4, #1
81610300:	eafffff1 	b	816102cc <drop_var_from_set.part.1+0x18>

81610304 <match_string.isra.2>:
81610304:	e2000d07 	and	r0, r0, #448	; 0x1c0
81610308:	e1a03001 	mov	r3, r1
8161030c:	e3500040 	cmp	r0, #64	; 0x40
81610310:	e1a01002 	mov	r1, r2
81610314:	e92d4010 	push	{r4, lr}
81610318:	0a000006 	beq	81610338 <match_string.isra.2+0x34>
8161031c:	e3500080 	cmp	r0, #128	; 0x80
81610320:	0a000009 	beq	8161034c <match_string.isra.2+0x48>
81610324:	e1a01000 	mov	r1, r0
81610328:	e59f0030 	ldr	r0, [pc, #48]	; 81610360 <match_string.isra.2+0x5c>
8161032c:	eb0008d2 	bl	8161267c <printf>
81610330:	e3a00000 	mov	r0, #0
81610334:	e8bd8010 	pop	{r4, pc}
81610338:	e1a00003 	mov	r0, r3
8161033c:	eb00053f 	bl	81611840 <strcmp>
81610340:	e16f0f10 	clz	r0, r0
81610344:	e1a002a0 	lsr	r0, r0, #5
81610348:	e8bd8010 	pop	{r4, pc}
8161034c:	e1a00003 	mov	r0, r3
81610350:	eb0005b7 	bl	81611a34 <strstr>
81610354:	e2900000 	adds	r0, r0, #0
81610358:	13a00001 	movne	r0, #1
8161035c:	e8bd8010 	pop	{r4, pc}
81610360:	8161645a 	.word	0x8161645a

81610364 <hcreate_r>:
81610364:	e92d4070 	push	{r4, r5, r6, lr}
81610368:	e2514000 	subs	r4, r1, #0
8161036c:	1a000004 	bne	81610384 <hcreate_r+0x20>
81610370:	e59f3080 	ldr	r3, [pc, #128]	; 816103f8 <hcreate_r+0x94>
81610374:	e3a02016 	mov	r2, #22
81610378:	e5832000 	str	r2, [r3]
8161037c:	e3a00000 	mov	r0, #0
81610380:	e8bd8070 	pop	{r4, r5, r6, pc}
81610384:	e5943000 	ldr	r3, [r4]
81610388:	e3530000 	cmp	r3, #0
8161038c:	03805001 	orreq	r5, r0, #1
81610390:	1afffff9 	bne	8161037c <hcreate_r+0x18>
81610394:	e3a06003 	mov	r6, #3
81610398:	e1a01006 	mov	r1, r6
8161039c:	e1a00005 	mov	r0, r5
816103a0:	eb000a38 	bl	81612c88 <__aeabi_uidivmod>
816103a4:	e0030696 	mul	r3, r6, r6
816103a8:	e1550003 	cmp	r5, r3
816103ac:	8a00000d 	bhi	816103e8 <hcreate_r+0x84>
816103b0:	e3510000 	cmp	r1, #0
816103b4:	0a00000d 	beq	816103f0 <hcreate_r+0x8c>
816103b8:	e3a03000 	mov	r3, #0
816103bc:	e5845004 	str	r5, [r4, #4]
816103c0:	e5843008 	str	r3, [r4, #8]
816103c4:	e3a01014 	mov	r1, #20
816103c8:	e2850001 	add	r0, r5, #1
816103cc:	ebffdda5 	bl	81607a68 <calloc>
816103d0:	e5840000 	str	r0, [r4]
816103d4:	e2900000 	adds	r0, r0, #0
816103d8:	13a00001 	movne	r0, #1
816103dc:	e8bd8070 	pop	{r4, r5, r6, pc}
816103e0:	e2866002 	add	r6, r6, #2
816103e4:	eaffffeb 	b	81610398 <hcreate_r+0x34>
816103e8:	e3510000 	cmp	r1, #0
816103ec:	1afffffb 	bne	816103e0 <hcreate_r+0x7c>
816103f0:	e2855002 	add	r5, r5, #2
816103f4:	eaffffe6 	b	81610394 <hcreate_r+0x30>
816103f8:	81619e84 	.word	0x81619e84

816103fc <hdestroy_r>:
816103fc:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81610400:	e2504000 	subs	r4, r0, #0
81610404:	13a05001 	movne	r5, #1
81610408:	13a07014 	movne	r7, #20
8161040c:	1a00000d 	bne	81610448 <hdestroy_r+0x4c>
81610410:	e59f304c 	ldr	r3, [pc, #76]	; 81610464 <hdestroy_r+0x68>
81610414:	e3a02016 	mov	r2, #22
81610418:	e5832000 	str	r2, [r3]
8161041c:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81610420:	e0030597 	mul	r3, r7, r5
81610424:	e0806003 	add	r6, r0, r3
81610428:	e7903003 	ldr	r3, [r0, r3]
8161042c:	e3530000 	cmp	r3, #0
81610430:	da000003 	ble	81610444 <hdestroy_r+0x48>
81610434:	e5960004 	ldr	r0, [r6, #4]
81610438:	ebffdb7e 	bl	81607238 <free>
8161043c:	e5960008 	ldr	r0, [r6, #8]
81610440:	ebffdb7c 	bl	81607238 <free>
81610444:	e2855001 	add	r5, r5, #1
81610448:	e8940009 	ldm	r4, {r0, r3}
8161044c:	e1550003 	cmp	r5, r3
81610450:	9afffff2 	bls	81610420 <hdestroy_r+0x24>
81610454:	ebffdb77 	bl	81607238 <free>
81610458:	e3a03000 	mov	r3, #0
8161045c:	e5843000 	str	r3, [r4]
81610460:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
81610464:	81619e84 	.word	0x81619e84

81610468 <hmatch_r>:
81610468:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
8161046c:	e1a04001 	mov	r4, r1
81610470:	e1a0a000 	mov	sl, r0
81610474:	e1a06002 	mov	r6, r2
81610478:	e1a05003 	mov	r5, r3
8161047c:	eb000514 	bl	816118d4 <strlen>
81610480:	e2844001 	add	r4, r4, #1
81610484:	e3a0b014 	mov	fp, #20
81610488:	e1a08000 	mov	r8, r0
8161048c:	e5953004 	ldr	r3, [r5, #4]
81610490:	e1530004 	cmp	r3, r4
81610494:	8a000005 	bhi	816104b0 <hmatch_r+0x48>
81610498:	e59f3060 	ldr	r3, [pc, #96]	; 81610500 <hmatch_r+0x98>
8161049c:	e3a02003 	mov	r2, #3
816104a0:	e3a00000 	mov	r0, #0
816104a4:	e5832000 	str	r2, [r3]
816104a8:	e5860000 	str	r0, [r6]
816104ac:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816104b0:	e007049b 	mul	r7, fp, r4
816104b4:	e5953000 	ldr	r3, [r5]
816104b8:	e0831007 	add	r1, r3, r7
816104bc:	e7933007 	ldr	r3, [r3, r7]
816104c0:	e3530000 	cmp	r3, #0
816104c4:	da00000b 	ble	816104f8 <hmatch_r+0x90>
816104c8:	e1a02008 	mov	r2, r8
816104cc:	e5911004 	ldr	r1, [r1, #4]
816104d0:	e1a0000a 	mov	r0, sl
816104d4:	eb0004e3 	bl	81611868 <strncmp>
816104d8:	e3500000 	cmp	r0, #0
816104dc:	1a000005 	bne	816104f8 <hmatch_r+0x90>
816104e0:	e5953000 	ldr	r3, [r5]
816104e4:	e1a00004 	mov	r0, r4
816104e8:	e0833007 	add	r3, r3, r7
816104ec:	e2833004 	add	r3, r3, #4
816104f0:	e5863000 	str	r3, [r6]
816104f4:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816104f8:	e2844001 	add	r4, r4, #1
816104fc:	eaffffe2 	b	8161048c <hmatch_r+0x24>
81610500:	81619e84 	.word	0x81619e84

81610504 <hsearch_r>:
81610504:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81610508:	e24dd018 	sub	sp, sp, #24
8161050c:	e28dc018 	add	ip, sp, #24
81610510:	e59d4040 	ldr	r4, [sp, #64]	; 0x40
81610514:	e90c000f 	stmdb	ip, {r0, r1, r2, r3}
81610518:	e59d8008 	ldr	r8, [sp, #8]
8161051c:	e59d600c 	ldr	r6, [sp, #12]
81610520:	e1a00008 	mov	r0, r8
81610524:	eb0004ea 	bl	816118d4 <strlen>
81610528:	e0883000 	add	r3, r8, r0
8161052c:	e1580003 	cmp	r8, r3
81610530:	1a000031 	bne	816105fc <hsearch_r+0xf8>
81610534:	e5941004 	ldr	r1, [r4, #4]
81610538:	e3a05014 	mov	r5, #20
8161053c:	eb0009d1 	bl	81612c88 <__aeabi_uidivmod>
81610540:	e5943000 	ldr	r3, [r4]
81610544:	e3510000 	cmp	r1, #0
81610548:	11a07001 	movne	r7, r1
8161054c:	03a07001 	moveq	r7, #1
81610550:	e0050795 	mul	r5, r5, r7
81610554:	e0832005 	add	r2, r3, r5
81610558:	e7933005 	ldr	r3, [r3, r5]
8161055c:	e3530000 	cmp	r3, #0
81610560:	0a0000e8 	beq	81610908 <hsearch_r+0x404>
81610564:	e3730001 	cmn	r3, #1
81610568:	01a01007 	moveq	r1, r7
8161056c:	13a01000 	movne	r1, #0
81610570:	e1570003 	cmp	r7, r3
81610574:	e58d1000 	str	r1, [sp]
81610578:	1a00004a 	bne	816106a8 <hsearch_r+0x1a4>
8161057c:	e5921004 	ldr	r1, [r2, #4]
81610580:	e1a00008 	mov	r0, r8
81610584:	eb0004ad 	bl	81611840 <strcmp>
81610588:	e3500000 	cmp	r0, #0
8161058c:	1a000045 	bne	816106a8 <hsearch_r+0x1a4>
81610590:	e59d3038 	ldr	r3, [sp, #56]	; 0x38
81610594:	e3560000 	cmp	r6, #0
81610598:	e2433001 	sub	r3, r3, #1
8161059c:	e16f3f13 	clz	r3, r3
816105a0:	e1a032a3 	lsr	r3, r3, #5
816105a4:	03a03000 	moveq	r3, #0
816105a8:	e3530000 	cmp	r3, #0
816105ac:	0a000035 	beq	81610688 <hsearch_r+0x184>
816105b0:	e594a00c 	ldr	sl, [r4, #12]
816105b4:	e35a0000 	cmp	sl, #0
816105b8:	0a000012 	beq	81610608 <hsearch_r+0x104>
816105bc:	e5940000 	ldr	r0, [r4]
816105c0:	e3a02002 	mov	r2, #2
816105c4:	e59d3044 	ldr	r3, [sp, #68]	; 0x44
816105c8:	e1a01006 	mov	r1, r6
816105cc:	e0800005 	add	r0, r0, r5
816105d0:	e2800004 	add	r0, r0, #4
816105d4:	e12fff3a 	blx	sl
816105d8:	e3500000 	cmp	r0, #0
816105dc:	0a000009 	beq	81610608 <hsearch_r+0x104>
816105e0:	e59f339c 	ldr	r3, [pc, #924]	; 81610984 <hsearch_r+0x480>
816105e4:	e3a02001 	mov	r2, #1
816105e8:	e5832000 	str	r2, [r3]
816105ec:	e3a00000 	mov	r0, #0
816105f0:	e59d303c 	ldr	r3, [sp, #60]	; 0x3c
816105f4:	e5830000 	str	r0, [r3]
816105f8:	ea000028 	b	816106a0 <hsearch_r+0x19c>
816105fc:	e5732001 	ldrb	r2, [r3, #-1]!
81610600:	e0820200 	add	r0, r2, r0, lsl #4
81610604:	eaffffc8 	b	8161052c <hsearch_r+0x28>
81610608:	e5943000 	ldr	r3, [r4]
8161060c:	e0833005 	add	r3, r3, r5
81610610:	e593a00c 	ldr	sl, [r3, #12]
81610614:	e35a0000 	cmp	sl, #0
81610618:	0a000009 	beq	81610644 <hsearch_r+0x140>
8161061c:	e59d3044 	ldr	r3, [sp, #68]	; 0x44
81610620:	e3a02002 	mov	r2, #2
81610624:	e1a01006 	mov	r1, r6
81610628:	e1a00008 	mov	r0, r8
8161062c:	e12fff3a 	blx	sl
81610630:	e3500000 	cmp	r0, #0
81610634:	0a000002 	beq	81610644 <hsearch_r+0x140>
81610638:	e59f3344 	ldr	r3, [pc, #836]	; 81610984 <hsearch_r+0x480>
8161063c:	e3a02016 	mov	r2, #22
81610640:	eaffffe8 	b	816105e8 <hsearch_r+0xe4>
81610644:	e5943000 	ldr	r3, [r4]
81610648:	e0833005 	add	r3, r3, r5
8161064c:	e5930008 	ldr	r0, [r3, #8]
81610650:	ebffdaf8 	bl	81607238 <free>
81610654:	e1a00006 	mov	r0, r6
81610658:	e5948000 	ldr	r8, [r4]
8161065c:	eb0004b1 	bl	81611928 <strdup>
81610660:	e5943000 	ldr	r3, [r4]
81610664:	e0888005 	add	r8, r8, r5
81610668:	e0833005 	add	r3, r3, r5
8161066c:	e5880008 	str	r0, [r8, #8]
81610670:	e5933008 	ldr	r3, [r3, #8]
81610674:	e3530000 	cmp	r3, #0
81610678:	1a000002 	bne	81610688 <hsearch_r+0x184>
8161067c:	e59f3300 	ldr	r3, [pc, #768]	; 81610984 <hsearch_r+0x480>
81610680:	e3a0200c 	mov	r2, #12
81610684:	eaffffd7 	b	816105e8 <hsearch_r+0xe4>
81610688:	e5943000 	ldr	r3, [r4]
8161068c:	e1a00007 	mov	r0, r7
81610690:	e0835005 	add	r5, r3, r5
81610694:	e59d303c 	ldr	r3, [sp, #60]	; 0x3c
81610698:	e2855004 	add	r5, r5, #4
8161069c:	e5835000 	str	r5, [r3]
816106a0:	e28dd018 	add	sp, sp, #24
816106a4:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816106a8:	e5941004 	ldr	r1, [r4, #4]
816106ac:	e1a00007 	mov	r0, r7
816106b0:	e1a05007 	mov	r5, r7
816106b4:	e2411002 	sub	r1, r1, #2
816106b8:	eb000972 	bl	81612c88 <__aeabi_uidivmod>
816106bc:	e2813001 	add	r3, r1, #1
816106c0:	e58d3004 	str	r3, [sp, #4]
816106c4:	e59d3004 	ldr	r3, [sp, #4]
816106c8:	e1550003 	cmp	r5, r3
816106cc:	95943004 	ldrls	r3, [r4, #4]
816106d0:	959d2004 	ldrls	r2, [sp, #4]
816106d4:	859d3004 	ldrhi	r3, [sp, #4]
816106d8:	90433002 	subls	r3, r3, r2
816106dc:	90855003 	addls	r5, r5, r3
816106e0:	80455003 	subhi	r5, r5, r3
816106e4:	e1570005 	cmp	r7, r5
816106e8:	0a000089 	beq	81610914 <hsearch_r+0x410>
816106ec:	e3a03014 	mov	r3, #20
816106f0:	e00a0593 	mul	sl, r3, r5
816106f4:	e5943000 	ldr	r3, [r4]
816106f8:	e083200a 	add	r2, r3, sl
816106fc:	e793300a 	ldr	r3, [r3, sl]
81610700:	e1570003 	cmp	r7, r3
81610704:	0a000043 	beq	81610818 <hsearch_r+0x314>
81610708:	e5943000 	ldr	r3, [r4]
8161070c:	e793300a 	ldr	r3, [r3, sl]
81610710:	e3530000 	cmp	r3, #0
81610714:	1affffea 	bne	816106c4 <hsearch_r+0x1c0>
81610718:	e59d3038 	ldr	r3, [sp, #56]	; 0x38
8161071c:	e3530001 	cmp	r3, #1
81610720:	159f325c 	ldrne	r3, [pc, #604]	; 81610984 <hsearch_r+0x480>
81610724:	13a02003 	movne	r2, #3
81610728:	1affffae 	bne	816105e8 <hsearch_r+0xe4>
8161072c:	e5942008 	ldr	r2, [r4, #8]
81610730:	e5943004 	ldr	r3, [r4, #4]
81610734:	e1520003 	cmp	r2, r3
81610738:	0affffcf 	beq	8161067c <hsearch_r+0x178>
8161073c:	e59d3000 	ldr	r3, [sp]
81610740:	e1a00008 	mov	r0, r8
81610744:	e3530000 	cmp	r3, #0
81610748:	01a03005 	moveq	r3, r5
8161074c:	e3a05014 	mov	r5, #20
81610750:	e0050395 	mul	r5, r5, r3
81610754:	e58d3000 	str	r3, [sp]
81610758:	e5943000 	ldr	r3, [r4]
8161075c:	e083a005 	add	sl, r3, r5
81610760:	e7837005 	str	r7, [r3, r5]
81610764:	eb00046f 	bl	81611928 <strdup>
81610768:	e5947000 	ldr	r7, [r4]
8161076c:	e0877005 	add	r7, r7, r5
81610770:	e58a0004 	str	r0, [sl, #4]
81610774:	e1a00006 	mov	r0, r6
81610778:	eb00046a 	bl	81611928 <strdup>
8161077c:	e5870008 	str	r0, [r7, #8]
81610780:	e5940000 	ldr	r0, [r4]
81610784:	e0800005 	add	r0, r0, r5
81610788:	e5903004 	ldr	r3, [r0, #4]
8161078c:	e3530000 	cmp	r3, #0
81610790:	0affffb9 	beq	8161067c <hsearch_r+0x178>
81610794:	e5903008 	ldr	r3, [r0, #8]
81610798:	e3530000 	cmp	r3, #0
8161079c:	0affffb6 	beq	8161067c <hsearch_r+0x178>
816107a0:	e5943008 	ldr	r3, [r4, #8]
816107a4:	e2800004 	add	r0, r0, #4
816107a8:	e2833001 	add	r3, r3, #1
816107ac:	e5843008 	str	r3, [r4, #8]
816107b0:	ebffecf8 	bl	8160bb98 <env_callback_init>
816107b4:	e5940000 	ldr	r0, [r4]
816107b8:	e0800005 	add	r0, r0, r5
816107bc:	e2800004 	add	r0, r0, #4
816107c0:	ebffed91 	bl	8160be0c <env_flags_init>
816107c4:	e594700c 	ldr	r7, [r4, #12]
816107c8:	e3570000 	cmp	r7, #0
816107cc:	0a000052 	beq	8161091c <hsearch_r+0x418>
816107d0:	e5940000 	ldr	r0, [r4]
816107d4:	e3a02000 	mov	r2, #0
816107d8:	e59d3044 	ldr	r3, [sp, #68]	; 0x44
816107dc:	e1a01006 	mov	r1, r6
816107e0:	e0800005 	add	r0, r0, r5
816107e4:	e2800004 	add	r0, r0, #4
816107e8:	e12fff37 	blx	r7
816107ec:	e3500000 	cmp	r0, #0
816107f0:	0a000049 	beq	8161091c <hsearch_r+0x418>
816107f4:	e5941000 	ldr	r1, [r4]
816107f8:	e1a00004 	mov	r0, r4
816107fc:	e59d2000 	ldr	r2, [sp]
81610800:	e0811005 	add	r1, r1, r5
81610804:	e2811004 	add	r1, r1, #4
81610808:	ebfffe95 	bl	81610264 <_hdelete.isra.0>
8161080c:	e59f3170 	ldr	r3, [pc, #368]	; 81610984 <hsearch_r+0x480>
81610810:	e59d2038 	ldr	r2, [sp, #56]	; 0x38
81610814:	eaffff73 	b	816105e8 <hsearch_r+0xe4>
81610818:	e5921004 	ldr	r1, [r2, #4]
8161081c:	e1a00008 	mov	r0, r8
81610820:	eb000406 	bl	81611840 <strcmp>
81610824:	e3500000 	cmp	r0, #0
81610828:	1affffb6 	bne	81610708 <hsearch_r+0x204>
8161082c:	e59d3038 	ldr	r3, [sp, #56]	; 0x38
81610830:	e3560000 	cmp	r6, #0
81610834:	e2433001 	sub	r3, r3, #1
81610838:	e16f3f13 	clz	r3, r3
8161083c:	e1a032a3 	lsr	r3, r3, #5
81610840:	03a03000 	moveq	r3, #0
81610844:	e3530000 	cmp	r3, #0
81610848:	0a000025 	beq	816108e4 <hsearch_r+0x3e0>
8161084c:	e594b00c 	ldr	fp, [r4, #12]
81610850:	e35b0000 	cmp	fp, #0
81610854:	0a000008 	beq	8161087c <hsearch_r+0x378>
81610858:	e5940000 	ldr	r0, [r4]
8161085c:	e3a02002 	mov	r2, #2
81610860:	e59d3044 	ldr	r3, [sp, #68]	; 0x44
81610864:	e1a01006 	mov	r1, r6
81610868:	e080000a 	add	r0, r0, sl
8161086c:	e2800004 	add	r0, r0, #4
81610870:	e12fff3b 	blx	fp
81610874:	e3500000 	cmp	r0, #0
81610878:	1affff58 	bne	816105e0 <hsearch_r+0xdc>
8161087c:	e5943000 	ldr	r3, [r4]
81610880:	e083300a 	add	r3, r3, sl
81610884:	e593b00c 	ldr	fp, [r3, #12]
81610888:	e35b0000 	cmp	fp, #0
8161088c:	0a000006 	beq	816108ac <hsearch_r+0x3a8>
81610890:	e59d3044 	ldr	r3, [sp, #68]	; 0x44
81610894:	e3a02002 	mov	r2, #2
81610898:	e1a01006 	mov	r1, r6
8161089c:	e1a00008 	mov	r0, r8
816108a0:	e12fff3b 	blx	fp
816108a4:	e3500000 	cmp	r0, #0
816108a8:	1affff62 	bne	81610638 <hsearch_r+0x134>
816108ac:	e5943000 	ldr	r3, [r4]
816108b0:	e083300a 	add	r3, r3, sl
816108b4:	e5930008 	ldr	r0, [r3, #8]
816108b8:	ebffda5e 	bl	81607238 <free>
816108bc:	e1a00006 	mov	r0, r6
816108c0:	e594b000 	ldr	fp, [r4]
816108c4:	eb000417 	bl	81611928 <strdup>
816108c8:	e5943000 	ldr	r3, [r4]
816108cc:	e08bb00a 	add	fp, fp, sl
816108d0:	e083300a 	add	r3, r3, sl
816108d4:	e58b0008 	str	r0, [fp, #8]
816108d8:	e5933008 	ldr	r3, [r3, #8]
816108dc:	e3530000 	cmp	r3, #0
816108e0:	0affff65 	beq	8161067c <hsearch_r+0x178>
816108e4:	e5943000 	ldr	r3, [r4]
816108e8:	e3750001 	cmn	r5, #1
816108ec:	e59d203c 	ldr	r2, [sp, #60]	; 0x3c
816108f0:	e1a00005 	mov	r0, r5
816108f4:	e083300a 	add	r3, r3, sl
816108f8:	e2833004 	add	r3, r3, #4
816108fc:	e5823000 	str	r3, [r2]
81610900:	0affff80 	beq	81610708 <hsearch_r+0x204>
81610904:	eaffff65 	b	816106a0 <hsearch_r+0x19c>
81610908:	e1a05007 	mov	r5, r7
8161090c:	e58d3000 	str	r3, [sp]
81610910:	eaffff80 	b	81610718 <hsearch_r+0x214>
81610914:	e1a05007 	mov	r5, r7
81610918:	eaffff7e 	b	81610718 <hsearch_r+0x214>
8161091c:	e5943000 	ldr	r3, [r4]
81610920:	e0833005 	add	r3, r3, r5
81610924:	e593700c 	ldr	r7, [r3, #12]
81610928:	e3570000 	cmp	r7, #0
8161092c:	0a00000d 	beq	81610968 <hsearch_r+0x464>
81610930:	e59d3044 	ldr	r3, [sp, #68]	; 0x44
81610934:	e3a02000 	mov	r2, #0
81610938:	e1a01006 	mov	r1, r6
8161093c:	e1a00008 	mov	r0, r8
81610940:	e12fff37 	blx	r7
81610944:	e3500000 	cmp	r0, #0
81610948:	0a000006 	beq	81610968 <hsearch_r+0x464>
8161094c:	e5941000 	ldr	r1, [r4]
81610950:	e1a00004 	mov	r0, r4
81610954:	e59d2000 	ldr	r2, [sp]
81610958:	e0811005 	add	r1, r1, r5
8161095c:	e2811004 	add	r1, r1, #4
81610960:	ebfffe3f 	bl	81610264 <_hdelete.isra.0>
81610964:	eaffff33 	b	81610638 <hsearch_r+0x134>
81610968:	e5941000 	ldr	r1, [r4]
8161096c:	e3a00001 	mov	r0, #1
81610970:	e59d303c 	ldr	r3, [sp, #60]	; 0x3c
81610974:	e0815005 	add	r5, r1, r5
81610978:	e2855004 	add	r5, r5, #4
8161097c:	e5835000 	str	r5, [r3]
81610980:	eaffff46 	b	816106a0 <hsearch_r+0x19c>
81610984:	81619e84 	.word	0x81619e84

81610988 <hdelete_r>:
81610988:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
8161098c:	e24dd02c 	sub	sp, sp, #44	; 0x2c
81610990:	e28d3014 	add	r3, sp, #20
81610994:	e3a06000 	mov	r6, #0
81610998:	e58d3004 	str	r3, [sp, #4]
8161099c:	e28d3028 	add	r3, sp, #40	; 0x28
816109a0:	e58d0018 	str	r0, [sp, #24]
816109a4:	e1a07000 	mov	r7, r0
816109a8:	e58d600c 	str	r6, [sp, #12]
816109ac:	e1a05001 	mov	r5, r1
816109b0:	e58d1008 	str	r1, [sp, #8]
816109b4:	e1a08002 	mov	r8, r2
816109b8:	e58d6000 	str	r6, [sp]
816109bc:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
816109c0:	ebfffecf 	bl	81610504 <hsearch_r>
816109c4:	e2504000 	subs	r4, r0, #0
816109c8:	1a000005 	bne	816109e4 <hdelete_r+0x5c>
816109cc:	e59f30a0 	ldr	r3, [pc, #160]	; 81610a74 <hdelete_r+0xec>
816109d0:	e3a02003 	mov	r2, #3
816109d4:	e5832000 	str	r2, [r3]
816109d8:	e1a00004 	mov	r0, r4
816109dc:	e28dd02c 	add	sp, sp, #44	; 0x2c
816109e0:	e8bd85f0 	pop	{r4, r5, r6, r7, r8, sl, pc}
816109e4:	e595a00c 	ldr	sl, [r5, #12]
816109e8:	e35a0000 	cmp	sl, #0
816109ec:	0a000008 	beq	81610a14 <hdelete_r+0x8c>
816109f0:	e1a03008 	mov	r3, r8
816109f4:	e3a02001 	mov	r2, #1
816109f8:	e1a01006 	mov	r1, r6
816109fc:	e59d0014 	ldr	r0, [sp, #20]
81610a00:	e12fff3a 	blx	sl
81610a04:	e3500000 	cmp	r0, #0
81610a08:	159f3064 	ldrne	r3, [pc, #100]	; 81610a74 <hdelete_r+0xec>
81610a0c:	13a02001 	movne	r2, #1
81610a10:	1a00000e 	bne	81610a50 <hdelete_r+0xc8>
81610a14:	e5952000 	ldr	r2, [r5]
81610a18:	e3a03014 	mov	r3, #20
81610a1c:	e0232493 	mla	r3, r3, r4, r2
81610a20:	e593600c 	ldr	r6, [r3, #12]
81610a24:	e3560000 	cmp	r6, #0
81610a28:	0a00000b 	beq	81610a5c <hdelete_r+0xd4>
81610a2c:	e1a03008 	mov	r3, r8
81610a30:	e3a02001 	mov	r2, #1
81610a34:	e3a01000 	mov	r1, #0
81610a38:	e1a00007 	mov	r0, r7
81610a3c:	e12fff36 	blx	r6
81610a40:	e3500000 	cmp	r0, #0
81610a44:	0a000004 	beq	81610a5c <hdelete_r+0xd4>
81610a48:	e59f3024 	ldr	r3, [pc, #36]	; 81610a74 <hdelete_r+0xec>
81610a4c:	e3a02016 	mov	r2, #22
81610a50:	e5832000 	str	r2, [r3]
81610a54:	e3a04000 	mov	r4, #0
81610a58:	eaffffde 	b	816109d8 <hdelete_r+0x50>
81610a5c:	e1a02004 	mov	r2, r4
81610a60:	e59d1014 	ldr	r1, [sp, #20]
81610a64:	e1a00005 	mov	r0, r5
81610a68:	e3a04001 	mov	r4, #1
81610a6c:	ebfffdfc 	bl	81610264 <_hdelete.isra.0>
81610a70:	eaffffd8 	b	816109d8 <hdelete_r+0x50>
81610a74:	81619e84 	.word	0x81619e84

81610a78 <hexport_r>:
81610a78:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81610a7c:	e28db01c 	add	fp, sp, #28
81610a80:	e24dd020 	sub	sp, sp, #32
81610a84:	e590c004 	ldr	ip, [r0, #4]
81610a88:	e16f4f10 	clz	r4, r0
81610a8c:	e3530000 	cmp	r3, #0
81610a90:	e59b5004 	ldr	r5, [fp, #4]
81610a94:	e1a042a4 	lsr	r4, r4, #5
81610a98:	e1a0c10c 	lsl	ip, ip, #2
81610a9c:	03a04001 	moveq	r4, #1
81610aa0:	e28cc00a 	add	ip, ip, #10
81610aa4:	e3540000 	cmp	r4, #0
81610aa8:	e3ccc007 	bic	ip, ip, #7
81610aac:	e04dd00c 	sub	sp, sp, ip
81610ab0:	159f32cc 	ldrne	r3, [pc, #716]	; 81610d84 <hexport_r+0x30c>
81610ab4:	13a02016 	movne	r2, #22
81610ab8:	1a00006d 	bne	81610c74 <hexport_r+0x1fc>
81610abc:	e50b302c 	str	r3, [fp, #-44]	; 0xffffffd4
81610ac0:	e1a0700d 	mov	r7, sp
81610ac4:	e3a03001 	mov	r3, #1
81610ac8:	e1a06002 	mov	r6, r2
81610acc:	e50b3020 	str	r3, [fp, #-32]	; 0xffffffe0
81610ad0:	e1a0a001 	mov	sl, r1
81610ad4:	e2023010 	and	r3, r2, #16
81610ad8:	e50b0028 	str	r0, [fp, #-40]	; 0xffffffd8
81610adc:	e50b4024 	str	r4, [fp, #-36]	; 0xffffffdc
81610ae0:	e50b3034 	str	r3, [fp, #-52]	; 0xffffffcc
81610ae4:	ea00004d 	b	81610c20 <hexport_r+0x1a8>
81610ae8:	e51b2020 	ldr	r2, [fp, #-32]	; 0xffffffe0
81610aec:	e3a03014 	mov	r3, #20
81610af0:	e0030293 	mul	r3, r3, r2
81610af4:	e51b2028 	ldr	r2, [fp, #-40]	; 0xffffffd8
81610af8:	e5922000 	ldr	r2, [r2]
81610afc:	e0828003 	add	r8, r2, r3
81610b00:	e7923003 	ldr	r3, [r2, r3]
81610b04:	e3530000 	cmp	r3, #0
81610b08:	da000041 	ble	81610c14 <hexport_r+0x19c>
81610b0c:	e2062020 	and	r2, r6, #32
81610b10:	e3a03000 	mov	r3, #0
81610b14:	e50b2030 	str	r2, [fp, #-48]	; 0xffffffd0
81610b18:	ea00000f 	b	81610b5c <hexport_r+0xe4>
81610b1c:	e51b2034 	ldr	r2, [fp, #-52]	; 0xffffffcc
81610b20:	e3520000 	cmp	r2, #0
81610b24:	1a000011 	bne	81610b70 <hexport_r+0xf8>
81610b28:	e51b2030 	ldr	r2, [fp, #-48]	; 0xffffffd0
81610b2c:	e3520000 	cmp	r2, #0
81610b30:	0a000008 	beq	81610b58 <hexport_r+0xe0>
81610b34:	e59b200c 	ldr	r2, [fp, #12]
81610b38:	e1a00006 	mov	r0, r6
81610b3c:	e5981008 	ldr	r1, [r8, #8]
81610b40:	e50b3038 	str	r3, [fp, #-56]	; 0xffffffc8
81610b44:	e7922103 	ldr	r2, [r2, r3, lsl #2]
81610b48:	ebfffded 	bl	81610304 <match_string.isra.2>
81610b4c:	e51b3038 	ldr	r3, [fp, #-56]	; 0xffffffc8
81610b50:	e3500000 	cmp	r0, #0
81610b54:	1a00000e 	bne	81610b94 <hexport_r+0x11c>
81610b58:	e2833001 	add	r3, r3, #1
81610b5c:	e59b2008 	ldr	r2, [fp, #8]
81610b60:	e1520003 	cmp	r2, r3
81610b64:	caffffec 	bgt	81610b1c <hexport_r+0xa4>
81610b68:	e3a03000 	mov	r3, #0
81610b6c:	ea000009 	b	81610b98 <hexport_r+0x120>
81610b70:	e59b200c 	ldr	r2, [fp, #12]
81610b74:	e1a00006 	mov	r0, r6
81610b78:	e5981004 	ldr	r1, [r8, #4]
81610b7c:	e50b3038 	str	r3, [fp, #-56]	; 0xffffffc8
81610b80:	e7922103 	ldr	r2, [r2, r3, lsl #2]
81610b84:	ebfffdde 	bl	81610304 <match_string.isra.2>
81610b88:	e51b3038 	ldr	r3, [fp, #-56]	; 0xffffffc8
81610b8c:	e3500000 	cmp	r0, #0
81610b90:	0affffe4 	beq	81610b28 <hexport_r+0xb0>
81610b94:	e3a03001 	mov	r3, #1
81610b98:	e59b2008 	ldr	r2, [fp, #8]
81610b9c:	e2233001 	eor	r3, r3, #1
81610ba0:	e3520000 	cmp	r2, #0
81610ba4:	d3a03000 	movle	r3, #0
81610ba8:	c2033001 	andgt	r3, r3, #1
81610bac:	e3530000 	cmp	r3, #0
81610bb0:	1a000017 	bne	81610c14 <hexport_r+0x19c>
81610bb4:	e3160008 	tst	r6, #8
81610bb8:	e5980004 	ldr	r0, [r8, #4]
81610bbc:	0a000002 	beq	81610bcc <hexport_r+0x154>
81610bc0:	e5d03000 	ldrb	r3, [r0]
81610bc4:	e353002e 	cmp	r3, #46	; 0x2e
81610bc8:	0a000011 	beq	81610c14 <hexport_r+0x19c>
81610bcc:	e2843001 	add	r3, r4, #1
81610bd0:	e50b3030 	str	r3, [fp, #-48]	; 0xffffffd0
81610bd4:	e2883004 	add	r3, r8, #4
81610bd8:	e7873104 	str	r3, [r7, r4, lsl #2]
81610bdc:	eb00033c 	bl	816118d4 <strlen>
81610be0:	e51b3024 	ldr	r3, [fp, #-36]	; 0xffffffdc
81610be4:	e35a0000 	cmp	sl, #0
81610be8:	e2834002 	add	r4, r3, #2
81610bec:	15983008 	ldrne	r3, [r8, #8]
81610bf0:	12433001 	subne	r3, r3, #1
81610bf4:	e0804004 	add	r4, r0, r4
81610bf8:	1a000025 	bne	81610c94 <hexport_r+0x21c>
81610bfc:	e5980008 	ldr	r0, [r8, #8]
81610c00:	eb000333 	bl	816118d4 <strlen>
81610c04:	e0844000 	add	r4, r4, r0
81610c08:	e2843002 	add	r3, r4, #2
81610c0c:	e51b4030 	ldr	r4, [fp, #-48]	; 0xffffffd0
81610c10:	e50b3024 	str	r3, [fp, #-36]	; 0xffffffdc
81610c14:	e51b3020 	ldr	r3, [fp, #-32]	; 0xffffffe0
81610c18:	e2833001 	add	r3, r3, #1
81610c1c:	e50b3020 	str	r3, [fp, #-32]	; 0xffffffe0
81610c20:	e51b3028 	ldr	r3, [fp, #-40]	; 0xffffffd8
81610c24:	e51b2020 	ldr	r2, [fp, #-32]	; 0xffffffe0
81610c28:	e5933004 	ldr	r3, [r3, #4]
81610c2c:	e1530002 	cmp	r3, r2
81610c30:	2affffac 	bcs	81610ae8 <hexport_r+0x70>
81610c34:	e59f314c 	ldr	r3, [pc, #332]	; 81610d88 <hexport_r+0x310>
81610c38:	e3a02004 	mov	r2, #4
81610c3c:	e1a01004 	mov	r1, r4
81610c40:	e1a00007 	mov	r0, r7
81610c44:	ebfff80b 	bl	8160ec78 <qsort>
81610c48:	e51b3024 	ldr	r3, [fp, #-36]	; 0xffffffdc
81610c4c:	e3550000 	cmp	r5, #0
81610c50:	e2832001 	add	r2, r3, #1
81610c54:	0a000012 	beq	81610ca4 <hexport_r+0x22c>
81610c58:	e1550002 	cmp	r5, r2
81610c5c:	2a000011 	bcs	81610ca8 <hexport_r+0x230>
81610c60:	e1a01005 	mov	r1, r5
81610c64:	e59f0120 	ldr	r0, [pc, #288]	; 81610d8c <hexport_r+0x314>
81610c68:	eb000683 	bl	8161267c <printf>
81610c6c:	e59f3110 	ldr	r3, [pc, #272]	; 81610d84 <hexport_r+0x30c>
81610c70:	e3a0200c 	mov	r2, #12
81610c74:	e3e00000 	mvn	r0, #0
81610c78:	e5832000 	str	r2, [r3]
81610c7c:	e24bd01c 	sub	sp, fp, #28
81610c80:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81610c84:	e152000a 	cmp	r2, sl
81610c88:	1352005c 	cmpne	r2, #92	; 0x5c
81610c8c:	12844001 	addne	r4, r4, #1
81610c90:	02844002 	addeq	r4, r4, #2
81610c94:	e5f32001 	ldrb	r2, [r3, #1]!
81610c98:	e3520000 	cmp	r2, #0
81610c9c:	1afffff8 	bne	81610c84 <hexport_r+0x20c>
81610ca0:	eaffffd8 	b	81610c08 <hexport_r+0x190>
81610ca4:	e1a05002 	mov	r5, r2
81610ca8:	e51b302c 	ldr	r3, [fp, #-44]	; 0xffffffd4
81610cac:	e5936000 	ldr	r6, [r3]
81610cb0:	e3560000 	cmp	r6, #0
81610cb4:	0a00000c 	beq	81610cec <hexport_r+0x274>
81610cb8:	e1a02005 	mov	r2, r5
81610cbc:	e3a01000 	mov	r1, #0
81610cc0:	e1a00006 	mov	r0, r6
81610cc4:	ebffbe7d 	bl	816006c0 <memset>
81610cc8:	e3a02000 	mov	r2, #0
81610ccc:	e3a0003d 	mov	r0, #61	; 0x3d
81610cd0:	e3a0c05c 	mov	ip, #92	; 0x5c
81610cd4:	e1520004 	cmp	r2, r4
81610cd8:	1a00000c 	bne	81610d10 <hexport_r+0x298>
81610cdc:	e3a03000 	mov	r3, #0
81610ce0:	e1a00005 	mov	r0, r5
81610ce4:	e5c63000 	strb	r3, [r6]
81610ce8:	eaffffe3 	b	81610c7c <hexport_r+0x204>
81610cec:	e1a01005 	mov	r1, r5
81610cf0:	e3a00001 	mov	r0, #1
81610cf4:	ebffdb5b 	bl	81607a68 <calloc>
81610cf8:	e51b302c 	ldr	r3, [fp, #-44]	; 0xffffffd4
81610cfc:	e3500000 	cmp	r0, #0
81610d00:	e1a06000 	mov	r6, r0
81610d04:	e5830000 	str	r0, [r3]
81610d08:	1affffee 	bne	81610cc8 <hexport_r+0x250>
81610d0c:	eaffffd6 	b	81610c6c <hexport_r+0x1f4>
81610d10:	e7973102 	ldr	r3, [r7, r2, lsl #2]
81610d14:	e5931000 	ldr	r1, [r3]
81610d18:	e2411001 	sub	r1, r1, #1
81610d1c:	e5f1e001 	ldrb	lr, [r1, #1]!
81610d20:	e2863001 	add	r3, r6, #1
81610d24:	e35e0000 	cmp	lr, #0
81610d28:	1a00000a 	bne	81610d58 <hexport_r+0x2e0>
81610d2c:	e5c60000 	strb	r0, [r6]
81610d30:	e7971102 	ldr	r1, [r7, r2, lsl #2]
81610d34:	e5911004 	ldr	r1, [r1, #4]
81610d38:	e2411001 	sub	r1, r1, #1
81610d3c:	e5f1e001 	ldrb	lr, [r1, #1]!
81610d40:	e2836001 	add	r6, r3, #1
81610d44:	e35e0000 	cmp	lr, #0
81610d48:	1a000005 	bne	81610d64 <hexport_r+0x2ec>
81610d4c:	e5c3a000 	strb	sl, [r3]
81610d50:	e2822001 	add	r2, r2, #1
81610d54:	eaffffde 	b	81610cd4 <hexport_r+0x25c>
81610d58:	e543e001 	strb	lr, [r3, #-1]
81610d5c:	e1a06003 	mov	r6, r3
81610d60:	eaffffed 	b	81610d1c <hexport_r+0x2a4>
81610d64:	e35e005c 	cmp	lr, #92	; 0x5c
81610d68:	115e000a 	cmpne	lr, sl
81610d6c:	05c3c000 	strbeq	ip, [r3]
81610d70:	e5d1e000 	ldrb	lr, [r1]
81610d74:	01a03006 	moveq	r3, r6
81610d78:	e2833001 	add	r3, r3, #1
81610d7c:	e543e001 	strb	lr, [r3, #-1]
81610d80:	eaffffed 	b	81610d3c <hexport_r+0x2c4>
81610d84:	81619e84 	.word	0x81619e84
81610d88:	81610250 	.word	0x81610250
81610d8c:	81615f68 	.word	0x81615f68

81610d90 <himport_r>:
81610d90:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81610d94:	e28db01c 	add	fp, sp, #28
81610d98:	e24dd030 	sub	sp, sp, #48	; 0x30
81610d9c:	e1a08003 	mov	r8, r3
81610da0:	e59b300c 	ldr	r3, [fp, #12]
81610da4:	e2507000 	subs	r7, r0, #0
81610da8:	e1a04103 	lsl	r4, r3, #2
81610dac:	e284300a 	add	r3, r4, #10
81610db0:	e3c33007 	bic	r3, r3, #7
81610db4:	e04dd003 	sub	sp, sp, r3
81610db8:	e28d3010 	add	r3, sp, #16
81610dbc:	e50b3038 	str	r3, [fp, #-56]	; 0xffffffc8
81610dc0:	1a000006 	bne	81610de0 <himport_r+0x50>
81610dc4:	e59f33a8 	ldr	r3, [pc, #936]	; 81611174 <himport_r+0x3e4>
81610dc8:	e1a04007 	mov	r4, r7
81610dcc:	e3a02016 	mov	r2, #22
81610dd0:	e5832000 	str	r2, [r3]
81610dd4:	e1a00004 	mov	r0, r4
81610dd8:	e24bd01c 	sub	sp, fp, #28
81610ddc:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81610de0:	e2820001 	add	r0, r2, #1
81610de4:	e1a0a001 	mov	sl, r1
81610de8:	e1a06002 	mov	r6, r2
81610dec:	ebffd99c 	bl	81607464 <malloc>
81610df0:	e2505000 	subs	r5, r0, #0
81610df4:	059f3378 	ldreq	r3, [pc, #888]	; 81611174 <himport_r+0x3e4>
81610df8:	03a0200c 	moveq	r2, #12
81610dfc:	01a04005 	moveq	r4, r5
81610e00:	05832000 	streq	r2, [r3]
81610e04:	0afffff2 	beq	81610dd4 <himport_r+0x44>
81610e08:	e1a0100a 	mov	r1, sl
81610e0c:	e1a02006 	mov	r2, r6
81610e10:	ebffbe61 	bl	8160079c <memcpy>
81610e14:	e3a03000 	mov	r3, #0
81610e18:	e7c53006 	strb	r3, [r5, r6]
81610e1c:	e085a006 	add	sl, r5, r6
81610e20:	e59b300c 	ldr	r3, [fp, #12]
81610e24:	e3530000 	cmp	r3, #0
81610e28:	0a000003 	beq	81610e3c <himport_r+0xac>
81610e2c:	e1a02004 	mov	r2, r4
81610e30:	e59b1010 	ldr	r1, [fp, #16]
81610e34:	e51b0038 	ldr	r0, [fp, #-56]	; 0xffffffc8
81610e38:	ebffbe57 	bl	8160079c <memcpy>
81610e3c:	e59b3004 	ldr	r3, [fp, #4]
81610e40:	e3130001 	tst	r3, #1
81610e44:	1a000004 	bne	81610e5c <himport_r+0xcc>
81610e48:	e5973000 	ldr	r3, [r7]
81610e4c:	e3530000 	cmp	r3, #0
81610e50:	0a000001 	beq	81610e5c <himport_r+0xcc>
81610e54:	e1a00007 	mov	r0, r7
81610e58:	ebfffd67 	bl	816103fc <hdestroy_r>
81610e5c:	e5973000 	ldr	r3, [r7]
81610e60:	e3530000 	cmp	r3, #0
81610e64:	1a00000a 	bne	81610e94 <himport_r+0x104>
81610e68:	e1a001a6 	lsr	r0, r6, #3
81610e6c:	e1a01007 	mov	r1, r7
81610e70:	e2800040 	add	r0, r0, #64	; 0x40
81610e74:	e3500c02 	cmp	r0, #512	; 0x200
81610e78:	23a00c02 	movcs	r0, #512	; 0x200
81610e7c:	ebfffd38 	bl	81610364 <hcreate_r>
81610e80:	e2504000 	subs	r4, r0, #0
81610e84:	1a000002 	bne	81610e94 <himport_r+0x104>
81610e88:	e1a00005 	mov	r0, r5
81610e8c:	ebffd8e9 	bl	81607238 <free>
81610e90:	eaffffcf 	b	81610dd4 <himport_r+0x44>
81610e94:	e3560000 	cmp	r6, #0
81610e98:	1a000003 	bne	81610eac <himport_r+0x11c>
81610e9c:	e1a00005 	mov	r0, r5
81610ea0:	ebffd8e4 	bl	81607238 <free>
81610ea4:	e3a04001 	mov	r4, #1
81610ea8:	eaffffc9 	b	81610dd4 <himport_r+0x44>
81610eac:	e59b3008 	ldr	r3, [fp, #8]
81610eb0:	e3530000 	cmp	r3, #0
81610eb4:	0a00000a 	beq	81610ee4 <himport_r+0x154>
81610eb8:	e246c001 	sub	ip, r6, #1
81610ebc:	e1a03005 	mov	r3, r5
81610ec0:	e085c00c 	add	ip, r5, ip
81610ec4:	e3a02000 	mov	r2, #0
81610ec8:	e15a0003 	cmp	sl, r3
81610ecc:	9a000003 	bls	81610ee0 <himport_r+0x150>
81610ed0:	e1a01003 	mov	r1, r3
81610ed4:	e4d10001 	ldrb	r0, [r1], #1
81610ed8:	e3500000 	cmp	r0, #0
81610edc:	1a00002c 	bne	81610f94 <himport_r+0x204>
81610ee0:	e0466002 	sub	r6, r6, r2
81610ee4:	e1a04005 	mov	r4, r5
81610ee8:	e0853006 	add	r3, r5, r6
81610eec:	e50b303c 	str	r3, [fp, #-60]	; 0xffffffc4
81610ef0:	e5d43000 	ldrb	r3, [r4]
81610ef4:	e3530009 	cmp	r3, #9
81610ef8:	13530020 	cmpne	r3, #32
81610efc:	0a000030 	beq	81610fc4 <himport_r+0x234>
81610f00:	e3530023 	cmp	r3, #35	; 0x23
81610f04:	11a03004 	movne	r3, r4
81610f08:	1a000031 	bne	81610fd4 <himport_r+0x244>
81610f0c:	e4d43001 	ldrb	r3, [r4], #1
81610f10:	e3530000 	cmp	r3, #0
81610f14:	11530008 	cmpne	r3, r8
81610f18:	e1a06004 	mov	r6, r4
81610f1c:	1afffffa 	bne	81610f0c <himport_r+0x17c>
81610f20:	e51b303c 	ldr	r3, [fp, #-60]	; 0xffffffc4
81610f24:	e1560003 	cmp	r6, r3
81610f28:	2a000002 	bcs	81610f38 <himport_r+0x1a8>
81610f2c:	e5d63000 	ldrb	r3, [r6]
81610f30:	e3530000 	cmp	r3, #0
81610f34:	1a000023 	bne	81610fc8 <himport_r+0x238>
81610f38:	e1a00005 	mov	r0, r5
81610f3c:	e59f4234 	ldr	r4, [pc, #564]	; 81611178 <himport_r+0x3e8>
81610f40:	ebffd8bc 	bl	81607238 <free>
81610f44:	e59f5230 	ldr	r5, [pc, #560]	; 8161117c <himport_r+0x3ec>
81610f48:	e3a06000 	mov	r6, #0
81610f4c:	e59b300c 	ldr	r3, [fp, #12]
81610f50:	e1560003 	cmp	r6, r3
81610f54:	aaffffd2 	bge	81610ea4 <himport_r+0x114>
81610f58:	e51b3038 	ldr	r3, [fp, #-56]	; 0xffffffc8
81610f5c:	e7930106 	ldr	r0, [r3, r6, lsl #2]
81610f60:	e3500000 	cmp	r0, #0
81610f64:	0a000008 	beq	81610f8c <himport_r+0x1fc>
81610f68:	e1a01007 	mov	r1, r7
81610f6c:	e59b2004 	ldr	r2, [fp, #4]
81610f70:	ebfffe84 	bl	81610988 <hdelete_r>
81610f74:	e51b3038 	ldr	r3, [fp, #-56]	; 0xffffffc8
81610f78:	e7931106 	ldr	r1, [r3, r6, lsl #2]
81610f7c:	e3500000 	cmp	r0, #0
81610f80:	01a00005 	moveq	r0, r5
81610f84:	11a00004 	movne	r0, r4
81610f88:	eb0005bb 	bl	8161267c <printf>
81610f8c:	e2866001 	add	r6, r6, #1
81610f90:	eaffffed 	b	81610f4c <himport_r+0x1bc>
81610f94:	e350000d 	cmp	r0, #13
81610f98:	1a000005 	bne	81610fb4 <himport_r+0x224>
81610f9c:	e15c0003 	cmp	ip, r3
81610fa0:	9a000003 	bls	81610fb4 <himport_r+0x224>
81610fa4:	e5d1e000 	ldrb	lr, [r1]
81610fa8:	e35e000a 	cmp	lr, #10
81610fac:	02822001 	addeq	r2, r2, #1
81610fb0:	0a000001 	beq	81610fbc <himport_r+0x22c>
81610fb4:	e0433002 	sub	r3, r3, r2
81610fb8:	e5c30000 	strb	r0, [r3]
81610fbc:	e1a03001 	mov	r3, r1
81610fc0:	eaffffc0 	b	81610ec8 <himport_r+0x138>
81610fc4:	e2846001 	add	r6, r4, #1
81610fc8:	e1a04006 	mov	r4, r6
81610fcc:	eaffffc7 	b	81610ef0 <himport_r+0x160>
81610fd0:	e1a0300a 	mov	r3, sl
81610fd4:	e1a0a003 	mov	sl, r3
81610fd8:	e4da2001 	ldrb	r2, [sl], #1
81610fdc:	e352003d 	cmp	r2, #61	; 0x3d
81610fe0:	13520000 	cmpne	r2, #0
81610fe4:	0a000011 	beq	81611030 <himport_r+0x2a0>
81610fe8:	e1520008 	cmp	r2, r8
81610fec:	1afffff7 	bne	81610fd0 <himport_r+0x240>
81610ff0:	e1a02008 	mov	r2, r8
81610ff4:	e5d30001 	ldrb	r0, [r3, #1]
81610ff8:	e0401008 	sub	r1, r0, r8
81610ffc:	e3500000 	cmp	r0, #0
81611000:	e16f1f11 	clz	r1, r1
81611004:	e1a012a1 	lsr	r1, r1, #5
81611008:	03a01001 	moveq	r1, #1
8161100c:	e1580002 	cmp	r8, r2
81611010:	03811001 	orreq	r1, r1, #1
81611014:	e3510000 	cmp	r1, #0
81611018:	0a000018 	beq	81611080 <himport_r+0x2f0>
8161101c:	e352003d 	cmp	r2, #61	; 0x3d
81611020:	03a02000 	moveq	r2, #0
81611024:	05c32000 	strbeq	r2, [r3]
81611028:	01a0300a 	moveq	r3, sl
8161102c:	ea000001 	b	81611038 <himport_r+0x2a8>
81611030:	e3520000 	cmp	r2, #0
81611034:	1affffee 	bne	81610ff4 <himport_r+0x264>
81611038:	e3a02000 	mov	r2, #0
8161103c:	e2836001 	add	r6, r3, #1
81611040:	e5c32000 	strb	r2, [r3]
81611044:	e59b300c 	ldr	r3, [fp, #12]
81611048:	e1530002 	cmp	r3, r2
8161104c:	1a000004 	bne	81611064 <himport_r+0x2d4>
81611050:	e59b2004 	ldr	r2, [fp, #4]
81611054:	e1a01007 	mov	r1, r7
81611058:	e1a00004 	mov	r0, r4
8161105c:	ebfffe49 	bl	81610988 <hdelete_r>
81611060:	eaffffae 	b	81610f20 <himport_r+0x190>
81611064:	e51b2038 	ldr	r2, [fp, #-56]	; 0xffffffc8
81611068:	e1a00004 	mov	r0, r4
8161106c:	e59b100c 	ldr	r1, [fp, #12]
81611070:	ebfffc8f 	bl	816102b4 <drop_var_from_set.part.1>
81611074:	e3500000 	cmp	r0, #0
81611078:	0affffa8 	beq	81610f20 <himport_r+0x190>
8161107c:	eafffff3 	b	81611050 <himport_r+0x2c0>
81611080:	e5c31000 	strb	r1, [r3]
81611084:	e1a0300a 	mov	r3, sl
81611088:	e1a0100a 	mov	r1, sl
8161108c:	e1a06003 	mov	r6, r3
81611090:	e4d62001 	ldrb	r2, [r6], #1
81611094:	e1520008 	cmp	r2, r8
81611098:	13520000 	cmpne	r2, #0
8161109c:	13a00001 	movne	r0, #1
816110a0:	03a00000 	moveq	r0, #0
816110a4:	1a00000c 	bne	816110dc <himport_r+0x34c>
816110a8:	e5c10000 	strb	r0, [r1]
816110ac:	e5d43000 	ldrb	r3, [r4]
816110b0:	e3530000 	cmp	r3, #0
816110b4:	1a000010 	bne	816110fc <himport_r+0x36c>
816110b8:	e59f20b4 	ldr	r2, [pc, #180]	; 81611174 <himport_r+0x3e4>
816110bc:	e1a00005 	mov	r0, r5
816110c0:	e50b3038 	str	r3, [fp, #-56]	; 0xffffffc8
816110c4:	e3a01016 	mov	r1, #22
816110c8:	e5821000 	str	r1, [r2]
816110cc:	ebffd859 	bl	81607238 <free>
816110d0:	e51b3038 	ldr	r3, [fp, #-56]	; 0xffffffc8
816110d4:	e1a04003 	mov	r4, r3
816110d8:	eaffff3d 	b	81610dd4 <himport_r+0x44>
816110dc:	e352005c 	cmp	r2, #92	; 0x5c
816110e0:	1a000002 	bne	816110f0 <himport_r+0x360>
816110e4:	e5d32001 	ldrb	r2, [r3, #1]
816110e8:	e3520000 	cmp	r2, #0
816110ec:	11a03006 	movne	r3, r6
816110f0:	e4d32001 	ldrb	r2, [r3], #1
816110f4:	e4c12001 	strb	r2, [r1], #1
816110f8:	eaffffe3 	b	8161108c <himport_r+0x2fc>
816110fc:	e59b300c 	ldr	r3, [fp, #12]
81611100:	e3530000 	cmp	r3, #0
81611104:	1a000013 	bne	81611158 <himport_r+0x3c8>
81611108:	e59b3004 	ldr	r3, [fp, #4]
8161110c:	e50b402c 	str	r4, [fp, #-44]	; 0xffffffd4
81611110:	e50ba028 	str	sl, [fp, #-40]	; 0xffffffd8
81611114:	e58d300c 	str	r3, [sp, #12]
81611118:	e24b3030 	sub	r3, fp, #48	; 0x30
8161111c:	e58d3004 	str	r3, [sp, #4]
81611120:	e3a03001 	mov	r3, #1
81611124:	e58d3000 	str	r3, [sp]
81611128:	e24b301c 	sub	r3, fp, #28
8161112c:	e58d7008 	str	r7, [sp, #8]
81611130:	e913000f 	ldmdb	r3, {r0, r1, r2, r3}
81611134:	ebfffcf2 	bl	81610504 <hsearch_r>
81611138:	e51b3030 	ldr	r3, [fp, #-48]	; 0xffffffd0
8161113c:	e3530000 	cmp	r3, #0
81611140:	1affff76 	bne	81610f20 <himport_r+0x190>
81611144:	e1a0200a 	mov	r2, sl
81611148:	e1a01004 	mov	r1, r4
8161114c:	e59f002c 	ldr	r0, [pc, #44]	; 81611180 <himport_r+0x3f0>
81611150:	eb000549 	bl	8161267c <printf>
81611154:	eaffff71 	b	81610f20 <himport_r+0x190>
81611158:	e51b2038 	ldr	r2, [fp, #-56]	; 0xffffffc8
8161115c:	e1a00004 	mov	r0, r4
81611160:	e59b100c 	ldr	r1, [fp, #12]
81611164:	ebfffc52 	bl	816102b4 <drop_var_from_set.part.1>
81611168:	e3500000 	cmp	r0, #0
8161116c:	0affff6b 	beq	81610f20 <himport_r+0x190>
81611170:	eaffffe4 	b	81611108 <himport_r+0x378>
81611174:	81619e84 	.word	0x81619e84
81611178:	81616000 	.word	0x81616000
8161117c:	81615fc9 	.word	0x81615fc9
81611180:	81615f98 	.word	0x81615f98

81611184 <hwalk_r>:
81611184:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81611188:	e1a05000 	mov	r5, r0
8161118c:	e1a06001 	mov	r6, r1
81611190:	e3a04001 	mov	r4, #1
81611194:	e3a07014 	mov	r7, #20
81611198:	e5953004 	ldr	r3, [r5, #4]
8161119c:	e1530004 	cmp	r3, r4
816111a0:	2a000001 	bcs	816111ac <hwalk_r+0x28>
816111a4:	e3a00000 	mov	r0, #0
816111a8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
816111ac:	e0030497 	mul	r3, r7, r4
816111b0:	e5952000 	ldr	r2, [r5]
816111b4:	e0820003 	add	r0, r2, r3
816111b8:	e7923003 	ldr	r3, [r2, r3]
816111bc:	e3530000 	cmp	r3, #0
816111c0:	ca000001 	bgt	816111cc <hwalk_r+0x48>
816111c4:	e2844001 	add	r4, r4, #1
816111c8:	eafffff2 	b	81611198 <hwalk_r+0x14>
816111cc:	e2800004 	add	r0, r0, #4
816111d0:	e12fff36 	blx	r6
816111d4:	e3500000 	cmp	r0, #0
816111d8:	0afffff9 	beq	816111c4 <hwalk_r+0x40>
816111dc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

816111e0 <display_options_get_banner_priv>:
816111e0:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
816111e4:	e1a05003 	mov	r5, r3
816111e8:	e1a06002 	mov	r6, r2
816111ec:	e59f3070 	ldr	r3, [pc, #112]	; 81611264 <display_options_get_banner_priv+0x84>
816111f0:	e59f2070 	ldr	r2, [pc, #112]	; 81611268 <display_options_get_banner_priv+0x88>
816111f4:	e3500000 	cmp	r0, #0
816111f8:	e1a07001 	mov	r7, r1
816111fc:	e1a00006 	mov	r0, r6
81611200:	e58d3000 	str	r3, [sp]
81611204:	e1a01005 	mov	r1, r5
81611208:	e59f305c 	ldr	r3, [pc, #92]	; 8161126c <display_options_get_banner_priv+0x8c>
8161120c:	11a03002 	movne	r3, r2
81611210:	e59f2058 	ldr	r2, [pc, #88]	; 81611270 <display_options_get_banner_priv+0x90>
81611214:	eb000500 	bl	8161261c <snprintf>
81611218:	e3570000 	cmp	r7, #0
8161121c:	11550000 	cmpne	r5, r0
81611220:	e1a04000 	mov	r4, r0
81611224:	da000005 	ble	81611240 <display_options_get_banner_priv+0x60>
81611228:	e0451000 	sub	r1, r5, r0
8161122c:	e1a03007 	mov	r3, r7
81611230:	e59f203c 	ldr	r2, [pc, #60]	; 81611274 <display_options_get_banner_priv+0x94>
81611234:	e0860000 	add	r0, r6, r0
81611238:	eb0004f7 	bl	8161261c <snprintf>
8161123c:	e0844000 	add	r4, r4, r0
81611240:	e2453002 	sub	r3, r5, #2
81611244:	e59f101c 	ldr	r1, [pc, #28]	; 81611268 <display_options_get_banner_priv+0x88>
81611248:	e1530004 	cmp	r3, r4
8161124c:	d2454003 	suble	r4, r5, #3
81611250:	e0860004 	add	r0, r6, r4
81611254:	eb00016a 	bl	81611804 <strcpy>
81611258:	e1a00006 	mov	r0, r6
8161125c:	e28dd00c 	add	sp, sp, #12
81611260:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81611264:	81617bd4 	.word	0x81617bd4
81611268:	81616239 	.word	0x81616239
8161126c:	8161623b 	.word	0x8161623b
81611270:	81614b10 	.word	0x81614b10
81611274:	81614b15 	.word	0x81614b15

81611278 <display_options_get_banner>:
81611278:	e1a03002 	mov	r3, r2
8161127c:	e1a02001 	mov	r2, r1
81611280:	e3a01000 	mov	r1, #0
81611284:	eaffffd5 	b	816111e0 <display_options_get_banner_priv>

81611288 <display_options>:
81611288:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
8161128c:	e24dd0cc 	sub	sp, sp, #204	; 0xcc
81611290:	e3a020c8 	mov	r2, #200	; 0xc8
81611294:	e3a00001 	mov	r0, #1
81611298:	e1a0100d 	mov	r1, sp
8161129c:	ebfffff5 	bl	81611278 <display_options_get_banner>
816112a0:	e1a0100d 	mov	r1, sp
816112a4:	e59f000c 	ldr	r0, [pc, #12]	; 816112b8 <display_options+0x30>
816112a8:	eb0004f3 	bl	8161267c <printf>
816112ac:	e3a00000 	mov	r0, #0
816112b0:	e28dd0cc 	add	sp, sp, #204	; 0xcc
816112b4:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
816112b8:	81615e2c 	.word	0x81615e2c

816112bc <print_size>:
816112bc:	e92d4dff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, sl, fp, lr}
816112c0:	e1a03001 	mov	r3, r1
816112c4:	e1a07002 	mov	r7, r2
816112c8:	e3e01009 	mvn	r1, #9
816112cc:	e3a02000 	mov	r2, #0
816112d0:	e00a0291 	mul	sl, r1, r2
816112d4:	e28ac03c 	add	ip, sl, #60	; 0x3c
816112d8:	e28ae01c 	add	lr, sl, #28
816112dc:	e26c8020 	rsb	r8, ip, #32
816112e0:	e1a04c30 	lsr	r4, r0, ip
816112e4:	e1844813 	orr	r4, r4, r3, lsl r8
816112e8:	e1a05c33 	lsr	r5, r3, ip
816112ec:	e1844e33 	orr	r4, r4, r3, lsr lr
816112f0:	e1946005 	orrs	r6, r4, r5
816112f4:	0a000009 	beq	81611320 <print_size+0x64>
816112f8:	e59f10f0 	ldr	r1, [pc, #240]	; 816113f0 <print_size+0x134>
816112fc:	e7d16002 	ldrb	r6, [r1, r2]
81611300:	e3560000 	cmp	r6, #0
81611304:	1a000009 	bne	81611330 <print_size+0x74>
81611308:	e1a02000 	mov	r2, r0
8161130c:	e58d7000 	str	r7, [sp]
81611310:	e59f00dc 	ldr	r0, [pc, #220]	; 816113f4 <print_size+0x138>
81611314:	eb0004d8 	bl	8161267c <printf>
81611318:	e28dd010 	add	sp, sp, #16
8161131c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81611320:	e2822001 	add	r2, r2, #1
81611324:	e3520006 	cmp	r2, #6
81611328:	1affffe8 	bne	816112d0 <print_size+0x14>
8161132c:	eafffff5 	b	81611308 <print_size+0x4c>
81611330:	e3e0b000 	mvn	fp, #0
81611334:	e1a01004 	mov	r1, r4
81611338:	e1a02c1b 	lsl	r2, fp, ip
8161133c:	e1c00c1b 	bic	r0, r0, fp, lsl ip
81611340:	e1822e1b 	orr	r2, r2, fp, lsl lr
81611344:	e58d0008 	str	r0, [sp, #8]
81611348:	e182283b 	orr	r2, r2, fp, lsr r8
8161134c:	e3a04000 	mov	r4, #0
81611350:	e1c33002 	bic	r3, r3, r2
81611354:	e58d300c 	str	r3, [sp, #12]
81611358:	e1cd20d8 	ldrd	r2, [sp, #8]
8161135c:	e1923003 	orrs	r3, r2, r3
81611360:	e3a02000 	mov	r2, #0
81611364:	01a04002 	moveq	r4, r2
81611368:	0a000013 	beq	816113bc <print_size+0x100>
8161136c:	e3a04001 	mov	r4, #1
81611370:	e28a503b 	add	r5, sl, #59	; 0x3b
81611374:	e28aa01b 	add	sl, sl, #27
81611378:	e265b020 	rsb	fp, r5, #32
8161137c:	e1820a14 	orr	r0, r2, r4, lsl sl
81611380:	e1a03514 	lsl	r3, r4, r5
81611384:	e1800b34 	orr	r0, r0, r4, lsr fp
81611388:	e59d4008 	ldr	r4, [sp, #8]
8161138c:	e3a0200a 	mov	r2, #10
81611390:	e08ba294 	umull	sl, fp, r4, r2
81611394:	e59d400c 	ldr	r4, [sp, #12]
81611398:	e093300a 	adds	r3, r3, sl
8161139c:	e1a0cc33 	lsr	ip, r3, ip
816113a0:	e02bb492 	mla	fp, r2, r4, fp
816113a4:	e0a0200b 	adc	r2, r0, fp
816113a8:	e18cc812 	orr	ip, ip, r2, lsl r8
816113ac:	e18c4e32 	orr	r4, ip, r2, lsr lr
816113b0:	e3540009 	cmp	r4, #9
816113b4:	8244400a 	subhi	r4, r4, #10
816113b8:	82811001 	addhi	r1, r1, #1
816113bc:	e59f0034 	ldr	r0, [pc, #52]	; 816113f8 <print_size+0x13c>
816113c0:	eb0004ad 	bl	8161267c <printf>
816113c4:	e3540000 	cmp	r4, #0
816113c8:	0a000002 	beq	816113d8 <print_size+0x11c>
816113cc:	e1a01004 	mov	r1, r4
816113d0:	e59f0024 	ldr	r0, [pc, #36]	; 816113fc <print_size+0x140>
816113d4:	eb0004a8 	bl	8161267c <printf>
816113d8:	e1a02007 	mov	r2, r7
816113dc:	e1a01006 	mov	r1, r6
816113e0:	e59f0018 	ldr	r0, [pc, #24]	; 81611400 <print_size+0x144>
816113e4:	e28dd010 	add	sp, sp, #16
816113e8:	e8bd4df0 	pop	{r4, r5, r6, r7, r8, sl, fp, lr}
816113ec:	ea0004a2 	b	8161267c <printf>
816113f0:	81616574 	.word	0x81616574
816113f4:	816166f2 	.word	0x816166f2
816113f8:	816166bf 	.word	0x816166bf
816113fc:	816166c3 	.word	0x816166c3
81611400:	816166ff 	.word	0x816166ff

81611404 <print_buffer>:
81611404:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81611408:	e24dd058 	sub	sp, sp, #88	; 0x58
8161140c:	e1a04003 	mov	r4, r3
81611410:	e1a0b001 	mov	fp, r1
81611414:	e59d5078 	ldr	r5, [sp, #120]	; 0x78
81611418:	e1a06002 	mov	r6, r2
8161141c:	e58d0004 	str	r0, [sp, #4]
81611420:	e0030592 	mul	r3, r2, r5
81611424:	e3530040 	cmp	r3, #64	; 0x40
81611428:	9a000003 	bls	8161143c <print_buffer+0x38>
8161142c:	e1a01002 	mov	r1, r2
81611430:	e3a00040 	mov	r0, #64	; 0x40
81611434:	eb0005b9 	bl	81612b20 <__aeabi_uidiv>
81611438:	e1a05000 	mov	r5, r0
8161143c:	e3550000 	cmp	r5, #0
81611440:	1a000003 	bne	81611454 <print_buffer+0x50>
81611444:	e1a01006 	mov	r1, r6
81611448:	e3a00010 	mov	r0, #16
8161144c:	eb0005b3 	bl	81612b20 <__aeabi_uidiv>
81611450:	e1a05000 	mov	r5, r0
81611454:	e1a03086 	lsl	r3, r6, #1
81611458:	e58d3008 	str	r3, [sp, #8]
8161145c:	e2833001 	add	r3, r3, #1
81611460:	e58d300c 	str	r3, [sp, #12]
81611464:	e3540000 	cmp	r4, #0
81611468:	1a000002 	bne	81611478 <print_buffer+0x74>
8161146c:	e1a00004 	mov	r0, r4
81611470:	e28dd058 	add	sp, sp, #88	; 0x58
81611474:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81611478:	e59d1004 	ldr	r1, [sp, #4]
8161147c:	e1a0800b 	mov	r8, fp
81611480:	e59f0114 	ldr	r0, [pc, #276]	; 8161159c <print_buffer+0x198>
81611484:	e3a07000 	mov	r7, #0
81611488:	eb00047b 	bl	8161267c <printf>
8161148c:	e1540005 	cmp	r4, r5
81611490:	31a0a004 	movcc	sl, r4
81611494:	21a0a005 	movcs	sl, r5
81611498:	e157000a 	cmp	r7, sl
8161149c:	e28d3014 	add	r3, sp, #20
816114a0:	1a000018 	bne	81611508 <print_buffer+0x104>
816114a4:	e007069a 	mul	r7, sl, r6
816114a8:	e08bb007 	add	fp, fp, r7
816114ac:	e155000a 	cmp	r5, sl
816114b0:	8a00002e 	bhi	81611570 <print_buffer+0x16c>
816114b4:	e59f00e4 	ldr	r0, [pc, #228]	; 816115a0 <print_buffer+0x19c>
816114b8:	e28d1014 	add	r1, sp, #20
816114bc:	e3a03000 	mov	r3, #0
816114c0:	e3a0c02e 	mov	ip, #46	; 0x2e
816114c4:	e1570003 	cmp	r7, r3
816114c8:	1a00002a 	bne	81611578 <print_buffer+0x174>
816114cc:	e28d3058 	add	r3, sp, #88	; 0x58
816114d0:	e59f00cc 	ldr	r0, [pc, #204]	; 816115a4 <print_buffer+0x1a0>
816114d4:	e0833007 	add	r3, r3, r7
816114d8:	e3a02000 	mov	r2, #0
816114dc:	e044400a 	sub	r4, r4, sl
816114e0:	e5432044 	strb	r2, [r3, #-68]	; 0xffffffbc
816114e4:	eb000464 	bl	8161267c <printf>
816114e8:	e59d3004 	ldr	r3, [sp, #4]
816114ec:	e0833007 	add	r3, r3, r7
816114f0:	e58d3004 	str	r3, [sp, #4]
816114f4:	ebffd62d 	bl	81606db0 <ctrlc>
816114f8:	e3500000 	cmp	r0, #0
816114fc:	0affffd8 	beq	81611464 <print_buffer+0x60>
81611500:	e3e00000 	mvn	r0, #0
81611504:	eaffffd9 	b	81611470 <print_buffer+0x6c>
81611508:	e3560004 	cmp	r6, #4
8161150c:	05982000 	ldreq	r2, [r8]
81611510:	07832107 	streq	r2, [r3, r7, lsl #2]
81611514:	0a000007 	beq	81611538 <print_buffer+0x134>
81611518:	e3560002 	cmp	r6, #2
8161151c:	01a01087 	lsleq	r1, r7, #1
81611520:	01d820b0 	ldrheq	r2, [r8]
81611524:	15d82000 	ldrbne	r2, [r8]
81611528:	06ff2072 	uxtheq	r2, r2
8161152c:	018320b1 	strheq	r2, [r3, r1]
81611530:	16ef2072 	uxtbne	r2, r2
81611534:	17c32007 	strbne	r2, [r3, r7]
81611538:	e59d1008 	ldr	r1, [sp, #8]
8161153c:	e0888006 	add	r8, r8, r6
81611540:	e59f0060 	ldr	r0, [pc, #96]	; 816115a8 <print_buffer+0x1a4>
81611544:	e2877001 	add	r7, r7, #1
81611548:	eb00044b 	bl	8161267c <printf>
8161154c:	eaffffd1 	b	81611498 <print_buffer+0x94>
81611550:	e59f0054 	ldr	r0, [pc, #84]	; 816115ac <print_buffer+0x1a8>
81611554:	e2888001 	add	r8, r8, #1
81611558:	ebffd60d 	bl	81606d94 <puts>
8161155c:	e59d300c 	ldr	r3, [sp, #12]
81611560:	e1530008 	cmp	r3, r8
81611564:	8afffff9 	bhi	81611550 <print_buffer+0x14c>
81611568:	e2455001 	sub	r5, r5, #1
8161156c:	eaffffce 	b	816114ac <print_buffer+0xa8>
81611570:	e3a08000 	mov	r8, #0
81611574:	eafffff8 	b	8161155c <print_buffer+0x158>
81611578:	e7d12003 	ldrb	r2, [r1, r3]
8161157c:	e7d0e002 	ldrb	lr, [r0, r2]
81611580:	e31e0097 	tst	lr, #151	; 0x97
81611584:	0a000001 	beq	81611590 <print_buffer+0x18c>
81611588:	e3120080 	tst	r2, #128	; 0x80
8161158c:	0a000000 	beq	81611594 <print_buffer+0x190>
81611590:	e7c1c003 	strb	ip, [r1, r3]
81611594:	e2833001 	add	r3, r3, #1
81611598:	eaffffc9 	b	816114c4 <print_buffer+0xc0>
8161159c:	81616547 	.word	0x81616547
816115a0:	816140e1 	.word	0x816140e1
816115a4:	81616684 	.word	0x81616684
816115a8:	8161667e 	.word	0x8161667e
816115ac:	81616122 	.word	0x81616122

816115b0 <crc32_no_comp>:
816115b0:	e3110003 	tst	r1, #3
816115b4:	e92d4030 	push	{r4, r5, lr}
816115b8:	0a00000b 	beq	816115ec <crc32_no_comp+0x3c>
816115bc:	e3520000 	cmp	r2, #0
816115c0:	0a000009 	beq	816115ec <crc32_no_comp+0x3c>
816115c4:	e59fc0b4 	ldr	ip, [pc, #180]	; 81611680 <crc32_no_comp+0xd0>
816115c8:	e4d13001 	ldrb	r3, [r1], #1
816115cc:	e2522001 	subs	r2, r2, #1
816115d0:	e0233000 	eor	r3, r3, r0
816115d4:	e6ef3073 	uxtb	r3, r3
816115d8:	e79c3103 	ldr	r3, [ip, r3, lsl #2]
816115dc:	e0230420 	eor	r0, r3, r0, lsr #8
816115e0:	0a000001 	beq	816115ec <crc32_no_comp+0x3c>
816115e4:	e3110003 	tst	r1, #3
816115e8:	1afffff6 	bne	816115c8 <crc32_no_comp+0x18>
816115ec:	e2023003 	and	r3, r2, #3
816115f0:	e59fc088 	ldr	ip, [pc, #136]	; 81611680 <crc32_no_comp+0xd0>
816115f4:	e3c22003 	bic	r2, r2, #3
816115f8:	e1a04001 	mov	r4, r1
816115fc:	e0825001 	add	r5, r2, r1
81611600:	e1550004 	cmp	r5, r4
81611604:	1a00000e 	bne	81611644 <crc32_no_comp+0x94>
81611608:	e3530000 	cmp	r3, #0
8161160c:	e2422004 	sub	r2, r2, #4
81611610:	e0811002 	add	r1, r1, r2
81611614:	08bd8030 	popeq	{r4, r5, pc}
81611618:	e2811003 	add	r1, r1, #3
8161161c:	e59fc05c 	ldr	ip, [pc, #92]	; 81611680 <crc32_no_comp+0xd0>
81611620:	e0812003 	add	r2, r1, r3
81611624:	e5f13001 	ldrb	r3, [r1, #1]!
81611628:	e0233000 	eor	r3, r3, r0
8161162c:	e1510002 	cmp	r1, r2
81611630:	e6ef3073 	uxtb	r3, r3
81611634:	e79c3103 	ldr	r3, [ip, r3, lsl #2]
81611638:	e0230420 	eor	r0, r3, r0, lsr #8
8161163c:	1afffff8 	bne	81611624 <crc32_no_comp+0x74>
81611640:	e8bd8030 	pop	{r4, r5, pc}
81611644:	e494e004 	ldr	lr, [r4], #4
81611648:	e020000e 	eor	r0, r0, lr
8161164c:	e6efe070 	uxtb	lr, r0
81611650:	e79ce10e 	ldr	lr, [ip, lr, lsl #2]
81611654:	e02ee420 	eor	lr, lr, r0, lsr #8
81611658:	e6ef007e 	uxtb	r0, lr
8161165c:	e79c0100 	ldr	r0, [ip, r0, lsl #2]
81611660:	e020e42e 	eor	lr, r0, lr, lsr #8
81611664:	e6ef007e 	uxtb	r0, lr
81611668:	e79c0100 	ldr	r0, [ip, r0, lsl #2]
8161166c:	e020042e 	eor	r0, r0, lr, lsr #8
81611670:	e6efe070 	uxtb	lr, r0
81611674:	e79ce10e 	ldr	lr, [ip, lr, lsl #2]
81611678:	e02e0420 	eor	r0, lr, r0, lsr #8
8161167c:	eaffffdf 	b	81611600 <crc32_no_comp+0x50>
81611680:	81612dac 	.word	0x81612dac

81611684 <crc32>:
81611684:	e92d4010 	push	{r4, lr}
81611688:	e1e00000 	mvn	r0, r0
8161168c:	ebffffc7 	bl	816115b0 <crc32_no_comp>
81611690:	e1e00000 	mvn	r0, r0
81611694:	e8bd8010 	pop	{r4, pc}

81611698 <crc32_wd>:
81611698:	eafffff9 	b	81611684 <crc32>

8161169c <__div64_32>:
8161169c:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816116a0:	e1a04001 	mov	r4, r1
816116a4:	e1c060d0 	ldrd	r6, [r0]
816116a8:	e1a08000 	mov	r8, r0
816116ac:	e3a05000 	mov	r5, #0
816116b0:	e1510007 	cmp	r1, r7
816116b4:	83a01000 	movhi	r1, #0
816116b8:	81a0c001 	movhi	ip, r1
816116bc:	8a000007 	bhi	816116e0 <__div64_32+0x44>
816116c0:	e1a00007 	mov	r0, r7
816116c4:	e1a0a001 	mov	sl, r1
816116c8:	eb000514 	bl	81612b20 <__aeabi_uidiv>
816116cc:	e3a01000 	mov	r1, #0
816116d0:	e0566001 	subs	r6, r6, r1
816116d4:	e0020a90 	mul	r2, r0, sl
816116d8:	e1a0c000 	mov	ip, r0
816116dc:	e0c77002 	sbc	r7, r7, r2
816116e0:	e3a02001 	mov	r2, #1
816116e4:	e3a03000 	mov	r3, #0
816116e8:	ea000003 	b	816116fc <__div64_32+0x60>
816116ec:	e0944004 	adds	r4, r4, r4
816116f0:	e0a55005 	adc	r5, r5, r5
816116f4:	e0922002 	adds	r2, r2, r2
816116f8:	e0a33003 	adc	r3, r3, r3
816116fc:	e3540001 	cmp	r4, #1
81611700:	e2d50000 	sbcs	r0, r5, #0
81611704:	a3a00001 	movge	r0, #1
81611708:	b3a00000 	movlt	r0, #0
8161170c:	e1550007 	cmp	r5, r7
81611710:	01540006 	cmpeq	r4, r6
81611714:	32000001 	andcc	r0, r0, #1
81611718:	23a00000 	movcs	r0, #0
8161171c:	e3500000 	cmp	r0, #0
81611720:	1afffff1 	bne	816116ec <__div64_32+0x50>
81611724:	e1570005 	cmp	r7, r5
81611728:	01560004 	cmpeq	r6, r4
8161172c:	3a000003 	bcc	81611740 <__div64_32+0xa4>
81611730:	e0566004 	subs	r6, r6, r4
81611734:	e0c77005 	sbc	r7, r7, r5
81611738:	e0911002 	adds	r1, r1, r2
8161173c:	e0acc003 	adc	ip, ip, r3
81611740:	e1b030a3 	lsrs	r3, r3, #1
81611744:	e1a02062 	rrx	r2, r2
81611748:	e1b050a5 	lsrs	r5, r5, #1
8161174c:	e1a04064 	rrx	r4, r4
81611750:	e1920003 	orrs	r0, r2, r3
81611754:	1afffff2 	bne	81611724 <__div64_32+0x88>
81611758:	e8881002 	stm	r8, {r1, ip}
8161175c:	e1a00006 	mov	r0, r6
81611760:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}

81611764 <hang>:
81611764:	e59f0010 	ldr	r0, [pc, #16]	; 8161177c <hang+0x18>
81611768:	e92d4010 	push	{r4, lr}
8161176c:	ebffd588 	bl	81606d94 <puts>
81611770:	e3e0001d 	mvn	r0, #29
81611774:	ebffcbc6 	bl	81604694 <show_boot_progress>
81611778:	eafffffe 	b	81611778 <hang+0x14>
8161177c:	81615f19 	.word	0x81615f19

81611780 <skip_spaces>:
81611780:	e59f101c 	ldr	r1, [pc, #28]	; 816117a4 <skip_spaces+0x24>
81611784:	e1a03000 	mov	r3, r0
81611788:	e1a00003 	mov	r0, r3
8161178c:	e2833001 	add	r3, r3, #1
81611790:	e5d02000 	ldrb	r2, [r0]
81611794:	e7d12002 	ldrb	r2, [r1, r2]
81611798:	e3120020 	tst	r2, #32
8161179c:	1afffff9 	bne	81611788 <skip_spaces+0x8>
816117a0:	e12fff1e 	bx	lr
816117a4:	816140e1 	.word	0x816140e1

816117a8 <strim>:
816117a8:	e92d4010 	push	{r4, lr}
816117ac:	ebfffff3 	bl	81611780 <skip_spaces>
816117b0:	e1a04000 	mov	r4, r0
816117b4:	eb000046 	bl	816118d4 <strlen>
816117b8:	e3500000 	cmp	r0, #0
816117bc:	0a000007 	beq	816117e0 <strim+0x38>
816117c0:	e2400001 	sub	r0, r0, #1
816117c4:	e59f1034 	ldr	r1, [pc, #52]	; 81611800 <strim+0x58>
816117c8:	e0840000 	add	r0, r4, r0
816117cc:	e1500004 	cmp	r0, r4
816117d0:	e1a03000 	mov	r3, r0
816117d4:	2a000003 	bcs	816117e8 <strim+0x40>
816117d8:	e3a02000 	mov	r2, #0
816117dc:	e5c32001 	strb	r2, [r3, #1]
816117e0:	e1a00004 	mov	r0, r4
816117e4:	e8bd8010 	pop	{r4, pc}
816117e8:	e5d32000 	ldrb	r2, [r3]
816117ec:	e2400001 	sub	r0, r0, #1
816117f0:	e7d12002 	ldrb	r2, [r1, r2]
816117f4:	e3120020 	tst	r2, #32
816117f8:	1afffff3 	bne	816117cc <strim+0x24>
816117fc:	eafffff5 	b	816117d8 <strim+0x30>
81611800:	816140e1 	.word	0x816140e1

81611804 <strcpy>:
81611804:	e2402001 	sub	r2, r0, #1
81611808:	e4d13001 	ldrb	r3, [r1], #1
8161180c:	e3530000 	cmp	r3, #0
81611810:	e5e23001 	strb	r3, [r2, #1]!
81611814:	1afffffb 	bne	81611808 <strcpy+0x4>
81611818:	e12fff1e 	bx	lr

8161181c <strncpy>:
8161181c:	e240c001 	sub	ip, r0, #1
81611820:	e0812002 	add	r2, r1, r2
81611824:	e1510002 	cmp	r1, r2
81611828:	012fff1e 	bxeq	lr
8161182c:	e4d13001 	ldrb	r3, [r1], #1
81611830:	e3530000 	cmp	r3, #0
81611834:	e5ec3001 	strb	r3, [ip, #1]!
81611838:	1afffff9 	bne	81611824 <strncpy+0x8>
8161183c:	e12fff1e 	bx	lr

81611840 <strcmp>:
81611840:	e2402001 	sub	r2, r0, #1
81611844:	e5f23001 	ldrb	r3, [r2, #1]!
81611848:	e4d10001 	ldrb	r0, [r1], #1
8161184c:	e0430000 	sub	r0, r3, r0
81611850:	e6af0070 	sxtb	r0, r0
81611854:	e3500000 	cmp	r0, #0
81611858:	112fff1e 	bxne	lr
8161185c:	e3530000 	cmp	r3, #0
81611860:	1afffff7 	bne	81611844 <strcmp+0x4>
81611864:	e12fff1e 	bx	lr

81611868 <strncmp>:
81611868:	e2403001 	sub	r3, r0, #1
8161186c:	e0812002 	add	r2, r1, r2
81611870:	e1510002 	cmp	r1, r2
81611874:	1a000001 	bne	81611880 <strncmp+0x18>
81611878:	e3a00000 	mov	r0, #0
8161187c:	e12fff1e 	bx	lr
81611880:	e5f3c001 	ldrb	ip, [r3, #1]!
81611884:	e4d10001 	ldrb	r0, [r1], #1
81611888:	e04c0000 	sub	r0, ip, r0
8161188c:	e6af0070 	sxtb	r0, r0
81611890:	e3500000 	cmp	r0, #0
81611894:	112fff1e 	bxne	lr
81611898:	e35c0000 	cmp	ip, #0
8161189c:	1afffff3 	bne	81611870 <strncmp+0x8>
816118a0:	e12fff1e 	bx	lr

816118a4 <strchr>:
816118a4:	e6ef1071 	uxtb	r1, r1
816118a8:	e5d02000 	ldrb	r2, [r0]
816118ac:	e1a03000 	mov	r3, r0
816118b0:	e2800001 	add	r0, r0, #1
816118b4:	e1520001 	cmp	r2, r1
816118b8:	1a000001 	bne	816118c4 <strchr+0x20>
816118bc:	e1a00003 	mov	r0, r3
816118c0:	e12fff1e 	bx	lr
816118c4:	e3520000 	cmp	r2, #0
816118c8:	1afffff6 	bne	816118a8 <strchr+0x4>
816118cc:	e1a03002 	mov	r3, r2
816118d0:	eafffff9 	b	816118bc <strchr+0x18>

816118d4 <strlen>:
816118d4:	e1a03000 	mov	r3, r0
816118d8:	e5d32000 	ldrb	r2, [r3]
816118dc:	e2831001 	add	r1, r3, #1
816118e0:	e3520000 	cmp	r2, #0
816118e4:	1a000001 	bne	816118f0 <strlen+0x1c>
816118e8:	e0430000 	sub	r0, r3, r0
816118ec:	e12fff1e 	bx	lr
816118f0:	e1a03001 	mov	r3, r1
816118f4:	eafffff7 	b	816118d8 <strlen+0x4>

816118f8 <strnlen>:
816118f8:	e0801001 	add	r1, r0, r1
816118fc:	e1a03000 	mov	r3, r0
81611900:	e1530001 	cmp	r3, r1
81611904:	0a000003 	beq	81611918 <strnlen+0x20>
81611908:	e5d32000 	ldrb	r2, [r3]
8161190c:	e283c001 	add	ip, r3, #1
81611910:	e3520000 	cmp	r2, #0
81611914:	1a000001 	bne	81611920 <strnlen+0x28>
81611918:	e0430000 	sub	r0, r3, r0
8161191c:	e12fff1e 	bx	lr
81611920:	e1a0300c 	mov	r3, ip
81611924:	eafffff5 	b	81611900 <strnlen+0x8>

81611928 <strdup>:
81611928:	e92d4070 	push	{r4, r5, r6, lr}
8161192c:	e2504000 	subs	r4, r0, #0
81611930:	0a000007 	beq	81611954 <strdup+0x2c>
81611934:	ebffffe6 	bl	816118d4 <strlen>
81611938:	e1a05004 	mov	r5, r4
8161193c:	e2800001 	add	r0, r0, #1
81611940:	ebffd6c7 	bl	81607464 <malloc>
81611944:	e2504000 	subs	r4, r0, #0
81611948:	0a000001 	beq	81611954 <strdup+0x2c>
8161194c:	e1a01005 	mov	r1, r5
81611950:	ebffffab 	bl	81611804 <strcpy>
81611954:	e1a00004 	mov	r0, r4
81611958:	e8bd8070 	pop	{r4, r5, r6, pc}

8161195c <strpbrk>:
8161195c:	e1a02000 	mov	r2, r0
81611960:	e4d23001 	ldrb	r3, [r2], #1
81611964:	e3530000 	cmp	r3, #0
81611968:	1a00000e 	bne	816119a8 <strpbrk+0x4c>
8161196c:	e1a00003 	mov	r0, r3
81611970:	e12fff1e 	bx	lr
81611974:	e15e0003 	cmp	lr, r3
81611978:	049df004 	popeq	{pc}		; (ldreq pc, [sp], #4)
8161197c:	e5fce001 	ldrb	lr, [ip, #1]!
81611980:	e35e0000 	cmp	lr, #0
81611984:	1afffffa 	bne	81611974 <strpbrk+0x18>
81611988:	e1a00002 	mov	r0, r2
8161198c:	e4d23001 	ldrb	r3, [r2], #1
81611990:	e3530000 	cmp	r3, #0
81611994:	0a000001 	beq	816119a0 <strpbrk+0x44>
81611998:	e241c001 	sub	ip, r1, #1
8161199c:	eafffff6 	b	8161197c <strpbrk+0x20>
816119a0:	e1a00003 	mov	r0, r3
816119a4:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)
816119a8:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
816119ac:	eafffff9 	b	81611998 <strpbrk+0x3c>

816119b0 <memmove>:
816119b0:	e1500001 	cmp	r0, r1
816119b4:	e92d4010 	push	{r4, lr}
816119b8:	e1a04000 	mov	r4, r0
816119bc:	8a000002 	bhi	816119cc <memmove+0x1c>
816119c0:	ebffbb75 	bl	8160079c <memcpy>
816119c4:	e1a00004 	mov	r0, r4
816119c8:	e8bd8010 	pop	{r4, pc}
816119cc:	e1a0c002 	mov	ip, r2
816119d0:	e081100c 	add	r1, r1, ip
816119d4:	e0802002 	add	r2, r0, r2
816119d8:	e04cc001 	sub	ip, ip, r1
816119dc:	e171000c 	cmn	r1, ip
816119e0:	0afffff7 	beq	816119c4 <memmove+0x14>
816119e4:	e5713001 	ldrb	r3, [r1, #-1]!
816119e8:	e5623001 	strb	r3, [r2, #-1]!
816119ec:	eafffffa 	b	816119dc <memmove+0x2c>

816119f0 <memcmp>:
816119f0:	e3a0c000 	mov	ip, #0
816119f4:	e15c0002 	cmp	ip, r2
816119f8:	1a000002 	bne	81611a08 <memcmp+0x18>
816119fc:	e3a03000 	mov	r3, #0
81611a00:	e1a00003 	mov	r0, r3
81611a04:	e12fff1e 	bx	lr
81611a08:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
81611a0c:	e7d0300c 	ldrb	r3, [r0, ip]
81611a10:	e7d1e00c 	ldrb	lr, [r1, ip]
81611a14:	e28cc001 	add	ip, ip, #1
81611a18:	e053300e 	subs	r3, r3, lr
81611a1c:	1a000002 	bne	81611a2c <memcmp+0x3c>
81611a20:	e15c0002 	cmp	ip, r2
81611a24:	1afffff8 	bne	81611a0c <memcmp+0x1c>
81611a28:	e3a03000 	mov	r3, #0
81611a2c:	e1a00003 	mov	r0, r3
81611a30:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

81611a34 <strstr>:
81611a34:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
81611a38:	e1a04000 	mov	r4, r0
81611a3c:	e1a00001 	mov	r0, r1
81611a40:	e1a07001 	mov	r7, r1
81611a44:	ebffffa2 	bl	816118d4 <strlen>
81611a48:	e2506000 	subs	r6, r0, #0
81611a4c:	0a00000e 	beq	81611a8c <strstr+0x58>
81611a50:	e1a00004 	mov	r0, r4
81611a54:	ebffff9e 	bl	816118d4 <strlen>
81611a58:	e0848000 	add	r8, r4, r0
81611a5c:	e1a05000 	mov	r5, r0
81611a60:	e1550006 	cmp	r5, r6
81611a64:	e0484005 	sub	r4, r8, r5
81611a68:	b3a04000 	movlt	r4, #0
81611a6c:	ba000006 	blt	81611a8c <strstr+0x58>
81611a70:	e1a02006 	mov	r2, r6
81611a74:	e1a01007 	mov	r1, r7
81611a78:	e1a00004 	mov	r0, r4
81611a7c:	e2455001 	sub	r5, r5, #1
81611a80:	ebffffda 	bl	816119f0 <memcmp>
81611a84:	e3500000 	cmp	r0, #0
81611a88:	1afffff4 	bne	81611a60 <strstr+0x2c>
81611a8c:	e1a00004 	mov	r0, r4
81611a90:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

81611a94 <memchr>:
81611a94:	e0802002 	add	r2, r0, r2
81611a98:	e6ef1071 	uxtb	r1, r1
81611a9c:	e1500002 	cmp	r0, r2
81611aa0:	1a000001 	bne	81611aac <memchr+0x18>
81611aa4:	e3a00000 	mov	r0, #0
81611aa8:	e12fff1e 	bx	lr
81611aac:	e1a03000 	mov	r3, r0
81611ab0:	e4d3c001 	ldrb	ip, [r3], #1
81611ab4:	e15c0001 	cmp	ip, r1
81611ab8:	012fff1e 	bxeq	lr
81611abc:	e1a00003 	mov	r0, r3
81611ac0:	eafffff5 	b	81611a9c <memchr+0x8>

81611ac4 <tick_to_time>:
81611ac4:	e3a0cffa 	mov	ip, #1000	; 0x3e8
81611ac8:	e3a03000 	mov	r3, #0
81611acc:	e92d40f0 	push	{r4, r5, r6, r7, lr}
81611ad0:	e0854c90 	umull	r4, r5, r0, ip
81611ad4:	e59fe060 	ldr	lr, [pc, #96]	; 81611b3c <tick_to_time+0x78>
81611ad8:	e02c519c 	mla	ip, ip, r1, r5
81611adc:	e59f105c 	ldr	r1, [pc, #92]	; 81611b40 <tick_to_time+0x7c>
81611ae0:	e0876194 	umull	r6, r7, r4, r1
81611ae4:	e1a02007 	mov	r2, r7
81611ae8:	e3a07000 	mov	r7, #0
81611aec:	e0a32c91 	umlal	r2, r3, r1, ip
81611af0:	e0810e94 	umull	r0, r1, r4, lr
81611af4:	e0924000 	adds	r4, r2, r0
81611af8:	e0b35001 	adcs	r5, r3, r1
81611afc:	23a06001 	movcs	r6, #1
81611b00:	33a06000 	movcc	r6, #0
81611b04:	e1a02005 	mov	r2, r5
81611b08:	e1963007 	orrs	r3, r6, r7
81611b0c:	e3a03000 	mov	r3, #0
81611b10:	e0a32c9e 	umlal	r2, r3, lr, ip
81611b14:	01a01007 	moveq	r1, r7
81611b18:	13a01001 	movne	r1, #1
81611b1c:	01a00006 	moveq	r0, r6
81611b20:	13a00000 	movne	r0, #0
81611b24:	e0900002 	adds	r0, r0, r2
81611b28:	e0a11003 	adc	r1, r1, r3
81611b2c:	e1a00920 	lsr	r0, r0, #18
81611b30:	e1800701 	orr	r0, r0, r1, lsl #14
81611b34:	e1a01921 	lsr	r1, r1, #18
81611b38:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81611b3c:	431bde82 	.word	0x431bde82
81611b40:	d7b634db 	.word	0xd7b634db

81611b44 <timer_read_counter>:
81611b44:	e59f300c 	ldr	r3, [pc, #12]	; 81611b58 <timer_read_counter+0x14>
81611b48:	e5930004 	ldr	r0, [r3, #4]
81611b4c:	f57ff05f 	dmb	sy
81611b50:	e1e00000 	mvn	r0, r0
81611b54:	e12fff1e 	bx	lr
81611b58:	13110000 	.word	0x13110000

81611b5c <get_ticks>:
81611b5c:	e92d4010 	push	{r4, lr}
81611b60:	ebfffff7 	bl	81611b44 <timer_read_counter>
81611b64:	e599207c 	ldr	r2, [r9, #124]	; 0x7c
81611b68:	e1520000 	cmp	r2, r0
81611b6c:	85992078 	ldrhi	r2, [r9, #120]	; 0x78
81611b70:	82822001 	addhi	r2, r2, #1
81611b74:	85892078 	strhi	r2, [r9, #120]	; 0x78
81611b78:	e589007c 	str	r0, [r9, #124]	; 0x7c
81611b7c:	e5991078 	ldr	r1, [r9, #120]	; 0x78
81611b80:	e599007c 	ldr	r0, [r9, #124]	; 0x7c
81611b84:	e8bd8010 	pop	{r4, pc}

81611b88 <timer_init>:
81611b88:	e3a00000 	mov	r0, #0
81611b8c:	e12fff1e 	bx	lr

81611b90 <get_timer>:
81611b90:	e92d4010 	push	{r4, lr}
81611b94:	e1a04000 	mov	r4, r0
81611b98:	ebffffef 	bl	81611b5c <get_ticks>
81611b9c:	ebffffc8 	bl	81611ac4 <tick_to_time>
81611ba0:	e0400004 	sub	r0, r0, r4
81611ba4:	e8bd8010 	pop	{r4, pc}

81611ba8 <__udelay>:
81611ba8:	e12fff1e 	bx	lr

81611bac <udelay>:
81611bac:	e92d4070 	push	{r4, r5, r6, lr}
81611bb0:	e1a04000 	mov	r4, r0
81611bb4:	e59f601c 	ldr	r6, [pc, #28]	; 81611bd8 <udelay+0x2c>
81611bb8:	e1540006 	cmp	r4, r6
81611bbc:	31a05004 	movcc	r5, r4
81611bc0:	21a05006 	movcs	r5, r6
81611bc4:	e1a00005 	mov	r0, r5
81611bc8:	ebfffff6 	bl	81611ba8 <__udelay>
81611bcc:	e0544005 	subs	r4, r4, r5
81611bd0:	1afffff8 	bne	81611bb8 <udelay+0xc>
81611bd4:	e8bd8070 	pop	{r4, r5, r6, pc}
81611bd8:	00989680 	.word	0x00989680
81611bdc:	00000000 	.word	0x00000000

81611be0 <put_dec>:
81611be0:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81611be4:	e1a01000 	mov	r1, r0
81611be8:	e59fe228 	ldr	lr, [pc, #552]	; 81611e18 <put_dec+0x238>
81611bec:	e1a06002 	mov	r6, r2
81611bf0:	e59f8224 	ldr	r8, [pc, #548]	; 81611e1c <put_dec+0x23c>
81611bf4:	e1a07003 	mov	r7, r3
81611bf8:	e28f3f82 	add	r3, pc, #520	; 0x208
81611bfc:	e1c320d0 	ldrd	r2, [r3]
81611c00:	e1a00006 	mov	r0, r6
81611c04:	e1570003 	cmp	r7, r3
81611c08:	01560002 	cmpeq	r6, r2
81611c0c:	8a000035 	bhi	81611ce8 <put_dec+0x108>
81611c10:	e7e35450 	ubfx	r5, r0, #8, #4
81611c14:	e7e36256 	ubfx	r6, r6, #4, #4
81611c18:	e1a04620 	lsr	r4, r0, #12
81611c1c:	e0863005 	add	r3, r6, r5
81611c20:	e0833004 	add	r3, r3, r4
81611c24:	e200000f 	and	r0, r0, #15
81611c28:	e3a02006 	mov	r2, #6
81611c2c:	e3a0c0cd 	mov	ip, #205	; 0xcd
81611c30:	e0200392 	mla	r0, r2, r3, r0
81611c34:	e3a0e00a 	mov	lr, #10
81611c38:	e0843184 	add	r3, r4, r4, lsl #3
81611c3c:	e0833006 	add	r3, r3, r6
81611c40:	e002009c 	mul	r2, ip, r0
81611c44:	e1a025a2 	lsr	r2, r2, #11
81611c48:	e060029e 	mls	r0, lr, r2, r0
81611c4c:	e2800030 	add	r0, r0, #48	; 0x30
81611c50:	e5c10000 	strb	r0, [r1]
81611c54:	e0850105 	add	r0, r5, r5, lsl #2
81611c58:	e0833000 	add	r3, r3, r0
81611c5c:	e0922003 	adds	r2, r2, r3
81611c60:	1a000001 	bne	81611c6c <put_dec+0x8c>
81611c64:	e2810001 	add	r0, r1, #1
81611c68:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81611c6c:	e003029c 	mul	r3, ip, r2
81611c70:	e1a035a3 	lsr	r3, r3, #11
81611c74:	e062239e 	mls	r2, lr, r3, r2
81611c78:	e0833085 	add	r3, r3, r5, lsl #1
81611c7c:	e2822030 	add	r2, r2, #48	; 0x30
81611c80:	e5c12001 	strb	r2, [r1, #1]
81611c84:	e1942003 	orrs	r2, r4, r3
81611c88:	1a000001 	bne	81611c94 <put_dec+0xb4>
81611c8c:	e2810002 	add	r0, r1, #2
81611c90:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81611c94:	e3a0200d 	mov	r2, #13
81611c98:	e0020392 	mul	r2, r2, r3
81611c9c:	e1a023a2 	lsr	r2, r2, #7
81611ca0:	e063329e 	mls	r3, lr, r2, r3
81611ca4:	e0922104 	adds	r2, r2, r4, lsl #2
81611ca8:	e2833030 	add	r3, r3, #48	; 0x30
81611cac:	e5c13002 	strb	r3, [r1, #2]
81611cb0:	1a000001 	bne	81611cbc <put_dec+0xdc>
81611cb4:	e2810003 	add	r0, r1, #3
81611cb8:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81611cbc:	e003029c 	mul	r3, ip, r2
81611cc0:	e1a035a3 	lsr	r3, r3, #11
81611cc4:	e3530000 	cmp	r3, #0
81611cc8:	e062239e 	mls	r2, lr, r3, r2
81611ccc:	02810004 	addeq	r0, r1, #4
81611cd0:	12833030 	addne	r3, r3, #48	; 0x30
81611cd4:	12810005 	addne	r0, r1, #5
81611cd8:	15c13004 	strbne	r3, [r1, #4]
81611cdc:	e2822030 	add	r2, r2, #48	; 0x30
81611ce0:	e5c12003 	strb	r2, [r1, #3]
81611ce4:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81611ce8:	e28f3e12 	add	r3, pc, #288	; 0x120
81611cec:	e1c320d0 	ldrd	r2, [r3]
81611cf0:	e3a05000 	mov	r5, #0
81611cf4:	e1a0c007 	mov	ip, r7
81611cf8:	e2811005 	add	r1, r1, #5
81611cfc:	e0a3269e 	umlal	r2, r3, lr, r6
81611d00:	e1a04003 	mov	r4, r3
81611d04:	e0a5479e 	umlal	r4, r5, lr, r7
81611d08:	e3a07000 	mov	r7, #0
81611d0c:	e0832896 	umull	r2, r3, r6, r8
81611d10:	e3a06000 	mov	r6, #0
81611d14:	e094a002 	adds	sl, r4, r2
81611d18:	e0b5b003 	adcs	fp, r5, r3
81611d1c:	23a06001 	movcs	r6, #1
81611d20:	23a07000 	movcs	r7, #0
81611d24:	e1a0200b 	mov	r2, fp
81611d28:	e1963007 	orrs	r3, r6, r7
81611d2c:	e3a03000 	mov	r3, #0
81611d30:	e0a32c98 	umlal	r2, r3, r8, ip
81611d34:	01a05007 	moveq	r5, r7
81611d38:	13a05001 	movne	r5, #1
81611d3c:	01a04006 	moveq	r4, r6
81611d40:	13a04000 	movne	r4, #0
81611d44:	e0924004 	adds	r4, r2, r4
81611d48:	e0a35005 	adc	r5, r3, r5
81611d4c:	e59f30cc 	ldr	r3, [pc, #204]	; 81611e20 <put_dec+0x240>
81611d50:	e1a06824 	lsr	r6, r4, #16
81611d54:	e3a0c00a 	mov	ip, #10
81611d58:	e1866805 	orr	r6, r6, r5, lsl #16
81611d5c:	e1a07825 	lsr	r7, r5, #16
81611d60:	e0630693 	mls	r3, r3, r6, r0
81611d64:	e7e3b253 	ubfx	fp, r3, #4, #4
81611d68:	e7e35453 	ubfx	r5, r3, #8, #4
81611d6c:	e08b0005 	add	r0, fp, r5
81611d70:	e1a04623 	lsr	r4, r3, #12
81611d74:	e0802004 	add	r2, r0, r4
81611d78:	e203000f 	and	r0, r3, #15
81611d7c:	e3a03006 	mov	r3, #6
81611d80:	e0200293 	mla	r0, r3, r2, r0
81611d84:	e3a020cd 	mov	r2, #205	; 0xcd
81611d88:	e0843184 	add	r3, r4, r4, lsl #3
81611d8c:	e083300b 	add	r3, r3, fp
81611d90:	e00a0092 	mul	sl, r2, r0
81611d94:	e1a0a5aa 	lsr	sl, sl, #11
81611d98:	e0600a9c 	mls	r0, ip, sl, r0
81611d9c:	e2800030 	add	r0, r0, #48	; 0x30
81611da0:	e5410005 	strb	r0, [r1, #-5]
81611da4:	e0850105 	add	r0, r5, r5, lsl #2
81611da8:	e0833000 	add	r3, r3, r0
81611dac:	e08aa003 	add	sl, sl, r3
81611db0:	e3a0300d 	mov	r3, #13
81611db4:	e00b0a92 	mul	fp, r2, sl
81611db8:	e1a0b5ab 	lsr	fp, fp, #11
81611dbc:	e08b5085 	add	r5, fp, r5, lsl #1
81611dc0:	e06aab9c 	mls	sl, ip, fp, sl
81611dc4:	e0030593 	mul	r3, r3, r5
81611dc8:	e28aa030 	add	sl, sl, #48	; 0x30
81611dcc:	e541a004 	strb	sl, [r1, #-4]
81611dd0:	e1a033a3 	lsr	r3, r3, #7
81611dd4:	e0834104 	add	r4, r3, r4, lsl #2
81611dd8:	e065539c 	mls	r5, ip, r3, r5
81611ddc:	e0020492 	mul	r2, r2, r4
81611de0:	e2855030 	add	r5, r5, #48	; 0x30
81611de4:	e5415003 	strb	r5, [r1, #-3]
81611de8:	e1a025a2 	lsr	r2, r2, #11
81611dec:	e06c429c 	mls	ip, ip, r2, r4
81611df0:	e2822030 	add	r2, r2, #48	; 0x30
81611df4:	e28cc030 	add	ip, ip, #48	; 0x30
81611df8:	e541c002 	strb	ip, [r1, #-2]
81611dfc:	e5412001 	strb	r2, [r1, #-1]
81611e00:	eaffff7c 	b	81611bf8 <put_dec+0x18>
81611e04:	e320f000 	nop	{0}
81611e08:	0001869f 	.word	0x0001869f
81611e0c:	00000000 	.word	0x00000000
81611e10:	1b478423 	.word	0x1b478423
81611e14:	a7c5ac47 	.word	0xa7c5ac47
81611e18:	1b478423 	.word	0x1b478423
81611e1c:	a7c5ac47 	.word	0xa7c5ac47
81611e20:	000186a0 	.word	0x000186a0

81611e24 <number>:
81611e24:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81611e28:	e24dd060 	sub	sp, sp, #96	; 0x60
81611e2c:	e1a0a001 	mov	sl, r1
81611e30:	e1a04000 	mov	r4, r0
81611e34:	e59db080 	ldr	fp, [sp, #128]	; 0x80
81611e38:	e59d708c 	ldr	r7, [sp, #140]	; 0x8c
81611e3c:	e25b100a 	subs	r1, fp, #10
81611e40:	e59d5084 	ldr	r5, [sp, #132]	; 0x84
81611e44:	13a01001 	movne	r1, #1
81611e48:	e3170010 	tst	r7, #16
81611e4c:	e0011327 	and	r1, r1, r7, lsr #6
81611e50:	e58d1008 	str	r1, [sp, #8]
81611e54:	e2071020 	and	r1, r7, #32
81611e58:	13c77001 	bicne	r7, r7, #1
81611e5c:	e2178002 	ands	r8, r7, #2
81611e60:	e58d1004 	str	r1, [sp, #4]
81611e64:	0a000006 	beq	81611e84 <number+0x60>
81611e68:	e3520000 	cmp	r2, #0
81611e6c:	e2d31000 	sbcs	r1, r3, #0
81611e70:	aa00002b 	bge	81611f24 <number+0x100>
81611e74:	e2722000 	rsbs	r2, r2, #0
81611e78:	e2455001 	sub	r5, r5, #1
81611e7c:	e2e33000 	rsc	r3, r3, #0
81611e80:	e3a0802d 	mov	r8, #45	; 0x2d
81611e84:	e59d1008 	ldr	r1, [sp, #8]
81611e88:	e3510000 	cmp	r1, #0
81611e8c:	0a00002c 	beq	81611f44 <number+0x120>
81611e90:	e35b0010 	cmp	fp, #16
81611e94:	12455001 	subne	r5, r5, #1
81611e98:	02455002 	subeq	r5, r5, #2
81611e9c:	e1921003 	orrs	r1, r2, r3
81611ea0:	0a000029 	beq	81611f4c <number+0x128>
81611ea4:	e35b0010 	cmp	fp, #16
81611ea8:	e3a06000 	mov	r6, #0
81611eac:	e24b1001 	sub	r1, fp, #1
81611eb0:	e58d100c 	str	r1, [sp, #12]
81611eb4:	13a0c003 	movne	ip, #3
81611eb8:	03a0c004 	moveq	ip, #4
81611ebc:	e26c1020 	rsb	r1, ip, #32
81611ec0:	e58d1010 	str	r1, [sp, #16]
81611ec4:	e24c1020 	sub	r1, ip, #32
81611ec8:	e58d1014 	str	r1, [sp, #20]
81611ecc:	e28d001c 	add	r0, sp, #28
81611ed0:	e59d100c 	ldr	r1, [sp, #12]
81611ed4:	e2866001 	add	r6, r6, #1
81611ed8:	e080e006 	add	lr, r0, r6
81611edc:	e6ef0072 	uxtb	r0, r2
81611ee0:	e0000001 	and	r0, r0, r1
81611ee4:	e59f11f8 	ldr	r1, [pc, #504]	; 816120e4 <number+0x2c0>
81611ee8:	e7d10000 	ldrb	r0, [r1, r0]
81611eec:	e59d1004 	ldr	r1, [sp, #4]
81611ef0:	e1810000 	orr	r0, r1, r0
81611ef4:	e59d1010 	ldr	r1, [sp, #16]
81611ef8:	e54e0001 	strb	r0, [lr, #-1]
81611efc:	e1a00c32 	lsr	r0, r2, ip
81611f00:	e1800113 	orr	r0, r0, r3, lsl r1
81611f04:	e59d1014 	ldr	r1, [sp, #20]
81611f08:	e1a0ec33 	lsr	lr, r3, ip
81611f0c:	e1800133 	orr	r0, r0, r3, lsr r1
81611f10:	e1a0300e 	mov	r3, lr
81611f14:	e1a02000 	mov	r2, r0
81611f18:	e1921003 	orrs	r1, r2, r3
81611f1c:	1affffea 	bne	81611ecc <number+0xa8>
81611f20:	ea00000c 	b	81611f58 <number+0x134>
81611f24:	e3170004 	tst	r7, #4
81611f28:	12455001 	subne	r5, r5, #1
81611f2c:	13a0802b 	movne	r8, #43	; 0x2b
81611f30:	1affffd3 	bne	81611e84 <number+0x60>
81611f34:	e2178008 	ands	r8, r7, #8
81611f38:	12455001 	subne	r5, r5, #1
81611f3c:	13a08020 	movne	r8, #32
81611f40:	eaffffcf 	b	81611e84 <number+0x60>
81611f44:	e1921003 	orrs	r1, r2, r3
81611f48:	1a00002b 	bne	81611ffc <number+0x1d8>
81611f4c:	e3a06001 	mov	r6, #1
81611f50:	e3a03030 	mov	r3, #48	; 0x30
81611f54:	e5cd301c 	strb	r3, [sp, #28]
81611f58:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
81611f5c:	e1560003 	cmp	r6, r3
81611f60:	a1a03006 	movge	r3, r6
81611f64:	e3170011 	tst	r7, #17
81611f68:	e0450003 	sub	r0, r5, r3
81611f6c:	0a000033 	beq	81612040 <number+0x21c>
81611f70:	e3580000 	cmp	r8, #0
81611f74:	0a000002 	beq	81611f84 <number+0x160>
81611f78:	e154000a 	cmp	r4, sl
81611f7c:	e2844001 	add	r4, r4, #1
81611f80:	35448001 	strbcc	r8, [r4, #-1]
81611f84:	e59d2008 	ldr	r2, [sp, #8]
81611f88:	e3520000 	cmp	r2, #0
81611f8c:	0a00000b 	beq	81611fc0 <number+0x19c>
81611f90:	e154000a 	cmp	r4, sl
81611f94:	33a02030 	movcc	r2, #48	; 0x30
81611f98:	35c42000 	strbcc	r2, [r4]
81611f9c:	e2842001 	add	r2, r4, #1
81611fa0:	e35b0010 	cmp	fp, #16
81611fa4:	11a04002 	movne	r4, r2
81611fa8:	1a000004 	bne	81611fc0 <number+0x19c>
81611fac:	e15a0002 	cmp	sl, r2
81611fb0:	e2844002 	add	r4, r4, #2
81611fb4:	859d2004 	ldrhi	r2, [sp, #4]
81611fb8:	83822058 	orrhi	r2, r2, #88	; 0x58
81611fbc:	85442001 	strbhi	r2, [r4, #-1]
81611fc0:	e3170010 	tst	r7, #16
81611fc4:	1a00000a 	bne	81611ff4 <number+0x1d0>
81611fc8:	e3170001 	tst	r7, #1
81611fcc:	e1a01000 	mov	r1, r0
81611fd0:	e1a02004 	mov	r2, r4
81611fd4:	13a0c030 	movne	ip, #48	; 0x30
81611fd8:	03a0c020 	moveq	ip, #32
81611fdc:	e2511001 	subs	r1, r1, #1
81611fe0:	5a00001a 	bpl	81612050 <number+0x22c>
81611fe4:	e1c02fc0 	bic	r2, r0, r0, asr #31
81611fe8:	e2400001 	sub	r0, r0, #1
81611fec:	e0844002 	add	r4, r4, r2
81611ff0:	e0400002 	sub	r0, r0, r2
81611ff4:	e3a02030 	mov	r2, #48	; 0x30
81611ff8:	ea00001b 	b	8161206c <number+0x248>
81611ffc:	e35b000a 	cmp	fp, #10
81612000:	1affffa7 	bne	81611ea4 <number+0x80>
81612004:	e28d601c 	add	r6, sp, #28
81612008:	e1a00006 	mov	r0, r6
8161200c:	ebfffef3 	bl	81611be0 <put_dec>
81612010:	e0406006 	sub	r6, r0, r6
81612014:	eaffffcf 	b	81611f58 <number+0x134>
81612018:	e152000a 	cmp	r2, sl
8161201c:	e2822001 	add	r2, r2, #1
81612020:	3542e001 	strbcc	lr, [r2, #-1]
81612024:	e25cc001 	subs	ip, ip, #1
81612028:	5afffffa 	bpl	81612018 <number+0x1f4>
8161202c:	e1c02fc0 	bic	r2, r0, r0, asr #31
81612030:	e2400001 	sub	r0, r0, #1
81612034:	e0844002 	add	r4, r4, r2
81612038:	e0400002 	sub	r0, r0, r2
8161203c:	eaffffcb 	b	81611f70 <number+0x14c>
81612040:	e1a0c000 	mov	ip, r0
81612044:	e1a02004 	mov	r2, r4
81612048:	e3a0e020 	mov	lr, #32
8161204c:	eafffff4 	b	81612024 <number+0x200>
81612050:	e152000a 	cmp	r2, sl
81612054:	e2822001 	add	r2, r2, #1
81612058:	3542c001 	strbcc	ip, [r2, #-1]
8161205c:	eaffffde 	b	81611fdc <number+0x1b8>
81612060:	e154000a 	cmp	r4, sl
81612064:	e2844001 	add	r4, r4, #1
81612068:	35442001 	strbcc	r2, [r4, #-1]
8161206c:	e2433001 	sub	r3, r3, #1
81612070:	e1560003 	cmp	r6, r3
81612074:	dafffff9 	ble	81612060 <number+0x23c>
81612078:	e28d101c 	add	r1, sp, #28
8161207c:	e1a03006 	mov	r3, r6
81612080:	e1a02004 	mov	r2, r4
81612084:	e2533001 	subs	r3, r3, #1
81612088:	5a00000c 	bpl	816120c0 <number+0x29c>
8161208c:	e3560000 	cmp	r6, #0
81612090:	a0846006 	addge	r6, r4, r6
81612094:	b2846000 	addlt	r6, r4, #0
81612098:	e1a02000 	mov	r2, r0
8161209c:	e3a01020 	mov	r1, #32
816120a0:	e1a03006 	mov	r3, r6
816120a4:	e2522001 	subs	r2, r2, #1
816120a8:	5a000009 	bpl	816120d4 <number+0x2b0>
816120ac:	e3500000 	cmp	r0, #0
816120b0:	a0860000 	addge	r0, r6, r0
816120b4:	b2860000 	addlt	r0, r6, #0
816120b8:	e28dd060 	add	sp, sp, #96	; 0x60
816120bc:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
816120c0:	e152000a 	cmp	r2, sl
816120c4:	e2822001 	add	r2, r2, #1
816120c8:	37d3c001 	ldrbcc	ip, [r3, r1]
816120cc:	3542c001 	strbcc	ip, [r2, #-1]
816120d0:	eaffffeb 	b	81612084 <number+0x260>
816120d4:	e153000a 	cmp	r3, sl
816120d8:	e2833001 	add	r3, r3, #1
816120dc:	35431001 	strbcc	r1, [r3, #-1]
816120e0:	eaffffef 	b	816120a4 <number+0x280>
816120e4:	81614b00 	.word	0x81614b00

816120e8 <vsnprintf_internal>:
816120e8:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
816120ec:	e0907001 	adds	r7, r0, r1
816120f0:	e24dd018 	sub	sp, sp, #24
816120f4:	e1a08001 	mov	r8, r1
816120f8:	e1a06002 	mov	r6, r2
816120fc:	e1a04000 	mov	r4, r0
81612100:	e1a02003 	mov	r2, r3
81612104:	e1a0a000 	mov	sl, r0
81612108:	21e08000 	mvncs	r8, r0
8161210c:	23e07000 	mvncs	r7, #0
81612110:	e5d63000 	ldrb	r3, [r6]
81612114:	e3530000 	cmp	r3, #0
81612118:	1a00000a 	bne	81612148 <vsnprintf_internal+0x60>
8161211c:	e3580000 	cmp	r8, #0
81612120:	0a000005 	beq	8161213c <vsnprintf_internal+0x54>
81612124:	e1540007 	cmp	r4, r7
81612128:	35c43000 	strbcc	r3, [r4]
8161212c:	e2843001 	add	r3, r4, #1
81612130:	e1570003 	cmp	r7, r3
81612134:	33a03000 	movcc	r3, #0
81612138:	35473001 	strbcc	r3, [r7, #-1]
8161213c:	e044000a 	sub	r0, r4, sl
81612140:	e28dd018 	add	sp, sp, #24
81612144:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81612148:	e3530025 	cmp	r3, #37	; 0x25
8161214c:	0a000003 	beq	81612160 <vsnprintf_internal+0x78>
81612150:	e1540007 	cmp	r4, r7
81612154:	35c43000 	strbcc	r3, [r4]
81612158:	e2844001 	add	r4, r4, #1
8161215c:	ea0000cc 	b	81612494 <vsnprintf_internal+0x3ac>
81612160:	e3a05000 	mov	r5, #0
81612164:	ea000007 	b	81612188 <vsnprintf_internal+0xa0>
81612168:	e353002d 	cmp	r3, #45	; 0x2d
8161216c:	0a000003 	beq	81612180 <vsnprintf_internal+0x98>
81612170:	e3530030 	cmp	r3, #48	; 0x30
81612174:	1a00000c 	bne	816121ac <vsnprintf_internal+0xc4>
81612178:	e3855001 	orr	r5, r5, #1
8161217c:	ea000000 	b	81612184 <vsnprintf_internal+0x9c>
81612180:	e3855010 	orr	r5, r5, #16
81612184:	e1a06000 	mov	r6, r0
81612188:	e5d63001 	ldrb	r3, [r6, #1]
8161218c:	e2860001 	add	r0, r6, #1
81612190:	e353002b 	cmp	r3, #43	; 0x2b
81612194:	0a000015 	beq	816121f0 <vsnprintf_internal+0x108>
81612198:	8afffff2 	bhi	81612168 <vsnprintf_internal+0x80>
8161219c:	e3530020 	cmp	r3, #32
816121a0:	0a000014 	beq	816121f8 <vsnprintf_internal+0x110>
816121a4:	e3530023 	cmp	r3, #35	; 0x23
816121a8:	0a000014 	beq	81612200 <vsnprintf_internal+0x118>
816121ac:	e2431030 	sub	r1, r3, #48	; 0x30
816121b0:	e3510009 	cmp	r1, #9
816121b4:	91a0c000 	movls	ip, r0
816121b8:	93a03000 	movls	r3, #0
816121bc:	93a0600a 	movls	r6, #10
816121c0:	9a000012 	bls	81612210 <vsnprintf_internal+0x128>
816121c4:	e353002a 	cmp	r3, #42	; 0x2a
816121c8:	13e03000 	mvnne	r3, #0
816121cc:	1a000014 	bne	81612224 <vsnprintf_internal+0x13c>
816121d0:	e5923000 	ldr	r3, [r2]
816121d4:	e2821004 	add	r1, r2, #4
816121d8:	e2860002 	add	r0, r6, #2
816121dc:	e3530000 	cmp	r3, #0
816121e0:	e1a02001 	mov	r2, r1
816121e4:	b2633000 	rsblt	r3, r3, #0
816121e8:	b3855010 	orrlt	r5, r5, #16
816121ec:	ea00000c 	b	81612224 <vsnprintf_internal+0x13c>
816121f0:	e3855004 	orr	r5, r5, #4
816121f4:	eaffffe2 	b	81612184 <vsnprintf_internal+0x9c>
816121f8:	e3855008 	orr	r5, r5, #8
816121fc:	eaffffe0 	b	81612184 <vsnprintf_internal+0x9c>
81612200:	e3855040 	orr	r5, r5, #64	; 0x40
81612204:	eaffffde 	b	81612184 <vsnprintf_internal+0x9c>
81612208:	e0231396 	mla	r3, r6, r3, r1
8161220c:	e2433030 	sub	r3, r3, #48	; 0x30
81612210:	e1a0000c 	mov	r0, ip
81612214:	e4dc1001 	ldrb	r1, [ip], #1
81612218:	e241e030 	sub	lr, r1, #48	; 0x30
8161221c:	e35e0009 	cmp	lr, #9
81612220:	9afffff8 	bls	81612208 <vsnprintf_internal+0x120>
81612224:	e5d01000 	ldrb	r1, [r0]
81612228:	e351002e 	cmp	r1, #46	; 0x2e
8161222c:	13e0c000 	mvnne	ip, #0
81612230:	1a000015 	bne	8161228c <vsnprintf_internal+0x1a4>
81612234:	e5d01001 	ldrb	r1, [r0, #1]
81612238:	e280e001 	add	lr, r0, #1
8161223c:	e241c030 	sub	ip, r1, #48	; 0x30
81612240:	e35c0009 	cmp	ip, #9
81612244:	93a0c000 	movls	ip, #0
81612248:	93a0b00a 	movls	fp, #10
8161224c:	9a000009 	bls	81612278 <vsnprintf_internal+0x190>
81612250:	e351002a 	cmp	r1, #42	; 0x2a
81612254:	02800002 	addeq	r0, r0, #2
81612258:	0592c000 	ldreq	ip, [r2]
8161225c:	11a0000e 	movne	r0, lr
81612260:	02822004 	addeq	r2, r2, #4
81612264:	13a0c000 	movne	ip, #0
81612268:	01cccfcc 	biceq	ip, ip, ip, asr #31
8161226c:	ea000006 	b	8161228c <vsnprintf_internal+0x1a4>
81612270:	e02c1c9b 	mla	ip, fp, ip, r1
81612274:	e24cc030 	sub	ip, ip, #48	; 0x30
81612278:	e1a0000e 	mov	r0, lr
8161227c:	e4de1001 	ldrb	r1, [lr], #1
81612280:	e2416030 	sub	r6, r1, #48	; 0x30
81612284:	e3560009 	cmp	r6, #9
81612288:	9afffff8 	bls	81612270 <vsnprintf_internal+0x188>
8161228c:	e5d01000 	ldrb	r1, [r0]
81612290:	e3510074 	cmp	r1, #116	; 0x74
81612294:	13510068 	cmpne	r1, #104	; 0x68
81612298:	e20160df 	and	r6, r1, #223	; 0xdf
8161229c:	03a0e001 	moveq	lr, #1
816122a0:	13a0e000 	movne	lr, #0
816122a4:	e356004c 	cmp	r6, #76	; 0x4c
816122a8:	038ee001 	orreq	lr, lr, #1
816122ac:	e356005a 	cmp	r6, #90	; 0x5a
816122b0:	038ee001 	orreq	lr, lr, #1
816122b4:	e35e0000 	cmp	lr, #0
816122b8:	01a06000 	moveq	r6, r0
816122bc:	03e01000 	mvneq	r1, #0
816122c0:	0a000006 	beq	816122e0 <vsnprintf_internal+0x1f8>
816122c4:	e351006c 	cmp	r1, #108	; 0x6c
816122c8:	e2806001 	add	r6, r0, #1
816122cc:	e5d0e001 	ldrb	lr, [r0, #1]
816122d0:	1a000002 	bne	816122e0 <vsnprintf_internal+0x1f8>
816122d4:	e35e006c 	cmp	lr, #108	; 0x6c
816122d8:	02806002 	addeq	r6, r0, #2
816122dc:	03a0104c 	moveq	r1, #76	; 0x4c
816122e0:	e5d60000 	ldrb	r0, [r6]
816122e4:	e350006e 	cmp	r0, #110	; 0x6e
816122e8:	0044000a 	subeq	r0, r4, sl
816122ec:	04923004 	ldreq	r3, [r2], #4
816122f0:	05830000 	streq	r0, [r3]
816122f4:	0a000066 	beq	81612494 <vsnprintf_internal+0x3ac>
816122f8:	8a000018 	bhi	81612360 <vsnprintf_internal+0x278>
816122fc:	e3500063 	cmp	r0, #99	; 0x63
81612300:	0a00004c 	beq	81612438 <vsnprintf_internal+0x350>
81612304:	8a00000e 	bhi	81612344 <vsnprintf_internal+0x25c>
81612308:	e3500025 	cmp	r0, #37	; 0x25
8161230c:	0a000094 	beq	81612564 <vsnprintf_internal+0x47c>
81612310:	e3500058 	cmp	r0, #88	; 0x58
81612314:	0a000034 	beq	816123ec <vsnprintf_internal+0x304>
81612318:	e1540007 	cmp	r4, r7
8161231c:	33a03025 	movcc	r3, #37	; 0x25
81612320:	35c43000 	strbcc	r3, [r4]
81612324:	e5d61000 	ldrb	r1, [r6]
81612328:	e2843001 	add	r3, r4, #1
8161232c:	e3510000 	cmp	r1, #0
81612330:	0a00008e 	beq	81612570 <vsnprintf_internal+0x488>
81612334:	e1570003 	cmp	r7, r3
81612338:	e2844002 	add	r4, r4, #2
8161233c:	85441001 	strbhi	r1, [r4, #-1]
81612340:	ea000053 	b	81612494 <vsnprintf_internal+0x3ac>
81612344:	e3500064 	cmp	r0, #100	; 0x64
81612348:	0a000001 	beq	81612354 <vsnprintf_internal+0x26c>
8161234c:	e3500069 	cmp	r0, #105	; 0x69
81612350:	1afffff0 	bne	81612318 <vsnprintf_internal+0x230>
81612354:	e3855002 	orr	r5, r5, #2
81612358:	e3a0e00a 	mov	lr, #10
8161235c:	ea000025 	b	816123f8 <vsnprintf_internal+0x310>
81612360:	e3500073 	cmp	r0, #115	; 0x73
81612364:	0a000050 	beq	816124ac <vsnprintf_internal+0x3c4>
81612368:	8a00001a 	bhi	816123d8 <vsnprintf_internal+0x2f0>
8161236c:	e350006f 	cmp	r0, #111	; 0x6f
81612370:	0a00001f 	beq	816123f4 <vsnprintf_internal+0x30c>
81612374:	e3500070 	cmp	r0, #112	; 0x70
81612378:	1affffe6 	bne	81612318 <vsnprintf_internal+0x230>
8161237c:	e3730001 	cmn	r3, #1
81612380:	e1a00004 	mov	r0, r4
81612384:	13855020 	orrne	r5, r5, #32
81612388:	03855021 	orreq	r5, r5, #33	; 0x21
8161238c:	03a03008 	moveq	r3, #8
81612390:	e282b004 	add	fp, r2, #4
81612394:	e1a01007 	mov	r1, r7
81612398:	e5922000 	ldr	r2, [r2]
8161239c:	e98d1008 	stmib	sp, {r3, ip}
816123a0:	e3a03010 	mov	r3, #16
816123a4:	e58d500c 	str	r5, [sp, #12]
816123a8:	e58d3000 	str	r3, [sp]
816123ac:	e3a03000 	mov	r3, #0
816123b0:	ebfffe9b 	bl	81611e24 <number>
816123b4:	e1a04000 	mov	r4, r0
816123b8:	e5d63001 	ldrb	r3, [r6, #1]
816123bc:	e2862001 	add	r2, r6, #1
816123c0:	e59f1228 	ldr	r1, [pc, #552]	; 816125f0 <vsnprintf_internal+0x508>
816123c4:	e7d13003 	ldrb	r3, [r1, r3]
816123c8:	e3130007 	tst	r3, #7
816123cc:	0a000050 	beq	81612514 <vsnprintf_internal+0x42c>
816123d0:	e1a06002 	mov	r6, r2
816123d4:	eafffff7 	b	816123b8 <vsnprintf_internal+0x2d0>
816123d8:	e3500075 	cmp	r0, #117	; 0x75
816123dc:	0affffdd 	beq	81612358 <vsnprintf_internal+0x270>
816123e0:	e3500078 	cmp	r0, #120	; 0x78
816123e4:	03855020 	orreq	r5, r5, #32
816123e8:	1affffca 	bne	81612318 <vsnprintf_internal+0x230>
816123ec:	e3a0e010 	mov	lr, #16
816123f0:	ea000000 	b	816123f8 <vsnprintf_internal+0x310>
816123f4:	e3a0e008 	mov	lr, #8
816123f8:	e351004c 	cmp	r1, #76	; 0x4c
816123fc:	1a00005e 	bne	8161257c <vsnprintf_internal+0x494>
81612400:	e2822007 	add	r2, r2, #7
81612404:	e3c22007 	bic	r2, r2, #7
81612408:	e282b008 	add	fp, r2, #8
8161240c:	e1c200d0 	ldrd	r0, [r2]
81612410:	e98d1008 	stmib	sp, {r3, ip}
81612414:	e1a02000 	mov	r2, r0
81612418:	e1a03001 	mov	r3, r1
8161241c:	e1a00004 	mov	r0, r4
81612420:	e58d500c 	str	r5, [sp, #12]
81612424:	e1a01007 	mov	r1, r7
81612428:	e58de000 	str	lr, [sp]
8161242c:	ebfffe7c 	bl	81611e24 <number>
81612430:	e1a04000 	mov	r4, r0
81612434:	ea000036 	b	81612514 <vsnprintf_internal+0x42c>
81612438:	e3150010 	tst	r5, #16
8161243c:	1a00000b 	bne	81612470 <vsnprintf_internal+0x388>
81612440:	e1a01003 	mov	r1, r3
81612444:	e3a00020 	mov	r0, #32
81612448:	ea000002 	b	81612458 <vsnprintf_internal+0x370>
8161244c:	e1540007 	cmp	r4, r7
81612450:	e2844001 	add	r4, r4, #1
81612454:	35440001 	strbcc	r0, [r4, #-1]
81612458:	e2411001 	sub	r1, r1, #1
8161245c:	e3510000 	cmp	r1, #0
81612460:	cafffff9 	bgt	8161244c <vsnprintf_internal+0x364>
81612464:	e3530001 	cmp	r3, #1
81612468:	a0433003 	subge	r3, r3, r3
8161246c:	b2433001 	sublt	r3, r3, #1
81612470:	e1540007 	cmp	r4, r7
81612474:	e2844001 	add	r4, r4, #1
81612478:	35921000 	ldrcc	r1, [r2]
8161247c:	32822004 	addcc	r2, r2, #4
81612480:	35441001 	strbcc	r1, [r4, #-1]
81612484:	e3a01020 	mov	r1, #32
81612488:	e2433001 	sub	r3, r3, #1
8161248c:	e3530000 	cmp	r3, #0
81612490:	ca000001 	bgt	8161249c <vsnprintf_internal+0x3b4>
81612494:	e2866001 	add	r6, r6, #1
81612498:	eaffff1c 	b	81612110 <vsnprintf_internal+0x28>
8161249c:	e1540007 	cmp	r4, r7
816124a0:	e2844001 	add	r4, r4, #1
816124a4:	35441001 	strbcc	r1, [r4, #-1]
816124a8:	eafffff6 	b	81612488 <vsnprintf_internal+0x3a0>
816124ac:	e282b004 	add	fp, r2, #4
816124b0:	e5922000 	ldr	r2, [r2]
816124b4:	e58d3014 	str	r3, [sp, #20]
816124b8:	e1a0100c 	mov	r1, ip
816124bc:	e59f3130 	ldr	r3, [pc, #304]	; 816125f4 <vsnprintf_internal+0x50c>
816124c0:	e3520000 	cmp	r2, #0
816124c4:	01a02003 	moveq	r2, r3
816124c8:	e58d2010 	str	r2, [sp, #16]
816124cc:	e1a00002 	mov	r0, r2
816124d0:	ebfffd08 	bl	816118f8 <strnlen>
816124d4:	e3150010 	tst	r5, #16
816124d8:	e59d2010 	ldr	r2, [sp, #16]
816124dc:	e59d3014 	ldr	r3, [sp, #20]
816124e0:	03a0c020 	moveq	ip, #32
816124e4:	0a00000f 	beq	81612528 <vsnprintf_internal+0x440>
816124e8:	e1a01004 	mov	r1, r4
816124ec:	e041c004 	sub	ip, r1, r4
816124f0:	e150000c 	cmp	r0, ip
816124f4:	ca000010 	bgt	8161253c <vsnprintf_internal+0x454>
816124f8:	e3500000 	cmp	r0, #0
816124fc:	a0844000 	addge	r4, r4, r0
81612500:	e3a01020 	mov	r1, #32
81612504:	e0843003 	add	r3, r4, r3
81612508:	e0432004 	sub	r2, r3, r4
8161250c:	e1500002 	cmp	r0, r2
81612510:	ba00000f 	blt	81612554 <vsnprintf_internal+0x46c>
81612514:	e1a0200b 	mov	r2, fp
81612518:	eaffffdd 	b	81612494 <vsnprintf_internal+0x3ac>
8161251c:	e1570004 	cmp	r7, r4
81612520:	e2844001 	add	r4, r4, #1
81612524:	8544c001 	strbhi	ip, [r4, #-1]
81612528:	e2433001 	sub	r3, r3, #1
8161252c:	e2831001 	add	r1, r3, #1
81612530:	e1500001 	cmp	r0, r1
81612534:	bafffff8 	blt	8161251c <vsnprintf_internal+0x434>
81612538:	eaffffea 	b	816124e8 <vsnprintf_internal+0x400>
8161253c:	e1570001 	cmp	r7, r1
81612540:	e2811001 	add	r1, r1, #1
81612544:	85d2c000 	ldrbhi	ip, [r2]
81612548:	82822001 	addhi	r2, r2, #1
8161254c:	8541c001 	strbhi	ip, [r1, #-1]
81612550:	eaffffe5 	b	816124ec <vsnprintf_internal+0x404>
81612554:	e1570004 	cmp	r7, r4
81612558:	e2844001 	add	r4, r4, #1
8161255c:	85441001 	strbhi	r1, [r4, #-1]
81612560:	eaffffe8 	b	81612508 <vsnprintf_internal+0x420>
81612564:	e1540007 	cmp	r4, r7
81612568:	35c40000 	strbcc	r0, [r4]
8161256c:	eafffef9 	b	81612158 <vsnprintf_internal+0x70>
81612570:	e2466001 	sub	r6, r6, #1
81612574:	e1a04003 	mov	r4, r3
81612578:	eaffffc5 	b	81612494 <vsnprintf_internal+0x3ac>
8161257c:	e351006c 	cmp	r1, #108	; 0x6c
81612580:	e282b004 	add	fp, r2, #4
81612584:	1a000004 	bne	8161259c <vsnprintf_internal+0x4b4>
81612588:	e3150002 	tst	r5, #2
8161258c:	e5920000 	ldr	r0, [r2]
81612590:	1a000010 	bne	816125d8 <vsnprintf_internal+0x4f0>
81612594:	e3a01000 	mov	r1, #0
81612598:	eaffff9c 	b	81612410 <vsnprintf_internal+0x328>
8161259c:	e3c10020 	bic	r0, r1, #32
816125a0:	e350005a 	cmp	r0, #90	; 0x5a
816125a4:	05920000 	ldreq	r0, [r2]
816125a8:	0afffff9 	beq	81612594 <vsnprintf_internal+0x4ac>
816125ac:	e3510074 	cmp	r1, #116	; 0x74
816125b0:	05920000 	ldreq	r0, [r2]
816125b4:	0a000007 	beq	816125d8 <vsnprintf_internal+0x4f0>
816125b8:	e3510068 	cmp	r1, #104	; 0x68
816125bc:	e2050002 	and	r0, r5, #2
816125c0:	e5922000 	ldr	r2, [r2]
816125c4:	1a000005 	bne	816125e0 <vsnprintf_internal+0x4f8>
816125c8:	e3500000 	cmp	r0, #0
816125cc:	06ff0072 	uxtheq	r0, r2
816125d0:	16bf0072 	sxthne	r0, r2
816125d4:	0affffee 	beq	81612594 <vsnprintf_internal+0x4ac>
816125d8:	e1a01fc0 	asr	r1, r0, #31
816125dc:	eaffff8b 	b	81612410 <vsnprintf_internal+0x328>
816125e0:	e3500000 	cmp	r0, #0
816125e4:	e1a00002 	mov	r0, r2
816125e8:	1afffffa 	bne	816125d8 <vsnprintf_internal+0x4f0>
816125ec:	eaffffe8 	b	81612594 <vsnprintf_internal+0x4ac>
816125f0:	816140e1 	.word	0x816140e1
816125f4:	81617c08 	.word	0x81617c08

816125f8 <vscnprintf>:
816125f8:	e92d4010 	push	{r4, lr}
816125fc:	e1a04001 	mov	r4, r1
81612600:	ebfffeb8 	bl	816120e8 <vsnprintf_internal>
81612604:	e1500004 	cmp	r0, r4
81612608:	38bd8010 	popcc	{r4, pc}
8161260c:	e3540000 	cmp	r4, #0
81612610:	12440001 	subne	r0, r4, #1
81612614:	01a00004 	moveq	r0, r4
81612618:	e8bd8010 	pop	{r4, pc}

8161261c <snprintf>:
8161261c:	e92d000c 	push	{r2, r3}
81612620:	e92d4007 	push	{r0, r1, r2, lr}
81612624:	e28d3014 	add	r3, sp, #20
81612628:	e59d2010 	ldr	r2, [sp, #16]
8161262c:	e58d3004 	str	r3, [sp, #4]
81612630:	ebfffeac 	bl	816120e8 <vsnprintf_internal>
81612634:	e28dd00c 	add	sp, sp, #12
81612638:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
8161263c:	e28dd008 	add	sp, sp, #8
81612640:	e12fff1e 	bx	lr

81612644 <vsprintf>:
81612644:	e1a03002 	mov	r3, r2
81612648:	e1a02001 	mov	r2, r1
8161264c:	e3e01102 	mvn	r1, #-2147483648	; 0x80000000
81612650:	eafffea4 	b	816120e8 <vsnprintf_internal>

81612654 <sprintf>:
81612654:	e92d000e 	push	{r1, r2, r3}
81612658:	e92d4003 	push	{r0, r1, lr}
8161265c:	e28d2010 	add	r2, sp, #16
81612660:	e59d100c 	ldr	r1, [sp, #12]
81612664:	e58d2004 	str	r2, [sp, #4]
81612668:	ebfffff5 	bl	81612644 <vsprintf>
8161266c:	e28dd008 	add	sp, sp, #8
81612670:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
81612674:	e28dd00c 	add	sp, sp, #12
81612678:	e12fff1e 	bx	lr

8161267c <printf>:
8161267c:	e92d000f 	push	{r0, r1, r2, r3}
81612680:	e3001117 	movw	r1, #279	; 0x117
81612684:	e92d4010 	push	{r4, lr}
81612688:	e24dde12 	sub	sp, sp, #288	; 0x120
8161268c:	e28d3f4b 	add	r3, sp, #300	; 0x12c
81612690:	e28d0008 	add	r0, sp, #8
81612694:	e59d2128 	ldr	r2, [sp, #296]	; 0x128
81612698:	e58d3004 	str	r3, [sp, #4]
8161269c:	ebffffd5 	bl	816125f8 <vscnprintf>
816126a0:	e1a04000 	mov	r4, r0
816126a4:	e28d0008 	add	r0, sp, #8
816126a8:	ebffd1b9 	bl	81606d94 <puts>
816126ac:	e1a00004 	mov	r0, r4
816126b0:	e28dde12 	add	sp, sp, #288	; 0x120
816126b4:	e8bd4010 	pop	{r4, lr}
816126b8:	e28dd010 	add	sp, sp, #16
816126bc:	e12fff1e 	bx	lr

816126c0 <vprintf>:
816126c0:	e92d4010 	push	{r4, lr}
816126c4:	e24ddf46 	sub	sp, sp, #280	; 0x118
816126c8:	e1a03001 	mov	r3, r1
816126cc:	e1a02000 	mov	r2, r0
816126d0:	e3001117 	movw	r1, #279	; 0x117
816126d4:	e1a0000d 	mov	r0, sp
816126d8:	ebffffc6 	bl	816125f8 <vscnprintf>
816126dc:	e1a04000 	mov	r4, r0
816126e0:	e1a0000d 	mov	r0, sp
816126e4:	ebffd1aa 	bl	81606d94 <puts>
816126e8:	e1a00004 	mov	r0, r4
816126ec:	e28ddf46 	add	sp, sp, #280	; 0x118
816126f0:	e8bd8010 	pop	{r4, pc}

816126f4 <simple_itoa>:
816126f4:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
816126f8:	e3a03000 	mov	r3, #0
816126fc:	e59f4040 	ldr	r4, [pc, #64]	; 81612744 <simple_itoa+0x50>
81612700:	e1a05000 	mov	r5, r0
81612704:	e59f703c 	ldr	r7, [pc, #60]	; 81612748 <simple_itoa+0x54>
81612708:	e2844014 	add	r4, r4, #20
8161270c:	e5c43001 	strb	r3, [r4, #1]
81612710:	e1a00005 	mov	r0, r5
81612714:	e3a0100a 	mov	r1, #10
81612718:	eb00015a 	bl	81612c88 <__aeabi_uidivmod>
8161271c:	e1a06004 	mov	r6, r4
81612720:	e2811030 	add	r1, r1, #48	; 0x30
81612724:	e4461001 	strb	r1, [r6], #-1
81612728:	e0810795 	umull	r0, r1, r5, r7
8161272c:	e1b051a1 	lsrs	r5, r1, #3
81612730:	1a000001 	bne	8161273c <simple_itoa+0x48>
81612734:	e1a00004 	mov	r0, r4
81612738:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
8161273c:	e1a04006 	mov	r4, r6
81612740:	eafffff2 	b	81612710 <simple_itoa+0x1c>
81612744:	81619e88 	.word	0x81619e88
81612748:	cccccccd 	.word	0xcccccccd

8161274c <panic_finish>:
8161274c:	e92d4010 	push	{r4, lr}
81612750:	e3a0000a 	mov	r0, #10
81612754:	ebffd184 	bl	81606d6c <putc>
81612758:	e59f0018 	ldr	r0, [pc, #24]	; 81612778 <panic_finish+0x2c>
8161275c:	ebfffd12 	bl	81611bac <udelay>
81612760:	e3a03000 	mov	r3, #0
81612764:	e1a02003 	mov	r2, r3
81612768:	e1a01003 	mov	r1, r3
8161276c:	e1a00003 	mov	r0, r3
81612770:	ebffba81 	bl	8160117c <do_reset>
81612774:	eafffffe 	b	81612774 <panic_finish+0x28>
81612778:	000186a0 	.word	0x000186a0

8161277c <panic>:
8161277c:	e92d000f 	push	{r0, r1, r2, r3}
81612780:	e92d4007 	push	{r0, r1, r2, lr}
81612784:	e28d1014 	add	r1, sp, #20
81612788:	e59d0010 	ldr	r0, [sp, #16]
8161278c:	e58d1004 	str	r1, [sp, #4]
81612790:	ebffffca 	bl	816126c0 <vprintf>
81612794:	ebffffec 	bl	8161274c <panic_finish>

81612798 <simple_strtoul>:
81612798:	e92d4030 	push	{r4, r5, lr}
8161279c:	e5d03000 	ldrb	r3, [r0]
816127a0:	e59f409c 	ldr	r4, [pc, #156]	; 81612844 <simple_strtoul+0xac>
816127a4:	e3530030 	cmp	r3, #48	; 0x30
816127a8:	1a00000f 	bne	816127ec <simple_strtoul+0x54>
816127ac:	e5d0c001 	ldrb	ip, [r0, #1]
816127b0:	e2803001 	add	r3, r0, #1
816127b4:	e35c0078 	cmp	ip, #120	; 0x78
816127b8:	1a000005 	bne	816127d4 <simple_strtoul+0x3c>
816127bc:	e5d0c002 	ldrb	ip, [r0, #2]
816127c0:	e7d4c00c 	ldrb	ip, [r4, ip]
816127c4:	e31c0044 	tst	ip, #68	; 0x44
816127c8:	12800002 	addne	r0, r0, #2
816127cc:	13a02010 	movne	r2, #16
816127d0:	1a000002 	bne	816127e0 <simple_strtoul+0x48>
816127d4:	e3520000 	cmp	r2, #0
816127d8:	e1a00003 	mov	r0, r3
816127dc:	03a02008 	moveq	r2, #8
816127e0:	e1a03000 	mov	r3, r0
816127e4:	e3a00000 	mov	r0, #0
816127e8:	ea00000c 	b	81612820 <simple_strtoul+0x88>
816127ec:	e3520000 	cmp	r2, #0
816127f0:	03a0200a 	moveq	r2, #10
816127f4:	eafffff9 	b	816127e0 <simple_strtoul+0x48>
816127f8:	e31e0004 	tst	lr, #4
816127fc:	124cc030 	subne	ip, ip, #48	; 0x30
81612800:	1a000003 	bne	81612814 <simple_strtoul+0x7c>
81612804:	e31e0002 	tst	lr, #2
81612808:	124cc020 	subne	ip, ip, #32
8161280c:	16efc07c 	uxtbne	ip, ip
81612810:	e24cc037 	sub	ip, ip, #55	; 0x37
81612814:	e15c0002 	cmp	ip, r2
81612818:	2a000005 	bcs	81612834 <simple_strtoul+0x9c>
8161281c:	e020c092 	mla	r0, r2, r0, ip
81612820:	e1a05003 	mov	r5, r3
81612824:	e4d3c001 	ldrb	ip, [r3], #1
81612828:	e7d4e00c 	ldrb	lr, [r4, ip]
8161282c:	e31e0044 	tst	lr, #68	; 0x44
81612830:	1afffff0 	bne	816127f8 <simple_strtoul+0x60>
81612834:	e3510000 	cmp	r1, #0
81612838:	08bd8030 	popeq	{r4, r5, pc}
8161283c:	e5815000 	str	r5, [r1]
81612840:	e8bd8030 	pop	{r4, r5, pc}
81612844:	816140e1 	.word	0x816140e1

81612848 <strict_strtoul>:
81612848:	e92d40f7 	push	{r0, r1, r2, r4, r5, r6, r7, lr}
8161284c:	e3a03000 	mov	r3, #0
81612850:	e5823000 	str	r3, [r2]
81612854:	e1a04000 	mov	r4, r0
81612858:	e1a07001 	mov	r7, r1
8161285c:	e1a05002 	mov	r5, r2
81612860:	ebfffc1b 	bl	816118d4 <strlen>
81612864:	e2506000 	subs	r6, r0, #0
81612868:	1a000002 	bne	81612878 <strict_strtoul+0x30>
8161286c:	e3e00015 	mvn	r0, #21
81612870:	e28dd00c 	add	sp, sp, #12
81612874:	e8bd80f0 	pop	{r4, r5, r6, r7, pc}
81612878:	e1a02007 	mov	r2, r7
8161287c:	e28d1004 	add	r1, sp, #4
81612880:	e1a00004 	mov	r0, r4
81612884:	ebffffc3 	bl	81612798 <simple_strtoul>
81612888:	e59d3004 	ldr	r3, [sp, #4]
8161288c:	e1530004 	cmp	r3, r4
81612890:	0afffff5 	beq	8161286c <strict_strtoul+0x24>
81612894:	e5d32000 	ldrb	r2, [r3]
81612898:	e3520000 	cmp	r2, #0
8161289c:	0a000004 	beq	816128b4 <strict_strtoul+0x6c>
816128a0:	e0433004 	sub	r3, r3, r4
816128a4:	e2833001 	add	r3, r3, #1
816128a8:	e1530006 	cmp	r3, r6
816128ac:	0352000a 	cmpeq	r2, #10
816128b0:	1affffed 	bne	8161286c <strict_strtoul+0x24>
816128b4:	e5850000 	str	r0, [r5]
816128b8:	e3a00000 	mov	r0, #0
816128bc:	eaffffeb 	b	81612870 <strict_strtoul+0x28>

816128c0 <simple_strtol>:
816128c0:	e5d0c000 	ldrb	ip, [r0]
816128c4:	e35c002d 	cmp	ip, #45	; 0x2d
816128c8:	0a000000 	beq	816128d0 <simple_strtol+0x10>
816128cc:	eaffffb1 	b	81612798 <simple_strtoul>
816128d0:	e92d4010 	push	{r4, lr}
816128d4:	e2800001 	add	r0, r0, #1
816128d8:	ebffffae 	bl	81612798 <simple_strtoul>
816128dc:	e2600000 	rsb	r0, r0, #0
816128e0:	e8bd8010 	pop	{r4, pc}

816128e4 <ustrtoul>:
816128e4:	e92d4010 	push	{r4, lr}
816128e8:	e1a04001 	mov	r4, r1
816128ec:	ebffffa9 	bl	81612798 <simple_strtoul>
816128f0:	e5913000 	ldr	r3, [r1]
816128f4:	e5d32000 	ldrb	r2, [r3]
816128f8:	e352004b 	cmp	r2, #75	; 0x4b
816128fc:	0a000009 	beq	81612928 <ustrtoul+0x44>
81612900:	8a000004 	bhi	81612918 <ustrtoul+0x34>
81612904:	e3520047 	cmp	r2, #71	; 0x47
81612908:	18bd8010 	popne	{r4, pc}
8161290c:	e1a00500 	lsl	r0, r0, #10
81612910:	e1a00500 	lsl	r0, r0, #10
81612914:	ea000003 	b	81612928 <ustrtoul+0x44>
81612918:	e352004d 	cmp	r2, #77	; 0x4d
8161291c:	0afffffb 	beq	81612910 <ustrtoul+0x2c>
81612920:	e352006b 	cmp	r2, #107	; 0x6b
81612924:	18bd8010 	popne	{r4, pc}
81612928:	e5d32001 	ldrb	r2, [r3, #1]
8161292c:	e1a00500 	lsl	r0, r0, #10
81612930:	e3520069 	cmp	r2, #105	; 0x69
81612934:	18bd8010 	popne	{r4, pc}
81612938:	e5d32002 	ldrb	r2, [r3, #2]
8161293c:	e3520042 	cmp	r2, #66	; 0x42
81612940:	02833003 	addeq	r3, r3, #3
81612944:	12833002 	addne	r3, r3, #2
81612948:	e5843000 	str	r3, [r4]
8161294c:	e8bd8010 	pop	{r4, pc}

81612950 <simple_strtoull>:
81612950:	e92d4df0 	push	{r4, r5, r6, r7, r8, sl, fp, lr}
81612954:	e1a0c001 	mov	ip, r1
81612958:	e5d03000 	ldrb	r3, [r0]
8161295c:	e59fe0bc 	ldr	lr, [pc, #188]	; 81612a20 <simple_strtoull+0xd0>
81612960:	e3530030 	cmp	r3, #48	; 0x30
81612964:	1a000012 	bne	816129b4 <simple_strtoull+0x64>
81612968:	e5d01001 	ldrb	r1, [r0, #1]
8161296c:	e2803001 	add	r3, r0, #1
81612970:	e3510078 	cmp	r1, #120	; 0x78
81612974:	1a000005 	bne	81612990 <simple_strtoull+0x40>
81612978:	e5d01002 	ldrb	r1, [r0, #2]
8161297c:	e7de1001 	ldrb	r1, [lr, r1]
81612980:	e3110044 	tst	r1, #68	; 0x44
81612984:	12800002 	addne	r0, r0, #2
81612988:	13a02010 	movne	r2, #16
8161298c:	1a000002 	bne	8161299c <simple_strtoull+0x4c>
81612990:	e3520000 	cmp	r2, #0
81612994:	e1a00003 	mov	r0, r3
81612998:	03a02008 	moveq	r2, #8
8161299c:	e1a03000 	mov	r3, r0
816129a0:	e3a00000 	mov	r0, #0
816129a4:	e1a01000 	mov	r1, r0
816129a8:	e1a06002 	mov	r6, r2
816129ac:	e3a07000 	mov	r7, #0
816129b0:	ea000011 	b	816129fc <simple_strtoull+0xac>
816129b4:	e3520000 	cmp	r2, #0
816129b8:	03a0200a 	moveq	r2, #10
816129bc:	eafffff6 	b	8161299c <simple_strtoull+0x4c>
816129c0:	e3150004 	tst	r5, #4
816129c4:	12444030 	subne	r4, r4, #48	; 0x30
816129c8:	1a000003 	bne	816129dc <simple_strtoull+0x8c>
816129cc:	e3150002 	tst	r5, #2
816129d0:	12444020 	subne	r4, r4, #32
816129d4:	16ef4074 	uxtbne	r4, r4
816129d8:	e2444037 	sub	r4, r4, #55	; 0x37
816129dc:	e1a05fc4 	asr	r5, r4, #31
816129e0:	e1570005 	cmp	r7, r5
816129e4:	01560004 	cmpeq	r6, r4
816129e8:	9a000008 	bls	81612a10 <simple_strtoull+0xc0>
816129ec:	e08ba092 	umull	sl, fp, r2, r0
816129f0:	e09a0004 	adds	r0, sl, r4
816129f4:	e02bb192 	mla	fp, r2, r1, fp
816129f8:	e0ab1005 	adc	r1, fp, r5
816129fc:	e1a08003 	mov	r8, r3
81612a00:	e4d34001 	ldrb	r4, [r3], #1
81612a04:	e7de5004 	ldrb	r5, [lr, r4]
81612a08:	e3150044 	tst	r5, #68	; 0x44
81612a0c:	1affffeb 	bne	816129c0 <simple_strtoull+0x70>
81612a10:	e35c0000 	cmp	ip, #0
81612a14:	08bd8df0 	popeq	{r4, r5, r6, r7, r8, sl, fp, pc}
81612a18:	e58c8000 	str	r8, [ip]
81612a1c:	e8bd8df0 	pop	{r4, r5, r6, r7, r8, sl, fp, pc}
81612a20:	816140e1 	.word	0x816140e1

81612a24 <ustrtoull>:
81612a24:	e92d4010 	push	{r4, lr}
81612a28:	e1a04001 	mov	r4, r1
81612a2c:	ebffffc7 	bl	81612950 <simple_strtoull>
81612a30:	e5943000 	ldr	r3, [r4]
81612a34:	e5d32000 	ldrb	r2, [r3]
81612a38:	e352004b 	cmp	r2, #75	; 0x4b
81612a3c:	0a00000d 	beq	81612a78 <ustrtoull+0x54>
81612a40:	8a000008 	bhi	81612a68 <ustrtoull+0x44>
81612a44:	e3520047 	cmp	r2, #71	; 0x47
81612a48:	18bd8010 	popne	{r4, pc}
81612a4c:	e1a01501 	lsl	r1, r1, #10
81612a50:	e1811b20 	orr	r1, r1, r0, lsr #22
81612a54:	e1a00500 	lsl	r0, r0, #10
81612a58:	e1a01501 	lsl	r1, r1, #10
81612a5c:	e1811b20 	orr	r1, r1, r0, lsr #22
81612a60:	e1a00500 	lsl	r0, r0, #10
81612a64:	ea000003 	b	81612a78 <ustrtoull+0x54>
81612a68:	e352004d 	cmp	r2, #77	; 0x4d
81612a6c:	0afffff9 	beq	81612a58 <ustrtoull+0x34>
81612a70:	e352006b 	cmp	r2, #107	; 0x6b
81612a74:	18bd8010 	popne	{r4, pc}
81612a78:	e5d32001 	ldrb	r2, [r3, #1]
81612a7c:	e1a01501 	lsl	r1, r1, #10
81612a80:	e1811b20 	orr	r1, r1, r0, lsr #22
81612a84:	e1a00500 	lsl	r0, r0, #10
81612a88:	e3520069 	cmp	r2, #105	; 0x69
81612a8c:	18bd8010 	popne	{r4, pc}
81612a90:	e5d32002 	ldrb	r2, [r3, #2]
81612a94:	e3520042 	cmp	r2, #66	; 0x42
81612a98:	02833003 	addeq	r3, r3, #3
81612a9c:	12833002 	addne	r3, r3, #2
81612aa0:	e5843000 	str	r3, [r4]
81612aa4:	e8bd8010 	pop	{r4, pc}

81612aa8 <trailing_strtoln>:
81612aa8:	e3510000 	cmp	r1, #0
81612aac:	e92d4010 	push	{r4, lr}
81612ab0:	e1a04000 	mov	r4, r0
81612ab4:	1a000001 	bne	81612ac0 <trailing_strtoln+0x18>
81612ab8:	ebfffb85 	bl	816118d4 <strlen>
81612abc:	e0841000 	add	r1, r4, r0
81612ac0:	e59f304c 	ldr	r3, [pc, #76]	; 81612b14 <trailing_strtoln+0x6c>
81612ac4:	e5512001 	ldrb	r2, [r1, #-1]
81612ac8:	e7d32002 	ldrb	r2, [r3, r2]
81612acc:	e3120004 	tst	r2, #4
81612ad0:	e1a02003 	mov	r2, r3
81612ad4:	12413001 	subne	r3, r1, #1
81612ad8:	1a000001 	bne	81612ae4 <trailing_strtoln+0x3c>
81612adc:	e3e00000 	mvn	r0, #0
81612ae0:	e8bd8010 	pop	{r4, pc}
81612ae4:	e1530004 	cmp	r3, r4
81612ae8:	e1a00003 	mov	r0, r3
81612aec:	9afffffa 	bls	81612adc <trailing_strtoln+0x34>
81612af0:	e5d01000 	ldrb	r1, [r0]
81612af4:	e2433001 	sub	r3, r3, #1
81612af8:	e7d21001 	ldrb	r1, [r2, r1]
81612afc:	e2111004 	ands	r1, r1, #4
81612b00:	1afffff7 	bne	81612ae4 <trailing_strtoln+0x3c>
81612b04:	e3a0200a 	mov	r2, #10
81612b08:	e2800001 	add	r0, r0, #1
81612b0c:	e8bd4010 	pop	{r4, lr}
81612b10:	eaffff20 	b	81612798 <simple_strtoul>
81612b14:	816140e1 	.word	0x816140e1

81612b18 <trailing_strtol>:
81612b18:	e3a01000 	mov	r1, #0
81612b1c:	eaffffe1 	b	81612aa8 <trailing_strtoln>

81612b20 <__aeabi_uidiv>:
81612b20:	e2512001 	subs	r2, r1, #1
81612b24:	012fff1e 	bxeq	lr
81612b28:	3a000062 	bcc	81612cb8 <Ldiv0>
81612b2c:	e1500001 	cmp	r0, r1
81612b30:	9a00001a 	bls	81612ba0 <__aeabi_uidiv+0x80>
81612b34:	e1110002 	tst	r1, r2
81612b38:	0a00001b 	beq	81612bac <__aeabi_uidiv+0x8c>
81612b3c:	e16f3f11 	clz	r3, r1
81612b40:	e16f2f10 	clz	r2, r0
81612b44:	e0432002 	sub	r2, r3, r2
81612b48:	e3a03001 	mov	r3, #1
81612b4c:	e1a01211 	lsl	r1, r1, r2
81612b50:	e1a03213 	lsl	r3, r3, r2
81612b54:	e3a02000 	mov	r2, #0
81612b58:	e1500001 	cmp	r0, r1
81612b5c:	20400001 	subcs	r0, r0, r1
81612b60:	21822003 	orrcs	r2, r2, r3
81612b64:	e15000a1 	cmp	r0, r1, lsr #1
81612b68:	204000a1 	subcs	r0, r0, r1, lsr #1
81612b6c:	218220a3 	orrcs	r2, r2, r3, lsr #1
81612b70:	e1500121 	cmp	r0, r1, lsr #2
81612b74:	20400121 	subcs	r0, r0, r1, lsr #2
81612b78:	21822123 	orrcs	r2, r2, r3, lsr #2
81612b7c:	e15001a1 	cmp	r0, r1, lsr #3
81612b80:	204001a1 	subcs	r0, r0, r1, lsr #3
81612b84:	218221a3 	orrcs	r2, r2, r3, lsr #3
81612b88:	e3500000 	cmp	r0, #0
81612b8c:	11b03223 	lsrsne	r3, r3, #4
81612b90:	11a01221 	lsrne	r1, r1, #4
81612b94:	1affffef 	bne	81612b58 <__aeabi_uidiv+0x38>
81612b98:	e1a00002 	mov	r0, r2
81612b9c:	e12fff1e 	bx	lr
81612ba0:	03a00001 	moveq	r0, #1
81612ba4:	13a00000 	movne	r0, #0
81612ba8:	e12fff1e 	bx	lr
81612bac:	e16f2f11 	clz	r2, r1
81612bb0:	e262201f 	rsb	r2, r2, #31
81612bb4:	e1a00230 	lsr	r0, r0, r2
81612bb8:	e12fff1e 	bx	lr

81612bbc <__aeabi_idiv>:
81612bbc:	e3510000 	cmp	r1, #0
81612bc0:	e020c001 	eor	ip, r0, r1
81612bc4:	0a00003b 	beq	81612cb8 <Ldiv0>
81612bc8:	42611000 	rsbmi	r1, r1, #0
81612bcc:	e2512001 	subs	r2, r1, #1
81612bd0:	0a00001f 	beq	81612c54 <__aeabi_idiv+0x98>
81612bd4:	e1b03000 	movs	r3, r0
81612bd8:	42603000 	rsbmi	r3, r0, #0
81612bdc:	e1530001 	cmp	r3, r1
81612be0:	9a00001e 	bls	81612c60 <__aeabi_idiv+0xa4>
81612be4:	e1110002 	tst	r1, r2
81612be8:	0a000020 	beq	81612c70 <__aeabi_idiv+0xb4>
81612bec:	e16f2f11 	clz	r2, r1
81612bf0:	e16f0f13 	clz	r0, r3
81612bf4:	e0420000 	sub	r0, r2, r0
81612bf8:	e3a02001 	mov	r2, #1
81612bfc:	e1a01011 	lsl	r1, r1, r0
81612c00:	e1a02012 	lsl	r2, r2, r0
81612c04:	e3a00000 	mov	r0, #0
81612c08:	e1530001 	cmp	r3, r1
81612c0c:	20433001 	subcs	r3, r3, r1
81612c10:	21800002 	orrcs	r0, r0, r2
81612c14:	e15300a1 	cmp	r3, r1, lsr #1
81612c18:	204330a1 	subcs	r3, r3, r1, lsr #1
81612c1c:	218000a2 	orrcs	r0, r0, r2, lsr #1
81612c20:	e1530121 	cmp	r3, r1, lsr #2
81612c24:	20433121 	subcs	r3, r3, r1, lsr #2
81612c28:	21800122 	orrcs	r0, r0, r2, lsr #2
81612c2c:	e15301a1 	cmp	r3, r1, lsr #3
81612c30:	204331a1 	subcs	r3, r3, r1, lsr #3
81612c34:	218001a2 	orrcs	r0, r0, r2, lsr #3
81612c38:	e3530000 	cmp	r3, #0
81612c3c:	11b02222 	lsrsne	r2, r2, #4
81612c40:	11a01221 	lsrne	r1, r1, #4
81612c44:	1affffef 	bne	81612c08 <__aeabi_idiv+0x4c>
81612c48:	e35c0000 	cmp	ip, #0
81612c4c:	42600000 	rsbmi	r0, r0, #0
81612c50:	e12fff1e 	bx	lr
81612c54:	e13c0000 	teq	ip, r0
81612c58:	42600000 	rsbmi	r0, r0, #0
81612c5c:	e12fff1e 	bx	lr
81612c60:	33a00000 	movcc	r0, #0
81612c64:	01a00fcc 	asreq	r0, ip, #31
81612c68:	03800001 	orreq	r0, r0, #1
81612c6c:	e12fff1e 	bx	lr
81612c70:	e16f2f11 	clz	r2, r1
81612c74:	e262201f 	rsb	r2, r2, #31
81612c78:	e35c0000 	cmp	ip, #0
81612c7c:	e1a00233 	lsr	r0, r3, r2
81612c80:	42600000 	rsbmi	r0, r0, #0
81612c84:	e12fff1e 	bx	lr

81612c88 <__aeabi_uidivmod>:
81612c88:	e92d5003 	push	{r0, r1, ip, lr}
81612c8c:	ebffffa3 	bl	81612b20 <__aeabi_uidiv>
81612c90:	e8bd5006 	pop	{r1, r2, ip, lr}
81612c94:	e0030290 	mul	r3, r0, r2
81612c98:	e0411003 	sub	r1, r1, r3
81612c9c:	e12fff1e 	bx	lr

81612ca0 <__aeabi_idivmod>:
81612ca0:	e92d5003 	push	{r0, r1, ip, lr}
81612ca4:	ebffffc4 	bl	81612bbc <__aeabi_idiv>
81612ca8:	e8bd5006 	pop	{r1, r2, ip, lr}
81612cac:	e0030290 	mul	r3, r0, r2
81612cb0:	e0411003 	sub	r1, r1, r3
81612cb4:	e12fff1e 	bx	lr

81612cb8 <Ldiv0>:
81612cb8:	e52de008 	str	lr, [sp, #-8]!
81612cbc:	eb000001 	bl	81612cc8 <__div0>
81612cc0:	e3a00000 	mov	r0, #0
81612cc4:	e49df008 	ldr	pc, [sp], #8

81612cc8 <__div0>:
81612cc8:	eafffaa5 	b	81611764 <hang>
