	thumb_func_start Duel_CheckWin
Duel_CheckWin: @ 0x08021628
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, _080216B4 @ =0x02015EE8
	ldrb r0, [r1]
	sub r0, #2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #6
	bls _08021642
	b _08021824
_08021642:
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	ldr r1, _080216B8 @ =0x020192E0
	mov sl, r1
	cmp r0, #0
	beq _08021660
	ldr r1, _080216BC @ =0x00001B12
	add r1, sl
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08021660
	b _08021824
_08021660:
	mov r6, sl
	ldr r2, _080216BC @ =0x00001B12
	add r5, r6, r2
	mov r0, #0xC0
	ldrb r1, [r5]
	orr r1, r0
	strb r1, [r5]
	add r7, r6, #4
	ldrh r2, [r6, #4]
	cmp r2, #0
	beq _08021680
	ldr r3, _080216C0 @ =0x00000D68
	add r0, r6, r3
	ldrh r0, [r0]
	cmp r0, #0
	bne _080216C8
_08021680:
	ldr r4, _080216C0 @ =0x00000D68
	add r3, r6, r4
	ldrh r0, [r3]
	cmp r2, r0
	bls _08021694
	mov r0, #0x3F
	and r1, r0
	mov r0, #0x40
	orr r1, r0
	strb r1, [r5]
_08021694:
	ldrh r1, [r6, #4]
	ldrh r3, [r3]
	cmp r1, r3
	bcs _080216A8
	mov r0, #0x3F
	ldrb r2, [r5]
	and r0, r2
	mov r1, #0x80
	orr r0, r1
	strb r0, [r5]
_080216A8:
	ldr r3, _080216C4 @ =0x00001ACC
	add r1, r6, r3
	mov r0, #0x10
	ldrb r4, [r1]
	orr r0, r4
	b _08021818
_080216B4: .4byte 0x02015EE8
_080216B8: .4byte 0x020192E0
_080216BC: .4byte 0x00001B12
_080216C0: .4byte 0x00000D68
_080216C4: .4byte 0x00001ACC
_080216C8:
	mov r0, #1
	mov r9, r0
	mov r2, #1
	add r0, r2, #0
	ldrb r3, [r7, #7]
	and r0, r3
	cmp r0, #0
	bne _080216F2
	ldr r4, _08021718 @ =0x00000D6F
	add r4, r4, r6
	mov r8, r4
	add r0, r2, #0
	ldrb r3, [r4]
	and r0, r3
	cmp r0, #0
	beq _08021720
	mov r0, #0x3F
	and r1, r0
	mov r0, #0x40
	orr r1, r0
	strb r1, [r5]
_080216F2:
	ldr r4, _08021718 @ =0x00000D6F
	add r1, r6, r4
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0802170C
	mov r0, #0x3F
	ldrb r1, [r5]
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strb r0, [r5]
_0802170C:
	ldr r2, _0802171C @ =0x00001ACC
	add r1, r6, r2
	mov r0, #0x10
	ldrb r3, [r1]
	orr r0, r3
	b _08021818
_08021718: .4byte 0x00000D6F
_0802171C: .4byte 0x00001ACC
_08021720:
	mov r0, #0
	bl HasExodiaInHand
	mov r1, r9
	and r1, r0
	lsl r1, r1, #1
	mov r4, #3
	neg r4, r4
	add r0, r4, #0
	ldrb r2, [r7, #7]
	and r0, r2
	orr r0, r1
	strb r0, [r7, #7]
	mov r0, #1
	bl HasExodiaInHand
	mov r3, r9
	and r0, r3
	lsl r0, r0, #1
	mov r1, r8
	ldrb r1, [r1]
	and r4, r1
	orr r4, r0
	mov r2, r8
	strb r4, [r2]
	mov r2, #2
	add r0, r2, #0
	ldrb r3, [r7, #7]
	and r0, r3
	cmp r0, #0
	bne _08021766
	add r0, r4, #0
	and r0, r2
	cmp r0, #0
	beq _080217A0
_08021766:
	and r4, r2
	cmp r4, #0
	bne _08021778
	mov r0, #0x3F
	ldrb r4, [r5]
	and r0, r4
	mov r1, #0x40
	orr r0, r1
	strb r0, [r5]
_08021778:
	add r0, r2, #0
	ldrb r7, [r7, #7]
	and r0, r7
	cmp r0, #0
	bne _0802178E
	mov r0, #0x3F
	ldrb r1, [r5]
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strb r0, [r5]
_0802178E:
	ldr r2, _0802179C @ =0x00001ACC
	add r1, r6, r2
	mov r0, #0x10
	ldrb r3, [r1]
	orr r0, r3
	b _08021818
	.align 2, 0
_0802179C: .4byte 0x00001ACC
_080217A0:
	mov r0, #0
	bl HasDestinyBoardComplete
	mov r1, r9
	and r1, r0
	lsl r1, r1, #2
	mov r4, #5
	neg r4, r4
	add r0, r4, #0
	ldrb r2, [r7, #7]
	and r0, r2
	orr r0, r1
	strb r0, [r7, #7]
	mov r0, #1
	bl HasDestinyBoardComplete
	mov r3, r9
	and r0, r3
	lsl r0, r0, #2
	mov r1, r8
	ldrb r1, [r1]
	and r4, r1
	orr r4, r0
	mov r2, r8
	strb r4, [r2]
	mov r2, #4
	add r0, r2, #0
	ldrb r3, [r7, #7]
	and r0, r3
	cmp r0, #0
	bne _080217E6
	add r0, r4, #0
	and r0, r2
	cmp r0, #0
	beq _08021824
_080217E6:
	and r4, r2
	cmp r4, #0
	bne _080217F8
	mov r0, #0x3F
	ldrb r4, [r5]
	and r0, r4
	mov r1, #0x40
	orr r0, r1
	strb r0, [r5]
_080217F8:
	add r0, r2, #0
	ldrb r7, [r7, #7]
	and r0, r7
	cmp r0, #0
	bne _0802180E
	mov r0, #0x3F
	ldrb r1, [r5]
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strb r0, [r5]
_0802180E:
	ldr r1, _08021820 @ =0x00001ACC
	add r1, sl
	mov r0, #0x10
	ldrb r2, [r1]
	orr r0, r2
_08021818:
	strb r0, [r1]
	mov r0, #1
	b _08021826
	.align 2, 0
_08021820: .4byte 0x00001ACC
_08021824:
	mov r0, #0
_08021826:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end Duel_CheckWin

