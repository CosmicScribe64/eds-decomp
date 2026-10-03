	thumb_func_start TurnOrder_DrawDuelLogo
TurnOrder_DrawDuelLogo: @ 0x080287A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0x24]
	mov r7, #0
	ldr r0, _080288BC @ =0x08087BA4
	mov sl, r0
	ldr r1, _080288C0 @ =0x02020310
	mov r9, r1
_080287C6:
	mov r0, r8
	add r0, #0x40
	lsl r0, r0, #1
	add r0, sl
	mov r2, #0
	ldsh r5, [r0, r2]
	add r0, r5, #0
	mov r1, #0x80
	lsl r1, r1, #7
	bl MulFix8
	add r4, r0, #0
	mov r0, #0x9A
	lsl r0, r0, #2
	add r0, sl
	mov r1, #0
	ldsh r0, [r0, r1]
	sub r5, r5, r0
	mov r0, #0x60
	add r1, r5, #0
	bl MulFix8
	lsl r4, r4, #8
	asr r4, r4, #0x10
	add r6, r4, #0
	mul r6, r7
	sub r6, #4
	asr r0, r0, #8
	sub r6, r6, r0
	mov r2, r8
	lsl r0, r2, #1
	add r0, sl
	mov r1, #0
	ldsh r4, [r0, r1]
	add r0, r4, #0
	mov r1, #0x80
	lsl r1, r1, #7
	bl MulFix8
	add r5, r0, #0
	ldr r2, _080288C4 @ =0x08087D8C
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r4, r4, r0
	mov r0, #0x60
	add r1, r4, #0
	bl MulFix8
	add r4, r0, #0
	ldr r1, _080288C8 @ =0x08082712
	ldr r2, [sp, #0x24]
	lsl r0, r2, #1
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0x9C
	lsl r0, r0, #3
	bl MulFix8
	add r5, #0xA
	asr r5, r5, #8
	add r3, r5, #0
	mul r3, r7
	asr r4, r4, #8
	sub r3, r3, r4
	asr r0, r0, #4
	add r3, r3, r0
	sub r3, #0x5E
	ldr r0, _080288CC @ =0x0000FFFF
	and r3, r0
	ldr r1, _080288D0 @ =0x08082706
	lsl r0, r7, #1
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0x40
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xA
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	mov r2, r9
	str r2, [sp, #0x20]
	add r2, r6, #0
	bl OamListAddSprite
	ldr r1, [r0]
	mov r2, #0xC0
	lsl r2, r2, #2
	orr r1, r2
	str r1, [r0]
	mov r0, r8
	lsl r1, r0, #8
	ldr r0, _080288D4 @ =0x0000061C
	add r0, r9
	strh r1, [r0]
	mov r1, #0xC3
	lsl r1, r1, #3
	add r1, r9
	mov r0, #0x80
	lsl r0, r0, #1
	strh r0, [r1]
	ldr r1, _080288D8 @ =0x0202092A
	strh r0, [r1]
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #2
	bls _080287C6
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080288BC: .4byte gSineTable
_080288C0: .4byte 0x02020310
_080288C4: .4byte gUnk_08087D8C
_080288C8: .4byte gSquareTable
_080288CC: .4byte 0x0000FFFF
_080288D0: .4byte gDuelLogoTileNums
_080288D4: .4byte 0x0000061C
_080288D8: .4byte 0x0202092A
	thumb_func_end TurnOrder_DrawDuelLogo

