	thumb_func_start SoundIsBGMFadeDone
SoundIsBGMFadeDone: @ 0x0807E7E8
	push {r4, lr}
	ldr r0, _0807E808 @ =0x03005210
	mov r3, #0
	ldr r1, _0807E80C @ =0x00000191
	add r2, r0, r1
	ldr r4, _0807E810 @ =0x00000193
	add r1, r0, r4
	ldrb r0, [r2]
	ldrb r1, [r1]
	cmp r0, r1
	bne _0807E800
	mov r3, #1
_0807E800:
	add r0, r3, #0
	pop {r4}
	pop {r1}
	bx r1
_0807E808: .4byte 0x03005210
_0807E80C: .4byte 0x00000191
_0807E810: .4byte 0x00000193
	thumb_func_end SoundIsBGMFadeDone

