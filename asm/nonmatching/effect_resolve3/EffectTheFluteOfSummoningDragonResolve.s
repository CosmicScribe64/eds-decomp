	thumb_func_start EffectTheFluteOfSummoningDragonResolve
EffectTheFluteOfSummoningDragonResolve: @ 0x08033AEC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _08033B24 @ =0xFFFFFD00
	add sp, r4
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _08033B04
	b _08033D98
_08033B04:
	ldr r1, _08033B28 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	add r2, r1, #0
	cmp r0, #4
	bls _08033B18
	b _08033D98
_08033B18:
	lsl r0, r0, #2
	ldr r1, _08033B2C @ =0x08033B30
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08033B24: .4byte 0xFFFFFD00
_08033B28: .4byte 0x02017A40
_08033B2C: .4byte 0x08033B30
_08033B30:
	.4byte _08033D38
	.4byte _08033C9C
	.4byte _08033BA0
	.4byte _08033B58
	.4byte _08033B44
_08033B44:
	ldr r3, _08033B8C @ =0x000003E1
	add r1, r2, r3
	mov r0, #0
	strb r0, [r1]
	mov r0, #0xF8
	lsl r0, r0, #2
	add r1, r2, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_08033B58:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _08033B68
	b _08033D98
_08033B68:
	add r0, r6, #0
	mov r1, #0
	mov r2, #0
	bl EffectTheFluteOfSummoningDragonPrepare
	cmp r0, #0
	bne _08033B78
	b _08033D98
_08033B78:
	ldr r1, _08033B90 @ =0x08082BFC
	ldr r2, _08033B94 @ =0x08082C34
	mov r0, sp
	bl FormatStr
	ldr r0, _08033B98 @ =0x00000205
	ldr r1, _08033B9C @ =0x00000914
	mov r2, #0xB
	mov r3, sp
	b _08033D78
_08033B8C: .4byte 0x000003E1
_08033B90: .4byte gStrFluteSpecialSummonQuestion
_08033B94: .4byte gStrDragon
_08033B98: .4byte 0x00000205
_08033B9C: .4byte 0x00000914
_08033BA0:
	ldr r1, _08033BC4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08033BD8
	add r4, sp, #0x100
	ldr r1, _08033BC8 @ =0x08082BFC
	ldr r2, _08033BCC @ =0x08082C34
	add r0, r4, #0
	bl FormatStr
	ldr r0, _08033BD0 @ =0x00000205
	ldr r1, _08033BD4 @ =0x00000914
	mov r2, #0xB
	add r3, r4, #0
	b _08033D78
	.align 2, 0
_08033BC4: .4byte 0x03000040
_08033BC8: .4byte gStrFluteSpecialSummonQuestion
_08033BCC: .4byte gStrDragon
_08033BD0: .4byte 0x00000205
_08033BD4: .4byte 0x00000914
_08033BD8:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08033BE4
	b _08033D58
_08033BE4:
	ldrb r2, [r6, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	ldr r0, _08033C78 @ =0x0201CFB0
	ldr r3, _08033C7C @ =0x0000082C
	add r3, r3, r0
	mov r8, r3
	ldr r0, [r3]
	lsl r0, r0, #2
	ldr r7, _08033C80 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r5, _08033C84 @ =0x02019968
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r2, r0, #0x15
	lsl r0, r2, #2
	ldr r1, _08033C88 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r4, r0, #0x14
	cmp r4, #1
	bne _08033C94
	lsl r0, r2, #1
	ldr r2, _08033C8C @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	beq _08033C38
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl HasFaceUpToonWorld
	cmp r0, #0
	beq _08033C94
_08033C38:
	ldrb r2, [r6, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	add r0, r4, #0
	and r0, r1
	add r1, r0, #0
	mul r1, r7
	add r1, r1, r5
	mov r3, r8
	ldr r0, [r3]
	lsl r0, r0, #2
	add r5, r1, r0
	and r4, r2
	mov r0, #0xC2
	cmp r4, #0
	beq _08033C5A
	ldr r0, _08033C90 @ =0x000080C2
_08033C5A:
	ldrh r1, [r5]
	ldrh r2, [r5, #2]
	mov r3, #0
	bl DuelCmd_Push
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummonChoosePosition
	mov r0, #0x7D
	b _08033D9A
_08033C78: .4byte 0x0201CFB0
_08033C7C: .4byte 0x0000082C
_08033C80: .4byte 0x00000D64
_08033C84: .4byte 0x02019968
_08033C88: .4byte gCardStats
_08033C8C: .4byte gCardIdToNumber
_08033C90: .4byte 0x000080C2
_08033C94:
	mov r0, #3
	bl PlaySE
	b _08033D58
_08033C9C:
	ldr r0, _08033D20 @ =0x000003E1
	add r1, r2, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #2
	beq _08033D98
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	beq _08033D98
	mov r2, #0
	ldr r4, _08033D24 @ =0x020192E4
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r0, r1, #0x1F
	ldr r5, _08033D28 @ =0x00000D64
	mul r0, r5
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r2, r0
	bge _08033D98
	add r3, r1, #0
	mov r6, #1
	ldr r0, _08033D2C @ =0x00000684
	add r0, r0, r4
	mov ip, r0
	add r7, r4, #0
	ldr r4, _08033D30 @ =0x000007FF
_08033CE0:
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	lsl r0, r2, #2
	mul r1, r5
	add r0, r0, r1
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08033D34 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #1
	beq _08033D70
	add r2, #1
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r5
	add r0, r0, r7
	ldrb r0, [r0, #2]
	cmp r2, r0
	blt _08033CE0
	b _08033D98
_08033D20: .4byte 0x000003E1
_08033D24: .4byte 0x020192E4
_08033D28: .4byte 0x00000D64
_08033D2C: .4byte 0x00000684
_08033D30: .4byte 0x000007FF
_08033D34: .4byte gCardStats
_08033D38:
	ldr r0, _08033D5C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08033D98
	add r4, sp, #0x200
	ldr r1, _08033D60 @ =0x08082C70
	ldr r2, _08033D64 @ =0x08082C34
	add r0, r4, #0
	bl FormatStr
	ldr r0, _08033D68 @ =0x00000205
	ldr r1, _08033D6C @ =0x00000914
	mov r2, #0xB
	add r3, r4, #0
	bl TextBoxOpen
_08033D58:
	mov r0, #0x7E
	b _08033D9A
_08033D5C: .4byte 0x0201AE60
_08033D60: .4byte gStrFluteSelectFromHand
_08033D64: .4byte gStrDragon
_08033D68: .4byte 0x00000205
_08033D6C: .4byte 0x00000914
_08033D70:
	ldr r0, _08033D8C @ =0x00000205
	ldr r1, _08033D90 @ =0x00000914
	ldr r3, _08033D94 @ =0x08082C3C
	mov r2, #0xB
_08033D78:
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7C
	b _08033D9A
	.align 2, 0
_08033D8C: .4byte 0x00000205
_08033D90: .4byte 0x00000914
_08033D94: .4byte gStrSpecialSummonAnotherQuestion
_08033D98:
	mov r0, #0
_08033D9A:
	mov r3, #0xC0
	lsl r3, r3, #2
	add sp, r3
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTheFluteOfSummoningDragonResolve
	.align 2, 0

