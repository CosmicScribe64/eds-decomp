	thumb_func_start sub_08058514
sub_08058514: @ 0x08058514
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r2, r0, #0
	cmp r2, #0
	bne _08058524
	b _08058916
_08058524:
	ldr r0, _0805856C @ =0x000007FF
	ldrh r1, [r2]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08058570 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08058574 @ =0x0000047B
	cmp r1, r0
	bne _0805853A
	b _08058912
_0805853A:
	cmp r1, r0
	bgt _080585EC
	sub r0, #0x73
	cmp r1, r0
	bne _08058546
	b _08058710
_08058546:
	cmp r1, r0
	bgt _0805859C
	sub r0, #0x5D
	cmp r1, r0
	bne _08058552
	b _080586A4
_08058552:
	cmp r1, r0
	bgt _08058578
	mov r0, #0xAA
	lsl r0, r0, #2
	cmp r1, r0
	bge _08058560
	b _08058916
_08058560:
	add r0, #1
	cmp r1, r0
	bgt _08058568
	b _08058912
_08058568:
	add r0, #4
	b _0805866E
_0805856C: .4byte 0x000007FF
_08058570: .4byte gUnk_08622AB4
_08058574: .4byte 0x0000047B
_08058578:
	ldr r0, _08058588 @ =0x000003CA
	cmp r1, r0
	bne _08058580
	b _08058912
_08058580:
	cmp r1, r0
	bgt _0805858C
	sub r0, #0xA
	b _0805866E
_08058588: .4byte 0x000003CA
_0805858C:
	ldr r0, _08058598 @ =0x000003DE
	cmp r1, r0
	bne _08058594
	b _08058912
_08058594:
	add r0, #0xC
	b _0805866E
_08058598: .4byte 0x000003DE
_0805859C:
	ldr r0, _080585C0 @ =0x00000447
	cmp r1, r0
	bne _080585A4
	b _08058912
_080585A4:
	cmp r1, r0
	bgt _080585D4
	sub r0, #0x27
	cmp r1, r0
	bne _080585B0
	b _0805878C
_080585B0:
	cmp r1, r0
	bgt _080585C4
	sub r0, #0x12
	cmp r1, r0
	bne _080585BC
	b _0805874C
_080585BC:
	b _08058916
	.align 2, 0
_080585C0: .4byte 0x00000447
_080585C4:
	ldr r0, _080585D0 @ =0x00000444
	cmp r1, r0
	bne _080585CC
	b _0805876C
_080585CC:
	b _08058916
	.align 2, 0
_080585D0: .4byte 0x00000444
_080585D4:
	ldr r0, _080585E8 @ =0x0000044A
	cmp r1, r0
	bge _080585DC
	b _08058916
_080585DC:
	add r0, #1
	cmp r1, r0
	bgt _080585E4
	b _08058912
_080585E4:
	add r0, #0x2A
	b _08058640
_080585E8: .4byte 0x0000044A
_080585EC:
	ldr r0, _0805861C @ =0x0000052B
	cmp r1, r0
	bne _080585F4
	b _08058912
_080585F4:
	cmp r1, r0
	bgt _08058654
	sub r0, #0x77
	cmp r1, r0
	bgt _08058630
	sub r0, #1
	cmp r1, r0
	blt _08058606
	b _08058912
_08058606:
	sub r0, #0x34
	cmp r1, r0
	bne _0805860E
	b _08058912
_0805860E:
	cmp r1, r0
	bgt _08058620
	sub r0, #1
	cmp r1, r0
	bne _0805861A
	b _080587E8
_0805861A:
	b _08058916
_0805861C: .4byte 0x0000052B
_08058620:
	ldr r0, _0805862C @ =0x0000049A
	cmp r1, r0
	bne _08058628
	b _08058894
_08058628:
	b _08058916
	.align 2, 0
_0805862C: .4byte 0x0000049A
_08058630:
	ldr r0, _08058650 @ =0x000004BE
	cmp r1, r0
	bne _08058638
	b _080588CC
_08058638:
	cmp r1, r0
	bge _0805863E
	b _08058916
_0805863E:
	add r0, #0x5A
_08058640:
	cmp r1, r0
	ble _08058646
	b _08058916
_08058646:
	sub r0, #1
	cmp r1, r0
	bge _0805864E
	b _08058916
_0805864E:
	b _08058912
_08058650: .4byte 0x000004BE
_08058654:
	mov r0, #0xB2
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08058676
	sub r0, #1
	cmp r1, r0
	blt _08058664
	b _08058912
_08058664:
	sub r0, #0x62
	cmp r1, r0
	bne _0805866C
	b _08058912
_0805866C:
	add r0, #0x5A
_0805866E:
	cmp r1, r0
	bne _08058674
	b _08058912
_08058674:
	b _08058916
_08058676:
	ldr r0, _0805868C @ =0x000005A7
	cmp r1, r0
	bne _0805867E
	b _08058912
_0805867E:
	cmp r1, r0
	bgt _08058690
	sub r0, #0x16
	cmp r1, r0
	bne _0805868A
	b _08058908
_0805868A:
	b _08058916
_0805868C: .4byte 0x000005A7
_08058690:
	mov r0, #0xBF
	lsl r0, r0, #3
	cmp r1, r0
	bne _0805869A
	b _08058912
_0805869A:
	add r0, #8
	cmp r1, r0
	beq _080586A2
	b _08058916
_080586A2:
	b _08058912
_080586A4:
	mov r4, #1
	neg r4, r4
	mov r0, #1
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl sub_080573D0
	add r5, r0, #0
	cmp r5, r4
	ble _080586CE
	ldr r1, _08058708 @ =0x020192E4
	ldr r2, _0805870C @ =0x00000D64
	add r0, r1, r2
	ldrh r0, [r0]
	cmp r5, r0
	bge _080586CE
	ldrh r1, [r1]
	cmp r5, r1
	ble _080586CE
	b _08058912
_080586CE:
	mov r4, #1
	neg r4, r4
	mov r0, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl sub_080573D0
	add r5, r0, #0
	cmp r5, r4
	bgt _080586E6
	b _08058916
_080586E6:
	ldr r2, _08058708 @ =0x020192E4
	ldr r3, _0805870C @ =0x00000D64
	add r0, r2, r3
	ldrh r1, [r0]
	cmp r5, r1
	blt _080586F4
	b _08058916
_080586F4:
	ldrh r0, [r2]
	cmp r5, r0
	ble _080586FC
	b _08058912
_080586FC:
	sub r0, r0, r5
	cmp r0, r1
	bge _08058704
	b _08058912
_08058704:
	b _08058916
	.align 2, 0
_08058708: .4byte 0x020192E4
_0805870C: .4byte 0x00000D64
_08058710:
	mov r0, #0
	bl sub_08008860
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r0, _08058748 @ =0x020192E4
	ldrh r0, [r0]
	cmp r1, r0
	blt _0805872A
	b _08058912
_0805872A:
	mov r0, #0
	bl sub_08008860
	add r4, r0, #0
	bl sub_08076F9C
	mov r1, #3
	bl __modsi3
	add r0, #1
	cmp r4, r0
	ble _08058744
	b _08058912
_08058744:
	b _08058916
	.align 2, 0
_08058748: .4byte 0x020192E4
_0805874C:
	ldr r1, _08058764 @ =0x000004C5
	mov r0, #1
	bl sub_08056E04
	cmp r0, #0
	beq _0805875A
	b _08058912
_0805875A:
	ldr r1, _08058768 @ =0x0000042F
	mov r0, #1
	bl sub_08056E04
	b _0805890E
_08058764: .4byte 0x000004C5
_08058768: .4byte 0x0000042F
_0805876C:
	mov r0, #0
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
	add r4, r0, #0
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
	add r4, r4, r0
	cmp r4, #2
	ble _0805878A
	b _08058912
_0805878A:
	b _08058916
_0805878C:
	ldrh r4, [r2, #6]
	mov r1, #0
	add r0, r4, #0
	bl sub_0800C894
	ldr r1, _080587E0 @ =0x020192E4
	ldr r2, _080587E4 @ =0x00000D64
	add r1, r1, r2
	ldrh r1, [r1]
	cmp r0, r1
	blt _080587A4
	b _08058912
_080587A4:
	mov r0, #1
	bl sub_08008860
	cmp r0, #0
	bne _080587BA
	mov r0, #0
	bl sub_08008860
	cmp r0, #1
	ble _080587BA
	b _08058912
_080587BA:
	add r0, r4, #0
	bl sub_0805761C
	add r5, r0, #0
	ldr r0, _080587E0 @ =0x020192E4
	ldr r3, _080587E4 @ =0x00000D64
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r5, r0
	blt _080587D0
	b _08058912
_080587D0:
	mov r0, #1
	sub r0, r0, r4
	bl sub_0805761C
	cmp r5, r0
	blt _080587DE
	b _08058912
_080587DE:
	b _08058916
_080587E0: .4byte 0x020192E4
_080587E4: .4byte 0x00000D64
_080587E8:
	mov r5, #0
	mov r4, #0
	mov r0, #1
	mov r9, r0
	ldr r1, _08058884 @ =0x00000D64
	mov r8, r1
_080587F4:
	mov r6, #0
	add r7, r4, #1
	mov r2, r9
	and r4, r2
	mov r3, r8
	mul r3, r4
	add r4, r3, #0
_08058802:
	mov r0, #0x94
	mul r0, r6
	add r0, r0, r4
	ldr r1, _08058888 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08058820
	bl sub_08007730
	cmp r0, #0
	beq _08058820
	add r5, #1
_08058820:
	add r6, #1
	cmp r6, #4
	ble _08058802
	add r4, r7, #0
	cmp r4, #1
	ble _080587F4
	ldr r1, _0805888C @ =0x020192E4
	lsl r0, r5, #5
	sub r0, r0, r5
	lsl r0, r0, #2
	add r0, r0, r5
	lsl r0, r0, #2
	ldrh r1, [r1]
	cmp r1, r0
	blt _08058912
	mov r5, #0
	mov r4, #0
	mov r6, #4
_08058844:
	ldr r0, _08058888 @ =0x0201930C
	add r0, r4, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0805885C
	bl sub_08007730
	cmp r0, #0
	beq _0805885C
	add r5, #1
_0805885C:
	ldr r0, _08058890 @ =0x0201A070
	add r0, r4, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08058874
	bl sub_08007730
	cmp r0, #0
	beq _08058874
	sub r5, #1
_08058874:
	add r4, #0x94
	sub r6, #1
	cmp r6, #0
	bge _08058844
	cmp r5, #1
	bgt _08058912
	b _08058916
	.align 2, 0
_08058884: .4byte 0x00000D64
_08058888: .4byte 0x0201930C
_0805888C: .4byte 0x020192E4
_08058890: .4byte 0x0201A070
_08058894:
	mov r4, #0
_08058896:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _080588C8 @ =0x0201A070
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080588BE
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080588BE
	mov r0, #1
	add r1, r4, #0
	bl sub_08009298
	cmp r0, #0
	bgt _08058912
_080588BE:
	add r4, #1
	cmp r4, #4
	ble _08058896
	b _08058916
	.align 2, 0
_080588C8: .4byte 0x0201A070
_080588CC:
	ldrh r4, [r2, #6]
	mov r1, #0
	add r0, r4, #0
	bl sub_0800C894
	add r5, r0, #0
	ldr r1, _08058900 @ =0x020192E4
	ldrh r2, [r1]
	lsr r0, r2, #1
	cmp r5, r0
	bge _08058912
	ldr r3, _08058904 @ =0x00000D64
	add r0, r1, r3
	ldrh r1, [r0]
	cmp r5, r1
	bge _08058912
	sub r0, r2, r5
	cmp r0, r1
	bgt _08058912
	mov r0, #0
	bl sub_08008860
	cmp r0, #1
	beq _08058912
	b _08058916
	.align 2, 0
_08058900: .4byte 0x020192E4
_08058904: .4byte 0x00000D64
_08058908:
	mov r0, #0
	bl sub_08008860
_0805890E:
	cmp r0, #0
	beq _08058916
_08058912:
	mov r0, #1
	b _08058918
_08058916:
	mov r0, #0
_08058918:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08058514

