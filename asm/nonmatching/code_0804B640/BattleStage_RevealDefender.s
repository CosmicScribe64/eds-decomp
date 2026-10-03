	thumb_func_start BattleStage_RevealDefender
BattleStage_RevealDefender: @ 0x0804BC78
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r7, r0, #0
	ldr r6, _0804BCB4 @ =0x02018450
	mov r0, #2
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _0804BC94
	b _0804BFDC
_0804BC94:
	ldr r2, _0804BCB8 @ =0x020192E0
	mov ip, r2
	ldr r3, _0804BCBC @ =0x0201ADF6
	ldrh r3, [r3]
	mov sl, r3
	mov r4, sl
	lsl r4, r4, #0x17
	mov r9, r4
	lsr r3, r4, #0x18
	cmp r3, #1
	beq _0804BD64
	cmp r3, #1
	bgt _0804BCC0
	cmp r3, #0
	beq _0804BCC8
	b _0804BFDC
_0804BCB4: .4byte 0x02018450
_0804BCB8: .4byte 0x020192E0
_0804BCBC: .4byte 0x0201ADF6
_0804BCC0:
	cmp r3, #2
	bne _0804BCC6
	b _0804BF88
_0804BCC6:
	b _0804BFDC
_0804BCC8:
	mov r0, #1
	sub r4, r0, r7
	add r2, r4, #0
	and r2, r0
	ldrb r5, [r6, #1]
	lsl r5, r5, #0x1C
	str r5, [sp, #0]
	lsr r0, r5, #0x1D
	mov r5, #0x94
	add r1, r0, #0
	mul r1, r5
	ldr r0, _0804BD28 @ =0x00000D64
	add r5, r2, #0
	mul r5, r0
	mov r8, r5
	add r1, r8
	ldr r0, _0804BD2C @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0804BD30
	ldr r2, [sp, #0]
	lsr r1, r2, #0x1D
	add r0, r4, #0
	mov r2, #0
	bl FlipFieldCard
	ldrb r6, [r6, #1]
	lsl r0, r6, #0x1C
	lsr r0, r0, #0x1D
	mov r3, #0x94
	mul r0, r3
	add r0, r8
	ldr r5, _0804BD2C @ =0x0201930C
	add r0, r0, r5
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r7, #0
	bl ShowRevealedCard
	add r0, r4, #0
	bl TriggerMysteriousPuppeteer
	b _0804BD4A
	.align 2, 0
_0804BD28: .4byte 0x00000D64
_0804BD2C: .4byte 0x0201930C
_0804BD30:
	strh r3, [r6, #2]
	mov r1, r9
	lsr r0, r1, #0x18
	add r0, #1
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804BD54 @ =0xFFFFFE01
	mov r2, sl
	and r1, r2
	orr r1, r0
	ldr r3, _0804BD58 @ =0x0201ADF6
	strh r1, [r3]
_0804BD4A:
	ldr r2, _0804BD5C @ =0x020192E0
	ldr r4, _0804BD60 @ =0x00001B16
	add r2, r2, r4
	b _0804BF5E
	.align 2, 0
_0804BD54: .4byte 0xFFFFFE01
_0804BD58: .4byte 0x0201ADF6
_0804BD5C: .4byte 0x020192E0
_0804BD60: .4byte 0x00001B16
_0804BD64:
	sub r2, r3, r7
	and r2, r3
	ldrb r5, [r6, #1]
	lsl r0, r5, #0x1C
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0804BF18 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	mov r1, ip
	add r1, #0x2C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r6, #2]
	ldr r1, _0804BF1C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804BF20 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #1
	bl HasFlipEffect
	mov r1, #1
	and r0, r1
	lsl r0, r0, #2
	mov r5, #5
	neg r5, r5
	add r1, r5, #0
	ldrb r2, [r6]
	and r1, r2
	orr r1, r0
	strb r1, [r6]
	ldr r4, _0804BF24 @ =0x000005FA
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0804BDC6
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0804BDCE
_0804BDC6:
	add r0, r5, #0
	ldrb r3, [r6]
	and r0, r3
	strb r0, [r6]
_0804BDCE:
	ldr r5, _0804BF28 @ =0x02018450
	ldr r0, _0804BF1C @ =0x000007FF
	ldrh r4, [r5, #2]
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0804BF20 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804BF2C @ =0x000002DF
	cmp r1, r0
	beq _0804BDEC
	ldr r0, _0804BF30 @ =0x00000492
	cmp r1, r0
	beq _0804BDEC
	b _0804BF58
_0804BDEC:
	mov r2, #1
	sub r6, r2, r7
	add r3, r6, #0
	and r3, r2
	ldrb r4, [r5, #1]
	lsl r0, r4, #0x1C
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mov sl, r1
	mov r1, sl
	mul r1, r0
	ldr r0, _0804BF18 @ =0x00000D64
	add r4, r3, #0
	mul r4, r0
	mov r8, r4
	add r1, r8
	ldr r0, _0804BF34 @ =0x0201930C
	add r1, r1, r0
	ldrb r1, [r1, #6]
	and r2, r1
	cmp r2, #0
	bne _0804BE1A
	b _0804BF4C
_0804BE1A:
	ldr r4, _0804BF24 @ =0x000005FA
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0804BE2A
	b _0804BF4C
_0804BE2A:
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	mov r9, r0
	cmp r0, #0
	beq _0804BE3A
	b _0804BF4C
_0804BE3A:
	ldrb r1, [r5, #1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1D
	mov r2, sl
	mul r2, r0
	add r0, r2, #0
	add r0, r8
	ldr r3, _0804BF34 @ =0x0201930C
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r6, #0
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	beq _0804BF4C
	add r0, r6, #0
	bl FindFreeSpellTrapZone
	add r4, r0, #0
	ldrh r1, [r5, #2]
	add r0, r7, #0
	bl sub_080197C0
	mov r0, #1
	sub r0, r0, r7
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrb r2, [r5, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	lsl r1, r1, #8
	orr r1, r0
	lsl r4, r4, #0x18
	lsr r4, r4, #0x10
	orr r4, r0
	add r0, r6, #0
	add r2, r4, #0
	bl MoveFieldCard
	lsl r2, r7, #0x18
	lsr r2, r2, #0x18
	ldrh r3, [r5]
	lsl r0, r3, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	add r0, r6, #0
	add r1, r4, #0
	bl EquipCard
	mov r0, #0x35
	cmp r7, #0
	beq _0804BEAA
	ldr r0, _0804BF38 @ =0x00008035
_0804BEAA:
	ldrh r4, [r5]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r7, #0
	mov r1, #0
	bl CalcBattle
	mov r1, #9
	neg r1, r1
	add r0, r1, #0
	ldrb r2, [r5, #8]
	and r0, r2
	strb r0, [r5, #8]
	ldrb r3, [r5, #0x14]
	and r1, r3
	strb r1, [r5, #0x14]
	mov r4, r9
	strh r4, [r5, #0x12]
	strh r4, [r5, #0x1E]
	mov r0, #5
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	strb r0, [r5]
	ldr r2, _0804BF34 @ =0x0201930C
	ldr r4, _0804BF3C @ =0x00001AE8
	add r3, r2, r4
	ldr r2, [r3]
	lsr r0, r2, #9
	lsl r0, r0, #0x18
	lsr r0, r0, #8
	mov r5, #0x80
	lsl r5, r5, #0xB
	add r0, r0, r5
	lsr r0, r0, #0x10
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #9
	ldr r1, _0804BF40 @ =0xFFFE01FF
	and r1, r2
	orr r1, r0
	str r1, [r3]
	ldr r0, _0804BF34 @ =0x0201930C
	ldr r2, _0804BF44 @ =0x00001AEA
	add r1, r0, r2
	ldr r0, _0804BF48 @ =0xFFFFFE01
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	mov r0, #0
	b _0804BFDE
_0804BF18: .4byte 0x00000D64
_0804BF1C: .4byte 0x000007FF
_0804BF20: .4byte gCardIdToNumber
_0804BF24: .4byte 0x000005FA
_0804BF28: .4byte 0x02018450
_0804BF2C: .4byte 0x000002DF
_0804BF30: .4byte 0x00000492
_0804BF34: .4byte 0x0201930C
_0804BF38: .4byte 0x00008035
_0804BF3C: .4byte 0x00001AE8
_0804BF40: .4byte 0xFFFE01FF
_0804BF44: .4byte 0x00001AEA
_0804BF48: .4byte 0xFFFFFE01
_0804BF4C:
	ldr r1, _0804BF78 @ =0x02018450
	mov r0, #5
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_0804BF58:
	ldr r2, _0804BF7C @ =0x020192E0
	ldr r5, _0804BF80 @ =0x00001B16
	add r2, r2, r5
_0804BF5E:
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804BF84 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	mov r0, #0
	b _0804BFDE
_0804BF78: .4byte 0x02018450
_0804BF7C: .4byte 0x020192E0
_0804BF80: .4byte 0x00001B16
_0804BF84: .4byte 0xFFFFFE01
_0804BF88:
	mov r0, #1
	sub r0, r0, r7
	lsl r2, r7, #0x18
	lsr r2, r2, #0x18
	ldrh r3, [r6]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x1D
	lsl r1, r1, #8
	orr r2, r1
	mov r1, #1
	sub r1, r1, r7
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrb r6, [r6, #1]
	lsl r3, r6, #0x1C
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r1, r3
	lsl r1, r1, #0x10
	orr r2, r1
	mov r1, #0x11
	bl EventResponse_Request
	ldr r4, _0804BFD4 @ =0x0201ADF6
	ldrh r2, [r4]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804BFD8 @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	mov r0, #0
	b _0804BFDE
	.align 2, 0
_0804BFD4: .4byte 0x0201ADF6
_0804BFD8: .4byte 0xFFFFFE01
_0804BFDC:
	mov r0, #1
_0804BFDE:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end BattleStage_RevealDefender
	.align 2, 0

