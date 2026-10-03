	thumb_func_start IsMaterialOfFusion
IsMaterialOfFusion: @ 0x0803CDEC
	push {r4, lr}
	ldr r3, _0803CE70 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _0803CE74 @ =0x08622AB4
	add r0, r0, r2
	ldrh r4, [r0]
	and r1, r3
	lsl r1, r1, #1
	add r1, r1, r2
	ldrh r1, [r1]
	ldr r2, _0803CE78 @ =0x000007CF
	cmp r4, r2
	bls _0803CE10
	ldr r3, _0803CE7C @ =0xFFFFF830
	add r0, r4, r3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_0803CE10:
	cmp r1, r2
	bls _0803CE1C
	ldr r2, _0803CE7C @ =0xFFFFF830
	add r0, r1, r2
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_0803CE1C:
	mov r2, #0
	ldr r0, _0803CE80 @ =0x0819A7C8
_0803CE20:
	ldrh r3, [r0]
	cmp r4, r3
	bne _0803CE32
	ldrh r3, [r0, #2]
	cmp r3, r1
	beq _0803CE88
	ldrh r3, [r0, #4]
	cmp r3, r1
	beq _0803CE88
_0803CE32:
	add r0, #8
	add r2, #1
	cmp r2, #0x34
	bls _0803CE20
	mov r2, #0
	ldr r0, _0803CE84 @ =0x0819A970
_0803CE3E:
	ldrh r3, [r0]
	cmp r4, r3
	bne _0803CE56
	ldrh r3, [r0, #2]
	cmp r3, r1
	beq _0803CE88
	ldrh r3, [r0, #4]
	cmp r3, r1
	beq _0803CE88
	ldrh r3, [r0, #6]
	cmp r3, r1
	beq _0803CE88
_0803CE56:
	add r0, #8
	add r2, #1
	cmp r2, #3
	bls _0803CE3E
	add r0, r1, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803CE88
	mov r0, #0
	b _0803CE8A
	.align 2, 0
_0803CE70: .4byte 0x000007FF
_0803CE74: .4byte gCardIdToNumber
_0803CE78: .4byte 0x000007CF
_0803CE7C: .4byte 0xFFFFF830
_0803CE80: .4byte gFusionRecipes2
_0803CE84: .4byte gFusionRecipes3
_0803CE88:
	mov r0, #1
_0803CE8A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end IsMaterialOfFusion

