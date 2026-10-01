	thumb_func_start sub_080600AC
sub_080600AC: @ 0x080600AC
	push {r4, lr}
	ldr r4, _080600CC @ =0x0201AE60
	add r4, #0x21
	ldrb r0, [r4]
	sub r0, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl sub_0805FEA4
	ldrb r0, [r4]
	cmp r0, #0
	beq _080600D0
	mov r0, #0
	b _080600D2
	.align 2, 0
_080600CC: .4byte 0x0201AE60
_080600D0:
	mov r0, #1
_080600D2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080600AC

