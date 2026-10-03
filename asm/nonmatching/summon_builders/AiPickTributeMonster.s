	thumb_func_start AiPickTributeMonster
AiPickTributeMonster: @ 0x080563B8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0xC
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	mov r4, #0
	ldr r1, _08056424 @ =0x0201A070
	ldr r2, _08056428 @ =0x000007FF
_080563D0:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080563F2
	cmp r4, r6
	beq _080563F2
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _0805642C @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, #0x2F
	beq _08056420
_080563F2:
	add r4, #1
	cmp r4, #4
	ble _080563D0
	mov r4, #0
	ldr r3, _08056428 @ =0x000007FF
	ldr r2, _08056430 @ =0x0000023D
_080563FE:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08056434
	cmp r4, r6
	beq _08056434
	and r0, r3
	lsl r0, r0, #1
	ldr r5, _0805642C @ =0x08622AB4
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r2
	bne _08056434
_08056420:
	add r0, r4, #0
	b _08056494
_08056424: .4byte 0x0201A070
_08056428: .4byte 0x000007FF
_0805642C: .4byte gCardIdToNumber
_08056430: .4byte 0x0000023D
_08056434:
	add r4, #1
	cmp r4, #4
	ble _080563FE
	mov r7, #0xFA
	lsl r7, r7, #7
	mov r0, #1
	neg r0, r0
	mov r8, r0
	mov r4, #0
_08056446:
	mov r0, #0x94
	mul r0, r4
	ldr r1, _080564A4 @ =0x0201A070
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0805648C
	cmp r4, r6
	beq _0805648C
	mov r5, #1
	bl IsFusionMonster
	cmp r0, #0
	beq _08056468
	mov r5, #0
_08056468:
	cmp r5, #0
	bne _08056472
	mov r3, r9
	cmp r3, #0
	beq _0805648C
_08056472:
	mov r0, #1
	add r1, r4, #0
	mov r2, sp
	bl GetZoneCardStats
	ldr r0, [sp, #4]
	lsl r0, r0, #1
	ldr r1, [sp, #8]
	add r0, r0, r1
	cmp r0, r7
	bge _0805648C
	mov r8, r4
	add r7, r0, #0
_0805648C:
	add r4, #1
	cmp r4, #4
	ble _08056446
	mov r0, r8
_08056494:
	add sp, #0xC
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080564A4: .4byte 0x0201A070
	thumb_func_end AiPickTributeMonster

