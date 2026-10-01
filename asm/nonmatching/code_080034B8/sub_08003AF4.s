	thumb_func_start sub_08003AF4
sub_08003AF4: @ 0x08003AF4
	push {lr}
	ldr r0, _08003B44 @ =0x0201F814
	mov r1, #4
	bl sub_08075278
	bl sub_080759F4
	bl sub_080757AC
	bl sub_08073574
	ldr r0, _08003B48 @ =0x0400004C
	mov r1, #0
	strh r1, [r0]
	sub r0, #0x4C
	strh r1, [r0]
	ldr r1, _08003B4C @ =0x04000008
	mov r0, #4
	strh r0, [r1]
	add r1, #2
	ldr r2, _08003B50 @ =0x00004105
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08003B54 @ =0x00004306
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08003B58 @ =0x00000507
	add r0, r2, #0
	strh r0, [r1]
	ldr r0, _08003B5C @ =0x03000040
	ldr r1, _08003B60 @ =0x0000040E
	add r0, r0, r1
	mov r1, #0x63
	strh r1, [r0]
	mov r0, #1
	pop {r1}
	bx r1
	.align 2, 0
_08003B44: .4byte 0x0201F814
_08003B48: .4byte 0x0400004C
_08003B4C: .4byte 0x04000008
_08003B50: .4byte 0x00004105
_08003B54: .4byte 0x00004306
_08003B58: .4byte 0x00000507
_08003B5C: .4byte 0x03000040
_08003B60: .4byte 0x0000040E
	thumb_func_end sub_08003AF4

