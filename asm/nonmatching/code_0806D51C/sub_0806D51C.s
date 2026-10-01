	thumb_func_start sub_0806D51C
sub_0806D51C: @ 0x0806D51C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r1, _0806D600 @ =0x03000040
	ldr r0, _0806D604 @ =0x0000040E
	add r1, r1, r0
	mov r5, #0
	mov r2, #0
	mov r0, #1
	strh r0, [r1]
	ldr r4, _0806D608 @ =0x0201DB20
	ldr r1, _0806D60C @ =0x00000632
	add r0, r4, r1
	strh r2, [r0]
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r4, r3
	strh r2, [r0]
	add r1, #8
	add r0, r4, r1
	strh r2, [r0]
	add r3, #8
	add r0, r4, r3
	strh r2, [r0]
	add r1, #4
	add r0, r4, r1
	strh r2, [r0]
	add r3, #4
	add r0, r4, r3
	strh r2, [r0]
	ldr r1, _0806D610 @ =0x00001BB8
	add r0, r4, r1
	bl sub_08066244
	mov r2, #0xE1
	lsl r2, r2, #5
	add r0, r4, r2
	bl sub_080666AC
	ldr r3, _0806D614 @ =0x000018AC
	add r1, r4, r3
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	ldr r1, _0806D618 @ =0x00001C55
	add r0, r4, r1
	strb r5, [r0]
	ldr r2, _0806D61C @ =0x00001C48
	add r6, r4, r2
	mov r5, #0x1F
	neg r5, r5
	add r0, r5, #0
	ldrb r3, [r6]
	and r0, r3
	mov r1, #8
	mov r8, r1
	mov r2, r8
	orr r0, r2
	strb r0, [r6]
	ldr r3, _0806D620 @ =0x00001C1C
	add r0, r4, r3
	ldrb r1, [r0]
	lsl r2, r1, #1
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r4, r3
	add r1, r1, r0
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r3, _0806D624 @ =0x00001494
	add r1, r4, r3
	add r0, r0, r1
	ldrh r0, [r0]
	mov r3, #0xC4
	lsl r3, r3, #3
	add r1, r4, r3
	add r2, r2, r1
	ldrh r1, [r2]
	ldr r3, _0806D628 @ =0x00001BB0
	add r2, r4, r3
	bl sub_08065F34
	ldr r0, _0806D62C @ =0x00001C34
	add r1, r4, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	and r0, r5
	strb r0, [r1]
	ldrb r3, [r6]
	and r5, r3
	mov r0, r8
	orr r5, r0
	strb r5, [r6]
	ldr r1, _0806D630 @ =0x00001C3D
	add r4, r4, r1
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	mov r1, #3
	orr r0, r1
	strb r0, [r4]
	mov r0, #1
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0806D600: .4byte 0x03000040
_0806D604: .4byte 0x0000040E
_0806D608: .4byte 0x0201DB20
_0806D60C: .4byte 0x00000632
_0806D610: .4byte 0x00001BB8
_0806D614: .4byte 0x000018AC
_0806D618: .4byte 0x00001C55
_0806D61C: .4byte 0x00001C48
_0806D620: .4byte 0x00001C1C
_0806D624: .4byte 0x00001494
_0806D628: .4byte 0x00001BB0
_0806D62C: .4byte 0x00001C34
_0806D630: .4byte 0x00001C3D
	thumb_func_end sub_0806D51C

