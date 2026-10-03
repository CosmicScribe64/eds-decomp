	thumb_func_start EffectToonWorldChainA
EffectToonWorldChainA: @ 0x0802C974
	push {r4, r5, lr}
	add r4, r0, #0
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0x43
	cmp r0, #0
	beq _0802C988
	ldr r2, _0802C9BC @ =0x00008043
_0802C988:
	mov r1, #0xFA
	lsl r1, r1, #2
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xB3
	cmp r0, #0
	beq _0802C9A4
	ldr r2, _0802C9C0 @ =0x000080B3
_0802C9A4:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
	pop {r4, r5}
	pop {r1}
	bx r1
_0802C9BC: .4byte 0x00008043
_0802C9C0: .4byte 0x000080B3
	thumb_func_end EffectToonWorldChainA

