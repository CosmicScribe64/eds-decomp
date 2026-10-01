	thumb_func_start sub_080609C4
sub_080609C4: @ 0x080609C4
	push {r4, lr}
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0
	strh r0, [r1]
	ldr r4, _08060A7C @ =0x03000040
	ldr r0, _08060A80 @ =0x0000040E
	add r1, r4, r0
	ldr r0, _08060A84 @ =0x00000603
	strh r0, [r1]
	bl sub_08073574
	bl sub_0806041C
	bl sub_0806075C
	bl sub_08060578
	ldr r3, _08060A88 @ =0x0867BB7C
	mov r0, #0
	mov r1, #0x60
	mov r2, #0x10
	bl sub_080731D0
	mov r1, #0
	ldr r0, _08060A8C @ =0x000020E6
	add r2, r4, r0
	mov r0, #0x85
	lsl r0, r0, #7
	add r3, r0, #0
	mov r0, #0x85
	lsl r0, r0, #6
	add r4, r4, r0
_08060A06:
	add r0, r1, r3
	strh r0, [r2]
	strh r0, [r4]
	add r2, #2
	add r4, #2
	add r1, #1
	cmp r1, #1
	ble _08060A06
	ldr r4, _08060A90 @ =0x020192E0
	ldr r1, _08060A94 @ =0x00001ACC
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1C
	bl sub_0806044C
	ldrh r1, [r4, #4]
	mov r0, #0
	bl sub_08060934
	ldr r2, _08060A98 @ =0x00000D68
	add r0, r4, r2
	ldrh r1, [r0]
	mov r0, #1
	bl sub_08060934
	ldr r0, _08060A9C @ =0x00001B12
	add r4, r4, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1B
	lsr r1, r1, #0x1D
	bl sub_08060964
	bl sub_080611AC
	bl sub_0805ED9C
	bl sub_0805ED78
	bl sub_080759F4
	bl sub_080757AC
	ldr r0, _08060A7C @ =0x03000040
	ldr r1, _08060AA0 @ =0x00000414
	add r0, r0, r1
	ldr r1, _08060AA4 @ =0x08060401
	str r1, [r0]
	ldr r1, _08060AA8 @ =0x0201CFB0
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08060A7C: .4byte 0x03000040
_08060A80: .4byte 0x0000040E
_08060A84: .4byte 0x00000603
_08060A88: .4byte gUnk_0867BB7C
_08060A8C: .4byte 0x000020E6
_08060A90: .4byte 0x020192E0
_08060A94: .4byte 0x00001ACC
_08060A98: .4byte 0x00000D68
_08060A9C: .4byte 0x00001B12
_08060AA0: .4byte 0x00000414
_08060AA4: .4byte sub_08060400
_08060AA8: .4byte 0x0201CFB0
	thumb_func_end sub_080609C4

