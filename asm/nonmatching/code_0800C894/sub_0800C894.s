	thumb_func_start sub_0800C894
sub_0800C894: @ 0x0800C894
	push {lr}
	sub sp, #0xC
	mov r2, sp
	bl sub_0800ABC8
	ldr r0, [sp, #4]
	add sp, #0xC
	pop {r1}
	bx r1
	thumb_func_end sub_0800C894
	.align 2, 0

