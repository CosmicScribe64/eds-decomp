	thumb_func_start ShowRevealedCard
ShowRevealedCard: @ 0x08019840
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x76
	cmp r0, #0
	beq _0801984E
	ldr r2, _0801985C @ =0x00008076
_0801984E:
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	pop {r0}
	bx r0
_0801985C: .4byte 0x00008076
	thumb_func_end ShowRevealedCard

