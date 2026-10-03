	thumb_func_start IsOpponentUnlocked
IsOpponentUnlocked: @ 0x08063DAC
	push {lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	sub r0, #1
	cmp r0, #0x17
	bhi _08063E98
	lsl r0, r0, #2
	ldr r1, _08063DC4 @ =0x08063DC8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08063DC4: .4byte 0x08063DC8
_08063DC8:
	.4byte _08063E7C
	.4byte _08063E7C
	.4byte _08063E7C
	.4byte _08063E7C
	.4byte _08063E7C
	.4byte _08063E28
	.4byte _08063E28
	.4byte _08063E28
	.4byte _08063E28
	.4byte _08063E28
	.4byte _08063E2E
	.4byte _08063E2E
	.4byte _08063E2E
	.4byte _08063E2E
	.4byte _08063E2E
	.4byte _08063E34
	.4byte _08063E34
	.4byte _08063E34
	.4byte _08063E34
	.4byte _08063E34
	.4byte _08063E8E
	.4byte _08063E3A
	.4byte _08063E58
	.4byte _08063E88
_08063E28:
	bl IsCampaignLevel2Unlocked
	b _08063E92
_08063E2E:
	bl IsCampaignLevel3Unlocked
	b _08063E92
_08063E34:
	bl IsCampaignLevel4Unlocked
	b _08063E92
_08063E3A:
	mov r1, #0
	ldr r0, _08063E50 @ =0x02011C20
	ldr r2, _08063E54 @ =0x00002162
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #1
	bls _08063E4A
	mov r1, #1
_08063E4A:
	add r0, r1, #0
	b _08063E9A
	.align 2, 0
_08063E50: .4byte 0x02011C20
_08063E54: .4byte 0x00002162
_08063E58:
	ldr r1, _08063E80 @ =0x02011C20
	ldr r0, _08063E84 @ =0x08624568
	ldrh r0, [r0]
	lsl r0, r0, #2
	add r1, r0, r1
	ldrh r2, [r1, #8]
	lsl r0, r2, #0x16
	cmp r0, #0
	bne _08063E7C
	ldrb r1, [r1, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08063E7C
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08063E98
_08063E7C:
	mov r0, #1
	b _08063E9A
_08063E80: .4byte 0x02011C20
_08063E84: .4byte gUnk_08624568
_08063E88:
	bl IsCardCollectionComplete
	b _08063E92
_08063E8E:
	bl IsCampaignLevel5Unlocked
_08063E92:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08063E9A
_08063E98:
	mov r0, #0
_08063E9A:
	pop {r1}
	bx r1
	thumb_func_end IsOpponentUnlocked
	.align 2, 0

