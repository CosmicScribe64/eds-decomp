	thumb_func_start DeckEdit_DrawScrollBar
DeckEdit_DrawScrollBar: @ 0x08065F78
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	str r2, [sp, #0x24]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldrh r3, [r2]
	lsr r3, r3, #8
	mov sl, r3
	ldrh r4, [r2, #2]
	lsr r7, r4, #8
	add r0, #1
	cmp r1, r0
	bne _08065FA6
	add r0, r7, r3
	cmp r0, #0x57
	bgt _08065FA6
	add r7, #1
_08065FA6:
	mov r0, sl
	cmp r0, #0
	beq _08066014
	cmp r1, #3
	bls _08066018
	lsl r5, r0, #3
	mov r1, #0x80
	lsl r1, r1, #1
	sub r1, r1, r5
	mov r0, #0x10
	bl MulFix8
	sub r3, r7, #4
	sub r3, r3, r0
	mov r0, #0xFF
	and r3, r0
	mov r0, #8
	str r0, [sp, #0]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #7
	str r0, [sp, #0xC]
	mov r1, #0
	str r1, [sp, #0x10]
	mov r0, #0xC0
	lsl r0, r0, #2
	str r0, [sp, #0x14]
	str r1, [sp, #0x18]
	str r1, [sp, #0x1C]
	ldr r4, _08066008 @ =0x0201DB20
	str r4, [sp, #0x20]
	mov r0, #0
	mov r1, #0x89
	mov r2, #0xE4
	bl OamListAddSprite
	add r5, #0x10
	ldr r1, _0806600C @ =0x000018B2
	add r0, r4, r1
	strh r5, [r0]
	ldr r2, _08066010 @ =0x000018B0
	add r4, r4, r2
	add r0, r4, #0
	bl ObjAffineApply
	b _08066088
	.align 2, 0
_08066008: .4byte 0x0201DB20
_0806600C: .4byte 0x000018B2
_08066010: .4byte 0x000018B0
_08066014:
	cmp r1, #3
	bhi _08066088
_08066018:
	mov r3, #8
	str r3, [sp, #0]
	mov r7, #0x20
	str r7, [sp, #4]
	mov r4, #4
	mov r9, r4
	str r4, [sp, #8]
	mov r0, #7
	mov r8, r0
	str r0, [sp, #0xC]
	mov r4, #0
	str r4, [sp, #0x10]
	mov r6, #0xC0
	lsl r6, r6, #2
	str r6, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1C]
	ldr r5, _0806614C @ =0x0201DB20
	str r5, [sp, #0x20]
	mov r0, #0
	mov r1, #0x89
	mov r2, #0xE4
	mov r3, #0xC
	bl OamListAddSprite
	mov r1, #8
	str r1, [sp, #0]
	str r7, [sp, #4]
	mov r2, r9
	str r2, [sp, #8]
	mov r3, r8
	str r3, [sp, #0xC]
	str r4, [sp, #0x10]
	str r6, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1C]
	str r5, [sp, #0x20]
	mov r0, #0
	mov r1, #0x89
	mov r2, #0xE4
	mov r3, #0x24
	bl OamListAddSprite
	ldr r4, _08066150 @ =0x000018B2
	add r1, r5, r4
	mov r0, #0x80
	lsl r0, r0, #2
	strh r0, [r1]
	ldr r0, _08066154 @ =0x000018B0
	add r5, r5, r0
	add r0, r5, #0
	bl ObjAffineApply
	mov r7, #0
	mov r1, #0x58
	mov sl, r1
_08066088:
	add r3, r7, #0
	add r3, #8
	mov r2, #0xFF
	and r3, r2
	mov r5, #8
	str r5, [sp, #0]
	str r5, [sp, #4]
	mov r4, #4
	mov r9, r4
	str r4, [sp, #8]
	mov r0, #7
	mov r8, r0
	str r0, [sp, #0xC]
	mov r4, #0
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1C]
	ldr r6, _0806614C @ =0x0201DB20
	str r6, [sp, #0x20]
	mov r0, #0
	mov r1, #0xF
	mov r2, #0xE8
	bl OamListAddSprite
	add r3, r7, #0
	add r3, #0xC
	add r3, sl
	mov r1, #0xFF
	and r3, r1
	str r5, [sp, #0]
	str r5, [sp, #4]
	mov r2, r9
	str r2, [sp, #8]
	mov r0, r8
	str r0, [sp, #0xC]
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1C]
	str r6, [sp, #0x20]
	mov r0, #0
	mov r1, #0x2F
	mov r2, #0xE8
	bl OamListAddSprite
	ldr r2, [sp, #0x24]
	ldrb r1, [r2, #4]
	mov r3, #1
	add r0, r3, #0
	and r0, r1
	cmp r0, #0
	beq _08066110
	mov r0, #2
	neg r0, r0
	and r0, r1
	strb r0, [r2, #4]
	ldr r2, _08066158 @ =0x0600E03A
	ldr r1, _0806615C @ =0x08087450
	ldr r4, [sp, #0x24]
	ldrb r4, [r4, #5]
	add r1, r4, r1
	mov r4, #0xA0
	lsl r4, r4, #7
	add r0, r4, #0
	ldrb r1, [r1]
	orr r0, r1
	strh r0, [r2]
_08066110:
	ldr r0, [sp, #0x24]
	ldrb r1, [r0, #6]
	add r0, r3, #0
	and r0, r1
	cmp r0, #0
	beq _0806613C
	mov r0, #2
	neg r0, r0
	and r0, r1
	ldr r1, [sp, #0x24]
	strb r0, [r1, #6]
	ldr r2, _08066160 @ =0x0600E37A
	ldr r0, _0806615C @ =0x08087450
	ldrb r1, [r1, #7]
	add r1, #3
	add r1, r1, r0
	mov r3, #0xA0
	lsl r3, r3, #7
	add r0, r3, #0
	ldrb r1, [r1]
	orr r0, r1
	strh r0, [r2]
_0806613C:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0806614C: .4byte 0x0201DB20
_08066150: .4byte 0x000018B2
_08066154: .4byte 0x000018B0
_08066158: .4byte 0x0600E03A
_0806615C: .4byte gScrollArrowTiles
_08066160: .4byte 0x0600E37A
	thumb_func_end DeckEdit_DrawScrollBar

