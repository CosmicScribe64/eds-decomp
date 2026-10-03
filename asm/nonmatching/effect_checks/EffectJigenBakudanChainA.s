	thumb_func_start EffectJigenBakudanChainA
EffectJigenBakudanChainA: @ 0x0802C890
	push {lr}
	add r1, r0, #0
	mov r0, #0xFC
	ldrb r2, [r1, #3]
	and r0, r2
	cmp r0, #8
	bne _0802C8AE
	ldrb r2, [r1, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r1, #2]
	lsl r1, r1, #0x16
	lsr r1, r1, #0x1A
	bl TributeMonster
_0802C8AE:
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end EffectJigenBakudanChainA

