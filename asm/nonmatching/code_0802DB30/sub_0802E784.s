	thumb_func_start sub_0802E784
sub_0802E784: @ 0x0802E784
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	lsl r4, r2, #0x10
	lsr r4, r4, #0x10
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008860
	ldrb r0, [r5, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	bl sub_08008860
	cmp r4, #0
	beq _0802E7AA
	b _0802E8F6
_0802E7AA:
	ldrb r0, [r5, #2]
	lsl r2, r0, #0x1F
	lsr r1, r2, #0x1F
	add r3, r0, #0
	ldrb r0, [r5, #6]
	cmp r0, r1
	bne _0802E7BA
	b _0802E8F6
_0802E7BA:
	mov r0, #0xFC
	ldrb r1, [r5, #3]
	and r0, r1
	cmp r0, #0x40
	beq _0802E7C6
	b _0802E8F6
_0802E7C6:
	ldr r0, _0802E7F4 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0802E7F8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r4, [r0]
	mov r0, #0x84
	lsl r0, r0, #3
	cmp r4, r0
	beq _0802E83C
	cmp r4, r0
	bgt _0802E810
	sub r0, #0x6F
	cmp r4, r0
	beq _0802E88C
	cmp r4, r0
	bgt _0802E800
	ldr r0, _0802E7FC @ =0x000002AD
	cmp r4, r0
	beq _0802E83C
	b _0802E8F6
	.align 2, 0
_0802E7F4: .4byte 0x000007FF
_0802E7F8: .4byte gUnk_08622AB4
_0802E7FC: .4byte 0x000002AD
_0802E800:
	mov r0, #0xF0
	lsl r0, r0, #2
	cmp r4, r0
	beq _0802E8F2
	add r0, #0x1E
	cmp r4, r0
	beq _0802E8BC
	b _0802E8F6
_0802E810:
	ldr r0, _0802E824 @ =0x000004BE
	cmp r4, r0
	beq _0802E8F2
	cmp r4, r0
	bgt _0802E828
	sub r0, #0x74
	cmp r4, r0
	beq _0802E8F2
	b _0802E8F6
	.align 2, 0
_0802E824: .4byte 0x000004BE
_0802E828:
	ldr r0, _0802E838 @ =0x00000587
	cmp r4, r0
	beq _0802E8F2
	add r0, #9
	cmp r4, r0
	bne _0802E8F6
	b _0802E8F2
	.align 2, 0
_0802E838: .4byte 0x00000587
_0802E83C:
	mov r4, #0
	ldr r7, _0802E884 @ =0x0201930C
	lsl r5, r3, #0x1F
	mov r3, #1
	ldr r6, _0802E888 @ =0x00000D64
_0802E846:
	lsr r1, r5, #0x1F
	sub r1, r3, r1
	and r1, r3
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	add r0, r1, #0
	mul r0, r6
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802E87A
	lsr r0, r5, #0x1F
	sub r0, r3, r0
	and r0, r3
	add r1, r0, #0
	mul r1, r6
	add r1, r2, r1
	add r1, r1, r7
	add r0, r3, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802E8F2
_0802E87A:
	add r4, #1
	cmp r4, #9
	ble _0802E846
	b _0802E8F6
	.align 2, 0
_0802E884: .4byte 0x0201930C
_0802E888: .4byte 0x00000D64
_0802E88C:
	ldrh r1, [r5, #8]
	lsr r0, r1, #8
	cmp r0, #4
	bhi _0802E8F6
	lsr r0, r2, #0x1F
	bl sub_08008860
	cmp r0, #0
	beq _0802E8F6
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #1
	ble _0802E8F6
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl sub_08044224
	b _0802E8EE
_0802E8BC:
	ldrh r1, [r5, #8]
	lsr r0, r1, #8
	cmp r0, #4
	bhi _0802E8F6
	lsr r0, r2, #0x1F
	bl sub_08008860
	cmp r0, #0
	beq _0802E8F6
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	beq _0802E8F6
	ldrb r5, [r5, #2]
	lsl r1, r5, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
_0802E8EE:
	cmp r0, #1
	ble _0802E8F6
_0802E8F2:
	mov r0, #1
	b _0802E8F8
_0802E8F6:
	mov r0, #0
_0802E8F8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802E784
	.align 2, 0

