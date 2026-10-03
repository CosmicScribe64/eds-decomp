	thumb_func_start SoundStartSEVariant
SoundStartSEVariant: @ 0x0807E870
	push {r4, lr}
	add r4, r1, #0
	bl SoundRequestSE
	ldr r1, _0807E890 @ =0x03005210
	mov r0, #3
	and r4, r0
	ldr r2, _0807E894 @ =0x00000197
	add r0, r1, r2
	strb r4, [r0]
	bl SoundStartPendingSE
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0807E890: .4byte 0x03005210
_0807E894: .4byte 0x00000197
	thumb_func_end SoundStartSEVariant

