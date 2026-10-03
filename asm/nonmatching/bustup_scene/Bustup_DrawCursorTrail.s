	thumb_func_start Bustup_DrawCursorTrail
Bustup_DrawCursorTrail: @ 0x08000AC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	mov r5, #5
	ldr r7, _08000BB0 @ =0x02013DE0
	ldr r0, _08000BB4 @ =0x000009A8
	add r0, r0, r7
	mov sl, r0
	ldr r1, _08000BB8 @ =0x000009E8
	add r1, r1, r7
	mov r8, r1
_08000AE4:
	lsl r4, r5, #3
	mov r2, sl
	ldrh r2, [r2]
	add r0, r2, r4
	lsl r1, r0, #1
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r1, r0
	add r0, #0x40
	lsl r0, r0, #1
	ldr r1, _08000BBC @ =0x08087BA4
	add r0, r0, r1
	mov r2, #0
	ldsh r0, [r0, r2]
	mov r1, #0x80
	lsl r1, r1, #3
	mov r9, r1
	bl MulFix8
	ldr r2, _08000BC0 @ =0x000009D4
	add r1, r7, r2
	asr r0, r0, #8
	ldrb r1, [r1]
	add r0, r1, r0
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	mov r1, sl
	ldrh r1, [r1]
	add r0, r1, r4
	lsl r1, r0, #2
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r1, r0
	lsl r0, r0, #1
	ldr r2, _08000BBC @ =0x08087BA4
	add r0, r0, r2
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r1, #0xC0
	lsl r1, r1, #2
	bl MulFix8
	ldr r2, _08000BC4 @ =0x000009D6
	add r1, r7, r2
	asr r0, r0, #8
	ldrb r1, [r1]
	add r0, r1, r0
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r5, #5
	beq _08000BD8
	add r0, r5, #1
	mov r1, #3
	mul r0, r1
	sub r0, #0x1E
	mov r2, r8
	ldrb r2, [r2]
	sub r0, r2, r0
	mov r1, #0x1E
	bl __modsi3
	lsl r0, r0, #2
	add r3, r0, r7
	ldr r0, _08000BC8 @ =0x000009EC
	add r1, r3, r0
	ldrb r0, [r1]
	cmp r0, #0xFF
	beq _08000BD8
	add r2, r0, #0
	ldr r1, _08000BCC @ =0x000009ED
	add r0, r3, r1
	mov r3, #0
	ldrb r0, [r0]
	orr r3, r0
	mov r0, #0x20
	str r0, [sp, #0]
	mov r0, #0x10
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	ldr r0, _08000BD0 @ =0x08080A5C
	add r0, r5, r0
	ldrb r0, [r0]
	str r0, [sp, #0xC]
	mov r0, #0
	str r0, [sp, #0x10]
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	ldr r0, _08000BD4 @ =0x02014888
	str r0, [sp, #0x1C]
	mov r0, #1
	mov r1, #4
	bl OamListAddSprite
	ldr r1, [r0]
	mov r2, r9
	orr r1, r2
	str r1, [r0]
	b _08000C30
	.align 2, 0
_08000BB0: .4byte 0x02013DE0
_08000BB4: .4byte 0x000009A8
_08000BB8: .4byte 0x000009E8
_08000BBC: .4byte gSineTable
_08000BC0: .4byte 0x000009D4
_08000BC4: .4byte 0x000009D6
_08000BC8: .4byte 0x000009EC
_08000BCC: .4byte 0x000009ED
_08000BD0: .4byte gCursorTrailPalettes
_08000BD4: .4byte 0x02014888
_08000BD8:
	add r4, #0
	mov r0, #0x20
	str r0, [sp, #0]
	mov r0, #0x10
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	ldr r1, _08000C4C @ =0x08080A5C
	mov r0, #5
	sub r0, r0, r5
	add r0, r0, r1
	ldrb r0, [r0]
	str r0, [sp, #0xC]
	mov r0, #0
	mov r1, #4
	add r2, r6, #0
	add r3, r4, #0
	bl OamListAddSprite
	mov r1, r8
	ldrb r1, [r1]
	lsl r0, r1, #2
	add r0, r0, r7
	ldr r2, _08000C50 @ =0x000009EC
	add r0, r0, r2
	strb r6, [r0]
	mov r1, r8
	ldrb r1, [r1]
	lsl r0, r1, #2
	add r0, r0, r7
	add r2, #1
	add r0, r0, r2
	strb r4, [r0]
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r1, #0x1E
	bl __umodsi3
	mov r2, r8
	strb r0, [r2]
_08000C30:
	sub r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #0xFF
	beq _08000C3C
	b _08000AE4
_08000C3C:
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08000C4C: .4byte gCursorTrailPalettes
_08000C50: .4byte 0x000009EC
	thumb_func_end Bustup_DrawCursorTrail

