	thumb_func_start DeckEdit_DrawStatementLabels
DeckEdit_DrawStatementLabels: @ 0x080679A8
	push {lr}
	sub sp, #0xC
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r2, r0, #1
	add r2, r2, r0
	lsl r2, r2, #2
	ldr r0, _080679D8 @ =0x086F1B10
	add r2, r2, r0
	ldr r1, _080679DC @ =0x0600E3B0
	mov r0, #0x1E
	str r0, [sp, #0]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	add r0, r2, #0
	mov r2, #6
	mov r3, #6
	bl CopyMapRectSetPalette
	add sp, #0xC
	pop {r0}
	bx r0
_080679D8: .4byte gDeckEditMenuTilemapStatementLabels
_080679DC: .4byte 0x0600E3B0
	thumb_func_end DeckEdit_DrawStatementLabels

