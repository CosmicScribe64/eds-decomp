	thumb_func_start sub_0803DEB8
sub_0803DEB8: @ 0x0803DEB8
	push {r4, r5, r6, lr}
	add r6, r0, #0
	ldrb r1, [r6, #2]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _0803DF08
	mov r0, #8
	neg r0, r0
	ldrb r1, [r6, #0xA]
	and r0, r1
	strb r0, [r6, #0xA]
	ldrh r0, [r6]
	bl sub_08056ECC
	cmp r0, #0
	blt _0803DF58
	lsl r1, r0, #2
	ldr r0, _0803DF04 @ =0x0201D81C
	add r4, r1, r0
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019820
	ldrh r1, [r4]
	add r0, r6, #0
	bl sub_0803DD7C
	ldrh r1, [r4, #2]
	add r0, r6, #0
	bl sub_0803DD7C
	b _0803DF58
	.align 2, 0
_0803DF04: .4byte 0x0201D81C
_0803DF08:
	ldr r0, _0803DF5C @ =0x02017A40
	ldr r3, _0803DF60 @ =0x000003E5
	add r4, r0, r3
	ldrb r0, [r4]
	cmp r0, #0
	beq _0803DF68
	cmp r0, #1
	beq _0803DFB0
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _0803DF64 @ =0x0201D810
	ldrb r2, [r4, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r4, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r5, r4, #0
	add r5, #0xC
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019820
	ldrb r1, [r4, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	ldrh r4, [r4, #6]
	add r0, r4, r0
	lsl r0, r0, #2
	add r4, r0, r5
	ldrh r1, [r4]
	add r0, r6, #0
	bl sub_0803DD7C
	ldrh r1, [r4, #2]
	add r0, r6, #0
	bl sub_0803DD7C
_0803DF58:
	mov r0, #1
	b _0803DFD4
_0803DF5C: .4byte 0x02017A40
_0803DF60: .4byte 0x000003E5
_0803DF64: .4byte 0x0201D810
_0803DF68:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r6, #0xA]
	and r0, r2
	strb r0, [r6, #0xA]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803DF9C @ =0x000007FF
	ldrh r6, [r6]
	and r1, r6
	lsl r1, r1, #1
	ldr r3, _0803DFA0 @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	beq _0803DF58
	ldr r0, _0803DFA4 @ =0x00000206
	ldr r1, _0803DFA8 @ =0x00000712
	ldr r3, _0803DFAC @ =0x08083C50
	mov r2, #0xB
	bl sub_080602A4
	b _0803DFCC
_0803DF9C: .4byte 0x000007FF
_0803DFA0: .4byte gUnk_08622AB4
_0803DFA4: .4byte 0x00000206
_0803DFA8: .4byte 0x00000712
_0803DFAC: .4byte gUnk_08083C50
_0803DFB0:
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803DFDC @ =0x000007FF
	ldrh r6, [r6]
	and r2, r6
	lsl r2, r2, #1
	ldr r3, _0803DFE0 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
_0803DFCC:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_0803DFD4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803DFDC: .4byte 0x000007FF
_0803DFE0: .4byte gUnk_08622AB4
	thumb_func_end sub_0803DEB8

