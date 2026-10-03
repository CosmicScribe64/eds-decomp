	thumb_func_start DuelCursor_PickAny
DuelCursor_PickAny: @ 0x0805304C
	push {r4, r5, r6, r7, lr}
	ldr r0, _0805308C @ =0x03000040
	mov ip, r0
	ldrh r7, [r0, #6]
	ldr r2, _08053090 @ =0x0201CFB0
	ldr r1, _08053094 @ =0x00000824
	add r0, r2, r1
	ldr r4, [r0]
	ldr r3, _08053098 @ =0x00000828
	add r0, r2, r3
	ldr r6, [r0]
	add r1, #8
	add r0, r2, r1
	ldr r5, [r0]
	sub r3, #0x20
	add r1, r2, r3
	mov r0, #8
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	mov r0, #0x20
	and r0, r7
	mov r3, ip
	cmp r0, #0
	bne _08053080
	b _08053256
_08053080:
	cmp r4, #0
	beq _0805309C
	cmp r4, #1
	beq _0805317A
	b _08053256
	.align 2, 0
_0805308C: .4byte 0x03000040
_08053090: .4byte 0x0201CFB0
_08053094: .4byte 0x00000824
_08053098: .4byte 0x00000828
_0805309C:
	cmp r6, #0xF
	bls _080530A2
	b _080536BC
_080530A2:
	lsl r0, r6, #2
	ldr r1, _080530AC @ =0x080530B0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080530AC: .4byte 0x080530B0
_080530B0:
	.4byte _080530F0
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _08053108
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _0805315E
	.4byte _08053120
	.4byte _08053164
	.4byte _0805316A
	.4byte _08053172
	.4byte _08053172
_080530F0:
	cmp r5, #0
	ble _080530FC
	sub r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080530FC:
	add r0, r4, #0
	mov r1, #0xA
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_08053108:
	cmp r5, #0
	ble _08053114
	sub r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_08053114:
	add r0, r4, #0
	mov r1, #0xC
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_08053120:
	ldr r2, _08053140 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08053144 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r0, [r1, #2]
	cmp r0, #0
	beq _08053156
	cmp r5, #0
	ble _08053148
	sub r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
	.align 2, 0
_08053140: .4byte 0x020192E4
_08053144: .4byte 0x00000D64
_08053148:
	ldrb r2, [r1, #2]
	sub r2, #1
	add r0, r4, #0
	add r1, r6, #0
	bl DuelCursor_Select
	b _080536B6
_08053156:
	mov r0, #3
	bl PlaySE
	b _080536BC
_0805315E:
	add r0, r4, #0
	mov r1, #0xE
	b _080536B0
_08053164:
	add r0, r4, #0
	mov r1, #0xD
	b _080536B0
_0805316A:
	add r0, r4, #0
	mov r1, #5
	mov r2, #4
	b _080536B2
_08053172:
	add r0, r4, #0
	mov r1, #0
	mov r2, #4
	b _080536B2
_0805317A:
	cmp r6, #0xF
	bls _08053180
	b _080536BC
_08053180:
	lsl r0, r6, #2
	ldr r1, _0805318C @ =0x08053190
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805318C: .4byte 0x08053190
_08053190:
	.4byte _080531D0
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080531E8
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _08053240
	.4byte _08053200
	.4byte _08053246
	.4byte _0805324C
	.4byte _08053252
	.4byte _08053252
_080531D0:
	cmp r5, #3
	bgt _080531DC
	add r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080531DC:
	add r0, r4, #0
	mov r1, #0xE
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_080531E8:
	cmp r5, #3
	bgt _080531F4
	add r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080531F4:
	add r0, r4, #0
	mov r1, #0xD
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_08053200:
	ldr r2, _08053224 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08053228 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r0, [r1, #2]
	cmp r0, #0
	beq _08053238
	ldrb r0, [r1, #2]
	sub r0, #1
	cmp r5, r0
	bge _0805322C
	add r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
	.align 2, 0
_08053224: .4byte 0x020192E4
_08053228: .4byte 0x00000D64
_0805322C:
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_08053238:
	mov r0, #3
	bl PlaySE
	b _080536BC
_08053240:
	add r0, r4, #0
	mov r1, #0
	b _080536B0
_08053246:
	add r0, r4, #0
	mov r1, #5
	b _080536B0
_0805324C:
	add r0, r4, #0
	mov r1, #0xC
	b _080536B0
_08053252:
	add r0, r4, #0
	b _080536AE
_08053256:
	mov r0, #0x10
	and r0, r7
	cmp r0, #0
	bne _08053260
	b _08053436
_08053260:
	cmp r4, #0
	beq _0805326A
	cmp r4, #1
	beq _0805334E
	b _08053436
_0805326A:
	ldr r1, _08053280 @ =0x00000828
	add r0, r2, r1
	ldr r0, [r0]
	cmp r0, #0xF
	bls _08053276
	b _080536BC
_08053276:
	lsl r0, r0, #2
	ldr r1, _08053284 @ =0x08053288
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08053280: .4byte 0x00000828
_08053284: .4byte 0x08053288
_08053288:
	.4byte _080532C8
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080532E0
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _08053338
	.4byte _080532F8
	.4byte _0805333E
	.4byte _08053344
	.4byte _0805334A
	.4byte _0805334A
_080532C8:
	cmp r5, #3
	bgt _080532D4
	add r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080532D4:
	add r0, r4, #0
	mov r1, #0xE
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_080532E0:
	cmp r5, #3
	bgt _080532EC
	add r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080532EC:
	add r0, r4, #0
	mov r1, #0xD
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_080532F8:
	ldr r2, _0805331C @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08053320 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r0, [r1, #2]
	cmp r0, #0
	beq _08053330
	ldrb r0, [r1, #2]
	sub r0, #1
	cmp r5, r0
	bge _08053324
	add r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
	.align 2, 0
_0805331C: .4byte 0x020192E4
_08053320: .4byte 0x00000D64
_08053324:
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_08053330:
	mov r0, #3
	bl PlaySE
	b _080536BC
_08053338:
	add r0, r4, #0
	mov r1, #0
	b _080536B0
_0805333E:
	add r0, r4, #0
	mov r1, #5
	b _080536B0
_08053344:
	add r0, r4, #0
	mov r1, #0xC
	b _080536B0
_0805334A:
	add r0, r4, #0
	b _080536AE
_0805334E:
	ldr r3, _08053364 @ =0x00000828
	add r0, r2, r3
	ldr r0, [r0]
	cmp r0, #0xF
	bls _0805335A
	b _080536BC
_0805335A:
	lsl r0, r0, #2
	ldr r1, _08053368 @ =0x0805336C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08053364: .4byte 0x00000828
_08053368: .4byte 0x0805336C
_0805336C:
	.4byte _080533AC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080533C4
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _0805341A
	.4byte _080533DC
	.4byte _08053420
	.4byte _08053426
	.4byte _0805342E
	.4byte _0805342E
_080533AC:
	cmp r5, #0
	ble _080533B8
	sub r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080533B8:
	add r0, r4, #0
	mov r1, #0xA
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_080533C4:
	cmp r5, #0
	ble _080533D0
	sub r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
_080533D0:
	add r0, r4, #0
	mov r1, #0xC
	mov r2, #0
	bl DuelCursor_Select
	b _080536B6
_080533DC:
	ldr r2, _080533FC @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08053400 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r0, [r1, #2]
	cmp r0, #0
	beq _08053412
	cmp r5, #0
	ble _08053404
	sub r2, r5, #1
	add r0, r4, #0
	add r1, r6, #0
	b _080536B2
	.align 2, 0
_080533FC: .4byte 0x020192E4
_08053400: .4byte 0x00000D64
_08053404:
	ldrb r2, [r1, #2]
	sub r2, #1
	add r0, r4, #0
	add r1, r6, #0
	bl DuelCursor_Select
	b _080536B6
_08053412:
	mov r0, #3
	bl PlaySE
	b _080536BC
_0805341A:
	add r0, r4, #0
	mov r1, #0xE
	b _080536B0
_08053420:
	add r0, r4, #0
	mov r1, #0xD
	b _080536B0
_08053426:
	add r0, r4, #0
	mov r1, #5
	mov r2, #4
	b _080536B2
_0805342E:
	add r0, r4, #0
	mov r1, #0
	mov r2, #4
	b _080536B2
_08053436:
	mov r0, #0x40
	and r0, r7
	cmp r0, #0
	bne _08053440
	b _08053570
_08053440:
	cmp r4, #0
	beq _0805344A
	cmp r4, #1
	beq _080534DA
	b _08053570
_0805344A:
	ldr r1, _08053460 @ =0x00000828
	add r0, r2, r1
	ldr r0, [r0]
	cmp r0, #0xF
	bls _08053456
	b _080536BC
_08053456:
	lsl r0, r0, #2
	ldr r1, _08053464 @ =0x08053468
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08053460: .4byte 0x00000828
_08053464: .4byte 0x08053468
_08053468:
	.4byte _080534A8
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080534B4
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080534C2
	.4byte _080534BC
	.4byte _080534CA
	.4byte _080534CE
	.4byte _080534D4
	.4byte _080536AA
_080534A8:
	mov r0, #1
	sub r0, r0, r4
	mov r2, #4
	sub r2, r2, r5
	add r1, r6, #0
	b _080536B2
_080534B4:
	add r0, r4, #0
	mov r1, #0
	add r2, r5, #0
	b _080536B2
_080534BC:
	add r0, r4, #0
	mov r1, #5
	b _080536B0
_080534C2:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xF
	b _080536B0
_080534CA:
	add r0, r4, #0
	b _080536AE
_080534CE:
	add r0, r4, #0
	mov r1, #0xE
	b _080536B0
_080534D4:
	add r0, r4, #0
	mov r1, #0xF
	b _080536B0
_080534DA:
	ldr r3, _080534F0 @ =0x00000828
	add r0, r2, r3
	ldr r0, [r0]
	cmp r0, #0xF
	bls _080534E6
	b _080536BC
_080534E6:
	lsl r0, r0, #2
	ldr r1, _080534F4 @ =0x080534F8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080534F0: .4byte 0x00000828
_080534F4: .4byte 0x080534F8
_080534F8:
	.4byte _08053538
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _08053540
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _0805354E
	.4byte _08053546
	.4byte _08053554
	.4byte _0805355C
	.4byte _08053564
	.4byte _0805356A
_08053538:
	add r0, r4, #0
	mov r1, #5
	add r2, r5, #0
	b _080536B2
_08053540:
	add r0, r4, #0
	mov r1, #0xB
	b _080536B0
_08053546:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xB
	b _080536B0
_0805354E:
	add r0, r4, #0
	mov r1, #0xC
	b _080536B0
_08053554:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xD
	b _080536B0
_0805355C:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xC
	b _080536B0
_08053564:
	add r0, r4, #0
	mov r1, #0xD
	b _080536B0
_0805356A:
	add r0, r4, #0
	mov r1, #0xE
	b _080536B0
_08053570:
	mov r0, #0x80
	and r7, r0
	cmp r7, #0
	bne _0805357A
	b _080536C0
_0805357A:
	cmp r4, #0
	beq _08053584
	cmp r4, #1
	beq _0805361C
	b _080536C0
_08053584:
	ldr r1, _0805359C @ =0x00000828
	add r0, r2, r1
	ldr r0, [r0]
	cmp r0, #0xF
	bls _08053590
	b _080536BC
_08053590:
	lsl r0, r0, #2
	ldr r1, _080535A0 @ =0x080535A4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805359C: .4byte 0x00000828
_080535A0: .4byte 0x080535A4
_080535A4:
	.4byte _080535E4
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080535EC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080535FA
	.4byte _080535F2
	.4byte _08053600
	.4byte _08053608
	.4byte _08053610
	.4byte _08053616
_080535E4:
	add r0, r4, #0
	mov r1, #5
	add r2, r5, #0
	b _080536B2
_080535EC:
	add r0, r4, #0
	mov r1, #0xB
	b _080536B0
_080535F2:
	mov r0, #1
	sub r0, r0, r4
	add r1, r6, #0
	b _080536B0
_080535FA:
	add r0, r4, #0
	mov r1, #0xC
	b _080536B0
_08053600:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xD
	b _080536B0
_08053608:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xC
	b _080536B0
_08053610:
	add r0, r4, #0
	mov r1, #0xD
	b _080536B0
_08053616:
	add r0, r4, #0
	mov r1, #0xE
	b _080536B0
_0805361C:
	ldr r3, _08053630 @ =0x00000828
	add r0, r2, r3
	ldr r0, [r0]
	cmp r0, #0xF
	bhi _080536BC
	lsl r0, r0, #2
	ldr r1, _08053634 @ =0x08053638
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08053630: .4byte 0x00000828
_08053634: .4byte 0x08053638
_08053638:
	.4byte _08053678
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _08053684
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _080536BC
	.4byte _08053692
	.4byte _0805368C
	.4byte _0805369A
	.4byte _0805369E
	.4byte _080536A4
	.4byte _080536AA
_08053678:
	mov r0, #1
	sub r0, r0, r4
	mov r2, #4
	sub r2, r2, r5
	add r1, r6, #0
	b _080536B2
_08053684:
	add r0, r4, #0
	mov r1, #0
	add r2, r5, #0
	b _080536B2
_0805368C:
	add r0, r4, #0
	mov r1, #5
	b _080536B0
_08053692:
	mov r0, #1
	sub r0, r0, r4
	mov r1, #0xF
	b _080536B0
_0805369A:
	add r0, r4, #0
	b _080536AE
_0805369E:
	add r0, r4, #0
	mov r1, #0xE
	b _080536B0
_080536A4:
	add r0, r4, #0
	mov r1, #0xF
	b _080536B0
_080536AA:
	mov r0, #1
	sub r0, r0, r4
_080536AE:
	mov r1, #0xA
_080536B0:
	mov r2, #0
_080536B2:
	bl DuelCursor_Select
_080536B6:
	mov r0, #0
	bl PlaySE
_080536BC:
	mov r0, #0
	b _080536CC
_080536C0:
	mov r0, #1
	ldrh r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _080536BC
	mov r0, #1
_080536CC:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelCursor_PickAny
	.align 2, 0

