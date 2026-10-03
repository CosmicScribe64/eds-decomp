	thumb_func_start Ease_Start
Ease_Start: @ 0x0807B100
	push {r4, lr}
	mov r4, #1
	strb r4, [r3]
	strh r0, [r3, #2]
	strh r1, [r3, #4]
	strh r2, [r3, #6]
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end Ease_Start
	.align 2, 0

