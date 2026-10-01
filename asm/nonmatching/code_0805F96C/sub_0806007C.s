	thumb_func_start sub_0806007C
sub_0806007C: @ 0x0806007C
	push {r4, r5, lr}
	ldr r4, _0806009C @ =0x0201AE60
	add r5, r4, #0
	add r5, #0x21
	ldrb r0, [r5]
	bl sub_0805FEA4
	ldrb r1, [r5]
	ldrh r2, [r4, #0xA]
	ldrh r3, [r4, #0xE]
	add r0, r2, r3
	add r0, #2
	cmp r1, r0
	blt _080600A0
	mov r0, #1
	b _080600A6
_0806009C: .4byte 0x0201AE60
_080600A0:
	add r0, r1, #1
	strb r0, [r5]
	mov r0, #0
_080600A6:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0806007C

