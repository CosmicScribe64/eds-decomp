	thumb_func_start Calendar_DrawMonth
Calendar_DrawMonth: @ 0x08002388
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	bl Calendar_DrawCursorAndHeader
	ldr r4, _0800247C @ =0x0201F7D0
	ldrh r1, [r4, #2]
	mov r0, sp
	bl DayCountToDate
	add r5, sp, #4
	ldrh r1, [r4]
	add r0, r5, #0
	bl DayCountToDate
	ldr r0, [sp, #0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r7, #0
	mov r6, #3
_080023B6:
	add r0, r4, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	mov r0, #0xE0
	sub r0, r0, r7
	mov r1, #0x80
	lsl r1, r1, #0xD
	orr r0, r1
	ldr r1, _08002480 @ =0x00006240
	add r2, r2, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0
	bl AddSprite
	add r0, r4, #0
	mov r1, #0xA
	bl __divsi3
	add r4, r0, #0
	add r7, #6
	sub r6, #1
	cmp r6, #0
	bge _080023B6
	mov r2, #1
	mov sl, r2
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	bl GetDayOfWeek
	add r7, r0, #0
	add r5, r7, #0
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	bl GetDaysInMonth
	str r0, [sp, #8]
	ldr r1, _0800247C @ =0x0201F7D0
	mov r0, #4
	ldrb r1, [r1, #8]
	and r0, r1
	cmp r0, #0
	beq _08002438
	cmp r7, #0x14
	bgt _08002436
_08002420:
	add r7, #1
	mov r3, #1
	add sl, r3
	add r5, #1
	add r0, r5, #0
	mov r1, #7
	bl __modsi3
	add r5, r0, #0
	cmp r7, #0x14
	ble _08002420
_08002436:
	mov r7, #0
_08002438:
	ldr r0, [sp, #8]
	cmp sl, r0
	ble _08002440
	b _08002568
_08002440:
	cmp r7, #0x14
	ble _08002446
	b _08002568
_08002446:
	mov r1, #0xC0
	lsl r1, r1, #0xE
	str r1, [sp, #0xC]
	mov r2, #0xA0
	lsl r2, r2, #0xE
	str r2, [sp, #0x10]
	mov r3, #0
	mov r9, r3
	lsl r0, r5, #5
	mov r8, r0
_0800245A:
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	mov r2, sl
	bl GetCalendarEvents
	add r6, r0, #0
	mov r2, #0
	mov r4, r8
	add r4, #0x18
	cmp r5, #0
	beq _08002484
	cmp r5, #6
	beq _08002488
	b _0800248A
_0800247C: .4byte 0x0201F7D0
_08002480: .4byte 0x00006240
_08002484:
	mov r2, #1
	b _0800248A
_08002488:
	mov r2, #2
_0800248A:
	mov r0, r8
	add r0, #0x10
	ldr r1, [sp, #0x10]
	orr r0, r1
	lsl r2, r2, #5
	add r2, sl
	mov r3, #0xC4
	lsl r3, r3, #7
	add r2, r2, r3
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0
	bl AddSprite
	mov r0, #0x80
	lsl r0, r0, #0xD
	and r0, r6
	cmp r0, #0
	beq _080024C2
	add r0, r4, #0
	ldr r1, [sp, #0xC]
	orr r0, r1
	mov r1, #0x40
	mov r2, #0xB9
	lsl r2, r2, #1
	bl AddSprite8bpp
	add r4, #8
_080024C2:
	mov r0, #0x80
	lsl r0, r0, #0xE
	and r0, r6
	cmp r0, #0
	beq _080024E0
	mov r0, r9
	add r0, #0x30
	lsl r0, r0, #0x10
	orr r0, r4
	mov r1, #0x40
	mov r2, #0xBA
	lsl r2, r2, #1
	bl AddSprite8bpp
	add r4, #8
_080024E0:
	mov r0, #0xFD
	lsl r0, r0, #0x16
	and r0, r6
	cmp r0, #0
	beq _080024FE
	mov r0, r9
	add r0, #0x30
	lsl r0, r0, #0x10
	orr r4, r0
	add r0, r4, #0
	mov r1, #0x40
	mov r2, #0xB8
	lsl r2, r2, #1
	bl AddSprite8bpp
_080024FE:
	mov r0, sp
	ldr r1, _08002578 @ =0x0000FFFF
	ldrh r0, [r0]
	and r1, r0
	ldr r0, _08002578 @ =0x0000FFFF
	mov r2, sp
	ldrh r2, [r2, #4]
	and r0, r2
	cmp r1, r0
	bne _08002532
	ldr r0, [sp, #4]
	lsl r0, r0, #0xB
	lsr r0, r0, #0x1B
	cmp sl, r0
	bne _08002532
	mov r0, r8
	add r0, #0xD
	mov r1, r9
	add r1, #0x25
	lsl r1, r1, #0x10
	orr r0, r1
	mov r1, #0x80
	mov r2, #0xBE
	lsl r2, r2, #1
	bl AddSprite8bpp
_08002532:
	mov r3, #0x20
	add r8, r3
	add r5, #1
	cmp r5, #6
	ble _08002556
	mov r0, #0
	mov r8, r0
	mov r5, #0
	ldr r1, [sp, #0xC]
	mov r2, #0xC0
	lsl r2, r2, #0xD
	add r1, r1, r2
	str r1, [sp, #0xC]
	ldr r3, [sp, #0x10]
	add r3, r3, r2
	str r3, [sp, #0x10]
	mov r0, #0x18
	add r9, r0
_08002556:
	mov r1, #1
	add sl, r1
	add r7, #1
	ldr r2, [sp, #8]
	cmp sl, r2
	bgt _08002568
	cmp r7, #0x14
	bgt _08002568
	b _0800245A
_08002568:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08002578: .4byte 0x0000FFFF
	thumb_func_end Calendar_DrawMonth

