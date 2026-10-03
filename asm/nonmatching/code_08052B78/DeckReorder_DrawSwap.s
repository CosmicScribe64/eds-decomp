	thumb_func_start DeckReorder_DrawSwap
DeckReorder_DrawSwap: @ 0x08053770
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r9, r0
	str r1, [sp, #0]
	str r2, [sp, #4]
	mov r6, #0
	ldr r0, _08053824 @ =0x02017F84
	mov r8, r0
	ldr r1, _08053828 @ =0x0819D27C
	mov sl, r1
_0805378C:
	lsl r0, r6, #2
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	lsl r1, r6, #5
	add r5, r1, #0
	add r5, #0x24
	mov r1, r9
	lsl r2, r1, #4
	mov r1, #0x60
	sub r4, r1, r2
	bl GetCardIconObjTile
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r0, r9
	cmp r0, #0
	beq _080537BC
	mov r7, #0x40
_080537BC:
	ldr r1, [sp, #0]
	cmp r6, r1
	bne _080537DE
	mov r0, r8
	sub r0, #6
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x18
	lsl r0, r0, #3
	mov r2, sl
	add r1, r0, r2
	ldr r1, [r1]
	add r5, r5, r1
	ldr r1, _0805382C @ =0x0819D280
	add r0, r0, r1
	ldr r0, [r0]
	sub r4, r4, r0
_080537DE:
	ldr r2, [sp, #4]
	cmp r6, r2
	bne _08053800
	mov r0, r8
	sub r0, #6
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x18
	lsl r0, r0, #3
	mov r2, sl
	add r1, r0, r2
	ldr r1, [r1]
	sub r5, r5, r1
	ldr r1, _0805382C @ =0x0819D280
	add r0, r0, r1
	ldr r0, [r0]
	add r4, r4, r0
_08053800:
	lsl r2, r4, #0x10
	orr r2, r5
	ldr r0, _08053830 @ =0x02017F7C
	ldrb r0, [r0]
	cmp r6, r0
	bne _08053840
	ldr r1, _08053834 @ =0x03000040
	ldr r0, _08053838 @ =0x0000485E
	add r1, r1, r0
	mov r0, #0x1E
	ldrh r1, [r1]
	and r0, r1
	ldr r1, _0805383C @ =0x081A4424
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	b _08053844
	.align 2, 0
_08053824: .4byte 0x02017F84
_08053828: .4byte gCardJumpArc
_0805382C: .4byte gUnk_0819D280
_08053830: .4byte 0x02017F7C
_08053834: .4byte 0x03000040
_08053838: .4byte 0x0000485E
_0805383C: .4byte gPulseScaleCurve
_08053840:
	mov r3, #0x80
	lsl r3, r3, #0x11
_08053844:
	add r0, r2, #0
	mov r1, #0x80
	add r2, r7, #0
	bl AddAffineSprite
	add r6, #1
	cmp r6, #4
	ble _0805378C
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DeckReorder_DrawSwap

