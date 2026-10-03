	thumb_func_start CardMenu_Execute
CardMenu_Execute: @ 0x0801E260
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r1, _0801E288 @ =0x020192E0
	ldr r2, _0801E28C @ =0x00001B2C
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	sub r0, #1
	add r5, r1, #0
	cmp r0, #0xB
	bls _0801E27E
	b _0801E90C
_0801E27E:
	lsl r0, r0, #2
	ldr r1, _0801E290 @ =0x0801E294
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0801E288: .4byte 0x020192E0
_0801E28C: .4byte 0x00001B2C
_0801E290: .4byte 0x0801E294
_0801E294:
	.4byte _0801E2C4
	.4byte _0801E2C4
	.4byte _0801E2FC
	.4byte _0801E328
	.4byte _0801E380
	.4byte _0801E39E
	.4byte _0801E80C
	.4byte _0801E854
	.4byte _0801E87C
	.4byte _0801E398
	.4byte _0801E38A
	.4byte _0801E38E
_0801E2C4:
	ldr r3, _0801E2F0 @ =0x00001B33
	add r0, r5, r3
	ldrb r0, [r0]
	lsr r2, r0, #2
	ldr r4, _0801E2F4 @ =0x00001B34
	add r1, r5, r4
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #6
	orr r0, r2
	cmp r0, #0
	beq _0801E2E0
	b _0801E90C
_0801E2E0:
	ldr r6, _0801E2F8 @ =0x00001B2C
	add r0, r5, r6
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	bl CardMenu_ChangePosition
	b _0801E928
_0801E2F0: .4byte 0x00001B33
_0801E2F4: .4byte 0x00001B34
_0801E2F8: .4byte 0x00001B2C
_0801E2FC:
	ldr r1, _0801E320 @ =0x00001B33
	add r0, r5, r1
	ldrb r0, [r0]
	lsr r2, r0, #2
	ldr r3, _0801E324 @ =0x00001B34
	add r1, r5, r3
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #6
	orr r0, r2
	cmp r0, #0
	beq _0801E318
	b _0801E90C
_0801E318:
	bl CardMenu_FlipSummon
	b _0801E928
	.align 2, 0
_0801E320: .4byte 0x00001B33
_0801E324: .4byte 0x00001B34
_0801E328:
	ldr r4, _0801E368 @ =0x00001B33
	add r0, r5, r4
	ldrb r0, [r0]
	lsr r2, r0, #2
	ldr r6, _0801E36C @ =0x00001B34
	add r1, r5, r6
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #6
	orr r0, r2
	cmp r0, #0xB
	beq _0801E344
	b _0801E90C
_0801E344:
	ldr r0, _0801E370 @ =0x00001B28
	add r1, r5, r0
	ldr r0, _0801E374 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0801E378 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0801E37C
	mov r0, #0
	b _0801E382
	.align 2, 0
_0801E368: .4byte 0x00001B33
_0801E36C: .4byte 0x00001B34
_0801E370: .4byte 0x00001B28
_0801E374: .4byte 0x000007FF
_0801E378: .4byte gCardStats
_0801E37C:
	mov r0, #0
	b _0801E43E
_0801E380:
	mov r0, #1
_0801E382:
	mov r1, #0
	bl CardMenu_SummonMonster
	b _0801E928
_0801E38A:
	mov r0, #1
	b _0801E390
_0801E38E:
	mov r0, #0
_0801E390:
	mov r1, #1
	bl CardMenu_SummonMonster
	b _0801E928
_0801E398:
	bl CardMenu_FusionSummon
	b _0801E928
_0801E39E:
	ldr r2, _0801E3CC @ =0x00001B33
	add r2, r2, r5
	mov r8, r2
	ldrb r3, [r2]
	lsr r1, r3, #2
	ldr r4, _0801E3D0 @ =0x00001B34
	add r7, r5, r4
	mov r6, #1
	add r0, r6, #0
	ldrb r2, [r7]
	and r0, r2
	lsl r4, r0, #6
	orr r4, r1
	add r0, r4, #0
	cmp r0, #5
	bne _0801E3C0
	b _0801E534
_0801E3C0:
	cmp r0, #5
	bgt _0801E3D4
	cmp r0, #0
	bne _0801E3CA
	b _0801E64C
_0801E3CA:
	b _0801E90C
_0801E3CC: .4byte 0x00001B33
_0801E3D0: .4byte 0x00001B34
_0801E3D4:
	cmp r0, #0xA
	beq _0801E478
	cmp r0, #0xB
	beq _0801E3DE
	b _0801E90C
_0801E3DE:
	ldr r0, _0801E414 @ =0x00001B28
	add r4, r5, r0
	ldr r2, _0801E418 @ =0x000007FF
	ldrh r1, [r4]
	and r2, r1
	lsl r0, r2, #2
	ldr r1, _0801E41C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0801E43C
	lsl r0, r2, #1
	ldr r2, _0801E420 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	cmp r1, #0x47
	beq _0801E424
	mov r0, #0xD4
	lsl r0, r0, #1
	cmp r1, r0
	beq _0801E44C
	b _0801E90C
	.align 2, 0
_0801E414: .4byte 0x00001B28
_0801E418: .4byte 0x000007FF
_0801E41C: .4byte gCardStats
_0801E420: .4byte gCardIdToNumber
_0801E424:
	add r2, r5, #4
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	add r1, r6, #0
	and r1, r0
	ldr r0, _0801E448 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #0x10
	ldrb r3, [r1, #8]
	orr r0, r3
	strb r0, [r1, #8]
_0801E43C:
	mov r0, #1
_0801E43E:
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	b _0801E928
_0801E448: .4byte 0x00000D64
_0801E44C:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	ldrh r7, [r7]
	lsl r1, r7, #0x17
	lsr r1, r1, #0x18
	mov r2, #0
	mov r3, #1
	bl DiscardHandCard
	mov r0, r8
	ldrb r0, [r0]
	lsl r1, r0, #0x1E
	lsr r1, r1, #0x1F
	add r0, r6, #0
	and r0, r1
	lsl r0, r0, #0x1F
	mov r1, #0xC0
	lsl r1, r1, #0xF
	ldrh r4, [r4]
	orr r1, r4
	orr r0, r1
	b _0801E7F4
_0801E478:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	add r1, r6, #0
	and r1, r0
	ldr r0, _0801E514 @ =0x00000D64
	mul r1, r0
	ldr r2, _0801E518 @ =0x000005F4
	add r0, r5, r2
	add r1, r1, r0
	mov r2, #2
	add r0, r2, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0801E4AE
	add r0, r2, #0
	and r0, r3
	mov r1, #0x7F
	cmp r0, #0
	beq _0801E4A2
	ldr r1, _0801E51C @ =0x0000807F
_0801E4A2:
	add r0, r1, #0
	mov r1, #0xA
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0801E4AE:
	ldr r6, _0801E520 @ =0x020192E0
	ldr r3, _0801E524 @ =0x00001B33
	add r7, r6, r3
	ldrb r3, [r7]
	lsl r0, r3, #0x1E
	mov r5, #1
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r1, _0801E528 @ =0x00001B34
	add r4, r6, r1
	ldrh r1, [r4]
	lsl r2, r1, #0x17
	lsr r2, r2, #0x18
	lsr r3, r3, #2
	add r1, r5, #0
	ldrb r4, [r4]
	and r1, r4
	lsl r1, r1, #6
	orr r1, r3
	add r2, r2, r1
	mov r1, #0x1F
	and r2, r1
	lsl r2, r2, #0x10
	mov r1, #0x80
	lsl r1, r1, #0xE
	orr r2, r1
	orr r0, r2
	ldr r2, _0801E52C @ =0x00001B28
	add r1, r6, r2
	ldrh r1, [r1]
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	ldr r3, _0801E530 @ =0x00001B12
	add r0, r6, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #1
	bhi _0801E502
	b _0801E90C
_0801E502:
	add r2, r6, #4
	ldrb r7, [r7]
	lsl r0, r7, #0x1E
	lsr r0, r0, #0x1F
	and r5, r0
	ldr r0, _0801E514 @ =0x00000D64
	add r1, r5, #0
	mul r1, r0
	b _0801E620
_0801E514: .4byte 0x00000D64
_0801E518: .4byte 0x000005F4
_0801E51C: .4byte 0x0000807F
_0801E520: .4byte 0x020192E0
_0801E524: .4byte 0x00001B33
_0801E528: .4byte 0x00001B34
_0801E52C: .4byte 0x00001B28
_0801E530: .4byte 0x00001B12
_0801E534:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	add r2, r6, #0
	and r2, r0
	ldrh r7, [r7]
	lsl r6, r7, #0x17
	lsr r0, r6, #0x18
	add r0, #5
	mov r1, #0x94
	mul r1, r0
	ldr r0, _0801E62C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r0, r5, #0
	add r0, #0x2C
	add r1, r1, r0
	mov r2, #2
	add r0, r2, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0801E57A
	add r0, r2, #0
	and r0, r3
	mov r2, #0x7F
	cmp r0, #0
	beq _0801E56C
	ldr r2, _0801E630 @ =0x0000807F
_0801E56C:
	lsr r1, r6, #0x18
	add r1, r1, r4
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0801E57A:
	ldr r5, _0801E634 @ =0x020192E0
	ldr r6, _0801E638 @ =0x00001B33
	add r7, r5, r6
	ldrb r0, [r7]
	lsl r3, r0, #0x1E
	mov r4, #1
	lsr r2, r3, #0x1F
	ldr r1, _0801E63C @ =0x00001B34
	add r6, r5, r1
	ldrh r1, [r6]
	lsl r0, r1, #0x17
	lsr r0, r0, #0x18
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0801E62C @ =0x00000D64
	mov r8, r1
	mov r1, r8
	mul r1, r2
	add r0, r0, r1
	add r1, r5, #0
	add r1, #0x2C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0xD
	cmp r0, #0
	bge _0801E5C6
	add r0, r2, #0
	ldr r1, _0801E640 @ =0x0862467A
	ldrh r1, [r1]
	bl ShowCardEffect
	ldrb r2, [r7]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #0xFA
	lsl r1, r1, #3
	bl LoseLifePoints
_0801E5C6:
	ldrb r3, [r7]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1F
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #0x1F
	ldrh r1, [r6]
	lsl r2, r1, #0x17
	lsr r2, r2, #0x18
	lsr r3, r3, #2
	add r1, r4, #0
	ldrb r6, [r6]
	and r1, r6
	lsl r1, r1, #6
	orr r1, r3
	add r2, r2, r1
	mov r1, #0x1F
	and r2, r1
	lsl r2, r2, #0x10
	mov r1, #0x80
	lsl r1, r1, #0xE
	orr r2, r1
	orr r0, r2
	ldr r2, _0801E644 @ =0x00001B28
	add r1, r5, r2
	ldrh r1, [r1]
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	ldr r3, _0801E648 @ =0x00001B12
	add r0, r5, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #1
	bhi _0801E612
	b _0801E90C
_0801E612:
	add r2, r5, #4
	ldrb r7, [r7]
	lsl r0, r7, #0x1E
	lsr r0, r0, #0x1F
	and r4, r0
	mov r1, r8
	mul r1, r4
_0801E620:
	add r1, r1, r2
	mov r0, #0x20
	ldrb r4, [r1, #9]
	orr r0, r4
	strb r0, [r1, #9]
	b _0801E90C
_0801E62C: .4byte 0x00000D64
_0801E630: .4byte 0x0000807F
_0801E634: .4byte 0x020192E0
_0801E638: .4byte 0x00001B33
_0801E63C: .4byte 0x00001B34
_0801E640: .4byte gUnk_0862467A
_0801E644: .4byte 0x00001B28
_0801E648: .4byte 0x00001B12
_0801E64C:
	ldr r6, _0801E678 @ =0x00001B28
	add r1, r5, r6
	ldr r0, _0801E67C @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0801E680 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xD0
	lsl r0, r0, #1
	cmp r1, r0
	bne _0801E668
	b _0801E778
_0801E668:
	cmp r1, r0
	bgt _0801E684
	cmp r1, #0x51
	beq _0801E698
	sub r0, #0x1A
	cmp r1, r0
	beq _0801E698
	b _0801E7B8
_0801E678: .4byte 0x00001B28
_0801E67C: .4byte 0x000007FF
_0801E680: .4byte gCardIdToNumber
_0801E684:
	ldr r0, _0801E694 @ =0x00000243
	cmp r1, r0
	beq _0801E778
	add r0, #0x98
	cmp r1, r0
	beq _0801E778
	b _0801E7B8
	.align 2, 0
_0801E694: .4byte 0x00000243
_0801E698:
	ldr r2, _0801E6AC @ =0x00001B30
	add r4, r5, r2
	ldrh r3, [r4]
	lsl r0, r3, #0x16
	lsr r0, r0, #0x18
	cmp r0, #0
	beq _0801E6B0
	cmp r0, #1
	beq _0801E6F8
	b _0801E754
_0801E6AC: .4byte 0x00001B30
_0801E6B0:
	ldr r6, _0801E6E8 @ =0x00001B33
	add r0, r5, r6
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, _0801E6EC @ =0x00001B34
	add r1, r5, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	bl TributeMonster
	cmp r0, #0
	beq _0801E6CE
	b _0801E8AA
_0801E6CE:
	ldr r3, _0801E6F0 @ =0x00001B2C
	add r0, r5, r3
	mov r1, #3
	neg r1, r1
	ldrb r6, [r0]
	and r1, r6
	strb r1, [r0]
	ldr r0, _0801E6F4 @ =0xFFFFFC03
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	b _0801E928
	.align 2, 0
_0801E6E8: .4byte 0x00001B33
_0801E6EC: .4byte 0x00001B34
_0801E6F0: .4byte 0x00001B2C
_0801E6F4: .4byte 0xFFFFFC03
_0801E6F8:
	ldr r2, _0801E73C @ =0x00001B33
	add r6, r5, r2
	ldrb r3, [r6]
	lsl r0, r3, #0x1E
	lsr r2, r0, #0x1F
	ldr r0, _0801E740 @ =0x00001B28
	add r1, r5, r0
	ldr r0, _0801E744 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0801E748 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0801E74C @ =0x00000187
	ldrh r0, [r0]
	cmp r0, #0x51
	bne _0801E71C
	ldr r1, _0801E750 @ =0x000002E5
_0801E71C:
	add r0, r2, #0
	mov r2, sp
	bl TakeDeckCardByNumber
	cmp r0, #0
	beq _0801E754
	ldrb r6, [r6]
	lsl r0, r6, #0x1E
	lsr r0, r0, #0x1F
	mov r1, sp
	mov r2, #1
	mov r3, #1
	bl QueueSpecialSummonChoosePosition
	b _0801E8AA
	.align 2, 0
_0801E73C: .4byte 0x00001B33
_0801E740: .4byte 0x00001B28
_0801E744: .4byte 0x000007FF
_0801E748: .4byte gCardIdToNumber
_0801E74C: .4byte 0x00000187
_0801E750: .4byte 0x000002E5
_0801E754:
	ldr r1, _0801E76C @ =0x020192E0
	ldr r3, _0801E770 @ =0x00001B2C
	add r2, r1, r3
	mov r0, #3
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	strb r0, [r2]
	ldr r6, _0801E774 @ =0x00001B30
	add r1, r1, r6
	b _0801E920
	.align 2, 0
_0801E76C: .4byte 0x020192E0
_0801E770: .4byte 0x00001B2C
_0801E774: .4byte 0x00001B30
_0801E778:
	ldr r5, _0801E7AC @ =0x020192E0
	ldr r3, _0801E7B0 @ =0x00001B33
	add r0, r5, r3
	ldrb r3, [r0]
	lsl r0, r3, #0x1E
	mov r2, #1
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r6, _0801E7B4 @ =0x00001B34
	add r4, r5, r6
	ldrh r6, [r4]
	lsl r1, r6, #0x17
	lsr r1, r1, #0x18
	lsr r3, r3, #2
	ldrb r4, [r4]
	and r2, r4
	lsl r2, r2, #6
	orr r2, r3
	add r1, r1, r2
	mov r2, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	mov r2, #0x88
	lsl r2, r2, #0x13
	b _0801E7E8
	.align 2, 0
_0801E7AC: .4byte 0x020192E0
_0801E7B0: .4byte 0x00001B33
_0801E7B4: .4byte 0x00001B34
_0801E7B8:
	ldr r5, _0801E7FC @ =0x020192E0
	ldr r2, _0801E800 @ =0x00001B33
	add r0, r5, r2
	ldrb r3, [r0]
	lsl r0, r3, #0x1E
	mov r2, #1
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r6, _0801E804 @ =0x00001B34
	add r4, r5, r6
	ldrh r6, [r4]
	lsl r1, r6, #0x17
	lsr r1, r1, #0x18
	lsr r3, r3, #2
	ldrb r4, [r4]
	and r2, r4
	lsl r2, r2, #6
	orr r2, r3
	add r1, r1, r2
	mov r2, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xF
_0801E7E8:
	orr r1, r2
	orr r0, r1
	ldr r1, _0801E808 @ =0x00001B28
	add r5, r5, r1
	ldrh r5, [r5]
	orr r0, r5
_0801E7F4:
	mov r1, #0
	bl Chain_AddPending
	b _0801E90C
_0801E7FC: .4byte 0x020192E0
_0801E800: .4byte 0x00001B33
_0801E804: .4byte 0x00001B34
_0801E808: .4byte 0x00001B28
_0801E80C:
	ldr r3, _0801E83C @ =0x02018450
	ldr r2, _0801E840 @ =0x020192E0
	ldr r4, _0801E844 @ =0x00001B34
	add r0, r2, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x18
	mov r1, #7
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _0801E848 @ =0xFFFFFE3F
	ldrh r6, [r3]
	and r0, r6
	orr r0, r1
	strh r0, [r3]
	ldr r0, _0801E84C @ =0x00001B16
	add r2, r2, r0
	ldr r0, _0801E850 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #4
	orr r0, r1
	strh r0, [r2]
	b _0801E90C
_0801E83C: .4byte 0x02018450
_0801E840: .4byte 0x020192E0
_0801E844: .4byte 0x00001B34
_0801E848: .4byte 0xFFFFFE3F
_0801E84C: .4byte 0x00001B16
_0801E850: .4byte 0xFFFFFE01
_0801E854:
	mov r2, #0xD9
	lsl r2, r2, #5
	add r1, r5, r2
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	ldr r3, _0801E874 @ =0x00001B2C
	add r1, r5, r3
	mov r0, #3
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r6, _0801E878 @ =0x00001B30
	add r1, r5, r6
	b _0801E920
_0801E874: .4byte 0x00001B2C
_0801E878: .4byte 0x00001B30
_0801E87C:
	ldr r3, _0801E890 @ =0x00001B30
	add r4, r5, r3
	ldrh r6, [r4]
	lsl r0, r6, #0x16
	lsr r0, r0, #0x18
	cmp r0, #0
	beq _0801E894
	cmp r0, #1
	beq _0801E8D4
	b _0801E928
_0801E890: .4byte 0x00001B30
_0801E894:
	ldr r0, _0801E8C4 @ =0x00000206
	ldr r1, _0801E8C8 @ =0x00000412
	ldr r3, _0801E8CC @ =0x08081CA4
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
_0801E8AA:
	ldrh r2, [r4]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _0801E8D0 @ =0xFFFFFC03
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	b _0801E928
	.align 2, 0
_0801E8C4: .4byte 0x00000206
_0801E8C8: .4byte 0x00000412
_0801E8CC: .4byte gStrDoYouSurrender
_0801E8D0: .4byte 0xFFFFFC03
_0801E8D4:
	ldr r0, _0801E900 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0801E8E8
	mov r0, #0x40
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0801E8E8:
	ldr r1, _0801E904 @ =0x00001B2C
	add r0, r5, r1
	mov r1, #3
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	strb r1, [r0]
	ldr r0, _0801E908 @ =0xFFFFFC03
	ldrh r3, [r4]
	and r0, r3
	strh r0, [r4]
	b _0801E928
_0801E900: .4byte 0x0201AE60
_0801E904: .4byte 0x00001B2C
_0801E908: .4byte 0xFFFFFC03
_0801E90C:
	ldr r1, _0801E934 @ =0x020192E0
	ldr r4, _0801E938 @ =0x00001B2C
	add r2, r1, r4
	mov r0, #3
	neg r0, r0
	ldrb r6, [r2]
	and r0, r6
	strb r0, [r2]
	ldr r0, _0801E93C @ =0x00001B30
	add r1, r1, r0
_0801E920:
	ldr r0, _0801E940 @ =0xFFFFFC03
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
_0801E928:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801E934: .4byte 0x020192E0
_0801E938: .4byte 0x00001B2C
_0801E93C: .4byte 0x00001B30
_0801E940: .4byte 0xFFFFFC03
	thumb_func_end CardMenu_Execute

