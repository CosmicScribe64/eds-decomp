	thumb_func_start SoundStopAllSE
SoundStopAllSE: @ 0x0807EA88
	ldr r0, _0807EA9C @ =0x03005210
	mov r1, #0xC4
	lsl r1, r1, #1
	add r0, r0, r1
	ldrh r2, [r0]
	mov r1, #4
	orr r1, r2
	strh r1, [r0]
	bx lr
	.align 2, 0
_0807EA9C: .4byte 0x03005210
	thumb_func_end SoundStopAllSE

