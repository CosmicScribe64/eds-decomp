	thumb_func_start sub_08017460
sub_08017460: @ 0x08017460
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r6, r0, #0
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0]
	mov r5, #1
	add r1, r6, #0
	and r1, r5
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	ldr r0, _080174CC @ =0x00000D64
	mul r1, r0
	add r0, r2, r1
	ldr r3, _080174D0 @ =0x0201930C
	add r4, r0, r3
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r7, r0, #0x14
	cmp r7, #0
	bne _0801749E
	b _08017A9E
_0801749E:
	add r0, r1, r3
	add r0, r0, r2
	str r0, [sp, #4]
	mov r0, sl
	lsl r0, r0, #0x10
	str r0, [sp, #8]
	ldr r1, [sp, #0]
	cmp r1, #0
	beq _08017554
	ldr r0, _080174D4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r2, _080174D8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _080174DC @ =0x0000014D
	cmp r1, r0
	beq _080174E4
	ldr r0, _080174E0 @ =0x0000048A
	cmp r1, r0
	beq _0801750C
	b _08017520
	.align 2, 0
_080174CC: .4byte 0x00000D64
_080174D0: .4byte 0x0201930C
_080174D4: .4byte 0x000007FF
_080174D8: .4byte gUnk_08622AB4
_080174DC: .4byte 0x0000014D
_080174E0: .4byte 0x0000048A
_080174E4:
	mov r0, #2
	ldrb r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	beq _08017520
	mov r3, sl
	cmp r3, #0xA
	bne _08017554
	ldr r4, _08017508 @ =0x0000058F
	add r0, r6, #0
	add r1, r4, #0
	bl sub_080184D8
	sub r0, r5, r6
	add r1, r4, #0
	bl sub_080184D8
	b _08017520
_08017508: .4byte 0x0000058F
_0801750C:
	mov r0, #0x8F
	cmp r6, #0
	beq _08017514
	ldr r0, _08017584 @ =0x0000808F
_08017514:
	ldr r2, [sp, #8]
	lsr r1, r2, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08017520:
	mov r3, sl
	cmp r3, #0xA
	bne _08017554
	mov r0, #1
	and r0, r6
	ldr r1, _08017588 @ =0x00000D64
	mul r1, r0
	mov r0, #0xB9
	lsl r0, r0, #3
	add r1, r1, r0
	ldr r0, _0801758C @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08017554
	mov r0, #0x11
	cmp r6, #0
	beq _0801754A
	ldr r0, _08017590 @ =0x00008011
_0801754A:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08017554:
	ldr r4, _08017594 @ =0x00000453
	mov r0, #0
	add r1, r4, #0
	bl sub_080086CC
	cmp r0, #0
	bgt _0801756E
	mov r0, #1
	add r1, r4, #0
	bl sub_080086CC
	cmp r0, #0
	ble _0801759C
_0801756E:
	mov r0, #0x7A
	cmp r6, #0
	beq _08017576
	ldr r0, _08017598 @ =0x0000807A
_08017576:
	ldr r2, [sp, #8]
	lsr r1, r2, #0x10
	mov r2, r8
	mov r3, #0
	bl sub_0801EC58
	b _08017618
_08017584: .4byte 0x0000808F
_08017588: .4byte 0x00000D64
_0801758C: .4byte 0x0201930C
_08017590: .4byte 0x00008011
_08017594: .4byte 0x00000453
_08017598: .4byte 0x0000807A
_0801759C:
	ldr r3, _08017624 @ =0x000007FF
	and r3, r7
	lsl r0, r3, #2
	ldr r1, _08017628 @ =0x08621DE0
	add r4, r0, r1
	ldr r0, [r4]
	mov r5, #0xF8
	lsl r5, r5, #0x11
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _080175C8
	ldr r2, _0801762C @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08017630 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #8
	ldrb r2, [r1, #0xB]
	orr r0, r2
	strb r0, [r1, #0xB]
_080175C8:
	lsl r0, r3, #1
	ldr r3, _08017634 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	mov r0, #0xEF
	lsl r0, r0, #1
	cmp r1, r0
	beq _080175DE
	ldr r0, _08017638 @ =0x00000412
	cmp r1, r0
	bne _08017644
_080175DE:
	mov r0, #0x73
	cmp r6, #0
	beq _080175E6
	ldr r0, _0801763C @ =0x00008073
_080175E6:
	add r1, r7, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x81
	cmp r6, #0
	beq _080175F8
	ldr r0, _08017640 @ =0x00008081
_080175F8:
	ldr r2, [sp, #8]
	lsr r1, r2, #0x10
	mov r2, r8
	mov r3, #0
	bl sub_0801EC58
	ldr r0, [r4]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08017610
	b _08017A9E
_08017610:
	mov r3, sl
	cmp r3, #4
	ble _08017618
	b _08017A9E
_08017618:
	add r0, r6, #0
	mov r1, sl
	mov r2, #1
	bl sub_08017DE0
	b _08017A9E
_08017624: .4byte 0x000007FF
_08017628: .4byte gUnk_08621DE0
_0801762C: .4byte 0x020192E4
_08017630: .4byte 0x00000D64
_08017634: .4byte gUnk_08622AB4
_08017638: .4byte 0x00000412
_0801763C: .4byte 0x00008073
_08017640: .4byte 0x00008081
_08017644:
	mov r0, #0x79
	cmp r6, #0
	beq _0801764C
	ldr r0, _08017680 @ =0x00008079
_0801764C:
	ldr r2, [sp, #8]
	lsr r1, r2, #0x10
	mov r2, r8
	mov r3, #0
	bl sub_0801EC58
	ldr r0, [r4]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08017684
	cmp r0, #0x16
	bne _0801769C
	mov r0, #1
	sub r0, r0, r6
	lsl r4, r6, #0x18
	mov r3, sl
	lsl r5, r3, #0x18
	lsr r2, r4, #8
	orr r2, r5
	lsr r2, r2, #0x10
	mov r1, #0x14
	bl sub_08042AB0
	b _080176B2
	.align 2, 0
_08017680: .4byte 0x00008079
_08017684:
	mov r0, #1
	sub r0, r0, r6
	lsl r4, r6, #0x18
	mov r1, sl
	lsl r5, r1, #0x18
	lsr r2, r4, #8
	orr r2, r5
	lsr r2, r2, #0x10
	mov r1, #0x15
	bl sub_08042AB0
	b _080176B2
_0801769C:
	mov r0, #1
	sub r0, r0, r6
	lsl r4, r6, #0x18
	mov r2, sl
	lsl r5, r2, #0x18
	lsr r2, r4, #8
	orr r2, r5
	lsr r2, r2, #0x10
	mov r1, #0x1E
	bl sub_08042AB0
_080176B2:
	mov r8, r4
	mov r9, r5
	ldr r0, _080176D0 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r3, _080176D4 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _080176D8 @ =0x00000447
	cmp r1, r0
	beq _0801771C
	cmp r1, r0
	bgt _080176DC
	sub r0, #0x1B
	b _080176E6
_080176D0: .4byte 0x000007FF
_080176D4: .4byte gUnk_08622AB4
_080176D8: .4byte 0x00000447
_080176DC:
	mov r0, #0x91
	lsl r0, r0, #3
	cmp r1, r0
	beq _080176F0
	ldr r0, _080176EC @ =0x000005EA
_080176E6:
	cmp r1, r0
	beq _08017784
	b _080177AA
_080176EC: .4byte 0x000005EA
_080176F0:
	add r0, r6, #0
	mov r1, sl
	bl sub_08009538
	lsl r5, r0, #0x10
	lsr r4, r5, #0x10
	ldr r0, _08017718 @ =0x0000FFFF
	cmp r4, r0
	beq _080177AA
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _080177AA
	mov r2, r8
	lsr r1, r2, #8
	mov r3, r9
	orr r1, r3
	lsr r1, r1, #0x10
	add r0, r4, #0
	mov r2, #1
	b _08017762
_08017718: .4byte 0x0000FFFF
_0801771C:
	add r0, r6, #0
	mov r1, sl
	bl sub_08009538
	lsl r5, r0, #0x10
	lsr r4, r5, #0x10
	ldr r0, _08017778 @ =0x0000FFFF
	cmp r4, r0
	beq _080177AA
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _080177AA
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mov r1, sl
	mul r1, r0
	ldr r0, _0801777C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08017780 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080177AA
	mov r2, r8
	lsr r1, r2, #8
	mov r3, r9
	orr r1, r3
	lsr r1, r1, #0x10
	add r0, r4, #0
	mov r2, #2
_08017762:
	bl sub_08009424
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	lsr r1, r5, #0x18
	mov r2, #1
	mov r3, #1
	bl sub_08017460
	b _080177AA
	.align 2, 0
_08017778: .4byte 0x0000FFFF
_0801777C: .4byte 0x00000D64
_08017780: .4byte 0x0201930C
_08017784:
	add r0, r6, #0
	mov r1, sl
	bl sub_08009538
	lsl r1, r0, #0x10
	lsr r4, r1, #0x10
	ldr r0, _080177E4 @ =0x0000FFFF
	cmp r4, r0
	beq _080177AA
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #0x18
	mov r3, r8
	lsr r2, r3, #8
	mov r3, r9
	orr r2, r3
	lsr r2, r2, #0x10
	bl sub_08017314
_080177AA:
	add r0, r6, #0
	mov r1, #1
	bl sub_08046C20
	ldr r0, [sp, #0]
	cmp r0, #0
	bne _080177BA
	b _08017A9E
_080177BA:
	ldr r0, _080177E8 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _080177EC @ =0x08622AB4
	add r0, r0, r1
	ldrh r4, [r0]
	ldr r0, _080177F0 @ =0x000001CD
	cmp r4, r0
	bne _080177CE
	b _080178BC
_080177CE:
	cmp r4, r0
	bgt _0801780C
	sub r0, #0x97
	cmp r4, r0
	beq _0801788A
	cmp r4, r0
	bgt _080177F4
	cmp r4, #0x2F
	beq _08017860
	sub r0, #7
	b _08017806
_080177E4: .4byte 0x0000FFFF
_080177E8: .4byte 0x000007FF
_080177EC: .4byte gUnk_08622AB4
_080177F0: .4byte 0x000001CD
_080177F4:
	mov r0, #0x9C
	lsl r0, r0, #1
	cmp r4, r0
	bge _080177FE
	b _08017A94
_080177FE:
	add r0, #1
	cmp r4, r0
	ble _0801788A
	add r0, #7
_08017806:
	cmp r4, r0
	beq _0801788A
	b _08017A94
_0801780C:
	ldr r0, _08017828 @ =0x0000048A
	cmp r4, r0
	bne _08017814
	b _08017A80
_08017814:
	cmp r4, r0
	bgt _08017840
	sub r0, #0xD0
	cmp r4, r0
	bne _08017820
	b _08017908
_08017820:
	cmp r4, r0
	bgt _08017830
	ldr r0, _0801782C @ =0x0000023D
	b _0801784C
_08017828: .4byte 0x0000048A
_0801782C: .4byte 0x0000023D
_08017830:
	ldr r0, _0801783C @ =0x0000045C
	cmp r4, r0
	bne _08017838
	b _08017A38
_08017838:
	b _08017A94
	.align 2, 0
_0801783C: .4byte 0x0000045C
_08017840:
	ldr r0, _08017854 @ =0x000004DA
	cmp r4, r0
	beq _08017880
	cmp r4, r0
	bgt _08017858
	sub r0, #1
_0801784C:
	cmp r4, r0
	beq _08017860
	b _08017A94
	.align 2, 0
_08017854: .4byte 0x000004DA
_08017858:
	ldr r0, _0801787C @ =0x000004E9
	cmp r4, r0
	beq _08017860
	b _08017A94
_08017860:
	ldr r2, [sp, #4]
	ldr r1, [r2]
	mov r0, #0xE0
	lsl r0, r0, #9
	and r0, r1
	cmp r0, #0
	bne _08017876
	mov r3, sl
	cmp r3, #4
	bgt _08017876
	b _08017A94
_08017876:
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	b _08017A62
_0801787C: .4byte 0x000004E9
_08017880:
	ldr r1, [sp, #4]
	ldr r0, [r1]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	b _08017A62
_0801788A:
	ldr r2, [sp, #4]
	ldr r0, [r2]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	lsl r1, r0, #0x1F
	ldr r0, _080178A4 @ =0x02017A40
	ldr r3, _080178A8 @ =0x0000048A
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, #0x13
	bne _080178B0
	ldr r0, _080178AC @ =0x26600000
	b _080178B2
_080178A4: .4byte 0x02017A40
_080178A8: .4byte 0x0000048A
_080178AC: .4byte 0x26600000
_080178B0:
	ldr r0, _080178B8 @ =0x28600000
_080178B2:
	orr r0, r7
	orr r0, r1
	b _08017A6A
_080178B8: .4byte 0x28600000
_080178BC:
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mov r1, sl
	mul r1, r0
	ldr r0, _080178F8 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080178FC @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	beq _080178DC
	b _08017A94
_080178DC:
	mov r0, #0x73
	cmp r6, #0
	beq _080178E4
	ldr r0, _08017900 @ =0x00008073
_080178E4:
	add r1, r7, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r1, _08017904 @ =0x00001388
	add r0, r6, #0
	bl sub_08019860
	b _08017A94
_080178F8: .4byte 0x00000D64
_080178FC: .4byte 0x0201930C
_08017900: .4byte 0x00008073
_08017904: .4byte 0x00001388
_08017908:
	mov r1, #1
	and r1, r6
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	ldr r0, _08017A1C @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
	add r2, r2, r5
	ldr r0, _08017A20 @ =0x0201930C
	add r1, r2, r0
	mov r0, #2
	ldrb r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	bne _0801792A
	b _08017A94
_0801792A:
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08017938
	b _08017A94
_08017938:
	mov r0, #0x73
	cmp r6, #0
	beq _08017940
	ldr r0, _08017A24 @ =0x00008073
_08017940:
	add r1, r7, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	add r0, r6, #0
	add r1, r4, #0
	mov r2, sl
	bl sub_0800849C
	mov r3, sl
	lsl r3, r3, #1
	mov r8, r3
	cmp r0, #0
	bne _080179E8
	mov r4, #0
_08017960:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r5
	ldr r1, _08017A20 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08017996
	ldr r2, _08017A28 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _08017A2C @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_0800756C
	cmp r0, #0
	beq _08017996
	add r0, r6, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #1
	bl sub_08017460
_08017996:
	add r4, #1
	cmp r4, #4
	ble _08017960
	mov r4, #0
	mov r0, #1
	sub r5, r0, r6
	add r1, r5, #0
	and r1, r0
	ldr r0, _08017A1C @ =0x00000D64
	add r7, r1, #0
	mul r7, r0
_080179AC:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08017A20 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _080179E2
	ldr r2, _08017A28 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _08017A2C @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_0800756C
	cmp r0, #0
	beq _080179E2
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #1
	bl sub_08017460
_080179E2:
	add r4, #1
	cmp r4, #4
	ble _080179AC
_080179E8:
	ldr r2, _08017A30 @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08017A1C @ =0x00000D64
	mul r0, r1
	add r0, r8
	add r2, #0xE
	add r1, r0, r2
	ldrh r0, [r1]
	cmp r0, #0
	beq _08017A94
	add r1, r0, #0
	add r0, r6, #0
	bl sub_08019980
	mov r0, #0xB3
	cmp r6, #0
	beq _08017A0E
	ldr r0, _08017A34 @ =0x000080B3
_08017A0E:
	ldr r2, [sp, #8]
	lsr r1, r2, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _08017A94
_08017A1C: .4byte 0x00000D64
_08017A20: .4byte 0x0201930C
_08017A24: .4byte 0x00008073
_08017A28: .4byte 0x000007FF
_08017A2C: .4byte gUnk_08622AB4
_08017A30: .4byte 0x020192E4
_08017A34: .4byte 0x000080B3
_08017A38:
	mov r3, #1
	add r2, r6, #0
	and r2, r3
	mov r0, #0x94
	mov r1, sl
	mul r1, r0
	ldr r0, _08017A74 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08017A78 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _08017A94
	ldr r1, [sp, #4]
	ldr r0, [r1]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	and r0, r3
_08017A62:
	lsl r0, r0, #0x1F
	ldr r1, _08017A7C @ =0x3C600000
	orr r7, r1
	orr r0, r7
_08017A6A:
	mov r1, #0
	bl sub_0801FBCC
	b _08017A94
	.align 2, 0
_08017A74: .4byte 0x00000D64
_08017A78: .4byte 0x0201930C
_08017A7C: .4byte 0x3C600000
_08017A80:
	mov r0, #0x8F
	cmp r6, #0
	beq _08017A88
	ldr r0, _08017AB0 @ =0x0000808F
_08017A88:
	ldr r2, [sp, #8]
	lsr r1, r2, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08017A94:
	add r0, r6, #0
	mov r1, sl
	mov r2, #1
	bl sub_08017DE0
_08017A9E:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017AB0: .4byte 0x0000808F
	thumb_func_end sub_08017460

