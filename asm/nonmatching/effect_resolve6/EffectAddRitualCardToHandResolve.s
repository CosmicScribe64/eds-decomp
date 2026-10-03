	thumb_func_start EffectAddRitualCardToHandResolve
EffectAddRitualCardToHandResolve: @ 0x08036F28
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08036F38
	b _08037078
_08036F38:
	ldr r0, _08036F54 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _08037018
	cmp r0, #0x7E
	bgt _08036F58
	cmp r0, #0x7D
	bne _08036F50
	b _08037044
_08036F50:
	b _08037078
	.align 2, 0
_08036F54: .4byte 0x02017A40
_08036F58:
	cmp r0, #0x7F
	beq _08036FF0
	cmp r0, #0x80
	beq _08036F62
	b _08037078
_08036F62:
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r5, _08036FA0 @ =0x000007FF
	add r1, r5, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	ldr r3, _08036FA4 @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bne _08036F84
	b _08037078
_08036F84:
	add r0, r5, #0
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08036FA4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08036FA8 @ =0x00000455
	cmp r1, r0
	beq _08036FAC
	add r0, #0xD
	cmp r1, r0
	beq _08036FC8
	b _08037078
_08036FA0: .4byte 0x000007FF
_08036FA4: .4byte gCardIdToNumber
_08036FA8: .4byte 0x00000455
_08036FAC:
	ldr r0, _08036FBC @ =0x00000206
	ldr r1, _08036FC0 @ =0x00000712
	ldr r3, _08036FC4 @ =0x08083020
	mov r2, #0xB
	bl TextBoxOpen
	b _08036FD4
	.align 2, 0
_08036FBC: .4byte 0x00000206
_08036FC0: .4byte 0x00000712
_08036FC4: .4byte gStrSenjuAddRitualMonsterPrompt
_08036FC8:
	ldr r0, _08036FE4 @ =0x00000206
	ldr r1, _08036FE8 @ =0x00000712
	ldr r3, _08036FEC @ =0x0808306C
	mov r2, #0xB
	bl TextBoxOpen
_08036FD4:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7F
	b _0803707A
	.align 2, 0
_08036FE4: .4byte 0x00000206
_08036FE8: .4byte 0x00000712
_08036FEC: .4byte gStrSonicBirdAddRitualMagicPrompt
_08036FF0:
	ldr r0, _08037008 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08037078
	ldr r0, _0803700C @ =0x00000206
	ldr r1, _08037010 @ =0x00000712
	ldr r3, _08037014 @ =0x080830B4
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7E
	b _0803707A
_08037008: .4byte 0x0201AE60
_0803700C: .4byte 0x00000206
_08037010: .4byte 0x00000712
_08037014: .4byte gStrRitualSearchSelectCard
_08037018:
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803703C @ =0x000007FF
	ldrh r4, [r4]
	and r2, r4
	lsl r2, r2, #1
	ldr r3, _08037040 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7D
	b _0803707A
	.align 2, 0
_0803703C: .4byte 0x000007FF
_08037040: .4byte gCardIdToNumber
_08037044:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _08037070 @ =0x0201D810
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x15
	lsr r1, r1, #0x14
	ldr r2, _08037074 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	bl AddDeckCardToHand
	mov r0, #0x7C
	b _0803707A
_08037070: .4byte 0x0201D810
_08037074: .4byte gCardIdToNumber
_08037078:
	mov r0, #0
_0803707A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectAddRitualCardToHandResolve

