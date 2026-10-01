	thumb_func_start sub_08023228
sub_08023228: @ 0x08023228
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r0, _080232AC @ =0x04000128
	ldrh r0, [r0]
	ldr r6, _080232B0 @ =0x02017FB0
	mov r0, #0xF0
	lsl r0, r0, #8
	strh r0, [r6]
	add r0, r6, #0
	bl sub_080722B0
	add r1, r0, #0
	ldr r0, _080232B4 @ =0x030049D0
	ldr r2, _080232B8 @ =0x00000522
	add r0, r0, r2
	ldrh r2, [r0]
	cmp r2, #0
	beq _08023254
	bl _08024054 @ far jump
_08023254:
	cmp r1, #0
	bne _0802325C
	bl _08024074 @ far jump
_0802325C:
	ldrh r1, [r6]
	ldr r0, _080232BC @ =0x0000F042
	cmp r1, r0
	bne _08023268
	bl _0802401C @ far jump
_08023268:
	cmp r1, r0
	ble _0802326E
	b _080233E8
_0802326E:
	sub r0, #0x2F
	cmp r1, r0
	bne _08023278
	bl _08023C82 @ far jump
_08023278:
	cmp r1, r0
	bgt _0802332C
	sub r0, #0x11
	cmp r1, r0
	bne _08023284
	b _080235AC
_08023284:
	cmp r1, r0
	bgt _080232E4
	ldr r0, _080232C0 @ =0x0000EE02
	cmp r1, r0
	bne _08023290
	b _08023590
_08023290:
	cmp r1, r0
	bgt _080232C4
	sub r0, #2
	cmp r1, r0
	bne _0802329E
	bl _08024040 @ far jump
_0802329E:
	add r0, #1
	cmp r1, r0
	bne _080232A6
	b _08023578
_080232A6:
	bl _08024054 @ far jump
	.align 2, 0
_080232AC: .4byte 0x04000128
_080232B0: .4byte 0x02017FB0
_080232B4: .4byte 0x030049D0
_080232B8: .4byte 0x00000522
_080232BC: .4byte 0x0000F042
_080232C0: .4byte 0x0000EE02
_080232C4:
	mov r0, #0xF0
	lsl r0, r0, #8
	cmp r1, r0
	bne _080232CE
	b _0802364E
_080232CE:
	cmp r1, r0
	ble _080232D4
	b _08023560
_080232D4:
	ldr r0, _080232E0 @ =0x0000EE03
	cmp r1, r0
	bne _080232DC
	b _080235A0
_080232DC:
	bl _08024054 @ far jump
_080232E0: .4byte 0x0000EE03
_080232E4:
	ldr r0, _08023304 @ =0x0000F005
	cmp r1, r0
	bne _080232EC
	b _08023608
_080232EC:
	cmp r1, r0
	bgt _08023308
	sub r0, #2
	cmp r1, r0
	bne _080232F8
	b _080235C0
_080232F8:
	add r0, #1
	cmp r1, r0
	bne _08023300
	b _080235F0
_08023300:
	bl _08024054 @ far jump
_08023304: .4byte 0x0000F005
_08023308:
	ldr r0, _08023328 @ =0x0000F011
	cmp r1, r0
	bne _08023312
	bl _08023C72 @ far jump
_08023312:
	cmp r1, r0
	ble _0802331A
	bl _08023C7A @ far jump
_0802331A:
	sub r0, #0xB
	cmp r1, r0
	bne _08023322
	b _08023620
_08023322:
	bl _08024054 @ far jump
	.align 2, 0
_08023328: .4byte 0x0000F011
_0802332C:
	ldr r0, _08023360 @ =0x0000F025
	cmp r1, r0
	bne _08023336
	bl _08023F04 @ far jump
_08023336:
	cmp r1, r0
	bgt _08023398
	sub r0, #4
	cmp r1, r0
	bne _08023344
	bl _08023C9A @ far jump
_08023344:
	cmp r1, r0
	bgt _08023364
	sub r0, #0xD
	cmp r1, r0
	bne _08023352
	bl _08023C8A @ far jump
_08023352:
	add r0, #1
	cmp r1, r0
	bne _0802335C
	bl _08023C92 @ far jump
_0802335C:
	bl _08024054 @ far jump
_08023360: .4byte 0x0000F025
_08023364:
	ldr r0, _08023394 @ =0x0000F023
	cmp r1, r0
	bne _0802336E
	bl _08023DC0 @ far jump
_0802336E:
	cmp r1, r0
	ble _08023376
	bl _08023E60 @ far jump
_08023376:
	ldrh r0, [r6, #2]
	lsr r6, r0, #8
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	mov r0, #1
	sub r6, r0, r6
	mov r3, #0
	mov r8, r3
	cmp r8, r7
	blt _0802338E
	bl _08023D7A @ far jump
_0802338E:
	bl _08023D38 @ far jump
	.align 2, 0
_08023394: .4byte 0x0000F023
_08023398:
	ldr r0, _080233C0 @ =0x0000F033
	cmp r1, r0
	bne _080233A2
	bl _08023FBC @ far jump
_080233A2:
	cmp r1, r0
	bgt _080233C4
	sub r0, #2
	cmp r1, r0
	bne _080233B0
	bl _08023F94 @ far jump
_080233B0:
	add r0, #1
	cmp r1, r0
	bne _080233BA
	bl _08023FA4 @ far jump
_080233BA:
	bl _08024054 @ far jump
	.align 2, 0
_080233C0: .4byte 0x0000F033
_080233C4:
	ldr r0, _080233E4 @ =0x0000F035
	cmp r1, r0
	bne _080233CE
	bl _08023FE4 @ far jump
_080233CE:
	cmp r1, r0
	bge _080233D6
	bl _08023FCC @ far jump
_080233D6:
	add r0, #0xC
	cmp r1, r0
	bne _080233E0
	bl _08023FF4 @ far jump
_080233E0:
	bl _08024054 @ far jump
_080233E4: .4byte 0x0000F035
_080233E8:
	ldr r0, _08023424 @ =0x0000F062
	cmp r1, r0
	bne _080233F2
	bl _08023BF4 @ far jump
_080233F2:
	cmp r1, r0
	bgt _080234B0
	sub r0, #0xC
	cmp r1, r0
	bne _080233FE
	b _080239D0
_080233FE:
	cmp r1, r0
	bgt _0802346C
	sub r0, #4
	cmp r1, r0
	bne _0802340A
	b _08023860
_0802340A:
	cmp r1, r0
	bgt _08023428
	sub r0, #0xF
	cmp r1, r0
	bne _08023418
	bl _0802402C @ far jump
_08023418:
	add r0, #0xE
	cmp r1, r0
	bne _08023420
	b _080236E8
_08023420:
	bl _08024054 @ far jump
_08023424: .4byte 0x0000F062
_08023428:
	ldr r0, _0802345C @ =0x0000F054
	cmp r1, r0
	bne _08023430
	b _08023944
_08023430:
	cmp r1, r0
	ble _08023436
	b _080239B8
_08023436:
	add r1, r6, #2
	mov r0, #0
	bl sub_0801FBF4
	ldr r1, _08023460 @ =0x02017A40
	ldr r0, _08023464 @ =0x00000491
	add r1, r1, r0
	mov r0, #0x80
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r3, _08023468 @ =0x00000307
	add r1, r6, r3
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_0802345C: .4byte 0x0000F054
_08023460: .4byte 0x02017A40
_08023464: .4byte 0x00000491
_08023468: .4byte 0x00000307
_0802346C:
	ldr r0, _0802348C @ =0x0000F059
	cmp r1, r0
	bne _08023474
	b _08023874
_08023474:
	cmp r1, r0
	bgt _08023490
	sub r0, #2
	cmp r1, r0
	bne _08023480
	b _08023690
_08023480:
	add r0, #1
	cmp r1, r0
	bne _08023488
	b _080236C4
_08023488:
	bl _08024054 @ far jump
_0802348C: .4byte 0x0000F059
_08023490:
	ldr r0, _080234AC @ =0x0000F05B
	cmp r1, r0
	bne _08023498
	b _0802367C
_08023498:
	cmp r1, r0
	bge _0802349E
	b _08023664
_0802349E:
	add r0, #6
	cmp r1, r0
	bne _080234A6
	b _08023BE0
_080234A6:
	bl _08024054 @ far jump
	.align 2, 0
_080234AC: .4byte 0x0000F05B
_080234B0:
	ldr r0, _080234E0 @ =0x0000F081
	cmp r1, r0
	bne _080234B8
	b _08023AD0
_080234B8:
	cmp r1, r0
	bgt _08023510
	sub r0, #0x1C
	cmp r1, r0
	bne _080234C6
	bl _08023C68 @ far jump
_080234C6:
	cmp r1, r0
	bgt _080234E4
	sub r0, #2
	cmp r1, r0
	bne _080234D2
	b _08023C38
_080234D2:
	add r0, #1
	cmp r1, r0
	bne _080234DA
	b _08023C4E
_080234DA:
	bl _08024054 @ far jump
	.align 2, 0
_080234E0: .4byte 0x0000F081
_080234E4:
	ldr r0, _080234FC @ =0x0000F072
	cmp r1, r0
	bne _080234EC
	b _08023BB4
_080234EC:
	cmp r1, r0
	bgt _08023500
	sub r0, #1
	cmp r1, r0
	bne _080234F8
	b _08023B6C
_080234F8:
	bl _08024054 @ far jump
_080234FC: .4byte 0x0000F072
_08023500:
	ldr r0, _0802350C @ =0x0000F073
	cmp r1, r0
	bne _08023508
	b _08023BCC
_08023508:
	bl _08024054 @ far jump
_0802350C: .4byte 0x0000F073
_08023510:
	ldr r0, _08023530 @ =0x0000F092
	cmp r1, r0
	bne _08023518
	b _08023AB8
_08023518:
	cmp r1, r0
	bgt _08023534
	sub r0, #0x10
	cmp r1, r0
	bne _08023524
	b _08023B2C
_08023524:
	add r0, #0xF
	cmp r1, r0
	bne _0802352C
	b _08023A6C
_0802352C:
	bl _08024054 @ far jump
_08023530: .4byte 0x0000F092
_08023534:
	ldr r0, _0802354C @ =0x0000F0A2
	cmp r1, r0
	bne _0802353C
	b _08023A40
_0802353C:
	cmp r1, r0
	bgt _08023550
	sub r0, #1
	cmp r1, r0
	bne _08023548
	b _08023A18
_08023548:
	bl _08024054 @ far jump
_0802354C: .4byte 0x0000F0A2
_08023550:
	ldr r0, _0802355C @ =0x0000F0B1
	cmp r1, r0
	bne _08023558
	b _08023A00
_08023558:
	bl _08024054 @ far jump
_0802355C: .4byte 0x0000F0B1
_08023560:
	ldr r1, _08023574 @ =0x02017FB0
	mov r3, #0xC1
	lsl r3, r3, #2
	add r1, r1, r3
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_08023574: .4byte 0x02017FB0
_08023578:
	ldr r1, _0802358C @ =0x02017FB0
	mov r3, #0xC1
	lsl r3, r3, #2
	add r1, r1, r3
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_0802358C: .4byte 0x02017FB0
_08023590:
	mov r3, #0xC1
	lsl r3, r3, #2
	add r1, r6, r3
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
_080235A0:
	ldr r1, _080235A8 @ =0x0201CFB0
	mov r0, #1
	bl _08024032 @ far jump
_080235A8: .4byte 0x0201CFB0
_080235AC:
	ldr r0, _080235BC @ =0x00000306
	add r1, r6, r0
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_080235BC: .4byte 0x00000306
_080235C0:
	ldr r3, _080235E4 @ =0x00000306
	add r1, r6, r3
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r1, _080235E8 @ =0x020192E0
	ldr r3, _080235EC @ =0x00001B12
	add r1, r1, r3
	ldrb r6, [r6, #2]
	lsl r2, r6, #6
	mov r0, #0x3F
	ldrb r3, [r1]
	and r0, r3
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_080235E4: .4byte 0x00000306
_080235E8: .4byte 0x020192E0
_080235EC: .4byte 0x00001B12
_080235F0:
	ldr r1, _08023600 @ =0x02017FB0
	ldr r0, _08023604 @ =0x00000306
	add r1, r1, r0
	mov r0, #0x40
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
_08023600: .4byte 0x02017FB0
_08023604: .4byte 0x00000306
_08023608:
	ldr r1, _08023618 @ =0x020192E0
	ldr r3, _0802361C @ =0x00001B14
	add r1, r1, r3
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _08023644
_08023618: .4byte 0x020192E0
_0802361C: .4byte 0x00001B14
_08023620:
	ldr r1, _08023654 @ =0x020192E0
	ldr r3, _08023658 @ =0x00001B14
	add r1, r1, r3
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _0802365C @ =0x02017FB0
	ldr r3, _08023660 @ =0x00000306
	add r1, r1, r3
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bl sub_080617F4
_08023644:
	mov r0, #0
	mov r1, #5
	mov r2, #0
	bl sub_08024134
_0802364E:
	mov r0, #1
	bl _08024076 @ far jump
_08023654: .4byte 0x020192E0
_08023658: .4byte 0x00001B14
_0802365C: .4byte 0x02017FB0
_08023660: .4byte 0x00000306
_08023664:
	ldr r0, _08023674 @ =0x0201CF90
	ldr r1, _08023678 @ =0x02017FB2
	mov r2, #0x14
	bl sub_08075294
	bl sub_08055AB4
	b _0802364E
_08023674: .4byte 0x0201CF90
_08023678: .4byte 0x02017FB2
_0802367C:
	ldr r3, _0802368C @ =0x00000306
	add r1, r6, r3
	mov r0, #0x80
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_0802368C: .4byte 0x00000306
_08023690:
	mov r3, #0x8A
	lsl r3, r3, #3
	add r1, r6, r3
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	add r3, #2
	add r1, r6, r3
	mov r0, #0
	strh r0, [r1]
	ldrh r1, [r6, #2]
	ldr r2, _080236C0 @ =0x00000454
	add r0, r6, r2
	strh r1, [r0]
	ldrh r1, [r6, #4]
	add r3, #4
	add r0, r6, r3
	strh r1, [r0]
	ldrh r1, [r6, #6]
	add r2, #4
	add r0, r6, r2
	b _08023BE8
	.align 2, 0
_080236C0: .4byte 0x00000454
_080236C4:
	ldr r1, _080236E0 @ =0x02017FB0
	mov r3, #0x8A
	lsl r3, r3, #3
	add r2, r1, r3
	mov r0, #2
	ldrb r3, [r2]
	orr r0, r3
	strb r0, [r2]
	ldrh r0, [r1, #2]
	ldr r2, _080236E4 @ =0x0000045A
	add r1, r1, r2
	strh r0, [r1]
	b _0802364E
	.align 2, 0
_080236E0: .4byte 0x02017FB0
_080236E4: .4byte 0x0000045A
_080236E8:
	ldr r4, _08023724 @ =0x02017EFC
	ldr r1, _08023728 @ =0x02017FB2
	add r0, r4, #0
	mov r2, #0x14
	bl sub_08075294
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4, #2]
	ldrb r4, [r4, #3]
	lsr r0, r4, #2
	sub r0, #5
	cmp r0, #0x19
	bls _08023718
	b _0802381A
_08023718:
	lsl r0, r0, #2
	ldr r1, _0802372C @ =0x08023730
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08023724: .4byte 0x02017EFC
_08023728: .4byte 0x02017FB2
_0802372C: .4byte 0x08023730
_08023730:
	.4byte _08023798
	.4byte _08023798
	.4byte _08023798
	.4byte _08023798
	.4byte _0802381A
	.4byte _0802381A
	.4byte _0802381A
	.4byte _0802381A
	.4byte _080237EC
	.4byte _080237EC
	.4byte _080237D4
	.4byte _08023798
	.4byte _08023798
	.4byte _0802381A
	.4byte _08023808
	.4byte _08023798
	.4byte _08023798
	.4byte _0802381A
	.4byte _0802381A
	.4byte _0802381A
	.4byte _08023798
	.4byte _08023798
	.4byte _08023798
	.4byte _0802381A
	.4byte _08023798
	.4byte _08023798
_08023798:
	ldr r3, _080237C8 @ =0x02017A40
	ldr r0, _080237CC @ =0x000004C2
	add r4, r3, r0
	ldrh r2, [r4]
	mov r1, #1
	sub r0, r1, r2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r2, r2, #8
	lsl r2, r2, #8
	orr r0, r2
	strh r0, [r4]
	ldr r2, _080237D0 @ =0x000004C4
	add r3, r3, r2
	ldrh r0, [r3]
	sub r1, r1, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsr r0, r0, #8
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r3]
	b _0802381A
	.align 2, 0
_080237C8: .4byte 0x02017A40
_080237CC: .4byte 0x000004C2
_080237D0: .4byte 0x000004C4
_080237D4:
	ldr r1, _080237E4 @ =0x02017A40
	ldr r3, _080237E8 @ =0x000004C4
	add r1, r1, r3
	mov r0, #1
	ldrh r2, [r1]
	sub r0, r0, r2
	strh r0, [r1]
	b _0802381A
_080237E4: .4byte 0x02017A40
_080237E8: .4byte 0x000004C4
_080237EC:
	ldr r0, _08023800 @ =0x02017A40
	ldr r3, _08023804 @ =0x000004C4
	add r0, r0, r3
	ldrh r1, [r0]
	lsr r2, r1, #8
	lsl r1, r1, #0x18
	lsr r1, r1, #0x10
	orr r2, r1
	strh r2, [r0]
	b _0802381A
_08023800: .4byte 0x02017A40
_08023804: .4byte 0x000004C4
_08023808:
	ldr r0, _08023848 @ =0x02017A40
	ldr r1, _0802384C @ =0x000004C2
	add r2, r0, r1
	ldrh r3, [r2]
	add r1, #2
	add r0, r0, r1
	ldrh r1, [r0]
	strh r3, [r2]
	strh r1, [r0]
_0802381A:
	ldr r1, _08023848 @ =0x02017A40
	ldr r3, _08023850 @ =0x00000492
	add r2, r1, r3
	mov r0, #1
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	ldr r0, _08023854 @ =0x00000493
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _08023858 @ =0x02017FB0
	ldr r3, _0802385C @ =0x00000307
	add r1, r1, r3
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_08023848: .4byte 0x02017A40
_0802384C: .4byte 0x000004C2
_08023850: .4byte 0x00000492
_08023854: .4byte 0x00000493
_08023858: .4byte 0x02017FB0
_0802385C: .4byte 0x00000307
_08023860:
	ldr r3, _08023870 @ =0x00000307
	add r1, r6, r3
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	bl _08024036 @ far jump
	.align 2, 0
_08023870: .4byte 0x00000307
_08023874:
	ldrh r4, [r6, #2]
	ldrh r3, [r6, #4]
	ldrh r2, [r6, #6]
	ldrh r5, [r6, #8]
	sub r0, r3, #5
	cmp r0, #0x19
	bhi _08023930
	lsl r0, r0, #2
	ldr r1, _0802388C @ =0x08023890
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0802388C: .4byte 0x08023890
_08023890:
	.4byte _080238F8
	.4byte _080238F8
	.4byte _080238F8
	.4byte _080238F8
	.4byte _08023930
	.4byte _08023930
	.4byte _08023930
	.4byte _08023930
	.4byte _0802391E
	.4byte _0802391E
	.4byte _08023914
	.4byte _080238F8
	.4byte _080238F8
	.4byte _08023930
	.4byte _0802392A
	.4byte _08023930
	.4byte _08023930
	.4byte _08023930
	.4byte _08023930
	.4byte _08023930
	.4byte _080238F8
	.4byte _080238F8
	.4byte _080238F8
	.4byte _08023930
	.4byte _080238F8
	.4byte _080238F8
_080238F8:
	mov r1, #1
	sub r0, r1, r2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r2, r2, #8
	lsl r2, r2, #8
	orr r2, r0
	sub r1, r1, r5
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsr r0, r5, #8
	lsl r5, r0, #8
	orr r5, r1
	b _08023930
_08023914:
	mov r0, #1
	sub r0, r0, r5
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	b _08023930
_0802391E:
	lsr r0, r5, #8
	lsl r1, r5, #0x18
	lsl r0, r0, #0x10
	orr r0, r1
	lsr r5, r0, #0x10
	b _08023930
_0802392A:
	ldr r0, _08023940 @ =0x02017FB0
	ldrh r2, [r0, #8]
	ldrh r5, [r0, #6]
_08023930:
	lsl r0, r5, #0x10
	orr r2, r0
	add r0, r4, #0
	add r1, r3, #0
	bl sub_08042AB0
	b _0802364E
	.align 2, 0
_08023940: .4byte 0x02017FB0
_08023944:
	ldr r4, _080239B0 @ =0x02017F10
	add r1, r6, #2
	add r0, r4, #0
	mov r2, #0x14
	bl sub_08075294
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r5, #1
	and r1, r5
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4, #2]
	ldrh r1, [r4, #6]
	sub r0, r5, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #8
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r4, #6]
	ldrh r1, [r4, #8]
	sub r0, r5, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #8
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r4, #8]
	add r0, r4, #0
	sub r0, #0x14
	add r1, r4, #0
	mov r2, #0x14
	bl sub_08075294
	add r1, r4, #0
	sub r1, #0x3E
	mov r0, #1
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	sub r4, #0x3D
	ldrb r0, [r4]
	orr r5, r0
	strb r5, [r4]
	ldr r2, _080239B4 @ =0x00000307
	add r1, r6, r2
	mov r0, #4
	b _08024032
_080239B0: .4byte 0x02017F10
_080239B4: .4byte 0x00000307
_080239B8:
	ldr r1, _080239C8 @ =0x02017FB0
	ldr r0, _080239CC @ =0x00000307
	add r1, r1, r0
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
	.align 2, 0
_080239C8: .4byte 0x02017FB0
_080239CC: .4byte 0x00000307
_080239D0:
	add r1, r6, #2
	mov r0, #1
	bl sub_0801FBF4
	ldr r1, _080239F4 @ =0x02017A40
	ldr r3, _080239F8 @ =0x00000491
	add r1, r1, r3
	mov r0, #0x80
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r3, _080239FC @ =0x00000307
	add r1, r6, r3
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
	.align 2, 0
_080239F4: .4byte 0x02017A40
_080239F8: .4byte 0x00000491
_080239FC: .4byte 0x00000307
_08023A00:
	ldr r0, _08023A14 @ =0x02017FB0
	ldrh r3, [r0, #4]
	lsl r1, r3, #0x10
	ldrh r0, [r0, #2]
	orr r1, r0
	add r0, r1, #0
	bl sub_0801FBCC
	b _0802364E
	.align 2, 0
_08023A14: .4byte 0x02017FB0
_08023A18:
	ldr r2, _08023A34 @ =0x02017FB0
	ldrh r1, [r2, #2]
	add r2, #4
	mov r0, #0
	mov r3, #8
	bl sub_080226CC
	ldr r1, _08023A38 @ =0x020192E0
	ldr r0, _08023A3C @ =0x00001B50
	add r1, r1, r0
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
_08023A34: .4byte 0x02017FB0
_08023A38: .4byte 0x020192E0
_08023A3C: .4byte 0x00001B50
_08023A40:
	ldr r0, _08023A64 @ =0x0201AE44
	add r1, r6, #2
	mov r2, #0x10
	bl sub_08075294
	ldr r3, _08023A68 @ =0x00000307
	add r1, r6, r3
	mov r0, #0x80
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	sub r3, #1
	add r1, r6, r3
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08024036
_08023A64: .4byte 0x0201AE44
_08023A68: .4byte 0x00000307
_08023A6C:
	ldr r4, _08023AAC @ =0x0201840C
	ldr r3, _08023AB0 @ =0xFFFFFBA6
	add r1, r4, r3
	add r0, r4, #0
	mov r2, #0x28
	bl sub_08075294
	mov r2, #2
	neg r2, r2
	add r0, r2, #0
	ldrb r1, [r4, #2]
	and r0, r1
	strb r0, [r4, #2]
	ldrb r3, [r4, #0x16]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	and r2, r3
	orr r2, r1
	strb r2, [r4, #0x16]
	add r1, r4, #0
	add r1, #0x31
	mov r0, #0
	strb r0, [r1]
	ldr r2, _08023AB4 @ =0xFFFFFEAC
	add r1, r4, r2
	mov r0, #4
	b _08024032
	.align 2, 0
_08023AAC: .4byte 0x0201840C
_08023AB0: .4byte 0xFFFFFBA6
_08023AB4: .4byte 0xFFFFFEAC
_08023AB8:
	mov r0, #0xC2
	lsl r0, r0, #2
	add r1, r6, r0
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r3, _08023ACC @ =0x00000306
	b _08023B3C
	.align 2, 0
_08023ACC: .4byte 0x00000306
_08023AD0:
	ldr r3, _08023B1C @ =0x0000045C
	add r0, r6, r3
	add r1, r6, #2
	mov r2, #0x28
	bl sub_08075294
	ldr r0, _08023B20 @ =0x0000045E
	add r1, r6, r0
	mov r2, #2
	neg r2, r2
	add r0, r2, #0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	ldr r0, _08023B24 @ =0x00000472
	add r5, r6, r0
	ldrb r4, [r5]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r3, #1
	and r1, r3
	and r2, r4
	orr r2, r1
	strb r2, [r5]
	ldr r2, _08023B28 @ =0x0000048E
	add r1, r6, r2
	mov r0, #0
	strb r0, [r1]
	mov r1, #0xC2
	lsl r1, r1, #2
	add r0, r6, r1
	ldrb r2, [r0]
	orr r3, r2
	strb r3, [r0]
	b _0802364E
	.align 2, 0
_08023B1C: .4byte 0x0000045C
_08023B20: .4byte 0x0000045E
_08023B24: .4byte 0x00000472
_08023B28: .4byte 0x0000048E
_08023B2C:
	mov r3, #0xC2
	lsl r3, r3, #2
	add r1, r6, r3
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	sub r3, #2
_08023B3C:
	add r1, r6, r3
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _08023B68 @ =0x02017A40
	add r3, #0xCB
	add r2, r1, r3
	ldrb r3, [r2]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	mov r2, #0xA0
	lsl r2, r2, #2
	add r1, r1, r2
	add r0, r0, r1
	add r1, r6, #2
	mov r2, #0x14
	bl sub_08075294
	b _0802364E
_08023B68: .4byte 0x02017A40
_08023B6C:
	ldr r4, _08023BA4 @ =0x0201840C
	ldr r3, _08023BA8 @ =0xFFFFFBAA
	add r1, r4, r3
	add r0, r4, #0
	mov r2, #0x28
	bl sub_08075294
	ldr r1, _08023BAC @ =0xFFFFFBA4
	add r0, r4, r1
	add r2, r4, #0
	add r2, #0x34
	mov r1, #1
	ldrb r3, [r0, #4]
	and r1, r3
	ldrb r0, [r0, #2]
	lsl r0, r0, #1
	orr r0, r1
	strb r0, [r2]
	add r1, r4, #0
	add r1, #0x33
	mov r0, #0
	strb r0, [r1]
	ldr r0, _08023BB0 @ =0xFFFFFEAC
	add r1, r4, r0
	mov r0, #0x10
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
_08023BA4: .4byte 0x0201840C
_08023BA8: .4byte 0xFFFFFBAA
_08023BAC: .4byte 0xFFFFFBA4
_08023BB0: .4byte 0xFFFFFEAC
_08023BB4:
	ldrh r0, [r6, #2]
	ldrh r3, [r6, #6]
	lsl r1, r3, #0x10
	ldrh r2, [r6, #4]
	orr r1, r2
	ldrh r3, [r6, #0xA]
	lsl r2, r3, #0x10
	ldrh r6, [r6, #8]
	orr r2, r6
	bl sub_0801FA90
	b _0802364E
_08023BCC:
	ldr r1, _08023BDC @ =0x02017FB0
	mov r0, #0xC2
	lsl r0, r0, #2
	add r1, r1, r0
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
_08023BDC: .4byte 0x02017FB0
_08023BE0:
	ldr r0, _08023BEC @ =0x02017FB0
	ldrh r1, [r0, #2]
	ldr r3, _08023BF0 @ =0x0000044C
	add r0, r0, r3
_08023BE8:
	strh r1, [r0]
	b _0802364E
_08023BEC: .4byte 0x02017FB0
_08023BF0: .4byte 0x0000044C
_08023BF4:
	ldrh r1, [r6, #2]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, #0xC3
	lsl r2, r2, #2
	add r1, r6, r2
	add r0, r0, r1
	add r1, r6, #4
	mov r2, #0x14
	bl sub_08075294
	ldrh r3, [r6, #2]
	lsl r1, r3, #2
	add r1, r1, r3
	lsl r1, r1, #2
	add r1, r1, r6
	ldr r0, _08023C34 @ =0x0000030E
	add r1, r1, r0
	ldrb r3, [r1]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	mov r2, #1
	sub r2, r2, r0
	mov r0, #1
	and r2, r0
	mov r0, #2
	neg r0, r0
	and r0, r3
	orr r0, r2
	b _08024036
	.align 2, 0
_08023C34: .4byte 0x0000030E
_08023C38:
	mov r1, #0xC3
	lsl r1, r1, #2
	add r0, r6, r1
	mov r1, #0
	bl sub_0801A7B4
	mov r2, #0xC2
	lsl r2, r2, #2
	add r1, r6, r2
	mov r0, #0x40
	b _08024032
_08023C4E:
	ldr r4, _08023C64 @ =0x020182BC
	add r0, r4, #0
	mov r1, #1
	bl sub_0801A7B4
	sub r4, #4
	mov r0, #0x40
	ldrb r1, [r4]
	orr r0, r1
	strb r0, [r4]
	b _0802364E
_08023C64: .4byte 0x020182BC
_08023C68:
	mov r2, #0xC2
	lsl r2, r2, #2
	add r1, r6, r2
	mov r0, #0x80
	b _08024032
_08023C72:
	mov r0, #0
	bl sub_080229EC
	b _0802364E
_08023C7A:
	mov r0, #0
	bl sub_08022A9C
	b _0802364E
_08023C82:
	mov r0, #0
	bl sub_08022B4C
	b _0802364E
_08023C8A:
	mov r0, #0
	bl sub_08022BFC
	b _0802364E
_08023C92:
	mov r0, #0
	bl sub_08022CAC
	b _0802364E
_08023C9A:
	ldrh r0, [r6, #2]
	lsr r6, r0, #8
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	mov r0, #1
	sub r6, r0, r6
	mov r0, #0
	mov r8, r0
	cmp r2, r7
	bge _08023CF6
	mov r2, #1
	add r0, r6, #0
	and r0, r2
	ldr r1, _08023D1C @ =0x00000D64
	mul r1, r0
	ldr r0, _08023D20 @ =0x02019968
	add r4, r1, r0
	add r5, r4, #0
_08023CBE:
	mov r3, r8
	lsl r1, r3, #2
	ldr r0, _08023D24 @ =0x02017FB4
	add r1, r1, r0
	add r0, r5, #0
	str r2, [sp, #0]
	bl sub_08007558
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	ldr r2, [sp, #0]
	sub r0, r2, r0
	and r0, r2
	lsl r0, r0, #4
	mov r3, #0x11
	neg r3, r3
	add r1, r3, #0
	ldrb r3, [r4, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r4, #1]
	add r4, #4
	add r5, #4
	mov r0, #1
	add r8, r0
	cmp r8, r7
	blt _08023CBE
_08023CF6:
	ldr r1, _08023D28 @ =0x020192E4
	mov r0, #1
	and r6, r0
	ldr r0, _08023D1C @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	strb r7, [r0, #2]
	ldr r0, _08023D2C @ =0x0000F031
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08023D30 @ =0x02017FB0
	ldr r2, _08023D34 @ =0x00000305
	add r1, r1, r2
	mov r0, #0x20
	b _08024032
	.align 2, 0
_08023D1C: .4byte 0x00000D64
_08023D20: .4byte 0x02019968
_08023D24: .4byte 0x02017FB4
_08023D28: .4byte 0x020192E4
_08023D2C: .4byte 0x0000F031
_08023D30: .4byte 0x02017FB0
_08023D34: .4byte 0x00000305
_08023D38:
	mov r0, r8
	lsl r3, r0, #2
	ldr r1, _08023DA4 @ =0x02017FB4
	add r1, r3, r1
	mov r5, #1
	add r0, r6, #0
	and r0, r5
	ldr r2, _08023DA8 @ =0x00000D64
	add r4, r0, #0
	mul r4, r2
	ldr r0, _08023DAC @ =0x02019AA8
	add r4, r4, r0
	add r4, r4, r3
	add r0, r4, #0
	bl sub_08007558
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	sub r5, r5, r0
	mov r0, #1
	and r5, r0
	lsl r5, r5, #4
	mov r0, #0x11
	neg r0, r0
	ldrb r1, [r4, #1]
	and r0, r1
	orr r0, r5
	strb r0, [r4, #1]
	mov r2, #1
	add r8, r2
	cmp r8, r7
	blt _08023D38
_08023D7A:
	ldr r1, _08023DB0 @ =0x020192E4
	mov r0, #1
	and r6, r0
	ldr r0, _08023DA8 @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	strb r7, [r0, #3]
	ldr r0, _08023DB4 @ =0x0000F032
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08023DB8 @ =0x02017FB0
	ldr r3, _08023DBC @ =0x00000305
	add r1, r1, r3
	mov r0, #0x40
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
	.align 2, 0
_08023DA4: .4byte 0x02017FB4
_08023DA8: .4byte 0x00000D64
_08023DAC: .4byte 0x02019AA8
_08023DB0: .4byte 0x020192E4
_08023DB4: .4byte 0x0000F032
_08023DB8: .4byte 0x02017FB0
_08023DBC: .4byte 0x00000305
_08023DC0:
	ldrh r0, [r6, #2]
	lsr r6, r0, #8
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	mov r0, #1
	sub r6, r0, r6
	mov r3, #0
	mov r8, r3
	cmp r8, r7
	bge _08023E1C
	mov r2, #1
	add r0, r6, #0
	and r0, r2
	ldr r1, _08023E44 @ =0x00000D64
	mul r1, r0
	ldr r0, _08023E48 @ =0x02019BE8
	add r4, r1, r0
	add r5, r4, #0
_08023DE4:
	mov r0, r8
	lsl r1, r0, #2
	ldr r0, _08023E4C @ =0x02017FB4
	add r1, r1, r0
	add r0, r5, #0
	str r2, [sp, #0]
	bl sub_08007558
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	ldr r2, [sp, #0]
	sub r0, r2, r0
	and r0, r2
	lsl r0, r0, #4
	mov r3, #0x11
	neg r3, r3
	add r1, r3, #0
	ldrb r3, [r4, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r4, #1]
	add r4, #4
	add r5, #4
	mov r0, #1
	add r8, r0
	cmp r8, r7
	blt _08023DE4
_08023E1C:
	ldr r1, _08023E50 @ =0x020192E4
	mov r0, #1
	and r6, r0
	ldr r0, _08023E44 @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	strb r7, [r0, #4]
	bl sub_080611AC
	ldr r0, _08023E54 @ =0x0000F033
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08023E58 @ =0x02017FB0
	ldr r2, _08023E5C @ =0x00000305
	add r1, r1, r2
	mov r0, #0x80
	b _08024032
_08023E44: .4byte 0x00000D64
_08023E48: .4byte 0x02019BE8
_08023E4C: .4byte 0x02017FB4
_08023E50: .4byte 0x020192E4
_08023E54: .4byte 0x0000F033
_08023E58: .4byte 0x02017FB0
_08023E5C: .4byte 0x00000305
_08023E60:
	ldr r0, _08023EE8 @ =0x02017FB0
	ldrh r0, [r0, #2]
	lsr r6, r0, #8
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	mov r0, #1
	sub r6, r0, r6
	mov r0, #0
	mov r8, r0
	cmp r8, r7
	bge _08023EBE
	mov r2, #1
	add r0, r6, #0
	and r0, r2
	ldr r1, _08023EEC @ =0x00000D64
	mul r1, r0
	ldr r0, _08023EF0 @ =0x02019D28
	add r4, r1, r0
	add r5, r4, #0
_08023E86:
	mov r3, r8
	lsl r1, r3, #2
	ldr r0, _08023EF4 @ =0x02017FB4
	add r1, r1, r0
	add r0, r5, #0
	str r2, [sp, #0]
	bl sub_08007558
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	ldr r2, [sp, #0]
	sub r0, r2, r0
	and r0, r2
	lsl r0, r0, #4
	mov r3, #0x11
	neg r3, r3
	add r1, r3, #0
	ldrb r3, [r4, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r4, #1]
	add r4, #4
	add r5, #4
	mov r0, #1
	add r8, r0
	cmp r8, r7
	blt _08023E86
_08023EBE:
	ldr r1, _08023EF8 @ =0x020192E4
	mov r0, #1
	and r6, r0
	ldr r0, _08023EEC @ =0x00000D64
	mul r0, r6
	add r0, r0, r1
	strb r7, [r0, #5]
	bl sub_080611AC
	ldr r0, _08023EFC @ =0x0000F034
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08023EE8 @ =0x02017FB0
	ldr r2, _08023F00 @ =0x00000306
	add r1, r1, r2
	mov r0, #1
	b _08024032
	.align 2, 0
_08023EE8: .4byte 0x02017FB0
_08023EEC: .4byte 0x00000D64
_08023EF0: .4byte 0x02019D28
_08023EF4: .4byte 0x02017FB4
_08023EF8: .4byte 0x020192E4
_08023EFC: .4byte 0x0000F034
_08023F00: .4byte 0x00000306
_08023F04:
	ldrh r0, [r6, #2]
	lsr r6, r0, #8
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	mov r0, #1
	sub r6, r0, r6
	mov r0, #0
	mov r8, r0
	cmp r2, r7
	bge _08023F60
	mov r2, #1
	and r6, r2
	ldr r0, _08023F7C @ =0x00000D64
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08023F80 @ =0x02019E68
	add r4, r1, r0
	add r5, r4, #0
_08023F28:
	mov r3, r8
	lsl r1, r3, #2
	ldr r0, _08023F84 @ =0x02017FB4
	add r1, r1, r0
	add r0, r5, #0
	str r2, [sp, #0]
	bl sub_08007558
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	ldr r2, [sp, #0]
	sub r0, r2, r0
	and r0, r2
	lsl r0, r0, #4
	mov r3, #0x11
	neg r3, r3
	add r1, r3, #0
	ldrb r3, [r4, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r4, #1]
	add r4, #4
	add r5, #4
	mov r0, #1
	add r8, r0
	cmp r8, r7
	blt _08023F28
_08023F60:
	bl sub_080611AC
	ldr r0, _08023F88 @ =0x0000F035
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08023F8C @ =0x02017FB0
	ldr r2, _08023F90 @ =0x00000306
	add r1, r1, r2
	mov r0, #1
	b _08024032
	.align 2, 0
_08023F7C: .4byte 0x00000D64
_08023F80: .4byte 0x02019E68
_08023F84: .4byte 0x02017FB4
_08023F88: .4byte 0x0000F035
_08023F8C: .4byte 0x02017FB0
_08023F90: .4byte 0x00000306
_08023F94:
	ldr r0, _08023FA0 @ =0x00000305
	add r1, r6, r0
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
_08023FA0: .4byte 0x00000305
_08023FA4:
	ldr r1, _08023FB4 @ =0x02017FB0
	ldr r3, _08023FB8 @ =0x00000305
	add r1, r1, r3
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
	.align 2, 0
_08023FB4: .4byte 0x02017FB0
_08023FB8: .4byte 0x00000305
_08023FBC:
	ldr r3, _08023FC8 @ =0x00000305
	add r1, r6, r3
	mov r0, #4
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
_08023FC8: .4byte 0x00000305
_08023FCC:
	ldr r1, _08023FDC @ =0x02017FB0
	ldr r3, _08023FE0 @ =0x00000305
	add r1, r1, r3
	mov r0, #8
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
	.align 2, 0
_08023FDC: .4byte 0x02017FB0
_08023FE0: .4byte 0x00000305
_08023FE4:
	ldr r3, _08023FF0 @ =0x00000305
	add r1, r6, r3
	mov r0, #0x10
	ldrb r2, [r1]
	orr r0, r2
	b _08024036
_08023FF0: .4byte 0x00000305
_08023FF4:
	bl sub_08022D5C
	ldr r1, _08024010 @ =0x02017FB0
	ldr r3, _08024014 @ =0x00000307
	add r2, r1, r3
	mov r0, #1
	ldrb r3, [r2]
	orr r0, r3
	strb r0, [r2]
	ldr r0, _08024018 @ =0x0000048C
	add r1, r1, r0
	mov r0, #0
	b _08024036
	.align 2, 0
_08024010: .4byte 0x02017FB0
_08024014: .4byte 0x00000307
_08024018: .4byte 0x0000048C
_0802401C:
	ldr r1, _08024028 @ =0x00000202
	add r0, r6, r1
	strh r2, [r0]
	bl _0802364E @ far jump
	.align 2, 0
_08024028: .4byte 0x00000202
_0802402C:
	ldr r2, _0802403C @ =0x00000307
	add r1, r6, r2
	mov r0, #2
_08024032:
	ldrb r3, [r1]
	orr r0, r3
_08024036:
	strb r0, [r1]
	bl _0802364E @ far jump
_0802403C: .4byte 0x00000307
_08024040:
	bl sub_08077BCC
	ldr r1, _0802404C @ =0x020192E0
	ldr r0, _08024050 @ =0x00001B12
	add r1, r1, r0
	b _0802406C
_0802404C: .4byte 0x020192E0
_08024050: .4byte 0x00001B12
_08024054:
	mov r0, #0xEE
	lsl r0, r0, #8
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	bl sub_08077BCC
	ldr r1, _08024084 @ =0x020192E0
	ldr r3, _08024088 @ =0x00001B12
	add r1, r1, r3
_0802406C:
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_08024074:
	mov r0, #0
_08024076:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08024084: .4byte 0x020192E0
_08024088: .4byte 0x00001B12
	thumb_func_end sub_08023228

