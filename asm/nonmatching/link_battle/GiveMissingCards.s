	thumb_func_start GiveMissingCards
GiveMissingCards: @ 0x0801ADE0
	push {r4, r5, lr}
	mov r4, #1
	ldr r5, _0801AE1C @ =0x0000FFFE
_0801ADE6:
	ldr r0, _0801AE20 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0801AE24 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r5
	bhi _0801AE0C
	ldr r0, _0801AE28 @ =0x02011C20
	lsl r1, r4, #2
	add r1, r1, r0
	ldrh r1, [r1, #8]
	lsl r0, r1, #0x16
	cmp r0, #0
	bne _0801AE0C
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	bl AddCardToTrunk
_0801AE0C:
	add r4, #1
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r4, r0
	ble _0801ADE6
	pop {r4, r5}
	pop {r0}
	bx r0
_0801AE1C: .4byte 0x0000FFFE
_0801AE20: .4byte 0x000007FF
_0801AE24: .4byte gCardIdToNumber
_0801AE28: .4byte 0x02011C20
	thumb_func_end GiveMissingCards

