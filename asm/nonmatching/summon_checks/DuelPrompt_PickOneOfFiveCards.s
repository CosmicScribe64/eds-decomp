	thumb_func_start DuelPrompt_PickOneOfFiveCards
DuelPrompt_PickOneOfFiveCards: @ 0x08053F98
	push {r4, r5, lr}
	ldr r4, _08053FB8 @ =0x020192E0
	ldr r0, _08053FBC @ =0x00001B50
	add r1, r4, r0
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08053FC0
	bl Random
	mov r1, #5
	bl __modsi3
	b _08053FCE
	.align 2, 0
_08053FB8: .4byte 0x020192E0
_08053FBC: .4byte 0x00001B50
_08053FC0:
	ldr r0, _08053FE4 @ =0x00001B62
	add r5, r4, r0
	ldrb r0, [r5]
	cmp r0, #0
	beq _08053FF0
	ldr r0, _08053FE8 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
_08053FCE:
	lsl r0, r0, #1
	ldr r2, _08053FEC @ =0x00001B52
	add r1, r4, r2
	add r0, r0, r1
	ldrh r1, [r0]
	add r2, #0x12
	add r0, r4, r2
	strh r1, [r0]
	mov r0, #1
	b _0805400E
	.align 2, 0
_08053FE4: .4byte 0x00001B62
_08053FE8: .4byte 0x0201AE60
_08053FEC: .4byte 0x00001B52
_08053FF0:
	ldr r0, _08054014 @ =0x00000206
	ldr r1, _08054018 @ =0x00000213
	ldr r3, _0805401C @ =0x08086350
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _08054020 @ =0x08053E59
	ldr r2, _08054024 @ =0x08053EF9
	mov r0, #5
	bl TextBoxSetMenu
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
_0805400E:
	pop {r4, r5}
	pop {r1}
	bx r1
_08054014: .4byte 0x00000206
_08054018: .4byte 0x00000213
_0805401C: .4byte gStrPromptSelectOneOfFive
_08054020: .4byte FiveCardMenu_Draw
_08054024: .4byte FiveCardMenu_HandleInput
	thumb_func_end DuelPrompt_PickOneOfFiveCards

