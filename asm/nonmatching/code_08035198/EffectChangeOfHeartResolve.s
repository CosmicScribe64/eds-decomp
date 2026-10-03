	thumb_func_start EffectChangeOfHeartResolve
EffectChangeOfHeartResolve: @ 0x080353A4
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _08035476
	mov r4, #7
	ldrb r0, [r5, #0xA]
	and r4, r0
	cmp r4, #1
	bne _08035476
	ldrb r6, [r5, #0xC]
	ldrh r1, [r5, #0xC]
	lsr r7, r1, #8
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	add r3, r0, #0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r6, r0
	beq _08035476
	and r4, r6
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08035438 @ =0x00000D64
	mul r0, r4
	add r1, r1, r0
	ldr r0, _0803543C @ =0x0201930C
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08035476
	mov r0, #1
	neg r0, r0
	cmp r3, r0
	beq _08035476
	ldr r0, _08035440 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08035444 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08035448 @ =0x000004B1
	ldrh r0, [r0]
	cmp r0, r1
	bne _08035450
	mov r0, #3
	ldrb r1, [r4, #6]
	and r0, r1
	cmp r0, #1
	bne _08035450
	mov r0, #0x7F
	cmp r6, #0
	beq _08035420
	ldr r0, _0803544C @ =0x0000807F
_08035420:
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r6, #0
	bl sub_080197C0
	b _08035476
_08035438: .4byte 0x00000D64
_0803543C: .4byte 0x0201930C
_08035440: .4byte 0x000007FF
_08035444: .4byte gCardIdToNumber
_08035448: .4byte 0x000004B1
_0803544C: .4byte 0x0000807F
_08035450:
	ldrb r0, [r5, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r5, #0xC]
	add r2, r0, #0
	lsl r4, r3, #0x18
	lsr r4, r4, #0x10
	orr r2, r4
	bl MoveFieldCard
	ldrb r1, [r5, #2]
	lsl r2, r1, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r5]
	add r2, r0, #0
	orr r2, r4
	mov r3, #3
	bl QueueAddZoneLink
_08035476:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectChangeOfHeartResolve
	.align 2, 0

