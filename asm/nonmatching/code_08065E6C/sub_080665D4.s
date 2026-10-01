	thumb_func_start sub_080665D4
sub_080665D4: @ 0x080665D4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x20
	mov r8, r0
	add r7, r1, #0
	mov r6, #0
	mov r5, #0
_080665E4:
	lsl r0, r6, #4
	mov r1, r8
	add r4, r0, r1
	ldrb r0, [r4, #8]
	cmp r0, #0
	beq _08066632
	ldr r1, _08066648 @ =0x081A7144
	ldrb r2, [r4, #1]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	mov r2, #2
	ldsh r1, [r4, r2]
	str r1, [sp, #0]
	mov r1, #4
	str r1, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #0xC]
	str r5, [sp, #0x10]
	str r5, [sp, #0x14]
	str r5, [sp, #0x18]
	str r7, [sp, #0x1C]
	mov r1, #5
	mov r2, #1
	mov r3, #3
	neg r3, r3
	bl sub_08077EF4
	mov r2, #0x80
	lsl r2, r2, #1
	add r1, r2, #0
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
	ldrb r4, [r4, #0xE]
	lsl r1, r4, #9
	ldrh r2, [r0, #2]
	orr r1, r2
	strh r1, [r0, #2]
_08066632:
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #5
	bls _080665E4
	add sp, #0x20
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08066648: .4byte gUnk_081A7144
	thumb_func_end sub_080665D4

