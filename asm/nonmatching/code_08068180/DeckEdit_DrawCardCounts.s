	thumb_func_start DeckEdit_DrawCardCounts
DeckEdit_DrawCardCounts: @ 0x08068180
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	ldr r0, _080682F0 @ =0x0201DB20
	mov r9, r0
	ldr r1, _080682F4 @ =0x0201EFC0
	add r1, r1, r6
	mov r8, r1
	ldrb r1, [r1]
	lsl r5, r6, #1
	mov r0, #0xC4
	lsl r0, r0, #3
	add r0, r9
	add r5, r5, r0
	ldrh r2, [r5]
	add r0, r6, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	ldr r2, _080682F8 @ =0x02011C20
	add r0, r0, r2
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x16
	mov r3, #0x88
	str r3, [sp, #0]
	ldr r1, _080682FC @ =0x081A6EB4
	mov sl, r1
	str r1, [sp, #4]
	mov r2, #1
	str r2, [sp, #8]
	mov r3, #8
	str r3, [sp, #0xC]
	mov r7, #0
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r1, r9
	str r1, [sp, #0x1C]
	mov r1, #2
	mov r3, #0x30
	bl DrawNumberSprites
	mov r2, r8
	ldrb r1, [r2]
	ldrh r2, [r5]
	add r0, r6, #0
	bl DeckEdit_GetListCard
	add r4, r0, #0
	mov r3, r8
	ldrb r1, [r3]
	ldrh r2, [r5]
	add r0, r6, #0
	bl DeckEdit_GetListCard
	lsl r4, r4, #0x10
	lsr r4, r4, #0xE
	ldr r1, _080682F8 @ =0x02011C20
	add r4, r4, r1
	ldrb r4, [r4, #9]
	lsl r1, r4, #0x1C
	lsr r1, r1, #0x1E
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	ldr r2, _080682F8 @ =0x02011C20
	add r0, r0, r2
	ldrb r0, [r0, #9]
	lsr r0, r0, #6
	add r1, r1, r0
	mov r4, #0x77
	str r4, [sp, #0]
	mov r3, sl
	str r3, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r2, #8
	str r2, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r3, r9
	str r3, [sp, #0x1C]
	add r0, r1, #0
	mov r1, #2
	mov r2, #1
	mov r3, #0x68
	bl DrawNumberSprites
	ldr r1, _08068300 @ =0x02013CEC
	ldrh r2, [r1]
	ldr r1, _08068304 @ =0x02013CE8
	ldrh r1, [r1]
	add r0, r2, r1
	mov r2, #0x88
	str r2, [sp, #0]
	mov r3, sl
	str r3, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r2, #8
	str r2, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r3, r9
	str r3, [sp, #0x1C]
	mov r1, #2
	mov r2, #1
	mov r3, #0x68
	bl DrawNumberSprites
	mov r0, r8
	ldrb r1, [r0]
	ldrh r2, [r5]
	add r0, r6, #0
	bl DeckEdit_GetListCard
	mov r2, r8
	ldrb r1, [r2]
	ldrh r2, [r5]
	add r0, r6, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	ldr r3, _080682F8 @ =0x02011C20
	add r0, r0, r3
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	str r4, [sp, #0]
	mov r1, sl
	str r1, [sp, #4]
	mov r2, #1
	str r2, [sp, #8]
	mov r3, #8
	str r3, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r1, r9
	str r1, [sp, #0x1C]
	mov r1, #2
	mov r3, #0xA8
	bl DrawNumberSprites
	ldr r2, _080682F8 @ =0x02011C20
	ldr r3, _08068308 @ =0x000020CA
	add r2, r2, r3
	mov r8, r2
	ldrh r0, [r2]
	mov r1, #0x88
	str r1, [sp, #0]
	mov r2, sl
	str r2, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r1, #8
	str r1, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r2, r9
	str r2, [sp, #0x1C]
	mov r1, #2
	mov r2, #1
	mov r3, #0xA8
	bl DrawNumberSprites
	cmp r6, #1
	beq _0806835C
	cmp r6, #1
	bgt _0806830C
	cmp r6, #0
	beq _08068312
	b _0806841A
	.align 2, 0
_080682F0: .4byte 0x0201DB20
_080682F4: .4byte 0x0201EFC0
_080682F8: .4byte 0x02011C20
_080682FC: .4byte gDeckEditDigitSprites
_08068300: .4byte 0x02013CEC
_08068304: .4byte 0x02013CE8
_08068308: .4byte 0x000020CA
_0806830C:
	cmp r6, #2
	beq _080683B4
	b _0806841A
_08068312:
	ldr r3, _0806834C @ =0x02011C20
	ldr r1, _08068350 @ =0x000020C6
	add r0, r3, r1
	ldr r2, _08068354 @ =0x02013CE8
	ldrh r2, [r2]
	ldrh r0, [r0]
	add r0, r2, r0
	mov r3, r8
	ldrh r3, [r3]
	add r0, r3, r0
	ldr r1, _08068358 @ =0x02013CEC
	ldrh r1, [r1]
	add r0, r1, r0
	mov r2, #0x88
	str r2, [sp, #0]
	mov r3, sl
	str r3, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r2, #8
	str r2, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r3, r9
	str r3, [sp, #0x1C]
	mov r1, #4
	mov r2, #1
	b _080683A4
_0806834C: .4byte 0x02011C20
_08068350: .4byte 0x000020C6
_08068354: .4byte 0x02013CE8
_08068358: .4byte 0x02013CEC
_0806835C:
	ldr r1, _080683AC @ =0x02013CE8
	ldrh r0, [r1]
	mov r1, #0x78
	str r1, [sp, #0]
	mov r2, sl
	str r2, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r1, #8
	str r1, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r2, r9
	str r2, [sp, #0x1C]
	mov r1, #2
	mov r2, #1
	mov r3, #0xE0
	bl DrawNumberSprites
	ldr r3, _080683B0 @ =0x02013CEC
	ldrh r0, [r3]
	mov r1, #0x8B
	str r1, [sp, #0]
	mov r1, sl
	str r1, [sp, #4]
	mov r2, #1
	str r2, [sp, #8]
	mov r3, #8
	str r3, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r1, r9
	str r1, [sp, #0x1C]
	mov r1, #2
_080683A4:
	mov r3, #0xE0
	bl DrawNumberSprites
	b _0806841A
_080683AC: .4byte 0x02013CE8
_080683B0: .4byte 0x02013CEC
_080683B4:
	ldr r5, _0806842C @ =0x000014A2
	add r5, r9
	ldrb r2, [r5]
	lsl r0, r2, #1
	ldr r4, _08068430 @ =0x00001712
	add r4, r9
	add r0, r0, r4
	ldrh r0, [r0]
	mov r1, #0x79
	str r1, [sp, #0]
	mov r3, sl
	str r3, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r2, #8
	str r2, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r3, r9
	str r3, [sp, #0x1C]
	mov r1, #2
	mov r2, #1
	mov r3, #0xE0
	bl DrawNumberSprites
	ldrb r5, [r5]
	lsl r0, r5, #1
	add r0, r0, r4
	mov r1, r8
	ldrh r1, [r1]
	ldrh r0, [r0]
	sub r0, r1, r0
	mov r1, #0x8A
	str r1, [sp, #0]
	mov r2, sl
	str r2, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r1, #8
	str r1, [sp, #0xC]
	str r7, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	mov r2, r9
	str r2, [sp, #0x1C]
	mov r1, #2
	mov r2, #1
	mov r3, #0xE0
	bl DrawNumberSprites
_0806841A:
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0806842C: .4byte 0x000014A2
_08068430: .4byte 0x00001712
	thumb_func_end DeckEdit_DrawCardCounts

