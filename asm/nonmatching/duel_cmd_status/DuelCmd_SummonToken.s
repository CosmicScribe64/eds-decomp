	thumb_func_start DuelCmd_SummonToken
DuelCmd_SummonToken: @ 0x0801296C
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	ldr r0, _080129A8 @ =0x020185C0
	ldrh r1, [r0]
	lsr r6, r1, #0xF
	ldrb r7, [r0, #2]
	ldrh r2, [r0, #4]
	ldr r3, _080129AC @ =0x0000080A
	add r4, r0, r3
	ldrb r3, [r4]
	lsl r1, r3, #0x19
	cmp r1, #0
	bne _080129B0
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _08012AA4
	.align 2, 0
_080129A8: .4byte 0x020185C0
_080129AC: .4byte 0x0000080A
_080129B0:
	cmp r2, #1
	beq _080129C8
	cmp r2, #1
	bgt _080129BE
	cmp r2, #0
	beq _080129CC
	b _080129D0
_080129BE:
	cmp r2, #2
	beq _080129C8
	cmp r2, #3
	beq _080129CC
	b _080129D0
_080129C8:
	mov r5, #1
	b _080129E4
_080129CC:
	mov r5, #0
	b _080129E4
_080129D0:
	ldr r2, _080129E0 @ =0x0000080D
	add r1, r0, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _08012AA2
	.align 2, 0
_080129E0: .4byte 0x0000080D
_080129E4:
	mov r1, #0xF0
	lsl r1, r1, #3
	add r0, r2, r1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	ldr r0, _080129FC @ =0x0000FFFF
	cmp r1, r0
	bne _08012A00
	mov r0, #0
	b _08012A2E
	.align 2, 0
_080129FC: .4byte 0x0000FFFF
_08012A00:
	ldr r0, _08012A14 @ =0x000007CF
	cmp r1, r0
	bhi _08012A1C
	add r0, #0x30
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08012A18 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	b _08012A2E
_08012A14: .4byte 0x000007CF
_08012A18: .4byte gCardNumberToId
_08012A1C:
	ldr r3, _08012AAC @ =0xFFFFF830
	add r0, r2, r3
	ldr r1, _08012AB0 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08012AB4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_08012A2E:
	lsl r1, r0, #0x14
	lsr r1, r1, #0x14
	ldr r2, _08012AB8 @ =0xFFFFF000
	ldr r0, [sp, #4]
	and r0, r2
	orr r0, r1
	mov r3, #1
	add r4, r6, #0
	and r4, r3
	lsl r2, r4, #0xC
	ldr r1, _08012ABC @ =0xFFFFEFFF
	and r0, r1
	orr r0, r2
	lsl r2, r4, #0xD
	ldr r1, _08012AC0 @ =0xFFFFDFFF
	and r0, r1
	orr r0, r2
	mov r1, #0x80
	lsl r1, r1, #7
	orr r0, r1
	ldr r1, _08012AC4 @ =0xFFFF7FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #9
	orr r0, r1
	ldr r1, _08012AC8 @ =0xFFFDFFFF
	and r0, r1
	ldr r1, _08012ACC @ =0xFFFBFFFF
	and r0, r1
	str r0, [sp, #4]
	str r3, [sp, #0]
	add r0, r6, #0
	add r1, r7, #0
	add r2, sp, #4
	add r3, r5, #0
	bl PlaceMonsterCard
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08012AD0 @ =0x00000D64
	mul r0, r4
	add r1, r1, r0
	ldr r0, _08012AD4 @ =0x0201930C
	add r1, r1, r0
	mov r0, #4
	ldrb r2, [r1, #7]
	orr r0, r2
	strb r0, [r1, #7]
	bl DrawAllAreaTiles
	ldr r1, _08012AD8 @ =0x020185C0
	ldr r3, _08012ADC @ =0x0000080D
	add r1, r1, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_08012AA2:
	strb r0, [r1]
_08012AA4:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08012AAC: .4byte 0xFFFFF830
_08012AB0: .4byte 0x000007FF
_08012AB4: .4byte gCardNumberToId
_08012AB8: .4byte 0xFFFFF000
_08012ABC: .4byte 0xFFFFEFFF
_08012AC0: .4byte 0xFFFFDFFF
_08012AC4: .4byte 0xFFFF7FFF
_08012AC8: .4byte 0xFFFDFFFF
_08012ACC: .4byte 0xFFFBFFFF
_08012AD0: .4byte 0x00000D64
_08012AD4: .4byte 0x0201930C
_08012AD8: .4byte 0x020185C0
_08012ADC: .4byte 0x0000080D
	thumb_func_end DuelCmd_SummonToken

