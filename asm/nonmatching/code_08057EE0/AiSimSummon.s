	thumb_func_start AiSimSummon
AiSimSummon: @ 0x08057EE0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	mov r6, #0xF
	ldr r0, _08057F60 @ =0xFFFFF000
	mov ip, r0
	mov r2, #3
	mov r5, #7
	mov r4, #0x94
_08057EF8:
	add r0, r3, #0
	and r0, r6
	cmp r0, #0
	beq _08057F14
	add r0, r3, #0
	and r0, r5
	add r1, r0, #0
	mul r1, r4
	ldr r0, _08057F64 @ =0x0201A070
	add r1, r1, r0
	mov r0, ip
	ldrh r7, [r1]
	and r0, r7
	strh r0, [r1]
_08057F14:
	lsr r3, r3, #4
	sub r2, #1
	cmp r2, #0
	bge _08057EF8
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	mov r0, #0x94
	mul r2, r0
	ldr r1, _08057F64 @ =0x0201A070
	add r2, r2, r1
	mov r3, r8
	lsl r0, r3, #2
	ldr r7, _08057F68 @ =0x0000065C
	add r1, r1, r7
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r0, _08057F60 @ =0xFFFFF000
	ldrh r3, [r2]
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	mov r0, #2
	ldrb r7, [r2, #6]
	orr r0, r7
	mov r1, #2
	neg r1, r1
	and r0, r1
	strb r0, [r2, #6]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08057F60: .4byte 0xFFFFF000
_08057F64: .4byte 0x0201A070
_08057F68: .4byte 0x0000065C
	thumb_func_end AiSimSummon

