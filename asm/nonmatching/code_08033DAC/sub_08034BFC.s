	thumb_func_start sub_08034BFC
sub_08034BFC: @ 0x08034BFC
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r4, r1, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _08034C0E
	b _08035182
_08034C0E:
	cmp r4, #0
	bne _08034C14
	b _08035182
_08034C14:
	ldr r5, _08034C44 @ =0x000007FF
	add r0, r5, #0
	ldrh r2, [r6]
	and r0, r2
	lsl r0, r0, #1
	ldr r7, _08034C48 @ =0x08622AB4
	add r0, r0, r7
	ldrh r1, [r0]
	ldr r0, _08034C4C @ =0x00000405
	cmp r1, r0
	bne _08034C2C
	b _08034E5C
_08034C2C:
	cmp r1, r0
	bgt _08034C60
	sub r0, #8
	cmp r1, r0
	bne _08034C38
	b _08034D48
_08034C38:
	cmp r1, r0
	bgt _08034C50
	sub r0, #2
	cmp r1, r0
	beq _08034C94
	b _08035182
_08034C44: .4byte 0x000007FF
_08034C48: .4byte gUnk_08622AB4
_08034C4C: .4byte 0x00000405
_08034C50:
	ldr r0, _08034C5C @ =0x000003FE
	cmp r1, r0
	bne _08034C58
	b _08034DA8
_08034C58:
	b _08035182
	.align 2, 0
_08034C5C: .4byte 0x000003FE
_08034C60:
	ldr r0, _08034C78 @ =0x00000426
	cmp r1, r0
	bne _08034C68
	b _08034EC8
_08034C68:
	cmp r1, r0
	bgt _08034C7C
	sub r0, #0x20
	cmp r1, r0
	bne _08034C74
	b _08034E80
_08034C74:
	b _08035182
	.align 2, 0
_08034C78: .4byte 0x00000426
_08034C7C:
	ldr r0, _08034C90 @ =0x000005FA
	cmp r1, r0
	bne _08034C84
	b _08034F90
_08034C84:
	add r0, #1
	cmp r1, r0
	bne _08034C8C
	b _08034FF8
_08034C8C:
	b _08035182
	.align 2, 0
_08034C90: .4byte 0x000005FA
_08034C94:
	add r0, r5, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	ldr r1, _08034D34 @ =0x0000014F
	ldrh r0, [r0]
	cmp r0, r1
	beq _08034CA8
	b _08035182
_08034CA8:
	mov r5, #1
	add r0, r5, #0
	ldrb r2, [r4, #2]
	and r0, r2
	add r1, r5, #0
	ldrb r2, [r6, #2]
	and r1, r2
	cmp r0, r1
	bne _08034CBC
	b _08035182
_08034CBC:
	mov r0, #0xB0
	cmp r1, #0
	beq _08034CC4
	ldr r0, _08034D38 @ =0x000080B0
_08034CC4:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xB1
	cmp r0, #0
	beq _08034CDC
	ldr r2, _08034D3C @ =0x000080B1
_08034CDC:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r4, #0
	mov r5, #1
_08034CF0:
	ldrb r2, [r6, #2]
	lsl r3, r2, #0x1F
	lsr r1, r3, #0x1F
	sub r1, r5, r1
	and r1, r5
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _08034D40 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08034D44 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08034D2C
	lsr r0, r3, #0x1F
	sub r0, r5, r0
	add r1, r4, #0
	bl sub_08030028
	ldrb r0, [r6, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r5, r1
	add r2, r4, #0
	bl sub_08046CB0
_08034D2C:
	add r4, #1
	cmp r4, #4
	ble _08034CF0
	b _08035182
_08034D34: .4byte 0x0000014F
_08034D38: .4byte 0x000080B0
_08034D3C: .4byte 0x000080B1
_08034D40: .4byte 0x00000D64
_08034D44: .4byte 0x0201930C
_08034D48:
	add r0, r5, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	mov r1, #0xFC
	lsl r1, r1, #2
	ldrh r0, [r0]
	cmp r0, r1
	beq _08034D5E
	b _08035182
_08034D5E:
	mov r5, #1
	add r0, r5, #0
	ldrb r2, [r4, #2]
	and r0, r2
	add r1, r5, #0
	ldrb r6, [r6, #2]
	and r1, r6
	cmp r0, r1
	bne _08034D72
	b _08035182
_08034D72:
	mov r0, #0xB0
	cmp r1, #0
	beq _08034D7A
	ldr r0, _08034DA0 @ =0x000080B0
_08034D7A:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xB1
	cmp r0, #0
	beq _08034D92
	ldr r2, _08034DA4 @ =0x000080B1
_08034D92:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #1
	b _08034FE8
	.align 2, 0
_08034DA0: .4byte 0x000080B0
_08034DA4: .4byte 0x000080B1
_08034DA8:
	add r0, r5, #0
	ldrh r2, [r4]
	and r0, r2
	lsl r0, r0, #1
	add r0, r0, r7
	mov r1, #0xA8
	lsl r1, r1, #1
	ldrh r0, [r0]
	cmp r0, r1
	beq _08034DBE
	b _08035182
_08034DBE:
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	add r1, r5, #0
	ldrb r2, [r6, #2]
	and r1, r2
	cmp r0, r1
	bne _08034DD2
	b _08035182
_08034DD2:
	mov r0, #0xB0
	cmp r1, #0
	beq _08034DDA
	ldr r0, _08034E4C @ =0x000080B0
_08034DDA:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xB1
	cmp r0, #0
	beq _08034DF2
	ldr r2, _08034E50 @ =0x000080B1
_08034DF2:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r4, #0
	mov r5, #1
_08034E06:
	ldrb r2, [r6, #2]
	lsl r3, r2, #0x1F
	lsr r1, r3, #0x1F
	sub r1, r5, r1
	and r1, r5
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _08034E54 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08034E58 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08034E42
	lsr r0, r3, #0x1F
	sub r0, r5, r0
	add r1, r4, #0
	bl sub_08030028
	ldrb r0, [r6, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r5, r1
	add r2, r4, #0
	bl sub_08046CB0
_08034E42:
	add r4, #1
	cmp r4, #4
	ble _08034E06
	b _08035182
	.align 2, 0
_08034E4C: .4byte 0x000080B0
_08034E50: .4byte 0x000080B1
_08034E54: .4byte 0x00000D64
_08034E58: .4byte 0x0201930C
_08034E5C:
	add r0, r5, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08034E7C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08034E78
	b _08035182
_08034E78:
	b _08035148
	.align 2, 0
_08034E7C: .4byte gUnk_08621DE0
_08034E80:
	add r0, r5, #0
	ldrh r2, [r4]
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08034EC0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08034E9C
	b _08035182
_08034E9C:
	mov r5, #1
	add r0, r5, #0
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0xB0
	cmp r0, #0
	beq _08034EAC
	ldr r1, _08034EC4 @ =0x000080B0
_08034EAC:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r2, [r4, #2]
	and r0, r2
	b _0803516A
_08034EC0: .4byte gUnk_08621DE0
_08034EC4: .4byte 0x000080B0
_08034EC8:
	add r0, r5, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	ldr r1, _08034F78 @ =0x0000029F
	ldrh r0, [r0]
	cmp r0, r1
	beq _08034EDC
	b _08035182
_08034EDC:
	mov r5, #1
	add r0, r5, #0
	ldrb r2, [r4, #2]
	and r0, r2
	add r1, r5, #0
	ldrb r2, [r6, #2]
	and r1, r2
	cmp r0, r1
	bne _08034EF0
	b _08035182
_08034EF0:
	mov r0, #0xB0
	cmp r1, #0
	beq _08034EF8
	ldr r0, _08034F7C @ =0x000080B0
_08034EF8:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xB1
	cmp r0, #0
	beq _08034F10
	ldr r2, _08034F80 @ =0x000080B1
_08034F10:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r4, #5
	mov r5, #1
_08034F24:
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	and r0, r5
	mov r1, #0x94
	add r2, r4, #0
	mul r2, r1
	ldr r1, _08034F84 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08034F88 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08034F70
	add r0, r5, #0
	and r0, r3
	mov r2, #0x8B
	cmp r0, #0
	bne _08034F52
	ldr r2, _08034F8C @ =0x0000808B
_08034F52:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	add r1, r4, #0
	mov r2, #1
	bl sub_08018544
_08034F70:
	add r4, #1
	cmp r4, #0xA
	ble _08034F24
	b _08035182
_08034F78: .4byte 0x0000029F
_08034F7C: .4byte 0x000080B0
_08034F80: .4byte 0x000080B1
_08034F84: .4byte 0x00000D64
_08034F88: .4byte 0x0201930C
_08034F8C: .4byte 0x0000808B
_08034F90:
	add r2, r5, #0
	ldrh r0, [r4]
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _08034FF0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08034FAC
	b _08035182
_08034FAC:
	lsl r0, r2, #1
	add r0, r0, r7
	ldrh r0, [r0]
	mov r1, #1
	bl sub_08007590
	cmp r0, #0
	bne _08034FD4
	add r0, r5, #0
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r0, [r0]
	mov r1, #0
	bl sub_08007590
	cmp r0, #0
	bne _08034FD4
	b _08035182
_08034FD4:
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0xB0
	cmp r0, #0
	beq _08034FE2
	ldr r1, _08034FF4 @ =0x000080B0
_08034FE2:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
_08034FE8:
	mov r3, #0
	bl sub_0801EC58
	b _08035182
_08034FF0: .4byte gUnk_08621DE0
_08034FF4: .4byte 0x000080B0
_08034FF8:
	add r0, r5, #0
	ldrh r2, [r4]
	and r0, r2
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r1, [r0]
	ldr r0, _08035038 @ =0x00000424
	cmp r1, r0
	bne _0803500C
	b _08035148
_0803500C:
	cmp r1, r0
	bgt _080350A4
	sub r0, #0x34
	cmp r1, r0
	bne _08035018
	b _08035148
_08035018:
	cmp r1, r0
	bgt _08035068
	ldr r0, _0803503C @ =0x00000147
	cmp r1, r0
	bgt _08035040
	sub r0, #9
	cmp r1, r0
	blt _0803502A
	b _08035148
_0803502A:
	sub r0, #2
	cmp r1, r0
	ble _08035032
	b _08035182
_08035032:
	sub r0, #0x10
	b _080350D4
	.align 2, 0
_08035038: .4byte 0x00000424
_0803503C: .4byte 0x00000147
_08035040:
	ldr r0, _08035050 @ =0x0000028D
	cmp r1, r0
	bne _08035048
	b _08035148
_08035048:
	cmp r1, r0
	bgt _08035054
	sub r0, #2
	b _08035130
_08035050: .4byte 0x0000028D
_08035054:
	mov r0, #0xA4
	lsl r0, r0, #2
	cmp r1, r0
	bne _0803505E
	b _08035148
_0803505E:
	ldr r0, _08035064 @ =0x000003C2
	b _08035130
	.align 2, 0
_08035064: .4byte 0x000003C2
_08035068:
	ldr r0, _08035084 @ =0x00000413
	cmp r1, r0
	bgt _08035090
	sub r0, #1
	cmp r1, r0
	bge _08035148
	sub r0, #0x13
	cmp r1, r0
	beq _08035148
	cmp r1, r0
	bgt _08035088
	sub r0, #0xA
	b _080350CE
	.align 2, 0
_08035084: .4byte 0x00000413
_08035088:
	ldr r0, _0803508C @ =0x00000403
	b _08035130
_0803508C: .4byte 0x00000403
_08035090:
	ldr r0, _080350A0 @ =0x00000416
	cmp r1, r0
	blt _08035182
	add r0, #1
	cmp r1, r0
	ble _08035148
	add r0, #0xB
	b _08035130
_080350A0: .4byte 0x00000416
_080350A4:
	ldr r0, _080350C8 @ =0x00000521
	cmp r1, r0
	beq _08035148
	cmp r1, r0
	bgt _08035104
	sub r0, #0x9C
	cmp r1, r0
	beq _08035148
	cmp r1, r0
	bgt _080350E0
	sub r0, #0x55
	cmp r1, r0
	beq _08035148
	cmp r1, r0
	bgt _080350CC
	sub r0, #4
	b _08035130
	.align 2, 0
_080350C8: .4byte 0x00000521
_080350CC:
	ldr r0, _080350DC @ =0x00000434
_080350CE:
	cmp r1, r0
	bgt _08035182
	sub r0, #1
_080350D4:
	cmp r1, r0
	blt _08035182
	b _08035148
	.align 2, 0
_080350DC: .4byte 0x00000434
_080350E0:
	ldr r0, _080350F0 @ =0x0000049E
	cmp r1, r0
	beq _08035148
	cmp r1, r0
	bgt _080350F4
	sub r0, #0x16
	b _08035130
	.align 2, 0
_080350F0: .4byte 0x0000049E
_080350F4:
	ldr r0, _08035100 @ =0x000004BB
	cmp r1, r0
	beq _08035148
	add r0, #9
	b _08035130
	.align 2, 0
_08035100: .4byte 0x000004BB
_08035104:
	ldr r0, _08035120 @ =0x000005AB
	cmp r1, r0
	bgt _08035124
	sub r0, #3
	cmp r1, r0
	bge _08035148
	sub r0, #0x1D
	cmp r1, r0
	blt _08035182
	add r0, #1
	cmp r1, r0
	ble _08035148
	add r0, #2
	b _08035130
_08035120: .4byte 0x000005AB
_08035124:
	ldr r0, _08035138 @ =0x0000060A
	cmp r1, r0
	beq _08035148
	cmp r1, r0
	bgt _0803513C
	sub r0, #6
_08035130:
	cmp r1, r0
	beq _08035148
	b _08035182
	.align 2, 0
_08035138: .4byte 0x0000060A
_0803513C:
	ldr r0, _0803518C @ =0x0000060C
	cmp r1, r0
	beq _08035148
	add r0, #2
	cmp r1, r0
	bne _08035182
_08035148:
	mov r5, #1
	add r0, r5, #0
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0xB0
	cmp r0, #0
	beq _08035158
	ldr r1, _08035190 @ =0x000080B0
_08035158:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
_0803516A:
	mov r2, #0xB1
	cmp r0, #0
	beq _08035172
	ldr r2, _08035194 @ =0x000080B1
_08035172:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_08035182:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803518C: .4byte 0x0000060C
_08035190: .4byte 0x000080B0
_08035194: .4byte 0x000080B1
	thumb_func_end sub_08034BFC

