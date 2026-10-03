	thumb_func_start FadeOutBGMAtSpeed
FadeOutBGMAtSpeed: @ 0x08077BF8
	push {lr}
	bl SoundFadeOutBGM
	ldr r0, _08077C0C @ =0x03000040
	ldr r1, _08077C10 @ =0x0000485C
	add r0, r0, r1
	ldr r1, _08077C14 @ =0x0000FFFF
	strh r1, [r0]
	pop {r0}
	bx r0
_08077C0C: .4byte 0x03000040
_08077C10: .4byte 0x0000485C
_08077C14: .4byte 0x0000FFFF
	thumb_func_end FadeOutBGMAtSpeed

