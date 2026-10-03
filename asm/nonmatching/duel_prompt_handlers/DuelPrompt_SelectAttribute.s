	thumb_func_start DuelPrompt_SelectAttribute
DuelPrompt_SelectAttribute: @ 0x08052560
	push {r4, lr}
	ldr r2, _0805257C @ =0x020192E0
	ldr r0, _08052580 @ =0x00001B62
	add r4, r2, r0
	ldrb r0, [r4]
	cmp r0, #0
	beq _0805258C
	ldr r0, _08052584 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _08052588 @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
	b _080525AA
_0805257C: .4byte 0x020192E0
_08052580: .4byte 0x00001B62
_08052584: .4byte 0x0201AE60
_08052588: .4byte 0x00001B64
_0805258C:
	ldr r0, _080525B0 @ =0x00000206
	ldr r1, _080525B4 @ =0x0000040F
	ldr r3, _080525B8 @ =0x08086254
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _080525BC @ =0x08052391
	ldr r2, _080525C0 @ =0x0805243D
	mov r0, #5
	bl TextBoxSetMenu
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_080525AA:
	pop {r4}
	pop {r1}
	bx r1
_080525B0: .4byte 0x00000206
_080525B4: .4byte 0x0000040F
_080525B8: .4byte gStrPromptSelectAttribute
_080525BC: .4byte AttributeMenu_Draw
_080525C0: .4byte AttributeMenu_HandleInput
	thumb_func_end DuelPrompt_SelectAttribute

