	thumb_func_start RitualTributeSelectStep
RitualTributeSelectStep: @ 0x080437CC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r4, #0xF0
	ldr r0, _080438C4 @ =0x02017A40
	mov r8, r0
	mov r1, #0xF0
	lsl r1, r1, #2
	add r1, r8
	mov r9, r1
	ldrh r1, [r1]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r8
	mov r2, #0xA0
	lsl r2, r2, #2
	add r0, r0, r2
	ldrh r0, [r0]
	bl FindRitualRecipe
	mov r3, r9
	ldrh r1, [r3]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r8
	ldr r1, _080438C8 @ =0x00000282
	mov sl, r1
	add r0, sl
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	bl CountMonsters
	cmp r0, #4
	bgt _08043820
	mov r4, #0xF1
_08043820:
	add r0, r4, #0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _0804382C
	b _08043A78
_0804382C:
	ldr r1, _080438CC @ =0x0201CFB0
	ldr r2, _080438D0 @ =0x00000828
	add r0, r1, r2
	ldr r0, [r0]
	add r4, r1, #0
	cmp r0, #0
	bne _0804383C
	b _080439A0
_0804383C:
	cmp r0, #0xB
	beq _08043842
	b _08043A78
_08043842:
	mov r3, r9
	ldrh r0, [r3]
	sub r0, #1
	lsl r3, r0, #2
	add r3, r3, r0
	lsl r3, r3, #2
	add r3, r8
	mov r1, sl
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r2, r0, #0x1F
	lsr r2, r2, #0x1F
	ldr r1, _080438D4 @ =0x0000082C
	add r0, r4, r1
	ldr r0, [r0]
	lsl r0, r0, #2
	ldr r1, _080438D8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080438DC @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldr r0, _080438E0 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r2, _080438E4 @ =0x08622AB4
	add r7, r0, r2
	ldrh r5, [r7]
	ldr r4, _080438E8 @ =0x0819A990
	mov r0, #0xA0
	lsl r0, r0, #2
	add r3, r3, r0
	ldrh r0, [r3]
	bl FindRitualRecipe
	lsl r0, r0, #2
	add r0, r0, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x13
	cmp r5, r0
	bne _080438EC
	mov r2, r9
	ldrh r1, [r2]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r8
	add r0, sl
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r7]
	bl CountHandCardsByNumber
	cmp r0, #1
	bgt _080438EC
	mov r0, #3
	bl PlaySE
	mov r0, #0
	b _08043A8A
_080438C4: .4byte 0x02017A40
_080438C8: .4byte 0x00000282
_080438CC: .4byte 0x0201CFB0
_080438D0: .4byte 0x00000828
_080438D4: .4byte 0x0000082C
_080438D8: .4byte 0x00000D64
_080438DC: .4byte 0x02019968
_080438E0: .4byte 0x000007FF
_080438E4: .4byte gCardIdToNumber
_080438E8: .4byte gRitualRecipes
_080438EC:
	ldr r0, _08043914 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r3, _08043918 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08043906
	b _08043A78
_08043906:
	cmp r0, #0x15
	blt _08043924
	cmp r0, #0x17
	ble _0804391C
	cmp r0, #0x18
	beq _08043920
	b _08043924
_08043914: .4byte 0x000007FF
_08043918: .4byte gCardStats
_0804391C:
	mov r0, #0
	b _08043938
_08043920:
	mov r0, #0xA
	b _08043938
_08043924:
	ldr r0, _0804394C @ =0x000007FF
	and r6, r0
	lsl r0, r6, #2
	ldr r1, _08043950 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043938:
	add r2, r0, #0
	ldr r0, _08043954 @ =0x02017A40
	ldr r3, _08043958 @ =0x000003E1
	add r1, r0, r3
	ldrb r0, [r1]
	cmp r0, r2
	bge _0804395C
	mov r0, #0
	b _0804395E
	.align 2, 0
_0804394C: .4byte 0x000007FF
_08043950: .4byte gCardStats
_08043954: .4byte 0x02017A40
_08043958: .4byte 0x000003E1
_0804395C:
	sub r0, r0, r2
_0804395E:
	strb r0, [r1]
	ldr r2, _08043990 @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r2, r1
	ldrh r1, [r0]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r2, _08043994 @ =0x00000282
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08043998 @ =0x0201CFB0
	ldr r3, _0804399C @ =0x0000082C
	add r1, r1, r3
	ldr r1, [r1]
	mov r2, #0
	mov r3, #1
	bl DiscardHandCard
	b _08043A78
_08043990: .4byte 0x02017A40
_08043994: .4byte 0x00000282
_08043998: .4byte 0x0201CFB0
_0804399C: .4byte 0x0000082C
_080439A0:
	mov r0, r9
	ldrh r1, [r0]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r8
	add r0, sl
	ldrb r0, [r0]
	lsl r2, r0, #0x1F
	lsr r2, r2, #0x1F
	ldr r1, _080439F8 @ =0x0000082C
	add r0, r4, r1
	ldr r1, [r0]
	mov r0, #0x94
	mul r0, r1
	ldr r1, _080439FC @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08043A00 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldr r0, _08043A04 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r2, _08043A08 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08043A78
	cmp r0, #0x15
	blt _08043A14
	cmp r0, #0x17
	ble _08043A0C
	cmp r0, #0x18
	beq _08043A10
	b _08043A14
	.align 2, 0
_080439F8: .4byte 0x0000082C
_080439FC: .4byte 0x00000D64
_08043A00: .4byte 0x0201930C
_08043A04: .4byte 0x000007FF
_08043A08: .4byte gCardStats
_08043A0C:
	mov r0, #0
	b _08043A28
_08043A10:
	mov r0, #0xA
	b _08043A28
_08043A14:
	ldr r0, _08043A3C @ =0x000007FF
	and r6, r0
	lsl r0, r6, #2
	ldr r3, _08043A40 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043A28:
	add r2, r0, #0
	ldr r0, _08043A44 @ =0x02017A40
	ldr r3, _08043A48 @ =0x000003E1
	add r1, r0, r3
	ldrb r0, [r1]
	cmp r0, r2
	bge _08043A4C
	mov r0, #0
	b _08043A4E
	.align 2, 0
_08043A3C: .4byte 0x000007FF
_08043A40: .4byte gCardStats
_08043A44: .4byte 0x02017A40
_08043A48: .4byte 0x000003E1
_08043A4C:
	sub r0, r0, r2
_08043A4E:
	strb r0, [r1]
	ldr r2, _08043A98 @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r2, r1
	ldrh r1, [r0]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r2, _08043A9C @ =0x00000282
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08043AA0 @ =0x0000082C
	add r1, r4, r3
	ldr r1, [r1]
	bl TributeMonster
_08043A78:
	mov r1, #0
	ldr r0, _08043A98 @ =0x02017A40
	ldr r2, _08043AA4 @ =0x000003E1
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _08043A88
	mov r1, #1
_08043A88:
	add r0, r1, #0
_08043A8A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08043A98: .4byte 0x02017A40
_08043A9C: .4byte 0x00000282
_08043AA0: .4byte 0x0000082C
_08043AA4: .4byte 0x000003E1
	thumb_func_end RitualTributeSelectStep

