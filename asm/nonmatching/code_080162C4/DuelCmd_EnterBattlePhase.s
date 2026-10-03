	thumb_func_start DuelCmd_EnterBattlePhase
DuelCmd_EnterBattlePhase: @ 0x080164D0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, _080164F4 @ =0x020185C0
	ldr r2, _080164F8 @ =0x0000080A
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x19
	add r4, r0, #0
	cmp r1, #4
	bls _080164EA
	b _080167EA
_080164EA:
	lsl r0, r1, #2
	ldr r1, _080164FC @ =0x08016500
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080164F4: .4byte 0x020185C0
_080164F8: .4byte 0x0000080A
_080164FC: .4byte 0x08016500
_08016500:
	.4byte _08016514
	.4byte _08016560
	.4byte _0801662C
	.4byte _08016698
	.4byte _0801672C
_08016514:
	ldr r0, _08016544 @ =0x050003E0
	ldr r1, _08016548 @ =0x0867F01C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0801654C @ =0x06016C80
	ldr r1, _08016550 @ =0x0867F63C
	mov r4, #0x80
	lsl r4, r4, #2
	add r2, r4, #0
	bl CopyDoubleWords
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	ldr r2, _08016554 @ =0x020185C0
	ldr r3, _08016558 @ =0x0000080C
	add r1, r2, r3
	ldr r0, _0801655C @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
	orr r0, r4
	b _080166FC
_08016544: .4byte 0x050003E0
_08016548: .4byte gPhaseBannerPal
_0801654C: .4byte 0x06016C80
_08016550: .4byte gBattlePhaseBannerGfx
_08016554: .4byte 0x020185C0
_08016558: .4byte 0x0000080C
_0801655C: .4byte 0xFFFFF01F
_08016560:
	ldr r1, _080165F0 @ =0x0000080C
	add r3, r4, r1
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0
	ble _0801660C
	sub r0, #1
	mov r6, #0x7F
	and r0, r6
	lsl r0, r0, #5
	ldr r5, _080165F4 @ =0xFFFFF01F
	add r2, r5, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _080165F8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016598
	ldr r1, _080165FC @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080165AC
_08016598:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #4
	ble _080165AC
	sub r0, #3
	and r0, r6
	lsl r0, r0, #5
	and r2, r5
	orr r2, r0
	strh r2, [r3]
_080165AC:
	ldr r2, _08016600 @ =0x08081728
	mov r8, r2
	ldr r3, _080165F0 @ =0x0000080C
	add r4, r4, r3
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r0, r8
	ldr r0, [r0]
	mov r5, #0x80
	lsl r5, r5, #0xF
	orr r0, r5
	mov r6, #0x81
	lsl r6, r6, #7
	ldr r2, _08016604 @ =0x0000F364
	add r1, r6, #0
	bl AddSprite
	ldrh r4, [r4]
	lsl r0, r4, #0x14
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r0, r8
	ldr r1, [r0]
	mov r0, #0xD0
	sub r0, r0, r1
	orr r0, r5
	ldr r2, _08016608 @ =0x0000F36C
	add r1, r6, #0
	bl AddSprite
	b _08016828
	.align 2, 0
_080165F0: .4byte 0x0000080C
_080165F4: .4byte 0xFFFFF01F
_080165F8: .4byte 0x03000040
_080165FC: .4byte 0x0201CFB0
_08016600: .4byte gBattleBannerSlideX
_08016604: .4byte 0x0000F364
_08016608: .4byte 0x0000F36C
_0801660C:
	mov r0, #0x1D
	bl PlaySE
	ldr r2, _08016660 @ =0x0000080A
	add r3, r4, r2
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_0801662C:
	ldr r0, _08016664 @ =0x00400058
	mov r4, #0x81
	lsl r4, r4, #7
	ldr r2, _08016668 @ =0x0000F364
	add r1, r4, #0
	bl AddSprite
	ldr r0, _0801666C @ =0x00400078
	ldr r2, _08016670 @ =0x0000F36C
	add r1, r4, #0
	bl AddSprite
	ldr r1, _08016674 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801665C
	ldr r1, _08016678 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801667C
_0801665C:
	mov r0, #4
	b _0801667E
_08016660: .4byte 0x0000080A
_08016664: .4byte 0x00400058
_08016668: .4byte 0x0000F364
_0801666C: .4byte 0x00400078
_08016670: .4byte 0x0000F36C
_08016674: .4byte 0x03000040
_08016678: .4byte 0x0201CFB0
_0801667C:
	mov r0, #1
_0801667E:
	bl DuelFieldFadeToWhite
	cmp r0, #0
	bne _08016688
	b _08016828
_08016688:
	ldr r2, _08016690 @ =0x020185C0
	ldr r3, _08016694 @ =0x0000080A
	add r2, r2, r3
	b _08016702
_08016690: .4byte 0x020185C0
_08016694: .4byte 0x0000080A
_08016698:
	ldr r0, _080166CC @ =0x00400058
	mov r4, #0x81
	lsl r4, r4, #7
	ldr r2, _080166D0 @ =0x0000F364
	add r1, r4, #0
	bl AddSprite
	ldr r0, _080166D4 @ =0x00400078
	ldr r2, _080166D8 @ =0x0000F36C
	add r1, r4, #0
	bl AddSprite
	ldr r1, _080166DC @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _080166C8
	ldr r1, _080166E0 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080166E4
_080166C8:
	mov r0, #4
	b _080166E6
_080166CC: .4byte 0x00400058
_080166D0: .4byte 0x0000F364
_080166D4: .4byte 0x00400078
_080166D8: .4byte 0x0000F36C
_080166DC: .4byte 0x03000040
_080166E0: .4byte 0x0201CFB0
_080166E4:
	mov r0, #1
_080166E6:
	bl DuelFieldFadeFromWhite
	cmp r0, #0
	bne _080166F0
	b _08016828
_080166F0:
	ldr r2, _0801671C @ =0x020185C0
	ldr r0, _08016720 @ =0x0000080C
	add r1, r2, r0
	ldr r0, _08016724 @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
_080166FC:
	strh r0, [r1]
	ldr r0, _08016728 @ =0x0000080A
	add r2, r2, r0
_08016702:
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08016828
	.align 2, 0
_0801671C: .4byte 0x020185C0
_08016720: .4byte 0x0000080C
_08016724: .4byte 0xFFFFF01F
_08016728: .4byte 0x0000080A
_0801672C:
	ldr r1, _080167B4 @ =0x0000080C
	add r7, r4, r1
	ldrh r2, [r7]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xF
	bgt _080167D0
	ldr r4, _080167B8 @ =0x08081728
	lsl r0, r0, #2
	add r0, r0, r4
	ldr r0, [r0]
	mov r5, #0x80
	lsl r5, r5, #0xF
	orr r0, r5
	mov r6, #0x81
	lsl r6, r6, #7
	ldr r2, _080167BC @ =0x0000F364
	add r1, r6, #0
	bl AddSprite
	ldrh r3, [r7]
	lsl r0, r3, #0x14
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r0, r0, r4
	ldr r1, [r0]
	mov r0, #0xD0
	sub r0, r0, r1
	orr r0, r5
	ldr r2, _080167C0 @ =0x0000F36C
	add r1, r6, #0
	bl AddSprite
	ldrh r1, [r7]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r4, #0x7F
	and r0, r4
	lsl r0, r0, #5
	ldr r3, _080167C4 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r7]
	ldr r1, _080167C8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801679E
	ldr r1, _080167CC @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08016828
_0801679E:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xB
	bgt _08016828
	add r0, #3
	and r0, r4
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r7]
	b _08016828
_080167B4: .4byte 0x0000080C
_080167B8: .4byte gBattleBannerSlideX
_080167BC: .4byte 0x0000F364
_080167C0: .4byte 0x0000F36C
_080167C4: .4byte 0xFFFFF01F
_080167C8: .4byte 0x03000040
_080167CC: .4byte 0x0201CFB0
_080167D0:
	ldr r0, _08016834 @ =0x0000080A
	add r3, r4, r0
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_080167EA:
	ldr r4, _08016838 @ =0x020192E0
	ldr r1, _0801683C @ =0x00001B12
	add r4, r4, r1
	mov r1, #0x1D
	neg r1, r1
	ldrb r2, [r4]
	and r1, r2
	mov r0, #0xC
	orr r1, r0
	strb r1, [r4]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1B
	lsr r1, r1, #0x1D
	bl DrawPhaseIndicator
	ldrb r4, [r4]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #0
	mov r2, #0
	bl DuelCursor_Select
	ldr r1, _08016840 @ =0x020185C0
	ldr r3, _08016844 @ =0x0000080D
	add r1, r1, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08016828:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08016834: .4byte 0x0000080A
_08016838: .4byte 0x020192E0
_0801683C: .4byte 0x00001B12
_08016840: .4byte 0x020185C0
_08016844: .4byte 0x0000080D
	thumb_func_end DuelCmd_EnterBattlePhase

