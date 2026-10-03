	thumb_func_start EffectSplitFusionResolve
EffectSplitFusionResolve: @ 0x0803C838
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	ldrb r7, [r5, #0xC]
	ldrh r0, [r5, #0xC]
	lsr r0, r0, #8
	mov r8, r0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _0803C854
	b _0803C9F0
_0803C854:
	ldr r1, _0803C874 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	add r4, r1, #0
	cmp r0, #4
	bls _0803C868
	b _0803C9F0
_0803C868:
	lsl r0, r0, #2
	ldr r1, _0803C878 @ =0x0803C87C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803C874: .4byte 0x02017A40
_0803C878: .4byte 0x0803C87C
_0803C87C:
	.4byte _0803C9B8
	.4byte _0803C978
	.4byte _0803C944
	.4byte _0803C91C
	.4byte _0803C890
_0803C890:
	mov r6, #7
	ldrb r3, [r5, #0xA]
	and r6, r3
	cmp r6, #1
	beq _0803C89C
	b _0803C9F0
_0803C89C:
	ldrh r1, [r5, #0xC]
	add r0, r5, #0
	bl EffectFaceUpFusionMonsterCheck
	cmp r0, #0
	bne _0803C8AA
	b _0803C9F0
_0803C8AA:
	add r0, r7, #0
	mov r1, r8
	bl ReturnFieldCardToDeck
	ldr r4, _0803C908 @ =0x02017A40
	and r7, r6
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	ldr r1, _0803C90C @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r1, _0803C910 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r2, _0803C914 @ =0x00000542
	add r4, r4, r2
	strh r0, [r4]
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	add r7, r0, #0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803C918 @ =0x0000060A
	ldrh r2, [r4]
	bl CollectEffectTargets
	cmp r0, #0
	bne _0803C8F4
	b _0803C9F0
_0803C8F4:
	cmp r7, r0
	bge _0803C8FA
	b _0803C9F0
_0803C8FA:
	ldrb r5, [r5, #2]
	and r6, r5
	cmp r6, #0
	bne _0803C9F0
	mov r0, #0x7F
	b _0803C9F2
	.align 2, 0
_0803C908: .4byte 0x02017A40
_0803C90C: .4byte 0x00000D64
_0803C910: .4byte 0x0201930C
_0803C914: .4byte 0x00000542
_0803C918: .4byte 0x0000060A
_0803C91C:
	ldr r0, _0803C938 @ =0x00000206
	ldr r1, _0803C93C @ =0x00000613
	ldr r3, _0803C940 @ =0x08083AD0
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7E
	b _0803C9F2
	.align 2, 0
_0803C938: .4byte 0x00000206
_0803C93C: .4byte 0x00000613
_0803C940: .4byte gStrSummonFusionMaterialsQuestion
_0803C944:
	ldr r0, _0803C968 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0803C9F0
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803C96C @ =0x0000060A
	ldr r3, _0803C970 @ =0x00000542
	add r2, r4, r3
	ldrh r2, [r2]
	bl CollectEffectTargets
	ldr r2, _0803C974 @ =0x000003E1
	add r1, r4, r2
	strb r0, [r1]
	mov r0, #0x7D
	b _0803C9F2
_0803C968: .4byte 0x0201AE60
_0803C96C: .4byte 0x0000060A
_0803C970: .4byte 0x00000542
_0803C974: .4byte 0x000003E1
_0803C978:
	ldr r3, _0803C9AC @ =0x000003E1
	add r1, r4, r3
	ldrb r0, [r1]
	cmp r0, #0
	beq _0803C9F0
	sub r0, #1
	strb r0, [r1]
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	mov r3, #0xD3
	cmp r0, #0
	beq _0803C994
	ldr r3, _0803C9B0 @ =0x000080D3
_0803C994:
	ldrb r1, [r1]
	lsl r0, r1, #2
	ldr r1, _0803C9B4 @ =0x0201D81C
	add r0, r0, r1
	ldrh r1, [r0]
	ldrh r2, [r0, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7C
	b _0803C9F2
_0803C9AC: .4byte 0x000003E1
_0803C9B0: .4byte 0x000080D3
_0803C9B4: .4byte 0x0201D81C
_0803C9B8:
	ldr r2, _0803C9E8 @ =0x0201D810
	ldr r0, _0803C9EC @ =0x000003E1
	add r3, r4, r0
	ldrb r4, [r3]
	lsl r1, r4, #2
	add r1, r1, r2
	mov r0, #0x11
	neg r0, r0
	ldrb r4, [r1, #0xE]
	and r0, r4
	strb r0, [r1, #0xE]
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldrb r3, [r3]
	lsl r1, r3, #2
	add r2, #0xC
	add r1, r1, r2
	mov r2, #1
	mov r3, #0x20
	bl QueueSpecialSummonChoosePosition
	mov r0, #0x7D
	b _0803C9F2
_0803C9E8: .4byte 0x0201D810
_0803C9EC: .4byte 0x000003E1
_0803C9F0:
	mov r0, #0
_0803C9F2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectSplitFusionResolve

