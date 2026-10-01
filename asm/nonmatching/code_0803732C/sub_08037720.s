	thumb_func_start sub_08037720
sub_08037720: @ 0x08037720
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _08037730
	b _080378B4
_08037730:
	ldr r1, _08037750 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	add r2, r1, #0
	cmp r0, #4
	bls _08037744
	b _080378B4
_08037744:
	lsl r0, r0, #2
	ldr r1, _08037754 @ =0x08037758
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08037750: .4byte 0x02017A40
_08037754: .4byte 0x08037758
_08037758:
	.4byte _08037840
	.4byte _08037824
	.4byte _080377FC
	.4byte _08037780
	.4byte _0803776C
_0803776C:
	ldr r0, _080377C0 @ =0x000003E1
	add r1, r2, r0
	mov r0, #3
	strb r0, [r1]
	mov r0, #0xF8
	lsl r0, r0, #2
	add r1, r2, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_08037780:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080377C4 @ =0x0000047B
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	bne _08037794
	b _080378B4
_08037794:
	mov r0, #1
	ldrb r2, [r6, #2]
	and r0, r2
	cmp r0, #0
	beq _080377D4
	ldrh r0, [r6]
	bl sub_08056ECC
	ldr r1, _080377C8 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1, #5]
	and r0, r2
	strb r0, [r1, #5]
	ldr r0, _080377CC @ =0x02015F00
	ldr r2, _080377D0 @ =0x00001B22
	add r0, r0, r2
	ldrh r0, [r0]
	strh r0, [r1, #6]
	mov r0, #0x7C
	b _080378B6
	.align 2, 0
_080377C0: .4byte 0x000003E1
_080377C4: .4byte 0x0000047B
_080377C8: .4byte 0x0201D810
_080377CC: .4byte 0x02015F00
_080377D0: .4byte 0x00001B22
_080377D4:
	ldr r0, _080377F0 @ =0x00000206
	ldr r1, _080377F4 @ =0x00000712
	ldr r3, _080377F8 @ =0x08083164
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7E
	b _080378B6
	.align 2, 0
_080377F0: .4byte 0x00000206
_080377F4: .4byte 0x00000712
_080377F8: .4byte gUnk_08083164
_080377FC:
	ldr r0, _08037814 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _080378B4
	ldr r0, _08037818 @ =0x00000206
	ldr r1, _0803781C @ =0x00000712
	ldr r3, _08037820 @ =0x080831A8
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7D
	b _080378B6
_08037814: .4byte 0x0201AE60
_08037818: .4byte 0x00000206
_0803781C: .4byte 0x00000712
_08037820: .4byte gUnk_080831A8
_08037824:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803783C @ =0x0000047B
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7C
	b _080378B6
	.align 2, 0
_0803783C: .4byte 0x0000047B
_08037840:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _080378A0 @ =0x0201D810
	ldrb r2, [r4, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r2, [r4, #6]
	add r1, r2, r1
	lsl r1, r1, #2
	add r5, r4, #0
	add r5, #0xC
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x15
	lsr r1, r1, #0x14
	ldr r2, _080378A4 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	bl sub_08019554
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldrb r2, [r4, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r4, [r4, #6]
	add r1, r4, r1
	lsl r1, r1, #2
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019820
	ldr r0, _080378A8 @ =0x02017A40
	ldr r1, _080378AC @ =0x000003E1
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #1
	strb r1, [r0]
	lsl r1, r1, #0x18
	cmp r1, #0
	beq _080378B0
	mov r0, #0x7F
	b _080378B6
	.align 2, 0
_080378A0: .4byte 0x0201D810
_080378A4: .4byte gUnk_08622AB4
_080378A8: .4byte 0x02017A40
_080378AC: .4byte 0x000003E1
_080378B0:
	mov r0, #0x64
	b _080378B6
_080378B4:
	mov r0, #0
_080378B6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08037720

