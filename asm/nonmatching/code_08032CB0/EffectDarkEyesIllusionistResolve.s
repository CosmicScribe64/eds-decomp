	thumb_func_start EffectDarkEyesIllusionistResolve
EffectDarkEyesIllusionistResolve: @ 0x08032CB0
	push {r4, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	bne _08032D00
	mov r2, #7
	ldrb r4, [r3, #0xA]
	and r2, r4
	cmp r2, #1
	bne _08032D00
	ldrh r0, [r3, #0xC]
	lsr r1, r0, #8
	ldrb r4, [r3, #0xC]
	and r2, r4
	mov r0, #0x94
	mul r0, r1
	ldr r1, _08032D08 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08032D0C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032D00
	ldrb r0, [r3, #2]
	lsl r1, r0, #0x1F
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
_08032D00:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08032D08: .4byte 0x00000D64
_08032D0C: .4byte 0x0201930C
	thumb_func_end EffectDarkEyesIllusionistResolve

