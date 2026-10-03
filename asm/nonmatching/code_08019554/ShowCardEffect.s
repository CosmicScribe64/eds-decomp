	thumb_func_start ShowCardEffect
ShowCardEffect: @ 0x080197E0
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x73
	cmp r0, #0
	beq _080197EE
	ldr r2, _080197FC @ =0x00008073
_080197EE:
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	pop {r0}
	bx r0
_080197FC: .4byte 0x00008073
	thumb_func_end ShowCardEffect

