	thumb_func_start TurnOrder_UpdateChoiceBob
TurnOrder_UpdateChoiceBob: @ 0x0802899C
	push {r4, lr}
	add r4, r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r3, r0, #0
	lsl r0, r3, #2
	add r2, r0, r4
	ldrh r0, [r2]
	add r0, #0x20
	strh r0, [r2]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	mov r1, #0x80
	lsl r1, r1, #4
	cmp r0, r1
	ble _080289BE
	strh r1, [r2]
_080289BE:
	mov r0, #1
	eor r3, r0
	lsl r0, r3, #2
	add r1, r0, r4
	ldrh r0, [r1]
	sub r0, #0x40
	strh r0, [r1]
	lsl r0, r0, #0x10
	cmp r0, #0
	bge _080289D6
	mov r0, #0
	strh r0, [r1]
_080289D6:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end TurnOrder_UpdateChoiceBob

