	thumb_func_start ListFilter_ProgressBarVBlank
ListFilter_ProgressBarVBlank: @ 0x08069FAC
	push {r4, lr}
	ldr r3, _08069FD4 @ =0x0201DB20
	ldr r0, _08069FD8 @ =0x00001C54
	add r3, r3, r0
	ldrb r0, [r3]
	add r0, #1
	strb r0, [r3]
	ldr r0, _08069FDC @ =0x086F41A0
	ldr r1, _08069FE0 @ =0x06003C00
	ldrb r4, [r3]
	lsl r2, r4, #1
	add r2, r2, r4
	mov r3, #0x7F
	and r2, r3
	bl ListFilter_DrawProgressBar
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08069FD4: .4byte 0x0201DB20
_08069FD8: .4byte 0x00001C54
_08069FDC: .4byte gListFilterBarFillTiles
_08069FE0: .4byte 0x06003C00
	thumb_func_end ListFilter_ProgressBarVBlank

