	thumb_func_start sub_08004358
sub_08004358: @ 0x08004358
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r6, r1, #0
	add r4, r2, #0
	mov r5, #0
	sub r0, r6, #1
	cmp r0, #0xB
	bls _0800436A
	b _0800448A
_0800436A:
	lsl r0, r0, #2
	ldr r1, _08004374 @ =0x08004378
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08004374: .4byte 0x08004378
_08004378:
	.4byte _080043A8
	.4byte _080043E2
	.4byte _0800448A
	.4byte _080043F4
	.4byte _080043FC
	.4byte _0800448A
	.4byte _0800441C
	.4byte _0800448A
	.4byte _0800442E
	.4byte _08004436
	.4byte _0800446A
	.4byte _08004480
_080043A8:
	cmp r4, #1
	bne _080043AE
	orr r5, r4
_080043AE:
	add r0, r7, #0
	add r1, r6, #0
	mov r2, #1
	bl sub_080042D8
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080042D8
	sub r0, r4, #1
	mov r1, #7
	bl __udivsi3
	cmp r0, #1
	bne _0800448A
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080042D8
	cmp r0, #1
	bne _0800448A
	mov r0, #0x80
	lsl r0, r0, #4
	b _08004488
_080043E2:
	cmp r4, #0xB
	bne _080043EA
	mov r0, #2
	orr r5, r0
_080043EA:
	cmp r4, #0x18
	bne _0800448A
	mov r0, #0x80
	lsl r0, r0, #6
	b _08004488
_080043F4:
	cmp r4, #0x1D
	bne _0800448A
	mov r0, #4
	b _08004488
_080043FC:
	cmp r4, #4
	beq _08004414
	cmp r4, #4
	bhi _0800440A
	cmp r4, #3
	beq _08004410
	b _0800448A
_0800440A:
	cmp r4, #5
	beq _08004418
	b _0800448A
_08004410:
	mov r0, #8
	b _08004488
_08004414:
	mov r0, #0x10
	b _08004488
_08004418:
	mov r0, #0x20
	b _08004488
_0800441C:
	cmp r4, #0x14
	bne _08004424
	mov r0, #0x40
	orr r5, r0
_08004424:
	cmp r4, #7
	bne _0800448A
	mov r0, #0x80
	lsl r0, r0, #7
	b _08004488
_0800442E:
	cmp r4, #0xF
	bne _0800448A
	mov r0, #0x80
	b _08004488
_08004436:
	add r0, r7, #0
	add r1, r6, #0
	mov r2, #1
	bl sub_080042D8
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080042D8
	sub r0, r4, #1
	mov r1, #7
	bl __udivsi3
	cmp r0, #1
	bne _0800448A
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080042D8
	cmp r0, #1
	bne _0800448A
	mov r0, #0x80
	lsl r0, r0, #5
	b _08004488
_0800446A:
	cmp r4, #3
	beq _08004474
	cmp r4, #0x17
	beq _0800447A
	b _0800448A
_08004474:
	mov r0, #0x80
	lsl r0, r0, #1
	b _08004488
_0800447A:
	mov r0, #0x80
	lsl r0, r0, #2
	b _08004488
_08004480:
	cmp r4, #0x17
	bne _0800448A
	mov r0, #0x80
	lsl r0, r0, #3
_08004488:
	orr r5, r0
_0800448A:
	add r0, r5, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08004358
	.align 2, 0

