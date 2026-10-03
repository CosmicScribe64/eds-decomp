	thumb_func_start CalcBattle
CalcBattle: @ 0x0801D264
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r9, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	ldr r3, _0801D2B4 @ =0x02018450
	mov r2, #1
	mov r1, r9
	and r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r4, [r3]
	and r0, r4
	orr r0, r1
	strb r0, [r3]
	ldrb r0, [r3, #4]
	orr r2, r0
	strb r2, [r3, #4]
	mov r1, #0
	mov r8, r1
	add r3, #8
	mov sl, r3
	mov r6, sl
_0801D29C:
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	mov r0, sl
	add r0, #0x18
	add r2, r1, r0
	cmp r8, r9
	bne _0801D2B8
	sub r0, #0x20
	ldrh r0, [r0]
	lsl r0, r0, #0x17
	b _0801D2C0
_0801D2B4: .4byte 0x02018450
_0801D2B8:
	mov r0, sl
	sub r0, #8
	ldrb r0, [r0, #1]
	lsl r0, r0, #0x1C
_0801D2C0:
	lsr r7, r0, #0x1D
	mov r0, r8
	mov r3, #1
	and r0, r3
	ldr r1, _0801D3DC @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r4, _0801D3E0 @ =0x0201930C
	add r1, r5, r4
	mov r0, #0x94
	add r4, r7, #0
	mul r4, r0
	add r1, r1, r4
	add r0, r2, #0
	mov r2, #0x94
	bl MemCopy16
	mov r0, #8
	neg r0, r0
	add r1, r0, #0
	ldrb r2, [r6]
	and r1, r2
	orr r1, r7
	mov r3, #9
	neg r3, r3
	add r0, r3, #0
	and r1, r0
	strb r1, [r6]
	add r4, r4, r5
	ldr r0, _0801D3E0 @ =0x0201930C
	add r4, r4, r0
	ldrb r2, [r4, #6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r3, #1
	and r0, r3
	lsl r0, r0, #4
	sub r3, #0x12
	add r2, r3, #0
	and r1, r2
	orr r1, r0
	strb r1, [r6]
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r6, #2]
	mov r0, r8
	add r1, r7, #0
	bl GetZoneCardAtk
	strh r0, [r6, #4]
	mov r0, r8
	add r1, r7, #0
	bl GetZoneCardDef
	strh r0, [r6, #6]
	mov r4, #0
	strh r4, [r6, #0xA]
	add r6, #0xC
	mov r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #1
	ble _0801D29C
	ldr r7, _0801D3E4 @ =0x02018450
	cmp r8, r9
	beq _0801D366
	mov r0, #2
	ldrb r2, [r7]
	and r0, r2
	cmp r0, #0
	beq _0801D366
	mov r0, #1
	mov r3, r9
	sub r0, r0, r3
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r7
	mov r0, #0
	strh r0, [r1, #0xA]
	strh r0, [r1, #0xC]
	strh r0, [r1, #0xE]
_0801D366:
	mov r0, #0x20
	ldrb r4, [r7]
	and r0, r4
	mov r1, r9
	lsl r1, r1, #1
	mov r8, r1
	cmp r0, #0
	beq _0801D382
	mov r0, r8
	add r0, r9
	lsl r0, r0, #2
	add r0, r0, r7
	mov r1, #0
	strh r1, [r0, #0xC]
_0801D382:
	mov r0, r8
	add r0, r9
	lsl r0, r0, #2
	add r4, r0, r7
	ldrh r0, [r4, #0xC]
	mov r6, #0
	strh r0, [r4, #0x10]
	mov r0, #1
	mov r2, r9
	sub r3, r0, r2
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #2
	add r5, r0, r7
	ldrh r0, [r5, #0xC]
	strh r0, [r5, #0x10]
	mov r0, #0x11
	neg r0, r0
	ldrb r1, [r4, #8]
	and r0, r1
	strb r0, [r4, #8]
	mov r0, #2
	ldrb r2, [r7]
	and r0, r2
	cmp r0, #0
	beq _0801D3EC
	ldrh r0, [r4, #0xC]
	strh r0, [r5, #0x12]
	strh r6, [r5, #0x10]
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801D3C4
	b _0801D80C
_0801D3C4:
	ldr r1, _0801D3E8 @ =0x0000058F
	mov r2, #1
	neg r2, r2
	add r0, r3, #0
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	bgt _0801D3D6
	b _0801D80C
_0801D3D6:
	strh r6, [r5, #0x12]
	b _0801D80C
	.align 2, 0
_0801D3DC: .4byte 0x00000D64
_0801D3E0: .4byte 0x0201930C
_0801D3E4: .4byte 0x02018450
_0801D3E8: .4byte 0x0000058F
_0801D3EC:
	ldr r0, _0801D408 @ =0x000007FF
	ldrh r1, [r4, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0801D40C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0801D410 @ =0x000001DD
	cmp r1, r0
	beq _0801D418
	ldr r0, _0801D414 @ =0x000004E5
	cmp r1, r0
	beq _0801D434
	b _0801D456
_0801D408: .4byte 0x000007FF
_0801D40C: .4byte gCardIdToNumber
_0801D410: .4byte 0x000001DD
_0801D414: .4byte 0x000004E5
_0801D418:
	ldrb r7, [r7, #1]
	lsl r1, r7, #0x1C
	lsr r1, r1, #0x1D
	add r0, r3, #0
	bl GetZoneCardAttribute
	cmp r0, #6
	bne _0801D456
	ldrh r3, [r4, #0xC]
	mov r1, #0xFA
	lsl r1, r1, #2
	add r0, r3, r1
	strh r0, [r4, #0xC]
	b _0801D456
_0801D434:
	ldrb r7, [r7, #1]
	lsl r1, r7, #0x1C
	lsr r1, r1, #0x1D
	add r0, r3, #0
	bl GetZoneCardType
	cmp r0, #0xF
	bne _0801D456
	mov r2, #0xFA
	lsl r2, r2, #3
	add r0, r2, #0
	ldrh r3, [r4, #0xC]
	add r1, r0, r3
	strh r1, [r4, #0xC]
	ldrh r1, [r4, #0xE]
	add r0, r0, r1
	strh r0, [r4, #0xE]
_0801D456:
	ldr r2, _0801D484 @ =0x02018450
	mov r1, #1
	mov r3, r9
	sub r1, r1, r3
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r4, r0, r2
	ldr r0, _0801D488 @ =0x000007FF
	ldrh r1, [r4, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _0801D48C @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0801D490 @ =0x0000011F
	cmp r1, r0
	beq _0801D498
	ldr r0, _0801D494 @ =0x000004E5
	cmp r1, r0
	beq _0801D4AE
	b _0801D4D0
	.align 2, 0
_0801D484: .4byte 0x02018450
_0801D488: .4byte 0x000007FF
_0801D48C: .4byte gCardIdToNumber
_0801D490: .4byte 0x0000011F
_0801D494: .4byte 0x000004E5
_0801D498:
	ldrh r2, [r2]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	mov r0, r9
	bl GetZoneCardAttribute
	cmp r0, #1
	bne _0801D4D0
	ldrh r1, [r4, #0xE]
	lsr r0, r1, #1
	b _0801D4CE
_0801D4AE:
	ldrh r2, [r2]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	mov r0, r9
	bl GetZoneCardType
	cmp r0, #0xF
	bne _0801D4D0
	mov r2, #0xFA
	lsl r2, r2, #3
	add r0, r2, #0
	ldrh r3, [r4, #0xC]
	add r1, r0, r3
	strh r1, [r4, #0xC]
	ldrh r1, [r4, #0xE]
	add r0, r0, r1
_0801D4CE:
	strh r0, [r4, #0xE]
_0801D4D0:
	ldr r2, [sp, #0]
	cmp r2, #0
	beq _0801D4E4
	ldr r0, _0801D558 @ =0x02018450
	mov r1, r8
	add r1, r9
	lsl r1, r1, #2
	add r1, r1, r0
	mov r0, #0
	strh r0, [r1, #0xC]
_0801D4E4:
	ldr r7, _0801D558 @ =0x02018450
	mov r0, r8
	add r0, r9
	lsl r0, r0, #2
	add r6, r0, r7
	ldrh r0, [r6, #0xC]
	strh r0, [r6, #0x10]
	mov r1, #1
	mov r3, r9
	sub r1, r1, r3
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r5, r0, r7
	ldrh r0, [r5, #0xC]
	strh r0, [r5, #0x10]
	ldrb r4, [r5, #8]
	lsl r0, r4, #0x1B
	cmp r0, #0
	bge _0801D510
	ldrh r0, [r5, #0xE]
	strh r0, [r5, #0x10]
_0801D510:
	ldrh r0, [r7]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	ldr r2, _0801D55C @ =0x00000291
	mov r0, r9
	bl CountActiveZoneLinksFromCard
	add r4, r0, #0
	ldrh r0, [r5, #0xC]
	bl HalveRoundUp
	mul r0, r4
	ldrh r2, [r6, #0x10]
	add r1, r2, r0
	strh r1, [r6, #0x10]
	ldrh r3, [r5, #0x10]
	lsl r0, r1, #0x10
	lsr r2, r0, #0x10
	add r0, r3, #0
	cmp r2, r0
	bne _0801D560
	ldrb r3, [r5, #8]
	lsl r0, r3, #0x1B
	cmp r0, #0
	blt _0801D5EC
	cmp r2, #0
	beq _0801D5EC
	mov r0, #8
	ldrb r1, [r6, #8]
	orr r1, r0
	strb r1, [r6, #8]
	ldrb r4, [r5, #8]
	orr r0, r4
	strb r0, [r5, #8]
	b _0801D5EC
	.align 2, 0
_0801D558: .4byte 0x02018450
_0801D55C: .4byte 0x00000291
_0801D560:
	cmp r2, r0
	bls _0801D5D8
	mov r4, #0
	ldrb r1, [r5, #8]
	lsl r0, r1, #0x1B
	cmp r0, #0
	blt _0801D570
	mov r4, #1
_0801D570:
	ldrh r2, [r7]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	ldr r2, _0801D5C4 @ =0x00000521
	mov r0, r9
	bl CountActiveZoneLinksFromCard
	cmp r0, #0
	beq _0801D584
	mov r4, #1
_0801D584:
	ldr r0, _0801D5C8 @ =0x000007FF
	ldrh r3, [r6, #0xA]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0801D5CC @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0801D5D0 @ =0x0000053D
	ldrh r0, [r0]
	cmp r0, r1
	bne _0801D59A
	mov r4, #1
_0801D59A:
	ldrh r7, [r7]
	lsl r1, r7, #0x17
	lsr r1, r1, #0x1D
	ldr r2, _0801D5D4 @ =0x00000604
	mov r0, r9
	bl CountActiveZoneLinksFromCard
	cmp r0, #0
	beq _0801D5AE
	mov r4, #1
_0801D5AE:
	cmp r4, #0
	beq _0801D5BA
	ldrh r6, [r6, #0x10]
	ldrh r2, [r5, #0x10]
	sub r0, r6, r2
	strh r0, [r5, #0x12]
_0801D5BA:
	mov r0, #8
	ldrb r3, [r5, #8]
	orr r0, r3
	strb r0, [r5, #8]
	b _0801D5EC
_0801D5C4: .4byte 0x00000521
_0801D5C8: .4byte 0x000007FF
_0801D5CC: .4byte gCardIdToNumber
_0801D5D0: .4byte 0x0000053D
_0801D5D4: .4byte 0x00000604
_0801D5D8:
	sub r0, r3, r1
	strh r0, [r6, #0x12]
	ldrb r5, [r5, #8]
	lsl r0, r5, #0x1B
	cmp r0, #0
	blt _0801D5EC
	mov r0, #8
	ldrb r4, [r6, #8]
	orr r0, r4
	strb r0, [r6, #8]
_0801D5EC:
	ldr r5, _0801D618 @ =0x02018450
	mov r0, r8
	add r0, r9
	lsl r0, r0, #2
	add r2, r0, r5
	ldr r0, _0801D61C @ =0x000007FF
	ldrh r1, [r2, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _0801D620 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0801D624 @ =0x000004CF
	cmp r1, r0
	beq _0801D638
	cmp r1, r0
	bgt _0801D62C
	ldr r0, _0801D628 @ =0x00000199
	cmp r1, r0
	beq _0801D68C
	b _0801D6B2
	.align 2, 0
_0801D618: .4byte 0x02018450
_0801D61C: .4byte 0x000007FF
_0801D620: .4byte gCardIdToNumber
_0801D624: .4byte 0x000004CF
_0801D628: .4byte 0x00000199
_0801D62C:
	ldr r0, _0801D634 @ =0x000004E3
	cmp r1, r0
	beq _0801D666
	b _0801D6B2
_0801D634: .4byte 0x000004E3
_0801D638:
	mov r0, #9
	neg r0, r0
	ldrb r4, [r2, #8]
	and r0, r4
	strb r0, [r2, #8]
	mov r0, #0
	strh r0, [r2, #0x12]
	ldrh r1, [r2, #0xA]
	mov r2, #1
	mov r0, r9
	sub r2, r2, r0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	ldrb r5, [r5, #1]
	lsl r0, r5, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	mov r0, r9
	mov r3, #3
	bl QueueAddZoneLink
	b _0801D6B2
_0801D666:
	mov r0, #1
	mov r1, r9
	sub r0, r0, r1
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r5
	ldr r0, _0801D688 @ =0x0000076B
	ldrh r1, [r1, #0xC]
	cmp r1, r0
	bls _0801D6B2
	mov r0, #9
	neg r0, r0
	ldrb r3, [r2, #8]
	and r0, r3
	strb r0, [r2, #8]
	b _0801D6B2
_0801D688: .4byte 0x0000076B
_0801D68C:
	mov r0, #1
	mov r1, r9
	sub r4, r0, r1
	ldrb r2, [r5, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	add r0, r4, #0
	bl GetZoneCardAttribute
	cmp r0, #2
	bne _0801D6B2
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r5
	mov r1, #8
	ldrb r3, [r0, #8]
	orr r1, r3
	strb r1, [r0, #8]
_0801D6B2:
	ldr r5, _0801D81C @ =0x02018450
	ldrh r4, [r5]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x1D
	ldr r2, _0801D820 @ =0x0000049E
	mov r0, r9
	bl CountActiveZoneLinksFromCard
	cmp r0, #0
	beq _0801D6EC
	mov r0, #1
	mov r1, r9
	sub r4, r0, r1
	ldrb r2, [r5, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	add r0, r4, #0
	bl GetZoneCardType
	cmp r0, #1
	bne _0801D6EC
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r5
	mov r1, #0x40
	ldrb r3, [r0, #8]
	orr r1, r3
	strb r1, [r0, #8]
_0801D6EC:
	mov r0, #1
	mov r4, r9
	sub r0, r0, r4
	ldr r4, _0801D81C @ =0x02018450
	ldrb r2, [r4, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	ldr r2, _0801D820 @ =0x0000049E
	bl CountActiveZoneLinksFromCard
	cmp r0, #0
	beq _0801D724
	ldrh r3, [r4]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x1D
	mov r0, r9
	bl GetZoneCardType
	cmp r0, #1
	bne _0801D724
	mov r1, r8
	add r1, r9
	lsl r1, r1, #2
	add r1, r1, r4
	mov r0, #0x40
	ldrb r4, [r1, #8]
	orr r0, r4
	strb r0, [r1, #8]
_0801D724:
	ldr r2, _0801D81C @ =0x02018450
	mov r1, #1
	mov r0, r9
	sub r1, r1, r0
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r3, r0, r2
	ldr r0, _0801D824 @ =0x000007FF
	ldrh r1, [r3, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r4, _0801D828 @ =0x08622AB4
	add r0, r0, r4
	ldr r1, _0801D82C @ =0x000004E3
	add r7, r2, #0
	ldrh r0, [r0]
	cmp r0, r1
	bne _0801D764
	mov r0, r8
	add r0, r9
	lsl r0, r0, #2
	add r0, r0, r7
	ldr r1, _0801D830 @ =0x0000076B
	ldrh r0, [r0, #0xC]
	cmp r0, r1
	bls _0801D764
	mov r0, #9
	neg r0, r0
	ldrb r1, [r3, #8]
	and r0, r1
	strb r0, [r3, #8]
_0801D764:
	ldr r2, _0801D834 @ =0x020192E4
	mov r1, #1
	mov r4, r9
	sub r3, r1, r4
	add r0, r3, #0
	and r0, r1
	ldr r1, _0801D838 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1E
	cmp r0, #0
	bge _0801D794
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r7
	mov r1, #9
	neg r1, r1
	ldrb r2, [r0, #8]
	and r1, r2
	strb r1, [r0, #8]
	mov r1, #0
	strh r1, [r0, #0x12]
_0801D794:
	ldr r0, _0801D81C @ =0x02018450
	lsl r1, r3, #1
	add r1, r1, r3
	lsl r1, r1, #2
	add r4, r1, r0
	ldrh r0, [r4, #0x12]
	cmp r0, #0
	beq _0801D7B8
	ldr r1, _0801D83C @ =0x0000058F
	mov r2, #1
	neg r2, r2
	add r0, r3, #0
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	ble _0801D7B8
	mov r0, #0
	strh r0, [r4, #0x12]
_0801D7B8:
	ldr r0, _0801D81C @ =0x02018450
	mov r1, r8
	add r1, r9
	lsl r1, r1, #2
	add r4, r1, r0
	ldrh r0, [r4, #0x12]
	cmp r0, #0
	beq _0801D7DC
	ldr r1, _0801D83C @ =0x0000058F
	mov r2, #1
	neg r2, r2
	mov r0, r9
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	ble _0801D7DC
	mov r0, #0
	strh r0, [r4, #0x12]
_0801D7DC:
	ldr r7, _0801D81C @ =0x02018450
	mov r4, #1
	mov r5, #0x21
	neg r5, r5
	add r3, r7, #0
	add r3, #8
	mov r0, #1
	mov r8, r0
_0801D7EC:
	ldrb r2, [r3]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1F
	and r1, r4
	lsl r1, r1, #5
	add r0, r5, #0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	add r3, #0xC
	mov r1, #1
	neg r1, r1
	add r8, r1
	mov r2, r8
	cmp r2, #0
	bge _0801D7EC
_0801D80C:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801D81C: .4byte 0x02018450
_0801D820: .4byte 0x0000049E
_0801D824: .4byte 0x000007FF
_0801D828: .4byte gCardIdToNumber
_0801D82C: .4byte 0x000004E3
_0801D830: .4byte 0x0000076B
_0801D834: .4byte 0x020192E4
_0801D838: .4byte 0x00000D64
_0801D83C: .4byte 0x0000058F
	thumb_func_end CalcBattle

