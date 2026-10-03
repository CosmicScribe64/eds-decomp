	thumb_func_start Calendar_FadeIn
Calendar_FadeIn: @ 0x08002704
	push {lr}
	bl Calendar_DrawMonth
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _08002724 @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #4
	bl FadeFromBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	.align 2, 0
_08002724: .4byte 0x00001F04
	thumb_func_end Calendar_FadeIn

