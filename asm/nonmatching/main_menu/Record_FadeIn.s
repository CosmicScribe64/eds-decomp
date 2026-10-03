	thumb_func_start Record_FadeIn
Record_FadeIn: @ 0x08003C58
	push {lr}
	bl Record_DrawSprites
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0xF8
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #2
	bl FadeFromBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end Record_FadeIn

