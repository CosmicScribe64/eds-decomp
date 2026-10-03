	thumb_func_start DuelCmd_ReturnToDeck
DuelCmd_ReturnToDeck: @ 0x0800E438
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, _0800E484 @ =0x020185C0
	ldrh r0, [r4]
	lsr r7, r0, #0xF
	ldrh r1, [r4, #2]
	mov r8, r1
	ldr r2, _0800E488 @ =0x0000080A
	add r5, r4, r2
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	mov r9, r0
	cmp r0, #0
	beq _0800E48C
	cmp r0, #1
	beq _0800E4EA
	add r0, r7, #0
	mov r1, r8
	bl ClearZoneCardStatusFlags
	add r0, r7, #0
	mov r1, r8
	bl ReturnZoneCardToDeck
	add r0, r7, #0
	mov r1, #0
	mov r2, r8
	bl DuelCursor_Select
	bl DrawAllAreaTiles
	b _0800E4A6
	.align 2, 0
_0800E484: .4byte 0x020185C0
_0800E488: .4byte 0x0000080A
_0800E48C:
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	ldr r1, _0800E4B8 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r1, _0800E4BC @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800E4C4
_0800E4A6:
	ldr r2, _0800E4C0 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0800E5F4
	.align 2, 0
_0800E4B8: .4byte 0x00000D64
_0800E4BC: .4byte 0x0201930C
_0800E4C0: .4byte 0x0000080D
_0800E4C4:
	mov r0, r8
	bl GetZoneArea
	add r1, r0, #0
	add r0, r7, #0
	bl DuelScreen_ScrollToZone
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	b _0800E5F4
_0800E4EA:
	ldr r0, _0800E604 @ =0x00000814
	add r0, r0, r4
	mov sl, r0
	add r0, r7, #0
	mov r1, r9
	and r0, r1
	ldr r1, _0800E608 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r6, _0800E60C @ =0x0201930C
	add r1, r5, r6
	mov r0, #0x94
	mov r4, r8
	mul r4, r0
	add r1, r1, r4
	mov r0, sl
	bl CopyDuelCard
	add r4, r4, r5
	add r4, r4, r6
	mov r0, #5
	neg r0, r0
	ldrb r2, [r4, #2]
	and r0, r2
	strb r0, [r4, #2]
	add r0, r7, #0
	mov r1, r8
	bl ClearZoneTiles
	mov r0, sl
	ldr r5, [r0]
	lsl r0, r5, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0800E610 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _0800E614 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0800E5D8
	mov r0, r9
	and r7, r0
	mov r3, #2
	neg r3, r3
	ldr r2, [sp, #0]
	and r2, r3
	orr r2, r7
	mov r1, #0x1F
	neg r1, r1
	and r2, r1
	ldr r0, _0800E618 @ =0x000001FF
	mov r1, r8
	and r1, r0
	lsl r0, r1, #5
	ldr r7, _0800E61C @ =0xFFFFC01F
	and r2, r7
	orr r2, r0
	str r2, [sp, #0]
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, r9
	and r0, r1
	lsl r0, r0, #0xE
	ldr r6, _0800E620 @ =0xFFFFBFFF
	add r1, r6, #0
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	mov r2, r9
	and r0, r2
	lsl r0, r0, #0xF
	ldr r2, _0800E624 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	lsl r1, r5, #0x13
	lsr r1, r1, #0x1F
	mov r0, r9
	and r1, r0
	ldr r0, [sp, #4]
	and r0, r3
	orr r0, r1
	str r0, [sp, #4]
	mov r1, sl
	ldrh r1, [r1]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	bl IsFusionMonster
	mov r1, #0xD
	cmp r0, #0
	beq _0800E5B0
	mov r1, #0xC
_0800E5B0:
	lsl r1, r1, #1
	ldr r0, [sp, #4]
	mov r2, #0x1F
	neg r2, r2
	and r0, r2
	orr r0, r1
	and r0, r7
	and r0, r6
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	mov r1, sl
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
_0800E5D8:
	ldr r2, _0800E628 @ =0x020185C0
	ldr r0, _0800E62C @ =0x0000080A
	add r2, r2, r0
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
_0800E5F4:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800E604: .4byte 0x00000814
_0800E608: .4byte 0x00000D64
_0800E60C: .4byte 0x0201930C
_0800E610: .4byte gCardIdToNumber
_0800E614: .4byte 0xFFFFF880
_0800E618: .4byte 0x000001FF
_0800E61C: .4byte 0xFFFFC01F
_0800E620: .4byte 0xFFFFBFFF
_0800E624: .4byte 0xFFFF7FFF
_0800E628: .4byte 0x020185C0
_0800E62C: .4byte 0x0000080A
	thumb_func_end DuelCmd_ReturnToDeck

