	thumb_func_start EffectAxeOfDespairResolve
EffectAxeOfDespairResolve: @ 0x08030928
	push {r4, lr}
	sub sp, #0x100
	add r4, r0, #0
	ldrb r2, [r4, #2]
	mov r0, #0xE
	and r0, r2
	cmp r0, #6
	beq _08030944
	add r0, r4, #0
	bl EffectEquipResolve
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08030A1E
_08030944:
	ldr r1, _0803095C @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r1, r3
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _080309DC
	cmp r0, #0x7E
	bgt _08030960
	cmp r0, #0x7D
	beq _080309EA
	b _08030A1C
_0803095C: .4byte 0x02017A40
_08030960:
	cmp r0, #0x7F
	beq _080309C0
	cmp r0, #0x80
	bne _08030A1C
	mov r0, #1
	and r0, r2
	cmp r0, #0
	bne _08030A1C
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	bl CountTributableMonsters
	cmp r0, #0
	beq _08030A12
	ldr r1, _080309AC @ =0x080828CC
	ldr r0, _080309B0 @ =0x08624052
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _080309B4 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _080309B8 @ =0x00000205
	ldr r1, _080309BC @ =0x00000914
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7F
	b _08030A1E
_080309AC: .4byte gStrTributeToReturnToDeckPrompt
_080309B0: .4byte gUnk_08624052
_080309B4: .4byte gCardNames
_080309B8: .4byte 0x00000205
_080309BC: .4byte 0x00000914
_080309C0:
	ldr r0, _080309D4 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08030A12
	ldr r2, _080309D8 @ =0x000003E5
	add r1, r1, r2
	mov r0, #0
	strb r0, [r1]
_080309D0:
	mov r0, #0x7E
	b _08030A1E
_080309D4: .4byte 0x0201AE60
_080309D8: .4byte 0x000003E5
_080309DC:
	add r0, r4, #0
	bl EffectTributeTargetChainB
	cmp r0, #0
	beq _080309D0
	mov r0, #0x7D
	b _08030A1E
_080309EA:
	ldrb r0, [r4, #0xC]
	ldrh r3, [r4, #0xC]
	lsr r1, r3, #8
	bl TributeMonster
	cmp r0, #0
	beq _08030A12
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xD0
	cmp r0, #0
	beq _08030A06
	ldr r2, _08030A18 @ =0x000080D0
_08030A06:
	ldrh r1, [r4]
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08030A12:
	mov r0, #0x64
	b _08030A1E
	.align 2, 0
_08030A18: .4byte 0x000080D0
_08030A1C:
	mov r0, #0
_08030A1E:
	add sp, #0x100
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectAxeOfDespairResolve
	.align 2, 0

