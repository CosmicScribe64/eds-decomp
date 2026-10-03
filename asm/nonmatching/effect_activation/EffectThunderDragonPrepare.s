	thumb_func_start EffectThunderDragonPrepare
EffectThunderDragonPrepare: @ 0x0802D78C
	push {lr}
	add r3, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802D79A
	mov r0, #0
	b _0802D7BE
_0802D79A:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802D7C4 @ =0x000007FF
	ldrh r3, [r3]
	and r1, r3
	lsl r1, r1, #1
	ldr r2, _0802D7C8 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	mov r1, #0
	cmp r0, #0
	ble _0802D7BC
	mov r1, #1
_0802D7BC:
	add r0, r1, #0
_0802D7BE:
	pop {r1}
	bx r1
	.align 2, 0
_0802D7C4: .4byte 0x000007FF
_0802D7C8: .4byte gCardIdToNumber
	thumb_func_end EffectThunderDragonPrepare

