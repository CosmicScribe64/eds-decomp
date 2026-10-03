	thumb_func_start ParseSignedDecimal
ParseSignedDecimal: @ 0x08078C48
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r3, #0
	ldr r2, [r5]
	mov r4, #1
	ldrb r0, [r2]
	cmp r0, #0x2C
	beq _08078C92
	cmp r0, #0x29
	beq _08078C92
	cmp r0, #0x2D
	beq _08078C92
	ldr r6, _08078C6C @ =0x0000FFD0
_08078C62:
	ldrb r1, [r2]
	cmp r1, #0x2D
	bne _08078C74
	ldr r4, _08078C70 @ =0x0000FFFF
	b _08078C84
_08078C6C: .4byte 0x0000FFD0
_08078C70: .4byte 0x0000FFFF
_08078C74:
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r0, r6
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	add r2, #1
_08078C84:
	ldrb r0, [r2]
	cmp r0, #0x2C
	beq _08078C92
	cmp r0, #0x29
	beq _08078C92
	cmp r0, #0x2D
	bne _08078C62
_08078C92:
	add r2, #1
	str r2, [r5]
	lsl r0, r4, #0x10
	asr r0, r0, #0x10
	mul r0, r3
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end ParseSignedDecimal
	.align 2, 0

