	thumb_func_start DuelPhase_Standby
DuelPhase_Standby: @ 0x0804FC4C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10C
	ldr r1, _0804FC84 @ =0x020192E0
	ldr r2, _0804FC88 @ =0x00001B12
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r7, r0, #0x1F
	add r2, r1, #4
	ldr r0, _0804FC8C @ =0x00000D64
	mul r0, r7
	add r3, r0, r2
	ldrb r2, [r3, #9]
	lsl r0, r2, #0x1E
	mov r9, r1
	cmp r0, #0
	bge _0804FC90
	mov r0, #3
	neg r0, r0
	and r0, r2
	strb r0, [r3, #9]
	bl _08050A5C @ far jump
	.align 2, 0
_0804FC84: .4byte 0x020192E0
_0804FC88: .4byte 0x00001B12
_0804FC8C: .4byte 0x00000D64
_0804FC90:
	mov r0, #0xD9
	lsl r0, r0, #5
	add r0, r9
	ldrb r0, [r0]
	cmp r0, #0x7A
	bls _0804FCA0
	bl _08050A5C @ far jump
_0804FCA0:
	lsl r0, r0, #2
	ldr r1, _0804FCAC @ =0x0804FCB0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804FCAC: .4byte 0x0804FCB0
_0804FCB0:
	.4byte _0804FE9C
	.4byte _0804FEC4
	.4byte _0804FEF8
	.4byte _0804FF30
	.4byte _0804FFC8
	.4byte _0805006C
	.4byte _080500E4
	.4byte _0805015C
	.4byte _080501BC
	.4byte _08050224
	.4byte _08050304
	.4byte _08050364
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _0805038E
	.4byte _080504C0
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _080504CA
	.4byte _080504DE
	.4byte _08050630
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _080506A4
	.4byte _08050860
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050A5C
	.4byte _08050924
	.4byte _08050988
	.4byte _080509E8
_0804FE9C:
	mov r0, #0x51
	cmp r7, #0
	beq _0804FEA4
	ldr r0, _0804FEBC @ =0x00008051
_0804FEA4:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _0804FEC0 @ =0x020192E0
	mov r3, #0xD9
	lsl r3, r3, #5
	add r0, r0, r3
	mov r1, #0x14
	bl _080509A8 @ far jump
_0804FEBC: .4byte 0x00008051
_0804FEC0: .4byte 0x020192E0
_0804FEC4:
	add r0, r7, #0
	bl ApplyStandbyPhaseEffects
	ldr r0, _0804FEE8 @ =0x020192E0
	ldr r4, _0804FEEC @ =0x00001B22
	add r1, r0, r4
	mov r2, #0
	strb r2, [r1]
	ldr r6, _0804FEF0 @ =0x00001B23
	add r1, r0, r6
	strb r2, [r1]
	ldr r3, _0804FEF4 @ =0x00001B21
	add r1, r0, r3
	strb r2, [r1]
	sub r4, #2
	add r0, r0, r4
	bl _080509A4 @ far jump
_0804FEE8: .4byte 0x020192E0
_0804FEEC: .4byte 0x00001B22
_0804FEF0: .4byte 0x00001B23
_0804FEF4: .4byte 0x00001B21
_0804FEF8:
	add r0, r7, #0
	bl SinisterSerpentStandbyStep
	cmp r0, #0
	beq _0804FF8E
	ldr r0, _0804FF20 @ =0x020192E0
	ldr r6, _0804FF24 @ =0x00001B22
	add r1, r0, r6
	mov r2, #0
	strb r2, [r1]
	ldr r3, _0804FF28 @ =0x00001B23
	add r1, r0, r3
	strb r2, [r1]
	ldr r4, _0804FF2C @ =0x00001B21
	add r1, r0, r4
	strb r2, [r1]
	sub r6, #2
	add r0, r0, r6
	bl _080509A4 @ far jump
_0804FF20: .4byte 0x020192E0
_0804FF24: .4byte 0x00001B22
_0804FF28: .4byte 0x00001B23
_0804FF2C: .4byte 0x00001B21
_0804FF30:
	ldr r1, _0804FF94 @ =0x020192E4
	ldr r0, _0804FF98 @ =0x00000D64
	mul r0, r7
	add r0, r0, r1
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1D
	cmp r0, #0
	beq _0804FFA8
	mov r0, #0x4B
	cmp r7, #0
	beq _0804FF4A
	ldr r0, _0804FF9C @ =0x0000804B
_0804FF4A:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r7, #0
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _0804FFA8
	ldr r4, _0804FFA0 @ =0x0000057D
	add r5, sp, #0x104
	add r0, r7, #0
	add r1, r4, #0
	add r2, r5, #0
	bl RemoveGraveyardCardByNumber
	cmp r0, #0
	beq _0804FFA8
	lsl r0, r4, #1
	ldr r1, _0804FFA4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r7, #0
	bl ShowCardEffect
	mov r0, #0x20
	str r0, [sp, #0]
	add r0, r7, #0
	add r1, r5, #0
	mov r2, #1
	mov r3, #1
	bl QueueSpecialSummon
_0804FF8E:
	mov r0, #0
	bl _08050A5E @ far jump
_0804FF94: .4byte 0x020192E4
_0804FF98: .4byte 0x00000D64
_0804FF9C: .4byte 0x0000804B
_0804FFA0: .4byte 0x0000057D
_0804FFA4: .4byte gCardNumberToId
_0804FFA8:
	ldr r0, _0804FFC0 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r1, r0, r2
	ldrb r2, [r1]
	add r2, #1
	mov r3, #0
	strb r2, [r1]
	ldr r4, _0804FFC4 @ =0x00001B22
	add r0, r0, r4
	strb r3, [r0]
	b _0804FF8E
_0804FFC0: .4byte 0x020192E0
_0804FFC4: .4byte 0x00001B22
_0804FFC8:
	ldr r1, _08050018 @ =0x00001B22
	add r1, r9
	ldr r2, _0805001C @ =0x00000D64
	add r0, r7, #0
	mul r0, r2
	add r0, r9
	ldrb r6, [r1]
	ldrb r3, [r0, #0xA]
	cmp r6, r3
	bcs _0805003E
	add r4, r1, #0
	ldr r6, _08050020 @ =0x00000CC8
	add r6, r9
	add r5, r0, #0
	add r3, r7, #0
	mul r3, r2
_0804FFE8:
	ldrb r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r3
	add r0, r0, r6
	ldr r1, _08050024 @ =0x00000402
	ldrh r0, [r0]
	cmp r0, r1
	bne _08050030
	ldr r0, _08050028 @ =0x0862457C
	ldrh r1, [r0]
	add r0, r7, #0
	bl ShowCardEffect
	mov r0, #0xCF
	cmp r7, #0
	beq _0805000A
	ldr r0, _0805002C @ =0x000080CF
_0805000A:
	ldrb r1, [r4]
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _0804FF8E
	.align 2, 0
_08050018: .4byte 0x00001B22
_0805001C: .4byte 0x00000D64
_08050020: .4byte 0x00000CC8
_08050024: .4byte 0x00000402
_08050028: .4byte gCardNumberToId_LightforceSword
_0805002C: .4byte 0x000080CF
_08050030:
	add r0, r2, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrb r1, [r5, #0xA]
	cmp r0, r1
	bcc _0804FFE8
_0805003E:
	ldr r0, _08050060 @ =0x00001B22
	add r0, r9
	mov r1, #0
	strb r1, [r0]
	ldr r0, _08050064 @ =0x00001B23
	add r0, r9
	strb r1, [r0]
	ldr r0, _08050068 @ =0x00001B21
	add r0, r9
	strb r1, [r0]
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	ldrb r0, [r1]
	add r0, #1
	bl _08050A52 @ far jump
_08050060: .4byte 0x00001B22
_08050064: .4byte 0x00001B23
_08050068: .4byte 0x00001B21
_0805006C:
	mov r5, #1
	sub r4, r5, r7
	ldr r2, _080500BC @ =0x00000489
	mov r8, r2
	add r0, r4, #0
	mov r1, r8
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _080500D0
	ldr r6, _080500C0 @ =0x020192E4
	add r0, r4, #0
	and r0, r5
	ldr r2, _080500C4 @ =0x00000D64
	mul r0, r2
	add r0, r0, r6
	ldr r1, _080500C8 @ =0x000001F3
	ldrh r0, [r0]
	cmp r0, r1
	bls _080500D0
	and r7, r5
	add r0, r7, #0
	mul r0, r2
	add r0, r0, r6
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _080500D0
	add r0, r4, #0
	mov r1, #0xF
	mov r2, r8
	mov r3, #0
	bl DuelPrompt_Post
	ldr r3, _080500CC @ =0x00001B1C
	add r1, r6, r3
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0804FF8E
	.align 2, 0
_080500BC: .4byte 0x00000489
_080500C0: .4byte 0x020192E4
_080500C4: .4byte 0x00000D64
_080500C8: .4byte 0x000001F3
_080500CC: .4byte 0x00001B1C
_080500D0:
	ldr r0, _080500E0 @ =0x020192E0
	mov r4, #0xD9
	lsl r4, r4, #5
	add r0, r0, r4
	mov r1, #7
	bl _080509A8 @ far jump
	.align 2, 0
_080500E0: .4byte 0x020192E0
_080500E4:
	ldr r0, _08050140 @ =0x00001B64
	add r0, r9
	ldrh r0, [r0]
	cmp r0, #0
	beq _08050150
	mov r0, #0x43
	cmp r7, #1
	beq _080500F6
	ldr r0, _08050144 @ =0x00008043
_080500F6:
	mov r1, #0xFA
	lsl r1, r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r6, #1
	sub r4, r6, r7
	ldr r5, _08050148 @ =0x00000489
	add r0, r4, #0
	add r1, r5, #0
	bl FindFaceUpCardOnField2
	lsl r5, r5, #1
	ldr r1, _0805014C @ =0x08623DF4
	add r5, r5, r1
	and r4, r6
	lsl r4, r4, #0x1F
	mov r1, #0x1F
	and r1, r0
	lsl r1, r1, #0x10
	mov r0, #0xC4
	lsl r0, r0, #0x13
	orr r1, r0
	orr r4, r1
	ldrh r5, [r5]
	orr r4, r5
	add r0, r4, #0
	mov r1, #0
	bl Chain_AddPending
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	mov r0, #5
	bl _08050A52 @ far jump
_08050140: .4byte 0x00001B64
_08050144: .4byte 0x00008043
_08050148: .4byte 0x00000489
_0805014C: .4byte gCardNumberToId
_08050150:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	mov r0, #7
	bl _08050A52 @ far jump
_0805015C:
	mov r0, #1
	sub r4, r0, r7
	ldr r5, _08050194 @ =0x000005ED
	add r0, r4, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0805019C
	add r0, r7, #0
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #0
	ble _0805019C
	add r0, r4, #0
	mov r1, #0xF
	add r2, r5, #0
	mov r3, #0
	bl DuelPrompt_Post
	ldr r0, _08050198 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	bl _080509A4 @ far jump
_08050194: .4byte 0x000005ED
_08050198: .4byte 0x020192E0
_0805019C:
	ldr r1, _080501B4 @ =0x020192E0
	ldr r3, _080501B8 @ =0x00001B21
	add r2, r1, r3
	mov r0, #5
	strb r0, [r2]
	mov r4, #0xD9
	lsl r4, r4, #5
	add r1, r1, r4
	mov r0, #9
	strb r0, [r1]
	b _0804FF8E
	.align 2, 0
_080501B4: .4byte 0x020192E0
_080501B8: .4byte 0x00001B21
_080501BC:
	ldr r6, _08050210 @ =0x020192E0
	mov r8, r6
	ldr r0, _08050214 @ =0x00001B64
	add r0, r8
	ldrh r0, [r0]
	cmp r0, #0
	beq _080501FC
	mov r6, #1
	sub r4, r6, r7
	ldr r5, _08050218 @ =0x000005ED
	add r0, r4, #0
	add r1, r5, #0
	bl FindFaceUpCardOnField2
	lsl r5, r5, #1
	ldr r1, _0805021C @ =0x08623DF4
	add r5, r5, r1
	and r4, r6
	lsl r4, r4, #0x1F
	mov r1, #0x1F
	and r1, r0
	lsl r1, r1, #0x10
	mov r0, #0xC8
	lsl r0, r0, #0x13
	orr r1, r0
	orr r4, r1
	ldrh r5, [r5]
	orr r4, r5
	add r0, r4, #0
	mov r1, #0
	bl Chain_AddPending
_080501FC:
	ldr r1, _08050220 @ =0x00001B21
	add r1, r8
	mov r0, #5
	strb r0, [r1]
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r8
	mov r0, #9
	bl _08050A52 @ far jump
_08050210: .4byte 0x020192E0
_08050214: .4byte 0x00001B64
_08050218: .4byte 0x000005ED
_0805021C: .4byte gCardNumberToId
_08050220: .4byte 0x00001B21
_08050224:
	ldr r0, _080502CC @ =0x00001B21
	add r0, r9
	ldrb r2, [r0]
	cmp r2, #9
	bhi _080502F2
	add r5, r0, #0
	mov r3, r9
	add r3, #0x2C
	mov r4, #0x94
	ldr r0, _080502D0 @ =0x00000D64
	add r6, r7, #0
	mul r6, r0
	mov r8, r6
_0805023E:
	ldrb r1, [r5]
	add r0, r1, #0
	mul r0, r4
	add r0, r8
	add r1, r0, r3
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _080502E4
	mov r0, #2
	ldrb r6, [r1, #6]
	and r0, r6
	cmp r0, #0
	beq _080502E4
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080502E4
	ldr r0, _080502D4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _080502D8 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _080502DC @ =0x00000592
	ldrh r0, [r0]
	cmp r0, r1
	bne _080502E4
	add r0, r7, #0
	str r3, [sp, #0x108]
	bl CountFreeMonsterZones
	ldr r3, [sp, #0x108]
	cmp r0, #0
	ble _080502E4
	ldrb r2, [r5]
	add r0, r2, #0
	mul r0, r4
	add r0, r8
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r7, #0
	bl ShowCardEffect
	mov r4, #0xA3
	cmp r7, #0
	beq _080502A6
	ldr r4, _080502E0 @ =0x000080A3
_080502A6:
	add r0, r7, #0
	bl FindFreeMonsterZone
	add r1, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrb r3, [r5]
	lsl r0, r3, #8
	orr r1, r0
	add r0, r4, #0
	mov r2, #3
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0804FF8E
	.align 2, 0
_080502CC: .4byte 0x00001B21
_080502D0: .4byte 0x00000D64
_080502D4: .4byte 0x000007FF
_080502D8: .4byte gCardIdToNumber
_080502DC: .4byte 0x00000592
_080502E0: .4byte 0x000080A3
_080502E4:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bls _0805023E
_080502F2:
	bl DuelCursor_Refresh
	ldr r0, _08050300 @ =0x020192E0
	mov r4, #0xD9
	lsl r4, r4, #5
	add r0, r0, r4
	b _080509A4
_08050300: .4byte 0x020192E0
_08050304:
	add r0, r7, #0
	bl HasActivatableStandbyCard
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08050324
	bl DuelCursor_Refresh
	ldr r0, _08050320 @ =0x020192E0
	mov r6, #0xD9
	lsl r6, r6, #5
	add r0, r0, r6
	mov r1, #0x64
	b _080509A8
_08050320: .4byte 0x020192E0
_08050324:
	bl DuelScreen_HandleInput
	cmp r0, #0
	beq _0805032E
	b _0804FF8E
_0805032E:
	ldr r1, _08050354 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0805033C
	b _0804FF8E
_0805033C:
	ldr r0, _08050358 @ =0x00000206
	ldr r1, _0805035C @ =0x00000713
	ldr r3, _08050360 @ =0x08085C9C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	b _0805099C
_08050354: .4byte 0x03000040
_08050358: .4byte 0x00000206
_0805035C: .4byte 0x00000713
_08050360: .4byte gStrCompleteStandbyPhase
_08050364:
	ldr r0, _08050374 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08050378
	cmp r0, #1
	beq _08050382
	b _0804FF8E
	.align 2, 0
_08050374: .4byte 0x0201AE60
_08050378:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	mov r0, #0xA
	b _0805038A
_08050382:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	mov r0, #0x64
_0805038A:
	strb r0, [r1]
	b _0804FF8E
_0805038E:
	mov r1, #0xC0
	lsl r1, r1, #3
	add r0, r7, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	cmp r4, #0
	bgt _080503A0
	b _080504AC
_080503A0:
	bl Random
	mov r1, #6
	bl __modsi3
	add r0, #1
	mov r8, r0
	ldr r2, _0805043C @ =0x086249F4
	ldrh r1, [r2]
	add r0, r7, #0
	bl ShowCardEffect
	mov r0, #0xE4
	cmp r7, #0
	beq _080503C0
	ldr r0, _08050440 @ =0x000080E4
_080503C0:
	mov r3, r8
	lsl r1, r3, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x12
	cmp r7, #0
	beq _080503D6
	ldr r0, _08050444 @ =0x00008012
_080503D6:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r6, #0
	sub r4, #1
	mov sl, r4
	ldr r4, _08050448 @ =0x000007FF
	add r3, r4, #0
_080503EA:
	mov r4, #0
	add r0, r6, #1
	mov r9, r0
	add r5, r6, #0
	mov r1, #1
	and r5, r1
_080503F6:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0805044C @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _08050450 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08050498
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08050498
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08050454 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r1, r0
	lsr r1, r1, #0x14
	cmp r1, #0x15
	blt _08050460
	cmp r1, #0x17
	ble _08050458
	cmp r1, #0x18
	beq _0805045C
	b _08050460
_0805043C: .4byte gCardNumberToId_1536
_08050440: .4byte 0x000080E4
_08050444: .4byte 0x00008012
_08050448: .4byte 0x000007FF
_0805044C: .4byte 0x00000D64
_08050450: .4byte 0x0201930C
_08050454: .4byte gCardStats
_08050458:
	mov r1, #0
	b _08050472
_0805045C:
	mov r1, #0xA
	b _08050472
_08050460:
	and r2, r3
	lsl r0, r2, #2
	ldr r2, _080504B8 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	mov r0, #0xF0
	lsl r0, r0, #0x15
	and r1, r0
	lsr r1, r1, #0x19
_08050472:
	mov r0, #0
	cmp r1, r8
	bne _0805047A
	mov r0, #1
_0805047A:
	cmp r1, #5
	ble _08050486
	mov r1, r8
	cmp r1, #6
	bne _08050486
	mov r0, #1
_08050486:
	cmp r0, #0
	beq _08050498
	add r0, r6, #0
	add r1, r4, #0
	mov r2, #1
	str r3, [sp, #0x108]
	bl DestroyFieldCard
	ldr r3, [sp, #0x108]
_08050498:
	add r4, #1
	cmp r4, #4
	ble _080503F6
	mov r6, r9
	cmp r6, #1
	ble _080503EA
	mov r4, sl
	cmp r4, #0
	beq _080504AC
	b _080503A0
_080504AC:
	ldr r0, _080504BC @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	mov r1, #1
	b _080509A8
_080504B8: .4byte gCardStats
_080504BC: .4byte 0x020192E0
_080504C0:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	mov r0, #0x14
	b _08050A52
_080504CA:
	ldr r1, _0805054C @ =0x00001B21
	add r1, r9
	mov r0, #0
	strb r0, [r1]
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_080504DE:
	ldr r0, _0805054C @ =0x00001B21
	add r0, r9
	ldrb r3, [r0]
	cmp r3, #9
	bls _080504EA
	b _08050620
_080504EA:
	ldr r4, _08050550 @ =0x020192E0
	mov r9, r4
	mov r8, r0
	ldr r0, _08050554 @ =0x00000D64
	add r6, r7, #0
	mul r6, r0
	mov sl, r6
_080504F8:
	mov r0, r8
	ldrb r2, [r0]
	mov r0, #0x94
	mul r0, r2
	add r0, sl
	ldr r1, _08050550 @ =0x020192E0
	add r1, #0x2C
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _08050514
	b _0805060E
_08050514:
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _0805060E
	add r6, r1, #0
	mov r3, #0
	ldr r4, _08050558 @ =0x000007FF
	add r1, r4, #0
	add r0, r6, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805055C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08050560 @ =0x00000482
	cmp r1, r0
	beq _080505B2
	cmp r1, r0
	bgt _08050570
	sub r0, #0x37
	cmp r1, r0
	beq _080505B2
	cmp r1, r0
	bgt _08050564
	sub r0, #0x91
	b _0805057E
	.align 2, 0
_0805054C: .4byte 0x00001B21
_08050550: .4byte 0x020192E0
_08050554: .4byte 0x00000D64
_08050558: .4byte 0x000007FF
_0805055C: .4byte gCardIdToNumber
_08050560: .4byte 0x00000482
_08050564:
	ldr r0, _0805056C @ =0x0000047A
	cmp r1, r0
	beq _08050590
	b _080505EC
_0805056C: .4byte 0x0000047A
_08050570:
	mov r0, #0xB2
	lsl r0, r0, #3
	cmp r1, r0
	beq _080505B2
	cmp r1, r0
	bgt _08050584
	sub r0, #4
_0805057E:
	cmp r1, r0
	beq _080505B2
	b _080505EC
_08050584:
	ldr r0, _0805058C @ =0x00000597
	cmp r1, r0
	beq _080505A2
	b _080505EC
_0805058C: .4byte 0x00000597
_08050590:
	add r0, r7, #0
	mov r1, #1
	neg r1, r1
	bl CountTributableMonsters
	cmp r0, #0
	ble _080505A0
	b _08050A38
_080505A0:
	b _080505F0
_080505A2:
	add r0, r7, #0
	add r1, r2, #0
	bl CountTributableMonsters
	cmp r0, #0
	ble _080505B0
	b _08050A48
_080505B0:
	b _080505F0
_080505B2:
	ldr r5, _080505DC @ =0x020192E4
	mov r2, sl
	add r0, r2, r5
	ldrh r4, [r0]
	ldr r3, _080505E0 @ =0x000007FF
	add r1, r3, #0
	add r0, r6, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080505E4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl GetMaintenanceLpCost
	cmp r4, r0
	blt _080505F0
	ldr r2, _080505E8 @ =0x00001B1C
	add r1, r5, r2
	mov r0, #0x6E
	b _08050A52
	.align 2, 0
_080505DC: .4byte 0x020192E4
_080505E0: .4byte 0x000007FF
_080505E4: .4byte gCardIdToNumber
_080505E8: .4byte 0x00001B1C
_080505EC:
	cmp r3, #0
	beq _0805060E
_080505F0:
	mov r0, #0x74
	cmp r7, #0
	beq _080505F8
	ldr r0, _0805062C @ =0x00008074
_080505F8:
	add r1, r6, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	mov r3, r8
	ldrb r1, [r3]
	add r0, r7, #0
	mov r2, #1
	bl DestroyFieldCard
_0805060E:
	mov r4, r8
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bhi _08050620
	b _080504F8
_08050620:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	ldrb r0, [r1]
	add r0, #1
	b _08050A52
_0805062C: .4byte 0x00008074
_08050630:
	mov r4, #5
	ldr r0, _0805068C @ =0x00000D64
	add r5, r7, #0
	mul r5, r0
_08050638:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r5
	ldr r1, _08050690 @ =0x0201930C
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08050682
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08050682
	ldr r0, _08050694 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r6, _08050698 @ =0x08622AB4
	add r0, r0, r6
	ldrh r1, [r0]
	ldr r0, _0805069C @ =0x00000416
	cmp r1, r0
	beq _0805066E
	add r0, #0xE
	cmp r1, r0
	bne _08050682
_0805066E:
	mov r0, #0xB4
	cmp r7, #0
	beq _08050676
	ldr r0, _080506A0 @ =0x000080B4
_08050676:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08050682:
	add r4, #1
	cmp r4, #9
	ble _08050638
	b _0805099C
	.align 2, 0
_0805068C: .4byte 0x00000D64
_08050690: .4byte 0x0201930C
_08050694: .4byte 0x000007FF
_08050698: .4byte gCardIdToNumber
_0805069C: .4byte 0x00000416
_080506A0: .4byte 0x000080B4
_080506A4:
	add r2, r7, #0
	mov r3, #1
	and r2, r3
	ldr r1, _080506FC @ =0x00001B21
	add r1, r9
	mov r0, #0x94
	ldrb r1, [r1]
	mul r0, r1
	ldr r1, _08050700 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	mov r1, r9
	add r1, #0x2C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	cmp r7, #0
	bne _080506CC
	b _080507F0
_080506CC:
	ldr r0, _08050704 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r4, _08050708 @ =0x08622AB4
	add r0, r0, r4
	ldrh r2, [r0]
	ldr r0, _0805070C @ =0x000003BA
	cmp r2, r0
	beq _08050714
	add r0, #0x91
	cmp r2, r0
	beq _080507C4
	ldr r0, _08050710 @ =0x00000D68
	add r0, r9
	ldrh r4, [r0]
	add r0, r2, #0
	bl GetMaintenanceLpCost
	mov r1, #0xFA
	lsl r1, r1, #2
	add r0, r0, r1
	cmp r4, r0
	ble _080507E4
	b _080507D0
_080506FC: .4byte 0x00001B21
_08050700: .4byte 0x00000D64
_08050704: .4byte 0x000007FF
_08050708: .4byte gCardIdToNumber
_0805070C: .4byte 0x000003BA
_08050710: .4byte 0x00000D68
_08050714:
	mov r0, #1
	ldr r1, _080507B0 @ =0x000002D6
	bl AiFindHandCardByNumber
	neg r1, r0
	orr r1, r0
	lsr r4, r1, #0x1F
	ldr r3, _080507B4 @ =0x000002D7
	mov sl, r3
	mov r0, #1
	mov r1, sl
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _08050734
	mov r4, #1
_08050734:
	mov r0, #0xB6
	lsl r0, r0, #2
	mov r8, r0
	mov r0, #1
	mov r1, r8
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _08050748
	mov r4, #1
_08050748:
	ldr r5, _080507B8 @ =0x000002FE
	mov r0, #1
	add r1, r5, #0
	bl AiFindHandCardByNumber
	cmp r0, #0
	beq _08050758
	mov r4, #1
_08050758:
	mov r0, #1
	ldr r1, _080507B0 @ =0x000002D6
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _08050766
	mov r4, #1
_08050766:
	mov r0, #1
	mov r1, sl
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _08050774
	mov r4, #1
_08050774:
	mov r0, #1
	mov r1, r8
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _08050782
	mov r4, #1
_08050782:
	mov r0, #1
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _08050790
	mov r4, #1
_08050790:
	ldr r2, _080507BC @ =0x0201AE60
	mov r0, #0
	strh r0, [r2, #0x14]
	cmp r4, #0
	beq _08050830
	ldr r1, _080507C0 @ =0x00000D68
	add r1, r9
	mov r0, #0xFA
	lsl r0, r0, #2
	ldrh r1, [r1]
	cmp r1, r0
	bls _08050830
	mov r1, #1
	strh r1, [r2, #0x14]
	b _08050830
	.align 2, 0
_080507B0: .4byte 0x000002D6
_080507B4: .4byte 0x000002D7
_080507B8: .4byte 0x000002FE
_080507BC: .4byte 0x0201AE60
_080507C0: .4byte 0x00000D68
_080507C4:
	ldr r1, _080507D8 @ =0x00000D68
	add r1, r9
	ldr r0, _080507DC @ =0x00001B58
	ldrh r1, [r1]
	cmp r1, r0
	bls _080507E4
_080507D0:
	ldr r0, _080507E0 @ =0x0201AE60
	mov r2, #1
	strh r2, [r0, #0x14]
	b _08050830
_080507D8: .4byte 0x00000D68
_080507DC: .4byte 0x00001B58
_080507E0: .4byte 0x0201AE60
_080507E4:
	ldr r1, _080507EC @ =0x0201AE60
	mov r0, #0
	strh r0, [r1, #0x14]
	b _08050830
_080507EC: .4byte 0x0201AE60
_080507F0:
	add r4, sp, #0x84
	ldr r1, _08050844 @ =0x08085CB4
	lsl r2, r6, #6
	ldr r3, _08050848 @ =0x0822C720
	add r2, r2, r3
	add r0, r4, #0
	bl FormatStr
	ldr r0, _0805084C @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08050850 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl GetMaintenanceLpCost
	add r2, r0, #0
	add r0, sp, #4
	add r1, r4, #0
	bl FormatInt
	ldr r0, _08050854 @ =0x00000206
	ldr r1, _08050858 @ =0x00000613
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
_08050830:
	add r0, r7, #0
	add r1, r6, #0
	bl ShowActivatedCard
	ldr r0, _0805085C @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	b _080509A4
	.align 2, 0
_08050844: .4byte gStrMaintainLpCostFmt
_08050848: .4byte gCardNames
_0805084C: .4byte 0x000007FF
_08050850: .4byte gCardIdToNumber
_08050854: .4byte 0x00000206
_08050858: .4byte 0x00000613
_0805085C: .4byte 0x020192E0
_08050860:
	ldr r0, _080508D0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _080508F4
	ldr r2, _080508D4 @ =0x020192E0
	ldr r3, _080508D8 @ =0x00001B21
	add r3, r3, r2
	mov r8, r3
	mov r0, #0x94
	ldrb r4, [r3]
	mul r0, r4
	ldr r1, _080508DC @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	add r2, #0x2C
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	mov r5, #0x43
	cmp r7, #0
	beq _0805088E
	ldr r5, _080508E0 @ =0x00008043
_0805088E:
	ldr r4, _080508E4 @ =0x000007FF
	and r4, r6
	lsl r4, r4, #1
	ldr r6, _080508E8 @ =0x08622AB4
	add r4, r4, r6
	ldrh r0, [r4]
	bl GetMaintenanceLpCost
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r5, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _080508EC @ =0x000003BA
	ldrh r4, [r4]
	cmp r4, r0
	bne _08050904
	mov r0, #0xB3
	cmp r7, #0
	beq _080508BE
	ldr r0, _080508F0 @ =0x000080B3
_080508BE:
	mov r2, r8
	ldrb r1, [r2]
	mov r2, #0xFA
	lsl r2, r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _08050904
	.align 2, 0
_080508D0: .4byte 0x0201AE60
_080508D4: .4byte 0x020192E0
_080508D8: .4byte 0x00001B21
_080508DC: .4byte 0x00000D64
_080508E0: .4byte 0x00008043
_080508E4: .4byte 0x000007FF
_080508E8: .4byte gCardIdToNumber
_080508EC: .4byte 0x000003BA
_080508F0: .4byte 0x000080B3
_080508F4:
	ldr r0, _0805091C @ =0x020192E0
	ldr r3, _08050920 @ =0x00001B21
	add r0, r0, r3
	ldrb r1, [r0]
	add r0, r7, #0
	mov r2, #1
	bl DestroyFieldCard
_08050904:
	ldr r1, _0805091C @ =0x020192E0
	ldr r4, _08050920 @ =0x00001B21
	add r2, r1, r4
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	mov r6, #0xD9
	lsl r6, r6, #5
	add r1, r1, r6
	mov r0, #0x65
	b _08050A52
	.align 2, 0
_0805091C: .4byte 0x020192E0
_08050920: .4byte 0x00001B21
_08050924:
	ldr r1, _08050970 @ =0x00001B21
	add r1, r9
	mov r0, #0x94
	ldrb r1, [r1]
	mul r0, r1
	ldr r1, _08050974 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	mov r1, r9
	add r1, #0x2C
	add r0, r0, r1
	ldr r2, [r0]
	lsl r2, r2, #0x14
	ldr r1, _08050978 @ =0x08085D08
	lsr r2, r2, #0xE
	ldr r0, _0805097C @ =0x0822C720
	add r2, r2, r0
	add r0, sp, #4
	bl FormatStr
	ldr r0, _08050980 @ =0x00000206
	ldr r1, _08050984 @ =0x00000813
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	ldrb r0, [r1]
	add r0, #1
	b _08050A52
	.align 2, 0
_08050970: .4byte 0x00001B21
_08050974: .4byte 0x00000D64
_08050978: .4byte gStrMaintainTributeFmt
_0805097C: .4byte gCardNames
_08050980: .4byte 0x00000206
_08050984: .4byte 0x00000813
_08050988:
	ldr r0, _080509B0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _080509C4
	ldr r0, _080509B4 @ =0x00000206
	ldr r1, _080509B8 @ =0x00000712
	ldr r3, _080509BC @ =0x08085D70
	mov r2, #0xB
	bl TextBoxOpen
_0805099C:
	ldr r0, _080509C0 @ =0x020192E0
	mov r1, #0xD9
	lsl r1, r1, #5
	add r0, r0, r1
_080509A4:
	ldrb r1, [r0]
	add r1, #1
_080509A8:
	strb r1, [r0]
	bl _0804FF8E @ far jump
	.align 2, 0
_080509B0: .4byte 0x0201AE60
_080509B4: .4byte 0x00000206
_080509B8: .4byte 0x00000712
_080509BC: .4byte gStrSelectMonsterAsTribute
_080509C0: .4byte 0x020192E0
_080509C4:
	ldr r4, _080509E4 @ =0x00001B21
	add r4, r9
	ldrb r1, [r4]
	add r0, r7, #0
	mov r2, #1
	bl DestroyFieldCard
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r1, #0xD9
	lsl r1, r1, #5
	add r1, r9
	mov r0, #0x65
	b _08050A52
	.align 2, 0
_080509E4: .4byte 0x00001B21
_080509E8:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _080509F6
	bl _0804FF8E @ far jump
_080509F6:
	ldr r1, _08050A24 @ =0x0201CFB0
	ldr r2, _08050A28 @ =0x00000824
	add r0, r1, r2
	ldr r0, [r0]
	ldr r3, _08050A2C @ =0x0000082C
	add r1, r1, r3
	ldr r1, [r1]
	bl TributeMonster
	ldr r1, _08050A30 @ =0x020192E0
	ldr r4, _08050A34 @ =0x00001B21
	add r2, r1, r4
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	mov r6, #0xD9
	lsl r6, r6, #5
	add r1, r1, r6
	mov r0, #0x65
	strb r0, [r1]
	bl _0804FF8E @ far jump
	.align 2, 0
_08050A24: .4byte 0x0201CFB0
_08050A28: .4byte 0x00000824
_08050A2C: .4byte 0x0000082C
_08050A30: .4byte 0x020192E0
_08050A34: .4byte 0x00001B21
_08050A38:
	ldr r0, _08050A44 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r1, r0, r2
	b _08050A50
	.align 2, 0
_08050A44: .4byte 0x020192E0
_08050A48:
	ldr r3, _08050A58 @ =0x020192E0
	mov r4, #0xD9
	lsl r4, r4, #5
	add r1, r3, r4
_08050A50:
	mov r0, #0x78
_08050A52:
	strb r0, [r1]
	bl _0804FF8E @ far jump
_08050A58: .4byte 0x020192E0
_08050A5C:
	mov r0, #1
_08050A5E:
	add sp, #0x10C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelPhase_Standby
	.align 2, 0

