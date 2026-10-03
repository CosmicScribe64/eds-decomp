	thumb_func_start RemoveCardFromSavedFusionDeck
RemoveCardFromSavedFusionDeck: @ 0x08077784
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	bl DebugCheckCardId
	ldr r5, _080777F0 @ =0x02011C20
	lsl r0, r4, #2
	add r3, r0, r5
	ldrb r2, [r3, #9]
	lsl r1, r2, #0x18
	lsr r0, r1, #0x1E
	cmp r0, #0
	beq _08077806
	add r1, r0, #0
	sub r1, #1
	lsl r1, r1, #6
	mov r0, #0x3F
	and r0, r2
	orr r0, r1
	strb r0, [r3, #9]
	mov r2, #0
	ldr r1, _080777F4 @ =0x000020CC
	add r0, r5, r1
	ldrh r1, [r0]
	cmp r2, r1
	bge _08077806
	ldr r1, _080777F8 @ =0x0000209E
	add r6, r5, r1
	add r1, r0, #0
	add r7, r1, #0
	mov r3, #0
_080777C4:
	add r0, r3, r6
	ldrh r0, [r0]
	cmp r0, r4
	bne _080777FC
	ldrh r0, [r1]
	sub r0, #1
	strh r0, [r1]
	cmp r2, r0
	bge _08077806
	add r4, r7, #0
	ldr r1, _080777F8 @ =0x0000209E
	add r0, r3, r1
	add r1, r0, r5
_080777DE:
	ldrh r0, [r1, #2]
	strh r0, [r1]
	add r1, #2
	add r2, #1
	ldrh r0, [r4]
	cmp r2, r0
	blt _080777DE
	b _08077806
	.align 2, 0
_080777F0: .4byte 0x02011C20
_080777F4: .4byte 0x000020CC
_080777F8: .4byte 0x0000209E
_080777FC:
	add r3, #2
	add r2, #1
	ldrh r0, [r1]
	cmp r2, r0
	blt _080777C4
_08077806:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end RemoveCardFromSavedFusionDeck

