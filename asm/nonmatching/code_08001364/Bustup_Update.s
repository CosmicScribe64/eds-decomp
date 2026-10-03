	thumb_func_start Bustup_Update
Bustup_Update: @ 0x080017E8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x18
	ldr r7, _0800184C @ =0x020150CC
	add r0, r7, #0
	bl FadeTick
	ldrb r0, [r7, #6]
	cmp r0, #2
	bne _0800180C
	ldr r0, _08001850 @ =0x03000040
	ldr r1, _08001854 @ =0x00004859
	add r0, r0, r1
	ldrb r2, [r0]
	ldrb r3, [r7, #7]
	add r1, r2, r3
	strb r1, [r0]
_0800180C:
	add r6, r7, #0
	add r6, #0x90
	ldrb r3, [r6]
	mov r0, #1
	add r2, r0, #0
	and r2, r3
	cmp r2, #0
	beq _0800181E
	b _08001940
_0800181E:
	ldr r5, _08001850 @ =0x03000040
	mov r1, #1
	mov ip, r1
	ldrh r1, [r5, #6]
	and r0, r1
	cmp r0, #0
	bne _0800182E
	b _0800192E
_0800182E:
	ldr r0, _08001858 @ =0xFFFFF6D9
	add r4, r7, r0
	mov r1, #0
	ldsb r1, [r4, r1]
	mov r0, #2
	neg r0, r0
	cmp r1, r0
	beq _08001884
	cmp r1, r0
	bgt _0800185C
	sub r0, #1
	cmp r1, r0
	beq _08001924
	b _0800192E
	.align 2, 0
_0800184C: .4byte 0x020150CC
_08001850: .4byte 0x03000040
_08001854: .4byte 0x00004859
_08001858: .4byte 0xFFFFF6D9
_0800185C:
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	beq _0800186E
	cmp r1, #0
	bne _0800192E
	mov r1, ip
	strb r1, [r4]
	b _0800192E
_0800186E:
	strb r2, [r4]
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _0800187C
	mov r2, ip
	strb r2, [r4]
_0800187C:
	mov r0, #1
	bl PlaySE
	b _0800192E
_08001884:
	mov r0, #2
	and r0, r3
	cmp r0, #0
	bne _080018B2
	mov r3, #4
	ldsh r0, [r7, r3]
	cmp r0, #0
	bne _0800192E
	mov r1, #0xC0
	lsl r1, r1, #1
	mov r0, #0
	mov r2, #1
	add r3, r7, #0
	bl FadeStart
	mov r0, #1
	bl PlaySE
	mov r0, #1
	ldrb r1, [r6]
	orr r0, r1
	strb r0, [r6]
	b _0800192E
_080018B2:
	ldr r2, _0800190C @ =0x00004859
	add r1, r5, r2
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	add r2, r7, #0
	add r2, #0x80
	ldrh r0, [r2]
	add r0, #1
	mov r1, #0
	strh r0, [r2]
	ldr r3, _08001910 @ =0xFFFFF6E0
	add r0, r7, r3
	strb r1, [r0]
	sub r0, r7, #1
	ldr r1, _08001914 @ =0x00004884
	add r4, r5, r1
	ldrb r0, [r0]
	lsl r1, r0, #4
	ldr r0, _08001918 @ =0xFFFFF00F
	ldrh r3, [r4]
	and r0, r3
	orr r0, r1
	strh r0, [r4]
	ldrh r0, [r2]
	ldr r2, _0800191C @ =0x00004886
	add r1, r5, r2
	strh r0, [r1]
	ldr r6, _08001920 @ =0x08080A64
	ldrh r5, [r1]
	add r0, r5, #0
	bl GetDialogueEventId
	add r2, r0, #0
	ldrh r4, [r4]
	lsl r3, r4, #0x14
	lsr r3, r3, #0x18
	add r0, r6, #0
	add r1, r5, #0
	bl DebugPrintf
	bl DebugPrintFlush
	b _0800192E
	.align 2, 0
_0800190C: .4byte 0x00004859
_08001910: .4byte 0xFFFFF6E0
_08001914: .4byte 0x00004884
_08001918: .4byte 0xFFFFF00F
_0800191C: .4byte 0x00004886
_08001920: .4byte gStrDebugDM5Script
_08001924:
	mov r0, #3
	strb r0, [r4]
	mov r0, #1
	bl PlaySE
_0800192E:
	ldr r1, _0800193C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080019EC
	b _080019C8
_0800193C: .4byte 0x03000040
_08001940:
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _080019EC
	mov r0, #2
	neg r0, r0
	and r0, r3
	strb r0, [r6]
	ldr r5, _080019D4 @ =0x03000040
	ldr r3, _080019D8 @ =0x00004859
	add r1, r5, r3
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	add r4, r7, #0
	add r4, #0x80
	ldrh r0, [r4]
	add r0, #1
	mov r6, #0
	strh r0, [r4]
	ldrh r0, [r4]
	bl GetDialogueSpeaker
	sub r1, r7, #1
	strb r0, [r1]
	ldr r2, _080019DC @ =0xFFFFF6E0
	add r0, r7, r2
	strb r6, [r0]
	ldr r3, _080019E0 @ =0x00004884
	add r7, r5, r3
	ldrb r1, [r1]
	lsl r1, r1, #4
	ldr r0, _080019E4 @ =0xFFFFF00F
	mov r8, r0
	ldrh r2, [r7]
	and r0, r2
	orr r0, r1
	strh r0, [r7]
	ldrh r0, [r4]
	add r3, #2
	add r6, r5, r3
	strh r0, [r6]
	ldr r5, _080019E8 @ =0x08080A84
	ldrh r4, [r6]
	add r0, r4, #0
	bl GetDialogueEventId
	add r2, r0, #0
	ldrh r0, [r7]
	lsl r3, r0, #0x14
	lsr r3, r3, #0x18
	add r0, r5, #0
	add r1, r4, #0
	bl DebugPrintf
	bl DebugPrintFlush
	ldrh r0, [r6]
	bl GetDialogueEventId
	add r1, r0, #0
	cmp r1, #0
	bne _080019EC
	mov r0, r8
	ldrh r2, [r7]
	and r0, r2
	strh r0, [r7]
	strh r1, [r6]
_080019C8:
	mov r0, #2
	bl PlaySE
	mov r0, #1
	b _08001AB2
	.align 2, 0
_080019D4: .4byte 0x03000040
_080019D8: .4byte 0x00004859
_080019DC: .4byte 0xFFFFF6E0
_080019E0: .4byte 0x00004884
_080019E4: .4byte 0xFFFFF00F
_080019E8: .4byte gStrDebugMoveToScript
_080019EC:
	ldr r5, _08001AC0 @ =0x02014EA0
	add r0, r5, #0
	bl Bustup_TickBlink
	ldr r3, _08001AC4 @ =0xFFFFF8E4
	add r1, r5, r3
	ldrb r2, [r1]
	lsl r0, r2, #0x1D
	mov r4, #0
	cmp r0, #0
	beq _08001A22
	add r6, r5, #0
	add r5, r1, #0
_08001A06:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r6
	bl AnimStateTick
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldrb r3, [r5]
	lsl r0, r3, #0x1D
	lsr r0, r0, #0x1D
	cmp r4, r0
	bcc _08001A06
_08001A22:
	ldr r1, _08001AC8 @ =0x02013DE0
	ldr r2, _08001ACC @ =0x000009A4
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	mov r4, #0
	cmp r0, #0
	beq _08001A70
	mov r3, #0x86
	lsl r3, r3, #5
	add r6, r1, r3
	mov r5, #0
	add r7, r1, r2
_08001A3C:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r6
	mov r1, #1
	str r1, [sp, #0]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #0xC]
	str r5, [sp, #0x10]
	ldr r2, _08001AD0 @ =0xFFFFF9E8
	add r1, r6, r2
	str r1, [sp, #0x14]
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl AnimBlockDraw
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldrb r3, [r7]
	lsl r0, r3, #0x1D
	lsr r0, r0, #0x1D
	cmp r4, r0
	bcc _08001A3C
_08001A70:
	ldr r4, _08001AD4 @ =0x02014888
	add r0, r4, #0
	bl OamListFlush
	add r0, r4, #0
	bl OamListClear
	add r5, r4, #0
	sub r5, #0xFC
	add r0, r5, #0
	bl Bustup_ClearHiddenBox
	ldr r1, _08001AD8 @ =0x03000040
	ldr r0, _08001ADC @ =0x0000040C
	add r1, r1, r0
	ldrh r2, [r1]
	ldr r0, _08001AE0 @ =0x0000FFFE
	and r0, r2
	ldrh r2, [r1]
	strh r0, [r1]
	sub r4, #0xDA
	ldrh r1, [r4]
	cmp r1, #1
	bne _08001AAA
	add r0, r5, #0
	bl Bustup_ShowPage
	mov r0, #0
	strh r0, [r4]
_08001AAA:
	add r0, r5, #0
	bl Bustup_UpdateTextBox
	mov r0, #0
_08001AB2:
	add sp, #0x18
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08001AC0: .4byte 0x02014EA0
_08001AC4: .4byte 0xFFFFF8E4
_08001AC8: .4byte 0x02013DE0
_08001ACC: .4byte 0x000009A4
_08001AD0: .4byte 0xFFFFF9E8
_08001AD4: .4byte 0x02014888
_08001AD8: .4byte 0x03000040
_08001ADC: .4byte 0x0000040C
_08001AE0: .4byte 0x0000FFFE
	thumb_func_end Bustup_Update

