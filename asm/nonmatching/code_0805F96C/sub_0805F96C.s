	thumb_func_start sub_0805F96C
sub_0805F96C: @ 0x0805F96C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	bl sub_0805ECFC
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r0, _0805F9AC @ =0x0201CFB0
	ldr r2, _0805F9B0 @ =0x00000824
	add r1, r0, r2
	ldr r4, [r1]
	add r2, #4
	add r1, r0, r2
	ldr r6, [r1]
	ldr r1, _0805F9B4 @ =0x0000082C
	add r0, r0, r1
	ldr r7, [r0]
	mov r8, r4
	add r2, r6, r7
	mov r9, r2
	bl sub_0805ED9C
	cmp r6, #0xF
	bls _0805F9A0
	b _0805FB8C
_0805F9A0:
	lsl r0, r6, #2
	ldr r1, _0805F9B8 @ =0x0805F9BC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805F9AC: .4byte 0x0201CFB0
_0805F9B0: .4byte 0x00000824
_0805F9B4: .4byte 0x0000082C
_0805F9B8: .4byte 0x0805F9BC
_0805F9BC:
	.4byte _0805F9FC
	.4byte _0805FB8C
	.4byte _0805FB8C
	.4byte _0805FB8C
	.4byte _0805FB8C
	.4byte _0805FA3C
	.4byte _0805FB8C
	.4byte _0805FB8C
	.4byte _0805FB8C
	.4byte _0805FB8C
	.4byte _0805FA5C
	.4byte _0805FA98
	.4byte _0805FABC
	.4byte _0805FAE0
	.4byte _0805FB10
	.4byte _0805FB44
_0805F9FC:
	cmp r4, #0
	beq _0805FA24
	mov r0, #1
	and r4, r0
	mov r0, #0x94
	mul r0, r7
	ldr r1, _0805FA1C @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r1, _0805FA20 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	b _0805FA26
	.align 2, 0
_0805FA1C: .4byte 0x00000D64
_0805FA20: .4byte 0x0201930C
_0805FA24:
	mov r0, #1
_0805FA26:
	cmp r5, #0
	bne _0805FA2C
	b _0805FB8C
_0805FA2C:
	cmp r0, #0
	bne _0805FA32
	b _0805FB8C
_0805FA32:
	mov r0, r8
	mov r1, r9
	bl sub_0805F728
	b _0805FB8C
_0805FA3C:
	cmp r4, #0
	beq _0805FA7C
	mov r0, #1
	and r0, r4
	ldr r1, _0805FA54 @ =0x00000D64
	mul r0, r1
	mov r1, #0x94
	mul r1, r7
	add r0, r0, r1
	ldr r1, _0805FA58 @ =0x020195F0
	b _0805FA6A
	.align 2, 0
_0805FA54: .4byte 0x00000D64
_0805FA58: .4byte 0x020195F0
_0805FA5C:
	cmp r4, #0
	beq _0805FA7C
	mov r0, #1
	and r0, r4
	ldr r1, _0805FA74 @ =0x00000D64
	mul r0, r1
	ldr r1, _0805FA78 @ =0x020198D4
_0805FA6A:
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	b _0805FA7E
_0805FA74: .4byte 0x00000D64
_0805FA78: .4byte 0x020198D4
_0805FA7C:
	mov r0, #1
_0805FA7E:
	cmp r5, #0
	bne _0805FA84
	b _0805FB8C
_0805FA84:
	cmp r0, #0
	bne _0805FA8A
	b _0805FB8C
_0805FA8A:
	add r3, r6, r7
	add r0, r5, #0
	mov r1, #1
	add r2, r4, #0
	bl sub_0805F270
	b _0805FB8C
_0805FA98:
	cmp r4, #0
	beq _0805FAA8
	add r0, r4, #0
	bl sub_0800A368
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0805FAAA
_0805FAA8:
	mov r0, #1
_0805FAAA:
	cmp r5, #0
	beq _0805FB8C
	cmp r0, #0
	beq _0805FB8C
	add r0, r5, #0
	mov r1, #1
	bl sub_0805F074
	b _0805FB8C
_0805FABC:
	cmp r4, #0
	beq _0805FAC6
	cmp r4, #1
	beq _0805FAD8
	b _0805FB8C
_0805FAC6:
	ldr r4, _0805FAD0 @ =0x080864BC
	ldr r0, _0805FAD4 @ =0x020192E4
	ldrb r5, [r0, #5]
	b _0805FB54
	.align 2, 0
_0805FAD0: .4byte gUnk_080864BC
_0805FAD4: .4byte 0x020192E4
_0805FAD8:
	ldr r1, _0805FADC @ =0x080864CC
	b _0805FAFE
_0805FADC: .4byte gUnk_080864CC
_0805FAE0:
	cmp r4, #0
	beq _0805FAEA
	cmp r4, #1
	beq _0805FAFC
	b _0805FB8C
_0805FAEA:
	ldr r4, _0805FAF4 @ =0x080864E4
	ldr r0, _0805FAF8 @ =0x020192E4
	ldrb r5, [r0, #3]
	b _0805FB54
	.align 2, 0
_0805FAF4: .4byte gUnk_080864E4
_0805FAF8: .4byte 0x020192E4
_0805FAFC:
	ldr r1, _0805FB0C @ =0x080864F0
_0805FAFE:
	mov r0, #0xA
	mov r2, #0
	mov r3, #0
	bl sub_0805F64C
	b _0805FB8C
	.align 2, 0
_0805FB0C: .4byte gUnk_080864F0
_0805FB10:
	cmp r4, #0
	beq _0805FB1A
	cmp r4, #1
	beq _0805FB2C
	b _0805FB8C
_0805FB1A:
	ldr r4, _0805FB24 @ =0x08086500
	ldr r0, _0805FB28 @ =0x020192E4
	ldrb r5, [r0, #4]
	b _0805FB54
	.align 2, 0
_0805FB24: .4byte gUnk_08086500
_0805FB28: .4byte 0x020192E4
_0805FB2C:
	ldr r4, _0805FB38 @ =0x08086510
	ldr r0, _0805FB3C @ =0x020192E4
	ldr r1, _0805FB40 @ =0x00000D64
	add r0, r0, r1
	ldrb r5, [r0, #4]
	b _0805FB54
_0805FB38: .4byte gUnk_08086510
_0805FB3C: .4byte 0x020192E4
_0805FB40: .4byte 0x00000D64
_0805FB44:
	cmp r4, #0
	beq _0805FB4E
	cmp r4, #1
	beq _0805FB70
	b _0805FB8C
_0805FB4E:
	ldr r4, _0805FB68 @ =0x08086524
	ldr r0, _0805FB6C @ =0x020192E4
	ldrb r5, [r0, #6]
_0805FB54:
	add r0, r4, #0
	bl sub_080753CC
	add r3, r0, #0
	mov r0, #0xA
	add r1, r4, #0
	add r2, r5, #0
	bl sub_0805F64C
	b _0805FB8C
_0805FB68: .4byte gUnk_08086524
_0805FB6C: .4byte 0x020192E4
_0805FB70:
	ldr r4, _0805FB98 @ =0x08086538
	ldr r0, _0805FB9C @ =0x020192E4
	ldr r2, _0805FBA0 @ =0x00000D64
	add r0, r0, r2
	ldrb r5, [r0, #6]
	add r0, r4, #0
	bl sub_080753CC
	add r3, r0, #0
	mov r0, #0xA
	add r1, r4, #0
	add r2, r5, #0
	bl sub_0805F64C
_0805FB8C:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805FB98: .4byte gUnk_08086538
_0805FB9C: .4byte 0x020192E4
_0805FBA0: .4byte 0x00000D64
	thumb_func_end sub_0805F96C

