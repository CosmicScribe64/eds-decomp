	thumb_func_start AiStrategyValkyrion
AiStrategyValkyrion: @ 0x0805C508
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r0, _0805C520 @ =0x02015EF0
	ldrb r0, [r0, #2]
	cmp r0, #8
	bls _0805C516
	b _0805C92C
_0805C516:
	lsl r0, r0, #2
	ldr r1, _0805C524 @ =0x0805C528
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0805C520: .4byte 0x02015EF0
_0805C524: .4byte 0x0805C528
_0805C528:
	.4byte _0805C54C
	.4byte _0805C5BC
	.4byte _0805C5F0
	.4byte _0805C5BC
	.4byte _0805C624
	.4byte _0805C790
	.4byte _0805C7E0
	.4byte _0805C890
	.4byte _0805C8E8
_0805C54C:
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl CountSpellTrapsFiltered
	cmp r0, #0
	ble _0805C5AA
	ldr r0, _0805C57C @ =0x0000029F
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C590
	ldr r2, _0805C580 @ =0x020192E0
	ldr r0, _0805C584 @ =0x00001B30
	add r1, r2, r0
	ldr r0, _0805C588 @ =0xFFFFFC03
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r5, _0805C58C @ =0x02015EF0
	ldrb r6, [r5, #0xB]
	lsl r0, r6, #2
	b _0805C80C
_0805C57C: .4byte 0x0000029F
_0805C580: .4byte 0x020192E0
_0805C584: .4byte 0x00001B30
_0805C588: .4byte 0xFFFFFC03
_0805C58C: .4byte 0x02015EF0
_0805C590:
	ldr r0, _0805C5B4 @ =0x00000425
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C59C
	b _0805C7F8
_0805C59C:
	mov r0, #0x87
	lsl r0, r0, #3
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C5AA
	b _0805C7F8
_0805C5AA:
	ldr r1, _0805C5B8 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #2
	b _0805C91C
	.align 2, 0
_0805C5B4: .4byte 0x00000425
_0805C5B8: .4byte 0x02015EF0
_0805C5BC:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r1, _0805C5E4 @ =0x020192E0
	ldr r2, _0805C5E8 @ =0x00001B2C
	add r1, r1, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805C5D8
	b _0805C84C
_0805C5D8:
	ldr r1, _0805C5EC @ =0x02015EF0
	ldrb r0, [r1, #2]
	sub r0, #1
	strb r0, [r1, #2]
	b _0805C84C
	.align 2, 0
_0805C5E4: .4byte 0x020192E0
_0805C5E8: .4byte 0x00001B2C
_0805C5EC: .4byte 0x02015EF0
_0805C5F0:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _0805C614
	ldr r0, _0805C61C @ =0x0000014F
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C606
	b _0805C7F8
_0805C606:
	mov r0, #0xA8
	lsl r0, r0, #1
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C614
	b _0805C7F8
_0805C614:
	ldr r1, _0805C620 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #2
	b _0805C91C
_0805C61C: .4byte 0x0000014F
_0805C620: .4byte 0x02015EF0
_0805C624:
	ldr r0, _0805C644 @ =0x0862448E
	ldrh r1, [r0]
	mov r0, #1
	bl CanSummonFromHand
	cmp r0, #0
	bne _0805C650
	ldr r1, _0805C648 @ =0x02015F00
	ldr r2, _0805C64C @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805C8F6
	.align 2, 0
_0805C644: .4byte gCardNumberToId_ValkyrionTheMagnaWarrior
_0805C648: .4byte 0x02015F00
_0805C64C: .4byte 0x00001B24
_0805C650:
	mov r1, #0
_0805C652:
	mov r2, #0
	cmp r1, #1
	beq _0805C670
	cmp r1, #1
	bgt _0805C662
	cmp r1, #0
	beq _0805C668
	b _0805C67A
_0805C662:
	cmp r1, #2
	beq _0805C676
	b _0805C67A
_0805C668:
	ldr r6, _0805C66C @ =0x000002E1
	b _0805C67A
_0805C66C: .4byte 0x000002E1
_0805C670:
	mov r6, #0xBD
	lsl r6, r6, #2
	b _0805C67A
_0805C676:
	mov r6, #0xC8
	lsl r6, r6, #2
_0805C67A:
	mov r4, #0
	add r7, r1, #1
	cmp r2, #0
	bne _0805C6B8
_0805C682:
	mov r0, #0x94
	mul r0, r4
	ldr r1, _0805C764 @ =0x0201A070
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0805C6AE
	ldr r0, _0805C768 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0805C76C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	bne _0805C6AE
	mov r0, #1
	add r1, r4, #0
	bl TributeMonster
	mov r2, #1
_0805C6AE:
	add r4, #1
	cmp r4, #4
	bgt _0805C6B8
	cmp r2, #0
	beq _0805C682
_0805C6B8:
	mov r4, #0
	ldr r0, _0805C770 @ =0x020192E4
	ldr r3, _0805C774 @ =0x00000D66
	add r0, r0, r3
	ldrb r1, [r0]
	cmp r4, r1
	bge _0805C706
	cmp r2, #0
	bne _0805C706
	add r5, r0, #0
_0805C6CC:
	lsl r0, r4, #2
	ldr r1, _0805C778 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0805C6FA
	ldr r0, _0805C768 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _0805C76C @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, r6
	bne _0805C6FA
	mov r0, #1
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl DiscardHandCard
	mov r2, #1
_0805C6FA:
	add r4, #1
	ldrb r0, [r5]
	cmp r4, r0
	bge _0805C706
	cmp r2, #0
	beq _0805C6CC
_0805C706:
	add r1, r7, #0
	cmp r1, #2
	ble _0805C652
	mov r0, #1
	bl FindFreeMonsterZone
	ldr r1, _0805C77C @ =0x02015F00
	ldr r2, _0805C780 @ =0x00001B25
	add r1, r1, r2
	strb r0, [r1]
	mov r1, #0
	ldr r2, _0805C770 @ =0x020192E4
	ldr r3, _0805C774 @ =0x00000D66
	add r0, r2, r3
	ldrb r0, [r0]
	cmp r1, r0
	bge _0805C752
	ldr r6, _0805C784 @ =0x0000034D
	add r0, r2, r3
	ldrb r3, [r0]
	ldr r0, _0805C788 @ =0x000013E8
	add r2, r2, r0
	ldr r5, _0805C768 @ =0x000007FF
	ldr r4, _0805C76C @ =0x08622AB4
_0805C736:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r6
	bne _0805C74A
	b _0805C904
_0805C74A:
	add r2, #4
	add r1, #1
	cmp r1, r3
	blt _0805C736
_0805C752:
	ldr r1, _0805C77C @ =0x02015F00
	ldr r2, _0805C78C @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805C8F6
	.align 2, 0
_0805C764: .4byte 0x0201A070
_0805C768: .4byte 0x000007FF
_0805C76C: .4byte gCardIdToNumber
_0805C770: .4byte 0x020192E4
_0805C774: .4byte 0x00000D66
_0805C778: .4byte 0x0201A6CC
_0805C77C: .4byte 0x02015F00
_0805C780: .4byte 0x00001B25
_0805C784: .4byte 0x0000034D
_0805C788: .4byte 0x000013E8
_0805C78C: .4byte 0x00001B24
_0805C790:
	ldr r0, _0805C7CC @ =0x00008008
	ldr r4, _0805C7D0 @ =0x02015F00
	ldr r6, _0805C7D4 @ =0x00001B25
	add r4, r4, r6
	ldrb r1, [r4]
	lsl r2, r1, #8
	mov r1, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r2, [r4]
	mov r1, #0x1F
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #0x10
	mov r1, #0x94
	mul r1, r2
	ldr r2, _0805C7D8 @ =0x0201A070
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r2, _0805C7DC @ =0x80400000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	b _0805C916
	.align 2, 0
_0805C7CC: .4byte 0x00008008
_0805C7D0: .4byte 0x02015F00
_0805C7D4: .4byte 0x00001B25
_0805C7D8: .4byte 0x0201A070
_0805C7DC: .4byte 0x80400000
_0805C7E0:
	mov r0, #0x91
	lsl r0, r0, #3
	bl AiTryPlaySpellTrap
	cmp r0, #0
	bne _0805C7F8
	mov r0, #0xFC
	lsl r0, r0, #2
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C878
_0805C7F8:
	ldr r2, _0805C850 @ =0x020192E0
	ldr r3, _0805C854 @ =0x00001B30
	add r1, r2, r3
	ldr r0, _0805C858 @ =0xFFFFFC03
	ldrh r6, [r1]
	and r0, r6
	strh r0, [r1]
	ldr r5, _0805C85C @ =0x02015EF0
	ldrb r1, [r5, #0xB]
	lsl r0, r1, #2
_0805C80C:
	ldr r3, _0805C860 @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805C864 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805C868 @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805C86C @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805C870 @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805C874 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
_0805C84C:
	mov r0, #0
	b _0805C92E
_0805C850: .4byte 0x020192E0
_0805C854: .4byte 0x00001B30
_0805C858: .4byte 0xFFFFFC03
_0805C85C: .4byte 0x02015EF0
_0805C860: .4byte 0x000013EC
_0805C864: .4byte 0x00001B28
_0805C868: .4byte 0x00001B33
_0805C86C: .4byte 0x00001B34
_0805C870: .4byte 0xFFFFFE01
_0805C874: .4byte 0x00001B2C
_0805C878:
	ldr r1, _0805C888 @ =0x02015F00
	ldr r2, _0805C88C @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805C8F6
_0805C888: .4byte 0x02015F00
_0805C88C: .4byte 0x00001B24
_0805C890:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r3, _0805C8CC @ =0x020192E0
	ldr r6, _0805C8D0 @ =0x00001B2C
	add r1, r3, r6
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805C84C
	ldr r0, _0805C8D4 @ =0x00001B14
	add r2, r3, r0
	ldr r0, [r2]
	ldr r1, _0805C8D8 @ =0xFFFE01FF
	and r0, r1
	str r0, [r2]
	ldr r2, _0805C8DC @ =0x00001B16
	add r1, r3, r2
	ldr r0, _0805C8E0 @ =0xFFFFFE01
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r1, _0805C8E4 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
	b _0805C84C
_0805C8CC: .4byte 0x020192E0
_0805C8D0: .4byte 0x00001B2C
_0805C8D4: .4byte 0x00001B14
_0805C8D8: .4byte 0xFFFE01FF
_0805C8DC: .4byte 0x00001B16
_0805C8E0: .4byte 0xFFFFFE01
_0805C8E4: .4byte 0x02015EF0
_0805C8E8:
	ldr r1, _0805C8FC @ =0x02015F00
	ldr r6, _0805C900 @ =0x00001B24
	add r1, r1, r6
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0805C8F6:
	strb r0, [r1]
	b _0805C84C
	.align 2, 0
_0805C8FC: .4byte 0x02015F00
_0805C900: .4byte 0x00001B24
_0805C904:
	ldr r0, _0805C920 @ =0x02015F00
	ldr r3, _0805C924 @ =0x00001B25
	add r0, r0, r3
	ldrb r2, [r0]
	mov r0, #1
	str r0, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
_0805C916:
	ldr r1, _0805C928 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #1
_0805C91C:
	strb r0, [r1, #2]
	b _0805C84C
_0805C920: .4byte 0x02015F00
_0805C924: .4byte 0x00001B25
_0805C928: .4byte 0x02015EF0
_0805C92C:
	mov r0, #1
_0805C92E:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiStrategyValkyrion
	.align 2, 0

