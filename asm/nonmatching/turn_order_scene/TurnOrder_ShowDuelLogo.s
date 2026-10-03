	thumb_func_start TurnOrder_ShowDuelLogo
TurnOrder_ShowDuelLogo: @ 0x080294E8
	push {r4, r5, lr}
	sub sp, #0x10
	ldr r2, _08029520 @ =0x02020310
	ldr r0, _08029524 @ =0x00000ACF
	add r1, r2, r0
	ldrb r0, [r1]
	cmp r0, #0xF
	bhi _08029500
	add r0, #1
	strb r0, [r1]
	cmp r0, #0xF
	bls _08029534
_08029500:
	ldr r1, _08029528 @ =0x00000ACE
	add r4, r2, r1
	ldrb r2, [r4]
	lsl r0, r2, #0x18
	cmp r0, #0
	bge _0802952C
	lsr r0, r0, #0x18
	cmp r0, #0xF4
	bne _08029518
	mov r0, #0x2A
	bl PlaySE
_08029518:
	ldrb r0, [r4]
	add r0, #3
	strb r0, [r4]
	b _08029534
_08029520: .4byte 0x02020310
_08029524: .4byte 0x00000ACF
_08029528: .4byte 0x00000ACE
_0802952C:
	mov r0, #0
	strb r0, [r4]
	bl FadeOutBGM
_08029534:
	ldr r4, _08029630 @ =0x02020310
	mov r0, #0xB2
	lsl r0, r0, #4
	add r2, r4, r0
	ldrh r0, [r2]
	add r1, r0, #1
	strh r1, [r2]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl __floatsidf
	ldr r2, _08029634 @ =0x40568000
	ldr r3, _08029638 @ =0x00000000
	bl __eqdf2
	cmp r0, #0
	beq _08029562
	ldr r1, _0802963C @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08029596
_08029562:
	mov r1, #0xAD
	lsl r1, r1, #4
	add r0, r4, r1
	ldrh r0, [r0]
	bl SetBldY
	ldr r1, _08029640 @ =0x04000050
	mov r0, #0xBF
	strh r0, [r1]
	ldr r2, _08029644 @ =0x00000AD2
	add r1, r4, r2
	mov r0, #0xC0
	lsl r0, r0, #2
	strh r0, [r1]
	ldr r0, _08029648 @ =0x00000AF5
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	mov r1, #0xB0
	lsl r1, r1, #4
	add r0, r4, r1
	bl Timer_Reset
	bl FadeOutBGM
_08029596:
	ldr r0, _08029630 @ =0x02020310
	ldr r2, _0802964C @ =0x00000ACF
	add r1, r0, r2
	add r3, r0, #0
	ldrb r1, [r1]
	cmp r1, #0xF
	bne _080295B8
	ldr r4, _08029650 @ =0x00000ADE
	add r0, r3, r4
	mov r1, #0
	ldsh r0, [r0, r1]
	cmp r0, #0
	bne _080295B8
	add r2, #0xD
	add r1, r3, r2
	mov r0, #0xC0
	strh r0, [r1]
_080295B8:
	ldr r4, _08029654 @ =0x00000ACE
	add r0, r3, r4
	mov r1, #0
	ldsb r1, [r0, r1]
	mov r0, #0xA
	neg r0, r0
	cmp r1, r0
	ble _080295F2
	ldr r1, _08029650 @ =0x00000ADE
	add r0, r3, r1
	mov r2, #0
	ldsh r1, [r0, r2]
	cmp r1, #0
	bne _080295F2
	mov r2, #0x80
	lsl r2, r2, #1
	mov r0, #6
	str r0, [sp, #0]
	mov r0, #0x14
	str r0, [sp, #4]
	add r4, #0xE
	add r0, r3, r4
	str r0, [sp, #8]
	str r1, [sp, #0xC]
	mov r0, #0xC0
	mov r1, #0
	mov r3, #0x38
	bl TweenInit
_080295F2:
	ldr r4, _08029658 @ =0x02020DEC
	add r0, r4, #0
	bl TweenUpdate
	add r5, r4, #0
	sub r5, #0x1D
	ldrb r0, [r5]
	ldrb r1, [r4, #0x18]
	add r2, r4, #0
	sub r2, #0x1C
	ldrh r2, [r2]
	add r3, r4, #0
	sub r3, #0x18
	str r0, [sp, #0]
	str r4, [sp, #4]
	bl TurnOrder_DrawChosenTurnBanner
	ldrb r0, [r5]
	add r1, r4, #0
	sub r1, #0xE
	ldrb r1, [r1]
	add r2, r4, #0
	sub r2, #0xD
	ldrb r2, [r2]
	bl TurnOrder_DrawDuelLogo
	mov r0, #0
	add sp, #0x10
	pop {r4, r5}
	pop {r1}
	bx r1
_08029630: .4byte 0x02020310
_08029634: .4byte 0x40568000
_08029638: .4byte 0x00000000
_0802963C: .4byte 0x03000040
_08029640: .4byte 0x04000050
_08029644: .4byte 0x00000AD2
_08029648: .4byte 0x00000AF5
_0802964C: .4byte 0x00000ACF
_08029650: .4byte 0x00000ADE
_08029654: .4byte 0x00000ACE
_08029658: .4byte 0x02020DEC
	thumb_func_end TurnOrder_ShowDuelLogo

