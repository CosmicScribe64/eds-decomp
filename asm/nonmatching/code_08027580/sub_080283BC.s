	thumb_func_start sub_080283BC
sub_080283BC: @ 0x080283BC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	add r6, r3, #0
	ldr r0, [sp, #0x48]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0x24]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	mov r0, #0x78
	mov r9, r0
	mov r1, #0x30
	mov sl, r1
	mov r3, #0
	mov r5, #0
_080283E8:
	lsl r0, r3, #2
	add r1, r0, r6
	ldrh r2, [r1]
	mov r7, #0
	ldsh r0, [r1, r7]
	cmp r0, #0x7F
	ble _080283FE
	add r0, r2, #0
	sub r0, #0x80
	strh r0, [r1]
	b _08028400
_080283FE:
	strh r5, [r1]
_08028400:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #1
	bls _080283E8
	cmp r4, #0
	beq _0802841C
	cmp r4, #1
	bne _08028414
	b _0802853C
_08028414:
	ldr r2, _08028418 @ =0x02020310
	b _08028656
_08028418: .4byte 0x02020310
_0802841C:
	ldr r5, _080284A4 @ =0x08087BA4
	ldr r1, [sp, #0x4C]
	mov r2, #0
	ldsh r0, [r1, r2]
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r5
	mov r3, #0
	ldsh r1, [r0, r3]
	mov r0, #0x80
	lsl r0, r0, #1
	sub r0, r0, r1
	mov r1, #0xC0
	lsl r1, r1, #6
	bl sub_0807B4D0
	asr r0, r0, #8
	sub r0, #0x50
	mov r1, r9
	add r7, r1, r0
	ldr r2, [sp, #0x24]
	lsl r1, r2, #1
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r1, r0
	lsl r0, r0, #1
	add r0, r0, r5
	mov r3, #0
	ldsh r0, [r0, r3]
	mov r2, #0
	ldsh r1, [r6, r2]
	bl sub_0807B4D0
	add r3, r0, #0
	asr r3, r3, #8
	add r3, sl
	ldr r0, _080284A8 @ =0x080826E6
	ldrh r1, [r0]
	mov r0, #0x40
	str r0, [sp, #0]
	mov r2, #0x20
	str r2, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #3
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1C]
	ldr r0, _080284AC @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	add r2, r7, #0
	bl sub_0807B6B8
	add r1, r0, #0
	ldr r2, [r1]
	mov r0, #8
	mov r3, r8
	and r0, r3
	cmp r0, #0
	beq _080284B4
	ldr r0, _080284B0 @ =0x08000400
	b _080284B8
_080284A4: .4byte gUnk_08087BA4
_080284A8: .4byte gUnk_080826E6
_080284AC: .4byte 0x02020310
_080284B0: .4byte 0x08000400
_080284B4:
	mov r0, #0x80
	lsl r0, r0, #0x14
_080284B8:
	orr r2, r0
	str r2, [r1]
	ldr r0, _0802850C @ =0x080826E6
	ldrh r1, [r0, #2]
	mov r2, r9
	sub r2, #0x10
	ldr r4, [sp, #0x4C]
	mov r7, #2
	ldsh r0, [r4, r7]
	add r3, r0, #0
	mul r3, r0
	asr r3, r3, #1
	add r3, sl
	mov r0, #0x40
	str r0, [sp, #0]
	mov r4, #0x20
	str r4, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #9
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08028510 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	bl sub_0807B6B8
	add r1, r0, #0
	ldr r2, [r1]
	mov r0, #8
	mov r7, r8
	and r0, r7
	cmp r0, #0
	beq _08028518
	ldr r0, _08028514 @ =0x08000700
	b _0802851A
_0802850C: .4byte gUnk_080826E6
_08028510: .4byte 0x02020310
_08028514: .4byte 0x08000700
_08028518:
	ldr r0, _08028530 @ =0x08000300
_0802851A:
	orr r2, r0
	str r2, [r1]
	ldr r2, _08028534 @ =0x02020310
	ldr r0, [sp, #0x4C]
	ldrh r0, [r0, #2]
	lsl r1, r0, #9
	ldr r3, _08028538 @ =0x0000067C
	add r0, r2, r3
	strh r1, [r0]
	b _08028656
	.align 2, 0
_08028530: .4byte 0x08000300
_08028534: .4byte 0x02020310
_08028538: .4byte 0x0000067C
_0802853C:
	ldr r0, _0802858C @ =0x080826E6
	ldrh r1, [r0]
	mov r2, r9
	sub r2, #0x70
	ldr r4, [sp, #0x4C]
	mov r7, #2
	ldsh r0, [r4, r7]
	add r3, r0, #0
	mul r3, r0
	asr r3, r3, #1
	add r3, sl
	mov r0, #0x40
	str r0, [sp, #0]
	mov r4, #0x20
	str r4, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #9
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08028590 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	bl sub_0807B6B8
	add r1, r0, #0
	ldr r2, [r1]
	mov r0, #8
	mov r7, r8
	and r0, r7
	cmp r0, #0
	beq _08028598
	ldr r0, _08028594 @ =0x08000700
	b _0802859A
_0802858C: .4byte gUnk_080826E6
_08028590: .4byte 0x02020310
_08028594: .4byte 0x08000700
_08028598:
	ldr r0, _08028628 @ =0x08000300
_0802859A:
	orr r2, r0
	str r2, [r1]
	ldr r4, _0802862C @ =0x08087BA4
	ldr r1, [sp, #0x4C]
	mov r2, #0
	ldsh r0, [r1, r2]
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r4
	mov r3, #0
	ldsh r1, [r0, r3]
	mov r0, #0x80
	lsl r0, r0, #1
	sub r0, r0, r1
	mov r1, #0xC0
	lsl r1, r1, #6
	bl sub_0807B4D0
	asr r0, r0, #8
	sub r0, #0x10
	mov r7, r9
	sub r5, r7, r0
	ldr r0, [sp, #0x24]
	lsl r1, r0, #1
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r1, r0
	lsl r0, r0, #1
	add r0, r0, r4
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r2, #4
	ldsh r1, [r6, r2]
	bl sub_0807B4D0
	add r3, r0, #0
	asr r3, r3, #8
	add r3, sl
	ldr r0, _08028630 @ =0x080826E6
	ldrh r1, [r0, #2]
	mov r4, #0x40
	str r4, [sp, #0]
	mov r7, #0x20
	str r7, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #3
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08028634 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	add r2, r5, #0
	bl sub_0807B6B8
	add r1, r0, #0
	ldr r2, [r1]
	mov r0, #8
	mov r3, r8
	and r0, r3
	cmp r0, #0
	beq _0802863C
	ldr r0, _08028638 @ =0x08000400
	b _08028640
_08028628: .4byte 0x08000300
_0802862C: .4byte gUnk_08087BA4
_08028630: .4byte gUnk_080826E6
_08028634: .4byte 0x02020310
_08028638: .4byte 0x08000400
_0802863C:
	mov r0, #0x80
	lsl r0, r0, #0x14
_08028640:
	orr r2, r0
	str r2, [r1]
	ldr r0, _08028678 @ =0x02020310
	ldr r4, [sp, #0x4C]
	ldrh r4, [r4, #2]
	lsl r1, r4, #9
	neg r1, r1
	ldr r7, _0802867C @ =0x0000067C
	add r2, r0, r7
	strh r1, [r2]
	add r2, r0, #0
_08028656:
	mov r1, #0xCF
	lsl r1, r1, #3
	add r0, r2, r1
	mov r1, #0x80
	lsl r1, r1, #1
	strh r1, [r0]
	ldr r3, _08028680 @ =0x0000067A
	add r0, r2, r3
	strh r1, [r0]
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08028678: .4byte 0x02020310
_0802867C: .4byte 0x0000067C
_08028680: .4byte 0x0000067A
	thumb_func_end sub_080283BC

