	thumb_func_start BanishTopDeckCards
BanishTopDeckCards: @ 0x08019CD0
	push {lr}
	mov r2, #0x63
	cmp r0, #0
	beq _08019CDA
	ldr r2, _08019CEC @ =0x00008063
_08019CDA:
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	pop {r0}
	bx r0
_08019CEC: .4byte 0x00008063
	thumb_func_end BanishTopDeckCards

