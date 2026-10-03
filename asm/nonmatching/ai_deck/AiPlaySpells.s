	thumb_func_start AiPlaySpells
AiPlaySpells: @ 0x08059B14
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x14
	mov r4, #1
	ldr r5, _08059B44 @ =0x02015EF0
	ldrb r2, [r5, #6]
	add r0, r2, #0
	cmp r0, #5
	bne _08059B2A
	b _0805A104
_08059B2A:
	cmp r0, #5
	bgt _08059B56
	cmp r0, #2
	bne _08059B34
	b _08059CDC
_08059B34:
	cmp r0, #2
	bgt _08059B48
	cmp r0, #0
	beq _08059B7C
	cmp r0, #1
	beq _08059B98
	b _0805A2FC
	.align 2, 0
_08059B44: .4byte 0x02015EF0
_08059B48:
	cmp r0, #3
	bne _08059B4E
	b _08059F1C
_08059B4E:
	cmp r0, #4
	bne _08059B54
	b _08059FF4
_08059B54:
	b _0805A2FC
_08059B56:
	cmp r0, #0x66
	bne _08059B5C
	b _0805A1E0
_08059B5C:
	cmp r0, #0x66
	bgt _08059B6E
	cmp r0, #0x64
	bne _08059B66
	b _0805A174
_08059B66:
	cmp r0, #0x65
	bne _08059B6C
	b _0805A1B8
_08059B6C:
	b _0805A2FC
_08059B6E:
	cmp r0, #0xC8
	bne _08059B74
	b _0805A228
_08059B74:
	cmp r0, #0xC9
	bne _08059B7A
	b _0805A27A
_08059B7A:
	b _0805A2FC
_08059B7C:
	ldr r0, _08059B90 @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _08059B94
	mov r0, #0x64
	b _0805A1D6
	.align 2, 0
_08059B90: .4byte 0x02015EE8
_08059B94:
	add r0, r2, #1
	b _0805A1D6
_08059B98:
	ldr r0, _08059CC0 @ =0x000003F2
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059BA6
	b _0805A0E8
_08059BA6:
	mov r0, #0x82
	lsl r0, r0, #3
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059BB6
	b _0805A0E8
_08059BB6:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _08059BD8
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	bne _08059BD8
	ldr r0, _08059CC4 @ =0x0000014F
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059BD8
	b _0805A0E8
_08059BD8:
	bl AiIsOpponentThreatening
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059BF0
	ldr r0, _08059CC4 @ =0x0000014F
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059BF0
	b _0805A0E8
_08059BF0:
	mov r0, #1
	mov r1, #0x2F
	bl CountMonstersByNumber
	cmp r0, #0
	bgt _08059C08
	ldr r1, _08059CC8 @ =0x0000023D
	mov r0, #1
	bl CountMonstersByNumber
	cmp r0, #0
	ble _08059C16
_08059C08:
	ldr r0, _08059CC4 @ =0x0000014F
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059C16
	b _0805A0E8
_08059C16:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _08059C3A
	ldr r0, _08059CCC @ =0x000003FB
	bl AiRiskSetCounter
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059C3A
	ldr r0, _08059CC4 @ =0x0000014F
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059C3A
	b _0805A0E8
_08059C3A:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _08059C5E
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	bne _08059C5E
	mov r0, #0xA8
	lsl r0, r0, #1
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059C5E
	b _0805A0E8
_08059C5E:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _08059C84
	ldr r0, _08059CD0 @ =0x000003FE
	bl AiRiskSetCounter
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059C84
	mov r0, #0xA8
	lsl r0, r0, #1
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059C84
	b _0805A0E8
_08059C84:
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	ble _08059CAA
	mov r0, #1
	bl CountHandMonsters
	cmp r0, #0
	ble _08059CAA
	ldr r0, _08059CD4 @ =0x000003FF
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059CAA
	b _0805A0E8
_08059CAA:
	mov r0, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	bgt _08059CBA
	b _0805A1A0
_08059CBA:
	ldr r0, _08059CD8 @ =0x000003E9
	b _0805A196
	.align 2, 0
_08059CC0: .4byte 0x000003F2
_08059CC4: .4byte 0x0000014F
_08059CC8: .4byte 0x0000023D
_08059CCC: .4byte 0x000003FB
_08059CD0: .4byte 0x000003FE
_08059CD4: .4byte 0x000003FF
_08059CD8: .4byte 0x000003E9
_08059CDC:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	bgt _08059CE8
	b _08059E96
_08059CE8:
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	bne _08059DDE
	mov r6, #0
	ldr r0, _08059D4C @ =0x020192E4
	ldr r1, _08059D50 @ =0x00000D66
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r6, r0
	bge _08059DDE
	ldr r3, _08059D54 @ =0x000007FF
	add r7, r3, #0
	mov r0, #0xF8
	lsl r0, r0, #0x11
	mov r8, r0
_08059D0A:
	lsl r0, r6, #2
	ldr r1, _08059D58 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r4, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08059D5C @ =0x08621DE0
	add r5, r0, r1
	ldr r0, [r5]
	mov r3, r8
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08059DD0
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08059DD0
	ldr r0, [r5]
	mov r1, r8
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08059D68
	cmp r0, #0x17
	ble _08059D60
	cmp r0, #0x18
	beq _08059D64
	b _08059D68
_08059D4C: .4byte 0x020192E4
_08059D50: .4byte 0x00000D66
_08059D54: .4byte 0x000007FF
_08059D58: .4byte 0x0201A6CC
_08059D5C: .4byte gCardStats
_08059D60:
	mov r0, #0
	b _08059D7C
_08059D64:
	mov r0, #0xA
	b _08059D7C
_08059D68:
	add r0, r4, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r3, _08059DA0 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08059D7C:
	cmp r0, #4
	bls _08059DD0
	add r0, r4, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08059DA0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r3, r8
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08059DAC
	cmp r0, #0x17
	ble _08059DA4
	cmp r0, #0x18
	beq _08059DA8
	b _08059DAC
_08059DA0: .4byte gCardStats
_08059DA4:
	mov r0, #0
	b _08059DBE
_08059DA8:
	mov r0, #0xA
	b _08059DBE
_08059DAC:
	and r4, r7
	lsl r0, r4, #2
	ldr r1, _08059E44 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08059DBE:
	cmp r0, #6
	bhi _08059DD0
	ldr r0, _08059E48 @ =0x00000403
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059DD0
	b _0805A0E8
_08059DD0:
	add r6, #1
	ldr r0, _08059E4C @ =0x020192E4
	ldr r3, _08059E50 @ =0x00000D66
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r6, r0
	blt _08059D0A
_08059DDE:
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	ble _08059E96
	mov r6, #0
	ldr r0, _08059E4C @ =0x020192E4
	ldr r1, _08059E50 @ =0x00000D66
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r6, r0
	bge _08059E96
	mov r3, #0xF8
	lsl r3, r3, #0x11
	mov r8, r3
	ldr r0, _08059E54 @ =0x000007FF
	add r7, r0, #0
_08059E00:
	lsl r0, r6, #2
	ldr r1, _08059E58 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r4, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08059E44 @ =0x08621DE0
	add r5, r0, r1
	ldr r0, [r5]
	mov r3, r8
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08059E88
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08059E88
	ldr r0, [r5]
	mov r1, r8
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08059E64
	cmp r0, #0x17
	ble _08059E5C
	cmp r0, #0x18
	beq _08059E60
	b _08059E64
	.align 2, 0
_08059E44: .4byte gCardStats
_08059E48: .4byte 0x00000403
_08059E4C: .4byte 0x020192E4
_08059E50: .4byte 0x00000D66
_08059E54: .4byte 0x000007FF
_08059E58: .4byte 0x0201A6CC
_08059E5C:
	mov r0, #0
	b _08059E76
_08059E60:
	mov r0, #0xA
	b _08059E76
_08059E64:
	and r4, r7
	lsl r0, r4, #2
	ldr r3, _08059F00 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08059E76:
	cmp r0, #4
	bls _08059E88
	ldr r0, _08059F04 @ =0x00000403
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059E88
	b _0805A0E8
_08059E88:
	add r6, #1
	ldr r0, _08059F08 @ =0x020192E4
	ldr r1, _08059F0C @ =0x00000D66
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r6, r0
	blt _08059E00
_08059E96:
	mov r4, #0xFC
	lsl r4, r4, #2
	ldr r0, _08059F10 @ =0x086245D4
	ldrh r1, [r0]
	mov r0, sp
	strh r1, [r0]
	mov r2, sp
	ldrb r0, [r2, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r2, #2]
	mov r0, sp
	mov r1, #0
	mov r2, #1
	bl EffectMonsterRebornPrepare
	cmp r0, #0
	beq _08059ED4
	ldr r0, _08059F14 @ =0x00000402
	bl AiRiskSetCounter
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059ED4
	add r0, r4, #0
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059ED4
	b _0805A0E8
_08059ED4:
	mov r4, #0x91
	lsl r4, r4, #3
	ldr r0, _08059F18 @ =0x08624704
	ldrh r1, [r0]
	mov r0, sp
	strh r1, [r0]
	mov r2, sp
	ldrb r0, [r2, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r2, #2]
	mov r0, sp
	mov r1, #0
	mov r2, #1
	bl EffectPrematureBurialPrepare
	cmp r0, #0
	bne _08059EFA
	b _0805A1A0
_08059EFA:
	add r0, r4, #0
	b _0805A196
	.align 2, 0
_08059F00: .4byte gCardStats
_08059F04: .4byte 0x00000403
_08059F08: .4byte 0x020192E4
_08059F0C: .4byte 0x00000D66
_08059F10: .4byte gUnk_086245D4
_08059F14: .4byte 0x00000402
_08059F18: .4byte gUnk_08624704
_08059F1C:
	ldr r0, _08059FAC @ =0x02015EE8
	ldr r0, [r0, #4]
	and r0, r4
	cmp r0, #0
	beq _08059FEE
	mov r1, #0xFC
	lsl r1, r1, #2
	mov r0, #1
	bl FindHandCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _08059F48
	mov r0, #0xF2
	lsl r0, r0, #2
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059F48
	b _0805A0E8
_08059F48:
	mov r1, #0x91
	lsl r1, r1, #3
	mov r0, #1
	bl FindHandCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _08059F6A
	mov r0, #0xF2
	lsl r0, r0, #2
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059F6A
	b _0805A0E8
_08059F6A:
	mov r4, #0
	ldr r0, _08059FB0 @ =0x020192E4
	ldr r3, _08059FB4 @ =0x00000D67
	add r0, r0, r3
	ldrb r6, [r0]
	cmp r4, r6
	blt _08059F7A
	b _0805A1A0
_08059F7A:
	add r5, r0, #0
_08059F7C:
	lsl r0, r4, #2
	ldr r1, _08059FB8 @ =0x0201A80C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08059FBC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xF2
	lsl r0, r0, #2
	cmp r1, r0
	beq _08059FCE
	cmp r1, r0
	bgt _08059FC0
	mov r0, #0xA8
	lsl r0, r0, #1
	cmp r1, r0
	bgt _08059FDE
	sub r0, #1
	cmp r1, r0
	blt _08059FDE
	b _08059FCE
	.align 2, 0
_08059FAC: .4byte 0x02015EE8
_08059FB0: .4byte 0x020192E4
_08059FB4: .4byte 0x00000D67
_08059FB8: .4byte 0x0201A80C
_08059FBC: .4byte gCardIdToNumber
_08059FC0:
	mov r0, #0xFC
	lsl r0, r0, #2
	cmp r1, r0
	beq _08059FCE
	add r0, #0x98
	cmp r1, r0
	bne _08059FDE
_08059FCE:
	mov r0, #0xF2
	lsl r0, r0, #2
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08059FDE
	b _0805A0E8
_08059FDE:
	add r4, #1
	cmp r4, #2
	ble _08059FE6
	b _0805A1A0
_08059FE6:
	ldrb r3, [r5]
	cmp r4, r3
	blt _08059F7C
	b _0805A1A0
_08059FEE:
	mov r0, #0xF2
	lsl r0, r0, #2
	b _0805A196
_08059FF4:
	mov r4, #0
	ldr r1, _0805A0EC @ =0x0000040E
	mov r2, #1
	neg r2, r2
	mov r0, #1
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	ble _0805A008
	mov r4, #1
_0805A008:
	mov r0, #0
	mov r1, #0x10
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A016
	mov r4, #1
_0805A016:
	mov r0, #0
	mov r1, #0x11
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A024
	mov r4, #1
_0805A024:
	mov r0, #0
	mov r1, #0x12
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A032
	mov r4, #1
_0805A032:
	mov r0, #0
	mov r1, #0x13
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A040
	mov r4, #1
_0805A040:
	mov r0, #0
	mov r1, #0x14
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A04E
	mov r4, #1
_0805A04E:
	ldr r1, _0805A0F0 @ =0x0000014F
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A05C
	mov r4, #1
_0805A05C:
	mov r1, #0xA8
	lsl r1, r1, #1
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A06C
	mov r4, #1
_0805A06C:
	mov r1, #0xA4
	lsl r1, r1, #2
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A07C
	mov r4, #1
_0805A07C:
	mov r1, #0xFC
	lsl r1, r1, #2
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A08C
	mov r4, #1
_0805A08C:
	ldr r1, _0805A0F4 @ =0x00000403
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A09A
	mov r4, #1
_0805A09A:
	mov r1, #0x84
	lsl r1, r1, #3
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A0AA
	mov r4, #1
_0805A0AA:
	ldr r1, _0805A0F8 @ =0x0000042C
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A0B8
	mov r4, #1
_0805A0B8:
	mov r1, #0x91
	lsl r1, r1, #3
	mov r0, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _0805A0C8
	mov r4, #1
_0805A0C8:
	cmp r4, #0
	beq _0805A0D8
	ldr r0, _0805A0FC @ =0x000004C5
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805A0E8
_0805A0D8:
	ldr r0, _0805A100 @ =0x02015EF0
	ldrb r2, [r0, #6]
	add r2, #1
	mov r1, #0
	strb r2, [r0, #6]
	strb r1, [r0, #7]
	strb r1, [r0, #8]
	strb r1, [r0, #9]
_0805A0E8:
	mov r0, #0
	b _0805A2FE
_0805A0EC: .4byte 0x0000040E
_0805A0F0: .4byte 0x0000014F
_0805A0F4: .4byte 0x00000403
_0805A0F8: .4byte 0x0000042C
_0805A0FC: .4byte 0x000004C5
_0805A100: .4byte 0x02015EF0
_0805A104:
	mov r0, #0
	strb r0, [r5, #7]
	ldr r6, _0805A168 @ =0x08086394
_0805A10A:
	ldrb r1, [r5, #7]
	lsl r0, r1, #1
	add r0, r0, r6
	ldrh r0, [r0]
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #0
	bne _0805A0E8
	ldrb r0, [r5, #7]
	add r0, #1
	strb r0, [r5, #7]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x35
	bls _0805A10A
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	bgt _0805A13C
	b _0805A2FC
_0805A13C:
	ldr r0, _0805A16C @ =0x02015EF0
	strb r4, [r0, #7]
	ldr r5, _0805A170 @ =0x08086400
	add r4, r0, #0
_0805A144:
	ldrb r3, [r4, #7]
	lsl r0, r3, #1
	add r0, r0, r5
	ldrh r0, [r0]
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805A0E8
	ldrb r0, [r4, #7]
	add r0, #1
	strb r0, [r4, #7]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x22
	bls _0805A144
	b _0805A2FC
	.align 2, 0
_0805A168: .4byte gAiGenericSpells
_0805A16C: .4byte 0x02015EF0
_0805A170: .4byte gAiEquipSpells
_0805A174:
	bl AiCountExodiaOnField
	cmp r0, #0
	bne _0805A1A0
	mov r0, #1
	mov r1, #0x2F
	bl CountMonstersByNumber
	cmp r0, #0
	bgt _0805A194
	ldr r1, _0805A1AC @ =0x0000023D
	mov r0, #1
	bl CountMonstersByNumber
	cmp r0, #0
	ble _0805A1A0
_0805A194:
	ldr r0, _0805A1B0 @ =0x0000014F
_0805A196:
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805A0E8
_0805A1A0:
	ldr r1, _0805A1B4 @ =0x02015EF0
	ldrb r0, [r1, #6]
	add r0, #1
	strb r0, [r1, #6]
	b _0805A0E8
	.align 2, 0
_0805A1AC: .4byte 0x0000023D
_0805A1B0: .4byte 0x0000014F
_0805A1B4: .4byte 0x02015EF0
_0805A1B8:
	mov r0, #0xF2
	lsl r0, r0, #2
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805A0E8
	ldr r0, _0805A1DC @ =0x000003F2
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805A0E8
	ldrb r0, [r5, #6]
	add r0, #1
_0805A1D6:
	strb r0, [r5, #6]
	b _0805A0E8
	.align 2, 0
_0805A1DC: .4byte 0x000003F2
_0805A1E0:
	bl AiCountExodiaOnField
	cmp r0, #0
	bne _0805A204
	mov r0, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	bgt _0805A1F8
	b _0805A2FC
_0805A1F8:
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	beq _0805A204
	b _0805A2FC
_0805A204:
	ldr r4, _0805A224 @ =0x0000015B
	mov r2, #1
	neg r2, r2
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	bne _0805A2FC
	add r0, r4, #0
	bl AiTryPlaySpellTrap
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0805A2FC
	b _0805A0E8
_0805A224: .4byte 0x0000015B
_0805A228:
	ldr r2, _0805A2A0 @ =0x020192E0
	ldr r6, _0805A2A4 @ =0x00001B30
	add r1, r2, r6
	ldr r0, _0805A2A8 @ =0xFFFFFC03
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldrb r6, [r5, #0xB]
	lsl r0, r6, #2
	ldr r3, _0805A2AC @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805A2B0 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805A2B4 @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805A2B8 @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805A2BC @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805A2C0 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	ldrb r0, [r5, #6]
	add r0, #1
	strb r0, [r5, #6]
_0805A27A:
	ldr r4, _0805A2C4 @ =0x0000049C
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0805A294
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0805A2C8
_0805A294:
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	b _0805A2D2
_0805A2A0: .4byte 0x020192E0
_0805A2A4: .4byte 0x00001B30
_0805A2A8: .4byte 0xFFFFFC03
_0805A2AC: .4byte 0x000013EC
_0805A2B0: .4byte 0x00001B28
_0805A2B4: .4byte 0x00001B33
_0805A2B8: .4byte 0x00001B34
_0805A2BC: .4byte 0xFFFFFE01
_0805A2C0: .4byte 0x00001B2C
_0805A2C4: .4byte 0x0000049C
_0805A2C8:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
_0805A2D2:
	ldr r1, _0805A2F0 @ =0x020192E0
	ldr r3, _0805A2F4 @ =0x00001B2C
	add r1, r1, r3
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #0
	beq _0805A2E8
	b _0805A0E8
_0805A2E8:
	ldr r0, _0805A2F8 @ =0x02015EF0
	strb r1, [r0, #0xA]
	b _0805A0E8
	.align 2, 0
_0805A2F0: .4byte 0x020192E0
_0805A2F4: .4byte 0x00001B2C
_0805A2F8: .4byte 0x02015EF0
_0805A2FC:
	mov r0, #1
_0805A2FE:
	add sp, #0x14
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiPlaySpells
	.align 2, 0

