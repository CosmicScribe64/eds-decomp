	thumb_func_start EffectSnatchStealResolve
EffectSnatchStealResolve: @ 0x08035E0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r0, #0
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	lsr r2, r3, #0x1F
	ldrh r1, [r4, #2]
	lsl r0, r1, #0x16
	lsr r1, r0, #0x1A
	mov r0, #8
	ldrb r5, [r4, #4]
	and r0, r5
	cmp r0, #0
	bne _08035EF2
	mov r0, #0x94
	mul r1, r0
	ldr r5, _08035F04 @ =0x00000D64
	add r0, r2, #0
	mul r0, r5
	add r1, r1, r0
	ldr r0, _08035F08 @ =0x0201930C
	mov r9, r0
	add r1, r9
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08035EF2
	mov r5, #2
	mov sl, r5
	mov r0, sl
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08035EF2
	ldr r0, _08035F0C @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _08035F10 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08035F14 @ =0x0000042C
	ldrh r0, [r0]
	cmp r0, r1
	bne _08035EF2
	mov r5, #7
	ldrb r0, [r4, #0xA]
	and r5, r0
	cmp r5, #1
	bne _08035EF2
	ldrb r6, [r4, #0xC]
	ldrh r1, [r4, #0xC]
	lsr r7, r1, #8
	lsr r0, r3, #0x1F
	bl FindFreeMonsterZone
	mov r8, r0
	ldrb r3, [r4, #2]
	lsl r2, r3, #0x1F
	lsr r0, r2, #0x1F
	cmp r6, r0
	beq _08035EF2
	and r5, r6
	mov r1, #0x94
	add r0, r7, #0
	mul r0, r1
	ldr r3, _08035F04 @ =0x00000D64
	add r1, r5, #0
	mul r1, r3
	add r0, r0, r1
	mov r5, r9
	add r1, r0, r5
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08035EF2
	mov r0, sl
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08035EF2
	lsr r0, r2, #0x1F
	add r1, r0, #0
	ldrh r3, [r4, #2]
	lsl r2, r3, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	lsl r2, r7, #8
	orr r6, r2
	add r2, r6, #0
	bl EquipCard
	mov r0, #4
	ldrb r5, [r4, #4]
	and r0, r5
	cmp r0, #0
	bne _08035EF2
	mov r0, #1
	neg r0, r0
	cmp r8, r0
	beq _08035EF2
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	lsr r0, r3, #0x1F
	ldrh r1, [r4, #0xC]
	add r3, r0, #0
	mov r4, r8
	lsl r2, r4, #0x18
	lsr r2, r2, #0x10
	orr r2, r3
	bl MoveFieldCard
_08035EF2:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08035F04: .4byte 0x00000D64
_08035F08: .4byte 0x0201930C
_08035F0C: .4byte 0x000007FF
_08035F10: .4byte gCardIdToNumber
_08035F14: .4byte 0x0000042C
	thumb_func_end EffectSnatchStealResolve

