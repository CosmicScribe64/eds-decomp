	thumb_func_start DestinyBoardScene_Update
DestinyBoardScene_Update: @ 0x080279AC
	push {r4, r5, r6, lr}
	sub sp, #0x18
	ldr r5, _08027A74 @ =0x02020DBC
	add r0, r5, #0
	bl FadeTick
	bl DestinyBoardScene_AdvanceWave
	add r4, r5, #0
	add r4, #0x59
	ldrb r0, [r4]
	cmp r0, #1
	bne _080279F0
	mov r1, #8
	neg r1, r1
	add r0, r1, #0
	ldrb r2, [r5, #0x1C]
	and r0, r2
	mov r3, #1
	orr r0, r3
	strb r0, [r5, #0x1C]
	add r2, r5, #0
	add r2, #0x30
	add r0, r1, #0
	ldrb r6, [r2]
	and r0, r6
	orr r0, r3
	strb r0, [r2]
	add r0, r5, #0
	add r0, #0x44
	ldrb r2, [r0]
	and r1, r2
	orr r1, r3
	strb r1, [r0]
_080279F0:
	ldrb r0, [r4]
	cmp r0, #0
	beq _080279FA
	sub r0, #1
	strb r0, [r4]
_080279FA:
	mov r4, #0
	add r5, #8
_080279FE:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r5
	bl ScrollLayer_Move
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #3
	bls _080279FE
	ldr r2, _08027A78 @ =0x02020310
	ldr r6, _08027A7C @ =0x00000AB4
	add r1, r2, r6
	mov r3, #7
	add r0, r3, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08027A46
	ldr r0, _08027A80 @ =0x00000AC8
	add r1, r2, r0
	add r0, r3, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08027A46
	ldr r1, _08027A84 @ =0x00000B05
	add r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _08027A46
	add r6, #0x3E
	add r1, r2, r6
	ldr r0, _08027A88 @ =0x0000FFFE
	strh r0, [r1]
_08027A46:
	ldr r4, _08027A8C @ =0x02020DD8
	add r0, r4, #0
	bl ScrollLayer_StreamRow
	add r0, r4, #0
	add r0, #0x14
	bl ScrollLayer_StreamRow
	ldr r1, _08027A90 @ =0xFFFFFE50
	add r0, r4, r1
	bl AnimBlockTick
	add r5, r4, #0
	add r5, #0x3C
	ldrb r1, [r5]
	cmp r1, #1
	beq _08027B14
	cmp r1, #1
	bgt _08027A94
	cmp r1, #0
	beq _08027A9A
	b _08027B62
	.align 2, 0
_08027A74: .4byte 0x02020DBC
_08027A78: .4byte 0x02020310
_08027A7C: .4byte 0x00000AB4
_08027A80: .4byte 0x00000AC8
_08027A84: .4byte 0x00000B05
_08027A88: .4byte 0x0000FFFE
_08027A8C: .4byte 0x02020DD8
_08027A90: .4byte 0xFFFFFE50
_08027A94:
	cmp r1, #2
	beq _08027B44
	b _08027B62
_08027A9A:
	ldr r2, _08027AFC @ =0xFFFFFE5E
	add r1, r4, r2
	ldrb r2, [r1]
	mov r0, #0
	ldsb r0, [r1, r0]
	cmp r0, #0
	bne _08027ABA
	mov r0, #0xFF
	strb r0, [r1]
	ldr r6, _08027B00 @ =0xFFFFFE72
	add r1, r4, r6
	mov r0, #1
	strb r0, [r1]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_08027ABA:
	ldr r0, _08027B04 @ =0xFFFFFE5C
	add r5, r4, r0
	ldr r0, [r5]
	ldr r4, _08027B08 @ =0xFF00FF00
	and r0, r4
	mov r1, #0xA0
	lsl r1, r1, #4
	cmp r0, r1
	bne _08027AD6
	ldr r0, _08027B0C @ =0x086DE178
	ldr r1, _08027B10 @ =0x06010000
	mov r2, #0x10
	bl CopyTileSheetTo2D
_08027AD6:
	ldr r0, [r5]
	and r0, r4
	mov r1, #0x80
	lsl r1, r1, #1
	cmp r0, r1
	bne _08027AE8
	mov r0, #0x2D
	bl PlaySE
_08027AE8:
	ldr r0, [r5]
	and r0, r4
	mov r1, #0xB0
	lsl r1, r1, #4
	cmp r0, r1
	bne _08027B62
	mov r0, #0x2E
	bl PlaySE
	b _08027B62
_08027AFC: .4byte 0xFFFFFE5E
_08027B00: .4byte 0xFFFFFE72
_08027B04: .4byte 0xFFFFFE5C
_08027B08: .4byte 0xFF00FF00
_08027B0C: .4byte gFinalLetterTiles
_08027B10: .4byte 0x06010000
_08027B14:
	ldr r6, _08027B3C @ =0xFFFFFE72
	add r2, r4, r6
	ldrb r3, [r2]
	mov r0, #0
	ldsb r0, [r2, r0]
	cmp r0, #0
	bne _08027B62
	mov r0, #0xFF
	strb r0, [r2]
	ldr r2, _08027B40 @ =0xFFFFFE9A
	add r0, r4, r2
	strb r1, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	add r1, r4, #0
	add r1, #0x58
	mov r0, #0x1E
	strb r0, [r1]
	b _08027B62
_08027B3C: .4byte 0xFFFFFE72
_08027B40: .4byte 0xFFFFFE9A
_08027B44:
	ldr r6, _08027B78 @ =0xFFFFFE9A
	add r1, r4, r6
	mov r0, #1
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x58
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	bne _08027B62
	mov r0, #3
	strb r0, [r5]
_08027B62:
	ldr r4, _08027B7C @ =0x02020310
	ldr r1, _08027B80 @ =0x00000B04
	add r0, r4, r1
	ldrb r0, [r0]
	cmp r0, #0
	blt _08027BDE
	cmp r0, #2
	ble _08027B84
	cmp r0, #3
	beq _08027BBC
	b _08027BDE
_08027B78: .4byte 0xFFFFFE9A
_08027B7C: .4byte 0x02020310
_08027B80: .4byte 0x00000B04
_08027B84:
	ldr r2, _08027BB4 @ =0x00000918
	add r0, r4, r2
	mov r2, #0
	str r2, [sp, #0]
	str r2, [sp, #4]
	mov r1, #8
	str r1, [sp, #8]
	str r2, [sp, #0xC]
	ldr r6, _08027BB8 @ =0x00000AE2
	add r1, r4, r6
	ldrh r1, [r1]
	lsl r2, r1, #0x10
	asr r2, r2, #0x14
	mov r1, #0x48
	sub r1, r1, r2
	str r1, [sp, #0x10]
	str r4, [sp, #0x14]
	mov r1, #0
	mov r2, #1
	mov r3, #0
	bl AnimBlockDraw
	b _08027BDE
	.align 2, 0
_08027BB4: .4byte 0x00000918
_08027BB8: .4byte 0x00000AE2
_08027BBC:
	bl DestinyBoardScene_LaunchLetters
	ldr r0, _08027C14 @ =0x00000962
	add r1, r4, r0
	mov r0, #1
	strb r0, [r1]
	ldr r1, _08027C18 @ =0x00000ACE
	add r0, r4, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x10
	asr r0, r0, #0x14
	mov r2, #0x48
	sub r2, r2, r0
	mov r0, #3
	mov r1, #0
	bl DestinyBoardScene_DrawFinalLetters
_08027BDE:
	mov r4, #0
	ldr r5, _08027C1C @ =0x02020928
_08027BE2:
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #3
	add r0, r0, r5
	bl ObjAffineApply
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #4
	bls _08027BE2
	ldr r4, _08027C20 @ =0x02020310
	add r0, r4, #0
	bl OamListFlush
	add r0, r4, #0
	bl OamListClear
	ldr r2, _08027C24 @ =0x00000AB2
	add r4, r4, r2
	ldrb r4, [r4]
	cmp r4, #2
	beq _08027C28
	mov r0, #0
	b _08027C2A
_08027C14: .4byte 0x00000962
_08027C18: .4byte 0x00000ACE
_08027C1C: .4byte 0x02020928
_08027C20: .4byte 0x02020310
_08027C24: .4byte 0x00000AB2
_08027C28:
	mov r0, #1
_08027C2A:
	add sp, #0x18
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end DestinyBoardScene_Update
	.align 2, 0

