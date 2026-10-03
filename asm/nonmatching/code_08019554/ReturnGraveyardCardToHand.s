	thumb_func_start ReturnGraveyardCardToHand
ReturnGraveyardCardToHand: @ 0x08019554
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r3, #0
	ldr r2, _080195AC @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _080195B0 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r5, [r1, #4]
	cmp r3, r5
	bge _080195C8
	ldr r5, _080195B4 @ =0x00000904
	add r5, r5, r2
	mov ip, r5
	add r6, r0, #0
	add r5, r1, #0
_0801957A:
	mov r1, ip
	add r0, r6, r1
	lsl r1, r3, #2
	add r0, r0, r1
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080195B8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _080195C0
	mov r0, #0xD2
	cmp r4, #0
	beq _0801959A
	ldr r0, _080195BC @ =0x000080D2
_0801959A:
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
	b _080195CA
	.align 2, 0
_080195AC: .4byte 0x020192E4
_080195B0: .4byte 0x00000D64
_080195B4: .4byte 0x00000904
_080195B8: .4byte gCardIdToNumber
_080195BC: .4byte 0x000080D2
_080195C0:
	add r3, #1
	ldrb r0, [r5, #4]
	cmp r3, r0
	blt _0801957A
_080195C8:
	mov r0, #0
_080195CA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end ReturnGraveyardCardToHand

