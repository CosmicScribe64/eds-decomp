	thumb_func_start EffectPlaceParasiteParacideOnDeckResolve
EffectPlaceParasiteParacideOnDeckResolve: @ 0x0803A890
	push {r4, r5, r6, lr}
	sub sp, #0x80
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803A966
	ldr r1, _0803A8C4 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _0803A90C
	mov r1, #1
	add r0, r1, #0
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	beq _0803A8CC
	ldr r0, _0803A8C8 @ =0x0201AE60
	strh r1, [r0, #0x14]
	mov r0, #0x7F
	b _0803A968
	.align 2, 0
_0803A8C4: .4byte 0x02017A40
_0803A8C8: .4byte 0x0201AE60
_0803A8CC:
	ldr r1, _0803A8F8 @ =0x08083604
	ldr r0, _0803A8FC @ =0x086243E8
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _0803A900 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _0803A904 @ =0x00000206
	ldr r1, _0803A908 @ =0x00000613
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7F
	b _0803A968
_0803A8F8: .4byte gStrPlaceOnDeckTopPrompt
_0803A8FC: .4byte gUnk_086243E8
_0803A900: .4byte gCardNames
_0803A904: .4byte 0x00000206
_0803A908: .4byte 0x00000613
_0803A90C:
	ldr r0, _0803A970 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0803A966
	mov r2, #0xFA
	lsl r2, r2, #2
	add r5, r1, r2
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803A974 @ =0x000002FA
	add r2, r5, #0
	bl RemoveDeckCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _0803A966
	mov r6, #1
	add r0, r6, #0
	ldrb r2, [r4, #2]
	and r0, r2
	mov r1, #0x60
	cmp r0, #0
	beq _0803A940
	ldr r1, _0803A978 @ =0x00008060
_0803A940:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r6, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0x6A
	cmp r0, #0
	beq _0803A95A
	ldr r3, _0803A97C @ =0x0000806A
_0803A95A:
	ldrh r1, [r5]
	ldrh r2, [r5, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
_0803A966:
	mov r0, #0
_0803A968:
	add sp, #0x80
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0803A970: .4byte 0x0201AE60
_0803A974: .4byte 0x000002FA
_0803A978: .4byte 0x00008060
_0803A97C: .4byte 0x0000806A
	thumb_func_end EffectPlaceParasiteParacideOnDeckResolve

