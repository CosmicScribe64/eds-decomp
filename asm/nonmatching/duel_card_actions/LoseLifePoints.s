	thumb_func_start LoseLifePoints
LoseLifePoints: @ 0x08019860
	push {r4, r5, lr}
	add r5, r0, #0
	cmp r1, #0
	beq _0801988A
	mov r0, #0x43
	cmp r5, #0
	beq _08019870
	ldr r0, _08019890 @ =0x00008043
_08019870:
	lsl r4, r1, #0x10
	lsr r4, r4, #0x10
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	lsl r2, r5, #0x10
	orr r2, r4
	add r0, r5, #0
	mov r1, #0xF
	bl EventResponse_Request
_0801988A:
	pop {r4, r5}
	pop {r0}
	bx r0
_08019890: .4byte 0x00008043
	thumb_func_end LoseLifePoints

