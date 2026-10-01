	thumb_func_start sub_08028684
sub_08028684: @ 0x08028684
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x24
	ldr r0, [sp, #0x3C]
	ldr r5, [sp, #0x40]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	mov r6, #0x40
	mov r7, #0x20
	cmp r4, #0
	beq _080286A8
	cmp r4, #1
	beq _08028704
	b _0802876A
_080286A8:
	mov r1, #0
	ldsh r0, [r5, r1]
	mov r1, #0x80
	lsl r1, r1, #6
	bl sub_0807B4D0
	mov r2, #2
	ldsh r3, [r5, r2]
	asr r0, r0, #8
	sub r3, r3, r0
	add r3, #0x50
	ldr r0, _080286F8 @ =0x080826E6
	ldrh r1, [r0]
	str r6, [sp, #0]
	str r7, [sp, #4]
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
	ldr r0, _080286FC @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	mov r2, #0x58
	bl sub_0807B6B8
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, #8
	mov r3, r8
	and r0, r3
	cmp r0, #0
	beq _08028764
	ldr r0, _08028700 @ =0x08000500
	b _08028766
_080286F8: .4byte gUnk_080826E6
_080286FC: .4byte 0x02020310
_08028700: .4byte 0x08000500
_08028704:
	mov r1, #0
	ldsh r0, [r5, r1]
	mov r1, #0x80
	lsl r1, r1, #6
	bl sub_0807B4D0
	mov r2, #2
	ldsh r3, [r5, r2]
	asr r0, r0, #8
	sub r3, r3, r0
	add r3, #0x50
	ldr r0, _08028758 @ =0x080826E6
	ldrh r1, [r0, #2]
	str r6, [sp, #0]
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
	ldr r0, _0802875C @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	mov r2, #0x58
	bl sub_0807B6B8
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, #8
	mov r3, r8
	and r0, r3
	cmp r0, #0
	beq _08028764
	ldr r0, _08028760 @ =0x08000500
	b _08028766
	.align 2, 0
_08028758: .4byte gUnk_080826E6
_0802875C: .4byte 0x02020310
_08028760: .4byte 0x08000500
_08028764:
	ldr r0, _08028794 @ =0x08000100
_08028766:
	orr r1, r0
	str r1, [r2]
_0802876A:
	ldr r1, _08028798 @ =0x02020310
	ldr r0, _0802879C @ =0x0000067C
	add r2, r1, r0
	mov r0, #0
	strh r0, [r2]
	mov r3, #0xCF
	lsl r3, r3, #3
	add r2, r1, r3
	mov r0, #0x80
	lsl r0, r0, #1
	strh r0, [r2]
	ldrh r0, [r5]
	ldr r2, _080287A0 @ =0x0000067A
	add r1, r1, r2
	strh r0, [r1]
	add sp, #0x24
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08028794: .4byte 0x08000100
_08028798: .4byte 0x02020310
_0802879C: .4byte 0x0000067C
_080287A0: .4byte 0x0000067A
	thumb_func_end sub_08028684

