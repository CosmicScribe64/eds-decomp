	thumb_func_start Calendar_UpdateEventNames
Calendar_UpdateEventNames: @ 0x08002094
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	ldr r4, _08002150 @ =0x0201F7D0
	ldrh r1, [r4, #2]
	mov r0, sp
	bl DayCountToDate
	mov r3, #1
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	mov r2, #1
	str r3, [sp, #8]
	bl GetDayOfWeek
	add r6, r0, #0
	add r5, r6, #0
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	bl GetDaysInMonth
	mov r8, r0
	mov r2, #0
	mov r0, #4
	ldrb r4, [r4, #8]
	and r0, r4
	ldr r3, [sp, #8]
	cmp r0, #0
	beq _080020FE
	cmp r6, #0x14
	bgt _080020FC
_080020E0:
	add r6, #1
	add r3, #1
	add r5, #1
	add r0, r5, #0
	mov r1, #7
	str r2, [sp, #4]
	str r3, [sp, #8]
	bl __modsi3
	add r5, r0, #0
	ldr r2, [sp, #4]
	ldr r3, [sp, #8]
	cmp r6, #0x14
	ble _080020E0
_080020FC:
	mov r6, #0
_080020FE:
	cmp r3, r8
	bgt _0800216E
	cmp r6, #0x14
	bgt _0800216E
	ldr r7, _08002150 @ =0x0201F7D0
_08002108:
	ldrb r1, [r7, #8]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x1D
	cmp r5, r0
	bne _08002158
	ldrh r1, [r7, #8]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1D
	cmp r2, r0
	bne _08002158
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	add r2, r3, #0
	bl GetCalendarEvents
	add r4, r0, #0
	ldr r0, _08002154 @ =0x3F700000
	and r4, r0
	ldr r0, [r7, #4]
	cmp r0, r4
	beq _0800216E
	bl Calendar_ClearEventPanel
	add r0, r4, #0
	bl Calendar_DrawEventNames
	bl Calendar_FlipPage
	add r0, r4, #0
	bl Calendar_DrawEventNames
	str r4, [r7, #4]
	b _0800216E
_08002150: .4byte 0x0201F7D0
_08002154: .4byte 0x3F700000
_08002158:
	add r5, #1
	cmp r5, #6
	ble _08002162
	mov r5, #0
	add r2, #1
_08002162:
	add r3, #1
	add r6, #1
	cmp r3, r8
	bgt _0800216E
	cmp r6, #0x14
	ble _08002108
_0800216E:
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end Calendar_UpdateEventNames
	.align 2, 0

