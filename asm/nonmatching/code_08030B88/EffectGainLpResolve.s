	thumb_func_start EffectGainLpResolve
EffectGainLpResolve: @ 0x08031180
	push {r4, r5, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _080311FE
	ldr r0, _080311B0 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080311B4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080311B8 @ =0x00000155
	cmp r1, r0
	beq _080311D4
	cmp r1, r0
	bgt _080311BC
	sub r0, #3
	cmp r1, r0
	beq _080311C8
	b _080311FE
	.align 2, 0
_080311B0: .4byte 0x000007FF
_080311B4: .4byte gCardIdToNumber
_080311B8: .4byte 0x00000155
_080311BC:
	ldr r0, _080311C4 @ =0x00000523
	cmp r1, r0
	beq _080311DC
	b _080311FE
_080311C4: .4byte 0x00000523
_080311C8:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xFA
	lsl r1, r1, #1
	b _080311FA
_080311D4:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	b _080311F6
_080311DC:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r4, #0xFA
	lsl r4, r4, #2
	add r1, r4, #0
	bl GainLifePoints
	ldrb r5, [r5, #2]
	lsl r1, r5, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
_080311F6:
	mov r1, #0xFA
	lsl r1, r1, #2
_080311FA:
	bl GainLifePoints
_080311FE:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectGainLpResolve
	.align 2, 0

