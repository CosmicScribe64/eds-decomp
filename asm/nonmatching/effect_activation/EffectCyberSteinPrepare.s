	thumb_func_start EffectCyberSteinPrepare
EffectCyberSteinPrepare: @ 0x0802D6D4
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r0, _0802D6F8 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0802D6FC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802D700 @ =0x000001A3
	cmp r1, r0
	beq _0802D704
	add r0, #0x56
	cmp r1, r0
	beq _0802D750
_0802D6F2:
	mov r0, #0
	b _0802D77A
	.align 2, 0
_0802D6F8: .4byte 0x000007FF
_0802D6FC: .4byte gCardIdToNumber
_0802D700: .4byte 0x000001A3
_0802D704:
	ldr r6, _0802D744 @ =0x020192E4
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	mov r2, #1
	lsr r0, r3, #0x1F
	ldr r5, _0802D748 @ =0x00000D64
	mul r0, r5
	add r0, r0, r6
	ldr r1, _0802D74C @ =0x00001387
	ldrh r0, [r0]
	cmp r0, r1
	bls _0802D6F2
	lsr r0, r3, #0x1F
	and r2, r0
	add r0, r2, #0
	mul r0, r5
	add r0, r0, r6
	ldrb r0, [r0, #5]
	cmp r0, #0
	beq _0802D6F2
	lsr r0, r3, #0x1F
	bl CanSpecialSummon
	cmp r0, #0
	beq _0802D6F2
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	b _0802D774
	.align 2, 0
_0802D744: .4byte 0x020192E4
_0802D748: .4byte 0x00000D64
_0802D74C: .4byte 0x00001387
_0802D750:
	ldr r5, _0802D780 @ =0x020192E4
	ldrb r4, [r4, #2]
	lsl r3, r4, #0x1F
	mov r2, #1
	lsr r0, r3, #0x1F
	ldr r4, _0802D784 @ =0x00000D64
	mul r0, r4
	add r0, r0, r5
	ldr r1, _0802D788 @ =0x00000BB7
	ldrh r0, [r0]
	cmp r0, r1
	bls _0802D6F2
	lsr r0, r3, #0x1F
	and r2, r0
	add r0, r2, #0
	mul r0, r4
	add r0, r0, r5
	ldrb r0, [r0, #5]
_0802D774:
	cmp r0, #0
	beq _0802D6F2
	mov r0, #1
_0802D77A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0802D780: .4byte 0x020192E4
_0802D784: .4byte 0x00000D64
_0802D788: .4byte 0x00000BB7
	thumb_func_end EffectCyberSteinPrepare

