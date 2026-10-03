	thumb_func_start DuelPrompt_ConfirmGraveyardSummon
DuelPrompt_ConfirmGraveyardSummon: @ 0x0802215C
	push {r4, r5, lr}
	sub sp, #0x100
	ldr r2, _080221A8 @ =0x020192E0
	ldr r0, _080221AC @ =0x00001B62
	add r5, r2, r0
	ldrb r1, [r5]
	cmp r1, #0
	bne _080221C8
	ldr r3, _080221B0 @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	ldr r4, _080221B4 @ =0x08081E6C
	ldr r0, _080221B8 @ =0x086249D4
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _080221BC @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	add r1, r4, #0
	bl FormatStr
	ldr r0, _080221C0 @ =0x00000206
	ldr r1, _080221C4 @ =0x00000713
	mov r2, #0xB
	add r3, r4, #0
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _080221D4
	.align 2, 0
_080221A8: .4byte 0x020192E0
_080221AC: .4byte 0x00001B62
_080221B0: .4byte 0x00001B64
_080221B4: .4byte gStrPromptOpponentSpecialSummonedFmt
_080221B8: .4byte gUnk_086249D4
_080221BC: .4byte gCardNames
_080221C0: .4byte 0x00000206
_080221C4: .4byte 0x00000713
_080221C8:
	ldr r0, _080221DC @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _080221E0 @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
_080221D4:
	add sp, #0x100
	pop {r4, r5}
	pop {r1}
	bx r1
_080221DC: .4byte 0x0201AE60
_080221E0: .4byte 0x00001B64
	thumb_func_end DuelPrompt_ConfirmGraveyardSummon

