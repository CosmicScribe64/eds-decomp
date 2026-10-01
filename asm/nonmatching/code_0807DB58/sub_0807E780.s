	thumb_func_start sub_0807E780
sub_0807E780: @ 0x0807E780
	push {r4, lr}
	ldr r1, _0807E7B0 @ =0x03005210
	mov ip, r1
	mov r3, #0xC4
	lsl r3, r3, #1
	add r3, ip
	ldrh r2, [r3]
	mov r4, #0x80
	lsl r4, r4, #1
	add r1, r4, #0
	mov r4, #0
	orr r1, r2
	strh r1, [r3]
	ldr r1, _0807E7B4 @ =0x00000193
	add r1, ip
	strb r4, [r1]
	mov r1, #0xC9
	lsl r1, r1, #1
	add r1, ip
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0807E7B0: .4byte 0x03005210
_0807E7B4: .4byte 0x00000193
	thumb_func_end sub_0807E780

