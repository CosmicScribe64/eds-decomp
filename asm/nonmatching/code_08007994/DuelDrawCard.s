	thumb_func_start DuelDrawCard
DuelDrawCard: @ 0x080080B4
	push {r4, lr}
	sub sp, #4
	add r4, r0, #0
	mov r1, #0
	mov r2, sp
	bl TakeDeckCardAt
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080080D0
	add r0, r4, #0
	mov r1, sp
	bl AddCardToHand
_080080D0:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end DuelDrawCard

