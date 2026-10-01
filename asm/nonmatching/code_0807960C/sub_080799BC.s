	thumb_func_start sub_080799BC
sub_080799BC: @ 0x080799BC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x14
	ldr r5, [sp, #0x34]
	ldr r4, [sp, #0x38]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	lsl r5, r5, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r0, #1
	and r6, r0
	lsr r7, r1, #0x11
	lsr r5, r5, #0x11
	cmp r4, #8
	bne _08079A38
	add r4, r5, #0
	mul r4, r2
	add r0, r7, r4
	lsl r0, r0, #1
	ldr r1, [sp, #0x30]
	add r0, r1, r0
	mov r1, sp
	mov r2, #0xA
	bl CpuSet
	mov r3, #0
	mov r1, #0x80
	lsl r1, r1, #8
_08079A0A:
	add r0, r1, #0
	asr r0, r3
	mov r2, r9
	and r0, r2
	cmp r0, #0
	beq _08079A1E
	add r0, r3, r6
	add r0, sp
	mov r2, r8
	strb r2, [r0]
_08079A1E:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #0xF
	bls _08079A0A
	add r1, r7, r4
	lsl r1, r1, #1
	ldr r4, [sp, #0x30]
	add r1, r4, r1
	mov r0, sp
	mov r2, #0xA
	bl CpuSet
_08079A38:
	add sp, #0x14
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_080799BC
	.align 2, 0

