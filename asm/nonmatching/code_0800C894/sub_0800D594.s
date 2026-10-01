	thumb_func_start sub_0800D594
sub_0800D594: @ 0x0800D594
	push {r4, lr}
	ldr r1, _0800D60C @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0800D5B4
	ldr r0, _0800D610 @ =0x020192E0
	ldr r1, _0800D614 @ =0x00001B12
	add r0, r0, r1
	mov r1, #2
	ldrb r0, [r0]
	and r1, r0
	ldr r4, _0800D618 @ =0x020185C0
	cmp r1, #0
	bne _0800D5F6
_0800D5B4:
	ldr r3, _0800D61C @ =0x02018450
	ldr r2, _0800D618 @ =0x020185C0
	mov r0, #7
	ldrh r4, [r2, #2]
	lsr r1, r4, #8
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0xF
	neg r0, r0
	ldrb r4, [r3, #1]
	and r0, r4
	orr r0, r1
	strb r0, [r3, #1]
	ldrh r0, [r2, #4]
	add r4, r2, #0
	cmp r0, #0
	beq _0800D5F6
	ldr r2, _0800D610 @ =0x020192E0
	ldr r0, _0800D620 @ =0x00001B14
	add r3, r2, r0
	ldr r0, [r3]
	ldr r1, _0800D624 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #4
	orr r0, r1
	str r0, [r3]
	ldr r1, _0800D628 @ =0x00001B16
	add r2, r2, r1
	ldr r0, _0800D62C @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
_0800D5F6:
	ldr r2, _0800D630 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800D60C: .4byte 0x02015EE8
_0800D610: .4byte 0x020192E0
_0800D614: .4byte 0x00001B12
_0800D618: .4byte 0x020185C0
_0800D61C: .4byte 0x02018450
_0800D620: .4byte 0x00001B14
_0800D624: .4byte 0xFFFE01FF
_0800D628: .4byte 0x00001B16
_0800D62C: .4byte 0xFFFFFE01
_0800D630: .4byte 0x0000080D
	thumb_func_end sub_0800D594

