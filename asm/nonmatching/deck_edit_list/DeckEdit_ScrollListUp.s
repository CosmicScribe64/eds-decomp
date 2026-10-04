	thumb_func_start DeckEdit_ScrollListUp
DeckEdit_ScrollListUp: @ 0x080679E0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	str r0, [sp, #0xC]
	ldr r5, _08067BCC @ =0x0201DB20
	ldr r0, _08067BD0 @ =0x00001C1C
	add r0, r0, r5
	mov sl, r0
	ldrb r1, [r0]
	lsl r0, r1, #1
	ldr r2, _08067BD4 @ =0x0201E140
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, #0
	bne _08067A06
	b _08067D4E
_08067A06:
	ldr r3, _08067BD8 @ =0x00001C58
	add r0, r5, r3
	mov r4, #0x1E
	strh r4, [r0]
	ldr r6, _08067BDC @ =0x000018AC
	add r1, r5, r6
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r2, #1
	neg r2, r2
	mov r0, #0xC5
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #6
	mov r1, #0
	bl Ease_Start
	mov r1, sl
	ldrb r1, [r1]
	lsl r0, r1, #1
	ldr r2, _08067BD4 @ =0x0201E140
	add r0, r0, r2
	ldrh r1, [r0]
	sub r1, #1
	strh r1, [r0]
	ldr r3, _08067BE0 @ =0x00000634
	add r7, r5, r3
	mov r0, #1
	ldrb r6, [r7]
	eor r0, r6
	strb r0, [r7]
	ldr r0, _08067BE4 @ =0x0808749C
	mov r2, #0
	ldsh r1, [r0, r2]
	str r1, [sp, #0x10]
	add r3, #0xA
	add r3, r3, r5
	mov r8, r3
	ldrh r6, [r3]
	add r3, r6, r1
	mov r0, #0xFF
	mov r9, r0
	and r3, r0
	asr r3, r3, #3
	str r4, [sp, #0]
	mov r0, #5
	str r0, [sp, #4]
	ldr r1, _08067BE8 @ =0x0201E160
	str r1, [sp, #8]
	mov r0, #0
	ldr r1, _08067BEC @ =0x0600C000
	mov r2, #0
	bl FillMapRectWrap
	mov r2, sl
	ldrb r4, [r2]
	lsl r1, r4, #1
	ldr r3, _08067BD4 @ =0x0201E140
	add r2, r1, r3
	ldr r6, _08067BF0 @ =0x0201EFC0
	add r0, r4, r6
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	ldr r6, _08067BF4 @ =0x00001494
	add r0, r5, r6
	add r1, r1, r0
	ldrh r0, [r2]
	ldrh r1, [r1]
	cmp r0, r1
	bcs _08067B1E
	ldrh r2, [r2]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, r8
	ldrh r1, [r1]
	ldr r2, [sp, #0x10]
	add r3, r1, r2
	mov r4, r9
	and r3, r4
	lsr r3, r3, #3
	ldr r6, _08067BE8 @ =0x0201E160
	str r6, [sp, #0]
	ldr r1, _08067BEC @ =0x0600C000
	mov r2, #0
	bl DeckEdit_DrawCursorRowName
	mov r1, sl
	ldrb r0, [r1]
	ldr r2, _08067BF0 @ =0x0201EFC0
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r2, r0, #1
	ldr r3, _08067BD4 @ =0x0201E140
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrb r2, [r7]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r4, _08067BF8 @ =0x06008000
	add r1, r1, r4
	bl LoadCardArt8bpp
	mov r6, #0xC6
	lsl r6, r6, #3
	add r1, r5, r6
	mov r0, r9
	ldrh r1, [r1]
	and r0, r1
	lsr r0, r0, #3
	add r0, #0x13
	ldr r1, _08067BFC @ =0x00000632
	add r2, r5, r1
	mov r1, r9
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	sub r1, #8
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrb r2, [r7]
	mov r3, #0
	str r3, [sp, #0]
	mov r3, #1
	bl DeckEdit_DrawPortraitTilemap
_08067B1E:
	mov r4, r8
	ldrh r0, [r4]
	sub r0, #0x28
	strh r0, [r4]
	ldr r6, _08067BFC @ =0x00000632
	add r1, r5, r6
	ldrh r0, [r1]
	sub r0, #0x50
	strh r0, [r1]
	ldr r0, _08067C00 @ =0x00000635
	add r1, r5, r0
	mov r0, #1
	strb r0, [r1]
	ldr r6, _08067C04 @ =0x0600D000
	ldr r0, _08067C08 @ =0x08087494
	mov r1, #0
	ldsh r4, [r0, r1]
	ldr r2, _08067C0C @ =0x0000063A
	add r5, r5, r2
	ldrh r0, [r5]
	add r3, r0, r4
	mov r1, r9
	and r3, r1
	asr r3, r3, #3
	mov r0, #0x1B
	str r0, [sp, #0]
	mov r0, #2
	str r0, [sp, #4]
	ldr r2, _08067BE8 @ =0x0201E160
	str r2, [sp, #8]
	mov r0, #0
	add r1, r6, #0
	mov r2, #3
	bl FillMapRectWrap
	mov r0, sl
	ldrb r3, [r0]
	lsl r0, r3, #1
	ldr r1, _08067BD4 @ =0x0201E140
	add r2, r0, r1
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #2
	cmp r0, #0
	blt _08067C10
	ldr r1, _08067BF0 @ =0x0201EFC0
	add r0, r3, r1
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #2
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrh r5, [r5]
	add r3, r5, r4
	mov r2, r9
	and r3, r2
	lsr r3, r3, #3
	ldr r4, _08067BE8 @ =0x0201E160
	str r4, [sp, #0]
	mov r5, #0
	str r5, [sp, #4]
	add r1, r6, #0
	mov r2, #0
	bl DeckEdit_DrawListRowName
	mov r6, sl
	ldrb r0, [r6]
	ldr r2, _08067BF0 @ =0x0201EFC0
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r2, r0, #1
	ldr r3, _08067BD4 @ =0x0201E140
	add r2, r2, r3
	ldrh r2, [r2]
	sub r2, #2
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	bl DeckEdit_GetListCard
	ldr r4, [sp, #0xC]
	strh r0, [r4]
	b _08067C18
_08067BCC: .4byte 0x0201DB20
_08067BD0: .4byte 0x00001C1C
_08067BD4: .4byte 0x0201E140
_08067BD8: .4byte 0x00001C58
_08067BDC: .4byte 0x000018AC
_08067BE0: .4byte 0x00000634
_08067BE4: .4byte gDeckEditCursorRowRedrawY
_08067BE8: .4byte 0x0201E160
_08067BEC: .4byte 0x0600C000
_08067BF0: .4byte 0x0201EFC0
_08067BF4: .4byte 0x00001494
_08067BF8: .4byte 0x06008000
_08067BFC: .4byte 0x00000632
_08067C00: .4byte 0x00000635
_08067C04: .4byte 0x0600D000
_08067C08: .4byte gDeckEditListRowRedrawY
_08067C0C: .4byte 0x0000063A
_08067C10:
	ldr r5, _08067D60 @ =0x0000FFFF
	add r0, r5, #0
	ldr r6, [sp, #0xC]
	strh r0, [r6]
_08067C18:
	ldr r0, _08067D64 @ =0x08087494
	mov r2, #2
	ldsh r1, [r0, r2]
	mov r9, r1
	ldr r6, _08067D68 @ =0x0201DB20
	ldr r3, _08067D6C @ =0x0000063A
	add r5, r6, r3
	ldrh r3, [r5]
	add r3, r9
	mov r7, #0xFF
	and r3, r7
	asr r3, r3, #3
	mov r0, #0x1B
	str r0, [sp, #0]
	mov r4, #2
	str r4, [sp, #4]
	mov r0, #0xC8
	lsl r0, r0, #3
	add r0, r0, r6
	mov r8, r0
	str r0, [sp, #8]
	mov r0, #0
	ldr r1, _08067D70 @ =0x0600D000
	mov r2, #3
	bl FillMapRectWrap
	ldr r1, _08067D74 @ =0x0201F73C
	ldrb r4, [r1]
	lsl r1, r4, #1
	ldr r2, _08067D78 @ =0x0201E140
	add r0, r1, r2
	ldrh r2, [r0]
	add r2, #1
	ldr r3, _08067D7C @ =0x0201EFC0
	add r0, r4, r3
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	ldr r0, _08067D80 @ =0x00001494
	add r0, r0, r6
	mov sl, r0
	add r1, sl
	ldrh r1, [r1]
	cmp r2, r1
	bge _08067C9E
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldrh r3, [r5]
	add r3, r9
	and r3, r7
	lsr r3, r3, #3
	mov r1, r8
	str r1, [sp, #0]
	mov r1, #3
	str r1, [sp, #4]
	ldr r1, _08067D70 @ =0x0600D000
	mov r2, #0
	bl DeckEdit_DrawListRowName
_08067C9E:
	ldrh r0, [r5]
	sub r0, #0x10
	strh r0, [r5]
	ldr r5, _08067D84 @ =0x0600C000
	ldr r2, _08067D88 @ =0x0000063E
	add r4, r6, r2
	ldrh r2, [r4]
	add r2, #0x38
	and r2, r7
	lsr r2, r2, #3
	add r0, r5, #0
	mov r1, #0xB
	mov r3, r8
	bl DeckEdit_DrawAtkDef
	ldrh r2, [r4]
	add r2, #0x38
	and r2, r7
	lsr r2, r2, #3
	add r0, r5, #0
	mov r1, #0x11
	mov r3, #6
	bl DeckEdit_DrawLevelStars
	mov r0, #0
	bl DeckEdit_DrawCardIcons
	ldr r3, _08067D74 @ =0x0201F73C
	ldrb r1, [r3]
	lsl r2, r1, #1
	ldr r4, _08067D7C @ =0x0201EFC0
	add r1, r1, r4
	ldrb r5, [r1]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #1
	add r0, r2, r0
	add r0, sl
	ldrh r0, [r0]
	ldr r1, _08067D78 @ =0x0201E140
	add r2, r2, r1
	ldrh r1, [r2]
	ldr r3, _08067D8C @ =0x00001BB0
	add r2, r6, r3
	bl DeckEdit_CalcScrollBar
	ldr r4, _08067D90 @ =0x00001BB5
	add r1, r6, r4
	ldrb r0, [r1]
	cmp r0, #0
	beq _08067D14
	mov r5, #2
	strb r5, [r1]
	ldr r0, _08067D94 @ =0x00001BB4
	add r1, r6, r0
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_08067D14:
	ldr r3, _08067D98 @ =0x00000635
	add r0, r6, r3
	ldrb r0, [r0]
	ldr r4, [sp, #0xC]
	ldrh r1, [r4]
	ldr r5, _08067D74 @ =0x0201F73C
	ldrb r4, [r5]
	ldr r2, _08067D7C @ =0x0201EFC0
	add r3, r4, r2
	ldrb r5, [r3]
	lsl r2, r5, #1
	add r2, r2, r5
	add r2, r2, r4
	lsl r2, r2, #1
	add r2, sl
	ldrh r2, [r2]
	ldr r4, _08067D9C @ =0x00001BB8
	add r3, r6, r4
	ldr r5, _08067DA0 @ =0x000018B0
	add r4, r6, r5
	str r4, [sp, #0]
	bl DeckEdit_ScrollFrameSlots
	mov r0, #1
	bl DeckEdit_RotateListRowRing
	mov r0, #0
	bl PlaySE
_08067D4E:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08067D60: .4byte 0x0000FFFF
_08067D64: .4byte gDeckEditListRowRedrawY
_08067D68: .4byte 0x0201DB20
_08067D6C: .4byte 0x0000063A
_08067D70: .4byte 0x0600D000
_08067D74: .4byte 0x0201F73C
_08067D78: .4byte 0x0201E140
_08067D7C: .4byte 0x0201EFC0
_08067D80: .4byte 0x00001494
_08067D84: .4byte 0x0600C000
_08067D88: .4byte 0x0000063E
_08067D8C: .4byte 0x00001BB0
_08067D90: .4byte 0x00001BB5
_08067D94: .4byte 0x00001BB4
_08067D98: .4byte 0x00000635
_08067D9C: .4byte 0x00001BB8
_08067DA0: .4byte 0x000018B0
	thumb_func_end DeckEdit_ScrollListUp

