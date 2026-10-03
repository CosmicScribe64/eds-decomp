	thumb_func_start SioGetMultiPlayerId
SioGetMultiPlayerId: @ 0x0807BED8
	ldr r0, _0807BEE4 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	bx lr
_0807BEE4: .4byte 0x04000128
	thumb_func_end SioGetMultiPlayerId

