	thumb_func_start DuelPrompt_SelectOwnReplacementTarget
DuelPrompt_SelectOwnReplacementTarget: @ 0x080221E4
	push {r4, r5, r6, lr}
	add r6, r1, #0
	ldr r4, _080221FC @ =0x020192E0
	ldr r0, _08022200 @ =0x00001B62
	add r5, r4, r0
	ldrb r1, [r5]
	cmp r1, #0
	beq _08022204
	cmp r1, #1
	beq _08022230
	mov r0, #1
	b _0802225A
_080221FC: .4byte 0x020192E0
_08022200: .4byte 0x00001B62
_08022204:
	ldr r2, _08022220 @ =0x00001B64
	add r0, r4, r2
	strh r1, [r0]
	ldr r0, _08022224 @ =0x00000206
	ldr r1, _08022228 @ =0x00000713
	ldr r3, _0802222C @ =0x08081EE0
	mov r2, #0xB
	bl TextBoxOpen
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _08022258
	.align 2, 0
_08022220: .4byte 0x00001B64
_08022224: .4byte 0x00000206
_08022228: .4byte 0x00000713
_0802222C: .4byte gStrPromptSelectOwnReplacementTarget
_08022230:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08022258
	ldr r0, _08022260 @ =0x0201CFB0
	ldr r1, _08022264 @ =0x0000082C
	add r0, r0, r1
	ldr r1, [r0]
	cmp r1, r6
	beq _08022252
	ldr r2, _08022268 @ =0x00001B64
	add r0, r4, r2
	strh r1, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_08022252:
	mov r0, #3
	bl PlaySE
_08022258:
	mov r0, #0
_0802225A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08022260: .4byte 0x0201CFB0
_08022264: .4byte 0x0000082C
_08022268: .4byte 0x00001B64
	thumb_func_end DuelPrompt_SelectOwnReplacementTarget

