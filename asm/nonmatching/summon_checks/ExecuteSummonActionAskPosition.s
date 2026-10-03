	thumb_func_start ExecuteSummonActionAskPosition
ExecuteSummonActionAskPosition: @ 0x08054B60
	push {r4, r5, r6, lr}
	ldr r1, _08054B7C @ =0x0201CF90
	ldrh r2, [r1, #0xE]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	add r4, r1, #0
	cmp r0, #4
	bls _08054B72
	b _08054E74
_08054B72:
	lsl r0, r0, #2
	ldr r1, _08054B80 @ =0x08054B84
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08054B7C: .4byte 0x0201CF90
_08054B80: .4byte 0x08054B84
_08054B84:
	.4byte _08054B98
	.4byte _08054BF0
	.4byte _08054C50
	.4byte _08054CCC
	.4byte _08054D58
_08054B98:
	ldrb r3, [r4]
	lsl r0, r3, #0x1F
	cmp r0, #0
	beq _08054BC4
	ldrb r0, [r4, #3]
	lsr r1, r0, #7
	ldr r0, _08054BBC @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	mov r1, #0
	bl AiShouldSetMonster
	ldr r1, _08054BC0 @ =0x0201AE60
	strh r0, [r1, #0x14]
	b _08054D34
	.align 2, 0
_08054BBC: .4byte 0x00007FFF
_08054BC0: .4byte 0x0201AE60
_08054BC4:
	ldr r0, _08054BDC @ =0x00000207
	ldr r1, _08054BE0 @ =0x0000030F
	ldr r3, _08054BE4 @ =0x08086370
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _08054BE8 @ =0x08054771
	ldr r2, _08054BEC @ =0x0805487D
	mov r0, #5
	bl TextBoxSetMenu
	b _08054D34
_08054BDC: .4byte 0x00000207
_08054BE0: .4byte 0x0000030F
_08054BE4: .4byte gStrSelectDisplayPosition
_08054BE8: .4byte SummonPositionMenu_Draw
_08054BEC: .4byte SummonPositionMenu_HandleInput
_08054BF0:
	ldr r2, _08054C10 @ =0x0201CF90
	ldr r0, _08054C14 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	lsl r0, r0, #7
	mov r1, #0x7F
	ldrb r3, [r2, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r2, #1]
	lsl r0, r1, #0x18
	cmp r0, #0
	bge _08054C18
	mov r0, #0x41
	neg r0, r0
	and r1, r0
	b _08054C1C
_08054C10: .4byte 0x0201CF90
_08054C14: .4byte 0x0201AE60
_08054C18:
	mov r0, #0x40
	orr r1, r0
_08054C1C:
	strb r1, [r2, #1]
	ldr r4, _08054C48 @ =0x0000047F
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08054C3A
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08054C3A
	b _08054E4E
_08054C3A:
	ldr r1, _08054C4C @ =0x0201CF90
	mov r0, #0x40
	ldrb r2, [r1, #1]
	orr r0, r2
	strb r0, [r1, #1]
	b _08054E4E
	.align 2, 0
_08054C48: .4byte 0x0000047F
_08054C4C: .4byte 0x0201CF90
_08054C50:
	ldrb r3, [r4, #3]
	lsl r0, r3, #0x1E
	cmp r0, #0
	bge _08054C68
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrb r2, [r4, #2]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	bl TributeMonster
_08054C68:
	ldrb r3, [r4, #3]
	lsl r0, r3, #0x1D
	cmp r0, #0
	bge _08054C80
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrb r2, [r4, #2]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1D
	bl TributeMonster
_08054C80:
	ldrb r5, [r4]
	lsl r0, r5, #0x1F
	mov r6, #0xC4
	cmp r0, #0
	beq _08054C8C
	ldr r6, _08054CC4 @ =0x000080C4
_08054C8C:
	ldrb r3, [r4, #3]
	lsr r0, r3, #7
	ldr r1, _08054CC8 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	ldrh r3, [r4]
	lsr r0, r3, #6
	mov r3, #0xF
	add r2, r3, #0
	and r2, r0
	lsl r2, r2, #4
	lsl r0, r5, #0x1A
	lsr r0, r0, #0x1B
	and r3, r0
	orr r2, r3
	ldrb r3, [r4, #1]
	lsl r0, r3, #0x19
	lsr r0, r0, #0x1F
	lsr r3, r3, #7
	lsl r3, r3, #1
	orr r0, r3
	lsl r0, r0, #8
	orr r2, r0
	add r0, r6, #0
	b _08054D2E
	.align 2, 0
_08054CC4: .4byte 0x000080C4
_08054CC8: .4byte 0x00007FFF
_08054CCC:
	add r5, r4, #0
	ldrb r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0
	bl DuelCursor_Select
	ldrb r1, [r5]
	lsl r0, r1, #0x1F
	mov r3, #0x90
	cmp r0, #0
	beq _08054CEA
	ldr r3, _08054D14 @ =0x00008090
_08054CEA:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrh r2, [r5, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r5, #1]
	lsl r0, r1, #0x19
	cmp r0, #0
	blt _08054D1C
	ldr r0, _08054D18 @ =0xFFFFF01F
	ldrh r2, [r5, #0xE]
	and r0, r2
	mov r3, #0xA0
	lsl r3, r3, #1
	add r1, r3, #0
	orr r0, r1
	strh r0, [r5, #0xE]
	mov r0, #0
	b _08054E76
_08054D14: .4byte 0x00008090
_08054D18: .4byte 0xFFFFF01F
_08054D1C:
	ldrb r1, [r4, #3]
	lsr r0, r1, #7
	ldr r1, _08054D50 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	mov r0, #0x71
	mov r2, #1
_08054D2E:
	mov r3, #0
	bl DuelCmd_Push
_08054D34:
	ldrh r2, [r4, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08054D54 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r4, #0xE]
	mov r0, #0
	b _08054E76
	.align 2, 0
_08054D50: .4byte 0x00007FFF
_08054D54: .4byte 0xFFFFF01F
_08054D58:
	ldr r4, _08054D98 @ =0x0201CF90
	ldrb r3, [r4]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl TriggerMysteriousPuppeteer
	ldrb r0, [r4, #3]
	lsr r1, r0, #7
	ldr r0, _08054D9C @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	ldr r1, _08054DA0 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08054DA4 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08054DA8 @ =0x00000462
	cmp r1, r0
	beq _08054DEC
	cmp r1, r0
	bgt _08054DC0
	mov r0, #0xC7
	lsl r0, r0, #2
	cmp r1, r0
	beq _08054E28
	cmp r1, r0
	bgt _08054DB0
	ldr r0, _08054DAC @ =0x000001F3
	b _08054DCC
_08054D98: .4byte 0x0201CF90
_08054D9C: .4byte 0x00007FFF
_08054DA0: .4byte 0x000007FF
_08054DA4: .4byte gCardIdToNumber
_08054DA8: .4byte 0x00000462
_08054DAC: .4byte 0x000001F3
_08054DB0:
	ldr r0, _08054DBC @ =0x00000455
	cmp r1, r0
	beq _08054DEC
	add r0, #9
	b _08054DE0
	.align 2, 0
_08054DBC: .4byte 0x00000455
_08054DC0:
	ldr r0, _08054DD4 @ =0x000004DE
	cmp r1, r0
	beq _08054DEC
	cmp r1, r0
	bgt _08054DD8
	sub r0, #6
_08054DCC:
	cmp r1, r0
	beq _08054DEC
	b _08054E4E
	.align 2, 0
_08054DD4: .4byte 0x000004DE
_08054DD8:
	ldr r0, _08054DE8 @ =0x00000534
	cmp r1, r0
	beq _08054DEC
	add r0, #0x51
_08054DE0:
	cmp r1, r0
	beq _08054E3C
	b _08054E4E
	.align 2, 0
_08054DE8: .4byte 0x00000534
_08054DEC:
	ldr r5, _08054E20 @ =0x0201CF90
	ldrb r4, [r5]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	lsl r0, r1, #0x1F
	lsl r4, r4, #0x1A
	lsr r4, r4, #0x1B
	lsl r2, r4, #0x10
	mov r3, #0xA4
	lsl r3, r3, #0x14
	orr r2, r3
	orr r0, r2
	ldrb r2, [r5, #3]
	lsr r3, r2, #7
	ldr r2, _08054E24 @ =0x00007FFF
	ldrh r5, [r5, #4]
	and r2, r5
	lsl r2, r2, #1
	orr r2, r3
	orr r0, r2
	lsl r4, r4, #8
	orr r1, r4
	bl Chain_AddPending
	b _08054E4E
	.align 2, 0
_08054E20: .4byte 0x0201CF90
_08054E24: .4byte 0x00007FFF
_08054E28:
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	mov r2, #0
	mov r3, #0
	bl ChangeBattlePosition
	b _08054E4E
_08054E3C:
	ldr r0, _08054E6C @ =0x0201CF90
	ldrb r1, [r0]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	mov r2, #1
	bl DestroyFieldCard
_08054E4E:
	ldr r3, _08054E6C @ =0x0201CF90
	ldrh r2, [r3, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08054E70 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3, #0xE]
	mov r0, #0
	b _08054E76
	.align 2, 0
_08054E6C: .4byte 0x0201CF90
_08054E70: .4byte 0xFFFFF01F
_08054E74:
	mov r0, #1
_08054E76:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end ExecuteSummonActionAskPosition

