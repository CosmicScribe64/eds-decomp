	thumb_func_start sub_0802D30C
sub_0802D30C: @ 0x0802D30C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	add r6, r1, #0
	mov r5, #5
_0802D318:
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl sub_0802D0E0
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802D37C
	add r5, #1
	cmp r5, #9
	ble _0802D318
	ldr r2, _0802D370 @ =0x020192E0
	ldr r1, _0802D374 @ =0x00001B12
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	cmp r6, r0
	bne _0802D380
	mov r5, #0
	add r2, #4
	mov r0, #1
	and r0, r6
	ldr r1, _0802D378 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r1, [r0, #2]
	cmp r5, r1
	bge _0802D3F0
	add r7, r0, #0
_0802D354:
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl sub_0802D25C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802D37C
	add r5, #1
	ldrb r0, [r7, #2]
	cmp r5, r0
	blt _0802D354
	b _0802D3F0
	.align 2, 0
_0802D370: .4byte 0x020192E0
_0802D374: .4byte 0x00001B12
_0802D378: .4byte 0x00000D64
_0802D37C:
	mov r0, #1
	b _0802D3F2
_0802D380:
	ldr r0, _0802D3FC @ =0x000007FF
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802D400 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802D3F0
	mov r5, #0
	mov r0, #1
	and r0, r6
	ldr r1, _0802D404 @ =0x000005F5
	mov r8, r1
	ldr r1, _0802D408 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_0802D3AA:
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	add r1, r1, r7
	ldr r0, _0802D40C @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802D3EA
	cmp r4, #0
	beq _0802D3EA
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0802D058
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802D3EA
	ldr r1, _0802D3FC @ =0x000007FF
	add r0, r1, #0
	and r4, r0
	lsl r0, r4, #1
	ldr r1, _0802D410 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r8
	beq _0802D37C
_0802D3EA:
	add r5, #1
	cmp r5, #4
	ble _0802D3AA
_0802D3F0:
	mov r0, #0
_0802D3F2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802D3FC: .4byte 0x000007FF
_0802D400: .4byte gUnk_08621DE0
_0802D404: .4byte 0x000005F5
_0802D408: .4byte 0x00000D64
_0802D40C: .4byte 0x0201930C
_0802D410: .4byte gUnk_08622AB4
	thumb_func_end sub_0802D30C

