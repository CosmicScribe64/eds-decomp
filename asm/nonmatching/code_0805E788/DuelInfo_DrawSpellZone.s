	thumb_func_start DuelInfo_DrawSpellZone
DuelInfo_DrawSpellZone: @ 0x0805F270
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r9, r2
	mov sl, r3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	lsl r0, r0, #6
	ldr r1, _0805F318 @ =0x0822C720
	add r6, r0, r1
	add r0, r6, #0
	bl StrLenWide
	mov r4, #0xC
	mov r7, #0xEC
	mov r1, #0
	str r1, [sp, #4]
	cmp r0, #0xF
	ble _0805F2A6
	mov r4, #0xA
_0805F2A6:
	mov r0, #0x20
	mov r1, #2
	bl TextCanvasInit
	lsr r5, r4, #1
	mov r1, #9
	sub r1, r1, r5
	lsl r4, r4, #8
	mov r0, #8
	add r2, r4, #0
	orr r2, r0
	mov r0, #3
	add r3, r6, #0
	bl TextDrawString
	mov r1, #8
	sub r1, r1, r5
	mov r0, #7
	orr r4, r0
	mov r0, #2
	add r2, r4, #0
	add r3, r6, #0
	bl TextDrawString
	mov r2, #1
	mov r0, r9
	and r2, r0
	mov r0, #0x94
	mov r1, sl
	mul r1, r0
	ldr r0, _0805F31C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0805F320 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0805F2F8
	b _0805F3F2
_0805F2F8:
	ldr r0, _0805F324 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805F328 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805F32C @ =0x0000015B
	cmp r1, r0
	beq _0805F336
	cmp r1, r0
	bgt _0805F330
	cmp r1, #0x47
	beq _0805F336
	b _0805F3F2
	.align 2, 0
_0805F318: .4byte gCardNames
_0805F31C: .4byte 0x00000D64
_0805F320: .4byte 0x0201930C
_0805F324: .4byte 0x000007FF
_0805F328: .4byte gCardIdToNumber
_0805F32C: .4byte 0x0000015B
_0805F330:
	ldr r0, _0805F394 @ =0x000004CE
	cmp r1, r0
	bne _0805F3F2
_0805F336:
	sub r7, #0x3C
	mov r2, #1
	mov r0, r9
	and r2, r0
	mov r0, #0x94
	mov r1, sl
	mul r1, r0
	add r0, r1, #0
	ldr r1, _0805F398 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0805F39C @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1A
	lsr r4, r0, #0x1C
	cmp r4, #9
	bgt _0805F3B0
	add r0, r7, #1
	ldr r5, _0805F3A0 @ =0x00000A08
	mov r1, #4
	add r2, r5, #0
	add r3, r4, #0
	bl TextDrawNumber
	ldr r2, _0805F3A4 @ =0x00000A07
	add r0, r7, #0
	mov r1, #3
	add r3, r4, #0
	bl TextDrawNumber
	add r0, r7, #0
	add r0, #0xB
	ldr r4, _0805F3A8 @ =0x08086490
	mov r1, #4
	add r2, r5, #0
	add r3, r4, #0
	bl TextDrawString
	add r0, r7, #0
	add r0, #0xA
	ldr r2, _0805F3AC @ =0x00000A04
	mov r1, #3
	add r3, r4, #0
	bl TextDrawString
	b _0805F3EE
_0805F394: .4byte 0x000004CE
_0805F398: .4byte 0x00000D64
_0805F39C: .4byte 0x0201930C
_0805F3A0: .4byte 0x00000A08
_0805F3A4: .4byte 0x00000A07
_0805F3A8: .4byte gTurnCounterLabel
_0805F3AC: .4byte 0x00000A04
_0805F3B0:
	sub r7, #0xA
	add r0, r7, #0
	add r0, #0xB
	ldr r5, _0805F448 @ =0x00000A08
	mov r1, #4
	add r2, r5, #0
	add r3, r4, #0
	bl TextDrawNumber
	add r0, r7, #0
	add r0, #0xA
	ldr r2, _0805F44C @ =0x00000A07
	mov r1, #3
	add r3, r4, #0
	bl TextDrawNumber
	add r0, r7, #0
	add r0, #0x15
	ldr r4, _0805F450 @ =0x08086490
	mov r1, #4
	add r2, r5, #0
	add r3, r4, #0
	bl TextDrawString
	add r0, r7, #0
	add r0, #0x14
	ldr r2, _0805F454 @ =0x00000A04
	mov r1, #3
	add r3, r4, #0
	bl TextDrawString
_0805F3EE:
	mov r0, #1
	str r0, [sp, #4]
_0805F3F2:
	ldr r0, _0805F458 @ =0x0201CFB8
	mov r1, #9
	bl TextCanvasToTiles
	ldr r0, _0805F45C @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F460 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0805F416
	b _0805F5A6
_0805F416:
	ldr r0, [sp, #0]
	cmp r0, #0
	bne _0805F41E
	b _0805F5A6
_0805F41E:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq _0805F426
	b _0805F5A6
_0805F426:
	ldr r1, _0805F464 @ =0x02011C20
	mov r0, #0x7F
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805F470
	ldr r1, _0805F468 @ =0x08086470
	mov r0, #0x17
	mov r2, #5
	bl TextCellsPutString
	ldr r1, _0805F46C @ =0x08086478
	mov r0, #0x37
	mov r2, #4
	bl TextCellsPutString
	b _0805F484
_0805F448: .4byte 0x00000A08
_0805F44C: .4byte 0x00000A07
_0805F450: .4byte gTurnCounterLabel
_0805F454: .4byte 0x00000A04
_0805F458: .4byte 0x0201CFB8
_0805F45C: .4byte 0x000007FF
_0805F460: .4byte gCardStats
_0805F464: .4byte 0x02011C20
_0805F468: .4byte gInfoAtkLabelJp
_0805F46C: .4byte gInfoDefLabelJp
_0805F470:
	ldr r1, _0805F4A8 @ =0x08086480
	mov r0, #0x17
	mov r2, #5
	bl TextCellsPutString
	ldr r1, _0805F4AC @ =0x08086488
	mov r0, #0x37
	mov r2, #4
	bl TextCellsPutString
_0805F484:
	ldr r0, _0805F4B0 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F4B4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F4C2
	cmp r0, #0x17
	ble _0805F4B8
	cmp r0, #0x18
	beq _0805F4BC
	b _0805F4C2
_0805F4A8: .4byte gInfoAtkLabel
_0805F4AC: .4byte gInfoDefLabel
_0805F4B0: .4byte 0x000007FF
_0805F4B4: .4byte gCardStats
_0805F4B8:
	mov r0, #0
	b _0805F4DA
_0805F4BC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805F4DA
_0805F4C2:
	ldr r0, _0805F50C @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F510 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805F4DA:
	add r1, r0, #0
	mov r0, #0x1A
	mov r2, #7
	mov r3, #4
	bl TextCellsPutNumber
	ldr r0, _0805F50C @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F510 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F51E
	cmp r0, #0x17
	ble _0805F514
	cmp r0, #0x18
	beq _0805F518
	b _0805F51E
	.align 2, 0
_0805F50C: .4byte 0x000007FF
_0805F510: .4byte gCardStats
_0805F514:
	mov r0, #0
	b _0805F536
_0805F518:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805F536
_0805F51E:
	ldr r0, _0805F570 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F574 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _0805F578 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805F536:
	add r1, r0, #0
	mov r0, #0x3A
	mov r2, #7
	mov r3, #4
	bl TextCellsPutNumber
	mov r0, #0x18
	mov r1, #3
	bl TextCellsCopyBgTile
	ldr r0, _0805F570 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F574 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805F584
	cmp r0, #0x17
	ble _0805F57C
	cmp r0, #0x18
	beq _0805F580
	b _0805F584
	.align 2, 0
_0805F570: .4byte 0x000007FF
_0805F574: .4byte gCardStats
_0805F578: .4byte 0x000001FF
_0805F57C:
	mov r0, #0
	b _0805F59A
_0805F580:
	mov r0, #0xA
	b _0805F59A
_0805F584:
	ldr r0, _0805F5E4 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0805F5E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805F59A:
	add r1, r0, #0
	mov r0, #0x37
	mov r2, #7
	mov r3, #2
	bl TextCellsPutNumber
_0805F5A6:
	mov r1, #1
	mov r0, r9
	and r1, r0
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	ldr r0, _0805F5EC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0805F5F0 @ =0x0201930C
	add r2, r2, r0
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	beq _0805F636
	ldr r0, _0805F5E4 @ =0x000007FF
	mov r1, r8
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805F5F4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805F5F8 @ =0x00000479
	cmp r1, r0
	beq _0805F5FC
	mov r0, #0xB5
	lsl r0, r0, #3
	cmp r1, r0
	beq _0805F61C
	b _0805F636
_0805F5E4: .4byte 0x000007FF
_0805F5E8: .4byte gCardStats
_0805F5EC: .4byte 0x00000D64
_0805F5F0: .4byte 0x0201930C
_0805F5F4: .4byte gCardIdToNumber
_0805F5F8: .4byte 0x00000479
_0805F5FC:
	ldr r1, _0805F618 @ =0x081A41A4
	add r0, r2, #0
	add r0, #0x90
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1B
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r2, [r0]
	mov r0, #0x1C
	mov r1, #9
	bl TextCellsLoadIcon
	b _0805F636
_0805F618: .4byte gCardTypeIcons
_0805F61C:
	ldr r1, _0805F648 @ =0x081A41F8
	add r0, r2, #0
	add r0, #0x90
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1B
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r2, [r0]
	mov r0, #0x1C
	mov r1, #9
	bl TextCellsLoadIcon
_0805F636:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805F648: .4byte gCardAttributeIcons
	thumb_func_end DuelInfo_DrawSpellZone

