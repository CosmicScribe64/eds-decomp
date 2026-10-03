	thumb_func_start SummonStep_SpecialFromHand
SummonStep_SpecialFromHand: @ 0x080555B0
	push {r4, r5, r6, lr}
	ldr r6, _080555C8 @ =0x0201CF90
	ldrh r1, [r6, #0xE]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #1
	beq _0805566C
	cmp r0, #1
	bgt _080555CC
	cmp r0, #0
	beq _080555D2
	b _08055720
_080555C8: .4byte 0x0201CF90
_080555CC:
	cmp r0, #2
	beq _08055694
	b _08055720
_080555D2:
	ldrb r2, [r6, #3]
	lsl r0, r2, #0x1E
	cmp r0, #0
	bge _080555EA
	ldrb r1, [r6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrb r2, [r6, #2]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	bl TributeMonster
_080555EA:
	ldrb r1, [r6, #3]
	lsl r0, r1, #0x1D
	cmp r0, #0
	bge _08055602
	ldrb r2, [r6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrb r2, [r6, #2]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1D
	bl TributeMonster
_08055602:
	ldrb r1, [r6, #3]
	lsl r0, r1, #0x1C
	cmp r0, #0
	bge _0805561A
	ldrb r2, [r6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r2, [r6, #2]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	bl TributeMonster
_0805561A:
	ldrb r4, [r6]
	lsl r0, r4, #0x1F
	mov r5, #0xC4
	cmp r0, #0
	beq _08055626
	ldr r5, _08055664 @ =0x000080C4
_08055626:
	ldrb r1, [r6, #3]
	lsr r0, r1, #7
	ldr r1, _08055668 @ =0x00007FFF
	ldrh r2, [r6, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	ldrh r2, [r6]
	lsr r0, r2, #6
	mov r3, #0xF
	add r2, r3, #0
	and r2, r0
	lsl r2, r2, #4
	lsl r0, r4, #0x1A
	lsr r0, r0, #0x1B
	and r3, r0
	orr r2, r3
	ldrb r3, [r6, #1]
	lsl r0, r3, #0x19
	lsr r0, r0, #0x1F
	lsr r3, r3, #7
	lsl r3, r3, #1
	orr r0, r3
	lsl r0, r0, #8
	orr r2, r0
	add r0, r5, #0
_0805565A:
	mov r3, #0
	bl DuelCmd_Push
	b _080556EE
	.align 2, 0
_08055664: .4byte 0x000080C4
_08055668: .4byte 0x00007FFF
_0805566C:
	ldrb r2, [r6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0
	bl DuelCursor_Select
	ldrb r1, [r6, #3]
	lsr r0, r1, #7
	ldr r1, _08055690 @ =0x00007FFF
	ldrh r2, [r6, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	mov r0, #0x71
	mov r2, #1
	b _0805565A
_08055690: .4byte 0x00007FFF
_08055694:
	ldrb r1, [r6]
	lsl r0, r1, #0x1F
	mov r3, #0x90
	cmp r0, #0
	beq _080556A0
	ldr r3, _08055708 @ =0x00008090
_080556A0:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrh r2, [r6, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r6, #3]
	lsr r1, r0, #7
	ldr r0, _0805570C @ =0x00007FFF
	ldrh r2, [r6, #4]
	and r0, r2
	lsl r5, r0, #1
	orr r5, r1
	ldr r0, _08055710 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _08055714 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08055718 @ =0x000004DE
	ldrh r0, [r0]
	cmp r0, r1
	bne _080556EE
	ldrb r3, [r6]
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	lsl r0, r1, #0x1F
	lsl r3, r3, #0x1A
	lsr r3, r3, #0x1B
	lsl r2, r3, #0x10
	mov r4, #0xA4
	lsl r4, r4, #0x14
	orr r2, r4
	orr r0, r2
	orr r0, r5
	lsl r3, r3, #8
	orr r1, r3
	bl Chain_AddPending
_080556EE:
	ldrh r2, [r6, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _0805571C @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r6, #0xE]
	mov r0, #0
	b _08055722
_08055708: .4byte 0x00008090
_0805570C: .4byte 0x00007FFF
_08055710: .4byte 0x000007FF
_08055714: .4byte gCardIdToNumber
_08055718: .4byte 0x000004DE
_0805571C: .4byte 0xFFFFF01F
_08055720:
	mov r0, #1
_08055722:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end SummonStep_SpecialFromHand

