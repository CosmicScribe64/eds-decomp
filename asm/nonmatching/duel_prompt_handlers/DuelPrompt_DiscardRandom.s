	thumb_func_start DuelPrompt_DiscardRandom
DuelPrompt_DiscardRandom: @ 0x08051A9C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	mov r8, r2
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	ldr r2, _08051AEC @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08051AF0 @ =0x00000D64
	mul r0, r1
	add r7, r0, r2
	ldrb r0, [r7, #2]
	cmp r0, #0
	beq _08051BAC
	ldr r0, _08051AF4 @ =0x00001B5E
	add r5, r2, r0
	ldrb r4, [r5]
	cmp r4, #0
	bne _08051B04
	add r0, r6, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	ldr r0, _08051AF8 @ =0x02017A40
	ldr r2, _08051AFC @ =0x000004FC
	add r1, r0, r2
	strb r4, [r1]
	ldr r1, _08051B00 @ =0x000004FD
	add r0, r0, r1
	mov r2, r8
	strb r2, [r0]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _08051BAE
_08051AEC: .4byte 0x020192E4
_08051AF0: .4byte 0x00000D64
_08051AF4: .4byte 0x00001B5E
_08051AF8: .4byte 0x02017A40
_08051AFC: .4byte 0x000004FC
_08051B00: .4byte 0x000004FD
_08051B04:
	ldr r1, _08051B4C @ =0x02017A40
	ldr r0, _08051B50 @ =0x000004FD
	add r5, r1, r0
	ldrb r0, [r5]
	cmp r0, #0
	beq _08051BAC
	ldr r2, _08051B54 @ =0x000004FC
	add r2, r2, r1
	mov r8, r2
	ldrb r0, [r2]
	cmp r0, #9
	bhi _08051B60
	ldr r1, _08051B58 @ =0x0201CFB0
	ldr r2, _08051B5C @ =0x00000808
	add r1, r1, r2
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	bl Random
	ldrb r1, [r7, #2]
	bl __modsi3
	add r2, r0, #0
	add r0, r6, #0
	mov r1, #0xB
	bl DuelCursor_Select
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	mov r0, #0
	b _08051BAE
	.align 2, 0
_08051B4C: .4byte 0x02017A40
_08051B50: .4byte 0x000004FD
_08051B54: .4byte 0x000004FC
_08051B58: .4byte 0x0201CFB0
_08051B5C: .4byte 0x00000808
_08051B60:
	mov r3, #8
	cmp r6, #0
	beq _08051B68
	ldr r3, _08051BA0 @ =0x00008008
_08051B68:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	ldr r4, _08051BA4 @ =0x0201CFB0
	ldr r2, _08051BA8 @ =0x0000082C
	add r4, r4, r2
	ldrb r0, [r4]
	lsl r2, r0, #8
	mov r0, #0xB
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, [r4]
	add r0, r6, #0
	mov r2, r9
	mov r3, #1
	bl DiscardHandCard
	ldrb r0, [r5]
	sub r0, #1
	mov r1, #0
	strb r0, [r5]
	mov r2, r8
	strb r1, [r2]
	mov r0, #0
	b _08051BAE
	.align 2, 0
_08051BA0: .4byte 0x00008008
_08051BA4: .4byte 0x0201CFB0
_08051BA8: .4byte 0x0000082C
_08051BAC:
	mov r0, #1
_08051BAE:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelPrompt_DiscardRandom
	.align 2, 0

