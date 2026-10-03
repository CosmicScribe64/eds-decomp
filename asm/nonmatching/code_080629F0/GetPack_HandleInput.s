	thumb_func_start GetPack_HandleInput
GetPack_HandleInput: @ 0x0806347C
	push {r4, r5, r6, lr}
	bl GetPack_ScrollBg
	bl GetPack_DrawCardSprites
	ldr r2, _080634C8 @ =0x03000040
	ldr r0, _080634CC @ =0x02015160
	mov r1, #0x8A
	lsl r1, r1, #1
	add r6, r0, r1
	ldrb r4, [r6]
	lsl r1, r4, #0x1D
	lsr r1, r1, #0x18
	neg r1, r1
	ldr r3, _080634D0 @ =0x0808658C
	lsl r5, r4, #0x1A
	lsr r0, r5, #0x1D
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r0, [r0]
	sub r1, r1, r0
	ldr r0, _080634D4 @ =0x00004422
	add r2, r2, r0
	strh r1, [r2]
	add r2, r4, #0
	mov r0, #0xC0
	and r0, r2
	cmp r0, #0
	beq _0806352A
	lsl r0, r2, #0x18
	lsr r0, r0, #0x1E
	cmp r0, #1
	beq _080634D8
	cmp r0, #2
	beq _08063500
	mov r0, #0x3F
	and r0, r2
	b _08063528
_080634C8: .4byte 0x03000040
_080634CC: .4byte 0x02015160
_080634D0: .4byte gPackCursorSlideOffsets
_080634D4: .4byte 0x00004422
_080634D8:
	mov r0, #0x38
	and r0, r2
	cmp r0, #0
	beq _080634FA
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1D
	sub r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #3
	mov r1, #0x39
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r6]
_080634F6:
	mov r0, #0
	b _08063606
_080634FA:
	mov r0, #0x3F
	and r0, r2
	b _08063528
_08063500:
	lsr r0, r5, #0x1D
	add r0, #1
	mov r3, #7
	and r0, r3
	lsl r0, r0, #3
	mov r2, #0x39
	neg r2, r2
	add r1, r2, #0
	and r1, r4
	orr r1, r0
	strb r1, [r6]
	mov r0, #0x38
	and r0, r1
	cmp r0, #0
	bne _080634F6
	mov r0, #0x3F
	and r1, r0
	and r1, r2
	add r0, r1, #1
	and r0, r3
_08063528:
	strb r0, [r6]
_0806352A:
	ldr r1, _08063570 @ =0x03000040
	mov r0, #0x40
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0806357E
	ldr r0, _08063574 @ =0x02015160
	mov r1, #0x8A
	lsl r1, r1, #1
	add r3, r0, r1
	ldrb r2, [r3]
	mov r0, #7
	and r0, r2
	cmp r0, #0
	beq _08063578
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	sub r1, #1
	mov r0, #7
	and r1, r0
	mov r0, #8
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, #0x38
	orr r0, r1
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x40
	orr r0, r1
	strb r0, [r3]
	mov r0, #0
	bl PlaySE
	b _0806357E
_08063570: .4byte 0x03000040
_08063574: .4byte 0x02015160
_08063578:
	mov r0, #3
	bl PlaySE
_0806357E:
	ldr r1, _080635B4 @ =0x03000040
	mov r0, #0x80
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080635C2
	ldr r0, _080635B8 @ =0x02015160
	mov r1, #0x8A
	lsl r1, r1, #1
	add r2, r0, r1
	ldrb r1, [r2]
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1D
	cmp r0, #3
	bhi _080635BC
	mov r0, #0x39
	neg r0, r0
	and r0, r1
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strb r0, [r2]
	mov r0, #0
	bl PlaySE
	b _080635C2
_080635B4: .4byte 0x03000040
_080635B8: .4byte 0x02015160
_080635BC:
	mov r0, #3
	bl PlaySE
_080635C2:
	ldr r4, _080635E8 @ =0x03000040
	ldrh r1, [r4, #6]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _080635F4
	mov r0, #1
	bl PlaySE
	ldr r1, _080635EC @ =0x00004859
	add r0, r4, r1
	ldrb r1, [r0]
	add r1, #3
	mov r2, #0
	strb r1, [r0]
	ldr r1, _080635F0 @ =0x0000485A
	add r0, r4, r1
	strb r2, [r0]
	b _080634F6
_080635E8: .4byte 0x03000040
_080635EC: .4byte 0x00004859
_080635F0: .4byte 0x0000485A
_080635F4:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _080635FE
	b _080634F6
_080635FE:
	mov r0, #2
	bl PlaySE
	mov r0, #1
_08063606:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end GetPack_HandleInput

