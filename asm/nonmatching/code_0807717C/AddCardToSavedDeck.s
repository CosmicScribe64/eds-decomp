	thumb_func_start AddCardToSavedDeck
AddCardToSavedDeck: @ 0x080774EC
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r6, #0
	bl DebugCheckCardId
	ldr r7, _08077548 @ =0x02011C20
	lsl r0, r6, #2
	add r5, r0, r7
	ldrb r0, [r5, #9]
	lsl r4, r0, #0x1C
	lsr r4, r4, #0x1E
	add r0, r6, #0
	bl GetCardCopyLimit
	cmp r4, r0
	bge _08077542
	ldr r2, _0807754C @ =0x000020C8
	add r3, r7, r2
	ldrh r0, [r3]
	cmp r0, #0x3B
	bhi _08077542
	ldrb r2, [r5, #9]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1E
	add r1, #1
	mov r0, #3
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0xD
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #9]
	ldrh r0, [r3]
	add r1, r0, #1
	strh r1, [r3]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r2, _08077550 @ =0x00002008
	add r1, r7, r2
	add r0, r0, r1
	strh r6, [r0]
_08077542:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08077548: .4byte 0x02011C20
_0807754C: .4byte 0x000020C8
_08077550: .4byte 0x00002008
	thumb_func_end AddCardToSavedDeck

