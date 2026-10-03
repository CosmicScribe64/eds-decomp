	thumb_func_start SideDeckSwap_Init
SideDeckSwap_Init: @ 0x0806A9AC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r5, _0806ABF8 @ =0x0201DB20
	ldr r1, _0806ABFC @ =0x00001C5C
	add r0, r5, #0
	bl MemClear16
	ldr r0, _0806AC00 @ =0x03000040
	ldr r1, _0806AC04 @ =0x0000040E
	add r0, r0, r1
	mov r4, #0
	mov r1, #1
	strh r1, [r0]
	bl ResetBgScroll
	ldr r0, _0806AC08 @ =0x04000012
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #0xC
	strh r4, [r0]
	add r0, #2
	strh r4, [r0]
	add r0, #0x12
	strh r4, [r0]
	add r0, #2
	strh r4, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0806AC0C @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl OamListClear
	ldr r2, _0806AC10 @ =0x00000632
	add r0, r5, r2
	strh r4, [r0]
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r5, r3
	strh r4, [r0]
	ldr r1, _0806AC14 @ =0x0000063A
	add r0, r5, r1
	strh r4, [r0]
	add r2, #6
	add r0, r5, r2
	strh r4, [r0]
	add r3, #0xE
	add r0, r5, r3
	strh r4, [r0]
	add r1, #2
	add r0, r5, r1
	strh r4, [r0]
	sub r2, #0x18
	add r2, r2, r5
	mov r9, r2
	mov r3, #0
	mov r8, r3
	mov r6, #0
	mov r0, #0xA5
	lsl r0, r0, #5
	add r0, r0, r5
	mov ip, r0
	ldr r1, _0806AC18 @ =0x00001494
	add r7, r5, r1
_0806AA4E:
	lsl r0, r4, #1
	mov r2, r9
	add r1, r0, r2
	strh r6, [r1]
	mov r3, ip
	add r1, r4, r3
	mov r2, r8
	strb r2, [r1]
	mov r2, #0
	add r1, r4, #1
	add r3, r0, #0
_0806AA64:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r3, r0
	add r0, r0, r7
	strh r6, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, #1
	bls _0806AA64
	lsl r0, r1, #0x10
	lsr r4, r0, #0x10
	cmp r4, #2
	bls _0806AA4E
	ldr r3, _0806AC1C @ =0x00001C1C
	add r0, r5, r3
	mov r2, #0
	mov r1, #2
	strb r1, [r0]
	add r3, #1
	add r0, r5, r3
	strb r1, [r0]
	ldr r1, _0806AC20 @ =0x00000634
	add r0, r5, r1
	strb r2, [r0]
	ldr r3, _0806AC24 @ =0x00000635
	add r0, r5, r3
	strb r2, [r0]
	ldr r0, _0806AC28 @ =0x00001710
	add r1, r5, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _0806AC2C @ =0x000018AC
	add r1, r5, r3
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r0, #0xC5
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl Ease_Init
	mov r1, #0xC8
	lsl r1, r1, #3
	add r0, r5, r1
	bl ClearKatakanaFlag
	ldr r2, _0806AC30 @ =0x000018B0
	add r0, r5, r2
	bl ObjAffineInit
	mov r4, #1
	ldr r0, _0806AC34 @ =0x000007FF
	add r3, r0, #0
	ldr r1, _0806AC38 @ =0x08622AB4
	ldrh r0, [r1, #2]
	ldr r2, _0806AC3C @ =0x0000FFFF
	cmp r0, r2
	beq _0806AB88
	ldr r0, _0806AC40 @ =0x00001496
	add r0, r0, r5
	mov r9, r0
	ldr r0, _0806AC44 @ =0x00001498
	add r0, r0, r5
	mov r8, r0
	add r7, r2, #0
_0806AAF6:
	add r0, r4, #0
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _0806AC48 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0806AB66
	ldr r1, _0806AC4C @ =0x02011C20
	lsl r0, r4, #2
	add r6, r0, r1
	ldrb r2, [r6, #9]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _0806AB3C
	ldr r3, _0806AC50 @ =0x000014A1
	add r0, r5, r3
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r9
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r4, #0
	mov r1, #1
	bl DeckEdit_SetListCard
_0806AB3C:
	ldrb r6, [r6, #9]
	lsl r0, r6, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _0806AB66
	ldr r1, _0806AC54 @ =0x000014A2
	add r0, r5, r1
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r8
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r4, #0
	mov r1, #2
	bl DeckEdit_SetListCard
_0806AB66:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r4, r0
	bhi _0806AB88
	ldr r2, _0806AC34 @ =0x000007FF
	add r3, r2, #0
	add r0, r4, #0
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0806AC38 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _0806AAF6
_0806AB88:
	bl DeckEdit_CountSideDeckMonsters
	ldr r7, _0806ABF8 @ =0x0201DB20
	ldr r3, _0806AC1C @ =0x00001C1C
	add r6, r7, r3
	ldrb r1, [r6]
	lsl r2, r1, #1
	mov r0, #0xA5
	lsl r0, r0, #5
	add r4, r7, r0
	add r1, r1, r4
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r1, _0806AC18 @ =0x00001494
	add r5, r7, r1
	add r0, r0, r5
	ldrh r0, [r0]
	mov r3, #0xC4
	lsl r3, r3, #3
	add r1, r7, r3
	add r2, r2, r1
	ldrh r1, [r2]
	ldr r3, _0806AC58 @ =0x00001BB0
	add r2, r7, r3
	bl DeckEdit_CalcScrollBar
	ldr r0, _0806AC5C @ =0x00001BB4
	add r2, r7, r0
	mov r0, #1
	ldrb r1, [r2]
	orr r1, r0
	strb r1, [r2]
	ldr r2, _0806AC60 @ =0x00001BB6
	add r1, r7, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldrb r1, [r6]
	add r4, r1, r4
	ldrb r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r2
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, #5
	bls _0806AC68
	ldr r3, _0806AC64 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #1
	b _0806AC6E
	.align 2, 0
_0806ABF8: .4byte 0x0201DB20
_0806ABFC: .4byte 0x00001C5C
_0806AC00: .4byte 0x03000040
_0806AC04: .4byte 0x0000040E
_0806AC08: .4byte 0x04000012
_0806AC0C: .4byte 0x0000E0FF
_0806AC10: .4byte 0x00000632
_0806AC14: .4byte 0x0000063A
_0806AC18: .4byte 0x00001494
_0806AC1C: .4byte 0x00001C1C
_0806AC20: .4byte 0x00000634
_0806AC24: .4byte 0x00000635
_0806AC28: .4byte 0x00001710
_0806AC2C: .4byte 0x000018AC
_0806AC30: .4byte 0x000018B0
_0806AC34: .4byte 0x000007FF
_0806AC38: .4byte gCardIdToNumber
_0806AC3C: .4byte 0x0000FFFF
_0806AC40: .4byte 0x00001496
_0806AC44: .4byte 0x00001498
_0806AC48: .4byte 0xFFFFF880
_0806AC4C: .4byte 0x02011C20
_0806AC50: .4byte 0x000014A1
_0806AC54: .4byte 0x000014A2
_0806AC58: .4byte 0x00001BB0
_0806AC5C: .4byte 0x00001BB4
_0806AC60: .4byte 0x00001BB6
_0806AC64: .4byte 0x00001BB5
_0806AC68:
	ldr r3, _0806ACF8 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #0
_0806AC6E:
	strb r1, [r0]
	ldr r2, _0806ACFC @ =0x00001BB7
	add r0, r7, r2
	strb r1, [r0]
	ldr r4, _0806AD00 @ =0x0201F6D8
	add r0, r4, #0
	bl DeckEdit_ResetFrameSlots
	add r0, r4, #0
	add r0, #0x68
	bl DeckEdit_ResetCardMove
	add r2, r4, #0
	add r2, #0x7C
	mov r0, #2
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	add r5, r4, #0
	add r5, #0xA2
	mov r0, #4
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	mov r3, #1
	orr r0, r3
	strb r0, [r5]
	add r2, #8
	ldr r0, [r2]
	ldr r1, _0806AD04 @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #9
	orr r0, r1
	str r0, [r2]
	add r6, r4, #0
	add r6, #0x85
	mov r2, #8
	neg r2, r2
	add r0, r2, #0
	ldrb r1, [r6]
	and r0, r1
	mov r1, #3
	orr r0, r1
	and r0, r2
	orr r0, r3
	strb r0, [r6]
	mov r0, #0x1D
	neg r0, r0
	ldrb r2, [r5]
	and r0, r2
	strb r0, [r5]
	ldr r0, _0806AD08 @ =0x081A70FC
	ldr r3, _0806AD0C @ =0xFFFFFB60
	add r1, r4, r3
	bl AnimBlockInit
	mov r0, #1
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0806ACF8: .4byte 0x00001BB5
_0806ACFC: .4byte 0x00001BB7
_0806AD00: .4byte 0x0201F6D8
_0806AD04: .4byte 0xFFFC7FFF
_0806AD08: .4byte gDeckEditAnimScripts
_0806AD0C: .4byte 0xFFFFFB60
	thumb_func_end SideDeckSwap_Init

