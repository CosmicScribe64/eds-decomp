	thumb_func_start sub_0802A658
sub_0802A658: @ 0x0802A658
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	mov r7, #0x68
	mov r6, #0
	mov r0, #0x81
	lsl r0, r0, #7
	mov r8, r0
_0802A674:
	lsl r0, r6, #0x11
	lsr r2, r0, #0x10
	lsl r5, r2, #5
	add r4, r5, #2
	mov r0, r9
	asr r0, r6
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0802A6C6
	cmp r6, sl
	bne _0802A6B8
	lsl r2, r2, #0x15
	lsr r2, r2, #0x10
	add r0, r7, #0
	mov r1, #0x40
	bl sub_080761F0
	add r2, r4, #2
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r0, #0xA0
	mov r1, r8
	bl sub_080761F0
	add r2, r5, #0
	add r2, #8
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r0, #0xC0
	mov r1, r8
	bl sub_080761F0
	b _0802A6C4
_0802A6B8:
	lsl r2, r4, #0x10
	lsr r2, r2, #0x10
	add r0, r7, #0
	mov r1, #0x40
	bl sub_080761F0
_0802A6C4:
	add r7, #0x12
_0802A6C6:
	add r6, #1
	cmp r6, #3
	ble _0802A674
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0802A658
	.align 2, 0

