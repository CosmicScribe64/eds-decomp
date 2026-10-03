	thumb_func_start EffectBlockTwoMonsterZonesResolve
EffectBlockTwoMonsterZonesResolve: @ 0x0803A6B0
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803A710
	mov r0, #7
	ldrb r3, [r5, #0xA]
	and r0, r3
	cmp r0, #2
	bne _0803A710
	add r4, r5, #0
	add r4, #0xC
	mov r6, #1
_0803A6CE:
	ldrh r0, [r4]
	lsr r1, r0, #8
	mov r2, #1
	ldrb r3, [r4]
	and r2, r3
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0803A718 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803A71C @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0803A708
	ldrb r0, [r5, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	ldrh r3, [r5, #2]
	lsl r2, r3, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r4]
	mov r3, #2
	bl QueueAddZoneLink
_0803A708:
	add r4, #2
	sub r6, #1
	cmp r6, #0
	bge _0803A6CE
_0803A710:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0803A718: .4byte 0x00000D64
_0803A71C: .4byte 0x0201930C
	thumb_func_end EffectBlockTwoMonsterZonesResolve

