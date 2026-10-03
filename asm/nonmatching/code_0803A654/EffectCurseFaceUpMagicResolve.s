	thumb_func_start EffectCurseFaceUpMagicResolve
EffectCurseFaceUpMagicResolve: @ 0x0803B06C
	push {r4, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	bne _0803B0FC
	mov r2, #7
	ldrb r4, [r3, #0xA]
	and r2, r4
	cmp r2, #1
	bne _0803B0F4
	ldrh r0, [r3, #0xC]
	lsr r1, r0, #8
	ldrb r4, [r3, #0xC]
	and r2, r4
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0803B0E4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803B0E8 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0803B0F4
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803B0F4
	ldr r0, _0803B0EC @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0803B0F0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0803B0F4
	ldrb r4, [r3, #2]
	lsl r1, r4, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	ldrh r4, [r3, #2]
	lsl r2, r4, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r3, #0xC]
	mov r3, #2
	bl QueueAddZoneLink
	b _0803B0FC
	.align 2, 0
_0803B0E4: .4byte 0x00000D64
_0803B0E8: .4byte 0x0201930C
_0803B0EC: .4byte 0x000007FF
_0803B0F0: .4byte gCardStats
_0803B0F4:
	mov r0, #8
	ldrb r1, [r3, #4]
	orr r0, r1
	strb r0, [r3, #4]
_0803B0FC:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectCurseFaceUpMagicResolve

