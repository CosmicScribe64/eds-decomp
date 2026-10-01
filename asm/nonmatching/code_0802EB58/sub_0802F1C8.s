	thumb_func_start sub_0802F1C8
sub_0802F1C8: @ 0x0802F1C8
	push {r4, lr}
	add r4, r0, #0
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	beq _0802F1F8
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	beq _0802F1F8
	mov r0, #1
	b _0802F1FA
_0802F1F8:
	mov r0, #0
_0802F1FA:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802F1C8

