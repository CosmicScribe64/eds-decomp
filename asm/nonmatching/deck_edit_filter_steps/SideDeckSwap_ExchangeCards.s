	thumb_func_start SideDeckSwap_ExchangeCards
SideDeckSwap_ExchangeCards: @ 0x0806AFA4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0806AFE0 @ =0x0201DB20
	ldr r1, _0806AFE4 @ =0x000014A1
	add r5, r0, r1
	ldrb r1, [r5]
	ldr r2, _0806AFE8 @ =0x00000622
	add r4, r0, r2
	ldrh r2, [r4]
	mov r0, #1
	bl DeckEdit_GetListCard
	bl DeckEdit_IsFusionMonster
	cmp r0, #0
	beq _0806AFEC
	ldrb r1, [r5]
	ldrh r2, [r4]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromSavedFusionDeck
	b _0806AFFE
	.align 2, 0
_0806AFE0: .4byte 0x0201DB20
_0806AFE4: .4byte 0x000014A1
_0806AFE8: .4byte 0x00000622
_0806AFEC:
	ldrb r1, [r5]
	ldrh r2, [r4]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromSavedDeck
_0806AFFE:
	ldr r0, _0806B040 @ =0x0201DB20
	ldr r1, _0806B044 @ =0x000014A2
	add r5, r0, r1
	ldrb r1, [r5]
	ldr r2, _0806B048 @ =0x00000624
	add r4, r0, r2
	ldrh r2, [r4]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl RemoveCardFromSavedSideDeck
	ldrb r1, [r5]
	ldrh r2, [r4]
	mov r0, #2
	bl DeckEdit_GetListCard
	bl DeckEdit_IsFusionMonster
	cmp r0, #0
	beq _0806B04C
	ldrb r1, [r5]
	ldrh r2, [r4]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToSavedFusionDeck
	b _0806B05E
_0806B040: .4byte 0x0201DB20
_0806B044: .4byte 0x000014A2
_0806B048: .4byte 0x00000624
_0806B04C:
	ldrb r1, [r5]
	ldrh r2, [r4]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToSavedDeck
_0806B05E:
	ldr r4, _0806B0EC @ =0x0201DB20
	ldr r0, _0806B0F0 @ =0x000014A1
	add r6, r4, r0
	ldrb r1, [r6]
	ldr r2, _0806B0F4 @ =0x00000622
	add r2, r2, r4
	mov r8, r2
	ldrh r2, [r2]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToSavedSideDeck
	ldr r0, _0806B0F8 @ =0x000014A2
	add r5, r4, r0
	ldrb r1, [r5]
	ldr r2, _0806B0FC @ =0x00000624
	add r7, r4, r2
	ldrh r2, [r7]
	mov r0, #2
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov sl, r0
	ldrb r1, [r6]
	mov r0, r8
	ldrh r2, [r0]
	mov r0, #1
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	mov r1, #0
	strb r1, [r6]
	strb r1, [r5]
	mov r2, #0xE2
	lsl r2, r2, #5
	add r0, r4, r2
	strb r1, [r0]
	add r2, #3
	add r0, r4, r2
	strb r1, [r0]
	sub r2, #2
	add r0, r4, r2
	strb r1, [r0]
	add r2, #3
	add r0, r4, r2
	strb r1, [r0]
	ldr r0, _0806B100 @ =0x00001C3D
	add r2, r4, r0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	bl DeckEdit_BuildCardLists
	ldr r2, _0806B104 @ =0x00001C1C
	add r0, r4, r2
	ldrb r0, [r0]
	cmp r0, #1
	beq _0806B108
	cmp r0, #2
	beq _0806B13E
	b _0806B176
_0806B0EC: .4byte 0x0201DB20
_0806B0F0: .4byte 0x000014A1
_0806B0F4: .4byte 0x00000622
_0806B0F8: .4byte 0x000014A2
_0806B0FC: .4byte 0x00000624
_0806B100: .4byte 0x00001C3D
_0806B104: .4byte 0x00001C1C
_0806B108:
	mov r0, sl
	bl SideDeckSwap_FindCardInList
	mov r1, r8
	strh r0, [r1]
	ldrb r2, [r5]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	ldr r2, _0806B134 @ =0x00001498
	add r1, r4, r2
	add r0, r0, r1
	ldrh r0, [r0]
	add r3, r0, #0
	ldrh r1, [r7]
	cmp r3, r1
	bne _0806B176
	cmp r3, #0
	bne _0806B138
	strh r3, [r7]
	b _0806B176
	.align 2, 0
_0806B134: .4byte 0x00001498
_0806B138:
	sub r0, #1
	strh r0, [r7]
	b _0806B176
_0806B13E:
	mov r0, r9
	bl SideDeckSwap_FindCardInList
	strh r0, [r7]
	ldrb r2, [r6]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	ldr r2, _0806B16C @ =0x00001496
	add r1, r4, r2
	add r0, r0, r1
	ldrh r0, [r0]
	add r3, r0, #0
	mov r1, r8
	ldrh r1, [r1]
	cmp r3, r1
	bne _0806B176
	cmp r3, #0
	bne _0806B170
	mov r2, r8
	strh r3, [r2]
	b _0806B176
	.align 2, 0
_0806B16C: .4byte 0x00001496
_0806B170:
	sub r0, #1
	mov r1, r8
	strh r0, [r1]
_0806B176:
	mov r0, #2
	bl DeckEdit_StartListSlide
	bl DeckEdit_CountSideDeckMonsters
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end SideDeckSwap_ExchangeCards
	.align 2, 0

