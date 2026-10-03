	thumb_func_start DuelCmd_ShowJustAMomentBanner
DuelCmd_ShowJustAMomentBanner: @ 0x080134EC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r1, _08013514 @ =0x020185C0
	ldrh r0, [r1]
	lsr r0, r0, #0xF
	mov r8, r0
	ldr r2, _08013518 @ =0x0000080A
	add r6, r1, r2
	ldrb r3, [r6]
	lsl r0, r3, #0x19
	lsr r5, r0, #0x19
	add r7, r1, #0
	cmp r5, #1
	beq _080135A4
	cmp r5, #1
	bgt _0801351C
	cmp r5, #0
	beq _0801352A
	b _080137DC
_08013514: .4byte 0x020185C0
_08013518: .4byte 0x0000080A
_0801351C:
	cmp r5, #2
	bne _08013522
	b _08013664
_08013522:
	cmp r5, #3
	bne _08013528
	b _08013724
_08013528:
	b _080137DC
_0801352A:
	ldr r4, _08013580 @ =0x0201CFB0
	ldr r0, _08013584 @ =0x00000808
	add r1, r4, r0
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _08013588 @ =0x050003E0
	ldr r1, _0801358C @ =0x08687B9C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08013590 @ =0x06016C80
	ldr r1, _08013594 @ =0x08688BBC
	mov r2, #0x80
	lsl r2, r2, #3
	bl CopyDoubleWords
	ldr r0, _08013598 @ =0x0000085C
	add r4, r4, r0
	str r5, [r4]
	ldr r2, _0801359C @ =0x0000080C
	add r1, r7, r2
	ldr r0, _080135A0 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r6]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	mov r0, #0x16
	bl PlaySE
	b _080137EA
_08013580: .4byte 0x0201CFB0
_08013584: .4byte 0x00000808
_08013588: .4byte 0x050003E0
_0801358C: .4byte gDuelBannerPal
_08013590: .4byte 0x06016C80
_08013594: .4byte gJustAMomentBannerGfx
_08013598: .4byte 0x0000085C
_0801359C: .4byte 0x0000080C
_080135A0: .4byte 0xFFFFF01F
_080135A4:
	ldr r0, _080135C4 @ =0x0000080C
	add r1, r7, r0
	ldrh r4, [r1]
	lsl r0, r4, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0xF
	bgt _0801364C
	mov r1, r8
	cmp r1, #0
	beq _080135CC
	ldr r1, _080135C8 @ =0x081A44FC
	lsl r0, r2, #1
	add r0, r0, r1
	ldrh r0, [r0]
	b _080135D8
	.align 2, 0
_080135C4: .4byte 0x0000080C
_080135C8: .4byte gBannerSlideOffsets
_080135CC:
	ldr r0, _0801362C @ =0x081A44FC
	lsl r1, r2, #1
	add r1, r1, r0
	mov r0, #0x80
	ldrh r1, [r1]
	sub r0, r0, r1
_080135D8:
	lsl r0, r0, #0x10
	mov r1, #0x58
	orr r0, r1
	ldr r1, _08013630 @ =0x000040C0
	ldr r2, _08013634 @ =0x0000F364
	bl AddSprite
	ldr r0, _08013638 @ =0x020185C0
	ldr r2, _0801363C @ =0x0000080C
	add r3, r0, r2
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08013640 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _08013644 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801361E
	ldr r1, _08013648 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801361E
	b _080137EA
_0801361E:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #7
	ble _08013628
	b _080137EA
_08013628:
	add r0, #7
	b _080137B0
_0801362C: .4byte gBannerSlideOffsets
_08013630: .4byte 0x000040C0
_08013634: .4byte 0x0000F364
_08013638: .4byte 0x020185C0
_0801363C: .4byte 0x0000080C
_08013640: .4byte 0xFFFFF01F
_08013644: .4byte 0x03000040
_08013648: .4byte 0x0201CFB0
_0801364C:
	ldr r0, _080136D8 @ =0xFFFFF01F
	and r0, r4
	strh r0, [r1]
	mov r1, #2
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r6]
	mov r0, #0x16
	bl PlaySE
_08013664:
	ldr r1, _080136DC @ =0x020185C0
	ldr r0, _080136E0 @ =0x0000080C
	add r6, r1, r0
	ldrh r2, [r6]
	lsl r0, r2, #0x14
	lsr r5, r0, #0x19
	add r7, r1, #0
	cmp r5, #0x3F
	bgt _080136FC
	ldr r0, _080136E4 @ =0x00400058
	ldr r1, _080136E8 @ =0x000040C0
	ldr r2, _080136EC @ =0x0000F364
	ldr r4, _080136F0 @ =0x081A4424
	mov r3, #0xF
	and r5, r3
	lsl r3, r5, #1
	add r3, r3, r4
	ldrh r3, [r3]
	lsl r3, r3, #0x10
	bl AddAffineSprite
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r4, #0x7F
	and r0, r4
	lsl r0, r0, #5
	ldr r3, _080136D8 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r6]
	ldr r1, _080136F4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _080136C0
	ldr r1, _080136F8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080136C0
	b _080137EA
_080136C0:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x37
	ble _080136CA
	b _080137EA
_080136CA:
	add r0, #7
	and r0, r4
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r6]
	b _080137EA
_080136D8: .4byte 0xFFFFF01F
_080136DC: .4byte 0x020185C0
_080136E0: .4byte 0x0000080C
_080136E4: .4byte 0x00400058
_080136E8: .4byte 0x000040C0
_080136EC: .4byte 0x0000F364
_080136F0: .4byte gPulseScaleCurve
_080136F4: .4byte 0x03000040
_080136F8: .4byte 0x0201CFB0
_080136FC:
	ldr r0, _08013744 @ =0xFFFFF01F
	and r0, r2
	mov r2, #0x80
	lsl r2, r2, #2
	add r1, r2, #0
	orr r0, r1
	strh r0, [r6]
	ldr r0, _08013748 @ =0x0000080A
	add r3, r7, r0
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
_08013724:
	ldr r1, _0801374C @ =0x0000080C
	add r0, r7, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0
	beq _080137DC
	mov r0, r8
	cmp r0, #0
	beq _08013754
	ldr r1, _08013750 @ =0x081A44FC
	sub r0, r2, #1
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	b _08013762
_08013744: .4byte 0xFFFFF01F
_08013748: .4byte 0x0000080A
_0801374C: .4byte 0x0000080C
_08013750: .4byte gBannerSlideOffsets
_08013754:
	ldr r0, _080137BC @ =0x081A44FC
	sub r1, r2, #1
	lsl r1, r1, #1
	add r1, r1, r0
	mov r0, #0x80
	ldrh r1, [r1]
	sub r0, r0, r1
_08013762:
	lsl r0, r0, #0x10
	mov r1, #0x58
	orr r0, r1
	ldr r1, _080137C0 @ =0x000040C0
	ldr r2, _080137C4 @ =0x0000F364
	bl AddSprite
	ldr r0, _080137C8 @ =0x020185C0
	ldr r1, _080137CC @ =0x0000080C
	add r3, r0, r1
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	sub r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _080137D0 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _080137D4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _080137A6
	ldr r1, _080137D8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080137EA
_080137A6:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #8
	ble _080137EA
	sub r0, #7
_080137B0:
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
	b _080137EA
_080137BC: .4byte gBannerSlideOffsets
_080137C0: .4byte 0x000040C0
_080137C4: .4byte 0x0000F364
_080137C8: .4byte 0x020185C0
_080137CC: .4byte 0x0000080C
_080137D0: .4byte 0xFFFFF01F
_080137D4: .4byte 0x03000040
_080137D8: .4byte 0x0201CFB0
_080137DC:
	ldr r2, _080137F4 @ =0x0000080D
	add r1, r7, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080137EA:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080137F4: .4byte 0x0000080D
	thumb_func_end DuelCmd_ShowJustAMomentBanner

