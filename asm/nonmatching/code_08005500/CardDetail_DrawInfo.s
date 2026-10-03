	thumb_func_start CardDetail_DrawInfo
CardDetail_DrawInfo: @ 0x08005A70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x98
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r0, r7, #6
	ldr r1, _08005ADC @ =0x0822C720
	add r1, r1, r0
	mov r8, r1
	mov r0, r8
	bl StrLen
	add r2, r0, #0
	mov r4, #4
	mov r9, r4
	mov r6, #2
	mov sl, r6
	mov r6, #0xC
	cmp r2, #0x24
	ble _08005AA2
	mov sl, r4
	mov r6, #0xA
_08005AA2:
	ldr r1, _08005AE0 @ =0x02013D90
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08005ABA
	add r0, r2, #0
	mul r0, r6
	asr r0, r0, #1
	mov r1, #0x78
	sub r1, r1, r0
	mov r9, r1
_08005ABA:
	ldr r0, _08005AE4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08005AE8 @ =0x08621DE0
	add r4, r0, r1
	ldr r0, [r4]
	mov r5, #0xF8
	lsl r5, r5, #0x11
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08005B6C
	cmp r0, #0x16
	bgt _08005AEC
	cmp r0, #0x15
	beq _08005AFA
	b _08005C3C
_08005ADC: .4byte gCardNames
_08005AE0: .4byte 0x02013D90
_08005AE4: .4byte 0x000007FF
_08005AE8: .4byte gCardStats
_08005AEC:
	cmp r0, #0x17
	bne _08005AF2
	b _08005C7C
_08005AF2:
	cmp r0, #0x18
	bne _08005AF8
	b _08005C14
_08005AF8:
	b _08005C3C
_08005AFA:
	ldr r0, _08005B28 @ =0x05000220
	ldr r1, _08005B2C @ =0x08636348
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005B30 @ =0x06010400
	ldr r1, _08005B34 @ =0x086366A8
	mov r2, #0x80
	bl CopyDoubleWords
	ldr r1, [r4]
	add r0, r1, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08005B38
	cmp r0, #0x15
	blt _08005B38
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08005B3A
_08005B28: .4byte 0x05000220
_08005B2C: .4byte gTrapIconPal
_08005B30: .4byte 0x06010400
_08005B34: .4byte gTrapIconGfx
_08005B38:
	mov r0, #0
_08005B3A:
	cmp r0, #0
	bne _08005B40
	b _08005C7C
_08005B40:
	ldr r4, _08005B60 @ =0x08637394
	ldr r0, _08005B64 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _08005B68 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08005BE8
	cmp r0, #0x15
	bge _08005BD0
	b _08005BE8
_08005B60: .4byte gUnk_08637394
_08005B64: .4byte 0x000007FF
_08005B68: .4byte gCardStats
_08005B6C:
	ldr r0, _08005B9C @ =0x05000220
	ldr r1, _08005BA0 @ =0x08636368
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005BA4 @ =0x06010400
	ldr r1, _08005BA8 @ =0x08636728
	mov r2, #0x80
	bl CopyDoubleWords
	ldr r1, [r4]
	add r0, r1, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08005BAC
	cmp r0, #0x15
	blt _08005BAC
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08005BAE
	.align 2, 0
_08005B9C: .4byte 0x05000220
_08005BA0: .4byte gMagicIconPal
_08005BA4: .4byte 0x06010400
_08005BA8: .4byte gMagicIconGfx
_08005BAC:
	mov r0, #0
_08005BAE:
	cmp r0, #0
	beq _08005C7C
	ldr r4, _08005BDC @ =0x08637394
	ldr r0, _08005BE0 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08005BE4 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08005BE8
	cmp r0, #0x15
	blt _08005BE8
_08005BD0:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08005BEA
	.align 2, 0
_08005BDC: .4byte gUnk_08637394
_08005BE0: .4byte 0x000007FF
_08005BE4: .4byte gCardStats
_08005BE8:
	mov r0, #0
_08005BEA:
	sub r0, #1
	lsl r0, r0, #5
	add r4, r4, r0
	ldr r0, _08005C08 @ =0x05000240
	ldr r1, _08005C0C @ =0x08637454
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005C10 @ =0x06010480
	add r1, r4, #0
	mov r2, #0x20
	bl CopyDoubleWords
	b _08005C7C
	.align 2, 0
_08005C08: .4byte 0x05000240
_08005C0C: .4byte gSpellTrapSubtypeIconPal
_08005C10: .4byte 0x06010480
_08005C14:
	ldr r0, _08005C2C @ =0x05000220
	ldr r1, _08005C30 @ =0x08636388
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005C34 @ =0x06010400
	ldr r1, _08005C38 @ =0x086367A8
	mov r2, #0x80
	bl CopyDoubleWords
	b _08005C7C
	.align 2, 0
_08005C2C: .4byte 0x05000220
_08005C30: .4byte gDivineIconPal
_08005C34: .4byte 0x06010400
_08005C38: .4byte gDivineIconGfx
_08005C3C:
	ldr r0, _08005D34 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _08005D38 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsr r4, r1, #0x1D
	cmp r4, #0
	beq _08005C7C
	cmp r4, #6
	bhi _08005C7C
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r1, r0
	lsr r0, r1, #0x14
	cmp r0, #0x14
	bhi _08005C7C
	ldr r0, _08005D3C @ =0x05000220
	ldr r1, _08005D40 @ =0x08198950
	lsl r4, r4, #2
	add r1, r4, r1
	ldr r1, [r1]
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005D44 @ =0x06010400
	ldr r1, _08005D48 @ =0x0819897C
	add r4, r4, r1
	ldr r1, [r4]
	mov r2, #0x80
	bl CopyDoubleWords
_08005C7C:
	bl ClearBgMapBuffer0
	mov r2, #0x88
	lsl r2, r2, #2
	mov r3, #0xF2
	lsl r3, r3, #1
	ldr r0, _08005D4C @ =0x00000807
	str r0, [sp, #0]
	str r6, [sp, #4]
	mov r4, r9
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	mov r6, sl
	lsl r1, r6, #0x10
	orr r0, r1
	str r0, [sp, #8]
	mov r0, r8
	str r0, [sp, #0xC]
	mov r4, #1
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	mov r0, #0
	mov r1, #0
	bl CardDetail_DrawTextBox
	ldr r0, _08005D50 @ =0x02013D90
	ldrb r0, [r0]
	and r4, r0
	cmp r4, #0
	beq _08005CBA
	b _0800636C
_08005CBA:
	ldr r4, _08005D34 @ =0x000007FF
	and r4, r7
	lsl r0, r4, #1
	ldr r1, _08005D54 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _08005D58 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08005CD4
	b _080060B0
_08005CD4:
	ldr r0, _08005D5C @ =0x05000020
	ldr r2, _08005D40 @ =0x08198950
	lsl r4, r4, #2
	ldr r6, _08005D38 @ =0x08621DE0
	add r4, r4, r6
	ldr r1, [r4]
	lsr r1, r1, #0x1D
	lsl r1, r1, #2
	add r1, r1, r2
	ldr r1, [r1]
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005D60 @ =0x06004400
	ldr r2, _08005D48 @ =0x0819897C
	ldr r1, [r4]
	lsr r1, r1, #0x1D
	lsl r1, r1, #2
	add r1, r1, r2
	ldr r1, [r1]
	mov r2, #0x80
	bl CopyDoubleWords
	ldr r2, _08005D64 @ =0x03000040
	ldr r0, _08005D68 @ =0x000004B6
	add r1, r2, r0
	mov r0, #0x81
	lsl r0, r0, #5
	strh r0, [r1]
	mov r6, #0x97
	lsl r6, r6, #3
	add r1, r2, r6
	add r0, #1
	strh r0, [r1]
	ldr r0, _08005D6C @ =0x000004F6
	add r1, r2, r0
	ldr r0, _08005D70 @ =0x00001022
	strh r0, [r1]
	add r6, #0x40
	add r1, r2, r6
	add r0, #1
	strh r0, [r1]
	mov r3, #0
	mov r5, #0xF0
	lsl r5, r5, #0xC
	ldr r0, _08005D74 @ =0x0000041C
	add r2, r2, r0
	b _08005D8C
_08005D34: .4byte 0x000007FF
_08005D38: .4byte gCardStats
_08005D3C: .4byte 0x05000220
_08005D40: .4byte gCardIconPals
_08005D44: .4byte 0x06010400
_08005D48: .4byte gCardIconGfx
_08005D4C: .4byte 0x00000807
_08005D50: .4byte 0x02013D90
_08005D54: .4byte gCardIdToNumber
_08005D58: .4byte 0xFFFFF880
_08005D5C: .4byte 0x05000020
_08005D60: .4byte 0x06004400
_08005D64: .4byte 0x03000040
_08005D68: .4byte 0x000004B6
_08005D6C: .4byte 0x000004F6
_08005D70: .4byte 0x00001022
_08005D74: .4byte 0x0000041C
_08005D78:
	lsr r0, r5, #0x10
	add r0, #0x60
	lsl r0, r0, #1
	add r0, r0, r2
	mov r1, #3
	strh r1, [r0]
	mov r1, #0x80
	lsl r1, r1, #9
	add r5, r5, r1
	add r3, #1
_08005D8C:
	ldr r0, [r4]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08005DAC
	cmp r0, #0x17
	ble _08005DA4
	cmp r0, #0x18
	beq _08005DA8
	b _08005DAC
_08005DA4:
	mov r0, #0
	b _08005DB6
_08005DA8:
	mov r0, #0xA
	b _08005DB6
_08005DAC:
	ldr r0, [r4]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08005DB6:
	cmp r3, r0
	blt _08005D78
	ldr r1, _08005E54 @ =0x08081538
	add r0, sp, #0x18
	bl StrCopy
	ldr r1, _08005E58 @ =0x081988D0
	ldr r5, _08005E5C @ =0x000007FF
	and r5, r7
	lsl r5, r5, #2
	ldr r2, _08005E60 @ =0x08621DE0
	add r5, r5, r2
	ldr r0, [r5]
	mov r4, #0xF8
	lsl r4, r4, #0x11
	mov r8, r4
	and r0, r4
	lsr r0, r0, #0x12
	add r0, r0, r1
	ldr r1, [r0]
	add r0, sp, #0x18
	bl StrCat
	ldr r1, _08005E64 @ =0x0808153C
	add r0, sp, #0x18
	bl StrCat
	ldr r6, _08005E68 @ =0x00000A08
	mov r0, #4
	mov r1, #0x14
	add r2, r6, #0
	add r3, sp, #0x18
	bl TextDrawString
	ldr r2, _08005E6C @ =0x00000A07
	mov r0, #3
	mov r1, #0x13
	add r3, sp, #0x18
	bl TextDrawString
	mov r0, #0x11
	mov r1, #0x10
	bl TextCanvasInit
	ldr r4, _08005E70 @ =0x08081540
	add r0, r4, #0
	bl StrLen
	mov r9, r0
	ldr r2, _08005E74 @ =0x00000A0D
	mov r0, #4
	mov r1, #0x20
	add r3, r4, #0
	bl TextDrawString
	ldr r2, _08005E78 @ =0x00000A05
	mov r0, #3
	mov r1, #0x1F
	add r3, r4, #0
	bl TextDrawString
	mov r1, r9
	add r1, #4
	lsl r0, r1, #2
	add r0, r0, r1
	add r4, r0, #4
	add r2, r6, #0
	ldr r0, [r5]
	mov r6, r8
	and r0, r6
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08005E86
	cmp r0, #0x17
	ble _08005E7C
	cmp r0, #0x18
	beq _08005E80
	b _08005E86
	.align 2, 0
_08005E54: .4byte gStrOpenBracket
_08005E58: .4byte gCardTypeNames
_08005E5C: .4byte 0x000007FF
_08005E60: .4byte gCardStats
_08005E64: .4byte gStrCloseBracket
_08005E68: .4byte 0x00000A08
_08005E6C: .4byte 0x00000A07
_08005E70: .4byte gStrAtk
_08005E74: .4byte 0x00000A0D
_08005E78: .4byte 0x00000A05
_08005E7C:
	mov r0, #0
	b _08005E9C
_08005E80:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08005E9C
_08005E86:
	ldr r0, _08005ED4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08005ED8 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08005E9C:
	add r3, r0, #0
	add r0, r4, #0
	mov r1, #0x20
	bl TextDrawNumber
	mov r1, r9
	add r1, #4
	lsl r0, r1, #2
	add r0, r0, r1
	add r2, r0, #3
	ldr r0, _08005ED4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r4, _08005ED8 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08005EE6
	cmp r0, #0x17
	ble _08005EDC
	cmp r0, #0x18
	beq _08005EE0
	b _08005EE6
	.align 2, 0
_08005ED4: .4byte 0x000007FF
_08005ED8: .4byte gCardStats
_08005EDC:
	mov r0, #0
	b _08005EFC
_08005EE0:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08005EFC
_08005EE6:
	ldr r0, _08005F58 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r6, _08005F5C @ =0x08621DE0
	add r0, r0, r6
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08005EFC:
	add r3, r0, #0
	add r0, r2, #0
	mov r1, #0x1F
	ldr r2, _08005F60 @ =0x00000A07
	bl TextDrawNumber
	ldr r4, _08005F64 @ =0x08081544
	add r0, r4, #0
	bl StrLen
	mov r9, r0
	ldr r2, _08005F68 @ =0x00000A0B
	mov r0, #4
	mov r1, #0x2C
	add r3, r4, #0
	bl TextDrawString
	ldr r2, _08005F6C @ =0x00000A03
	mov r0, #3
	mov r1, #0x2B
	add r3, r4, #0
	bl TextDrawString
	mov r1, r9
	add r1, #4
	lsl r0, r1, #2
	add r0, r0, r1
	add r2, r0, #4
	ldr r0, _08005F58 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08005F5C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08005F7A
	cmp r0, #0x17
	ble _08005F70
	cmp r0, #0x18
	beq _08005F74
	b _08005F7A
	.align 2, 0
_08005F58: .4byte 0x000007FF
_08005F5C: .4byte gCardStats
_08005F60: .4byte 0x00000A07
_08005F64: .4byte gStrDef
_08005F68: .4byte 0x00000A0B
_08005F6C: .4byte 0x00000A03
_08005F70:
	mov r0, #0
	b _08005F90
_08005F74:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08005F90
_08005F7A:
	ldr r0, _08005FC8 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r4, _08005FCC @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	ldr r0, _08005FD0 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08005F90:
	add r3, r0, #0
	add r0, r2, #0
	mov r1, #0x2C
	ldr r2, _08005FD4 @ =0x00000A08
	bl TextDrawNumber
	mov r1, r9
	add r1, #4
	lsl r0, r1, #2
	add r0, r0, r1
	add r2, r0, #3
	ldr r0, _08005FC8 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r6, _08005FCC @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08005FE2
	cmp r0, #0x17
	ble _08005FD8
	cmp r0, #0x18
	beq _08005FDC
	b _08005FE2
_08005FC8: .4byte 0x000007FF
_08005FCC: .4byte gCardStats
_08005FD0: .4byte 0x000001FF
_08005FD4: .4byte 0x00000A08
_08005FD8:
	mov r0, #0
	b _08005FF8
_08005FDC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08005FF8
_08005FE2:
	ldr r0, _0800607C @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08006080 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08006084 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08005FF8:
	add r3, r0, #0
	add r0, r2, #0
	mov r1, #0x2B
	ldr r2, _08006088 @ =0x00000A07
	bl TextDrawNumber
	ldr r0, _0800608C @ =0x06008900
	mov r1, #9
	bl TextCanvasToTiles
	mov r1, #0x92
	lsl r1, r1, #2
	mov r3, #0
	ldr r2, _08006090 @ =0x08081548
	mov r9, r2
	ldr r4, _08006094 @ =0x0300245C
	mov r8, r4
	mov r7, #0x80
	lsl r7, r7, #9
_0800601E:
	mov r4, #0
	add r6, r3, #1
	add r0, r3, #2
	lsl r0, r0, #0x10
	lsr r5, r0, #0xB
	mov r3, #0xD0
	lsl r3, r3, #0xC
_0800602C:
	lsr r0, r3, #0x10
	add r0, r0, r5
	lsl r0, r0, #1
	add r0, r8
	add r2, r1, #0
	add r1, r2, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	strh r2, [r0]
	add r3, r3, r7
	add r4, #1
	cmp r4, #0x10
	ble _0800602C
	add r3, r6, #0
	cmp r3, #0xF
	ble _0800601E
	ldr r1, _08006098 @ =0x0000120D
	ldr r2, _0800609C @ =0x00000212
	mov r3, #0x89
	lsl r3, r3, #2
	ldr r0, _080060A0 @ =0x00000806
	str r0, [sp, #0]
	mov r0, #0xA
	str r0, [sp, #4]
	ldr r0, _080060A4 @ =0x00030003
	str r0, [sp, #8]
	mov r6, r9
	str r6, [sp, #0xC]
	mov r4, #0
	str r4, [sp, #0x10]
	mov r0, #0xB
	str r0, [sp, #0x14]
	mov r0, #4
	bl CardDetail_DrawTextBox
	ldr r0, _080060A8 @ =0x03000040
	ldr r1, _080060AC @ =0x00004426
	add r0, r0, r1
	b _08006364
	.align 2, 0
_0800607C: .4byte 0x000007FF
_08006080: .4byte gCardStats
_08006084: .4byte 0x000001FF
_08006088: .4byte 0x00000A07
_0800608C: .4byte 0x06008900
_08006090: .4byte gStrNotACard
_08006094: .4byte 0x0300245C
_08006098: .4byte 0x0000120D
_0800609C: .4byte 0x00000212
_080060A0: .4byte 0x00000806
_080060A4: .4byte 0x00030003
_080060A8: .4byte 0x03000040
_080060AC: .4byte 0x00004426
_080060B0:
	ldr r1, _080060E0 @ =0x08081538
	add r0, sp, #0x18
	bl StrCopy
	lsl r0, r4, #2
	ldr r2, _080060E4 @ =0x08621DE0
	add r4, r0, r2
	ldr r0, [r4]
	mov r5, #0xF8
	lsl r5, r5, #0x11
	and r0, r5
	lsr r2, r0, #0x14
	cmp r2, #0x15
	blt _08006168
	cmp r2, #0x16
	ble _080060EC
	cmp r2, #0x18
	bgt _08006168
	ldr r0, _080060E8 @ =0x081988D0
	lsl r1, r2, #2
	add r1, r1, r0
	ldr r1, [r1]
	b _08006294
	.align 2, 0
_080060E0: .4byte gStrOpenBracket
_080060E4: .4byte gCardStats
_080060E8: .4byte gCardTypeNames
_080060EC:
	ldr r1, _08006114 @ =0x081988D0
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r1, [r0]
	add r0, sp, #0x18
	bl StrCat
	ldr r4, [r4]
	add r0, r4, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08006118
	cmp r0, #0x15
	blt _08006118
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r4, r0
	lsr r0, r4, #0x11
	b _0800611A
_08006114: .4byte gCardTypeNames
_08006118:
	mov r0, #0
_0800611A:
	cmp r0, #0
	bne _08006120
	b _080062B4
_08006120:
	add r2, sp, #0x18
	ldr r3, _0800614C @ =0x08198934
	ldr r0, _08006150 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r4, _08006154 @ =0x08621DE0
	add r0, r0, r4
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08006158
	cmp r0, #0x15
	blt _08006158
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0800615A
	.align 2, 0
_0800614C: .4byte gSpellTrapSubtypeSuffixes
_08006150: .4byte 0x000007FF
_08006154: .4byte gCardStats
_08006158:
	mov r0, #0
_0800615A:
	lsl r0, r0, #2
	add r0, r3, r0
	ldr r1, [r0]
	add r0, r2, #0
	bl StrCat
	b _080062B4
_08006168:
	ldr r2, _0800619C @ =0x081988D0
	ldr r4, _080061A0 @ =0x000007FF
	and r4, r7
	lsl r0, r4, #2
	ldr r6, _080061A4 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x12
	add r0, r0, r2
	ldr r1, [r0]
	add r0, sp, #0x18
	bl StrCat
	lsl r4, r4, #1
	ldr r0, _080061A8 @ =0x08622AB4
	add r4, r4, r0
	ldrh r1, [r4]
	ldr r0, _080061AC @ =0x00000776
	cmp r1, r0
	bne _080061B0
	mov r0, #3
	b _08006212
	.align 2, 0
_0800619C: .4byte gCardTypeNames
_080061A0: .4byte 0x000007FF
_080061A4: .4byte gCardStats
_080061A8: .4byte gCardIdToNumber
_080061AC: .4byte 0x00000776
_080061B0:
	cmp r1, r0
	blt _080061C0
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080061C0
	mov r0, #1
	b _08006212
_080061C0:
	ldr r0, _080061E4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _080061E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080061F2
	cmp r0, #0x16
	bgt _080061EC
	cmp r0, #0x15
	beq _080061F6
	b _080061FE
	.align 2, 0
_080061E4: .4byte 0x000007FF
_080061E8: .4byte gCardStats
_080061EC:
	cmp r0, #0x17
	beq _080061FA
	b _080061FE
_080061F2:
	mov r0, #7
	b _08006212
_080061F6:
	mov r0, #8
	b _08006212
_080061FA:
	mov r0, #9
	b _08006212
_080061FE:
	ldr r0, _08006224 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _08006228 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08006212:
	cmp r0, #1
	beq _0800622C
	cmp r0, #1
	ble _080062B4
	cmp r0, #2
	beq _08006234
	cmp r0, #3
	beq _08006280
	b _080062B4
_08006224: .4byte 0x000007FF
_08006228: .4byte gCardStats
_0800622C:
	ldr r1, _08006230 @ =0x0808155C
	b _08006294
_08006230: .4byte gStrEffectSuffix
_08006234:
	ldr r0, _08006254 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r4, _08006258 @ =0x08622AB4
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _0800625C @ =0x000004D9
	cmp r1, r0
	beq _0800626C
	cmp r1, r0
	bgt _08006260
	mov r0, #0xCB
	lsl r0, r0, #2
	cmp r1, r0
	beq _0800626C
	b _08006278
_08006254: .4byte 0x000007FF
_08006258: .4byte gCardIdToNumber
_0800625C: .4byte 0x000004D9
_08006260:
	ldr r0, _08006270 @ =0x00000536
	cmp r1, r0
	beq _0800626C
	add r0, #0xC0
	cmp r1, r0
	bne _08006278
_0800626C:
	ldr r1, _08006274 @ =0x08081564
	b _08006294
_08006270: .4byte 0x00000536
_08006274: .4byte gStrFusionEffectSuffix
_08006278:
	ldr r1, _0800627C @ =0x08081574
	b _08006294
_0800627C: .4byte gStrFusionSuffix
_08006280:
	ldr r0, _0800629C @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r6, _080062A0 @ =0x08622AB4
	add r0, r0, r6
	ldr r1, _080062A4 @ =0x000002DA
	ldrh r0, [r0]
	cmp r0, r1
	bne _080062AC
	ldr r1, _080062A8 @ =0x0808157C
_08006294:
	add r0, sp, #0x18
	bl StrCat
	b _080062B4
_0800629C: .4byte 0x000007FF
_080062A0: .4byte gCardIdToNumber
_080062A4: .4byte 0x000002DA
_080062A8: .4byte gStrRitualEffectSuffix
_080062AC:
	ldr r1, _0800637C @ =0x08081500
	add r0, sp, #0x18
	bl StrCat
_080062B4:
	ldr r1, _08006380 @ =0x0808153C
	add r0, sp, #0x18
	bl StrCat
	add r0, sp, #0x18
	bl StrLen
	add r2, r0, #0
	mov r6, #0xA
	cmp r2, #0x16
	ble _080062CC
	mov r6, #8
_080062CC:
	ldr r1, _08006384 @ =0x0000020D
	ldr r2, _08006388 @ =0x00000212
	mov r3, #0x89
	lsl r3, r3, #2
	ldr r0, _0800638C @ =0x00000807
	str r0, [sp, #0]
	str r6, [sp, #4]
	lsr r4, r6, #1
	mov r0, #9
	sub r0, r0, r4
	lsl r0, r0, #0x10
	mov r4, #4
	orr r0, r4
	str r0, [sp, #8]
	add r0, sp, #0x18
	str r0, [sp, #0xC]
	mov r4, #1
	str r4, [sp, #0x10]
	mov r0, #9
	str r0, [sp, #0x14]
	mov r0, #0
	bl CardDetail_DrawTextBox
	ldr r1, _08006390 @ =0x0000040D
	ldr r2, _08006394 @ =0x00001812
	mov r3, #0x92
	lsl r3, r3, #2
	ldr r0, _08006398 @ =0x00000907
	str r0, [sp, #0]
	mov r5, #0xA
	str r5, [sp, #4]
	ldr r0, _0800639C @ =0x00020004
	str r0, [sp, #8]
	lsl r0, r7, #4
	sub r0, r0, r7
	lsl r0, r0, #5
	ldr r6, _080063A0 @ =0x082461A0
	add r0, r0, r6
	str r0, [sp, #0xC]
	str r4, [sp, #0x10]
	mov r4, #0
	str r4, [sp, #0x14]
	mov r0, #4
	bl CardDetail_DrawTextBox
	ldr r0, _080063A4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _080063A8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bls _0800635E
	ldr r1, _080063AC @ =0x0000120D
	ldr r2, _080063B0 @ =0x00000211
	ldr r3, _080063B4 @ =0x00000369
	ldr r0, _080063B8 @ =0x00000806
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, _080063BC @ =0x00030003
	str r0, [sp, #8]
	ldr r0, _080063C0 @ =0x0808158C
	str r0, [sp, #0xC]
	str r4, [sp, #0x10]
	mov r0, #0xB
	str r0, [sp, #0x14]
	mov r0, #4
	bl CardDetail_DrawTextBox
_0800635E:
	ldr r0, _080063C4 @ =0x03000040
	ldr r2, _080063C8 @ =0x00004426
	add r0, r0, r2
_08006364:
	strh r4, [r0]
	ldr r0, _080063CC @ =0x02013D90
	str r4, [r0, #0x38]
	str r4, [r0, #0x34]
_0800636C:
	add sp, #0x98
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800637C: .4byte gStrRitualSuffix
_08006380: .4byte gStrCloseBracket
_08006384: .4byte 0x0000020D
_08006388: .4byte 0x00000212
_0800638C: .4byte 0x00000807
_08006390: .4byte 0x0000040D
_08006394: .4byte 0x00001812
_08006398: .4byte 0x00000907
_0800639C: .4byte 0x00020004
_080063A0: .4byte gCardDescriptions
_080063A4: .4byte 0x000007FF
_080063A8: .4byte gCardStats
_080063AC: .4byte 0x0000120D
_080063B0: .4byte 0x00000211
_080063B4: .4byte 0x00000369
_080063B8: .4byte 0x00000806
_080063BC: .4byte 0x00030003
_080063C0: .4byte gStrNotPlayable
_080063C4: .4byte 0x03000040
_080063C8: .4byte 0x00004426
_080063CC: .4byte 0x02013D90
	thumb_func_end CardDetail_DrawInfo

