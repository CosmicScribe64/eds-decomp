	thumb_func_start sub_0805E788
sub_0805E788: @ 0x0805E788
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r1, _0805E7B8 @ =0x02018450
	mov r2, #0xAE
	lsl r2, r2, #1
	add r0, r1, r2
	ldrb r0, [r0]
	add r5, r1, #0
	cmp r0, #4
	bls _0805E7AE
	b _0805ECF0
_0805E7AE:
	lsl r0, r0, #2
	ldr r1, _0805E7BC @ =0x0805E7C0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0805E7B8: .4byte 0x02018450
_0805E7BC: .4byte 0x0805E7C0
_0805E7C0:
	.4byte _0805E7D4
	.4byte _0805E7F8
	.4byte _0805E85C
	.4byte _0805E8AA
	.4byte _0805EC6E
_0805E7D4:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xE0
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	ldr r1, _0805E83C @ =0x04000050
	ldr r6, _0805E840 @ =0x00003FFF
	add r0, r6, #0
	strh r0, [r1]
	mov r0, #0xAE
	lsl r0, r0, #1
	add r1, r5, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0805E7F8:
	ldr r1, _0805E844 @ =0x04000054
	ldr r2, _0805E848 @ =0x0000015D
	add r4, r5, r2
	mov r0, #0xF
	ldrb r3, [r4]
	sub r0, r0, r3
	strh r0, [r1]
	ldrb r2, [r4]
	cmp r2, #0xE
	bhi _0805E854
	add r3, r2, #1
	strb r3, [r4]
	ldr r1, _0805E84C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805E82A
	ldr r1, _0805E850 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805E82A
	b _0805EB6E
_0805E82A:
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xB
	bls _0805E834
	b _0805EB6E
_0805E834:
	add r0, r2, #4
	strb r0, [r4]
	b _0805EB6E
	.align 2, 0
_0805E83C: .4byte 0x04000050
_0805E840: .4byte 0x00003FFF
_0805E844: .4byte 0x04000054
_0805E848: .4byte 0x0000015D
_0805E84C: .4byte 0x03000040
_0805E850: .4byte 0x0201CFB0
_0805E854:
	mov r6, #0xAE
	lsl r6, r6, #1
	add r1, r5, r6
	b _0805EC66
_0805E85C:
	bl sub_080757F4
	ldr r4, _0805E8C4 @ =0x04000208
	mov r5, #0
	strh r5, [r4]
	ldr r2, _0805E8C8 @ =0x04000200
	ldrh r3, [r2]
	ldr r1, _0805E8CC @ =0x0000FFFD
	add r0, r1, #0
	and r0, r3
	strh r0, [r2]
	mov r3, #1
	strh r3, [r4]
	strh r5, [r4]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r1, _0805E8D0 @ =0x03000000
	mov r0, #0
	str r0, [r1, #4]
	strh r3, [r4]
	bl sub_0805DD3C
	ldr r0, _0805E8D4 @ =0x02018450
	ldr r2, _0805E8D8 @ =0x0000015D
	add r1, r0, r2
	strb r5, [r1]
	mov r3, #0xAF
	lsl r3, r3, #1
	add r1, r0, r3
	strb r5, [r1]
	ldr r6, _0805E8DC @ =0x0000015F
	add r1, r0, r6
	strb r5, [r1]
	sub r2, #1
	add r1, r0, r2
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0805E8AA:
	ldr r1, _0805E8D4 @ =0x02018450
	ldr r3, _0805E8D8 @ =0x0000015D
	add r0, r1, r3
	ldrb r0, [r0]
	add r5, r1, #0
	cmp r0, #4
	bls _0805E8BA
	b _0805EB6E
_0805E8BA:
	lsl r0, r0, #2
	ldr r1, _0805E8E0 @ =0x0805E8E4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0805E8C4: .4byte 0x04000208
_0805E8C8: .4byte 0x04000200
_0805E8CC: .4byte 0x0000FFFD
_0805E8D0: .4byte 0x03000000
_0805E8D4: .4byte 0x02018450
_0805E8D8: .4byte 0x0000015D
_0805E8DC: .4byte 0x0000015F
_0805E8E0: .4byte 0x0805E8E4
_0805E8E4:
	.4byte _0805E8F8
	.4byte _0805E90E
	.4byte _0805E978
	.4byte _0805EA94
	.4byte _0805EBBA
_0805E8F8:
	cmp r7, #0
	bne _0805E904
	mov r6, #0xAE
	lsl r6, r6, #1
	add r1, r5, r6
	b _0805EC66
_0805E904:
	ldr r0, _0805E954 @ =0x0000015D
	add r1, r5, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0805E90E:
	ldr r1, _0805E958 @ =0x0000015F
	add r4, r5, r1
	ldrb r2, [r4]
	cmp r2, #0x1D
	bhi _0805E964
	add r0, r7, #0
	mov r1, sp
	mov r2, #0
	bl sub_0805DEA4
	ldrb r3, [r4]
	add r2, r3, #1
	strb r2, [r4]
	ldr r1, _0805E95C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805E942
	ldr r1, _0805E960 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805E942
	b _0805EB6E
_0805E942:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x15
	bls _0805E94C
	b _0805EB6E
_0805E94C:
	add r0, r3, #0
	add r0, #8
	strb r0, [r4]
	b _0805EB6E
_0805E954: .4byte 0x0000015D
_0805E958: .4byte 0x0000015F
_0805E95C: .4byte 0x03000040
_0805E960: .4byte 0x0201CFB0
_0805E964:
	mov r0, #9
	bl sub_08077AEC
	mov r0, #0
	strb r0, [r4]
	ldr r3, _0805EA44 @ =0x0000015D
	add r1, r5, r3
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0805E978:
	add r0, r7, #0
	mov r1, sp
	mov r2, #1
	bl sub_0805DEA4
	ldr r1, _0805EA48 @ =0x02018450
	ldr r6, _0805EA4C @ =0x0000015F
	add r0, r1, r6
	ldrb r0, [r0]
	cmp r0, #0x1D
	bhi _0805EA5C
	mov r4, #0
	mov r6, #0
_0805E992:
	mov r0, #6
	lsl r0, r6
	and r0, r7
	cmp r0, #0
	beq _0805E9E0
	bl sub_08076F9C
	add r1, r0, #0
	ldr r5, _0805EA50 @ =0x081A4194
	add r0, r6, r5
	ldr r2, [r0]
	add r0, r1, #0
	cmp r1, #0
	bge _0805E9B0
	add r0, #0xF
_0805E9B0:
	asr r0, r0, #4
	lsl r0, r0, #4
	sub r0, r1, r0
	sub r0, #8
	lsl r0, r0, #8
	str r0, [r2]
	bl sub_08076F9C
	add r1, r0, #0
	lsl r0, r4, #1
	add r0, #1
	lsl r0, r0, #2
	add r0, r0, r5
	ldr r2, [r0]
	add r0, r1, #0
	cmp r1, #0
	bge _0805E9D4
	add r0, #0xF
_0805E9D4:
	asr r0, r0, #4
	lsl r0, r0, #4
	sub r0, r1, r0
	sub r0, #8
	lsl r0, r0, #8
	str r0, [r2]
_0805E9E0:
	mov r0, #4
	lsl r0, r6
	and r0, r7
	cmp r0, #0
	beq _0805EA04
	mov r0, #1
	sub r0, r0, r4
	lsl r0, r0, #2
	add r0, sp
	lsl r2, r4, #2
	add r2, sp
	ldr r1, [r0]
	ldr r0, [r2]
	sub r1, r1, r0
	add r0, r4, #0
	mov r2, #1
	bl sub_0805DF04
_0805EA04:
	add r6, #8
	add r4, #1
	cmp r4, #1
	ble _0805E992
	ldr r0, _0805EA48 @ =0x02018450
	ldr r1, _0805EA4C @ =0x0000015F
	add r3, r0, r1
	ldrb r4, [r3]
	add r2, r4, #1
	strb r2, [r3]
	ldr r1, _0805EA54 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805EA32
	ldr r1, _0805EA58 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805EA32
	b _0805EB6E
_0805EA32:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x15
	bls _0805EA3C
	b _0805EB6E
_0805EA3C:
	add r0, r4, #0
	add r0, #8
	strb r0, [r3]
	b _0805EB6E
_0805EA44: .4byte 0x0000015D
_0805EA48: .4byte 0x02018450
_0805EA4C: .4byte 0x0000015F
_0805EA50: .4byte gUnk_081A4194
_0805EA54: .4byte 0x03000040
_0805EA58: .4byte 0x0201CFB0
_0805EA5C:
	mov r4, #0
	add r5, r1, #0
	mov r6, #6
	mov r3, #0
	ldr r2, _0805EB74 @ =0x081A4194
_0805EA66:
	lsl r1, r4, #3
	add r0, r6, #0
	lsl r0, r1
	and r0, r7
	cmp r0, #0
	beq _0805EA7A
	ldr r0, [r2]
	str r3, [r0]
	ldr r0, [r2, #4]
	str r3, [r0]
_0805EA7A:
	add r2, #8
	add r4, #1
	cmp r4, #1
	ble _0805EA66
	ldr r2, _0805EB78 @ =0x0000015F
	add r1, r5, r2
	mov r0, #0
	strb r0, [r1]
	ldr r3, _0805EB7C @ =0x0000015D
	add r1, r5, r3
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0805EA94:
	add r0, r7, #0
	mov r1, sp
	mov r2, #1
	bl sub_0805DEA4
	ldr r2, _0805EB80 @ =0x02018450
	ldr r6, _0805EB78 @ =0x0000015F
	add r4, r2, r6
	ldrb r3, [r4]
	cmp r3, #0x1F
	bhi _0805EBA4
	ldr r2, _0805EB84 @ =0x04000050
	mov r0, #0xC0
	strh r0, [r2]
	mov r5, #2
	add r0, r7, #0
	and r0, r5
	cmp r0, #0
	beq _0805EAC4
	ldrh r0, [r2]
	ldr r6, _0805EB88 @ =0x00000404
	add r1, r6, #0
	orr r0, r1
	strh r0, [r2]
_0805EAC4:
	mov r0, #0x80
	lsl r0, r0, #2
	and r0, r7
	cmp r0, #0
	beq _0805EAD8
	ldrh r0, [r2]
	ldr r6, _0805EB8C @ =0x00000808
	add r1, r6, #0
	orr r0, r1
	strh r0, [r2]
_0805EAD8:
	ldr r1, _0805EB90 @ =0x04000054
	ldrb r0, [r4]
	strh r0, [r1]
	add r2, r3, #1
	strb r2, [r4]
	ldr r1, _0805EB94 @ =0x03000040
	add r0, r5, #0
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805EAFA
	ldr r1, _0805EB98 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805EB08
_0805EAFA:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x15
	bhi _0805EB08
	add r0, r3, #0
	add r0, #8
	strb r0, [r4]
_0805EB08:
	ldr r0, _0805EB80 @ =0x02018450
	ldr r1, _0805EB78 @ =0x0000015F
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x20
	bne _0805EB3E
	mov r0, #2
	and r0, r7
	cmp r0, #0
	beq _0805EB28
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0805EB9C @ =0x0000FBFF
	and r0, r1
	strh r0, [r2]
_0805EB28:
	mov r0, #0x80
	lsl r0, r0, #2
	and r0, r7
	cmp r0, #0
	beq _0805EB3E
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0805EBA0 @ =0x0000F7FF
	and r0, r1
	strh r0, [r2]
_0805EB3E:
	mov r4, #0
	mov r6, sp
	mov r5, sp
_0805EB44:
	lsl r1, r4, #3
	mov r0, #4
	lsl r0, r1
	and r0, r7
	cmp r0, #0
	beq _0805EB66
	mov r0, #1
	sub r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r1, [r0]
	ldr r0, [r5]
	sub r1, r1, r0
	add r0, r4, #0
	mov r2, #0
	bl sub_0805DF04
_0805EB66:
	add r5, #4
	add r4, #1
	cmp r4, #1
	ble _0805EB44
_0805EB6E:
	mov r0, #0
	b _0805ECF2
	.align 2, 0
_0805EB74: .4byte gUnk_081A4194
_0805EB78: .4byte 0x0000015F
_0805EB7C: .4byte 0x0000015D
_0805EB80: .4byte 0x02018450
_0805EB84: .4byte 0x04000050
_0805EB88: .4byte 0x00000404
_0805EB8C: .4byte 0x00000808
_0805EB90: .4byte 0x04000054
_0805EB94: .4byte 0x03000040
_0805EB98: .4byte 0x0201CFB0
_0805EB9C: .4byte 0x0000FBFF
_0805EBA0: .4byte 0x0000F7FF
_0805EBA4:
	ldr r1, _0805EC0C @ =0x04000050
	mov r0, #0
	strh r0, [r1]
	add r1, #4
	strh r0, [r1]
	strb r0, [r4]
	ldr r3, _0805EC10 @ =0x0000015D
	add r1, r2, r3
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_0805EBBA:
	add r0, r7, #0
	mov r1, sp
	mov r2, #1
	bl sub_0805DEA4
	mov r4, #0
	mov r6, sp
	mov r5, sp
_0805EBCA:
	lsl r1, r4, #3
	mov r0, #4
	lsl r0, r1
	and r0, r7
	cmp r0, #0
	beq _0805EBEC
	mov r0, #1
	sub r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r1, [r0]
	ldr r0, [r5]
	sub r1, r1, r0
	add r0, r4, #0
	mov r2, #0
	bl sub_0805DF04
_0805EBEC:
	add r5, #4
	add r4, #1
	cmp r4, #1
	ble _0805EBCA
	ldr r1, _0805EC14 @ =0x03000040
	mov r5, #2
	add r0, r5, #0
	ldrh r6, [r1, #6]
	and r0, r6
	cmp r0, #0
	beq _0805EC1C
	ldr r0, _0805EC18 @ =0x02018450
	mov r1, #0xAE
	lsl r1, r1, #1
	add r0, r0, r1
	b _0805ECE2
_0805EC0C: .4byte 0x04000050
_0805EC10: .4byte 0x0000015D
_0805EC14: .4byte 0x03000040
_0805EC18: .4byte 0x02018450
_0805EC1C:
	ldr r0, _0805EC54 @ =0x02018450
	ldr r2, _0805EC58 @ =0x0000015F
	add r4, r0, r2
	ldrb r2, [r4]
	cmp r2, #0x3B
	bhi _0805EC60
	add r3, r2, #1
	strb r3, [r4]
	add r0, r5, #0
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805EC42
	ldr r1, _0805EC5C @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805EB6E
_0805EC42:
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x33
	bhi _0805EB6E
	add r0, r2, #0
	add r0, #8
	strb r0, [r4]
	b _0805EB6E
	.align 2, 0
_0805EC54: .4byte 0x02018450
_0805EC58: .4byte 0x0000015F
_0805EC5C: .4byte 0x0201CFB0
_0805EC60:
	mov r3, #0xAE
	lsl r3, r3, #1
	add r1, r0, r3
_0805EC66:
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0805EB6E
_0805EC6E:
	add r0, r7, #0
	mov r1, sp
	mov r2, #1
	bl sub_0805DEA4
	mov r4, #0
	mov r6, sp
	mov r5, sp
_0805EC7E:
	lsl r1, r4, #3
	mov r0, #4
	lsl r0, r1
	and r0, r7
	cmp r0, #0
	beq _0805ECA0
	mov r0, #1
	sub r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r1, [r0]
	ldr r0, [r5]
	sub r1, r1, r0
	add r0, r4, #0
	mov r2, #0
	bl sub_0805DF04
_0805ECA0:
	add r5, #4
	add r4, #1
	cmp r4, #1
	ble _0805EC7E
	ldr r1, _0805ECC4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805ECC0
	ldr r1, _0805ECC8 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805ECCC
_0805ECC0:
	mov r0, #4
	b _0805ECCE
_0805ECC4: .4byte 0x03000040
_0805ECC8: .4byte 0x0201CFB0
_0805ECCC:
	mov r0, #1
_0805ECCE:
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805ECDA
	b _0805EB6E
_0805ECDA:
	ldr r0, _0805ECEC @ =0x02018450
	mov r6, #0xAE
	lsl r6, r6, #1
	add r0, r0, r6
_0805ECE2:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0805EB6E
	.align 2, 0
_0805ECEC: .4byte 0x02018450
_0805ECF0:
	mov r0, #1
_0805ECF2:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0805E788
	.align 2, 0

