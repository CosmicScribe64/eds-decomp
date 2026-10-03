	thumb_func_start DuelCmd_ClearZoneStatusFlags
DuelCmd_ClearZoneStatusFlags: @ 0x080120E0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r2, _080121A4 @ =0x020185C0
	ldrh r0, [r2]
	lsr r4, r0, #0xF
	ldrh r5, [r2, #2]
	mov r6, #1
	add r0, r4, #0
	and r0, r6
	ldr r1, _080121A8 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	mov r8, r3
	ldr r7, _080121AC @ =0x0201930C
	mov sl, r7
	mov r1, r8
	add r1, sl
	mov r0, #0x94
	add r3, r5, #0
	mul r3, r0
	str r3, [sp, #4]
	add r1, r1, r3
	ldr r7, _080121B0 @ =0x0000080A
	add r7, r7, r2
	mov r9, r7
	ldrb r3, [r7]
	lsl r0, r3, #0x19
	cmp r0, #0
	bne _080121CC
	mov r0, #0x10
	bl PlaySE
	and r4, r6
	mov r0, #2
	neg r0, r0
	ldr r2, [sp, #0]
	and r2, r0
	orr r2, r4
	sub r0, #0x1D
	and r2, r0
	ldr r0, _080121B4 @ =0x000001FF
	and r5, r0
	lsl r1, r5, #5
	ldr r0, _080121B8 @ =0xFFFFC01F
	and r2, r0
	orr r2, r1
	str r2, [sp, #0]
	ldr r3, [sp, #4]
	add r3, r8
	add r3, sl
	ldrb r7, [r3, #6]
	lsl r0, r7, #0x1F
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xE
	ldr r1, _080121BC @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xF
	ldr r2, _080121C0 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	mov r0, sp
	ldrb r1, [r0, #1]
	mov r0, #0x40
	and r0, r1
	ldr r1, _080121C4 @ =0x0868DB94
	cmp r0, #0
	beq _08012180
	ldr r1, _080121C8 @ =0x0868EC38
_08012180:
	mov r0, sp
	mov r2, #0
	mov r3, #0
	bl DuelAnim_PlayZoneEffect
	mov r0, r9
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r9
	b _0801224C
_080121A4: .4byte 0x020185C0
_080121A8: .4byte 0x00000D64
_080121AC: .4byte 0x0201930C
_080121B0: .4byte 0x0000080A
_080121B4: .4byte 0x000001FF
_080121B8: .4byte 0xFFFFC01F
_080121BC: .4byte 0xFFFFBFFF
_080121C0: .4byte 0xFFFF7FFF
_080121C4: .4byte gNegateAnim
_080121C8: .4byte gNegateAnimSideways
_080121CC:
	add r0, r6, #0
	ldrh r3, [r2, #4]
	and r0, r3
	cmp r0, #0
	beq _080121E0
	mov r0, #0x41
	neg r0, r0
	ldrb r7, [r1, #1]
	and r0, r7
	strb r0, [r1, #1]
_080121E0:
	mov r0, #2
	ldrh r3, [r2, #4]
	and r0, r3
	cmp r0, #0
	beq _080121F2
	mov r0, #0x7F
	ldrb r7, [r1, #1]
	and r0, r7
	strb r0, [r1, #1]
_080121F2:
	mov r0, #4
	ldrh r3, [r2, #4]
	and r0, r3
	cmp r0, #0
	beq _08012206
	mov r0, #2
	neg r0, r0
	ldrb r7, [r1, #2]
	and r0, r7
	strb r0, [r1, #2]
_08012206:
	mov r0, #8
	ldrh r3, [r2, #4]
	and r0, r3
	cmp r0, #0
	beq _0801221A
	mov r0, #3
	neg r0, r0
	ldrb r7, [r1, #2]
	and r0, r7
	strb r0, [r1, #2]
_0801221A:
	mov r0, #0x10
	ldrh r3, [r2, #4]
	and r0, r3
	cmp r0, #0
	beq _0801222E
	mov r0, #0x41
	neg r0, r0
	ldrb r7, [r1, #7]
	and r0, r7
	strb r0, [r1, #7]
_0801222E:
	mov r0, #0x20
	ldrh r3, [r2, #4]
	and r0, r3
	cmp r0, #0
	beq _08012240
	mov r0, #0x7F
	ldrb r7, [r1, #7]
	and r0, r7
	strb r0, [r1, #7]
_08012240:
	ldr r0, _08012260 @ =0x0000080D
	add r1, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0801224C:
	strb r0, [r1]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08012260: .4byte 0x0000080D
	thumb_func_end DuelCmd_ClearZoneStatusFlags

