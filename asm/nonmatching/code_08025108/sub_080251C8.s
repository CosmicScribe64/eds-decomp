	thumb_func_start sub_080251C8
sub_080251C8: @ 0x080251C8
	push {r4, lr}
	add r4, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r3, #0
	mov r2, #0
	cmp r3, r1
	bcs _080251F6
_080251D8:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r4
	ldrb r0, [r0, #0xA]
	cmp r0, #2
	beq _080251EC
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
_080251EC:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, r1
	bcc _080251D8
_080251F6:
	add r0, r3, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080251C8
	.align 2, 0

