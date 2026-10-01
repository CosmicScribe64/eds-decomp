	thumb_func_start sub_0807570C
sub_0807570C: @ 0x0807570C
	ldr r1, _08075730 @ =0x04000108
	mov r2, #0xF4
	lsl r2, r2, #8
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _08075734 @ =0x0400010A
	ldrh r0, [r2]
	mov r1, #0xC3
	orr r0, r1
	strh r0, [r2]
	ldr r0, _08075738 @ =0x030049D0
	ldr r1, _0807573C @ =0x0000082C
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	bx lr
	.align 2, 0
_08075730: .4byte 0x04000108
_08075734: .4byte 0x0400010A
_08075738: .4byte 0x030049D0
_0807573C: .4byte 0x0000082C
	thumb_func_end sub_0807570C

