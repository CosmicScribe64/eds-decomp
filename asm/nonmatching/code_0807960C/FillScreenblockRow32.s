	thumb_func_start FillScreenblockRow32
FillScreenblockRow32: @ 0x0807A4E8
	push {r4, r5, lr}
	ldr r4, [sp, #0xC]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	lsl r4, r4, #0x10
	lsr r1, r1, #0xD
	lsr r2, r2, #0x17
	mov r0, #0xC0
	lsl r0, r0, #0x13
	add r2, r2, r0
	add r1, r1, r2
	lsr r3, r3, #0x12
	add r1, r1, r3
	mov r0, #0
	lsr r4, r4, #0x11
	cmp r0, r4
	bcs _0807A520
	lsl r2, r5, #0x10
	orr r2, r5
_0807A514:
	stmia r1!, {r2}
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, r4
	bcc _0807A514
_0807A520:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end FillScreenblockRow32
	.align 2, 0

