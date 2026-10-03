	thumb_func_start DuelCmd_ShowCardAssemble
DuelCmd_ShowCardAssemble: @ 0x08014C30
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r4, _08014C58 @ =0x020185C0
	ldr r1, _08014C5C @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	add r6, r4, #0
	cmp r0, #8
	bls _08014C4E
	b _080150B8
_08014C4E:
	lsl r0, r0, #2
	ldr r1, _08014C60 @ =0x08014C64
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08014C58: .4byte 0x020185C0
_08014C5C: .4byte 0x0000080A
_08014C60: .4byte 0x08014C64
_08014C64:
	.4byte _08014C88
	.4byte _08014CA0
	.4byte _08014CDC
	.4byte _08014CEC
	.4byte _08014CF4
	.4byte _08014D04
	.4byte _08014E80
	.4byte _08014F98
	.4byte _08015098
_08014C88:
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	ldr r2, _08014C98 @ =0x020185C0
	ldr r3, _08014C9C @ =0x0000080A
	add r2, r2, r3
	b _08014CB6
_08014C98: .4byte 0x020185C0
_08014C9C: .4byte 0x0000080A
_08014CA0:
	bl UnloadDuelUiGfx
	ldr r2, _08014CD0 @ =0x020185C0
	ldr r5, _08014CD4 @ =0x0000080C
	add r1, r2, r5
	ldr r0, _08014CD8 @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	sub r5, #2
	add r2, r2, r5
_08014CB6:
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
	b _080150CA
	.align 2, 0
_08014CD0: .4byte 0x020185C0
_08014CD4: .4byte 0x0000080C
_08014CD8: .4byte 0xFFFFF01F
_08014CDC:
	ldrh r0, [r6, #2]
	bl LoadCardFrame
	ldr r0, _08014CE8 @ =0x0000080A
	add r3, r6, r0
	b _0801509C
_08014CE8: .4byte 0x0000080A
_08014CEC:
	ldrh r0, [r6, #2]
	bl LoadCardPicture
	b _08015098
_08014CF4:
	ldrh r0, [r6, #2]
	bl DrawCardInfo
	ldr r2, _08014D00 @ =0x0000080A
	add r3, r6, r2
	b _0801509C
_08014D00: .4byte 0x0000080A
_08014D04:
	mov r0, #0
	ldr r3, _08014DB8 @ =0x02018DCC
	mov sl, r3
_08014D0A:
	mov r6, #0
	lsl r7, r0, #5
	lsl r5, r0, #1
	mov r8, r5
	add r0, #1
	mov r9, r0
_08014D16:
	lsl r3, r6, #5
	add r4, r3, #0
	add r4, #0x44
	add r1, r7, #2
	mov r2, sl
	ldrh r2, [r2]
	lsl r0, r2, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x17
	bgt _08014D66
	sub r4, #0x68
	add r5, r7, #0
	sub r5, #0x3E
	ldr r1, _08014DBC @ =0x08081768
	mov r0, #0x17
	sub r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r0, [r0]
	ldr r3, _08014DC0 @ =0xFFFFFF00
	add r0, r0, r3
	mov r1, #3
	bl __divsi3
	add r0, #0x80
	mul r4, r0
	mul r5, r0
	add r0, r4, #0
	cmp r4, #0
	bge _08014D54
	add r0, #0x7F
_08014D54:
	asr r4, r0, #7
	add r0, r5, #0
	cmp r5, #0
	bge _08014D5E
	add r0, #0x7F
_08014D5E:
	asr r5, r0, #7
	add r4, #0x68
	add r1, r5, #0
	add r1, #0x40
_08014D66:
	lsl r0, r1, #0x10
	orr r4, r0
	lsl r2, r6, #0x12
	lsr r2, r2, #0x10
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	add r2, r2, r0
	add r0, r4, #0
	mov r1, #0x80
	bl AddSprite8bppAlpha
	add r6, #1
	cmp r6, #3
	ble _08014D16
	mov r0, r9
	cmp r0, #4
	ble _08014D0A
	ldr r0, _08014DC4 @ =0x020185C0
	ldr r5, _08014DC8 @ =0x0000080C
	add r1, r0, r5
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r2, r1, #0x19
	add r6, r0, #0
	cmp r2, #0xF
	bgt _08014DD0
	ldr r1, _08014DCC @ =0x04000050
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
	b _08014DDA
_08014DB8: .4byte 0x02018DCC
_08014DBC: .4byte gScatterScaleCurve
_08014DC0: .4byte 0xFFFFFF00
_08014DC4: .4byte 0x020185C0
_08014DC8: .4byte 0x0000080C
_08014DCC: .4byte 0x04000050
_08014DD0:
	ldr r0, _08014E68 @ =0x04000054
	mov r1, #0
	strh r1, [r0]
	sub r0, #4
	strh r1, [r0]
_08014DDA:
	ldr r5, _08014E6C @ =0x0000080C
	add r3, r6, r5
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08014E70 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _08014E74 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08014E0E
	ldr r1, _08014E78 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08014E22
_08014E0E:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x17
	bgt _08014E22
	add r0, #7
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
_08014E22:
	ldr r0, _08014E6C @ =0x0000080C
	add r4, r6, r0
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _08014E32
	b _080150CA
_08014E32:
	bl TextCellsClear
	ldrh r0, [r6, #2]
	bl DuelInfo_DrawCardNameCentered
	ldr r0, _08014E70 @ =0xFFFFF01F
	ldrh r2, [r4]
	and r0, r2
	strh r0, [r4]
	ldr r5, _08014E7C @ =0x0000080A
	add r3, r6, r5
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
	b _080150CA
	.align 2, 0
_08014E68: .4byte 0x04000054
_08014E6C: .4byte 0x0000080C
_08014E70: .4byte 0xFFFFF01F
_08014E74: .4byte 0x03000040
_08014E78: .4byte 0x0201CFB0
_08014E7C: .4byte 0x0000080A
_08014E80:
	mov r0, #0
_08014E82:
	mov r6, #0
	lsl r7, r0, #5
	lsl r1, r0, #1
	mov r8, r1
	add r0, #1
	mov r9, r0
	add r0, r7, #2
	lsl r7, r0, #0x10
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0xB
	mov r4, #0x44
_08014E9C:
	add r0, r4, #0
	orr r0, r7
	lsl r2, r6, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r5
	mov r1, #0x80
	bl AddSprite8bpp
	add r4, #0x20
	add r6, #1
	cmp r6, #3
	ble _08014E9C
	mov r0, r9
	cmp r0, #4
	ble _08014E82
	ldr r0, _08014EE0 @ =0x020185C0
	ldr r2, _08014EE4 @ =0x0000080C
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r2, r1, #0x19
	add r6, r0, #0
	cmp r2, #0x1F
	bgt _08014F10
	cmp r2, #0xF
	bgt _08014EF4
	ldr r0, _08014EE8 @ =0x04000054
	strh r2, [r0]
	ldr r1, _08014EEC @ =0x04000050
	ldr r3, _08014EF0 @ =0x00001090
	add r0, r3, #0
	strh r0, [r1]
	b _08014F1A
	.align 2, 0
_08014EE0: .4byte 0x020185C0
_08014EE4: .4byte 0x0000080C
_08014EE8: .4byte 0x04000054
_08014EEC: .4byte 0x04000050
_08014EF0: .4byte 0x00001090
_08014EF4:
	ldr r1, _08014F08 @ =0x04000054
	mov r0, #0x1F
	sub r0, r0, r2
	strh r0, [r1]
	sub r1, #4
	ldr r5, _08014F0C @ =0x00001090
	add r0, r5, #0
	strh r0, [r1]
	b _08014F1A
	.align 2, 0
_08014F08: .4byte 0x04000054
_08014F0C: .4byte 0x00001090
_08014F10:
	ldr r0, _08014F80 @ =0x04000054
	mov r1, #0
	strh r1, [r0]
	sub r0, #4
	strh r1, [r0]
_08014F1A:
	ldr r0, _08014F84 @ =0x0000080C
	add r3, r6, r0
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08014F88 @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _08014F8C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08014F4E
	ldr r1, _08014F90 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08014F62
_08014F4E:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x17
	bgt _08014F62
	add r0, #7
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
_08014F62:
	ldr r1, _08014F84 @ =0x0000080C
	add r2, r6, r1
	ldrh r1, [r2]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x1F
	bgt _08014F72
	b _080150CA
_08014F72:
	ldr r0, _08014F88 @ =0xFFFFF01F
	and r0, r1
	strh r0, [r2]
	ldr r2, _08014F94 @ =0x0000080A
	add r3, r6, r2
	b _0801509C
	.align 2, 0
_08014F80: .4byte 0x04000054
_08014F84: .4byte 0x0000080C
_08014F88: .4byte 0xFFFFF01F
_08014F8C: .4byte 0x03000040
_08014F90: .4byte 0x0201CFB0
_08014F94: .4byte 0x0000080A
_08014F98:
	mov r0, #0
_08014F9A:
	mov r6, #0
	lsl r7, r0, #5
	lsl r3, r0, #1
	mov r8, r3
	add r0, #1
	mov r9, r0
	add r0, r7, #2
	lsl r7, r0, #0x10
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0xB
	mov r4, #0x44
_08014FB4:
	add r0, r4, #0
	orr r0, r7
	lsl r2, r6, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r5
	mov r1, #0x80
	bl AddSprite8bppAlpha
	add r4, #0x20
	add r6, #1
	cmp r6, #3
	ble _08014FB4
	mov r0, r9
	cmp r0, #4
	ble _08014F9A
	ldr r0, _08015008 @ =0x020185C0
	ldr r5, _0801500C @ =0x0000080C
	add r1, r0, r5
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x19
	add r6, r0, #0
	cmp r4, #0x30
	ble _08015018
	ldr r1, _08015010 @ =0x04000050
	mov r2, #0xF4
	lsl r2, r2, #4
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _08015014 @ =0x04000052
	mov r1, #0x40
	sub r1, r1, r4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r4, #0
	sub r0, #0x30
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	strh r1, [r2]
	b _08015022
	.align 2, 0
_08015008: .4byte 0x020185C0
_0801500C: .4byte 0x0000080C
_08015010: .4byte 0x04000050
_08015014: .4byte 0x04000052
_08015018:
	ldr r0, _08015078 @ =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
_08015022:
	ldr r3, _0801507C @ =0x0000080C
	add r4, r6, r3
	ldrh r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x3F
	bgt _0801508C
	ldr r1, _08015080 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08015048
	ldr r1, _08015084 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801505C
_08015048:
	cmp r2, #0x37
	bgt _0801505C
	add r0, r2, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _08015088 @ =0xFFFFF01F
	and r1, r3
	orr r1, r0
	strh r1, [r4]
_0801505C:
	ldr r5, _0801507C @ =0x0000080C
	add r3, r6, r5
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08015088 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _080150CA
_08015078: .4byte 0x04000050
_0801507C: .4byte 0x0000080C
_08015080: .4byte 0x03000040
_08015084: .4byte 0x0201CFB0
_08015088: .4byte 0xFFFFF01F
_0801508C:
	ldr r0, _08015094 @ =0x0000080A
	add r3, r6, r0
	b _0801509C
	.align 2, 0
_08015094: .4byte 0x0000080A
_08015098:
	ldr r1, _080150B4 @ =0x0000080A
	add r3, r6, r1
_0801509C:
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
	b _080150CA
_080150B4: .4byte 0x0000080A
_080150B8:
	bl LoadDuelUiGfx
	ldr r2, _080150D8 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
_080150CA:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080150D8: .4byte 0x0000080D
	thumb_func_end DuelCmd_ShowCardAssemble

