	thumb_func_start DeckEdit_Update
DeckEdit_Update: @ 0x0806DBB0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	ldr r0, _0806DDE4 @ =0x03000040
	ldr r1, _0806DDE8 @ =0x000003FF
	ldrh r0, [r0, #6]
	and r0, r1
	str r0, [sp, #0x28]
	ldr r4, _0806DDEC @ =0x0201E148
	add r0, r4, #0
	bl Ease_Tick
	ldr r2, _0806DDF0 @ =0x000010E8
	add r1, r4, r2
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0806DBDE
	b _0806DE3C
_0806DBDE:
	mov r3, #2
	ldsh r0, [r4, r3]
	cmp r0, #4
	bne _0806DBEC
	ldrb r5, [r4, #0xD]
	cmp r5, #3
	beq _0806DBFA
_0806DBEC:
	cmp r0, #3
	beq _0806DBF2
	b _0806DE3C
_0806DBF2:
	ldrb r4, [r4, #0xD]
	cmp r4, #4
	beq _0806DBFA
	b _0806DE3C
_0806DBFA:
	ldr r6, _0806DDF4 @ =0x0201DB20
	ldr r0, _0806DDF8 @ =0x00001710
	add r1, r6, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r7, _0806DDFC @ =0x0600D000
	ldr r2, _0806DE00 @ =0x01000200
	add r0, sp, #0x20
	add r1, r7, #0
	bl CpuFastSet
	ldr r4, _0806DE04 @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _0806DE08 @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #2
	cmp r0, #0
	blt _0806DC5E
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r6, r4
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #2
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r5, _0806DE0C @ =0x0000063A
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
_0806DC5E:
	ldr r4, _0806DE04 @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _0806DE08 @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #1
	mov r4, #0xC8
	lsl r4, r4, #3
	add r4, r4, r6
	mov sl, r4
	cmp r0, #0
	blt _0806DCAA
	mov r5, #0xA5
	lsl r5, r5, #5
	add r0, r6, r5
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #1
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r2, _0806DE0C @ =0x0000063A
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
_0806DCAA:
	ldr r3, _0806DE04 @ =0x0201F73C
	ldrb r4, [r3]
	lsl r1, r4, #1
	ldr r0, _0806DE08 @ =0x0201E140
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
	ldr r0, _0806DE10 @ =0x00001494
	add r0, r0, r6
	mov r8, r0
	add r1, r8
	ldrh r1, [r1]
	cmp r2, r1
	bge _0806DD06
	ldrh r2, [r5]
	add r2, #1
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r2, _0806DE0C @ =0x0000063A
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
_0806DD06:
	ldr r5, _0806DE04 @ =0x0201F73C
	ldrb r4, [r5]
	lsl r1, r4, #1
	ldr r0, _0806DE08 @ =0x0201E140
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
	bge _0806DD56
	ldrh r2, [r5]
	add r2, #2
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r4, _0806DE0C @ =0x0000063A
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
_0806DD56:
	ldr r7, _0806DE14 @ =0x0600C000
	ldr r0, _0806DE18 @ =0x0000063E
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
	ldr r2, _0806DE04 @ =0x0201F73C
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
	beq _0806DE1C
	ldr r6, _0806DE08 @ =0x0201E140
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
	b _0806DE3C
_0806DDE4: .4byte 0x03000040
_0806DDE8: .4byte 0x000003FF
_0806DDEC: .4byte 0x0201E148
_0806DDF0: .4byte 0x000010E8
_0806DDF4: .4byte 0x0201DB20
_0806DDF8: .4byte 0x00001710
_0806DDFC: .4byte 0x0600D000
_0806DE00: .4byte 0x01000200
_0806DE04: .4byte 0x0201F73C
_0806DE08: .4byte 0x0201E140
_0806DE0C: .4byte 0x0000063A
_0806DE10: .4byte 0x00001494
_0806DE14: .4byte 0x0600C000
_0806DE18: .4byte 0x0000063E
_0806DE1C:
	ldr r6, _0806DE88 @ =0x0201E140
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
_0806DE3C:
	ldr r5, _0806DE8C @ =0x0201DB20
	ldr r2, _0806DE90 @ =0x0000062A
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
	ldr r4, _0806DE94 @ =0x000018B0
	add r3, r5, r4
	ldr r4, _0806DE98 @ =0x00001BB8
	add r4, r4, r5
	str r4, [sp, #0]
	bl DeckEdit_TweenFrameSlots
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #1
	bge _0806DE6E
	b _0806E212
_0806DE6E:
	cmp r0, #2
	ble _0806DE9C
	cmp r0, #4
	ble _0806DE78
	b _0806E212
_0806DE78:
	ldrb r0, [r6]
	cmp r0, #1
	bne _0806DE80
	b _0806E060
_0806DE80:
	cmp r0, #2
	bne _0806DE86
	b _0806E15C
_0806DE86:
	b _0806E1BE
_0806DE88: .4byte 0x0201E140
_0806DE8C: .4byte 0x0201DB20
_0806DE90: .4byte 0x0000062A
_0806DE94: .4byte 0x000018B0
_0806DE98: .4byte 0x00001BB8
_0806DE9C:
	ldrb r0, [r6]
	cmp r0, #1
	beq _0806DEA8
	cmp r0, #2
	beq _0806DF58
	b _0806DFF2
_0806DEA8:
	ldr r1, _0806DF30 @ =0x0400001C
	mov r2, #0xC6
	lsl r2, r2, #3
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806DF34 @ =0x080875D2
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _0806DF38 @ =0x0400001E
	ldr r6, _0806DF3C @ =0x00000632
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806DF40 @ =0x04000014
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
	ldr r2, _0806DF44 @ =0x04000016
	add r6, #8
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806DF48 @ =0x04000010
	ldr r2, _0806DF4C @ =0x0000063C
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
	ldr r2, _0806DF50 @ =0x04000012
	ldr r4, _0806DF54 @ =0x0000063E
	add r1, r5, r4
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	b _0806E212
	.align 2, 0
_0806DF30: .4byte 0x0400001C
_0806DF34: .4byte gDeckEditEaseCurve
_0806DF38: .4byte 0x0400001E
_0806DF3C: .4byte 0x00000632
_0806DF40: .4byte 0x04000014
_0806DF44: .4byte 0x04000016
_0806DF48: .4byte 0x04000010
_0806DF4C: .4byte 0x0000063C
_0806DF50: .4byte 0x04000012
_0806DF54: .4byte 0x0000063E
_0806DF58:
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806E038 @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _0806E03C @ =0x00000632
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
	ldr r2, _0806E040 @ =0x0000063A
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
	ldr r2, _0806E044 @ =0x0000063E
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	ldr r4, _0806E048 @ =0x00001BB5
	add r2, r5, r4
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806DFD2
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _0806E04C @ =0x00001BB4
	add r1, r5, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0806DFD2:
	ldr r3, _0806E050 @ =0x00001BB7
	add r2, r5, r3
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806DFEC
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r4, _0806E054 @ =0x00001BB6
	add r1, r5, r4
	ldrb r5, [r1]
	orr r0, r5
	strb r0, [r1]
_0806DFEC:
	mov r0, #0
	mov r6, r8
	strb r0, [r6]
_0806DFF2:
	ldr r2, _0806E058 @ =0x0400001C
	ldr r1, _0806E05C @ =0x0201DB20
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r4, _0806E03C @ =0x00000632
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
	ldr r6, _0806E040 @ =0x0000063A
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
	b _0806E212
	.align 2, 0
_0806E038: .4byte gDeckEditEaseCurve
_0806E03C: .4byte 0x00000632
_0806E040: .4byte 0x0000063A
_0806E044: .4byte 0x0000063E
_0806E048: .4byte 0x00001BB5
_0806E04C: .4byte 0x00001BB4
_0806E050: .4byte 0x00001BB7
_0806E054: .4byte 0x00001BB6
_0806E058: .4byte 0x0400001C
_0806E05C: .4byte 0x0201DB20
_0806E060:
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806E0EC @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _0806E0F0 @ =0x0400001C
	mov r3, #0xC6
	lsl r3, r3, #3
	add r1, r5, r3
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806E0F4 @ =0x0400001E
	ldr r6, _0806E0F8 @ =0x00000632
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
	ldr r1, _0806E0FC @ =0x04000050
	ldr r6, _0806E100 @ =0x00003F43
	add r0, r6, #0
	strh r0, [r1]
	mov r1, #0
	ldsh r0, [r7, r1]
	cmp r0, #3
	bgt _0806E118
	ldr r0, _0806E104 @ =0x04000014
	strh r2, [r0]
	ldr r1, _0806E108 @ =0x04000016
	ldr r4, _0806E10C @ =0x0000063A
	add r0, r5, r4
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _0806E110 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0806E114 @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	lsr r0, r3, #0x19
	bl SetBldAlpha
	b _0806E212
	.align 2, 0
_0806E0EC: .4byte gDeckEditEaseCurve
_0806E0F0: .4byte 0x0400001C
_0806E0F4: .4byte 0x0400001E
_0806E0F8: .4byte 0x00000632
_0806E0FC: .4byte 0x04000050
_0806E100: .4byte 0x00003F43
_0806E104: .4byte 0x04000014
_0806E108: .4byte 0x04000016
_0806E10C: .4byte 0x0000063A
_0806E110: .4byte 0x04000010
_0806E114: .4byte 0x0000063E
_0806E118:
	ldr r0, _0806E144 @ =0x04000014
	ldr r1, _0806E148 @ =0x0000FFC0
	add r2, r2, r1
	strh r2, [r0]
	ldr r1, _0806E14C @ =0x04000016
	ldr r3, _0806E150 @ =0x0000063A
	add r0, r5, r3
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _0806E154 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0806E158 @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x20
	sub r0, r0, r4
	asr r0, r0, #1
	bl SetBldAlpha
	b _0806E212
_0806E144: .4byte 0x04000014
_0806E148: .4byte 0x0000FFC0
_0806E14C: .4byte 0x04000016
_0806E150: .4byte 0x0000063A
_0806E154: .4byte 0x04000010
_0806E158: .4byte 0x0000063E
_0806E15C:
	add r4, r5, #0
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r2, _0806E23C @ =0x080875D2
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
	ldr r0, _0806E240 @ =0x00001BB5
	add r2, r4, r0
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806E19E
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r2, _0806E244 @ =0x00001BB4
	add r1, r4, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_0806E19E:
	ldr r5, _0806E248 @ =0x00001BB7
	add r2, r4, r5
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806E1B8
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _0806E24C @ =0x00001BB6
	add r1, r4, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0806E1B8:
	mov r0, #0
	mov r3, r8
	strb r0, [r3]
_0806E1BE:
	ldr r2, _0806E250 @ =0x0400001C
	ldr r1, _0806E254 @ =0x0201DB20
	mov r4, #0xC6
	lsl r4, r4, #3
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r5, _0806E258 @ =0x00000632
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
	ldr r3, _0806E25C @ =0x0000063A
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
	ldr r1, _0806E260 @ =0x04000050
	ldr r6, _0806E264 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #2
	mov r2, #0x80
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
_0806E212:
	ldr r3, _0806E254 @ =0x0201DB20
	ldr r4, _0806E268 @ =0x0000061E
	add r0, r3, r4
	ldrb r0, [r0]
	add r6, r3, #0
	cmp r0, #0
	beq _0806E224
	bl _0806EB2E @ far jump
_0806E224:
	ldr r5, _0806E26C @ =0x00001C48
	add r4, r3, r5
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, #0
	beq _0806E270
	cmp r0, #1
	bne _0806E238
	b _0806E70C
_0806E238:
	bl _0806EB2E @ far jump
_0806E23C: .4byte gDeckEditEaseCurve
_0806E240: .4byte 0x00001BB5
_0806E244: .4byte 0x00001BB4
_0806E248: .4byte 0x00001BB7
_0806E24C: .4byte 0x00001BB6
_0806E250: .4byte 0x0400001C
_0806E254: .4byte 0x0201DB20
_0806E258: .4byte 0x00000632
_0806E25C: .4byte 0x0000063A
_0806E260: .4byte 0x04000050
_0806E264: .4byte 0x00003FC8
_0806E268: .4byte 0x0000061E
_0806E26C: .4byte 0x00001C48
_0806E270:
	mov r2, #0xC5
	lsl r2, r2, #3
	add r0, r3, r2
	ldrb r0, [r0]
	cmp r0, #1
	bne _0806E27E
	b _0806E6CA
_0806E27E:
	ldr r5, [sp, #0x28]
	cmp r5, #0x40
	beq _0806E2BA
	cmp r5, #0x40
	bhi _0806E28E
	cmp r5, #2
	beq _0806E296
	b _0806E2CA
_0806E28E:
	ldr r0, [sp, #0x28]
	cmp r0, #0x80
	beq _0806E2C2
	b _0806E2CA
_0806E296:
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r3, r2
	mov r0, #0
	mov r2, #0
	bl FadeStart
	mov r0, #0x1F
	neg r0, r0
	ldrb r3, [r4]
	and r0, r3
	strb r0, [r4]
	mov r0, #2
	bl PlaySE
	b _0806E6CA
_0806E2BA:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListUp
	b _0806E6CA
_0806E2C2:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListDown
	b _0806E6CA
_0806E2CA:
	add r4, r6, #0
	ldr r5, _0806E304 @ =0x00001C1C
	add r0, r4, r5
	ldrb r1, [r0]
	lsl r2, r1, #1
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r4, r3
	add r1, r1, r0
	ldrb r5, [r1]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #1
	add r0, r2, r0
	sub r3, #0xC
	add r1, r4, r3
	add r3, r0, r1
	ldrh r5, [r3]
	cmp r5, #5
	bhi _0806E2F4
	b _0806E6CA
_0806E2F4:
	ldr r0, [sp, #0x28]
	cmp r0, #0x10
	beq _0806E308
	cmp r0, #0x20
	bne _0806E300
	b _0806E4F4
_0806E300:
	b _0806E6CA
	.align 2, 0
_0806E304: .4byte 0x00001C1C
_0806E308:
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r4, r1
	add r1, r2, r0
	ldrh r2, [r1]
	add r2, #5
	ldrh r0, [r3]
	sub r0, #1
	cmp r2, r0
	ble _0806E322
	mov r0, #0
	strh r0, [r1]
	b _0806E324
_0806E322:
	strh r2, [r1]
_0806E324:
	ldr r6, _0806E468 @ =0x0201DB20
	ldr r2, _0806E46C @ =0x000018AC
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
	ldr r5, _0806E470 @ =0x00000634
	add r4, r6, r5
	mov r0, #1
	mov r9, r0
	mov r0, r9
	ldrb r1, [r4]
	eor r0, r1
	strb r0, [r4]
	ldr r2, _0806E474 @ =0x00001C1C
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
	ldr r3, _0806E478 @ =0x06008000
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
	ldr r3, _0806E47C @ =0x00000632
	add r2, r6, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldrb r2, [r4]
	mov r3, #1
	bl DeckEdit_PlaceCardArt
	ldr r4, _0806E480 @ =0x00000635
	add r1, r6, r4
	mov r0, #3
	strb r0, [r1]
	ldr r0, _0806E484 @ =0x00001710
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
	ldr r5, _0806E488 @ =0x00001494
	add r1, r6, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r2, r7
	ldrh r1, [r2]
	ldr r3, _0806E48C @ =0x00001BB0
	add r2, r6, r3
	bl DeckEdit_CalcScrollBar
	ldr r4, _0806E490 @ =0x00001BB7
	add r1, r6, r4
	ldrb r0, [r1]
	cmp r0, #0
	beq _0806E400
	mov r0, #2
	strb r0, [r1]
	ldr r5, _0806E494 @ =0x00001BB6
	add r1, r6, r5
	mov r0, r9
	ldrb r6, [r1]
	orr r0, r6
	strb r0, [r1]
_0806E400:
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
_0806E414:
	ldrh r3, [r7]
	lsl r2, r3, #0x10
	cmp r2, #0
	blt _0806E4A0
	ldr r6, _0806E468 @ =0x0201DB20
	ldr r4, _0806E474 @ =0x00001C1C
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
	bcs _0806E4A0
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _0806E498 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _0806E49C @ =0x00001BB8
	add r3, r6, r4
	bl DeckEdit_InitFrameSlot
	b _0806E4AE
	.align 2, 0
_0806E468: .4byte 0x0201DB20
_0806E46C: .4byte 0x000018AC
_0806E470: .4byte 0x00000634
_0806E474: .4byte 0x00001C1C
_0806E478: .4byte 0x06008000
_0806E47C: .4byte 0x00000632
_0806E480: .4byte 0x00000635
_0806E484: .4byte 0x00001710
_0806E488: .4byte 0x00001494
_0806E48C: .4byte 0x00001BB0
_0806E490: .4byte 0x00001BB7
_0806E494: .4byte 0x00001BB6
_0806E498: .4byte 0x000018B0
_0806E49C: .4byte 0x00001BB8
_0806E4A0:
	ldr r0, _0806E4E0 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _0806E4E4 @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_0806E4AE:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _0806E414
	ldr r1, _0806E4E0 @ =0x0201DB20
	ldr r0, _0806E4E8 @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _0806E4EC @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _0806E4F0 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl PlaySE
	b _0806E6CA
_0806E4E0: .4byte 0x0201DB20
_0806E4E4: .4byte 0x00001BC4
_0806E4E8: .4byte 0x00001C14
_0806E4EC: .4byte 0x00001BB8
_0806E4F0: .4byte 0x00001C58
_0806E4F4:
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r6, r1
	add r1, r2, r0
	ldrh r0, [r1]
	cmp r0, #4
	bhi _0806E506
	sub r0, r5, #1
	b _0806E508
_0806E506:
	sub r0, #5
_0806E508:
	strh r0, [r1]
	ldr r7, _0806E654 @ =0x0201DB20
	ldr r2, _0806E658 @ =0x000018AC
	add r1, r7, r2
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r2, #1
	neg r2, r2
	mov r4, #0xC5
	lsl r4, r4, #3
	add r3, r7, r4
	mov r0, #6
	mov r1, #0
	bl Ease_Start
	ldr r6, _0806E65C @ =0x00000634
	add r5, r7, r6
	mov r0, #1
	mov sl, r0
	mov r0, sl
	ldrb r1, [r5]
	eor r0, r1
	strb r0, [r5]
	ldr r2, _0806E660 @ =0x00001C1C
	add r2, r2, r7
	mov r9, r2
	ldrb r0, [r2]
	mov r3, #0xA5
	lsl r3, r3, #5
	add r6, r7, r3
	add r1, r0, r6
	ldrb r1, [r1]
	lsl r2, r0, #1
	sub r4, #8
	add r4, r4, r7
	mov r8, r4
	add r2, r8
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	ldrb r2, [r5]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _0806E664 @ =0x06008000
	add r1, r1, r3
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
	ldr r3, _0806E668 @ =0x00000632
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
	ldr r4, _0806E66C @ =0x00000635
	add r1, r7, r4
	mov r0, #4
	strb r0, [r1]
	ldr r5, _0806E670 @ =0x00001710
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
	ldr r5, _0806E674 @ =0x00001494
	add r1, r7, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r8
	ldrh r1, [r2]
	ldr r6, _0806E678 @ =0x00001BB0
	add r2, r7, r6
	bl DeckEdit_CalcScrollBar
	ldr r0, _0806E67C @ =0x00001BB5
	add r1, r7, r0
	ldrb r0, [r1]
	cmp r0, #0
	beq _0806E5EE
	mov r0, #2
	strb r0, [r1]
	ldr r2, _0806E680 @ =0x00001BB4
	add r1, r7, r2
	mov r0, sl
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_0806E5EE:
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
_0806E602:
	ldrh r6, [r7]
	lsl r2, r6, #0x10
	cmp r2, #0
	blt _0806E68C
	ldr r6, _0806E654 @ =0x0201DB20
	ldr r1, _0806E660 @ =0x00001C1C
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
	ldr r1, _0806E674 @ =0x00001494
	add r1, r1, r6
	mov r8, r1
	add r0, r8
	lsr r1, r2, #0x10
	ldrh r0, [r0]
	cmp r1, r0
	bcs _0806E68C
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _0806E684 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _0806E688 @ =0x00001BB8
	add r3, r6, r4
	bl DeckEdit_InitFrameSlot
	b _0806E69A
_0806E654: .4byte 0x0201DB20
_0806E658: .4byte 0x000018AC
_0806E65C: .4byte 0x00000634
_0806E660: .4byte 0x00001C1C
_0806E664: .4byte 0x06008000
_0806E668: .4byte 0x00000632
_0806E66C: .4byte 0x00000635
_0806E670: .4byte 0x00001710
_0806E674: .4byte 0x00001494
_0806E678: .4byte 0x00001BB0
_0806E67C: .4byte 0x00001BB5
_0806E680: .4byte 0x00001BB4
_0806E684: .4byte 0x000018B0
_0806E688: .4byte 0x00001BB8
_0806E68C:
	ldr r0, _0806E6E8 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _0806E6EC @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_0806E69A:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _0806E602
	ldr r1, _0806E6E8 @ =0x0201DB20
	ldr r0, _0806E6F0 @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _0806E6F4 @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _0806E6F8 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl PlaySE
_0806E6CA:
	ldr r5, [sp, #0x28]
	cmp r5, #1
	bne _0806E700
	ldr r0, _0806E6E8 @ =0x0201DB20
	ldr r6, _0806E6FC @ =0x00001C3D
	add r0, r0, r6
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #1
	orr r1, r2
	strb r1, [r0]
	b _0806EAC4
	.align 2, 0
_0806E6E8: .4byte 0x0201DB20
_0806E6EC: .4byte 0x00001BC4
_0806E6F0: .4byte 0x00001C14
_0806E6F4: .4byte 0x00001BB8
_0806E6F8: .4byte 0x00001C58
_0806E6FC: .4byte 0x00001C3D
_0806E700:
	ldr r0, _0806E708 @ =0x0201F73C
	bl DeckEdit_HandleShoulderKeys
	b _0806EB2E
_0806E708: .4byte 0x0201F73C
_0806E70C:
	mov r4, #0xE1
	lsl r4, r4, #5
	add r0, r3, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _0806E71A
	b _0806EB2E
_0806E71A:
	ldr r5, [sp, #0x28]
	cmp r5, #2
	bne _0806E722
	b _0806EAD8
_0806E722:
	cmp r5, #2
	bhi _0806E72E
	cmp r5, #1
	bne _0806E72C
	b _0806E848
_0806E72C:
	b _0806EAF8
_0806E72E:
	ldr r6, [sp, #0x28]
	cmp r6, #0x10
	beq _0806E73A
	cmp r6, #0x20
	beq _0806E7B4
	b _0806EAF8
_0806E73A:
	ldr r0, _0806E7A4 @ =0x00001C3C
	add r5, r3, r0
	ldr r2, [r5]
	lsl r1, r2, #0xE
	lsr r1, r1, #0x1D
	add r1, #1
	mov r7, #7
	add r0, r1, #0
	and r0, r7
	lsl r0, r0, #0xF
	ldr r6, _0806E7A8 @ =0xFFFC7FFF
	add r4, r6, #0
	and r4, r2
	orr r4, r0
	str r4, [r5]
	mov r0, #7
	and r1, r0
	ldr r2, _0806E7AC @ =0x00001C1C
	add r0, r3, r2
	ldrb r0, [r0]
	add r0, #1
	cmp r1, r0
	bne _0806E77A
	lsl r0, r4, #0xE
	lsr r0, r0, #0x1D
	add r0, #1
	and r0, r7
	lsl r0, r0, #0xF
	add r1, r6, #0
	and r1, r4
	orr r1, r0
	str r1, [r5]
_0806E77A:
	ldr r2, [r5]
	mov r1, #0xE0
	lsl r1, r1, #0xA
	add r0, r2, #0
	and r0, r1
	cmp r0, r1
	bne _0806E78C
	and r2, r6
	str r2, [r5]
_0806E78C:
	ldr r2, [r5]
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	and r0, r7
	lsl r0, r0, #0xF
	add r1, r6, #0
	and r1, r2
	orr r1, r0
	str r1, [r5]
	ldr r4, _0806E7B0 @ =0x00001C3D
	add r2, r3, r4
	b _0806E822
_0806E7A4: .4byte 0x00001C3C
_0806E7A8: .4byte 0xFFFC7FFF
_0806E7AC: .4byte 0x00001C1C
_0806E7B0: .4byte 0x00001C3D
_0806E7B4:
	ldr r6, _0806E7D4 @ =0x00001C3C
	add r5, r3, r6
	ldr r2, [r5]
	mov r0, #0xE0
	lsl r0, r0, #0xA
	and r0, r2
	cmp r0, #0
	bne _0806E7DC
	ldr r0, _0806E7D8 @ =0xFFFC7FFF
	and r0, r2
	mov r1, #0xC0
	lsl r1, r1, #0xA
	orr r0, r1
	str r0, [r5]
	b _0806E81C
	.align 2, 0
_0806E7D4: .4byte 0x00001C3C
_0806E7D8: .4byte 0xFFFC7FFF
_0806E7DC:
	lsl r1, r2, #0xE
	lsr r1, r1, #0x1D
	sub r1, #1
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	mov r6, #7
	and r0, r6
	lsl r0, r0, #0xF
	ldr r7, _0806E838 @ =0xFFFC7FFF
	add r4, r7, #0
	and r4, r2
	orr r4, r0
	str r4, [r5]
	mov r0, #7
	and r1, r0
	ldr r2, _0806E83C @ =0x00001C1C
	add r0, r3, r2
	ldrb r0, [r0]
	add r0, #1
	cmp r1, r0
	bne _0806E81C
	lsl r0, r4, #0xE
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	and r0, r6
	lsl r0, r0, #0xF
	add r1, r7, #0
	and r1, r4
	orr r1, r0
	str r1, [r5]
_0806E81C:
	ldr r2, _0806E840 @ =0x0201DB20
	ldr r4, _0806E844 @ =0x00001C3D
	add r2, r2, r4
_0806E822:
	mov r0, #8
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0
	bl PlaySE
	b _0806EB2E
_0806E838: .4byte 0xFFFC7FFF
_0806E83C: .4byte 0x00001C1C
_0806E840: .4byte 0x0201DB20
_0806E844: .4byte 0x00001C3D
_0806E848:
	ldr r6, _0806E864 @ =0x00001C3C
	add r0, r3, r6
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	cmp r0, #6
	bls _0806E858
	b _0806EB2E
_0806E858:
	lsl r0, r0, #2
	ldr r1, _0806E868 @ =0x0806E86C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0806E864: .4byte 0x00001C3C
_0806E868: .4byte 0x0806E86C
_0806E86C:
	.4byte _0806E888
	.4byte _0806E8E8
	.4byte _0806E914
	.4byte _0806E940
	.4byte _0806EA30
	.4byte _0806EA60
	.4byte _0806EA90
_0806E888:
	ldr r3, _0806E8D8 @ =0x0201DB20
	ldr r1, _0806E8DC @ =0x00001C1C
	add r0, r3, r1
	ldrb r2, [r0]
	mov r4, #0xA5
	lsl r4, r4, #5
	add r1, r3, r4
	add r1, r2, r1
	ldrb r5, [r1]
	lsl r0, r5, #1
	add r0, r0, r5
	add r0, r0, r2
	lsl r0, r0, #1
	ldr r6, _0806E8E0 @ =0x00001494
	add r1, r3, r6
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	bne _0806E8B0
	b _0806EB2E
_0806E8B0:
	ldr r0, _0806E8E4 @ =0x00001C48
	add r2, r3, r0
	mov r0, #0x1F
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #4
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r3, r2
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _0806EAC4
	.align 2, 0
_0806E8D8: .4byte 0x0201DB20
_0806E8DC: .4byte 0x00001C1C
_0806E8E0: .4byte 0x00001494
_0806E8E4: .4byte 0x00001C48
_0806E8E8:
	bl DeckEdit_GetSelectedCardCopies
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0806E8F4
	b _0806EA28
_0806E8F4:
	ldr r2, _0806E90C @ =0x0201DB20
	ldr r3, _0806E910 @ =0x00001C3B
	add r0, r2, r3
	ldrb r0, [r0]
	mov r4, #0xE1
	lsl r4, r4, #5
	add r2, r2, r4
	mov r1, #0
	bl DeckEdit_StartCardMove
	b _0806EB2E
	.align 2, 0
_0806E90C: .4byte 0x0201DB20
_0806E910: .4byte 0x00001C3B
_0806E914:
	bl DeckEdit_GetSelectedCardCopies
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0806E920
	b _0806EA28
_0806E920:
	ldr r2, _0806E938 @ =0x0201DB20
	ldr r5, _0806E93C @ =0x00001C3B
	add r0, r2, r5
	ldrb r0, [r0]
	mov r6, #0xE1
	lsl r6, r6, #5
	add r2, r2, r6
	mov r1, #1
	bl DeckEdit_StartCardMove
	b _0806EB2E
	.align 2, 0
_0806E938: .4byte 0x0201DB20
_0806E93C: .4byte 0x00001C3B
_0806E940:
	bl DeckEdit_GetSelectedCardCopies
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806EA28
	ldr r3, _0806E988 @ =0x0201DB20
	ldr r1, _0806E98C @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r3, r3, r4
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _0806E990 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r5, _0806E994 @ =0x08622AB4
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _0806E998 @ =0x00000776
	cmp r1, r0
	bne _0806E99C
	mov r0, #3
	b _0806E9FE
	.align 2, 0
_0806E988: .4byte 0x0201DB20
_0806E98C: .4byte 0x00001C1C
_0806E990: .4byte 0x000007FF
_0806E994: .4byte gCardIdToNumber
_0806E998: .4byte 0x00000776
_0806E99C:
	cmp r1, r0
	blt _0806E9AC
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806E9AC
	mov r0, #1
	b _0806E9FE
_0806E9AC:
	ldr r0, _0806E9D0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r6, _0806E9D4 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806E9DE
	cmp r0, #0x16
	bgt _0806E9D8
	cmp r0, #0x15
	beq _0806E9E2
	b _0806E9EA
	.align 2, 0
_0806E9D0: .4byte 0x000007FF
_0806E9D4: .4byte gCardStats
_0806E9D8:
	cmp r0, #0x17
	beq _0806E9E6
	b _0806E9EA
_0806E9DE:
	mov r0, #7
	b _0806E9FE
_0806E9E2:
	mov r0, #8
	b _0806E9FE
_0806E9E6:
	mov r0, #9
	b _0806E9FE
_0806E9EA:
	ldr r0, _0806EA18 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0806EA1C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806E9FE:
	cmp r0, #2
	beq _0806EA28
	ldr r2, _0806EA20 @ =0x0201DB20
	ldr r3, _0806EA24 @ =0x00001C3B
	add r0, r2, r3
	ldrb r0, [r0]
	mov r4, #0xE1
	lsl r4, r4, #5
	add r2, r2, r4
	mov r1, #2
	bl DeckEdit_StartCardMove
	b _0806EB2E
_0806EA18: .4byte 0x000007FF
_0806EA1C: .4byte gCardStats
_0806EA20: .4byte 0x0201DB20
_0806EA24: .4byte 0x00001C3B
_0806EA28:
	mov r0, #3
	bl PlaySE
	b _0806EB2E
_0806EA30:
	ldr r3, _0806EA58 @ =0x0201DB20
	ldr r5, _0806EA5C @ =0x00001C48
	add r2, r3, r5
	mov r0, #0x1F
	neg r0, r0
	ldrb r6, [r2]
	and r0, r6
	mov r1, #2
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r0, #0xC3
	lsl r0, r0, #3
	add r3, r3, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _0806EAC4
_0806EA58: .4byte 0x0201DB20
_0806EA5C: .4byte 0x00001C48
_0806EA60:
	ldr r3, _0806EA88 @ =0x0201DB20
	ldr r1, _0806EA8C @ =0x00001C48
	add r2, r3, r1
	mov r0, #0x1F
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	mov r1, #6
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r5, #0xC3
	lsl r5, r5, #3
	add r3, r3, r5
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _0806EAC4
_0806EA88: .4byte 0x0201DB20
_0806EA8C: .4byte 0x00001C48
_0806EA90:
	ldr r4, _0806EACC @ =0x0201DB20
	ldr r6, _0806EAD0 @ =0x00001C3D
	add r2, r4, r6
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #2
	orr r0, r1
	strb r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r4, r2
	mov r0, #0
	mov r2, #0
	bl FadeStart
	ldr r3, _0806EAD4 @ =0x00001C48
	add r4, r4, r3
	mov r0, #0x1F
	neg r0, r0
	ldrb r5, [r4]
	and r0, r5
	strb r0, [r4]
_0806EAC4:
	mov r0, #1
	bl PlaySE
	b _0806EB2E
_0806EACC: .4byte 0x0201DB20
_0806EAD0: .4byte 0x00001C3D
_0806EAD4: .4byte 0x00001C48
_0806EAD8:
	ldr r6, _0806EAF4 @ =0x00001C3D
	add r0, r3, r6
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	mov r0, #2
	bl PlaySE
	b _0806EB2E
	.align 2, 0
_0806EAF4: .4byte 0x00001C3D
_0806EAF8:
	ldr r4, _0806EB18 @ =0x0201F73C
	add r0, r4, #0
	bl DeckEdit_HandleShoulderKeys
	ldr r3, _0806EB1C @ =0xFFFFEA0C
	add r4, r4, r3
	ldrb r4, [r4]
	cmp r4, #1
	beq _0806EB2E
	ldr r4, [sp, #0x28]
	cmp r4, #0x40
	beq _0806EB20
	cmp r4, #0x80
	beq _0806EB28
	b _0806EB2E
	.align 2, 0
_0806EB18: .4byte 0x0201F73C
_0806EB1C: .4byte 0xFFFFEA0C
_0806EB20:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListUp
	b _0806EB2E
_0806EB28:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListDown
_0806EB2E:
	ldr r0, _0806EC70 @ =0x081A6524
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
	ldr r4, _0806EC74 @ =0x0201DB20
	str r4, [sp, #0x1C]
	mov r1, #5
	mov r2, #0xC
	bl OamListAddSpriteGroup
	ldr r5, _0806EC78 @ =0x000017DA
	add r1, r4, r5
	mov r0, #1
	strb r0, [r1]
	ldr r6, _0806EC7C @ =0x00001C1C
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
	ldr r2, _0806EC80 @ =0x00001494
	add r1, r4, r2
	add r3, r3, r1
	ldrh r1, [r3]
	ldr r3, _0806EC84 @ =0x00001BB0
	add r2, r4, r3
	bl DeckEdit_DrawScrollBar
	ldr r5, _0806EC88 @ =0x00001BBC
	add r0, r4, r5
	add r1, r4, #0
	bl DeckEdit_DrawFrameSlots
	mov r5, #1
	ldr r6, _0806EC8C @ =0x000018B0
	add r4, r4, r6
_0806EBA0:
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	add r0, r0, r4
	bl ObjAffineApply
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #6
	bls _0806EBA0
	ldr r0, _0806EC90 @ =0x0201F740
	mov r8, r0
	bl DeckEdit_UpdateCardMove
	mov r6, r8
	sub r6, #4
	mov r1, r8
	sub r1, #3
	ldr r5, _0806EC94 @ =0xFFFFFAF8
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
	ldr r0, _0806EC98 @ =0x081A6EA4
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
	ldr r4, _0806EC9C @ =0xFFFFE3E0
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
	ldr r0, _0806ECA0 @ =0xFFFFE9F8
	add r0, r8
	bl FadeTick
	ldr r0, _0806ECA4 @ =0xFFFFE9FE
	add r0, r8
	ldrb r1, [r0]
	cmp r1, #2
	beq _0806ECB0
	cmp r1, #3
	bne _0806ECB4
	strb r7, [r0]
	ldr r1, _0806ECA8 @ =0x04000050
	ldr r2, _0806ECAC @ =0x00003FC8
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r3, #0x80
	lsl r3, r3, #5
	add r0, r3, #0
	strh r0, [r1]
	b _0806ED32
	.align 2, 0
_0806EC70: .4byte gUnk_081A6524
_0806EC74: .4byte 0x0201DB20
_0806EC78: .4byte 0x000017DA
_0806EC7C: .4byte 0x00001C1C
_0806EC80: .4byte 0x00001494
_0806EC84: .4byte 0x00001BB0
_0806EC88: .4byte 0x00001BBC
_0806EC8C: .4byte 0x000018B0
_0806EC90: .4byte 0x0201F740
_0806EC94: .4byte 0xFFFFFAF8
_0806EC98: .4byte gUnk_081A6EA4
_0806EC9C: .4byte 0xFFFFE3E0
_0806ECA0: .4byte 0xFFFFE9F8
_0806ECA4: .4byte 0xFFFFE9FE
_0806ECA8: .4byte 0x04000050
_0806ECAC: .4byte 0x00003FC8
_0806ECB0:
	mov r0, #1
	b _0806ED34
_0806ECB4:
	ldr r0, _0806ECDC @ =0xFFFFEA08
	add r0, r8
	ldrb r0, [r0]
	cmp r0, #0
	bne _0806ED32
	cmp r1, #0
	bne _0806ED32
	ldr r4, _0806ECE0 @ =0xFFFFFC8C
	add r4, r8
	ldrh r5, [r4]
	lsl r2, r5, #0x10
	cmp r2, #0
	blt _0806ECEC
	ldr r1, _0806ECE4 @ =0x04000050
	ldr r6, _0806ECE8 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #4
	asr r0, r2, #0x18
	b _0806ECFE
_0806ECDC: .4byte 0xFFFFEA08
_0806ECE0: .4byte 0xFFFFFC8C
_0806ECE4: .4byte 0x04000050
_0806ECE8: .4byte 0x00003FC8
_0806ECEC:
	ldr r1, _0806ED18 @ =0x04000050
	ldr r2, _0806ED1C @ =0x00003F88
	add r0, r2, #0
	strh r0, [r1]
	add r1, #4
	mov r3, #0
	ldsh r0, [r4, r3]
	neg r0, r0
	asr r0, r0, #8
_0806ECFE:
	strh r0, [r1]
	ldr r0, _0806ED20 @ =0x0201DB20
	ldr r4, _0806ED24 @ =0x000018AC
	add r2, r0, r4
	ldrh r3, [r2]
	mov r5, #0
	ldsh r1, [r2, r5]
	ldr r0, _0806ED28 @ =0x000003FF
	cmp r1, r0
	ble _0806ED2C
	add r0, #1
	b _0806ED30
	.align 2, 0
_0806ED18: .4byte 0x04000050
_0806ED1C: .4byte 0x00003F88
_0806ED20: .4byte 0x0201DB20
_0806ED24: .4byte 0x000018AC
_0806ED28: .4byte 0x000003FF
_0806ED2C:
	add r0, r3, #0
	add r0, #0x30
_0806ED30:
	strh r0, [r2]
_0806ED32:
	mov r0, #0
_0806ED34:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DeckEdit_Update

