	thumb_func_start sub_08005860
sub_08005860: @ 0x08005860
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r5, r2, #0
	ldr r2, [sp, #0x2C]
	lsl r0, r0, #0x10
	lsl r3, r3, #0x10
	lsl r2, r2, #0x10
	lsr r4, r2, #0x10
	str r4, [sp, #0]
	lsl r2, r3, #8
	lsr r2, r2, #0x18
	mov r9, r2
	lsr r6, r3, #0x18
	lsl r2, r0, #8
	lsr r2, r2, #0x18
	str r2, [sp, #4]
	lsr r0, r0, #0x18
	mov sl, r0
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsr r7, r1, #0x10
	add r0, r5, #0
	bl sub_080753CC
	ldr r1, _080059A4 @ =0x0000018F
	cmp r0, r1
	bgt _080058EC
	ldr r0, [sp, #4]
	mov r1, sl
	add r2, r4, #0
	mov r3, #2
	bl sub_08074B38
	mov r0, r8
	add r0, #1
	add r1, r7, #1
	ldr r2, [sp, #0x28]
	lsl r4, r2, #0x18
	lsr r4, r4, #0x10
	add r2, r6, #0
	orr r2, r4
	add r3, r5, #0
	bl sub_0807501C
	mov r0, r9
	orr r4, r0
	mov r0, r8
	add r1, r7, #0
	add r2, r4, #0
	add r3, r5, #0
	bl sub_0807501C
	ldr r3, _080059A8 @ =0x02013D90
	ldr r2, _080059AC @ =0x02000000
	ldr r1, _080059B0 @ =0x00010003
	add r2, r2, r1
	ldrb r4, [r2]
	ldr r1, [sp, #0x28]
	sub r0, r4, r1
	add r1, r0, #1
	and r0, r1
	str r0, [r3, #0x3C]
	ldrb r2, [r2]
	cmp r2, #0xBF
	bls _08005992
_080058EC:
	ldr r0, [sp, #4]
	mov r1, sl
	ldr r2, [sp, #0]
	mov r3, #1
	bl sub_08074B38
	mov r0, r8
	add r0, #1
	add r1, r7, #1
	ldr r2, [sp, #0x28]
	lsl r4, r2, #0x18
	lsr r4, r4, #0x10
	orr r6, r4
	add r2, r6, #0
	add r3, r5, #0
	bl sub_0807501C
	mov r0, r9
	orr r4, r0
	mov r0, r8
	add r1, r7, #0
	add r2, r4, #0
	add r3, r5, #0
	bl sub_0807501C
	ldr r0, _080059AC @ =0x02000000
	ldr r1, _080059B0 @ =0x00010003
	add r6, r0, r1
	ldrb r2, [r6]
	ldr r4, [sp, #0x28]
	sub r0, r2, r4
	add r1, r0, #1
	and r0, r1
	ldr r1, _080059A8 @ =0x02013D90
	str r0, [r1, #0x3C]
	ldrb r2, [r6]
	cmp r2, #0xBF
	bls _08005992
	ldr r0, [sp, #4]
	mov r1, sl
	ldr r2, [sp, #0]
	mov r3, #1
	bl sub_08074B38
	mov r4, #0x80
	lsl r4, r4, #4
	mov r0, r9
	orr r4, r0
	mov r0, r8
	add r1, r7, #0
	add r2, r4, #0
	add r3, r5, #0
	bl sub_0807501C
	ldrb r0, [r6]
	add r1, r0, #0
	sub r1, #8
	sub r0, #7
	and r1, r0
	ldr r2, _080059A8 @ =0x02013D90
	str r1, [r2, #0x3C]
	ldrb r0, [r6]
	cmp r0, #0xBF
	bls _08005992
	ldr r0, [sp, #4]
	mov r1, sl
	ldr r2, [sp, #0]
	mov r3, #0
	bl sub_08074B38
	mov r0, r8
	add r1, r7, #0
	add r2, r4, #0
	add r3, r5, #0
	bl sub_0807501C
	ldrb r0, [r6]
	add r1, r0, #0
	sub r1, #8
	sub r0, #7
	and r1, r0
	ldr r2, _080059A8 @ =0x02013D90
	str r1, [r2, #0x3C]
_08005992:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080059A4: .4byte 0x0000018F
_080059A8: .4byte 0x02013D90
_080059AC: .4byte 0x02000000
_080059B0: .4byte 0x00010003
	thumb_func_end sub_08005860

