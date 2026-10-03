	thumb_func_start DrawBgDecimal
DrawBgDecimal: @ 0x08072C0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	add r4, r0, #0
	add r6, r2, #0
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0xC]
	lsr r5, r1, #0x10
	mov r2, #1
	cmp r5, #8
	bhi _08072C9C
	mov r0, sp
	add r1, r0, r5
	mov r0, #0
	strb r0, [r1]
	cmp r6, #0
	bge _08072C3C
	neg r6, r6
_08072C3C:
	lsl r1, r4, #0x10
	mov r8, r1
	lsr r4, r4, #0x10
	mov r9, r4
	ldr r0, _08072C6C @ =0x08087634
	mov sl, r0
_08072C48:
	sub r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	cmp r6, #0
	bne _08072C56
	cmp r2, #0
	beq _08072C70
_08072C56:
	mov r1, sp
	add r4, r1, r5
	add r0, r6, #0
	mov r1, #0xA
	bl __modsi3
	lsl r0, r0, #2
	add r0, sl
	ldr r0, [r0]
	strb r0, [r4]
	b _08072C7E
_08072C6C: .4byte gDecimalDigitChars
_08072C70:
	mov r1, sp
	add r0, r1, r5
	mov r1, #0x20
	cmp r7, #0
	beq _08072C7C
	mov r1, #0x30
_08072C7C:
	strb r1, [r0]
_08072C7E:
	add r0, r6, #0
	mov r1, #0xA
	bl __divsi3
	add r6, r0, #0
	mov r2, #0
	cmp r5, #0
	bne _08072C48
	mov r1, r8
	lsr r0, r1, #0x10
	mov r1, r9
	ldr r2, [sp, #0xC]
	mov r3, sp
	bl DrawBgString
_08072C9C:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DrawBgDecimal

