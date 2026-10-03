	thumb_func_start ProhibitCardSelect_Update
ProhibitCardSelect_Update: @ 0x0806F934
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	ldr r0, _0806FB68 @ =0x03000040
	ldr r1, _0806FB6C @ =0x000003FF
	ldrh r0, [r0, #6]
	and r0, r1
	str r0, [sp, #0x28]
	ldr r4, _0806FB70 @ =0x0201E148
	add r0, r4, #0
	bl Ease_Tick
	ldr r2, _0806FB74 @ =0x000010E8
	add r1, r4, r2
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0806F962
	b _0806FBC0
_0806F962:
	mov r3, #2
	ldsh r0, [r4, r3]
	cmp r0, #4
	bne _0806F970
	ldrb r5, [r4, #0xD]
	cmp r5, #3
	beq _0806F97E
_0806F970:
	cmp r0, #3
	beq _0806F976
	b _0806FBC0
_0806F976:
	ldrb r4, [r4, #0xD]
	cmp r4, #4
	beq _0806F97E
	b _0806FBC0
_0806F97E:
	ldr r6, _0806FB78 @ =0x0201DB20
	ldr r0, _0806FB7C @ =0x00001710
	add r1, r6, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r7, _0806FB80 @ =0x0600D000
	ldr r2, _0806FB84 @ =0x01000200
	add r0, sp, #0x20
	add r1, r7, #0
	bl CpuFastSet
	ldr r4, _0806FB88 @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _0806FB8C @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #2
	cmp r0, #0
	blt _0806F9E2
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r6, r4
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #2
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r5, _0806FB90 @ =0x0000063A
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
_0806F9E2:
	ldr r4, _0806FB88 @ =0x0201F73C
	ldrb r3, [r4]
	lsl r0, r3, #1
	ldr r5, _0806FB8C @ =0x0201E140
	add r2, r0, r5
	mov r1, #0
	ldsh r0, [r2, r1]
	sub r0, #1
	mov r4, #0xC8
	lsl r4, r4, #3
	add r4, r4, r6
	mov sl, r4
	cmp r0, #0
	blt _0806FA2E
	mov r5, #0xA5
	lsl r5, r5, #5
	add r0, r6, r5
	add r0, r3, r0
	ldrb r1, [r0]
	ldrh r2, [r2]
	sub r2, #1
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r2, _0806FB90 @ =0x0000063A
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
_0806FA2E:
	ldr r3, _0806FB88 @ =0x0201F73C
	ldrb r4, [r3]
	lsl r1, r4, #1
	ldr r0, _0806FB8C @ =0x0201E140
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
	ldr r0, _0806FB94 @ =0x00001494
	add r0, r0, r6
	mov r8, r0
	add r1, r8
	ldrh r1, [r1]
	cmp r2, r1
	bge _0806FA8A
	ldrh r2, [r5]
	add r2, #1
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r2, _0806FB90 @ =0x0000063A
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
_0806FA8A:
	ldr r5, _0806FB88 @ =0x0201F73C
	ldrb r4, [r5]
	lsl r1, r4, #1
	ldr r0, _0806FB8C @ =0x0201E140
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
	bge _0806FADA
	ldrh r2, [r5]
	add r2, #2
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r4, _0806FB90 @ =0x0000063A
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
_0806FADA:
	ldr r7, _0806FB98 @ =0x0600C000
	ldr r0, _0806FB9C @ =0x0000063E
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
	ldr r2, _0806FB88 @ =0x0201F73C
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
	beq _0806FBA0
	ldr r6, _0806FB8C @ =0x0201E140
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
	b _0806FBC0
_0806FB68: .4byte 0x03000040
_0806FB6C: .4byte 0x000003FF
_0806FB70: .4byte 0x0201E148
_0806FB74: .4byte 0x000010E8
_0806FB78: .4byte 0x0201DB20
_0806FB7C: .4byte 0x00001710
_0806FB80: .4byte 0x0600D000
_0806FB84: .4byte 0x01000200
_0806FB88: .4byte 0x0201F73C
_0806FB8C: .4byte 0x0201E140
_0806FB90: .4byte 0x0000063A
_0806FB94: .4byte 0x00001494
_0806FB98: .4byte 0x0600C000
_0806FB9C: .4byte 0x0000063E
_0806FBA0:
	ldr r6, _0806FC0C @ =0x0201E140
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
_0806FBC0:
	ldr r5, _0806FC10 @ =0x0201DB20
	ldr r2, _0806FC14 @ =0x0000062A
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
	ldr r4, _0806FC18 @ =0x000018B0
	add r3, r5, r4
	ldr r4, _0806FC1C @ =0x00001BB8
	add r4, r4, r5
	str r4, [sp, #0]
	bl DeckEdit_TweenFrameSlots
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #1
	bge _0806FBF2
	b _0806FF96
_0806FBF2:
	cmp r0, #2
	ble _0806FC20
	cmp r0, #4
	ble _0806FBFC
	b _0806FF96
_0806FBFC:
	ldrb r0, [r6]
	cmp r0, #1
	bne _0806FC04
	b _0806FDE4
_0806FC04:
	cmp r0, #2
	bne _0806FC0A
	b _0806FEE0
_0806FC0A:
	b _0806FF42
_0806FC0C: .4byte 0x0201E140
_0806FC10: .4byte 0x0201DB20
_0806FC14: .4byte 0x0000062A
_0806FC18: .4byte 0x000018B0
_0806FC1C: .4byte 0x00001BB8
_0806FC20:
	ldrb r0, [r6]
	cmp r0, #1
	beq _0806FC2C
	cmp r0, #2
	beq _0806FCDC
	b _0806FD76
_0806FC2C:
	ldr r1, _0806FCB4 @ =0x0400001C
	mov r2, #0xC6
	lsl r2, r2, #3
	add r0, r5, r2
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806FCB8 @ =0x080875D2
	mov r3, #0
	ldsh r1, [r7, r3]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _0806FCBC @ =0x0400001E
	ldr r6, _0806FCC0 @ =0x00000632
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806FCC4 @ =0x04000014
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
	ldr r2, _0806FCC8 @ =0x04000016
	add r6, #8
	add r1, r5, r6
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806FCCC @ =0x04000010
	ldr r2, _0806FCD0 @ =0x0000063C
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
	ldr r2, _0806FCD4 @ =0x04000012
	ldr r4, _0806FCD8 @ =0x0000063E
	add r1, r5, r4
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	b _0806FF96
	.align 2, 0
_0806FCB4: .4byte 0x0400001C
_0806FCB8: .4byte gDeckEditEaseCurve
_0806FCBC: .4byte 0x0400001E
_0806FCC0: .4byte 0x00000632
_0806FCC4: .4byte 0x04000014
_0806FCC8: .4byte 0x04000016
_0806FCCC: .4byte 0x04000010
_0806FCD0: .4byte 0x0000063C
_0806FCD4: .4byte 0x04000012
_0806FCD8: .4byte 0x0000063E
_0806FCDC:
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806FDBC @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _0806FDC0 @ =0x00000632
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
	ldr r2, _0806FDC4 @ =0x0000063A
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
	ldr r2, _0806FDC8 @ =0x0000063E
	add r1, r5, r2
	asr r0, r0, #8
	ldrh r3, [r1]
	add r0, r3, r0
	strh r0, [r1]
	ldr r4, _0806FDCC @ =0x00001BB5
	add r2, r5, r4
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806FD56
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _0806FDD0 @ =0x00001BB4
	add r1, r5, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0806FD56:
	ldr r3, _0806FDD4 @ =0x00001BB7
	add r2, r5, r3
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806FD70
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r4, _0806FDD8 @ =0x00001BB6
	add r1, r5, r4
	ldrb r5, [r1]
	orr r0, r5
	strb r0, [r1]
_0806FD70:
	mov r0, #0
	mov r6, r8
	strb r0, [r6]
_0806FD76:
	ldr r2, _0806FDDC @ =0x0400001C
	ldr r1, _0806FDE0 @ =0x0201DB20
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r4, _0806FDC0 @ =0x00000632
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
	ldr r6, _0806FDC4 @ =0x0000063A
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
	b _0806FF96
	.align 2, 0
_0806FDBC: .4byte gDeckEditEaseCurve
_0806FDC0: .4byte 0x00000632
_0806FDC4: .4byte 0x0000063A
_0806FDC8: .4byte 0x0000063E
_0806FDCC: .4byte 0x00001BB5
_0806FDD0: .4byte 0x00001BB4
_0806FDD4: .4byte 0x00001BB7
_0806FDD8: .4byte 0x00001BB6
_0806FDDC: .4byte 0x0400001C
_0806FDE0: .4byte 0x0201DB20
_0806FDE4:
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r4, _0806FE70 @ =0x080875D2
	mov r6, #0
	ldsh r1, [r7, r6]
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	bl MulFix8
	ldr r2, _0806FE74 @ =0x0400001C
	mov r3, #0xC6
	lsl r3, r3, #3
	add r1, r5, r3
	asr r0, r0, #8
	ldrh r1, [r1]
	add r0, r1, r0
	strh r0, [r2]
	ldr r1, _0806FE78 @ =0x0400001E
	ldr r6, _0806FE7C @ =0x00000632
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
	ldr r1, _0806FE80 @ =0x04000050
	ldr r6, _0806FE84 @ =0x00003F43
	add r0, r6, #0
	strh r0, [r1]
	mov r1, #0
	ldsh r0, [r7, r1]
	cmp r0, #3
	bgt _0806FE9C
	ldr r0, _0806FE88 @ =0x04000014
	strh r2, [r0]
	ldr r1, _0806FE8C @ =0x04000016
	ldr r4, _0806FE90 @ =0x0000063A
	add r0, r5, r4
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _0806FE94 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0806FE98 @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	lsr r0, r3, #0x19
	bl SetBldAlpha
	b _0806FF96
	.align 2, 0
_0806FE70: .4byte gDeckEditEaseCurve
_0806FE74: .4byte 0x0400001C
_0806FE78: .4byte 0x0400001E
_0806FE7C: .4byte 0x00000632
_0806FE80: .4byte 0x04000050
_0806FE84: .4byte 0x00003F43
_0806FE88: .4byte 0x04000014
_0806FE8C: .4byte 0x04000016
_0806FE90: .4byte 0x0000063A
_0806FE94: .4byte 0x04000010
_0806FE98: .4byte 0x0000063E
_0806FE9C:
	ldr r0, _0806FEC8 @ =0x04000014
	ldr r1, _0806FECC @ =0x0000FFC0
	add r2, r2, r1
	strh r2, [r0]
	ldr r1, _0806FED0 @ =0x04000016
	ldr r3, _0806FED4 @ =0x0000063A
	add r0, r5, r3
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, _0806FED8 @ =0x04000010
	strh r2, [r0]
	sub r1, #4
	ldr r6, _0806FEDC @ =0x0000063E
	add r0, r5, r6
	ldrh r0, [r0]
	strh r0, [r1]
	mov r0, #0x20
	sub r0, r0, r4
	asr r0, r0, #1
	bl SetBldAlpha
	b _0806FF96
_0806FEC8: .4byte 0x04000014
_0806FECC: .4byte 0x0000FFC0
_0806FED0: .4byte 0x04000016
_0806FED4: .4byte 0x0000063A
_0806FED8: .4byte 0x04000010
_0806FEDC: .4byte 0x0000063E
_0806FEE0:
	add r4, r5, #0
	mov r0, #0
	strb r0, [r6]
	mov r0, #0xA0
	lsl r0, r0, #7
	ldr r2, _0806FFC0 @ =0x080875D2
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
	ldr r0, _0806FFC4 @ =0x00001BB5
	add r2, r4, r0
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806FF22
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r2, _0806FFC8 @ =0x00001BB4
	add r1, r4, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_0806FF22:
	ldr r5, _0806FFCC @ =0x00001BB7
	add r2, r4, r5
	ldrb r0, [r2]
	cmp r0, #0
	beq _0806FF3C
	mov r0, #1
	mov r1, #1
	strb r1, [r2]
	ldr r6, _0806FFD0 @ =0x00001BB6
	add r1, r4, r6
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0806FF3C:
	mov r0, #0
	mov r3, r8
	strb r0, [r3]
_0806FF42:
	ldr r2, _0806FFD4 @ =0x0400001C
	ldr r1, _0806FFD8 @ =0x0201DB20
	mov r4, #0xC6
	lsl r4, r4, #3
	add r0, r1, r4
	ldrh r0, [r0]
	strh r0, [r2]
	add r2, #2
	ldr r5, _0806FFDC @ =0x00000632
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
	ldr r3, _0806FFE0 @ =0x0000063A
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
	ldr r1, _0806FFE4 @ =0x04000050
	ldr r6, _0806FFE8 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #2
	mov r2, #0x80
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
_0806FF96:
	ldr r5, _0806FFD8 @ =0x0201DB20
	ldr r3, _0806FFEC @ =0x0000061E
	add r0, r5, r3
	ldrb r0, [r0]
	add r3, r5, #0
	cmp r0, #0
	beq _0806FFA6
	b _0807073E
_0806FFA6:
	ldr r4, _0806FFF0 @ =0x00001C48
	add r4, r4, r5
	mov r8, r4
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, #0
	beq _0806FFF4
	cmp r0, #1
	bne _0806FFBC
	b _08070478
_0806FFBC:
	b _0807073E
	.align 2, 0
_0806FFC0: .4byte gDeckEditEaseCurve
_0806FFC4: .4byte 0x00001BB5
_0806FFC8: .4byte 0x00001BB4
_0806FFCC: .4byte 0x00001BB7
_0806FFD0: .4byte 0x00001BB6
_0806FFD4: .4byte 0x0400001C
_0806FFD8: .4byte 0x0201DB20
_0806FFDC: .4byte 0x00000632
_0806FFE0: .4byte 0x0000063A
_0806FFE4: .4byte 0x04000050
_0806FFE8: .4byte 0x00003FC8
_0806FFEC: .4byte 0x0000061E
_0806FFF0: .4byte 0x00001C48
_0806FFF4:
	mov r6, #0xC5
	lsl r6, r6, #3
	add r0, r5, r6
	ldrb r0, [r0]
	cmp r0, #1
	bne _08070002
	b _08070436
_08070002:
	ldr r0, [sp, #0x28]
	cmp r0, #0x40
	beq _08070022
	cmp r0, #0x40
	bhi _08070012
	cmp r0, #2
	beq _0807001A
	b _08070032
_08070012:
	ldr r1, [sp, #0x28]
	cmp r1, #0x80
	beq _0807002A
	b _08070032
_0807001A:
	mov r0, #3
	bl PlaySE
	b _08070436
_08070022:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListUp
	b _08070436
_0807002A:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListDown
	b _08070436
_08070032:
	ldr r2, _08070068 @ =0x00001C1C
	add r0, r3, r2
	ldrb r1, [r0]
	lsl r2, r1, #1
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r3, r4
	add r1, r1, r0
	ldrb r5, [r1]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r6, _0807006C @ =0x00001494
	add r1, r3, r6
	add r4, r0, r1
	ldrh r5, [r4]
	cmp r5, #5
	bhi _0807005A
	b _08070436
_0807005A:
	ldr r0, [sp, #0x28]
	cmp r0, #0x10
	beq _08070070
	cmp r0, #0x20
	bne _08070066
	b _0807025C
_08070066:
	b _08070436
_08070068: .4byte 0x00001C1C
_0807006C: .4byte 0x00001494
_08070070:
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r3, r1
	add r1, r2, r0
	ldrh r2, [r1]
	add r2, #5
	ldrh r0, [r4]
	sub r0, #1
	cmp r2, r0
	ble _0807008A
	mov r0, #0
	strh r0, [r1]
	b _0807008C
_0807008A:
	strh r2, [r1]
_0807008C:
	ldr r6, _080701D0 @ =0x0201DB20
	ldr r2, _080701D4 @ =0x000018AC
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
	ldr r5, _080701D8 @ =0x00000634
	add r4, r6, r5
	mov r0, #1
	mov r9, r0
	mov r0, r9
	ldrb r1, [r4]
	eor r0, r1
	strb r0, [r4]
	ldr r2, _080701DC @ =0x00001C1C
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
	ldr r3, _080701E0 @ =0x06008000
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
	ldr r3, _080701E4 @ =0x00000632
	add r2, r6, r3
	ldrh r2, [r2]
	and r1, r2
	lsr r1, r1, #3
	add r1, #2
	ldrb r2, [r4]
	mov r3, #1
	bl DeckEdit_PlaceCardArt
	ldr r4, _080701E8 @ =0x00000635
	add r1, r6, r4
	mov r0, #3
	strb r0, [r1]
	ldr r0, _080701EC @ =0x00001710
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
	ldr r5, _080701F0 @ =0x00001494
	add r1, r6, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r2, r7
	ldrh r1, [r2]
	ldr r3, _080701F4 @ =0x00001BB0
	add r2, r6, r3
	bl DeckEdit_CalcScrollBar
	ldr r4, _080701F8 @ =0x00001BB7
	add r1, r6, r4
	ldrb r0, [r1]
	cmp r0, #0
	beq _08070168
	mov r0, #2
	strb r0, [r1]
	ldr r5, _080701FC @ =0x00001BB6
	add r1, r6, r5
	mov r0, r9
	ldrb r6, [r1]
	orr r0, r6
	strb r0, [r1]
_08070168:
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
_0807017C:
	ldrh r3, [r7]
	lsl r2, r3, #0x10
	cmp r2, #0
	blt _08070208
	ldr r6, _080701D0 @ =0x0201DB20
	ldr r4, _080701DC @ =0x00001C1C
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
	bcs _08070208
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _08070200 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _08070204 @ =0x00001BB8
	add r3, r6, r4
	bl DeckEdit_InitFrameSlot
	b _08070216
	.align 2, 0
_080701D0: .4byte 0x0201DB20
_080701D4: .4byte 0x000018AC
_080701D8: .4byte 0x00000634
_080701DC: .4byte 0x00001C1C
_080701E0: .4byte 0x06008000
_080701E4: .4byte 0x00000632
_080701E8: .4byte 0x00000635
_080701EC: .4byte 0x00001710
_080701F0: .4byte 0x00001494
_080701F4: .4byte 0x00001BB0
_080701F8: .4byte 0x00001BB7
_080701FC: .4byte 0x00001BB6
_08070200: .4byte 0x000018B0
_08070204: .4byte 0x00001BB8
_08070208:
	ldr r0, _08070248 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _0807024C @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_08070216:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _0807017C
	ldr r1, _08070248 @ =0x0201DB20
	ldr r0, _08070250 @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _08070254 @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _08070258 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl PlaySE
	b _08070436
_08070248: .4byte 0x0201DB20
_0807024C: .4byte 0x00001BC4
_08070250: .4byte 0x00001C14
_08070254: .4byte 0x00001BB8
_08070258: .4byte 0x00001C58
_0807025C:
	mov r6, #0xC4
	lsl r6, r6, #3
	add r0, r3, r6
	add r1, r2, r0
	ldrh r0, [r1]
	cmp r0, #4
	bhi _0807026E
	sub r0, r5, #1
	b _08070270
_0807026E:
	sub r0, #5
_08070270:
	strh r0, [r1]
	ldr r7, _080703C0 @ =0x0201DB20
	ldr r0, _080703C4 @ =0x000018AC
	add r1, r7, r0
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r2, #1
	neg r2, r2
	mov r1, #0xC5
	lsl r1, r1, #3
	add r3, r7, r1
	mov r0, #6
	mov r1, #0
	bl Ease_Start
	ldr r2, _080703C8 @ =0x00000634
	add r5, r7, r2
	mov r3, #1
	mov sl, r3
	mov r0, sl
	ldrb r4, [r5]
	eor r0, r4
	strb r0, [r5]
	ldr r6, _080703CC @ =0x00001C1C
	add r6, r6, r7
	mov r9, r6
	ldrb r0, [r6]
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
	ldr r4, _080703D0 @ =0x06008000
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
	ldr r3, _080703D4 @ =0x00000632
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
	ldr r4, _080703D8 @ =0x00000635
	add r1, r7, r4
	mov r0, #4
	strb r0, [r1]
	ldr r5, _080703DC @ =0x00001710
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
	ldr r5, _080703E0 @ =0x00001494
	add r1, r7, r5
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r8
	ldrh r1, [r2]
	ldr r6, _080703E4 @ =0x00001BB0
	add r2, r7, r6
	bl DeckEdit_CalcScrollBar
	ldr r0, _080703E8 @ =0x00001BB5
	add r1, r7, r0
	ldrb r0, [r1]
	cmp r0, #0
	beq _08070358
	mov r0, #2
	strb r0, [r1]
	ldr r2, _080703EC @ =0x00001BB4
	add r1, r7, r2
	mov r0, sl
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_08070358:
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
_0807036C:
	ldrh r6, [r7]
	lsl r2, r6, #0x10
	cmp r2, #0
	blt _080703F8
	ldr r6, _080703C0 @ =0x0201DB20
	ldr r1, _080703CC @ =0x00001C1C
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
	ldr r1, _080703E0 @ =0x00001494
	add r1, r1, r6
	mov r8, r1
	add r0, r8
	lsr r1, r2, #0x10
	ldrh r0, [r0]
	cmp r1, r0
	bcs _080703F8
	ldrh r2, [r7]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r2, r5, #1
	ldr r3, _080703F0 @ =0x000018B0
	add r0, r6, r3
	str r0, [sp, #0]
	add r0, r5, #0
	ldr r4, _080703F4 @ =0x00001BB8
	add r3, r6, r4
	bl DeckEdit_InitFrameSlot
	b _08070406
	.align 2, 0
_080703C0: .4byte 0x0201DB20
_080703C4: .4byte 0x000018AC
_080703C8: .4byte 0x00000634
_080703CC: .4byte 0x00001C1C
_080703D0: .4byte 0x06008000
_080703D4: .4byte 0x00000632
_080703D8: .4byte 0x00000635
_080703DC: .4byte 0x00001710
_080703E0: .4byte 0x00001494
_080703E4: .4byte 0x00001BB0
_080703E8: .4byte 0x00001BB5
_080703EC: .4byte 0x00001BB4
_080703F0: .4byte 0x000018B0
_080703F4: .4byte 0x00001BB8
_080703F8:
	ldr r0, _08070454 @ =0x0201DB20
	lsl r1, r5, #4
	add r1, r1, r0
	ldr r6, _08070458 @ =0x00001BC4
	add r1, r1, r6
	mov r0, #0
	strb r0, [r1]
_08070406:
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _0807036C
	ldr r1, _08070454 @ =0x0201DB20
	ldr r0, _0807045C @ =0x00001C14
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r3, _08070460 @ =0x00001BB8
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	ldr r4, _08070464 @ =0x00001C58
	add r1, r1, r4
	mov r0, #0x1E
	strh r0, [r1]
	mov r0, #0
	bl PlaySE
_08070436:
	ldr r5, [sp, #0x28]
	cmp r5, #1
	bne _0807046C
	ldr r0, _08070454 @ =0x0201DB20
	ldr r6, _08070468 @ =0x00001C3D
	add r0, r0, r6
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #1
	orr r1, r2
	strb r1, [r0]
	b _080706D0
	.align 2, 0
_08070454: .4byte 0x0201DB20
_08070458: .4byte 0x00001BC4
_0807045C: .4byte 0x00001C14
_08070460: .4byte 0x00001BB8
_08070464: .4byte 0x00001C58
_08070468: .4byte 0x00001C3D
_0807046C:
	ldr r0, _08070474 @ =0x0201F73C
	bl CardSelect_HandleListSwitch
	b _0807073E
_08070474: .4byte 0x0201F73C
_08070478:
	mov r4, #0xE1
	lsl r4, r4, #5
	add r0, r5, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _08070486
	b _0807073E
_08070486:
	ldr r6, _080704AC @ =0x00001866
	add r0, r5, r6
	mov r1, #0
	ldsb r1, [r0, r1]
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	beq _08070498
	b _0807073E
_08070498:
	ldr r0, [sp, #0x28]
	cmp r0, #2
	bne _080704A0
	b _080706E8
_080704A0:
	cmp r0, #2
	bhi _080704B0
	cmp r0, #1
	bne _080704AA
	b _080705D0
_080704AA:
	b _08070708
_080704AC: .4byte 0x00001866
_080704B0:
	ldr r1, [sp, #0x28]
	cmp r1, #0x10
	beq _080704BC
	cmp r1, #0x20
	beq _08070530
	b _08070708
_080704BC:
	ldr r2, _080704EC @ =0x00001C3C
	add r3, r5, r2
	ldr r2, [r3]
	lsl r1, r2, #0xE
	lsr r1, r1, #0x1D
	add r1, #1
	mov r0, #7
	and r0, r1
	lsl r0, r0, #0xF
	ldr r5, _080704F0 @ =0xFFFC7FFF
	add r4, r5, #0
	and r4, r2
	orr r4, r0
	str r4, [r3]
	mov r0, #7
	and r0, r1
	cmp r0, #5
	beq _0807050A
	cmp r0, #5
	bgt _080704F4
	cmp r0, #1
	beq _080704FA
	b _08070516
	.align 2, 0
_080704EC: .4byte 0x00001C3C
_080704F0: .4byte 0xFFFC7FFF
_080704F4:
	cmp r0, #7
	beq _08070504
	b _08070516
_080704FA:
	add r0, r5, #0
	and r0, r4
	mov r1, #0x80
	lsl r1, r1, #0xA
	b _08070512
_08070504:
	and r4, r5
	str r4, [r3]
	b _08070516
_0807050A:
	add r0, r5, #0
	and r0, r4
	mov r1, #0xC0
	lsl r1, r1, #0xA
_08070512:
	orr r0, r1
	str r0, [r3]
_08070516:
	ldr r2, _08070528 @ =0x0201DB20
	ldr r4, _0807052C @ =0x00001C3D
	add r2, r2, r4
	mov r0, #8
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	b _080705B0
	.align 2, 0
_08070528: .4byte 0x0201DB20
_0807052C: .4byte 0x00001C3D
_08070530:
	ldr r6, _08070548 @ =0x00001C3C
	add r2, r5, r6
	ldr r1, [r2]
	lsl r0, r1, #0xE
	lsr r0, r0, #0x1D
	cmp r0, #4
	beq _08070552
	cmp r0, #4
	bgt _0807054C
	cmp r0, #0
	beq _08070560
	b _08070584
_08070548: .4byte 0x00001C3C
_0807054C:
	cmp r0, #6
	beq _08070570
	b _08070584
_08070552:
	ldr r0, _0807055C @ =0xFFFC7FFF
	and r1, r0
	str r1, [r2]
	b _080705A2
	.align 2, 0
_0807055C: .4byte 0xFFFC7FFF
_08070560:
	ldr r0, _0807056C @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #0xA
	b _08070578
	.align 2, 0
_0807056C: .4byte 0xFFFC7FFF
_08070570:
	ldr r0, _08070580 @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #0xA
_08070578:
	orr r0, r1
	str r0, [r2]
	b _080705A2
	.align 2, 0
_08070580: .4byte 0xFFFC7FFF
_08070584:
	ldr r0, _080705C0 @ =0x00001C3C
	add r3, r3, r0
	ldr r2, [r3]
	lsl r0, r2, #0xE
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #7
	and r0, r1
	lsl r0, r0, #0xF
	ldr r1, _080705C4 @ =0xFFFC7FFF
	and r1, r2
	orr r1, r0
	str r1, [r3]
_080705A2:
	ldr r2, _080705C8 @ =0x0201DB20
	ldr r3, _080705CC @ =0x00001C3D
	add r2, r2, r3
	mov r0, #8
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
_080705B0:
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0
	bl PlaySE
	b _0807073E
	.align 2, 0
_080705C0: .4byte 0x00001C3C
_080705C4: .4byte 0xFFFC7FFF
_080705C8: .4byte 0x0201DB20
_080705CC: .4byte 0x00001C3D
_080705D0:
	ldr r6, _080705E8 @ =0x00001C3C
	add r0, r5, r6
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	cmp r0, #4
	beq _08070618
	cmp r0, #4
	bgt _080705EC
	cmp r0, #0
	beq _080705F6
	b _0807073E
_080705E8: .4byte 0x00001C3C
_080705EC:
	cmp r0, #5
	beq _0807063A
	cmp r0, #6
	beq _0807065C
	b _0807073E
_080705F6:
	mov r0, #0x1F
	neg r0, r0
	and r0, r2
	mov r1, #4
	orr r0, r1
	mov r1, r8
	strb r0, [r1]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r2, #0xC3
	lsl r2, r2, #3
	add r3, r5, r2
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _080706D0
_08070618:
	mov r0, #0x1F
	neg r0, r0
	and r0, r2
	mov r1, #2
	orr r0, r1
	mov r3, r8
	strb r0, [r3]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r4, #0xC3
	lsl r4, r4, #3
	add r3, r5, r4
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _080706D0
_0807063A:
	mov r0, #0x1F
	neg r0, r0
	and r0, r2
	mov r1, #6
	orr r0, r1
	mov r6, r8
	strb r0, [r6]
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r0, #0xC3
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _080706D0
_0807065C:
	ldr r1, _080706D8 @ =0x00001C1C
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
	add r7, r5, r3
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	cmp r0, #0
	beq _0807073E
	ldrb r4, [r4]
	lsl r2, r4, #1
	add r0, r4, r6
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r6, _080706DC @ =0x00001494
	add r1, r5, r6
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _0807073E
	add r0, r2, r7
	ldrh r2, [r0]
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	ldr r1, _080706E0 @ =0x03000040
	ldr r2, _080706E4 @ =0x00004872
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
	mov r5, r8
	ldrb r5, [r5]
	and r0, r5
	mov r6, r8
	strb r0, [r6]
_080706D0:
	mov r0, #1
	bl PlaySE
	b _0807073E
_080706D8: .4byte 0x00001C1C
_080706DC: .4byte 0x00001494
_080706E0: .4byte 0x03000040
_080706E4: .4byte 0x00004872
_080706E8:
	ldr r1, _08070704 @ =0x00001C3D
	add r0, r5, r1
	mov r1, #8
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	mov r0, #2
	bl PlaySE
	b _0807073E
	.align 2, 0
_08070704: .4byte 0x00001C3D
_08070708:
	ldr r4, _08070728 @ =0x0201F73C
	add r0, r4, #0
	bl CardSelect_HandleListSwitch
	ldr r3, _0807072C @ =0xFFFFEA0C
	add r4, r4, r3
	ldrb r4, [r4]
	cmp r4, #1
	beq _0807073E
	ldr r4, [sp, #0x28]
	cmp r4, #0x40
	beq _08070730
	cmp r4, #0x80
	beq _08070738
	b _0807073E
	.align 2, 0
_08070728: .4byte 0x0201F73C
_0807072C: .4byte 0xFFFFEA0C
_08070730:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListUp
	b _0807073E
_08070738:
	add r0, sp, #0x24
	bl DeckEdit_ScrollListDown
_0807073E:
	ldr r0, _08070868 @ =0x081A6524
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
	ldr r4, _0807086C @ =0x0201DB20
	str r4, [sp, #0x1C]
	mov r1, #5
	mov r2, #0xB
	bl OamListAddSpriteGroup
	ldr r5, _08070870 @ =0x000017DA
	add r1, r4, r5
	mov r0, #1
	strb r0, [r1]
	ldr r6, _08070874 @ =0x00001C1C
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
	ldr r2, _08070878 @ =0x00001494
	add r1, r4, r2
	add r3, r3, r1
	ldrh r1, [r3]
	ldr r3, _0807087C @ =0x00001BB0
	add r2, r4, r3
	bl DeckEdit_DrawScrollBar
	ldr r5, _08070880 @ =0x00001BBC
	add r0, r4, r5
	add r1, r4, #0
	bl DeckEdit_DrawFrameSlots
	mov r5, #1
	ldr r6, _08070884 @ =0x000018B0
	add r4, r4, r6
_080707B0:
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	add r0, r0, r4
	bl ObjAffineApply
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #6
	bls _080707B0
	ldr r7, _08070888 @ =0x0201F740
	add r0, r7, #0
	bl DeckEdit_UpdateCardMove
	add r4, r7, #0
	add r4, #0x1C
	add r0, r4, #0
	bl DeckEdit_UpdateCommandMenuAnim
	add r0, r4, #0
	bl DeckEdit_DrawCommandMenu
	ldr r0, _0807088C @ =0x081A6EA4
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	mov r6, #0
	str r6, [sp, #4]
	str r6, [sp, #8]
	mov r1, #1
	str r1, [sp, #0xC]
	str r1, [sp, #0x10]
	str r6, [sp, #0x14]
	str r6, [sp, #0x18]
	ldr r1, _08070890 @ =0xFFFFE3E0
	add r4, r7, r1
	str r4, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	bl OamListAddSpriteGroup
	ldr r2, _08070894 @ =0xFFFFFAF8
	add r5, r7, r2
	add r0, r5, #0
	bl AnimBlockTick
	str r6, [sp, #0]
	str r6, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	str r6, [sp, #0xC]
	str r6, [sp, #0x10]
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
	ldr r3, _08070898 @ =0xFFFFE9F8
	add r0, r7, r3
	bl FadeTick
	ldr r4, _0807089C @ =0xFFFFE9FE
	add r0, r7, r4
	ldrb r1, [r0]
	cmp r1, #2
	beq _080708A8
	cmp r1, #3
	bne _080708AC
	strb r6, [r0]
	ldr r1, _080708A0 @ =0x04000050
	ldr r5, _080708A4 @ =0x00003FC8
	add r0, r5, #0
	strh r0, [r1]
	add r1, #2
	mov r6, #0x80
	lsl r6, r6, #5
	add r0, r6, #0
	strh r0, [r1]
	b _0807092A
_08070868: .4byte gUnk_081A6524
_0807086C: .4byte 0x0201DB20
_08070870: .4byte 0x000017DA
_08070874: .4byte 0x00001C1C
_08070878: .4byte 0x00001494
_0807087C: .4byte 0x00001BB0
_08070880: .4byte 0x00001BBC
_08070884: .4byte 0x000018B0
_08070888: .4byte 0x0201F740
_0807088C: .4byte gUnk_081A6EA4
_08070890: .4byte 0xFFFFE3E0
_08070894: .4byte 0xFFFFFAF8
_08070898: .4byte 0xFFFFE9F8
_0807089C: .4byte 0xFFFFE9FE
_080708A0: .4byte 0x04000050
_080708A4: .4byte 0x00003FC8
_080708A8:
	mov r0, #1
	b _0807092C
_080708AC:
	ldr r2, _080708D4 @ =0xFFFFEA08
	add r0, r7, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _0807092A
	cmp r1, #0
	bne _0807092A
	ldr r3, _080708D8 @ =0xFFFFFC8C
	add r4, r7, r3
	ldrh r5, [r4]
	lsl r2, r5, #0x10
	cmp r2, #0
	blt _080708E4
	ldr r1, _080708DC @ =0x04000050
	ldr r6, _080708E0 @ =0x00003FC8
	add r0, r6, #0
	strh r0, [r1]
	add r1, #4
	asr r0, r2, #0x18
	b _080708F6
_080708D4: .4byte 0xFFFFEA08
_080708D8: .4byte 0xFFFFFC8C
_080708DC: .4byte 0x04000050
_080708E0: .4byte 0x00003FC8
_080708E4:
	ldr r1, _08070910 @ =0x04000050
	ldr r2, _08070914 @ =0x00003F88
	add r0, r2, #0
	strh r0, [r1]
	add r1, #4
	mov r3, #0
	ldsh r0, [r4, r3]
	neg r0, r0
	asr r0, r0, #8
_080708F6:
	strh r0, [r1]
	ldr r0, _08070918 @ =0x0201DB20
	ldr r4, _0807091C @ =0x000018AC
	add r2, r0, r4
	ldrh r3, [r2]
	mov r5, #0
	ldsh r1, [r2, r5]
	ldr r0, _08070920 @ =0x000003FF
	cmp r1, r0
	ble _08070924
	add r0, #1
	b _08070928
	.align 2, 0
_08070910: .4byte 0x04000050
_08070914: .4byte 0x00003F88
_08070918: .4byte 0x0201DB20
_0807091C: .4byte 0x000018AC
_08070920: .4byte 0x000003FF
_08070924:
	add r0, r3, #0
	add r0, #0x30
_08070928:
	strh r0, [r2]
_0807092A:
	mov r0, #0
_0807092C:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end ProhibitCardSelect_Update

