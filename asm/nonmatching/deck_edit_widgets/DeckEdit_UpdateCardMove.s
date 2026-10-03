	thumb_func_start DeckEdit_UpdateCardMove
DeckEdit_UpdateCardMove: @ 0x0806699C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x20
	add r6, r0, #0
	ldrb r0, [r6]
	cmp r0, #5
	bls _080669AE
	b _0806703A
_080669AE:
	lsl r0, r0, #2
	ldr r1, _080669B8 @ =0x080669BC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080669B8: .4byte 0x080669BC
_080669BC:
	.4byte _0806703E
	.4byte _080669D4
	.4byte _08066B34
	.4byte _08066B6C
	.4byte _08066BEC
	.4byte _08066C28
_080669D4:
	ldrb r0, [r6, #0xD]
	cmp r0, #1
	beq _080669EC
	cmp r0, #1
	bgt _080669E6
	cmp r0, #0
	bne _080669E4
	b _08066B12
_080669E4:
	b _08066B34
_080669E6:
	cmp r0, #2
	beq _08066ABA
	b _08066B34
_080669EC:
	ldr r3, _08066A40 @ =0x0201DB20
	ldr r0, _08066A44 @ =0x00001C1C
	add r5, r3, r0
	ldrb r0, [r5]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r1, r1, r3
	mov r8, r1
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r7, r3, r4
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl DeckEdit_IsFusionMonster
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #0
	beq _08066A72
	ldr r0, _08066A48 @ =0x02011C20
	ldr r1, _08066A4C @ =0x000020CC
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0x13
	bhi _08066A6E
	ldrb r3, [r5]
	cmp r3, #0
	beq _08066A50
	cmp r3, #0
	bge _08066A38
	b _08066B34
_08066A38:
	cmp r3, #2
	ble _08066A3E
	b _08066B34
_08066A3E:
	b _08066B12
_08066A40: .4byte 0x0201DB20
_08066A44: .4byte 0x00001C1C
_08066A48: .4byte 0x02011C20
_08066A4C: .4byte 0x000020CC
_08066A50:
	mov r2, r8
	ldrb r1, [r2]
	ldrh r2, [r7]
	mov r0, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl IsBelowCardCopyLimit
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0
	bne _08066B12
	b _08066B20
_08066A6E:
	mov r0, #0
	b _08066B20
_08066A72:
	ldr r0, _08066A90 @ =0x02011C20
	ldr r3, _08066A94 @ =0x000020C8
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, #0x3B
	bhi _08066AB6
	ldrb r3, [r5]
	cmp r3, #0
	beq _08066A98
	cmp r3, #0
	blt _08066B34
	cmp r3, #2
	bgt _08066B34
	b _08066B12
	.align 2, 0
_08066A90: .4byte 0x02011C20
_08066A94: .4byte 0x000020C8
_08066A98:
	mov r5, r8
	ldrb r1, [r5]
	ldrh r2, [r7]
	mov r0, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl IsBelowCardCopyLimit
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08066B12
	strb r4, [r6]
	b _08066B22
_08066AB6:
	strb r4, [r6]
	b _08066B22
_08066ABA:
	ldr r0, _08066ADC @ =0x02011C20
	ldr r1, _08066AE0 @ =0x000020CA
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0xE
	bhi _08066B2A
	ldr r2, _08066AE4 @ =0x0201DB20
	ldr r3, _08066AE8 @ =0x00001C1C
	add r0, r2, r3
	ldrb r0, [r0]
	cmp r0, #0
	beq _08066AEC
	cmp r0, #0
	blt _08066B34
	cmp r0, #2
	bgt _08066B34
	b _08066B12
_08066ADC: .4byte 0x02011C20
_08066AE0: .4byte 0x000020CA
_08066AE4: .4byte 0x0201DB20
_08066AE8: .4byte 0x00001C1C
_08066AEC:
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r2, r4
	ldrb r1, [r0]
	mov r5, #0xC4
	lsl r5, r5, #3
	add r0, r2, r5
	ldrh r2, [r0]
	mov r0, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl IsBelowCardCopyLimit
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0
	beq _08066B20
_08066B12:
	add r0, r6, #0
	bl DeckEdit_BeginCardMove
	mov r0, #1
	bl PlaySE
	b _08066B34
_08066B20:
	strb r0, [r6]
_08066B22:
	mov r0, #3
	bl PlaySE
	b _08066B34
_08066B2A:
	mov r0, #0
	strb r0, [r6]
	mov r0, #3
	bl PlaySE
_08066B34:
	ldr r2, _08066BD8 @ =0x0201DB20
	ldr r1, _08066BDC @ =0x08087480
	ldrb r0, [r6, #0xC]
	add r1, r0, r1
	ldrb r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r4, _08066BE0 @ =0x00001726
	add r1, r0, r4
	ldrb r2, [r1]
	mov r0, #0
	ldsb r0, [r1, r0]
	cmp r0, #0
	beq _08066B56
	b _0806703E
_08066B56:
	mov r0, #0xFF
	strb r0, [r1]
	add r3, r6, #4
	mov r0, #0
	mov r1, #6
	mov r2, #1
	bl Ease_Start
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
_08066B6C:
	mov r5, #0xE
	ldsh r0, [r6, r5]
	lsl r0, r0, #8
	ldr r5, _08066BE4 @ =0x080875D2
	mov r2, #6
	ldsh r1, [r6, r2]
	lsl r1, r1, #1
	add r1, r1, r5
	ldrh r1, [r1]
	bl MulFix8
	add r4, r0, #0
	asr r4, r4, #8
	sub r4, #3
	mov r3, #0x10
	ldsh r0, [r6, r3]
	lsl r0, r0, #8
	mov r2, #6
	ldsh r1, [r6, r2]
	lsl r1, r1, #1
	add r1, r1, r5
	ldrh r1, [r1]
	bl MulFix8
	asr r0, r0, #8
	add r0, #0x28
	ldr r1, _08066BE8 @ =0x081A6D84
	str r0, [sp, #0]
	mov r0, #4
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	str r0, [sp, #0xC]
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	ldr r0, _08066BD8 @ =0x0201DB20
	str r0, [sp, #0x1C]
	add r0, r1, #0
	mov r1, #0
	mov r2, #1
	add r3, r4, #0
	bl OamListAddSpriteGroup
	add r0, r6, #4
	bl Ease_Tick
	ldrb r3, [r6, #4]
	cmp r3, #2
	beq _08066BD2
	b _0806703E
_08066BD2:
	ldrb r0, [r6]
	add r0, #1
	b _0806703C
_08066BD8: .4byte 0x0201DB20
_08066BDC: .4byte gCardFrameAnimIds
_08066BE0: .4byte 0x00001726
_08066BE4: .4byte gDeckEditEaseCurve
_08066BE8: .4byte gCardMoveSprite
_08066BEC:
	ldr r3, _08066C1C @ =0x0201DB20
	ldrb r1, [r6, #0xD]
	add r1, #0xA
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r4, _08066C20 @ =0x00001726
	add r0, r0, r4
	mov r2, #0
	mov r1, #1
	strb r1, [r0]
	ldrb r1, [r6, #0xD]
	add r1, #0xA
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r5, _08066C24 @ =0x00001727
	add r0, r0, r5
	strb r2, [r0]
	ldrb r0, [r6]
	add r0, #1
	b _0806703C
_08066C1C: .4byte 0x0201DB20
_08066C20: .4byte 0x00001726
_08066C24: .4byte 0x00001727
_08066C28:
	ldr r2, _08066C5C @ =0x0201DB20
	ldrb r1, [r6, #0xD]
	add r1, #0xA
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r1, _08066C60 @ =0x00001726
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	cmp r0, #0
	beq _08066C46
	b _0806703E
_08066C46:
	strb r0, [r6]
	ldr r3, _08066C64 @ =0x00001C1C
	add r4, r2, r3
	ldrb r0, [r4]
	cmp r0, #1
	beq _08066C8E
	cmp r0, #1
	bgt _08066C68
	cmp r0, #0
	beq _08066C6E
	b _08066D0C
_08066C5C: .4byte 0x0201DB20
_08066C60: .4byte 0x00001726
_08066C64: .4byte 0x00001C1C
_08066C68:
	cmp r0, #2
	beq _08066CF2
	b _08066D0C
_08066C6E:
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r2, r4
	ldrb r1, [r0]
	mov r5, #0xC4
	lsl r5, r5, #3
	add r0, r2, r5
	ldrh r2, [r0]
	mov r0, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromTrunk
	b _08066D0C
_08066C8E:
	mov r0, #0xA5
	lsl r0, r0, #5
	add r5, r2, r0
	ldr r1, _08066CD4 @ =0x000014A1
	add r0, r2, r1
	ldrb r1, [r0]
	mov r3, #0xC4
	lsl r3, r3, #3
	add r7, r2, r3
	add r3, #2
	add r0, r2, r3
	ldrh r2, [r0]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl DeckEdit_IsFusionMonster
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08066CD8
	ldrb r0, [r4]
	add r1, r0, r5
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromSavedFusionDeck
	b _08066D0C
_08066CD4: .4byte 0x000014A1
_08066CD8:
	ldrb r0, [r4]
	add r1, r0, r5
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromSavedDeck
	b _08066D0C
_08066CF2:
	ldr r4, _08066D1C @ =0x000014A2
	add r0, r2, r4
	ldrb r1, [r0]
	ldr r5, _08066D20 @ =0x00000624
	add r0, r2, r5
	ldrh r2, [r0]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromSavedSideDeck
_08066D0C:
	ldrb r0, [r6, #0xD]
	cmp r0, #1
	beq _08066D60
	cmp r0, #1
	bgt _08066D24
	cmp r0, #0
	beq _08066D2A
	b _08066DFC
_08066D1C: .4byte 0x000014A2
_08066D20: .4byte 0x00000624
_08066D24:
	cmp r0, #2
	beq _08066DD2
	b _08066DFC
_08066D2A:
	ldr r3, _08066D58 @ =0x0201DB20
	ldr r1, _08066D5C @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r3, r3, r4
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToTrunk
	b _08066DFC
	.align 2, 0
_08066D58: .4byte 0x0201DB20
_08066D5C: .4byte 0x00001C1C
_08066D60:
	ldr r3, _08066DB0 @ =0x0201DB20
	ldr r0, _08066DB4 @ =0x00001C1C
	add r5, r3, r0
	ldrb r0, [r5]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r4, r3, r1
	add r1, r0, r4
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov ip, r2
	mov r2, #0xC4
	lsl r2, r2, #3
	add r7, r3, r2
	mov r3, ip
	add r2, r3, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl DeckEdit_IsFusionMonster
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08066DB8
	ldrb r0, [r5]
	add r1, r0, r4
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToSavedFusionDeck
	b _08066DFC
	.align 2, 0
_08066DB0: .4byte 0x0201DB20
_08066DB4: .4byte 0x00001C1C
_08066DB8:
	ldrb r0, [r5]
	add r1, r0, r4
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r7
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToSavedDeck
	b _08066DFC
_08066DD2:
	ldr r3, _08066E48 @ =0x0201DB20
	ldr r4, _08066E4C @ =0x00001C1C
	add r0, r3, r4
	ldrb r0, [r0]
	mov r5, #0xA5
	lsl r5, r5, #5
	add r1, r3, r5
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r3, r3, r4
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToSavedSideDeck
_08066DFC:
	ldr r3, _08066E48 @ =0x0201DB20
	mov r0, #0xA5
	lsl r0, r0, #5
	add r5, r3, r0
	ldrb r1, [r6, #0xD]
	add r0, r1, r5
	mov r1, #0
	strb r1, [r0]
	ldr r2, _08066E50 @ =0x00001C3F
	add r0, r3, r2
	ldrb r4, [r6, #0xD]
	add r0, r4, r0
	strb r1, [r0]
	add r2, #3
	add r0, r3, r2
	ldrb r6, [r6, #0xD]
	add r0, r6, r0
	strb r1, [r0]
	ldr r4, _08066E54 @ =0x00001C3D
	add r2, r3, r4
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	ldr r2, _08066E4C @ =0x00001C1C
	add r0, r3, r2
	ldrb r0, [r0]
	cmp r0, #1
	beq _08066E84
	cmp r0, #1
	bgt _08066E58
	cmp r0, #0
	beq _08066E60
	b _08066FCE
	.align 2, 0
_08066E48: .4byte 0x0201DB20
_08066E4C: .4byte 0x00001C1C
_08066E50: .4byte 0x00001C3F
_08066E54: .4byte 0x00001C3D
_08066E58:
	cmp r0, #2
	bne _08066E5E
	b _08066FAC
_08066E5E:
	b _08066FCE
_08066E60:
	ldr r4, _08066E80 @ =0x02011C20
	ldrb r1, [r5]
	mov r5, #0xC4
	lsl r5, r5, #3
	add r0, r3, r5
	ldrh r2, [r0]
	mov r0, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x16
	b _08066FCC
_08066E80: .4byte 0x02011C20
_08066E84:
	ldr r1, _08066EB0 @ =0x000014A1
	add r0, r3, r1
	ldrb r1, [r0]
	ldr r2, _08066EB4 @ =0x00000622
	add r0, r3, r2
	ldrh r2, [r0]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08066EB8 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _08066EBC @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08066EC0 @ =0x00000776
	cmp r1, r0
	bne _08066EC4
	mov r0, #3
	b _08066F26
_08066EB0: .4byte 0x000014A1
_08066EB4: .4byte 0x00000622
_08066EB8: .4byte 0x000007FF
_08066EBC: .4byte gCardIdToNumber
_08066EC0: .4byte 0x00000776
_08066EC4:
	cmp r1, r0
	blt _08066ED4
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08066ED4
	mov r0, #1
	b _08066F26
_08066ED4:
	ldr r0, _08066EF8 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r4, _08066EFC @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08066F06
	cmp r0, #0x16
	bgt _08066F00
	cmp r0, #0x15
	beq _08066F0A
	b _08066F12
	.align 2, 0
_08066EF8: .4byte 0x000007FF
_08066EFC: .4byte gCardStats
_08066F00:
	cmp r0, #0x17
	beq _08066F0E
	b _08066F12
_08066F06:
	mov r0, #7
	b _08066F26
_08066F0A:
	mov r0, #8
	b _08066F26
_08066F0E:
	mov r0, #9
	b _08066F26
_08066F12:
	ldr r0, _08066F5C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r5, _08066F60 @ =0x08621DE0
	add r0, r0, r5
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08066F26:
	cmp r0, #2
	bne _08066F70
	ldr r4, _08066F64 @ =0x02011C20
	ldr r3, _08066F68 @ =0x0201DB20
	ldr r1, _08066F6C @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r5, #0xC4
	lsl r5, r5, #3
	add r3, r3, r5
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsr r0, r0, #6
	b _08066FCC
	.align 2, 0
_08066F5C: .4byte 0x000007FF
_08066F60: .4byte gCardStats
_08066F64: .4byte 0x02011C20
_08066F68: .4byte 0x0201DB20
_08066F6C: .4byte 0x00001C1C
_08066F70:
	ldr r4, _08066FA0 @ =0x02011C20
	ldr r3, _08066FA4 @ =0x0201DB20
	ldr r1, _08066FA8 @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r5, #0xC4
	lsl r5, r5, #3
	add r3, r3, r5
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1C
	b _08066FCA
_08066FA0: .4byte 0x02011C20
_08066FA4: .4byte 0x0201DB20
_08066FA8: .4byte 0x00001C1C
_08066FAC:
	ldr r4, _08067010 @ =0x02011C20
	ldr r1, _08067014 @ =0x000014A2
	add r0, r3, r1
	ldrb r1, [r0]
	ldr r2, _08067018 @ =0x00000624
	add r0, r3, r2
	ldrh r2, [r0]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
_08066FCA:
	lsr r0, r0, #0x1E
_08066FCC:
	mov r8, r0
_08066FCE:
	bl DeckEdit_BuildCardLists
	ldr r2, _0806701C @ =0x0201DB20
	ldr r3, _08067020 @ =0x00001C1C
	add r0, r2, r3
	ldrb r1, [r0]
	lsl r3, r1, #1
	mov r4, #0xA5
	lsl r4, r4, #5
	add r0, r2, r4
	add r1, r1, r0
	ldrb r5, [r1]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #1
	add r0, r3, r0
	sub r4, #0xC
	add r1, r2, r4
	add r0, r0, r1
	mov r5, #0xC4
	lsl r5, r5, #3
	add r2, r2, r5
	add r3, r3, r2
	ldrh r0, [r0]
	add r1, r0, #0
	ldrh r2, [r3]
	cmp r1, r2
	bne _08067028
	cmp r1, #0
	bne _08067024
	strh r1, [r3]
	b _08067028
	.align 2, 0
_08067010: .4byte 0x02011C20
_08067014: .4byte 0x000014A2
_08067018: .4byte 0x00000624
_0806701C: .4byte 0x0201DB20
_08067020: .4byte 0x00001C1C
_08067024:
	sub r0, #1
	strh r0, [r3]
_08067028:
	mov r3, r8
	cmp r3, #0
	bne _08067034
	mov r0, #2
	bl DeckEdit_StartListSlide
_08067034:
	bl DeckEdit_CountSideDeckMonsters
	b _0806703E
_0806703A:
	mov r0, #0
_0806703C:
	strb r0, [r6]
_0806703E:
	add sp, #0x20
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DeckEdit_UpdateCardMove
	.align 2, 0

