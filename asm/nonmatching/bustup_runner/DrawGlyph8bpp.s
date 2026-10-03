	thumb_func_start DrawGlyph8bpp
DrawGlyph8bpp: @ 0x08001E4C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r7, r0, #0
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r3, #0
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #1
	str r0, [sp, #0]
_08001E6A:
	ldr r1, _08001EB4 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08001EBC
	ldr r1, [sp, #0]
	add r0, r1, r3
	lsl r0, r0, #1
	ldr r1, _08001EB8 @ =0x081D0200
	add r0, r0, r1
	ldrh r4, [r0]
	mov r6, #0x80
	lsl r6, r6, #8
	lsr r1, r4, #8
	lsl r0, r4, #0x18
	lsr r4, r0, #0x10
	orr r4, r1
	mov r5, #0
	mov r0, #0xF0
	add r0, r0, r7
	mov r9, r0
	add r3, #1
	mov r8, r3
_08001E9A:
	add r0, r4, #0
	and r0, r6
	cmp r0, #0
	beq _08001EAA
	add r0, r7, r5
	mov r1, sl
	bl PlotPixel8bpp
_08001EAA:
	lsr r6, r6, #1
	add r5, #1
	cmp r5, #0xF
	ble _08001E9A
	b _08001EEC
_08001EB4: .4byte 0x02011C20
_08001EB8: .4byte gFontKanji10x10
_08001EBC:
	ldr r1, [sp, #0]
	add r0, r1, r3
	ldr r1, _08001F04 @ =0x08229500
	add r0, r0, r1
	ldrb r6, [r0]
	mov r4, #0x80
	mov r5, #0
	mov r0, #0xF0
	add r0, r0, r7
	mov r9, r0
	add r3, #1
	mov r8, r3
_08001ED4:
	add r0, r6, #0
	and r0, r4
	cmp r0, #0
	beq _08001EE4
	add r0, r7, r5
	mov r1, sl
	bl PlotPixel8bpp
_08001EE4:
	lsr r4, r4, #1
	add r5, #1
	cmp r5, #7
	ble _08001ED4
_08001EEC:
	mov r7, r9
	mov r3, r8
	cmp r3, #9
	ble _08001E6A
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08001F04: .4byte gFontLatin8x10
	thumb_func_end DrawGlyph8bpp

