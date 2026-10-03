	thumb_func_start BitmapPlotRow8
BitmapPlotRow8: @ 0x080798B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r5, [sp, #0x30]
	ldr r4, [sp, #0x34]
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	lsl r5, r5, #0x10
	lsl r4, r4, #0x18
	lsr r3, r4, #0x18
	mov r0, #1
	and r6, r0
	lsr r1, r1, #0x11
	mov r8, r1
	add r0, r2, #0
	lsr r4, r5, #0x11
	cmp r3, #4
	beq _080798F6
	cmp r3, #8
	beq _08079964
	b _080799AC
_080798F6:
	mul r4, r0
	mov r1, r8
	add r0, r1, r4
	lsl r0, r0, #1
	ldr r2, [sp, #0x2C]
	add r0, r2, r0
	mov r1, sp
	mov r2, #5
	bl CpuSet
	mov r3, #0
	mov sl, r4
	mov r5, #0x80
	mov r4, #0xF0
	mov ip, r4
_08079914:
	lsl r2, r3, #1
	add r0, r5, #0
	asr r0, r2
	and r0, r7
	cmp r0, #0
	beq _08079930
	add r0, r3, r6
	mov r4, sp
	add r1, r4, r0
	mov r0, ip
	ldrb r4, [r1]
	and r0, r4
	add r0, r9
	strb r0, [r1]
_08079930:
	add r1, r2, #1
	add r0, r5, #0
	asr r0, r1
	and r0, r7
	cmp r0, #0
	beq _08079946
	add r0, r3, r6
	mov r2, sp
	add r1, r2, r0
	mov r0, #0
	strb r0, [r1]
_08079946:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #3
	bls _08079914
	mov r1, r8
	add r1, sl
	lsl r1, r1, #1
	ldr r4, [sp, #0x2C]
	add r1, r4, r1
	mov r0, sp
	mov r2, #5
	bl CpuSet
	b _080799AC
_08079964:
	mul r4, r2
	mov r1, r8
	add r0, r1, r4
	lsl r0, r0, #1
	ldr r2, [sp, #0x2C]
	add r0, r2, r0
	mov r1, sp
	mov r2, #5
	bl CpuSet
	mov r3, #0
	mov sl, r4
	mov r1, #0x80
_0807997E:
	add r0, r1, #0
	asr r0, r3
	and r0, r7
	cmp r0, #0
	beq _08079990
	add r0, r3, r6
	add r0, sp
	mov r4, r9
	strb r4, [r0]
_08079990:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #7
	bls _0807997E
	mov r1, r8
	add r1, sl
	lsl r1, r1, #1
	ldr r0, [sp, #0x2C]
	add r1, r0, r1
	mov r0, sp
	mov r2, #5
	bl CpuSet
_080799AC:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end BitmapPlotRow8

