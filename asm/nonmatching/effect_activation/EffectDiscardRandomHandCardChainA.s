	thumb_func_start EffectDiscardRandomHandCardChainA
EffectDiscardRandomHandCardChainA: @ 0x0802CCF0
	push {r4, lr}
	add r1, r0, #0
	ldr r0, _0802CD08 @ =0x02017A40
	mov r2, #0xF9
	lsl r2, r2, #2
	add r4, r0, r2
	ldrb r0, [r4]
	cmp r0, #0
	beq _0802CD0C
	mov r0, #1
	b _0802CD22
	.align 2, 0
_0802CD08: .4byte 0x02017A40
_0802CD0C:
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0
	mov r2, #1
	bl DuelPrompt_PostRandomDiscard
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_0802CD22:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectDiscardRandomHandCardChainA

