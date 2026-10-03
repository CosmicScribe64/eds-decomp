	thumb_func_start BattleStage_EndBattlePhase
BattleStage_EndBattlePhase: @ 0x0804DC88
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r0
	ldr r0, _0804DCB4 @ =0x020192E0
	ldr r2, _0804DCB8 @ =0x00001B16
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r8, r0
	cmp r1, #8
	bls _0804DCA8
	b _0804E168
_0804DCA8:
	lsl r0, r1, #2
	ldr r1, _0804DCBC @ =0x0804DCC0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804DCB4: .4byte 0x020192E0
_0804DCB8: .4byte 0x00001B16
_0804DCBC: .4byte 0x0804DCC0
_0804DCC0:
	.4byte _0804DCE4
	.4byte _0804DDBC
	.4byte _0804DE84
	.4byte _0804E168
	.4byte _0804E168
	.4byte _0804DEDC
	.4byte _0804DF58
	.4byte _0804E020
	.4byte _0804E108
_0804DCE4:
	mov r7, #0
	mov r4, #1
	mov r5, r9
	and r5, r4
	mov sl, r5
_0804DCEE:
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _0804DD94 @ =0x00000D64
	mov r2, sl
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	ldr r0, _0804DD98 @ =0x0201930C
	add r3, r1, r0
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0804DD44
	ldr r4, _0804DD9C @ =0x000007FF
	add r0, r4, #0
	add r1, r2, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r5, _0804DDA0 @ =0x08622AB4
	add r1, r1, r5
	mov r0, #0xA8
	lsl r0, r0, #3
	ldrh r1, [r1]
	cmp r1, r0
	bne _0804DD44
	ldr r1, _0804DDA4 @ =0x00002003
	add r0, r1, #0
	ldrh r3, [r3, #6]
	and r0, r3
	cmp r0, #2
	bne _0804DD44
	mov r0, r9
	add r1, r2, #0
	bl ShowCardEffect
	mov r0, r9
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl ChangeBattlePosition
_0804DD44:
	mov r5, #0
	add r4, r7, #1
	ldr r2, _0804DDA8 @ =0x0000049E
	mov r8, r2
	ldr r6, _0804DDAC @ =0x08624730
_0804DD4E:
	add r0, r5, #0
	add r1, r7, #0
	mov r2, r8
	bl HasZoneCardEffectLink
	cmp r0, #0
	beq _0804DD6E
	ldrh r1, [r6]
	add r0, r5, #0
	bl ShowCardEffect
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #1
	bl DestroyFieldCard
_0804DD6E:
	add r5, #1
	cmp r5, #1
	ble _0804DD4E
	add r7, r4, #0
	cmp r7, #4
	ble _0804DCEE
	ldr r2, _0804DDB0 @ =0x020192E0
	ldr r4, _0804DDB4 @ =0x00001B16
	add r2, r2, r4
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804DDB8 @ =0xFFFFFE01
	and r0, r3
	b _0804E148
_0804DD94: .4byte 0x00000D64
_0804DD98: .4byte 0x0201930C
_0804DD9C: .4byte 0x000007FF
_0804DDA0: .4byte gCardIdToNumber
_0804DDA4: .4byte 0x00002003
_0804DDA8: .4byte 0x0000049E
_0804DDAC: .4byte gUnk_08624730
_0804DDB0: .4byte 0x020192E0
_0804DDB4: .4byte 0x00001B16
_0804DDB8: .4byte 0xFFFFFE01
_0804DDBC:
	mov r7, #0
	ldr r5, _0804DE54 @ =0x020192E4
	mov ip, r5
	ldr r0, _0804DE58 @ =0x0000052F
	mov sl, r0
	ldr r1, _0804DE5C @ =0x00000D64
	mov r9, r1
_0804DDCA:
	mov r5, #0
	mov r0, #1
	and r0, r7
	mov r1, r9
	mul r1, r0
	mov r2, ip
	add r0, r1, r2
	ldrb r0, [r0, #4]
	cmp r5, r0
	bge _0804DE7C
	mov r4, #1
	ldr r6, _0804DE60 @ =0x02019BE8
	ldr r2, _0804DE64 @ =0xFFFFF6FC
	add r0, r6, r2
	add r2, r1, #0
	add r3, r2, r0
_0804DDEA:
	add r0, r2, r6
	lsl r1, r5, #2
	add r0, r0, r1
	ldr r1, [r0]
	lsl r0, r1, #7
	cmp r0, #0
	bge _0804DE74
	sub r3, r4, r7
	add r2, r3, #0
	and r2, r4
	lsl r0, r1, #4
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r1, r0
	mov r0, r9
	mul r0, r2
	add r1, r1, r0
	ldr r4, _0804DE68 @ =0xFFFFF724
	add r0, r6, r4
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r6, #0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804DE46
	cmp r4, #0
	beq _0804DE46
	add r0, r3, #0
	bl FindFreeSpellTrapZone
	cmp r0, #0
	blt _0804DE46
	ldr r1, _0804DE6C @ =0x000007FF
	add r0, r1, #0
	and r4, r0
	lsl r0, r4, #1
	ldr r2, _0804DE70 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, sl
	bne _0804DE46
	mov r6, #1
_0804DE46:
	add r0, r7, #0
	add r1, r5, #0
	add r2, r6, #0
	bl ResolvePendingGraveyardEquip
_0804DE50:
	mov r0, #0
	b _0804E232
_0804DE54: .4byte 0x020192E4
_0804DE58: .4byte 0x0000052F
_0804DE5C: .4byte 0x00000D64
_0804DE60: .4byte 0x02019BE8
_0804DE64: .4byte 0xFFFFF6FC
_0804DE68: .4byte 0xFFFFF724
_0804DE6C: .4byte 0x000007FF
_0804DE70: .4byte gCardIdToNumber
_0804DE74:
	add r5, #1
	ldrb r0, [r3, #4]
	cmp r5, r0
	blt _0804DDEA
_0804DE7C:
	add r7, #1
	cmp r7, #1
	ble _0804DDCA
	b _0804DFEC
_0804DE84:
	mov r7, #0
	ldr r1, _0804DECC @ =0x020192E4
	mov r9, r1
	ldr r3, _0804DED0 @ =0x00000D64
_0804DE8C:
	mov r5, #0
	mov r1, #1
	and r1, r7
	add r0, r1, #0
	mul r0, r3
	add r0, r9
	ldrb r0, [r0, #4]
	cmp r5, r0
	bge _0804DEC2
	mov r6, #1
	ldr r0, _0804DED0 @ =0x00000D64
	mul r1, r0
	ldr r4, _0804DED4 @ =0x02019BE8
	ldr r2, _0804DED8 @ =0xFFFFF6FC
	add r0, r4, r2
	add r0, r1, r0
	ldrb r2, [r0, #4]
	add r1, r1, r4
_0804DEB0:
	ldr r0, [r1]
	lsl r0, r0, #3
	cmp r0, #0
	bge _0804DEBA
	b _0804E1C8
_0804DEBA:
	add r1, #4
	add r5, #1
	cmp r5, r2
	blt _0804DEB0
_0804DEC2:
	add r7, #1
	cmp r7, #1
	ble _0804DE8C
	b _0804DFEC
	.align 2, 0
_0804DECC: .4byte 0x020192E4
_0804DED0: .4byte 0x00000D64
_0804DED4: .4byte 0x02019BE8
_0804DED8: .4byte 0xFFFFF6FC
_0804DEDC:
	ldr r6, _0804DF34 @ =0x00001B17
	add r6, r8
	ldrb r4, [r6]
	lsr r1, r4, #1
	ldr r5, _0804DF38 @ =0x00001B18
	add r5, r8
	mov r4, #1
	add r0, r4, #0
	ldrb r7, [r5]
	and r0, r7
	lsl r0, r0, #7
	orr r0, r1
	sub r0, r4, r0
	bl FindFreeMonsterZone
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0804DF48
	ldrb r6, [r6]
	lsr r1, r6, #1
	add r0, r4, #0
	ldrb r2, [r5]
	and r0, r2
	lsl r0, r0, #7
	orr r0, r1
	mov r2, #0xDA
	cmp r0, #0
	beq _0804DF18
	ldr r2, _0804DF3C @ =0x000080DA
_0804DF18:
	ldrh r5, [r5]
	lsl r1, r5, #0x17
	lsr r1, r1, #0x18
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldr r2, _0804DF40 @ =0x00001B16
	add r2, r8
	ldr r0, _0804DF44 @ =0xFFFFFE01
	ldrh r4, [r2]
	and r0, r4
	b _0804E146
_0804DF34: .4byte 0x00001B17
_0804DF38: .4byte 0x00001B18
_0804DF3C: .4byte 0x000080DA
_0804DF40: .4byte 0x00001B16
_0804DF44: .4byte 0xFFFFFE01
_0804DF48:
	ldr r0, _0804DF54 @ =0x0862486C
	ldrh r1, [r0]
	mov r0, r9
	bl ShowCardEffect
	b _0804DFEC
_0804DF54: .4byte gUnk_0862486C
_0804DF58:
	ldr r0, _0804DF94 @ =0x00001B17
	add r0, r8
	ldrb r0, [r0]
	lsr r2, r0, #1
	ldr r1, _0804DF98 @ =0x00001B18
	add r1, r8
	mov r3, #1
	add r0, r3, #0
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #7
	orr r0, r2
	cmp r0, #0
	beq _0804DFAC
	ldr r1, _0804DF9C @ =0x02015EE8
	add r0, r3, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	bne _0804DFAC
	ldr r0, _0804DFA0 @ =0x00001B64
	add r0, r8
	strh r3, [r0]
	ldr r2, _0804DFA4 @ =0x00001B16
	add r2, r8
	ldr r0, _0804DFA8 @ =0xFFFFFE01
	ldrh r5, [r2]
	and r0, r5
	mov r1, #0x10
	b _0804E148
_0804DF94: .4byte 0x00001B17
_0804DF98: .4byte 0x00001B18
_0804DF9C: .4byte 0x02015EE8
_0804DFA0: .4byte 0x00001B64
_0804DFA4: .4byte 0x00001B16
_0804DFA8: .4byte 0xFFFFFE01
_0804DFAC:
	ldr r0, _0804E008 @ =0x00001B17
	add r0, r8
	ldrb r0, [r0]
	lsr r1, r0, #1
	ldr r3, _0804E00C @ =0x00001B18
	add r3, r8
	mov r0, #1
	add r2, r0, #0
	ldrb r7, [r3]
	and r2, r7
	lsl r2, r2, #7
	orr r2, r1
	sub r0, r0, r2
	mov r1, #1
	and r2, r1
	ldrh r3, [r3]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	lsl r1, r1, #2
	ldr r3, _0804E010 @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _0804E014 @ =0x00000908
	add r2, r8
	add r1, r1, r2
	ldr r2, [r1]
	lsl r2, r2, #0x14
	lsr r2, r2, #0x14
	mov r1, #0x12
	mov r3, #0
	bl DuelPrompt_Post
_0804DFEC:
	ldr r3, _0804E018 @ =0x00001B16
	add r3, r8
	ldrh r2, [r3]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804E01C @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _0804DE50
_0804E008: .4byte 0x00001B17
_0804E00C: .4byte 0x00001B18
_0804E010: .4byte 0x00000D64
_0804E014: .4byte 0x00000908
_0804E018: .4byte 0x00001B16
_0804E01C: .4byte 0xFFFFFE01
_0804E020:
	ldr r0, _0804E094 @ =0x00001B64
	add r0, r8
	ldrh r0, [r0]
	cmp r0, #0
	beq _0804E0B8
	ldr r0, _0804E098 @ =0x00001B17
	add r0, r8
	ldrb r0, [r0]
	lsr r0, r0, #1
	ldr r3, _0804E09C @ =0x00001B18
	add r3, r8
	mov r2, #1
	ldrb r1, [r3]
	and r2, r1
	lsl r2, r2, #7
	orr r2, r0
	mov r1, #1
	add r0, r2, #0
	and r0, r1
	ldr r1, _0804E0A0 @ =0x00000D64
	mul r1, r0
	ldr r0, _0804E0A4 @ =0x00000908
	add r0, r8
	add r1, r1, r0
	ldrh r3, [r3]
	lsl r0, r3, #0x17
	lsr r0, r0, #0x18
	lsl r0, r0, #2
	add r4, r1, r0
	mov r0, #0xD3
	cmp r2, #0
	beq _0804E062
	ldr r0, _0804E0A8 @ =0x000080D3
_0804E062:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _0804E0AC @ =0x00001B1C
	add r0, r8
	add r1, r4, #0
	bl CopyDuelCard
	ldr r3, _0804E0B0 @ =0x00001B16
	add r3, r8
	ldrh r2, [r3]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804E0B4 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _0804DE50
	.align 2, 0
_0804E094: .4byte 0x00001B64
_0804E098: .4byte 0x00001B17
_0804E09C: .4byte 0x00001B18
_0804E0A0: .4byte 0x00000D64
_0804E0A4: .4byte 0x00000908
_0804E0A8: .4byte 0x000080D3
_0804E0AC: .4byte 0x00001B1C
_0804E0B0: .4byte 0x00001B16
_0804E0B4: .4byte 0xFFFFFE01
_0804E0B8:
	ldr r0, _0804E0F4 @ =0x00001B17
	add r0, r8
	ldrb r0, [r0]
	lsr r1, r0, #1
	ldr r3, _0804E0F8 @ =0x00001B18
	add r3, r8
	mov r0, #1
	ldrb r2, [r3]
	and r0, r2
	lsl r0, r0, #7
	orr r0, r1
	mov r2, #0xDA
	cmp r0, #0
	beq _0804E0D6
	ldr r2, _0804E0FC @ =0x000080DA
_0804E0D6:
	ldrh r3, [r3]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldr r2, _0804E100 @ =0x00001B16
	add r2, r8
	ldr r0, _0804E104 @ =0xFFFFFE01
	ldrh r4, [r2]
	and r0, r4
	b _0804E146
	.align 2, 0
_0804E0F4: .4byte 0x00001B17
_0804E0F8: .4byte 0x00001B18
_0804E0FC: .4byte 0x000080DA
_0804E100: .4byte 0x00001B16
_0804E104: .4byte 0xFFFFFE01
_0804E108:
	ldr r1, _0804E150 @ =0x00001B1F
	add r1, r8
	mov r0, #0x11
	neg r0, r0
	ldrb r5, [r1]
	and r0, r5
	strb r0, [r1]
	ldr r0, _0804E154 @ =0x00001B17
	add r0, r8
	ldrb r0, [r0]
	lsr r3, r0, #1
	ldr r2, _0804E158 @ =0x00001B18
	add r2, r8
	mov r0, #1
	add r1, r0, #0
	ldrb r2, [r2]
	and r1, r2
	lsl r1, r1, #7
	orr r1, r3
	sub r0, r0, r1
	ldr r1, _0804E15C @ =0x00001B1C
	add r1, r8
	mov r2, #1
	mov r3, #0x20
	bl QueueSpecialSummonChoosePosition
	ldr r2, _0804E160 @ =0x00001B16
	add r2, r8
	ldr r0, _0804E164 @ =0xFFFFFE01
	ldrh r7, [r2]
	and r0, r7
_0804E146:
	mov r1, #4
_0804E148:
	orr r0, r1
	strh r0, [r2]
	b _0804DE50
	.align 2, 0
_0804E150: .4byte 0x00001B1F
_0804E154: .4byte 0x00001B17
_0804E158: .4byte 0x00001B18
_0804E15C: .4byte 0x00001B1C
_0804E160: .4byte 0x00001B16
_0804E164: .4byte 0xFFFFFE01
_0804E168:
	ldr r4, _0804E1AC @ =0x020192E4
	mov r0, #1
	mov r1, r9
	and r0, r1
	ldr r1, _0804E1B0 @ =0x00000D64
	mul r0, r1
	add r1, r0, r4
	ldrb r2, [r1, #8]
	lsl r0, r2, #0x19
	cmp r0, #0
	bge _0804E228
	mov r0, #0x47
	mov r5, r9
	cmp r5, #0
	beq _0804E188
	ldr r0, _0804E1B4 @ =0x00008047
_0804E188:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r7, _0804E1B8 @ =0x00001B10
	add r2, r4, r7
	ldr r0, [r2]
	ldr r1, _0804E1BC @ =0xFFFE01FF
	and r0, r1
	str r0, [r2]
	ldr r0, _0804E1C0 @ =0x00001B12
	add r1, r4, r0
	ldr r0, _0804E1C4 @ =0xFFFFFE01
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	b _0804DE50
_0804E1AC: .4byte 0x020192E4
_0804E1B0: .4byte 0x00000D64
_0804E1B4: .4byte 0x00008047
_0804E1B8: .4byte 0x00001B10
_0804E1BC: .4byte 0xFFFE01FF
_0804E1C0: .4byte 0x00001B12
_0804E1C4: .4byte 0xFFFFFE01
_0804E1C8:
	lsl r2, r7, #0x10
	lsr r1, r2, #0x10
	mov r0, #0x7F
	and r1, r0
	ldr r7, _0804E218 @ =0x0000120F
	add r3, r4, r7
	lsl r1, r1, #1
	add r0, r6, #0
	ldrb r7, [r3]
	and r0, r7
	orr r0, r1
	strb r0, [r3]
	lsr r2, r2, #0x17
	ldr r0, _0804E21C @ =0x00001210
	add r3, r4, r0
	and r2, r6
	mov r0, #2
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	orr r0, r2
	strb r0, [r3]
	mov r0, #0xFF
	and r5, r0
	lsl r2, r5, #1
	ldr r1, _0804E220 @ =0xFFFFFE01
	add r0, r1, #0
	ldrh r5, [r3]
	and r0, r5
	orr r0, r2
	strh r0, [r3]
	ldr r7, _0804E224 @ =0x0000120E
	add r2, r4, r7
	ldrh r0, [r2]
	and r1, r0
	mov r0, #0xA
	orr r1, r0
	strh r1, [r2]
	b _0804DE50
	.align 2, 0
_0804E218: .4byte 0x0000120F
_0804E21C: .4byte 0x00001210
_0804E220: .4byte 0xFFFFFE01
_0804E224: .4byte 0x0000120E
_0804E228:
	mov r0, #0x10
	ldrb r2, [r1, #9]
	orr r0, r2
	strb r0, [r1, #9]
	mov r0, #1
_0804E232:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end BattleStage_EndBattlePhase

