	thumb_func_start RenderBoldGlyphTile
RenderBoldGlyphTile: @ 0x08072778
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r1, #0
	lsl r0, r0, #0x18
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	lsr r0, r0, #0x15
	ldr r1, _08072804 @ =0x0822BB00
	add r5, r0, r1
	mov r0, #0xF
	mov r9, r0
	mov r1, #3
	mov r8, r1
_0807279A:
	ldrh r1, [r5]
	lsr r0, r1, #4
	mov r1, r9
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl ExpandGlyphNibble
	strh r0, [r4]
	add r4, #2
	mov r0, #0xF
	ldrh r1, [r5]
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl ExpandGlyphNibble
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #0xC
	mov r1, r9
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl ExpandGlyphNibble
	strh r0, [r4]
	add r4, #2
	ldrh r1, [r5]
	lsr r0, r1, #8
	mov r1, r9
	and r0, r1
	add r1, r7, #0
	add r2, r6, #0
	bl ExpandGlyphNibble
	strh r0, [r4]
	add r4, #2
	add r5, #2
	mov r0, #1
	neg r0, r0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _0807279A
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08072804: .4byte gFontLatin8x8Bold
	thumb_func_end RenderBoldGlyphTile

