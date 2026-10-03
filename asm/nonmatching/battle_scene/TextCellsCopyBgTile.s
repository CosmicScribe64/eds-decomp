	thumb_func_start TextCellsCopyBgTile
TextCellsCopyBgTile: @ 0x0805EEDC
	push {lr}
	lsl r1, r1, #0x10
	lsl r0, r0, #5
	ldr r2, _0805EEF8 @ =0x0201CFB8
	add r0, r0, r2
	lsr r1, r1, #0xB
	ldr r2, _0805EEFC @ =0x06004000
	add r1, r1, r2
	mov r2, #0x20
	bl MemCopy16
	pop {r0}
	bx r0
	.align 2, 0
_0805EEF8: .4byte 0x0201CFB8
_0805EEFC: .4byte 0x06004000
	thumb_func_end TextCellsCopyBgTile

