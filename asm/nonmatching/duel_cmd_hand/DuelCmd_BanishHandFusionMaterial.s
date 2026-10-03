	thumb_func_start DuelCmd_BanishHandFusionMaterial
DuelCmd_BanishHandFusionMaterial: @ 0x08011610
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x10
	ldr r4, _0801163C @ =0x020185C0
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	ldrh r3, [r4, #2]
	ldr r1, _08011640 @ =0x0000080A
	add r1, r1, r4
	mov r9, r1
	ldrb r5, [r1]
	lsl r0, r5, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _0801166C
	cmp r7, #1
	bgt _08011644
	cmp r7, #0
	beq _0801164A
	b _0801174C
_0801163C: .4byte 0x020185C0
_08011640: .4byte 0x0000080A
_08011644:
	cmp r7, #2
	beq _0801172C
	b _0801174C
_0801164A:
	add r0, r2, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	mov r0, r9
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r9
	b _0801175E
_0801166C:
	ldr r5, _08011710 @ =0x00000814
	add r5, r5, r4
	mov r8, r5
	add r0, r2, #0
	and r0, r7
	ldr r1, _08011714 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r6, _08011718 @ =0x02019968
	add r1, r5, r6
	lsl r4, r3, #2
	add r1, r1, r4
	mov r0, r8
	str r2, [sp, #8]
	str r3, [sp, #0xC]
	bl CopyDuelCard
	add r4, r4, r5
	add r4, r4, r6
	ldr r0, _0801171C @ =0xFFFFF000
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldr r2, [sp, #8]
	and r2, r7
	mov r6, #2
	neg r6, r6
	ldr r0, [sp, #0]
	and r0, r6
	orr r0, r2
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, #0x16
	orr r0, r1
	ldr r1, _08011720 @ =0x000001FF
	ldr r3, [sp, #0xC]
	and r3, r1
	lsl r1, r3, #5
	ldr r5, _08011724 @ =0xFFFFC01F
	and r0, r5
	orr r0, r1
	ldr r4, _08011728 @ =0xFFFFBFFF
	and r0, r4
	mov r3, #0x80
	lsl r3, r3, #8
	orr r0, r3
	str r0, [sp, #0]
	mov r2, r8
	ldr r0, [r2]
	lsl r2, r0, #0x13
	lsr r2, r2, #0x1F
	and r2, r7
	ldr r1, [sp, #4]
	and r1, r6
	orr r1, r2
	mov r2, #0x1E
	orr r1, r2
	and r1, r5
	and r1, r4
	orr r1, r3
	str r1, [sp, #4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
	mov r5, r9
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
	b _08011760
	.align 2, 0
_08011710: .4byte 0x00000814
_08011714: .4byte 0x00000D64
_08011718: .4byte 0x02019968
_0801171C: .4byte 0xFFFFF000
_08011720: .4byte 0x000001FF
_08011724: .4byte 0xFFFFC01F
_08011728: .4byte 0xFFFFBFFF
_0801172C:
	ldrh r0, [r4, #4]
	cmp r0, #0
	beq _08011738
	add r0, r2, #0
	bl CompactHand
_08011738:
	ldr r1, _08011770 @ =0x00000816
	add r0, r4, r1
	mov r1, #0x10
	ldrb r2, [r0]
	orr r1, r2
	strb r1, [r0]
	ldr r5, _08011774 @ =0x00000814
	add r0, r4, r5
	bl AddCardToBanished
_0801174C:
	bl DrawAllAreaTiles
	ldr r1, _08011778 @ =0x020185C0
	ldr r0, _0801177C @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0801175E:
	strb r0, [r1]
_08011760:
	add sp, #0x10
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08011770: .4byte 0x00000816
_08011774: .4byte 0x00000814
_08011778: .4byte 0x020185C0
_0801177C: .4byte 0x0000080D
	thumb_func_end DuelCmd_BanishHandFusionMaterial

