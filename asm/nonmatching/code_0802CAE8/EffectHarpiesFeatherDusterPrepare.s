	thumb_func_start EffectHarpiesFeatherDusterPrepare
EffectHarpiesFeatherDusterPrepare: @ 0x0802D958
	push {lr}
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl CountSpellTrapsFiltered
	mov r1, #0
	cmp r0, #0
	ble _0802D976
	mov r1, #1
_0802D976:
	add r0, r1, #0
	pop {r1}
	bx r1
	thumb_func_end EffectHarpiesFeatherDusterPrepare

