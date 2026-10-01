	thumb_func_start sub_0805A8E8
sub_0805A8E8: @ 0x0805A8E8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0805A904 @ =0x02015EF0
	ldrb r1, [r0, #2]
	mov r9, r0
	cmp r1, #0
	beq _0805A908
	cmp r1, #1
	beq _0805A914
_0805A900:
	mov r0, #1
	b _0805AB78
_0805A904: .4byte 0x02015EF0
_0805A908:
	mov r0, r9
	strb r1, [r0, #3]
	ldrb r0, [r0, #2]
	add r0, #1
	mov r1, r9
	strb r0, [r1, #2]
_0805A914:
	ldr r0, _0805A97C @ =0x020192E4
	ldr r2, _0805A980 @ =0x00000D66
	add r0, r0, r2
	ldrb r1, [r0]
	cmp r1, #0
	bne _0805A922
	b _0805AB6E
_0805A922:
	mov r2, r9
	ldrb r3, [r2, #3]
	cmp r3, r1
	bcc _0805A92C
	b _0805AB6E
_0805A92C:
	ldr r4, _0805A984 @ =0x000007FF
	mov sl, r4
_0805A930:
	ldrb r2, [r2, #3]
	lsl r0, r2, #2
	ldr r1, _0805A988 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r8, r0
	mov r0, #1
	bl sub_08008C6C
	add r7, r0, #0
	mov r4, #0
	cmp r7, #0
	blt _0805A900
	mov r0, r8
	mov r6, sl
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _0805A98C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805A990 @ =0x00000406
	cmp r1, r0
	bne _0805A964
	b _0805AA90
_0805A964:
	cmp r1, r0
	bgt _0805A9A8
	sub r0, #8
	cmp r1, r0
	beq _0805A9E4
	cmp r1, r0
	bgt _0805A994
	sub r0, #3
	cmp r1, r0
	beq _0805A9D8
	b _0805AAE0
	.align 2, 0
_0805A97C: .4byte 0x020192E4
_0805A980: .4byte 0x00000D66
_0805A984: .4byte 0x000007FF
_0805A988: .4byte 0x0201A6CC
_0805A98C: .4byte gUnk_08622AB4
_0805A990: .4byte 0x00000406
_0805A994:
	ldr r0, _0805A9A4 @ =0x00000402
	cmp r1, r0
	beq _0805A9EC
	add r0, #3
	cmp r1, r0
	beq _0805AA02
	b _0805AAE0
	.align 2, 0
_0805A9A4: .4byte 0x00000402
_0805A9A8:
	ldr r0, _0805A9BC @ =0x00000482
	cmp r1, r0
	beq _0805AA02
	cmp r1, r0
	bgt _0805A9C0
	sub r0, #0x79
	cmp r1, r0
	beq _0805AA90
	b _0805AAE0
	.align 2, 0
_0805A9BC: .4byte 0x00000482
_0805A9C0:
	ldr r0, _0805A9D4 @ =0x000004DD
	cmp r1, r0
	bne _0805A9C8
	b _0805AB54
_0805A9C8:
	add r0, #0x45
	cmp r1, r0
	bne _0805A9D0
	b _0805AB54
_0805A9D0:
	b _0805AAE0
	.align 2, 0
_0805A9D4: .4byte 0x000004DD
_0805A9D8:
	mov r0, #0
	ldr r1, _0805A9E0 @ =0x0000014F
	b _0805A9F2
	.align 2, 0
_0805A9E0: .4byte 0x0000014F
_0805A9E4:
	mov r0, #0
	mov r1, #0xA8
	lsl r1, r1, #1
	b _0805A9F2
_0805A9EC:
	mov r0, #0
	mov r1, #0xFC
	lsl r1, r1, #2
_0805A9F2:
	bl sub_0800A2A8
	mov r1, #0
	cmp r0, #0
	ble _0805A9FE
	mov r1, #1
_0805A9FE:
	add r4, r1, #0
	b _0805AB1E
_0805AA02:
	mov r3, #5
	ldr r5, _0805AA80 @ =0x020192E4
	mov r2, #0x28
	add r2, r2, r5
	mov ip, r2
	ldr r6, _0805AA84 @ =0x000007FF
_0805AA0E:
	mov r0, #0x94
	mul r0, r3
	mov r1, ip
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0805AA42
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0805AA88 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0805AA42
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0805AA42
	mov r4, #1
_0805AA42:
	add r3, #1
	cmp r3, #9
	ble _0805AA0E
	ldrb r0, [r5, #2]
	cmp r0, #0
	beq _0805AB1E
	ldr r5, _0805AA84 @ =0x000007FF
	ldr r0, _0805AA80 @ =0x020192E4
	ldr r2, _0805AA8C @ =0x00000684
	add r1, r0, r2
	mov r2, #0xF8
	lsl r2, r2, #0x11
	ldrb r3, [r0, #2]
_0805AA5C:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _0805AA88 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0805AA76
	mov r4, #1
_0805AA76:
	add r1, #4
	sub r3, #1
	cmp r3, #0
	bne _0805AA5C
	b _0805AB1E
_0805AA80: .4byte 0x020192E4
_0805AA84: .4byte 0x000007FF
_0805AA88: .4byte gUnk_08621DE0
_0805AA8C: .4byte 0x00000684
_0805AA90:
	mov r3, #5
	ldr r0, _0805AAD4 @ =0x0201930C
	mov ip, r0
	ldr r5, _0805AAD8 @ =0x000007FF
_0805AA98:
	mov r0, #0x94
	mul r0, r3
	mov r1, ip
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0805AACC
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _0805AADC @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0805AACC
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0805AACC
	mov r4, #1
_0805AACC:
	add r3, #1
	cmp r3, #9
	ble _0805AA98
	b _0805AB1E
_0805AAD4: .4byte 0x0201930C
_0805AAD8: .4byte 0x000007FF
_0805AADC: .4byte gUnk_08621DE0
_0805AAE0:
	mov r2, r8
	mov r0, sl
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0805AB44 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _0805AB22
	cmp r0, #0x16
	bne _0805AB1E
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #5
	bne _0805AB0C
	mov r4, #1
_0805AB0C:
	lsl r0, r2, #1
	ldr r2, _0805AB48 @ =0x08622AB4
	add r0, r0, r2
	mov r1, #0x9B
	lsl r1, r1, #1
	ldrh r0, [r0]
	cmp r0, r1
	bne _0805AB1E
	mov r4, #1
_0805AB1E:
	cmp r4, #0
	beq _0805AB54
_0805AB22:
	mov r0, #1
	bl sub_08046A74
	ldr r0, _0805AB4C @ =0x02015EF0
	mov r1, #0xF
	ldrb r2, [r0, #3]
	and r2, r1
	lsl r2, r2, #4
	and r7, r1
	orr r2, r7
	ldr r0, _0805AB50 @ =0x000080C5
	mov r1, r8
	mov r3, #0
	bl sub_0801EC58
	b _0805AB76
	.align 2, 0
_0805AB44: .4byte gUnk_08621DE0
_0805AB48: .4byte gUnk_08622AB4
_0805AB4C: .4byte 0x02015EF0
_0805AB50: .4byte 0x000080C5
_0805AB54:
	ldr r2, _0805AB88 @ =0x02015EF0
	ldrb r0, [r2, #3]
	add r0, #1
	strb r0, [r2, #3]
	ldr r3, _0805AB8C @ =0x0201A04A
	ldrb r1, [r3]
	cmp r1, #0
	beq _0805AB6E
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, r1
	bcs _0805AB6E
	b _0805A930
_0805AB6E:
	mov r4, r9
	ldrb r0, [r4, #2]
	add r0, #1
	strb r0, [r4, #2]
_0805AB76:
	mov r0, #0
_0805AB78:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0805AB88: .4byte 0x02015EF0
_0805AB8C: .4byte 0x0201A04A
	thumb_func_end sub_0805A8E8

