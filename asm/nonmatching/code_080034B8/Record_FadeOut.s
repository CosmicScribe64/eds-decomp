	thumb_func_start Record_FadeOut
Record_FadeOut: @ 0x08003E80
	push {lr}
	bl Record_DrawSprites
	mov r0, #2
	bl FadeToBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end Record_FadeOut

