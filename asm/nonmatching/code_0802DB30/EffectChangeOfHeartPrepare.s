	thumb_func_start EffectChangeOfHeartPrepare
EffectChangeOfHeartPrepare: @ 0x0802E4B0
	push {r4, lr}
	add r4, r0, #0
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	bl CountMonsters
	cmp r0, #0
	beq _0802E4D8
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	beq _0802E4D8
	mov r0, #1
	b _0802E4DA
_0802E4D8:
	mov r0, #0
_0802E4DA:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectChangeOfHeartPrepare

