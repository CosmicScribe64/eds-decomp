	thumb_func_start InflictBattleDamage
InflictBattleDamage: @ 0x08019894
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r4, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	mov r0, #1
	sub r0, r0, r6
	ldr r1, _08019934 @ =0x00000415
	mov r9, r1
	bl CountActiveCardsOnField
	add r5, r0, #0
	cmp r4, #0
	beq _08019974
	mov r0, #0x43
	cmp r6, #0
	beq _080198C4
	ldr r0, _08019938 @ =0x00008043
_080198C4:
	lsl r4, r4, #0x10
	lsr r1, r4, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	cmp r5, #0
	ble _080198FA
	mov r2, #0x73
	cmp r6, #1
	beq _080198DC
	ldr r2, _0801993C @ =0x00008073
_080198DC:
	mov r1, r9
	lsl r0, r1, #1
	ldr r1, _08019940 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r6, #0
	mov r1, #1
	add r2, r5, #0
	bl DuelPrompt_PostRandomDiscard
_080198FA:
	lsl r0, r7, #0x18
	lsr r0, r0, #0x18
	cmp r0, r6
	bne _08019944
	lsr r2, r4, #0x10
	mov r5, #0xF
	add r3, r7, #0
	and r3, r5
	lsr r0, r7, #8
	mov r4, #0xF
	and r0, r4
	lsl r0, r0, #4
	orr r3, r0
	mov r1, r8
	and r1, r5
	mov r5, r8
	lsr r0, r5, #8
	and r0, r4
	lsl r0, r0, #4
	orr r1, r0
	lsl r1, r1, #8
	orr r3, r1
	lsl r3, r3, #0x10
	orr r2, r3
	add r0, r6, #0
	mov r1, #0xD
	bl EventResponse_Request
	b _08019974
_08019934: .4byte 0x00000415
_08019938: .4byte 0x00008043
_0801993C: .4byte 0x00008073
_08019940: .4byte gCardNumberToId
_08019944:
	lsr r2, r4, #0x10
	mov r5, #0xF
	mov r3, r8
	and r3, r5
	mov r1, r8
	lsr r0, r1, #8
	mov r4, #0xF
	and r0, r4
	lsl r0, r0, #4
	orr r3, r0
	add r1, r7, #0
	and r1, r5
	lsr r0, r7, #8
	and r0, r4
	lsl r0, r0, #4
	orr r1, r0
	lsl r1, r1, #8
	orr r3, r1
	lsl r3, r3, #0x10
	orr r2, r3
	add r0, r6, #0
	mov r1, #0xE
	bl EventResponse_Request
_08019974:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end InflictBattleDamage

