	thumb_func_start sub_08078748
sub_08078748: @ 0x08078748
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	add r6, r1, #0
	ldr r0, [sp, #0x1C]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r2, #0x10
	beq _080787AC
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r2, r0
	bne _080787E2
	lsl r0, r3, #0x10
	mov r4, #0
	add r5, r0, #0
	cmp r5, #0
	beq _080787E2
	lsl r1, r1, #5
	mov r8, r1
	ldr r0, _080787A8 @ =0x001FFFFF
	mov r9, r0
_08078780:
	add r0, r7, #0
	add r1, r6, #0
	mov r2, r8
	mov r3, r9
	and r2, r3
	bl CpuSet
	mov r0, #0x80
	lsl r0, r0, #2
	add r7, r7, r0
	mov r3, #0x80
	lsl r3, r3, #3
	add r6, r6, r3
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r0, r5
	bcc _08078780
	b _080787E2
	.align 2, 0
_080787A8: .4byte 0x001FFFFF
_080787AC:
	lsl r0, r3, #0x10
	mov r4, #0
	add r5, r0, #0
	cmp r5, #0
	beq _080787E2
	lsl r1, r1, #4
	mov r8, r1
	ldr r0, _080787F0 @ =0x001FFFFF
	mov r9, r0
_080787BE:
	add r0, r7, #0
	add r1, r6, #0
	mov r2, r8
	mov r3, r9
	and r2, r3
	bl CpuSet
	mov r0, #0x80
	lsl r0, r0, #2
	add r7, r7, r0
	mov r3, #0x80
	lsl r3, r3, #3
	add r6, r6, r3
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r0, r5
	bcc _080787BE
_080787E2:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080787F0: .4byte 0x001FFFFF
	thumb_func_end sub_08078748

