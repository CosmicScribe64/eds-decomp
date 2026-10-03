	thumb_func_start LoadOpponentDeck
LoadOpponentDeck: @ 0x08059258
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r0, _080592B8 @ =0x03000040
	ldr r1, _080592BC @ =0x00004870
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r7, r0, #0x1B
	mov r6, #0
	ldr r4, _080592C0 @ =0x0201A80C
	mov r5, #0xA0
	lsl r5, r5, #1
	add r0, r4, #0
	add r1, r5, #0
	bl MemClear16
	mov r1, #0xA0
	lsl r1, r1, #2
	add r0, r4, r1
	add r1, r5, #0
	bl MemClear16
	ldr r1, _080592C4 @ =0xFFFFF83F
	add r0, r4, r1
	strb r6, [r0]
	ldr r0, _080592C8 @ =0xFFFFF841
	add r4, r4, r0
	strb r6, [r4]
	cmp r7, #0
	beq _080592F8
	cmp r7, #0xB
	bne _080592AC
	ldr r0, _080592CC @ =0x02015EE8
	ldr r1, [r0, #4]
	mov r2, #0x80
	lsl r2, r2, #2
	orr r1, r2
	str r1, [r0, #4]
_080592AC:
	mov r1, r8
	cmp r1, #0
	beq _080592D4
	lsl r1, r7, #3
	ldr r0, _080592D0 @ =0x0819DD34
	b _080592D8
_080592B8: .4byte 0x03000040
_080592BC: .4byte 0x00004870
_080592C0: .4byte 0x0201A80C
_080592C4: .4byte 0xFFFFF83F
_080592C8: .4byte 0xFFFFF841
_080592CC: .4byte 0x02015EE8
_080592D0: .4byte gOpponentAltDecks
_080592D4:
	lsl r1, r7, #3
	ldr r0, _08059308 @ =0x0819DC6C
_080592D8:
	add r6, r1, r0
	mov r4, #0
	ldrh r0, [r6, #4]
	cmp r4, r0
	bge _080592F8
_080592E2:
	ldr r0, [r6]
	lsl r1, r4, #1
	add r1, r1, r0
	ldrh r1, [r1]
	mov r0, #1
	bl AddCardNumberToDeckTop
	add r4, #1
	ldrh r1, [r6, #4]
	cmp r4, r1
	blt _080592E2
_080592F8:
	mov r0, #1
	bl RemoveOverLimitDeckCards
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08059308: .4byte gOpponentDecks
	thumb_func_end LoadOpponentDeck

