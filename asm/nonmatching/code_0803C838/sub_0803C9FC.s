	thumb_func_start sub_0803C9FC
sub_0803C9FC: @ 0x0803C9FC
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r2, [r4, #4]
	and r0, r2
	cmp r0, #0
	bne _0803CAFC
	ldr r6, _0803CA24 @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r5, r6, r3
	ldrb r0, [r5]
	cmp r0, #0x7E
	beq _0803CA80
	cmp r0, #0x7E
	bgt _0803CA28
	cmp r0, #0x7D
	beq _0803CAAC
	b _0803CAFC
	.align 2, 0
_0803CA24: .4byte 0x02017A40
_0803CA28:
	cmp r0, #0x7F
	beq _0803CA4A
	cmp r0, #0x80
	bne _0803CAFC
	add r0, r4, #0
	mov r2, #0
	bl sub_0802FFE4
	cmp r0, #0
	beq _0803CAFC
	ldr r1, _0803CA6C @ =0x000003E1
	add r0, r6, r1
	mov r1, #3
	strb r1, [r0]
	ldrb r0, [r5]
	sub r0, #1
	strb r0, [r5]
_0803CA4A:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803CA70 @ =0x0000060D
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	beq _0803CAFC
	ldr r0, _0803CA74 @ =0x00000206
	ldr r1, _0803CA78 @ =0x00000613
	ldr r3, _0803CA7C @ =0x08083B24
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7E
	b _0803CAFE
_0803CA6C: .4byte 0x000003E1
_0803CA70: .4byte 0x0000060D
_0803CA74: .4byte 0x00000206
_0803CA78: .4byte 0x00000613
_0803CA7C: .4byte gUnk_08083B24
_0803CA80:
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803CAA4 @ =0x000007FF
	ldrh r4, [r4]
	and r2, r4
	lsl r2, r2, #1
	ldr r3, _0803CAA8 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7D
	b _0803CAFE
	.align 2, 0
_0803CAA4: .4byte 0x000007FF
_0803CAA8: .4byte gUnk_08622AB4
_0803CAAC:
	ldr r0, _0803CAEC @ =0x0201D810
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
	mov r3, #0xDE
	cmp r0, #0
	beq _0803CACC
	ldr r3, _0803CAF0 @ =0x000080DE
_0803CACC:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _0803CAF4 @ =0x000003E1
	add r1, r6, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	cmp r0, #0
	beq _0803CAF8
	mov r0, #0x7F
	b _0803CAFE
_0803CAEC: .4byte 0x0201D810
_0803CAF0: .4byte 0x000080DE
_0803CAF4: .4byte 0x000003E1
_0803CAF8:
	mov r0, #0xA
	b _0803CAFE
_0803CAFC:
	mov r0, #0
_0803CAFE:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0803C9FC

