	thumb_func_start EffectOpponentGraveSummonResolve
EffectOpponentGraveSummonResolve: @ 0x0803BED0
	push {r4, r5, r6, lr}
	sub sp, #4
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _0803BEE2
	b _0803C054
_0803BEE2:
	ldr r0, _0803BEFC @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803BF5C
	cmp r0, #0x7F
	bgt _0803BF00
	cmp r0, #0x7E
	beq _0803BF88
	b _0803C054
	.align 2, 0
_0803BEFC: .4byte 0x02017A40
_0803BF00:
	cmp r0, #0x80
	beq _0803BF06
	b _0803C054
_0803BF06:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r5, #1
	sub r0, r5, r0
	ldr r1, _0803BF58 @ =0x00000447
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bne _0803BF1E
	b _0803C054
_0803BF1E:
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _0803BF30
	b _0803C054
_0803BF30:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	bl CanSpecialSummon
	cmp r0, #0
	bne _0803BF42
	b _0803C054
_0803BF42:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	mov r1, #0x13
	mov r2, #0
	mov r3, #0
	bl DuelPrompt_Post
	mov r0, #0x7F
	b _0803C056
_0803BF58: .4byte 0x00000447
_0803BF5C:
	ldr r0, _0803BF80 @ =0x020192E0
	ldr r2, _0803BF84 @ =0x00001B64
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, #0
	beq _0803C054
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldrh r2, [r4]
	mov r1, #0xE
	mov r3, #0
	bl DuelPrompt_Post
	mov r0, #0x7E
	b _0803C056
_0803BF80: .4byte 0x020192E0
_0803BF84: .4byte 0x00001B64
_0803BF88:
	ldr r3, _0803C040 @ =0x020192E0
	ldr r2, _0803C044 @ =0x00001B64
	add r1, r3, r2
	add r2, #2
	add r0, r3, r2
	ldrh r0, [r0]
	lsl r2, r0, #0x10
	ldrh r1, [r1]
	orr r2, r1
	str r2, [sp, #0]
	mov r5, sp
	ldr r1, _0803C048 @ =0x02015EE8
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0803BFD2
	ldr r0, _0803C04C @ =0x00001B12
	add r1, r3, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0803BFD2
	lsl r1, r2, #0x13
	lsr r1, r1, #0x1F
	sub r1, r6, r1
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	ldrb r2, [r5, #1]
	mov r0, #0x11
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #1]
_0803BFD2:
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r3, #0xD3
	cmp r0, #0
	bne _0803BFE2
	ldr r3, _0803C050 @ =0x000080D3
_0803BFE2:
	ldr r2, [sp, #0]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, _0803C048 @ =0x02015EE8
	add r0, r6, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0803C028
	ldr r1, _0803C040 @ =0x020192E0
	ldr r2, _0803C04C @ =0x00001B12
	add r1, r1, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0803C028
	ldr r1, [r5]
	lsl r1, r1, #0x13
	lsr r1, r1, #0x1F
	sub r1, r6, r1
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	ldrb r2, [r5, #1]
	mov r0, #0x11
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #1]
_0803C028:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, sp
	mov r2, #1
	mov r3, #0x20
	bl QueueSpecialSummonChoosePosition
	mov r0, #0x7D
	b _0803C056
_0803C040: .4byte 0x020192E0
_0803C044: .4byte 0x00001B64
_0803C048: .4byte 0x02015EE8
_0803C04C: .4byte 0x00001B12
_0803C050: .4byte 0x000080D3
_0803C054:
	mov r0, #0
_0803C056:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectOpponentGraveSummonResolve
	.align 2, 0

