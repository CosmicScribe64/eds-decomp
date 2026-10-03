	thumb_func_start TurnOrder_DrawTurnChoice
TurnOrder_DrawTurnChoice: @ 0x08028238
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x24
	add r5, r3, #0
	ldr r0, [sp, #0x40]
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	mov r0, #0x40
	mov r8, r0
	mov r1, #0x20
	mov r9, r1
	ldr r2, _08028294 @ =0x08087BA4
	lsl r1, r4, #1
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r1, r0
	lsl r0, r0, #1
	add r0, r0, r2
	mov r2, #0
	ldsh r0, [r0, r2]
	mov r2, #0
	ldsh r1, [r5, r2]
	bl MulFix8
	asr r0, r0, #8
	add r3, r0, #0
	add r3, #0x30
	ldr r0, _08028298 @ =0x080826E6
	ldrh r1, [r0]
	mov r0, r8
	str r0, [sp, #0]
	mov r2, r9
	str r2, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	cmp r6, #0
	bne _0802829C
	mov r0, #3
	b _0802829E
_08028294: .4byte gSineTable
_08028298: .4byte gTurnChoiceBannerTileNums
_0802829C:
	mov r0, #9
_0802829E:
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _080282CC @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	mov r2, #0x28
	bl OamListAddSprite
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, #8
	and r0, r7
	cmp r0, #0
	beq _080282D4
	ldr r0, _080282D0 @ =0x08000400
	b _080282D8
	.align 2, 0
_080282CC: .4byte 0x02020310
_080282D0: .4byte 0x08000400
_080282D4:
	mov r0, #0x80
	lsl r0, r0, #0x14
_080282D8:
	orr r1, r0
	str r1, [r2]
	ldr r1, _08028334 @ =0x02020310
	ldr r0, _08028338 @ =0x0000067C
	add r2, r1, r0
	mov r0, #0
	strh r0, [r2]
	mov r2, #0xCF
	lsl r2, r2, #3
	add r0, r1, r2
	mov r2, #0x80
	lsl r2, r2, #1
	strh r2, [r0]
	ldr r0, _0802833C @ =0x0000067A
	add r1, r1, r0
	strh r2, [r1]
	ldr r2, _08028340 @ =0x08087BA4
	lsl r1, r4, #1
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r1, r0
	lsl r0, r0, #1
	add r0, r0, r2
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r2, #4
	ldsh r1, [r5, r2]
	bl MulFix8
	asr r0, r0, #8
	add r3, r0, #0
	add r3, #0x30
	ldr r0, _08028344 @ =0x080826E6
	ldrh r1, [r0, #2]
	mov r2, #0x88
	mov r0, r8
	str r0, [sp, #0]
	mov r0, r9
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	cmp r6, #1
	bne _08028348
	mov r0, #3
	b _0802834A
_08028334: .4byte 0x02020310
_08028338: .4byte 0x0000067C
_0802833C: .4byte 0x0000067A
_08028340: .4byte gSineTable
_08028344: .4byte gTurnChoiceBannerTileNums
_08028348:
	mov r0, #9
_0802834A:
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08028374 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	bl OamListAddSprite
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, #8
	and r0, r7
	cmp r0, #0
	beq _0802837C
	ldr r0, _08028378 @ =0x0A000400
	b _08028380
_08028374: .4byte 0x02020310
_08028378: .4byte 0x0A000400
_0802837C:
	mov r0, #0xA0
	lsl r0, r0, #0x14
_08028380:
	orr r1, r0
	str r1, [r2]
	ldr r1, _080283B0 @ =0x02020310
	ldr r0, _080283B4 @ =0x00000694
	add r2, r1, r0
	mov r0, #0
	strh r0, [r2]
	mov r2, #0xD2
	lsl r2, r2, #3
	add r0, r1, r2
	mov r2, #0x80
	lsl r2, r2, #1
	strh r2, [r0]
	ldr r0, _080283B8 @ =0x00000692
	add r1, r1, r0
	strh r2, [r1]
	add sp, #0x24
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080283B0: .4byte 0x02020310
_080283B4: .4byte 0x00000694
_080283B8: .4byte 0x00000692
	thumb_func_end TurnOrder_DrawTurnChoice

