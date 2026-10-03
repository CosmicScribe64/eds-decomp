	thumb_func_start DuelPhase_TurnEnd
DuelPhase_TurnEnd: @ 0x08050A70
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x80
	ldr r1, _08050AA0 @ =0x020192E0
	ldr r2, _08050AA4 @ =0x00001B12
	add r0, r1, r2
	ldrb r2, [r0]
	lsl r0, r2, #0x1E
	lsr r4, r0, #0x1F
	mov r3, #0xD9
	lsl r3, r3, #5
	add r0, r1, r3
	ldrb r0, [r0]
	mov r8, r1
	cmp r0, #4
	bls _08050A96
	b _08050DE0
_08050A96:
	lsl r0, r0, #2
	ldr r1, _08050AA8 @ =0x08050AAC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08050AA0: .4byte 0x020192E0
_08050AA4: .4byte 0x00001B12
_08050AA8: .4byte 0x08050AAC
_08050AAC:
	.4byte _08050AC0
	.4byte _08050B7A
	.4byte _08050C6C
	.4byte _08050CB8
	.4byte _08050CD4
_08050AC0:
	mov r5, #0
	ldr r3, _08050B48 @ =0x020192E4
	ldr r0, _08050B4C @ =0x00000D64
	add r2, r4, #0
	mul r2, r0
	add r0, r2, r3
	ldrb r1, [r0, #2]
	cmp r5, r1
	bge _08050AEC
	ldr r6, _08050B50 @ =0x00000684
	add r0, r3, r6
	add r3, r1, #0
	add r1, r2, r0
_08050ADA:
	ldr r0, [r1]
	lsl r0, r0, #0xD
	cmp r0, #0
	bge _08050AE4
	b _08050D0C
_08050AE4:
	add r1, #4
	add r5, #1
	cmp r5, r3
	blt _08050ADA
_08050AEC:
	mov r5, #5
	ldr r7, _08050B54 @ =0x0201930C
	mov ip, r7
	ldr r0, _08050B4C @ =0x00000D64
	add r7, r4, #0
	mul r7, r0
	ldr r0, _08050B58 @ =0x000007FF
	mov r9, r0
_08050AFC:
	mov r0, #0x94
	mul r0, r5
	add r0, r0, r7
	mov r1, ip
	add r3, r0, r1
	ldr r1, [r3]
	lsl r0, r1, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08050B72
	lsl r0, r1, #0xD
	cmp r0, #0
	bge _08050B72
	mov r6, #1
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _08050B6C
	mov r3, r9
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _08050B5C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08050B60
	cmp r0, #0x15
	blt _08050B60
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08050B62
_08050B48: .4byte 0x020192E4
_08050B4C: .4byte 0x00000D64
_08050B50: .4byte 0x00000684
_08050B54: .4byte 0x0201930C
_08050B58: .4byte 0x000007FF
_08050B5C: .4byte gCardStats
_08050B60:
	mov r0, #0
_08050B62:
	cmp r0, #4
	bgt _08050B6C
	cmp r0, #2
	blt _08050B6C
	mov r6, #0
_08050B6C:
	cmp r6, #0
	beq _08050B72
	b _08050D20
_08050B72:
	add r5, #1
	cmp r5, #0xA
	ble _08050AFC
	b _08050CF2
_08050B7A:
	mov r5, #0
	ldr r6, _08050C10 @ =0x020192E4
	mov r1, #1
	sub r2, r1, r4
	add r0, r2, #0
	and r0, r1
	ldr r1, _08050C14 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	add r0, r3, r6
	ldrb r1, [r0, #2]
	cmp r5, r1
	bge _08050BB0
	add r7, r2, #0
	ldr r2, _08050C18 @ =0x00000684
	add r0, r6, r2
	add r2, r1, #0
	add r1, r3, r0
_08050B9E:
	ldr r0, [r1]
	lsl r0, r0, #0xD
	cmp r0, #0
	bge _08050BA8
	b _08050D3C
_08050BA8:
	add r1, #4
	add r5, #1
	cmp r5, r2
	blt _08050B9E
_08050BB0:
	mov r5, #5
	ldr r3, _08050C1C @ =0x0201930C
	mov ip, r3
	mov r0, #1
	eor r0, r4
	ldr r1, _08050C14 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
	ldr r6, _08050C20 @ =0x000007FF
	mov r9, r6
_08050BC4:
	mov r0, #0x94
	mul r0, r5
	add r0, r0, r7
	mov r1, ip
	add r3, r0, r1
	ldr r1, [r3]
	lsl r0, r1, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08050C3A
	lsl r0, r1, #0xD
	cmp r0, #0
	bge _08050C3A
	mov r6, #1
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _08050C34
	mov r3, r9
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _08050C24 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08050C28
	cmp r0, #0x15
	blt _08050C28
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _08050C2A
_08050C10: .4byte 0x020192E4
_08050C14: .4byte 0x00000D64
_08050C18: .4byte 0x00000684
_08050C1C: .4byte 0x0201930C
_08050C20: .4byte 0x000007FF
_08050C24: .4byte gCardStats
_08050C28:
	mov r0, #0
_08050C2A:
	cmp r0, #4
	bgt _08050C34
	cmp r0, #2
	blt _08050C34
	mov r6, #0
_08050C34:
	cmp r6, #0
	beq _08050C3A
	b _08050D5C
_08050C3A:
	add r5, #1
	cmp r5, #0xA
	ble _08050BC4
	cmp r4, #0
	beq _08050C50
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_08050C50:
	mov r0, #0xD9
	lsl r0, r0, #5
	add r0, r8
	ldrb r1, [r0]
	add r1, #1
	mov r2, #0
	strb r1, [r0]
	ldr r0, _08050C68 @ =0x00001B21
	add r0, r8
	strb r2, [r0]
	mov r0, #0
	b _08050E24
_08050C68: .4byte 0x00001B21
_08050C6C:
	ldr r0, _08050CB0 @ =0x00001B21
	add r0, r8
	ldrb r2, [r0]
	cmp r2, #4
	bhi _08050CF2
	add r5, r0, #0
	ldr r0, _08050CB4 @ =0x00000D64
	add r3, r4, #0
	mul r3, r0
	mov r6, r8
	add r6, #0x2C
_08050C82:
	ldrb r1, [r5]
	mov r0, #0x94
	mul r0, r1
	add r0, r0, r3
	add r4, r0, r6
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08050CA2
	ldrh r7, [r4, #6]
	lsl r0, r7, #0x16
	lsr r0, r0, #0x1C
	cmp r0, #1
	bls _08050CA2
	b _08050D9C
_08050CA2:
	add r0, r1, #1
	strb r0, [r5]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #4
	bls _08050C82
	b _08050CF2
_08050CB0: .4byte 0x00001B21
_08050CB4: .4byte 0x00000D64
_08050CB8:
	ldr r1, _08050CCC @ =0x00001B12
	add r1, r8
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	mov r1, #2
	cmp r0, #0
	beq _08050CE6
	ldr r1, _08050CD0 @ =0x00008002
	b _08050CE6
_08050CCC: .4byte 0x00001B12
_08050CD0: .4byte 0x00008002
_08050CD4:
	ldr r1, _08050D04 @ =0x00001B12
	add r1, r8
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	mov r1, #3
	cmp r0, #0
	beq _08050CE6
	ldr r1, _08050D08 @ =0x00008003
_08050CE6:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08050CF2:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	mov r0, #0
	b _08050E24
	.align 2, 0
_08050D04: .4byte 0x00001B12
_08050D08: .4byte 0x00008003
_08050D0C:
	ldr r0, _08050D1C @ =0x0862467A
	ldrh r1, [r0]
	add r0, r4, #0
	bl ShowCardEffect
	add r0, r4, #0
	b _08050D4A
	.align 2, 0
_08050D1C: .4byte gUnk_0862467A
_08050D20:
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08050D38 @ =0x00000D64
	mul r0, r4
	add r1, r1, r0
	add r1, ip
	mov r0, #5
	neg r0, r0
	ldrb r2, [r1, #2]
	and r0, r2
	b _08050D7A
_08050D38: .4byte 0x00000D64
_08050D3C:
	ldr r0, _08050D58 @ =0x0862467A
	ldrh r1, [r0]
	add r0, r7, #0
	bl ShowCardEffect
	mov r0, #1
	sub r0, r0, r4
_08050D4A:
	add r1, r5, #0
	mov r2, #0
	mov r3, #1
	bl DiscardHandCard
	mov r0, #0
	b _08050E24
_08050D58: .4byte gUnk_0862467A
_08050D5C:
	mov r0, #1
	sub r4, r0, r4
	add r2, r4, #0
	and r2, r0
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08050D94 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, ip
	mov r0, #5
	neg r0, r0
	ldrb r3, [r1, #2]
	and r0, r3
_08050D7A:
	strb r0, [r1, #2]
	ldr r0, _08050D98 @ =0x0862467A
	ldrh r1, [r0]
	add r0, r4, #0
	bl ShowCardEffect
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl DestroyFieldCard
	mov r0, #0
	b _08050E24
_08050D94: .4byte 0x00000D64
_08050D98: .4byte gUnk_0862467A
_08050D9C:
	ldr r1, _08050DD0 @ =0x08085D94
	lsl r2, r2, #6
	ldr r6, _08050DD4 @ =0x0822C720
	add r2, r2, r6
	mov r0, sp
	bl FormatStr
	ldrh r4, [r4, #6]
	lsl r2, r4, #0x16
	lsr r2, r2, #0x1C
	sub r2, #1
	mov r0, sp
	mov r1, sp
	bl FormatInt
	ldr r0, _08050DD8 @ =0x00000206
	ldr r1, _08050DDC @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _08050E24
_08050DD0: .4byte gStrTurnsUntilDestroyedFmt
_08050DD4: .4byte gCardNames
_08050DD8: .4byte 0x00000206
_08050DDC: .4byte 0x00000712
_08050DE0:
	ldr r3, _08050E34 @ =0x02015EE8
	mov r4, #1
	add r0, r4, #0
	ldrb r7, [r3, #1]
	and r0, r7
	cmp r0, #0
	bne _08050E0A
	mov r0, #2
	and r0, r2
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #0
	bne _08050E00
	ldr r0, _08050E38 @ =0x02015EF0
	strb r1, [r0]
	strb r1, [r0, #1]
_08050E00:
	add r0, r4, #0
	ldrb r3, [r3, #1]
	and r0, r3
	cmp r0, #0
	beq _08050E16
_08050E0A:
	ldr r0, _08050E3C @ =0x0000F002
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
_08050E16:
	ldr r0, _08050E40 @ =0x020192E0
	ldr r1, _08050E44 @ =0x00001B10
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	mov r0, #1
_08050E24:
	add sp, #0x80
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08050E34: .4byte 0x02015EE8
_08050E38: .4byte 0x02015EF0
_08050E3C: .4byte 0x0000F002
_08050E40: .4byte 0x020192E0
_08050E44: .4byte 0x00001B10
	thumb_func_end DuelPhase_TurnEnd

