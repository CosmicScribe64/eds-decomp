	thumb_func_start EffectHandMonsterAndGraveTargetPrepare
EffectHandMonsterAndGraveTargetPrepare: @ 0x0802FB14
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountHandMonsters
	cmp r0, #0
	beq _0802FB38
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802FB3C @ =0x0000058D
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bgt _0802FB40
_0802FB38:
	mov r0, #0
	b _0802FB42
_0802FB3C: .4byte 0x0000058D
_0802FB40:
	mov r0, #1
_0802FB42:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectHandMonsterAndGraveTargetPrepare

