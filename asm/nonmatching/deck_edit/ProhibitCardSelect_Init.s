	thumb_func_start ProhibitCardSelect_Init
ProhibitCardSelect_Init: @ 0x0806F0F8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r5, _0806F2F4 @ =0x0201DB20
	ldr r1, _0806F2F8 @ =0x00001C5C
	add r0, r5, #0
	bl MemClear16
	ldr r0, _0806F2FC @ =0x03000040
	ldr r1, _0806F300 @ =0x0000040E
	add r0, r0, r1
	mov r4, #0
	mov r1, #1
	strh r1, [r0]
	ldr r0, _0806F304 @ =0x04000012
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
	ldr r0, _0806F308 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl OamListClear
	ldr r2, _0806F30C @ =0x00000632
	add r0, r5, r2
	strh r4, [r0]
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r5, r3
	strh r4, [r0]
	ldr r1, _0806F310 @ =0x0000063A
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
	mov r8, r2
	mov r3, #0
	mov ip, r3
	mov r6, #0
	mov r0, #0xA5
	lsl r0, r0, #5
	add r7, r5, r0
	ldr r1, _0806F314 @ =0x00001494
	add r5, r5, r1
_0806F194:
	lsl r0, r4, #1
	mov r2, r8
	add r1, r0, r2
	strh r6, [r1]
	add r1, r4, r7
	mov r3, ip
	strb r3, [r1]
	mov r2, #0
	add r1, r4, #1
	add r3, r0, #0
_0806F1A8:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r3, r0
	add r0, r0, r5
	strh r6, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, #1
	bls _0806F1A8
	lsl r0, r1, #0x10
	lsr r4, r0, #0x10
	cmp r4, #2
	bls _0806F194
	ldr r5, _0806F2F4 @ =0x0201DB20
	ldr r1, _0806F318 @ =0x00001C1D
	add r0, r5, r1
	mov r1, #0
	strb r1, [r0]
	ldr r2, _0806F31C @ =0x00001C1C
	add r0, r5, r2
	strb r1, [r0]
	ldr r3, _0806F320 @ =0x00000634
	add r0, r5, r3
	strb r1, [r0]
	ldr r2, _0806F324 @ =0x00000635
	add r0, r5, r2
	strb r1, [r0]
	ldr r3, _0806F328 @ =0x00001710
	add r1, r5, r3
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _0806F32C @ =0x000018AC
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
	ldr r2, _0806F330 @ =0x000018B0
	add r0, r5, r2
	bl ObjAffineInit
	mov r4, #1
	ldr r2, _0806F334 @ =0x08622AB4
	ldrh r1, [r2, #2]
	ldr r0, _0806F338 @ =0x0000FFFF
	cmp r1, r0
	beq _0806F28A
	ldr r7, _0806F33C @ =0x000007FF
	add r6, r2, #0
	ldr r3, _0806F314 @ =0x00001494
	add r3, r3, r5
	mov r9, r3
	mov r0, #0xCD
	lsl r0, r0, #2
	mov r8, r0
_0806F238:
	add r0, r4, #0
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r6
	ldrh r0, [r0]
	ldr r1, _0806F340 @ =0xFFFFF894
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x63
	bls _0806F270
	mov r2, #0xA5
	lsl r2, r2, #5
	add r0, r5, r2
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
	mov r1, #0
	bl DeckEdit_SetListCard
_0806F270:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, r8
	bhi _0806F28A
	add r0, r4, #0
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r6
	ldr r1, _0806F338 @ =0x0000FFFF
	ldrh r0, [r0]
	cmp r0, r1
	bne _0806F238
_0806F28A:
	ldr r7, _0806F2F4 @ =0x0201DB20
	ldr r3, _0806F31C @ =0x00001C1C
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
	ldr r1, _0806F314 @ =0x00001494
	add r5, r7, r1
	add r0, r0, r5
	ldrh r0, [r0]
	mov r3, #0xC4
	lsl r3, r3, #3
	add r1, r7, r3
	add r2, r2, r1
	ldrh r1, [r2]
	ldr r3, _0806F344 @ =0x00001BB0
	add r2, r7, r3
	bl DeckEdit_CalcScrollBar
	ldr r0, _0806F348 @ =0x00001BB4
	add r2, r7, r0
	mov r0, #1
	ldrb r1, [r2]
	orr r1, r0
	strb r1, [r2]
	ldr r2, _0806F34C @ =0x00001BB6
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
	bls _0806F354
	ldr r3, _0806F350 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #1
	b _0806F35A
_0806F2F4: .4byte 0x0201DB20
_0806F2F8: .4byte 0x00001C5C
_0806F2FC: .4byte 0x03000040
_0806F300: .4byte 0x0000040E
_0806F304: .4byte 0x04000012
_0806F308: .4byte 0x0000E0FF
_0806F30C: .4byte 0x00000632
_0806F310: .4byte 0x0000063A
_0806F314: .4byte 0x00001494
_0806F318: .4byte 0x00001C1D
_0806F31C: .4byte 0x00001C1C
_0806F320: .4byte 0x00000634
_0806F324: .4byte 0x00000635
_0806F328: .4byte 0x00001710
_0806F32C: .4byte 0x000018AC
_0806F330: .4byte 0x000018B0
_0806F334: .4byte gCardIdToNumber
_0806F338: .4byte 0x0000FFFF
_0806F33C: .4byte 0x000007FF
_0806F340: .4byte 0xFFFFF894
_0806F344: .4byte 0x00001BB0
_0806F348: .4byte 0x00001BB4
_0806F34C: .4byte 0x00001BB6
_0806F350: .4byte 0x00001BB5
_0806F354:
	ldr r3, _0806F3E0 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #0
_0806F35A:
	strb r1, [r0]
	ldr r2, _0806F3E4 @ =0x00001BB7
	add r0, r7, r2
	strb r1, [r0]
	ldr r4, _0806F3E8 @ =0x0201F6D8
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
	add r3, r4, #0
	add r3, #0xA2
	mov r0, #4
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	mov r1, #2
	orr r0, r1
	strb r0, [r3]
	add r2, #8
	ldr r0, [r2]
	ldr r1, _0806F3EC @ =0xFFFC7FFF
	and r0, r1
	str r0, [r2]
	add r2, #1
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x1D
	neg r0, r0
	ldrb r2, [r3]
	and r0, r2
	strb r0, [r3]
	ldr r0, _0806F3F0 @ =0x081A70FC
	ldr r3, _0806F3F4 @ =0xFFFFFB60
	add r1, r4, r3
	bl AnimBlockInit
	ldr r0, _0806F3F8 @ =0x03000040
	ldr r1, _0806F3FC @ =0x00004872
	add r0, r0, r1
	mov r1, #0
	strh r1, [r0]
	mov r0, #1
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0806F3E0: .4byte 0x00001BB5
_0806F3E4: .4byte 0x00001BB7
_0806F3E8: .4byte 0x0201F6D8
_0806F3EC: .4byte 0xFFFC7FFF
_0806F3F0: .4byte gDeckEditAnimScripts
_0806F3F4: .4byte 0xFFFFFB60
_0806F3F8: .4byte 0x03000040
_0806F3FC: .4byte 0x00004872
	thumb_func_end ProhibitCardSelect_Init

