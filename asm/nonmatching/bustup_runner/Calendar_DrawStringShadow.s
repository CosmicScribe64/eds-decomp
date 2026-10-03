	thumb_func_start Calendar_DrawStringShadow
Calendar_DrawStringShadow: @ 0x08001F08
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r3, r1, #0
	add r5, r2, #0
	ldr r1, _08001F30 @ =0x0201F7D0
	mov r0, #8
	ldrb r1, [r1, #8]
	and r0, r1
	ldr r6, _08001F34 @ =0x0600A000
	cmp r0, #0
	beq _08001F22
	mov r6, #0xC0
	lsl r6, r6, #0x13
_08001F22:
	add r6, r6, r4
	lsl r0, r3, #4
	sub r0, r0, r3
	lsl r0, r0, #4
	add r6, r6, r0
	b _08001FA2
	.align 2, 0
_08001F30: .4byte 0x0201F7D0
_08001F34: .4byte 0x0600A000
_08001F38:
	ldr r1, _08001F84 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08001F88
	add r4, r6, #0
	add r4, #0xF1
	ldrb r1, [r5]
	lsl r0, r1, #8
	ldrb r1, [r5, #1]
	orr r0, r1
	bl SjisToGlyphIndex
	add r2, r0, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0xFF
	bl DrawGlyph8bpp
	ldrb r1, [r5]
	lsl r0, r1, #8
	ldrb r1, [r5, #1]
	orr r0, r1
	bl SjisToGlyphIndex
	add r2, r0, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r6, #0
	mov r1, #0xF7
	bl DrawGlyph8bpp
	add r6, #0xA
	add r5, #2
	b _08001FA2
	.align 2, 0
_08001F84: .4byte 0x02011C20
_08001F88:
	add r0, r6, #0
	add r0, #0xF1
	ldrb r2, [r5]
	mov r1, #0xFF
	bl DrawGlyph8bpp
	ldrb r2, [r5]
	add r0, r6, #0
	mov r1, #0xF7
	bl DrawGlyph8bpp
	add r6, #5
	add r5, #1
_08001FA2:
	ldrb r0, [r5]
	cmp r0, #0
	bne _08001F38
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end Calendar_DrawStringShadow
	.align 2, 0

