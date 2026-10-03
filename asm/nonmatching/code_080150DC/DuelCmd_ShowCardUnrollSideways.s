	thumb_func_start DuelCmd_ShowCardUnrollSideways
DuelCmd_ShowCardUnrollSideways: @ 0x08015E40
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _08015E64 @ =0x020185C0
	ldr r1, _08015E68 @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	add r6, r4, #0
	cmp r0, #8
	bls _08015E5A
	b _080162A4
_08015E5A:
	lsl r0, r0, #2
	ldr r1, _08015E6C @ =0x08015E70
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08015E64: .4byte 0x020185C0
_08015E68: .4byte 0x0000080A
_08015E6C: .4byte 0x08015E70
_08015E70:
	.4byte _08015E94
	.4byte _08015EB0
	.4byte _08015EDC
	.4byte _08015EEC
	.4byte _08015EF4
	.4byte _08015F18
	.4byte _08016058
	.4byte _08016164
	.4byte _08016284
_08015E94:
	bl TextCellsClear
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	ldr r2, _08015EA8 @ =0x020185C0
	ldr r3, _08015EAC @ =0x0000080A
	add r2, r2, r3
	b _08015EBA
_08015EA8: .4byte 0x020185C0
_08015EAC: .4byte 0x0000080A
_08015EB0:
	bl UnloadDuelUiGfx
	ldr r2, _08015ED4 @ =0x020185C0
	ldr r0, _08015ED8 @ =0x0000080A
	add r2, r2, r0
_08015EBA:
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
	b _080162B6
	.align 2, 0
_08015ED4: .4byte 0x020185C0
_08015ED8: .4byte 0x0000080A
_08015EDC:
	ldrh r0, [r6, #2]
	bl LoadCardFrame
	ldr r1, _08015EE8 @ =0x0000080A
	add r3, r6, r1
	b _08016288
_08015EE8: .4byte 0x0000080A
_08015EEC:
	ldrh r0, [r6, #2]
	bl LoadCardPicture
	b _08016284
_08015EF4:
	ldrh r0, [r6, #2]
	bl DrawCardInfo
	ldr r3, _08015F0C @ =0x0000080C
	add r1, r6, r3
	ldr r0, _08015F10 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r0, _08015F14 @ =0x0000080A
	add r3, r6, r0
	b _08016288
_08015F0C: .4byte 0x0000080C
_08015F10: .4byte 0xFFFFF01F
_08015F14: .4byte 0x0000080A
_08015F18:
	mov r1, #0
	ldr r7, _08015F98 @ =0x02018DCC
_08015F1C:
	mov r4, #0
	lsl r0, r1, #5
	lsl r2, r1, #1
	add r1, #1
	mov r8, r1
	add r0, #2
	lsl r6, r0, #0x10
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0xB
_08015F30:
	lsl r2, r4, #5
	add r1, r2, #0
	add r1, #0x44
	ldrh r3, [r7]
	lsl r0, r3, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xF
	bgt _08015F50
	sub r1, #0x68
	mul r1, r0
	add r0, r1, #0
	cmp r1, #0
	bge _08015F4C
	add r0, #0xF
_08015F4C:
	asr r1, r0, #4
	add r1, #0x68
_08015F50:
	orr r1, r6
	lsl r2, r4, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r5
	add r0, r1, #0
	mov r1, #0x80
	bl AddSprite8bppAlpha
	add r4, #1
	cmp r4, #3
	ble _08015F30
	mov r1, r8
	cmp r1, #4
	ble _08015F1C
	ldr r0, _08015F9C @ =0x020185C0
	ldr r2, _08015FA0 @ =0x0000080C
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r2, r1, #0x19
	add r6, r0, #0
	cmp r2, #0xF
	bgt _08015FA8
	ldr r1, _08015FA4 @ =0x04000050
	mov r3, #0xF4
	lsl r3, r3, #4
	add r0, r3, #0
	strh r0, [r1]
	add r1, #2
	mov r0, #0x10
	sub r0, r0, r2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r2, r0
	strh r2, [r1]
	b _08015FB2
_08015F98: .4byte 0x02018DCC
_08015F9C: .4byte 0x020185C0
_08015FA0: .4byte 0x0000080C
_08015FA4: .4byte 0x04000050
_08015FA8:
	ldr r0, _08016040 @ =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
_08015FB2:
	ldr r0, _08016044 @ =0x0000080C
	add r3, r6, r0
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08016048 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _0801604C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08015FE6
	ldr r1, _08016050 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08015FFA
_08015FE6:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xB
	bgt _08015FFA
	add r0, #3
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
_08015FFA:
	ldr r1, _08016044 @ =0x0000080C
	add r4, r6, r1
	ldrh r2, [r4]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xF
	bgt _0801600A
	b _080162B6
_0801600A:
	bl TextCellsClear
	ldrh r0, [r6, #2]
	bl DuelInfo_DrawCardNameCentered
	ldr r0, _08016048 @ =0xFFFFF01F
	ldrh r3, [r4]
	and r0, r3
	strh r0, [r4]
	ldr r0, _08016054 @ =0x0000080A
	add r3, r6, r0
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
	mov r0, #0x2C
	bl PlaySE
	b _080162B6
	.align 2, 0
_08016040: .4byte 0x04000050
_08016044: .4byte 0x0000080C
_08016048: .4byte 0xFFFFF01F
_0801604C: .4byte 0x03000040
_08016050: .4byte 0x0201CFB0
_08016054: .4byte 0x0000080A
_08016058:
	mov r1, #0
_0801605A:
	mov r4, #0
	lsl r0, r1, #5
	lsl r2, r1, #1
	add r1, #1
	mov r8, r1
	add r0, #2
	lsl r7, r0, #0x10
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0xB
	mov r5, #0x44
_08016070:
	add r0, r5, #0
	orr r0, r7
	lsl r2, r4, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r6
	mov r1, #0x80
	bl AddSprite8bpp
	add r5, #0x20
	add r4, #1
	cmp r4, #3
	ble _08016070
	mov r1, r8
	cmp r1, #4
	ble _0801605A
	ldr r0, _080160B4 @ =0x020185C0
	ldr r2, _080160B8 @ =0x0000080C
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r2, r1, #0x19
	add r6, r0, #0
	cmp r2, #0x1F
	bgt _080160E4
	cmp r2, #0xF
	bgt _080160C8
	ldr r0, _080160BC @ =0x04000054
	strh r2, [r0]
	ldr r1, _080160C0 @ =0x04000050
	ldr r3, _080160C4 @ =0x00001090
	add r0, r3, #0
	strh r0, [r1]
	b _080160EE
	.align 2, 0
_080160B4: .4byte 0x020185C0
_080160B8: .4byte 0x0000080C
_080160BC: .4byte 0x04000054
_080160C0: .4byte 0x04000050
_080160C4: .4byte 0x00001090
_080160C8:
	ldr r1, _080160DC @ =0x04000054
	mov r0, #0x1F
	sub r0, r0, r2
	strh r0, [r1]
	sub r1, #4
	ldr r2, _080160E0 @ =0x00001090
	add r0, r2, #0
	strh r0, [r1]
	b _080160EE
	.align 2, 0
_080160DC: .4byte 0x04000054
_080160E0: .4byte 0x00001090
_080160E4:
	ldr r0, _08016150 @ =0x04000054
	mov r1, #0
	strh r1, [r0]
	sub r0, #4
	strh r1, [r0]
_080160EE:
	ldr r0, _08016154 @ =0x0000080C
	add r3, r6, r0
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08016158 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _0801615C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016122
	ldr r1, _08016160 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08016136
_08016122:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1B
	bgt _08016136
	add r0, #3
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
_08016136:
	ldr r1, _08016154 @ =0x0000080C
	add r2, r6, r1
	ldrh r1, [r2]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _08016146
	b _080162B6
_08016146:
	ldr r0, _08016158 @ =0xFFFFF01F
	and r0, r1
	strh r0, [r2]
	b _08016284
	.align 2, 0
_08016150: .4byte 0x04000054
_08016154: .4byte 0x0000080C
_08016158: .4byte 0xFFFFF01F
_0801615C: .4byte 0x03000040
_08016160: .4byte 0x0201CFB0
_08016164:
	mov r1, #0
	ldr r7, _080161F0 @ =0x02018DCC
_08016168:
	mov r4, #0
	lsl r0, r1, #5
	lsl r2, r1, #1
	add r1, #1
	mov r8, r1
	add r0, #2
	lsl r6, r0, #0x10
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0xB
_0801617C:
	lsl r3, r4, #5
	add r1, r3, #0
	add r1, #0x44
	ldrh r2, [r7]
	lsl r0, r2, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x27
	ble _080161A0
	sub r1, #0x68
	mov r0, #0x38
	sub r0, r0, r2
	mul r1, r0
	add r0, r1, #0
	cmp r1, #0
	bge _0801619C
	add r0, #0xF
_0801619C:
	asr r1, r0, #4
	add r1, #0x68
_080161A0:
	orr r1, r6
	lsl r2, r4, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r5
	add r0, r1, #0
	mov r1, #0x80
	bl AddSprite8bppAlpha
	add r4, #1
	cmp r4, #3
	ble _0801617C
	mov r1, r8
	cmp r1, #4
	ble _08016168
	ldr r0, _080161F4 @ =0x020185C0
	ldr r3, _080161F8 @ =0x0000080C
	add r1, r0, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x19
	add r6, r0, #0
	cmp r4, #0x27
	ble _08016204
	ldr r1, _080161FC @ =0x04000050
	mov r2, #0xF4
	lsl r2, r2, #4
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _08016200 @ =0x04000052
	mov r1, #0x38
	sub r1, r1, r4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r4, #0
	sub r0, #0x28
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	strh r1, [r2]
	b _0801620E
_080161F0: .4byte 0x02018DCC
_080161F4: .4byte 0x020185C0
_080161F8: .4byte 0x0000080C
_080161FC: .4byte 0x04000050
_08016200: .4byte 0x04000052
_08016204:
	ldr r0, _08016264 @ =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
_0801620E:
	ldr r3, _08016268 @ =0x0000080C
	add r4, r6, r3
	ldrh r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x37
	bgt _08016278
	ldr r1, _0801626C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016234
	ldr r1, _08016270 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08016248
_08016234:
	cmp r2, #0x2F
	bgt _08016248
	add r0, r2, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _08016274 @ =0xFFFFF01F
	and r1, r3
	orr r1, r0
	strh r1, [r4]
_08016248:
	ldr r0, _08016268 @ =0x0000080C
	add r3, r6, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08016274 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _080162B6
_08016264: .4byte 0x04000050
_08016268: .4byte 0x0000080C
_0801626C: .4byte 0x03000040
_08016270: .4byte 0x0201CFB0
_08016274: .4byte 0xFFFFF01F
_08016278:
	ldr r1, _08016280 @ =0x0000080A
	add r3, r6, r1
	b _08016288
	.align 2, 0
_08016280: .4byte 0x0000080A
_08016284:
	ldr r2, _080162A0 @ =0x0000080A
	add r3, r6, r2
_08016288:
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
	b _080162B6
_080162A0: .4byte 0x0000080A
_080162A4:
	bl LoadDuelUiGfx
	ldr r3, _080162C0 @ =0x0000080D
	add r1, r4, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080162B6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080162C0: .4byte 0x0000080D
	thumb_func_end DuelCmd_ShowCardUnrollSideways

