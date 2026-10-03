	thumb_func_start DuelCmd_DirectAttack
DuelCmd_DirectAttack: @ 0x0800D234
	push {r4, r5, r6, r7, lr}
	ldr r1, _0800D258 @ =0x020185C0
	ldrh r0, [r1]
	lsr r3, r0, #0xF
	ldrh r2, [r1, #2]
	ldr r4, _0800D25C @ =0x0000080A
	add r5, r1, r4
	ldrb r4, [r5]
	lsl r0, r4, #0x19
	lsr r6, r0, #0x19
	add r4, r1, #0
	cmp r6, #1
	beq _0800D288
	cmp r6, #1
	bgt _0800D260
	cmp r6, #0
	beq _0800D266
	b _0800D380
_0800D258: .4byte 0x020185C0
_0800D25C: .4byte 0x0000080A
_0800D260:
	cmp r6, #2
	beq _0800D2F4
	b _0800D380
_0800D266:
	ldr r1, _0800D280 @ =0x0201CFB0
	ldr r0, _0800D284 @ =0x00000808
	add r1, r1, r0
	mov r0, #8
	ldrb r4, [r1]
	orr r0, r4
	strb r0, [r1]
	add r0, r3, #0
	mov r1, #0
	bl DuelCursor_Select
	b _0800D2BA
	.align 2, 0
_0800D280: .4byte 0x0201CFB0
_0800D284: .4byte 0x00000808
_0800D288:
	ldr r1, _0800D2D4 @ =0x0201CFB0
	ldr r0, _0800D2D8 @ =0x00000808
	add r1, r1, r0
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _0800D2DC @ =0x050003E0
	ldr r1, _0800D2E0 @ =0x08687B9C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0800D2E4 @ =0x06016C80
	ldr r1, _0800D2E8 @ =0x086883BC
	mov r2, #0x80
	lsl r2, r2, #3
	bl CopyDoubleWords
	ldr r0, _0800D2EC @ =0x0000080C
	add r1, r4, r0
	ldr r0, _0800D2F0 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
_0800D2BA:
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
	b _0800D38E
	.align 2, 0
_0800D2D4: .4byte 0x0201CFB0
_0800D2D8: .4byte 0x00000808
_0800D2DC: .4byte 0x050003E0
_0800D2E0: .4byte gDuelBannerPal
_0800D2E4: .4byte 0x06016C80
_0800D2E8: .4byte gDirectAttackBannerGfx
_0800D2EC: .4byte 0x0000080C
_0800D2F0: .4byte 0xFFFFF01F
_0800D2F4:
	ldr r0, _0800D360 @ =0x0000080C
	add r7, r4, r0
	ldrh r1, [r7]
	lsl r0, r1, #0x14
	lsr r5, r0, #0x19
	cmp r5, #0x5F
	bgt _0800D380
	ldr r0, _0800D364 @ =0x00300058
	ldr r1, _0800D368 @ =0x000040C0
	ldr r2, _0800D36C @ =0x0000F364
	ldr r4, _0800D370 @ =0x081A43E4
	mov r3, #0x1F
	and r5, r3
	lsl r3, r5, #1
	add r3, r3, r4
	ldrh r3, [r3]
	lsl r3, r3, #0x10
	bl AddAffineSprite
	ldrh r1, [r7]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r4, #0x7F
	and r0, r4
	lsl r0, r0, #5
	ldr r3, _0800D374 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r7]
	ldr r0, _0800D378 @ =0x03000040
	ldrh r0, [r0, #4]
	and r6, r0
	cmp r6, #0
	bne _0800D348
	ldr r1, _0800D37C @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800D38E
_0800D348:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x57
	bgt _0800D38E
	add r0, #7
	and r0, r4
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r7]
	b _0800D38E
	.align 2, 0
_0800D360: .4byte 0x0000080C
_0800D364: .4byte 0x00300058
_0800D368: .4byte 0x000040C0
_0800D36C: .4byte 0x0000F364
_0800D370: .4byte gBounceScaleCurve
_0800D374: .4byte 0xFFFFF01F
_0800D378: .4byte 0x03000040
_0800D37C: .4byte 0x0201CFB0
_0800D380:
	ldr r2, _0800D394 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_0800D38E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800D394: .4byte 0x0000080D
	thumb_func_end DuelCmd_DirectAttack

