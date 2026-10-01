	thumb_func_start sub_08001B34
sub_08001B34: @ 0x08001B34
	push {r4, lr}
	ldr r1, _08001B6C @ =0x0813ADD4
	ldr r0, _08001B70 @ =0x03000040
	ldr r2, _08001B74 @ =0x00004859
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08001B80
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08001B5A
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08001B5A:
	ldr r1, _08001B78 @ =0x02013DE0
	ldr r0, _08001B7C @ =0x0000137C
	add r1, r1, r0
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r0, #0
	b _08001B8E
_08001B6C: .4byte gUnk_0813ADD4
_08001B70: .4byte 0x03000040
_08001B74: .4byte 0x00004859
_08001B78: .4byte 0x02013DE0
_08001B7C: .4byte 0x0000137C
_08001B80:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08001B94 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08001B8E:
	pop {r4}
	pop {r1}
	bx r1
_08001B94: .4byte 0x0000E0FF
	thumb_func_end sub_08001B34

