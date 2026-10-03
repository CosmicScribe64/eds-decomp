	thumb_func_start EffectRitualSummonPrepare
EffectRitualSummonPrepare: @ 0x08043AA8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CanSpecialSummon
	cmp r0, #0
	beq _08043B84
	mov r2, #0
	ldr r3, _08043B14 @ =0x0819A990
_08043AC2:
	lsl r0, r2, #2
	add r4, r0, r3
	ldrh r6, [r4]
	lsl r1, r6, #0x13
	mov r8, r0
	cmp r1, #0
	beq _08043B84
	ldr r1, [r4]
	lsl r1, r1, #6
	lsr r1, r1, #0x13
	ldr r6, _08043B18 @ =0x000007FF
	add r0, r6, #0
	ldrh r7, [r5]
	and r0, r7
	lsl r0, r0, #1
	ldr r7, _08043B1C @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r1, r0
	bne _08043B88
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r2, #0
	bl HandHasRitualMonster
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08043B84
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r2, r0, #0x1F
	ldrh r4, [r4]
	lsl r0, r4, #0x13
	lsr r1, r0, #0x13
	ldr r0, _08043B20 @ =0x0000FFFF
	cmp r1, r0
	bne _08043B24
	mov r0, #0
	b _08043B50
	.align 2, 0
_08043B14: .4byte gRitualRecipes
_08043B18: .4byte 0x000007FF
_08043B1C: .4byte gCardIdToNumber
_08043B20: .4byte 0x0000FFFF
_08043B24:
	ldr r0, _08043B38 @ =0x000007CF
	cmp r1, r0
	bhi _08043B40
	and r1, r6
	lsl r0, r1, #1
	ldr r6, _08043B3C @ =0x08623DF4
	add r0, r0, r6
	ldrh r0, [r0]
	b _08043B50
	.align 2, 0
_08043B38: .4byte 0x000007CF
_08043B3C: .4byte gCardNumberToId
_08043B40:
	ldr r7, _08043B78 @ =0xFFFFF830
	add r0, r1, r7
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08043B7C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_08043B50:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	bl SumHandLevelsExcept
	add r4, r0, #0
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	bl SumTributableMonsterLevels
	add r4, r4, r0
	ldr r0, _08043B80 @ =0x0819A990
	add r0, r8
	ldrb r0, [r0, #3]
	lsr r0, r0, #2
	cmp r4, r0
	blt _08043B84
	mov r0, #1
	b _08043B8C
_08043B78: .4byte 0xFFFFF830
_08043B7C: .4byte gCardNumberToId
_08043B80: .4byte gRitualRecipes
_08043B84:
	mov r0, #0
	b _08043B8C
_08043B88:
	add r2, #1
	b _08043AC2
_08043B8C:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectRitualSummonPrepare
	.align 2, 0

