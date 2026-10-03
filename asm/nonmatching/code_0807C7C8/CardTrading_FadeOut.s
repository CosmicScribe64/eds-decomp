	thumb_func_start CardTrading_FadeOut
CardTrading_FadeOut: @ 0x0807D064
	push {lr}
	ldr r2, _0807D08C @ =0x0201F780
	ldrb r1, [r2, #2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r2, [r2, #3]
	lsl r2, r2, #0x1B
	lsr r2, r2, #0x1C
	bl CardTrading_DrawMenu
	mov r0, #2
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0807D090
	mov r0, #0
	b _0807D09A
_0807D08C: .4byte 0x0201F780
_0807D090:
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0
	strh r0, [r1]
	mov r0, #1
_0807D09A:
	pop {r1}
	bx r1
	thumb_func_end CardTrading_FadeOut
	.align 2, 0

