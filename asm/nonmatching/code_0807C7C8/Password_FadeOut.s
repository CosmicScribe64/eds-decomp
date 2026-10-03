	thumb_func_start Password_FadeOut
Password_FadeOut: @ 0x0807CB64
	push {lr}
	mov r0, #2
	bl FadeToBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end Password_FadeOut

