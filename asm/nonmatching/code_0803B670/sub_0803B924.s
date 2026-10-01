	thumb_func_start sub_0803B924
sub_0803B924: @ 0x0803B924
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _0803B934
	b _0803BAE0
_0803B934:
	ldr r0, _0803B950 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #0x7A
	cmp r0, #6
	bls _0803B946
	b _0803BAE0
_0803B946:
	lsl r0, r0, #2
	ldr r1, _0803B954 @ =0x0803B958
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0803B950: .4byte 0x02017A40
_0803B954: .4byte 0x0803B958
_0803B958:
	.4byte _0803BABC
	.4byte _0803BA94
	.4byte _0803BA5C
	.4byte _0803BA30
	.4byte _0803BA14
	.4byte _0803B9C8
	.4byte _0803B974
_0803B974:
	ldrb r1, [r4, #2]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _0803B980
	b _0803BAE0
_0803B980:
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xBD
	lsl r1, r1, #3
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	bne _0803B994
	b _0803BAE0
_0803B994:
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	bl sub_08008AF8
	cmp r0, #0
	bne _0803B9AA
	b _0803BAE0
_0803B9AA:
	ldr r0, _0803B9BC @ =0x00000206
	ldr r1, _0803B9C0 @ =0x00000712
	ldr r3, _0803B9C4 @ =0x080838D4
	mov r2, #0xB
	bl sub_080602A4
_0803B9B6:
	mov r0, #0x7F
	b _0803BAE2
	.align 2, 0
_0803B9BC: .4byte 0x00000206
_0803B9C0: .4byte 0x00000712
_0803B9C4: .4byte gUnk_080838D4
_0803B9C8:
	mov r0, #0xF0
	bl sub_08052F38
	cmp r0, #0
	beq _0803B9B6
	ldr r0, _0803BA04 @ =0x0201CFB0
	ldr r1, _0803BA08 @ =0x0000082C
	add r5, r0, r1
	ldrh r2, [r4, #2]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1A
	ldr r1, [r5]
	cmp r1, r0
	beq _0803BA0C
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A6C
	cmp r0, #0
	beq _0803BA0C
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r5]
	bl sub_08017FF4
	mov r0, #0x7E
	b _0803BAE2
	.align 2, 0
_0803BA04: .4byte 0x0201CFB0
_0803BA08: .4byte 0x0000082C
_0803BA0C:
	mov r0, #3
	bl sub_08077AEC
	b _0803B9B6
_0803BA14:
	ldr r0, _0803BA24 @ =0x00000206
	ldr r1, _0803BA28 @ =0x00000712
	ldr r3, _0803BA2C @ =0x08083904
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7D
	b _0803BAE2
_0803BA24: .4byte 0x00000206
_0803BA28: .4byte 0x00000712
_0803BA2C: .4byte gUnk_08083904
_0803BA30:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803BA54 @ =0x000007FF
	ldrh r4, [r4]
	and r2, r4
	lsl r2, r2, #1
	ldr r3, _0803BA58 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7C
	b _0803BAE2
	.align 2, 0
_0803BA54: .4byte 0x000007FF
_0803BA58: .4byte gUnk_08622AB4
_0803BA5C:
	ldr r0, _0803BA8C @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0xDC
	cmp r0, #0
	beq _0803BA7C
	ldr r3, _0803BA90 @ =0x000080DC
_0803BA7C:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x7B
	b _0803BAE2
_0803BA8C: .4byte 0x0201D810
_0803BA90: .4byte 0x000080DC
_0803BA94:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0803BAB8 @ =0x0201D810
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	mov r2, #1
	mov r3, #0
	bl sub_08056094
	mov r0, #0x7A
	b _0803BAE2
_0803BAB8: .4byte 0x0201D810
_0803BABC:
	ldrb r0, [r4, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r4]
	add r2, r0, #0
	ldr r3, _0803BADC @ =0x0201CF90
	ldrb r3, [r3]
	lsl r3, r3, #0x1A
	lsr r3, r3, #0x1B
	lsl r3, r3, #8
	orr r2, r3
	mov r3, #3
	bl sub_08017AB4
	mov r0, #0x78
	b _0803BAE2
_0803BADC: .4byte 0x0201CF90
_0803BAE0:
	mov r0, #0
_0803BAE2:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0803B924

