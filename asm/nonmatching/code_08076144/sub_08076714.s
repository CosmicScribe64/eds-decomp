	thumb_func_start sub_08076714
sub_08076714: @ 0x08076714
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	str r4, [sp, #0]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	lsl r1, r0, #0x10
	lsr r2, r1, #0x10
	lsr r6, r0, #0x10
	mov r7, #0xFF
	lsl r7, r7, #8
	and r7, r4
	lsl r0, r4, #8
	ldr r5, _08076788 @ =0xFFFFFE00
	add r1, r5, #0
	and r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov ip, r0
	lsr r0, r3, #0x10
	mov r9, r0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	ldr r0, _0807678C @ =0x03000040
	ldr r3, _08076790 @ =0x00004830
	add r1, r0, r3
	add r3, r0, #0
	ldrb r1, [r1]
	cmp r1, #0x80
	bne _08076760
	b _080768CE
_08076760:
	ldr r5, _08076794 @ =0x00004831
	add r0, r3, r5
	ldrb r0, [r0]
	cmp r0, #0x20
	bne _0807676C
	b _080768CE
_0807676C:
	ldr r0, _08076798 @ =0x00004040
	cmp r4, r0
	beq _08076836
	cmp r4, r0
	bgt _080767AA
	cmp r4, #0x80
	beq _08076850
	cmp r4, #0x80
	bgt _0807679C
	cmp r4, #0
	beq _080767EC
	cmp r4, #0x40
	beq _0807681A
	b _08076882
_08076788: .4byte 0xFFFFFE00
_0807678C: .4byte 0x03000040
_08076790: .4byte 0x00004830
_08076794: .4byte 0x00004831
_08076798: .4byte 0x00004040
_0807679C:
	cmp r4, #0xC0
	beq _08076872
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r4, r0
	beq _0807680E
	b _08076882
_080767AA:
	mov r0, #0x80
	lsl r0, r0, #8
	cmp r4, r0
	beq _080767F6
	cmp r4, r0
	bgt _080767C6
	mov r0, #0x81
	lsl r0, r0, #7
	cmp r4, r0
	beq _08076842
	add r0, #0x40
	cmp r4, r0
	beq _08076864
	b _08076882
_080767C6:
	ldr r0, _080767D8 @ =0x00008080
	cmp r4, r0
	beq _08076828
	cmp r4, r0
	bgt _080767DC
	sub r0, #0x40
	cmp r4, r0
	beq _08076802
	b _08076882
_080767D8: .4byte 0x00008080
_080767DC:
	ldr r0, _080767E8 @ =0x000080C0
	ldr r1, [sp, #0]
	cmp r1, r0
	beq _0807685E
	b _08076882
	.align 2, 0
_080767E8: .4byte 0x000080C0
_080767EC:
	sub r0, r2, #4
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	sub r0, r6, #4
	b _0807687E
_080767F6:
	sub r0, r2, #4
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #8
	b _0807687E
_08076802:
	sub r0, r2, #4
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _0807687E
_0807680E:
	add r0, r2, #0
	sub r0, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	sub r0, r6, #4
	b _0807687E
_0807681A:
	add r0, r2, #0
	sub r0, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #8
	b _0807687E
_08076828:
	add r0, r2, #0
	sub r0, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _0807687E
_08076836:
	add r0, r2, #0
	sub r0, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	sub r0, r6, #4
	b _0807687E
_08076842:
	add r0, r2, #0
	sub r0, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #8
	b _0807687E
_08076850:
	add r0, r2, #0
	sub r0, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _0807687E
_0807685E:
	add r0, r2, #0
	sub r0, #0x10
	b _08076876
_08076864:
	add r0, r2, #0
	sub r0, #0x20
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _0807687E
_08076872:
	add r0, r2, #0
	sub r0, #0x20
_08076876:
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x20
_0807687E:
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
_08076882:
	ldr r4, _080768E0 @ =0x00004830
	add r5, r3, r4
	ldrb r0, [r5]
	lsl r1, r0, #3
	ldr r4, _080768E4 @ =0x00004430
	add r0, r3, r4
	add r1, r1, r0
	mov r0, #0xFF
	and r6, r0
	orr r7, r6
	mov r4, #0xC0
	lsl r4, r4, #2
	add r0, r4, #0
	orr r7, r0
	strh r7, [r1]
	ldr r0, _080768E8 @ =0x000001FF
	and r2, r0
	mov r0, ip
	orr r2, r0
	ldr r0, _080768EC @ =0x00004831
	add r4, r3, r0
	ldrb r3, [r4]
	lsl r0, r3, #9
	orr r2, r0
	strh r2, [r1, #2]
	mov r0, sl
	strh r0, [r1, #4]
	ldrb r0, [r4]
	mov r1, r9
	mov r2, r8
	bl sub_08076160
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080768CE:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080768E0: .4byte 0x00004830
_080768E4: .4byte 0x00004430
_080768E8: .4byte 0x000001FF
_080768EC: .4byte 0x00004831
	thumb_func_end sub_08076714

