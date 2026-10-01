	thumb_func_start sub_08004914
sub_08004914: @ 0x08004914
	push {lr}
	ldr r1, _08004928 @ =0x02011C20
	ldr r2, _0800492C @ =0x00002150
	add r1, r1, r2
	ldrh r1, [r1]
	bl sub_080047F4
	pop {r0}
	bx r0
	.align 2, 0
_08004928: .4byte 0x02011C20
_0800492C: .4byte 0x00002150
	thumb_func_end sub_08004914

