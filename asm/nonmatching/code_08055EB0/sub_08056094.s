	thumb_func_start sub_08056094
sub_08056094: @ 0x08056094
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r0, #0
	add r7, r1, #0
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	ldr r4, _08056190 @ =0x0000047F
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _080560C4
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _080560C6
_080560C4:
	mov r6, #1
_080560C6:
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl sub_08007834
	cmp r0, #0
	beq _080560D6
	mov r6, #1
_080560D6:
	ldr r4, _08056194 @ =0x0201CF90
	mov r0, #1
	mov r8, r0
	add r1, r5, #0
	mov r2, r8
	and r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	add r0, r5, #0
	bl sub_08008A44
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0x3F
	neg r1, r1
	ldrb r2, [r4]
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	ldr r0, _08056198 @ =0xFFFFC03F
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	mov r2, r8
	and r6, r2
	lsl r1, r6, #6
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r4, #1]
	and r0, r2
	orr r0, r1
	mov r6, #0x7F
	and r0, r6
	strb r0, [r4, #1]
	mov r0, #8
	neg r0, r0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	strb r0, [r4, #2]
	mov r0, #3
	neg r0, r0
	ldrb r2, [r4, #3]
	and r0, r2
	add r1, #0x34
	and r0, r1
	strb r0, [r4, #3]
	ldr r3, [r7]
	lsl r3, r3, #0x14
	lsr r1, r3, #0x14
	mov r2, #1
	and r1, r2
	lsl r1, r1, #7
	and r0, r6
	orr r0, r1
	strb r0, [r4, #3]
	lsr r3, r3, #0x15
	ldr r0, _0805619C @ =0xFFFF8000
	ldrh r1, [r4, #4]
	and r0, r1
	orr r0, r3
	strh r0, [r4, #4]
	add r0, r4, #0
	add r0, #8
	add r1, r7, #0
	bl sub_08007558
	mov r0, #0x1D
	neg r0, r0
	ldrb r2, [r4, #0xE]
	and r0, r2
	mov r1, #0x14
	orr r0, r1
	strb r0, [r4, #0xE]
	mov r1, #4
	mov r0, r9
	orr r0, r1
	strh r0, [r4, #0xC]
	bl sub_08055A00
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08056190: .4byte 0x0000047F
_08056194: .4byte 0x0201CF90
_08056198: .4byte 0xFFFFC03F
_0805619C: .4byte 0xFFFF8000
	thumb_func_end sub_08056094

