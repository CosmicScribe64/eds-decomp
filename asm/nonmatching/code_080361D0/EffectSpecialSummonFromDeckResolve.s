	thumb_func_start EffectSpecialSummonFromDeckResolve
EffectSpecialSummonFromDeckResolve: @ 0x08036D30
	push {r4, r5, lr}
	sub sp, #4
	add r4, r0, #0
	ldr r2, _08036D74 @ =0x0201D810
	ldrb r1, [r2, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	ldrh r3, [r2, #6]
	add r0, r3, r0
	lsl r0, r0, #2
	add r1, r2, #0
	add r1, #0xC
	add r5, r0, r1
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08036D56
	b _08036F1C
_08036D56:
	ldr r0, _08036D78 @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	sub r0, #0x7B
	cmp r0, #5
	bls _08036D68
	b _08036F1C
_08036D68:
	lsl r0, r0, #2
	ldr r1, _08036D7C @ =0x08036D80
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08036D74: .4byte 0x0201D810
_08036D78: .4byte 0x02017A40
_08036D7C: .4byte 0x08036D80
_08036D80:
	.4byte _08036EFA
	.4byte _08036ED4
	.4byte _08036EB0
	.4byte _08036E84
	.4byte _08036E5C
	.4byte _08036D98
_08036D98:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CanSpecialSummon
	cmp r0, #0
	bne _08036DA8
	b _08036F1C
_08036DA8:
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08036DE4 @ =0x000007FF
	ldrh r3, [r4]
	and r1, r3
	lsl r1, r1, #1
	ldr r2, _08036DE8 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bne _08036DF8
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	beq _08036DD2
	b _08036F1C
_08036DD2:
	ldr r0, _08036DEC @ =0x00000206
	ldr r1, _08036DF0 @ =0x00000712
	ldr r3, _08036DF4 @ =0x08082F34
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x64
	b _08036F1E
	.align 2, 0
_08036DE4: .4byte 0x000007FF
_08036DE8: .4byte gCardIdToNumber
_08036DEC: .4byte 0x00000206
_08036DF0: .4byte 0x00000712
_08036DF4: .4byte gStrRecruiterNoCardsInDeck
_08036DF8:
	mov r0, #1
	ldrb r3, [r4, #2]
	and r0, r3
	cmp r0, #0
	beq _08036E34
	ldrh r0, [r4]
	bl AiPickCardListEntry
	cmp r0, #0
	bge _08036E0E
	b _08036F1C
_08036E0E:
	ldr r1, _08036E28 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1, #5]
	and r0, r2
	strb r0, [r1, #5]
	ldr r0, _08036E2C @ =0x02015F00
	ldr r3, _08036E30 @ =0x00001B22
	add r0, r0, r3
	ldrh r0, [r0]
	strh r0, [r1, #6]
	mov r0, #0x7D
	b _08036F1E
_08036E28: .4byte 0x0201D810
_08036E2C: .4byte 0x02015F00
_08036E30: .4byte 0x00001B22
_08036E34:
	ldr r0, _08036E50 @ =0x00000206
	ldr r1, _08036E54 @ =0x00000712
	ldr r3, _08036E58 @ =0x08082F64
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7F
	b _08036F1E
	.align 2, 0
_08036E50: .4byte 0x00000206
_08036E54: .4byte 0x00000712
_08036E58: .4byte gStrRecruiterSummonPrompt
_08036E5C:
	ldr r0, _08036E74 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08036F1C
	ldr r0, _08036E78 @ =0x00000206
	ldr r1, _08036E7C @ =0x00000712
	ldr r3, _08036E80 @ =0x08082FA4
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7E
	b _08036F1E
_08036E74: .4byte 0x0201AE60
_08036E78: .4byte 0x00000206
_08036E7C: .4byte 0x00000712
_08036E80: .4byte gStrRecruiterSelectMonster
_08036E84:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _08036EA8 @ =0x000007FF
	ldrh r4, [r4]
	and r2, r4
	lsl r2, r2, #1
	ldr r3, _08036EAC @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7D
	b _08036F1E
	.align 2, 0
_08036EA8: .4byte 0x000007FF
_08036EAC: .4byte gCardIdToNumber
_08036EB0:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0x65
	cmp r0, #0
	beq _08036EBE
	ldr r3, _08036ED0 @ =0x00008065
_08036EBE:
	ldrh r1, [r5]
	ldrh r2, [r5, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7C
	b _08036F1E
	.align 2, 0
_08036ED0: .4byte 0x00008065
_08036ED4:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	mov r2, #0
	str r2, [sp, #0]
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummon
	mov r0, #0x7B
	b _08036F1E
_08036EFA:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _08036F08
	ldr r1, _08036F18 @ =0x00008060
_08036F08:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x64
	b _08036F1E
_08036F18: .4byte 0x00008060
_08036F1C:
	mov r0, #0
_08036F1E:
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectSpecialSummonFromDeckResolve
	.align 2, 0

