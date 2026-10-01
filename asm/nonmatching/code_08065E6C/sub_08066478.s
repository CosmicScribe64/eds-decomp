	thumb_func_start sub_08066478
sub_08066478: @ 0x08066478
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r3, #0
	ldr r7, [sp, #0x18]
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldrb r5, [r6]
	cmp r2, #1
	beq _08066496
	cmp r2, #2
	beq _08066534
	b _080665BC
_08066496:
	ldr r0, _080664D4 @ =0x0000FFFF
	cmp r1, r0
	beq _080664CA
	lsl r4, r5, #4
	add r4, r6, r4
	mov r0, #0
	strb r0, [r4, #4]
	strb r2, [r4, #0xC]
	ldr r0, _080664D8 @ =0x08087464
	ldrh r0, [r0]
	strh r0, [r4, #6]
	add r0, r1, #0
	bl sub_0806635C
	strb r0, [r4, #5]
	ldr r0, _080664DC @ =0x08087472
	ldrh r2, [r0]
	strh r2, [r4, #0xE]
	add r1, r5, #1
	strb r1, [r4, #0x12]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r7, r0
	strh r2, [r0, #2]
	strh r2, [r0]
_080664CA:
	ldrb r0, [r6]
	cmp r0, #0
	beq _080664E0
	sub r0, #1
	b _080664E2
_080664D4: .4byte 0x0000FFFF
_080664D8: .4byte gUnk_08087464
_080664DC: .4byte gUnk_08087472
_080664E0:
	mov r0, #5
_080664E2:
	strb r0, [r6]
	mov r5, #0
	ldr r0, _0806652C @ =0x08087464
	mov r8, r0
	ldr r7, _08066530 @ =0x08087472
_080664EC:
	lsl r0, r5, #4
	add r4, r6, r0
	ldrb r0, [r4, #0xC]
	cmp r0, #0
	beq _0806651E
	ldrb r0, [r4, #4]
	add r0, #1
	strb r0, [r4, #4]
	lsl r0, r0, #1
	add r0, r8
	ldrh r1, [r4, #6]
	ldrh r0, [r0]
	sub r0, r0, r1
	strh r0, [r4, #0xA]
	strh r1, [r4, #8]
	ldrb r1, [r4, #4]
	lsl r0, r1, #1
	add r0, r0, r7
	ldrh r0, [r0]
	ldrh r1, [r4, #0xE]
	sub r0, r0, r1
	mov r1, #6
	bl __divsi3
	strh r0, [r4, #0x10]
_0806651E:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #5
	bls _080664EC
	b _080665BC
	.align 2, 0
_0806652C: .4byte gUnk_08087464
_08066530: .4byte gUnk_08087472
_08066534:
	ldr r0, _080665C8 @ =0x0000FFFF
	cmp r1, r0
	beq _0806656A
	lsl r4, r5, #4
	add r4, r6, r4
	mov r0, #6
	strb r0, [r4, #4]
	mov r0, #1
	strb r0, [r4, #0xC]
	ldr r0, _080665CC @ =0x08087464
	ldrh r0, [r0, #0xC]
	strh r0, [r4, #6]
	add r0, r1, #0
	bl sub_0806635C
	strb r0, [r4, #5]
	ldr r0, _080665D0 @ =0x08087472
	ldrh r2, [r0, #0xC]
	strh r2, [r4, #0xE]
	add r1, r5, #1
	strb r1, [r4, #0x12]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r7, r0
	strh r2, [r0, #2]
	strh r2, [r0]
_0806656A:
	ldrb r0, [r6]
	add r0, #1
	mov r1, #6
	bl __modsi3
	strb r0, [r6]
	mov r5, #0
	ldr r0, _080665CC @ =0x08087464
	mov r8, r0
	ldr r7, _080665D0 @ =0x08087472
_0806657E:
	lsl r0, r5, #4
	add r4, r6, r0
	ldrb r0, [r4, #0xC]
	cmp r0, #0
	beq _080665B2
	ldrb r0, [r4, #4]
	sub r0, #1
	strb r0, [r4, #4]
	lsl r0, r0, #1
	add r0, r8
	ldrh r1, [r4, #6]
	ldrh r0, [r0]
	sub r0, r1, r0
	strh r0, [r4, #0xA]
	sub r1, r1, r0
	strh r1, [r4, #8]
	ldrb r1, [r4, #4]
	lsl r0, r1, #1
	add r0, r0, r7
	ldrh r0, [r0]
	ldrh r1, [r4, #0xE]
	sub r0, r0, r1
	mov r1, #6
	bl __divsi3
	strh r0, [r4, #0x10]
_080665B2:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #5
	bls _0806657E
_080665BC:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080665C8: .4byte 0x0000FFFF
_080665CC: .4byte gUnk_08087464
_080665D0: .4byte gUnk_08087472
	thumb_func_end sub_08066478

