	thumb_func_start sub_0807EAB8
sub_0807EAB8: @ 0x0807EAB8
	ldr r0, _0807EACC @ =0x03005210
	mov r1, #0xC4
	lsl r1, r1, #1
	add r0, r0, r1
	ldrh r2, [r0]
	mov r1, #5
	orr r1, r2
	strh r1, [r0]
	bx lr
	.align 2, 0
_0807EACC: .4byte 0x03005210
	thumb_func_end sub_0807EAB8

