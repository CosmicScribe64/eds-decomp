	thumb_func_start Calendar_SetCursorDate
Calendar_SetCursorDate: @ 0x0800217C
	push {r4, r5, lr}
	sub sp, #4
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, sp
	bl DayCountToDate
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl GetDayOfWeek
	ldr r4, _08002204 @ =0x0201F7D0
	mov r1, #7
	and r0, r1
	lsl r0, r0, #4
	mov r1, #0x71
	neg r1, r1
	ldrb r2, [r4, #8]
	and r1, r2
	orr r1, r0
	strb r1, [r4, #8]
	ldr r1, [sp, #0]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	mov r2, #1
	bl GetDayOfWeek
	add r1, r0, #0
	ldr r0, [sp, #0]
	lsl r0, r0, #0xB
	lsr r0, r0, #0x1B
	sub r1, #1
	add r0, r0, r1
	mov r1, #7
	bl __udivsi3
	mov r5, #7
	and r0, r5
	lsl r0, r0, #7
	ldr r3, _08002208 @ =0xFFFFFC7F
	add r1, r3, #0
	ldrh r2, [r4, #8]
	and r1, r2
	orr r1, r0
	strh r1, [r4, #8]
	lsl r2, r1, #0x16
	lsr r0, r2, #0x1D
	cmp r0, #2
	bls _0800220C
	sub r0, #2
	and r0, r5
	lsl r0, r0, #7
	and r1, r3
	orr r1, r0
	strh r1, [r4, #8]
	mov r0, #4
	ldrb r1, [r4, #8]
	orr r0, r1
	b _08002214
	.align 2, 0
_08002204: .4byte 0x0201F7D0
_08002208: .4byte 0xFFFFFC7F
_0800220C:
	mov r0, #5
	neg r0, r0
	ldrb r2, [r4, #8]
	and r0, r2
_08002214:
	strb r0, [r4, #8]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end Calendar_SetCursorDate
	.align 2, 0

