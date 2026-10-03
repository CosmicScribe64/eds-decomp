	thumb_func_start QueueAddZoneLink
QueueAddZoneLink: @ 0x08017AB4
	push {r4, lr}
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r1, #0x85
	cmp r0, #0
	beq _08017ACA
	ldr r1, _08017AD8 @ =0x00008085
_08017ACA:
	add r0, r1, #0
	add r1, r4, #0
	bl DuelCmd_Push
	pop {r4}
	pop {r0}
	bx r0
_08017AD8: .4byte 0x00008085
	thumb_func_end QueueAddZoneLink

