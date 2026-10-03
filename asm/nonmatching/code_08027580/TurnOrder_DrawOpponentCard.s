	thumb_func_start TurnOrder_DrawOpponentCard
TurnOrder_DrawOpponentCard: @ 0x08028178
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #0x24
	add r4, r0, #0
	add r0, r1, #0
	add r5, r2, #0
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r0, r0, #0x18
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	mov r6, #0x20
	mov r1, #0x40
	mov r8, r1
	lsr r0, r0, #0x10
	mov r1, #0xB0
	lsl r1, r1, #1
	bl MulFix8
	add r3, r0, #0
	asr r3, r3, #8
	add r3, #0xC0
	ldr r1, _080281EC @ =0x080826E0
	lsl r0, r4, #1
	add r0, r0, r1
	ldrh r1, [r0]
	str r6, [sp, #0]
	mov r2, r8
	str r2, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	ldr r0, _080281F0 @ =0x08082703
	add r4, r4, r0
	ldrb r0, [r4]
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _080281F4 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	mov r2, #0x69
	bl OamListAddSprite
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, #2
	and r5, r0
	cmp r5, #0
	beq _080281FC
	ldr r0, _080281F8 @ =0x06000400
	b _08028200
	.align 2, 0
_080281EC: .4byte gHandCardTileNums
_080281F0: .4byte gHandCardPalNums
_080281F4: .4byte 0x02020310
_080281F8: .4byte 0x06000400
_080281FC:
	mov r0, #0xC0
	lsl r0, r0, #0x13
_08028200:
	orr r1, r0
	str r1, [r2]
	ldr r1, _0802822C @ =0x02020310
	ldr r0, _08028230 @ =0x00000664
	add r2, r1, r0
	mov r0, #0
	strh r0, [r2]
	mov r2, #0xCC
	lsl r2, r2, #3
	add r0, r1, r2
	mov r2, #0x80
	lsl r2, r2, #1
	strh r2, [r0]
	ldr r0, _08028234 @ =0x00000662
	add r1, r1, r0
	strh r2, [r1]
	add sp, #0x24
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0802822C: .4byte 0x02020310
_08028230: .4byte 0x00000664
_08028234: .4byte 0x00000662
	thumb_func_end TurnOrder_DrawOpponentCard

