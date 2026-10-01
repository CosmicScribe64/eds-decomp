	thumb_func_start sub_0802297C
sub_0802297C: @ 0x0802297C
	push {r4, r5, lr}
	sub sp, #8
	lsl r0, r0, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r4, _080229B4 @ =0xFFFF0000
	lsl r1, r1, #0x10
	ldr r5, _080229B8 @ =0x0000FFFF
	lsr r0, r0, #0x10
	orr r0, r1
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r4
	orr r0, r2
	lsl r3, r3, #0x10
	and r0, r5
	orr r0, r3
	str r0, [sp, #4]
	mov r0, sp
	mov r1, #8
	bl sub_080723B4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add sp, #8
	pop {r4, r5}
	pop {r1}
	bx r1
_080229B4: .4byte 0xFFFF0000
_080229B8: .4byte 0x0000FFFF
	thumb_func_end sub_0802297C

