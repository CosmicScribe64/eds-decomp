	thumb_func_start DuelPhase_ShowResult
DuelPhase_ShowResult: @ 0x080218AC
	push {r4, r5, r6, lr}
	ldr r4, _080218C8 @ =0x020192E0
	mov r0, #0xD9
	lsl r0, r0, #5
	add r6, r4, r0
	ldrb r1, [r6]
	cmp r1, #1
	bne _080218BE
	b _080219D0
_080218BE:
	cmp r1, #1
	bgt _080218CC
	cmp r1, #0
	beq _080218D4
	b _08021A3A
_080218C8: .4byte 0x020192E0
_080218CC:
	cmp r1, #2
	bne _080218D2
	b _08021A28
_080218D2:
	b _08021A3A
_080218D4:
	bl StopBGM
	ldr r1, _0802197C @ =0x00001B13
	add r0, r4, r1
	mov r1, #2
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	strb r1, [r0]
	mov r2, #2
	add r0, r2, #0
	ldrb r3, [r4, #0xB]
	and r0, r3
	cmp r0, #0
	bne _08021900
	ldr r0, _08021980 @ =0x00000D6F
	add r1, r4, r0
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08021924
_08021900:
	mov r0, #0x13
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #5
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x12
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08021924:
	ldr r1, _08021984 @ =0x020192E4
	mov r2, #4
	add r0, r2, #0
	ldrb r3, [r1, #7]
	and r0, r3
	cmp r0, #0
	bne _08021940
	ldr r0, _08021988 @ =0x00000D6B
	add r1, r1, r0
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08021964
_08021940:
	mov r0, #0x13
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #6
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x12
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08021964:
	ldr r0, _0802198C @ =0x020192E0
	ldr r1, _08021990 @ =0x00001B12
	add r0, r0, r1
	ldrb r0, [r0]
	lsr r0, r0, #6
	cmp r0, #2
	beq _080219A0
	cmp r0, #2
	bgt _08021994
	cmp r0, #1
	beq _0802199A
	b _080219BA
_0802197C: .4byte 0x00001B13
_08021980: .4byte 0x00000D6F
_08021984: .4byte 0x020192E4
_08021988: .4byte 0x00000D6B
_0802198C: .4byte 0x020192E0
_08021990: .4byte 0x00001B12
_08021994:
	cmp r0, #3
	beq _080219AE
	b _080219BA
_0802199A:
	mov r0, #4
	mov r1, #0
	b _080219A4
_080219A0:
	mov r0, #4
	mov r1, #1
_080219A4:
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _080219BA
_080219AE:
	mov r0, #4
	mov r1, #2
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080219BA:
	ldr r0, _080219CC @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_080219C8:
	mov r0, #0
	b _08021A3C
_080219CC: .4byte 0x020192E0
_080219D0:
	ldr r0, _08021A10 @ =0x020185C0
	ldr r3, _08021A14 @ =0x00000808
	add r0, r0, r3
	ldrh r5, [r0]
	cmp r5, #0
	bne _080219C8
	ldr r0, _08021A18 @ =0x02015EE8
	ldrb r0, [r0, #1]
	and r1, r0
	cmp r1, #0
	beq _08021A3A
	ldr r0, _08021A1C @ =0x0000F003
	ldr r2, _08021A20 @ =0x00001B12
	add r1, r4, r2
	ldrb r1, [r1]
	lsr r2, r1, #6
	mov r1, #1
	sub r1, r1, r2
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
	ldr r3, _08021A24 @ =0x00001B21
	add r0, r4, r3
	strb r5, [r0]
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _080219C8
	.align 2, 0
_08021A10: .4byte 0x020185C0
_08021A14: .4byte 0x00000808
_08021A18: .4byte 0x02015EE8
_08021A1C: .4byte 0x0000F003
_08021A20: .4byte 0x00001B12
_08021A24: .4byte 0x00001B21
_08021A28:
	ldr r0, _08021A44 @ =0x00001B21
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x13
	bls _080219C8
_08021A3A:
	mov r0, #1
_08021A3C:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08021A44: .4byte 0x00001B21
	thumb_func_end DuelPhase_ShowResult

