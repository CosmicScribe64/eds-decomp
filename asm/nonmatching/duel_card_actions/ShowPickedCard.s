	thumb_func_start ShowPickedCard
ShowPickedCard: @ 0x08019820
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x75
	cmp r0, #0
	beq _0801982E
	ldr r2, _0801983C @ =0x00008075
_0801982E:
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	pop {r0}
	bx r0
_0801983C: .4byte 0x00008075
	thumb_func_end ShowPickedCard

