	thumb_func_start EffectTwoFreeMonsterZonesPrepare
EffectTwoFreeMonsterZonesPrepare: @ 0x0802F8E8
	push {r4, lr}
	mov r0, #0
	bl CountFreeMonsterZones
	add r4, r0, #0
	mov r0, #1
	bl CountFreeMonsterZones
	mov r1, #0
	add r4, r4, r0
	cmp r4, #1
	ble _0802F902
	mov r1, #1
_0802F902:
	add r0, r1, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectTwoFreeMonsterZonesPrepare
	.align 2, 0

