	thumb_func_start ExodiaScene_GatherPieces
ExodiaScene_GatherPieces: @ 0x080268E4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	ldr r4, _08026948 @ =0x02020E28
	add r0, r4, #0
	bl FadeTick
	ldr r0, _0802694C @ =0xFFFFF4E8
	add r6, r4, r0
	sub r2, r4, #2
	ldrb r0, [r2]
	cmp r0, #0x3F
	bhi _08026908
	add r0, #1
	strb r0, [r2]
_08026908:
	ldr r1, _08026950 @ =0x08087BA4
	ldrb r0, [r2]
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r1
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r1, #0x80
	lsl r1, r1, #1
	sub r1, r1, r0
	mov r0, #0x40
	bl MulFix8
	strh r0, [r4, #4]
	ldrb r3, [r4, #6]
	cmp r3, #2
	bne _08026938
	mov r0, #0
	strb r0, [r4, #6]
	add r0, r4, #0
	add r0, #8
	mov r1, #1
	bl Timer_Start
_08026938:
	ldrb r4, [r4, #8]
	cmp r4, #2
	bne _08026954
	mov r0, #0x15
	bl PlaySE
	mov r0, #1
	b _080269DE
_08026948: .4byte 0x02020E28
_0802694C: .4byte 0xFFFFF4E8
_08026950: .4byte gSineTable
_08026954:
	ldr r7, _080269F0 @ =0x08199D74
	mov r5, #0
	mov r0, #0xAB
	lsl r0, r0, #4
	add r0, r0, r6
	mov sl, r0
	mov r9, r6
	mov r6, #0
	mov r1, #1
	mov r8, r1
_08026968:
	lsl r4, r5, #2
	add r4, r4, r5
	lsl r4, r4, #2
	mov r3, sl
	add r0, r4, r3
	bl LineStep
	add r4, r9
	mov r0, #0xAB
	lsl r0, r0, #4
	add r2, r4, r0
	ldrh r1, [r2]
	mov r3, #0x92
	lsl r3, r3, #4
	add r0, r4, r3
	strh r1, [r0]
	ldr r0, _080269F4 @ =0x00000AB2
	add r1, r4, r0
	ldrh r0, [r1]
	add r3, #2
	add r4, r4, r3
	strh r0, [r4]
	add r0, r7, #0
	add r7, #8
	ldrh r3, [r2]
	ldrh r1, [r1]
	str r1, [sp, #0]
	mov r1, r8
	str r1, [sp, #4]
	str r6, [sp, #8]
	str r6, [sp, #0xC]
	str r1, [sp, #0x10]
	str r6, [sp, #0x14]
	str r6, [sp, #0x18]
	mov r1, r9
	str r1, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	bl OamListAddSpriteGroup
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #4
	bls _08026968
	ldr r4, _080269F8 @ =0x02020310
	add r0, r4, #0
	bl OamListFlush
	add r0, r4, #0
	bl OamListClear
	mov r3, #0xB2
	lsl r3, r3, #4
	add r4, r4, r3
	add r0, r4, #0
	bl Timer_Tick
	mov r0, #0
_080269DE:
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080269F0: .4byte gExodiaPieceOamTemplates
_080269F4: .4byte 0x00000AB2
_080269F8: .4byte 0x02020310
	thumb_func_end ExodiaScene_GatherPieces

