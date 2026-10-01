	thumb_func_start sub_0804D320
sub_0804D320: @ 0x0804D320
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r4, r0, #0
	ldr r6, _0804D340 @ =0x02018450
	mov r0, #2
	ldrb r1, [r6]
	and r0, r1
	cmp r0, #0
	beq _0804D344
_0804D33A:
	mov r0, #1
	bl _0804DB4E @ far jump
_0804D340: .4byte 0x02018450
_0804D344:
	ldr r7, _0804D380 @ =0x020192E0
	ldr r2, _0804D384 @ =0x00001B16
	add r1, r7, r2
	mov r0, #0xFF
	lsl r0, r0, #1
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0804D388
	mov r5, #0xA8
	lsl r5, r5, #1
	add r3, r6, r5
	ldr r1, [r3]
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	beq _0804D33A
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xA9
	lsl r1, r1, #1
	add r2, r6, r1
	ldrh r2, [r2]
	lsl r2, r2, #0x10
	ldrh r3, [r3]
	orr r2, r3
	mov r1, #0x13
	bl sub_08042AB0
	b _0804D33A
_0804D380: .4byte 0x020192E0
_0804D384: .4byte 0x00001B16
_0804D388:
	lsl r1, r4, #1
	add r0, r1, r4
	lsl r0, r0, #2
	add r5, r0, r6
	ldr r0, _0804D3B8 @ =0x000007FF
	ldrh r2, [r5, #0xA]
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _0804D3BC @ =0x08622AB4
	add r0, r0, r3
	ldrh r2, [r0]
	mov r0, #0xC7
	lsl r0, r0, #1
	mov sl, r1
	cmp r2, r0
	beq _0804D414
	cmp r2, r0
	bgt _0804D3C0
	cmp r2, #0xFF
	beq _0804D3D8
	sub r0, #6
	cmp r2, r0
	beq _0804D414
	b _0804D4C6
_0804D3B8: .4byte 0x000007FF
_0804D3BC: .4byte gUnk_08622AB4
_0804D3C0:
	ldr r0, _0804D3D0 @ =0x00000199
	cmp r2, r0
	beq _0804D454
	ldr r0, _0804D3D4 @ =0x000005F3
	cmp r2, r0
	beq _0804D46A
	b _0804D4C6
	.align 2, 0
_0804D3D0: .4byte 0x00000199
_0804D3D4: .4byte 0x000005F3
_0804D3D8:
	lsl r0, r4, #0x1F
	ldrh r1, [r6]
	lsl r3, r1, #0x17
	lsr r1, r3, #0x1D
	lsl r1, r1, #0x10
	mov r2, #0x91
	lsl r2, r2, #0x16
	orr r1, r2
	orr r0, r1
	ldrh r5, [r5, #0xA]
	orr r0, r5
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r1, r3
	mov r2, #1
	sub r2, r2, r4
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	ldrb r6, [r6, #1]
	lsl r3, r6, #0x1C
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r2, r3
	lsl r2, r2, #0x10
	orr r1, r2
	bl sub_0801FBCC
	b _0804D4C6
_0804D414:
	add r0, r4, #0
	bl sub_0804A998
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804D4C6
	ldr r5, _0804D44C @ =0x02018450
	mov r2, sl
	add r0, r2, r4
	lsl r0, r0, #2
	add r0, r0, r5
	ldrh r1, [r0, #0xA]
	add r0, r4, #0
	bl sub_080197E0
	mov r0, #0x95
	cmp r4, #1
	beq _0804D43A
	ldr r0, _0804D450 @ =0x00008095
_0804D43A:
	ldrb r5, [r5, #1]
	lsl r1, r5, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #5
	mov r3, #0
	bl sub_0801EC58
	b _0804D4C6
	.align 2, 0
_0804D44C: .4byte 0x02018450
_0804D450: .4byte 0x00008095
_0804D454:
	add r0, r4, #0
	bl sub_0804A998
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804D4C6
	ldrh r1, [r5, #0xA]
	add r0, r4, #0
	bl sub_080197E0
	b _0804D4C6
_0804D46A:
	mov r3, #1
	sub r0, r3, r4
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r6
	ldrb r1, [r1, #8]
	lsl r0, r1, #0x1C
	cmp r0, #0
	bge _0804D4C6
	add r2, r4, #0
	and r2, r3
	ldrh r3, [r6]
	lsl r0, r3, #0x17
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0804D620 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	add r1, r7, #0
	add r1, #0x2C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804D4C6
	ldrh r1, [r5, #0xA]
	add r0, r4, #0
	bl sub_080197E0
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	ldrh r6, [r6]
	lsl r2, r6, #0x17
	lsr r1, r2, #0x1D
	lsl r1, r1, #8
	orr r1, r0
	lsr r2, r2, #0x1D
	lsl r2, r2, #8
	orr r2, r0
	mov r3, #0x86
	lsl r3, r3, #1
	add r0, r4, #0
	bl sub_08017AB4
_0804D4C6:
	ldr r6, _0804D624 @ =0x02018450
	mov r5, #0xA8
	lsl r5, r5, #1
	add r1, r6, r5
	ldr r0, _0804D628 @ =0x0000FFFF
	strh r0, [r1]
	mov r1, sl
	add r0, r1, r4
	lsl r0, r0, #2
	add r5, r0, r6
	ldrb r2, [r5, #8]
	lsl r0, r2, #0x1C
	cmp r0, #0
	blt _0804D4E4
	b _0804D6DA
_0804D4E4:
	mov r3, #0
	mov r9, r3
	mov r7, #1
	sub r3, r7, r4
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r1, _0804D62C @ =0x000007FF
	mov ip, r1
	ldrh r0, [r0, #0xA]
	and r1, r0
	lsl r1, r1, #1
	ldr r2, _0804D630 @ =0x08622AB4
	mov r8, r2
	add r1, r8
	ldr r0, _0804D634 @ =0x000002F9
	ldrh r1, [r1]
	cmp r1, r0
	bne _0804D520
	ldr r2, _0804D638 @ =0x020192E4
	and r3, r7
	ldr r0, _0804D620 @ =0x00000D64
	add r1, r3, #0
	mul r1, r0
	add r1, r1, r2
	mov r0, #8
	ldrb r3, [r1, #8]
	orr r0, r3
	strb r0, [r1, #8]
_0804D520:
	mov r0, ip
	ldrh r5, [r5, #0xA]
	and r0, r5
	lsl r0, r0, #1
	add r0, r8
	ldrh r1, [r0]
	ldr r0, _0804D63C @ =0x000002DA
	cmp r1, r0
	beq _0804D538
	ldr r0, _0804D640 @ =0x00000536
	cmp r1, r0
	bne _0804D57C
_0804D538:
	ldrh r5, [r6]
	lsl r1, r5, #0x17
	lsr r1, r1, #0x1D
	add r0, r4, #0
	bl sub_0800A430
	ldr r1, _0804D628 @ =0x0000FFFF
	cmp r0, r1
	beq _0804D57C
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r0, r6, #0
	add r0, #0x20
	add r5, r1, r0
	ldrh r0, [r6]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r4, #0
	bl sub_08017F98
	mov r0, #0xA4
	cmp r4, #0
	beq _0804D56A
	ldr r0, _0804D644 @ =0x000080A4
_0804D56A:
	ldrh r6, [r6]
	lsl r1, r6, #0x17
	lsr r1, r1, #0x1D
	ldrh r2, [r5]
	ldrh r3, [r5, #2]
	bl sub_0801EC58
	mov r1, #1
	mov r9, r1
_0804D57C:
	ldr r6, _0804D624 @ =0x02018450
	mov r0, #1
	sub r0, r0, r4
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r6
	ldr r2, _0804D62C @ =0x000007FF
	add r0, r2, #0
	ldrh r1, [r1, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _0804D630 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, #0xFF
	bne _0804D5A0
	b _0804D6BC
_0804D5A0:
	mov r5, sl
	add r0, r5, r4
	lsl r0, r0, #2
	add r5, r0, r6
	add r0, r2, #0
	ldrh r1, [r5, #0xA]
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r2, [r0]
	cmp r2, #0xFF
	bne _0804D5BA
	b _0804D6BC
_0804D5BA:
	mov r3, r9
	cmp r3, #0
	beq _0804D5C2
	b _0804D6DA
_0804D5C2:
	ldrh r1, [r0]
	mov r0, #0x9D
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0804D5F6
	sub r0, #2
	cmp r1, r0
	blt _0804D5F6
	ldrh r1, [r5, #0xA]
	add r0, r4, #0
	bl sub_080197C0
	ldrh r1, [r5, #0xA]
	mov r2, #1
	sub r2, r2, r4
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	ldrb r6, [r6, #1]
	lsl r0, r6, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	add r0, r4, #0
	mov r3, #9
	bl sub_08017AB4
_0804D5F6:
	ldr r3, _0804D624 @ =0x02018450
	mov r0, #1
	sub r0, r0, r4
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r3
	ldr r0, _0804D62C @ =0x000007FF
	ldrh r1, [r1, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r5, _0804D630 @ =0x08622AB4
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _0804D648 @ =0x0000052F
	cmp r1, r0
	beq _0804D64C
	add r0, #0xD
	cmp r1, r0
	beq _0804D66E
	b _0804D680
_0804D620: .4byte 0x00000D64
_0804D624: .4byte 0x02018450
_0804D628: .4byte 0x0000FFFF
_0804D62C: .4byte 0x000007FF
_0804D630: .4byte gUnk_08622AB4
_0804D634: .4byte 0x000002F9
_0804D638: .4byte 0x020192E4
_0804D63C: .4byte 0x000002DA
_0804D640: .4byte 0x00000536
_0804D644: .4byte 0x000080A4
_0804D648: .4byte 0x0000052F
_0804D64C:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r1, r1, r3
	add r1, #0x23
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r2, #0xE
	ldrb r3, [r3, #1]
	and r2, r3
	mov r3, #0xF
	neg r3, r3
	and r0, r3
	orr r0, r2
	b _0804D67E
_0804D66E:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r1, r1, r3
	add r1, #0x23
	mov r0, #0x10
	ldrb r3, [r1]
	orr r0, r3
_0804D67E:
	strb r0, [r1]
_0804D680:
	mov r0, #1
	sub r0, r0, r4
	ldr r5, _0804D6B8 @ =0x02018450
	ldrh r1, [r5]
	lsl r2, r1, #0x17
	lsr r2, r2, #0x1D
	mov r1, #0x94
	add r3, r4, #0
	mul r3, r1
	add r1, r5, #0
	add r1, #0x20
	add r3, r3, r1
	add r1, r4, #0
	bl sub_08018690
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	ldrh r2, [r5]
	lsl r0, r2, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r1, r0
	mov r3, #0xA8
	lsl r3, r3, #1
	add r0, r5, r3
	strh r1, [r0]
	b _0804D6DA
	.align 2, 0
_0804D6B8: .4byte 0x02018450
_0804D6BC:
	mov r5, r9
	cmp r5, #0
	bne _0804D6DA
	ldr r3, _0804D710 @ =0x02018450
	ldrh r0, [r3]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	add r3, #0x20
	add r2, r2, r3
	add r0, r4, #0
	bl sub_08018664
_0804D6DA:
	ldr r7, _0804D710 @ =0x02018450
	mov r3, #1
	sub r5, r3, r4
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #2
	add r6, r0, r7
	ldr r0, _0804D714 @ =0x000007FF
	ldrh r1, [r6, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0804D718 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0804D71C @ =0x00000261
	cmp r1, r0
	bne _0804D6FE
	b _0804D818
_0804D6FE:
	cmp r1, r0
	bgt _0804D720
	cmp r1, #0xFF
	beq _0804D748
	sub r0, #0xD8
	cmp r1, r0
	beq _0804D7DA
	b _0804D906
	.align 2, 0
_0804D710: .4byte 0x02018450
_0804D714: .4byte 0x000007FF
_0804D718: .4byte gUnk_08622AB4
_0804D71C: .4byte 0x00000261
_0804D720:
	ldr r0, _0804D734 @ =0x000004B1
	cmp r1, r0
	bne _0804D728
	b _0804D854
_0804D728:
	cmp r1, r0
	bgt _0804D738
	sub r0, #0x98
	cmp r1, r0
	beq _0804D778
	b _0804D906
_0804D734: .4byte 0x000004B1
_0804D738:
	ldr r0, _0804D744 @ =0x000005F3
	cmp r1, r0
	bne _0804D740
	b _0804D8AC
_0804D740:
	b _0804D906
	.align 2, 0
_0804D744: .4byte 0x000005F3
_0804D748:
	and r5, r3
	lsl r0, r5, #0x1F
	ldrh r5, [r7]
	lsl r3, r5, #0x17
	lsr r1, r3, #0x1D
	lsl r1, r1, #0x10
	mov r2, #0x91
	lsl r2, r2, #0x16
	orr r1, r2
	orr r0, r1
	ldrh r6, [r6, #0xA]
	orr r0, r6
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r1, r3
	mov r2, #1
	sub r2, r2, r4
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	ldrb r7, [r7, #1]
	lsl r3, r7, #0x1C
	b _0804D7CA
_0804D778:
	add r0, r4, #0
	str r3, [sp, #0]
	bl sub_0804A998
	lsl r0, r0, #0x10
	ldr r3, [sp, #0]
	cmp r0, #0
	beq _0804D78A
	b _0804D906
_0804D78A:
	mov r1, sl
	add r0, r1, r4
	lsl r0, r0, #2
	add r0, r0, r7
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _0804D79C
	b _0804D906
_0804D79C:
	and r5, r3
	lsl r0, r5, #0x1F
	ldrb r2, [r7, #1]
	lsl r3, r2, #0x1C
	lsr r1, r3, #0x1D
	lsl r1, r1, #0x10
	mov r2, #0x91
	lsl r2, r2, #0x16
	orr r1, r2
	orr r0, r1
	ldrh r6, [r6, #0xA]
	orr r0, r6
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	ldrh r7, [r7]
	lsl r2, r7, #0x17
	lsr r2, r2, #0x1D
	lsl r2, r2, #8
	orr r1, r2
	mov r2, #1
	sub r2, r2, r4
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
_0804D7CA:
	lsr r3, r3, #0x1D
	lsl r3, r3, #8
	orr r2, r3
	lsl r2, r2, #0x10
	orr r1, r2
	bl sub_0801FBCC
	b _0804D906
_0804D7DA:
	add r0, r4, #0
	bl sub_0804A998
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804D7E8
	b _0804D906
_0804D7E8:
	ldrh r3, [r7]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x1D
	add r0, r4, #0
	bl sub_0800C8BC
	cmp r0, #7
	bne _0804D7FA
	b _0804D906
_0804D7FA:
	ldrh r1, [r6, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #0x95
	cmp r4, #0
	beq _0804D80A
	ldr r0, _0804D814 @ =0x00008095
_0804D80A:
	ldrh r7, [r7]
	lsl r1, r7, #0x17
	lsr r1, r1, #0x1D
	mov r2, #3
	b _0804D896
_0804D814: .4byte 0x00008095
_0804D818:
	add r0, r4, #0
	bl sub_0804A998
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804D906
	ldrh r0, [r7]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r4, #0
	bl sub_0800C8BC
	cmp r0, #2
	beq _0804D906
	ldrh r1, [r6, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #0x97
	cmp r4, #0
	beq _0804D844
	ldr r0, _0804D850 @ =0x00008097
_0804D844:
	ldrh r7, [r7]
	lsl r1, r7, #0x17
	lsr r1, r1, #0x1D
	mov r2, #1
	b _0804D896
	.align 2, 0
_0804D850: .4byte 0x00008097
_0804D854:
	ldrb r1, [r6, #8]
	lsl r0, r1, #0x1C
	cmp r0, #0
	blt _0804D906
	add r2, r5, #0
	and r2, r3
	ldrb r1, [r7, #1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r1, r0
	ldr r0, _0804D8A0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0804D8A4 @ =0x0201930C
	add r1, r1, r0
	add r0, r3, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804D906
	ldrh r1, [r6, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #0x7E
	cmp r4, #1
	beq _0804D88E
	ldr r0, _0804D8A8 @ =0x0000807E
_0804D88E:
	ldrb r7, [r7, #1]
	lsl r1, r7, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #0
_0804D896:
	mov r3, #0
	bl sub_0801EC58
	b _0804D906
	.align 2, 0
_0804D8A0: .4byte 0x00000D64
_0804D8A4: .4byte 0x0201930C
_0804D8A8: .4byte 0x0000807E
_0804D8AC:
	mov r2, sl
	add r0, r2, r4
	lsl r0, r0, #2
	add r0, r0, r7
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _0804D906
	add r2, r5, #0
	and r2, r3
	ldrb r3, [r7, #1]
	lsl r0, r3, #0x1C
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0804DA64 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0804DA68 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804D906
	ldrh r1, [r6, #0xA]
	add r0, r5, #0
	bl sub_080197E0
	mov r0, #1
	sub r0, r0, r4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrb r7, [r7, #1]
	lsl r2, r7, #0x1C
	lsr r1, r2, #0x1D
	lsl r1, r1, #8
	orr r1, r0
	lsr r2, r2, #0x1D
	lsl r2, r2, #8
	orr r2, r0
	mov r3, #0x86
	lsl r3, r3, #1
	add r0, r5, #0
	bl sub_08017AB4
_0804D906:
	ldr r7, _0804DA6C @ =0x02018450
	mov r5, #0xA9
	lsl r5, r5, #1
	add r1, r7, r5
	ldr r0, _0804DA70 @ =0x0000FFFF
	strh r0, [r1]
	mov r0, #1
	mov ip, r0
	sub r6, r0, r4
	lsl r0, r6, #1
	add r0, r0, r6
	lsl r0, r0, #2
	add r3, r0, r7
	ldrb r1, [r3, #8]
	lsl r0, r1, #0x1C
	cmp r0, #0
	blt _0804D92A
	b _0804DB30
_0804D92A:
	mov r2, #0
	mov r9, r2
	mov r5, sl
	add r0, r5, r4
	lsl r0, r0, #2
	add r0, r0, r7
	ldr r1, _0804DA74 @ =0x000007FF
	mov r8, r1
	ldrh r0, [r0, #0xA]
	and r1, r0
	lsl r1, r1, #1
	ldr r5, _0804DA78 @ =0x08622AB4
	add r1, r1, r5
	ldr r0, _0804DA7C @ =0x000002F9
	ldrh r1, [r1]
	cmp r1, r0
	bne _0804D962
	ldr r2, _0804DA80 @ =0x020192E4
	add r0, r4, #0
	mov r1, ip
	and r0, r1
	ldr r1, _0804DA64 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #8
	ldrb r2, [r1, #8]
	orr r0, r2
	strb r0, [r1, #8]
_0804D962:
	mov r0, r8
	ldrh r3, [r3, #0xA]
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _0804DA84 @ =0x000002DA
	cmp r1, r0
	beq _0804D97A
	ldr r0, _0804DA88 @ =0x00000536
	cmp r1, r0
	bne _0804D9BE
_0804D97A:
	ldrb r3, [r7, #1]
	lsl r1, r3, #0x1C
	lsr r1, r1, #0x1D
	add r0, r6, #0
	bl sub_0800A430
	ldr r1, _0804DA70 @ =0x0000FFFF
	cmp r0, r1
	beq _0804D9BE
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	add r0, r7, #0
	add r0, #0x20
	add r5, r1, r0
	ldrb r0, [r7, #1]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1D
	add r0, r6, #0
	bl sub_08017F98
	mov r0, #0xA4
	cmp r4, #1
	beq _0804D9AC
	ldr r0, _0804DA8C @ =0x000080A4
_0804D9AC:
	ldrb r7, [r7, #1]
	lsl r1, r7, #0x1C
	lsr r1, r1, #0x1D
	ldrh r2, [r5]
	ldrh r3, [r5, #2]
	bl sub_0801EC58
	mov r1, #1
	mov r9, r1
_0804D9BE:
	ldr r1, _0804DA6C @ =0x02018450
	mov r0, #1
	sub r6, r0, r4
	lsl r0, r6, #1
	add r0, r0, r6
	lsl r0, r0, #2
	add r5, r0, r1
	ldr r2, _0804DA74 @ =0x000007FF
	add r0, r2, #0
	ldrh r3, [r5, #0xA]
	and r0, r3
	lsl r0, r0, #1
	ldr r3, _0804DA78 @ =0x08622AB4
	add r7, r0, r3
	mov r8, r1
	ldrh r0, [r7]
	cmp r0, #0xFF
	bne _0804D9E4
	b _0804DB10
_0804D9E4:
	mov r3, sl
	add r1, r3, r4
	lsl r1, r1, #2
	add r1, r8
	add r0, r2, #0
	ldrh r1, [r1, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804DA78 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0xFF
	bne _0804DA00
	b _0804DB10
_0804DA00:
	mov r2, r9
	cmp r2, #0
	beq _0804DA08
	b _0804DB30
_0804DA08:
	ldrh r1, [r7]
	mov r0, #0x9D
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0804DA3A
	sub r0, #2
	cmp r1, r0
	blt _0804DA3A
	ldrh r1, [r5, #0xA]
	add r0, r4, #0
	bl sub_080197C0
	ldrh r1, [r5, #0xA]
	lsl r2, r4, #0x18
	lsr r2, r2, #0x18
	mov r3, r8
	ldrh r3, [r3]
	lsl r0, r3, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r2, r0
	add r0, r6, #0
	mov r3, #9
	bl sub_08017AB4
_0804DA3A:
	ldr r2, _0804DA6C @ =0x02018450
	mov r5, sl
	add r1, r5, r4
	lsl r1, r1, #2
	add r1, r1, r2
	ldr r0, _0804DA74 @ =0x000007FF
	ldrh r1, [r1, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0804DA78 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0804DA90 @ =0x0000052F
	mov r8, r2
	cmp r1, r0
	beq _0804DA94
	add r0, #0xD
	cmp r1, r0
	beq _0804DAC0
	b _0804DAD4
	.align 2, 0
_0804DA64: .4byte 0x00000D64
_0804DA68: .4byte 0x0201930C
_0804DA6C: .4byte 0x02018450
_0804DA70: .4byte 0x0000FFFF
_0804DA74: .4byte 0x000007FF
_0804DA78: .4byte gUnk_08622AB4
_0804DA7C: .4byte 0x000002F9
_0804DA80: .4byte 0x020192E4
_0804DA84: .4byte 0x000002DA
_0804DA88: .4byte 0x00000536
_0804DA8C: .4byte 0x000080A4
_0804DA90: .4byte 0x0000052F
_0804DA94:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0x94
	add r2, r0, #0
	mul r2, r1
	add r2, r8
	add r2, #0x23
	mov r0, #1
	ldrb r3, [r2]
	orr r0, r3
	strb r0, [r2]
	mov r5, r8
	ldrh r5, [r5]
	lsl r1, r5, #0x17
	lsr r1, r1, #0x1D
	lsl r1, r1, #1
	mov r3, #0xF
	neg r3, r3
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _0804DAD4
_0804DAC0:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0x94
	mul r1, r0
	add r1, r8
	add r1, #0x23
	mov r0, #0x10
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_0804DAD4:
	mov r1, #1
	sub r1, r1, r4
	mov r3, r8
	ldrb r3, [r3, #1]
	lsl r2, r3, #0x1C
	lsr r2, r2, #0x1D
	mov r0, #0x94
	add r3, r1, #0
	mul r3, r0
	mov r0, r8
	add r0, #0x20
	add r3, r3, r0
	add r0, r1, #0
	bl sub_08018690
	mov r1, #1
	sub r1, r1, r4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r5, r8
	ldrb r5, [r5, #1]
	lsl r0, r5, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r1, r0
	mov r0, #0xA9
	lsl r0, r0, #1
	add r0, r8
	strh r1, [r0]
	b _0804DB30
_0804DB10:
	mov r0, r9
	cmp r0, #0
	bne _0804DB30
	mov r0, #1
	sub r0, r0, r4
	mov r2, r8
	ldrb r2, [r2, #1]
	lsl r1, r2, #0x1C
	lsr r1, r1, #0x1D
	mov r2, #0x94
	mul r2, r0
	mov r3, r8
	add r3, #0x20
	add r2, r2, r3
	bl sub_08018664
_0804DB30:
	ldr r2, _0804DB60 @ =0x020192E0
	ldr r3, _0804DB64 @ =0x00001B16
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804DB68 @ =0xFFFFFE01
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	mov r0, #0
_0804DB4E:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0804DB60: .4byte 0x020192E0
_0804DB64: .4byte 0x00001B16
_0804DB68: .4byte 0xFFFFFE01
	thumb_func_end sub_0804D320

