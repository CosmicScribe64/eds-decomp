	thumb_func_start ReturnZoneCardToDeck
ReturnZoneCardToDeck: @ 0x08008F14
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r6, r1, #0
	mov r0, #1
	and r0, r5
	ldr r1, _08008F48 @ =0x00000D64
	mul r1, r0
	ldr r0, _08008F4C @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x94
	mul r0, r6
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl IsFusionMonster
	cmp r0, #0
	beq _08008F50
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl AddCardToFusionDeck
	b _08008F5C
_08008F48: .4byte 0x00000D64
_08008F4C: .4byte 0x0201930C
_08008F50:
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl AddCardToDeckTop
_08008F5C:
	add r0, r5, #0
	add r1, r6, #0
	bl RemoveLinksToZone
	add r0, r5, #0
	add r1, r6, #0
	bl ClearZone
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end ReturnZoneCardToDeck
	.align 2, 0

