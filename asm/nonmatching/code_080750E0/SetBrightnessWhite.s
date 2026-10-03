	thumb_func_start SetBrightnessWhite
SetBrightnessWhite: @ 0x08075A30
	ldr r2, _08075A58 @ =0x03000040
	ldr r0, _08075A5C @ =0x00004832
	add r2, r2, r0
	mov r0, #0x40
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x1F
	orr r0, r1
	strb r0, [r2]
	ldr r2, _08075A60 @ =0x04000050
	ldr r3, _08075A64 @ =0x00003FBF
	add r1, r3, #0
	strh r1, [r2]
	ldr r1, _08075A68 @ =0x04000054
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
	strh r0, [r1]
	bx lr
	.align 2, 0
_08075A58: .4byte 0x03000040
_08075A5C: .4byte 0x00004832
_08075A60: .4byte 0x04000050
_08075A64: .4byte 0x00003FBF
_08075A68: .4byte 0x04000054
	thumb_func_end SetBrightnessWhite

