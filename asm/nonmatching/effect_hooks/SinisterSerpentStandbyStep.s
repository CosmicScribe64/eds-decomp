	thumb_func_start SinisterSerpentStandbyStep
SinisterSerpentStandbyStep: @ 0x0804691C
	push {r4, r5, r6, lr}
	sub sp, #0x80
	add r5, r0, #0
	ldr r0, _08046938 @ =0x020192E0
	ldr r1, _0804693C @ =0x00001B22
	add r6, r0, r1
	ldrb r0, [r6]
	cmp r0, #0
	beq _08046940
	cmp r0, #1
	beq _080469A4
_08046932:
	mov r0, #1
	b _080469CA
	.align 2, 0
_08046938: .4byte 0x020192E0
_0804693C: .4byte 0x00001B22
_08046940:
	mov r4, #0xED
	lsl r4, r4, #1
	add r0, r5, #0
	add r1, r4, #0
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	beq _08046932
	cmp r5, #0
	beq _08046960
	ldr r0, _0804695C @ =0x0201AE60
	mov r1, #1
	strh r1, [r0, #0x14]
	b _080469C2
_0804695C: .4byte 0x0201AE60
_08046960:
	ldr r1, _08046990 @ =0x080854AC
	lsl r0, r4, #1
	ldr r2, _08046994 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _08046998 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _0804699C @ =0x00000206
	ldr r1, _080469A0 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	b _080469C2
	.align 2, 0
_08046990: .4byte gStrSinisterSerpentPrompt
_08046994: .4byte gCardNumberToId
_08046998: .4byte gCardNames
_0804699C: .4byte 0x00000206
_080469A0: .4byte 0x00000712
_080469A4:
	ldr r0, _080469D4 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _080469C2
	mov r4, #0xED
	lsl r4, r4, #1
	ldr r0, _080469D8 @ =0x086241A8
	ldrh r1, [r0]
	add r0, r5, #0
	bl ShowCardEffect
	add r0, r5, #0
	add r1, r4, #0
	bl ReturnGraveyardCardToHand
_080469C2:
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	mov r0, #0
_080469CA:
	add sp, #0x80
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080469D4: .4byte 0x0201AE60
_080469D8: .4byte gCardNumberToId_SinisterSerpent
	thumb_func_end SinisterSerpentStandbyStep

