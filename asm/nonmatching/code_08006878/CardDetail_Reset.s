	thumb_func_start CardDetail_Reset
CardDetail_Reset: @ 0x08006878
	push {lr}
	ldr r0, _08006888 @ =0x02013D90
	mov r1, #0x44
	bl MemClear16
	pop {r0}
	bx r0
	.align 2, 0
_08006888: .4byte 0x02013D90
	thumb_func_end CardDetail_Reset

