@ Cartridge header + Nintendo AGB SDK crt0 (startup) and the SDK's
@ multiple-interrupt dispatcher (IntrMain). See wiki/functions/crt0.md.
	.include "asm/macros.inc"
	.syntax unified
	.text
	.arm

	.global _start
_start: @ 0x08000000
	b start_vector
	@ Nintendo logo, title "YU-GI-OH!EDS", game code AY5E, maker A4, checksum...
	.incbin "build/assets/header.bin", 0x0, 0xBC

	arm_func_start start_vector
start_vector: @ 0x080000C0
	mov r0, #0x12           @ IRQ mode
	msr cpsr_fc, r0
	ldr sp, sp_irq
	mov r0, #0x1f           @ System mode
	msr cpsr_fc, r0
	ldr sp, sp_usr
	ldr r1, _0800021C       @ =INTR_VECTOR
	adr r0, IntrMain
	str r0, [r1]
	ldr r1, _08000220       @ =AgbMain
	mov lr, pc
	bx r1
	b start_vector
	arm_func_end start_vector

sp_usr: .4byte 0x03007B00
sp_irq: .4byte 0x03007FA0

	arm_func_start IntrMain
IntrMain: @ 0x080000FC
	mov r3, #0x4000000
	add r3, r3, #0x200      @ REG_IE
	ldr r2, [r3]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mrs r0, spsr
	push {r0, r1, r3, lr}
	and r1, r2, r2, lsr #16 @ IE & IF
	mov r2, #0
	ands r0, r1, #0x80      @ serial
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #2         @ hblank
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #1         @ vblank
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #4         @ vcount
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #8         @ timer0
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x10      @ timer1
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x20      @ timer2
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x40      @ timer3
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x100     @ dma0
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x200     @ dma1
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x400     @ dma2
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x800     @ dma3
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x1000    @ keypad
	bne .Ljump_intr
	add r2, r2, #4
	ands r0, r1, #0x2000    @ gamepak
.Lloop:
	bne .Lloop
.Ljump_intr:
	strh r0, [r3, #2]       @ REG_IF
	mov r1, #0x2280         @ allow nested serial/dma1(?)/gamepak interrupts
	strh r1, [r3]
	mrs r3, cpsr
	bic r3, r3, #0xdf
	orr r3, r3, #0x1f       @ System mode, IRQs enabled
	msr cpsr_fc, r3
	ldr r1, _08000224       @ =IntrTable
	add r1, r1, r2
	ldr r0, [r1]
	stmdb sp!, {lr}
	add lr, pc, #0
	bx r0
	ldm sp!, {lr}
	mrs r3, cpsr
	bic r3, r3, #0xdf
	orr r3, r3, #0x92       @ IRQ mode, IRQs disabled
	msr cpsr_fc, r3
	pop {r0, r1, r3, lr}
	strh r1, [r3]           @ restore REG_IE
	msr spsr_fc, r0
	bx lr
	arm_func_end IntrMain

_0800021C: .4byte 0x03007FFC    @ INTR_VECTOR
_08000220: .4byte AgbMain
_08000224: .4byte 0x03000000    @ IntrTable
