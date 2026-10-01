	thumb_func_start sub_0801401C
sub_0801401C: @ 0x0801401C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1C
	ldr r0, _0801405C @ =0x020185C0
	ldrh r1, [r0]
	lsr r7, r1, #0xF
	ldr r2, _08014060 @ =0x0000080A
	add r4, r0, r2
	ldrb r3, [r4]
	lsl r0, r3, #0x19
	cmp r0, #0
	bne _08014064
	add r0, r7, #0
	mov r1, #0
	bl sub_080240A8
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _080146D4
	.align 2, 0
_0801405C: .4byte 0x020185C0
_08014060: .4byte 0x0000080A
_08014064:
	mov r4, #0
	mov r8, r4
	add r1, r7, #0
	mov r5, #1
	and r1, r5
	ldr r5, _080143EC @ =0x0201930C
	mov r4, #0xF0
	lsl r4, r4, #2
	ldr r6, _080143F0 @ =0x00001AE6
	add r6, r6, r5
	mov sl, r6
	ldr r0, _080143F4 @ =0x00000D64
	add r2, r1, #0
	mul r2, r0
	mov r9, r2
_08014082:
	mov r0, #0x94
	mov r3, r8
	mul r3, r0
	add r0, r3, #0
	add r0, r9
	add r3, r0, r5
	ldrh r2, [r3, #6]
	add r0, r4, #0
	and r0, r2
	cmp r0, #0
	beq _080140D6
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #6
	ldr r6, _080143F8 @ =0xFFFFFC3F
	add r1, r6, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3, #6]
	ldr r1, _080143FC @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _080140C6
	mov r0, #2
	mov r1, sl
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _080140D6
_080140C6:
	and r2, r4
	cmp r2, #0
	bne _080140D6
	add r0, r7, #0
	mov r1, r8
	mov r2, #1
	bl sub_08018544
_080140D6:
	mov r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #4
	ble _08014082
	ldr r4, _08014400 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r5, _080143F4 @ =0x00000D64
	mov r9, r5
	mov r2, r9
	mul r2, r0
	add r2, r2, r4
	mov r6, #0x21
	neg r6, r6
	mov r8, r6
	mov r0, r8
	ldrb r1, [r2, #7]
	and r0, r1
	add r6, #0x18
	and r0, r6
	mov r5, #0x11
	neg r5, r5
	and r0, r5
	strb r0, [r2, #7]
	mov r1, #2
	neg r1, r1
	ldrb r3, [r2, #8]
	and r1, r3
	mov r0, #3
	neg r0, r0
	and r1, r0
	mov r3, #5
	neg r3, r3
	and r1, r3
	strb r1, [r2, #8]
	ldr r2, _08014404 @ =0x00001AC9
	add r1, r4, r2
	ldrb r2, [r1]
	and r0, r2
	and r0, r3
	and r0, r6
	and r0, r5
	mov r3, r8
	and r0, r3
	mov r2, #0x41
	neg r2, r2
	and r0, r2
	strb r0, [r1]
	mov r5, #0
	mov r8, r5
	mov ip, r4
	mov sl, r9
	mov r6, #0
	str r6, [sp, #0x14]
	mov r4, #1
	mov r7, sl
	mov r9, ip
	ldr r0, _08014408 @ =0x00000B86
	add r0, r9
	str r0, [sp, #0x10]
	mov r1, #0
	str r1, [sp, #0x18]
_08014154:
	mov r5, #0
	mov r0, r8
	mov r3, #1
	and r0, r3
	mov r6, sl
	mul r6, r0
	add r0, r6, #0
	add r0, ip
	ldrb r0, [r0, #2]
	cmp r5, r0
	bge _08014192
	mov r0, r8
	and r0, r4
	add r1, r0, #0
	mul r1, r7
	ldr r0, _0801440C @ =0x02019968
	mov r6, r9
	add r3, r1, r6
	add r0, #2
	add r1, r1, r0
_0801417C:
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	and r0, r2
	strb r0, [r1]
	add r1, #4
	add r5, #1
	ldrb r0, [r3, #2]
	cmp r5, r0
	blt _0801417C
_08014192:
	mov r5, #0
	mov r0, r8
	mov r1, #1
	and r0, r1
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	add r0, ip
	ldrb r0, [r0, #3]
	cmp r5, r0
	bge _080141D0
	mov r0, r8
	and r0, r4
	add r1, r0, #0
	mul r1, r7
	ldr r0, _08014410 @ =0x02019AA8
	mov r6, r9
	add r3, r1, r6
	add r0, #2
	add r1, r1, r0
_080141BA:
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	and r0, r2
	strb r0, [r1]
	add r1, #4
	add r5, #1
	ldrb r0, [r3, #3]
	cmp r5, r0
	blt _080141BA
_080141D0:
	mov r5, #0
	mov r0, r8
	mov r1, #1
	and r0, r1
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	add r0, ip
	ldrb r0, [r0, #5]
	cmp r5, r0
	bge _0801420E
	mov r0, r8
	and r0, r4
	add r1, r0, #0
	mul r1, r7
	ldr r0, _08014414 @ =0x02019D28
	mov r6, r9
	add r3, r1, r6
	add r0, #2
	add r1, r1, r0
_080141F8:
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	and r0, r2
	strb r0, [r1]
	add r1, #4
	add r5, #1
	ldrb r0, [r3, #5]
	cmp r5, r0
	blt _080141F8
_0801420E:
	mov r5, #0
	mov r0, r8
	mov r1, #1
	and r0, r1
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	add r0, ip
	ldrb r0, [r0, #4]
	cmp r5, r0
	bge _0801424C
	mov r0, r8
	and r0, r4
	add r1, r0, #0
	mul r1, r7
	ldr r0, _08014418 @ =0x02019BE8
	mov r6, r9
	add r3, r1, r6
	add r0, #2
	add r1, r1, r0
_08014236:
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	and r0, r2
	strb r0, [r1]
	add r1, #4
	add r5, #1
	ldrb r0, [r3, #4]
	cmp r5, r0
	blt _08014236
_0801424C:
	mov r5, #0
	ldr r1, [sp, #0x14]
	ldr r3, _0801441C @ =0x020192E0
	add r0, r1, r3
	ldrb r0, [r0, #0xA]
	cmp r5, r0
	bge _0801427E
	mov r0, r8
	and r0, r4
	mul r0, r7
	ldr r6, [sp, #0x18]
	add r3, r6, r3
	ldr r6, [sp, #0x10]
	add r1, r0, r6
_08014268:
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	and r0, r2
	strb r0, [r1]
	add r1, #4
	add r5, #1
	ldrb r0, [r3, #0xA]
	cmp r5, r0
	blt _08014268
_0801427E:
	ldr r1, [sp, #0x14]
	add r1, r1, r7
	str r1, [sp, #0x14]
	ldr r0, _080143F4 @ =0x00000D64
	ldr r3, [sp, #0x18]
	add r3, r3, r0
	str r3, [sp, #0x18]
	mov r5, #1
	add r8, r5
	mov r6, r8
	cmp r6, #1
	bgt _08014298
	b _08014154
_08014298:
	mov r5, #0
	mov r1, #1
	mov sl, r1
	mov r9, r0
	ldr r7, _080143EC @ =0x0201930C
	ldr r6, _08014420 @ =0x000007FF
_080142A4:
	mov r2, #5
	mov r8, r2
	add r4, r5, #1
	mov r3, sl
	and r5, r3
	mov r0, r9
	mul r0, r5
	add r3, r0, r7
_080142B4:
	mov r0, #0x94
	mov r5, r8
	mul r5, r0
	add r0, r5, #0
	add r2, r3, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080142F4
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08014424 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _080142F4
	mov r0, #2
	ldrb r5, [r2, #6]
	and r0, r5
	cmp r0, #0
	bne _080142F4
	add r1, r2, #0
	add r1, #0x91
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_080142F4:
	mov r5, #1
	add r8, r5
	mov r0, r8
	cmp r0, #9
	ble _080142B4
	add r5, r4, #0
	cmp r5, #1
	ble _080142A4
	mov r7, #0
	mov r4, #1
	mov r2, #0x21
	neg r2, r2
	ldr r3, _080143EC @ =0x0201930C
_0801430E:
	add r1, r7, #1
	str r1, [sp, #0xC]
	and r7, r4
	ldr r0, _080143F4 @ =0x00000D64
	mul r0, r7
	mov r5, #4
	mov r8, r5
	add r0, #0x8C
	add r1, r0, r3
_08014320:
	add r0, r2, #0
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
	add r1, #0x94
	mov r0, #1
	neg r0, r0
	add r8, r0
	mov r5, r8
	cmp r5, #0
	bge _08014320
	ldr r7, [sp, #0xC]
	cmp r7, #1
	ble _0801430E
	mov r0, #1
	ldr r6, _080143FC @ =0x02015EE8
	ldrb r6, [r6, #1]
	and r0, r6
	cmp r0, #0
	beq _0801435A
	ldr r0, _0801441C @ =0x020192E0
	ldr r2, _08014428 @ =0x00001B12
	add r1, r0, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801435A
	b _080146C4
_0801435A:
	mov r7, #0
_0801435C:
	mov r3, #0
	mov r8, r3
	add r4, r7, #1
	str r4, [sp, #0xC]
	add r1, r7, #0
	mov r5, #1
	and r1, r5
	lsl r0, r7, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
	sub r0, r5, r7
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	ldr r0, _080143F4 @ =0x00000D64
	add r6, r1, #0
	mul r6, r0
	str r6, [sp, #0]
_08014380:
	mov r4, #0
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	ldr r2, [sp, #0]
	add r0, r0, r2
	ldr r1, _080143EC @ =0x0201930C
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r9, r0
	mov r3, #1
	add r3, r8
	mov sl, r3
	cmp r0, #0
	bne _080143A6
	b _080146B2
_080143A6:
	ldr r5, _08014420 @ =0x000007FF
	add r0, r5, #0
	mov r1, r9
	and r1, r0
	lsl r1, r1, #1
	ldr r6, _0801442C @ =0x08622AB4
	add r1, r1, r6
	mov r0, #0x8B
	lsl r0, r0, #3
	ldrh r1, [r1]
	cmp r1, r0
	bne _080143CC
	ldr r1, _08014430 @ =0x00002002
	add r0, r1, #0
	ldrh r2, [r2, #6]
	and r0, r2
	cmp r0, #2
	bne _080143CC
	mov r4, #1
_080143CC:
	add r0, r7, #0
	mov r1, r8
	ldr r2, _08014434 @ =0x00000522
	bl sub_0800A78C
	cmp r0, #0
	beq _080143DC
	mov r4, #1
_080143DC:
	add r0, r7, #0
	mov r1, r8
	mov r2, #0xBD
	lsl r2, r2, #3
	bl sub_0800A78C
	b _08014438
	.align 2, 0
_080143EC: .4byte 0x0201930C
_080143F0: .4byte 0x00001AE6
_080143F4: .4byte 0x00000D64
_080143F8: .4byte 0xFFFFFC3F
_080143FC: .4byte 0x02015EE8
_08014400: .4byte 0x020192E4
_08014404: .4byte 0x00001AC9
_08014408: .4byte 0x00000B86
_0801440C: .4byte 0x02019968
_08014410: .4byte 0x02019AA8
_08014414: .4byte 0x02019D28
_08014418: .4byte 0x02019BE8
_0801441C: .4byte 0x020192E0
_08014420: .4byte 0x000007FF
_08014424: .4byte gUnk_08621DE0
_08014428: .4byte 0x00001B12
_0801442C: .4byte gUnk_08622AB4
_08014430: .4byte 0x00002002
_08014434: .4byte 0x00000522
_08014438:
	cmp r0, #0
	beq _0801443E
	mov r4, #1
_0801443E:
	cmp r4, #0
	beq _08014454
	add r0, r7, #0
	mov r1, r8
	mov r2, #1
	bl sub_08018544
	mov r2, #1
	add r2, r8
	mov sl, r2
	b _080146B2
_08014454:
	mov r3, #0
	mov r5, #0
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	ldr r4, [sp, #0]
	add r0, r2, r4
	ldr r1, _080144D8 @ =0x0201930C
	add r0, r0, r1
	add r0, #0x8A
	mov r6, #1
	add r6, r8
	mov sl, r6
	ldrh r0, [r0]
	cmp r5, r0
	bge _08014498
	mov r0, #1
	and r0, r7
	ldr r4, _080144DC @ =0x00000D64
	mul r0, r4
	add r0, r2, r0
	add r0, r0, r1
	add r1, r0, #0
	add r1, #0x8A
	ldrh r1, [r1]
	add r0, #0x4A
_08014488:
	ldrb r6, [r0]
	cmp r6, #4
	bne _08014490
	mov r3, #1
_08014490:
	add r0, #2
	add r5, #1
	cmp r5, r1
	blt _08014488
_08014498:
	cmp r3, #0
	beq _080144B0
	mov r0, r8
	lsl r2, r0, #0x18
	lsr r2, r2, #0x10
	ldr r1, [sp, #4]
	orr r2, r1
	add r0, r7, #0
	mov r1, #0
	mov r3, #4
	bl sub_08017ADC
_080144B0:
	mov r5, #0
	lsl r0, r7, #0x18
	lsr r6, r0, #0x18
_080144B6:
	ldr r1, _080144E0 @ =0x08198DCC
	lsl r0, r5, #1
	add r4, r0, r1
	ldrh r2, [r4]
	add r0, r7, #0
	mov r1, r8
	bl sub_0800A78C
	cmp r0, #0
	beq _08014534
	ldrh r1, [r4]
	add r2, r1, #0
	ldr r0, _080144E4 @ =0x0000FFFF
	cmp r1, r0
	bne _080144E8
	mov r0, #0
	b _0801451E
_080144D8: .4byte 0x0201930C
_080144DC: .4byte 0x00000D64
_080144E0: .4byte gUnk_08198DCC
_080144E4: .4byte 0x0000FFFF
_080144E8:
	ldr r0, _08014500 @ =0x000007CF
	cmp r1, r0
	bhi _0801450C
	ldr r2, _08014504 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _08014508 @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	b _0801451E
	.align 2, 0
_08014500: .4byte 0x000007CF
_08014504: .4byte 0x000007FF
_08014508: .4byte gUnk_08623DF4
_0801450C:
	ldr r4, _08014564 @ =0xFFFFF830
	add r0, r2, r4
	ldr r1, _08014568 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0801456C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_0801451E:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	mov r2, r8
	lsl r0, r2, #0x18
	lsl r2, r6, #0x10
	orr r2, r0
	add r0, r7, #0
	lsr r2, r2, #0x10
	mov r3, #3
	bl sub_08017ADC
_08014534:
	add r5, #1
	cmp r5, #0xB
	bls _080144B6
	ldr r4, _08014570 @ =0x00000403
	add r0, r7, #0
	mov r1, r8
	add r2, r4, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _080145B8
	mov r3, #1
	sub r0, r3, r7
	bl sub_08008A44
	cmp r0, #0
	bge _08014574
	add r0, r7, #0
	mov r1, r8
	mov r2, #1
	bl sub_08018544
	b _080145B8
	.align 2, 0
_08014564: .4byte 0xFFFFF830
_08014568: .4byte 0x000007FF
_0801456C: .4byte gUnk_08623DF4
_08014570: .4byte 0x00000403
_08014574:
	mov r1, #0x82
	cmp r7, #0
	beq _0801457C
	ldr r1, _080145EC @ =0x00008082
_0801457C:
	mov r6, r8
	lsl r5, r6, #0x18
	lsr r5, r5, #0x10
	ldr r2, [sp, #4]
	orr r5, r2
	lsl r6, r0, #0x18
	lsr r6, r6, #0x10
	ldr r3, [sp, #8]
	orr r6, r3
	add r0, r1, #0
	add r1, r5, #0
	add r2, r6, #0
	mov r3, #0
	bl sub_0801EC58
	lsl r4, r4, #1
	ldr r0, _080145F0 @ =0x08623DF4
	add r4, r4, r0
	ldrh r1, [r4]
	add r0, r7, #0
	add r2, r5, #0
	mov r3, #3
	bl sub_08017ADC
	ldrh r1, [r4]
	add r0, r7, #0
	add r2, r6, #0
	mov r3, #3
	bl sub_08017ADC
_080145B8:
	ldr r0, _080145F4 @ =0x020185C0
	ldrh r0, [r0]
	lsr r0, r0, #0xF
	cmp r7, r0
	bne _08014640
	ldr r4, _080145F8 @ =0x000004E7
	add r0, r7, #0
	mov r1, r8
	add r2, r4, #0
	bl sub_0800A78C
	cmp r0, #0
	beq _08014640
	mov r1, #1
	sub r0, r1, r7
	bl sub_08008A44
	cmp r0, #0
	bge _080145FC
	add r0, r7, #0
	mov r1, r8
	mov r2, #1
	bl sub_08018544
	b _08014640
	.align 2, 0
_080145EC: .4byte 0x00008082
_080145F0: .4byte gUnk_08623DF4
_080145F4: .4byte 0x020185C0
_080145F8: .4byte 0x000004E7
_080145FC:
	mov r1, #0x82
	cmp r7, #0
	beq _08014604
	ldr r1, _080146E4 @ =0x00008082
_08014604:
	mov r2, r8
	lsl r5, r2, #0x18
	lsr r5, r5, #0x10
	ldr r3, [sp, #4]
	orr r5, r3
	lsl r6, r0, #0x18
	lsr r6, r6, #0x10
	ldr r0, [sp, #8]
	orr r6, r0
	add r0, r1, #0
	add r1, r5, #0
	add r2, r6, #0
	mov r3, #0
	bl sub_0801EC58
	lsl r4, r4, #1
	ldr r1, _080146E8 @ =0x08623DF4
	add r4, r4, r1
	ldrh r1, [r4]
	add r0, r7, #0
	add r2, r5, #0
	mov r3, #3
	bl sub_08017ADC
	ldrh r1, [r4]
	add r0, r7, #0
	add r2, r6, #0
	mov r3, #3
	bl sub_08017ADC
_08014640:
	ldr r2, _080146EC @ =0x000007FF
	add r0, r2, #0
	mov r1, r9
	and r1, r0
	lsl r1, r1, #1
	ldr r3, _080146F0 @ =0x08622AB4
	add r1, r1, r3
	ldr r0, _080146F4 @ =0x000002F9
	ldrh r1, [r1]
	cmp r1, r0
	bne _080146B2
	ldr r0, _080146F8 @ =0x020192E4
	ldr r4, [sp, #0]
	add r0, r4, r0
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1C
	cmp r0, #0
	bge _080146B2
	add r0, r7, #0
	bl sub_08008A1C
	cmp r0, #0
	ble _080146B2
	add r0, r7, #0
	bl sub_08008A44
	add r4, r0, #0
	add r0, r7, #0
	mov r1, r9
	bl sub_080197E0
	mov r2, #0x71
	cmp r7, #0
	beq _08014686
	ldr r2, _080146FC @ =0x00008071
_08014686:
	ldr r0, _08014700 @ =0x08624CF4
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r2, #0xA3
	cmp r7, #0
	beq _0801469C
	ldr r2, _08014704 @ =0x000080A3
_0801469C:
	lsl r1, r4, #0x18
	mov r5, r8
	lsl r0, r5, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_080146B2:
	mov r8, sl
	mov r6, r8
	cmp r6, #4
	bgt _080146BC
	b _08014380
_080146BC:
	ldr r7, [sp, #0xC]
	cmp r7, #1
	bgt _080146C4
	b _0801435C
_080146C4:
	ldr r1, _08014708 @ =0x020185C0
	ldr r0, _0801470C @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080146D4:
	add sp, #0x1C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080146E4: .4byte 0x00008082
_080146E8: .4byte gUnk_08623DF4
_080146EC: .4byte 0x000007FF
_080146F0: .4byte gUnk_08622AB4
_080146F4: .4byte 0x000002F9
_080146F8: .4byte 0x020192E4
_080146FC: .4byte 0x00008071
_08014700: .4byte gUnk_08624CF4
_08014704: .4byte 0x000080A3
_08014708: .4byte 0x020185C0
_0801470C: .4byte 0x0000080D
	thumb_func_end sub_0801401C

