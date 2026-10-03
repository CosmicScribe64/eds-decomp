	thumb_func_start EffectLightforceSwordResolve
EffectLightforceSwordResolve: @ 0x08033AAC
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08033ADC
	ldr r2, _08033AE4 @ =0x020192E4
	ldrb r1, [r1, #2]
	lsl r4, r1, #0x1F
	lsr r0, r4, #0x1F
	mov r3, #1
	sub r0, r3, r0
	and r0, r3
	ldr r1, _08033AE8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08033ADC
	lsr r0, r4, #0x1F
	sub r0, r3, r0
	bl DuelPrompt_PostRandomBanishFaceDown
_08033ADC:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08033AE4: .4byte 0x020192E4
_08033AE8: .4byte 0x00000D64
	thumb_func_end EffectLightforceSwordResolve

