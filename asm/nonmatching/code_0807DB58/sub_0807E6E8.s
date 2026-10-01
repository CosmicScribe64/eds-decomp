	thumb_func_start sub_0807E6E8
sub_0807E6E8: @ 0x0807E6E8
	add r3, r0, #0
	ldr r2, _0807E720 @ =0x03005210
	mov r1, #0xC4
	lsl r1, r1, #1
	add r0, r2, r1
	ldrh r1, [r0]
	mov r0, #0x80
	and r0, r1
	cmp r0, #0
	beq _0807E70A
	mov r1, #0xC7
	lsl r1, r1, #1
	add r0, r2, r1
	mov r1, #0
	ldsh r0, [r0, r1]
	cmp r0, r3
	beq _0807E71C
_0807E70A:
	mov r1, #0xC5
	lsl r1, r1, #1
	add r0, r2, r1
	mov r1, #0
	strh r3, [r0]
	mov r3, #0xCA
	lsl r3, r3, #1
	add r0, r2, r3
	strb r1, [r0]
_0807E71C:
	bx lr
	.align 2, 0
_0807E720: .4byte 0x03005210
	thumb_func_end sub_0807E6E8

