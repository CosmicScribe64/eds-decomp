	thumb_func_start EffectSummonTokensInFreeZonesResolve
EffectSummonTokensInFreeZonesResolve: @ 0x080397A0
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _080397EC
	mov r5, #0
_080397B0:
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl IsMonsterZoneFree
	cmp r0, #0
	beq _080397E6
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xA3
	cmp r0, #0
	beq _080397CE
	ldr r2, _080397F4 @ =0x000080A3
_080397CE:
	lsl r1, r5, #0x18
	lsr r1, r1, #0x18
	ldrh r3, [r4, #2]
	lsl r0, r3, #0x16
	lsr r0, r0, #0x1A
	lsl r0, r0, #8
	orr r1, r0
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_080397E6:
	add r5, #1
	cmp r5, #4
	ble _080397B0
_080397EC:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
_080397F4: .4byte 0x000080A3
	thumb_func_end EffectSummonTokensInFreeZonesResolve

