	thumb_func_start sub_08001AE4
sub_08001AE4: @ 0x08001AE4
	push {r4, lr}
	ldr r1, _08001B10 @ =0x0813ADD4
	ldr r0, _08001B14 @ =0x03000040
	ldr r2, _08001B18 @ =0x00004859
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08001B1C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08001B0A
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08001B0A:
	mov r0, #0
	b _08001B2A
	.align 2, 0
_08001B10: .4byte gUnk_0813ADD4
_08001B14: .4byte 0x03000040
_08001B18: .4byte 0x00004859
_08001B1C:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08001B30 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08001B2A:
	pop {r4}
	pop {r1}
	bx r1
_08001B30: .4byte 0x0000E0FF
	thumb_func_end sub_08001AE4

