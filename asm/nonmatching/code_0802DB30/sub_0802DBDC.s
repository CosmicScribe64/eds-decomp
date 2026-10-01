	thumb_func_start sub_0802DBDC
sub_0802DBDC: @ 0x0802DBDC
	push {r4, r5, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802DC50
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #1
	ble _0802DC50
	ldr r5, _0802DC48 @ =0x0000058A
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802DC50
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802DC50
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802DC4C @ =0x000002E1
	bl sub_08009CAC
	cmp r0, #0
	beq _0802DC50
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xBD
	lsl r1, r1, #2
	bl sub_08009CAC
	cmp r0, #0
	beq _0802DC50
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xC8
	lsl r1, r1, #2
	bl sub_08009CAC
	cmp r0, #0
	beq _0802DC50
	mov r0, #1
	b _0802DC52
	.align 2, 0
_0802DC48: .4byte 0x0000058A
_0802DC4C: .4byte 0x000002E1
_0802DC50:
	mov r0, #0
_0802DC52:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802DBDC

