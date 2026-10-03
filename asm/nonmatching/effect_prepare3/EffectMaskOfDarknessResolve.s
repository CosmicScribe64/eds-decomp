	thumb_func_start EffectMaskOfDarknessResolve
EffectMaskOfDarknessResolve: @ 0x08030578
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _080305D6
	mov r0, #7
	ldrb r2, [r4, #0xA]
	and r0, r2
	cmp r0, #2
	bne _080305D6
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r2, [r4, #0xC]
	lsl r1, r2, #0x14
	lsr r5, r1, #0x14
	ldr r1, _080305E0 @ =0x000007FF
	and r1, r5
	lsl r1, r1, #1
	ldr r2, _080305E4 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	ble _080305D6
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl sub_08019820
	mov r0, #1
	ldrb r2, [r4, #2]
	and r0, r2
	mov r3, #0xD2
	cmp r0, #0
	beq _080305CA
	ldr r3, _080305E8 @ =0x000080D2
_080305CA:
	ldrh r1, [r4, #0xC]
	mov r2, #0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
_080305D6:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_080305E0: .4byte 0x000007FF
_080305E4: .4byte gCardIdToNumber
_080305E8: .4byte 0x000080D2
	thumb_func_end EffectMaskOfDarknessResolve

