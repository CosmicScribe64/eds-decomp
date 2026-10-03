	thumb_func_start CardTrading_UnusedCallbackWrapper
CardTrading_UnusedCallbackWrapper: @ 0x0807D3C0
	push {lr}
	bl CB_CardTrading
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end CardTrading_UnusedCallbackWrapper
	.align 2, 0

