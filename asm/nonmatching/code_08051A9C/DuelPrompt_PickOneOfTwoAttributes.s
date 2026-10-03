	thumb_func_start DuelPrompt_PickOneOfTwoAttributes
DuelPrompt_PickOneOfTwoAttributes: @ 0x08052714
	push {r4, r5, r6, lr}
	sub sp, #0x80
	ldr r4, _0805278C @ =0x020192E0
	ldr r1, _08052790 @ =0x00001B62
	add r0, r4, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _080527E8
	ldr r1, _08052794 @ =0x08086254
	mov r0, sp
	bl StrCopy
	ldr r1, _08052798 @ =0x0808628C
	mov r0, sp
	bl StrCat
	ldr r6, _0805279C @ =0x0819D264
	ldr r2, _080527A0 @ =0x00001B52
	add r5, r4, r2
	mov r4, #1
_0805273C:
	mov r0, sp
	ldr r1, _080527A4 @ =0x08086290
	bl StrCat
	ldrh r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r6
	ldr r1, [r0]
	mov r0, sp
	bl StrCat
	mov r0, sp
	ldr r1, _08052798 @ =0x0808628C
	bl StrCat
	add r5, #2
	sub r4, #1
	cmp r4, #0
	bge _0805273C
	ldr r0, _080527A8 @ =0x00000206
	ldr r1, _080527AC @ =0x0000050F
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	ldr r4, _0805278C @ =0x020192E0
	ldr r2, _080527B0 @ =0x00001B50
	add r1, r4, r2
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080527BC
	ldr r1, _080527B4 @ =0x0805FBA5
	ldr r2, _080527B8 @ =0x08052669
	mov r0, #5
	bl TextBoxSetMenu
	b _080527D0
	.align 2, 0
_0805278C: .4byte 0x020192E0
_08052790: .4byte 0x00001B62
_08052794: .4byte gStrPromptSelectAttribute
_08052798: .4byte gStrNewline
_0805279C: .4byte gAttributeNames
_080527A0: .4byte 0x00001B52
_080527A4: .4byte gStrMenuIndent
_080527A8: .4byte 0x00000206
_080527AC: .4byte 0x0000050F
_080527B0: .4byte 0x00001B50
_080527B4: .4byte TextBoxDrawChoiceCursor
_080527B8: .4byte TextBoxHandleChoiceInputCpu
_080527BC:
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r0, _080527E0 @ =0x00001B62
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_080527D0:
	ldr r0, _080527E4 @ =0x020192E0
	ldr r1, _080527E0 @ =0x00001B62
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r0, #0
	b _080527FE
_080527E0: .4byte 0x00001B62
_080527E4: .4byte 0x020192E0
_080527E8:
	ldr r0, _08052808 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	lsl r0, r0, #1
	ldr r2, _0805280C @ =0x00001B52
	add r1, r4, r2
	add r0, r0, r1
	ldrh r1, [r0]
	add r2, #0x12
	add r0, r4, r2
	strh r1, [r0]
	mov r0, #1
_080527FE:
	add sp, #0x80
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08052808: .4byte 0x0201AE60
_0805280C: .4byte 0x00001B52
	thumb_func_end DuelPrompt_PickOneOfTwoAttributes

