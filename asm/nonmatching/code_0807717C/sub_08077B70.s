	thumb_func_start sub_08077B70
sub_08077B70: @ 0x08077B70
	push {r4, r5, lr}
	add r4, r0, #0
	bl sub_08077A44
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077B92
	ldr r0, _08077B98 @ =0x03000040
	ldr r1, _08077B9C @ =0x0000485C
	add r5, r0, r1
	ldrh r0, [r5]
	cmp r0, r4
	beq _08077B92
	add r0, r4, #0
	bl sub_0807E674
	strh r4, [r5]
_08077B92:
	pop {r4, r5}
	pop {r0}
	bx r0
_08077B98: .4byte 0x03000040
_08077B9C: .4byte 0x0000485C
	thumb_func_end sub_08077B70

