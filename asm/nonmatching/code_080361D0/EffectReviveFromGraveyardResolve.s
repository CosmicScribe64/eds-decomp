	thumb_func_start EffectReviveFromGraveyardResolve
EffectReviveFromGraveyardResolve: @ 0x080367E4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #0
	beq _08036800
	b _08036980
_08036800:
	mov r0, #7
	ldrb r1, [r4, #0xA]
	and r0, r1
	cmp r0, #2
	beq _0803680C
	b _08036980
_0803680C:
	ldr r0, _080368EC @ =0x02017A40
	mov r8, r0
	mov r0, #0xF8
	lsl r0, r0, #2
	add r0, r8
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08036900
	cmp r0, #0x80
	beq _08036822
	b _08036980
_08036822:
	ldrb r1, [r4, #2]
	lsl r3, r1, #0x1F
	mov r5, #1
	lsr r2, r3, #0x1F
	ldrh r1, [r4, #2]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r0, r1
	ldr r1, _080368F0 @ =0x00000D64
	mov r9, r1
	mov r1, r9
	mul r1, r2
	add r0, r0, r1
	ldr r7, _080368F4 @ =0x0201930C
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0803684C
	b _08036980
_0803684C:
	ldrh r1, [r4, #0xE]
	lsl r0, r1, #0x10
	ldrh r1, [r4, #0xC]
	orr r0, r1
	str r0, [sp, #4]
	lsr r0, r3, #0x1F
	add r1, sp, #4
	bl IsCardInGraveyard
	cmp r0, #0
	bne _08036864
	b _08036980
_08036864:
	ldrb r0, [r4, #2]
	and r5, r0
	mov r0, #0xD3
	cmp r5, #0
	beq _08036870
	ldr r0, _080368F8 @ =0x000080D3
_08036870:
	ldrh r1, [r4, #0xC]
	ldrh r2, [r4, #0xE]
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x20
	str r1, [sp, #0]
	add r1, sp, #4
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummon
	ldr r0, _080368FC @ =0x000003E1
	add r0, r8
	strb r6, [r0]
	mov r3, #0
	ldrb r1, [r4, #2]
	lsl r5, r1, #0x1F
	mov r1, #1
	mov r8, r1
	mov ip, r9
	add r6, r0, #0
_080368A2:
	lsr r0, r5, #0x1F
	mov r1, r8
	and r1, r0
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	mov r0, ip
	mul r0, r1
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldrh r1, [r4, #0xC]
	cmp r0, r1
	bne _080368E0
	lsr r1, r5, #0x1F
	mov r0, r8
	and r0, r1
	mov r1, ip
	mul r1, r0
	add r1, r2, r1
	add r1, r1, r7
	mov r0, #0x80
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	beq _080368E0
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
_080368E0:
	add r3, #1
	cmp r3, #4
	ble _080368A2
	mov r0, #0x7F
	b _08036982
	.align 2, 0
_080368EC: .4byte 0x02017A40
_080368F0: .4byte 0x00000D64
_080368F4: .4byte 0x0201930C
_080368F8: .4byte 0x000080D3
_080368FC: .4byte 0x000003E1
_08036900:
	ldr r0, _0803691C @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08036920 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08036924 @ =0x00000447
	cmp r1, r0
	beq _08036928
	add r0, #0x41
	cmp r1, r0
	beq _08036954
	b _08036978
_0803691C: .4byte 0x000007FF
_08036920: .4byte gCardIdToNumber
_08036924: .4byte 0x00000447
_08036928:
	ldrb r0, [r4, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	add r1, r0, #0
	ldrh r4, [r4, #2]
	lsl r3, r4, #0x16
	lsr r3, r3, #0x1A
	lsl r3, r3, #8
	orr r1, r3
	add r2, r0, #0
	ldr r3, _08036950 @ =0x0201CF90
	ldrb r3, [r3]
	lsl r3, r3, #0x1A
	lsr r3, r3, #0x1B
	lsl r3, r3, #8
	orr r2, r3
	mov r3, #2
	bl QueueAddZoneLink
	b _08036978
_08036950: .4byte 0x0201CF90
_08036954:
	ldrb r1, [r4, #2]
	lsl r2, r1, #0x1F
	lsr r0, r2, #0x1F
	add r1, r0, #0
	ldrh r4, [r4, #2]
	lsl r3, r4, #0x16
	lsr r3, r3, #0x1A
	lsl r3, r3, #8
	orr r1, r3
	add r2, r0, #0
	ldr r3, _0803697C @ =0x0201CF90
	ldrb r3, [r3]
	lsl r3, r3, #0x1A
	lsr r3, r3, #0x1B
	lsl r3, r3, #8
	orr r2, r3
	bl EquipCard
_08036978:
	mov r0, #0x64
	b _08036982
_0803697C: .4byte 0x0201CF90
_08036980:
	mov r0, #0
_08036982:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectReviveFromGraveyardResolve

