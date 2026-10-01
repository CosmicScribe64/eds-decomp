	thumb_func_start sub_0807AD40
sub_0807AD40: @ 0x0807AD40
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	str r0, [sp, #0]
	add r0, r1, #0
	add r1, r2, #0
	ldr r6, [sp, #0x28]
	ldr r2, [sp, #0x2C]
	mov r8, r2
	ldr r2, [sp, #0x30]
	ldr r4, [sp, #0x34]
	ldr r5, [sp, #0x38]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	mov r3, r8
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	lsl r2, r2, #0x18
	lsr r7, r2, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov sl, r4
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	add r2, r5, #0
	bl sub_0807A490
	ldr r1, _0807ADE0 @ =0x0000FFFE
	and r1, r0
	ldr r0, [sp, #0]
	add r4, r0, r1
	add r0, r6, #0
	mov r1, r8
	add r2, r5, #0
	bl sub_0807A490
	ldr r1, _0807ADE0 @ =0x0000FFFE
	and r1, r0
	ldr r2, [sp, #0x24]
	add r5, r2, r1
	mov r6, #0
	cmp r6, sl
	bcs _0807ADCE
	ldr r3, _0807ADE4 @ =0x001FFFFF
	mov r8, r3
_0807ADB0:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, r8
	and r2, r7
	bl CpuSet
	mov r1, r9
	lsl r0, r1, #1
	add r4, r4, r0
	add r5, #0x40
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, sl
	bcc _0807ADB0
_0807ADCE:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0807ADE0: .4byte 0x0000FFFE
_0807ADE4: .4byte 0x001FFFFF
	thumb_func_end sub_0807AD40

