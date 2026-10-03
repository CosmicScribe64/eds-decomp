	thumb_func_start Campaign_RecordDuelResult
Campaign_RecordDuelResult: @ 0x0801BE90
	push {r4, r5, r6, r7, lr}
	bl GetCampaignLevel
	add r6, r0, #0
	mov r4, #0
	mov r7, #0
	ldr r5, _0801BECC @ =0x080819BE
_0801BE9E:
	ldrh r0, [r5]
	bl IsPackUnlocked
	cmp r0, #0
	beq _0801BEAA
	add r7, #1
_0801BEAA:
	add r5, #2
	add r4, #1
	cmp r4, #0x1B
	bls _0801BE9E
	ldr r0, _0801BED0 @ =0x020192E0
	ldr r1, _0801BED4 @ =0x00001B12
	add r0, r0, r1
	ldrb r0, [r0]
	lsr r0, r0, #6
	cmp r0, #2
	beq _0801BEF8
	cmp r0, #2
	bgt _0801BED8
	cmp r0, #1
	beq _0801BEDE
	b _0801BF24
	.align 2, 0
_0801BECC: .4byte gPackDisplayOrder
_0801BED0: .4byte 0x020192E0
_0801BED4: .4byte 0x00001B12
_0801BED8:
	cmp r0, #3
	beq _0801BF14
	b _0801BF24
_0801BEDE:
	ldr r0, _0801BEF0 @ =0x03000040
	ldr r2, _0801BEF4 @ =0x00004870
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	bl RecordDuelWin
	b _0801BF24
_0801BEF0: .4byte 0x03000040
_0801BEF4: .4byte 0x00004870
_0801BEF8:
	ldr r0, _0801BF0C @ =0x03000040
	ldr r1, _0801BF10 @ =0x00004870
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	bl RecordDuelLoss
	b _0801BF24
	.align 2, 0
_0801BF0C: .4byte 0x03000040
_0801BF10: .4byte 0x00004870
_0801BF14:
	ldr r0, _0801BF6C @ =0x03000040
	ldr r2, _0801BF70 @ =0x00004870
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	bl RecordDuelDraw
_0801BF24:
	bl GetCampaignLevel
	cmp r0, r6
	ble _0801BF3A
	ldr r0, _0801BF74 @ =0x02011C20
	ldr r1, _0801BF78 @ =0x00002164
	add r0, r0, r1
	mov r1, #1
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
_0801BF3A:
	mov r4, #0
	mov r6, #0
	ldr r5, _0801BF7C @ =0x080819BE
_0801BF40:
	ldrh r0, [r5]
	bl IsPackUnlocked
	cmp r0, #0
	beq _0801BF4C
	add r6, #1
_0801BF4C:
	add r5, #2
	add r4, #1
	cmp r4, #0x1B
	bls _0801BF40
	cmp r6, r7
	ble _0801BF66
	ldr r0, _0801BF74 @ =0x02011C20
	ldr r1, _0801BF78 @ =0x00002164
	add r0, r0, r1
	mov r1, #2
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
_0801BF66:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801BF6C: .4byte 0x03000040
_0801BF70: .4byte 0x00004870
_0801BF74: .4byte 0x02011C20
_0801BF78: .4byte 0x00002164
_0801BF7C: .4byte gPackDisplayOrder
	thumb_func_end Campaign_RecordDuelResult

