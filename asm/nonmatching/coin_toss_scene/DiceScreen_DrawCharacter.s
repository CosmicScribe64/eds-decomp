	thumb_func_start DiceScreen_DrawCharacter
DiceScreen_DrawCharacter: @ 0x08025430
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	ldr r0, _0802549C @ =0x0201F820
	ldr r1, _080254A0 @ =0x00000AEA
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #2
	beq _0802548E
	mov r4, #0
	ldrb r2, [r5, #0xC]
	cmp r4, r2
	bcs _0802548E
_0802545C:
	lsl r1, r4, #3
	ldr r0, [r5, #4]
	add r0, r0, r1
	mov r1, #0
	str r1, [sp, #0]
	mov r1, #0x80
	lsl r1, r1, #2
	str r1, [sp, #4]
	ldr r1, _0802549C @ =0x0201F820
	str r1, [sp, #8]
	mov r1, #1
	mov r2, r8
	add r3, r7, #0
	bl DiceScreen_AddOamPiece
	add r1, r6, #0
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldrb r0, [r5, #0xC]
	cmp r4, r0
	bcc _0802545C
_0802548E:
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802549C: .4byte 0x0201F820
_080254A0: .4byte 0x00000AEA
	thumb_func_end DiceScreen_DrawCharacter

