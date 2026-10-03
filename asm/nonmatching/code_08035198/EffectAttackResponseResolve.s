	thumb_func_start EffectAttackResponseResolve
EffectAttackResponseResolve: @ 0x080358AC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	mov r8, r0
	ldrb r5, [r0, #6]
	ldrh r0, [r0, #6]
	lsr r3, r0, #8
	mov r0, #4
	mov r1, r8
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080358CE
	b _08035BA8
_080358CE:
	mov r2, r8
	ldrh r1, [r2]
	ldr r0, _080358F0 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _080358F4 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _080358F8 @ =0x0000044A
	cmp r2, r0
	beq _08035964
	cmp r2, r0
	bgt _080358FC
	sub r0, #0x2A
	cmp r2, r0
	beq _08035910
	b _08035A54
_080358F0: .4byte 0x000007FF
_080358F4: .4byte gCardIdToNumber
_080358F8: .4byte 0x0000044A
_080358FC:
	ldr r0, _0803590C @ =0x000004BE
	cmp r2, r0
	beq _080359B0
	add r0, #0xC9
	cmp r2, r0
	bne _0803590A
	b _08035A0C
_0803590A:
	b _08035A54
_0803590C: .4byte 0x000004BE
_08035910:
	mov r4, #0
	mov r6, #1
	add r0, r5, #0
	and r0, r6
	ldr r1, _0803595C @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_0803591E:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08035960 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08035952
	add r0, r6, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08035952
	add r0, r5, #0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	mov r3, r8
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_08035952:
	add r4, #1
	cmp r4, #4
	ble _0803591E
	b _08035BA8
	.align 2, 0
_0803595C: .4byte 0x00000D64
_08035960: .4byte 0x0201930C
_08035964:
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _080359A8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080359AC @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08035982
	b _08035BA8
_08035982:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0803598E
	b _08035BA8
_0803598E:
	mov r0, r8
	ldrb r0, [r0, #2]
	lsl r4, r0, #0x1F
	lsr r4, r4, #0x1F
	add r0, r5, #0
	add r1, r3, #0
	bl GetZoneCardAtk
	add r1, r0, #0
	add r0, r4, #0
	bl GainLifePoints
	b _08035BA8
_080359A8: .4byte 0x00000D64
_080359AC: .4byte 0x0201930C
_080359B0:
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _08035A00 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08035A04 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _080359CE
	b _08035BA8
_080359CE:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _080359DA
	b _08035BA8
_080359DA:
	mov r0, #0x3B
	cmp r5, #0
	beq _080359E2
	ldr r0, _08035A08 @ =0x0000803B
_080359E2:
	add r4, r3, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardAtk
	add r1, r0, #0
	add r0, r5, #0
	bl LoseLifePoints
	b _08035BA8
_08035A00: .4byte 0x00000D64
_08035A04: .4byte 0x0201930C
_08035A08: .4byte 0x0000803B
_08035A0C:
	mov r0, #1
	and r5, r0
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _08035A4C @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _08035A50 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08035A2A
	b _08035BA8
_08035A2A:
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08035A36
	b _08035BA8
_08035A36:
	mov r1, r8
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r2, r8
	ldrh r1, [r2]
	ldrh r2, [r2, #6]
	mov r3, #3
	bl QueueAddZoneLink
	b _08035BA8
_08035A4C: .4byte 0x00000D64
_08035A50: .4byte 0x0201930C
_08035A54:
	ldr r0, _08035AFC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08035B00 @ =0x08622AB4
	add r0, r0, r3
	ldr r1, _08035B04 @ =0x000002AD
	ldrh r0, [r0]
	cmp r0, r1
	beq _08035A68
	b _08035BA8
_08035A68:
	ldr r6, _08035B08 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x78
	beq _08035B54
	cmp r0, #0x80
	beq _08035A7C
	b _08035BA8
_08035A7C:
	mov r7, #1
	neg r7, r7
	mov r9, r7
	mov r6, #0
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _08035B0C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov sl, r2
_08035A92:
	mov r0, #0x94
	mul r0, r4
	add r0, sl
	ldr r1, _08035B10 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08035AC6
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #2
	bne _08035AC6
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardAtk
	cmp r0, r7
	bne _08035ABC
	add r6, #1
_08035ABC:
	cmp r0, r7
	ble _08035AC6
	add r7, r0, #0
	mov r9, r4
	mov r6, #1
_08035AC6:
	add r4, #1
	cmp r4, #4
	ble _08035A92
	mov r3, r9
	cmp r3, #0
	blt _08035BA8
	cmp r6, #1
	beq _08035AE2
	mov r0, #1
	mov r1, r8
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0
	beq _08035B14
_08035AE2:
	add r0, r5, #0
	mov r1, r9
	bl DestroyFieldCardByEffect
	mov r2, r8
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	mov r2, r9
	bl OnCardDestroyedByEffect
	b _08035BA8
_08035AFC: .4byte 0x000007FF
_08035B00: .4byte gCardIdToNumber
_08035B04: .4byte 0x000002AD
_08035B08: .4byte 0x02017A40
_08035B0C: .4byte 0x00000D64
_08035B10: .4byte 0x0201930C
_08035B14:
	ldr r1, _08035B44 @ =0x08082DE8
	mov r0, sp
	add r2, r7, #0
	bl FormatInt
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _08035B48 @ =0x00000817
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	ldr r0, _08035B4C @ =0x02017A40
	ldr r3, _08035B50 @ =0x00000542
	add r1, r0, r3
	strh r7, [r1]
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	mov r1, #0x78
	strb r1, [r0]
_08035B3E:
	mov r0, #0x78
	b _08035BAA
	.align 2, 0
_08035B44: .4byte gStrWidespreadRuinTiePrompt
_08035B48: .4byte 0x00000817
_08035B4C: .4byte 0x02017A40
_08035B50: .4byte 0x00000542
_08035B54:
	lsl r1, r5, #4
	mov r0, #0xF0
	lsl r0, r1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08035B3E
	ldr r0, _08035B94 @ =0x0201CFB0
	ldr r2, _08035B98 @ =0x0000082C
	add r4, r0, r2
	ldr r1, [r4]
	add r0, r5, #0
	bl GetZoneCardAtk
	ldr r3, _08035B9C @ =0x00000542
	add r1, r6, r3
	ldrh r1, [r1]
	cmp r0, r1
	bne _08035BA0
	ldr r1, [r4]
	add r0, r5, #0
	bl DestroyFieldCardByEffect
	mov r1, r8
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, [r4]
	add r1, r5, #0
	bl OnCardDestroyedByEffect
	b _08035BA8
_08035B94: .4byte 0x0201CFB0
_08035B98: .4byte 0x0000082C
_08035B9C: .4byte 0x00000542
_08035BA0:
	mov r0, #3
	bl PlaySE
	b _08035B3E
_08035BA8:
	mov r0, #0
_08035BAA:
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectAttackResponseResolve
	.align 2, 0

