	thumb_func_start EffectGreenkappaPrepare
EffectGreenkappaPrepare: @ 0x0802D800
	push {r4, lr}
	mov r0, #0
	mov r1, #0
	mov r2, #1
	mov r3, #1
	bl CountSpellTrapsFiltered
	add r4, r0, #0
	mov r0, #1
	mov r1, #0
	mov r2, #1
	mov r3, #1
	bl CountSpellTrapsFiltered
	add r4, r4, r0
	mov r0, #0
	cmp r4, #1
	ble _0802D826
	mov r0, #1
_0802D826:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectGreenkappaPrepare

