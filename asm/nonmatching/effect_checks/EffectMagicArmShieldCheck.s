	thumb_func_start EffectMagicArmShieldCheck
EffectMagicArmShieldCheck: @ 0x0802BE70
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r0, r1, #0x10
	add r7, r0, #0
	lsl r0, r7, #0x18
	lsr r4, r0, #0x18
	lsr r3, r1, #0x18
	cmp r3, #4
	bgt _0802BEDC
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r4
	beq _0802BEDC
	mov r0, #1
	mov r8, r0
	add r1, r4, #0
	and r1, r0
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802BED4 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BED8 @ =0x0201930C
	add r6, r2, r0
	ldr r0, [r6]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BEDC
	ldrh r0, [r5]
	add r1, r4, #0
	add r2, r3, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802BEDC
	ldrh r5, [r5, #6]
	cmp r7, r5
	beq _0802BEDC
	ldrb r6, [r6, #6]
	lsr r0, r6, #1
	mov r1, r8
	and r0, r1
	b _0802BEDE
	.align 2, 0
_0802BED4: .4byte 0x00000D64
_0802BED8: .4byte 0x0201930C
_0802BEDC:
	mov r0, #0
_0802BEDE:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectMagicArmShieldCheck

