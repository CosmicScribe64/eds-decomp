	thumb_func_start sub_0807C4CC
sub_0807C4CC: @ 0x0807C4CC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl sub_0807BF68
	ldr r5, _0807C6B0 @ =0x0201F7B0
	ldrb r1, [r5, #0x10]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1C
	bl sub_0807BF9C
	bl sub_0807BFE0
	ldrb r2, [r5, #0x11]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1A
	add r1, #1
	mov r0, #0x3F
	and r1, r0
	mov r0, #0x40
	neg r0, r0
	mov r8, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x11]
	ldr r0, [r5, #0x10]
	ldr r7, _0807C6B4 @ =0xFFF03FFF
	and r0, r7
	mov r6, #0x80
	lsl r6, r6, #0xC
	orr r0, r6
	str r0, [r5, #0x10]
	ldr r4, _0807C6B8 @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #2
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C542
	ldrb r2, [r5, #0x10]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1C
	add r1, #7
	mov r0, #7
	and r1, r0
	mov r0, #0x10
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	mov r0, r8
	ldrb r1, [r5, #0x11]
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r5, #0x11]
	mov r0, #0x25
	bl sub_08077AEC
_0807C542:
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C576
	ldrb r2, [r5, #0x10]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1C
	add r1, #9
	mov r0, #7
	and r1, r0
	mov r0, #0x10
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	mov r0, r8
	ldrb r1, [r5, #0x11]
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r5, #0x11]
	mov r0, #0x25
	bl sub_08077AEC
_0807C576:
	mov r0, #0x40
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C5A6
	ldr r1, _0807C6BC @ =0x08087E24
	ldrb r2, [r5, #0x10]
	lsr r0, r2, #4
	lsl r0, r0, #3
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x18
	mov r0, #0xF
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	ldr r0, [r5, #0x10]
	and r0, r7
	orr r0, r6
	str r0, [r5, #0x10]
	mov r0, #0x25
	bl sub_08077AEC
_0807C5A6:
	mov r0, #0x80
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C5D6
	ldr r1, _0807C6BC @ =0x08087E24
	ldrb r2, [r5, #0x10]
	lsr r0, r2, #4
	lsl r0, r0, #3
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsr r1, r0, #4
	lsl r1, r1, #4
	mov r0, #0xF
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	ldr r0, [r5, #0x10]
	and r0, r7
	orr r0, r6
	str r0, [r5, #0x10]
	mov r0, #0x25
	bl sub_08077AEC
_0807C5D6:
	mov r0, #0x20
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C606
	ldr r1, _0807C6BC @ =0x08087E24
	ldrb r2, [r5, #0x10]
	lsr r0, r2, #4
	lsl r0, r0, #3
	add r0, r0, r1
	ldrb r0, [r0, #7]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x18
	mov r0, #0xF
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	ldr r0, [r5, #0x10]
	and r0, r7
	orr r0, r6
	str r0, [r5, #0x10]
	mov r0, #0x25
	bl sub_08077AEC
_0807C606:
	mov r0, #0x10
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C636
	ldr r1, _0807C6BC @ =0x08087E24
	ldrb r2, [r5, #0x10]
	lsr r0, r2, #4
	lsl r0, r0, #3
	add r0, r0, r1
	ldrb r0, [r0, #7]
	lsr r1, r0, #4
	lsl r1, r1, #4
	mov r0, #0xF
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	ldr r0, [r5, #0x10]
	and r0, r7
	orr r0, r6
	str r0, [r5, #0x10]
	mov r0, #0x25
	bl sub_08077AEC
_0807C636:
	mov r0, #1
	ldrh r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	bne _0807C642
	b _0807C750
_0807C642:
	mov r0, #0x26
	bl sub_08077AEC
	ldrb r2, [r5, #0x10]
	lsl r1, r2, #0x18
	lsr r0, r1, #0x1C
	cmp r0, #9
	bhi _0807C6D4
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1C
	add r0, r0, r5
	lsr r1, r1, #0x1C
	strb r1, [r0]
	ldr r2, _0807C6BC @ =0x08087E24
	ldrb r0, [r5, #0x10]
	lsl r1, r0, #0x18
	lsr r0, r1, #0x1C
	lsl r0, r0, #3
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #4
	lsr r1, r1, #0x1C
	lsl r1, r1, #3
	add r1, r1, r2
	ldrb r1, [r1, #1]
	sub r1, #0xC
	lsl r1, r1, #0x10
	orr r0, r1
	mov r2, #0x87
	lsl r2, r2, #5
	mov r1, #0x80
	bl sub_080761F0
	ldrb r2, [r5, #0x10]
	lsl r1, r2, #0x1C
	lsr r0, r1, #0x1C
	cmp r0, #6
	bhi _0807C6C0
	add r1, r0, #0
	add r1, #1
	mov r0, #0xF
	and r1, r0
	mov r0, #0x10
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #0x10]
	mov r0, r8
	ldrb r1, [r5, #0x11]
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r5, #0x11]
	b _0807C750
	.align 2, 0
_0807C6B0: .4byte 0x0201F7B0
_0807C6B4: .4byte 0xFFF03FFF
_0807C6B8: .4byte 0x03000040
_0807C6BC: .4byte gUnk_08087E24
_0807C6C0:
	mov r0, #0xF
	and r0, r2
	mov r1, #0xA0
	orr r0, r1
	strb r0, [r5, #0x10]
	ldr r0, [r5, #0x10]
	and r0, r7
	orr r0, r6
	str r0, [r5, #0x10]
	b _0807C750
_0807C6D4:
	ldr r0, _0807C730 @ =0x00780024
	ldr r2, _0807C734 @ =0x000010E3
	mov r1, #0x80
	bl sub_080761F0
	ldr r0, _0807C738 @ =0x00780044
	ldr r2, _0807C73C @ =0x000010E7
	mov r1, #0x80
	bl sub_080761F0
	add r4, r5, #0
	add r4, #8
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #8
	bl sub_08075294
	bl sub_0807C304
	strh r0, [r5, #0x14]
	mov r0, #0xF
	ldrh r1, [r5, #0x12]
	and r0, r1
	strh r0, [r5, #0x12]
	ldr r0, _0807C740 @ =0x08087F88
	bl sub_0801A7DC
	mov r5, #0
_0807C70C:
	add r0, r5, r4
	ldrb r1, [r0]
	add r1, #0x30
	ldr r0, _0807C744 @ =0x08087F94
	bl sub_0801A7DC
	add r5, #1
	cmp r5, #7
	ble _0807C70C
	ldr r0, _0807C748 @ =0x08087F98
	ldr r1, _0807C74C @ =0x0201F7B0
	ldrh r1, [r1, #0x14]
	bl sub_0801A7DC
	bl sub_0801A7E8
	b _0807C7B6
	.align 2, 0
_0807C730: .4byte 0x00780024
_0807C734: .4byte 0x000010E3
_0807C738: .4byte 0x00780044
_0807C73C: .4byte 0x000010E7
_0807C740: .4byte gUnk_08087F88
_0807C744: .4byte gUnk_08087F94
_0807C748: .4byte gUnk_08087F98
_0807C74C: .4byte 0x0201F7B0
_0807C750:
	ldr r4, _0807C7A0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0807C790
	ldr r3, _0807C7A4 @ =0x0201F7B0
	ldrb r2, [r3, #0x10]
	mov r0, #0xF
	and r0, r2
	cmp r0, #0
	beq _0807C7A8
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1C
	sub r1, #1
	mov r0, #0xF
	and r1, r0
	mov r0, #0x10
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #0x10]
	mov r0, #0x40
	neg r0, r0
	ldrb r1, [r3, #0x11]
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r3, #0x11]
	mov r0, #0x25
	bl sub_08077AEC
_0807C790:
	ldr r4, _0807C7A0 @ =0x03000040
	mov r0, #0xC
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	bne _0807C7A8
	mov r0, #0
	b _0807C7B8
_0807C7A0: .4byte 0x03000040
_0807C7A4: .4byte 0x0201F7B0
_0807C7A8:
	mov r0, #2
	bl sub_08077AEC
	ldr r0, _0807C7C4 @ =0x00004859
	add r1, r4, r0
	mov r0, #0xA
	strb r0, [r1]
_0807C7B6:
	mov r0, #1
_0807C7B8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0807C7C4: .4byte 0x00004859
	thumb_func_end sub_0807C4CC

