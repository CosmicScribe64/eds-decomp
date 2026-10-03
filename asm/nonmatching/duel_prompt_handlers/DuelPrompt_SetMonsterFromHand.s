	thumb_func_start DuelPrompt_SetMonsterFromHand
DuelPrompt_SetMonsterFromHand: @ 0x08051ED0
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r7, _08051EE8 @ =0x020192E0
	ldr r0, _08051EEC @ =0x00001B62
	add r5, r7, r0
	ldrb r4, [r5]
	cmp r4, #0
	beq _08051EF0
	cmp r4, #1
	beq _08051F10
	mov r0, #1
	b _08052010
_08051EE8: .4byte 0x020192E0
_08051EEC: .4byte 0x00001B62
_08051EF0:
	ldr r0, _08051F04 @ =0x00000206
	ldr r1, _08051F08 @ =0x00000712
	ldr r3, _08051F0C @ =0x08086018
	mov r2, #0xB
	bl TextBoxOpen
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0805200E
_08051F04: .4byte 0x00000206
_08051F08: .4byte 0x00000712
_08051F0C: .4byte gStrPromptSelectMonsterToSet
_08051F10:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0805200E
	and r4, r6
	ldr r0, _08051F74 @ =0x0201CFB0
	ldr r1, _08051F78 @ =0x0000082C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #2
	ldr r1, _08051F7C @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	mov r2, #0xD1
	lsl r2, r2, #3
	add r1, r7, r2
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r6, #0
	add r1, r4, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _08052008
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08052008
	ldr r0, _08051F80 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08051F84 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08051F90
	cmp r0, #0x17
	ble _08051F88
	cmp r0, #0x18
	beq _08051F8C
	b _08051F90
	.align 2, 0
_08051F74: .4byte 0x0201CFB0
_08051F78: .4byte 0x0000082C
_08051F7C: .4byte 0x00000D64
_08051F80: .4byte 0x000007FF
_08051F84: .4byte gCardStats
_08051F88:
	mov r0, #0
	b _08051FA4
_08051F8C:
	mov r0, #0xA
	b _08051FA4
_08051F90:
	ldr r0, _08051FEC @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r2, _08051FF0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08051FA4:
	cmp r0, #4
	bhi _08052008
	mov r5, #0xC4
	cmp r6, #0
	beq _08051FB0
	ldr r5, _08051FF4 @ =0x000080C4
_08051FB0:
	add r0, r6, #0
	bl FindFreeMonsterZone
	ldr r1, _08051FF8 @ =0x0201CFB0
	ldr r2, _08051FFC @ =0x0000082C
	add r1, r1, r2
	ldr r2, [r1]
	mov r3, #0xF
	mov r1, #0xF
	and r2, r1
	lsl r2, r2, #4
	and r0, r3
	orr r2, r0
	mov r1, #0x80
	lsl r1, r1, #2
	add r0, r1, #0
	orr r2, r0
	add r0, r5, #0
	add r1, r4, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _08052000 @ =0x020192E0
	ldr r2, _08052004 @ =0x00001B62
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0805200E
	.align 2, 0
_08051FEC: .4byte 0x000007FF
_08051FF0: .4byte gCardStats
_08051FF4: .4byte 0x000080C4
_08051FF8: .4byte 0x0201CFB0
_08051FFC: .4byte 0x0000082C
_08052000: .4byte 0x020192E0
_08052004: .4byte 0x00001B62
_08052008:
	mov r0, #3
	bl PlaySE
_0805200E:
	mov r0, #0
_08052010:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelPrompt_SetMonsterFromHand
	.align 2, 0

