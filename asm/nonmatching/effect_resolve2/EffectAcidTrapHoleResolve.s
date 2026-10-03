	thumb_func_start EffectAcidTrapHoleResolve
EffectAcidTrapHoleResolve: @ 0x080326C4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r0, #4
	mov r1, r9
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080326DC
	b _080327DA
_080326DC:
	mov r0, #7
	mov r2, r9
	ldrb r2, [r2, #0xA]
	and r0, r2
	cmp r0, #1
	bne _080327DA
	mov r1, r9
	ldrb r4, [r1, #0xC]
	ldrh r2, [r1, #0xC]
	lsr r5, r2, #8
	mov r8, r4
	mov r1, r8
	and r1, r0
	mov r8, r1
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08032740 @ =0x00000D64
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	ldr r0, _08032744 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldr r0, _08032748 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803274C
	cmp r0, #0x80
	bne _080327DA
	cmp r6, #0
	beq _080327DA
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #1
	bne _080327DA
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl FlipFieldCard
	mov r0, #0x7F
	b _080327DC
_08032740: .4byte 0x00000D64
_08032744: .4byte 0x0201930C
_08032748: .4byte 0x02017A40
_0803274C:
	add r0, r4, #0
	add r1, r5, #0
	bl GetZoneCardDef
	mov r1, #0xFA
	lsl r1, r1, #3
	cmp r0, r1
	ble _08032770
	add r0, r4, #0
	add r1, r6, #0
	bl ShowRevealedCard
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl FlipFieldCard
	b _080327DA
_08032770:
	add r0, r4, #0
	add r1, r6, #0
	bl ShowDestroyedCard
	ldr r0, _080327E8 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _080327EC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	beq _080327C2
	ldr r7, _080327F0 @ =0x000005FA
	mov r0, #0
	add r1, r7, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _080327C2
	mov r0, #1
	add r1, r7, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _080327C2
	mov r2, r8
	lsl r0, r2, #0x1F
	mov r1, #0x1F
	and r1, r5
	lsl r1, r1, #0x10
	mov r2, #0xB2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	orr r0, r6
	mov r1, #0
	bl Chain_AddPending
_080327C2:
	add r0, r4, #0
	add r1, r5, #0
	bl DestroyFieldCardByEffect
	mov r1, r9
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	add r2, r5, #0
	bl OnCardDestroyedByEffect
_080327DA:
	mov r0, #0
_080327DC:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080327E8: .4byte 0x000007FF
_080327EC: .4byte gCardIdToNumber
_080327F0: .4byte 0x000005FA
	thumb_func_end EffectAcidTrapHoleResolve

