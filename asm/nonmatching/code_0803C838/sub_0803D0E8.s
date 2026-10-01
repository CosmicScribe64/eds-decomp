	thumb_func_start sub_0803D0E8
sub_0803D0E8: @ 0x0803D0E8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	add r6, r2, #0
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	ldr r2, _0803D124 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _0803D128 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0803D110
	b _0803D3C2
_0803D110:
	lsl r0, r2, #1
	ldr r2, _0803D12C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0803D130 @ =0x00000776
	cmp r1, r0
	bne _0803D134
	mov r0, #3
	b _0803D196
	.align 2, 0
_0803D124: .4byte 0x000007FF
_0803D128: .4byte gUnk_08621DE0
_0803D12C: .4byte gUnk_08622AB4
_0803D130: .4byte 0x00000776
_0803D134:
	cmp r1, r0
	blt _0803D144
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0803D144
	mov r0, #1
	b _0803D196
_0803D144:
	ldr r0, _0803D168 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0803D16C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0803D176
	cmp r0, #0x16
	bgt _0803D170
	cmp r0, #0x15
	beq _0803D17A
	b _0803D182
	.align 2, 0
_0803D168: .4byte 0x000007FF
_0803D16C: .4byte gUnk_08621DE0
_0803D170:
	cmp r0, #0x17
	beq _0803D17E
	b _0803D182
_0803D176:
	mov r0, #7
	b _0803D196
_0803D17A:
	mov r0, #8
	b _0803D196
_0803D17E:
	mov r0, #9
	b _0803D196
_0803D182:
	ldr r0, _0803D248 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r2, _0803D24C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0803D196:
	cmp r0, #2
	beq _0803D19C
	b _0803D3C2
_0803D19C:
	ldr r0, _0803D248 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r3, _0803D250 @ =0x08622AB4
	add r0, r0, r3
	ldrh r2, [r0]
	ldr r0, _0803D254 @ =0x000007CF
	cmp r2, r0
	bls _0803D1B6
	ldr r1, _0803D258 @ =0xFFFFF830
	add r0, r2, r1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
_0803D1B6:
	mov r1, #0
	ldr r0, _0803D25C @ =0x0819A7C8
_0803D1BA:
	ldrh r3, [r0]
	cmp r2, r3
	bne _0803D264
	ldrh r1, [r0, #2]
	ldrh r4, [r0, #4]
	ldr r5, _0803D260 @ =0x0000FFFF
	mov r0, r8
	add r2, r5, #0
	add r3, r5, #0
	bl sub_0803CE90
	strh r0, [r6, #2]
	ldrh r2, [r6, #2]
	mov r0, r8
	add r1, r4, #0
	add r3, r5, #0
	bl sub_0803CE90
	strh r0, [r6]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, r5
	bne _0803D1EA
	b _0803D3C2
_0803D1EA:
	ldrh r0, [r6, #2]
	cmp r0, r5
	bne _0803D1F2
	b _0803D3C2
_0803D1F2:
	ldrh r1, [r6]
	mov r0, r8
	bl sub_0803D048
	ldr r4, _0803D248 @ =0x000007FF
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r2, _0803D250 @ =0x08622AB4
	add r1, r1, r2
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D232
	ldrh r1, [r6, #2]
	mov r0, r8
	bl sub_0803D048
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r3, _0803D250 @ =0x08622AB4
	add r1, r1, r3
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D232
	b _0803D3C2
_0803D232:
	mov r1, #0x80
	lsl r1, r1, #8
	add r0, r1, #0
	ldrh r2, [r6]
	and r0, r2
	cmp r0, #0
	bne _0803D242
	b _0803D3A4
_0803D242:
	add r0, r1, #0
	ldrh r6, [r6, #2]
	b _0803D394
_0803D248: .4byte 0x000007FF
_0803D24C: .4byte gUnk_08621DE0
_0803D250: .4byte gUnk_08622AB4
_0803D254: .4byte 0x000007CF
_0803D258: .4byte 0xFFFFF830
_0803D25C: .4byte gUnk_0819A7C8
_0803D260: .4byte 0x0000FFFF
_0803D264:
	add r0, #8
	add r1, #1
	cmp r1, #0x34
	bls _0803D1BA
	mov r1, #0
	ldr r0, _0803D3A8 @ =0x0819A970
_0803D270:
	ldrh r3, [r0]
	cmp r2, r3
	beq _0803D278
	b _0803D3B8
_0803D278:
	ldrh r1, [r0, #2]
	ldrh r4, [r0, #4]
	ldrh r5, [r0, #6]
	ldr r7, _0803D3AC @ =0x0000FFFF
	mov r0, r8
	add r2, r7, #0
	add r3, r7, #0
	bl sub_0803CE90
	strh r0, [r6, #4]
	ldrh r2, [r6, #4]
	mov r0, r8
	add r1, r4, #0
	add r3, r7, #0
	bl sub_0803CE90
	strh r0, [r6, #2]
	ldrh r2, [r6, #4]
	ldrh r3, [r6, #2]
	mov r0, r8
	add r1, r5, #0
	bl sub_0803CE90
	strh r0, [r6]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, r7
	bne _0803D2B2
	b _0803D3C2
_0803D2B2:
	ldrh r0, [r6, #2]
	cmp r0, r7
	bne _0803D2BA
	b _0803D3C2
_0803D2BA:
	ldrh r1, [r6, #4]
	cmp r1, r7
	bne _0803D2C2
	b _0803D3C2
_0803D2C2:
	ldrh r1, [r6]
	mov r0, r8
	bl sub_0803D048
	ldr r4, _0803D3B0 @ =0x000007FF
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r2, _0803D3B4 @ =0x08622AB4
	add r1, r1, r2
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D300
	ldrh r1, [r6, #2]
	mov r0, r8
	bl sub_0803D048
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r3, _0803D3B4 @ =0x08622AB4
	add r1, r1, r3
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803D3C2
_0803D300:
	ldrh r1, [r6]
	mov r0, r8
	bl sub_0803D048
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0803D3B4 @ =0x08622AB4
	add r1, r1, r0
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D33C
	ldrh r1, [r6, #4]
	mov r0, r8
	bl sub_0803D048
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r2, _0803D3B4 @ =0x08622AB4
	add r1, r1, r2
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803D3C2
_0803D33C:
	ldrh r1, [r6, #2]
	mov r0, r8
	bl sub_0803D048
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r3, _0803D3B4 @ =0x08622AB4
	add r1, r1, r3
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D378
	ldrh r1, [r6, #4]
	mov r0, r8
	bl sub_0803D048
	add r1, r4, #0
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0803D3B4 @ =0x08622AB4
	add r1, r1, r0
	ldrh r0, [r1]
	bl sub_0803CB28
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803D3C2
_0803D378:
	mov r1, #0x80
	lsl r1, r1, #8
	add r0, r1, #0
	ldrh r2, [r6]
	and r0, r2
	cmp r0, #0
	beq _0803D3A4
	add r0, r1, #0
	ldrh r3, [r6, #2]
	and r0, r3
	cmp r0, #0
	beq _0803D3A4
	add r0, r1, #0
	ldrh r6, [r6, #4]
_0803D394:
	and r0, r6
	cmp r0, #0
	beq _0803D3A4
	mov r0, r8
	bl sub_08008860
	cmp r0, #5
	beq _0803D3C2
_0803D3A4:
	mov r0, #1
	b _0803D3C4
_0803D3A8: .4byte gUnk_0819A970
_0803D3AC: .4byte 0x0000FFFF
_0803D3B0: .4byte 0x000007FF
_0803D3B4: .4byte gUnk_08622AB4
_0803D3B8:
	add r0, #8
	add r1, #1
	cmp r1, #3
	bhi _0803D3C2
	b _0803D270
_0803D3C2:
	mov r0, #0
_0803D3C4:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803D0E8
	.align 2, 0

