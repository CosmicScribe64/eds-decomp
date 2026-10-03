	thumb_func_start AiStrategyToonWorld
AiStrategyToonWorld: @ 0x0805D254
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r5, _0805D26C @ =0x02015EF0
	ldrb r0, [r5, #2]
	cmp r0, #1
	beq _0805D328
	cmp r0, #1
	bgt _0805D270
	cmp r0, #0
	beq _0805D27C
	b _0805D4A6
	.align 2, 0
_0805D26C: .4byte 0x02015EF0
_0805D270:
	cmp r0, #2
	beq _0805D354
	cmp r0, #3
	bne _0805D27A
	b _0805D378
_0805D27A:
	b _0805D4A6
_0805D27C:
	mov r0, #1
	bl HasFaceUpToonWorld
	cmp r0, #0
	beq _0805D28C
	mov r0, #2
	strb r0, [r5, #2]
	b _0805D4A6
_0805D28C:
	ldr r0, _0805D2E4 @ =0x000003BA
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805D30C
	ldr r2, _0805D2E8 @ =0x020192E0
	ldr r0, _0805D2EC @ =0x00001B30
	add r1, r2, r0
	ldr r0, _0805D2F0 @ =0xFFFFFC03
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldrb r6, [r5, #0xB]
	lsl r0, r6, #2
	ldr r3, _0805D2F4 @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805D2F8 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805D2FC @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805D300 @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805D304 @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805D308 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	b _0805D344
_0805D2E4: .4byte 0x000003BA
_0805D2E8: .4byte 0x020192E0
_0805D2EC: .4byte 0x00001B30
_0805D2F0: .4byte 0xFFFFFC03
_0805D2F4: .4byte 0x000013EC
_0805D2F8: .4byte 0x00001B28
_0805D2FC: .4byte 0x00001B33
_0805D300: .4byte 0x00001B34
_0805D304: .4byte 0xFFFFFE01
_0805D308: .4byte 0x00001B2C
_0805D30C:
	ldr r1, _0805D320 @ =0x02015F00
	ldr r2, _0805D324 @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	b _0805D4A6
	.align 2, 0
_0805D320: .4byte 0x02015F00
_0805D324: .4byte 0x00001B24
_0805D328:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r1, _0805D34C @ =0x020192E0
	ldr r6, _0805D350 @ =0x00001B2C
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805D344
	b _0805D4A6
_0805D344:
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
	b _0805D4A6
_0805D34C: .4byte 0x020192E0
_0805D350: .4byte 0x00001B2C
_0805D354:
	mov r0, #1
	bl HasFaceUpToonWorld
	cmp r0, #0
	bne _0805D344
	ldr r1, _0805D370 @ =0x02015F00
	ldr r0, _0805D374 @ =0x00001B24
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0805D4A6
_0805D370: .4byte 0x02015F00
_0805D374: .4byte 0x00001B24
_0805D378:
	mov r6, #0
	ldr r0, _0805D3D0 @ =0x020192E4
	ldr r3, _0805D3D4 @ =0x00000D66
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r6, r0
	bge _0805D458
	add r7, r5, #0
_0805D388:
	lsl r0, r6, #2
	ldr r1, _0805D3D8 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	ldr r0, _0805D3DC @ =0x000007FF
	add r1, r0, #0
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805D3E0 @ =0x08622AB4
	add r4, r0, r1
	ldrh r0, [r4]
	bl IsToonMonster
	cmp r0, #0
	beq _0805D44A
	mov r0, #1
	add r1, r5, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _0805D44A
	ldrh r1, [r4]
	mov r0, #0xB6
	lsl r0, r0, #2
	cmp r1, r0
	beq _0805D40C
	cmp r1, r0
	bgt _0805D3E4
	sub r0, #1
	cmp r1, r0
	beq _0805D3F0
	b _0805D44A
	.align 2, 0
_0805D3D0: .4byte 0x020192E4
_0805D3D4: .4byte 0x00000D66
_0805D3D8: .4byte 0x0201A6CC
_0805D3DC: .4byte 0x000007FF
_0805D3E0: .4byte gCardIdToNumber
_0805D3E4:
	ldr r0, _0805D3EC @ =0x000002FE
	cmp r1, r0
	beq _0805D42C
	b _0805D44A
_0805D3EC: .4byte 0x000002FE
_0805D3F0:
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	mov r0, #1
	str r0, [sp, #0]
	add r1, r6, #0
	mov r3, #0
_0805D400:
	bl QueueSpecialSummonFromHand
	ldrb r0, [r7, #2]
	sub r0, #1
	strb r0, [r7, #2]
	b _0805D4A6
_0805D40C:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	cmp r5, r4
	beq _0805D44A
	mov r3, #0x90
	orr r3, r5
	mov r0, #1
	str r0, [sp, #0]
	add r1, r6, #0
	add r2, r5, #0
	b _0805D400
_0805D42C:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r1, r0, #0
	cmp r5, r4
	beq _0805D44A
	cmp r1, r4
	bne _0805D47C
_0805D44A:
	add r6, #1
	ldr r0, _0805D46C @ =0x020192E4
	ldr r2, _0805D470 @ =0x00000D66
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r6, r0
	blt _0805D388
_0805D458:
	ldr r1, _0805D474 @ =0x02015F00
	ldr r3, _0805D478 @ =0x00001B24
	add r1, r1, r3
	mov r0, #2
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
	b _0805D4A6
	.align 2, 0
_0805D46C: .4byte 0x020192E4
_0805D470: .4byte 0x00000D66
_0805D474: .4byte 0x02015F00
_0805D478: .4byte 0x00001B24
_0805D47C:
	mov r2, #0x70
	neg r2, r2
	add r0, r2, #0
	add r3, r5, #0
	orr r3, r0
	lsl r3, r3, #0x18
	orr r1, r0
	lsl r0, r1, #0x18
	lsr r3, r3, #8
	orr r3, r0
	lsr r3, r3, #0x10
	mov r0, #1
	str r0, [sp, #0]
	add r1, r6, #0
	add r2, r5, #0
	bl QueueSpecialSummonFromHand
	ldr r1, _0805D4B0 @ =0x02015EF0
	ldrb r0, [r1, #2]
	sub r0, #1
	strb r0, [r1, #2]
_0805D4A6:
	mov r0, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0805D4B0: .4byte 0x02015EF0
	thumb_func_end AiStrategyToonWorld

