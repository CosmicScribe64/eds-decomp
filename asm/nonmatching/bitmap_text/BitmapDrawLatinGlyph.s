	thumb_func_start BitmapDrawLatinGlyph
BitmapDrawLatinGlyph: @ 0x08079B88
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	str r3, [sp, #0x10]
	ldr r3, [sp, #0x44]
	ldr r4, [sp, #0x48]
	ldr r5, [sp, #0x4C]
	mov r8, r5
	ldr r6, [sp, #0x50]
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0xC]
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp, #0x14]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r0, r8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	mov r8, r6
	cmp r4, #8
	bne _08079C30
	lsl r0, r5, #3
	ldr r1, _08079C2C @ =0x08228D00
	add r6, r0, r1
	mov r5, #3
	mov sl, r5
_08079BD4:
	ldrh r4, [r6]
	add r6, #2
	lsl r4, r4, #0x11
	lsl r0, r4, #8
	lsr r0, r0, #0x18
	add r2, r7, #0
	add r1, r2, #1
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	str r7, [sp, #0x1C]
	ldr r1, [sp, #0x10]
	str r1, [sp, #0]
	mov r5, r9
	str r5, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	ldr r1, [sp, #0xC]
	ldr r3, [sp, #0x14]
	bl BitmapPlotRow8
	lsr r4, r4, #0x18
	ldr r0, [sp, #0x1C]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldr r5, [sp, #0x10]
	str r5, [sp, #0]
	mov r0, r9
	str r0, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	add r0, r4, #0
	ldr r1, [sp, #0xC]
	ldr r2, [sp, #0x1C]
	ldr r3, [sp, #0x14]
	bl BitmapPlotRow8
	mov r5, #1
	neg r5, r5
	add sl, r5
	mov r0, sl
	cmp r0, #0
	bge _08079BD4
	b _08079CB4
_08079C2C: .4byte gFontLatin8x8
_08079C30:
	cmp r4, #0xA
	beq _08079C3A
	cmp r4, #0xC
	beq _08079C4C
	b _08079CB4
_08079C3A:
	lsl r0, r5, #2
	add r0, r0, r5
	lsl r0, r0, #1
	ldr r1, _08079C48 @ =0x08229500
	add r6, r0, r1
	b _08079C56
	.align 2, 0
_08079C48: .4byte gFontLatin8x10
_08079C4C:
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #2
	ldr r5, _08079CC4 @ =0x08229F00
	add r6, r0, r5
_08079C56:
	lsr r4, r4, #1
	cmp r4, #0
	beq _08079CB4
	mov sl, r4
_08079C5E:
	ldrh r4, [r6]
	add r6, #2
	lsl r4, r4, #0x11
	lsl r0, r4, #8
	lsr r0, r0, #0x18
	add r2, r7, #0
	add r1, r2, #1
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	str r7, [sp, #0x20]
	ldr r1, [sp, #0x10]
	str r1, [sp, #0]
	mov r5, r9
	str r5, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	ldr r1, [sp, #0xC]
	ldr r3, [sp, #0x14]
	bl BitmapPlotRow8
	lsr r4, r4, #0x18
	ldr r0, [sp, #0x20]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldr r5, [sp, #0x10]
	str r5, [sp, #0]
	mov r0, r9
	str r0, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	add r0, r4, #0
	ldr r1, [sp, #0xC]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x14]
	bl BitmapPlotRow8
	mov r5, #1
	neg r5, r5
	add sl, r5
	mov r0, sl
	cmp r0, #0
	bne _08079C5E
_08079CB4:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08079CC4: .4byte gFontLatin8x12
	thumb_func_end BitmapDrawLatinGlyph

