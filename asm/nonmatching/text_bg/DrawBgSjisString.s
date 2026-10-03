	thumb_func_start DrawBgSjisString
DrawBgSjisString: @ 0x08072A14
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov r9, r3
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	ldr r0, _08072AD8 @ =0x0300045C
	mov sl, r0
	lsl r0, r1, #8
	lsr r0, r0, #0x18
	str r0, [sp, #0]
	lsr r1, r1, #0x18
	str r1, [sp, #4]
	str r6, [sp, #8]
	lsl r0, r6, #1
	add sl, r0
_08072A42:
	mov r1, r9
	ldrh r0, [r1]
	ldrb r1, [r1]
	cmp r1, #0
	beq _08072AE8
	lsr r0, r0, #8
	lsl r1, r1, #8
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r7, #0x1F
	add r4, r6, #0
	and r4, r7
	ldr r0, _08072ADC @ =0x03000040
	ldr r2, _08072AE0 @ =0x0000441E
	add r1, r0, r2
	add r0, r7, #0
	ldrh r2, [r1]
	and r0, r2
	sub r0, #2
	cmp r4, r0
	blt _08072A7E
	add r0, r5, #0
	str r1, [sp, #0xC]
	bl IsLineStartForbidden
	lsl r0, r0, #0x10
	ldr r1, [sp, #0xC]
	cmp r0, #0
	beq _08072A96
_08072A7E:
	add r0, r7, #0
	ldrh r1, [r1]
	and r0, r1
	sub r0, #3
	cmp r4, r0
	blt _08072AA8
	add r0, r5, #0
	bl IsLineEndForbidden
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08072AA8
_08072A96:
	ldr r0, [sp, #8]
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	str r6, [sp, #8]
	lsl r1, r6, #1
	ldr r0, _08072AD8 @ =0x0300045C
	add r1, r1, r0
	mov sl, r1
_08072AA8:
	mov r0, r8
	lsl r1, r0, #5
	ldr r2, _08072AE4 @ =0x06004000
	add r1, r1, r2
	add r0, r5, #0
	ldr r2, [sp, #0]
	ldr r3, [sp, #4]
	bl RenderSjisGlyphTile
	mov r1, r8
	mov r0, sl
	strh r1, [r0]
	mov r2, #2
	add sl, r2
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r9, r2
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	b _08072A42
_08072AD8: .4byte 0x0300045C
_08072ADC: .4byte 0x03000040
_08072AE0: .4byte 0x0000441E
_08072AE4: .4byte 0x06004000
_08072AE8:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DrawBgSjisString

