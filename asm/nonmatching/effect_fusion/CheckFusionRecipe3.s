	thumb_func_start CheckFusionRecipe3
CheckFusionRecipe3: @ 0x0803CC18
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r7, _0803CD08 @ =0x0819A970
	ldr r5, _0803CD0C @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r4, _0803CD10 @ =0x08622AB4
	add r0, r0, r4
	ldrh r0, [r0]
	mov r9, r0
	and r1, r5
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	mov r8, r1
	and r2, r5
	lsl r2, r2, #1
	add r2, r2, r4
	ldrh r6, [r2]
	and r3, r5
	lsl r3, r3, #1
	add r3, r3, r4
	ldrh r4, [r3]
	mov r0, r8
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC64
	add r0, r6, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC64
	b _0803CDD6
_0803CC64:
	mov r0, r8
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC7E
	add r0, r4, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC7E
	b _0803CDD6
_0803CC7E:
	add r0, r6, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC98
	add r0, r4, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CC98
	b _0803CDD6
_0803CC98:
	ldr r1, [r7]
	ldr r0, _0803CD14 @ =0x03E703E7
	cmp r1, r0
	bne _0803CCAA
	ldr r0, _0803CD18 @ =0x000003E7
	ldrh r1, [r7, #4]
	cmp r1, r0
	bne _0803CCAA
	b _0803CDD6
_0803CCAA:
	ldrh r5, [r7]
	cmp r5, r9
	beq _0803CCB2
	b _0803CDDA
_0803CCB2:
	ldrh r0, [r7, #2]
	add r2, r0, #0
	add r3, r0, #0
	cmp r8, r2
	bne _0803CD1C
	ldrh r0, [r7, #4]
	ldrh r1, [r7, #6]
	cmp r6, r0
	bne _0803CCCA
	cmp r4, r1
	bne _0803CCCA
	b _0803CDD2
_0803CCCA:
	cmp r6, r1
	bne _0803CCD4
	cmp r4, r0
	bne _0803CCD4
	b _0803CDD2
_0803CCD4:
	add r0, r6, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CCEC
	ldrh r0, [r7, #4]
	cmp r4, r0
	beq _0803CDD2
	ldrh r1, [r7, #6]
	cmp r4, r1
	beq _0803CDD2
_0803CCEC:
	add r0, r4, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CDD6
	ldrh r5, [r7, #4]
_0803CCFA:
	cmp r6, r5
	beq _0803CDD2
	ldrh r7, [r7, #6]
_0803CD00:
	cmp r6, r7
	beq _0803CDD2
	b _0803CDD6
	.align 2, 0
_0803CD08: .4byte gFusionRecipes3
_0803CD0C: .4byte 0x000007FF
_0803CD10: .4byte gCardIdToNumber
_0803CD14: .4byte 0x03E703E7
_0803CD18: .4byte 0x000003E7
_0803CD1C:
	ldrh r0, [r7, #4]
	add r1, r0, #0
	cmp r8, r1
	bne _0803CD62
	ldrh r1, [r7, #6]
	cmp r6, r2
	bne _0803CD2E
	cmp r4, r1
	beq _0803CDD2
_0803CD2E:
	cmp r6, r1
	bne _0803CD3A
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	cmp r4, r0
	beq _0803CDD2
_0803CD3A:
	add r0, r6, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CD52
	ldrh r0, [r7, #2]
	cmp r4, r0
	beq _0803CDD2
	ldrh r1, [r7, #6]
	cmp r4, r1
	beq _0803CDD2
_0803CD52:
	add r0, r4, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CDD6
	ldrh r5, [r7, #2]
	b _0803CCFA
_0803CD62:
	ldrh r5, [r7, #6]
	cmp r8, r5
	bne _0803CDAA
	cmp r6, r2
	bne _0803CD70
	cmp r4, r1
	beq _0803CDD2
_0803CD70:
	cmp r6, r0
	bne _0803CD7C
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	cmp r4, r0
	beq _0803CDD2
_0803CD7C:
	add r0, r6, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CD94
	ldrh r0, [r7, #2]
	cmp r4, r0
	beq _0803CDD2
	ldrh r1, [r7, #4]
	cmp r4, r1
	beq _0803CDD2
_0803CD94:
	add r0, r4, #0
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CDD6
	ldrh r5, [r7, #2]
	cmp r6, r5
	beq _0803CDD2
	ldrh r7, [r7, #4]
	b _0803CD00
_0803CDAA:
	mov r0, r8
	bl IsFusionSubstitute
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803CDDA
	ldrh r0, [r7, #4]
	ldrh r1, [r7, #2]
	cmp r6, r1
	bne _0803CDC8
	cmp r4, r0
	beq _0803CDD2
	ldrh r5, [r7, #6]
	cmp r4, r5
	beq _0803CDD2
_0803CDC8:
	cmp r6, r0
	bne _0803CDD6
	ldrh r7, [r7, #6]
	cmp r4, r7
	bne _0803CDD6
_0803CDD2:
	mov r0, #1
	b _0803CDDE
_0803CDD6:
	mov r0, #0
	b _0803CDDE
_0803CDDA:
	add r7, #8
	b _0803CC98
_0803CDDE:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CheckFusionRecipe3
	.align 2, 0

