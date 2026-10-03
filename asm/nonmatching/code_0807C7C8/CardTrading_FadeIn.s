	thumb_func_start CardTrading_FadeIn
CardTrading_FadeIn: @ 0x0807CF6C
	push {lr}
	ldr r2, _0807CFA4 @ =0x0201F780
	ldrb r1, [r2, #2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r2, [r2, #3]
	lsl r2, r2, #0x1B
	lsr r2, r2, #0x1C
	bl CardTrading_DrawMenu
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x98
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #2
	bl FadeFromBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	.align 2, 0
_0807CFA4: .4byte 0x0201F780
	thumb_func_end CardTrading_FadeIn

