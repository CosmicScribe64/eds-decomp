	thumb_func_start RemoveCardFromSavedDeck
RemoveCardFromSavedDeck: @ 0x0807766C
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	bl DebugCheckCardId
	ldr r3, _080776DC @ =0x02011C20
	lsl r0, r4, #2
	add r5, r0, r3
	ldrb r2, [r5, #9]
	lsl r1, r2, #0x1C
	lsr r0, r1, #0x1E
	cmp r0, #0
	beq _080776F2
	sub r0, #1
	mov r1, #3
	and r0, r1
	lsl r0, r0, #2
	mov r1, #0xD
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r5, #9]
	mov r2, #0
	ldr r1, _080776E0 @ =0x000020C8
	add r0, r3, r1
	ldrh r1, [r0]
	cmp r2, r1
	bge _080776F2
	ldr r1, _080776E4 @ =0x00002008
	add r5, r3, r1
	add r1, r0, #0
	add r6, r1, #0
	add r7, r3, #0
	mov r3, #0
_080776B2:
	add r0, r3, r5
	ldrh r0, [r0]
	cmp r0, r4
	bne _080776E8
	ldrh r0, [r1]
	sub r0, #1
	strh r0, [r1]
	cmp r2, r0
	bge _080776F2
	add r4, r6, #0
	ldr r1, _080776E4 @ =0x00002008
	add r0, r3, r1
	add r1, r0, r7
_080776CC:
	ldrh r0, [r1, #2]
	strh r0, [r1]
	add r1, #2
	add r2, #1
	ldrh r0, [r4]
	cmp r2, r0
	blt _080776CC
	b _080776F2
_080776DC: .4byte 0x02011C20
_080776E0: .4byte 0x000020C8
_080776E4: .4byte 0x00002008
_080776E8:
	add r3, #2
	add r2, #1
	ldrh r0, [r1]
	cmp r2, r0
	blt _080776B2
_080776F2:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end RemoveCardFromSavedDeck

