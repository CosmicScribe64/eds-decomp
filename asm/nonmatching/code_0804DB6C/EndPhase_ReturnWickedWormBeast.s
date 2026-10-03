	thumb_func_start EndPhase_ReturnWickedWormBeast
EndPhase_ReturnWickedWormBeast: @ 0x0804E538
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _0804E5A0 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_0804E548:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0804E5A4 @ =0x0201930C
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r1, r2, #0
	cmp r2, #0
	beq _0804E594
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _0804E594
	ldr r3, _0804E5A8 @ =0x000007FF
	add r0, r3, #0
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _0804E5AC @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, #0x16
	bne _0804E594
	mov r0, #0x73
	cmp r5, #0
	beq _0804E582
	ldr r0, _0804E5B0 @ =0x00008073
_0804E582:
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	bl ReturnFieldCardToHand
_0804E594:
	add r4, #1
	cmp r4, #4
	ble _0804E548
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0804E5A0: .4byte 0x00000D64
_0804E5A4: .4byte 0x0201930C
_0804E5A8: .4byte 0x000007FF
_0804E5AC: .4byte gCardIdToNumber
_0804E5B0: .4byte 0x00008073
	thumb_func_end EndPhase_ReturnWickedWormBeast

