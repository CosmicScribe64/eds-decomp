	thumb_func_start Calendar_HandleInput
Calendar_HandleInput: @ 0x08002728
	push {r4, r5, r6, lr}
	sub sp, #4
	bl Calendar_DrawMonth
	ldr r4, _08002860 @ =0x0201F7D0
	mov r0, #3
	ldrb r1, [r4, #8]
	and r0, r1
	cmp r0, #3
	bne _080027C6
	ldr r5, _08002864 @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r2, [r5, #6]
	and r0, r2
	cmp r0, #0
	beq _08002786
	ldrh r1, [r4, #2]
	mov r0, sp
	bl DayCountToDate
	ldr r1, [sp, #0]
	lsl r0, r1, #0xB
	lsr r0, r0, #0x1B
	ldrh r2, [r4, #2]
	sub r0, r2, r0
	strh r0, [r4, #2]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	lsl r1, r1, #0x10
	lsr r1, r1, #0x1C
	bl GetDaysInMonth
	ldrh r1, [r4, #2]
	add r0, r1, r0
	add r0, #1
	strh r0, [r4, #2]
	mov r0, #4
	neg r0, r0
	ldrb r2, [r4, #8]
	and r0, r2
	strb r0, [r4, #8]
	bl Calendar_UpdateEventNames
	mov r0, #0
	bl PlaySE
_08002786:
	mov r0, #0x80
	lsl r0, r0, #2
	ldrh r5, [r5, #6]
	and r0, r5
	cmp r0, #0
	beq _080027C6
	ldrh r1, [r4, #2]
	mov r0, sp
	bl DayCountToDate
	ldrh r1, [r4, #2]
	cmp r1, #0x1E
	bls _080027C6
	ldr r0, [sp, #0]
	lsl r0, r0, #0xB
	lsr r0, r0, #0x1B
	sub r0, r1, r0
	strh r0, [r4, #2]
	mov r0, #4
	neg r0, r0
	ldrb r1, [r4, #8]
	and r0, r1
	strb r0, [r4, #8]
	bl Calendar_UpdateEventNames
	mov r0, #0
	bl PlaySE
	ldrh r1, [r4, #2]
	mov r0, sp
	bl DayCountToDate
_080027C6:
	ldr r6, _08002864 @ =0x03000040
	mov r0, #0x10
	ldrh r2, [r6, #6]
	and r0, r2
	cmp r0, #0
	beq _080027FC
	ldr r5, _08002860 @ =0x0201F7D0
	ldrb r4, [r5, #8]
	lsl r0, r4, #0x19
	lsr r0, r0, #0x1D
	add r0, #1
	mov r1, #7
	bl __modsi3
	mov r1, #7
	and r0, r1
	lsl r0, r0, #4
	mov r1, #0x71
	neg r1, r1
	and r1, r4
	orr r1, r0
	strb r1, [r5, #8]
	bl Calendar_UpdateEventNames
	mov r0, #0
	bl PlaySE
_080027FC:
	mov r0, #0x20
	ldrh r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	beq _08002830
	ldr r5, _08002860 @ =0x0201F7D0
	ldrb r4, [r5, #8]
	lsl r0, r4, #0x19
	lsr r0, r0, #0x1D
	add r0, #6
	mov r1, #7
	bl __modsi3
	mov r1, #7
	and r0, r1
	lsl r0, r0, #4
	mov r1, #0x71
	neg r1, r1
	and r1, r4
	orr r1, r0
	strb r1, [r5, #8]
	bl Calendar_UpdateEventNames
	mov r0, #0
	bl PlaySE
_08002830:
	mov r0, #0x40
	ldrh r6, [r6, #6]
	and r0, r6
	cmp r0, #0
	beq _080028A6
	ldr r3, _08002860 @ =0x0201F7D0
	ldrh r2, [r3, #8]
	mov r0, #0xE0
	lsl r0, r0, #2
	and r0, r2
	cmp r0, #0
	beq _0800286C
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1D
	sub r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #7
	ldr r1, _08002868 @ =0xFFFFFC7F
	and r1, r2
	orr r1, r0
	strh r1, [r3, #8]
	b _0800288E
	.align 2, 0
_08002860: .4byte 0x0201F7D0
_08002864: .4byte 0x03000040
_08002868: .4byte 0xFFFFFC7F
_0800286C:
	ldrb r1, [r3, #8]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _080028A0
	mov r0, #5
	neg r0, r0
	and r0, r1
	strb r0, [r3, #8]
	ldr r0, _0800289C @ =0xFFFFFC7F
	ldrh r2, [r3, #8]
	and r0, r2
	mov r2, #0x80
	lsl r2, r2, #1
	add r1, r2, #0
	orr r0, r1
	strh r0, [r3, #8]
_0800288E:
	bl Calendar_UpdateEventNames
	mov r0, #0
	bl PlaySE
	b _080028A6
	.align 2, 0
_0800289C: .4byte 0xFFFFFC7F
_080028A0:
	mov r0, #3
	bl PlaySE
_080028A6:
	ldr r1, _080028D0 @ =0x03000040
	mov r0, #0x80
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0800290A
	ldr r2, _080028D4 @ =0x0201F7D0
	ldrh r3, [r2, #8]
	lsl r1, r3, #0x16
	lsr r0, r1, #0x1D
	cmp r0, #1
	bhi _080028DC
	add r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #7
	ldr r1, _080028D8 @ =0xFFFFFC7F
	and r1, r3
	orr r1, r0
	strh r1, [r2, #8]
	b _080028F4
_080028D0: .4byte 0x03000040
_080028D4: .4byte 0x0201F7D0
_080028D8: .4byte 0xFFFFFC7F
_080028DC:
	ldrb r1, [r2, #8]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	bne _08002904
	mov r0, #4
	orr r0, r1
	strb r0, [r2, #8]
	ldr r0, _08002900 @ =0xFFFFFC7F
	ldrh r1, [r2, #8]
	and r0, r1
	strh r0, [r2, #8]
_080028F4:
	bl Calendar_UpdateEventNames
	mov r0, #0
	bl PlaySE
	b _0800290A
_08002900: .4byte 0xFFFFFC7F
_08002904:
	mov r0, #3
	bl PlaySE
_0800290A:
	ldr r1, _0800291C @ =0x03000040
	mov r0, #3
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08002920
	mov r0, #0
	b _08002928
	.align 2, 0
_0800291C: .4byte 0x03000040
_08002920:
	mov r0, #2
	bl PlaySE
	mov r0, #1
_08002928:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end Calendar_HandleInput

