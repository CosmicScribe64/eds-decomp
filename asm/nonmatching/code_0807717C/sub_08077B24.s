	thumb_func_start sub_08077B24
sub_08077B24: @ 0x08077B24
	push {r4, r5, lr}
	add r4, r0, #0
	bl sub_08077A5C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077B46
	ldr r0, _08077B4C @ =0x03000040
	ldr r1, _08077B50 @ =0x0000485C
	add r5, r0, r1
	ldrh r0, [r5]
	cmp r0, r4
	beq _08077B46
	add r0, r4, #0
	bl sub_0807E674
	strh r4, [r5]
_08077B46:
	pop {r4, r5}
	pop {r0}
	bx r0
_08077B4C: .4byte 0x03000040
_08077B50: .4byte 0x0000485C
	thumb_func_end sub_08077B24

