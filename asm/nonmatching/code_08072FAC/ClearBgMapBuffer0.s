	thumb_func_start ClearBgMapBuffer0
ClearBgMapBuffer0: @ 0x080734D4
	push {r4, lr}
	ldr r0, _080734F8 @ =0x0300045C
	mov r1, #0x80
	lsl r1, r1, #4
	bl MemClear16
	ldr r4, _080734FC @ =0x02010014
	mov r1, #0xE0
	lsl r1, r1, #5
	add r0, r4, #0
	bl MemClear16
	sub r4, #4
	mov r0, #0
	strh r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
_080734F8: .4byte 0x0300045C
_080734FC: .4byte 0x02010014
	thumb_func_end ClearBgMapBuffer0

