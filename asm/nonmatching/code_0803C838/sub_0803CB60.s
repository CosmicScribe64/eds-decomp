	thumb_func_start sub_0803CB60
sub_0803CB60: @ 0x0803CB60
	push {r4, r5, r6, r7, lr}
	ldr r5, _0803CBAC @ =0x0819A7C8
	ldr r4, _0803CBB0 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r3, _0803CBB4 @ =0x08622AB4
	add r0, r0, r3
	ldrh r7, [r0]
	and r1, r4
	lsl r1, r1, #1
	add r1, r1, r3
	ldrh r6, [r1]
	and r2, r4
	lsl r2, r2, #1
	add r2, r2, r3
	ldrh r4, [r2]
	add r0, r6, #0
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CB98
	add r0, r4, #0
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803CBA8
_0803CB98:
	ldr r1, [r5]
	ldr r0, _0803CBB8 @ =0x03E703E7
	cmp r1, r0
	bne _0803CBC0
	ldr r0, _0803CBBC @ =0x000003E7
	ldrh r1, [r5, #4]
	cmp r1, r0
	bne _0803CBC0
_0803CBA8:
	mov r0, #0
	b _0803CC12
_0803CBAC: .4byte gUnk_0819A7C8
_0803CBB0: .4byte 0x000007FF
_0803CBB4: .4byte gUnk_08622AB4
_0803CBB8: .4byte 0x03E703E7
_0803CBBC: .4byte 0x000003E7
_0803CBC0:
	ldrh r0, [r5]
	cmp r0, r7
	bne _0803CC0E
	ldrh r0, [r5, #2]
	ldrh r1, [r5, #4]
	cmp r6, r0
	bne _0803CBD2
	cmp r4, r1
	beq _0803CC0A
_0803CBD2:
	cmp r6, r1
	bne _0803CBDA
	cmp r4, r0
	beq _0803CC0A
_0803CBDA:
	add r0, r6, #0
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CBF2
	ldrh r1, [r5, #2]
	cmp r4, r1
	beq _0803CC0A
	ldrh r0, [r5, #4]
	cmp r4, r0
	beq _0803CC0A
_0803CBF2:
	add r0, r4, #0
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC0E
	ldrh r1, [r5, #2]
	cmp r6, r1
	beq _0803CC0A
	ldrh r0, [r5, #4]
	cmp r6, r0
	bne _0803CC0E
_0803CC0A:
	mov r0, #1
	b _0803CC12
_0803CC0E:
	add r5, #8
	b _0803CB98
_0803CC12:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803CB60

