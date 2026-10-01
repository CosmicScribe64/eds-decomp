	thumb_func_start sub_080656B4
sub_080656B4: @ 0x080656B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov ip, r2
	ldr r2, [sp, #0x30]
	ldr r4, [sp, #0x34]
	ldr r5, [sp, #0x38]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r6, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r9, r1
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov sl, r3
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #4]
	lsl r5, r5, #0x10
	lsr r7, r5, #0x10
	cmp r1, #0
	beq _080657DA
	cmp r0, #1
	beq _08065718
	cmp r0, #1
	bgt _080656FC
	cmp r0, #0
	beq _08065706
	b _08065744
_080656FC:
	cmp r6, #2
	beq _08065728
	cmp r6, #3
	beq _0806573C
	b _08065744
_08065706:
	ldr r0, _08065710 @ =0x0808733C
	mov r8, r0
	ldr r1, _08065714 @ =0x08087394
	str r1, [sp, #8]
	b _08065744
_08065710: .4byte gUnk_0808733C
_08065714: .4byte gUnk_08087394
_08065718:
	ldr r2, _08065720 @ =0x08087352
	mov r8, r2
	ldr r5, _08065724 @ =0x080873C0
	b _08065742
_08065720: .4byte gUnk_08087352
_08065724: .4byte gUnk_080873C0
_08065728:
	ldr r0, _08065734 @ =0x0808737C
	mov r8, r0
	ldr r1, _08065738 @ =0x08087424
	str r1, [sp, #8]
	b _08065744
	.align 2, 0
_08065734: .4byte gUnk_0808737C
_08065738: .4byte gUnk_08087424
_0806573C:
	ldr r2, _080657EC @ =0x0808738A
	mov r8, r2
	ldr r5, _080657F0 @ =0x08087440
_08065742:
	str r5, [sp, #8]
_08065744:
	mov r5, #0x1F
	mov r3, sl
	and r3, r5
	ldr r1, [sp, #0]
	and r1, r5
	lsl r1, r1, #5
	add r2, r3, r1
	lsl r2, r2, #1
	add r2, ip
	str r2, [sp, #0xC]
	mov r0, r9
	lsl r4, r0, #1
	add r4, r8
	ldrh r2, [r4]
	add r0, r2, r7
	ldr r2, _080657F4 @ =0x000003FF
	mov r8, r2
	mov r2, r8
	and r0, r2
	ldr r2, [sp, #4]
	lsl r6, r2, #0xC
	orr r0, r6
	ldr r2, [sp, #0xC]
	strh r0, [r2]
	mov r2, sl
	add r2, #1
	and r2, r5
	add r1, r2, r1
	lsl r1, r1, #1
	add r1, ip
	mov sl, r1
	ldrh r1, [r4]
	add r0, r1, r7
	add r0, #1
	mov r1, r8
	and r0, r1
	orr r0, r6
	mov r1, sl
	strh r0, [r1]
	ldr r1, [sp, #0]
	add r1, #1
	and r1, r5
	lsl r1, r1, #5
	add r3, r3, r1
	lsl r3, r3, #1
	add r3, ip
	ldrh r5, [r4]
	add r0, r5, r7
	add r0, #2
	mov r5, r8
	and r0, r5
	orr r0, r6
	strh r0, [r3]
	add r2, r2, r1
	lsl r2, r2, #1
	add r2, ip
	ldrh r4, [r4]
	add r0, r4, r7
	add r0, #3
	and r0, r5
	orr r0, r6
	strh r0, [r2]
	mov r1, r9
	lsl r0, r1, #2
	ldr r2, [sp, #8]
	add r0, r0, r2
	ldr r0, [r0]
	ldr r5, [sp, #4]
	lsl r1, r5, #5
	mov r2, #0xA0
	lsl r2, r2, #0x13
	add r1, r1, r2
	mov r2, #0x10
	bl CpuSet
_080657DA:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080657EC: .4byte gUnk_0808738A
_080657F0: .4byte gUnk_08087440
_080657F4: .4byte 0x000003FF
	thumb_func_end sub_080656B4

