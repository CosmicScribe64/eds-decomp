	thumb_func_start ScrollLayer_StreamRow
ScrollLayer_StreamRow: @ 0x08026DC8
	push {lr}
	add r3, r0, #0
	mov r2, #8
	ldsb r2, [r3, r2]
	ldrh r1, [r3, #6]
	lsl r0, r1, #0x10
	asr r0, r0, #0x14
	add r1, r0, #0
	cmp r0, #0
	bge _08026DDE
	add r1, r0, #7
_08026DDE:
	asr r1, r1, #3
	cmp r2, r1
	beq _08026E0C
	strb r1, [r3, #8]
	mov r2, #8
	ldsb r2, [r3, r2]
	add r0, r2, #0
	add r0, #0x16
	lsl r1, r0, #4
	sub r1, r1, r0
	lsl r1, r1, #2
	ldr r0, [r3, #0xC]
	add r0, r0, r1
	sub r2, #1
	mov r1, #0x1F
	and r2, r1
	lsl r2, r2, #6
	ldr r1, [r3, #0x10]
	add r1, r1, r2
	mov r2, #0x1E
	mov r3, #1
	bl CopyMapRect
_08026E0C:
	pop {r0}
	bx r0
	thumb_func_end ScrollLayer_StreamRow

