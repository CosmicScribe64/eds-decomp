	thumb_func_start CardTrading_ReverseThrow
CardTrading_ReverseThrow: @ 0x0807D1AC
	push {r4, lr}
	ldr r4, _0807D1D4 @ =0x0201F780
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r3, [r4, #3]
	lsl r2, r3, #0x1B
	lsr r2, r2, #0x1C
	bl CardTrading_DrawMenu
	ldrb r2, [r4, #3]
	lsl r1, r2, #0x1B
	lsr r0, r1, #0x1C
	cmp r0, #1
	bhi _0807D1D8
	mov r0, #1
	b _0807D1EE
	.align 2, 0
_0807D1D4: .4byte 0x0201F780
_0807D1D8:
	lsr r0, r1, #0x1C
	sub r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0x1F
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4, #3]
	mov r0, #0
_0807D1EE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end CardTrading_ReverseThrow

