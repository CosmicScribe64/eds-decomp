	thumb_func_start sub_0800C8A8
sub_0800C8A8: @ 0x0800C8A8
	push {lr}
	sub sp, #0xC
	mov r2, sp
	bl sub_0800ABC8
	ldr r0, [sp, #8]
	add sp, #0xC
	pop {r1}
	bx r1
	thumb_func_end sub_0800C8A8
	.align 2, 0

