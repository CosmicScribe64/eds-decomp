	thumb_func_start FlipFieldCard
FlipFieldCard: @ 0x08018DC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r0, #0
	mov r8, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	mov r0, #1
	mov r9, r0
	mov r1, r9
	and r1, r4
	mov r9, r1
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r0, _08018EA4 @ =0x00000D64
	mov r2, r9
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	ldr r0, _08018EA8 @ =0x0201930C
	add r6, r1, r0
	ldr r0, [r6]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r7, r0, #0
	cmp r7, #0
	beq _08018ECA
	mov r1, #0x7F
	cmp r4, #0
	beq _08018E0E
	ldr r1, _08018EAC @ =0x0000807F
_08018E0E:
	mov r2, r8
	lsl r0, r2, #0x10
	lsr r5, r0, #0x10
	add r0, r1, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, r8
	cmp r0, #4
	bgt _08018ECA
	mov r0, #2
	ldrb r6, [r6, #6]
	and r0, r6
	cmp r0, #0
	bne _08018EC0
	mov r0, #0x90
	cmp r4, #0
	beq _08018E38
	ldr r0, _08018EB0 @ =0x00008090
_08018E38:
	add r1, r5, #0
	mov r2, #2
	mov r3, #0
	bl DuelCmd_Push
	mov r1, sl
	cmp r1, #0
	beq _08018ECA
	ldr r0, _08018EB4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r2, _08018EB8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	beq _08018ECA
	add r0, r4, #0
	add r1, r7, #0
	mov r2, #0
	bl CanActivateEffectOfCard
	cmp r0, #0
	beq _08018ECA
	ldr r4, _08018EBC @ =0x000005FA
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08018ECA
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08018ECA
	mov r1, r9
	lsl r0, r1, #0x1F
	mov r1, #0x1F
	mov r2, r8
	and r1, r2
	lsl r1, r1, #0x10
	mov r2, #0xB2
	lsl r2, r2, #0x15
	orr r1, r2
	orr r0, r1
	orr r0, r7
	mov r1, #0
	bl Chain_AddPending
	b _08018ECA
_08018EA4: .4byte 0x00000D64
_08018EA8: .4byte 0x0201930C
_08018EAC: .4byte 0x0000807F
_08018EB0: .4byte 0x00008090
_08018EB4: .4byte 0x000007FF
_08018EB8: .4byte gCardIdToNumber
_08018EBC: .4byte 0x000005FA
_08018EC0:
	add r0, r4, #0
	mov r1, r8
	mov r2, #1
	bl DestroyLinkedCards
_08018ECA:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end FlipFieldCard

