	thumb_func_start CardMenu_GetMonsterCommands
CardMenu_GetMonsterCommands: @ 0x08049B74
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r1, #0
	add r7, r2, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov sl, r0
	mov r0, #1
	and r0, r5
	ldr r1, _08049BA8 @ =0x00000D64
	mul r1, r0
	ldr r2, _08049BAC @ =0x0201930C
	add r1, r1, r2
	mov r0, #0x94
	mul r0, r7
	add r1, r1, r0
	mov r8, r1
	mov r4, #0
	mov r9, r4
	cmp r5, #0
	beq _08049BB0
	mov r0, #0
	b _08049DDC
_08049BA8: .4byte 0x00000D64
_08049BAC: .4byte 0x0201930C
_08049BB0:
	add r1, r0, r2
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08049BE8
	mov r0, #0
	add r1, r7, #0
	bl GetZoneCardType
	cmp r0, #1
	bne _08049BE8
	mov r6, #0xA4
	lsl r6, r6, #1
	mov r0, #0
	add r1, r6, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08049BE4
	mov r0, #1
	add r1, r6, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08049BE8
_08049BE4:
	mov r0, #1
	mov r9, r0
_08049BE8:
	ldr r6, _08049C04 @ =0x020192E0
	ldr r1, _08049C08 @ =0x00001B12
	add r0, r6, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r2, r0, #0x1D
	cmp r2, #2
	beq _08049C88
	cmp r2, #2
	bgt _08049C0C
	cmp r2, #1
	beq _08049C18
	b _08049DDA
	.align 2, 0
_08049C04: .4byte 0x020192E0
_08049C08: .4byte 0x00001B12
_08049C0C:
	cmp r2, #3
	bne _08049C12
	b _08049DB0
_08049C12:
	cmp r2, #4
	beq _08049C88
	b _08049DDA
_08049C18:
	and r2, r5
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08049C58 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r0, r6, #0
	add r0, #0x2C
	add r1, r1, r0
	mov r0, #2
	ldrb r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	bne _08049C38
	b _08049DDA
_08049C38:
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08049C5C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08049C60 @ =0x00000243
	cmp r1, r0
	beq _08049C6C
	cmp r1, r0
	bgt _08049C64
	sub r0, #0xA3
	cmp r1, r0
	beq _08049C6C
	b _08049DDA
	.align 2, 0
_08049C58: .4byte 0x00000D64
_08049C5C: .4byte gCardIdToNumber
_08049C60: .4byte 0x00000243
_08049C64:
	ldr r0, _08049C84 @ =0x000002DB
	cmp r1, r0
	beq _08049C6C
	b _08049DDA
_08049C6C:
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #2
	bl CanActivateEffectInZone
	cmp r0, #0
	bne _08049C7C
	b _08049DDA
_08049C7C:
	mov r0, #0x40
	orr r4, r0
	b _08049DDA
	.align 2, 0
_08049C84: .4byte 0x000002DB
_08049C88:
	mov r0, #4
	mov r2, r8
	ldrb r2, [r2, #7]
	and r0, r2
	cmp r0, #0
	bne _08049D66
	ldr r2, _08049CE8 @ =0x020192E4
	mov r6, #1
	add r0, r5, #0
	and r0, r6
	ldr r1, _08049CEC @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08049D66
	mov r2, #0xAE
	lsl r2, r2, #1
	add r0, r5, #0
	add r1, r7, #0
	bl CountZoneLinksFromCard
	cmp r0, #0
	bne _08049D66
	ldr r2, _08049CF0 @ =0x000004DC
	add r0, r5, #0
	add r1, r7, #0
	bl CountZoneLinksFromCard
	cmp r0, #0
	bne _08049D66
	mov r0, r9
	cmp r0, #0
	bne _08049D66
	mov r2, r8
	ldrb r1, [r2, #6]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _08049CFE
	add r0, r6, #0
	and r0, r1
	cmp r0, #0
	beq _08049CF4
	mov r0, #4
	b _08049CF6
	.align 2, 0
_08049CE8: .4byte 0x020192E4
_08049CEC: .4byte 0x00000D64
_08049CF0: .4byte 0x000004DC
_08049CF4:
	mov r0, #2
_08049CF6:
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
	b _08049D26
_08049CFE:
	mov r0, #8
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
	add r0, r6, #0
	and r0, r1
	cmp r0, #0
	bne _08049D16
	mov r0, #2
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
_08049D16:
	mov r0, #0
	bl CanNormalSummon
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08049D26
	ldr r0, _08049D98 @ =0x0000FFF7
	and r4, r0
_08049D26:
	ldr r6, _08049D9C @ =0x00000536
	mov r0, #0
	add r1, r6, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _08049D40
	mov r0, #1
	add r1, r6, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _08049D66
_08049D40:
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r7
	ldr r1, _08049DA0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08049DA4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08049DA8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	beq _08049D66
	ldr r0, _08049DAC @ =0x0000FFF1
	and r4, r0
_08049D66:
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08049DA0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08049DA4 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08049DDA
	mov r0, sl
	add r1, r5, #0
	add r2, r7, #0
	bl CanActivateMonsterEffect
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08049DDA
	mov r0, #0x40
	b _08049DD4
_08049D98: .4byte 0x0000FFF7
_08049D9C: .4byte 0x00000536
_08049DA0: .4byte 0x00000D64
_08049DA4: .4byte 0x0201930C
_08049DA8: .4byte gCardIdToNumber
_08049DAC: .4byte 0x0000FFF1
_08049DB0:
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #1
	bl CanMonsterAttack
	cmp r0, #0
	beq _08049DDA
	mov r1, #1
	and r5, r1
	ldr r0, _08049DEC @ =0x00000D64
	mul r0, r5
	add r0, r6, r0
	ldrh r0, [r0, #0x2A]
	asr r0, r7
	and r0, r1
	cmp r0, #0
	bne _08049DDA
	mov r0, #0x80
_08049DD4:
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
_08049DDA:
	add r0, r4, #0
_08049DDC:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08049DEC: .4byte 0x00000D64
	thumb_func_end CardMenu_GetMonsterCommands

