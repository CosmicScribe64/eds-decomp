	thumb_func_start TurnOrder_RpsMain
TurnOrder_RpsMain: @ 0x080297B4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x18
	ldr r4, _08029820 @ =0x02020E08
	add r0, r4, #0
	bl FadeTick
	ldrb r0, [r4, #6]
	cmp r0, #2
	bne _080297D8
	ldr r0, _08029824 @ =0x03000040
	ldr r1, _08029828 @ =0x00004859
	add r0, r0, r1
	ldrb r2, [r0]
	ldrb r3, [r4, #7]
	add r1, r2, r3
	strb r1, [r0]
_080297D8:
	ldrb r5, [r4, #6]
	cmp r5, #3
	bne _080297F4
	ldr r1, _0802982C @ =0x04000050
	mov r2, #0x88
	lsl r2, r2, #3
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0
	strb r0, [r4, #6]
	add r1, r4, #0
	sub r1, #0x23
	mov r0, #6
	strb r0, [r1]
_080297F4:
	ldrb r0, [r4, #6]
	cmp r0, #0
	bne _08029834
	add r0, r4, #0
	sub r0, #0x23
	ldrb r0, [r0]
	cmp r0, #0
	bne _08029842
	ldr r2, _08029830 @ =0x0819A6B0
	sub r0, r4, #3
	ldrb r3, [r0]
	lsl r1, r3, #2
	add r1, r1, r2
	ldr r1, [r1]
	bl _call_via_r1
	lsl r0, r0, #0x18
	cmp r0, #0
	beq _08029834
	mov r0, #1
	b _080299CA
	.align 2, 0
_08029820: .4byte 0x02020E08
_08029824: .4byte 0x03000040
_08029828: .4byte 0x00004859
_0802982C: .4byte 0x04000050
_08029830: .4byte gTurnOrderRpsSubsteps
_08029834:
	ldr r0, _080298DC @ =0x02020310
	ldr r4, _080298E0 @ =0x00000AD5
	add r1, r0, r4
	ldrb r1, [r1]
	add r6, r0, #0
	cmp r1, #0
	beq _08029866
_08029842:
	ldr r1, _080298DC @ =0x02020310
	ldr r5, _080298E4 @ =0x00000AD4
	add r2, r1, r5
	ldr r0, _080298E0 @ =0x00000AD5
	add r3, r1, r0
	ldrb r4, [r2]
	ldrb r5, [r3]
	add r0, r4, r5
	strb r0, [r2]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r6, r1, #0
	cmp r0, #0x54
	bls _08029866
	mov r0, #0x55
	strb r0, [r2]
	mov r0, #0
	strb r0, [r3]
_08029866:
	ldr r1, _080298E8 @ =0x00000AF5
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #2
	bhi _0802994C
	ldr r2, _080298EC @ =0x00000ABC
	add r2, r2, r6
	mov r8, r2
	ldrb r0, [r2]
	mov r3, #0xAB
	lsl r3, r3, #4
	add r5, r6, r3
	ldrb r1, [r5]
	mov r4, #0xAC
	lsl r4, r4, #4
	add r7, r6, r4
	ldrh r2, [r7]
	bl TurnOrder_DrawOpponentCard
	ldr r0, _080298F0 @ =0x080826E0
	ldr r1, _080298F4 @ =0x08082703
	ldr r3, _080298F8 @ =0x00000AAC
	add r2, r6, r3
	ldrb r2, [r2]
	sub r4, #0x12
	add r3, r6, r4
	ldrb r3, [r3]
	ldrb r4, [r5]
	str r4, [sp, #0]
	ldrh r4, [r7]
	str r4, [sp, #4]
	ldr r4, _080298E4 @ =0x00000AD4
	ldrb r4, [r4, r6]
	str r4, [sp, #8]
	bl TurnOrder_DrawHandCarousel
	ldrb r5, [r5]
	cmp r5, #0x30
	bne _0802991E
	ldr r5, _080298FC @ =0x00000ABD
	add r0, r6, r5
	ldrb r0, [r0]
	cmp r0, #1
	bne _0802991E
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #0xFF
	beq _0802991E
	ldr r2, _08029900 @ =0x00000ABE
	add r0, r6, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _08029904
	ldrh r1, [r7]
	mov r0, #1
	bl TurnOrder_DrawBanner
	b _0802991E
	.align 2, 0
_080298DC: .4byte 0x02020310
_080298E0: .4byte 0x00000AD5
_080298E4: .4byte 0x00000AD4
_080298E8: .4byte 0x00000AF5
_080298EC: .4byte 0x00000ABC
_080298F0: .4byte gHandCardTileNums
_080298F4: .4byte gHandCardPalNums
_080298F8: .4byte 0x00000AAC
_080298FC: .4byte 0x00000ABD
_08029900: .4byte 0x00000ABE
_08029904:
	cmp r0, #1
	bne _08029912
	ldrh r1, [r7]
	mov r0, #2
	bl TurnOrder_DrawBanner
	b _0802991E
_08029912:
	cmp r0, #2
	bne _0802991E
	ldrh r1, [r7]
	mov r0, #4
	bl TurnOrder_DrawBanner
_0802991E:
	ldr r6, _080299D8 @ =0x02020310
	ldr r3, _080299DC @ =0x00000ABF
	add r5, r6, r3
	ldrb r0, [r5]
	cmp r0, #0xFF
	beq _0802994C
	ldr r1, _080299E0 @ =0x00000AC4
	add r4, r6, r1
	add r1, r4, #0
	bl TurnOrder_UpdateChoiceBob
	ldrb r0, [r5]
	ldr r2, _080299E4 @ =0x00000AF4
	add r1, r6, r2
	ldrb r1, [r1]
	mov r3, #0xAC
	lsl r3, r3, #4
	add r2, r6, r3
	ldrh r2, [r2]
	str r0, [sp, #0]
	add r3, r4, #0
	bl TurnOrder_DrawTurnChoice
_0802994C:
	mov r4, #0
	ldr r5, _080299E8 @ =0x02020928
_08029950:
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #3
	add r0, r0, r5
	bl ObjAffineApply
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #4
	bls _08029950
	ldr r6, _080299D8 @ =0x02020310
	ldr r4, _080299EC @ =0x00000926
	add r0, r6, r4
	mov r5, #0
	ldsb r5, [r0, r5]
	cmp r5, #1
	bne _08029998
	ldr r0, _080299F0 @ =0x00000918
	add r4, r6, r0
	add r0, r4, #0
	bl AnimStateTick
	str r5, [sp, #0]
	mov r0, #0
	str r0, [sp, #4]
	str r0, [sp, #8]
	str r0, [sp, #0xC]
	str r0, [sp, #0x10]
	str r6, [sp, #0x14]
	add r0, r4, #0
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl AnimBlockDraw
_08029998:
	add r0, r6, #0
	bl OamListFlush
	add r0, r6, #0
	bl OamListClear
	ldr r1, _080299F4 @ =0x00000AAC
	add r4, r6, r1
	add r0, r4, #0
	bl Scroller_Move
	add r0, r4, #0
	bl Scroller_SnapToStop
	mov r2, #0xAB
	lsl r2, r2, #4
	add r0, r6, r2
	bl Scroller_StopAtEnds
	ldr r3, _080299E4 @ =0x00000AF4
	add r1, r6, r3
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	mov r0, #0
_080299CA:
	add sp, #0x18
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080299D8: .4byte 0x02020310
_080299DC: .4byte 0x00000ABF
_080299E0: .4byte 0x00000AC4
_080299E4: .4byte 0x00000AF4
_080299E8: .4byte 0x02020928
_080299EC: .4byte 0x00000926
_080299F0: .4byte 0x00000918
_080299F4: .4byte 0x00000AAC
	thumb_func_end TurnOrder_RpsMain

