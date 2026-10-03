	thumb_func_start Calendar_FadeOut
Calendar_FadeOut: @ 0x08002930
	push {lr}
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end Calendar_FadeOut

