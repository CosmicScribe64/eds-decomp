	thumb_func_start EffectMorphingJar2Resolve
EffectMorphingJar2Resolve: @ 0x08037ED8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _08037EF2
	b _08038208
_08037EF2:
	ldr r1, _08037F10 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	add r2, r1, #0
	cmp r0, #4
	bls _08037F06
	b _08038208
_08037F06:
	lsl r0, r0, #2
	ldr r1, _08037F14 @ =0x08037F18
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08037F10: .4byte 0x02017A40
_08037F14: .4byte 0x08037F18
_08037F18:
	.4byte _080381EC
	.4byte _080381D8
	.4byte _08038060
	.4byte _08037FC8
	.4byte _08037F2C
_08037F2C:
	mov r2, #0
	ldr r3, _08037FB0 @ =0x0201930C
	mov ip, r3
	mov r0, #0xC
	add r0, r0, r6
	mov r8, r0
	mov r1, #1
	mov sl, r1
	ldr r3, _08037FB4 @ =0x00000D64
	mov r9, r3
	ldr r7, _08037FB8 @ =0x000007FF
_08037F42:
	lsl r1, r2, #1
	add r1, r8
	mov r0, #0
	strh r0, [r1]
	mov r3, #0
	add r5, r2, #1
	mov r0, sl
	and r2, r0
	mov r4, r9
	mul r4, r2
	add r2, r1, #0
_08037F58:
	mov r0, #0x94
	mul r0, r3
	add r0, r0, r4
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08037F86
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08037FBC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08037F86
	ldrh r0, [r2]
	add r0, #1
	strh r0, [r2]
_08037F86:
	add r3, #1
	cmp r3, #4
	ble _08037F58
	add r2, r5, #0
	cmp r2, #1
	ble _08037F42
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	bl ReturnAllMonstersToDeck
	ldr r1, _08037FC0 @ =0x02017A40
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08037FC4 @ =0x000003E1
	add r1, r1, r3
	strb r0, [r1]
_08037FAC:
	mov r0, #0x7F
	b _0803820A
_08037FB0: .4byte 0x0201930C
_08037FB4: .4byte 0x00000D64
_08037FB8: .4byte 0x000007FF
_08037FBC: .4byte gCardStats
_08037FC0: .4byte 0x02017A40
_08037FC4: .4byte 0x000003E1
_08037FC8:
	ldr r7, _0803802C @ =0x020192E4
	ldr r0, _08038030 @ =0x000003E1
	add r5, r2, r0
	ldrb r3, [r5]
	mov r1, #1
	add r0, r3, #0
	and r0, r1
	ldr r1, _08038034 @ =0x00000D64
	add r4, r0, #0
	mul r4, r1
	add r0, r4, r7
	ldrb r0, [r0, #3]
	cmp r0, #0
	beq _08038040
	lsl r0, r3, #1
	add r1, r6, #0
	add r1, #0xC
	add r1, r1, r0
	ldrh r0, [r1]
	cmp r0, #0
	beq _08038040
	ldr r1, _08038038 @ =0x000007C4
	add r0, r7, r1
	add r4, r4, r0
	mov r3, #0xFA
	lsl r3, r3, #2
	add r0, r2, r3
	add r1, r4, #0
	bl CopyDuelCard
	ldrb r0, [r5]
	mov r1, #0x61
	cmp r0, #0
	beq _0803800E
	ldr r1, _0803803C @ =0x00008061
_0803800E:
	add r0, r1, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r5]
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowRevealedCard
	mov r0, #0x7E
	b _0803820A
	.align 2, 0
_0803802C: .4byte 0x020192E4
_08038030: .4byte 0x000003E1
_08038034: .4byte 0x00000D64
_08038038: .4byte 0x000007C4
_0803803C: .4byte 0x00008061
_08038040:
	ldr r1, _0803805C @ =0x000003E1
	add r0, r2, r1
	mov r1, #1
	ldrb r2, [r0]
	sub r1, r1, r2
	strb r1, [r0]
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r1, r1, #0x1F
	ldrb r0, [r0]
	cmp r0, r1
	bne _08037FAC
	mov r0, #0x78
	b _0803820A
_0803805C: .4byte 0x000003E1
_08038060:
	ldr r1, _080380B0 @ =0x02017E28
	ldr r3, [r1]
	lsl r0, r3, #0x13
	lsr r0, r0, #0x1F
	sub r5, r1, #7
	ldrb r4, [r5]
	add r7, r1, #0
	cmp r0, r4
	beq _080380E8
	lsl r0, r3, #0xE
	cmp r0, #0
	bge _080380E8
	lsl r0, r3, #0x15
	lsr r0, r0, #0x14
	ldr r3, _080380B4 @ =0x08622AB4
	add r0, r0, r3
	ldr r1, _080380B8 @ =0x000002FA
	ldrh r0, [r0]
	cmp r0, r1
	bne _080380E8
	mov r0, #1
	sub r0, r0, r4
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _080380C0
	ldrb r0, [r5]
	mov r3, #0xC2
	cmp r0, #0
	beq _0803809E
	ldr r3, _080380BC @ =0x000080C2
_0803809E:
	ldrh r1, [r7]
	ldrh r2, [r7, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7D
	b _0803820A
	.align 2, 0
_080380B0: .4byte 0x02017E28
_080380B4: .4byte gCardIdToNumber
_080380B8: .4byte 0x000002FA
_080380BC: .4byte 0x000080C2
_080380C0:
	ldrb r0, [r5]
	ldr r3, _080380E0 @ =0x020192E4
	mov r2, #1
	add r1, r0, #0
	and r1, r2
	ldr r2, _080380E4 @ =0x00000D64
	mul r1, r2
	add r1, r1, r3
	ldrb r1, [r1, #2]
	sub r1, #1
	mov r2, #0
	mov r3, #1
	bl DiscardHandCard
	b _08037FAC
	.align 2, 0
_080380E0: .4byte 0x020192E4
_080380E4: .4byte 0x00000D64
_080380E8:
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldr r0, _08038114 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08038118 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0803819A
	cmp r0, #0x15
	blt _08038124
	cmp r0, #0x17
	ble _0803811C
	cmp r0, #0x18
	beq _08038120
	b _08038124
_08038114: .4byte 0x000007FF
_08038118: .4byte gCardStats
_0803811C:
	mov r0, #0
	b _08038138
_08038120:
	mov r0, #0xA
	b _08038138
_08038124:
	ldr r0, _08038174 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r2, _08038178 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08038138:
	cmp r0, #4
	bhi _08038184
	add r0, r3, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08038184
	ldr r2, _0803817C @ =0x02017E28
	sub r4, r2, #7
	ldrb r0, [r4]
	mov r3, #0xC2
	cmp r0, #0
	beq _08038154
	ldr r3, _08038180 @ =0x000080C2
_08038154:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r4, [r4]
	lsl r1, r4, #1
	add r0, r6, #0
	add r0, #0xC
	add r0, r0, r1
	ldrh r1, [r0]
	sub r1, #1
	strh r1, [r0]
	mov r0, #0x7C
	b _0803820A
_08038174: .4byte 0x000007FF
_08038178: .4byte gCardStats
_0803817C: .4byte 0x02017E28
_08038180: .4byte 0x000080C2
_08038184:
	ldr r2, _080381C8 @ =0x02017A40
	ldr r3, _080381CC @ =0x000003E1
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r1, r0, #1
	add r0, r6, #0
	add r0, #0xC
	add r0, r0, r1
	ldrh r1, [r0]
	sub r1, #1
	strh r1, [r0]
_0803819A:
	ldr r1, _080381CC @ =0x000003E1
	add r0, r2, r1
	ldrb r0, [r0]
	ldr r3, _080381D0 @ =0x020192E4
	mov r2, #1
	add r1, r0, #0
	and r1, r2
	ldr r2, _080381D4 @ =0x00000D64
	mul r1, r2
	add r1, r1, r3
	ldrb r1, [r1, #2]
	sub r1, #1
	ldrb r6, [r6, #2]
	lsl r3, r6, #0x1F
	lsr r3, r3, #0x1F
	eor r3, r0
	neg r2, r3
	orr r2, r3
	lsr r2, r2, #0x1F
	mov r3, #1
	bl DiscardHandCard
	b _08037FAC
_080381C8: .4byte 0x02017A40
_080381CC: .4byte 0x000003E1
_080381D0: .4byte 0x020192E4
_080381D4: .4byte 0x00000D64
_080381D8:
	ldr r3, _080381E8 @ =0x000003E1
	add r1, r2, r3
	mov r0, #1
	ldrb r1, [r1]
	sub r0, r0, r1
	add r3, #7
	b _080381F6
	.align 2, 0
_080381E8: .4byte 0x000003E1
_080381EC:
	ldr r1, _08038204 @ =0x000003E1
	add r0, r2, r1
	ldrb r0, [r0]
	mov r3, #0xFA
	lsl r3, r3, #2
_080381F6:
	add r1, r2, r3
	mov r2, #0
	str r2, [sp, #0]
	mov r3, #1
	bl QueueSpecialSummon
	b _08037FAC
_08038204: .4byte 0x000003E1
_08038208:
	mov r0, #0
_0803820A:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectMorphingJar2Resolve
	.align 2, 0

