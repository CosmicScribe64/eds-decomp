	thumb_func_start sub_08001364
sub_08001364: @ 0x08001364
	push {lr}
	mov r0, #0
	bl sub_080011F0
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end sub_08001364
	.align 2, 0

