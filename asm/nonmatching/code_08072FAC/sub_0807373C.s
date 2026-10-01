	thumb_func_start sub_0807373C
sub_0807373C: @ 0x0807373C
	ldr r3, _08073770 @ =0x04000208
	mov r0, #0
	strh r0, [r3]
	ldr r2, _08073774 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08073778 @ =0x0000FF3F
	and r0, r1
	strh r0, [r2]
	ldr r2, _0807377C @ =0x03005B60
	ldr r0, [r2, #4]
	mov r1, #0
	str r1, [r0]
	ldr r0, [r2]
	str r1, [r0]
	mov r0, #1
	strh r0, [r3]
	ldr r1, _08073780 @ =0x04000128
	mov r2, #0x80
	lsl r2, r2, #6
	add r0, r2, #0
	strh r0, [r1]
	add r1, #0xDA
	mov r0, #0xC0
	strh r0, [r1]
	bx lr
	.align 2, 0
_08073770: .4byte 0x04000208
_08073774: .4byte 0x04000200
_08073778: .4byte 0x0000FF3F
_0807377C: .4byte 0x03005B60
_08073780: .4byte 0x04000128
	thumb_func_end sub_0807373C

