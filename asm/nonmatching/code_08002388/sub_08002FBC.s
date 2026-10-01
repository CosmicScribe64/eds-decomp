	thumb_func_start sub_08002FBC
sub_08002FBC: @ 0x08002FBC
	push {lr}
	ldr r0, _08002FCC @ =0x08003AA5
	bl sub_080754F8
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08002FCC: .4byte sub_08003AA4
	thumb_func_end sub_08002FBC

