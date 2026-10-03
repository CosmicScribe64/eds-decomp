	thumb_func_start Bustup_DrawNumber
Bustup_DrawNumber: @ 0x08000324
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	ldr r4, [sp, #0x50]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0x24]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x28]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov sl, r4
	mov r6, #0
	cmp r6, r9
	bcs _08000408
	mov r0, r9
	sub r0, #1
	lsl r0, r0, #3
	str r0, [sp, #0x2C]
_0800035E:
	mov r0, r8
	mov r1, #0xA
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r7, r5, #0
	mov r0, r8
	mov r1, #0xA
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	cmp r6, #0
	bne _080003BC
	lsl r0, r5, #1
	ldr r1, _080003B4 @ =0x08080AC0
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #8
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r3, sl
	str r3, [sp, #0xC]
	str r6, [sp, #0x10]
	str r6, [sp, #0x14]
	str r6, [sp, #0x18]
	str r6, [sp, #0x1C]
	ldr r0, _080003B8 @ =0x02014888
	str r0, [sp, #0x20]
	mov r0, #0
	ldr r4, [sp, #0x24]
	ldr r3, [sp, #0x2C]
	add r2, r4, r3
	ldr r3, [sp, #0x28]
	bl OamListAddSprite
	mov r4, #1
	b _08000400
	.align 2, 0
_080003B4: .4byte gBustupDigitTiles
_080003B8: .4byte 0x02014888
_080003BC:
	mov r4, r8
	cmp r4, #0
	bne _080003C8
	add r4, r6, #1
	cmp r5, #0
	beq _08000400
_080003C8:
	lsl r0, r7, #1
	ldr r1, _08000418 @ =0x08080AC0
	add r0, r0, r1
	ldrh r1, [r0]
	add r4, r6, #1
	mov r3, r9
	sub r2, r3, r4
	lsl r2, r2, #3
	ldr r0, [sp, #0x24]
	add r2, r0, r2
	mov r0, #8
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r3, sl
	str r3, [sp, #0xC]
	mov r0, #0
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _0800041C @ =0x02014888
	str r0, [sp, #0x20]
	mov r0, #0
	ldr r3, [sp, #0x28]
	bl OamListAddSprite
_08000400:
	lsl r0, r4, #0x18
	lsr r6, r0, #0x18
	cmp r6, r9
	bcc _0800035E
_08000408:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08000418: .4byte gBustupDigitTiles
_0800041C: .4byte 0x02014888
	thumb_func_end Bustup_DrawNumber

