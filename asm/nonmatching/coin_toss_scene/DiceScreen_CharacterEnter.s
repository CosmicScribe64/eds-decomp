	thumb_func_start DiceScreen_CharacterEnter
DiceScreen_CharacterEnter: @ 0x08025CB8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	mov r0, #0x80
	lsl r0, r0, #1
	ldr r1, _08025D44 @ =0x08087BA4
	mov r8, r1
	ldr r6, _08025D48 @ =0x0201F820
	ldr r2, _08025D4C @ =0x00000ADA
	add r4, r6, r2
	mov r5, #0xFF
	add r1, r5, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	add r1, r8
	mov r2, #0
	ldsh r1, [r1, r2]
	bl MulFix8
	lsl r0, r0, #0xC
	lsr r0, r0, #0x10
	mov r9, r0
	mov r1, #0
	ldsh r0, [r4, r1]
	lsl r0, r0, #1
	and r0, r5
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r8
	mov r2, #0
	ldsh r1, [r0, r2]
	mov r0, #0x80
	bl MulFix8
	mov r2, #0
	ldsh r1, [r4, r2]
	sub r1, #0xBE
	lsr r2, r1, #0x1F
	add r1, r1, r2
	asr r1, r1, #1
	asr r0, r0, #4
	add r1, r1, r0
	sub r1, #8
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	ldr r0, _08025D50 @ =0x00000918
	add r4, r6, r0
	mov r0, #1
	strb r0, [r4, #0xE]
	add r0, r4, #0
	bl DiceScreen_TickCharacter
	mov r1, r9
	add r1, #0x68
	mov r2, r8
	sub r2, #8
	add r0, r4, #0
	mov r3, #0
	bl DiceScreen_DrawCharacter
	mov r5, #0
	ldr r1, _08025D54 @ =0x00000AD2
	add r1, r1, r6
	mov sl, r1
	b _08025D82
_08025D44: .4byte gSineTable
_08025D48: .4byte 0x0201F820
_08025D4C: .4byte 0x00000ADA
_08025D50: .4byte 0x00000918
_08025D54: .4byte 0x00000AD2
_08025D58:
	lsl r1, r5, #3
	ldr r0, [r0, #4]
	add r0, r0, r1
	ldr r2, _08025DBC @ =0x00000ACD
	add r1, r6, r2
	ldrb r1, [r1]
	str r1, [sp, #0]
	mov r1, #0x80
	lsl r1, r1, #2
	str r1, [sp, #4]
	str r6, [sp, #8]
	mov r1, #0
	mov r2, r9
	add r2, #0x68
	mov r3, r8
	add r3, #0x14
	bl DiceScreen_AddOamPiece
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
_08025D82:
	ldr r0, _08025DC0 @ =0x00000AC6
	add r7, r6, r0
	ldrb r1, [r7]
	lsl r4, r1, #2
	ldr r2, _08025DC4 @ =0x081999EC
	add r4, r4, r2
	mov r1, sl
	mov r2, #0
	ldsh r0, [r1, r2]
	mov r1, #0x14
	bl __modsi3
	lsl r0, r0, #0x10
	ldr r1, [r4]
	asr r0, r0, #0xD
	add r0, r0, r1
	ldrb r1, [r0, #1]
	cmp r5, r1
	bcc _08025D58
	ldr r2, _08025DC8 @ =0x00000AD8
	add r0, r6, r2
	bl Ease_Tick
	ldrb r0, [r7, #0x12]
	cmp r0, #2
	beq _08025DCC
	mov r0, #0
	b _08025DDE
	.align 2, 0
_08025DBC: .4byte 0x00000ACD
_08025DC0: .4byte 0x00000AC6
_08025DC4: .4byte gDieRollFrames
_08025DC8: .4byte 0x00000AD8
_08025DCC:
	mov r0, #0
	strb r0, [r7, #0x12]
	add r3, r7, #0
	add r3, #0x1A
	mov r1, #0xF
	mov r2, #1
	bl Ease_Start
	mov r0, #1
_08025DDE:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DiceScreen_CharacterEnter
	.align 2, 0

