	thumb_func_start DuelCmd_ExchangeHandCards
DuelCmd_ExchangeHandCards: @ 0x08011278
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r1, _080112A4 @ =0x020185C0
	ldrh r0, [r1]
	lsr r0, r0, #0xF
	mov ip, r0
	ldr r2, _080112A8 @ =0x02018DCA
	ldrb r2, [r2]
	lsl r0, r2, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _080112D8
	cmp r7, #1
	bgt _080112AC
	cmp r7, #0
	beq _080112B2
	b _0801143C
	.align 2, 0
_080112A4: .4byte 0x020185C0
_080112A8: .4byte 0x02018DCA
_080112AC:
	cmp r7, #2
	beq _08011378
	b _0801143C
_080112B2:
	mov r0, ip
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	ldr r3, _080112D4 @ =0x02018DCA
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08011478
_080112D4: .4byte 0x02018DCA
_080112D8:
	mov r1, ip
	and r1, r7
	mov r4, #2
	neg r4, r4
	mov r9, r4
	ldr r0, [sp, #0]
	and r0, r4
	orr r0, r1
	mov r5, #0x1F
	neg r5, r5
	mov r8, r5
	and r0, r5
	mov r6, #0x16
	orr r0, r6
	ldr r2, _08011360 @ =0x000001FF
	add r1, r2, #0
	ldr r3, _08011364 @ =0x020185C0
	ldrh r3, [r3, #2]
	and r1, r3
	lsl r1, r1, #5
	ldr r5, _08011368 @ =0xFFFFC01F
	and r0, r5
	orr r0, r1
	ldr r4, _0801136C @ =0xFFFFBFFF
	and r0, r4
	ldr r3, _08011370 @ =0xFFFF7FFF
	and r0, r3
	str r0, [sp, #0]
	mov r1, #1
	mov r0, ip
	sub r1, r1, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	and r1, r7
	ldr r0, [sp, #4]
	mov r7, r9
	and r0, r7
	orr r0, r1
	mov r1, r8
	and r0, r1
	orr r0, r6
	ldr r6, _08011364 @ =0x020185C0
	ldrh r6, [r6, #4]
	and r2, r6
	lsl r2, r2, #5
	and r0, r5
	orr r0, r2
	and r0, r4
	and r0, r3
	str r0, [sp, #4]
	add r2, sp, #4
	mov r0, #1
	mov r1, sp
	bl DuelAnim_MoveCard
	ldr r7, _08011374 @ =0x02018DCA
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _08011478
_08011360: .4byte 0x000001FF
_08011364: .4byte 0x020185C0
_08011368: .4byte 0xFFFFC01F
_0801136C: .4byte 0xFFFFBFFF
_08011370: .4byte 0xFFFF7FFF
_08011374: .4byte 0x02018DCA
_08011378:
	mov r1, #1
	mov r0, ip
	sub r1, r1, r0
	mov r7, #1
	and r1, r7
	mov r2, sp
	mov r3, #2
	neg r3, r3
	add r0, r3, #0
	ldrb r2, [r2]
	and r0, r2
	orr r0, r1
	mov r1, sp
	strb r0, [r1]
	mov r1, #0x1F
	neg r1, r1
	mov sl, r1
	and r0, r1
	mov r2, #0x16
	mov r9, r2
	mov r4, r9
	orr r0, r4
	mov r1, sp
	strb r0, [r1]
	ldr r5, _0801142C @ =0x000001FF
	add r1, r5, #0
	ldr r6, _08011430 @ =0x020185C0
	ldrh r6, [r6, #4]
	and r1, r6
	lsl r1, r1, #5
	mov r2, sp
	ldr r4, _08011434 @ =0xFFFFC01F
	add r0, r4, #0
	ldrh r2, [r2]
	and r0, r2
	orr r0, r1
	mov r1, sp
	strh r0, [r1]
	mov r6, sp
	ldrb r2, [r6, #1]
	mov r1, #0x41
	neg r1, r1
	add r0, r1, #0
	and r0, r2
	mov r2, #0x7F
	mov r8, r2
	and r0, r2
	strb r0, [r6, #1]
	mov r6, ip
	and r6, r7
	add r2, sp, #4
	ldrb r7, [r2]
	and r3, r7
	orr r3, r6
	mov r0, sl
	and r3, r0
	mov r6, r9
	orr r3, r6
	strb r3, [r2]
	ldr r7, _08011430 @ =0x020185C0
	ldrh r7, [r7, #2]
	and r5, r7
	lsl r5, r5, #5
	ldrh r0, [r2]
	and r4, r0
	orr r4, r5
	strh r4, [r2]
	ldrb r0, [r2, #1]
	and r1, r0
	mov r3, r8
	and r1, r3
	strb r1, [r2, #1]
	mov r0, #1
	mov r1, sp
	bl DuelAnim_MoveCard
	ldr r4, _08011438 @ =0x02018DCA
	ldrb r2, [r4]
	lsl r0, r2, #0x19
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	mov r1, #0x80
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _08011478
	.align 2, 0
_0801142C: .4byte 0x000001FF
_08011430: .4byte 0x020185C0
_08011434: .4byte 0xFFFFC01F
_08011438: .4byte 0x02018DCA
_0801143C:
	mov r2, #1
	mov r0, ip
	and r0, r2
	ldr r4, _08011488 @ =0x00000D64
	mul r0, r4
	ldr r3, _0801148C @ =0x02019968
	add r0, r0, r3
	ldr r6, _08011490 @ =0x020185C0
	ldrh r6, [r6, #2]
	lsl r1, r6, #2
	add r0, r0, r1
	mov r7, ip
	sub r1, r2, r7
	and r1, r2
	mul r1, r4
	add r1, r1, r3
	ldr r3, _08011490 @ =0x020185C0
	ldrh r3, [r3, #4]
	lsl r2, r3, #2
	add r1, r1, r2
	bl SwapDuelCards
	ldr r4, _08011490 @ =0x020185C0
	ldr r5, _08011494 @ =0x0000080D
	add r1, r4, r5
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
_08011478:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08011488: .4byte 0x00000D64
_0801148C: .4byte 0x02019968
_08011490: .4byte 0x020185C0
_08011494: .4byte 0x0000080D
	thumb_func_end DuelCmd_ExchangeHandCards

