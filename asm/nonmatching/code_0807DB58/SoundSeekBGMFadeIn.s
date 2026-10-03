	thumb_func_start SoundSeekBGMFadeIn
SoundSeekBGMFadeIn: @ 0x0807EA4C
	push {r4, r5, lr}
	add r4, r2, #0
	ldr r5, _0807EA7C @ =0x03005210
	bl SoundSeekBGM
	mov r1, #0xC9
	lsl r1, r1, #1
	add r0, r5, r1
	mov r1, #0
	strb r4, [r0]
	ldr r0, _0807EA80 @ =0x00000193
	add r2, r5, r0
	mov r0, #0x10
	strb r0, [r2]
	ldr r2, _0807EA84 @ =0x00000191
	add r0, r5, r2
	strb r1, [r0]
	sub r2, #1
	add r0, r5, r2
	strb r1, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0807EA7C: .4byte 0x03005210
_0807EA80: .4byte 0x00000193
_0807EA84: .4byte 0x00000191
	thumb_func_end SoundSeekBGMFadeIn

