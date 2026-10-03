	thumb_func_start CopyBitmapRows
CopyBitmapRows: @ 0x08000270
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r8, r1
	ldr r0, [sp, #0x1C]
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r5, r3, #0x10
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r4, #0
	cmp r4, r5
	bcs _080002B4
	lsr r0, r2, #0x11
	lsl r6, r0, #0x10
_08000292:
	lsl r0, r4, #4
	sub r0, r0, r4
	lsl r0, r0, #4
	add r0, r9
	add r1, r4, #0
	mul r1, r7
	asr r1, r1, #2
	lsl r1, r1, #2
	add r1, r8
	lsr r2, r6, #0x10
	bl CpuSet
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r5
	bcc _08000292
_080002B4:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CopyBitmapRows

