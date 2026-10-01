	thumb_func_start sub_0803FBEC
sub_0803FBEC: @ 0x0803FBEC
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _0803FC1C @ =0x02017A40
	ldr r1, _0803FC20 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _0803FC30
	ldr r0, _0803FC24 @ =0x00000206
	ldr r1, _0803FC28 @ =0x00000712
	ldr r3, _0803FC2C @ =0x08084470
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0803FC7A
_0803FC1C: .4byte 0x02017A40
_0803FC20: .4byte 0x000003E5
_0803FC24: .4byte 0x00000206
_0803FC28: .4byte 0x00000712
_0803FC2C: .4byte gUnk_08084470
_0803FC30:
	ldr r1, _0803FC44 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803FC48
	mov r0, #0
	strb r0, [r5]
	b _0803FC7A
	.align 2, 0
_0803FC44: .4byte 0x03000040
_0803FC48:
	ldr r0, _0803FC58 @ =0x00080008
	bl sub_08052F38
	cmp r0, #0
	bne _0803FC5C
	mov r0, #0
	b _0803FC7A
	.align 2, 0
_0803FC58: .4byte 0x00080008
_0803FC5C:
	ldr r0, _0803FC80 @ =0x0201CFB0
	ldr r3, _0803FC84 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	mov r0, #1
_0803FC7A:
	pop {r4, r5}
	pop {r1}
	bx r1
_0803FC80: .4byte 0x0201CFB0
_0803FC84: .4byte 0x00000824
	thumb_func_end sub_0803FBEC

