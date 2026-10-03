	thumb_func_start ListFilter_DrawProgressBar
ListFilter_DrawProgressBar: @ 0x08069F40
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r4, r1, #0
	lsl r2, r2, #0x18
	lsr r7, r2, #0x1B
	mov r0, #0xE0
	lsl r0, r0, #0x13
	and r0, r2
	lsr r0, r0, #0x18
	mov r9, r0
	mov r5, #0
	cmp r5, r7
	bcs _08069F88
	mov r0, #0x80
	lsl r0, r0, #2
	mov r8, r0
_08069F66:
	add r0, r6, #0
	add r1, r4, #0
	mov r2, #8
	bl ListFilter_CopyBarTileColumns
	mov r2, r8
	add r1, r4, r2
	add r0, r6, r2
	mov r2, #8
	bl ListFilter_CopyBarTileColumns
	add r4, #0x20
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, r7
	bcc _08069F66
_08069F88:
	add r0, r6, #0
	add r1, r4, #0
	mov r2, r9
	bl ListFilter_CopyBarTileColumns
	mov r1, #0x80
	lsl r1, r1, #2
	add r0, r6, r1
	add r1, r4, r1
	mov r2, r9
	bl ListFilter_CopyBarTileColumns
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end ListFilter_DrawProgressBar

