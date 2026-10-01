	thumb_func_start sub_08077AEC
sub_08077AEC: @ 0x08077AEC
	push {r4, lr}
	add r4, r0, #0
	bl sub_08077A44
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077B14
	ldr r0, _08077B1C @ =0x03000040
	ldr r2, _08077B20 @ =0x00004868
	add r1, r0, r2
	sub r2, #0xA
	add r0, r0, r2
	ldrh r0, [r0]
	ldrh r2, [r1]
	cmp r2, r0
	beq _08077B14
	strh r0, [r1]
	add r0, r4, #0
	bl sub_0807E814
_08077B14:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08077B1C: .4byte 0x03000040
_08077B20: .4byte 0x00004868
	thumb_func_end sub_08077AEC

