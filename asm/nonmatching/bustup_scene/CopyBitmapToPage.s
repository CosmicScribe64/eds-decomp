	thumb_func_start CopyBitmapToPage
CopyBitmapToPage: @ 0x080008A4
	push {r4, r5, r6, r7, lr}
	add r6, r1, #0
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	lsl r4, r2, #0x10
	lsr r7, r4, #0x10
	ldr r0, [r6]
	ldr r1, _080008D4 @ =0x0600A000
	cmp r5, #0
	bne _080008BC
	mov r1, #0xC0
	lsl r1, r1, #0x13
_080008BC:
	lsr r2, r4, #0x13
	bl CpuSet
	lsr r1, r4, #0x12
	ldr r0, [r6]
	add r2, r0, r1
	cmp r5, #0
	bne _080008D8
	mov r0, #0xC0
	lsl r0, r0, #0x13
	b _080008DA
	.align 2, 0
_080008D4: .4byte 0x0600A000
_080008D8:
	ldr r0, _080008FC @ =0x0600A000
_080008DA:
	add r1, r1, r0
	lsr r4, r7, #3
	add r0, r2, #0
	add r2, r4, #0
	bl CpuSet
	lsr r1, r7, #2
	lsl r2, r1, #1
	ldr r0, [r6]
	add r3, r0, r2
	add r7, r4, #0
	add r4, r1, #0
	cmp r5, #0
	bne _08000900
	mov r0, #0xC0
	lsl r0, r0, #0x13
	b _08000902
_080008FC: .4byte 0x0600A000
_08000900:
	ldr r0, _08000920 @ =0x0600A000
_08000902:
	add r1, r2, r0
	add r2, r7, #0
	add r0, r3, #0
	bl CpuSet
	lsl r0, r4, #1
	add r1, r0, r4
	ldr r0, [r6]
	add r3, r0, r1
	cmp r5, #0
	bne _08000924
	mov r0, #0xC0
	lsl r0, r0, #0x13
	b _08000926
	.align 2, 0
_08000920: .4byte 0x0600A000
_08000924:
	ldr r0, _08000938 @ =0x0600A000
_08000926:
	add r1, r1, r0
	add r2, r7, #0
	add r0, r3, #0
	bl CpuSet
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08000938: .4byte 0x0600A000
	thumb_func_end CopyBitmapToPage

