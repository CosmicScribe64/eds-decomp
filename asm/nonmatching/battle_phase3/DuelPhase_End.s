	thumb_func_start DuelPhase_End
DuelPhase_End: @ 0x0804E948
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r1, _0804E978 @ =0x020192E0
	ldr r2, _0804E97C @ =0x00001B12
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r7, r0, #0x1F
	mov r3, #0xD9
	lsl r3, r3, #5
	add r0, r1, r3
	ldrb r0, [r0]
	cmp r0, #0x16
	bls _0804E96E
	b _0804EF9C
_0804E96E:
	lsl r0, r0, #2
	ldr r1, _0804E980 @ =0x0804E984
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0804E978: .4byte 0x020192E0
_0804E97C: .4byte 0x00001B12
_0804E980: .4byte 0x0804E984
_0804E984:
	.4byte _0804E9E0
	.4byte _0804EA0C
	.4byte _0804EA58
	.4byte _0804EA6C
	.4byte _0804EB4C
	.4byte _0804EC94
	.4byte _0804ECD8
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804ED30
	.4byte _0804EE24
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EF9C
	.4byte _0804EE50
	.4byte _0804EEC0
	.4byte _0804EF2C
_0804E9E0:
	mov r0, #0x55
	cmp r7, #0
	beq _0804E9E8
	ldr r0, _0804EA44 @ =0x00008055
_0804E9E8:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, _0804EA48 @ =0x020192E0
	ldr r6, _0804EA4C @ =0x00001B22
	add r0, r1, r6
	mov r2, #0
	strb r2, [r0]
	ldr r3, _0804EA50 @ =0x00001B23
	add r0, r1, r3
	strb r2, [r0]
	sub r6, #2
	add r1, r1, r6
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0804EA0C:
	add r0, r7, #0
	bl EndPhase_TransferMushroomMan2
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804EA1A
	b _0804EC66
_0804EA1A:
	mov r0, #0x47
	cmp r7, #0
	beq _0804EA22
	ldr r0, _0804EA54 @ =0x00008047
_0804EA22:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r7, #0
	bl EndPhase_ReturnWickedWormBeast
	ldr r0, _0804EA48 @ =0x020192E0
	mov r1, #0xD9
	lsl r1, r1, #5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0804EC66
	.align 2, 0
_0804EA44: .4byte 0x00008055
_0804EA48: .4byte 0x020192E0
_0804EA4C: .4byte 0x00001B22
_0804EA50: .4byte 0x00001B23
_0804EA54: .4byte 0x00008047
_0804EA58:
	add r0, r7, #0
	bl EndPhase_DestroyLowLevelMonsters
	ldr r0, _0804EA68 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	b _0804EF7A
_0804EA68: .4byte 0x020192E0
_0804EA6C:
	mov r4, #0
	mov r3, #1
	mov r8, r3
	mov r6, r8
	sub r6, r6, r7
	str r6, [sp, #4]
	ldr r0, _0804EB38 @ =0x00000D64
	mov sl, r0
	mov r5, #0
	ldr r1, _0804EB3C @ =0x0201930C
	mov r9, r1
_0804EA82:
	ldr r0, [sp, #4]
	mov r2, r8
	and r0, r2
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	add r0, r5, r0
	mov r6, r9
	add r1, r0, r6
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804EB1A
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804EB1A
	add r0, r7, #0
	add r1, r4, #0
	ldr r2, _0804EB40 @ =0x0000060C
	bl FindZoneLinkFromCard
	add r1, r0, #0
	cmp r1, #0
	blt _0804EB1A
	add r0, r7, #0
	mov r2, r8
	and r0, r2
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	add r0, r5, r0
	add r0, r9
	lsl r1, r1, #1
	add r0, #0xA
	add r0, r0, r1
	ldrb r3, [r0]
	ldrh r0, [r0]
	lsr r2, r0, #8
	add r0, r3, #0
	mov r6, r8
	and r0, r6
	mov r1, sl
	mul r1, r0
	add r0, r1, #0
	add r0, r9
	mov r6, #0x94
	add r1, r2, #0
	mul r1, r6
	add r0, r0, r1
	mov r1, #0x3C
	ldrb r6, [r0, #6]
	and r1, r6
	cmp r1, #8
	bne _0804EB1A
	add r0, #0x91
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	bne _0804EB1A
	add r0, r3, #0
	add r1, r2, #0
	mov r2, #0
	bl ReturnFieldCardToHand
	ldr r0, [sp, #4]
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
	add r0, r7, #0
	ldr r1, [sp, #4]
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_0804EB1A:
	add r5, #0x94
	add r4, #1
	cmp r4, #4
	ble _0804EA82
	ldr r1, _0804EB44 @ =0x020192E0
	mov r0, #0xD9
	lsl r0, r0, #5
	add r2, r1, r0
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	ldr r2, _0804EB48 @ =0x00001B21
	add r1, r1, r2
	mov r0, #5
	b _0804EE42
_0804EB38: .4byte 0x00000D64
_0804EB3C: .4byte 0x0201930C
_0804EB40: .4byte 0x0000060C
_0804EB44: .4byte 0x020192E0
_0804EB48: .4byte 0x00001B21
_0804EB4C:
	ldr r3, _0804EBB0 @ =0x020192E0
	ldr r6, _0804EBB4 @ =0x00001B21
	add r2, r3, r6
	ldrb r0, [r2]
	cmp r0, #9
	bls _0804EB5A
	b _0804EC80
_0804EB5A:
	mov r0, #1
	sub r5, r0, r7
	add r1, r5, #0
	and r1, r0
	add r4, r2, #0
	add r3, #0x2C
	mov r9, r3
	mov r2, #0x94
	mov r8, r2
	ldr r0, _0804EBB8 @ =0x00000D64
	add r6, r1, #0
	mul r6, r0
_0804EB72:
	ldrb r1, [r4]
	mov r0, r8
	mul r0, r1
	add r0, r0, r6
	mov r2, r9
	add r3, r0, r2
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0804EC70
	ldrb r3, [r3, #6]
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _0804EC70
	ldr r0, _0804EBBC @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0804EBC0 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _0804EBC4 @ =0x000004CE
	cmp r2, r0
	beq _0804EBFE
	cmp r2, r0
	bgt _0804EBCC
	ldr r0, _0804EBC8 @ =0x0000015B
	cmp r2, r0
	beq _0804EBD6
	b _0804EC70
_0804EBB0: .4byte 0x020192E0
_0804EBB4: .4byte 0x00001B21
_0804EBB8: .4byte 0x00000D64
_0804EBBC: .4byte 0x000007FF
_0804EBC0: .4byte gCardIdToNumber
_0804EBC4: .4byte 0x000004CE
_0804EBC8: .4byte 0x0000015B
_0804EBCC:
	mov r0, #0xBF
	lsl r0, r0, #3
	cmp r2, r0
	beq _0804EC26
	b _0804EC70
_0804EBD6:
	lsl r0, r3, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #1
	bhi _0804EBF4
	mov r0, #0x8A
	cmp r7, #1
	beq _0804EBE6
	ldr r0, _0804EBF0 @ =0x0000808A
_0804EBE6:
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _0804EC60
_0804EBF0: .4byte 0x0000808A
_0804EBF4:
	add r0, r5, #0
	mov r2, #1
	bl DestroyFieldCard
	b _0804EC60
_0804EBFE:
	mov r0, #0x3C
	and r0, r3
	cmp r0, #0
	bne _0804EC1C
	mov r0, #0x8A
	cmp r7, #1
	beq _0804EC0E
	ldr r0, _0804EC18 @ =0x0000808A
_0804EC0E:
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _0804EC60
_0804EC18: .4byte 0x0000808A
_0804EC1C:
	add r0, r5, #0
	mov r2, #1
	bl DestroyFieldCard
	b _0804EC60
_0804EC26:
	add r0, r5, #0
	bl FindFreeSpellTrapZone
	cmp r0, #0
	blt _0804EC70
	ldrb r2, [r4]
	mov r0, r8
	mul r0, r2
	add r0, r0, r6
	add r0, r9
	add r0, #0x91
	mov r1, #8
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	bne _0804EC70
	mov r0, #0x8A
	cmp r7, #1
	beq _0804EC4E
	ldr r0, _0804EC6C @ =0x0000808A
_0804EC4E:
	add r1, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r4]
	add r0, r5, #0
	bl PlaceNextSpiritMessage
_0804EC60:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0804EC66:
	mov r0, #0
	b _0804EFD4
	.align 2, 0
_0804EC6C: .4byte 0x0000808A
_0804EC70:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bhi _0804EC80
	b _0804EB72
_0804EC80:
	ldr r3, _0804EC90 @ =0x020192E0
	mov r6, #0xD9
	lsl r6, r6, #5
	add r1, r3, r6
	ldrb r0, [r1]
	add r0, #1
	b _0804EE42
	.align 2, 0
_0804EC90: .4byte 0x020192E0
_0804EC94:
	mov r0, #1
	sub r4, r0, r7
	ldr r5, _0804ECC4 @ =0x000005EF
	add r0, r4, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0804ECC8
	add r0, r7, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	ble _0804ECC8
	add r0, r4, #0
	mov r1, #0xF
	add r2, r5, #0
	mov r3, #0
	bl DuelPrompt_Post
	b _0804EF72
	.align 2, 0
_0804ECC4: .4byte 0x000005EF
_0804ECC8:
	ldr r0, _0804ECD4 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	mov r1, #0xA
	b _0804EF7E
_0804ECD4: .4byte 0x020192E0
_0804ECD8:
	ldr r3, _0804ED20 @ =0x020192E0
	mov r8, r3
	ldr r0, _0804ED24 @ =0x00001B64
	add r0, r8
	ldrh r0, [r0]
	cmp r0, #0
	beq _0804ED18
	mov r6, #1
	sub r4, r6, r7
	ldr r5, _0804ED28 @ =0x000005EF
	add r0, r4, #0
	add r1, r5, #0
	bl FindFaceUpCardOnField2
	lsl r5, r5, #1
	ldr r1, _0804ED2C @ =0x08623DF4
	add r5, r5, r1
	and r4, r6
	lsl r4, r4, #0x1F
	mov r1, #0x1F
	and r1, r0
	lsl r1, r1, #0x10
	mov r0, #0xC8
	lsl r0, r0, #0x13
	orr r1, r0
	orr r4, r1
	ldrh r5, [r5]
	orr r4, r5
	add r0, r4, #0
	mov r1, #0
	bl Chain_AddPending
_0804ED18:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r8
	b _0804EE40
_0804ED20: .4byte 0x020192E0
_0804ED24: .4byte 0x00001B64
_0804ED28: .4byte 0x000005EF
_0804ED2C: .4byte gCardNumberToId
_0804ED30:
	ldr r2, _0804EDC8 @ =0x020192E0
	ldr r3, _0804EDCC @ =0x00001B21
	add r0, r2, r3
	mov r1, #0
	strb r1, [r0]
	mov r8, r0
	add r0, r7, #0
	mov r6, #1
	and r0, r6
	mov r1, #0x94
	mov r9, r1
	ldr r2, _0804EDD0 @ =0x00000D64
	mov sl, r2
	mov r3, sl
	mul r3, r0
	str r3, [sp, #0]
_0804ED50:
	mov r6, r8
	ldrb r1, [r6]
	add r0, r7, #0
	ldr r2, _0804EDD4 @ =0x0000060C
	bl CountZoneLinksFromCard
	cmp r0, #0
	beq _0804EE04
	ldrb r1, [r6]
	add r0, r7, #0
	ldr r2, _0804EDD4 @ =0x0000060C
	bl FindZoneLinkFromCard
	lsl r0, r0, #0x10
	ldrb r2, [r6]
	mov r1, r9
	mul r1, r2
	ldr r3, [sp, #0]
	add r1, r1, r3
	ldr r4, _0804EDC8 @ =0x020192E0
	add r4, #0x2C
	add r1, r1, r4
	lsr r0, r0, #0xF
	add r1, #0xA
	add r1, r1, r0
	ldrb r5, [r1]
	ldrh r1, [r1]
	lsr r6, r1, #8
	add r0, r5, #0
	mov r1, #1
	and r0, r1
	mov r1, r9
	mul r1, r6
	mov r2, sl
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	add r1, r1, r4
	add r2, r1, #0
	add r2, #0x91
	mov r0, #8
	ldrb r2, [r2]
	and r0, r2
	cmp r0, #0
	bne _0804EE04
	mov r0, #0x3C
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0804EDDC
	mov r0, #0x8A
	cmp r5, #0
	beq _0804EDBC
	ldr r0, _0804EDD8 @ =0x0000808A
_0804EDBC:
	add r1, r6, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _0804EE04
_0804EDC8: .4byte 0x020192E0
_0804EDCC: .4byte 0x00001B21
_0804EDD0: .4byte 0x00000D64
_0804EDD4: .4byte 0x0000060C
_0804EDD8: .4byte 0x0000808A
_0804EDDC:
	ldr r3, _0804EDFC @ =0x08624A0C
	ldrh r1, [r3]
	add r0, r7, #0
	bl ShowCardEffect
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0
	bl ReturnFieldCardToHand
	ldr r6, _0804EE00 @ =0x00001AF4
	add r1, r4, r6
	ldrb r0, [r1]
	add r0, #1
	b _0804EE42
	.align 2, 0
_0804EDFC: .4byte gCardNumberToId_1548
_0804EE00: .4byte 0x00001AF4
_0804EE04:
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #4
	bls _0804ED50
	ldr r2, _0804EE20 @ =0x020192E0
	mov r3, #0xD9
	lsl r3, r3, #5
	add r1, r2, r3
	mov r0, #0x14
	b _0804EE42
_0804EE20: .4byte 0x020192E0
_0804EE24:
	ldr r6, _0804EE48 @ =0x020192E0
	ldr r0, _0804EE4C @ =0x00001B21
	add r4, r6, r0
	ldrb r1, [r4]
	add r0, r7, #0
	mov r2, #1
	bl DestroyFieldCard
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r2, #0xD9
	lsl r2, r2, #5
	add r1, r6, r2
_0804EE40:
	mov r0, #0xA
_0804EE42:
	strb r0, [r1]
	b _0804EC66
	.align 2, 0
_0804EE48: .4byte 0x020192E0
_0804EE4C: .4byte 0x00001B21
_0804EE50:
	mov r4, #0
	ldr r3, _0804EEAC @ =0x020192E4
	add r1, r7, #0
	ldr r2, _0804EEB0 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	add r0, r0, r3
	ldrb r0, [r0, #4]
	cmp r4, r0
	bge _0804EEA0
	add r5, r1, #0
	add r6, r3, #0
_0804EE68:
	add r1, r5, #0
	mul r1, r2
	ldr r0, _0804EEB4 @ =0x02019BE8
	add r1, r1, r0
	lsl r0, r4, #2
	add r1, r1, r0
	ldr r2, [r1]
	lsl r0, r2, #8
	cmp r0, #0
	bge _0804EE90
	mov r0, #0xD2
	cmp r7, #0
	beq _0804EE84
	ldr r0, _0804EEB8 @ =0x000080D2
_0804EE84:
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	mov r3, #0
	bl DuelCmd_Push
_0804EE90:
	add r4, #1
	ldr r2, _0804EEB0 @ =0x00000D64
	add r0, r5, #0
	mul r0, r2
	add r0, r0, r6
	ldrb r0, [r0, #4]
	cmp r4, r0
	blt _0804EE68
_0804EEA0:
	ldr r0, _0804EEBC @ =0x020192E0
	mov r3, #0xD9
	lsl r3, r3, #5
	add r0, r0, r3
	b _0804EF7A
	.align 2, 0
_0804EEAC: .4byte 0x020192E4
_0804EEB0: .4byte 0x00000D64
_0804EEB4: .4byte 0x02019BE8
_0804EEB8: .4byte 0x000080D2
_0804EEBC: .4byte 0x020192E0
_0804EEC0:
	ldr r1, _0804EF14 @ =0x020192E4
	mov r5, #1
	add r4, r7, #0
	and r4, r5
	ldr r0, _0804EF18 @ =0x00000D64
	mul r0, r4
	add r0, r0, r1
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _0804EF08
	mov r0, #0x4C
	cmp r7, #0
	beq _0804EEDE
	ldr r0, _0804EF1C @ =0x0000804C
_0804EEDE:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	sub r0, r5, r7
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	ble _0804EF08
	ldr r2, _0804EF20 @ =0x086249C8
	lsl r0, r4, #0x1F
	ldr r1, _0804EF24 @ =0x26600000
	ldrh r2, [r2]
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
_0804EF08:
	ldr r0, _0804EF28 @ =0x020192E0
	mov r6, #0xD9
	lsl r6, r6, #5
	add r0, r0, r6
	b _0804EF7A
	.align 2, 0
_0804EF14: .4byte 0x020192E4
_0804EF18: .4byte 0x00000D64
_0804EF1C: .4byte 0x0000804C
_0804EF20: .4byte gCardNumberToId_1514
_0804EF24: .4byte 0x26600000
_0804EF28: .4byte 0x020192E0
_0804EF2C:
	ldr r1, _0804EF84 @ =0x020192E4
	mov r4, #1
	eor r4, r7
	ldr r0, _0804EF88 @ =0x00000D64
	mul r0, r4
	add r0, r0, r1
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _0804EF72
	mov r0, #0x4C
	cmp r7, #1
	beq _0804EF48
	ldr r0, _0804EF8C @ =0x0000804C
_0804EF48:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r7, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	ble _0804EF72
	ldr r2, _0804EF90 @ =0x086249C8
	lsl r0, r4, #0x1F
	ldr r1, _0804EF94 @ =0x26600000
	ldrh r2, [r2]
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
_0804EF72:
	ldr r0, _0804EF98 @ =0x020192E0
	mov r1, #0xD9
	lsl r1, r1, #5
	add r0, r0, r1
_0804EF7A:
	ldrb r1, [r0]
	add r1, #1
_0804EF7E:
	strb r1, [r0]
	b _0804EC66
	.align 2, 0
_0804EF84: .4byte 0x020192E4
_0804EF88: .4byte 0x00000D64
_0804EF8C: .4byte 0x0000804C
_0804EF90: .4byte gCardNumberToId_1514
_0804EF94: .4byte 0x26600000
_0804EF98: .4byte 0x020192E0
_0804EF9C:
	ldr r4, _0804EFE4 @ =0x00000593
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0804EFD2
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0804EFD2
	ldr r0, _0804EFE8 @ =0x020192E4
	ldr r1, _0804EFEC @ =0x00000D64
	mul r1, r7
	add r1, r1, r0
	ldrb r2, [r1, #2]
	cmp r2, #6
	bls _0804EFD2
	add r1, r2, #0
	sub r1, #6
	add r0, r7, #0
	mov r2, #0
	mov r3, #0
	bl DuelPrompt_PostDiscard
_0804EFD2:
	mov r0, #1
_0804EFD4:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0804EFE4: .4byte 0x00000593
_0804EFE8: .4byte 0x020192E4
_0804EFEC: .4byte 0x00000D64
	thumb_func_end DuelPhase_End

