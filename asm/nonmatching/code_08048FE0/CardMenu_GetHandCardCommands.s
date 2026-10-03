	thumb_func_start CardMenu_GetHandCardCommands
CardMenu_GetHandCardCommands: @ 0x08049514
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x14
	add r6, r1, #0
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r5, #0
	ldr r0, _080495AC @ =0x020192E0
	mov r8, r0
	ldr r0, _080495B0 @ =0x00001B12
	add r0, r8
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #2
	beq _0804953C
	cmp r0, #4
	beq _0804953C
	b _0804977E
_0804953C:
	ldr r4, _080495B4 @ =0x000007FF
	and r4, r7
	lsl r0, r4, #2
	ldr r1, _080495B8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _080495F4
	cmp r0, #0x16
	bne _08049606
	add r0, r6, #0
	add r1, r7, #0
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	bne _08049566
	b _0804977E
_08049566:
	add r0, r6, #0
	add r1, r7, #0
	mov r2, #1
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _08049576
	mov r5, #0x40
_08049576:
	mov r0, #0x10
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
	lsl r0, r4, #1
	ldr r1, _080495BC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xA4
	lsl r0, r0, #3
	cmp r1, r0
	beq _080495C4
	cmp r1, r0
	bge _08049594
	b _0804977E
_08049594:
	add r0, #0xE8
	cmp r1, r0
	ble _0804959C
	b _0804977E
_0804959C:
	sub r0, #3
	cmp r1, r0
	bge _080495A4
	b _0804977E
_080495A4:
	ldr r0, _080495C0 @ =0x0000FFEF
	and r5, r0
	b _0804977E
	.align 2, 0
_080495AC: .4byte 0x020192E0
_080495B0: .4byte 0x00001B12
_080495B4: .4byte 0x000007FF
_080495B8: .4byte gCardStats
_080495BC: .4byte gCardIdToNumber
_080495C0: .4byte 0x0000FFEF
_080495C4:
	mov r2, r8
	add r2, #4
	mov r0, #1
	and r0, r6
	ldr r1, _080495EC @ =0x00000D64
	mul r0, r1
	add r2, r0, r2
	ldrb r1, [r2, #9]
	lsl r0, r1, #0x1A
	cmp r0, #0
	blt _080495E4
	ldrb r2, [r2, #8]
	lsl r0, r2, #0x1B
	cmp r0, #0
	blt _080495E4
	b _0804977E
_080495E4:
	ldr r0, _080495F0 @ =0x0000FFBF
	and r5, r0
	b _0804977E
	.align 2, 0
_080495EC: .4byte 0x00000D64
_080495F0: .4byte 0x0000FFBF
_080495F4:
	add r0, r6, #0
	add r1, r7, #0
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	bne _08049602
	b _0804977E
_08049602:
	mov r5, #0x10
	b _0804977E
_08049606:
	add r0, r6, #0
	add r1, r7, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _080496F4
	add r0, r7, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	beq _08049634
	mov r1, #0xC0
	lsl r1, r1, #5
	add r0, r1, #0
	orr r5, r0
	mov r0, #0
	bl CanSpecialSummon
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080496D4
	mov r5, #0
	b _080496D4
_08049634:
	mov r0, #1
	and r0, r6
	ldr r1, _08049664 @ =0x00000D64
	mul r0, r1
	add r0, r8
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1B
	cmp r0, #0
	blt _08049648
	mov r5, #0x30
_08049648:
	lsl r0, r4, #1
	ldr r1, _08049668 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804966C @ =0x00000546
	cmp r1, r0
	beq _08049698
	cmp r1, r0
	bgt _08049670
	mov r0, #0xBD
	lsl r0, r0, #1
	cmp r1, r0
	beq _0804967A
	b _080496D4
_08049664: .4byte 0x00000D64
_08049668: .4byte gCardIdToNumber
_0804966C: .4byte 0x00000546
_08049670:
	mov r0, #0xBE
	lsl r0, r0, #3
	cmp r1, r0
	beq _08049686
	b _080496D4
_0804967A:
	ldr r1, _08049694 @ =0x000004DB
	add r0, r6, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _080496D4
_08049686:
	mov r1, #0xC0
	lsl r1, r1, #5
	add r0, r1, #0
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
	b _080496D4
_08049694: .4byte 0x000004DB
_08049698:
	mov r0, #0
	bl CountMonsters
	add r4, r0, #0
	add r4, #1
	mov r0, #1
	bl CountMonsters
	cmp r4, r0
	bge _080496C2
	mov r0, #0
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _080496C2
	mov r1, #0xC0
	lsl r1, r1, #5
	add r0, r1, #0
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
_080496C2:
	mov r1, #1
	neg r1, r1
	add r0, r6, #0
	bl CountTributableMonsters
	cmp r0, #0
	bne _080496D4
	ldr r0, _08049710 @ =0x0000FFCF
	and r5, r0
_080496D4:
	mov r0, #0
	bl CanSpecialSummon
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080496E4
	ldr r0, _08049714 @ =0x0000E7FF
	and r5, r0
_080496E4:
	mov r0, #0
	bl CanNormalSummon
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080496F4
	ldr r0, _08049718 @ =0x0000FFDF
	and r5, r0
_080496F4:
	ldr r0, _0804971C @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _08049720 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0x47
	beq _08049724
	mov r0, #0xD4
	lsl r0, r0, #1
	cmp r1, r0
	beq _08049768
	b _0804977E
	.align 2, 0
_08049710: .4byte 0x0000FFCF
_08049714: .4byte 0x0000E7FF
_08049718: .4byte 0x0000FFDF
_0804971C: .4byte 0x000007FF
_08049720: .4byte gCardIdToNumber
_08049724:
	add r0, r6, #0
	add r1, r7, #0
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	beq _0804977E
	add r0, r6, #0
	add r1, r7, #0
	mov r2, #1
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _0804977E
	add r0, r6, #0
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _0804977E
	ldr r2, _08049760 @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08049764 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	cmp r0, #0
	blt _0804977E
	b _08049776
	.align 2, 0
_08049760: .4byte 0x020192E4
_08049764: .4byte 0x00000D64
_08049768:
	add r0, r6, #0
	add r1, r7, #0
	mov r2, #1
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _0804977E
_08049776:
	mov r0, #0x40
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
_0804977E:
	ldr r1, _0804985C @ =0x020192E0
	ldr r0, _08049860 @ =0x00001B12
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080497D4
	ldr r0, _08049864 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08049868 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _080497D4
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #5
	bne _080497D4
	add r0, r6, #0
	add r1, r7, #0
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	beq _080497D4
	add r0, r6, #0
	add r1, r7, #0
	mov r2, #1
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _080497D4
	mov r0, #0x40
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
_080497D4:
	ldr r0, _08049864 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08049868 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08049816
	mov r0, #0x40
	and r0, r5
	cmp r0, #0
	beq _08049816
	ldr r4, _0804986C @ =0x0000049C
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _08049806
	ldr r0, _08049870 @ =0x0000FFBF
	and r5, r0
_08049806:
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _08049816
	ldr r0, _08049870 @ =0x0000FFBF
	and r5, r0
_08049816:
	ldr r0, _08049864 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08049868 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0804984E
	cmp r0, #0x15
	blt _0804984E
	ldr r2, _08049874 @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08049878 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsr r0, r0, #6
	cmp r0, #0
	beq _0804984E
	ldr r0, _0804987C @ =0x0000FFEF
	and r5, r0
	sub r0, #0x30
	and r5, r0
_0804984E:
	add r0, r5, #0
	add sp, #0x14
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0804985C: .4byte 0x020192E0
_08049860: .4byte 0x00001B12
_08049864: .4byte 0x000007FF
_08049868: .4byte gCardStats
_0804986C: .4byte 0x0000049C
_08049870: .4byte 0x0000FFBF
_08049874: .4byte 0x020192E4
_08049878: .4byte 0x00000D64
_0804987C: .4byte 0x0000FFEF
	thumb_func_end CardMenu_GetHandCardCommands

