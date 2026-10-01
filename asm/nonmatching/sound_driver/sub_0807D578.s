	thumb_func_start sub_0807D578
sub_0807D578: @ 0x0807D578
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r1, _0807D678 @ =0x04000084
	mov r0, #0x80
	strh r0, [r1]
	ldr r0, _0807D67C @ =0x04000060
	mov r3, #0
	strh r3, [r0]
	add r0, #2
	strh r3, [r0]
	add r0, #2
	mov r2, #0x80
	lsl r2, r2, #8
	add r1, r2, #0
	strh r1, [r0]
	add r0, #4
	strh r3, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r3, [r0]
	ldr r1, _0807D680 @ =0x04000072
	mov r5, #0x80
	lsl r5, r5, #6
	add r0, r5, #0
	strh r0, [r1]
	ldr r0, _0807D684 @ =0x04000074
	strh r3, [r0]
	add r0, #4
	strh r3, [r0]
	add r0, #4
	strh r3, [r0]
	ldr r2, _0807D688 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _0807D68C @ =0x0000F9F7
	and r0, r1
	strh r0, [r2]
	ldr r1, _0807D690 @ =0x04000080
	ldr r2, _0807D694 @ =0x0000FF77
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r0, #0xE
	strh r0, [r1]
	ldr r2, _0807D698 @ =0x04000088
	ldrh r1, [r2]
	ldr r0, _0807D69C @ =0x00003FFF
	and r0, r1
	mov r5, #0x80
	lsl r5, r5, #7
	add r1, r5, #0
	orr r0, r1
	strh r0, [r2]
	cmp r4, #0
	beq _0807D5EA
	ldr r0, _0807D6A0 @ =0x0807E325
	str r0, [r4]
_0807D5EA:
	ldr r2, _0807D6A4 @ =0x03005210
	mov r0, #0xC4
	lsl r0, r0, #1
	add r1, r2, r0
	mov r0, #0
	strh r0, [r1]
	mov r5, #0xC5
	lsl r5, r5, #1
	add r1, r2, r5
	ldr r0, _0807D6A8 @ =0x0000FFFF
	strh r0, [r1]
	mov r0, #0xC6
	lsl r0, r0, #1
	add r1, r2, r0
	mov r0, #1
	neg r0, r0
	strh r0, [r1]
	mov r1, #0xC8
	lsl r1, r1, #1
	add r0, r2, r1
	strb r3, [r0]
	ldr r3, _0807D6AC @ =0x00000191
	add r1, r2, r3
	mov r0, #0x10
	strb r0, [r1]
	add r5, #9
	add r1, r2, r5
	strb r0, [r1]
	add r3, #1
	add r1, r2, r3
	strb r0, [r1]
	sub r0, #0x11
	str r0, [r2]
	add r0, r2, #0
	add r0, #8
	mov r1, #0xA
	mov r3, #0
_0807D634:
	strb r3, [r0, #0x10]
	sub r1, #1
	add r0, #0x18
	cmp r1, #0
	bne _0807D634
	ldr r5, _0807D6B0 @ =0x00000195
	add r0, r2, r5
	strb r1, [r0]
	mov r1, #5
	mov r3, #0xB8
	lsl r3, r3, #1
	add r0, r2, r3
	mov r3, #0
_0807D64E:
	strb r3, [r0, #0x13]
	str r3, [r0]
	sub r0, #0x18
	sub r1, #1
	cmp r1, #0
	bge _0807D64E
	add r0, r2, #0
	mov r1, #0
	mov r2, #0
	bl sub_0807D518
	ldr r1, _0807D684 @ =0x04000074
	mov r5, #0x80
	lsl r5, r5, #8
	add r0, r5, #0
	strh r0, [r1]
	bl sub_0807D3D0
	pop {r4, r5}
	pop {r0}
	bx r0
_0807D678: .4byte 0x04000084
_0807D67C: .4byte 0x04000060
_0807D680: .4byte 0x04000072
_0807D684: .4byte 0x04000074
_0807D688: .4byte 0x04000200
_0807D68C: .4byte 0x0000F9F7
_0807D690: .4byte 0x04000080
_0807D694: .4byte 0x0000FF77
_0807D698: .4byte 0x04000088
_0807D69C: .4byte 0x00003FFF
_0807D6A0: .4byte sub_0807E324
_0807D6A4: .4byte 0x03005210
_0807D6A8: .4byte 0x0000FFFF
_0807D6AC: .4byte 0x00000191
_0807D6B0: .4byte 0x00000195
	thumb_func_end sub_0807D578

