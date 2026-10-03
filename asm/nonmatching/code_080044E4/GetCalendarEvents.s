	thumb_func_start GetCalendarEvents
GetCalendarEvents: @ 0x080044E4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	add r6, r1, #0
	add r5, r2, #0
	bl GetHolidayFlags
	add r7, r0, #0
	sub r0, r6, #2
	cmp r0, #0xA
	bls _08004500
	b _0800468E
_08004500:
	lsl r0, r0, #2
	ldr r1, _0800450C @ =0x08004510
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800450C: .4byte 0x08004510
_08004510:
	.4byte _0800453C
	.4byte _08004548
	.4byte _0800468E
	.4byte _0800468E
	.4byte _08004554
	.4byte _0800468E
	.4byte _0800468E
	.4byte _0800468E
	.4byte _080045DC
	.4byte _080045E6
	.4byte _08004684
_0800453C:
	cmp r5, #0xE
	beq _08004542
	b _0800468E
_08004542:
	mov r0, #0x80
	lsl r0, r0, #0xB
	b _0800468C
_08004548:
	cmp r5, #0xE
	beq _0800454E
	b _0800468E
_0800454E:
	mov r0, #0x80
	lsl r0, r0, #0xC
	b _0800468C
_08004554:
	cmp r5, #0x1C
	bne _0800455E
	mov r0, #0x80
	lsl r0, r0, #8
	orr r7, r0
_0800455E:
	mov r0, r8
	add r1, r6, #0
	mov r2, #1
	bl GetDayOfWeek
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	sub r4, r5, #1
	add r0, r4, #0
	mov r1, #7
	bl __udivsi3
	cmp r0, #0
	bne _08004594
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	cmp r0, #6
	bne _08004594
	mov r0, #0x80
	lsl r0, r0, #0x15
	orr r7, r0
_08004594:
	mov r0, r8
	add r1, r6, #0
	mov r2, #1
	bl GetDayOfWeek
	mov r0, r8
	add r1, r6, #0
	add r2, r4, #0
	bl GetDayOfWeek
	sub r0, r5, #2
	mov r1, #7
	bl __udivsi3
	cmp r0, #0
	bne _0800468E
	mov r0, r8
	add r1, r6, #0
	add r2, r4, #0
	bl GetDayOfWeek
	cmp r0, #6
	bne _0800468E
	ldr r0, _080045D4 @ =0x02011C20
	ldr r1, _080045D8 @ =0x00002160
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _0800468E
	mov r0, #0x80
	lsl r0, r0, #0x16
	b _0800468C
_080045D4: .4byte 0x02011C20
_080045D8: .4byte 0x00002160
_080045DC:
	cmp r5, #0x1F
	bne _0800468E
	mov r0, #0x80
	lsl r0, r0, #9
	b _0800468C
_080045E6:
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	cmp r0, #0
	bne _0800468E
	mov r0, r8
	add r1, r6, #0
	mov r2, #1
	bl GetDayOfWeek
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	sub r0, r5, #1
	mov r1, #7
	bl __udivsi3
	add r0, #1
	add r1, r0, #0
	cmp r0, #2
	beq _08004632
	cmp r0, #2
	bhi _08004622
	cmp r0, #1
	beq _0800462C
	b _0800468E
_08004622:
	cmp r1, #3
	beq _0800464C
	cmp r1, #4
	beq _08004668
	b _0800468E
_0800462C:
	mov r0, #0x80
	lsl r0, r0, #0x11
	b _0800468C
_08004632:
	ldr r0, _08004644 @ =0x02011C20
	ldr r1, _08004648 @ =0x0000215E
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _0800468E
	mov r0, #0x80
	lsl r0, r0, #0x12
	b _0800468C
_08004644: .4byte 0x02011C20
_08004648: .4byte 0x0000215E
_0800464C:
	ldr r0, _08004660 @ =0x02011C20
	ldr r1, _08004664 @ =0x0000215E
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #1
	bls _0800468E
	mov r0, #0x80
	lsl r0, r0, #0x13
	b _0800468C
	.align 2, 0
_08004660: .4byte 0x02011C20
_08004664: .4byte 0x0000215E
_08004668:
	ldr r0, _0800467C @ =0x02011C20
	ldr r1, _08004680 @ =0x0000215E
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #2
	bls _0800468E
	mov r0, #0x80
	lsl r0, r0, #0x14
	b _0800468C
	.align 2, 0
_0800467C: .4byte 0x02011C20
_08004680: .4byte 0x0000215E
_08004684:
	cmp r5, #0x18
	bne _0800468E
	mov r0, #0x80
	lsl r0, r0, #0xA
_0800468C:
	orr r7, r0
_0800468E:
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	cmp r0, #6
	bne _080046C8
	mov r0, r8
	add r1, r6, #0
	mov r2, #1
	bl GetDayOfWeek
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	sub r0, r5, #1
	mov r1, #7
	bl __udivsi3
	add r0, #1
	cmp r0, #2
	beq _080046C2
	cmp r0, #4
	bne _080046C8
_080046C2:
	mov r0, #0x80
	lsl r0, r0, #0xF
	orr r7, r0
_080046C8:
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080046DA
	b _080047E2
_080046DA:
	mov r4, #0
	mov r9, r4
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	cmp r0, #2
	bne _080046FC
	ldr r0, _080047F0 @ =0x000007D1
	cmp r8, r0
	bhi _080046FA
	cmp r6, #1
	bhi _080046FA
	cmp r5, #2
	bls _080046FC
_080046FA:
	mov r4, #1
_080046FC:
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	cmp r0, #1
	bne _0800471C
	add r2, r5, #1
	mov r0, r8
	add r1, r6, #0
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800471C
	mov r4, #1
_0800471C:
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl GetDayOfWeek
	cmp r0, #6
	bne _0800474C
	add r2, r5, #2
	mov r0, r8
	add r1, r6, #0
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800474C
	add r2, r5, #3
	mov r0, r8
	add r1, r6, #0
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800474C
	mov r4, #1
_0800474C:
	cmp r4, #0
	beq _08004756
	mov r0, #0x80
	lsl r0, r0, #0xD
	orr r7, r0
_08004756:
	cmp r5, #0x15
	bne _0800475E
	mov r0, #1
	mov r9, r0
_0800475E:
	cmp r5, #0x14
	bne _08004776
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x15
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004776
	mov r1, #1
	mov r9, r1
_08004776:
	cmp r5, #0x13
	bne _0800479E
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x14
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800479E
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x15
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800479E
	mov r0, #1
	mov r9, r0
_0800479E:
	cmp r5, #0x12
	bne _080047D6
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x13
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080047D6
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x14
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080047D6
	mov r0, r8
	add r1, r6, #0
	mov r2, #0x15
	bl IsDayOff
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080047D6
	mov r1, #1
	mov r9, r1
_080047D6:
	mov r0, r9
	cmp r0, #0
	beq _080047E2
	mov r0, #0x80
	lsl r0, r0, #0xE
	orr r7, r0
_080047E2:
	add r0, r7, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080047F0: .4byte 0x000007D1
	thumb_func_end GetCalendarEvents

