	thumb_func_start sub_0803B7C4
sub_0803B7C4: @ 0x0803B7C4
	push {r4, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	beq _0803B7D4
	b _0803B91C
_0803B7D4:
	ldr r1, _0803B7F4 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	add r2, r1, #0
	cmp r0, #4
	bls _0803B7E8
	b _0803B91C
_0803B7E8:
	lsl r0, r0, #2
	ldr r1, _0803B7F8 @ =0x0803B7FC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803B7F4: .4byte 0x02017A40
_0803B7F8: .4byte 0x0803B7FC
_0803B7FC:
	.4byte _0803B8D0
	.4byte _0803B8A4
	.4byte _0803B87C
	.4byte _0803B824
	.4byte _0803B810
_0803B810:
	ldr r0, _0803B864 @ =0x000003E1
	add r1, r2, r0
	mov r0, #2
	strb r0, [r1]
	mov r0, #0xF8
	lsl r0, r0, #2
	add r1, r2, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_0803B824:
	ldr r1, _0803B864 @ =0x000003E1
	add r4, r2, r1
	ldrb r0, [r4]
	cmp r0, #0
	beq _0803B91C
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803B868 @ =0x000005E7
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	beq _0803B91C
	ldr r3, _0803B86C @ =0x08083828
	ldrb r4, [r4]
	cmp r4, #2
	bne _0803B84A
	ldr r3, _0803B870 @ =0x080837D4
_0803B84A:
	ldr r0, _0803B874 @ =0x00000206
	ldr r1, _0803B878 @ =0x00000712
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7E
	b _0803B91E
	.align 2, 0
_0803B864: .4byte 0x000003E1
_0803B868: .4byte 0x000005E7
_0803B86C: .4byte gUnk_08083828
_0803B870: .4byte gUnk_080837D4
_0803B874: .4byte 0x00000206
_0803B878: .4byte 0x00000712
_0803B87C:
	ldr r0, _0803B894 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0803B91C
	ldr r0, _0803B898 @ =0x00000206
	ldr r1, _0803B89C @ =0x00000712
	ldr r3, _0803B8A0 @ =0x08083880
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7D
	b _0803B91E
_0803B894: .4byte 0x0201AE60
_0803B898: .4byte 0x00000206
_0803B89C: .4byte 0x00000712
_0803B8A0: .4byte gUnk_08083880
_0803B8A4:
	ldrb r2, [r3, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803B8C8 @ =0x000007FF
	ldrh r3, [r3]
	and r2, r3
	lsl r2, r2, #1
	ldr r3, _0803B8CC @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7C
	b _0803B91E
	.align 2, 0
_0803B8C8: .4byte 0x000007FF
_0803B8CC: .4byte gUnk_08622AB4
_0803B8D0:
	ldr r0, _0803B90C @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r2, [r0, #6]
	add r1, r2, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r3, [r3, #2]
	and r0, r3
	mov r3, #0xD4
	cmp r0, #0
	bne _0803B8F0
	ldr r3, _0803B910 @ =0x000080D4
_0803B8F0:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _0803B914 @ =0x02017A40
	ldr r3, _0803B918 @ =0x000003E1
	add r0, r0, r3
	ldrb r1, [r0]
	sub r1, #1
	strb r1, [r0]
	mov r0, #0x80
	b _0803B91E
_0803B90C: .4byte 0x0201D810
_0803B910: .4byte 0x000080D4
_0803B914: .4byte 0x02017A40
_0803B918: .4byte 0x000003E1
_0803B91C:
	mov r0, #0
_0803B91E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0803B7C4

