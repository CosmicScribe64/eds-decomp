	thumb_func_start CardTrading_ClearState
CardTrading_ClearState: @ 0x0807CDA0
	push {lr}
	ldr r0, _0807CDB0 @ =0x0201F780
	mov r1, #0x28
	bl MemClear16
	mov r0, #1
	pop {r1}
	bx r1
_0807CDB0: .4byte 0x0201F780
	thumb_func_end CardTrading_ClearState

