	thumb_func_start TradeCardSelect_Update
TradeCardSelect_Update: @ 0x08070F18
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	ldr r0, _0807114C @ =0x03000040
	ldr r1, _08071150 @ =0x000003FF
	ldrh r0, [r0, #6]
	and r0, r1
	str r0, [sp, #0x28]
	ldr r4, _08071154 @ =0x0201E148
	add r0, r4, #0
	bl Ease_Tick
	ldr r2, _08071158 @ =0x000010E8
	add r1, r4, r2
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08070F46
	b _080711A4
_08070F46:
	mov r3, #2
	ldsh r0, [r4, r3]
	cmp r0, #4
	bne _08070F54
	ldrb r5, [r4, #0xD]
	cmp r5, #3
	beq _08070F62
_08070F54:
	cmp r0, #3
	beq _08070F5A
	b _080711A4
_08070F5A:
	ldrb r4, [r4, #0xD]
	cmp r4, #4
	beq _08070F62
	b _080711A4
_08070F62:
	ldr r6, _0807115C @ =0x0201DB20
	ldr r0, _08071160 @ =0x00001710
	add r1, r6, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r7, _08071164 @ =0x0600D000
	ldr r2, _08071168 @ =0x01000200
	add r0, sp, #0x20
	add r1, r7, #0
	bl CpuFastSet
	ldr r4, _0807116C @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _08071170 @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #2
	cmp r0, #0
	blt _08070FC6
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r6, r4
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #2
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r5, _08071174 @ =0x0000063A
	add r1, r6, r5
	ldrb r1, [r1]
	lsr r3, r1, #3
	mov r2, #0xC8
	lsl r2, r2, #3
	add r1, r6, r2
	str r1, [sp, #0]
	mov r1, #1
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl DeckEdit_DrawListRowName
_08070FC6:
	ldr r4, _0807116C @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _08071170 @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #1
	mov r4, #0xC8
	lsl r4, r4, #3
	add r4, r4, r6
	mov sl, r4
	cmp r0, #0
	blt _08071012
	mov r5, #0xA5
	lsl r5, r5, #5
	add r0, r6, r5
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #1
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r2, _08071174 @ =0x0000063A
	add r1, r6, r2
	ldrh r3, [r1]
	add r3, #0x10
	mov r1, #0xFF
	and r3, r1
	asr r3, r3, #3
	str r4, [sp, #0]
	mov r1, #2
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl DeckEdit_DrawListRowName
_08071012:
	ldr r3, _0807116C @ =0x0201F73C
	ldrb r4, [r3]
	lsl r1, r4, #1
	ldr r0, _08071170 @ =0x0201E140
	add r5, r1, r0
	mov r3, #0
	ldsh r2, [r5, r3]
	add r2, #1
	mov r0, #0xA5
	lsl r0, r0, #5
	add r0, r0, r6
	mov r9, r0
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	ldr r0, _08071178 @ =0x00001494
	add r0, r0, r6
	mov r8, r0
	add r1, r8
	ldrh r1, [r1]
	cmp r2, r1
	bge _0807106E
	ldrh r2, [r5]
	add r2, #1
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r2, _08071174 @ =0x0000063A
	add r1, r6, r2
	ldrh r3, [r1]
	add r3, #0x48
	mov r1, #0xFF
	and r3, r1
	asr r3, r3, #3
	mov r4, sl
	str r4, [sp, #0]
	mov r1, #4
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl DeckEdit_DrawListRowName
_0807106E:
	ldr r5, _0807116C @ =0x0201F73C
	ldrb r4, [r5]
	lsl r1, r4, #1
	ldr r0, _08071170 @ =0x0201E140
	add r5, r1, r0
	mov r3, #0
	ldsh r2, [r5, r3]
	add r2, #2
	mov r3, r9
	add r0, r4, r3
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	add r1, r8
	ldrh r1, [r1]
	cmp r2, r1
	bge _080710BE
	ldrh r2, [r5]
	add r2, #2
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r4, _08071174 @ =0x0000063A
	add r1, r6, r4
	ldrh r3, [r1]
	add r3, #0x58
	mov r1, #0xFF
	and r3, r1
	asr r3, r3, #3
	mov r5, sl
	str r5, [sp, #0]
	mov r1, #5
	str r1, [sp, #4]
	add r1, r7, #0
	mov r2, #0
	bl DeckEdit_DrawListRowName
_080710BE:
	ldr r7, _0807117C @ =0x0600C000
	ldr r0, _08071180 @ =0x0000063E
	add r4, r6, r0
	ldrh r3, [r4]
	add r3, #0x20
	mov r5, #0xFF
	and r3, r5
	asr r3, r3, #3
	mov r0, #0x1E
	str r0, [sp, #0]
	mov r0, #6
	str r0, [sp, #4]
	mov r1, sl
	str r1, [sp, #8]
	mov r0, #0
	add r1, r7, #0
	mov r2, #0
	bl FillMapRectWrap
	ldr r2, _0807116C @ =0x0201F73C
	ldrb r3, [r2]
	lsl r2, r3, #1
	mov r6, r9
	add r0, r3, r6
	ldrb r1, [r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r2, r0
	add r0, r8
	ldrh r0, [r0]
	cmp r0, #0
	beq _08071184
	ldr r6, _08071170 @ =0x0201E140
	add r0, r2, r6
	ldrh r2, [r0]
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldrh r3, [r4]
	add r3, #0x20
	and r3, r5
	asr r3, r3, #3
	mov r1, sl
	str r1, [sp, #0]
	add r1, r7, #0
	mov r2, #0
	bl DeckEdit_DrawCursorRowName
	mov r0, #0
	bl DeckEdit_DrawCardIcons
	ldrh r2, [r4]
	add r2, #0x38
	and r2, r5
	asr r2, r2, #3
	add r0, r7, #0
	mov r1, #0xB
	mov r3, sl
	bl DeckEdit_DrawAtkDef
	ldrh r2, [r4]
	add r2, #0x38
	and r2, r5
	asr r2, r2, #3
	add r0, r7, #0
	mov r1, #0x11
	mov r3, #6
	bl DeckEdit_DrawLevelStars
	b _080711A4
_0807114C: .4byte 0x03000040
_08071150: .4byte 0x000003FF
_08071154: .4byte 0x0201E148
_08071158: .4byte 0x000010E8
_0807115C: .4byte 0x0201DB20
_08071160: .4byte 0x00001710
_08071164: .4byte 0x0600D000
_08071168: .4byte 0x01000200
_0807116C: .4byte 0x0201F73C
_08071170: .4byte 0x0201E140
_08071174: .4byte 0x0000063A
_08071178: .4byte 0x00001494
_0807117C: .4byte 0x0600C000
_08071180: .4byte 0x0000063E
_08071184:
	ldr r6, _080711F0 @ =0x0201E140
	add r0, r2, r6
	ldrh r2, [r0]
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldrh r3, [r4]
	add r3, #0x20
	and r3, r5
	asr r3, r3, #3
	mov r1, sl
	str r1, [sp, #0]
	add r1, r7, #0
	mov r2, #0
	bl DeckEdit_DrawNoCardsText
_080711A4:
	ldr r5, _080711F4 @ =0x0201DB20
	ldr r2, _080711F8 @ =0x0000062A
	add r7, r5, r2
	mov r3, #0
	ldsh r0, [r7, r3]
	mov r4, #0xC5
	lsl r4, r4, #3
	add r6, r5, r4
	ldrb r1, [r6]
	add r2, #0xB
	add r2, r2, r5
	mov r8, r2
	ldrb r2, [r2]
	ldr r4, _080711FC @ =0x000018B0
	add r3, r5, r4
	ldr r4, _08071200 @ =0x00001BB8
	add r4, r4, r5
	str r4, [sp, #0]
	bl DeckEdit_TweenFrameSlots
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #1
	bge _080711D6
	b _0807157A
_080711D6:
	cmp r0, #2
	ble _08071204
	cmp r0, #4
	ble _080711E0
	b _0807157A
_080711E0:
	ldrb r0, [r6]
	cmp r0, #1
	bne _080711E8
	b _080713C8
_080711E8:
	cmp r0, #2
	bne _080711EE
	b _080714C4
_080711EE:
	b _08071526
_080711F0: .4byte 0x0201E140
_080711F4: .4byte 0x0201DB20
_080711F8: .4byte 0x0000062A
_080711FC: .4byte 0x000018B0
_08071200: .4byte 0x00001BB8
_08071204:
	ldrb r0, [r6]
	cmp r0, #1
	beq _08071210
	cmp r0, #2
	beq _080712C0
	b _0807135A
_08071210:
	ldr r1, _08071298 @ =0x0400001C
	mov r2, #0xC6
	lsl r2, r2, #3
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0807129C @ =0x080875D2
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _080712A0 @ =0x0400001E
	ldr r6, _080712A4 @ =0x00000632
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _080712A8 @ =0x04000014
	mov r2, #0xC7
	lsl r2, r2, #3
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #5
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _080712AC @ =0x04000016
	add r6, #8
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _080712B0 @ =0x04000010
	ldr r2, _080712B4 @ =0x0000063C
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #6
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _080712B8 @ =0x04000012
	ldr r4, _080712BC @ =0x0000063E
	add r1, r5, r4
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	b _0807157A
	.align 2, 0
_08071298: .4byte 0x0400001C
_0807129C: .4byte gDeckEditEaseCurve
_080712A0: .4byte 0x0400001E
_080712A4: .4byte 0x00000632
_080712A8: .4byte 0x04000014
_080712AC: .4byte 0x04000016
_080712B0: .4byte 0x04000010
_080712B4: .4byte 0x0000063C
_080712B8: .4byte 0x04000012
_080712BC: .4byte 0x0000063E
_080712C0:
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _080713A0 @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _080713A4 @ =0x00000632
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #5
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _080713A8 @ =0x0000063A
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #6
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _080713AC @ =0x0000063E
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	ldr r4, _080713B0 @ =0x00001BB5
	add r2, r5, r4
	ldrb r0, [r2]
	cmp r0, #0
	beq _0807133A
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _080713B4 @ =0x00001BB4
	add r1, r5, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0807133A:
	ldr r3, _080713B8 @ =0x00001BB7
	add r2, r5, r3
	ldrb r0, [r2]
	cmp r0, #0
	beq _08071354
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r4, _080713BC @ =0x00001BB6
	add r1, r5, r4
	ldrb r5, [r1]
	orr r0, r5
	strb r0, [r1]
_08071354:
	mov r0, #0
	mov r6, r8
	strb r0, [r6]
_0807135A:
	ldr r2, _080713C0 @ =0x0400001C
	ldr r1, _080713C4 @ =0x0201DB20
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r4, _080713A4 @ =0x00000632
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #0xA
	mov r5, #0xC7
	lsl r5, r5, #3
	add r0, r1, r5
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r6, _080713A8 @ =0x0000063A
	add r0, r1, r6
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #6
	add r3, #0xC
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	add r4, #0xC
	add r1, r1, r4
	ldrh r0, [r1]
	strh r0, [r2]
	b _0807157A
	.align 2, 0
_080713A0: .4byte gDeckEditEaseCurve
_080713A4: .4byte 0x00000632
_080713A8: .4byte 0x0000063A
_080713AC: .4byte 0x0000063E
_080713B0: .4byte 0x00001BB5
_080713B4: .4byte 0x00001BB4
_080713B8: .4byte 0x00001BB7
_080713BC: .4byte 0x00001BB6
_080713C0: .4byte 0x0400001C
_080713C4: .4byte 0x0201DB20
_080713C8:
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _08071454 @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _08071458 @ =0x0400001C
	mov r3, #0xC6
	lsl r3, r3, #3
	add r1, r5, r3
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0807145C @ =0x0400001E
	ldr r6, _08071460 @ =0x00000632
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #7
	mov r2, #0
	ldsh r1, [r7, r2]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	asr r0, r0, #8
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r3, #0
	ldsh r0, [r7, r3]
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	lsr r0, r0, #3
	lsl r3, r0, #0x18
	lsr r4, r3, #0x18
	ldr r1, _08071464 @ =0x04000050
	ldr r6, _08071468 @ =0x00003F43
	add r0, r6, #0
	strh r0, [r1]
	mov r1, #0
	ldsh r0, [r7, r1]
	cmp r0, #3
	bgt _08071480
	ldr r0, _0807146C @ =0x04000014
	strh r2, [r0]
	ldr r1, _08071470 @ =0x04000016
	ldr r4, _08071474 @ =0x0000063A
	add r0, r5, r4
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _08071478 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0807147C @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	lsr r0, r3, #0x19
	bl SetBldAlpha
	b _0807157A
	.align 2, 0
_08071454: .4byte gDeckEditEaseCurve
_08071458: .4byte 0x0400001C
_0807145C: .4byte 0x0400001E
_08071460: .4byte 0x00000632
_08071464: .4byte 0x04000050
_08071468: .4byte 0x00003F43
_0807146C: .4byte 0x04000014
_08071470: .4byte 0x04000016
_08071474: .4byte 0x0000063A
_08071478: .4byte 0x04000010
_0807147C: .4byte 0x0000063E
_08071480:
	ldr r0, _080714AC @ =0x04000014
	ldr r1, _080714B0 @ =0x0000FFC0
	add r2, r2, r1
	strh r2, [r0]
	ldr r1, _080714B4 @ =0x04000016
	ldr r3, _080714B8 @ =0x0000063A
	add r0, r5, r3
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _080714BC @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _080714C0 @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x20
	sub r0, r0, r4
	asr r0, r0, #1
	bl SetBldAlpha
	b _0807157A
_080714AC: .4byte 0x04000014
_080714B0: .4byte 0x0000FFC0
_080714B4: .4byte 0x04000016
_080714B8: .4byte 0x0000063A
_080714BC: .4byte 0x04000010
_080714C0: .4byte 0x0000063E
_080714C4:
	add r4, r5, #0
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r2, _080715A0 @ =0x080875D2
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r2
	ldrh r1, [r1]
	bl MulFix8
	mov r5, #0xC6
	lsl r5, r5, #3
	add r1, r4, r5
	asr r0, r0, #8
	ldrh r6, [r1]
	add r0, r6, r0
	strh r0, [r1]
	ldr r0, _080715A4 @ =0x00001BB5
	add r2, r4, r0
	ldrb r0, [r2]
	cmp r0, #0
	beq _08071506
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r2, _080715A8 @ =0x00001BB4
	add r1, r4, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_08071506:
	ldr r5, _080715AC @ =0x00001BB7
	add r2, r4, r5
	ldrb r0, [r2]
	cmp r0, #0
	beq _08071520
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _080715B0 @ =0x00001BB6
	add r1, r4, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_08071520:
	mov r0, #0
	mov r3, r8
	strb r0, [r3]
_08071526:
	ldr r2, _080715B4 @ =0x0400001C
	ldr r1, _080715B8 @ =0x0201DB20
	mov r4, #0xC6
	lsl r4, r4, #3
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r5, _080715BC @ =0x00000632
	add r0, r1, r5
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #0xA
	mov r6, #0xC7
	lsl r6, r6, #3
	add r0, r1, r6
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r3, _080715C0 @ =0x0000063A
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	sub r2, #6
	add r4, #0xC
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	add r5, #0xC
	add r1, r1, r5
	ldrh r0, [r1]
	strh r0, [r2]
	ldr r1, _080715C4 @ =0x04000050
	ldr r6, _080715C8 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #2
	mov r2, #0x80
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
_0807157A:
	ldr r5, _080715B8 @ =0x0201DB20
	ldr r3, _080715CC @ =0x0000061E
	add r0, r5, r3
	ldrb r0, [r0]
	add r6, r5, #0
	cmp r0, #0
	beq _0807158A
	b _08071D26
_0807158A:
	ldr r4, _080715D0 @ =0x00001C48
	add r7, r5, r4
	ldrb r3, [r7]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, #0
	beq _080715D4
	cmp r0, #1
	bne _0807159E
	b _08071A74
_0807159E:
	b _08071D26
_080715A0: .4byte gDeckEditEaseCurve
_080715A4: .4byte 0x00001BB5
_080715A8: .4byte 0x00001BB4
_080715AC: .4byte 0x00001BB7
_080715B0: .4byte 0x00001BB6
_080715B4: .4byte 0x0400001C
_080715B8: .4byte 0x0201DB20
_080715BC: .4byte 0x00000632
_080715C0: .4byte 0x0000063A
_080715C4: .4byte 0x04000050
_080715C8: .4byte 0x00003FC8
_080715CC: .4byte 0x0000061E
_080715D0: .4byte 0x00001C48
_080715D4:
	mov r1, #0xC5
	lsl r1, r1, #3
	add r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #1
	bne _080715E2
	b _08071A32
_080715E2:
	ldr r2, [sp, #0x28]
	cmp r2, #0x40
	beq _0807161E
	cmp r2, #0x40
	bhi _080715F2
	cmp r2, #2
	beq _080715FA
	b _0807162E
_080715F2:
	ldr r3, [sp, #0x28]
	cmp r3, #0x80
	beq _08071626
	b _0807162E
_080715FA:
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r4, #0xC3
	lsl r4, r4, #3
	add r3, r5, r4
	mov r0, #0
	mov r2, #0
	bl FadeStart
	mov r0, #0x1F
	neg r0, r0
	ldrb r5, [r7]
	and r0, r5
	strb r0, [r7]
	mov r0, #2
	bl PlaySE
	b _08071A32
_0807161E:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListUp
	b _08071A32
_08071626:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListDown
	b _08071A32
_0807162E:
	ldr r1, _08071664 @ =0x00001C1C
	add r0, r6, r1
	ldrb r1, [r0]
	lsl r2, r1, #1
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r6, r3
	add r1, r1, r0
	ldrb r4, [r1]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r5, _08071668 @ =0x00001494
	add r1, r6, r5
	add r3, r0, r1
	ldrh r4, [r3]
	cmp r4, #5
	bhi _08071656
	b _08071A32
_08071656:
	ldr r0, [sp, #0x28]
	cmp r0, #0x10
	beq _0807166C
	cmp r0, #0x20
	bne _08071662
	b _08071858
_08071662:
	b _08071A32
_08071664: .4byte 0x00001C1C
_08071668: .4byte 0x00001494
_0807166C:
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r6, r1
	add r1, r2, r0
	ldrh r2, [r1]
	add r2, #5
	ldrh r0, [r3]
	sub r0, #1
	cmp r2, r0
	ble _08071686
	mov r0, #0
	strh r0, [r1]
	b _08071688
_08071686:
	strh r2, [r1]
_08071688:
	ldr r6, _080717CC @ =0x0201DB20
	ldr r2, _080717D0 @ =0x000018AC
	add r1, r6, r2
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r4, #0xC5
	lsl r4, r4, #3
	add r3, r6, r4
	mov r0, #0
	mov r1, #6
	mov r2, #1
	bl Ease_Start
	ldr r5, _080717D4 @ =0x00000634
	add r4, r6, r5
	mov r0, #1
	mov r9, r0
	mov r0, r9
	ldrb r1, [r4]
	eor r0, r1
	strb r0, [r4]
	ldr r2, _080717D8 @ =0x00001C1C
	add r2, r2, r6
	mov r8, r2
	ldrb r0, [r2]
	mov r3, #0xA5
	lsl r3, r3, #5
	add r5, r6, r3
	add r1, r0, r5
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r3, #0xC4
	lsl r3, r3, #3
	add r7, r6, r3
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	ldrb r2, [r4]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _080717DC @ =0x06008000
	add r1, r1, r3
	bl LoadCardArt8bpp
	mov r0, #0xC6
	lsl r0, r0, #3
	add r2, r6, r0
	mov r1, #0xFF
	add r0, r1, #0
	ldrh r2, [r2]
	and r0, r2
	lsr r0, r0, #3
	add r0, #0x1D
	ldr r3, _080717E0 @ =0x00000632
	add r2, r6, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldrb r2, [r4]
	mov r3, #1
	bl DeckEdit_PlaceCardArt
	ldr r4, _080717E4 @ =0x00000635
	add r1, r6, r4
	mov r0, #3
	strb r0, [r1]
	ldr r0, _080717E8 @ =0x00001710
	add r1, r6, r0
	mov r0, r9
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	ldrb r1, [r3]
	lsl r2, r1, #1
	add r1, r1, r5
	ldrb r4, [r1]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r5, _080717EC @ =0x00001494
	add r1, r6, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r2, r7
	ldrh r1, [r2]
	ldr r3, _080717F0 @ =0x00001BB0
	add r2, r6, r3
	bl DeckEdit_CalcScrollBar
	ldr r4, _080717F4 @ =0x00001BB7
	add r1, r6, r4
	ldrb r0, [r1]
	cmp r0, #0
	beq _08071764
	mov r0, #2
	strb r0, [r1]
	ldr r5, _080717F8 @ =0x00001BB6
	add r1, r6, r5
	mov r0, r9
	ldrb r6, [r1]
	orr r0, r6
	strb r0, [r1]
_08071764:
	add r0, sp, #0x24
	mov r2, r8
	ldrb r2, [r2]
	lsl r1, r2, #1
	add r1, r1, r7
	ldrh r1, [r1]
	sub r1, #2
	strh r1, [r0]
	mov r5, #0
	add r7, r0, #0
_08071778:
	ldrh r3, [r7]
	lsl r2, r3, #0x10
	cmp r2, #0
	blt _08071804
	ldr r6, _080717CC @ =0x0201DB20
	ldr r4, _080717D8 @ =0x00001C1C
	add r0, r6, r4
	ldrb r4, [r0]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r6, r1
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	add r0, r0, r4
	lsl r0, r0, #1
	sub r1, #0xC
	add r1, r1, r6
	mov r8, r1
	add r0, r8
	lsr r1, r2, #0x10
	ldrh r0, [r0]
	cmp r1, r0
	bcs _08071804
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _080717FC @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _08071800 @ =0x00001BB8
	add r3, r6, r4
	bl DeckEdit_InitFrameSlot
	b _08071812
	.align 2, 0
_080717CC: .4byte 0x0201DB20
_080717D0: .4byte 0x000018AC
_080717D4: .4byte 0x00000634
_080717D8: .4byte 0x00001C1C
_080717DC: .4byte 0x06008000
_080717E0: .4byte 0x00000632
_080717E4: .4byte 0x00000635
_080717E8: .4byte 0x00001710
_080717EC: .4byte 0x00001494
_080717F0: .4byte 0x00001BB0
_080717F4: .4byte 0x00001BB7
_080717F8: .4byte 0x00001BB6
_080717FC: .4byte 0x000018B0
_08071800: .4byte 0x00001BB8
_08071804:
	ldr r0, _08071844 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _08071848 @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_08071812:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _08071778
	ldr r1, _08071844 @ =0x0201DB20
	ldr r0, _0807184C @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _08071850 @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _08071854 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl PlaySE
	b _08071A32
_08071844: .4byte 0x0201DB20
_08071848: .4byte 0x00001BC4
_0807184C: .4byte 0x00001C14
_08071850: .4byte 0x00001BB8
_08071854: .4byte 0x00001C58
_08071858:
	mov r5, #0xC4
	lsl r5, r5, #3
	add r0, r6, r5
	add r1, r2, r0
	ldrh r0, [r1]
	cmp r0, #4
	bhi _0807186A
	sub r0, r4, #1
	b _0807186C
_0807186A:
	sub r0, #5
_0807186C:
	strh r0, [r1]
	ldr r7, _080719BC @ =0x0201DB20
	ldr r6, _080719C0 @ =0x000018AC
	add r1, r7, r6
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r2, #1
	neg r2, r2
	mov r0, #0xC5
	lsl r0, r0, #3
	add r3, r7, r0
	mov r0, #6
	mov r1, #0
	bl Ease_Start
	ldr r1, _080719C4 @ =0x00000634
	add r5, r7, r1
	mov r2, #1
	mov sl, r2
	mov r0, sl
	ldrb r3, [r5]
	eor r0, r3
	strb r0, [r5]
	ldr r4, _080719C8 @ =0x00001C1C
	add r4, r4, r7
	mov r9, r4
	ldrb r0, [r4]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r6, r7, r1
	add r1, r0, r6
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r3, #0xC4
	lsl r3, r3, #3
	add r3, r3, r7
	mov r8, r3
	add r2, r8
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	ldrb r2, [r5]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r4, _080719CC @ =0x06008000
	add r1, r1, r4
	bl LoadCardArt8bpp
	mov r0, #0xC6
	lsl r0, r0, #3
	add r4, r7, r0
	mov r1, #0xFF
	add r0, r1, #0
	ldrh r2, [r4]
	and r0, r2
	lsr r0, r0, #3
	add r0, #9
	ldr r3, _080719D0 @ =0x00000632
	add r2, r7, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldrb r2, [r5]
	mov r3, #1
	bl DeckEdit_PlaceCardArt
	ldrh r0, [r4]
	sub r0, #0x50
	strh r0, [r4]
	ldr r4, _080719D4 @ =0x00000635
	add r1, r7, r4
	mov r0, #4
	strb r0, [r1]
	ldr r5, _080719D8 @ =0x00001710
	add r1, r7, r5
	mov r0, sl
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r9
	ldrb r1, [r3]
	lsl r2, r1, #1
	add r1, r1, r6
	ldrb r4, [r1]
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r5, _080719DC @ =0x00001494
	add r1, r7, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r8
	ldrh r1, [r2]
	ldr r6, _080719E0 @ =0x00001BB0
	add r2, r7, r6
	bl DeckEdit_CalcScrollBar
	ldr r0, _080719E4 @ =0x00001BB5
	add r1, r7, r0
	ldrb r0, [r1]
	cmp r0, #0
	beq _08071954
	mov r0, #2
	strb r0, [r1]
	ldr r2, _080719E8 @ =0x00001BB4
	add r1, r7, r2
	mov r0, sl
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_08071954:
	add r0, sp, #0x24
	mov r4, r9
	ldrb r4, [r4]
	lsl r1, r4, #1
	add r1, r8
	ldrh r1, [r1]
	sub r1, #2
	strh r1, [r0]
	mov r5, #0
	add r7, r0, #0
_08071968:
	ldrh r6, [r7]
	lsl r2, r6, #0x10
	cmp r2, #0
	blt _080719F4
	ldr r6, _080719BC @ =0x0201DB20
	ldr r1, _080719C8 @ =0x00001C1C
	add r0, r6, r1
	ldrb r4, [r0]
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r6, r3
	add r0, r4, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	add r0, r0, r4
	lsl r0, r0, #1
	ldr r1, _080719DC @ =0x00001494
	add r1, r1, r6
	mov r8, r1
	add r0, r8
	lsr r1, r2, #0x10
	ldrh r0, [r0]
	cmp r1, r0
	bcs _080719F4
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _080719EC @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _080719F0 @ =0x00001BB8
	add r3, r6, r4
	bl DeckEdit_InitFrameSlot
	b _08071A02
	.align 2, 0
_080719BC: .4byte 0x0201DB20
_080719C0: .4byte 0x000018AC
_080719C4: .4byte 0x00000634
_080719C8: .4byte 0x00001C1C
_080719CC: .4byte 0x06008000
_080719D0: .4byte 0x00000632
_080719D4: .4byte 0x00000635
_080719D8: .4byte 0x00001710
_080719DC: .4byte 0x00001494
_080719E0: .4byte 0x00001BB0
_080719E4: .4byte 0x00001BB5
_080719E8: .4byte 0x00001BB4
_080719EC: .4byte 0x000018B0
_080719F0: .4byte 0x00001BB8
_080719F4:
	ldr r0, _08071A50 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _08071A54 @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_08071A02:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _08071968
	ldr r1, _08071A50 @ =0x0201DB20
	ldr r0, _08071A58 @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _08071A5C @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _08071A60 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl PlaySE
_08071A32:
	ldr r5, [sp, #0x28]
	cmp r5, #1
	bne _08071A68
	ldr r0, _08071A50 @ =0x0201DB20
	ldr r6, _08071A64 @ =0x00001C3D
	add r0, r0, r6
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #1
	orr r1, r2
	strb r1, [r0]
	b _08071CB0
	.align 2, 0
_08071A50: .4byte 0x0201DB20
_08071A54: .4byte 0x00001BC4
_08071A58: .4byte 0x00001C14
_08071A5C: .4byte 0x00001BB8
_08071A60: .4byte 0x00001C58
_08071A64: .4byte 0x00001C3D
_08071A68:
	ldr r0, _08071A70 @ =0x0201F73C
	bl CardSelect_HandleListSwitch
	b _08071D26
_08071A70: .4byte 0x0201F73C
_08071A74:
	mov r4, #0xE1
	lsl r4, r4, #5
	add r0, r5, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _08071A82
	b _08071D26
_08071A82:
	ldr r1, _08071AA8 @ =0x00001866
	add r0, r5, r1
	mov r1, #0
	ldsb r1, [r0, r1]
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	beq _08071A94
	b _08071D26
_08071A94:
	ldr r2, [sp, #0x28]
	cmp r2, #2
	bne _08071A9C
	b _08071CD0
_08071A9C:
	cmp r2, #2
	bhi _08071AAC
	cmp r2, #1
	beq _08071B88
	b _08071CF0
	.align 2, 0
_08071AA8: .4byte 0x00001866
_08071AAC:
	ldr r3, [sp, #0x28]
	cmp r3, #0x10
	beq _08071AB8
	cmp r3, #0x20
	beq _08071B18
	b _08071CF0
_08071AB8:
	ldr r4, _08071AEC @ =0x00001C3C
	add r3, r5, r4
	ldr r2, [r3]
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	add r0, #1
	mov r1, #7
	and r1, r0
	lsl r1, r1, #0xF
	ldr r5, _08071AF0 @ =0xFFFC7FFF
	add r4, r5, #0
	and r4, r2
	orr r4, r1
	str r4, [r3]
	mov r1, #7
	and r0, r1
	cmp r0, #1
	bne _08071AF4
	add r0, r5, #0
	and r0, r4
	mov r1, #0x80
	lsl r1, r1, #0xA
	orr r0, r1
	str r0, [r3]
	b _08071B04
	.align 2, 0
_08071AEC: .4byte 0x00001C3C
_08071AF0: .4byte 0xFFFC7FFF
_08071AF4:
	mov r1, #0xE0
	lsl r1, r1, #0xA
	add r0, r4, #0
	and r0, r1
	cmp r0, r1
	bne _08071B04
	and r4, r5
	str r4, [r3]
_08071B04:
	ldr r0, _08071B14 @ =0x00001C3D
	add r2, r6, r0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	b _08071B72
	.align 2, 0
_08071B14: .4byte 0x00001C3D
_08071B18:
	ldr r3, _08071B34 @ =0x00001C3C
	add r2, r5, r3
	ldr r4, [r2]
	lsl r0, r4, #0xE
	lsr r1, r0, #0x1D
	cmp r1, #0
	beq _08071B3C
	cmp r1, #4
	bne _08071B50
	ldr r0, _08071B38 @ =0xFFFC7FFF
	and r4, r0
	str r4, [r2]
	b _08071B66
	.align 2, 0
_08071B34: .4byte 0x00001C3C
_08071B38: .4byte 0xFFFC7FFF
_08071B3C:
	ldr r0, _08071B4C @ =0xFFFC7FFF
	and r0, r4
	mov r1, #0xC0
	lsl r1, r1, #0xA
	orr r0, r1
	str r0, [r2]
	b _08071B66
	.align 2, 0
_08071B4C: .4byte 0xFFFC7FFF
_08071B50:
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #7
	and r0, r1
	lsl r0, r0, #0xF
	ldr r1, _08071B80 @ =0xFFFC7FFF
	and r1, r4
	orr r1, r0
	str r1, [r2]
_08071B66:
	ldr r5, _08071B84 @ =0x00001C3D
	add r2, r6, r5
	mov r0, #8
	neg r0, r0
	ldrb r6, [r2]
	and r0, r6
_08071B72:
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0
	bl PlaySE
	b _08071D26
_08071B80: .4byte 0xFFFC7FFF
_08071B84: .4byte 0x00001C3D
_08071B88:
	ldr r1, _08071BA0 @ =0x00001C3C
	add r0, r5, r1
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	cmp r0, #4
	beq _08071BFC
	cmp r0, #4
	bgt _08071BA4
	cmp r0, #0
	beq _08071BAE
	b _08071D26
_08071BA0: .4byte 0x00001C3C
_08071BA4:
	cmp r0, #5
	beq _08071C1C
	cmp r0, #6
	beq _08071C3C
	b _08071D26
_08071BAE:
	ldr r2, _08071BF4 @ =0x00001C1C
	add r0, r5, r2
	ldrb r2, [r0]
	mov r4, #0xA5
	lsl r4, r4, #5
	add r1, r5, r4
	add r1, r2, r1
	ldrb r6, [r1]
	lsl r0, r6, #1
	add r0, r0, r6
	add r0, r0, r2
	lsl r0, r0, #1
	ldr r2, _08071BF8 @ =0x00001494
	add r1, r5, r2
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _08071CC8
	mov r0, #0x1F
	neg r0, r0
	and r0, r3
	mov r1, #4
	orr r0, r1
	strb r0, [r7]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r4, #0xC3
	lsl r4, r4, #3
	add r3, r5, r4
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _08071CB0
	.align 2, 0
_08071BF4: .4byte 0x00001C1C
_08071BF8: .4byte 0x00001494
_08071BFC:
	mov r0, #0x1F
	neg r0, r0
	and r0, r3
	mov r1, #2
	orr r0, r1
	strb r0, [r7]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r6, #0xC3
	lsl r6, r6, #3
	add r3, r5, r6
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _08071CB0
_08071C1C:
	mov r0, #0x1F
	neg r0, r0
	and r0, r3
	mov r1, #6
	orr r0, r1
	strb r0, [r7]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r0, #0xC3
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _08071CB0
_08071C3C:
	ldr r1, _08071CB8 @ =0x00001C1C
	add r4, r5, r1
	ldrb r0, [r4]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r6, r5, r2
	add r1, r0, r6
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r3, #0xC4
	lsl r3, r3, #3
	add r3, r3, r5
	mov r8, r3
	add r2, r8
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	cmp r0, #0
	beq _08071CC8
	ldrb r4, [r4]
	lsl r2, r4, #1
	add r0, r4, r6
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r6, _08071CBC @ =0x00001494
	add r1, r5, r6
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _08071CC8
	mov r1, r8
	add r0, r2, r1
	ldrh r2, [r0]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r1, _08071CC0 @ =0x03000040
	ldr r2, _08071CC4 @ =0x00004872
	add r1, r1, r2
	strh r0, [r1]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r4, #0xC3
	lsl r4, r4, #3
	add r3, r5, r4
	mov r0, #0
	mov r2, #0
	bl FadeStart
	mov r0, #0x1F
	neg r0, r0
	ldrb r5, [r7]
	and r0, r5
	strb r0, [r7]
_08071CB0:
	mov r0, #1
	bl PlaySE
	b _08071D26
_08071CB8: .4byte 0x00001C1C
_08071CBC: .4byte 0x00001494
_08071CC0: .4byte 0x03000040
_08071CC4: .4byte 0x00004872
_08071CC8:
	mov r0, #3
	bl PlaySE
	b _08071D26
_08071CD0:
	ldr r6, _08071CEC @ =0x00001C3D
	add r0, r5, r6
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	mov r0, #2
	bl PlaySE
	b _08071D26
	.align 2, 0
_08071CEC: .4byte 0x00001C3D
_08071CF0:
	ldr r4, _08071D10 @ =0x0201F73C
	add r0, r4, #0
	bl CardSelect_HandleListSwitch
	ldr r3, _08071D14 @ =0xFFFFEA0C
	add r4, r4, r3
	ldrb r4, [r4]
	cmp r4, #1
	beq _08071D26
	ldr r4, [sp, #0x28]
	cmp r4, #0x40
	beq _08071D18
	cmp r4, #0x80
	beq _08071D20
	b _08071D26
	.align 2, 0
_08071D10: .4byte 0x0201F73C
_08071D14: .4byte 0xFFFFEA0C
_08071D18:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListUp
	b _08071D26
_08071D20:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListDown
_08071D26:
	bl SideDeckSwap_UpdateExchange
	ldr r0, _08071E6C @ =0x081A6524
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	mov r1, #3
	str r1, [sp, #4]
	mov r1, #2
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r4, _08071E70 @ =0x0201DB20
	str r4, [sp, #0x1C]
	mov r1, #5
	mov r2, #0xC
	bl OamListAddSpriteGroup
	ldr r5, _08071E74 @ =0x000017DA
	add r1, r4, r5
	mov r0, #1
	strb r0, [r1]
	ldr r6, _08071E78 @ =0x00001C1C
	add r0, r4, r6
	ldrb r2, [r0]
	lsl r3, r2, #1
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r4, r1
	add r0, r3, r0
	ldrh r0, [r0]
	mov r5, #0xA5
	lsl r5, r5, #5
	add r1, r4, r5
	add r2, r2, r1
	ldrb r6, [r2]
	lsl r1, r6, #1
	add r1, r1, r6
	lsl r1, r1, #1
	add r3, r3, r1
	ldr r2, _08071E7C @ =0x00001494
	add r1, r4, r2
	add r3, r3, r1
	ldrh r1, [r3]
	ldr r3, _08071E80 @ =0x00001BB0
	add r2, r4, r3
	bl DeckEdit_DrawScrollBar
	ldr r5, _08071E84 @ =0x00001BBC
	add r0, r4, r5
	add r1, r4, #0
	bl DeckEdit_DrawFrameSlots
	mov r5, #1
	ldr r6, _08071E88 @ =0x000018B0
	add r4, r4, r6
_08071D9C:
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	add r0, r0, r4
	bl ObjAffineApply
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #6
	bls _08071D9C
	ldr r0, _08071E8C @ =0x0201F740
	mov r8, r0
	bl DeckEdit_UpdateCardMove
	mov r6, r8
	sub r6, #4
	mov r1, r8
	sub r1, #3
	ldr r5, _08071E90 @ =0xFFFFFAF8
	add r5, r8
	add r0, r6, #0
	add r2, r5, #0
	bl DeckEdit_UpdatePanelHighlight
	mov r4, r8
	add r4, #0x1C
	add r0, r4, #0
	bl DeckEdit_UpdateCommandMenuAnim
	add r0, r4, #0
	bl DeckEdit_DrawCommandMenu
	ldr r0, _08071E94 @ =0x081A6EA4
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	mov r7, #0
	str r7, [sp, #4]
	str r7, [sp, #8]
	mov r1, #1
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	ldr r4, _08071E98 @ =0xFFFFE3E0
	add r4, r8
	str r4, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	bl OamListAddSpriteGroup
	ldrb r0, [r6]
	bl DeckEdit_DrawCardCounts
	add r0, r5, #0
	bl AnimBlockTick
	str r7, [sp, #0]
	str r7, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	str r7, [sp, #0xC]
	str r7, [sp, #0x10]
	str r4, [sp, #0x14]
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl AnimBlockDraw
	bl DeckEdit_UpdateNameIndexLetters
	bl DeckEdit_DrawNameIndexTab
	add r0, r4, #0
	bl OamListFlush
	add r0, r4, #0
	bl OamListClear
	ldr r0, _08071E9C @ =0xFFFFE9F8
	add r0, r8
	bl FadeTick
	ldr r0, _08071EA0 @ =0xFFFFE9FE
	add r0, r8
	ldrb r1, [r0]
	cmp r1, #2
	beq _08071EAC
	cmp r1, #3
	bne _08071EB0
	strb r7, [r0]
	ldr r1, _08071EA4 @ =0x04000050
	ldr r2, _08071EA8 @ =0x00003FC8
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r3, #0x80
	lsl r3, r3, #5
	add r0, r3, #0
	strh r0, [r1]
	b _08071F2E
	.align 2, 0
_08071E6C: .4byte gDeckEditArtFrameSprites
_08071E70: .4byte 0x0201DB20
_08071E74: .4byte 0x000017DA
_08071E78: .4byte 0x00001C1C
_08071E7C: .4byte 0x00001494
_08071E80: .4byte 0x00001BB0
_08071E84: .4byte 0x00001BBC
_08071E88: .4byte 0x000018B0
_08071E8C: .4byte 0x0201F740
_08071E90: .4byte 0xFFFFFAF8
_08071E94: .4byte gDeckEditPanelCornerSprite
_08071E98: .4byte 0xFFFFE3E0
_08071E9C: .4byte 0xFFFFE9F8
_08071EA0: .4byte 0xFFFFE9FE
_08071EA4: .4byte 0x04000050
_08071EA8: .4byte 0x00003FC8
_08071EAC:
	mov r0, #1
	b _08071F30
_08071EB0:
	ldr r0, _08071ED8 @ =0xFFFFEA08
	add r0, r8
	ldrb r0, [r0]
	cmp r0, #0
	bne _08071F2E
	cmp r1, #0
	bne _08071F2E
	ldr r4, _08071EDC @ =0xFFFFFC8C
	add r4, r8
	ldrh r5, [r4]
	lsl r2, r5, #0x10
	cmp r2, #0
	blt _08071EE8
	ldr r1, _08071EE0 @ =0x04000050
	ldr r6, _08071EE4 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #4
	asr r0, r2, #0x18
	b _08071EFA
_08071ED8: .4byte 0xFFFFEA08
_08071EDC: .4byte 0xFFFFFC8C
_08071EE0: .4byte 0x04000050
_08071EE4: .4byte 0x00003FC8
_08071EE8:
	ldr r1, _08071F14 @ =0x04000050
	ldr r2, _08071F18 @ =0x00003F88
	add r0, r2, #0
	strh r0, [r1]
	add r1, #4
	mov r3, #0
	ldsh r0, [r4, r3]
	neg r0, r0
	asr r0, r0, #8
_08071EFA:
	strh r0, [r1]
	ldr r0, _08071F1C @ =0x0201DB20
	ldr r4, _08071F20 @ =0x000018AC
	add r2, r0, r4
	ldrh r3, [r2]
	mov r5, #0
	ldsh r1, [r2, r5]
	ldr r0, _08071F24 @ =0x000003FF
	cmp r1, r0
	ble _08071F28
	add r0, #1
	b _08071F2C
	.align 2, 0
_08071F14: .4byte 0x04000050
_08071F18: .4byte 0x00003F88
_08071F1C: .4byte 0x0201DB20
_08071F20: .4byte 0x000018AC
_08071F24: .4byte 0x000003FF
_08071F28:
	add r0, r3, #0
	add r0, #0x30
_08071F2C:
	strh r0, [r2]
_08071F2E:
	mov r0, #0
_08071F30:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end TradeCardSelect_Update

