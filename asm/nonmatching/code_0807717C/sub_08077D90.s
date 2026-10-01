	thumb_func_start sub_08077D90
sub_08077D90: @ 0x08077D90
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, [sp, #0x20]
	ldr r4, [sp, #0x24]
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	lsl r2, r5, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	add r5, r0, #0
	mov r8, r1
	cmp r4, #0x10
	beq _08077DF8
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r4, r0
	bne _08077E2C
	mov r4, #0
	lsr r0, r2, #0x13
	cmp r4, r0
	bcs _08077E2C
	lsl r1, r7, #2
	mov sl, r1
	mov r9, r0
_08077DCA:
	lsl r1, r6, #5
	add r1, r8
	add r0, r5, #0
	mov r2, sl
	ldr r3, _08077DF4 @ =0x001FFFFF
	and r2, r3
	bl CpuSet
	lsl r0, r7, #3
	add r5, r5, r0
	add r0, r6, #0
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, r9
	bcc _08077DCA
	b _08077E2C
	.align 2, 0
_08077DF4: .4byte 0x001FFFFF
_08077DF8:
	mov r4, #0
	lsr r0, r2, #0x13
	cmp r4, r0
	bcs _08077E2C
	lsl r1, r7, #1
	mov sl, r1
	mov r9, r0
_08077E06:
	lsl r1, r6, #5
	add r1, r8
	add r0, r5, #0
	mov r2, sl
	ldr r3, _08077E3C @ =0x001FFFFF
	and r2, r3
	bl CpuSet
	lsl r0, r7, #2
	add r5, r5, r0
	add r0, r6, #0
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, r9
	bcc _08077E06
_08077E2C:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08077E3C: .4byte 0x001FFFFF
	thumb_func_end sub_08077D90

