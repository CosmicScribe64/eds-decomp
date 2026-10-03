	thumb_func_start SoundRequestBGM
SoundRequestBGM: @ 0x0807E674
	ldr r2, _0807E68C @ =0x03005210
	mov r1, #0xC5
	lsl r1, r1, #1
	add r3, r2, r1
	mov r1, #0
	strh r0, [r3]
	mov r3, #0xCA
	lsl r3, r3, #1
	add r0, r2, r3
	strb r1, [r0]
	bx lr
	.align 2, 0
_0807E68C: .4byte 0x03005210
	thumb_func_end SoundRequestBGM

