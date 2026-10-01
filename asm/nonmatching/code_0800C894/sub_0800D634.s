	thumb_func_start sub_0800D634
sub_0800D634: @ 0x0800D634
	push {r4, lr}
	ldr r1, _0800D6A8 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0800D654
	ldr r0, _0800D6AC @ =0x020192E0
	ldr r1, _0800D6B0 @ =0x00001B12
	add r0, r0, r1
	mov r1, #2
	ldrb r0, [r0]
	and r1, r0
	ldr r4, _0800D6B4 @ =0x020185C0
	cmp r1, #0
	bne _0800D694
_0800D654:
	ldr r3, _0800D6B8 @ =0x02018450
	ldr r2, _0800D6B4 @ =0x020185C0
	ldrh r4, [r2, #2]
	lsr r1, r4, #8
	mov r0, #7
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _0800D6BC @ =0xFFFFFE3F
	ldrh r4, [r3]
	and r0, r4
	orr r0, r1
	strh r0, [r3]
	ldrh r0, [r2, #4]
	add r4, r2, #0
	cmp r0, #0
	beq _0800D694
	ldr r2, _0800D6AC @ =0x020192E0
	ldr r0, _0800D6C0 @ =0x00001B14
	add r3, r2, r0
	ldr r0, [r3]
	ldr r1, _0800D6C4 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #4
	orr r0, r1
	str r0, [r3]
	ldr r1, _0800D6C8 @ =0x00001B16
	add r2, r2, r1
	ldr r0, _0800D6CC @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
_0800D694:
	ldr r2, _0800D6D0 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_0800D6A8: .4byte 0x02015EE8
_0800D6AC: .4byte 0x020192E0
_0800D6B0: .4byte 0x00001B12
_0800D6B4: .4byte 0x020185C0
_0800D6B8: .4byte 0x02018450
_0800D6BC: .4byte 0xFFFFFE3F
_0800D6C0: .4byte 0x00001B14
_0800D6C4: .4byte 0xFFFE01FF
_0800D6C8: .4byte 0x00001B16
_0800D6CC: .4byte 0xFFFFFE01
_0800D6D0: .4byte 0x0000080D
	thumb_func_end sub_0800D634

