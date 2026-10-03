	thumb_func_start EffectAddStatModifierResolve
EffectAddStatModifierResolve: @ 0x08034708
	push {r4, lr}
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _08034758
	mov r3, #7
	ldrb r0, [r2, #0xA]
	and r3, r0
	cmp r3, #1
	bne _08034758
	ldrh r4, [r2, #0xC]
	lsr r1, r4, #8
	ldrb r0, [r2, #0xC]
	and r3, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _08034760 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08034764 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r3, [r1, #6]
	and r0, r3
	cmp r0, #0
	beq _08034758
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08034758
	ldrb r1, [r2, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r2]
	add r2, r4, #0
	mov r3, #3
	bl QueueAddZoneLink
_08034758:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08034760: .4byte 0x00000D64
_08034764: .4byte 0x0201930C
	thumb_func_end EffectAddStatModifierResolve

