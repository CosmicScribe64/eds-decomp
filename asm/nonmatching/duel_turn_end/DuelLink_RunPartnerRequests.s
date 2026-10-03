	thumb_func_start DuelLink_RunPartnerRequests
DuelLink_RunPartnerRequests: @ 0x080512F0
	push {r4, r5, r6, lr}
	ldr r0, _08051340 @ =0x02017FB0
	mov r1, #0x8A
	lsl r1, r1, #3
	add r4, r0, r1
	ldrb r2, [r4]
	lsl r1, r2, #0x1F
	add r5, r0, #0
	cmp r1, #0
	beq _08051354
	ldr r3, _08051344 @ =0x00000306
	add r0, r5, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08051354
	bl DuelLink_RunCardPrompt
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0805131C
	b _0805145C
_0805131C:
	mov r0, #2
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	ldr r0, _08051348 @ =0x0000F058
	ldr r2, _0805134C @ =0x0000045A
	add r1, r5, r2
	ldrh r1, [r1]
	ldr r3, _08051350 @ =0x00000456
	add r2, r5, r3
	ldrh r2, [r2]
	mov r4, #0x8B
	lsl r4, r4, #3
	add r3, r5, r4
	ldrh r3, [r3]
	b _08051458
	.align 2, 0
_08051340: .4byte 0x02017FB0
_08051344: .4byte 0x00000306
_08051348: .4byte 0x0000F058
_0805134C: .4byte 0x0000045A
_08051350: .4byte 0x00000456
_08051354:
	add r6, r5, #0
	mov r0, #0xC2
	lsl r0, r0, #2
	add r4, r6, r0
	ldrb r2, [r4]
	lsl r0, r2, #0x1D
	cmp r0, #0
	bge _080513A0
	bl DuelLink_RunRemoteChainA
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0805145C
	mov r0, #5
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	ldr r2, _08051394 @ =0x0000045E
	add r1, r6, r2
	mov r0, #1
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r0, _08051398 @ =0x0000F092
	ldr r4, _0805139C @ =0x0000045C
	add r1, r6, r4
	mov r2, #0x14
	bl DuelLink_SendMessageData
	b _0805145C
	.align 2, 0
_08051394: .4byte 0x0000045E
_08051398: .4byte 0x0000F092
_0805139C: .4byte 0x0000045C
_080513A0:
	lsl r0, r2, #0x1F
	cmp r0, #0
	beq _080513E0
	bl DuelLink_RunRemoteChainB
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0805145C
	mov r0, #2
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	ldr r2, _080513D4 @ =0x0000045E
	add r1, r6, r2
	mov r0, #1
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r0, _080513D8 @ =0x0000F082
	ldr r4, _080513DC @ =0x0000045C
	add r1, r6, r4
	mov r2, #0x14
	bl DuelLink_SendMessageData
	b _0805145C
_080513D4: .4byte 0x0000045E
_080513D8: .4byte 0x0000F082
_080513DC: .4byte 0x0000045C
_080513E0:
	ldr r1, _0805140C @ =0x00000306
	add r0, r5, r1
	mov r1, #0x84
	lsl r1, r1, #3
	ldrh r0, [r0]
	and r1, r0
	mov r0, #0x80
	lsl r0, r0, #3
	cmp r1, r0
	bne _08051414
	bl DuelLink_AnswerActivateQuery
	cmp r0, #0
	beq _0805145C
	ldr r2, _08051410 @ =0x00000307
	add r1, r5, r2
	mov r0, #5
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	b _0805145C
_0805140C: .4byte 0x00000306
_08051410: .4byte 0x00000307
_08051414:
	lsl r0, r2, #0x1B
	cmp r0, #0
	bge _08051438
	bl DuelLink_RunRemoteResolve
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0805145C
	mov r0, #0x11
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	ldr r0, _08051434 @ =0x0000F073
	b _08051452
	.align 2, 0
_08051434: .4byte 0x0000F073
_08051438:
	lsl r0, r2, #0x19
	cmp r0, #0
	bge _08051464
	bl ChainListScreen_Run
	cmp r0, #0
	beq _0805145C
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	strb r0, [r4]
	ldr r0, _08051460 @ =0x0000F065
_08051452:
	mov r1, #0
	mov r2, #0
	mov r3, #0
_08051458:
	bl DuelLink_SendMessage
_0805145C:
	mov r0, #1
	b _08051466
_08051460: .4byte 0x0000F065
_08051464:
	mov r0, #0
_08051466:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end DuelLink_RunPartnerRequests

