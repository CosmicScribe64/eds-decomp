	thumb_func_start TextDrawSjisString
TextDrawSjisString: @ 0x08074E60
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r8, r3
	lsl r2, r2, #0x10
	lsr r3, r2, #0x10
	str r3, [sp, #0]
	lsr r7, r2, #0x18
	add r5, r0, #0
	mov r9, r1
	str r5, [sp, #4]
	ldr r2, _08074F40 @ =0x02000000
	ldr r1, _08074F44 @ =0x00010002
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _08074F48 @ =0x00010003
	add r0, r2, r3
	strb r1, [r0]
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #0
	beq _08074F2E
	mov sl, r2
_08074E96:
	mov r2, r8
	ldrb r2, [r2]
	lsl r4, r2, #8
	mov r3, r8
	ldrb r3, [r3, #1]
	orr r4, r3
	ldr r1, _08074F4C @ =0x00010004
	add r1, sl
	mov r0, #0x80
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08074EF4
	lsl r1, r7, #1
	add r1, r1, r7
	add r1, r5, r1
	mov r6, #0x80
	lsl r6, r6, #9
	add r6, sl
	ldrb r2, [r6]
	lsl r0, r2, #3
	cmp r1, r0
	ble _08074ECE
	add r0, r4, #0
	bl IsLineStartForbidden
	cmp r0, #0
	beq _08074EE4
_08074ECE:
	lsl r0, r7, #2
	add r0, r5, r0
	ldrb r6, [r6]
	lsl r1, r6, #3
	cmp r0, r1
	ble _08074EF4
	add r0, r4, #0
	bl IsLineEndForbidden
	cmp r0, #0
	beq _08074EF4
_08074EE4:
	ldr r5, [sp, #4]
	ldr r0, _08074F4C @ =0x00010004
	add r0, sl
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	add r0, r7, r0
	add r9, r0
_08074EF4:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, r9
	ldr r3, [sp, #0]
	bl TextDrawSjisGlyph
	ldr r2, _08074F40 @ =0x02000000
	ldr r3, _08074F44 @ =0x00010002
	add r1, r2, r3
	add r0, r5, r7
	ldrb r3, [r1]
	cmp r3, r0
	bge _08074F10
	strb r0, [r1]
_08074F10:
	ldr r3, _08074F48 @ =0x00010003
	add r1, r2, r3
	mov r3, r9
	add r2, r3, r7
	ldrb r3, [r1]
	cmp r3, r2
	bge _08074F20
	strb r2, [r1]
_08074F20:
	add r5, r0, #0
	mov r0, #2
	add r8, r0
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #0
	bne _08074E96
_08074F2E:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08074F40: .4byte 0x02000000
_08074F44: .4byte 0x00010002
_08074F48: .4byte 0x00010003
_08074F4C: .4byte 0x00010004
	thumb_func_end TextDrawSjisString

