	thumb_func_start sub_0805E3B8
sub_0805E3B8: @ 0x0805E3B8
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	ldr r0, _0805E494 @ =0x020185A4
	mov r1, #0xC
	bl sub_08075278
	ldr r2, _0805E498 @ =0x0201CFB0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #5
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0x42
	strh r0, [r1]
	ldr r0, _0805E49C @ =0x03000040
	ldr r2, _0805E4A0 @ =0x0000040E
	add r0, r0, r2
	mov r2, #0
	mov r1, #1
	strh r1, [r0]
	ldr r0, _0805E4A4 @ =0x04000050
	strh r2, [r0]
	ldr r1, _0805E4A8 @ =0x0400000C
	ldr r2, _0805E4AC @ =0x00004084
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0805E4B0 @ =0x00004188
	add r0, r2, #0
	strh r0, [r1]
	bl sub_08073498
	bl sub_080757AC
	ldr r0, _0805E4B4 @ =0x05000240
	ldr r1, _0805E4B8 @ =0x0863840C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _0805E4BC @ =0x06010400
	ldr r1, _0805E4C0 @ =0x0863842C
	mov r2, #0xE0
	lsl r2, r2, #1
	bl sub_080752B0
	ldr r0, _0805E4C4 @ =0x05000260
	ldr r1, _0805E4C8 @ =0x0868247C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _0805E4CC @ =0x060105C0
	ldr r1, _0805E4D0 @ =0x0868267C
	mov r2, #0xC0
	lsl r2, r2, #5
	bl sub_080752B0
	ldr r0, _0805E4D4 @ =0x06004000
	mov r1, #0x80
	lsl r1, r1, #7
	bl sub_08075278
	mov r0, #0xC0
	lsl r0, r0, #0x13
	mov r1, #0x80
	lsl r1, r1, #4
	bl sub_08075278
	cmp r5, #0
	bne _0805E454
	b _0805E5D4
_0805E454:
	mov r4, #0xA6
	lsl r4, r4, #1
	mov r0, #0
	add r1, r5, #0
	add r2, r4, #0
	mov r3, #0x40
	bl sub_0805DF34
	mov r0, #0
	mov r1, #3
	mov r2, #5
	add r3, r4, #0
	bl sub_0805E100
	ldr r0, _0805E4D8 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _0805E4DC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0805E4F0
	cmp r0, #0x16
	bgt _0805E4E0
	cmp r0, #0x15
	beq _0805E4E6
	b _0805E500
	.align 2, 0
_0805E494: .4byte 0x020185A4
_0805E498: .4byte 0x0201CFB0
_0805E49C: .4byte 0x03000040
_0805E4A0: .4byte 0x0000040E
_0805E4A4: .4byte 0x04000050
_0805E4A8: .4byte 0x0400000C
_0805E4AC: .4byte 0x00004084
_0805E4B0: .4byte 0x00004188
_0805E4B4: .4byte 0x05000240
_0805E4B8: .4byte gUnk_0863840C
_0805E4BC: .4byte 0x06010400
_0805E4C0: .4byte gUnk_0863842C
_0805E4C4: .4byte 0x05000260
_0805E4C8: .4byte gUnk_0868247C
_0805E4CC: .4byte 0x060105C0
_0805E4D0: .4byte gUnk_0868267C
_0805E4D4: .4byte 0x06004000
_0805E4D8: .4byte 0x000007FF
_0805E4DC: .4byte gUnk_08621DE0
_0805E4E0:
	cmp r0, #0x17
	beq _0805E4F8
	b _0805E500
_0805E4E6:
	ldr r1, _0805E4EC @ =0x08631558
	b _0805E5BE
	.align 2, 0
_0805E4EC: .4byte gUnk_08631558
_0805E4F0:
	ldr r1, _0805E4F4 @ =0x0862EEC0
	b _0805E5BE
_0805E4F4: .4byte gUnk_0862EEC0
_0805E4F8:
	ldr r1, _0805E4FC @ =0x08633BF0
	b _0805E5BE
_0805E4FC: .4byte gUnk_08633BF0
_0805E500:
	ldr r0, _0805E518 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r2, _0805E51C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0805E520 @ =0x00000776
	cmp r1, r0
	bne _0805E524
	mov r0, #3
	b _0805E586
	.align 2, 0
_0805E518: .4byte 0x000007FF
_0805E51C: .4byte gUnk_08622AB4
_0805E520: .4byte 0x00000776
_0805E524:
	cmp r1, r0
	blt _0805E534
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0805E534
	mov r0, #1
	b _0805E586
_0805E534:
	ldr r0, _0805E558 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _0805E55C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0805E566
	cmp r0, #0x16
	bgt _0805E560
	cmp r0, #0x15
	beq _0805E56A
	b _0805E572
	.align 2, 0
_0805E558: .4byte 0x000007FF
_0805E55C: .4byte gUnk_08621DE0
_0805E560:
	cmp r0, #0x17
	beq _0805E56E
	b _0805E572
_0805E566:
	mov r0, #7
	b _0805E586
_0805E56A:
	mov r0, #8
	b _0805E586
_0805E56E:
	mov r0, #9
	b _0805E586
_0805E572:
	ldr r0, _0805E594 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _0805E598 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0805E586:
	cmp r0, #2
	beq _0805E5AC
	cmp r0, #2
	bgt _0805E59C
	cmp r0, #1
	beq _0805E5A2
	b _0805E5BC
_0805E594: .4byte 0x000007FF
_0805E598: .4byte gUnk_08621DE0
_0805E59C:
	cmp r0, #3
	beq _0805E5B4
	b _0805E5BC
_0805E5A2:
	ldr r1, _0805E5A8 @ =0x08627AF8
	b _0805E5BE
	.align 2, 0
_0805E5A8: .4byte gUnk_08627AF8
_0805E5AC:
	ldr r1, _0805E5B0 @ =0x0862A190
	b _0805E5BE
_0805E5B0: .4byte gUnk_0862A190
_0805E5B4:
	ldr r1, _0805E5B8 @ =0x0862C828
	b _0805E5BE
_0805E5B8: .4byte gUnk_0862C828
_0805E5BC:
	ldr r1, _0805E62C @ =0x08625460
_0805E5BE:
	mov r0, #0
	mov r2, #0x2C
	mov r3, #0x80
	bl sub_0805E054
	mov r0, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0x2C
	bl sub_0805E1D0
_0805E5D4:
	ldr r0, _0805E630 @ =0x06008000
	mov r1, #0x80
	lsl r1, r1, #7
	bl sub_08075278
	ldr r0, _0805E634 @ =0x06000800
	mov r1, #0x80
	lsl r1, r1, #4
	bl sub_08075278
	cmp r6, #0
	bne _0805E5EE
	b _0805E734
_0805E5EE:
	mov r4, #0xA6
	lsl r4, r4, #1
	mov r0, #1
	add r1, r6, #0
	add r2, r4, #0
	mov r3, #0xA0
	bl sub_0805DF34
	mov r0, #1
	mov r1, #0x12
	mov r2, #5
	add r3, r4, #0
	bl sub_0805E100
	ldr r0, _0805E638 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0805E63C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0805E650
	cmp r0, #0x16
	bgt _0805E640
	cmp r0, #0x15
	beq _0805E646
	b _0805E660
_0805E62C: .4byte gUnk_08625460
_0805E630: .4byte 0x06008000
_0805E634: .4byte 0x06000800
_0805E638: .4byte 0x000007FF
_0805E63C: .4byte gUnk_08621DE0
_0805E640:
	cmp r0, #0x17
	beq _0805E658
	b _0805E660
_0805E646:
	ldr r1, _0805E64C @ =0x08631558
	b _0805E71E
	.align 2, 0
_0805E64C: .4byte gUnk_08631558
_0805E650:
	ldr r1, _0805E654 @ =0x0862EEC0
	b _0805E71E
_0805E654: .4byte gUnk_0862EEC0
_0805E658:
	ldr r1, _0805E65C @ =0x08633BF0
	b _0805E71E
_0805E65C: .4byte gUnk_08633BF0
_0805E660:
	ldr r0, _0805E678 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r2, _0805E67C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0805E680 @ =0x00000776
	cmp r1, r0
	bne _0805E684
	mov r0, #3
	b _0805E6E6
	.align 2, 0
_0805E678: .4byte 0x000007FF
_0805E67C: .4byte gUnk_08622AB4
_0805E680: .4byte 0x00000776
_0805E684:
	cmp r1, r0
	blt _0805E694
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0805E694
	mov r0, #1
	b _0805E6E6
_0805E694:
	ldr r0, _0805E6B8 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0805E6BC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0805E6C6
	cmp r0, #0x16
	bgt _0805E6C0
	cmp r0, #0x15
	beq _0805E6CA
	b _0805E6D2
	.align 2, 0
_0805E6B8: .4byte 0x000007FF
_0805E6BC: .4byte gUnk_08621DE0
_0805E6C0:
	cmp r0, #0x17
	beq _0805E6CE
	b _0805E6D2
_0805E6C6:
	mov r0, #7
	b _0805E6E6
_0805E6CA:
	mov r0, #8
	b _0805E6E6
_0805E6CE:
	mov r0, #9
	b _0805E6E6
_0805E6D2:
	ldr r0, _0805E6F4 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r2, _0805E6F8 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0805E6E6:
	cmp r0, #2
	beq _0805E70C
	cmp r0, #2
	bgt _0805E6FC
	cmp r0, #1
	beq _0805E702
	b _0805E71C
_0805E6F4: .4byte 0x000007FF
_0805E6F8: .4byte gUnk_08621DE0
_0805E6FC:
	cmp r0, #3
	beq _0805E714
	b _0805E71C
_0805E702:
	ldr r1, _0805E708 @ =0x08627AF8
	b _0805E71E
	.align 2, 0
_0805E708: .4byte gUnk_08627AF8
_0805E70C:
	ldr r1, _0805E710 @ =0x0862A190
	b _0805E71E
_0805E710: .4byte gUnk_0862A190
_0805E714:
	ldr r1, _0805E718 @ =0x0862C828
	b _0805E71E
_0805E718: .4byte gUnk_0862C828
_0805E71C:
	ldr r1, _0805E768 @ =0x08625460
_0805E71E:
	mov r0, #1
	mov r2, #0x2C
	mov r3, #0xE0
	bl sub_0805E054
	mov r0, #1
	mov r1, #0x10
	mov r2, #1
	mov r3, #0x2C
	bl sub_0805E1D0
_0805E734:
	ldr r0, _0805E76C @ =0x02018450
	ldr r1, _0805E770 @ =0x0000015D
	add r0, r0, r1
	mov r4, #0
	strb r4, [r0]
	ldr r3, _0805E774 @ =0x04000208
	strh r4, [r3]
	ldr r2, _0805E778 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _0805E77C @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	ldr r1, _0805E780 @ =0x03000000
	ldr r0, _0805E784 @ =0x0805DC39
	str r0, [r1, #4]
	mov r5, #1
	strh r5, [r3]
	strh r4, [r3]
	ldrh r0, [r2]
	mov r1, #2
	orr r0, r1
	strh r0, [r2]
	strh r5, [r3]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0805E768: .4byte gUnk_08625460
_0805E76C: .4byte 0x02018450
_0805E770: .4byte 0x0000015D
_0805E774: .4byte 0x04000208
_0805E778: .4byte 0x04000200
_0805E77C: .4byte 0x0000FFFD
_0805E780: .4byte 0x03000000
_0805E784: .4byte sub_0805DC38
	thumb_func_end sub_0805E3B8

