	thumb_func_start CopyFullBitmapToPage
CopyFullBitmapToPage: @ 0x08000880
	push {lr}
	lsl r0, r0, #0x18
	ldr r1, [r1]
	ldr r3, _080008A0 @ =0x0600A000
	cmp r0, #0
	bne _08000890
	mov r3, #0xC0
	lsl r3, r3, #0x13
_08000890:
	mov r2, #0x96
	lsl r2, r2, #7
	add r0, r1, #0
	add r1, r3, #0
	bl CpuSet
	pop {r0}
	bx r0
_080008A0: .4byte 0x0600A000
	thumb_func_end CopyFullBitmapToPage

