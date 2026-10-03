	thumb_func_start TurnOrder_ChooseTurn
TurnOrder_ChooseTurn: @ 0x080292C8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x10
	ldr r7, _080292FC @ =0x02020310
	ldr r1, _08029300 @ =0x00000B0D
	add r0, r7, r1
	ldrb r2, [r0]
	cmp r2, #0
	beq _080292DA
	b _080293E6
_080292DA:
	ldr r1, _08029304 @ =0x03000040
	mov r0, #0x20
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802930C
	ldr r1, _08029308 @ =0x00000ABF
	add r0, r7, r1
	ldrb r1, [r0]
	cmp r1, #1
	bne _0802930C
	strb r2, [r0]
	mov r0, #0
	bl PlaySE
	b _0802932E
	.align 2, 0
_080292FC: .4byte 0x02020310
_08029300: .4byte 0x00000B0D
_08029304: .4byte 0x03000040
_08029308: .4byte 0x00000ABF
_0802930C:
	ldr r1, _080293A8 @ =0x03000040
	mov r0, #0x10
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802932E
	ldr r0, _080293AC @ =0x02020310
	ldr r2, _080293B0 @ =0x00000ABF
	add r1, r0, r2
	ldrb r0, [r1]
	cmp r0, #0
	bne _0802932E
	mov r0, #1
	strb r0, [r1]
	mov r0, #0
	bl PlaySE
_0802932E:
	ldr r1, _080293A8 @ =0x03000040
	mov r6, #1
	add r0, r6, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080293E6
	ldr r5, _080293AC @ =0x02020310
	ldr r0, _080293B4 @ =0x00000ACC
	add r1, r5, r0
	mov r4, #0
	mov r0, #0x80
	lsl r0, r0, #5
	strh r0, [r1]
	ldr r0, _080293B8 @ =0x086AC828
	mov r1, #0x10
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	ldr r0, _080293BC @ =0x086AD028
	mov r1, #0x18
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	ldr r0, _080293C0 @ =0x086AD828
	mov r1, #0x88
	lsl r1, r1, #1
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	mov r0, #3
	str r0, [sp, #0]
	str r6, [sp, #4]
	ldr r1, _080293C4 @ =0x00000ADC
	add r0, r5, r1
	str r0, [sp, #8]
	str r4, [sp, #0xC]
	mov r0, #0
	mov r1, #0
	mov r2, #0x40
	mov r3, #0xF
	bl TweenInit
	ldr r2, _080293C8 @ =0x00000B0E
	add r0, r5, r2
	ldrb r0, [r0]
	cmp r0, #1
	bne _080293D0
	ldr r1, _080293CC @ =0x00000B0D
	add r0, r5, r1
	mov r1, #1
	strb r1, [r0]
	add r2, #2
	add r0, r7, r2
	bl LinkSyncStart
	b _080293E0
	.align 2, 0
_080293A8: .4byte 0x03000040
_080293AC: .4byte 0x02020310
_080293B0: .4byte 0x00000ABF
_080293B4: .4byte 0x00000ACC
_080293B8: .4byte gDuelLogoTiles0
_080293BC: .4byte gDuelLogoTiles1
_080293C0: .4byte gDuelLogoTiles2
_080293C4: .4byte 0x00000ADC
_080293C8: .4byte 0x00000B0E
_080293CC: .4byte 0x00000B0D
_080293D0:
	ldr r0, _08029444 @ =0x00000AF5
	add r4, r5, r0
	add r0, r4, #0
	bl TurnOrder_AnimateTurnChoice
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080293E0:
	mov r0, #1
	bl PlaySE
_080293E6:
	ldr r4, _08029448 @ =0x02020310
	ldr r1, _0802944C @ =0x00000ACC
	add r2, r4, r1
	ldrh r0, [r2]
	add r0, #0x80
	strh r0, [r2]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0x80
	lsl r1, r1, #5
	cmp r0, r1
	bls _08029400
	strh r1, [r2]
_08029400:
	ldrh r2, [r2]
	lsr r0, r2, #8
	bl SetBldAlpha
	ldr r2, _08029450 @ =0x00000B0D
	add r5, r4, r2
	ldrb r0, [r5]
	cmp r0, #0
	beq _0802943A
	ldr r1, _08029454 @ =0x00000ABF
	add r0, r4, r1
	ldrb r1, [r0]
	mov r0, #0xB1
	lsl r0, r0, #4
	add r2, r7, r0
	mov r0, #0x52
	bl LinkSyncStep
	cmp r0, #0
	beq _0802943A
	ldr r1, _08029444 @ =0x00000AF5
	add r0, r4, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r1, #0
	strb r1, [r5]
	bl TurnOrder_AnimateTurnChoice
_0802943A:
	mov r0, #0
	add sp, #0x10
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08029444: .4byte 0x00000AF5
_08029448: .4byte 0x02020310
_0802944C: .4byte 0x00000ACC
_08029450: .4byte 0x00000B0D
_08029454: .4byte 0x00000ABF
	thumb_func_end TurnOrder_ChooseTurn

