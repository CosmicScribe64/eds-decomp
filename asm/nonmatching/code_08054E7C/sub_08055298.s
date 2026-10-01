	thumb_func_start sub_08055298
sub_08055298: @ 0x08055298
	push {r4, r5, r6, lr}
	ldr r6, _080552B0 @ =0x0201CF90
	ldrh r1, [r6, #0xE]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #1
	beq _080552EC
	cmp r0, #1
	bgt _080552B4
	cmp r0, #0
	beq _080552BA
	b _080553A8
_080552B0: .4byte 0x0201CF90
_080552B4:
	cmp r0, #2
	beq _0805531C
	b _080553A8
_080552BA:
	ldrb r1, [r6]
	lsl r0, r1, #0x1F
	mov r4, #0x77
	cmp r0, #0
	beq _080552C6
	ldr r4, _080552E8 @ =0x00008077
_080552C6:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrb r0, [r6, #1]
	lsl r2, r0, #0x19
	lsr r2, r2, #0x1F
	lsr r0, r0, #7
	lsl r0, r0, #1
	orr r0, r2
	lsl r0, r0, #8
	orr r1, r0
	ldrh r2, [r6, #8]
	ldrh r3, [r6, #0xA]
	add r0, r4, #0
	bl sub_0801EC58
	b _08055376
	.align 2, 0
_080552E8: .4byte 0x00008077
_080552EC:
	ldrb r2, [r6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0
	bl sub_08024134
	ldrb r2, [r6, #3]
	lsr r0, r2, #7
	ldr r1, _08055318 @ =0x00007FFF
	ldrh r2, [r6, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	mov r0, #0x71
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	b _08055376
	.align 2, 0
_08055318: .4byte 0x00007FFF
_0805531C:
	ldrb r1, [r6]
	lsl r0, r1, #0x1F
	mov r3, #0x90
	cmp r0, #0
	beq _08055328
	ldr r3, _08055390 @ =0x00008090
_08055328:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrh r2, [r6, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r0, [r6, #3]
	lsr r1, r0, #7
	ldr r0, _08055394 @ =0x00007FFF
	ldrh r2, [r6, #4]
	and r0, r2
	lsl r5, r0, #1
	orr r5, r1
	ldr r0, _08055398 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _0805539C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _080553A0 @ =0x000004DE
	ldrh r0, [r0]
	cmp r0, r1
	bne _08055376
	ldrb r3, [r6]
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	lsl r0, r1, #0x1F
	lsl r3, r3, #0x1A
	lsr r3, r3, #0x1B
	lsl r2, r3, #0x10
	mov r4, #0xC4
	lsl r4, r4, #0x14
	orr r2, r4
	orr r0, r2
	orr r0, r5
	lsl r3, r3, #8
	orr r1, r3
	bl sub_0801FBCC
_08055376:
	ldrh r2, [r6, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080553A4 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r6, #0xE]
	mov r0, #0
	b _080553AA
_08055390: .4byte 0x00008090
_08055394: .4byte 0x00007FFF
_08055398: .4byte 0x000007FF
_0805539C: .4byte gUnk_08622AB4
_080553A0: .4byte 0x000004DE
_080553A4: .4byte 0xFFFFF01F
_080553A8:
	mov r0, #1
_080553AA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08055298

