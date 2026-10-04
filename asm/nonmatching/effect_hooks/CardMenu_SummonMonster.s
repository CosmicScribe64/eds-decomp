	thumb_func_start CardMenu_SummonMonster
CardMenu_SummonMonster: @ 0x080471E8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x118
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r1, r1, #0x10
	lsr r3, r1, #0x10
	ldr r1, _08047220 @ =0x020192E0
	ldr r2, _08047224 @ =0x00001B30
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x18
	add r6, r1, #0
	cmp r0, #0x54
	bls _08047214
	bl _08048FBC @ far jump
_08047214:
	lsl r0, r0, #2
	ldr r1, _08047228 @ =0x0804722C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08047220: .4byte 0x020192E0
_08047224: .4byte 0x00001B30
_08047228: .4byte 0x0804722C
_0804722C:
	.4byte _08047380
	.4byte _080479A0
	.4byte _080479DA
	.4byte _08047A0C
	.4byte _08048E4C
	.4byte _08047AE4
	.4byte _08047BE4
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08047C84
	.4byte _08047CC0
	.4byte _08047DA0
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08047DEC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08047EF4
	.4byte _08047F80
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _080480B4
	.4byte _0804836C
	.4byte _080484B4
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _080484FC
	.4byte _08048584
	.4byte _08048624
	.4byte _08048678
	.4byte _08048584
	.4byte _08048624
	.4byte _0804869C
	.4byte _08048584
	.4byte _080486D0
	.4byte _08048FBC
	.4byte _08048730
	.4byte _080487B8
	.4byte _08048850
	.4byte _080488A4
	.4byte _080487B8
	.4byte _080488C4
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048924
	.4byte _08048AAC
	.4byte _08048BAC
	.4byte _08048C00
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048FBC
	.4byte _08048C44
	.4byte _08048D7C
	.4byte _08048E4C
	.4byte _08048E90
	.4byte _08048F5C
_08047380:
	ldr r4, _080473B4 @ =0x00001B28
	add r2, r6, r4
	ldr r0, _080473B8 @ =0x000007FF
	ldrh r5, [r2]
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _080473BC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080473C0 @ =0x000004E2
	cmp r1, r0
	bne _0804739A
	b _080475B8
_0804739A:
	cmp r1, r0
	bgt _080473F0
	cmp r1, #0x42
	beq _08047470
	cmp r1, #0x42
	bgt _080473C4
	cmp r1, #0x38
	ble _080473AC
	b _0804776C
_080473AC:
	cmp r1, #0x37
	bge _080473B2
	b _0804776C
_080473B2:
	b _08047470
_080473B4: .4byte 0x00001B28
_080473B8: .4byte 0x000007FF
_080473BC: .4byte gCardIdToNumber
_080473C0: .4byte 0x000004E2
_080473C4:
	ldr r0, _080473DC @ =0x00000175
	cmp r1, r0
	bne _080473CC
	b _08047530
_080473CC:
	cmp r1, r0
	bgt _080473E0
	sub r0, #5
	cmp r1, r0
	bne _080473D8
	b _080474D4
_080473D8:
	b _0804776C
	.align 2, 0
_080473DC: .4byte 0x00000175
_080473E0:
	ldr r0, _080473EC @ =0x0000034D
	cmp r1, r0
	bne _080473E8
	b _08047574
_080473E8:
	b _0804776C
	.align 2, 0
_080473EC: .4byte 0x0000034D
_080473F0:
	ldr r0, _08047410 @ =0x000005EA
	cmp r1, r0
	bne _080473F8
	b _08047690
_080473F8:
	cmp r1, r0
	bgt _08047418
	ldr r0, _08047414 @ =0x000004E9
	cmp r1, r0
	bne _08047404
	b _0804764C
_08047404:
	add r0, #0x5D
	cmp r1, r0
	bne _0804740C
	b _08047714
_0804740C:
	b _0804776C
	.align 2, 0
_08047410: .4byte 0x000005EA
_08047414: .4byte 0x000004E9
_08047418:
	ldr r0, _08047458 @ =0x000005EB
	cmp r1, r0
	bne _08047420
	b _080476D0
_08047420:
	add r0, #4
	cmp r1, r0
	ble _08047428
	b _0804776C
_08047428:
	ldr r2, _0804745C @ =0x00001B2A
	add r1, r6, r2
	mov r0, #0
	strh r0, [r1]
	ldr r3, _08047460 @ =0x00001B31
	add r1, r6, r3
	sub r0, #0x3D
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r5, _08047464 @ =0x00001B30
	add r2, r6, r5
	ldr r0, [r2]
	ldr r1, _08047468 @ =0xFFFC3FFF
	and r0, r1
	str r0, [r2]
	ldr r0, _0804746C @ =0xFFFFFC03
	ldrh r1, [r2]
	and r0, r1
	mov r3, #0x8C
	lsl r3, r3, #1
	add r1, r3, #0
	bl _08048E72 @ far jump
_08047458: .4byte 0x000005EB
_0804745C: .4byte 0x00001B2A
_08047460: .4byte 0x00001B31
_08047464: .4byte 0x00001B30
_08047468: .4byte 0xFFFC3FFF
_0804746C: .4byte 0xFFFFFC03
_08047470:
	add r5, sp, #0x84
	ldr r1, _080474B4 @ =0x08085604
	ldr r4, _080474B8 @ =0x00001B28
	add r0, r6, r4
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r4, _080474BC @ =0x0822C720
	add r2, r2, r4
	add r0, r5, #0
	bl FormatStr
	ldr r0, _080474C0 @ =0x0862401E
	ldrh r0, [r0]
	lsl r2, r0, #6
	add r2, r2, r4
	add r0, sp, #4
	add r1, r5, #0
	bl FormatStr
	ldr r0, _080474C4 @ =0x00000206
	ldr r1, _080474C8 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r5, _080474CC @ =0x00001B30
	add r2, r6, r5
	ldr r0, _080474D0 @ =0xFFFFFC03
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x50
	bl _08048E72 @ far jump
	.align 2, 0
_080474B4: .4byte gStrSpecialSummonSelectTribute
_080474B8: .4byte 0x00001B28
_080474BC: .4byte gCardNames
_080474C0: .4byte gCardNumberToId_PetitMoth
_080474C4: .4byte 0x00000206
_080474C8: .4byte 0x00000712
_080474CC: .4byte 0x00001B30
_080474D0: .4byte 0xFFFFFC03
_080474D4:
	add r5, sp, #0x84
	ldr r1, _08047514 @ =0x08085604
	ldrh r2, [r2]
	lsl r2, r2, #6
	ldr r4, _08047518 @ =0x0822C720
	add r2, r2, r4
	add r0, r5, #0
	bl FormatStr
	ldr r0, _0804751C @ =0x086240CE
	ldrh r0, [r0]
	lsl r2, r0, #6
	add r2, r2, r4
	add r0, sp, #4
	add r1, r5, #0
	bl FormatStr
	ldr r0, _08047520 @ =0x00000206
	ldr r1, _08047524 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r3, _08047528 @ =0x00001B30
	add r2, r6, r3
	ldr r0, _0804752C @ =0xFFFFFC03
	ldrh r4, [r2]
	and r0, r4
	mov r1, #0x50
	bl _08048E72 @ far jump
	.align 2, 0
_08047514: .4byte gStrSpecialSummonSelectTribute
_08047518: .4byte gCardNames
_0804751C: .4byte gCardNumberToId_LabyrinthWall
_08047520: .4byte 0x00000206
_08047524: .4byte 0x00000712
_08047528: .4byte 0x00001B30
_0804752C: .4byte 0xFFFFFC03
_08047530:
	ldr r5, _08047560 @ =0x00001B2A
	add r1, r6, r5
	mov r0, #0
	strh r0, [r1]
	ldr r0, _08047564 @ =0x00001B31
	add r1, r6, r0
	mov r0, #0x3D
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _08047568 @ =0x00001B30
	add r2, r6, r3
	ldr r0, [r2]
	ldr r1, _0804756C @ =0xFFFC3FFF
	and r0, r1
	str r0, [r2]
	ldr r0, _08047570 @ =0xFFFFFC03
	ldrh r4, [r2]
	and r0, r4
	mov r1, #0x78
	bl _08048E72 @ far jump
	.align 2, 0
_08047560: .4byte 0x00001B2A
_08047564: .4byte 0x00001B31
_08047568: .4byte 0x00001B30
_0804756C: .4byte 0xFFFC3FFF
_08047570: .4byte 0xFFFFFC03
_08047574:
	ldr r5, _080475A4 @ =0x00001B2A
	add r1, r6, r5
	mov r0, #0
	strh r0, [r1]
	ldr r0, _080475A8 @ =0x00001B31
	add r1, r6, r0
	mov r0, #0x3D
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _080475AC @ =0x00001B30
	add r2, r6, r3
	ldr r0, [r2]
	ldr r1, _080475B0 @ =0xFFFC3FFF
	and r0, r1
	str r0, [r2]
	ldr r0, _080475B4 @ =0xFFFFFC03
	ldrh r4, [r2]
	and r0, r4
	mov r1, #0xA0
	bl _08048E72 @ far jump
	.align 2, 0
_080475A4: .4byte 0x00001B2A
_080475A8: .4byte 0x00001B31
_080475AC: .4byte 0x00001B30
_080475B0: .4byte 0xFFFC3FFF
_080475B4: .4byte 0xFFFFFC03
_080475B8:
	ldr r0, _08047600 @ =0x00001B33
	add r5, r6, r0
	ldrb r1, [r5]
	lsl r2, r1, #0x1E
	lsr r1, r2, #0x1F
	ldr r0, _08047604 @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldrb r4, [r0, #6]
	cmp r4, #1
	bne _08047610
	add r0, r1, #0
	bl FindFreeMonsterZone
	add r2, r0, #0
	ldrb r5, [r5]
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08047608 @ =0x00001B34
	add r1, r6, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	str r4, [sp, #0]
	mov r3, #0
	bl QueueNormalSummon
	ldr r4, _0804760C @ =0x00001B2C
	add r1, r6, r4
	mov r0, #3
	neg r0, r0
	ldrb r5, [r1]
	and r0, r5
	bl _08048FC8 @ far jump
	.align 2, 0
_08047600: .4byte 0x00001B33
_08047604: .4byte 0x00000D64
_08047608: .4byte 0x00001B34
_0804760C: .4byte 0x00001B2C
_08047610:
	ldr r0, _08047638 @ =0x00000206
	ldr r1, _0804763C @ =0x00000712
	ldr r2, _08047640 @ =0x0819D1C4
	ldr r3, [r2, #4]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r0, _08047644 @ =0x00001B2A
	add r1, r6, r0
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08047648 @ =0x00001B30
	add r3, r6, r1
	bl _08048F1C @ far jump
_08047638: .4byte 0x00000206
_0804763C: .4byte 0x00000712
_08047640: .4byte gTributeSummonPrompts
_08047644: .4byte 0x00001B2A
_08047648: .4byte 0x00001B30
_0804764C:
	ldr r2, _0804767C @ =0x00001B2A
	add r1, r6, r2
	mov r0, #0
	strh r0, [r1]
	ldr r3, _08047680 @ =0x00001B31
	add r1, r6, r3
	sub r0, #0x3D
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r5, _08047684 @ =0x00001B30
	add r2, r6, r5
	ldr r0, [r2]
	ldr r1, _08047688 @ =0xFFFC3FFF
	and r0, r1
	str r0, [r2]
	ldr r0, _0804768C @ =0xFFFFFC03
	ldrh r1, [r2]
	and r0, r1
	mov r3, #0xA0
	lsl r3, r3, #1
	add r1, r3, #0
	bl _08048E72 @ far jump
_0804767C: .4byte 0x00001B2A
_08047680: .4byte 0x00001B31
_08047684: .4byte 0x00001B30
_08047688: .4byte 0xFFFC3FFF
_0804768C: .4byte 0xFFFFFC03
_08047690:
	ldr r4, _080476BC @ =0x00001B2A
	add r1, r6, r4
	mov r0, #0
	strh r0, [r1]
	ldr r5, _080476C0 @ =0x00001B31
	add r1, r6, r5
	sub r0, #0x3D
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _080476C4 @ =0x00001B30
	add r2, r6, r3
	ldr r0, [r2]
	ldr r1, _080476C8 @ =0xFFFC3FFF
	and r0, r1
	str r0, [r2]
	ldr r0, _080476CC @ =0xFFFFFC03
	ldrh r4, [r2]
	and r0, r4
	mov r1, #0xC8
	bl _08048E72 @ far jump
_080476BC: .4byte 0x00001B2A
_080476C0: .4byte 0x00001B31
_080476C4: .4byte 0x00001B30
_080476C8: .4byte 0xFFFC3FFF
_080476CC: .4byte 0xFFFFFC03
_080476D0:
	ldr r5, _08047700 @ =0x00001B2A
	add r1, r6, r5
	mov r0, #0
	strh r0, [r1]
	ldr r0, _08047704 @ =0x00001B31
	add r1, r6, r0
	mov r0, #0x3D
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _08047708 @ =0x00001B30
	add r2, r6, r3
	ldr r0, [r2]
	ldr r1, _0804770C @ =0xFFFC3FFF
	and r0, r1
	str r0, [r2]
	ldr r0, _08047710 @ =0xFFFFFC03
	ldrh r4, [r2]
	and r0, r4
	mov r1, #0xF0
	bl _08048E72 @ far jump
	.align 2, 0
_08047700: .4byte 0x00001B2A
_08047704: .4byte 0x00001B31
_08047708: .4byte 0x00001B30
_0804770C: .4byte 0xFFFC3FFF
_08047710: .4byte 0xFFFFFC03
_08047714:
	ldr r0, _08047760 @ =0x00001B2C
	add r5, r6, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #0xC
	bgt _0804776C
	cmp r0, #0xB
	blt _0804776C
	ldr r2, _08047764 @ =0x00001B33
	add r4, r6, r2
	ldrb r3, [r4]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	add r2, r0, #0
	ldrb r4, [r4]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	ldr r4, _08047768 @ =0x00001B34
	add r1, r6, r4
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r3, r9
	str r3, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	mov r0, #3
	neg r0, r0
	ldrb r4, [r5]
	and r0, r4
	strb r0, [r5]
	bl _08048FCA @ far jump
	.align 2, 0
_08047760: .4byte 0x00001B2C
_08047764: .4byte 0x00001B33
_08047768: .4byte 0x00001B34
_0804776C:
	ldr r5, _08047794 @ =0x00001B28
	add r0, r6, r5
	ldrh r2, [r0]
	ldr r0, _08047798 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0804779C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080477A8
	cmp r0, #0x17
	ble _080477A0
	cmp r0, #0x18
	beq _080477A4
	b _080477A8
_08047794: .4byte 0x00001B28
_08047798: .4byte 0x000007FF
_0804779C: .4byte gCardStats
_080477A0:
	mov r0, #0
	b _080477BC
_080477A4:
	mov r0, #0xA
	b _080477BC
_080477A8:
	ldr r0, _08047800 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r2, _08047804 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080477BC:
	cmp r0, #0
	bge _080477C2
	b _0804795C
_080477C2:
	cmp r0, #4
	ble _08047824
	cmp r0, #6
	ble _080477CC
	b _0804795C
_080477CC:
	ldr r0, _08047808 @ =0x00000206
	ldr r1, _0804780C @ =0x00000712
	ldr r2, _08047810 @ =0x0819D1C4
	ldr r3, [r2]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r2, _08047814 @ =0x020192E0
	ldr r3, _08047818 @ =0x00001B2A
	add r1, r2, r3
	mov r0, #0
	strh r0, [r1]
	ldr r4, _0804781C @ =0x00001B30
	add r2, r2, r4
	ldr r0, _08047820 @ =0xFFFFFC03
	ldrh r5, [r2]
	and r0, r5
	mov r1, #0x28
	bl _08048E72 @ far jump
	.align 2, 0
_08047800: .4byte 0x000007FF
_08047804: .4byte gCardStats
_08047808: .4byte 0x00000206
_0804780C: .4byte 0x00000712
_08047810: .4byte gTributeSummonPrompts
_08047814: .4byte 0x020192E0
_08047818: .4byte 0x00001B2A
_0804781C: .4byte 0x00001B30
_08047820: .4byte 0xFFFFFC03
_08047824:
	cmp r3, #0
	beq _08047864
	ldr r4, _08047858 @ =0x020192E0
	ldr r0, _0804785C @ =0x00001B33
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	add r2, r0, #0
	ldrb r5, [r5]
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08047860 @ =0x00001B34
	add r4, r4, r3
	ldrh r4, [r4]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x18
	mov r4, r9
	str r4, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	b _08047890
	.align 2, 0
_08047858: .4byte 0x020192E0
_0804785C: .4byte 0x00001B33
_08047860: .4byte 0x00001B34
_08047864:
	ldr r4, _0804793C @ =0x020192E0
	ldr r0, _08047940 @ =0x00001B33
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	add r2, r0, #0
	ldrb r5, [r5]
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08047944 @ =0x00001B34
	add r4, r4, r3
	ldrh r4, [r4]
	lsl r1, r4, #0x17
	lsr r1, r1, #0x18
	mov r4, r9
	str r4, [sp, #0]
	mov r3, #0
	bl QueueNormalSummon
_08047890:
	ldr r6, _0804793C @ =0x020192E0
	ldr r5, _08047948 @ =0x00001B28
	add r7, r6, r5
	ldr r0, _0804794C @ =0x000007FF
	ldrh r1, [r7]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08047950 @ =0x08622AB4
	add r0, r0, r2
	mov r1, #0xBE
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bne _08047928
	ldr r3, _08047954 @ =0x00001B2C
	add r0, r6, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #0xC
	bgt _08047928
	cmp r0, #0xB
	blt _08047928
	ldr r4, _08047940 @ =0x00001B33
	add r5, r6, r4
	ldrb r1, [r5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	mov r4, #1
	sub r0, r4, r0
	ldr r1, _08047958 @ =0x00000447
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	beq _08047928
	ldrb r2, [r5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	sub r0, r4, r0
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _08047928
	ldrb r3, [r5]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	sub r0, r4, r0
	bl CanSpecialSummon
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08047928
	ldrb r5, [r5]
	lsl r1, r5, #0x1E
	lsr r1, r1, #0x1F
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #0x1F
	ldr r4, _08047944 @ =0x00001B34
	add r1, r6, r4
	ldr r2, [r1]
	lsl r2, r2, #0xF
	lsr r2, r2, #0x18
	mov r1, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	mov r2, #0xE4
	lsl r2, r2, #0x14
	orr r1, r2
	orr r0, r1
	ldrh r7, [r7]
	orr r0, r7
	mov r1, #0
	bl Chain_AddPending
_08047928:
	ldr r1, _0804793C @ =0x020192E0
	ldr r5, _08047954 @ =0x00001B2C
	add r1, r1, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	bl _08048FC8 @ far jump
	.align 2, 0
_0804793C: .4byte 0x020192E0
_08047940: .4byte 0x00001B33
_08047944: .4byte 0x00001B34
_08047948: .4byte 0x00001B28
_0804794C: .4byte 0x000007FF
_08047950: .4byte gCardIdToNumber
_08047954: .4byte 0x00001B2C
_08047958: .4byte 0x00000447
_0804795C:
	ldr r0, _08047988 @ =0x00000206
	ldr r1, _0804798C @ =0x00000712
	ldr r2, _08047990 @ =0x0819D1C4
	ldr r3, [r2, #4]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r2, _08047994 @ =0x020192E0
	ldr r3, _08047998 @ =0x00001B2A
	add r1, r2, r3
	mov r0, #0
	strh r0, [r1]
	ldr r4, _0804799C @ =0x00001B30
	add r2, r2, r4
	bl _08048E60 @ far jump
	.align 2, 0
_08047988: .4byte 0x00000206
_0804798C: .4byte 0x00000712
_08047990: .4byte gTributeSummonPrompts
_08047994: .4byte 0x020192E0
_08047998: .4byte 0x00001B2A
_0804799C: .4byte 0x00001B30
_080479A0:
	ldr r0, _080479B8 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _080479C0
	ldr r5, _080479BC @ =0x00001B2C
	add r1, r6, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	bl _08048FC8 @ far jump
_080479B8: .4byte 0x0201AE60
_080479BC: .4byte 0x00001B2C
_080479C0:
	ldr r4, _080479F4 @ =0x00001B30
	add r3, r6, r4
	ldrh r2, [r3]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _080479F8 @ =0xFFFFFC03
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_080479DA:
	ldr r0, _080479FC @ =0x00000206
	ldr r1, _08047A00 @ =0x00000412
	ldr r2, _08047A04 @ =0x0819D1C4
	ldr r3, [r2, #8]
	mov r2, #0xB
	bl TextBoxOpen
	ldr r2, _08047A08 @ =0x020192E0
	ldr r5, _080479F4 @ =0x00001B30
	add r2, r2, r5
	bl _08048E60 @ far jump
	.align 2, 0
_080479F4: .4byte 0x00001B30
_080479F8: .4byte 0xFFFFFC03
_080479FC: .4byte 0x00000206
_08047A00: .4byte 0x00000412
_08047A04: .4byte gTributeSummonPrompts
_08047A08: .4byte 0x020192E0
_08047A0C:
	ldr r1, _08047A4C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08047A6C
	ldr r0, _08047A50 @ =0x00000206
	ldr r1, _08047A54 @ =0x00000712
	ldr r2, _08047A58 @ =0x0819D1C4
	ldr r3, [r2, #4]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r2, _08047A5C @ =0x020192E0
	ldr r0, _08047A60 @ =0x00001B2A
	add r1, r2, r0
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08047A64 @ =0x00001B30
	add r2, r2, r1
	ldr r0, _08047A68 @ =0xFFFFFC03
	ldrh r3, [r2]
	and r0, r3
	mov r1, #4
	bl _08048E72 @ far jump
	.align 2, 0
_08047A4C: .4byte 0x03000040
_08047A50: .4byte 0x00000206
_08047A54: .4byte 0x00000712
_08047A58: .4byte gTributeSummonPrompts
_08047A5C: .4byte 0x020192E0
_08047A60: .4byte 0x00001B2A
_08047A64: .4byte 0x00001B30
_08047A68: .4byte 0xFFFFFC03
_08047A6C:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08047A7A
	bl _08048FCA @ far jump
_08047A7A:
	ldr r7, _08047AC8 @ =0x0201CFB0
	ldr r4, _08047ACC @ =0x00000824
	add r5, r7, r4
	ldr r0, [r5]
	ldr r1, _08047AD0 @ =0x0000082C
	add r6, r7, r1
	ldr r1, [r6]
	bl IsTributableMonster
	cmp r0, #0
	bne _08047A94
	bl _08048F54 @ far jump
_08047A94:
	ldr r4, _08047AD4 @ =0x020192E0
	ldrb r2, [r5]
	lsl r0, r2, #8
	ldrb r3, [r6]
	orr r0, r3
	ldr r2, _08047AD8 @ =0x00001B2A
	add r1, r4, r2
	strh r0, [r1]
	mov r0, #1
	bl PlaySE
	ldrh r1, [r5]
	ldr r3, _08047ADC @ =0x00000828
	add r0, r7, r3
	ldrb r6, [r6]
	lsl r2, r6, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldr r5, _08047AE0 @ =0x00001B30
	add r4, r4, r5
	bl _08048344 @ far jump
_08047AC8: .4byte 0x0201CFB0
_08047ACC: .4byte 0x00000824
_08047AD0: .4byte 0x0000082C
_08047AD4: .4byte 0x020192E0
_08047AD8: .4byte 0x00001B2A
_08047ADC: .4byte 0x00000828
_08047AE0: .4byte 0x00001B30
_08047AE4:
	ldr r1, _08047B24 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08047B44
	ldr r0, _08047B28 @ =0x00000206
	ldr r1, _08047B2C @ =0x00000712
	ldr r2, _08047B30 @ =0x0819D1C4
	ldr r3, [r2, #4]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r2, _08047B34 @ =0x020192E0
	ldr r3, _08047B38 @ =0x00001B2A
	add r1, r2, r3
	mov r0, #0
	strh r0, [r1]
	ldr r4, _08047B3C @ =0x00001B30
	add r2, r2, r4
	ldr r0, _08047B40 @ =0xFFFFFC03
	ldrh r5, [r2]
	and r0, r5
	mov r1, #4
	bl _08048E72 @ far jump
	.align 2, 0
_08047B24: .4byte 0x03000040
_08047B28: .4byte 0x00000206
_08047B2C: .4byte 0x00000712
_08047B30: .4byte gTributeSummonPrompts
_08047B34: .4byte 0x020192E0
_08047B38: .4byte 0x00001B2A
_08047B3C: .4byte 0x00001B30
_08047B40: .4byte 0xFFFFFC03
_08047B44:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08047B52
	bl _08048FCA @ far jump
_08047B52:
	ldr r0, _08047BCC @ =0x020192E0
	mov r8, r0
	ldr r7, _08047BD0 @ =0x00001B2A
	add r7, r8
	ldr r4, _08047BD4 @ =0x0201CFB0
	ldr r1, _08047BD8 @ =0x0000082C
	add r6, r4, r1
	ldr r1, [r6]
	ldr r2, _08047BDC @ =0x00000824
	add r5, r4, r2
	ldr r2, [r5]
	ldrb r3, [r5]
	lsl r0, r3, #8
	ldrb r3, [r6]
	orr r0, r3
	ldrh r3, [r7]
	cmp r3, r0
	bne _08047B7A
	bl _08048F54 @ far jump
_08047B7A:
	add r0, r2, #0
	bl IsTributableMonster
	cmp r0, #0
	bne _08047B88
	bl _08048F54 @ far jump
_08047B88:
	mov r0, #1
	bl PlaySE
	ldrh r1, [r5]
	ldr r2, _08047BE0 @ =0x00000828
	add r0, r4, r2
	ldrb r3, [r6]
	lsl r2, r3, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldrh r0, [r7]
	mov r3, #0xF
	add r2, r3, #0
	and r2, r0
	lsr r0, r0, #8
	and r0, r3
	lsl r0, r0, #4
	orr r2, r0
	ldr r1, [r6]
	and r1, r3
	ldr r0, [r5]
	and r0, r3
	lsl r0, r0, #4
	orr r1, r0
	lsl r1, r1, #8
	orr r2, r1
	strh r2, [r7]
	bl _08048F18 @ far jump
	.align 2, 0
_08047BCC: .4byte 0x020192E0
_08047BD0: .4byte 0x00001B2A
_08047BD4: .4byte 0x0201CFB0
_08047BD8: .4byte 0x0000082C
_08047BDC: .4byte 0x00000824
_08047BE0: .4byte 0x00000828
_08047BE4:
	cmp r3, #0
	beq _08047C2C
	ldr r2, _08047C18 @ =0x020192E0
	ldr r4, _08047C1C @ =0x00001B33
	add r0, r2, r4
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r5, _08047C20 @ =0x00001B34
	add r1, r2, r5
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	ldr r3, _08047C24 @ =0x00001B2A
	add r2, r2, r3
	ldrh r4, [r2]
	mov r2, #7
	and r2, r4
	ldr r5, _08047C28 @ =0x00008080
	add r3, r5, #0
	orr r3, r4
	mov r4, r9
	str r4, [sp, #0]
	bl QueueSpecialSummonFromHand
	b _08047C5A
_08047C18: .4byte 0x020192E0
_08047C1C: .4byte 0x00001B33
_08047C20: .4byte 0x00001B34
_08047C24: .4byte 0x00001B2A
_08047C28: .4byte 0x00008080
_08047C2C:
	ldr r2, _08047C6C @ =0x020192E0
	ldr r5, _08047C70 @ =0x00001B33
	add r0, r2, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08047C74 @ =0x00001B34
	add r1, r2, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	ldr r4, _08047C78 @ =0x00001B2A
	add r2, r2, r4
	ldrh r4, [r2]
	mov r2, #7
	and r2, r4
	ldr r5, _08047C7C @ =0x00008080
	add r3, r5, #0
	orr r3, r4
	mov r4, r9
	str r4, [sp, #0]
	bl QueueNormalSummon
_08047C5A:
	ldr r1, _08047C6C @ =0x020192E0
	ldr r5, _08047C80 @ =0x00001B2C
	add r1, r1, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	bl _08048FC8 @ far jump
_08047C6C: .4byte 0x020192E0
_08047C70: .4byte 0x00001B33
_08047C74: .4byte 0x00001B34
_08047C78: .4byte 0x00001B2A
_08047C7C: .4byte 0x00008080
_08047C80: .4byte 0x00001B2C
_08047C84:
	ldr r0, _08047CA8 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _08047C90
	bl _08048FBC @ far jump
_08047C90:
	ldr r0, _08047CAC @ =0x00000206
	ldr r1, _08047CB0 @ =0x00000412
	ldr r2, _08047CB4 @ =0x0819D1C4
	ldr r3, [r2, #0x10]
	mov r2, #0xB
	bl TextBoxOpen
	ldr r2, _08047CB8 @ =0x020192E0
	ldr r5, _08047CBC @ =0x00001B30
	add r2, r2, r5
	bl _08048E60 @ far jump
_08047CA8: .4byte 0x0201AE60
_08047CAC: .4byte 0x00000206
_08047CB0: .4byte 0x00000412
_08047CB4: .4byte gTributeSummonPrompts
_08047CB8: .4byte 0x020192E0
_08047CBC: .4byte 0x00001B30
_08047CC0:
	ldr r1, _08047D00 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08047D20
	ldr r0, _08047D04 @ =0x00000206
	ldr r1, _08047D08 @ =0x00000712
	ldr r2, _08047D0C @ =0x0819D1C4
	ldr r3, [r2]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r2, _08047D10 @ =0x020192E0
	ldr r0, _08047D14 @ =0x00001B2A
	add r1, r2, r0
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08047D18 @ =0x00001B30
	add r2, r2, r1
	ldr r0, _08047D1C @ =0xFFFFFC03
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0x28
	bl _08048E72 @ far jump
	.align 2, 0
_08047D00: .4byte 0x03000040
_08047D04: .4byte 0x00000206
_08047D08: .4byte 0x00000712
_08047D0C: .4byte gTributeSummonPrompts
_08047D10: .4byte 0x020192E0
_08047D14: .4byte 0x00001B2A
_08047D18: .4byte 0x00001B30
_08047D1C: .4byte 0xFFFFFC03
_08047D20:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08047D2E
	bl _08048FCA @ far jump
_08047D2E:
	ldr r4, _08047D84 @ =0x0201CFB0
	ldr r5, _08047D88 @ =0x00000824
	add r6, r4, r5
	ldr r0, [r6]
	ldr r1, _08047D8C @ =0x0000082C
	add r5, r4, r1
	ldr r1, [r5]
	bl IsTributableMonster
	cmp r0, #0
	bne _08047D48
	bl _08048F54 @ far jump
_08047D48:
	mov r0, #1
	bl PlaySE
	ldrh r1, [r6]
	ldr r2, _08047D90 @ =0x00000828
	add r0, r4, r2
	ldrb r3, [r5]
	lsl r2, r3, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldr r3, _08047D94 @ =0x020192E0
	ldr r1, [r5]
	mov r2, #0xF
	and r1, r2
	ldr r0, [r6]
	and r0, r2
	lsl r0, r0, #4
	orr r1, r0
	ldr r4, _08047D98 @ =0x00001B2A
	add r0, r3, r4
	strh r1, [r0]
	ldr r5, _08047D9C @ =0x00001B30
	add r3, r3, r5
	bl _08048F1C @ far jump
	.align 2, 0
_08047D84: .4byte 0x0201CFB0
_08047D88: .4byte 0x00000824
_08047D8C: .4byte 0x0000082C
_08047D90: .4byte 0x00000828
_08047D94: .4byte 0x020192E0
_08047D98: .4byte 0x00001B2A
_08047D9C: .4byte 0x00001B30
_08047DA0:
	ldr r1, _08047DDC @ =0x00001B33
	add r0, r6, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, _08047DE0 @ =0x00001B34
	add r1, r6, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	ldr r3, _08047DE4 @ =0x00001B2A
	add r2, r6, r3
	ldrh r4, [r2]
	mov r2, #7
	and r2, r4
	mov r3, #0x80
	orr r3, r4
	mov r4, r9
	str r4, [sp, #0]
	bl QueueNormalSummon
	ldr r5, _08047DE8 @ =0x00001B2C
	add r1, r6, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	bl _08048FC8 @ far jump
	.align 2, 0
_08047DDC: .4byte 0x00001B33
_08047DE0: .4byte 0x00001B34
_08047DE4: .4byte 0x00001B2A
_08047DE8: .4byte 0x00001B2C
_08047DEC:
	ldr r1, _08047E14 @ =0x03000040
	mov r3, #2
	mov sl, r3
	mov r0, sl
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08047E20
	mov r0, #2
	bl PlaySE
	ldr r1, _08047E18 @ =0x020192E0
	ldr r4, _08047E1C @ =0x00001B2C
	add r1, r1, r4
	mov r0, #3
	neg r0, r0
	ldrb r5, [r1]
	and r0, r5
	bl _08048FC8 @ far jump
_08047E14: .4byte 0x03000040
_08047E18: .4byte 0x020192E0
_08047E1C: .4byte 0x00001B2C
_08047E20:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08047E2E
	bl _08048FCA @ far jump
_08047E2E:
	add r0, sp, #0x104
	ldr r1, _08047ECC @ =0x020192E0
	mov r8, r1
	ldr r1, _08047ED0 @ =0x00001B28
	add r1, r8
	ldrh r1, [r1]
	strh r1, [r0]
	ldr r6, _08047ED4 @ =0x00001B33
	add r6, r8
	ldrb r3, [r6]
	lsl r2, r3, #0x1E
	lsr r2, r2, #0x1F
	ldrb r3, [r0, #2]
	mov r1, #2
	neg r1, r1
	and r1, r3
	orr r1, r2
	strb r1, [r0, #2]
	ldr r4, _08047ED8 @ =0x0201CFB0
	ldr r5, _08047EDC @ =0x00000824
	add r7, r4, r5
	ldr r1, _08047EE0 @ =0x0000082C
	add r5, r4, r1
	ldrb r2, [r5]
	lsl r1, r2, #8
	ldrb r3, [r7]
	orr r1, r3
	bl EffectEquippedTributeCheck
	cmp r0, #0
	bne _08047E70
	bl _08048F54 @ far jump
_08047E70:
	mov r0, #1
	bl PlaySE
	mov r0, sl
	ldrb r1, [r6]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _08047E84
	ldr r3, _08047EE4 @ =0x00008008
_08047E84:
	ldrh r1, [r7]
	ldr r2, _08047EE8 @ =0x00000828
	add r0, r4, r2
	ldrb r4, [r5]
	lsl r2, r4, #8
	ldrb r0, [r0]
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	ldr r1, [r5]
	bl TributeMonster
	ldrb r6, [r6]
	lsl r0, r6, #0x1E
	lsr r0, r0, #0x1F
	ldr r1, _08047EEC @ =0x00001B34
	add r1, r8
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	ldr r2, [r5]
	mov r3, r9
	str r3, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	ldr r1, _08047EF0 @ =0x00001B2C
	add r1, r8
	bl _08048FC0 @ far jump
	.align 2, 0
_08047ECC: .4byte 0x020192E0
_08047ED0: .4byte 0x00001B28
_08047ED4: .4byte 0x00001B33
_08047ED8: .4byte 0x0201CFB0
_08047EDC: .4byte 0x00000824
_08047EE0: .4byte 0x0000082C
_08047EE4: .4byte 0x00008008
_08047EE8: .4byte 0x00000828
_08047EEC: .4byte 0x00001B34
_08047EF0: .4byte 0x00001B2C
_08047EF4:
	add r3, sp, #4
	ldr r4, _08047F14 @ =0x0808563C
	ldr r5, _08047F18 @ =0x00001B31
	add r0, r6, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	mov r2, #0xB9
	lsl r2, r2, #1
	add r1, r0, r2
	add r2, r1, #0
	ldr r0, _08047F1C @ =0x0000FFFF
	cmp r1, r0
	bne _08047F20
	mov r0, #0
	b _08047F4A
_08047F14: .4byte gStrTributeFromField
_08047F18: .4byte 0x00001B31
_08047F1C: .4byte 0x0000FFFF
_08047F20:
	ldr r0, _08047F30 @ =0x000007CF
	cmp r1, r0
	bhi _08047F38
	lsl r0, r1, #1
	ldr r5, _08047F34 @ =0x08623DF4
	add r0, r0, r5
	ldrh r0, [r0]
	b _08047F4A
_08047F30: .4byte 0x000007CF
_08047F34: .4byte gCardNumberToId
_08047F38:
	ldr r1, _08047F68 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08047F6C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08047F70 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_08047F4A:
	lsl r2, r0, #0x10
	lsr r2, r2, #0xA
	ldr r5, _08047F74 @ =0x0822C720
	add r2, r2, r5
	add r0, r3, #0
	add r1, r4, #0
	bl FormatStr
	ldr r0, _08047F78 @ =0x00000206
	ldr r1, _08047F7C @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl _08048E56 @ far jump
	.align 2, 0
_08047F68: .4byte 0xFFFFF830
_08047F6C: .4byte 0x000007FF
_08047F70: .4byte gCardNumberToId
_08047F74: .4byte gCardNames
_08047F78: .4byte 0x00000206
_08047F7C: .4byte 0x00000712
_08047F80:
	mov r0, #0xE0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08047F8E
	bl _08048FCA @ far jump
_08047F8E:
	ldr r1, _0804804C @ =0x0201CFB0
	mov r8, r1
	ldr r0, _08048050 @ =0x00000824
	add r0, r8
	ldr r4, [r0]
	ldr r2, _08048054 @ =0x0000082C
	add r2, r8
	mov sl, r2
	ldr r5, [r2]
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _08048058 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r7, _0804805C @ =0x0201930C
	add r2, r2, r7
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _08047FC2
	bl _08048FCA @ far jump
_08047FC2:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _08047FD0
	bl _08048FCA @ far jump
_08047FD0:
	ldr r0, _08048060 @ =0x000007FF
	and r1, r0
	lsl r1, r1, #1
	ldr r3, _08048064 @ =0x08622AB4
	add r1, r1, r3
	ldr r0, _08048068 @ =0x00001B05
	add r6, r7, r0
	ldrb r2, [r6]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1C
	mov r3, #0xB9
	lsl r3, r3, #1
	add r0, r0, r3
	ldrh r1, [r1]
	cmp r1, r0
	beq _08047FF4
	bl _08048F54 @ far jump
_08047FF4:
	mov r3, #8
	cmp r4, #0
	beq _08047FFC
	ldr r3, _0804806C @ =0x00008008
_08047FFC:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	ldr r0, _08048070 @ =0x00000828
	add r0, r8
	lsl r2, r5, #0x18
	lsr r2, r2, #0x10
	ldrb r0, [r0]
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r4, #0
	add r1, r5, #0
	bl TributeMonster
	ldrb r2, [r6]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	add r1, #1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0x3D
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #2
	bhi _08048078
	ldr r4, _08048074 @ =0x00001B04
	add r3, r7, r4
	ldrh r2, [r3]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	sub r1, #1
	bl _08048F24 @ far jump
_0804804C: .4byte 0x0201CFB0
_08048050: .4byte 0x00000824
_08048054: .4byte 0x0000082C
_08048058: .4byte 0x00000D64
_0804805C: .4byte 0x0201930C
_08048060: .4byte 0x000007FF
_08048064: .4byte gCardIdToNumber
_08048068: .4byte 0x00001B05
_0804806C: .4byte 0x00008008
_08048070: .4byte 0x00000828
_08048074: .4byte 0x00001B04
_08048078:
	ldr r5, _080480AC @ =0x00001B07
	add r0, r7, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, _080480B0 @ =0x00001B08
	add r1, r7, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r3, sl
	ldr r2, [r3]
	mov r4, r9
	str r4, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	sub r5, #7
	add r1, r7, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	bl _08048FC8 @ far jump
	.align 2, 0
_080480AC: .4byte 0x00001B07
_080480B0: .4byte 0x00001B08
_080480B4:
	ldr r3, _080480CC @ =0x00001B31
	add r0, r6, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #1
	beq _080480E0
	cmp r0, #1
	bgt _080480D0
	cmp r0, #0
	beq _080480D6
	b _080480EA
_080480CC: .4byte 0x00001B31
_080480D0:
	cmp r0, #2
	beq _080480E6
	b _080480EA
_080480D6:
	ldr r7, _080480DC @ =0x000002E1
	b _080480EA
	.align 2, 0
_080480DC: .4byte 0x000002E1
_080480E0:
	mov r7, #0xBD
	lsl r7, r7, #2
	b _080480EA
_080480E6:
	mov r7, #0xC8
	lsl r7, r7, #2
_080480EA:
	ldr r0, _0804819C @ =0x020192E0
	ldr r4, _080481A0 @ =0x00001B30
	add r5, r0, r4
	ldr r0, [r5]
	ldr r6, _080481A4 @ =0xFFFC3FFF
	and r0, r6
	str r0, [r5]
	ldr r4, _080481A8 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08048132
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08048132
	mov r0, #0
	add r1, r7, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _08048132
	ldr r2, [r5]
	lsl r1, r2, #0xE
	lsr r1, r1, #0x1C
	mov r0, #2
	orr r1, r0
	lsl r1, r1, #0xE
	add r0, r6, #0
	and r0, r2
	orr r0, r1
	str r0, [r5]
_08048132:
	mov r0, #0
	add r1, r7, #0
	bl CountHandCardsByNumber
	cmp r0, #0
	beq _08048170
	ldr r4, _0804819C @ =0x020192E0
	ldr r5, _080481AC @ =0x00001B31
	add r0, r4, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #1
	bls _08048158
	mov r0, #0
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _08048170
_08048158:
	ldr r0, _080481A0 @ =0x00001B30
	add r3, r4, r0
	ldr r2, [r3]
	lsl r1, r2, #0xE
	lsr r1, r1, #0x1C
	mov r0, #1
	orr r1, r0
	lsl r1, r1, #0xE
	ldr r0, _080481A4 @ =0xFFFC3FFF
	and r0, r2
	orr r0, r1
	str r0, [r3]
_08048170:
	ldr r0, _0804819C @ =0x020192E0
	ldr r1, _080481A0 @ =0x00001B30
	add r0, r0, r1
	ldr r0, [r0]
	lsl r2, r0, #0xE
	lsr r1, r2, #0x1C
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _080481F6
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _080481F6
	add r3, sp, #4
	ldr r4, _080481B0 @ =0x08085660
	ldr r0, _080481B4 @ =0x0000FFFF
	cmp r7, r0
	bne _080481B8
	mov r0, #0
	b _080481E6
	.align 2, 0
_0804819C: .4byte 0x020192E0
_080481A0: .4byte 0x00001B30
_080481A4: .4byte 0xFFFC3FFF
_080481A8: .4byte 0x0000058A
_080481AC: .4byte 0x00001B31
_080481B0: .4byte gStrTributeFromFieldOrHand
_080481B4: .4byte 0x0000FFFF
_080481B8:
	ldr r0, _080481CC @ =0x000007CF
	cmp r7, r0
	bhi _080481D4
	add r0, #0x30
	and r0, r7
	lsl r0, r0, #1
	ldr r2, _080481D0 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	b _080481E6
_080481CC: .4byte 0x000007CF
_080481D0: .4byte gCardNumberToId
_080481D4:
	ldr r5, _08048220 @ =0xFFFFF830
	add r0, r7, r5
	ldr r1, _08048224 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08048228 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_080481E6:
	lsl r2, r0, #0x10
	lsr r2, r2, #0xA
	ldr r5, _0804822C @ =0x0822C720
	add r2, r2, r5
	add r0, r3, #0
	add r1, r4, #0
	bl FormatStr
_080481F6:
	ldr r0, _08048230 @ =0x020192E0
	ldr r1, _08048234 @ =0x00001B30
	add r0, r0, r1
	ldr r0, [r0]
	lsl r2, r0, #0xE
	lsr r1, r2, #0x1C
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _0804827E
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _0804827E
	add r3, sp, #4
	ldr r4, _08048238 @ =0x08085698
	ldr r0, _0804823C @ =0x0000FFFF
	cmp r7, r0
	bne _08048240
	mov r0, #0
	b _0804826E
_08048220: .4byte 0xFFFFF830
_08048224: .4byte 0x000007FF
_08048228: .4byte gCardNumberToId
_0804822C: .4byte gCardNames
_08048230: .4byte 0x020192E0
_08048234: .4byte 0x00001B30
_08048238: .4byte gStrTributeFromHand
_0804823C: .4byte 0x0000FFFF
_08048240:
	ldr r0, _08048254 @ =0x000007CF
	cmp r7, r0
	bhi _0804825C
	add r0, #0x30
	and r0, r7
	lsl r0, r0, #1
	ldr r2, _08048258 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	b _0804826E
_08048254: .4byte 0x000007CF
_08048258: .4byte gCardNumberToId
_0804825C:
	ldr r5, _080482A4 @ =0xFFFFF830
	add r0, r7, r5
	ldr r1, _080482A8 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080482AC @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_0804826E:
	lsl r2, r0, #0x10
	lsr r2, r2, #0xA
	ldr r5, _080482B0 @ =0x0822C720
	add r2, r2, r5
	add r0, r3, #0
	add r1, r4, #0
	bl FormatStr
_0804827E:
	ldr r0, _080482B4 @ =0x020192E0
	ldr r1, _080482B8 @ =0x00001B30
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #8
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #8
	cmp r0, r1
	bne _08048302
	add r3, sp, #4
	ldr r4, _080482BC @ =0x0808563C
	ldr r0, _080482C0 @ =0x0000FFFF
	cmp r7, r0
	bne _080482C4
	mov r0, #0
	b _080482F2
	.align 2, 0
_080482A4: .4byte 0xFFFFF830
_080482A8: .4byte 0x000007FF
_080482AC: .4byte gCardNumberToId
_080482B0: .4byte gCardNames
_080482B4: .4byte 0x020192E0
_080482B8: .4byte 0x00001B30
_080482BC: .4byte gStrTributeFromField
_080482C0: .4byte 0x0000FFFF
_080482C4:
	ldr r0, _080482D8 @ =0x000007CF
	cmp r7, r0
	bhi _080482E0
	add r0, #0x30
	and r7, r0
	lsl r0, r7, #1
	ldr r2, _080482DC @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	b _080482F2
_080482D8: .4byte 0x000007CF
_080482DC: .4byte gCardNumberToId
_080482E0:
	ldr r5, _0804831C @ =0xFFFFF830
	add r0, r7, r5
	ldr r1, _08048320 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08048324 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_080482F2:
	lsl r2, r0, #0x10
	lsr r2, r2, #0xA
	ldr r5, _08048328 @ =0x0822C720
	add r2, r2, r5
	add r0, r3, #0
	add r1, r4, #0
	bl FormatStr
_08048302:
	ldr r2, _0804832C @ =0x020192E0
	ldr r0, _08048330 @ =0x00001B30
	add r4, r2, r0
	ldr r0, [r4]
	mov r1, #0xF0
	lsl r1, r1, #0xA
	and r0, r1
	cmp r0, #0
	bne _08048338
	ldr r3, _08048334 @ =0x00001B2C
	add r1, r2, r3
	bl _08048FC0 @ far jump
_0804831C: .4byte 0xFFFFF830
_08048320: .4byte 0x000007FF
_08048324: .4byte gCardNumberToId
_08048328: .4byte gCardNames
_0804832C: .4byte 0x020192E0
_08048330: .4byte 0x00001B30
_08048334: .4byte 0x00001B2C
_08048338:
	ldr r0, _08048360 @ =0x00000206
	ldr r1, _08048364 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
_08048344:
	ldrh r2, [r4]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08048368 @ =0xFFFFFC03
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	bl _08048FCA @ far jump
	.align 2, 0
_08048360: .4byte 0x00000206
_08048364: .4byte 0x00000712
_08048368: .4byte 0xFFFFFC03
_0804836C:
	ldr r5, _08048384 @ =0x00001B31
	add r0, r6, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #1
	beq _08048398
	cmp r0, #1
	bgt _08048388
	cmp r0, #0
	beq _0804838E
	b _080483A2
_08048384: .4byte 0x00001B31
_08048388:
	cmp r0, #2
	beq _0804839E
	b _080483A2
_0804838E:
	ldr r7, _08048394 @ =0x000002E1
	b _080483A2
	.align 2, 0
_08048394: .4byte 0x000002E1
_08048398:
	mov r7, #0xBD
	lsl r7, r7, #2
	b _080483A2
_0804839E:
	mov r7, #0xC8
	lsl r7, r7, #2
_080483A2:
	ldr r1, _0804843C @ =0x00001B30
	add r0, r6, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r0, r1, #0x1C
	mov r2, #1
	and r2, r0
	add r1, r0, #0
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _080483BE
	mov r0, #0xE0
	orr r2, r0
_080483BE:
	add r0, r2, #0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _080483CC
	bl _08048FCA @ far jump
_080483CC:
	ldr r4, _08048440 @ =0x0201CFB0
	ldr r2, _08048444 @ =0x00000824
	add r5, r4, r2
	ldr r3, [r5]
	mov r8, r3
	ldr r0, _08048448 @ =0x0000082C
	add r0, r0, r4
	mov r9, r0
	ldr r6, [r0]
	bl DuelCursor_GetCardId
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	cmp r1, #0
	bne _080483EE
	bl _08048FCA @ far jump
_080483EE:
	ldr r0, _0804844C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08048450 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	beq _08048402
	bl _08048F54 @ far jump
_08048402:
	ldr r1, [r5]
	mov r0, #8
	cmp r1, #0
	beq _0804840C
	ldr r0, _08048454 @ =0x00008008
_0804840C:
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, _08048458 @ =0x00000828
	add r4, r4, r2
	mov r3, r9
	ldrb r3, [r3]
	lsl r2, r3, #8
	ldrb r5, [r4]
	orr r2, r5
	mov r3, #0
	bl DuelCmd_Push
	ldr r4, [r4]
	cmp r4, #0
	beq _0804845C
	cmp r4, #0xB
	bne _08048464
	mov r0, r8
	add r1, r6, #0
	mov r2, #0
	mov r3, #0
	bl DiscardHandCard
	b _08048464
_0804843C: .4byte 0x00001B30
_08048440: .4byte 0x0201CFB0
_08048444: .4byte 0x00000824
_08048448: .4byte 0x0000082C
_0804844C: .4byte 0x000007FF
_08048450: .4byte gCardIdToNumber
_08048454: .4byte 0x00008008
_08048458: .4byte 0x00000828
_0804845C:
	mov r0, r8
	add r1, r6, #0
	bl TributeMonster
_08048464:
	ldr r4, _0804849C @ =0x020192E0
	ldr r0, _080484A0 @ =0x00001B31
	add r3, r4, r0
	ldrb r2, [r3]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	add r1, #1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0x3D
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #2
	bhi _080484A8
	ldr r1, _080484A4 @ =0x00001B30
	add r3, r4, r1
	ldrh r2, [r3]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	sub r1, #1
	bl _08048F24 @ far jump
	.align 2, 0
_0804849C: .4byte 0x020192E0
_080484A0: .4byte 0x00001B31
_080484A4: .4byte 0x00001B30
_080484A8:
	ldr r2, _080484B0 @ =0x00001B30
	add r3, r4, r2
	bl _08048F1C @ far jump
_080484B0: .4byte 0x00001B30
_080484B4:
	ldr r3, _080484F0 @ =0x00001B33
	add r4, r6, r3
	ldrb r5, [r4]
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	add r2, r0, #0
	ldrb r4, [r4]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _080484F4 @ =0x00001B34
	add r1, r6, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r4, r9
	str r4, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	ldr r5, _080484F8 @ =0x00001B2C
	add r1, r6, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	bl _08048FC8 @ far jump
	.align 2, 0
_080484F0: .4byte 0x00001B33
_080484F4: .4byte 0x00001B34
_080484F8: .4byte 0x00001B2C
_080484FC:
	ldr r2, _08048528 @ =0x020192E4
	ldr r0, _0804852C @ =0x0201CFB0
	ldr r3, _08048530 @ =0x00000824
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #1
	and r0, r1
	ldr r1, _08048534 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _08048540
	add r4, sp, #0x84
	ldr r1, _08048538 @ =0x080856BC
	ldr r2, _0804853C @ =0x08085708
	add r0, r4, #0
	bl FormatStr
	b _0804854C
	.align 2, 0
_08048528: .4byte 0x020192E4
_0804852C: .4byte 0x0201CFB0
_08048530: .4byte 0x00000824
_08048534: .4byte 0x00000D64
_08048538: .4byte gStrBanishFieldMonstersCount
_0804853C: .4byte gStrFiend
_08048540:
	add r4, sp, #0x84
	ldr r1, _0804856C @ =0x08085710
	ldr r2, _08048570 @ =0x08085708
	add r0, r4, #0
	bl FormatStr
_0804854C:
	add r1, r4, #0
	add r0, sp, #4
	mov r2, #3
	bl FormatInt
	ldr r0, _08048574 @ =0x00000206
	ldr r1, _08048578 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r2, _0804857C @ =0x020192E0
	ldr r4, _08048580 @ =0x00001B30
	add r2, r2, r4
	bl _08048E60 @ far jump
_0804856C: .4byte gStrBanishGraveyardMonstersCount
_08048570: .4byte gStrFiend
_08048574: .4byte 0x00000206
_08048578: .4byte 0x00000712
_0804857C: .4byte 0x020192E0
_08048580: .4byte 0x00001B30
_08048584:
	ldr r6, _080485C8 @ =0x020192E4
	ldr r4, _080485CC @ =0x0201CFB0
	ldr r0, _080485D0 @ =0x00000824
	add r5, r4, r0
	ldr r3, [r5]
	mov r0, #1
	and r0, r3
	ldr r1, _080485D4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r6
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _080485E8
	mov r1, #1
	neg r1, r1
	ldr r4, _080485D8 @ =0x00001B24
	add r2, r6, r4
	ldr r0, _080485DC @ =0x000007FF
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #1
	ldr r5, _080485E0 @ =0x08622AB4
	add r0, r0, r5
	ldrh r2, [r0]
	add r0, r3, #0
	mov r3, #0
	bl CardListView_Open
	ldr r0, _080485E4 @ =0x00001B2C
	add r3, r6, r0
	bl _08048F1C @ far jump
	.align 2, 0
_080485C8: .4byte 0x020192E4
_080485CC: .4byte 0x0201CFB0
_080485D0: .4byte 0x00000824
_080485D4: .4byte 0x00000D64
_080485D8: .4byte 0x00001B24
_080485DC: .4byte 0x000007FF
_080485E0: .4byte gCardIdToNumber
_080485E4: .4byte 0x00001B2C
_080485E8:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _080485F6
	bl _08048FCA @ far jump
_080485F6:
	ldr r0, [r5]
	ldr r1, _0804861C @ =0x0000082C
	add r4, r4, r1
	ldr r1, [r4]
	bl GetZoneCardType
	cmp r0, #3
	beq _0804860A
	bl _08048F54 @ far jump
_0804860A:
	ldr r0, [r5]
	ldr r1, [r4]
	mov r2, #0
	bl BanishFieldCard
	ldr r2, _08048620 @ =0x00001B2C
	add r3, r6, r2
	bl _08048F1C @ far jump
_0804861C: .4byte 0x0000082C
_08048620: .4byte 0x00001B2C
_08048624:
	ldr r4, _08048660 @ =0x020192E4
	ldr r0, _08048664 @ =0x0201CFB0
	ldr r3, _08048668 @ =0x00000824
	add r0, r0, r3
	ldr r2, [r0]
	mov r0, #1
	and r0, r2
	ldr r1, _0804866C @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08048658
	ldr r0, _08048670 @ =0x0201D810
	ldrb r5, [r0, #5]
	lsl r1, r5, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, #0
	bl BanishGraveyardCard
_08048658:
	ldr r5, _08048674 @ =0x00001B2C
	add r3, r4, r5
	bl _08048F1C @ far jump
_08048660: .4byte 0x020192E4
_08048664: .4byte 0x0201CFB0
_08048668: .4byte 0x00000824
_0804866C: .4byte 0x00000D64
_08048670: .4byte 0x0201D810
_08048674: .4byte 0x00001B2C
_08048678:
	ldr r1, _08048690 @ =0x08085758
	add r0, sp, #4
	mov r2, #2
	bl FormatInt
	ldr r0, _08048694 @ =0x00000206
	ldr r1, _08048698 @ =0x00000412
	mov r2, #0xB
	add r3, sp, #4
	bl _08048E56 @ far jump
	.align 2, 0
_08048690: .4byte gStrCardsRemaining
_08048694: .4byte 0x00000206
_08048698: .4byte 0x00000412
_0804869C:
	ldr r1, _080486BC @ =0x08085758
	add r0, sp, #4
	mov r2, #1
	bl FormatInt
	ldr r0, _080486C0 @ =0x00000206
	ldr r1, _080486C4 @ =0x00000412
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r2, _080486C8 @ =0x020192E0
	ldr r1, _080486CC @ =0x00001B30
	add r2, r2, r1
	bl _08048E60 @ far jump
_080486BC: .4byte gStrCardsRemaining
_080486C0: .4byte 0x00000206
_080486C4: .4byte 0x00000412
_080486C8: .4byte 0x020192E0
_080486CC: .4byte 0x00001B30
_080486D0:
	ldr r4, _08048714 @ =0x020192E4
	ldr r0, _08048718 @ =0x0201CFB0
	ldr r2, _0804871C @ =0x00000824
	add r0, r0, r2
	ldr r2, [r0]
	mov r0, #1
	and r0, r2
	ldr r1, _08048720 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08048704
	ldr r0, _08048724 @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r5, [r0, #6]
	add r1, r5, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, #0
	bl BanishGraveyardCard
_08048704:
	ldr r1, _08048728 @ =0x00001B2C
	add r0, r4, r1
	ldr r1, _0804872C @ =0xFFFFFC03
	ldrh r2, [r0]
	and r1, r2
	mov r3, #0x92
	lsl r3, r3, #1
	b _08048EAA
_08048714: .4byte 0x020192E4
_08048718: .4byte 0x0201CFB0
_0804871C: .4byte 0x00000824
_08048720: .4byte 0x00000D64
_08048724: .4byte 0x0201D810
_08048728: .4byte 0x00001B2C
_0804872C: .4byte 0xFFFFFC03
_08048730:
	ldr r2, _0804875C @ =0x020192E4
	ldr r0, _08048760 @ =0x0201CFB0
	ldr r4, _08048764 @ =0x00000824
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #1
	and r0, r1
	ldr r1, _08048768 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _08048774
	add r4, sp, #0x84
	ldr r1, _0804876C @ =0x080856BC
	ldr r2, _08048770 @ =0x0808577C
	add r0, r4, #0
	bl FormatStr
	b _08048780
	.align 2, 0
_0804875C: .4byte 0x020192E4
_08048760: .4byte 0x0201CFB0
_08048764: .4byte 0x00000824
_08048768: .4byte 0x00000D64
_0804876C: .4byte gStrBanishFieldMonstersCount
_08048770: .4byte gStrLight
_08048774:
	add r4, sp, #0x84
	ldr r1, _080487A0 @ =0x08085710
	ldr r2, _080487A4 @ =0x0808577C
	add r0, r4, #0
	bl FormatStr
_08048780:
	add r1, r4, #0
	add r0, sp, #4
	mov r2, #2
	bl FormatInt
	ldr r0, _080487A8 @ =0x00000206
	ldr r1, _080487AC @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r2, _080487B0 @ =0x020192E0
	ldr r5, _080487B4 @ =0x00001B30
	add r2, r2, r5
	b _08048E60
	.align 2, 0
_080487A0: .4byte gStrBanishGraveyardMonstersCount
_080487A4: .4byte gStrLight
_080487A8: .4byte 0x00000206
_080487AC: .4byte 0x00000712
_080487B0: .4byte 0x020192E0
_080487B4: .4byte 0x00001B30
_080487B8:
	ldr r6, _080487F8 @ =0x020192E4
	ldr r4, _080487FC @ =0x0201CFB0
	ldr r0, _08048800 @ =0x00000824
	add r5, r4, r0
	ldr r3, [r5]
	mov r0, #1
	and r0, r3
	ldr r1, _08048804 @ =0x00000D64
	mul r0, r1
	add r0, r0, r6
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08048818
	mov r1, #1
	neg r1, r1
	ldr r4, _08048808 @ =0x00001B24
	add r2, r6, r4
	ldr r0, _0804880C @ =0x000007FF
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #1
	ldr r5, _08048810 @ =0x08622AB4
	add r0, r0, r5
	ldrh r2, [r0]
	add r0, r3, #0
	mov r3, #0
	bl CardListView_Open
	ldr r0, _08048814 @ =0x00001B2C
	add r3, r6, r0
	b _08048F1C
_080487F8: .4byte 0x020192E4
_080487FC: .4byte 0x0201CFB0
_08048800: .4byte 0x00000824
_08048804: .4byte 0x00000D64
_08048808: .4byte 0x00001B24
_0804880C: .4byte 0x000007FF
_08048810: .4byte gCardIdToNumber
_08048814: .4byte 0x00001B2C
_08048818:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08048826
	bl _08048FCA @ far jump
_08048826:
	ldr r0, [r5]
	ldr r1, _08048848 @ =0x0000082C
	add r4, r4, r1
	ldr r1, [r4]
	bl GetZoneCardAttribute
	cmp r0, #1
	beq _08048838
	b _08048F54
_08048838:
	ldr r0, [r5]
	ldr r1, [r4]
	mov r2, #0
	bl BanishFieldCard
	ldr r2, _0804884C @ =0x00001B2C
	add r3, r6, r2
	b _08048F1C
_08048848: .4byte 0x0000082C
_0804884C: .4byte 0x00001B2C
_08048850:
	ldr r4, _0804888C @ =0x020192E4
	ldr r0, _08048890 @ =0x0201CFB0
	ldr r3, _08048894 @ =0x00000824
	add r0, r0, r3
	ldr r2, [r0]
	mov r0, #1
	and r0, r2
	ldr r1, _08048898 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08048884
	ldr r0, _0804889C @ =0x0201D810
	ldrb r5, [r0, #5]
	lsl r1, r5, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, #0
	bl BanishGraveyardCard
_08048884:
	ldr r5, _080488A0 @ =0x00001B2C
	add r3, r4, r5
	b _08048F1C
	.align 2, 0
_0804888C: .4byte 0x020192E4
_08048890: .4byte 0x0201CFB0
_08048894: .4byte 0x00000824
_08048898: .4byte 0x00000D64
_0804889C: .4byte 0x0201D810
_080488A0: .4byte 0x00001B2C
_080488A4:
	ldr r1, _080488B8 @ =0x08085758
	add r0, sp, #4
	mov r2, #1
	bl FormatInt
	ldr r0, _080488BC @ =0x00000206
	ldr r1, _080488C0 @ =0x00000412
	mov r2, #0xB
	add r3, sp, #4
	b _08048E56
_080488B8: .4byte gStrCardsRemaining
_080488BC: .4byte 0x00000206
_080488C0: .4byte 0x00000412
_080488C4:
	ldr r4, _08048908 @ =0x020192E4
	ldr r0, _0804890C @ =0x0201CFB0
	ldr r1, _08048910 @ =0x00000824
	add r0, r0, r1
	ldr r2, [r0]
	mov r0, #1
	and r0, r2
	ldr r1, _08048914 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _080488F8
	ldr r0, _08048918 @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r5, [r0, #6]
	add r1, r5, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, #0
	bl BanishGraveyardCard
_080488F8:
	ldr r1, _0804891C @ =0x00001B2C
	add r0, r4, r1
	ldr r1, _08048920 @ =0xFFFFFC03
	ldrh r2, [r0]
	and r1, r2
	mov r3, #0x92
	lsl r3, r3, #1
	b _08048EAA
_08048908: .4byte 0x020192E4
_0804890C: .4byte 0x0201CFB0
_08048910: .4byte 0x00000824
_08048914: .4byte 0x00000D64
_08048918: .4byte 0x0201D810
_0804891C: .4byte 0x00001B2C
_08048920: .4byte 0xFFFFFC03
_08048924:
	ldr r4, _08048948 @ =0x00001B28
	add r1, r6, r4
	ldr r0, _0804894C @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r5, _08048950 @ =0x08622AB4
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _08048954 @ =0x000005ED
	cmp r1, r0
	beq _080489B0
	cmp r1, r0
	bgt _08048958
	sub r0, #1
	cmp r1, r0
	beq _0804896C
	b _08048A7E
_08048948: .4byte 0x00001B28
_0804894C: .4byte 0x000007FF
_08048950: .4byte gCardIdToNumber
_08048954: .4byte 0x000005ED
_08048958:
	ldr r0, _08048968 @ =0x000005EE
	cmp r1, r0
	beq _080489F4
	add r0, #1
	cmp r1, r0
	beq _08048A38
	b _08048A7E
	.align 2, 0
_08048968: .4byte 0x000005EE
_0804896C:
	ldr r0, _0804898C @ =0x0201CFB0
	ldr r1, _08048990 @ =0x00000824
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #1
	and r0, r1
	ldr r1, _08048994 @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldrb r0, [r0, #0x10]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _080489A0
	ldr r1, _08048998 @ =0x08085784
	ldr r2, _0804899C @ =0x080857C4
	b _08048A56
_0804898C: .4byte 0x0201CFB0
_08048990: .4byte 0x00000824
_08048994: .4byte 0x00000D64
_08048998: .4byte gStrBanishFieldMonster
_0804899C: .4byte gStrFire
_080489A0:
	ldr r1, _080489A8 @ =0x080857CC
	ldr r2, _080489AC @ =0x080857C4
	b _08048A56
	.align 2, 0
_080489A8: .4byte gStrBanishGraveyardMonster
_080489AC: .4byte gStrFire
_080489B0:
	ldr r0, _080489D0 @ =0x0201CFB0
	ldr r2, _080489D4 @ =0x00000824
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #1
	and r0, r1
	ldr r1, _080489D8 @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldrb r0, [r0, #0x10]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _080489E4
	ldr r1, _080489DC @ =0x08085784
	ldr r2, _080489E0 @ =0x08085804
	b _08048A56
_080489D0: .4byte 0x0201CFB0
_080489D4: .4byte 0x00000824
_080489D8: .4byte 0x00000D64
_080489DC: .4byte gStrBanishFieldMonster
_080489E0: .4byte gStrWater
_080489E4:
	ldr r1, _080489EC @ =0x080857CC
	ldr r2, _080489F0 @ =0x08085804
	b _08048A56
	.align 2, 0
_080489EC: .4byte gStrBanishGraveyardMonster
_080489F0: .4byte gStrWater
_080489F4:
	ldr r0, _08048A14 @ =0x0201CFB0
	ldr r3, _08048A18 @ =0x00000824
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #1
	and r0, r1
	ldr r1, _08048A1C @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldrb r0, [r0, #0x10]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _08048A28
	ldr r1, _08048A20 @ =0x08085784
	ldr r2, _08048A24 @ =0x0808580C
	b _08048A56
_08048A14: .4byte 0x0201CFB0
_08048A18: .4byte 0x00000824
_08048A1C: .4byte 0x00000D64
_08048A20: .4byte gStrBanishFieldMonster
_08048A24: .4byte gStrEarth
_08048A28:
	ldr r1, _08048A30 @ =0x080857CC
	ldr r2, _08048A34 @ =0x0808580C
	b _08048A56
	.align 2, 0
_08048A30: .4byte gStrBanishGraveyardMonster
_08048A34: .4byte gStrEarth
_08048A38:
	ldr r0, _08048A60 @ =0x0201CFB0
	ldr r4, _08048A64 @ =0x00000824
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #1
	and r0, r1
	ldr r1, _08048A68 @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldrb r0, [r0, #0x10]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _08048A74
	ldr r1, _08048A6C @ =0x08085784
	ldr r2, _08048A70 @ =0x08085814
_08048A56:
	add r0, sp, #4
	bl FormatStr
	b _08048A7E
	.align 2, 0
_08048A60: .4byte 0x0201CFB0
_08048A64: .4byte 0x00000824
_08048A68: .4byte 0x00000D64
_08048A6C: .4byte gStrBanishFieldMonster
_08048A70: .4byte gStrWind
_08048A74:
	ldr r1, _08048A94 @ =0x080857CC
	ldr r2, _08048A98 @ =0x08085814
	add r0, sp, #4
	bl FormatStr
_08048A7E:
	ldr r0, _08048A9C @ =0x00000206
	ldr r1, _08048AA0 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r2, _08048AA4 @ =0x020192E0
	ldr r5, _08048AA8 @ =0x00001B30
	add r2, r2, r5
	b _08048E60
	.align 2, 0
_08048A94: .4byte gStrBanishGraveyardMonster
_08048A98: .4byte gStrWind
_08048A9C: .4byte 0x00000206
_08048AA0: .4byte 0x00000712
_08048AA4: .4byte 0x020192E0
_08048AA8: .4byte 0x00001B30
_08048AAC:
	ldr r4, _08048AEC @ =0x020192E4
	ldr r0, _08048AF0 @ =0x0201CFB0
	ldr r1, _08048AF4 @ =0x00000824
	add r0, r0, r1
	ldr r3, [r0]
	mov r0, #1
	and r0, r3
	ldr r1, _08048AF8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08048B08
	mov r1, #1
	neg r1, r1
	ldr r5, _08048AFC @ =0x00001B24
	add r2, r4, r5
	ldr r0, _08048B00 @ =0x000007FF
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _08048B04 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	add r0, r3, #0
	mov r3, #0
	bl CardListView_Open
	add r5, #8
	add r3, r4, r5
	b _08048F1C
_08048AEC: .4byte 0x020192E4
_08048AF0: .4byte 0x0201CFB0
_08048AF4: .4byte 0x00000824
_08048AF8: .4byte 0x00000D64
_08048AFC: .4byte 0x00001B24
_08048B00: .4byte 0x000007FF
_08048B04: .4byte gCardIdToNumber
_08048B08:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08048B14
	b _08048FCA
_08048B14:
	mov r5, #0
	ldr r0, _08048B3C @ =0x00001B24
	add r1, r4, r0
	ldr r0, _08048B40 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08048B44 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08048B48 @ =0x000005ED
	cmp r1, r0
	beq _08048B64
	cmp r1, r0
	bgt _08048B4C
	sub r0, #1
	cmp r1, r0
	beq _08048B60
	b _08048B6E
	.align 2, 0
_08048B3C: .4byte 0x00001B24
_08048B40: .4byte 0x000007FF
_08048B44: .4byte gCardIdToNumber
_08048B48: .4byte 0x000005ED
_08048B4C:
	ldr r0, _08048B5C @ =0x000005EE
	cmp r1, r0
	beq _08048B68
	add r0, #1
	cmp r1, r0
	beq _08048B6C
	b _08048B6E
	.align 2, 0
_08048B5C: .4byte 0x000005EE
_08048B60:
	mov r5, #4
	b _08048B6E
_08048B64:
	mov r5, #3
	b _08048B6E
_08048B68:
	mov r5, #5
	b _08048B6E
_08048B6C:
	mov r5, #6
_08048B6E:
	ldr r1, _08048B98 @ =0x0201CFB0
	ldr r2, _08048B9C @ =0x00000824
	add r6, r1, r2
	ldr r0, [r6]
	ldr r3, _08048BA0 @ =0x0000082C
	add r4, r1, r3
	ldr r1, [r4]
	bl GetZoneCardAttribute
	cmp r0, r5
	beq _08048B86
	b _08048F54
_08048B86:
	ldr r0, [r6]
	ldr r1, [r4]
	mov r2, #0
	bl BanishFieldCard
	ldr r2, _08048BA4 @ =0x020192E0
	ldr r4, _08048BA8 @ =0x00001B30
	add r2, r2, r4
	b _08048E60
_08048B98: .4byte 0x0201CFB0
_08048B9C: .4byte 0x00000824
_08048BA0: .4byte 0x0000082C
_08048BA4: .4byte 0x020192E0
_08048BA8: .4byte 0x00001B30
_08048BAC:
	ldr r4, _08048BE8 @ =0x020192E4
	ldr r0, _08048BEC @ =0x0201CFB0
	ldr r5, _08048BF0 @ =0x00000824
	add r0, r0, r5
	ldr r2, [r0]
	mov r0, #1
	and r0, r2
	ldr r1, _08048BF4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _08048BE0
	ldr r0, _08048BF8 @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r5, [r0, #6]
	add r1, r5, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, #0
	bl BanishGraveyardCard
_08048BE0:
	ldr r0, _08048BFC @ =0x00001B2C
	add r3, r4, r0
	b _08048F1C
	.align 2, 0
_08048BE8: .4byte 0x020192E4
_08048BEC: .4byte 0x0201CFB0
_08048BF0: .4byte 0x00000824
_08048BF4: .4byte 0x00000D64
_08048BF8: .4byte 0x0201D810
_08048BFC: .4byte 0x00001B2C
_08048C00:
	ldr r1, _08048C38 @ =0x00001B33
	add r4, r6, r1
	ldrb r2, [r4]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	add r2, r0, #0
	ldrb r4, [r4]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08048C3C @ =0x00001B34
	add r1, r6, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r4, r9
	str r4, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	ldr r5, _08048C40 @ =0x00001B2C
	add r1, r6, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08048FC8
_08048C38: .4byte 0x00001B33
_08048C3C: .4byte 0x00001B34
_08048C40: .4byte 0x00001B2C
_08048C44:
	ldr r3, _08048CBC @ =0x00001B33
	add r5, r6, r3
	ldrb r4, [r5]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	ldr r7, _08048CC0 @ =0x00000582
	add r1, r7, #0
	bl CountMonstersByNumber
	neg r4, r0
	orr r4, r0
	lsr r4, r4, #0x1F
	ldrb r5, [r5]
	lsl r0, r5, #0x1E
	lsr r0, r0, #0x1F
	ldr r5, _08048CC4 @ =0x00000584
	mov r8, r5
	mov r1, r8
	bl CountMonstersByNumber
	neg r1, r0
	orr r1, r0
	lsr r1, r1, #0x1F
	cmp r4, #0
	beq _08048D20
	cmp r1, #0
	beq _08048CE0
	add r5, sp, #0x84
	ldr r1, _08048CC8 @ =0x0808581C
	lsl r0, r7, #1
	ldr r2, _08048CCC @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r4, _08048CD0 @ =0x0822C720
	add r2, r2, r4
	add r0, r5, #0
	bl FormatStr
	mov r3, r8
	lsl r0, r3, #1
	ldr r1, _08048CCC @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r2, r0, #6
	add r2, r2, r4
	add r0, sp, #4
	add r1, r5, #0
	bl FormatStr
	ldr r0, _08048CD4 @ =0x00000206
	ldr r1, _08048CD8 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r2, _08048CDC @ =0x00001B30
	add r3, r6, r2
	b _08048F1C
	.align 2, 0
_08048CBC: .4byte 0x00001B33
_08048CC0: .4byte 0x00000582
_08048CC4: .4byte 0x00000584
_08048CC8: .4byte gStrTributeEitherFromField
_08048CCC: .4byte gCardNumberToId
_08048CD0: .4byte gCardNames
_08048CD4: .4byte 0x00000206
_08048CD8: .4byte 0x00000712
_08048CDC: .4byte 0x00001B30
_08048CE0:
	ldr r1, _08048D08 @ =0x08085850
	lsl r0, r7, #1
	ldr r3, _08048D0C @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r4, _08048D10 @ =0x0822C720
	add r2, r2, r4
	add r0, sp, #4
	bl FormatStr
	ldr r0, _08048D14 @ =0x00000206
	ldr r1, _08048D18 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r5, _08048D1C @ =0x00001B30
	add r3, r6, r5
	b _08048F1C
_08048D08: .4byte gStrTributeOneFromField
_08048D0C: .4byte gCardNumberToId
_08048D10: .4byte gCardNames
_08048D14: .4byte 0x00000206
_08048D18: .4byte 0x00000712
_08048D1C: .4byte 0x00001B30
_08048D20:
	cmp r1, #0
	beq _08048D68
	ldr r1, _08048D50 @ =0x08085850
	mov r2, r8
	lsl r0, r2, #1
	ldr r3, _08048D54 @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r4, _08048D58 @ =0x0822C720
	add r2, r2, r4
	add r0, sp, #4
	bl FormatStr
	ldr r0, _08048D5C @ =0x00000206
	ldr r1, _08048D60 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	ldr r5, _08048D64 @ =0x00001B30
	add r3, r6, r5
	b _08048F1C
	.align 2, 0
_08048D50: .4byte gStrTributeOneFromField
_08048D54: .4byte gCardNumberToId
_08048D58: .4byte gCardNames
_08048D5C: .4byte 0x00000206
_08048D60: .4byte 0x00000712
_08048D64: .4byte 0x00001B30
_08048D68:
	ldr r0, _08048D78 @ =0x00001B2C
	add r1, r6, r0
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08048FC8
	.align 2, 0
_08048D78: .4byte 0x00001B2C
_08048D7C:
	ldr r1, _08048D9C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08048DA8
	ldr r3, _08048DA0 @ =0x00001B30
	add r0, r6, r3
	ldr r1, _08048DA4 @ =0xFFFFFC03
	ldrh r4, [r0]
	and r1, r4
	mov r5, #0xA0
	lsl r5, r5, #1
	add r2, r5, #0
	b _08048EAC
	.align 2, 0
_08048D9C: .4byte 0x03000040
_08048DA0: .4byte 0x00001B30
_08048DA4: .4byte 0xFFFFFC03
_08048DA8:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08048DB4
	b _08048FCA
_08048DB4:
	ldr r4, _08048E24 @ =0x0201CFB0
	ldr r0, _08048E28 @ =0x00000824
	add r7, r4, r0
	ldr r0, [r7]
	ldr r1, _08048E2C @ =0x0000082C
	add r5, r4, r1
	ldr r1, [r5]
	bl IsTributableMonster
	cmp r0, #0
	bne _08048DCC
	b _08048F54
_08048DCC:
	ldr r2, _08048E30 @ =0x00000828
	add r6, r4, r2
	ldr r2, [r6]
	mov r0, #1
	and r2, r0
	ldr r3, [r5]
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08048E34 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r4, _08048E38 @ =0x0201930C
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08048E3C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08048E40 @ =0x00000582
	cmp r1, r0
	beq _08048E00
	add r0, #2
	cmp r1, r0
	beq _08048E00
	b _08048F54
_08048E00:
	ldr r2, _08048E44 @ =0x00001AFE
	add r0, r4, r2
	strh r3, [r0]
	mov r0, #1
	bl PlaySE
	ldrh r1, [r7]
	ldrb r5, [r5]
	lsl r2, r5, #8
	ldrb r6, [r6]
	orr r2, r6
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldr r5, _08048E48 @ =0x00001B04
	add r3, r4, r5
	b _08048F1C
_08048E24: .4byte 0x0201CFB0
_08048E28: .4byte 0x00000824
_08048E2C: .4byte 0x0000082C
_08048E30: .4byte 0x00000828
_08048E34: .4byte 0x00000D64
_08048E38: .4byte 0x0201930C
_08048E3C: .4byte gCardIdToNumber
_08048E40: .4byte 0x00000582
_08048E44: .4byte 0x00001AFE
_08048E48: .4byte 0x00001B04
_08048E4C:
	ldr r0, _08048E78 @ =0x00000206
	ldr r1, _08048E7C @ =0x00000412
	ldr r2, _08048E80 @ =0x0819D1C4
	ldr r3, [r2, #0xC]
	mov r2, #0xB
_08048E56:
	bl TextBoxOpen
	ldr r2, _08048E84 @ =0x020192E0
	ldr r0, _08048E88 @ =0x00001B30
	add r2, r2, r0
_08048E60:
	ldrh r3, [r2]
	lsl r1, r3, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08048E8C @ =0xFFFFFC03
	and r0, r3
_08048E72:
	orr r0, r1
	strh r0, [r2]
	b _08048FCA
_08048E78: .4byte 0x00000206
_08048E7C: .4byte 0x00000412
_08048E80: .4byte gTributeSummonPrompts
_08048E84: .4byte 0x020192E0
_08048E88: .4byte 0x00001B30
_08048E8C: .4byte 0xFFFFFC03
_08048E90:
	ldr r1, _08048EB4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08048EC0
	ldr r1, _08048EB8 @ =0x00001B30
	add r0, r6, r1
	ldr r1, _08048EBC @ =0xFFFFFC03
	ldrh r2, [r0]
	and r1, r2
	mov r3, #0xA0
	lsl r3, r3, #1
_08048EAA:
	add r2, r3, #0
_08048EAC:
	orr r1, r2
	strh r1, [r0]
	b _08048FCA
	.align 2, 0
_08048EB4: .4byte 0x03000040
_08048EB8: .4byte 0x00001B30
_08048EBC: .4byte 0xFFFFFC03
_08048EC0:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08048ECC
	b _08048FCA
_08048ECC:
	ldr r4, _08048F34 @ =0x020192E0
	mov r8, r4
	ldr r7, _08048F38 @ =0x00001B2A
	add r7, r8
	ldr r4, _08048F3C @ =0x0201CFB0
	ldr r5, _08048F40 @ =0x0000082C
	add r6, r4, r5
	ldr r1, [r6]
	ldrb r0, [r7]
	ldrb r2, [r6]
	cmp r0, r2
	beq _08048F54
	ldr r3, _08048F44 @ =0x00000824
	add r5, r4, r3
	ldr r0, [r5]
	bl IsTributableMonster
	cmp r0, #0
	beq _08048F54
	mov r0, #1
	bl PlaySE
	ldrh r1, [r5]
	ldr r5, _08048F48 @ =0x00000828
	add r0, r4, r5
	ldrb r3, [r6]
	lsl r2, r3, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldrb r6, [r6]
	lsl r0, r6, #8
	ldrb r4, [r7]
	orr r0, r4
	strh r0, [r7]
_08048F18:
	ldr r3, _08048F4C @ =0x00001B30
	add r3, r8
_08048F1C:
	ldrh r2, [r3]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	add r1, #1
_08048F24:
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08048F50 @ =0xFFFFFC03
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _08048FCA
_08048F34: .4byte 0x020192E0
_08048F38: .4byte 0x00001B2A
_08048F3C: .4byte 0x0201CFB0
_08048F40: .4byte 0x0000082C
_08048F44: .4byte 0x00000824
_08048F48: .4byte 0x00000828
_08048F4C: .4byte 0x00001B30
_08048F50: .4byte 0xFFFFFC03
_08048F54:
	mov r0, #3
	bl PlaySE
	b _08048FCA
_08048F5C:
	ldr r5, _08048FAC @ =0x00001B33
	add r4, r6, r5
	ldrb r1, [r4]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, _08048FB0 @ =0x00001B2A
	add r5, r6, r2
	ldrb r1, [r5]
	bl TributeMonster
	ldrb r3, [r4]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	ldrh r2, [r5]
	lsr r1, r2, #8
	bl TributeMonster
	ldrb r4, [r4]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08048FB4 @ =0x00001B34
	add r1, r6, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r2, #7
	ldrb r5, [r5]
	and r2, r5
	mov r4, r9
	str r4, [sp, #0]
	mov r3, #0
	bl QueueSpecialSummonFromHand
	ldr r5, _08048FB8 @ =0x00001B2C
	add r1, r6, r5
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08048FC8
_08048FAC: .4byte 0x00001B33
_08048FB0: .4byte 0x00001B2A
_08048FB4: .4byte 0x00001B34
_08048FB8: .4byte 0x00001B2C
_08048FBC:
	ldr r3, _08048FDC @ =0x00001B2C
	add r1, r6, r3
_08048FC0:
	mov r0, #3
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
_08048FC8:
	strb r0, [r1]
_08048FCA:
	add sp, #0x118
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08048FDC: .4byte 0x00001B2C
	thumb_func_end CardMenu_SummonMonster

