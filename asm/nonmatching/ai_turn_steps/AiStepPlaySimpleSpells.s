	thumb_func_start AiStepPlaySimpleSpells
AiStepPlaySimpleSpells: @ 0x0805AB90
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	ldr r4, _0805ABB0 @ =0x02015EF0
	ldrb r0, [r4, #0xA]
	cmp r0, #1
	bne _0805ABA6
	b _0805AD94
_0805ABA6:
	cmp r0, #1
	bgt _0805ABB4
	cmp r0, #0
	beq _0805ABBC
	b _0805B170
_0805ABB0: .4byte 0x02015EF0
_0805ABB4:
	cmp r0, #2
	bne _0805ABBA
	b _0805AF60
_0805ABBA:
	b _0805B170
_0805ABBC:
	strb r0, [r4, #6]
	ldr r0, _0805AC14 @ =0x08086448
	mov r9, r0
	mov r8, r4
	mov r1, #0xFA
	lsl r1, r1, #2
	mov sl, r1
_0805ABCA:
	mov r2, r8
	ldrb r2, [r2, #6]
	lsl r0, r2, #1
	add r0, r9
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #1
	bl CountActivatableSetCards
	cmp r0, #0
	bne _0805ABE2
	b _0805AD7A
_0805ABE2:
	mov r4, #0
	mov r1, r8
	ldrb r1, [r1, #6]
	lsl r0, r1, #1
	add r0, r9
	mov r2, #0
	ldsh r1, [r0, r2]
	ldr r0, _0805AC18 @ =0x000003EF
	cmp r1, r0
	bgt _0805AC30
	sub r0, #1
	cmp r1, r0
	blt _0805ABFE
	b _0805AD0E
_0805ABFE:
	mov r0, #0xAD
	lsl r0, r0, #1
	cmp r1, r0
	beq _0805AC68
	cmp r1, r0
	bgt _0805AC1C
	sub r0, #9
	cmp r1, r0
	bge _0805AC12
	b _0805AD0A
_0805AC12:
	b _0805AD0E
_0805AC14: .4byte gAiSimpleSpells
_0805AC18: .4byte 0x000003EF
_0805AC1C:
	ldr r0, _0805AC2C @ =0x0000029F
	cmp r1, r0
	beq _0805ACD4
	mov r0, #0xFB
	lsl r0, r0, #2
	cmp r1, r0
	beq _0805ACCA
	b _0805AD0A
_0805AC2C: .4byte 0x0000029F
_0805AC30:
	ldr r0, _0805AC48 @ =0x00000425
	cmp r1, r0
	beq _0805ACE0
	cmp r1, r0
	bgt _0805AC4C
	sub r0, #0x34
	cmp r1, r0
	beq _0805ACC0
	add r0, #0x1E
	cmp r1, r0
	beq _0805ACA4
	b _0805AD0A
_0805AC48: .4byte 0x00000425
_0805AC4C:
	ldr r0, _0805AC64 @ =0x0000042F
	cmp r1, r0
	beq _0805ACF6
	cmp r1, r0
	blt _0805AD0A
	add r0, #9
	cmp r1, r0
	bgt _0805AD0A
	sub r0, #1
	cmp r1, r0
	blt _0805AD0A
	b _0805ACD4
_0805AC64: .4byte 0x0000042F
_0805AC68:
	ldr r0, _0805AC9C @ =0x020192E4
	add r2, r0, #0
	ldrh r0, [r2]
	cmp r0, sl
	bhi _0805AC82
	ldr r0, _0805ACA0 @ =0x00000D64
	add r1, r2, r0
	mov r0, #0xFA
	lsl r0, r0, #1
	ldrh r1, [r1]
	cmp r1, r0
	bls _0805AC82
	mov r4, #1
_0805AC82:
	ldr r1, _0805ACA0 @ =0x00000D64
	add r0, r2, r1
	ldrh r1, [r0]
	mov r3, #0xFA
	lsl r3, r3, #1
	add r0, r1, r3
	ldrh r2, [r2]
	cmp r2, r0
	bge _0805AC96
	mov r4, #1
_0805AC96:
	cmp r1, r3
	bls _0805AD0A
	b _0805AD0E
_0805AC9C: .4byte 0x020192E4
_0805ACA0: .4byte 0x00000D64
_0805ACA4:
	ldr r0, _0805ACBC @ =0x020192E4
	ldrb r2, [r0, #2]
	mov r1, #0xC8
	mul r1, r2
	ldrh r0, [r0]
	cmp r1, r0
	ble _0805ACB4
	mov r4, #1
_0805ACB4:
	cmp r2, #2
	bls _0805AD0A
	b _0805AD0E
	.align 2, 0
_0805ACBC: .4byte 0x020192E4
_0805ACC0:
	mov r0, #0
	mov r1, #0x16
	bl CountFaceUpSpellTrapsOfType
	b _0805ACDA
_0805ACCA:
	mov r0, #0
	mov r1, #0x15
	bl CountFaceUpSpellTrapsOfType
	b _0805ACDA
_0805ACD4:
	mov r0, #0
	bl CountSpellTraps
_0805ACDA:
	cmp r0, #0
	ble _0805AD0A
	b _0805AD0E
_0805ACE0:
	mov r0, #0
	bl CountSpellTraps
	cmp r0, #0
	ble _0805AD0A
	mov r0, #1
	bl CountSpellTraps
	cmp r0, #0
	bne _0805AD0A
	b _0805AD0E
_0805ACF6:
	ldr r1, _0805ADE8 @ =0x020192E4
	ldr r2, _0805ADEC @ =0x00000D64
	add r0, r1, r2
	ldrh r0, [r0]
	cmp r0, sl
	bls _0805AD0A
	ldrb r0, [r1, #2]
	cmp r0, #0
	beq _0805AD0A
	mov r4, #1
_0805AD0A:
	cmp r4, #0
	beq _0805AD7A
_0805AD0E:
	mov r2, sp
	ldrb r1, [r2, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r2, #3]
	mov r5, #5
	mov r3, #0x94
	ldr r7, _0805ADF0 @ =0x0201A070
	mov r0, #0xB9
	lsl r0, r0, #2
	add r6, r7, r0
_0805AD24:
	ldr r0, [r6]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r4, #0
	beq _0805AD72
	ldr r1, _0805ADF4 @ =0x000007FF
	add r0, r1, #0
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r2, _0805ADF8 @ =0x08622AB4
	add r1, r1, r2
	mov r0, r8
	ldrb r2, [r0, #6]
	lsl r0, r2, #1
	add r0, r9
	ldrh r1, [r1]
	ldrh r0, [r0]
	cmp r1, r0
	bne _0805AD72
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r7
	add r0, #0x91
	mov r1, #4
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	beq _0805AD72
	mov r0, sp
	mov r1, #1
	add r2, r5, #0
	str r3, [sp, #0x14]
	bl CanActivateFieldCard
	ldr r3, [sp, #0x14]
	cmp r0, #0
	beq _0805AD72
	b _0805B134
_0805AD72:
	add r6, #0x94
	add r5, #1
	cmp r5, #9
	ble _0805AD24
_0805AD7A:
	mov r1, r8
	ldrb r0, [r1, #6]
	add r0, #1
	strb r0, [r1, #6]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x13
	bhi _0805AD8C
	b _0805ABCA
_0805AD8C:
	ldr r2, _0805ADFC @ =0x02015EF0
	ldrb r0, [r2, #0xA]
	add r0, #1
	strb r0, [r2, #0xA]
_0805AD94:
	mov r0, #0
	ldr r1, _0805ADFC @ =0x02015EF0
	strb r0, [r1, #6]
	ldr r7, _0805AE00 @ =0x08086448
	add r5, r1, #0
	mov r4, sp
	ldr r2, _0805AE04 @ =0x000007CF
	mov r8, r2
_0805ADA4:
	ldrb r1, [r5, #6]
	lsl r0, r1, #1
	add r0, r0, r7
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #1
	bl AiFindHandCardByNumber
	cmp r0, #0
	bge _0805ADBA
	b _0805AF40
_0805ADBA:
	mov r6, #0
	ldrb r1, [r5, #6]
	lsl r0, r1, #1
	add r0, r0, r7
	mov r2, #0
	ldsh r1, [r0, r2]
	ldr r0, _0805AE08 @ =0x000003EF
	cmp r1, r0
	bgt _0805AE20
	sub r0, #1
	cmp r1, r0
	bge _0805AEC2
	mov r0, #0xAD
	lsl r0, r0, #1
	cmp r1, r0
	beq _0805AE4C
	cmp r1, r0
	bgt _0805AE0C
	sub r0, #9
	cmp r1, r0
	blt _0805AEBE
	b _0805AEC2
	.align 2, 0
_0805ADE8: .4byte 0x020192E4
_0805ADEC: .4byte 0x00000D64
_0805ADF0: .4byte 0x0201A070
_0805ADF4: .4byte 0x000007FF
_0805ADF8: .4byte gCardIdToNumber
_0805ADFC: .4byte 0x02015EF0
_0805AE00: .4byte gAiSimpleSpells
_0805AE04: .4byte 0x000007CF
_0805AE08: .4byte 0x000003EF
_0805AE0C:
	ldr r0, _0805AE1C @ =0x0000029F
	cmp r1, r0
	beq _0805AE84
	mov r0, #0xFB
	lsl r0, r0, #2
	cmp r1, r0
	beq _0805AE7A
	b _0805AEBE
_0805AE1C: .4byte 0x0000029F
_0805AE20:
	ldr r0, _0805AE34 @ =0x0000042F
	cmp r1, r0
	beq _0805AEA6
	cmp r1, r0
	bgt _0805AE38
	sub r0, #0x3E
	cmp r1, r0
	beq _0805AE70
	add r0, #0x34
	b _0805AE40
_0805AE34: .4byte 0x0000042F
_0805AE38:
	ldr r0, _0805AE48 @ =0x00000437
	cmp r1, r0
	beq _0805AE84
	add r0, #1
_0805AE40:
	cmp r1, r0
	beq _0805AE90
	b _0805AEBE
	.align 2, 0
_0805AE48: .4byte 0x00000437
_0805AE4C:
	ldr r1, _0805AE64 @ =0x020192E4
	ldrh r0, [r1]
	cmp r0, r8
	bhi _0805AEBE
	ldr r2, _0805AE68 @ =0x00000D64
	add r1, r1, r2
	ldr r0, _0805AE6C @ =0x000005DC
	ldrh r1, [r1]
	cmp r1, r0
	bls _0805AEBE
	b _0805AEC2
	.align 2, 0
_0805AE64: .4byte 0x020192E4
_0805AE68: .4byte 0x00000D64
_0805AE6C: .4byte 0x000005DC
_0805AE70:
	mov r0, #0
	mov r1, #0x16
	bl CountFaceUpSpellTrapsOfType
	b _0805AE8A
_0805AE7A:
	mov r0, #0
	mov r1, #0x15
	bl CountFaceUpSpellTrapsOfType
	b _0805AE8A
_0805AE84:
	mov r0, #0
	bl CountSpellTraps
_0805AE8A:
	cmp r0, #0
	ble _0805AEBE
	b _0805AEC2
_0805AE90:
	mov r0, #0
	bl CountSpellTraps
	cmp r0, #0
	ble _0805AEBE
	mov r0, #1
	bl CountSpellTraps
	cmp r0, #0
	bne _0805AEBE
	b _0805AEC2
_0805AEA6:
	ldr r2, _0805AEE8 @ =0x020192E4
	ldr r0, _0805AEEC @ =0x00000D64
	add r1, r2, r0
	mov r0, #0xFA
	lsl r0, r0, #2
	ldrh r1, [r1]
	cmp r1, r0
	bls _0805AEBE
	ldrb r0, [r2, #2]
	cmp r0, #0
	beq _0805AEBE
	mov r6, #1
_0805AEBE:
	cmp r6, #0
	beq _0805AF40
_0805AEC2:
	ldrb r0, [r4, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #2]
	ldrb r1, [r4, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r4, #3]
	ldrb r1, [r5, #6]
	lsl r0, r1, #1
	add r0, r0, r7
	ldrh r1, [r0]
	add r2, r1, #0
	ldr r0, _0805AEF0 @ =0x0000FFFF
	cmp r1, r0
	bne _0805AEF4
	mov r0, #0
	b _0805AF22
	.align 2, 0
_0805AEE8: .4byte 0x020192E4
_0805AEEC: .4byte 0x00000D64
_0805AEF0: .4byte 0x0000FFFF
_0805AEF4:
	cmp r1, r8
	bhi _0805AF10
	ldr r2, _0805AF08 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0805AF0C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0805AF22
_0805AF08: .4byte 0x000007FF
_0805AF0C: .4byte gCardNumberToId
_0805AF10:
	ldr r1, _0805AF54 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _0805AF58 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0805AF5C @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_0805AF22:
	strh r0, [r4]
	mov r0, sp
	mov r1, #0
	mov r2, #1
	bl CanActivateEffect
	cmp r0, #0
	beq _0805AF40
	ldrh r1, [r4]
	mov r0, #1
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	beq _0805AF40
	b _0805B160
_0805AF40:
	ldrb r0, [r5, #6]
	add r0, #1
	strb r0, [r5, #6]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x13
	bhi _0805AF50
	b _0805ADA4
_0805AF50:
	b _0805B170
	.align 2, 0
_0805AF54: .4byte 0xFFFFF830
_0805AF58: .4byte 0x000007FF
_0805AF5C: .4byte gCardNumberToId
_0805AF60:
	ldr r5, _0805AF90 @ =0x0000049C
	mov r0, #0
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0805AF7A
	mov r0, #1
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0805B014
_0805AF7A:
	ldr r0, _0805AF94 @ =0x08086448
	ldrb r4, [r4, #6]
	lsl r1, r4, #1
	add r1, r1, r0
	ldrh r2, [r1]
	ldr r1, _0805AF98 @ =0x0000FFFF
	add r7, r0, #0
	cmp r2, r1
	bne _0805AF9C
	mov r0, #0
	b _0805AFCA
_0805AF90: .4byte 0x0000049C
_0805AF94: .4byte gAiSimpleSpells
_0805AF98: .4byte 0x0000FFFF
_0805AF9C:
	ldr r0, _0805AFB0 @ =0x000007CF
	cmp r2, r0
	bhi _0805AFB8
	add r0, #0x30
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0805AFB4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0805AFCA
_0805AFB0: .4byte 0x000007CF
_0805AFB4: .4byte gCardNumberToId
_0805AFB8:
	ldr r1, _0805B000 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _0805B004 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0805B008 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_0805AFCA:
	lsl r5, r0, #0x10
	lsr r5, r5, #0x10
	ldr r0, _0805B00C @ =0x02015EF0
	ldrb r0, [r0, #6]
	lsl r0, r0, #1
	add r0, r0, r7
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #1
	bl AiFindHandCardByNumber
	add r4, r0, #0
	mov r0, #1
	bl FindFreeSpellTrapZone
	mov r1, #0xF
	and r4, r1
	lsl r4, r4, #4
	and r0, r1
	orr r4, r0
	ldr r0, _0805B010 @ =0x000080C5
	add r1, r5, #0
	add r2, r4, #0
	mov r3, #0
	bl DuelCmd_Push
	b _0805B118
_0805B000: .4byte 0xFFFFF830
_0805B004: .4byte 0x000007FF
_0805B008: .4byte gCardNumberToId
_0805B00C: .4byte 0x02015EF0
_0805B010: .4byte 0x000080C5
_0805B014:
	ldr r0, _0805B02C @ =0x08086448
	ldrb r4, [r4, #6]
	lsl r1, r4, #1
	add r1, r1, r0
	ldrh r2, [r1]
	ldr r1, _0805B030 @ =0x0000FFFF
	add r7, r0, #0
	cmp r2, r1
	bne _0805B034
	mov r0, #0
	b _0805B062
	.align 2, 0
_0805B02C: .4byte gAiSimpleSpells
_0805B030: .4byte 0x0000FFFF
_0805B034:
	ldr r0, _0805B048 @ =0x000007CF
	cmp r2, r0
	bhi _0805B050
	add r0, #0x30
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0805B04C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0805B062
_0805B048: .4byte 0x000007CF
_0805B04C: .4byte gCardNumberToId
_0805B050:
	ldr r1, _0805B0BC @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _0805B0C0 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0805B0C4 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_0805B062:
	lsl r5, r0, #0x10
	lsr r5, r5, #0x10
	ldr r6, _0805B0C8 @ =0x02015EF0
	ldrb r1, [r6, #6]
	lsl r0, r1, #1
	add r0, r0, r7
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #1
	bl AiFindHandCardByNumber
	add r4, r0, #0
	mov r0, #1
	bl FindFreeSpellTrapZone
	mov r1, #0xF
	and r4, r1
	lsl r4, r4, #4
	and r0, r1
	orr r4, r0
	mov r1, #0x80
	lsl r1, r1, #1
	add r0, r1, #0
	orr r4, r0
	ldr r0, _0805B0CC @ =0x000080C5
	add r1, r5, #0
	add r2, r4, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
	bl FindFreeSpellTrapZone
	add r3, r0, #0
	ldrb r6, [r6, #6]
	lsl r0, r6, #1
	add r0, r0, r7
	ldrh r1, [r0]
	add r2, r1, #0
	ldr r0, _0805B0D0 @ =0x0000FFFF
	cmp r1, r0
	bne _0805B0D4
	mov r1, #0
	b _0805B102
	.align 2, 0
_0805B0BC: .4byte 0xFFFFF830
_0805B0C0: .4byte 0x000007FF
_0805B0C4: .4byte gCardNumberToId
_0805B0C8: .4byte 0x02015EF0
_0805B0CC: .4byte 0x000080C5
_0805B0D0: .4byte 0x0000FFFF
_0805B0D4:
	ldr r0, _0805B0E8 @ =0x000007CF
	cmp r1, r0
	bhi _0805B0F0
	add r0, #0x30
	and r1, r0
	lsl r0, r1, #1
	ldr r2, _0805B0EC @ =0x08623DF4
	add r0, r0, r2
	ldrh r1, [r0]
	b _0805B102
_0805B0E8: .4byte 0x000007CF
_0805B0EC: .4byte gCardNumberToId
_0805B0F0:
	ldr r1, _0805B120 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _0805B124 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0805B128 @ =0x08623DF4
	add r0, r0, r2
	ldrh r1, [r0]
	add r1, #1
_0805B102:
	mov r0, #0x1F
	and r0, r3
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, _0805B12C @ =0x80200000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
_0805B118:
	ldr r1, _0805B130 @ =0x02015EF0
	mov r0, #0
	strb r0, [r1, #0xA]
	b _0805B172
_0805B120: .4byte 0xFFFFF830
_0805B124: .4byte 0x000007FF
_0805B128: .4byte gCardNumberToId
_0805B12C: .4byte 0x80200000
_0805B130: .4byte 0x02015EF0
_0805B134:
	ldr r0, _0805B158 @ =0x0000807F
	lsl r1, r5, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x1F
	and r5, r0
	lsl r0, r5, #0x10
	ldr r1, _0805B15C @ =0x80200000
	orr r4, r1
	orr r0, r4
	mov r1, #0
	bl Chain_AddPending
	mov r0, #0
	b _0805B172
_0805B158: .4byte 0x0000807F
_0805B15C: .4byte 0x80200000
_0805B160:
	ldr r1, _0805B16C @ =0x02015EF0
	ldrb r0, [r1, #0xA]
	add r0, #1
	strb r0, [r1, #0xA]
	mov r0, #0
	b _0805B172
_0805B16C: .4byte 0x02015EF0
_0805B170:
	mov r0, #1
_0805B172:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiStepPlaySimpleSpells
	.align 2, 0

