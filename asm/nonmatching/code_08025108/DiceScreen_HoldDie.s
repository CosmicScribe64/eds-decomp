	thumb_func_start DiceScreen_HoldDie
DiceScreen_HoldDie: @ 0x08025DF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	mov r0, #0x80
	lsl r0, r0, #1
	ldr r6, _08025EAC @ =0x08087BA4
	ldr r7, _08025EB0 @ =0x0201F820
	ldr r1, _08025EB4 @ =0x00000ADA
	add r4, r7, r1
	mov r5, #0xFF
	add r1, r5, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	add r1, r1, r6
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
	add r0, r0, r6
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
	ldr r1, _08025EB8 @ =0x00000AEA
	add r0, r7, r1
	ldrb r0, [r0]
	cmp r0, #2
	beq _08025EA0
	ldr r2, _08025EBC @ =0x00000918
	add r4, r7, r2
	mov r0, #1
	strb r0, [r4, #0xE]
	add r0, r4, #0
	bl DiceScreen_TickCharacter
	mov r5, #0
	ldrb r0, [r4, #0xC]
	cmp r5, r0
	bcs _08025EA0
_08025E72:
	lsl r1, r5, #3
	ldr r0, [r4, #4]
	add r0, r0, r1
	mov r1, #0
	str r1, [sp, #0]
	mov r1, #0x80
	lsl r1, r1, #2
	str r1, [sp, #4]
	ldr r1, _08025EB0 @ =0x0201F820
	str r1, [sp, #8]
	mov r1, #1
	mov r2, r9
	add r2, #0x68
	mov r3, r8
	sub r3, #8
	bl DiceScreen_AddOamPiece
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldrb r1, [r4, #0xC]
	cmp r5, r1
	bcc _08025E72
_08025EA0:
	mov r5, #0
	ldr r6, _08025EB0 @ =0x0201F820
	ldr r2, _08025EC0 @ =0x00000AD2
	add r2, r2, r6
	mov sl, r2
	b _08025EEE
_08025EAC: .4byte gSineTable
_08025EB0: .4byte 0x0201F820
_08025EB4: .4byte 0x00000ADA
_08025EB8: .4byte 0x00000AEA
_08025EBC: .4byte 0x00000918
_08025EC0: .4byte 0x00000AD2
_08025EC4:
	lsl r1, r5, #3
	ldr r0, [r0, #4]
	add r0, r0, r1
	ldr r2, _08025F28 @ =0x00000ACD
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
_08025EEE:
	ldr r0, _08025F2C @ =0x00000AC6
	add r7, r6, r0
	ldrb r1, [r7]
	lsl r4, r1, #2
	ldr r2, _08025F30 @ =0x081999EC
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
	bcc _08025EC4
	mov r2, #0xAE
	lsl r2, r2, #4
	add r0, r6, r2
	bl Ease_Tick
	ldrb r0, [r7, #0x1A]
	cmp r0, #2
	beq _08025F34
	mov r0, #0
	b _08025F40
_08025F28: .4byte 0x00000ACD
_08025F2C: .4byte 0x00000AC6
_08025F30: .4byte gDieRollFrames
_08025F34:
	mov r0, #0
	strb r0, [r7, #0x1A]
	mov r0, #0x1F
	bl PlaySE
	mov r0, #1
_08025F40:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DiceScreen_HoldDie

