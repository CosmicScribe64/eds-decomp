	thumb_func_start DuelAnim_UpdateMoveCard
DuelAnim_UpdateMoveCard: @ 0x0805D848
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r6, _0805D880 @ =0x0201D7F0
	add r0, r6, #4
	mov r9, r0
	sub r0, #0xC
	ldrb r0, [r0]
	cmp r0, #0
	bne _0805D88A
	mov r1, #0x1E
	add r0, r1, #0
	ldrb r2, [r6]
	and r0, r2
	cmp r0, #0x1A
	bne _0805D884
	add r0, r1, #0
	ldrb r3, [r6, #4]
	and r0, r3
	cmp r0, #0x16
	bne _0805D884
	mov r0, #0xE
	bl PlaySE
	b _0805D88A
_0805D880: .4byte 0x0201D7F0
_0805D884:
	mov r0, #7
	bl PlaySE
_0805D88A:
	ldr r0, _0805D960 @ =0x0201CFB0
	ldr r1, _0805D964 @ =0x00000838
	add r1, r1, r0
	mov r8, r1
	ldrb r2, [r1]
	cmp r2, #0xF
	bls _0805D89A
	b _0805D9DE
_0805D89A:
	ldr r2, [r6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl GetAreaX
	str r0, [sp, #0]
	ldr r2, [r6]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl GetAreaY
	mov sl, r0
	mov r3, r9
	ldr r2, [r3]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl GetAreaX
	add r4, r0, #0
	mov r0, r9
	ldr r2, [r0]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl GetAreaY
	mov r2, #0x40
	mov r7, #0
	ldr r1, [sp, #0]
	sub r4, r4, r1
	mov r3, sl
	sub r5, r0, r3
	ldr r0, _0805D968 @ =0x081A4454
	mov ip, r0
	mov r1, r8
	ldrb r1, [r1]
	lsl r0, r1, #1
	add r0, ip
	ldrh r0, [r0]
	mul r4, r0
	mul r5, r0
	add r0, r4, #0
	cmp r4, #0
	bge _0805D912
	add r0, #0xFF
_0805D912:
	asr r4, r0, #8
	add r0, r5, #0
	cmp r5, #0
	bge _0805D91C
	add r0, #0xFF
_0805D91C:
	asr r5, r0, #8
	mov r0, #0x80
	mov r3, r9
	ldrb r3, [r3, #1]
	and r0, r3
	cmp r0, #0
	beq _0805D940
	ldr r1, _0805D960 @ =0x0201CFB0
	ldr r2, _0805D96C @ =0x00000834
	add r0, r1, r2
	ldr r0, [r0]
	bl GetCardIconObjTile
	mov r3, #0x80
	lsl r3, r3, #5
	add r0, r0, r3
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
_0805D940:
	mov r0, #0x40
	add r1, r0, #0
	ldrb r6, [r6, #1]
	and r1, r6
	mov r6, r9
	ldrb r6, [r6, #1]
	and r0, r6
	lsl r1, r1, #0x18
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r1, r0
	bne _0805D970
	cmp r3, #0
	beq _0805D986
	mov r7, #0x20
	b _0805D986
_0805D960: .4byte 0x0201CFB0
_0805D964: .4byte 0x00000838
_0805D968: .4byte gDuelAnimLerpWeights
_0805D96C: .4byte 0x00000834
_0805D970:
	cmp r3, #0
	beq _0805D97C
	mov r0, r8
	ldrb r0, [r0]
	lsl r7, r0, #1
	b _0805D986
_0805D97C:
	mov r3, r8
	ldrb r3, [r3]
	lsl r1, r3, #1
	mov r0, #0x20
	sub r7, r0, r1
_0805D986:
	ldr r6, [sp, #0]
	add r0, r6, r4
	mov r3, sl
	add r1, r3, r5
	lsl r1, r1, #0x10
	orr r0, r1
	mov r6, #0x80
	lsl r6, r6, #3
	add r2, r2, r6
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r3, _0805DA0C @ =0x081A43E4
	ldr r4, _0805DA10 @ =0x0201CFB0
	ldr r1, _0805DA14 @ =0x00000838
	add r5, r4, r1
	ldrb r6, [r5]
	lsl r1, r6, #1
	add r1, r1, r3
	ldrh r1, [r1]
	lsl r3, r1, #0x10
	orr r3, r7
	mov r1, #0x80
	bl AddAffineSprite
	ldrb r3, [r5]
	add r2, r3, #1
	strb r2, [r5]
	ldr r1, _0805DA18 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805D9D2
	mov r0, #1
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _0805D9DE
_0805D9D2:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xB
	bhi _0805D9DE
	add r0, r3, #4
	strb r0, [r5]
_0805D9DE:
	ldr r2, _0805DA10 @ =0x0201CFB0
	ldr r3, _0805DA14 @ =0x00000838
	add r0, r2, r3
	ldrb r0, [r0]
	cmp r0, #0x10
	bne _0805D9FA
	mov r6, #0x83
	lsl r6, r6, #4
	add r1, r2, r6
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0805D9FA:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805DA0C: .4byte gBounceScaleCurve
_0805DA10: .4byte 0x0201CFB0
_0805DA14: .4byte 0x00000838
_0805DA18: .4byte 0x03000040
	thumb_func_end DuelAnim_UpdateMoveCard

