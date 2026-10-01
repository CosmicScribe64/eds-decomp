	thumb_func_start sub_08024288
sub_08024288: @ 0x08024288
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	ldr r3, _080242B8 @ =0x0201CFB0
	mov r2, #0x83
	lsl r2, r2, #4
	add r5, r3, r2
	lsr r0, r0, #0xF
	mov r4, #1
	ldr r6, _080242BC @ =0x00000834
	add r2, r3, r6
	str r1, [r2]
	ldr r2, _080242C0 @ =0x00000838
	add r1, r3, r2
	mov r2, #0
	strb r2, [r1]
	add r6, #5
	add r3, r3, r6
	strb r2, [r3]
	orr r0, r4
	strb r0, [r5]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080242B8: .4byte 0x0201CFB0
_080242BC: .4byte 0x00000834
_080242C0: .4byte 0x00000838
	thumb_func_end sub_08024288

