	thumb_func_start EffectBlastJugglerCheck
EffectBlastJugglerCheck: @ 0x0802B9EC
	push {r4, r5, r6, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r4, r0, #0x18
	lsr r5, r1, #0x18
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _0802BA58 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BA5C @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BA52
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802BA52
	add r0, r4, #0
	add r1, r5, #0
	bl GetZoneCardAtk
	mov r1, #0xFA
	lsl r1, r1, #2
	cmp r0, r1
	bgt _0802BA52
	ldrh r0, [r6]
	add r1, r4, #0
	add r2, r5, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802BA52
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r4, r0
	bne _0802BA60
	ldrh r6, [r6, #2]
	lsl r0, r6, #0x16
	lsr r0, r0, #0x1A
	cmp r5, r0
	bne _0802BA60
_0802BA52:
	mov r0, #0
	b _0802BA62
	.align 2, 0
_0802BA58: .4byte 0x00000D64
_0802BA5C: .4byte 0x0201930C
_0802BA60:
	mov r0, #1
_0802BA62:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectBlastJugglerCheck

