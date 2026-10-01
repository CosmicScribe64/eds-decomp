	thumb_func_start sub_08054398
sub_08054398: @ 0x08054398
	push {r4, r5, r6, lr}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	cmp r6, #0
	bne _080543A6
	b _080545B6
_080543A6:
	add r0, r6, #0
	bl sub_0800966C
	cmp r0, #0
	beq _080543B2
	b _080545B6
_080543B2:
	ldr r2, _080543E0 @ =0x000007FF
	and r2, r6
	lsl r0, r2, #2
	ldr r1, _080543E4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _080543CC
	b _080545B6
_080543CC:
	lsl r0, r2, #1
	ldr r1, _080543E8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080543EC @ =0x00000776
	cmp r1, r0
	bne _080543F0
	mov r0, #3
	b _08054452
	.align 2, 0
_080543E0: .4byte 0x000007FF
_080543E4: .4byte gUnk_08621DE0
_080543E8: .4byte gUnk_08622AB4
_080543EC: .4byte 0x00000776
_080543F0:
	cmp r1, r0
	blt _08054400
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08054400
	mov r0, #1
	b _08054452
_08054400:
	ldr r0, _08054424 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08054428 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08054432
	cmp r0, #0x16
	bgt _0805442C
	cmp r0, #0x15
	beq _08054436
	b _0805443E
	.align 2, 0
_08054424: .4byte 0x000007FF
_08054428: .4byte gUnk_08621DE0
_0805442C:
	cmp r0, #0x17
	beq _0805443A
	b _0805443E
_08054432:
	mov r0, #7
	b _08054452
_08054436:
	mov r0, #8
	b _08054452
_0805443A:
	mov r0, #9
	b _08054452
_0805443E:
	ldr r0, _08054470 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08054474 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08054452:
	cmp r0, #3
	bne _08054458
	b _080545B6
_08054458:
	ldr r0, _08054470 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08054478 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805447C @ =0x00000776
	cmp r1, r0
	bne _08054480
	mov r0, #3
	b _080544E2
	.align 2, 0
_08054470: .4byte 0x000007FF
_08054474: .4byte gUnk_08621DE0
_08054478: .4byte gUnk_08622AB4
_0805447C: .4byte 0x00000776
_08054480:
	cmp r1, r0
	blt _08054490
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08054490
	mov r0, #1
	b _080544E2
_08054490:
	ldr r0, _080544B4 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _080544B8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080544C2
	cmp r0, #0x16
	bgt _080544BC
	cmp r0, #0x15
	beq _080544C6
	b _080544CE
	.align 2, 0
_080544B4: .4byte 0x000007FF
_080544B8: .4byte gUnk_08621DE0
_080544BC:
	cmp r0, #0x17
	beq _080544CA
	b _080544CE
_080544C2:
	mov r0, #7
	b _080544E2
_080544C6:
	mov r0, #8
	b _080544E2
_080544CA:
	mov r0, #9
	b _080544E2
_080544CE:
	ldr r0, _08054528 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0805452C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080544E2:
	cmp r0, #2
	beq _080545B6
	ldr r0, _08054528 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08054530 @ =0x08622AB4
	add r0, r0, r1
	ldrh r4, [r0]
	add r0, r4, #0
	bl sub_0800756C
	cmp r0, #0
	beq _08054506
	add r0, r5, #0
	bl sub_08008668
	cmp r0, #0
	beq _080545B6
_08054506:
	ldr r0, _08054534 @ =0x000002E5
	cmp r4, r0
	beq _080545B6
	cmp r4, r0
	bgt _0805455C
	cmp r4, #0x42
	beq _080545C0
	cmp r4, #0x42
	bgt _08054538
	cmp r4, #0x37
	bge _0805451E
	b _080546DC
_0805451E:
	cmp r4, #0x38
	ble _080545C0
	cmp r4, #0x3E
	beq _080545B6
	b _080546DC
_08054528: .4byte 0x000007FF
_0805452C: .4byte gUnk_08621DE0
_08054530: .4byte gUnk_08622AB4
_08054534: .4byte 0x000002E5
_08054538:
	ldr r0, _0805454C @ =0x00000175
	cmp r4, r0
	bne _08054540
	b _08054644
_08054540:
	cmp r4, r0
	bgt _08054550
	sub r0, #5
	cmp r4, r0
	beq _080545C0
	b _080546DC
_0805454C: .4byte 0x00000175
_08054550:
	ldr r0, _08054558 @ =0x00000187
	cmp r4, r0
	beq _080545B6
	b _080546DC
_08054558: .4byte 0x00000187
_0805455C:
	ldr r0, _0805457C @ =0x000004E9
	cmp r4, r0
	bne _08054564
	b _080546D0
_08054564:
	cmp r4, r0
	bgt _08054590
	sub r0, #0x37
	cmp r4, r0
	beq _080545B6
	cmp r4, r0
	bgt _08054584
	ldr r0, _08054580 @ =0x0000034D
	cmp r4, r0
	bne _0805457A
	b _0805468C
_0805457A:
	b _080546DC
_0805457C: .4byte 0x000004E9
_08054580: .4byte 0x0000034D
_08054584:
	ldr r0, _0805458C @ =0x000004E2
	cmp r4, r0
	beq _080545D6
	b _080546DC
_0805458C: .4byte 0x000004E2
_08054590:
	ldr r0, _080545BC @ =0x00000546
	cmp r4, r0
	beq _08054610
	cmp r4, r0
	bge _0805459C
	b _080546DC
_0805459C:
	add r0, #0xA9
	cmp r4, r0
	ble _080545A4
	b _080546DC
_080545A4:
	sub r0, #5
	cmp r4, r0
	bge _080545AC
	b _080546DC
_080545AC:
	add r0, r5, #0
	bl sub_08047170
	cmp r0, #0
	bne _0805469E
_080545B6:
	mov r0, #0
	b _0805476A
	.align 2, 0
_080545BC: .4byte 0x00000546
_080545C0:
	add r0, r5, #0
	bl sub_08047170
	cmp r0, #0
	beq _080545B6
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl sub_0802CFA0
	b _080546D6
_080545D6:
	ldr r2, _08054608 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _0805460C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #1
	bne _080545F4
	add r0, r5, #0
	bl sub_08008A1C
	cmp r0, #0
	ble _080545F4
	b _08054768
_080545F4:
	mov r1, #1
	neg r1, r1
	add r0, r5, #0
	bl sub_08008AF8
	cmp r0, #1
	ble _08054604
	b _08054768
_08054604:
	b _080545B6
	.align 2, 0
_08054608: .4byte 0x020192E4
_0805460C: .4byte 0x00000D64
_08054610:
	add r0, r5, #0
	bl sub_08008860
	add r4, r0, #0
	add r4, #1
	mov r0, #1
	sub r0, r0, r5
	bl sub_08008860
	cmp r4, r0
	bge _08054632
	add r0, r5, #0
	bl sub_08008A1C
	cmp r0, #0
	ble _08054632
	b _08054768
_08054632:
	mov r1, #1
	neg r1, r1
	add r0, r5, #0
	bl sub_08008AF8
	cmp r0, #0
	ble _08054642
	b _08054768
_08054642:
	b _080545B6
_08054644:
	add r0, r5, #0
	bl sub_08047170
	cmp r0, #0
	beq _080545B6
	mov r1, #1
	neg r1, r1
	add r0, r5, #0
	bl sub_08008AF8
	cmp r0, #2
	ble _080545B6
	mov r1, #0xB9
	lsl r1, r1, #1
	add r0, r5, #0
	bl sub_080086CC
	cmp r0, #0
	beq _080545B6
	ldr r1, _08054688 @ =0x00000173
	add r0, r5, #0
	bl sub_080086CC
	cmp r0, #0
	beq _080545B6
	mov r1, #0xBA
	lsl r1, r1, #1
	add r0, r5, #0
	bl sub_080086CC
	cmp r0, #0
	beq _080545B6
	b _08054768
	.align 2, 0
_08054688: .4byte 0x00000173
_0805468C:
	add r0, r5, #0
	bl sub_08047170
	cmp r0, #0
	beq _080545B6
	add r0, r5, #0
	bl sub_08054028
	b _080546D6
_0805469E:
	add r0, r5, #0
	bl sub_08008A1C
	cmp r0, #0
	bne _080546BE
	ldr r2, _080546C8 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _080546CC @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _080546BE
	b _080545B6
_080546BE:
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08054198
	b _080546D6
_080546C8: .4byte 0x020192E4
_080546CC: .4byte 0x00000D64
_080546D0:
	add r0, r5, #0
	bl sub_08054130
_080546D6:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0805476A
_080546DC:
	ldr r0, _08054700 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08054704 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08054710
	cmp r0, #0x17
	ble _08054708
	cmp r0, #0x18
	beq _0805470C
	b _08054710
	.align 2, 0
_08054700: .4byte 0x000007FF
_08054704: .4byte gUnk_08621DE0
_08054708:
	mov r0, #0
	b _08054724
_0805470C:
	mov r0, #0xA
	b _08054724
_08054710:
	ldr r0, _08054744 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08054748 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08054724:
	cmp r0, #0
	blt _08054758
	cmp r0, #4
	ble _0805474C
	cmp r0, #6
	bgt _08054758
	mov r1, #1
	neg r1, r1
	add r0, r5, #0
	bl sub_08008AF8
	cmp r0, #0
	bgt _08054740
	b _080545B6
_08054740:
	b _08054768
	.align 2, 0
_08054744: .4byte 0x000007FF
_08054748: .4byte gUnk_08621DE0
_0805474C:
	add r0, r5, #0
	bl sub_08008A1C
	cmp r0, #0
	bgt _08054768
	b _080545B6
_08054758:
	mov r1, #1
	neg r1, r1
	add r0, r5, #0
	bl sub_08008AF8
	cmp r0, #1
	bgt _08054768
	b _080545B6
_08054768:
	mov r0, #1
_0805476A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08054398

