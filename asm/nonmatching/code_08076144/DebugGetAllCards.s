	thumb_func_start DebugGetAllCards
DebugGetAllCards: @ 0x08077114
	push {r4, r5, r6, lr}
	mov r2, #1
_08077118:
	ldr r0, _0807716C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _08077170 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _08077174 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r6, r2, #1
	cmp r0, #0x4F
	bls _0807715C
	ldr r1, _08077178 @ =0x02011C20
	lsl r0, r2, #2
	add r4, r0, r1
	lsl r5, r2, #0x10
_0807713A:
	lsr r0, r5, #0x10
	bl AddCardToTrunk
	ldrh r0, [r4, #8]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x16
	ldrb r2, [r4, #9]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1E
	add r1, r1, r0
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1E
	add r1, r1, r0
	lsr r2, r2, #6
	add r1, r1, r2
	cmp r1, #2
	ble _0807713A
_0807715C:
	add r2, r6, #0
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r2, r0
	ble _08077118
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0807716C: .4byte 0x000007FF
_08077170: .4byte gCardIdToNumber
_08077174: .4byte 0xFFFFF880
_08077178: .4byte 0x02011C20
	thumb_func_end DebugGetAllCards

