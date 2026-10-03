	thumb_func_start EffectDestroySpecialSummonedResolve
EffectDestroySpecialSummonedResolve: @ 0x0803B6C0
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803B7B0
	ldr r6, _0803B734 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r3, r6, r2
	ldrb r0, [r3]
	cmp r0, #0x7F
	beq _0803B6F6
	ldrb r2, [r5, #2]
	cmp r0, #0x80
	bne _0803B756
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	ldr r2, _0803B738 @ =0x000003E1
	add r0, r6, r2
	strb r1, [r0]
	ldrb r0, [r3]
	sub r0, #1
	strb r0, [r3]
_0803B6F6:
	mov r4, #0
	ldr r0, _0803B738 @ =0x000003E1
	add r6, r6, r0
_0803B6FC:
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r6]
	orr r1, r2
	add r0, r5, #0
	bl EffectSpecialSummonedMonsterCheck
	cmp r0, #0
	bne _0803B73C
	add r4, #1
	cmp r4, #4
	ble _0803B6FC
	ldr r1, _0803B734 @ =0x02017A40
	ldr r0, _0803B738 @ =0x000003E1
	add r1, r1, r0
	mov r0, #1
	ldrb r2, [r1]
	sub r0, r0, r2
	strb r0, [r1]
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrb r1, [r1]
	cmp r1, r0
	bne _0803B756
	mov r0, #0x7F
	b _0803B7B2
	.align 2, 0
_0803B734: .4byte 0x02017A40
_0803B738: .4byte 0x000003E1
_0803B73C:
	ldrb r0, [r6]
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldrb r1, [r6]
	add r2, r4, #0
	bl OnCardDestroyedByEffect
	mov r0, #0x7F
	b _0803B7B2
_0803B756:
	mov r4, #1
	add r0, r4, #0
	and r0, r2
	mov r3, #0x49
	cmp r0, #0
	beq _0803B764
	ldr r3, _0803B7B8 @ =0x00008049
_0803B764:
	ldr r7, _0803B7BC @ =0x020192E4
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	and r1, r0
	ldr r6, _0803B7C0 @ =0x00000D64
	add r0, r1, #0
	mul r0, r6
	add r0, r0, r7
	ldrb r0, [r0, #7]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1F
	add r0, r3, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r5, #2]
	add r0, r4, #0
	and r0, r1
	mov r2, #0x49
	cmp r0, #0
	bne _0803B794
	ldr r2, _0803B7B8 @ =0x00008049
_0803B794:
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r4, r0
	and r0, r4
	mul r0, r6
	add r0, r0, r7
	ldrb r0, [r0, #7]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1F
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_0803B7B0:
	mov r0, #0
_0803B7B2:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803B7B8: .4byte 0x00008049
_0803B7BC: .4byte 0x020192E4
_0803B7C0: .4byte 0x00000D64
	thumb_func_end EffectDestroySpecialSummonedResolve

