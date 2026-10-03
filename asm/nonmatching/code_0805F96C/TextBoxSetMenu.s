	thumb_func_start TextBoxSetMenu
TextBoxSetMenu: @ 0x08060308
	push {r4, lr}
	ldr r3, _08060324 @ =0x0201AE60
	mov r4, #0
	strh r0, [r3, #4]
	str r1, [r3, #0x18]
	str r2, [r3, #0x1C]
	ldrh r0, [r3, #4]
	cmp r0, #0
	blt _08060336
	cmp r0, #1
	ble _08060328
	cmp r0, #2
	beq _0806032E
	b _08060336
_08060324: .4byte 0x0201AE60
_08060328:
	str r4, [r3, #0x18]
	str r4, [r3, #0x1C]
	b _08060336
_0806032E:
	ldr r0, _0806033C @ =0x0805FBA5
	str r0, [r3, #0x18]
	ldr r0, _08060340 @ =0x0805FC19
	str r0, [r3, #0x1C]
_08060336:
	pop {r4}
	pop {r0}
	bx r0
_0806033C: .4byte TextBoxDrawChoiceCursor
_08060340: .4byte TextBoxHandleChoiceInput
	thumb_func_end TextBoxSetMenu

