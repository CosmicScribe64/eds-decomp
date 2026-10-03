	thumb_func_start DuelCmd_SetZoneStatusFlags
DuelCmd_SetZoneStatusFlags: @ 0x08011FFC
	push {r4, r5, r6, r7, lr}
	ldr r2, _080120CC @ =0x020185C0
	ldrh r1, [r2]
	lsr r0, r1, #0xF
	add r6, r0, #0
	ldrh r3, [r2, #2]
	mov ip, r3
	mov r4, #1
	and r0, r4
	ldr r1, _080120D0 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r3, _080120D4 @ =0x0201930C
	add r1, r5, r3
	mov r0, #0x94
	mov r7, ip
	mul r7, r0
	add r0, r7, #0
	add r1, r1, r0
	add r0, r4, #0
	ldrh r7, [r2, #4]
	and r0, r7
	cmp r0, #0
	beq _08012034
	mov r0, #0x40
	ldrb r7, [r1, #1]
	orr r0, r7
	strb r0, [r1, #1]
_08012034:
	mov r0, #2
	ldrh r7, [r2, #4]
	and r0, r7
	cmp r0, #0
	beq _08012046
	mov r0, #0x80
	ldrb r7, [r1, #1]
	orr r0, r7
	strb r0, [r1, #1]
_08012046:
	mov r0, #4
	ldrh r7, [r2, #4]
	and r0, r7
	cmp r0, #0
	beq _08012058
	mov r0, #1
	ldrb r7, [r1, #2]
	orr r0, r7
	strb r0, [r1, #2]
_08012058:
	mov r0, #8
	ldrh r7, [r2, #4]
	and r0, r7
	cmp r0, #0
	beq _0801206A
	mov r0, #2
	ldrb r7, [r1, #2]
	orr r0, r7
	strb r0, [r1, #2]
_0801206A:
	mov r0, #0x10
	ldrh r7, [r2, #4]
	and r0, r7
	cmp r0, #0
	beq _0801207C
	mov r0, #0x40
	ldrb r7, [r1, #7]
	orr r0, r7
	strb r0, [r1, #7]
_0801207C:
	mov r0, #0x20
	ldrh r7, [r2, #4]
	and r0, r7
	cmp r0, #0
	beq _0801208E
	mov r0, #0x80
	ldrb r7, [r1, #7]
	orr r0, r7
	strb r0, [r1, #7]
_0801208E:
	ldr r1, _080120D8 @ =0x00001AE6
	add r0, r3, r1
	ldrb r1, [r0]
	mov r0, #0x1C
	and r0, r1
	cmp r0, #0xC
	bne _080120B8
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	cmp r6, r0
	bne _080120B8
	add r0, r3, #0
	sub r0, #0x28
	add r0, r5, r0
	add r1, r4, #0
	mov r3, ip
	lsl r1, r3
	ldrh r7, [r0, #0x26]
	bic r7, r1
	add r1, r7, #0
	strh r1, [r0, #0x26]
_080120B8:
	ldr r0, _080120DC @ =0x0000080D
	add r1, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080120CC: .4byte 0x020185C0
_080120D0: .4byte 0x00000D64
_080120D4: .4byte 0x0201930C
_080120D8: .4byte 0x00001AE6
_080120DC: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetZoneStatusFlags

