	thumb_func_start sub_080064AC
sub_080064AC: @ 0x080064AC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r1, _080064EC @ =0x02013D90
	mov r0, #1
	ldrb r2, [r1]
	and r0, r2
	mov r2, #0
	mov r8, r2
	add r5, r1, #0
	cmp r0, #0
	beq _080064C8
	mov r0, #0x48
	mov r8, r0
_080064C8:
	ldrh r2, [r5, #2]
	ldr r0, _080064F0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _080064F4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08006500
	cmp r0, #0x17
	ble _080064F8
	cmp r0, #0x18
	beq _080064FC
	b _08006500
_080064EC: .4byte 0x02013D90
_080064F0: .4byte 0x000007FF
_080064F4: .4byte gUnk_08621DE0
_080064F8:
	mov r0, #0
	b _08006514
_080064FC:
	mov r0, #0xA
	b _08006514
_08006500:
	ldr r0, _08006590 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r2, _08006594 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08006514:
	add r6, r0, #0
	ldr r4, _08006590 @ =0x000007FF
	add r0, r4, #0
	ldrh r1, [r5, #2]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08006598 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r1, _0800659C @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bhi _08006534
	b _08006862
_08006534:
	add r0, r4, #0
	ldrh r2, [r5, #2]
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08006594 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x17
	bne _08006550
	b _08006862
_08006550:
	cmp r0, #0x17
	bgt _080065A0
	cmp r0, #0x15
	bge _0800655A
	b _080067E8
_0800655A:
	mov r0, r8
	add r0, #0x4C
	mov r1, #0xB0
	lsl r1, r1, #0xD
	orr r0, r1
	mov r2, #0x81
	lsl r2, r2, #5
	mov r1, #0x40
	bl sub_080761F0
	ldrh r1, [r5, #2]
	add r0, r1, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r2, _08006594 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x16
	ble _08006586
	b _080067C8
_08006586:
	cmp r0, #0x15
	bge _0800658C
	b _080067C8
_0800658C:
	b _080067A8
	.align 2, 0
_08006590: .4byte 0x000007FF
_08006594: .4byte gUnk_08621DE0
_08006598: .4byte gUnk_08622AB4
_0800659C: .4byte 0xFFFFF880
_080065A0:
	cmp r0, #0x18
	beq _080065A6
	b _080067E8
_080065A6:
	mov r6, r8
	add r6, #0x4C
	mov r7, #0x98
	lsl r7, r7, #0xE
	mov r4, r8
	add r4, #0x54
	mov r5, #9
_080065B4:
	add r0, r4, #0
	orr r0, r7
	mov r1, #0
	mov r2, #2
	bl sub_080761F0
	sub r4, #8
	sub r5, #1
	cmp r5, #0
	bge _080065B4
	mov r0, #0xB0
	lsl r0, r0, #0xD
	orr r6, r0
	mov r2, #0x81
	lsl r2, r2, #5
	add r0, r6, #0
	mov r1, #0x40
	bl sub_080761F0
	ldr r1, _080065FC @ =0x02013D90
	ldr r0, _08006600 @ =0x000007FF
	ldrh r1, [r1, #2]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08006604 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08006608 @ =0x00000777
	cmp r1, r0
	beq _08006690
	cmp r1, r0
	bgt _0800660C
	sub r0, #1
	cmp r1, r0
	beq _08006616
	b _08006862
_080065FC: .4byte 0x02013D90
_08006600: .4byte 0x000007FF
_08006604: .4byte gUnk_08622AB4
_08006608: .4byte 0x00000777
_0800660C:
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	beq _08006708
	b _08006862
_08006616:
	ldr r0, _08006664 @ =0x0086003F
	mov r6, #0x80
	lsl r6, r6, #7
	ldr r2, _08006668 @ =0x0000303A
	add r1, r6, #0
	bl sub_080761F0
	ldr r0, _0800666C @ =0x0086004B
	ldr r5, _08006670 @ =0x00003034
	mov r1, #0
	add r2, r5, #0
	bl sub_080761F0
	ldr r0, _08006674 @ =0x0086004F
	ldr r4, _08006678 @ =0x00003030
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _0800667C @ =0x00860053
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _08006680 @ =0x00860057
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _08006684 @ =0x008E003F
	ldr r2, _08006688 @ =0x0000303C
	add r1, r6, #0
	bl sub_080761F0
	ldr r0, _0800668C @ =0x008E004B
	mov r1, #0
	add r2, r5, #0
	b _08006750
	.align 2, 0
_08006664: .4byte 0x0086003F
_08006668: .4byte 0x0000303A
_0800666C: .4byte 0x0086004B
_08006670: .4byte 0x00003034
_08006674: .4byte 0x0086004F
_08006678: .4byte 0x00003030
_0800667C: .4byte 0x00860053
_08006680: .4byte 0x00860057
_08006684: .4byte 0x008E003F
_08006688: .4byte 0x0000303C
_0800668C: .4byte 0x008E004B
_08006690:
	ldr r0, _080066DC @ =0x0086003F
	mov r6, #0x80
	lsl r6, r6, #7
	ldr r2, _080066E0 @ =0x0000303A
	add r1, r6, #0
	bl sub_080761F0
	ldr r0, _080066E4 @ =0x0086004B
	ldr r5, _080066E8 @ =0x0000303F
	mov r1, #0
	add r2, r5, #0
	bl sub_080761F0
	ldr r0, _080066EC @ =0x0086004F
	ldr r4, _080066F0 @ =0x00003030
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _080066F4 @ =0x00860053
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _080066F8 @ =0x00860057
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _080066FC @ =0x008E003F
	ldr r2, _08006700 @ =0x0000303C
	add r1, r6, #0
	bl sub_080761F0
	ldr r0, _08006704 @ =0x008E004B
	mov r1, #0
	add r2, r5, #0
	b _08006750
_080066DC: .4byte 0x0086003F
_080066E0: .4byte 0x0000303A
_080066E4: .4byte 0x0086004B
_080066E8: .4byte 0x0000303F
_080066EC: .4byte 0x0086004F
_080066F0: .4byte 0x00003030
_080066F4: .4byte 0x00860053
_080066F8: .4byte 0x00860057
_080066FC: .4byte 0x008E003F
_08006700: .4byte 0x0000303C
_08006704: .4byte 0x008E004B
_08006708:
	ldr r0, _08006774 @ =0x0086003F
	mov r5, #0x80
	lsl r5, r5, #7
	ldr r2, _08006778 @ =0x0000303A
	add r1, r5, #0
	bl sub_080761F0
	ldr r0, _0800677C @ =0x0086004B
	ldr r4, _08006780 @ =0x0000303E
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _08006784 @ =0x0086004F
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _08006788 @ =0x00860053
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _0800678C @ =0x00860057
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _08006790 @ =0x008E003F
	ldr r2, _08006794 @ =0x0000303C
	add r1, r5, #0
	bl sub_080761F0
	ldr r0, _08006798 @ =0x008E004B
	mov r1, #0
	add r2, r4, #0
_08006750:
	bl sub_080761F0
	ldr r0, _0800679C @ =0x008E004F
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _080067A0 @ =0x008E0053
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	ldr r0, _080067A4 @ =0x008E0057
	mov r1, #0
	add r2, r4, #0
	bl sub_080761F0
	b _08006862
_08006774: .4byte 0x0086003F
_08006778: .4byte 0x0000303A
_0800677C: .4byte 0x0086004B
_08006780: .4byte 0x0000303E
_08006784: .4byte 0x0086004F
_08006788: .4byte 0x00860053
_0800678C: .4byte 0x00860057
_08006790: .4byte 0x008E003F
_08006794: .4byte 0x0000303C
_08006798: .4byte 0x008E004B
_0800679C: .4byte 0x008E004F
_080067A0: .4byte 0x008E0053
_080067A4: .4byte 0x008E0057
_080067A8:
	ldr r0, _080067C0 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #2
	ldr r2, _080067C4 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xE0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x11
	b _080067CA
	.align 2, 0
_080067C0: .4byte 0x000007FF
_080067C4: .4byte gUnk_08621DE0
_080067C8:
	mov r0, #0
_080067CA:
	cmp r0, #0
	beq _08006862
	mov r0, r8
	add r0, #0x50
	mov r1, #0x90
	lsl r1, r1, #0xE
	orr r0, r1
	ldr r2, _080067E4 @ =0x00002024
	mov r1, #0
	bl sub_080761F0
	b _08006862
	.align 2, 0
_080067E4: .4byte 0x00002024
_080067E8:
	mov r5, #0
	cmp r5, r6
	bge _0800682C
	mov r7, #0x98
	lsl r7, r7, #0xE
	mov r4, r8
	add r4, #0x54
_080067F6:
	cmp r6, #9
	bgt _08006808
	add r0, r4, #0
	orr r0, r7
	mov r1, #0
	mov r2, #2
	bl sub_080761F0
	b _08006824
_08006808:
	mov r0, #0x4E
	mul r0, r5
	add r1, r6, #0
	bl __divsi3
	add r1, r0, #0
	mov r0, #0x54
	sub r0, r0, r1
	add r0, r8
	orr r0, r7
	mov r1, #0
	mov r2, #2
	bl sub_080761F0
_08006824:
	sub r4, #8
	add r5, #1
	cmp r5, r6
	blt _080067F6
_0800682C:
	ldr r0, _0800686C @ =0x02013D90
	ldr r1, _08006870 @ =0x000007FF
	ldrh r0, [r0, #2]
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08006874 @ =0x08621DE0
	add r1, r1, r0
	ldr r0, [r1]
	lsr r0, r0, #0x1D
	cmp r0, #0
	beq _0800685A
	cmp r0, #6
	bhi _0800685A
	mov r0, r8
	add r0, #0x4C
	mov r1, #0xB0
	lsl r1, r1, #0xD
	orr r0, r1
	mov r2, #0x81
	lsl r2, r2, #5
	mov r1, #0x40
	bl sub_080761F0
_0800685A:
	bl sub_0800642C
	bl sub_0800646C
_08006862:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800686C: .4byte 0x02013D90
_08006870: .4byte 0x000007FF
_08006874: .4byte gUnk_08621DE0
	thumb_func_end sub_080064AC

