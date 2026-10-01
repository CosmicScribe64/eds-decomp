	thumb_func_start sub_0807BE60
sub_0807BE60: @ 0x0807BE60
	push {lr}
	bl sub_0807373C
	ldr r0, _0807BE74 @ =0x03005B60
	ldr r1, _0807BE78 @ =0x00000B38
	bl sub_08075278
	mov r0, #1
	pop {r1}
	bx r1
_0807BE74: .4byte 0x03005B60
_0807BE78: .4byte 0x00000B38
	thumb_func_end sub_0807BE60

