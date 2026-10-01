	thumb_func_start sub_08064DF8
sub_08064DF8: @ 0x08064DF8
	push {lr}
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08064E10
	mov r0, #0
	bl sub_08064AF0
	mov r0, #0
	b _08064E1E
_08064E10:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08064E24 @ =0x0000EEFF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08064E1E:
	pop {r1}
	bx r1
	.align 2, 0
_08064E24: .4byte 0x0000EEFF
	thumb_func_end sub_08064DF8

