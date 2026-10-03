	thumb_func_start EffectTributeTwoMonstersPrepare
EffectTributeTwoMonstersPrepare: @ 0x0802FC38
	push {lr}
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	bl CountTributableMonsters
	cmp r0, #1
	bgt _0802FC50
	mov r0, #0
	b _0802FC52
_0802FC50:
	mov r0, #1
_0802FC52:
	pop {r1}
	bx r1
	thumb_func_end EffectTributeTwoMonstersPrepare
	.align 2, 0

