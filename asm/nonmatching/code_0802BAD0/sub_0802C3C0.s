	thumb_func_start sub_0802C3C0
sub_0802C3C0: @ 0x0802C3C0
	push {r4, r5, r6, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r3, r1, #8
	lsr r3, r3, #0x18
	lsr r5, r1, #0x18
	mov r2, #1
	and r2, r3
	mov r0, #0x94
	mul r0, r5
	ldr r1, _0802C434 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802C438 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r3, r0
	bne _0802C4D4
	cmp r5, #4
	bgt _0802C4D4
	cmp r4, #0
	beq _0802C4D4
	ldr r5, _0802C43C @ =0x0000058A
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802C4D4
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802C4D4
	ldr r0, _0802C440 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802C444 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802C450
	cmp r0, #0x17
	ble _0802C448
	cmp r0, #0x18
	beq _0802C44C
	b _0802C450
	.align 2, 0
_0802C434: .4byte 0x00000D64
_0802C438: .4byte 0x0201930C
_0802C43C: .4byte 0x0000058A
_0802C440: .4byte 0x000007FF
_0802C444: .4byte gUnk_08621DE0
_0802C448:
	mov r0, #0
	b _0802C464
_0802C44C:
	mov r0, #0xA
	b _0802C464
_0802C450:
	ldr r0, _0802C490 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802C494 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0802C464:
	cmp r0, #0
	beq _0802C4D4
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r3, r0, #0x1F
	ldr r0, _0802C490 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802C494 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802C4A0
	cmp r0, #0x17
	ble _0802C498
	cmp r0, #0x18
	beq _0802C49C
	b _0802C4A0
_0802C490: .4byte 0x000007FF
_0802C494: .4byte gUnk_08621DE0
_0802C498:
	mov r0, #0
	b _0802C4B4
_0802C49C:
	mov r0, #0xA
	b _0802C4B4
_0802C4A0:
	ldr r0, _0802C4C8 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _0802C4CC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0802C4B4:
	add r2, r0, #1
	add r0, r3, #0
	ldr r1, _0802C4D0 @ =0x00000526
	bl sub_08044224
	cmp r0, #0
	ble _0802C4D4
	mov r0, #1
	b _0802C4D6
	.align 2, 0
_0802C4C8: .4byte 0x000007FF
_0802C4CC: .4byte gUnk_08621DE0
_0802C4D0: .4byte 0x00000526
_0802C4D4:
	mov r0, #0
_0802C4D6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802C3C0

