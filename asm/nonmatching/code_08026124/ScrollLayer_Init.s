	thumb_func_start ScrollLayer_Init
ScrollLayer_Init: @ 0x08026D60
	push {r4, r5, lr}
	ldr r4, [sp, #0xC]
	mov r5, #0
	strh r5, [r4, #6]
	strh r2, [r4, #2]
	strh r3, [r4, #4]
	mov r2, #8
	neg r2, r2
	ldrb r3, [r4]
	and r2, r3
	strb r2, [r4]
	mov r2, #0xFF
	strb r2, [r4, #8]
	str r0, [r4, #0xC]
	str r1, [r4, #0x10]
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end ScrollLayer_Init

