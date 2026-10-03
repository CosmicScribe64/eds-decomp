	thumb_func_start SetBgScrollRegs
SetBgScrollRegs: @ 0x0807BCD4
	lsl r2, r2, #0x18
	lsr r2, r2, #0x16
	ldr r3, _0807BCEC @ =0x04000010
	add r2, r2, r3
	ldr r3, _0807BCF0 @ =0x000001FF
	and r1, r3
	lsl r1, r1, #0x10
	and r3, r0
	orr r1, r3
	str r1, [r2]
	bx lr
	.align 2, 0
_0807BCEC: .4byte 0x04000010
_0807BCF0: .4byte 0x000001FF
	thumb_func_end SetBgScrollRegs

