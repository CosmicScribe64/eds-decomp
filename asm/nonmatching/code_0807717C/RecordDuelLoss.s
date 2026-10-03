	thumb_func_start RecordDuelLoss
RecordDuelLoss: @ 0x08077998
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r5, _080779D0 @ =0x02011C20
	lsl r0, r4, #2
	add r0, r0, r5
	ldr r1, _080779D4 @ =0x000020D0
	add r3, r0, r1
	ldr r2, [r3]
	lsl r0, r2, #0xA
	lsr r1, r0, #0x15
	ldr r0, _080779D8 @ =0x000007FE
	cmp r1, r0
	bgt _080779C2
	add r0, r1, #1
	ldr r1, _080779DC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #0xB
	ldr r1, _080779E0 @ =0xFFC007FF
	and r1, r2
	orr r1, r0
	str r1, [r3]
_080779C2:
	ldr r1, _080779E4 @ =0x00002158
	add r0, r5, r1
	strh r4, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080779D0: .4byte 0x02011C20
_080779D4: .4byte 0x000020D0
_080779D8: .4byte 0x000007FE
_080779DC: .4byte 0x000007FF
_080779E0: .4byte 0xFFC007FF
_080779E4: .4byte 0x00002158
	thumb_func_end RecordDuelLoss

