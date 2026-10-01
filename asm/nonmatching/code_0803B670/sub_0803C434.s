	thumb_func_start sub_0803C434
sub_0803C434: @ 0x0803C434
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803C4E6
	mov r4, #7
	ldrb r0, [r5, #0xA]
	and r4, r0
	cmp r4, #2
	bne _0803C4E6
	ldrb r7, [r5, #0xC]
	ldrh r1, [r5, #0xC]
	lsr r6, r1, #8
	ldrh r0, [r5, #0xE]
	mov ip, r0
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	ldr r0, _0803C4A0 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803C4A4 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldrb r2, [r2, #6]
	and r4, r2
	cmp r4, #0
	beq _0803C4E6
	cmp r3, #0
	beq _0803C4E6
	ldr r0, _0803C4A8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0803C4AC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0803C4B8
	cmp r0, #0x17
	ble _0803C4B0
	cmp r0, #0x18
	beq _0803C4B4
	b _0803C4B8
	.align 2, 0
_0803C4A0: .4byte 0x00000D64
_0803C4A4: .4byte 0x0201930C
_0803C4A8: .4byte 0x000007FF
_0803C4AC: .4byte gUnk_08621DE0
_0803C4B0:
	mov r0, #0
	b _0803C4CC
_0803C4B4:
	mov r0, #0xA
	b _0803C4CC
_0803C4B8:
	ldr r0, _0803C4F0 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _0803C4F4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0803C4CC:
	cmp r0, ip
	bne _0803C4E6
	add r0, r7, #0
	add r1, r6, #0
	bl sub_08030028
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r7, #0
	add r2, r6, #0
	bl sub_08046CB0
_0803C4E6:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803C4F0: .4byte 0x000007FF
_0803C4F4: .4byte gUnk_08621DE0
	thumb_func_end sub_0803C434

