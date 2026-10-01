	thumb_func_start sub_0803BAE8
sub_0803BAE8: @ 0x0803BAE8
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _0803BAF8
	b _0803BCD4
_0803BAF8:
	ldr r1, _0803BB18 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x78
	add r3, r1, #0
	cmp r0, #8
	bls _0803BB0C
	b _0803BCB6
_0803BB0C:
	lsl r0, r0, #2
	ldr r1, _0803BB1C @ =0x0803BB20
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803BB18: .4byte 0x02017A40
_0803BB1C: .4byte 0x0803BB20
_0803BB20:
	.4byte _0803BC8C
	.4byte _0803BCB6
	.4byte _0803BCB6
	.4byte _0803BCB6
	.4byte _0803BC24
	.4byte _0803BBF8
	.4byte _0803BBCC
	.4byte _0803BB60
	.4byte _0803BB44
_0803BB44:
	ldr r6, _0803BBA8 @ =0x000003E1
	add r1, r3, r6
	mov r2, #0
	mov r0, #3
	strb r0, [r1]
	ldr r1, _0803BBAC @ =0x000003E2
	add r0, r3, r1
	strb r2, [r0]
	mov r2, #0xF8
	lsl r2, r2, #2
	add r1, r3, r2
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_0803BB60:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803BBB0 @ =0x000007FF
	ldrh r5, [r5]
	and r1, r5
	lsl r1, r1, #1
	ldr r6, _0803BBB4 @ =0x08622AB4
	add r1, r1, r6
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	ble _0803BBD4
	ldr r0, _0803BBB8 @ =0x02017A40
	ldr r1, _0803BBAC @ =0x000003E2
	add r0, r0, r1
	ldrb r0, [r0]
	ldr r3, _0803BBBC @ =0x080839A4
	cmp r0, #0
	beq _0803BB8E
	ldr r3, _0803BBC0 @ =0x08083960
_0803BB8E:
	ldr r0, _0803BBC4 @ =0x00000206
	ldr r1, _0803BBC8 @ =0x00000712
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7E
	b _0803BCD6
	.align 2, 0
_0803BBA8: .4byte 0x000003E1
_0803BBAC: .4byte 0x000003E2
_0803BBB0: .4byte 0x000007FF
_0803BBB4: .4byte gUnk_08622AB4
_0803BBB8: .4byte 0x02017A40
_0803BBBC: .4byte gUnk_080839A4
_0803BBC0: .4byte gUnk_08083960
_0803BBC4: .4byte 0x00000206
_0803BBC8: .4byte 0x00000712
_0803BBCC:
	ldr r0, _0803BBD8 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0803BBDC
_0803BBD4:
	mov r0, #0x78
	b _0803BCD6
_0803BBD8: .4byte 0x0201AE60
_0803BBDC:
	ldr r0, _0803BBEC @ =0x00000206
	ldr r1, _0803BBF0 @ =0x00000712
	ldr r3, _0803BBF4 @ =0x080839DC
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7D
	b _0803BCD6
_0803BBEC: .4byte 0x00000206
_0803BBF0: .4byte 0x00000712
_0803BBF4: .4byte gUnk_080839DC
_0803BBF8:
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803BC1C @ =0x000007FF
	ldrh r5, [r5]
	and r2, r5
	lsl r2, r2, #1
	ldr r3, _0803BC20 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7C
	b _0803BCD6
	.align 2, 0
_0803BC1C: .4byte 0x000007FF
_0803BC20: .4byte gUnk_08622AB4
_0803BC24:
	ldr r2, _0803BC78 @ =0x0201D810
	ldrb r6, [r2, #5]
	lsl r0, r6, #0x1E
	lsr r1, r0, #0x1E
	ldrh r3, [r2, #6]
	add r1, r1, r3
	lsl r1, r1, #2
	add r2, #0xC
	add r4, r1, r2
	lsr r0, r0, #0x1E
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x13
	mov r3, #0xD4
	cmp r0, #0
	bge _0803BC4A
	ldr r3, _0803BC7C @ =0x000080D4
_0803BC4A:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _0803BC80 @ =0x02017A40
	ldr r2, _0803BC84 @ =0x000003E1
	add r1, r0, r2
	ldrb r2, [r1]
	sub r2, #1
	strb r2, [r1]
	ldr r3, _0803BC88 @ =0x000003E2
	add r0, r0, r3
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	lsl r2, r2, #0x18
	cmp r2, #0
	beq _0803BBD4
	mov r0, #0x7F
	b _0803BCD6
	.align 2, 0
_0803BC78: .4byte 0x0201D810
_0803BC7C: .4byte 0x000080D4
_0803BC80: .4byte 0x02017A40
_0803BC84: .4byte 0x000003E1
_0803BC88: .4byte 0x000003E2
_0803BC8C:
	ldr r6, _0803BCDC @ =0x000003E2
	add r4, r3, r6
	ldrb r0, [r4]
	cmp r0, #0
	beq _0803BCB6
	ldrb r0, [r5, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r5]
	add r2, r0, #0
	ldrh r6, [r5, #2]
	lsl r3, r6, #0x16
	lsr r3, r3, #0x1A
	lsl r3, r3, #8
	orr r2, r3
	ldrb r4, [r4]
	lsl r3, r4, #8
	mov r4, #0xB
	orr r3, r4
	bl sub_08017AB4
_0803BCB6:
	mov r0, #1
	ldrb r1, [r5, #2]
	and r0, r1
	mov r2, #0x92
	cmp r0, #0
	beq _0803BCC4
	ldr r2, _0803BCE0 @ =0x00008092
_0803BCC4:
	ldrh r5, [r5, #2]
	lsl r1, r5, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0803BCD4:
	mov r0, #0
_0803BCD6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0803BCDC: .4byte 0x000003E2
_0803BCE0: .4byte 0x00008092
	thumb_func_end sub_0803BAE8

