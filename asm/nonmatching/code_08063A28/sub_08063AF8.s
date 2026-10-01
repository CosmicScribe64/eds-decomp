	thumb_func_start sub_08063AF8
sub_08063AF8: @ 0x08063AF8
	push {r4, r5, lr}
	ldr r1, _08063B30 @ =0x081A572C
	ldr r5, _08063B34 @ =0x03000040
	ldr r0, _08063B38 @ =0x00004859
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08063B40
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063B2C
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _08063B3C @ =0x0000485A
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_08063B2C:
	mov r0, #0
	b _08063B42
_08063B30: .4byte gUnk_081A572C
_08063B34: .4byte 0x03000040
_08063B38: .4byte 0x00004859
_08063B3C: .4byte 0x0000485A
_08063B40:
	mov r0, #1
_08063B42:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08063AF8

