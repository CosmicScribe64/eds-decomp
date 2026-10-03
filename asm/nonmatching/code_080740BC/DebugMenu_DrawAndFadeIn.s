	thumb_func_start DebugMenu_DrawAndFadeIn
DebugMenu_DrawAndFadeIn: @ 0x08074794
	push {r4, r5, r6, lr}
	ldr r0, _080747B4 @ =0x03000040
	ldr r1, _080747B8 @ =0x00004859
	add r3, r0, r1
	ldrb r0, [r3]
	cmp r0, #0
	beq _080747BC
	cmp r0, #1
	beq _08074848
	mov r0, #4
	bl FadeFromBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08074860
	.align 2, 0
_080747B4: .4byte 0x03000040
_080747B8: .4byte 0x00004859
_080747BC:
	bl ClearBgMapBuffer0
	bl DebugMenu_DrawLanguage
	ldr r0, _08074828 @ =0x08070025
	ldr r1, _0807482C @ =0x000403F8
	ldr r2, _08074830 @ =0x0000083C
	mov r3, #1
	bl DrawBgDecimal
	ldr r1, _08074834 @ =0x0000027D
	mov r0, #0
	bl SetTextArea
	mov r6, #0
	ldr r3, _08074838 @ =0x081A73A0
	ldr r0, [r3, #0x40]
	cmp r0, #0
	beq _08074812
	add r4, r3, #0
	mov r5, #0x20
_080747E6:
	mov r2, #2
	lsl r1, r6, #1
	add r0, r1, #4
	cmp r0, #0x13
	ble _080747F4
	mov r2, #0x10
	sub r0, #0x10
_080747F4:
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	orr r2, r0
	add r0, r2, #0
	ldr r1, _0807483C @ =0x00000807
	add r2, r5, #0
	add r3, r4, #0
	bl DrawBgString
	add r4, #0x44
	add r5, #0x10
	add r6, #1
	ldr r0, [r4, #0x40]
	cmp r0, #0
	bne _080747E6
_08074812:
	bl DebugMenu_DrawDate
	bl DebugMenu_DrawCalendarEvents
	ldr r0, _08074840 @ =0x03000040
	ldr r4, _08074844 @ =0x00004859
	add r0, r0, r4
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0807485E
_08074828: .4byte 0x08070025
_0807482C: .4byte 0x000403F8
_08074830: .4byte 0x0000083C
_08074834: .4byte 0x0000027D
_08074838: .4byte gDebugMenuItems
_0807483C: .4byte 0x00000807
_08074840: .4byte 0x03000040
_08074844: .4byte 0x00004859
_08074848:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r4, #0x88
	lsl r4, r4, #5
	add r1, r4, #0
	orr r0, r1
	strh r0, [r2]
	ldrb r0, [r3]
	add r0, #1
	strb r0, [r3]
_0807485E:
	mov r0, #0
_08074860:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end DebugMenu_DrawAndFadeIn
	.align 2, 0

