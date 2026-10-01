	thumb_func_start sub_08040294
sub_08040294: @ 0x08040294
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	add r6, r0, #0
	ldr r0, _080402BC @ =0x02017A40
	ldr r1, _080402C0 @ =0x000003E5
	add r1, r1, r0
	mov r9, r1
	ldrb r4, [r1]
	cmp r4, #1
	beq _08040300
	cmp r4, #1
	bgt _080402C4
	cmp r4, #0
	beq _080402D0
	b _08040464
	.align 2, 0
_080402BC: .4byte 0x02017A40
_080402C0: .4byte 0x000003E5
_080402C4:
	cmp r4, #2
	beq _08040390
	cmp r4, #3
	bne _080402CE
	b _080403E8
_080402CE:
	b _08040464
_080402D0:
	ldr r0, _080402F4 @ =0x00000206
	ldr r1, _080402F8 @ =0x00000712
	ldr r3, _080402FC @ =0x08084694
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r6, #0xA]
	and r0, r2
	strb r0, [r6, #0xA]
_080402E6:
	mov r3, r9
	ldrb r0, [r3]
	add r0, #1
	strb r0, [r3]
_080402EE:
	mov r0, #0
	b _08040466
	.align 2, 0
_080402F4: .4byte 0x00000206
_080402F8: .4byte 0x00000712
_080402FC: .4byte gUnk_08084694
_08040300:
	ldr r0, _08040370 @ =0x000C000C
	bl sub_08052F38
	cmp r0, #0
	beq _080402EE
	ldr r0, _08040374 @ =0x0201CFB0
	ldr r1, _08040378 @ =0x00000824
	add r7, r0, r1
	ldr r2, _0804037C @ =0x00000828
	add r2, r2, r0
	mov r8, r2
	ldr r3, _08040380 @ =0x0000082C
	add r3, r3, r0
	mov sl, r3
	ldr r0, [r2]
	ldr r1, [r3]
	add r0, r0, r1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x10
	ldrb r0, [r7]
	orr r5, r0
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0802C080
	cmp r0, #0
	beq _08040388
	mov r0, #1
	bl sub_08077AEC
	ldrb r1, [r6, #2]
	and r4, r1
	mov r0, #8
	cmp r4, #0
	beq _08040348
	ldr r0, _08040384 @ =0x00008008
_08040348:
	ldrh r1, [r7]
	mov r3, sl
	ldrb r3, [r3]
	lsl r2, r3, #8
	mov r4, r8
	ldrb r4, [r4]
	orr r2, r4
	mov r3, #0
	bl sub_0801EC58
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0803DD7C
	mov r1, r9
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _080402EE
	.align 2, 0
_08040370: .4byte 0x000C000C
_08040374: .4byte 0x0201CFB0
_08040378: .4byte 0x00000824
_0804037C: .4byte 0x00000828
_08040380: .4byte 0x0000082C
_08040384: .4byte 0x00008008
_08040388:
	mov r0, #3
	bl sub_08077AEC
	b _080402EE
_08040390:
	ldr r1, _080403D0 @ =0x080846CC
	ldrh r0, [r6, #0xC]
	mov r3, #1
	and r3, r0
	lsr r0, r0, #8
	mov r2, #0x94
	mul r0, r2
	ldr r2, _080403D4 @ =0x00000D64
	mul r2, r3
	add r0, r0, r2
	ldr r2, _080403D8 @ =0x0201930C
	add r0, r0, r2
	ldr r2, [r0]
	lsl r2, r2, #0x14
	lsr r2, r2, #0xE
	ldr r3, _080403DC @ =0x0822C720
	add r2, r2, r3
	mov r0, sp
	bl sub_080753F4
	ldr r0, _080403E0 @ =0x00000206
	ldr r1, _080403E4 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r4, r9
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _080402EE
	.align 2, 0
_080403D0: .4byte gUnk_080846CC
_080403D4: .4byte 0x00000D64
_080403D8: .4byte 0x0201930C
_080403DC: .4byte gUnk_0822C720
_080403E0: .4byte 0x00000206
_080403E4: .4byte 0x00000712
_080403E8:
	ldr r0, _08040448 @ =0x00E000E0
	bl sub_08052F38
	cmp r0, #0
	bne _080403F4
	b _080402EE
_080403F4:
	ldr r0, _0804044C @ =0x0201CFB0
	ldr r2, _08040450 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	mov r8, r1
	ldr r3, _08040454 @ =0x00000828
	add r1, r0, r3
	ldr r4, _08040458 @ =0x0000082C
	add r0, r0, r4
	ldr r1, [r1]
	ldr r0, [r0]
	add r5, r1, r0
	ldrb r7, [r6, #0xC]
	ldrh r0, [r6, #0xC]
	lsr r4, r0, #8
	add r0, r7, #0
	add r1, r4, #0
	mov r2, r8
	add r3, r5, #0
	bl sub_0800CCCC
	cmp r0, #0
	beq _0804045C
	add r0, r7, #0
	add r1, r4, #0
	bl sub_0800CD68
	mov r2, r8
	lsl r1, r2, #0x18
	lsl r2, r5, #0x18
	lsr r1, r1, #8
	orr r1, r2
	lsr r1, r1, #0x10
	cmp r0, r1
	beq _0804045C
	add r0, r6, #0
	mov r1, r8
	add r2, r5, #0
	bl sub_0803DDAC
	b _080402E6
	.align 2, 0
_08040448: .4byte 0x00E000E0
_0804044C: .4byte 0x0201CFB0
_08040450: .4byte 0x00000824
_08040454: .4byte 0x00000828
_08040458: .4byte 0x0000082C
_0804045C:
	mov r0, #3
	bl sub_08077AEC
	b _080402EE
_08040464:
	mov r0, #1
_08040466:
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08040294
	.align 2, 0

