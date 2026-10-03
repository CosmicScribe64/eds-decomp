	thumb_func_start AddEffectTargetUnchecked
AddEffectTargetUnchecked: @ 0x0803DE40
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	mov r9, r1
	add r5, r2, #0
	mov r7, #0
	add r4, r5, #0
	cmp r5, #4
	ble _0803DE5A
	mov r7, #5
	sub r4, r5, #5
_0803DE5A:
	cmp r5, #0xA
	bne _0803DE62
	mov r7, #0xA
	mov r4, #0
_0803DE62:
	mov r0, #1
	mov r8, r0
	ldrb r1, [r6, #2]
	and r0, r1
	cmp r0, #0
	bne _0803DE74
	mov r0, #1
	bl PlaySE
_0803DE74:
	mov r0, r8
	ldrb r1, [r6, #2]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803DE82
	ldr r3, _0803DEB4 @ =0x00008008
_0803DE82:
	mov r0, r9
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	lsl r2, r4, #0x18
	lsr r2, r2, #0x10
	orr r2, r7
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, r9
	lsl r1, r0, #0x18
	lsl r0, r5, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	add r0, r6, #0
	bl AddEffectTarget
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0803DEB4: .4byte 0x00008008
	thumb_func_end AddEffectTargetUnchecked

