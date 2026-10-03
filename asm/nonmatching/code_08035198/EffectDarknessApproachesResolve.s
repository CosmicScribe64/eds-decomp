	thumb_func_start EffectDarknessApproachesResolve
EffectDarknessApproachesResolve: @ 0x08035FDC
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08036020
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08036020
	ldrb r4, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r3, r1, #8
	and r2, r4
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _08036028 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803602C @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08036020
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #0
	bl FlipFieldCard
_08036020:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08036028: .4byte 0x00000D64
_0803602C: .4byte 0x0201930C
	thumb_func_end EffectDarknessApproachesResolve

