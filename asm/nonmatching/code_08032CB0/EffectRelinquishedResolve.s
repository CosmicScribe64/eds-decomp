	thumb_func_start EffectRelinquishedResolve
EffectRelinquishedResolve: @ 0x08032D10
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	mov r8, r0
	mov r0, #4
	ldrb r2, [r4, #4]
	and r0, r2
	cmp r0, #0
	beq _08032D36
	b _08032E50
_08032D36:
	mov r0, #1
	ldrb r3, [r4, #2]
	and r0, r3
	mov r2, #0x92
	cmp r0, #0
	beq _08032D44
	ldr r2, _08032E64 @ =0x00008092
_08032D44:
	ldrh r6, [r4, #2]
	lsl r1, r6, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r5, #7
	ldrb r7, [r4, #0xA]
	and r5, r7
	cmp r5, #1
	bne _08032E50
	mov r0, r8
	cmp r0, #0
	blt _08032E50
	ldrb r1, [r4, #0xC]
	str r1, [sp, #0]
	ldrh r2, [r4, #0xC]
	lsr r2, r2, #8
	mov sl, r2
	and r1, r5
	mov r3, #0x94
	mov ip, r3
	mov r0, sl
	mul r0, r3
	ldr r6, _08032E68 @ =0x00000D64
	mul r1, r6
	add r0, r0, r1
	ldr r6, _08032E6C @ =0x0201930C
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r9, r0
	ldrb r7, [r4, #2]
	lsl r2, r7, #0x1F
	lsr r0, r2, #0x1F
	add r1, r5, #0
	and r1, r0
	ldrh r0, [r4, #2]
	lsl r3, r0, #0x16
	lsr r0, r3, #0x1A
	mov r7, ip
	mul r7, r0
	add r0, r7, #0
	ldr r7, _08032E68 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032E50
	lsr r0, r2, #0x1F
	add r1, r5, #0
	and r1, r0
	lsr r0, r3, #0x1A
	mov r7, ip
	mul r7, r0
	add r0, r7, #0
	ldr r7, _08032E68 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08032E70 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08032E74 @ =0x000002DA
	ldrh r0, [r0]
	cmp r0, r1
	bne _08032E50
	lsr r1, r2, #0x1F
	add r0, r5, #0
	and r0, r1
	lsr r1, r3, #0x1A
	mov r3, ip
	mul r3, r1
	add r1, r3, #0
	ldr r7, _08032E68 @ =0x00000D64
	mul r0, r7
	add r1, r1, r0
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08032E50
	mov r0, r9
	cmp r0, #0
	beq _08032E50
	lsr r0, r2, #0x1F
	ldrh r1, [r4, #0xC]
	add r2, r0, #0
	mov r6, r8
	lsl r3, r6, #0x18
	lsr r6, r3, #0x10
	orr r2, r6
	bl MoveFieldCard
	ldr r0, [sp, #0]
	mov r1, sl
	mov r2, #0
	bl DestroyLinkedCards
	ldrb r7, [r4, #2]
	and r5, r7
	mov r0, #0x8C
	cmp r5, #0
	beq _08032E26
	ldr r0, _08032E78 @ =0x0000808C
_08032E26:
	mov r2, r8
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r3, [r4, #2]
	lsl r2, r3, #0x1F
	lsr r0, r2, #0x1F
	add r1, r0, #0
	orr r1, r6
	add r2, r0, #0
	ldrh r4, [r4, #2]
	lsl r3, r4, #0x16
	lsr r3, r3, #0x1A
	lsl r3, r3, #8
	orr r2, r3
	mov r3, #5
	bl QueueAddZoneLink
_08032E50:
	mov r0, #0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08032E64: .4byte 0x00008092
_08032E68: .4byte 0x00000D64
_08032E6C: .4byte 0x0201930C
_08032E70: .4byte gCardIdToNumber
_08032E74: .4byte 0x000002DA
_08032E78: .4byte 0x0000808C
	thumb_func_end EffectRelinquishedResolve

