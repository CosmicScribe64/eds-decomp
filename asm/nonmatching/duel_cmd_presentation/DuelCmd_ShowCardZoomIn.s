	thumb_func_start DuelCmd_ShowCardZoomIn
DuelCmd_ShowCardZoomIn: @ 0x080150DC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r4, _08015104 @ =0x020185C0
	ldr r1, _08015108 @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	cmp r0, #7
	bls _080150FA
	b _080153AC
_080150FA:
	lsl r0, r0, #2
	ldr r1, _0801510C @ =0x08015110
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08015104: .4byte 0x020185C0
_08015108: .4byte 0x0000080A
_0801510C: .4byte 0x08015110
_08015110:
	.4byte _08015130
	.4byte _08015148
	.4byte _08015184
	.4byte _08015194
	.4byte _080151A4
	.4byte _080151B4
	.4byte _080151E4
	.4byte _08015384
_08015130:
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	ldr r2, _08015140 @ =0x020185C0
	ldr r3, _08015144 @ =0x0000080A
	add r2, r2, r3
	b _0801515E
_08015140: .4byte 0x020185C0
_08015144: .4byte 0x0000080A
_08015148:
	bl UnloadDuelUiGfx
	ldr r2, _08015178 @ =0x020185C0
	ldr r7, _0801517C @ =0x0000080C
	add r1, r2, r7
	ldr r0, _08015180 @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	sub r7, #2
	add r2, r2, r7
_0801515E:
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
	b _080153BE
	.align 2, 0
_08015178: .4byte 0x020185C0
_0801517C: .4byte 0x0000080C
_08015180: .4byte 0xFFFFF01F
_08015184:
	ldr r1, _08015190 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl LoadCardFrame
	b _08015384
	.align 2, 0
_08015190: .4byte 0x020185C0
_08015194:
	ldr r1, _080151A0 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl LoadCardPicture
	b _08015384
	.align 2, 0
_080151A0: .4byte 0x020185C0
_080151A4:
	ldr r1, _080151B0 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl DrawCardInfo
	b _08015384
	.align 2, 0
_080151B0: .4byte 0x020185C0
_080151B4:
	bl TextCellsClear
	ldr r4, _080151DC @ =0x020185C0
	ldrh r0, [r4, #2]
	bl DuelInfo_DrawCardNameCentered
	ldr r0, _080151E0 @ =0x0000080A
	add r4, r4, r0
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _080153BE
_080151DC: .4byte 0x020185C0
_080151E0: .4byte 0x0000080A
_080151E4:
	mov r1, #0
	ldr r2, _08015268 @ =0x04000050
	mov r9, r2
	ldr r3, _0801526C @ =0x04000052
	mov r8, r3
_080151EE:
	mov r5, #0
	lsl r7, r1, #5
	str r7, [sp, #0]
	lsl r0, r1, #1
	add r1, #1
	mov sl, r1
	add r0, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0xB
_08015200:
	lsl r1, r5, #5
	add r4, r1, #0
	add r4, #0x44
	ldr r2, [sp, #0]
	add r2, #2
	ldr r3, _08015270 @ =0x020185C0
	ldr r7, _08015274 @ =0x0000080C
	add r0, r3, r7
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _0801523C
	sub r4, #0x68
	ldr r2, [sp, #0]
	sub r2, #0x3E
	mul r4, r0
	mul r2, r0
	add r0, r4, #0
	cmp r4, #0
	bge _0801522C
	add r0, #0x1F
_0801522C:
	asr r4, r0, #5
	add r0, r2, #0
	cmp r2, #0
	bge _08015236
	add r0, #0x1F
_08015236:
	asr r2, r0, #5
	add r4, #0x68
	add r2, #0x40
_0801523C:
	ldr r1, _08015270 @ =0x020185C0
	ldr r3, _08015274 @ =0x0000080C
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0xF
	bgt _08015278
	mov r7, #0xF4
	lsl r7, r7, #4
	add r0, r7, #0
	mov r1, r9
	strh r0, [r1]
	mov r0, #0x10
	sub r0, r0, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r3, r0
	mov r7, r8
	strh r3, [r7]
	b _080152A8
	.align 2, 0
_08015268: .4byte 0x04000050
_0801526C: .4byte 0x04000052
_08015270: .4byte 0x020185C0
_08015274: .4byte 0x0000080C
_08015278:
	cmp r3, #0x67
	ble _0801529E
	mov r1, #0xF4
	lsl r1, r1, #4
	add r0, r1, #0
	mov r7, r9
	strh r0, [r7]
	mov r1, #0x78
	sub r1, r1, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r3, #0
	sub r0, #0x68
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	mov r0, r8
	strh r1, [r0]
	b _080152A8
_0801529E:
	mov r0, #0
	mov r1, r9
	strh r0, [r1]
	mov r3, r8
	strh r0, [r3]
_080152A8:
	ldr r0, _080152D8 @ =0x020185C0
	ldr r7, _080152DC @ =0x0000080C
	add r0, r0, r7
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0xF
	bgt _080152E4
	lsl r0, r2, #0x10
	orr r4, r0
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r6
	ldr r1, _080152E0 @ =0x081A4424
	lsl r0, r3, #1
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	add r0, r4, #0
	mov r1, #0x80
	bl AddAffineSprite8bppAlpha
	b _080152F6
	.align 2, 0
_080152D8: .4byte 0x020185C0
_080152DC: .4byte 0x0000080C
_080152E0: .4byte gPulseScaleCurve
_080152E4:
	lsl r0, r2, #0x10
	orr r4, r0
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r6
	add r0, r4, #0
	mov r1, #0x80
	bl AddSprite8bppAlpha
_080152F6:
	add r5, #1
	cmp r5, #3
	bgt _080152FE
	b _08015200
_080152FE:
	mov r1, sl
	cmp r1, #4
	bgt _08015306
	b _080151EE
_08015306:
	ldr r0, _08015360 @ =0x020185C0
	ldr r1, _08015364 @ =0x0000080C
	add r4, r0, r1
	ldrh r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x77
	bgt _08015374
	ldr r1, _08015368 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801532E
	ldr r1, _0801536C @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08015342
_0801532E:
	cmp r2, #0x6F
	bgt _08015342
	add r0, r2, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _08015370 @ =0xFFFFF01F
	and r1, r3
	orr r1, r0
	strh r1, [r4]
_08015342:
	ldr r2, _08015360 @ =0x020185C0
	ldr r7, _08015364 @ =0x0000080C
	add r3, r2, r7
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08015370 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _080153BE
_08015360: .4byte 0x020185C0
_08015364: .4byte 0x0000080C
_08015368: .4byte 0x03000040
_0801536C: .4byte 0x0201CFB0
_08015370: .4byte 0xFFFFF01F
_08015374:
	ldr r0, _0801537C @ =0x020185C0
	ldr r1, _08015380 @ =0x0000080A
	add r3, r0, r1
	b _0801538A
_0801537C: .4byte 0x020185C0
_08015380: .4byte 0x0000080A
_08015384:
	ldr r2, _080153A4 @ =0x020185C0
	ldr r7, _080153A8 @ =0x0000080A
	add r3, r2, r7
_0801538A:
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
	b _080153BE
	.align 2, 0
_080153A4: .4byte 0x020185C0
_080153A8: .4byte 0x0000080A
_080153AC:
	bl LoadDuelUiGfx
	ldr r0, _080153D0 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080153BE:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080153D0: .4byte 0x0000080D
	thumb_func_end DuelCmd_ShowCardZoomIn

