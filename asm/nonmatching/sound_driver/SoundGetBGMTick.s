	thumb_func_start SoundGetBGMTick
SoundGetBGMTick: @ 0x0807E9BC
	ldr r0, _0807E9C4 @ =0x03005210
	ldr r0, [r0]
	bx lr
	.align 2, 0
_0807E9C4: .4byte 0x03005210
	thumb_func_end SoundGetBGMTick

