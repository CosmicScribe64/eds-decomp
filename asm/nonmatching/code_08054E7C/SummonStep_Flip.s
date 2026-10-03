	thumb_func_start SummonStep_Flip
SummonStep_Flip: @ 0x08054E7C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _08054E9C @ =0x0201CF90
	ldrh r1, [r4, #0xE]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #1
	bne _08054E90
	b _08054F8C
_08054E90:
	cmp r0, #1
	bgt _08054EA0
	cmp r0, #0
	beq _08054EAE
	b _0805528C
	.align 2, 0
_08054E9C: .4byte 0x0201CF90
_08054EA0:
	cmp r0, #2
	bne _08054EA6
	b _08054FD8
_08054EA6:
	cmp r0, #3
	bne _08054EAC
	b _0805508C
_08054EAC:
	b _0805528C
_08054EAE:
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r2, r0, #0x1F
	mov r5, #1
	add r0, r2, #0
	and r0, r5
	lsl r1, r1, #0x1A
	lsr r3, r1, #0x1B
	mov r1, #0x94
	mov r8, r1
	mov r1, r8
	mul r1, r3
	ldr r7, _08054F60 @ =0x00000D64
	mul r0, r7
	add r1, r1, r0
	ldr r6, _08054F64 @ =0x0201930C
	add r1, r1, r6
	add r0, r5, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08054F74
	mov r0, #0x7E
	cmp r2, #0
	beq _08054EE2
	ldr r0, _08054F68 @ =0x0000807E
_08054EE2:
	add r1, r3, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r4]
	lsl r1, r0, #0x1F
	lsr r2, r1, #0x1F
	add r1, r2, #0
	and r1, r5
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	mov r3, r8
	mul r3, r0
	add r0, r3, #0
	mul r1, r7
	add r0, r0, r1
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	ldr r0, _08054F6C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08054F70 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, #0x5E
	beq _08054F1E
	b _08055250
_08054F1E:
	add r0, r2, #0
	mov r2, #0
	bl CanActivateEffectOfCard
	cmp r0, #0
	bne _08054F2C
	b _08055250
_08054F2C:
	ldrb r2, [r4]
	lsl r4, r2, #0x1F
	lsr r4, r4, #0x1F
	and r4, r5
	lsl r0, r4, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	lsl r1, r2, #0x10
	mov r3, #0xA2
	lsl r3, r3, #0x15
	orr r1, r3
	orr r0, r1
	mov r1, r8
	mul r1, r2
	add r2, r4, #0
	mul r2, r7
	add r1, r1, r2
	add r1, r1, r6
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	b _08055250
_08054F60: .4byte 0x00000D64
_08054F64: .4byte 0x0201930C
_08054F68: .4byte 0x0000807E
_08054F6C: .4byte 0x000007FF
_08054F70: .4byte gCardIdToNumber
_08054F74:
	mov r0, #0x7F
	cmp r2, #0
	beq _08054F7C
	ldr r0, _08054F88 @ =0x0000807F
_08054F7C:
	add r1, r3, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _08055250
_08054F88: .4byte 0x0000807F
_08054F8C:
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0
	bl DuelCursor_Select
	ldrb r1, [r4, #3]
	lsr r0, r1, #7
	ldr r1, _08054FD0 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	mov r0, #0x71
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrh r2, [r4, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08054FD4 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r4, #0xE]
	mov r0, #0
	b _0805528E
	.align 2, 0
_08054FD0: .4byte 0x00007FFF
_08054FD4: .4byte 0xFFFFF01F
_08054FD8:
	ldrb r3, [r4, #3]
	lsr r1, r3, #7
	ldr r0, _08055008 @ =0x00007FFF
	ldrh r4, [r4, #4]
	and r0, r4
	lsl r2, r0, #1
	orr r2, r1
	ldr r0, _0805500C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08055010 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805501C
	cmp r0, #0x17
	ble _08055014
	cmp r0, #0x18
	beq _08055018
	b _0805501C
_08055008: .4byte 0x00007FFF
_0805500C: .4byte 0x000007FF
_08055010: .4byte gCardStats
_08055014:
	mov r0, #0
	b _08055030
_08055018:
	mov r0, #0xA
	b _08055030
_0805501C:
	ldr r0, _08055078 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r2, _0805507C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08055030:
	cmp r0, #2
	bls _08055036
	b _08055250
_08055036:
	ldr r5, _08055080 @ =0x000002AE
	mov r0, #0
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _08055052
	mov r0, #1
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _08055052
	b _08055250
_08055052:
	ldr r4, _08055084 @ =0x0201CF90
	ldrb r3, [r4]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r5, #1
	ldr r2, _08055088 @ =0x08623DF4
	add r1, r1, r2
	ldrh r1, [r1]
	bl ShowCardEffect
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	mov r2, #1
	bl DestroyFieldCard
	b _0805528C
_08055078: .4byte 0x000007FF
_0805507C: .4byte gCardStats
_08055080: .4byte 0x000002AE
_08055084: .4byte 0x0201CF90
_08055088: .4byte gCardNumberToId
_0805508C:
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	mov r3, #0x90
	cmp r0, #0
	beq _08055098
	ldr r3, _080550E4 @ =0x00008090
_08055098:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrh r2, [r4, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r3, [r4]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl TriggerMysteriousPuppeteer
	ldrb r0, [r4, #3]
	lsr r1, r0, #7
	ldr r0, _080550E8 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	ldr r1, _080550EC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _080550F0 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _080550F4 @ =0x00000462
	cmp r1, r0
	beq _08055122
	cmp r1, r0
	bgt _08055104
	mov r0, #0xC7
	lsl r0, r0, #2
	cmp r1, r0
	beq _0805519C
	cmp r1, r0
	bgt _080550FC
	ldr r0, _080550F8 @ =0x000001F3
	b _08055110
_080550E4: .4byte 0x00008090
_080550E8: .4byte 0x00007FFF
_080550EC: .4byte 0x000007FF
_080550F0: .4byte gCardIdToNumber
_080550F4: .4byte 0x00000462
_080550F8: .4byte 0x000001F3
_080550FC:
	ldr r0, _08055100 @ =0x00000455
	b _08055110
_08055100: .4byte 0x00000455
_08055104:
	ldr r0, _08055118 @ =0x000004DE
	cmp r1, r0
	beq _08055122
	cmp r1, r0
	bgt _0805511C
	sub r0, #6
_08055110:
	cmp r1, r0
	beq _08055122
	b _080551AE
	.align 2, 0
_08055118: .4byte 0x000004DE
_0805511C:
	ldr r0, _08055188 @ =0x00000534
	cmp r1, r0
	bne _080551AE
_08055122:
	ldr r5, _0805518C @ =0x0201CF90
	ldrb r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r6, #1
	add r3, r0, #0
	and r3, r6
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0x94
	mul r1, r2
	ldr r2, _08055190 @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _08055194 @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, #0
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _080551AE
	ldrb r4, [r5]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	add r0, r1, #0
	and r0, r6
	lsl r0, r0, #0x1F
	lsl r4, r4, #0x1A
	lsr r4, r4, #0x1B
	lsl r2, r4, #0x10
	mov r3, #0xC4
	lsl r3, r3, #0x14
	orr r2, r3
	orr r0, r2
	ldrb r2, [r5, #3]
	lsr r3, r2, #7
	ldr r2, _08055198 @ =0x00007FFF
	ldrh r5, [r5, #4]
	and r2, r5
	lsl r2, r2, #1
	orr r2, r3
	orr r0, r2
	lsl r4, r4, #8
	orr r1, r4
	bl Chain_AddPending
	b _080551AE
	.align 2, 0
_08055188: .4byte 0x00000534
_0805518C: .4byte 0x0201CF90
_08055190: .4byte 0x00000D64
_08055194: .4byte 0x0201930C
_08055198: .4byte 0x00007FFF
_0805519C:
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	mov r2, #0
	mov r3, #0
	bl ChangeBattlePosition
_080551AE:
	ldr r5, _0805526C @ =0x0201CF90
	ldrb r3, [r5, #3]
	lsr r1, r3, #7
	ldr r7, _08055270 @ =0x00007FFF
	add r0, r7, #0
	ldrh r2, [r5, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	ldr r1, _08055274 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08055278 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	beq _08055250
	ldrb r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r6, #1
	add r3, r0, #0
	and r3, r6
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0x94
	mul r1, r2
	ldr r2, _0805527C @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _08055280 @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, #0
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _08055250
	ldr r4, _08055284 @ =0x000005FA
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08055250
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08055250
	ldrb r4, [r5]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	add r0, r1, #0
	and r0, r6
	lsl r0, r0, #0x1F
	lsl r4, r4, #0x1A
	lsr r4, r4, #0x1B
	lsl r2, r4, #0x10
	mov r3, #0xC4
	lsl r3, r3, #0x14
	orr r2, r3
	orr r0, r2
	ldrb r2, [r5, #3]
	lsr r3, r2, #7
	add r2, r7, #0
	ldrh r5, [r5, #4]
	and r2, r5
	lsl r2, r2, #1
	orr r2, r3
	orr r0, r2
	lsl r4, r4, #8
	orr r1, r4
	bl Chain_AddPending
_08055250:
	ldr r3, _0805526C @ =0x0201CF90
	ldrh r2, [r3, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08055288 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3, #0xE]
	mov r0, #0
	b _0805528E
_0805526C: .4byte 0x0201CF90
_08055270: .4byte 0x00007FFF
_08055274: .4byte 0x000007FF
_08055278: .4byte gCardIdToNumber
_0805527C: .4byte 0x00000D64
_08055280: .4byte 0x0201930C
_08055284: .4byte 0x000005FA
_08055288: .4byte 0xFFFFF01F
_0805528C:
	mov r0, #1
_0805528E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end SummonStep_Flip

