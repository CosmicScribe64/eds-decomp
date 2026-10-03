	thumb_func_start CardMenu_Update
CardMenu_Update: @ 0x0801DC04
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r1, _0801DC3C @ =0x020192E0
	ldr r2, _0801DC40 @ =0x00001B2F
	add r0, r1, r2
	ldrb r0, [r0]
	lsr r3, r0, #2
	ldr r4, _0801DC44 @ =0x00001B30
	add r2, r1, r4
	mov r0, #3
	ldrb r2, [r2]
	and r0, r2
	lsl r0, r0, #6
	orr r0, r3
	add r4, r1, #0
	cmp r0, #0xC
	bls _0801DC30
	b _0801E220
_0801DC30:
	lsl r0, r0, #2
	ldr r1, _0801DC48 @ =0x0801DC4C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801DC3C: .4byte 0x020192E0
_0801DC40: .4byte 0x00001B2F
_0801DC44: .4byte 0x00001B30
_0801DC48: .4byte 0x0801DC4C
_0801DC4C:
	.4byte _0801DC80
	.4byte _0801DD16
	.4byte _0801DD68
	.4byte _0801E000
	.4byte _0801E220
	.4byte _0801E220
	.4byte _0801E220
	.4byte _0801E220
	.4byte _0801E220
	.4byte _0801E220
	.4byte _0801E0A0
	.4byte _0801E11C
	.4byte _0801E1DC
_0801DC80:
	ldr r1, _0801DCDC @ =0x0201CFB0
	ldr r0, _0801DCE0 @ =0x00000808
	add r1, r1, r0
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _0801DCE4 @ =0x00001B2C
	add r2, r4, r3
	ldr r0, _0801DCE8 @ =0xFFFFFC3F
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	mov r6, #0x3D
	neg r6, r6
	add r0, r6, #0
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	mov r5, #0
	ldr r0, [r2]
	lsl r0, r0, #6
	lsr r0, r0, #0x10
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0801DCBA
	b _0801E0CC
_0801DCBA:
	add r3, r2, #0
_0801DCBC:
	ldrb r2, [r3]
	mov r0, #0x3C
	and r0, r2
	cmp r0, #0
	beq _0801DCEC
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	add r1, r6, #0
	and r1, r2
	orr r1, r0
	strb r1, [r3]
	b _0801DCF6
_0801DCDC: .4byte 0x0201CFB0
_0801DCE0: .4byte 0x00000808
_0801DCE4: .4byte 0x00001B2C
_0801DCE8: .4byte 0xFFFFFC3F
_0801DCEC:
	add r0, r6, #0
	and r0, r2
	mov r1, #0x30
	orr r0, r1
	strb r0, [r3]
_0801DCF6:
	add r5, #1
	cmp r5, #0xC
	ble _0801DCFE
	b _0801E0CC
_0801DCFE:
	ldr r0, [r3]
	lsl r0, r0, #6
	lsr r0, r0, #0x10
	ldrb r2, [r3]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	asr r0, r1
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0801DCBC
	b _0801E0CC
_0801DD16:
	bl CardMenu_DrawCardPreview
	ldr r7, _0801DD54 @ =0x020192E0
	ldr r0, _0801DD58 @ =0x00001B2C
	add r4, r7, r0
	ldrh r2, [r4]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1C
	add r1, #1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _0801DD5C @ =0xFFFFFC3F
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	bl CardMenu_DrawIcons
	ldrh r4, [r4]
	lsl r0, r4, #0x16
	lsr r0, r0, #0x1C
	cmp r0, #7
	bhi _0801DD46
	b _0801E248
_0801DD46:
	ldr r1, _0801DD60 @ =0x00001B2F
	add r6, r7, r1
	ldrb r5, [r6]
	lsr r0, r5, #2
	ldr r2, _0801DD64 @ =0x00001B30
	add r4, r7, r2
	b _0801E0D8
_0801DD54: .4byte 0x020192E0
_0801DD58: .4byte 0x00001B2C
_0801DD5C: .4byte 0xFFFFFC3F
_0801DD60: .4byte 0x00001B2F
_0801DD64: .4byte 0x00001B30
_0801DD68:
	bl CardMenu_DrawCardPreview
	bl CardMenu_DrawIcons
	bl CardMenu_DrawLabel
	ldr r1, _0801DDAC @ =0x03000040
	mov r0, #0x20
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0801DDE4
	mov r5, #0
	ldr r4, _0801DDB0 @ =0x020192E0
	ldr r0, _0801DDB4 @ =0x00001B2C
	add r4, r4, r0
	mov r3, #0x3D
	neg r3, r3
_0801DD8C:
	ldrb r2, [r4]
	mov r0, #0x3C
	and r0, r2
	cmp r0, #0
	beq _0801DDB8
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	add r1, r3, #0
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _0801DDC2
_0801DDAC: .4byte 0x03000040
_0801DDB0: .4byte 0x020192E0
_0801DDB4: .4byte 0x00001B2C
_0801DDB8:
	add r0, r3, #0
	and r0, r2
	mov r1, #0x30
	orr r0, r1
	strb r0, [r4]
_0801DDC2:
	ldr r0, [r4]
	lsl r0, r0, #6
	lsr r0, r0, #0x10
	ldrb r2, [r4]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	asr r0, r1
	mov r1, #1
	and r0, r1
	cmp r0, #0
	bne _0801DDDE
	add r5, #1
	cmp r5, #0xC
	ble _0801DD8C
_0801DDDE:
	mov r0, #0
	bl PlaySE
_0801DDE4:
	ldr r1, _0801DE18 @ =0x03000040
	mov r0, #0x10
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0801DE4C
	mov r5, #0
	ldr r4, _0801DE1C @ =0x020192E0
	ldr r3, _0801DE20 @ =0x00001B2C
	add r4, r4, r3
	mov r3, #0x3D
	neg r3, r3
_0801DDFC:
	ldrb r2, [r4]
	lsl r1, r2, #0x1A
	lsr r0, r1, #0x1C
	cmp r0, #0xB
	bhi _0801DE24
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	add r1, r3, #0
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _0801DE2A
_0801DE18: .4byte 0x03000040
_0801DE1C: .4byte 0x020192E0
_0801DE20: .4byte 0x00001B2C
_0801DE24:
	add r0, r3, #0
	and r0, r2
	strb r0, [r4]
_0801DE2A:
	ldr r0, [r4]
	lsl r0, r0, #6
	lsr r0, r0, #0x10
	ldrb r2, [r4]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	asr r0, r1
	mov r1, #1
	and r0, r1
	cmp r0, #0
	bne _0801DE46
	add r5, #1
	cmp r5, #0xC
	ble _0801DDFC
_0801DE46:
	mov r0, #0
	bl PlaySE
_0801DE4C:
	ldr r0, _0801DE74 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0801DE80
	mov r0, #2
	bl PlaySE
	ldr r3, _0801DE78 @ =0x020192E0
	ldr r4, _0801DE7C @ =0x00001B2C
	add r1, r3, r4
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	add r4, #3
	b _0801E194
	.align 2, 0
_0801DE74: .4byte 0x03000040
_0801DE78: .4byte 0x020192E0
_0801DE7C: .4byte 0x00001B2C
_0801DE80:
	mov r0, #1
	mov sl, r0
	and r0, r1
	cmp r0, #0
	bne _0801DE8C
	b _0801E248
_0801DE8C:
	ldr r6, _0801DF58 @ =0x020192E0
	ldr r1, _0801DF5C @ =0x00001B2C
	add r1, r1, r6
	mov r9, r1
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r3, _0801DF60 @ =0x00001B30
	add r7, r6, r3
	ldr r0, _0801DF64 @ =0xFFFFFC03
	ldrh r4, [r7]
	and r0, r4
	strh r0, [r7]
	bl DuelCursor_GetCardId
	ldr r2, _0801DF68 @ =0x00001B28
	add r1, r6, r2
	strh r0, [r1]
	ldr r2, _0801DF6C @ =0x0201CFB0
	ldr r3, _0801DF70 @ =0x00000824
	add r0, r2, r3
	ldr r4, _0801DF74 @ =0x00001B33
	add r3, r6, r4
	ldrb r0, [r0]
	mov r1, #1
	and r0, r1
	lsl r0, r0, #1
	mov r5, #3
	neg r5, r5
	ldrb r4, [r3]
	and r5, r4
	orr r5, r0
	ldr r1, _0801DF78 @ =0x00000828
	add r0, r2, r1
	ldr r1, [r0]
	lsl r1, r1, #0x10
	lsr r0, r1, #0x10
	mov r4, #0x3F
	mov ip, r4
	and r0, r4
	lsl r0, r0, #2
	str r0, [sp, #0]
	mov r0, #3
	mov r8, r0
	and r5, r0
	ldr r4, [sp, #0]
	orr r5, r4
	strb r5, [r3]
	lsr r1, r1, #0x16
	mov r0, sl
	and r1, r0
	ldr r4, _0801DF7C @ =0x00001B34
	add r3, r6, r4
	mov r0, #1
	and r1, r0
	sub r0, #3
	ldrb r4, [r3]
	and r0, r4
	orr r0, r1
	strb r0, [r3]
	ldr r0, _0801DF80 @ =0x0000082C
	add r2, r2, r0
	ldrb r2, [r2]
	lsl r1, r2, #1
	ldr r0, _0801DF84 @ =0xFFFFFE01
	ldrh r2, [r3]
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	mov r0, #0x3C
	mov r4, r9
	ldrb r4, [r4]
	and r0, r4
	cmp r0, #0
	bne _0801DFC0
	lsr r1, r5, #2
	mov r0, sl
	ldrb r3, [r3]
	and r0, r3
	lsl r0, r0, #6
	orr r0, r1
	cmp r0, #0xC
	bne _0801DF8C
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #0xC
	mov r2, #0
	mov r3, #0
	bl CardListView_Open
	mov r0, #1
	bl PlaySE
	ldr r0, _0801DF88 @ =0x00001B2F
	add r2, r6, r0
	mov r0, r8
	ldrb r1, [r2]
	and r0, r1
	mov r1, #8
	b _0801DFAC
	.align 2, 0
_0801DF58: .4byte 0x020192E0
_0801DF5C: .4byte 0x00001B2C
_0801DF60: .4byte 0x00001B30
_0801DF64: .4byte 0xFFFFFC03
_0801DF68: .4byte 0x00001B28
_0801DF6C: .4byte 0x0201CFB0
_0801DF70: .4byte 0x00000824
_0801DF74: .4byte 0x00001B33
_0801DF78: .4byte 0x00000828
_0801DF7C: .4byte 0x00001B34
_0801DF80: .4byte 0x0000082C
_0801DF84: .4byte 0xFFFFFE01
_0801DF88: .4byte 0x00001B2F
_0801DF8C:
	mov r0, #1
	bl PlaySE
	mov r0, #3
	neg r0, r0
	mov r3, r9
	ldrb r3, [r3]
	and r0, r3
	mov r4, r9
	strb r0, [r4]
	ldr r0, _0801DFBC @ =0x00001B2F
	add r2, r6, r0
	mov r0, r8
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x28
_0801DFAC:
	orr r0, r1
	strb r0, [r2]
	mov r0, #4
	neg r0, r0
	ldrb r2, [r7]
	and r0, r2
	strb r0, [r7]
	b _0801E248
_0801DFBC: .4byte 0x00001B2F
_0801DFC0:
	ldr r3, _0801DFFC @ =0x00001B2F
	add r4, r6, r3
	ldrb r3, [r4]
	lsr r0, r3, #2
	mov r1, r8
	ldrb r2, [r7]
	and r1, r2
	lsl r1, r1, #6
	orr r1, r0
	add r1, #1
	add r2, r1, #0
	mov r0, ip
	and r2, r0
	lsl r2, r2, #2
	mov r0, r8
	and r0, r3
	orr r0, r2
	strb r0, [r4]
	lsr r1, r1, #6
	mov r2, r8
	and r1, r2
	mov r0, #3
	and r1, r0
	mov r0, #4
	neg r0, r0
	ldrb r3, [r7]
	and r0, r3
	orr r0, r1
	strb r0, [r7]
	b _0801E248
_0801DFFC: .4byte 0x00001B2F
_0801E000:
	ldr r0, _0801E048 @ =0x00001B2C
	add r4, r4, r0
	ldrh r2, [r4]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1C
	sub r1, #1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _0801E04C @ =0xFFFFFC3F
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	bl CardMenu_DrawCardPreview
	bl CardMenu_DrawIcons
	mov r0, #0xF0
	lsl r0, r0, #2
	ldrh r4, [r4]
	and r0, r4
	cmp r0, #0
	beq _0801E030
	b _0801E248
_0801E030:
	ldr r1, _0801E050 @ =0x0201CFB0
	ldr r2, _0801E054 @ =0x00000828
	add r0, r1, r2
	ldr r0, [r0]
	add r2, r1, #0
	cmp r0, #5
	beq _0801E05E
	cmp r0, #5
	bgt _0801E058
	cmp r0, #0
	beq _0801E05E
	b _0801E08C
_0801E048: .4byte 0x00001B2C
_0801E04C: .4byte 0xFFFFFC3F
_0801E050: .4byte 0x0201CFB0
_0801E054: .4byte 0x00000828
_0801E058:
	cmp r0, #0xA
	beq _0801E080
	b _0801E08C
_0801E05E:
	ldr r3, _0801E078 @ =0x00000824
	add r0, r2, r3
	ldr r0, [r0]
	ldr r4, _0801E07C @ =0x00000828
	add r1, r2, r4
	add r3, #8
	add r2, r2, r3
	ldr r1, [r1]
	ldr r2, [r2]
	add r1, r1, r2
	bl DrawZoneTiles
	b _0801E08C
_0801E078: .4byte 0x00000824
_0801E07C: .4byte 0x00000828
_0801E080:
	ldr r4, _0801E094 @ =0x00000824
	add r0, r1, r4
	ldr r0, [r0]
	mov r1, #0xA
	bl DrawZoneTiles
_0801E08C:
	ldr r3, _0801E098 @ =0x020192E0
	ldr r0, _0801E09C @ =0x00001B2F
	add r6, r3, r0
	b _0801E196
_0801E094: .4byte 0x00000824
_0801E098: .4byte 0x020192E0
_0801E09C: .4byte 0x00001B2F
_0801E0A0:
	bl DuelScreen_FadeOutStep
	cmp r0, #0
	bne _0801E0AA
	b _0801E248
_0801E0AA:
	ldr r2, _0801E108 @ =0x0201CFB0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #5
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r4, _0801E10C @ =0x020192E0
	ldr r2, _0801E110 @ =0x00001B28
	add r0, r4, r2
	ldrh r0, [r0]
	mov r1, #0
	mov r2, #0
	bl CardDetail_Init
_0801E0CC:
	ldr r3, _0801E114 @ =0x00001B2F
	add r6, r4, r3
	ldrb r5, [r6]
	lsr r0, r5, #2
	ldr r1, _0801E118 @ =0x00001B30
	add r4, r4, r1
_0801E0D8:
	mov r3, #3
	add r1, r3, #0
	ldrb r2, [r4]
	and r1, r2
	lsl r1, r1, #6
	orr r1, r0
	add r1, #1
	mov r2, #0x3F
	and r2, r1
	lsl r2, r2, #2
	add r0, r3, #0
	and r0, r5
	orr r0, r2
	strb r0, [r6]
	lsr r1, r1, #6
	and r1, r3
	mov r0, #4
	neg r0, r0
	ldrb r3, [r4]
	and r0, r3
	orr r0, r1
	strb r0, [r4]
	b _0801E248
	.align 2, 0
_0801E108: .4byte 0x0201CFB0
_0801E10C: .4byte 0x020192E0
_0801E110: .4byte 0x00001B28
_0801E114: .4byte 0x00001B2F
_0801E118: .4byte 0x00001B30
_0801E11C:
	bl CardDetail_Run
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801E128
	b _0801E248
_0801E128:
	bl DuelScreen_Init
	bl DuelScreen_DrawCursorInfo
	ldr r1, _0801E148 @ =0x0201CFB0
	ldr r4, _0801E14C @ =0x00000828
	add r0, r1, r4
	ldr r0, [r0]
	cmp r0, #5
	beq _0801E156
	cmp r0, #5
	bgt _0801E150
	cmp r0, #0
	beq _0801E156
	b _0801E190
	.align 2, 0
_0801E148: .4byte 0x0201CFB0
_0801E14C: .4byte 0x00000828
_0801E150:
	cmp r0, #0xA
	beq _0801E184
	b _0801E190
_0801E156:
	ldr r2, _0801E174 @ =0x0201CFB0
	ldr r1, _0801E178 @ =0x00000824
	add r0, r2, r1
	ldr r0, [r0]
	ldr r3, _0801E17C @ =0x00000828
	add r1, r2, r3
	ldr r4, _0801E180 @ =0x0000082C
	add r2, r2, r4
	ldr r1, [r1]
	ldr r2, [r2]
	add r1, r1, r2
	bl ClearZoneTiles
	b _0801E190
	.align 2, 0
_0801E174: .4byte 0x0201CFB0
_0801E178: .4byte 0x00000824
_0801E17C: .4byte 0x00000828
_0801E180: .4byte 0x0000082C
_0801E184:
	ldr r2, _0801E1CC @ =0x00000824
	add r0, r1, r2
	ldr r0, [r0]
	mov r1, #0xA
	bl ClearZoneTiles
_0801E190:
	ldr r3, _0801E1D0 @ =0x020192E0
	ldr r4, _0801E1D4 @ =0x00001B2F
_0801E194:
	add r6, r3, r4
_0801E196:
	ldrb r5, [r6]
	lsr r0, r5, #2
	ldr r1, _0801E1D8 @ =0x00001B30
	add r3, r3, r1
	mov r4, #3
	add r1, r4, #0
	ldrb r2, [r3]
	and r1, r2
	lsl r1, r1, #6
	orr r1, r0
	add r1, #1
	mov r2, #0x3F
	and r2, r1
	lsl r2, r2, #2
	add r0, r4, #0
	and r0, r5
	orr r0, r2
	strb r0, [r6]
	lsr r1, r1, #6
	and r1, r4
	mov r0, #4
	neg r0, r0
	ldrb r4, [r3]
	and r0, r4
	orr r0, r1
	strb r0, [r3]
	b _0801E248
_0801E1CC: .4byte 0x00000824
_0801E1D0: .4byte 0x020192E0
_0801E1D4: .4byte 0x00001B2F
_0801E1D8: .4byte 0x00001B30
_0801E1DC:
	bl CardMenu_DrawCardPreview
	bl CardMenu_DrawIcons
	bl CardMenu_DrawLabel
	bl DuelScreen_FadeInStep
	cmp r0, #0
	beq _0801E248
	ldr r2, _0801E214 @ =0x020192E0
	ldr r0, _0801E218 @ =0x00001B2F
	add r3, r2, r0
	mov r0, #3
	ldrb r1, [r3]
	and r0, r1
	mov r1, #8
	orr r0, r1
	strb r0, [r3]
	ldr r3, _0801E21C @ =0x00001B30
	add r2, r2, r3
	mov r0, #4
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	strb r0, [r2]
	b _0801E248
	.align 2, 0
_0801E214: .4byte 0x020192E0
_0801E218: .4byte 0x00001B2F
_0801E21C: .4byte 0x00001B30
_0801E220:
	ldr r0, _0801E258 @ =0x00001B2C
	add r1, r4, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _0801E25C @ =0x00001B2F
	add r1, r4, r3
	mov r0, #3
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	add r3, #1
	add r1, r4, r3
	mov r0, #4
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_0801E248:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801E258: .4byte 0x00001B2C
_0801E25C: .4byte 0x00001B2F
	thumb_func_end CardMenu_Update

