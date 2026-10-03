	thumb_func_start LinkBattle_DeckTooSmall
LinkBattle_DeckTooSmall: @ 0x0801AC88
	push {r4, r5, lr}
	ldr r5, _0801ACA4 @ =0x03000040
	ldr r0, _0801ACA8 @ =0x0000488A
	add r4, r5, r0
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0
	beq _0801ACAC
	cmp r0, #1
	beq _0801ACD4
	mov r0, #1
	b _0801AD0A
	.align 2, 0
_0801ACA4: .4byte 0x03000040
_0801ACA8: .4byte 0x0000488A
_0801ACAC:
	ldr r0, _0801ACCC @ =0x00000191
	bl StartDialogue
	ldrh r2, [r4]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801ACD0 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	b _0801AD08
	.align 2, 0
_0801ACCC: .4byte 0x00000191
_0801ACD0: .4byte 0xFFFFF00F
_0801ACD4:
	bl CB_Bustup
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801AD08
	ldrh r2, [r4]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801AD10 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	ldr r2, _0801AD14 @ =0x00004859
	add r0, r5, r2
	mov r1, #0
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_0801AD08:
	mov r0, #0
_0801AD0A:
	pop {r4, r5}
	pop {r1}
	bx r1
_0801AD10: .4byte 0xFFFFF00F
_0801AD14: .4byte 0x00004859
	thumb_func_end LinkBattle_DeckTooSmall

