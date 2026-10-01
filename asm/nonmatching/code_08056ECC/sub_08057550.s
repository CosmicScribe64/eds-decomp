	thumb_func_start sub_08057550
sub_08057550: @ 0x08057550
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	add r7, r0, #0
	str r1, [sp, #0xC]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x10]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0x14]
	mov r0, #1
	neg r0, r0
	mov sl, r0
	ldr r1, _080575C0 @ =0x0001869F
	mov r9, r1
	mov r5, #0
	mov r2, #1
	mov r8, r2
	mov r0, r8
	and r0, r7
	mov r8, r0
_08057582:
	ldr r1, [sp, #0xC]
	cmp r5, r1
	beq _08057604
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _080575C4 @ =0x00000D64
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	ldr r0, _080575C8 @ =0x0201930C
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08057604
	mov r6, #0
	ldrb r1, [r4, #6]
	mov r2, #2
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _080575CC
	add r0, r7, #0
	add r1, r5, #0
	mov r2, sp
	bl sub_0800ABC8
	b _080575E8
	.align 2, 0
_080575C0: .4byte 0x0001869F
_080575C4: .4byte 0x00000D64
_080575C8: .4byte 0x0201930C
_080575CC:
	add r0, r1, #0
	orr r0, r2
	strb r0, [r4, #6]
	add r0, r7, #0
	add r1, r5, #0
	mov r2, sp
	bl sub_0800ABC8
	mov r1, #3
	neg r1, r1
	add r0, r1, #0
	ldrb r2, [r4, #6]
	and r0, r2
	strb r0, [r4, #6]
_080575E8:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _080575F2
	ldr r0, [sp, #4]
	add r6, r6, r0
_080575F2:
	ldr r1, [sp, #0x14]
	cmp r1, #0
	beq _080575FC
	ldr r0, [sp, #8]
	add r6, r6, r0
_080575FC:
	cmp r6, r9
	bge _08057604
	mov sl, r5
	mov r9, r6
_08057604:
	add r5, #1
	cmp r5, #4
	ble _08057582
	mov r0, sl
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08057550

