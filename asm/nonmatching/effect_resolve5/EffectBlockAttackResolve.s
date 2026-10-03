	thumb_func_start EffectBlockAttackResolve
EffectBlockAttackResolve: @ 0x08035614
	push {r4, r5, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08035658
	mov r3, #7
	ldrb r0, [r1, #0xA]
	and r3, r0
	cmp r3, #1
	bne _08035658
	ldrb r5, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r4, r1, #8
	add r2, r5, #0
	and r2, r3
	mov r0, #0x94
	mul r0, r4
	ldr r1, _08035660 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08035664 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	and r3, r0
	cmp r3, #0
	bne _08035658
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl ChangeBattlePosition
_08035658:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
_08035660: .4byte 0x00000D64
_08035664: .4byte 0x0201930C
	thumb_func_end EffectBlockAttackResolve

