	thumb_func_start DuelPrompt_SelectTwoAttributes
DuelPrompt_SelectTwoAttributes: @ 0x080525C4
	push {r4, lr}
	ldr r2, _080525E4 @ =0x020192E0
	ldr r0, _080525E8 @ =0x00001B62
	add r4, r2, r0
	ldrb r0, [r4]
	cmp r0, #0
	beq _080525F4
	cmp r0, #1
	beq _0805261C
	ldr r0, _080525EC @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _080525F0 @ =0x00001B66
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
	b _08052644
_080525E4: .4byte 0x020192E0
_080525E8: .4byte 0x00001B62
_080525EC: .4byte 0x0201AE60
_080525F0: .4byte 0x00001B66
_080525F4:
	ldr r0, _08052608 @ =0x00000206
	ldr r1, _0805260C @ =0x0000040F
	ldr r3, _08052610 @ =0x08086254
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _08052614 @ =0x08052391
	ldr r2, _08052618 @ =0x0805243D
	b _08052636
	.align 2, 0
_08052608: .4byte 0x00000206
_0805260C: .4byte 0x0000040F
_08052610: .4byte gStrPromptSelectAttribute
_08052614: .4byte AttributeMenu_Draw
_08052618: .4byte AttributeMenu_HandleInput
_0805261C:
	ldr r0, _0805264C @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _08052650 @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	ldr r0, _08052654 @ =0x00000206
	ldr r1, _08052658 @ =0x00000411
	ldr r3, _0805265C @ =0x0808626C
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _08052660 @ =0x08052391
	ldr r2, _08052664 @ =0x080524A9
_08052636:
	mov r0, #5
	bl TextBoxSetMenu
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_08052644:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0805264C: .4byte 0x0201AE60
_08052650: .4byte 0x00001B64
_08052654: .4byte 0x00000206
_08052658: .4byte 0x00000411
_0805265C: .4byte gStrPromptSelectAnotherAttribute
_08052660: .4byte AttributeMenu_Draw
_08052664: .4byte AttributeMenu_HandleInputExcludeFirst
	thumb_func_end DuelPrompt_SelectTwoAttributes

