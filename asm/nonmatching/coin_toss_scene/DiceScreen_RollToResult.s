	thumb_func_start DiceScreen_RollToResult
DiceScreen_RollToResult: @ 0x080259DC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	mov r0, #0x80
	lsl r0, r0, #1
	ldr r6, _08025A3C @ =0x08087BA4
	ldr r7, _08025A40 @ =0x0201F820
	ldr r1, _08025A44 @ =0x00000ADA
	add r4, r7, r1
	mov r5, #0xFF
	add r1, r5, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	add r1, r1, r6
	mov r3, #0
	ldsh r1, [r1, r3]
	bl MulFix8
	lsl r0, r0, #0xC
	lsr r0, r0, #0x10
	str r0, [sp, #0xC]
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
	ldr r0, _08025A48 @ =0x03000040
	mov r2, #1
	ldrh r0, [r0, #6]
	and r2, r0
	cmp r2, #0
	beq _08025A50
	mov r1, #0xC0
	lsl r1, r1, #1
	ldr r0, _08025A4C @ =0x00000ABC
	add r3, r7, r0
	b _08025A64
_08025A3C: .4byte gSineTable
_08025A40: .4byte 0x0201F820
_08025A44: .4byte 0x00000ADA
_08025A48: .4byte 0x03000040
_08025A4C: .4byte 0x00000ABC
_08025A50:
	mov r1, #0xAD
	lsl r1, r1, #4
	add r3, r7, r1
	ldrb r0, [r3]
	cmp r0, #0
	bne _08025A74
	mov r1, #0xC0
	lsl r1, r1, #1
	ldr r2, _08025A70 @ =0x00000ABC
	add r3, r7, r2
_08025A64:
	mov r0, #0
	mov r2, #0
	bl FadeStart
	mov r0, #1
	b _08025C8E
_08025A70: .4byte 0x00000ABC
_08025A74:
	cmp r0, #2
	bne _08025A8C
	ldr r0, _08025AA0 @ =0x00000ACB
	add r1, r7, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	bne _08025A8C
	strb r2, [r3]
_08025A8C:
	mov r7, #0
	ldr r1, _08025AA4 @ =0x081999EC
	mov sl, r1
	ldr r6, _08025AA8 @ =0x0201F820
	ldr r2, _08025AAC @ =0x00000AD2
	add r2, r2, r6
	mov r9, r2
	mov r8, r6
	b _08025AE0
	.align 2, 0
_08025AA0: .4byte 0x00000ACB
_08025AA4: .4byte gDieRollFrames
_08025AA8: .4byte 0x0201F820
_08025AAC: .4byte 0x00000AD2
_08025AB0:
	lsl r1, r7, #3
	ldr r0, [r0, #4]
	add r0, r0, r1
	ldr r3, _08025BA4 @ =0x00000AC8
	add r1, r6, r3
	ldrb r1, [r1]
	sub r3, r5, r1
	lsl r3, r3, #1
	add r3, #0x64
	ldr r2, _08025BA8 @ =0x00000ACD
	add r1, r6, r2
	ldrb r1, [r1]
	str r1, [sp, #0]
	mov r1, #0x80
	lsl r1, r1, #2
	str r1, [sp, #4]
	str r6, [sp, #8]
	mov r1, #0
	mov r2, #0x68
	bl DiceScreen_AddOamPiece
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
_08025AE0:
	ldr r3, _08025BAC @ =0x00000AC6
	add r0, r6, r3
	ldrb r0, [r0]
	lsl r4, r0, #2
	add r4, sl
	mov r0, r9
	mov r1, #0
	ldsh r5, [r0, r1]
	add r0, r5, #0
	mov r1, #0x14
	bl __modsi3
	lsl r0, r0, #0x10
	ldr r1, [r4]
	asr r0, r0, #0xD
	add r0, r0, r1
	ldrb r2, [r0, #1]
	cmp r7, r2
	bcc _08025AB0
	mov r3, #0
	mov sl, r3
	mov r0, #0
	str r0, [sp, #0x14]
	mov r1, r8
	mov r2, #0xAD
	lsl r2, r2, #4
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #2
	bne _08025BD0
	ldr r3, _08025BB0 @ =0x0000092C
	add r3, r1, r3
	str r3, [sp, #0x10]
	ldr r0, _08025BB4 @ =0x00000AE8
	add r3, r1, r0
	ldrh r1, [r3]
	lsl r2, r1, #0x10
	lsr r1, r2, #0x10
	cmp r1, #0
	beq _08025B44
	mov r0, #0x80
	lsl r0, r0, #5
	cmp r1, r0
	bhi _08025B3E
	ldr r1, _08025BB8 @ =0x04000052
	lsr r0, r2, #0x18
	strh r0, [r1]
_08025B3E:
	ldrh r0, [r3]
	sub r0, #0x18
	strh r0, [r3]
_08025B44:
	ldr r2, _08025BBC @ =0x0201F820
	ldr r3, _08025BC0 @ =0x00000AEA
	add r0, r2, r3
	ldrb r0, [r0]
	cmp r0, #1
	bne _08025BF4
	ldr r0, _08025BC4 @ =0x00000AEC
	add r4, r2, r0
	ldr r1, _08025BC8 @ =0x00000AEE
	add r2, r2, r1
	ldrh r0, [r2]
	ldrh r3, [r4]
	add r1, r0, r3
	strh r1, [r4]
	add r0, #8
	strh r0, [r2]
	mov r0, #0x80
	lsl r0, r0, #6
	ldr r5, _08025BCC @ =0x08087BA4
	lsl r1, r1, #0x10
	lsr r1, r1, #0x18
	add r1, #0x40
	lsl r1, r1, #1
	add r1, r1, r5
	mov r2, #0
	ldsh r1, [r1, r2]
	bl MulFix8
	lsl r0, r0, #8
	lsr r0, r0, #0x10
	mov sl, r0
	mov r0, #0xA0
	lsl r0, r0, #5
	ldrh r4, [r4]
	lsr r1, r4, #8
	lsl r1, r1, #1
	add r1, r1, r5
	mov r3, #0
	ldsh r1, [r1, r3]
	bl MulFix8
	asr r0, r0, #8
	sub r0, #0x14
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x14]
	b _08025BF4
	.align 2, 0
_08025BA4: .4byte 0x00000AC8
_08025BA8: .4byte 0x00000ACD
_08025BAC: .4byte 0x00000AC6
_08025BB0: .4byte 0x0000092C
_08025BB4: .4byte 0x00000AE8
_08025BB8: .4byte 0x04000052
_08025BBC: .4byte 0x0201F820
_08025BC0: .4byte 0x00000AEA
_08025BC4: .4byte 0x00000AEC
_08025BC8: .4byte 0x00000AEE
_08025BCC: .4byte gSineTable
_08025BD0:
	cmp r0, #0
	beq _08025BF4
	ldr r0, _08025CA0 @ =0x00000918
	add r0, r8
	str r0, [sp, #0x10]
	ldr r0, _08025CA4 @ =0x00000AD2
	add r0, r8
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r1, #3
	bl __modsi3
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08025BF4
	mov r0, #0x1F
	bl PlaySE
_08025BF4:
	mov r0, #1
	ldr r2, [sp, #0x10]
	strb r0, [r2, #0xE]
	ldr r3, _08025CA8 @ =0x08087BA4
	mov r9, r3
	ldr r5, _08025CAC @ =0x0201F820
	ldr r0, _08025CB0 @ =0x00000ADA
	add r4, r5, r0
	mov r1, #0
	ldsh r0, [r4, r1]
	lsl r0, r0, #1
	mov r2, #0xFF
	mov r8, r2
	and r0, r2
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r9
	mov r3, #0
	ldsh r1, [r0, r3]
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
	lsr r6, r1, #0x10
	mov r3, #0xAD
	lsl r3, r3, #4
	add r7, r5, r3
	ldrb r0, [r7]
	cmp r0, #0
	beq _08025C86
	ldr r0, [sp, #0x10]
	bl DiceScreen_TickCharacter
	mov r4, sl
	add r4, #0x68
	ldr r0, [sp, #0xC]
	add r4, r0, r4
	mov r0, #0x80
	lsl r0, r0, #1
	ldr r2, _08025CB4 @ =0x00000AC5
	add r1, r5, r2
	ldrb r1, [r1]
	lsl r1, r1, #1
	add r1, #0x20
	mov r3, r8
	and r1, r3
	lsl r1, r1, #1
	add r1, r9
	mov r2, #0
	ldsh r1, [r1, r2]
	bl MulFix8
	add r2, r0, #0
	asr r2, r2, #4
	add r2, #8
	sub r2, r6, r2
	ldr r3, [sp, #0x14]
	add r2, r2, r3
	mov r3, #0x80
	lsl r3, r3, #3
	ldr r0, [sp, #0x10]
	add r1, r4, #0
	bl DiceScreen_DrawCharacter
_08025C86:
	add r0, r7, #0
	bl Ease_Tick
	mov r0, #0
_08025C8E:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025CA0: .4byte 0x00000918
_08025CA4: .4byte 0x00000AD2
_08025CA8: .4byte gSineTable
_08025CAC: .4byte 0x0201F820
_08025CB0: .4byte 0x00000ADA
_08025CB4: .4byte 0x00000AC5
	thumb_func_end DiceScreen_RollToResult

