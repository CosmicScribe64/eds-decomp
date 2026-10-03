	thumb_func_start DeckEdit_TickFadeIn
DeckEdit_TickFadeIn: @ 0x0806ED44
	push {r4, lr}
	ldr r4, _0806ED58 @ =0x0201E138
	add r0, r4, #0
	bl FadeTick
	ldrb r4, [r4, #6]
	cmp r4, #3
	beq _0806ED5C
	mov r0, #0
	b _0806ED5E
_0806ED58: .4byte 0x0201E138
_0806ED5C:
	mov r0, #1
_0806ED5E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DeckEdit_TickFadeIn

