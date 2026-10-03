	thumb_func_start TurnOrder_AnimateTurnChoice
TurnOrder_AnimateTurnChoice: @ 0x08029458
	push {r4, r5, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r4, _080294D8 @ =0x02020DEC
	add r0, r4, #0
	bl TweenUpdate
	add r0, r4, #0
	sub r0, #0x1D
	ldrb r0, [r0]
	ldrb r1, [r4, #0x18]
	add r2, r4, #0
	sub r2, #0x1C
	ldrh r2, [r2]
	add r3, r4, #0
	sub r3, #0x18
	str r0, [sp, #0]
	str r4, [sp, #4]
	bl TurnOrder_DrawTurnChoiceConfirm
	ldrb r0, [r4, #0x14]
	cmp r0, #2
	bne _080294CC
	ldr r0, _080294DC @ =0x086AC828
	mov r1, #0x10
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	ldr r0, _080294E0 @ =0x086AD028
	mov r1, #0x18
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	ldr r0, _080294E4 @ =0x086AD828
	mov r1, #0x88
	lsl r1, r1, #1
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	mov r2, #0x80
	lsl r2, r2, #1
	mov r0, #1
	str r0, [sp, #0]
	mov r0, #0
	str r0, [sp, #4]
	str r4, [sp, #8]
	str r0, [sp, #0xC]
	add r0, r2, #0
	mov r1, #0
	mov r3, #0
	bl TweenInit
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_080294CC:
	mov r0, #0
	add sp, #0x10
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_080294D8: .4byte 0x02020DEC
_080294DC: .4byte gDuelLogoTiles0
_080294E0: .4byte gDuelLogoTiles1
_080294E4: .4byte gDuelLogoTiles2
	thumb_func_end TurnOrder_AnimateTurnChoice

