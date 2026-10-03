	thumb_func_start TradeCardSelect_Run
TradeCardSelect_Run: @ 0x0806F01C
	push {r4, lr}
	ldr r1, _0806F048 @ =0x081A72E4
	ldr r0, _0806F04C @ =0x03000040
	ldr r2, _0806F050 @ =0x0000485B
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0806F054
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806F042
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806F042:
	mov r0, #0
	b _0806F056
	.align 2, 0
_0806F048: .4byte gTradeCardSelectSteps
_0806F04C: .4byte 0x03000040
_0806F050: .4byte 0x0000485B
_0806F054:
	mov r0, #1
_0806F056:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end TradeCardSelect_Run

