	thumb_func_start DuelCmd_ShowCardScatter
DuelCmd_ShowCardScatter: @ 0x08015720
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r4, _08015748 @ =0x020185C0
	ldr r1, _0801574C @ =0x0000080A
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	cmp r0, #7
	bls _0801573E
	b _08015A04
_0801573E:
	lsl r0, r0, #2
	ldr r1, _08015750 @ =0x08015754
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08015748: .4byte 0x020185C0
_0801574C: .4byte 0x0000080A
_08015750: .4byte 0x08015754
_08015754:
	.4byte _08015774
	.4byte _0801578C
	.4byte _080157C8
	.4byte _080157D8
	.4byte _080157E8
	.4byte _080157F8
	.4byte _08015828
	.4byte _080159DC
_08015774:
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	ldr r2, _08015784 @ =0x020185C0
	ldr r3, _08015788 @ =0x0000080A
	add r2, r2, r3
	b _080157A2
_08015784: .4byte 0x020185C0
_08015788: .4byte 0x0000080A
_0801578C:
	bl UnloadDuelUiGfx
	ldr r2, _080157BC @ =0x020185C0
	ldr r7, _080157C0 @ =0x0000080C
	add r1, r2, r7
	ldr r0, _080157C4 @ =0xFFFFF01F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	sub r7, #2
	add r2, r2, r7
_080157A2:
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08015A16
	.align 2, 0
_080157BC: .4byte 0x020185C0
_080157C0: .4byte 0x0000080C
_080157C4: .4byte 0xFFFFF01F
_080157C8:
	ldr r1, _080157D4 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl LoadCardFrame
	b _080159DC
	.align 2, 0
_080157D4: .4byte 0x020185C0
_080157D8:
	ldr r1, _080157E4 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl LoadCardPicture
	b _080159DC
	.align 2, 0
_080157E4: .4byte 0x020185C0
_080157E8:
	ldr r1, _080157F4 @ =0x020185C0
	ldrh r0, [r1, #2]
	bl DrawCardInfo
	b _080159DC
	.align 2, 0
_080157F4: .4byte 0x020185C0
_080157F8:
	bl TextCellsClear
	ldr r4, _08015820 @ =0x020185C0
	ldrh r0, [r4, #2]
	bl DuelInfo_DrawCardNameCentered
	ldr r0, _08015824 @ =0x0000080A
	add r4, r4, r0
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _08015A16
_08015820: .4byte 0x020185C0
_08015824: .4byte 0x0000080A
_08015828:
	mov r1, #0
	ldr r2, _080158BC @ =0x04000050
	mov r9, r2
	ldr r3, _080158C0 @ =0x04000052
	mov r8, r3
_08015832:
	mov r5, #0
	lsl r7, r1, #5
	str r7, [sp, #0]
	lsl r0, r1, #1
	add r1, #1
	mov sl, r1
	add r0, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0xB
_08015844:
	lsl r1, r5, #5
	add r4, r1, #0
	add r4, #0x44
	ldr r2, [sp, #0]
	add r2, #2
	ldr r3, _080158C4 @ =0x020185C0
	ldr r7, _080158C8 @ =0x0000080C
	add r0, r3, r7
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x67
	ble _08015890
	add r2, r1, #0
	sub r2, #0x24
	ldr r3, [sp, #0]
	sub r3, #0x3E
	sub r0, #0x68
	ldr r1, _080158CC @ =0x08081768
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r0, [r0]
	mul r2, r0
	mul r3, r0
	add r0, r2, #0
	cmp r2, #0
	bge _0801587C
	add r0, #0xFF
_0801587C:
	asr r2, r0, #8
	add r0, r3, #0
	cmp r3, #0
	bge _08015886
	add r0, #0xFF
_08015886:
	asr r3, r0, #8
	add r4, r2, #0
	add r4, #0x68
	add r2, r3, #0
	add r2, #0x40
_08015890:
	ldr r1, _080158C4 @ =0x020185C0
	ldr r3, _080158C8 @ =0x0000080C
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0xF
	bgt _080158D0
	mov r7, #0xF4
	lsl r7, r7, #4
	add r0, r7, #0
	mov r1, r9
	strh r0, [r1]
	mov r0, #0x10
	sub r0, r0, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r3, r0
	mov r7, r8
	strh r3, [r7]
	b _08015900
	.align 2, 0
_080158BC: .4byte 0x04000050
_080158C0: .4byte 0x04000052
_080158C4: .4byte 0x020185C0
_080158C8: .4byte 0x0000080C
_080158CC: .4byte gScatterScaleCurve
_080158D0:
	cmp r3, #0x67
	ble _080158F6
	mov r1, #0xF4
	lsl r1, r1, #4
	add r0, r1, #0
	mov r7, r9
	strh r0, [r7]
	mov r1, #0x78
	sub r1, r1, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r3, #0
	sub r0, #0x68
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	mov r0, r8
	strh r1, [r0]
	b _08015900
_080158F6:
	mov r0, #0
	mov r1, r9
	strh r0, [r1]
	mov r3, r8
	strh r0, [r3]
_08015900:
	ldr r0, _08015930 @ =0x020185C0
	ldr r7, _08015934 @ =0x0000080C
	add r0, r0, r7
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0xF
	bgt _0801593C
	lsl r0, r2, #0x10
	orr r4, r0
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r6
	ldr r1, _08015938 @ =0x081A4424
	lsl r0, r3, #1
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	add r0, r4, #0
	mov r1, #0x80
	bl AddAffineSprite8bppAlpha
	b _0801594E
	.align 2, 0
_08015930: .4byte 0x020185C0
_08015934: .4byte 0x0000080C
_08015938: .4byte gPulseScaleCurve
_0801593C:
	lsl r0, r2, #0x10
	orr r4, r0
	lsl r2, r5, #0x12
	lsr r2, r2, #0x10
	add r2, r2, r6
	add r0, r4, #0
	mov r1, #0x80
	bl AddSprite8bppAlpha
_0801594E:
	add r5, #1
	cmp r5, #3
	bgt _08015956
	b _08015844
_08015956:
	mov r1, sl
	cmp r1, #4
	bgt _0801595E
	b _08015832
_0801595E:
	ldr r0, _080159B8 @ =0x020185C0
	ldr r1, _080159BC @ =0x0000080C
	add r4, r0, r1
	ldrh r3, [r4]
	lsl r0, r3, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0x77
	bgt _080159CC
	ldr r1, _080159C0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08015986
	ldr r1, _080159C4 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801599A
_08015986:
	cmp r2, #0x6F
	bgt _0801599A
	add r0, r2, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _080159C8 @ =0xFFFFF01F
	and r1, r3
	orr r1, r0
	strh r1, [r4]
_0801599A:
	ldr r2, _080159B8 @ =0x020185C0
	ldr r7, _080159BC @ =0x0000080C
	add r3, r2, r7
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080159C8 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _08015A16
_080159B8: .4byte 0x020185C0
_080159BC: .4byte 0x0000080C
_080159C0: .4byte 0x03000040
_080159C4: .4byte 0x0201CFB0
_080159C8: .4byte 0xFFFFF01F
_080159CC:
	ldr r0, _080159D4 @ =0x020185C0
	ldr r1, _080159D8 @ =0x0000080A
	add r3, r0, r1
	b _080159E2
_080159D4: .4byte 0x020185C0
_080159D8: .4byte 0x0000080A
_080159DC:
	ldr r2, _080159FC @ =0x020185C0
	ldr r7, _08015A00 @ =0x0000080A
	add r3, r2, r7
_080159E2:
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08015A16
	.align 2, 0
_080159FC: .4byte 0x020185C0
_08015A00: .4byte 0x0000080A
_08015A04:
	bl LoadDuelUiGfx
	ldr r0, _08015A28 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08015A16:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08015A28: .4byte 0x0000080D
	thumb_func_end DuelCmd_ShowCardScatter

