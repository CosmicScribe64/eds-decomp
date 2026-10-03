	thumb_func_start SoundRequestBGMFadeIn
SoundRequestBGMFadeIn: @ 0x0807E690
	push {r4, lr}
	ldr r2, _0807E6AC @ =0x03005210
	mov r4, #0xC5
	lsl r4, r4, #1
	add r3, r2, r4
	strh r0, [r3]
	mov r3, #0xCA
	lsl r3, r3, #1
	add r0, r2, r3
	strb r1, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0807E6AC: .4byte 0x03005210
	thumb_func_end SoundRequestBGMFadeIn

