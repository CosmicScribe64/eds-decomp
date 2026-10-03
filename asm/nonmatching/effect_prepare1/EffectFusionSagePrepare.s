	thumb_func_start EffectFusionSagePrepare
EffectFusionSagePrepare: @ 0x0802E6C4
	push {lr}
	add r2, r0, #0
	ldrb r1, [r2, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E6F0 @ =0x000007FF
	ldrh r2, [r2]
	and r1, r2
	lsl r1, r1, #1
	ldr r2, _0802E6F4 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	mov r1, #0
	cmp r0, #0
	ble _0802E6EA
	mov r1, #1
_0802E6EA:
	add r0, r1, #0
	pop {r1}
	bx r1
_0802E6F0: .4byte 0x000007FF
_0802E6F4: .4byte gCardIdToNumber
	thumb_func_end EffectFusionSagePrepare

