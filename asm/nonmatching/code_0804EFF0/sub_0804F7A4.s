	thumb_func_start sub_0804F7A4
sub_0804F7A4: @ 0x0804F7A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r5, r0, #0
	mov r0, #0
	mov r8, r0
	ldr r0, _0804F890 @ =0x020192E4
	mov r7, #1
	add r1, r5, #0
	and r1, r7
	ldr r2, _0804F894 @ =0x00000D64
	add r6, r1, #0
	mul r6, r2
	mov r1, #0x28
	add r1, r1, r0
	mov sl, r1
	add r4, r6, r0
	ldr r2, _0804F898 @ =0x08624842
	mov r9, r2
_0804F7D0:
	ldrb r0, [r4, #0xB]
	lsr r1, r0, #4
	add r0, r7, #0
	ldrb r2, [r4, #0xC]
	and r0, r2
	lsl r0, r0, #4
	orr r0, r1
	mov r1, r8
	asr r0, r1
	and r0, r7
	cmp r0, #0
	beq _0804F81C
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	add r0, r0, r6
	add r0, sl
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0804F81C
	mov r0, r9
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #0xAA
	cmp r5, #0
	beq _0804F80E
	ldr r0, _0804F89C @ =0x000080AA
_0804F80E:
	mov r2, r8
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0804F81C:
	mov r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #4
	ble _0804F7D0
	mov r2, #0
	mov r8, r2
	mov sl, r0
	mov r9, r5
	mov r1, r9
	and r1, r0
	mov r9, r1
_0804F834:
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r0, _0804F894 @ =0x00000D64
	mov r2, r9
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	ldr r0, _0804F8A0 @ =0x0201930C
	add r7, r1, r0
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldrb r1, [r7, #6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r2, r1, #0x1F
	cmp r6, #0
	bne _0804F85E
	b _0804F9F6
_0804F85E:
	cmp r0, #0
	bne _0804F864
	b _0804F9F6
_0804F864:
	ldr r0, _0804F8A4 @ =0x000007FF
	add r1, r0, #0
	add r0, r6, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804F8A8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804F8AC @ =0x0000059D
	cmp r1, r0
	beq _0804F90E
	cmp r1, r0
	bgt _0804F8B4
	mov r0, #0x8A
	lsl r0, r0, #2
	cmp r1, r0
	beq _0804F8C8
	ldr r0, _0804F8B0 @ =0x00000459
	cmp r1, r0
	beq _0804F8DC
	b _0804F93E
	.align 2, 0
_0804F890: .4byte 0x020192E4
_0804F894: .4byte 0x00000D64
_0804F898: .4byte gUnk_08624842
_0804F89C: .4byte 0x000080AA
_0804F8A0: .4byte 0x0201930C
_0804F8A4: .4byte 0x000007FF
_0804F8A8: .4byte gUnk_08622AB4
_0804F8AC: .4byte 0x0000059D
_0804F8B0: .4byte 0x00000459
_0804F8B4:
	ldr r0, _0804F8C4 @ =0x0000059E
	cmp r1, r0
	beq _0804F914
	add r0, #3
	cmp r1, r0
	beq _0804F92C
	b _0804F93E
	.align 2, 0
_0804F8C4: .4byte 0x0000059E
_0804F8C8:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0x96
	lsl r1, r1, #1
	bl sub_08019860
	b _0804F93E
_0804F8DC:
	add r0, r5, #0
	bl sub_08008860
	add r4, r0, #0
	cmp r4, #1
	bne _0804F93E
	add r0, r5, #0
	add r1, r6, #0
	bl sub_080197E0
	ldrb r2, [r7, #6]
	and r4, r2
	cmp r4, #0
	bne _0804F904
	add r0, r5, #0
	mov r1, r8
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_0804F904:
	mov r0, #4
	ldrb r1, [r7, #7]
	orr r0, r1
	strb r0, [r7, #7]
	b _0804F93E
_0804F90E:
	cmp r2, #0
	bne _0804F93E
	b _0804F918
_0804F914:
	cmp r2, #0
	beq _0804F93E
_0804F918:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019980
	b _0804F93E
_0804F92C:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0xC8
	lsl r1, r1, #2
	bl sub_08019980
_0804F93E:
	ldr r4, _0804FA9C @ =0x0000058B
	add r0, r5, #0
	mov r1, r8
	add r2, r4, #0
	bl sub_0800A8CC
	cmp r0, #0
	beq _0804F976
	lsl r0, r4, #1
	ldr r2, _0804FAA0 @ =0x08623DF4
	add r0, r0, r2
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, r8
	add r2, r4, #0
	bl sub_0800A8CC
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r0, r5, #0
	bl sub_08019860
_0804F976:
	ldr r6, _0804FAA4 @ =0x000002DF
	add r0, r5, #0
	mov r1, r8
	add r2, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804F9B0
	add r0, r5, #0
	mov r1, r8
	bl sub_0800C894
	add r4, r0, #0
	lsl r0, r6, #1
	ldr r1, _0804FAA0 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, r8
	mov r2, #1
	bl sub_08018544
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08019860
_0804F9B0:
	ldr r6, _0804FAA8 @ =0x00000492
	add r0, r5, #0
	mov r1, r8
	add r2, r6, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _0804F9F6
	add r0, r5, #0
	mov r1, r8
	bl sub_0800C894
	add r4, r0, #0
	lsl r0, r6, #1
	ldr r2, _0804FAA0 @ =0x08623DF4
	add r0, r0, r2
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r4, #0
	bl sub_0807548C
	add r4, r0, #0
	add r0, r5, #0
	mov r1, r8
	add r2, r6, #0
	bl sub_0800A78C
	add r1, r4, #0
	mul r1, r0
	mov r2, sl
	sub r0, r2, r5
	bl sub_08019980
_0804F9F6:
	mov r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #4
	bgt _0804FA02
	b _0804F834
_0804FA02:
	add r0, r5, #0
	bl sub_08046E8C
	mov r2, #5
	mov r8, r2
	mov r0, #1
	and r0, r5
	str r0, [sp, #0]
_0804FA12:
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r2, _0804FAAC @ =0x00000D64
	mov sl, r2
	ldr r2, [sp, #0]
	mov r0, sl
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0804FAB0 @ =0x0201930C
	mov r9, r0
	add r1, r9
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r7, r0, #0x14
	ldrb r2, [r1, #6]
	lsl r0, r2, #0x1E
	lsr r2, r0, #0x1F
	add r1, #0x91
	ldrb r1, [r1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1F
	cmp r7, #0
	beq _0804FB20
	cmp r2, #0
	beq _0804FB20
	cmp r0, #0
	bne _0804FB20
	ldr r6, _0804FAB4 @ =0x00000589
	add r0, r5, #0
	mov r1, r8
	add r2, r6, #0
	bl sub_0800A8CC
	add r4, r0, #0
	cmp r4, #0
	ble _0804FA7A
	lsl r0, r6, #1
	ldr r1, _0804FAA0 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #2
	add r0, r5, #0
	bl sub_08019860
_0804FA7A:
	ldr r2, _0804FAB8 @ =0x000007FF
	add r1, r2, #0
	add r0, r7, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804FABC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804FAC0 @ =0x0000051F
	cmp r1, r0
	beq _0804FB04
	cmp r1, r0
	bgt _0804FAC4
	sub r0, #0xB4
	cmp r1, r0
	beq _0804FAD0
	b _0804FB20
_0804FA9C: .4byte 0x0000058B
_0804FAA0: .4byte gUnk_08623DF4
_0804FAA4: .4byte 0x000002DF
_0804FAA8: .4byte 0x00000492
_0804FAAC: .4byte 0x00000D64
_0804FAB0: .4byte 0x0201930C
_0804FAB4: .4byte 0x00000589
_0804FAB8: .4byte 0x000007FF
_0804FABC: .4byte gUnk_08622AB4
_0804FAC0: .4byte 0x0000051F
_0804FAC4:
	ldr r0, _0804FACC @ =0x00000527
	cmp r1, r0
	beq _0804FB18
	b _0804FB20
_0804FACC: .4byte 0x00000527
_0804FAD0:
	mov r1, r9
	sub r1, #0x28
	mov r0, #0
	cmp r5, #0x63
	ble _0804FADC
	mov r0, sl
_0804FADC:
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _0804FB20
	add r0, r5, #0
	add r1, r7, #0
	bl sub_080197E0
	mov r0, #0x43
	cmp r5, #0
	beq _0804FAF4
	ldr r0, _0804FB00 @ =0x00008043
_0804FAF4:
	mov r1, #0x64
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	b _0804FB20
_0804FB00: .4byte 0x00008043
_0804FB04:
	add r0, r5, #0
	add r1, r7, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0xFA
	lsl r1, r1, #1
	bl sub_08019860
	b _0804FB20
_0804FB18:
	add r0, r5, #0
	add r1, r7, #0
	bl sub_080197E0
_0804FB20:
	mov r2, #1
	add r8, r2
	mov r0, r8
	cmp r0, #0xA
	bgt _0804FB2C
	b _0804FA12
_0804FB2C:
	mov r1, #5
	mov r8, r1
	mov r0, #1
	sub r6, r0, r5
	add r7, r6, #0
	and r7, r0
_0804FB38:
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r0, _0804FB8C @ =0x00000D64
	mul r0, r7
	add r1, r1, r0
	ldr r0, _0804FB90 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldrb r2, [r1, #6]
	lsl r0, r2, #0x1E
	lsr r2, r0, #0x1F
	add r1, #0x91
	ldrb r1, [r1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1F
	cmp r4, #0
	beq _0804FBFC
	cmp r2, #0
	beq _0804FBFC
	cmp r0, #0
	bne _0804FBFC
	ldr r0, _0804FB94 @ =0x000007FF
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804FB98 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804FB9C @ =0x00000445
	cmp r1, r0
	beq _0804FBB4
	cmp r1, r0
	bgt _0804FBA0
	sub r0, #0x19
	cmp r1, r0
	beq _0804FBC2
	b _0804FBFC
	.align 2, 0
_0804FB8C: .4byte 0x00000D64
_0804FB90: .4byte 0x0201930C
_0804FB94: .4byte 0x000007FF
_0804FB98: .4byte gUnk_08622AB4
_0804FB9C: .4byte 0x00000445
_0804FBA0:
	ldr r0, _0804FBB0 @ =0x00000516
	cmp r1, r0
	beq _0804FBD6
	add r0, #9
	cmp r1, r0
	beq _0804FBEA
	b _0804FBFC
	.align 2, 0
_0804FBB0: .4byte 0x00000516
_0804FBB4:
	add r0, r5, #0
	bl sub_0800A1C4
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _0804FBFC
_0804FBC2:
	add r0, r6, #0
	add r1, r4, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019980
	b _0804FBFC
_0804FBD6:
	add r0, r6, #0
	add r1, r4, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0xFA
	lsl r1, r1, #1
	bl sub_08019860
	b _0804FBFC
_0804FBEA:
	add r0, r5, #0
	add r1, r4, #0
	bl sub_080197E0
	add r0, r5, #0
	mov r1, #0xFA
	lsl r1, r1, #1
	bl sub_08019860
_0804FBFC:
	mov r2, #1
	add r8, r2
	mov r0, r8
	cmp r0, #9
	ble _0804FB38
	ldr r4, _0804FC44 @ =0x000005A6
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08009CAC
	cmp r0, #0
	ble _0804FC34
	lsl r0, r4, #1
	ldr r1, _0804FC48 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_080197E0
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08009CAC
	mov r1, #0xC8
	mul r1, r0
	add r0, r5, #0
	bl sub_08019980
_0804FC34:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0804FC44: .4byte 0x000005A6
_0804FC48: .4byte gUnk_08623DF4
	thumb_func_end sub_0804F7A4

