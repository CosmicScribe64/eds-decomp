	thumb_func_start BattleScene_DrawBigNumber
BattleScene_DrawBigNumber: @ 0x0805DDC4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	add r5, r2, #0
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #0x14
	ldr r2, _0805DDF0 @ =0x302E0000
	add r0, r0, r2
	lsr r7, r0, #0x10
	add r6, #0x50
	cmp r5, #0
	bne _0805DDF4
	lsl r0, r1, #0x10
	orr r0, r6
	mov r1, #0x40
	add r2, r7, #0
	bl AddSprite
	b _0805DE28
	.align 2, 0
_0805DDF0: .4byte 0x302E0000
_0805DDF4:
	lsl r1, r1, #0x10
	mov r8, r1
_0805DDF8:
	add r4, r6, #0
	mov r0, r8
	orr r4, r0
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	lsl r2, r2, #2
	add r2, r7, r2
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0x40
	bl AddSprite
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #0x10
	cmp r5, #0
	bne _0805DDF8
_0805DE28:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end BattleScene_DrawBigNumber
	.align 2, 0

