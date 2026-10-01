	thumb_func_start sub_08062604
sub_08062604: @ 0x08062604
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov sl, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	mov r4, #7
	ldr r0, _080626D0 @ =0x02015160
	mov r2, #0x89
	lsl r2, r2, #1
	add r1, r0, r2
	ldr r0, _080626D4 @ =0x000007FF
	ldr r3, [sp, #0]
	and r0, r3
	lsl r0, r0, #1
	ldr r6, _080626D8 @ =0x08622AB4
	add r0, r0, r6
	ldrh r1, [r1]
	ldrh r0, [r0]
	cmp r1, r0
	bne _08062638
	mov r4, #0xF
_08062638:
	mov r0, #0x18
	mov r1, #2
	bl sub_08074B08
	mov r1, #0xA0
	lsl r1, r1, #4
	add r0, r1, #0
	orr r4, r0
	ldr r2, [sp, #0]
	lsl r3, r2, #6
	ldr r6, _080626DC @ =0x0822C720
	add r3, r3, r6
	mov r0, #2
	mov r1, #6
	add r2, r4, #0
	bl sub_0807501C
	mov r0, sl
	lsl r4, r0, #1
	add r4, sl
	lsl r4, r4, #4
	lsl r0, r4, #5
	ldr r1, _080626E0 @ =0x06004800
	add r0, r0, r1
	mov r1, #0
	bl sub_08075114
	mov r6, #0
	mov r2, sl
	lsl r7, r2, #2
	mov r9, r7
	ldr r3, _080626E4 @ =0x0300045C
	mov ip, r3
	mov r8, r4
_0806267C:
	lsl r1, r6, #1
	add r5, r6, #1
	mov r2, r9
	add r0, r2, r6
	lsl r0, r0, #5
	mov r3, r8
	add r3, #0x40
	add r0, #3
	add r1, r1, r6
	lsl r1, r1, #3
	mov r4, #0x17
	lsl r0, r0, #1
	mov r6, ip
	add r2, r0, r6
_08062698:
	add r0, r3, r1
	strh r0, [r2]
	add r3, #1
	add r2, #2
	sub r4, #1
	cmp r4, #0
	bge _08062698
	add r6, r5, #0
	cmp r6, #1
	ble _0806267C
	ldr r0, _080626D4 @ =0x000007FF
	ldr r1, [sp, #0]
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _080626E8 @ =0x08621DE0
	add r4, r0, r2
	ldr r0, [r4]
	mov r5, #0xF8
	lsl r5, r5, #0x11
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08062778
	cmp r0, #0x16
	bgt _080626EC
	cmp r0, #0x15
	beq _080626F4
	b _08062814
_080626D0: .4byte 0x02015160
_080626D4: .4byte 0x000007FF
_080626D8: .4byte gUnk_08622AB4
_080626DC: .4byte gUnk_0822C720
_080626E0: .4byte 0x06004800
_080626E4: .4byte 0x0300045C
_080626E8: .4byte gUnk_08621DE0
_080626EC:
	cmp r0, #0x18
	bne _080626F2
	b _080629DE
_080626F2:
	b _08062814
_080626F4:
	add r0, r7, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	add r0, #3
	mov r1, sl
	add r1, #5
	lsl r1, r1, #4
	mov r3, #0xC0
	lsl r3, r3, #2
	add r2, r7, r3
	ldr r3, _08062728 @ =0x08636CD8
	bl sub_0807326C
	ldr r1, [r4]
	add r0, r1, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0806272C
	cmp r0, #0x15
	blt _0806272C
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0806272E
_08062728: .4byte gUnk_08636CD8
_0806272C:
	mov r0, #0
_0806272E:
	cmp r0, #0
	bne _08062734
	b _080629DE
_08062734:
	add r0, r7, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	add r4, r0, #5
	mov r0, sl
	add r0, #0xA
	lsl r1, r0, #4
	mov r6, #0xC8
	lsl r6, r6, #2
	add r2, r7, r6
	ldr r5, _0806276C @ =0x081989D0
	ldr r0, _08062770 @ =0x000007FF
	ldr r3, [sp, #0]
	and r0, r3
	lsl r0, r0, #2
	ldr r6, _08062774 @ =0x08621DE0
	add r0, r0, r6
	ldr r3, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08062804
	cmp r0, #0x15
	bge _080627EC
	b _08062804
	.align 2, 0
_0806276C: .4byte gUnk_081989D0
_08062770: .4byte 0x000007FF
_08062774: .4byte gUnk_08621DE0
_08062778:
	add r0, r7, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	add r0, #3
	mov r1, sl
	add r1, #5
	lsl r1, r1, #4
	mov r3, #0xC0
	lsl r3, r3, #2
	add r2, r7, r3
	ldr r3, _080627AC @ =0x08636DA0
	bl sub_0807326C
	ldr r1, [r4]
	add r0, r1, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _080627B0
	cmp r0, #0x15
	blt _080627B0
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _080627B2
_080627AC: .4byte gUnk_08636DA0
_080627B0:
	mov r0, #0
_080627B2:
	cmp r0, #0
	bne _080627B8
	b _080629DE
_080627B8:
	add r0, r7, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0xB
	add r4, r0, #5
	mov r0, sl
	add r0, #0xA
	lsl r1, r0, #4
	mov r6, #0xC8
	lsl r6, r6, #2
	add r2, r7, r6
	ldr r5, _080627F8 @ =0x081989D0
	ldr r0, _080627FC @ =0x000007FF
	ldr r3, [sp, #0]
	and r0, r3
	lsl r0, r0, #2
	ldr r6, _08062800 @ =0x08621DE0
	add r0, r0, r6
	ldr r3, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08062804
	cmp r0, #0x15
	blt _08062804
_080627EC:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r3, r0
	lsr r0, r3, #0x11
	b _08062806
	.align 2, 0
_080627F8: .4byte gUnk_081989D0
_080627FC: .4byte 0x000007FF
_08062800: .4byte gUnk_08621DE0
_08062804:
	mov r0, #0
_08062806:
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r3, [r0]
	add r0, r4, #0
	bl sub_0807326C
	b _080629DE
_08062814:
	add r0, r7, #2
	str r0, [sp, #4]
	lsl r4, r0, #0x10
	lsr r4, r4, #0xB
	add r0, r4, #3
	mov r1, sl
	add r1, #5
	lsl r1, r1, #4
	mov r3, #0xC0
	lsl r3, r3, #2
	add r2, r7, r3
	ldr r6, _080628BC @ =0x081989A8
	mov r8, r6
	ldr r5, _080628C0 @ =0x000007FF
	ldr r3, [sp, #0]
	and r5, r3
	lsl r5, r5, #2
	ldr r6, _080628C4 @ =0x08621DE0
	add r5, r5, r6
	ldr r3, [r5]
	lsr r3, r3, #0x1D
	lsl r3, r3, #2
	add r3, r8
	ldr r3, [r3]
	bl sub_0807326C
	add r0, r4, #5
	mov r1, sl
	add r1, #0xA
	lsl r1, r1, #4
	mov r3, #0xC8
	lsl r3, r3, #2
	add r2, r7, r3
	ldr r6, _080628C8 @ =0x081989EC
	mov r8, r6
	ldr r3, [r5]
	mov r6, #0xF8
	lsl r6, r6, #0x11
	mov r9, r6
	and r3, r6
	lsr r3, r3, #0x12
	add r3, r8
	ldr r3, [r3]
	bl sub_0807326C
	add r4, #8
	mov r2, #0xD0
	lsl r2, r2, #2
	ldr r3, _080628CC @ =0x0863CA1C
	add r0, r4, #0
	mov r1, #0xF0
	bl sub_0807326C
	ldr r0, [sp, #4]
	lsl r6, r0, #5
	add r6, #9
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	mov r0, #0xE0
	lsl r0, r0, #0xB
	orr r6, r0
	mov r2, sl
	lsl r1, r2, #3
	mov r3, #0xD0
	lsl r3, r3, #1
	add r0, r1, r3
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r0, #0x80
	lsl r0, r0, #0xB
	orr r3, r0
	ldr r0, [r5]
	mov r2, r9
	and r0, r2
	lsr r0, r0, #0x14
	add r4, r1, #0
	cmp r0, #0x15
	blt _080628DA
	cmp r0, #0x17
	ble _080628D0
	cmp r0, #0x18
	beq _080628D4
	b _080628DA
	.align 2, 0
_080628BC: .4byte gUnk_081989A8
_080628C0: .4byte 0x000007FF
_080628C4: .4byte gUnk_08621DE0
_080628C8: .4byte gUnk_081989EC
_080628CC: .4byte gUnk_0863CA1C
_080628D0:
	mov r0, #0
	b _080628F2
_080628D4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080628F2
_080628DA:
	ldr r0, _08062944 @ =0x000007FF
	ldr r1, [sp, #0]
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08062948 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080628F2:
	add r2, r0, #0
	add r0, r6, #0
	add r1, r3, #0
	mov r3, #0
	bl sub_08072C0C
	add r0, r7, #3
	lsl r0, r0, #5
	add r0, #9
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r0, #0xE0
	lsl r0, r0, #0xB
	orr r3, r0
	mov r6, #0xE4
	lsl r6, r6, #1
	add r0, r4, r6
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r0, #0x80
	lsl r0, r0, #0xB
	orr r4, r0
	ldr r0, _08062944 @ =0x000007FF
	ldr r1, [sp, #0]
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08062948 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08062956
	cmp r0, #0x17
	ble _0806294C
	cmp r0, #0x18
	beq _08062950
	b _08062956
	.align 2, 0
_08062944: .4byte 0x000007FF
_08062948: .4byte gUnk_08621DE0
_0806294C:
	mov r0, #0
	b _0806296E
_08062950:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0806296E
_08062956:
	ldr r0, _0806298C @ =0x000007FF
	ldr r6, [sp, #0]
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08062990 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08062994 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0806296E:
	add r2, r0, #0
	add r0, r3, #0
	add r1, r4, #0
	mov r3, #0
	bl sub_08072C0C
	mov r6, #0
	ldr r2, _0806298C @ =0x000007FF
	add r1, r2, #0
	ldr r0, [sp, #0]
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08062990 @ =0x08621DE0
	add r5, r0, r3
	b _080629B0
_0806298C: .4byte 0x000007FF
_08062990: .4byte gUnk_08621DE0
_08062994: .4byte 0x000001FF
_08062998:
	add r1, r6, #0
	add r1, #0xE
	add r0, r7, #2
	lsl r0, r0, #5
	add r1, r1, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, #0
	mov r2, #2
	bl sub_08072E98
	add r6, #1
_080629B0:
	ldr r0, [r5]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080629D0
	cmp r0, #0x17
	ble _080629C8
	cmp r0, #0x18
	beq _080629CC
	b _080629D0
_080629C8:
	mov r0, #0
	b _080629DA
_080629CC:
	mov r0, #0xA
	b _080629DA
_080629D0:
	ldr r0, [r5]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080629DA:
	cmp r6, r0
	blt _08062998
_080629DE:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08062604
	.align 2, 0

