	thumb_func_start EffectMagicArmShieldResolve
EffectMagicArmShieldResolve: @ 0x08033FB0
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r2, [r4, #4]
	and r0, r2
	cmp r0, #0
	bne _08034034
	add r0, r4, #0
	mov r2, #0
	bl EffectAttackResponsePrepare
	cmp r0, #0
	beq _08034034
	mov r5, #7
	ldrb r0, [r4, #0xA]
	and r5, r0
	cmp r5, #1
	bne _08034034
	ldrh r1, [r4, #0xC]
	add r0, r4, #0
	bl EffectMagicArmShieldCheck
	cmp r0, #0
	beq _08034034
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	ldrb r2, [r4, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	strh r1, [r4, #0xE]
	add r0, r5, #0
	and r0, r2
	mov r2, #0xA2
	cmp r0, #0
	beq _08034004
	ldr r2, _0803403C @ =0x000080A2
_08034004:
	ldrh r1, [r4, #0xC]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r4, #0xC]
	ldrh r2, [r4, #0xE]
	bl MoveFieldCard
	ldrb r0, [r4, #2]
	and r5, r0
	mov r0, #0x38
	cmp r5, #0
	beq _0803402A
	ldr r0, _08034040 @ =0x00008038
_0803402A:
	ldrh r1, [r4, #0xE]
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08034034:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
_0803403C: .4byte 0x000080A2
_08034040: .4byte 0x00008038
	thumb_func_end EffectMagicArmShieldResolve

