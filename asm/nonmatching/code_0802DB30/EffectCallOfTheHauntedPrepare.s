	thumb_func_start EffectCallOfTheHauntedPrepare
EffectCallOfTheHauntedPrepare: @ 0x0802EB00
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CanSpecialSummon
	cmp r0, #0
	beq _0802EB20
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _0802EB24
_0802EB20:
	mov r0, #0
	b _0802EB48
_0802EB24:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802EB50 @ =0x000007FF
	ldrh r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _0802EB54 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	mov r1, #0
	cmp r0, #0
	ble _0802EB46
	mov r1, #1
_0802EB46:
	add r0, r1, #0
_0802EB48:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802EB50: .4byte 0x000007FF
_0802EB54: .4byte gCardIdToNumber
	thumb_func_end EffectCallOfTheHauntedPrepare

