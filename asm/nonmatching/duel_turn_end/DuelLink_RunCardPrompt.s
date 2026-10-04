	thumb_func_start DuelLink_RunCardPrompt
DuelLink_RunCardPrompt: @ 0x08050E48
	push {lr}
	sub sp, #0x100
	ldr r1, _08050E64 @ =0x02017FB0
	ldr r2, _08050E68 @ =0x00000452
	add r0, r1, r2
	ldrh r0, [r0]
	add r2, r1, #0
	cmp r0, #0
	beq _08050E6C
	cmp r0, #1
	bne _08050E60
	b _08050F8C
_08050E60:
	mov r0, #1
	b _08050FEE
_08050E64: .4byte 0x02017FB0
_08050E68: .4byte 0x00000452
_08050E6C:
	ldr r3, _08050E94 @ =0x00000454
	add r2, r2, r3
	ldr r0, _08050E98 @ =0x000007FF
	ldrh r1, [r2]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08050E9C @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	mov r0, #0xBA
	lsl r0, r0, #1
	cmp r1, r0
	bgt _08050EA0
	sub r0, #2
	cmp r1, r0
	bge _08050EB8
	cmp r1, #0x39
	beq _08050EDC
	b _08050F66
	.align 2, 0
_08050E94: .4byte 0x00000454
_08050E98: .4byte 0x000007FF
_08050E9C: .4byte gCardIdToNumber
_08050EA0:
	ldr r0, _08050EB0 @ =0x000004DB
	cmp r1, r0
	beq _08050F08
	ldr r0, _08050EB4 @ =0x000005F2
	cmp r1, r0
	beq _08050F40
	b _08050F66
	.align 2, 0
_08050EB0: .4byte 0x000004DB
_08050EB4: .4byte 0x000005F2
_08050EB8:
	ldr r1, _08050ED0 @ =0x08085DCC
	ldrh r2, [r2]
	lsl r2, r2, #6
	ldr r0, _08050ED4 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _08050ED8 @ =0x00000A14
	b _08050F1C
_08050ED0: .4byte gStrAttackTargetZeroAtkFmt
_08050ED4: .4byte gCardNames
_08050ED8: .4byte 0x00000A14
_08050EDC:
	ldr r1, _08050EF8 @ =0x08085E60
	ldr r0, _08050EFC @ =0x08623E66
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r3, _08050F00 @ =0x0822C720
	add r2, r2, r3
	mov r0, sp
	bl FormatStr
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _08050F04 @ =0x00000915
	b _08050F1C
	.align 2, 0
_08050EF8: .4byte gStrKuribohDiscardFmt
_08050EFC: .4byte gCardNumberToId_Kuriboh
_08050F00: .4byte gCardNames
_08050F04: .4byte 0x00000915
_08050F08:
	ldr r1, _08050F30 @ =0x08085EBC
	ldrh r2, [r2]
	lsl r2, r2, #6
	ldr r0, _08050F34 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _08050F38 @ =0x00000206
	ldr r1, _08050F3C @ =0x00000713
_08050F1C:
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	b _08050F66
_08050F30: .4byte gStrAttackTargetSubstituteFmt
_08050F34: .4byte gCardNames
_08050F38: .4byte 0x00000206
_08050F3C: .4byte 0x00000713
_08050F40:
	ldr r1, _08050F74 @ =0x08085F5C
	ldrh r2, [r2]
	lsl r2, r2, #6
	ldr r3, _08050F78 @ =0x0822C720
	add r2, r2, r3
	mov r0, sp
	bl FormatStr
	ldr r0, _08050F7C @ =0x00000206
	ldr r1, _08050F80 @ =0x00000713
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
_08050F66:
	ldr r0, _08050F84 @ =0x02017FB0
	ldr r1, _08050F88 @ =0x00000452
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	b _08050FEC
_08050F74: .4byte gStrAttackTargetRedirectFmt
_08050F78: .4byte gCardNames
_08050F7C: .4byte 0x00000206
_08050F80: .4byte 0x00000713
_08050F84: .4byte 0x02017FB0
_08050F88: .4byte 0x00000452
_08050F8C:
	ldr r3, _08050FB4 @ =0x00000454
	add r0, r2, r3
	ldr r1, _08050FB8 @ =0x000007FF
	ldrh r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _08050FBC @ =0x08622AB4
	add r1, r1, r0
	ldrh r1, [r1]
	mov r0, #0xBA
	lsl r0, r0, #1
	cmp r1, r0
	bgt _08050FC0
	sub r0, #2
	cmp r1, r0
	bge _08050FCC
	cmp r1, #0x39
	beq _08050FCC
	b _08050FEC
	.align 2, 0
_08050FB4: .4byte 0x00000454
_08050FB8: .4byte 0x000007FF
_08050FBC: .4byte gCardIdToNumber
_08050FC0:
	ldr r0, _08050FDC @ =0x000004DB
	cmp r1, r0
	beq _08050FCC
	ldr r0, _08050FE0 @ =0x000005F2
	cmp r1, r0
	bne _08050FEC
_08050FCC:
	ldr r0, _08050FE4 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _08050FE8 @ =0x0000045A
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
	b _08050FEE
	.align 2, 0
_08050FDC: .4byte 0x000004DB
_08050FE0: .4byte 0x000005F2
_08050FE4: .4byte 0x0201AE60
_08050FE8: .4byte 0x0000045A
_08050FEC:
	mov r0, #0
_08050FEE:
	add sp, #0x100
	pop {r1}
	bx r1
	thumb_func_end DuelLink_RunCardPrompt

