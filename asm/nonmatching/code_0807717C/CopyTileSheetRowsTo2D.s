	thumb_func_start CopyTileSheetRowsTo2D
CopyTileSheetRowsTo2D: @ 0x08077D38
	push {r4, r5, r6, r7, lr}
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	add r4, r0, #0
	add r5, r1, #0
	cmp r2, #0x10
	beq _08077D5C
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r2, r0
	bne _08077D88
	lsl r2, r3, #9
	add r0, r4, #0
	bl CpuSet
	b _08077D88
_08077D5C:
	mov r6, #0
	add r2, r3, #0
	cmp r6, r2
	bcs _08077D88
	add r7, r2, #0
_08077D66:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
	mov r0, #0x80
	lsl r0, r0, #2
	add r4, r4, r0
	mov r0, #0x80
	lsl r0, r0, #3
	add r5, r5, r0
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, r7
	bcc _08077D66
_08077D88:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CopyTileSheetRowsTo2D
	.align 2, 0

