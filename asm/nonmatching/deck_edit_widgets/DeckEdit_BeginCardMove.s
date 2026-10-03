	thumb_func_start DeckEdit_BeginCardMove
DeckEdit_BeginCardMove: @ 0x080666D8
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r2, _0806670C @ =0x0201DB20
	ldr r1, _08066710 @ =0x08087480
	ldrb r0, [r5, #0xC]
	add r1, r0, r1
	ldrb r4, [r1]
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r6, _08066714 @ =0x00001726
	add r0, r0, r6
	mov r1, #1
	strb r1, [r0]
	ldr r1, _08066718 @ =0x00001C1C
	add r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #1
	beq _08066750
	cmp r0, #1
	bgt _0806671C
	cmp r0, #0
	beq _08066724
	b _08066898
	.align 2, 0
_0806670C: .4byte 0x0201DB20
_08066710: .4byte gCardFrameAnimIds
_08066714: .4byte 0x00001726
_08066718: .4byte 0x00001C1C
_0806671C:
	cmp r0, #2
	bne _08066722
	b _08066878
_08066722:
	b _08066898
_08066724:
	ldr r4, _0806674C @ =0x02011C20
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r2, r3
	ldrb r1, [r0]
	mov r6, #0xC4
	lsl r6, r6, #3
	add r0, r2, r6
	ldrh r2, [r0]
	mov r0, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	lsr r3, r0, #0x16
	b _08066898
	.align 2, 0
_0806674C: .4byte 0x02011C20
_08066750:
	ldr r1, _0806677C @ =0x000014A1
	add r0, r2, r1
	ldrb r1, [r0]
	ldr r3, _08066780 @ =0x00000622
	add r0, r2, r3
	ldrh r2, [r0]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08066784 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r4, _08066788 @ =0x08622AB4
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _0806678C @ =0x00000776
	cmp r1, r0
	bne _08066790
	mov r0, #3
	b _080667F2
_0806677C: .4byte 0x000014A1
_08066780: .4byte 0x00000622
_08066784: .4byte 0x000007FF
_08066788: .4byte gCardIdToNumber
_0806678C: .4byte 0x00000776
_08066790:
	cmp r1, r0
	blt _080667A0
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080667A0
	mov r0, #1
	b _080667F2
_080667A0:
	ldr r0, _080667C4 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r6, _080667C8 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080667D2
	cmp r0, #0x16
	bgt _080667CC
	cmp r0, #0x15
	beq _080667D6
	b _080667DE
	.align 2, 0
_080667C4: .4byte 0x000007FF
_080667C8: .4byte gCardStats
_080667CC:
	cmp r0, #0x17
	beq _080667DA
	b _080667DE
_080667D2:
	mov r0, #7
	b _080667F2
_080667D6:
	mov r0, #8
	b _080667F2
_080667DA:
	mov r0, #9
	b _080667F2
_080667DE:
	ldr r0, _08066828 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0806682C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080667F2:
	cmp r0, #2
	bne _0806683C
	ldr r4, _08066830 @ =0x02011C20
	ldr r3, _08066834 @ =0x0201DB20
	ldr r2, _08066838 @ =0x00001C1C
	add r0, r3, r2
	ldrb r0, [r0]
	mov r6, #0xA5
	lsl r6, r6, #5
	add r1, r3, r6
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r6, #0xC4
	lsl r6, r6, #3
	add r3, r3, r6
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsr r3, r0, #6
	b _08066898
	.align 2, 0
_08066828: .4byte 0x000007FF
_0806682C: .4byte gCardStats
_08066830: .4byte 0x02011C20
_08066834: .4byte 0x0201DB20
_08066838: .4byte 0x00001C1C
_0806683C:
	ldr r4, _0806686C @ =0x02011C20
	ldr r3, _08066870 @ =0x0201DB20
	ldr r1, _08066874 @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r6, #0xC4
	lsl r6, r6, #3
	add r3, r3, r6
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1C
	b _08066896
_0806686C: .4byte 0x02011C20
_08066870: .4byte 0x0201DB20
_08066874: .4byte 0x00001C1C
_08066878:
	ldr r4, _080668C4 @ =0x02011C20
	ldr r1, _080668C8 @ =0x000014A2
	add r0, r2, r1
	ldrb r1, [r0]
	ldr r3, _080668CC @ =0x00000624
	add r0, r2, r3
	ldrh r2, [r0]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
_08066896:
	lsr r3, r0, #0x1E
_08066898:
	cmp r3, #1
	bne _080668B8
	ldr r4, _080668D0 @ =0x0201DB20
	ldr r6, _080668D4 @ =0x00001BB8
	add r0, r4, r6
	ldrb r0, [r0]
	add r0, #3
	mov r1, #6
	bl __modsi3
	lsl r0, r0, #4
	add r0, r0, r4
	ldr r1, _080668D8 @ =0x00001BC4
	add r0, r0, r1
	mov r1, #0
	strb r1, [r0]
_080668B8:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_080668C4: .4byte 0x02011C20
_080668C8: .4byte 0x000014A2
_080668CC: .4byte 0x00000624
_080668D0: .4byte 0x0201DB20
_080668D4: .4byte 0x00001BB8
_080668D8: .4byte 0x00001BC4
	thumb_func_end DeckEdit_BeginCardMove

