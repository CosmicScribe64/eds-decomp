	thumb_func_start AiPickMonsterToSet
AiPickMonsterToSet: @ 0x0805A30C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _0805A322
	b _0805A878
_0805A322:
	ldr r0, _0805A4EC @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _0805A3AE
	bl AiCountExodiaOnField
	cmp r0, #0
	beq _0805A348
	ldr r1, _0805A4F0 @ =0x00000259
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A348
	b _0805A87C
_0805A348:
	bl AiCountExodiaInDeck
	cmp r0, #0
	beq _0805A3AE
	mov r0, #1
	mov r1, #0x2F
	bl FindHandCardByNumber
	add r4, r0, #0
	mov r6, #1
	neg r6, r6
	cmp r4, r6
	ble _0805A364
	b _0805A87C
_0805A364:
	ldr r5, _0805A4F4 @ =0x0000023D
	mov r0, #1
	add r1, r5, #0
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r6
	ble _0805A376
	b _0805A87C
_0805A376:
	mov r0, #1
	mov r1, #0x2F
	bl CountMonstersByNumber
	cmp r0, #0
	bgt _0805A38E
	mov r0, #1
	add r1, r5, #0
	bl CountMonstersByNumber
	cmp r0, #0
	ble _0805A39E
_0805A38E:
	mov r0, #1
	add r1, r5, #0
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r6
	ble _0805A39E
	b _0805A87C
_0805A39E:
	ldr r1, _0805A4F8 @ =0x00000463
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A3AE
	b _0805A87C
_0805A3AE:
	mov r0, #0
	bl CountMonsters
	cmp r0, #1
	ble _0805A3C8
	ldr r1, _0805A4F0 @ =0x00000259
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A3C8
	b _0805A87C
_0805A3C8:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _0805A42A
	mov r1, #0xFA
	lsl r1, r1, #1
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	mov r5, #1
	neg r5, r5
	cmp r4, r5
	ble _0805A3E8
	b _0805A87C
_0805A3E8:
	mov r1, #0x87
	lsl r1, r1, #2
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A3FA
	b _0805A87C
_0805A3FA:
	ldr r1, _0805A4F0 @ =0x00000259
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A40A
	b _0805A87C
_0805A40A:
	mov r0, #1
	mov r1, #0xFF
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A41A
	b _0805A87C
_0805A41A:
	ldr r1, _0805A4FC @ =0x00000419
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A42A
	b _0805A87C
_0805A42A:
	mov r0, #1
	mov r1, #0x16
	bl CountGraveyardCardsOfType
	cmp r0, #0
	ble _0805A446
	ldr r1, _0805A500 @ =0x000001AB
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A446
	b _0805A87C
_0805A446:
	mov r0, #1
	mov r1, #0x15
	bl CountGraveyardCardsOfType
	cmp r0, #0
	ble _0805A462
	mov r0, #1
	mov r1, #0x65
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A462
	b _0805A87C
_0805A462:
	ldr r1, _0805A504 @ =0x020192E4
	ldr r2, _0805A508 @ =0x00000D66
	add r0, r1, r2
	ldrb r2, [r0]
	cmp r2, #2
	bls _0805A478
	add r0, r2, #0
	add r0, #2
	ldrb r1, [r1, #2]
	cmp r1, r0
	ble _0805A49C
_0805A478:
	ldr r1, _0805A50C @ =0x0000021B
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	mov r5, #1
	neg r5, r5
	cmp r4, r5
	ble _0805A48C
	b _0805A87C
_0805A48C:
	ldr r1, _0805A510 @ =0x0000024E
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A49C
	b _0805A87C
_0805A49C:
	ldr r0, _0805A504 @ =0x020192E4
	ldrb r0, [r0, #3]
	cmp r0, #4
	bhi _0805A4B4
	ldr r1, _0805A514 @ =0x00000231
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A4B4
	b _0805A87C
_0805A4B4:
	mov r7, #0
_0805A4B6:
	lsl r0, r7, #2
	ldr r1, _0805A518 @ =0x02019AA8
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0805A51C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805A520 @ =0x000002EF
	cmp r1, r0
	beq _0805A576
	cmp r1, r0
	bgt _0805A53C
	mov r0, #0xA8
	lsl r0, r0, #1
	cmp r1, r0
	bgt _0805A524
	sub r0, #1
	cmp r1, r0
	bge _0805A576
	cmp r1, #0x14
	bgt _0805A586
	cmp r1, #0x10
	blt _0805A586
	b _0805A576
	.align 2, 0
_0805A4EC: .4byte 0x02015EE8
_0805A4F0: .4byte 0x00000259
_0805A4F4: .4byte 0x0000023D
_0805A4F8: .4byte 0x00000463
_0805A4FC: .4byte 0x00000419
_0805A500: .4byte 0x000001AB
_0805A504: .4byte 0x020192E4
_0805A508: .4byte 0x00000D66
_0805A50C: .4byte 0x0000021B
_0805A510: .4byte 0x0000024E
_0805A514: .4byte 0x00000231
_0805A518: .4byte 0x02019AA8
_0805A51C: .4byte gCardIdToNumber
_0805A520: .4byte 0x000002EF
_0805A524:
	mov r0, #0xA4
	lsl r0, r0, #2
	cmp r1, r0
	beq _0805A576
	cmp r1, r0
	bgt _0805A534
	sub r0, #0xB6
	b _0805A56A
_0805A534:
	ldr r0, _0805A538 @ =0x0000029F
	b _0805A56A
_0805A538: .4byte 0x0000029F
_0805A53C:
	ldr r0, _0805A558 @ =0x000003F2
	cmp r1, r0
	beq _0805A576
	cmp r1, r0
	bgt _0805A55C
	sub r0, #0x2A
	cmp r1, r0
	beq _0805A576
	cmp r1, r0
	ble _0805A568
	mov r0, #0xFC
	lsl r0, r0, #2
	b _0805A56A
	.align 2, 0
_0805A558: .4byte 0x000003F2
_0805A55C:
	mov r0, #0x84
	lsl r0, r0, #3
	cmp r1, r0
	beq _0805A576
	cmp r1, r0
	bgt _0805A570
_0805A568:
	sub r0, #0x1D
_0805A56A:
	cmp r1, r0
	beq _0805A576
	b _0805A586
_0805A570:
	ldr r0, _0805A6E0 @ =0x0000042C
	cmp r1, r0
	bne _0805A586
_0805A576:
	mov r0, #1
	ldr r1, _0805A6E4 @ =0x00000231
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A586
	b _0805A79C
_0805A586:
	add r7, #1
	cmp r7, #4
	ble _0805A4B6
	ldr r1, _0805A6E8 @ =0x0000015B
	mov r0, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	ble _0805A5A8
	ldr r1, _0805A6EC @ =0x00000246
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A5A8
	b _0805A87C
_0805A5A8:
	mov r1, #0xA4
	lsl r1, r1, #1
	mov r0, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	ble _0805A5C6
	mov r0, #1
	mov r1, #0x27
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A5C6
	b _0805A87C
_0805A5C6:
	mov r0, #0
	bl CountSpellTraps
	cmp r0, #1
	ble _0805A5E0
	ldr r1, _0805A6F0 @ =0x00000109
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A5E0
	b _0805A87C
_0805A5E0:
	mov r0, #0
	bl CountActivatableSetCards
	cmp r0, #1
	ble _0805A5FA
	ldr r1, _0805A6F4 @ =0x00000249
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A5FA
	b _0805A87C
_0805A5FA:
	mov r0, #0
	bl CountActivatableSetCards
	cmp r0, #0
	ble _0805A614
	mov r0, #1
	mov r1, #0xDF
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, #0
	blt _0805A614
	b _0805A87C
_0805A614:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _0805A67C
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	bne _0805A67C
	ldr r1, _0805A6F8 @ =0x0000048B
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	mov r5, #1
	neg r5, r5
	cmp r4, r5
	ble _0805A63C
	b _0805A87C
_0805A63C:
	ldr r1, _0805A6FC @ =0x00000452
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A64C
	b _0805A87C
_0805A64C:
	ldr r1, _0805A700 @ =0x0000045A
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A65C
	b _0805A87C
_0805A65C:
	ldr r1, _0805A704 @ =0x0000045B
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A66C
	b _0805A87C
_0805A66C:
	ldr r1, _0805A708 @ =0x0000051B
	mov r0, #1
	bl FindHandCardByNumber
	add r4, r0, #0
	cmp r4, r5
	ble _0805A67C
	b _0805A87C
_0805A67C:
	mov r2, #0
	mov r9, r2
	mov r7, #0
_0805A682:
	mov r0, #0
	add r1, r7, #0
	bl GetZoneCardAtk
	cmp r9, r0
	bge _0805A690
	mov r9, r0
_0805A690:
	add r7, #1
	cmp r7, #4
	ble _0805A682
	mov r0, #0
	mov sl, r0
	mov r4, #1
	neg r4, r4
	mov r7, #0
	ldr r0, _0805A70C @ =0x020192E4
	ldr r2, _0805A710 @ =0x00000D66
	add r1, r0, r2
	ldrb r1, [r1]
	cmp r7, r1
	bge _0805A798
	ldr r0, _0805A714 @ =0x000007FF
	mov r8, r0
_0805A6B0:
	lsl r0, r7, #2
	ldr r1, _0805A718 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	add r0, r6, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _0805A71C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805A72A
	cmp r0, #0x17
	ble _0805A720
	cmp r0, #0x18
	beq _0805A724
	b _0805A72A
_0805A6E0: .4byte 0x0000042C
_0805A6E4: .4byte 0x00000231
_0805A6E8: .4byte 0x0000015B
_0805A6EC: .4byte 0x00000246
_0805A6F0: .4byte 0x00000109
_0805A6F4: .4byte 0x00000249
_0805A6F8: .4byte 0x0000048B
_0805A6FC: .4byte 0x00000452
_0805A700: .4byte 0x0000045A
_0805A704: .4byte 0x0000045B
_0805A708: .4byte 0x0000051B
_0805A70C: .4byte 0x020192E4
_0805A710: .4byte 0x00000D66
_0805A714: .4byte 0x000007FF
_0805A718: .4byte 0x0201A6CC
_0805A71C: .4byte gCardStats
_0805A720:
	mov r0, #0
	b _0805A744
_0805A724:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805A744
_0805A72A:
	add r0, r6, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _0805A7A0 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	ldr r2, _0805A7A4 @ =0x000001FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805A744:
	add r5, r0, #0
	cmp sl, r5
	bge _0805A78A
	cmp r9, r5
	bgt _0805A78A
	mov r0, #1
	add r1, r6, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _0805A78A
	add r0, r6, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0805A7A8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #2
	bl AiIsKeyCard
	cmp r0, #0
	bne _0805A78A
	add r0, r6, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _0805A78A
	add r0, r6, #0
	bl AiHasTributesFor
	cmp r0, #0
	beq _0805A78A
	add r4, r7, #0
	mov sl, r5
_0805A78A:
	add r7, #1
	ldr r0, _0805A7AC @ =0x020192E4
	ldr r1, _0805A7B0 @ =0x00000D66
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r7, r0
	blt _0805A6B0
_0805A798:
	cmp r4, #0
	blt _0805A7B8
_0805A79C:
	add r0, r4, #0
	b _0805A87C
_0805A7A0: .4byte gCardStats
_0805A7A4: .4byte 0x000001FF
_0805A7A8: .4byte gCardIdToNumber
_0805A7AC: .4byte 0x020192E4
_0805A7B0: .4byte 0x00000D66
_0805A7B4:
	add r0, r7, #0
	b _0805A87C
_0805A7B8:
	mov r7, #0
	ldr r2, _0805A7F8 @ =0x020192E4
	ldr r1, _0805A7FC @ =0x00000D66
	add r0, r2, r1
	ldrb r0, [r0]
	cmp r7, r0
	bge _0805A878
	ldr r2, _0805A800 @ =0x000007FF
	add r5, r2, #0
_0805A7CA:
	lsl r0, r7, #2
	ldr r1, _0805A804 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r4, #0
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _0805A808 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805A814
	cmp r0, #0x17
	ble _0805A80C
	cmp r0, #0x18
	beq _0805A810
	b _0805A814
_0805A7F8: .4byte 0x020192E4
_0805A7FC: .4byte 0x00000D66
_0805A800: .4byte 0x000007FF
_0805A804: .4byte 0x0201A6CC
_0805A808: .4byte gCardStats
_0805A80C:
	mov r0, #0
	b _0805A828
_0805A810:
	mov r0, #0xA
	b _0805A828
_0805A814:
	add r0, r4, #0
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _0805A88C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805A828:
	cmp r0, #4
	bhi _0805A86A
	add r2, r4, #0
	and r2, r5
	lsl r0, r2, #2
	ldr r1, _0805A88C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0805A86A
	lsl r0, r2, #1
	ldr r2, _0805A890 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #2
	bl AiIsKeyCard
	cmp r0, #0
	bne _0805A86A
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _0805A86A
	add r0, r4, #0
	bl AiHasTributesFor
	cmp r0, #0
	bne _0805A7B4
_0805A86A:
	add r7, #1
	ldr r0, _0805A894 @ =0x020192E4
	ldr r1, _0805A898 @ =0x00000D66
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r7, r0
	blt _0805A7CA
_0805A878:
	mov r0, #1
	neg r0, r0
_0805A87C:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0805A88C: .4byte gCardStats
_0805A890: .4byte gCardIdToNumber
_0805A894: .4byte 0x020192E4
_0805A898: .4byte 0x00000D66
	thumb_func_end AiPickMonsterToSet

