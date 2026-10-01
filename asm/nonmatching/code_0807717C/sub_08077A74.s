	thumb_func_start sub_08077A74
sub_08077A74: @ 0x08077A74
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077A94
	ldr r0, _08077A8C @ =0x02011C20
	ldr r1, _08077A90 @ =0x00002152
	add r0, r0, r1
	mov r1, #1
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
	b _08077AA2
	.align 2, 0
_08077A8C: .4byte 0x02011C20
_08077A90: .4byte 0x00002152
_08077A94:
	ldr r1, _08077AA4 @ =0x02011C20
	ldr r0, _08077AA8 @ =0x00002152
	add r1, r1, r0
	ldr r0, _08077AAC @ =0x0000FFFE
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
_08077AA2:
	bx lr
_08077AA4: .4byte 0x02011C20
_08077AA8: .4byte 0x00002152
_08077AAC: .4byte 0x0000FFFE
	thumb_func_end sub_08077A74

