	thumb_func_start EffectEquipTargetChainB
EffectEquipTargetChainB: @ 0x0803E3C4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r7, r0, #0
	ldrb r1, [r7, #2]
	mov r0, #0xE
	and r0, r1
	cmp r0, #6
	bne _0803E3DE
	b _0803E590
_0803E3DE:
	mov r6, #1
	add r2, r6, #0
	and r2, r1
	cmp r2, #0
	bne _0803E3EA
	b _0803E4FE
_0803E3EA:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r7, #0xA]
	and r0, r2
	strb r0, [r7, #0xA]
	lsl r4, r1, #0x1F
	lsr r5, r4, #0x1F
	ldr r0, _0803E41C @ =0x000007FF
	ldrh r3, [r7]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0803E420 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0803E424 @ =0x00000417
	cmp r1, r0
	bgt _0803E428
	sub r0, #1
	cmp r1, r0
	bge _0803E46C
	mov r0, #0xA4
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803E43C
	b _0803E476
_0803E41C: .4byte 0x000007FF
_0803E420: .4byte gCardIdToNumber
_0803E424: .4byte 0x00000417
_0803E428:
	ldr r0, _0803E438 @ =0x0000058B
	cmp r1, r0
	beq _0803E46C
	add r0, #0x81
	cmp r1, r0
	beq _0803E46C
	b _0803E476
	.align 2, 0
_0803E438: .4byte 0x0000058B
_0803E43C:
	ldr r3, _0803E464 @ =0x020192E4
	lsr r1, r4, #0x1F
	add r0, r6, #0
	and r0, r1
	ldr r2, _0803E468 @ =0x00000D64
	add r1, r0, #0
	mul r1, r2
	add r1, r1, r3
	lsr r0, r4, #0x1F
	sub r0, r6, r0
	and r0, r6
	mul r0, r2
	add r0, r0, r3
	ldrh r1, [r1]
	ldrh r0, [r0]
	cmp r1, r0
	bls _0803E476
	lsr r0, r4, #0x1F
	sub r5, r6, r0
	b _0803E476
_0803E464: .4byte 0x020192E4
_0803E468: .4byte 0x00000D64
_0803E46C:
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r5, r1, r0
_0803E476:
	mov r3, #1
	neg r3, r3
	mov r8, r3
	mov r9, r8
	mov sl, r8
	mov r0, #0
	str r0, [sp, #0]
_0803E484:
	mov r4, #0
	lsl r0, r5, #0x18
	lsr r6, r0, #0x18
_0803E48A:
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	orr r1, r6
	add r0, r7, #0
	bl EffectEquipTargetCheck
	cmp r0, #0
	beq _0803E4B4
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardAtk
	cmp r9, r0
	bge _0803E4B4
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardAtk
	mov r9, r0
	mov r8, r5
	mov sl, r4
_0803E4B4:
	add r4, #1
	cmp r4, #4
	ble _0803E48A
	mov r0, #1
	neg r0, r0
	cmp r9, r0
	ble _0803E4DA
	cmp r8, r0
	ble _0803E4DA
	cmp sl, r0
	ble _0803E4DA
	add r0, r7, #0
	mov r1, r8
	mov r2, sl
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803E590
_0803E4DA:
	ldr r1, [sp, #0]
	add r1, #1
	str r1, [sp, #0]
	mov r0, #1
	sub r5, r0, r5
	cmp r1, #1
	ble _0803E484
	sub r0, #2
	cmp r9, r0
	ble _0803E590
	cmp r8, r0
	ble _0803E590
	cmp sl, r0
	ble _0803E590
	add r0, r7, #0
	mov r1, r8
	mov r2, sl
	b _0803E58A
_0803E4FE:
	ldr r0, _0803E528 @ =0x02017A40
	ldr r3, _0803E52C @ =0x000003E5
	add r4, r0, r3
	ldrb r0, [r4]
	cmp r0, #0
	bne _0803E53C
	ldr r0, _0803E530 @ =0x00000206
	ldr r1, _0803E534 @ =0x00000712
	ldr r3, _0803E538 @ =0x08083E14
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r1, [r7, #0xA]
	and r0, r1
	strb r0, [r7, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0803E5A6
_0803E528: .4byte 0x02017A40
_0803E52C: .4byte 0x000003E5
_0803E530: .4byte 0x00000206
_0803E534: .4byte 0x00000712
_0803E538: .4byte gStrDesignateMonsterToEquip
_0803E53C:
	ldr r1, _0803E54C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E550
	strb r2, [r4]
	b _0803E5A6
_0803E54C: .4byte 0x03000040
_0803E550:
	ldr r0, _0803E594 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803E5A6
	ldr r0, _0803E598 @ =0x0201CFB0
	ldr r3, _0803E59C @ =0x00000824
	add r2, r0, r3
	add r3, #4
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldr r5, [r2]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r2]
	orr r1, r2
	add r0, r7, #0
	bl EffectEquipTargetCheck
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803E5A0
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
_0803E58A:
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
_0803E590:
	mov r0, #1
	b _0803E5A8
_0803E594: .4byte 0x00E000E0
_0803E598: .4byte 0x0201CFB0
_0803E59C: .4byte 0x00000824
_0803E5A0:
	mov r0, #3
	bl PlaySE
_0803E5A6:
	mov r0, #0
_0803E5A8:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectEquipTargetChainB

