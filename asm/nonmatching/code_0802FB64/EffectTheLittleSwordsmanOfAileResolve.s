	thumb_func_start EffectTheLittleSwordsmanOfAileResolve
EffectTheLittleSwordsmanOfAileResolve: @ 0x080307D4
	push {r4, r5, lr}
	add r4, r0, #0
	mov r2, #7
	ldrb r0, [r4, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08030826
	ldrb r5, [r4, #0xC]
	ldrh r0, [r4, #0xC]
	lsr r3, r0, #8
	and r2, r5
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08030830 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08030834 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08030826
	add r0, r5, #0
	add r1, r3, #0
	bl TributeMonster
	cmp r0, #0
	beq _08030826
	ldrb r0, [r4, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r4]
	add r2, r0, #0
	ldrh r4, [r4, #2]
	lsl r3, r4, #0x16
	lsr r3, r3, #0x1A
	lsl r3, r3, #8
	orr r2, r3
	mov r3, #3
	bl QueueAddZoneLink
_08030826:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08030830: .4byte 0x00000D64
_08030834: .4byte 0x0201930C
	thumb_func_end EffectTheLittleSwordsmanOfAileResolve

