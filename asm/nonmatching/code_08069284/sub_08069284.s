	thumb_func_start sub_08069284
sub_08069284: @ 0x08069284
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #4]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #8]
	mov r7, #0
	mov r0, #0
	str r0, [sp, #0x10]
	bl sub_080686E8
	ldr r1, [sp, #0]
	cmp r1, #1
	beq _080692E0
	cmp r1, #1
	bgt _080692BC
	cmp r1, #0
	beq _080692C4
	b _08069306
_080692BC:
	ldr r2, [sp, #0]
	cmp r2, #2
	beq _080692F8
	b _08069306
_080692C4:
	ldr r3, _080692D4 @ =0x0201EFC4
	ldr r5, _080692D8 @ =0xFFFFF1A0
	add r3, r3, r5
	mov r9, r3
	ldr r0, _080692DC @ =0xFFFFF8C8
	ldr r6, _080692D4 @ =0x0201EFC4
	add r0, r6, r0
	b _08069304
_080692D4: .4byte 0x0201EFC4
_080692D8: .4byte 0xFFFFF1A0
_080692DC: .4byte 0xFFFFF8C8
_080692E0:
	ldr r1, _080692F0 @ =0x0201EFC4
	ldr r2, _080692F4 @ =0xFFFFF80A
	add r1, r1, r2
	mov r9, r1
	ldr r3, _080692F0 @ =0x0201EFC4
	sub r3, #0xCE
	str r3, [sp, #0xC]
	b _08069306
_080692F0: .4byte 0x0201EFC4
_080692F4: .4byte 0xFFFFF80A
_080692F8:
	ldr r5, _08069330 @ =0x0201EFC4
	ldr r6, _08069334 @ =0xFFFFF8AA
	add r5, r5, r6
	mov r9, r5
	ldr r0, _08069330 @ =0x0201EFC4
	sub r0, #0x2E
_08069304:
	str r0, [sp, #0xC]
_08069306:
	ldr r0, _08069338 @ =0x0201DB20
	ldr r1, [sp, #0]
	lsl r4, r1, #1
	ldr r2, _0806933C @ =0x00001494
	add r0, r0, r2
	add r0, r4, r0
	ldrh r2, [r0]
	mov r0, r9
	ldr r1, [sp, #0xC]
	bl CpuSet
	mov r8, r4
	ldr r3, [sp, #4]
	cmp r3, #7
	bls _08069326
	b _08069906
_08069326:
	lsl r0, r3, #2
	ldr r1, _08069340 @ =0x08069344
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08069330: .4byte 0x0201EFC4
_08069334: .4byte 0xFFFFF8AA
_08069338: .4byte 0x0201DB20
_0806933C: .4byte 0x00001494
_08069340: .4byte 0x08069344
_08069344:
	.4byte _08069364
	.4byte _080693F8
	.4byte _08069504
	.4byte _08069610
	.4byte _0806971C
	.4byte _0806977C
	.4byte _080697DC
	.4byte _080698B8
_08069364:
	ldr r5, [sp, #8]
	cmp r5, #0
	bne _0806936C
	b _08069906
_0806936C:
	mov r4, #0
	ldr r0, _080693C8 @ =0x0201DB20
	ldr r6, _080693CC @ =0x00001494
	add r0, r0, r6
	add r0, r8
	ldrh r1, [r0]
	cmp r4, r1
	bcc _0806937E
	b _08069906
_0806937E:
	ldr r2, _080693D0 @ =0x000007FF
	mov sl, r2
	mov ip, r0
	lsl r0, r7, #1
	ldr r3, [sp, #0xC]
	add r5, r0, r3
_0806938A:
	lsl r2, r4, #1
	mov r6, r9
	add r0, r2, r6
	ldrh r3, [r0]
	add r0, r3, #0
	mov r1, sl
	and r0, r1
	lsl r0, r0, #2
	ldr r6, _080693D4 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r6, r2, #0
	cmp r0, #0x18
	bgt _080693DC
	cmp r0, #0x15
	blt _080693DC
	ldr r1, [sp, #0x10]
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	lsl r1, r1, #1
	ldr r0, _080693D8 @ =0x0201EFC4
	add r1, r1, r0
	strh r3, [r1]
	b _080693E8
	.align 2, 0
_080693C8: .4byte 0x0201DB20
_080693CC: .4byte 0x00001494
_080693D0: .4byte 0x000007FF
_080693D4: .4byte gUnk_08621DE0
_080693D8: .4byte 0x0201EFC4
_080693DC:
	mov r1, r9
	add r0, r6, r1
	ldrh r0, [r0]
	strh r0, [r5]
	add r5, #2
	add r7, #1
_080693E8:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r2, ip
	ldrh r2, [r2]
	cmp r4, r2
	bcc _0806938A
	b _08069906
_080693F8:
	mov r4, #0
	ldr r1, _08069458 @ =0x0201DB20
	ldr r3, _0806945C @ =0x00001494
	add r0, r1, r3
	add r0, r8
	ldrh r0, [r0]
	cmp r4, r0
	bcc _0806940A
	b _08069906
_0806940A:
	ldr r5, _08069460 @ =0x000007FF
	mov ip, r5
	mov r6, r8
	str r6, [sp, #0x14]
	lsl r0, r7, #1
	ldr r1, [sp, #0xC]
	add r5, r0, r1
	mov r2, #0xF8
	lsl r2, r2, #0x11
	mov sl, r2
_0806941E:
	lsl r1, r4, #1
	mov r3, r9
	add r0, r1, r3
	ldrh r2, [r0]
	add r3, r2, #0
	mov r6, ip
	and r3, r6
	lsl r0, r3, #2
	ldr r6, _08069464 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r6, sl
	and r0, r6
	lsr r0, r0, #0x14
	add r6, r1, #0
	cmp r0, #0x16
	bgt _08069444
	cmp r0, #0x15
	bge _080694DE
_08069444:
	lsl r0, r3, #1
	ldr r1, _08069468 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0806946C @ =0x00000776
	cmp r1, r0
	bne _08069470
	mov r0, #3
	b _080694CE
	.align 2, 0
_08069458: .4byte 0x0201DB20
_0806945C: .4byte 0x00001494
_08069460: .4byte 0x000007FF
_08069464: .4byte gUnk_08621DE0
_08069468: .4byte gUnk_08622AB4
_0806946C: .4byte 0x00000776
_08069470:
	cmp r1, r0
	blt _08069480
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08069480
	mov r0, #1
	b _080694CE
_08069480:
	add r0, r2, #0
	mov r3, ip
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080694A4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r3, sl
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080694AE
	cmp r0, #0x16
	bgt _080694A8
	cmp r0, #0x15
	beq _080694B2
	b _080694BA
	.align 2, 0
_080694A4: .4byte gUnk_08621DE0
_080694A8:
	cmp r0, #0x17
	beq _080694B6
	b _080694BA
_080694AE:
	mov r0, #7
	b _080694CE
_080694B2:
	mov r0, #8
	b _080694CE
_080694B6:
	mov r0, #9
	b _080694CE
_080694BA:
	mov r0, ip
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _080694F8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080694CE:
	cmp r0, #0
	bne _080694DE
	mov r2, r9
	add r0, r6, r2
	ldrh r0, [r0]
	strh r0, [r5]
	add r5, #2
	add r7, #1
_080694DE:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r3, _080694FC @ =0x0201DB20
	ldr r6, _08069500 @ =0x00001494
	add r0, r3, r6
	ldr r1, [sp, #0x14]
	add r0, r1, r0
	ldrh r0, [r0]
	cmp r4, r0
	bcc _0806941E
	b _08069906
	.align 2, 0
_080694F8: .4byte gUnk_08621DE0
_080694FC: .4byte 0x0201DB20
_08069500: .4byte 0x00001494
_08069504:
	mov r4, #0
	ldr r1, _08069564 @ =0x0201DB20
	ldr r2, _08069568 @ =0x00001494
	add r0, r1, r2
	add r0, r8
	ldrh r0, [r0]
	cmp r4, r0
	bcc _08069516
	b _08069906
_08069516:
	ldr r5, _0806956C @ =0x000007FF
	mov ip, r5
	mov r6, r8
	str r6, [sp, #0x18]
	lsl r0, r7, #1
	ldr r1, [sp, #0xC]
	add r5, r0, r1
	mov r2, #0xF8
	lsl r2, r2, #0x11
	mov sl, r2
_0806952A:
	lsl r1, r4, #1
	mov r3, r9
	add r0, r1, r3
	ldrh r2, [r0]
	add r3, r2, #0
	mov r6, ip
	and r3, r6
	lsl r0, r3, #2
	ldr r6, _08069570 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r6, sl
	and r0, r6
	lsr r0, r0, #0x14
	add r6, r1, #0
	cmp r0, #0x16
	bgt _08069550
	cmp r0, #0x15
	bge _080695EA
_08069550:
	lsl r0, r3, #1
	ldr r1, _08069574 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08069578 @ =0x00000776
	cmp r1, r0
	bne _0806957C
	mov r0, #3
	b _080695DA
	.align 2, 0
_08069564: .4byte 0x0201DB20
_08069568: .4byte 0x00001494
_0806956C: .4byte 0x000007FF
_08069570: .4byte gUnk_08621DE0
_08069574: .4byte gUnk_08622AB4
_08069578: .4byte 0x00000776
_0806957C:
	cmp r1, r0
	blt _0806958C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806958C
	mov r0, #1
	b _080695DA
_0806958C:
	add r0, r2, #0
	mov r3, ip
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080695B0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r3, sl
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080695BA
	cmp r0, #0x16
	bgt _080695B4
	cmp r0, #0x15
	beq _080695BE
	b _080695C6
	.align 2, 0
_080695B0: .4byte gUnk_08621DE0
_080695B4:
	cmp r0, #0x17
	beq _080695C2
	b _080695C6
_080695BA:
	mov r0, #7
	b _080695DA
_080695BE:
	mov r0, #8
	b _080695DA
_080695C2:
	mov r0, #9
	b _080695DA
_080695C6:
	mov r0, ip
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _08069604 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080695DA:
	cmp r0, #1
	bne _080695EA
	mov r2, r9
	add r0, r6, r2
	ldrh r0, [r0]
	strh r0, [r5]
	add r5, #2
	add r7, #1
_080695EA:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r3, _08069608 @ =0x0201DB20
	ldr r6, _0806960C @ =0x00001494
	add r0, r3, r6
	ldr r1, [sp, #0x18]
	add r0, r1, r0
	ldrh r0, [r0]
	cmp r4, r0
	bcc _0806952A
	b _08069906
	.align 2, 0
_08069604: .4byte gUnk_08621DE0
_08069608: .4byte 0x0201DB20
_0806960C: .4byte 0x00001494
_08069610:
	mov r4, #0
	ldr r1, _08069670 @ =0x0201DB20
	ldr r2, _08069674 @ =0x00001494
	add r0, r1, r2
	add r0, r8
	ldrh r0, [r0]
	cmp r4, r0
	bcc _08069622
	b _08069906
_08069622:
	ldr r5, _08069678 @ =0x000007FF
	mov ip, r5
	mov r6, r8
	str r6, [sp, #0x1C]
	lsl r0, r7, #1
	ldr r1, [sp, #0xC]
	add r5, r0, r1
	mov r2, #0xF8
	lsl r2, r2, #0x11
	mov sl, r2
_08069636:
	lsl r1, r4, #1
	mov r3, r9
	add r0, r1, r3
	ldrh r2, [r0]
	add r3, r2, #0
	mov r6, ip
	and r3, r6
	lsl r0, r3, #2
	ldr r6, _0806967C @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	mov r6, sl
	and r0, r6
	lsr r0, r0, #0x14
	add r6, r1, #0
	cmp r0, #0x16
	bgt _0806965C
	cmp r0, #0x15
	bge _080696F6
_0806965C:
	lsl r0, r3, #1
	ldr r1, _08069680 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08069684 @ =0x00000776
	cmp r1, r0
	bne _08069688
	mov r0, #3
	b _080696E6
	.align 2, 0
_08069670: .4byte 0x0201DB20
_08069674: .4byte 0x00001494
_08069678: .4byte 0x000007FF
_0806967C: .4byte gUnk_08621DE0
_08069680: .4byte gUnk_08622AB4
_08069684: .4byte 0x00000776
_08069688:
	cmp r1, r0
	blt _08069698
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08069698
	mov r0, #1
	b _080696E6
_08069698:
	add r0, r2, #0
	mov r3, ip
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080696BC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r3, sl
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080696C6
	cmp r0, #0x16
	bgt _080696C0
	cmp r0, #0x15
	beq _080696CA
	b _080696D2
	.align 2, 0
_080696BC: .4byte gUnk_08621DE0
_080696C0:
	cmp r0, #0x17
	beq _080696CE
	b _080696D2
_080696C6:
	mov r0, #7
	b _080696E6
_080696CA:
	mov r0, #8
	b _080696E6
_080696CE:
	mov r0, #9
	b _080696E6
_080696D2:
	mov r0, ip
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _08069710 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080696E6:
	cmp r0, #2
	bne _080696F6
	mov r2, r9
	add r0, r6, r2
	ldrh r0, [r0]
	strh r0, [r5]
	add r5, #2
	add r7, #1
_080696F6:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r3, _08069714 @ =0x0201DB20
	ldr r6, _08069718 @ =0x00001494
	add r0, r3, r6
	ldr r1, [sp, #0x1C]
	add r0, r1, r0
	ldrh r0, [r0]
	cmp r4, r0
	bcc _08069636
	b _08069906
	.align 2, 0
_08069710: .4byte gUnk_08621DE0
_08069714: .4byte 0x0201DB20
_08069718: .4byte 0x00001494
_0806971C:
	mov r4, #0
	ldr r0, _0806976C @ =0x0201DB20
	ldr r2, _08069770 @ =0x00001494
	add r0, r0, r2
	add r0, r8
	ldrh r3, [r0]
	cmp r4, r3
	bcc _0806972E
	b _08069906
_0806972E:
	ldr r6, _08069774 @ =0x000007FF
	add r5, r0, #0
	lsl r0, r7, #1
	ldr r1, [sp, #0xC]
	add r3, r0, r1
_08069738:
	lsl r0, r4, #1
	add r0, r9
	ldrh r2, [r0]
	add r0, r2, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08069778 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0806975C
	strh r2, [r3]
	add r3, #2
	add r7, #1
_0806975C:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldrh r2, [r5]
	cmp r4, r2
	bcc _08069738
	b _08069906
	.align 2, 0
_0806976C: .4byte 0x0201DB20
_08069770: .4byte 0x00001494
_08069774: .4byte 0x000007FF
_08069778: .4byte gUnk_08621DE0
_0806977C:
	mov r4, #0
	ldr r0, _080697CC @ =0x0201DB20
	ldr r3, _080697D0 @ =0x00001494
	add r0, r0, r3
	add r0, r8
	ldrh r5, [r0]
	cmp r4, r5
	bcc _0806978E
	b _08069906
_0806978E:
	ldr r6, _080697D4 @ =0x000007FF
	add r5, r0, #0
	lsl r0, r7, #1
	ldr r1, [sp, #0xC]
	add r3, r0, r1
_08069798:
	lsl r0, r4, #1
	add r0, r9
	ldrh r2, [r0]
	add r0, r2, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _080697D8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _080697BC
	strh r2, [r3]
	add r3, #2
	add r7, #1
_080697BC:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldrh r2, [r5]
	cmp r4, r2
	bcc _08069798
	b _08069906
	.align 2, 0
_080697CC: .4byte 0x0201DB20
_080697D0: .4byte 0x00001494
_080697D4: .4byte 0x000007FF
_080697D8: .4byte gUnk_08621DE0
_080697DC:
	mov r4, #0
	ldr r0, _08069820 @ =0x0201DB20
	ldr r3, _08069824 @ =0x00001494
	add r0, r0, r3
	mov r5, r8
	add r1, r5, r0
	ldrh r6, [r1]
	cmp r4, r6
	bcc _080697F0
	b _08069906
_080697F0:
	ldr r0, _08069828 @ =0x000007FF
	mov ip, r0
	ldr r2, _0806982C @ =0x00000776
	mov sl, r2
	lsl r0, r7, #1
	ldr r3, [sp, #0xC]
	add r5, r0, r3
	str r1, [sp, #0x20]
_08069800:
	lsl r1, r4, #1
	mov r6, r9
	add r0, r1, r6
	ldrh r2, [r0]
	add r0, r2, #0
	mov r3, ip
	and r0, r3
	lsl r0, r0, #1
	ldr r6, _08069830 @ =0x08622AB4
	add r0, r0, r6
	ldrh r3, [r0]
	add r6, r1, #0
	cmp r3, sl
	bne _08069834
	mov r0, #3
	b _08069892
_08069820: .4byte 0x0201DB20
_08069824: .4byte 0x00001494
_08069828: .4byte 0x000007FF
_0806982C: .4byte 0x00000776
_08069830: .4byte gUnk_08622AB4
_08069834:
	cmp r3, sl
	blt _08069844
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r3, r0
	bgt _08069844
	mov r0, #1
	b _08069892
_08069844:
	add r0, r2, #0
	mov r1, ip
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08069868 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08069872
	cmp r0, #0x16
	bgt _0806986C
	cmp r0, #0x15
	beq _08069876
	b _0806987E
_08069868: .4byte gUnk_08621DE0
_0806986C:
	cmp r0, #0x17
	beq _0806987A
	b _0806987E
_08069872:
	mov r0, #7
	b _08069892
_08069876:
	mov r0, #8
	b _08069892
_0806987A:
	mov r0, #9
	b _08069892
_0806987E:
	mov r0, ip
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _080698B4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08069892:
	cmp r0, #3
	bne _080698A2
	mov r2, r9
	add r0, r6, r2
	ldrh r0, [r0]
	strh r0, [r5]
	add r5, #2
	add r7, #1
_080698A2:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r3, [sp, #0x20]
	ldrh r3, [r3]
	cmp r4, r3
	bcc _08069800
	b _08069906
	.align 2, 0
_080698B4: .4byte gUnk_08621DE0
_080698B8:
	mov r4, #0
	ldr r0, _08069928 @ =0x0201DB20
	ldr r5, _0806992C @ =0x00001494
	add r0, r0, r5
	add r0, r8
	ldrh r6, [r0]
	cmp r4, r6
	bcs _08069906
	ldr r6, _08069930 @ =0x000007FF
	add r5, r0, #0
	lsl r0, r7, #1
	ldr r1, [sp, #0xC]
	add r3, r0, r1
_080698D2:
	lsl r0, r4, #1
	add r0, r9
	ldrh r2, [r0]
	add r0, r2, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08069934 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x18
	bgt _080698F4
	cmp r0, #0x15
	bge _080698FA
_080698F4:
	strh r2, [r3]
	add r3, #2
	add r7, #1
_080698FA:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldrh r2, [r5]
	cmp r4, r2
	bcc _080698D2
_08069906:
	ldr r3, [sp, #4]
	cmp r3, #0
	bne _0806994C
	ldr r5, [sp, #8]
	cmp r5, #0
	bne _0806993C
	ldr r0, _08069928 @ =0x0201DB20
	ldr r6, _08069938 @ =0x0000149A
	add r2, r0, r6
	add r2, r8
	ldr r1, _0806992C @ =0x00001494
	add r0, r0, r1
	add r0, r8
	ldrh r1, [r0]
	strh r1, [r2]
	ldrh r7, [r0]
	b _08069956
_08069928: .4byte 0x0201DB20
_0806992C: .4byte 0x00001494
_08069930: .4byte 0x000007FF
_08069934: .4byte gUnk_08621DE0
_08069938: .4byte 0x0000149A
_0806993C:
	ldr r0, _08069944 @ =0x0201DB20
	ldr r2, _08069948 @ =0x0000149A
	add r0, r0, r2
	b _08069952
_08069944: .4byte 0x0201DB20
_08069948: .4byte 0x0000149A
_0806994C:
	ldr r0, _08069968 @ =0x0201DB20
	ldr r3, _0806996C @ =0x0000149A
	add r0, r0, r3
_08069952:
	add r0, r8
	strh r7, [r0]
_08069956:
	ldr r5, [sp, #8]
	cmp r5, #5
	bhi _08069A26
	lsl r0, r5, #2
	ldr r1, _08069970 @ =0x08069974
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08069968: .4byte 0x0201DB20
_0806996C: .4byte 0x0000149A
_08069970: .4byte 0x08069974
_08069974:
	.4byte _08069A16
	.4byte _0806998C
	.4byte _080699AC
	.4byte _080699CC
	.4byte _080699EC
	.4byte _08069A0C
_0806998C:
	ldr r2, _080699A4 @ =0x08068E45
	add r0, r7, #0
	ldr r1, [sp, #0xC]
	bl sub_080690C4
	ldr r0, _080699A8 @ =0x0201DB20
	mov r2, #0xA5
	lsl r2, r2, #5
	add r0, r0, r2
	ldr r3, [sp, #0]
	add r0, r3, r0
	b _08069A22
_080699A4: .4byte sub_08068E44
_080699A8: .4byte 0x0201DB20
_080699AC:
	ldr r2, _080699C4 @ =0x08068EFD
	add r0, r7, #0
	ldr r1, [sp, #0xC]
	bl sub_080690C4
	ldr r0, _080699C8 @ =0x0201DB20
	mov r5, #0xA5
	lsl r5, r5, #5
	add r0, r0, r5
	ldr r6, [sp, #0]
	add r0, r6, r0
	b _08069A22
_080699C4: .4byte sub_08068EFC
_080699C8: .4byte 0x0201DB20
_080699CC:
	ldr r2, _080699E4 @ =0x08068FBD
	add r0, r7, #0
	ldr r1, [sp, #0xC]
	bl sub_080690C4
	ldr r0, _080699E8 @ =0x0201DB20
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r0, r1
	ldr r2, [sp, #0]
	add r0, r2, r0
	b _08069A22
_080699E4: .4byte sub_08068FBC
_080699E8: .4byte 0x0201DB20
_080699EC:
	ldr r2, _08069A04 @ =0x08068FED
	add r0, r7, #0
	ldr r1, [sp, #0xC]
	bl sub_080690C4
	ldr r0, _08069A08 @ =0x0201DB20
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r0, r3
	ldr r5, [sp, #0]
	add r0, r5, r0
	b _08069A22
_08069A04: .4byte sub_08068FEC
_08069A08: .4byte 0x0201DB20
_08069A0C:
	ldr r2, _08069A80 @ =0x08069015
	add r0, r7, #0
	ldr r1, [sp, #0xC]
	bl sub_080690C4
_08069A16:
	ldr r0, _08069A84 @ =0x0201DB20
	mov r6, #0xA5
	lsl r6, r6, #5
	add r0, r0, r6
	ldr r1, [sp, #0]
	add r0, r1, r0
_08069A22:
	mov r1, #1
	strb r1, [r0]
_08069A26:
	ldr r3, _08069A84 @ =0x0201DB20
	ldr r2, [sp, #4]
	cmp r2, #0
	bne _08069A64
	ldr r5, [sp, #8]
	cmp r5, #0
	beq _08069A64
	mov r4, #0
	ldr r6, [sp, #0x10]
	add r2, r6, r7
	cmp r4, r6
	bcs _08069A5C
_08069A3E:
	add r1, r7, r4
	lsl r1, r1, #1
	ldr r0, [sp, #0xC]
	add r1, r1, r0
	lsl r0, r4, #1
	ldr r5, _08069A88 @ =0x0201EFC4
	add r0, r0, r5
	ldrh r0, [r0]
	strh r0, [r1]
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r6, [sp, #0x10]
	cmp r4, r6
	bcc _08069A3E
_08069A5C:
	ldr r1, _08069A8C @ =0x0000149A
	add r0, r3, r1
	add r0, r8
	strh r2, [r0]
_08069A64:
	mov r2, #0xC4
	lsl r2, r2, #3
	add r0, r3, r2
	add r0, r8
	mov r1, #0
	strh r1, [r0]
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08069A80: .4byte sub_08069014
_08069A84: .4byte 0x0201DB20
_08069A88: .4byte 0x0201EFC4
_08069A8C: .4byte 0x0000149A
	thumb_func_end sub_08069284

