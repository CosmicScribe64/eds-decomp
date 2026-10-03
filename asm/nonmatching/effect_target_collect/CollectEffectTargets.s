	thumb_func_start CollectEffectTargets
CollectEffectTargets: @ 0x08044224
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	str r0, [sp, #8]
	str r2, [sp, #0xC]
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	mov r0, #0
	str r0, [sp, #0x10]
	mov r1, #0
	str r1, [sp, #0x14]
	ldr r0, _08044298 @ =0x0201D810
	mov r2, #0xC3
	lsl r2, r2, #2
	add r2, r2, r0
	mov ip, r2
	strh r1, [r2]
	ldr r1, _0804429C @ =0x0000045C
	mov sl, r0
	cmp r4, r1
	bne _08044258
	bl _08045988 @ far jump
_08044258:
	cmp r4, r1
	ble _0804425E
	b _080443D0
_0804425E:
	mov r0, #0xFC
	lsl r0, r0, #2
	cmp r4, r0
	bne _0804426A
	bl _08044D80 @ far jump
_0804426A:
	cmp r4, r0
	bgt _08044314
	mov r0, #0xD4
	lsl r0, r0, #1
	cmp r4, r0
	bne _08044278
	b _080449B4
_08044278:
	cmp r4, r0
	bgt _080442C4
	cmp r4, #0x65
	bne _08044282
	b _08044630
_08044282:
	cmp r4, #0x65
	bgt _080442A0
	cmp r4, #0xF
	bne _0804428C
	b _080447D4
_0804428C:
	cmp r4, #0x2F
	bne _08044292
	b _0804453C
_08044292:
	bl _08046586 @ far jump
	.align 2, 0
_08044298: .4byte 0x0201D810
_0804429C: .4byte 0x0000045C
_080442A0:
	ldr r0, _080442B8 @ =0x00000191
	cmp r4, r0
	bne _080442A8
	b _080448CC
_080442A8:
	cmp r4, r0
	bgt _080442BC
	sub r0, #0x54
	cmp r4, r0
	bne _080442B4
	b _080446C4
_080442B4:
	bl _08046586 @ far jump
_080442B8: .4byte 0x00000191
_080442BC:
	ldr r0, _080442C0 @ =0x000001A3
	b _080442DC
_080442C0: .4byte 0x000001A3
_080442C4:
	ldr r0, _080442E8 @ =0x0000023D
	cmp r4, r0
	bne _080442CE
	bl _08044AC8 @ far jump
_080442CE:
	cmp r4, r0
	bgt _080442EC
	sub r0, #0x92
	cmp r4, r0
	bne _080442DA
	b _08044A34
_080442DA:
	add r0, #0x4E
_080442DC:
	cmp r4, r0
	bne _080442E2
	b _08044954
_080442E2:
	bl _08046586 @ far jump
	.align 2, 0
_080442E8: .4byte 0x0000023D
_080442EC:
	ldr r0, _08044308 @ =0x000003C6
	cmp r4, r0
	bne _080442F6
	bl _08044C54 @ far jump
_080442F6:
	cmp r4, r0
	bgt _0804430C
	sub r0, #0x15
	cmp r4, r0
	bne _08044304
	bl _08044BB8 @ far jump
_08044304:
	bl _08046586 @ far jump
_08044308: .4byte 0x000003C6
_0804430C:
	ldr r0, _08044310 @ =0x000003EB
	b _0804435E
_08044310: .4byte 0x000003EB
_08044314:
	ldr r0, _08044348 @ =0x00000439
	cmp r4, r0
	bne _0804431E
	bl _080451BC @ far jump
_0804431E:
	cmp r4, r0
	bgt _0804437C
	sub r0, #0x39
	cmp r4, r0
	bne _0804432C
	bl _08044F4C @ far jump
_0804432C:
	cmp r4, r0
	bgt _0804434C
	sub r0, #0xD
	cmp r4, r0
	bne _0804433A
	bl _08044E34 @ far jump
_0804433A:
	add r0, #7
	cmp r4, r0
	bne _08044344
	bl _08044ECC @ far jump
_08044344:
	bl _08046586 @ far jump
_08044348: .4byte 0x00000439
_0804434C:
	mov r0, #0x82
	lsl r0, r0, #3
	cmp r4, r0
	bne _08044358
	bl _08045020 @ far jump
_08044358:
	cmp r4, r0
	bgt _0804436A
	sub r0, #6
_0804435E:
	cmp r4, r0
	bne _08044366
	bl _08044CF8 @ far jump
_08044366:
	bl _08046586 @ far jump
_0804436A:
	ldr r0, _08044378 @ =0x0000041E
	cmp r4, r0
	bne _08044374
	bl _080450A8 @ far jump
_08044374:
	bl _08046586 @ far jump
_08044378: .4byte 0x0000041E
_0804437C:
	ldr r0, _080443A8 @ =0x00000455
	cmp r4, r0
	bne _08044386
	bl _08045674 @ far jump
_08044386:
	cmp r4, r0
	bgt _080443B4
	sub r0, #0xE
	cmp r4, r0
	bne _08044394
	bl _080458D4 @ far jump
_08044394:
	cmp r4, r0
	bgt _080443AC
	sub r0, #4
	cmp r4, r0
	bne _080443A2
	bl _0804522C @ far jump
_080443A2:
	bl _08046586 @ far jump
	.align 2, 0
_080443A8: .4byte 0x00000455
_080443AC:
	ldr r0, _080443B0 @ =0x00000454
	b _08044400
_080443B0: .4byte 0x00000454
_080443B4:
	ldr r0, _080443CC @ =0x00000456
	cmp r4, r0
	bne _080443BE
	bl _080452C8 @ far jump
_080443BE:
	add r0, #4
	cmp r4, r0
	bge _080443C8
	bl _08046586 @ far jump
_080443C8:
	bl _08045790 @ far jump
_080443CC: .4byte 0x00000456
_080443D0:
	ldr r0, _0804440C @ =0x00000526
	cmp r4, r0
	bne _080443DA
	bl _08045E20 @ far jump
_080443DA:
	cmp r4, r0
	bgt _08044488
	sub r0, #0x9E
	cmp r4, r0
	bgt _08044444
	sub r0, #1
	cmp r4, r0
	blt _080443EE
	bl _080458D4 @ far jump
_080443EE:
	sub r0, #0x27
	cmp r4, r0
	bgt _08044410
	sub r0, #1
	cmp r4, r0
	blt _080443FE
	bl _080452C8 @ far jump
_080443FE:
	sub r0, #2
_08044400:
	cmp r4, r0
	bne _08044408
	bl _080452C8 @ far jump
_08044408:
	bl _08046586 @ far jump
_0804440C: .4byte 0x00000526
_08044410:
	ldr r0, _0804442C @ =0x00000463
	cmp r4, r0
	bne _0804441A
	bl _080452C8 @ far jump
_0804441A:
	cmp r4, r0
	bgt _08044430
	sub r0, #1
	cmp r4, r0
	bne _08044428
	bl _08045818 @ far jump
_08044428:
	bl _08046586 @ far jump
_0804442C: .4byte 0x00000463
_08044430:
	ldr r0, _08044440 @ =0x0000047B
	cmp r4, r0
	bne _0804443A
	bl _08045AE8 @ far jump
_0804443A:
	bl _08046586 @ far jump
	.align 2, 0
_08044440: .4byte 0x0000047B
_08044444:
	mov r0, #0x9B
	lsl r0, r0, #3
	cmp r4, r0
	bne _08044450
	bl _08045D18 @ far jump
_08044450:
	cmp r4, r0
	bgt _0804446C
	sub r0, #0x26
	cmp r4, r0
	bne _0804445E
	bl _08045BEC @ far jump
_0804445E:
	add r0, #0xA
	cmp r4, r0
	bne _08044468
	bl _08045C80 @ far jump
_08044468:
	bl _08046586 @ far jump
_0804446C:
	ldr r0, _08044484 @ =0x000004D9
	cmp r4, r0
	bne _08044476
	bl _08045D9C @ far jump
_08044476:
	add r0, #0x42
	cmp r4, r0
	bne _08044480
	bl _08045790 @ far jump
_08044480:
	bl _08046586 @ far jump
_08044484: .4byte 0x000004D9
_08044488:
	ldr r0, _080444C0 @ =0x000005EF
	cmp r4, r0
	bgt _080444DC
	sub r0, #4
	cmp r4, r0
	blt _08044498
	bl _0804623C @ far jump
_08044498:
	sub r0, #4
	cmp r4, r0
	bne _080444A2
	bl _08046084 @ far jump
_080444A2:
	cmp r4, r0
	bgt _080444C4
	sub r0, #0x5A
	cmp r4, r0
	bne _080444B0
	bl _08045F3C @ far jump
_080444B0:
	add r0, #0x12
	cmp r4, r0
	bne _080444BA
	bl _08045FDC @ far jump
_080444BA:
	bl _08046586 @ far jump
	.align 2, 0
_080444C0: .4byte 0x000005EF
_080444C4:
	ldr r0, _080444D8 @ =0x000005E9
	cmp r4, r0
	bne _080444CE
	bl _08046114 @ far jump
_080444CE:
	cmp r4, r0
	ble _080444D6
	bl _080461A4 @ far jump
_080444D6:
	b _08044954
_080444D8: .4byte 0x000005E9
_080444DC:
	ldr r0, _08044504 @ =0x000005FC
	cmp r4, r0
	bne _080444E6
	bl _080463CC @ far jump
_080444E6:
	cmp r4, r0
	bgt _08044508
	sub r0, #0xC
	cmp r4, r0
	bne _080444F4
	bl _080458D4 @ far jump
_080444F4:
	add r0, #4
	cmp r4, r0
	bne _080444FE
	bl _08046348 @ far jump
_080444FE:
	bl _08046586 @ far jump
	.align 2, 0
_08044504: .4byte 0x000005FC
_08044508:
	ldr r0, _08044524 @ =0x0000060B
	cmp r4, r0
	bne _08044512
	bl _08044CF8 @ far jump
_08044512:
	cmp r4, r0
	bgt _08044528
	sub r0, #1
	cmp r4, r0
	bne _08044520
	bl _0804645C @ far jump
_08044520:
	bl _08046586 @ far jump
_08044524: .4byte 0x0000060B
_08044528:
	ldr r0, _08044538 @ =0x0000060D
	cmp r4, r0
	bne _08044532
	bl _08046504 @ far jump
_08044532:
	bl _08046586 @ far jump
	.align 2, 0
_08044538: .4byte 0x0000060D
_0804453C:
	mov r4, #0
	mov r8, r4
	ldr r0, _080445B0 @ =0x020192E4
	mov r1, #1
	ldr r5, [sp, #8]
	and r1, r5
	ldr r2, _080445B4 @ =0x00000D64
	mul r1, r2
	add r3, r1, r0
	ldrb r6, [r3, #3]
	cmp r8, r6
	blt _08044558
	bl _08046586 @ far jump
_08044558:
	ldr r6, _080445B8 @ =0x000007FF
	ldr r5, _080445BC @ =0x0201D810
	add r2, r1, #0
	ldr r7, _080445C0 @ =0x000005DC
	mov ip, r7
	mov r0, #0xC3
	lsl r0, r0, #2
	add r4, r5, r0
	ldr r1, _080445B0 @ =0x020192E4
	mov r9, r1
_0804456C:
	ldr r0, _080445C4 @ =0x02019AA8
	add r0, r2, r0
	ldr r0, [r0]
	lsl r1, r0, #0x14
	lsr r0, r1, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _080445C8 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08044618
	lsr r1, r1, #0x14
	add r0, r1, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _080445C8 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080445D6
	cmp r0, #0x17
	ble _080445CC
	cmp r0, #0x18
	beq _080445D0
	b _080445D6
_080445B0: .4byte 0x020192E4
_080445B4: .4byte 0x00000D64
_080445B8: .4byte 0x000007FF
_080445BC: .4byte 0x0201D810
_080445C0: .4byte 0x000005DC
_080445C4: .4byte 0x02019AA8
_080445C8: .4byte gCardStats
_080445CC:
	mov r0, #0
	b _080445EA
_080445D0:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080445EA
_080445D6:
	and r1, r6
	lsl r0, r1, #2
	ldr r1, _08044628 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080445EA:
	cmp r0, ip
	bhi _08044618
	ldrh r7, [r4]
	lsl r1, r7, #2
	add r0, r5, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, _0804462C @ =0x000007C4
	add r0, r9
	add r0, r2, r0
	ldr r0, [r0]
	str r0, [r1]
	ldrh r0, [r4]
	lsl r1, r0, #1
	mov r7, #0x83
	lsl r7, r7, #2
	add r0, r5, r7
	add r1, r1, r0
	mov r0, #2
	strh r0, [r1]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044618:
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r3, #3]
	cmp r8, r1
	blt _0804456C
	bl _08046586 @ far jump
_08044628: .4byte gCardStats
_0804462C: .4byte 0x000007C4
_08044630:
	mov r2, #0
	mov r8, r2
	ldr r3, _080446B0 @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #8]
	and r0, r4
	ldr r1, _080446B4 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #4]
	cmp r8, r5
	blt _0804464E
	bl _08046586 @ far jump
_0804464E:
	ldr r6, _080446B8 @ =0x00000904
	add r0, r3, r6
	mov r6, sl
	mov r4, ip
	add r5, r1, #0
	add r3, r2, r0
	ldr r7, _080446BC @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_08044664:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080446C0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0804469E
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_0804469E:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _08044664
	bl _08046586 @ far jump
	.align 2, 0
_080446B0: .4byte 0x020192E4
_080446B4: .4byte 0x00000D64
_080446B8: .4byte 0x00000904
_080446BC: .4byte 0x000007FF
_080446C0: .4byte gCardStats
_080446C4:
	mov r2, #0
	mov r8, r2
	ldr r0, _080447B8 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _080447BC @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r4, [r2, #2]
	cmp r8, r4
	bge _08044740
	mov r5, #1
	mov ip, r5
	ldr r5, _080447C0 @ =0x0201D810
	mov r6, #0xC3
	lsl r6, r6, #2
	add r4, r5, r6
	add r7, r0, #0
	ldr r0, _080447C4 @ =0x00000684
	add r7, r7, r0
	mov r9, r7
	add r3, r1, #0
	mov r1, #0x83
	lsl r1, r1, #2
	add r7, r5, r1
	add r6, r2, #0
_080446FA:
	mov r2, r9
	add r0, r3, r2
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080447C8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0x3D
	blt _08044734
	cmp r1, #0x3E
	ble _08044718
	ldr r0, _080447CC @ =0x000004E1
	cmp r1, r0
	bne _08044734
_08044718:
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, ip
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044734:
	add r3, #4
	mov r2, #1
	add r8, r2
	ldrb r0, [r6, #2]
	cmp r8, r0
	blt _080446FA
_08044740:
	mov r1, #0
	mov r8, r1
	mov r0, #1
	ldr r2, [sp, #8]
	and r0, r2
	ldr r1, _080447BC @ =0x00000D64
	mul r0, r1
	ldr r3, _080447B8 @ =0x020192E4
	add r1, r0, r3
	ldrb r4, [r1, #3]
	cmp r8, r4
	blt _0804475C
	bl _08046586 @ far jump
_0804475C:
	ldr r5, _080447C0 @ =0x0201D810
	mov r6, #0xC3
	lsl r6, r6, #2
	add r4, r5, r6
	add r3, r0, #0
	mov r0, #0x83
	lsl r0, r0, #2
	add r7, r5, r0
	add r6, r1, #0
_0804476E:
	ldr r0, _080447D0 @ =0x02019AA8
	add r0, r3, r0
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080447C8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0x3D
	blt _080447A8
	cmp r1, #0x3E
	ble _0804478C
	ldr r0, _080447CC @ =0x000004E1
	cmp r1, r0
	bne _080447A8
_0804478C:
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_080447A8:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #3]
	cmp r8, r1
	blt _0804476E
	bl _08046586 @ far jump
_080447B8: .4byte 0x020192E4
_080447BC: .4byte 0x00000D64
_080447C0: .4byte 0x0201D810
_080447C4: .4byte 0x00000684
_080447C8: .4byte gCardIdToNumber
_080447CC: .4byte 0x000004E1
_080447D0: .4byte 0x02019AA8
_080447D4:
	mov r2, #0
	mov r8, r2
	ldr r0, _080448B0 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _080448B4 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r4, [r2, #2]
	cmp r8, r4
	bge _0804483E
	mov r5, #1
	mov r9, r5
	add r6, r0, #0
	ldr r7, _080448B8 @ =0x00000684
	add r0, r6, r7
	mov r6, sl
	mov r4, ip
	add r5, r2, #0
	add r3, r1, r0
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_08044804:
	ldr r2, [r3]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080448BC @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _080448C0 @ =0x000004B2
	ldrh r0, [r0]
	cmp r0, r1
	bne _08044832
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, r9
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044832:
	add r3, #4
	mov r2, #1
	add r8, r2
	ldrb r0, [r5, #2]
	cmp r8, r0
	blt _08044804
_0804483E:
	mov r1, #0
	mov r8, r1
	mov r0, #1
	ldr r2, [sp, #8]
	and r0, r2
	ldr r1, _080448B4 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	ldr r4, _080448B0 @ =0x020192E4
	add r2, r3, r4
	ldrb r5, [r2, #3]
	cmp r8, r5
	blt _0804485C
	bl _08046586 @ far jump
_0804485C:
	ldr r6, _080448C4 @ =0x000007C4
	add r0, r4, r6
	ldr r5, _080448C8 @ =0x0201D810
	mov r7, #0xC3
	lsl r7, r7, #2
	add r4, r5, r7
	add r6, r2, #0
	add r3, r3, r0
	mov r0, #0x83
	lsl r0, r0, #2
	add r7, r5, r0
_08044872:
	ldr r2, [r3]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080448BC @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _080448C0 @ =0x000004B2
	ldrh r0, [r0]
	cmp r0, r1
	bne _080448A0
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_080448A0:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #3]
	cmp r8, r1
	blt _08044872
	bl _08046586 @ far jump
_080448B0: .4byte 0x020192E4
_080448B4: .4byte 0x00000D64
_080448B8: .4byte 0x00000684
_080448BC: .4byte gCardIdToNumber
_080448C0: .4byte 0x000004B2
_080448C4: .4byte 0x000007C4
_080448C8: .4byte 0x0201D810
_080448CC:
	mov r2, #0
	mov r8, r2
	ldr r0, _08044940 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08044944 @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r4, [r0, #4]
	cmp r8, r4
	blt _080448E8
	bl _08046586 @ far jump
_080448E8:
	mov r6, sl
	mov r4, ip
	add r3, r1, #0
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
	add r5, r0, #0
_080448F6:
	ldr r0, _08044948 @ =0x02019BE8
	add r0, r3, r0
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804494C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08044950 @ =0x000003EB
	cmp r1, r0
	beq _08044912
	add r0, #0x1F
	cmp r1, r0
	bne _0804492E
_08044912:
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_0804492E:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _080448F6
	bl _08046586 @ far jump
	.align 2, 0
_08044940: .4byte 0x020192E4
_08044944: .4byte 0x00000D64
_08044948: .4byte 0x02019BE8
_0804494C: .4byte gCardIdToNumber
_08044950: .4byte 0x000003EB
_08044954:
	mov r2, #0
	mov r8, r2
	ldr r5, _080449A0 @ =0x020192E4
	mov r0, #1
	ldr r3, [sp, #8]
	and r0, r3
	ldr r1, _080449A4 @ =0x00000D64
	mul r0, r1
	add r1, r0, r5
	ldrb r4, [r1, #5]
	cmp r8, r4
	blt _08044970
	bl _08046586 @ far jump
_08044970:
	ldr r3, _080449A8 @ =0x0201DB1C
	ldr r7, _080449AC @ =0xFFFFFD00
	add r6, r3, r7
	add r4, r1, #0
	add r2, r0, #0
	ldr r0, _080449B0 @ =0x00000A44
	add r5, r5, r0
_0804497E:
	ldrh r7, [r3]
	lsl r1, r7, #2
	add r1, r1, r6
	add r0, r2, r5
	ldr r0, [r0]
	str r0, [r1]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r4, #5]
	cmp r8, r1
	blt _0804497E
	bl _08046586 @ far jump
_080449A0: .4byte 0x020192E4
_080449A4: .4byte 0x00000D64
_080449A8: .4byte 0x0201DB1C
_080449AC: .4byte 0xFFFFFD00
_080449B0: .4byte 0x00000A44
_080449B4:
	mov r2, #0
	mov r8, r2
	ldr r3, _08044A24 @ =0x020192E4
	mov r0, #1
	ldr r5, [sp, #8]
	and r0, r5
	ldr r1, _08044A28 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r6, [r1, #3]
	cmp r8, r6
	blt _080449D2
	bl _08046586 @ far jump
_080449D2:
	ldr r7, _08044A2C @ =0x000007C4
	add r0, r3, r7
	mov r9, r4
	mov r4, ip
	add r6, r1, #0
	add r3, r2, r0
	mov r5, sl
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_080449E6:
	ldr r2, [r3]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08044A30 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r9
	bne _08044A12
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044A12:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #3]
	cmp r8, r1
	blt _080449E6
	bl _08046586 @ far jump
	.align 2, 0
_08044A24: .4byte 0x020192E4
_08044A28: .4byte 0x00000D64
_08044A2C: .4byte 0x000007C4
_08044A30: .4byte gCardIdToNumber
_08044A34:
	mov r2, #0
	mov r8, r2
	ldr r3, _08044AB4 @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #8]
	and r0, r4
	ldr r1, _08044AB8 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #4]
	cmp r8, r5
	blt _08044A52
	bl _08046586 @ far jump
_08044A52:
	ldr r6, _08044ABC @ =0x00000904
	add r0, r3, r6
	mov r6, sl
	mov r4, ip
	add r5, r1, #0
	add r3, r2, r0
	ldr r7, _08044AC0 @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_08044A68:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08044AC4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08044AA2
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044AA2:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _08044A68
	bl _08046586 @ far jump
	.align 2, 0
_08044AB4: .4byte 0x020192E4
_08044AB8: .4byte 0x00000D64
_08044ABC: .4byte 0x00000904
_08044AC0: .4byte 0x000007FF
_08044AC4: .4byte gCardStats
_08044AC8:
	mov r2, #0
	mov r8, r2
	ldr r0, _08044B34 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08044B38 @ =0x00000D64
	mul r1, r2
	add r3, r1, r0
	ldrb r4, [r3, #3]
	cmp r8, r4
	blt _08044AE4
	bl _08046586 @ far jump
_08044AE4:
	ldr r6, _08044B3C @ =0x000007FF
	mov r5, sl
	add r2, r1, #0
	mov r4, ip
	add r7, r0, #0
	mov r9, r7
_08044AF0:
	ldr r0, _08044B40 @ =0x02019AA8
	add r0, r2, r0
	ldr r0, [r0]
	lsl r1, r0, #0x14
	lsr r0, r1, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _08044B44 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08044B96
	lsr r1, r1, #0x14
	add r0, r1, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _08044B44 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08044B52
	cmp r0, #0x17
	ble _08044B48
	cmp r0, #0x18
	beq _08044B4C
	b _08044B52
_08044B34: .4byte 0x020192E4
_08044B38: .4byte 0x00000D64
_08044B3C: .4byte 0x000007FF
_08044B40: .4byte 0x02019AA8
_08044B44: .4byte gCardStats
_08044B48:
	mov r0, #0
	b _08044B66
_08044B4C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08044B66
_08044B52:
	and r1, r6
	lsl r0, r1, #2
	ldr r1, _08044BA8 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08044BAC @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08044B66:
	ldr r7, _08044BB0 @ =0x000005DC
	cmp r0, r7
	bhi _08044B96
	ldrh r0, [r4]
	lsl r1, r0, #2
	add r0, r5, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, _08044BB4 @ =0x000007C4
	add r0, r9
	add r0, r2, r0
	ldr r0, [r0]
	str r0, [r1]
	ldrh r7, [r4]
	lsl r1, r7, #1
	mov r7, #0x83
	lsl r7, r7, #2
	add r0, r5, r7
	add r1, r1, r0
	mov r0, #2
	strh r0, [r1]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044B96:
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r3, #3]
	cmp r8, r1
	blt _08044AF0
	bl _08046586 @ far jump
	.align 2, 0
_08044BA8: .4byte gCardStats
_08044BAC: .4byte 0x000001FF
_08044BB0: .4byte 0x000005DC
_08044BB4: .4byte 0x000007C4
_08044BB8:
	mov r2, #0
	mov r8, r2
	ldr r3, _08044C3C @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #8]
	and r0, r4
	ldr r1, _08044C40 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #3]
	cmp r8, r5
	blt _08044BD6
	bl _08046586 @ far jump
_08044BD6:
	ldr r6, _08044C44 @ =0x000007C4
	add r0, r3, r6
	ldr r5, _08044C48 @ =0x0201D810
	mov r7, #0xC3
	lsl r7, r7, #2
	add r4, r5, r7
	add r6, r1, #0
	add r3, r2, r0
	ldr r0, _08044C4C @ =0x000007FF
	mov r9, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r7, r5, r1
_08044BF0:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08044C50 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08044C2A
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044C2A:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #3]
	cmp r8, r1
	blt _08044BF0
	bl _08046586 @ far jump
	.align 2, 0
_08044C3C: .4byte 0x020192E4
_08044C40: .4byte 0x00000D64
_08044C44: .4byte 0x000007C4
_08044C48: .4byte 0x0201D810
_08044C4C: .4byte 0x000007FF
_08044C50: .4byte gCardStats
_08044C54:
	mov r2, #0
	mov r8, r2
	ldr r0, _08044CE0 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08044CE4 @ =0x00000D64
	mul r2, r1
	add r6, r2, r0
	ldrb r4, [r6, #2]
	cmp r8, r4
	blt _08044C70
	bl _08046586 @ far jump
_08044C70:
	add r5, r0, #0
	ldr r7, _08044CE8 @ =0x00000684
	add r1, r5, r7
	mov r7, sl
	mov r5, ip
	add r3, r0, #0
	ldr r4, _08044CEC @ =0x000007C4
	add r0, r3, r4
	add r4, r2, r0
	add r3, r2, r1
	ldr r0, _08044CF0 @ =0x000007FF
	mov ip, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, sl
	mov r9, r1
_08044C90:
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08044CCE
	mov r2, ip
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08044CF4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r2, r0, #0x14
	cmp r2, #1
	bne _08044CCE
	ldrh r0, [r5]
	lsl r1, r0, #2
	add r0, r7, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r4]
	str r0, [r1]
	ldrh r1, [r5]
	lsl r0, r1, #1
	add r0, r9
	strh r2, [r0]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08044CCE:
	add r4, #4
	add r3, #4
	mov r2, #1
	add r8, r2
	ldrb r0, [r6, #2]
	cmp r8, r0
	blt _08044C90
	bl _08046586 @ far jump
_08044CE0: .4byte 0x020192E4
_08044CE4: .4byte 0x00000D64
_08044CE8: .4byte 0x00000684
_08044CEC: .4byte 0x000007C4
_08044CF0: .4byte 0x000007FF
_08044CF4: .4byte gCardStats
_08044CF8:
	mov r1, #0
	mov r8, r1
	ldr r3, _08044D70 @ =0x020192E4
	mov r0, #1
	ldr r2, [sp, #8]
	and r0, r2
	ldr r1, _08044D74 @ =0x00000D64
	mul r1, r0
	add r2, r1, r3
	ldrb r4, [r2, #5]
	cmp r8, r4
	blt _08044D14
	bl _08046586 @ far jump
_08044D14:
	ldr r5, _08044D78 @ =0x00000A44
	add r0, r3, r5
	add r7, r2, #0
	add r4, r1, r0
	ldr r6, _08044D7C @ =0x0201D810
	mov r0, #0xC3
	lsl r0, r0, #2
	add r5, r6, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, r1, r6
	mov r9, r1
_08044D2C:
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r0, [sp, #8]
	mov r2, sp
	bl FindFusionMaterials
	cmp r0, #0
	beq _08044D5C
	ldrh r2, [r5]
	lsl r1, r2, #2
	add r0, r6, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r4]
	str r0, [r1]
	ldrh r3, [r5]
	lsl r0, r3, #1
	add r0, r9
	mov r1, #8
	strh r1, [r0]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08044D5C:
	add r4, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r7, #5]
	cmp r8, r1
	blt _08044D2C
	ldr r2, _08044D7C @ =0x0201D810
	mov sl, r2
	bl _08046586 @ far jump
_08044D70: .4byte 0x020192E4
_08044D74: .4byte 0x00000D64
_08044D78: .4byte 0x00000A44
_08044D7C: .4byte 0x0201D810
_08044D80:
	mov r6, #0
	mov r7, ip
_08044D84:
	mov r3, #0
	mov r8, r3
	mov r1, #1
	and r1, r6
	ldr r2, _08044E20 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	ldr r4, _08044E24 @ =0x020192E4
	add r0, r0, r4
	add r5, r6, #1
	mov sl, r5
	ldrb r0, [r0, #4]
	cmp r8, r0
	bge _08044E16
	add r5, r1, #0
	ldr r0, _08044E28 @ =0x0201D810
	mov r9, r0
	mov r3, #0x83
	lsl r3, r3, #2
	add r3, r9
_08044DAC:
	mov r4, r8
	lsl r1, r4, #2
	add r0, r5, #0
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08044E2C @ =0x02019BE8
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _08044E30 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08044E02
	add r0, r6, #0
	mov r1, r8
	str r3, [sp, #0x24]
	bl CanReviveGraveyardCard
	lsl r0, r0, #0x10
	ldr r3, [sp, #0x24]
	cmp r0, #0
	beq _08044E02
	ldrh r2, [r7]
	lsl r1, r2, #2
	mov r0, r9
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r4]
	str r0, [r1]
	ldrh r4, [r7]
	lsl r0, r4, #1
	add r0, r0, r3
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
_08044E02:
	mov r0, #1
	add r8, r0
	ldr r1, _08044E24 @ =0x020192E4
	ldr r2, _08044E20 @ =0x00000D64
	add r0, r5, #0
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08044DAC
_08044E16:
	mov r6, sl
	cmp r6, #1
	ble _08044D84
	bl _08045F1C @ far jump
_08044E20: .4byte 0x00000D64
_08044E24: .4byte 0x020192E4
_08044E28: .4byte 0x0201D810
_08044E2C: .4byte 0x02019BE8
_08044E30: .4byte gCardStats
_08044E34:
	mov r2, #1
	str r2, [sp, #0x14]
	mov r3, #0
	mov r8, r3
	ldr r3, _08044EB8 @ =0x020192E4
	ldr r4, [sp, #8]
	sub r0, r2, r4
	and r0, r2
	ldr r1, _08044EBC @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #4]
	cmp r8, r5
	blt _08044E56
	bl _08046586 @ far jump
_08044E56:
	ldr r6, _08044EC0 @ =0x00000904
	add r0, r3, r6
	mov r6, sl
	mov r4, ip
	add r5, r1, #0
	add r3, r2, r0
	ldr r7, _08044EC4 @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_08044E6C:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08044EC8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08044EA6
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08044EA6:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _08044E6C
	bl _08046586 @ far jump
	.align 2, 0
_08044EB8: .4byte 0x020192E4
_08044EBC: .4byte 0x00000D64
_08044EC0: .4byte 0x00000904
_08044EC4: .4byte 0x000007FF
_08044EC8: .4byte gCardStats
_08044ECC:
	mov r2, #1
	str r2, [sp, #0x14]
	mov r3, #0
	mov r8, r3
	ldr r6, _08044F3C @ =0x020192E4
	ldr r4, [sp, #8]
	sub r0, r2, r4
	and r0, r2
	ldr r1, _08044F40 @ =0x00000D64
	mul r0, r1
	add r1, r0, r6
	ldrb r5, [r1, #3]
	cmp r8, r5
	blt _08044EEC
	bl _08046586 @ far jump
_08044EEC:
	ldr r4, _08044F44 @ =0x0201D810
	mov r7, #0xC3
	lsl r7, r7, #2
	add r3, r4, r7
	add r5, r1, #0
	add r2, r0, #0
	ldr r0, _08044F48 @ =0x000007C4
	add r7, r6, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r6, r4, r1
_08044F02:
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r0, r4, #0
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, r7
	ldr r0, [r0]
	str r0, [r1]
	ldrh r1, [r3]
	lsl r0, r1, #1
	add r0, r0, r6
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #3]
	cmp r8, r1
	blt _08044F32
	bl _08046586 @ far jump
_08044F32:
	mov r0, r8
	cmp r0, #4
	ble _08044F02
	bl _08046586 @ far jump
_08044F3C: .4byte 0x020192E4
_08044F40: .4byte 0x00000D64
_08044F44: .4byte 0x0201D810
_08044F48: .4byte 0x000007C4
_08044F4C:
	mov r1, #1
	str r1, [sp, #0x14]
	mov r2, #0
	mov r8, r2
	ldr r0, _08045010 @ =0x020192E4
	ldr r1, [sp, #8]
	ldr r3, [sp, #0x14]
	and r1, r3
	ldr r2, _08045014 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r4, [r2, #4]
	cmp r8, r4
	bge _08044FA8
	mov r5, sl
	mov r3, ip
	add r4, r2, #0
	add r2, r1, #0
	add r6, r0, #0
	ldr r0, _08045018 @ =0x00000904
	add r7, r6, r0
	mov r6, #0x83
	lsl r6, r6, #2
	add r6, sl
_08044F7C:
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r0, r5, #0
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, r7
	ldr r0, [r0]
	str r0, [r1]
	ldrh r1, [r3]
	lsl r0, r1, #1
	add r0, r0, r6
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r4, #4]
	cmp r8, r1
	blt _08044F7C
_08044FA8:
	mov r2, #0
	mov r8, r2
	mov r0, #1
	ldr r3, [sp, #8]
	sub r1, r0, r3
	and r1, r0
	ldr r0, _08045014 @ =0x00000D64
	mul r0, r1
	ldr r4, _08045010 @ =0x020192E4
	add r2, r0, r4
	ldrb r5, [r2, #4]
	cmp r8, r5
	blt _08044FC6
	bl _08046586 @ far jump
_08044FC6:
	ldr r4, _0804501C @ =0x0201D810
	mov r6, #0xC3
	lsl r6, r6, #2
	add r3, r4, r6
	add r5, r2, #0
	add r2, r0, #0
	ldr r0, _08045010 @ =0x020192E4
	ldr r1, _08045018 @ =0x00000904
	add r7, r0, r1
	mov r0, #0x83
	lsl r0, r0, #2
	add r6, r4, r0
_08044FDE:
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r0, r4, #0
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, r7
	ldr r0, [r0]
	str r0, [r1]
	ldrh r1, [r3]
	lsl r0, r1, #1
	add r0, r0, r6
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _08044FDE
	bl _08046586 @ far jump
	.align 2, 0
_08045010: .4byte 0x020192E4
_08045014: .4byte 0x00000D64
_08045018: .4byte 0x00000904
_0804501C: .4byte 0x0201D810
_08045020:
	mov r2, #0
	mov r8, r2
	ldr r0, _08045094 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08045098 @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r4, [r0, #3]
	cmp r8, r4
	blt _0804503C
	bl _08046586 @ far jump
_0804503C:
	mov r6, sl
	mov r4, ip
	add r3, r1, #0
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
	add r5, r0, #0
_0804504A:
	ldr r0, _0804509C @ =0x02019AA8
	add r0, r3, r0
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080450A0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080450A4 @ =0x000003EB
	cmp r1, r0
	beq _08045066
	add r0, #0x1F
	cmp r1, r0
	bne _08045082
_08045066:
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045082:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #3]
	cmp r8, r1
	blt _0804504A
	bl _08046586 @ far jump
	.align 2, 0
_08045094: .4byte 0x020192E4
_08045098: .4byte 0x00000D64
_0804509C: .4byte 0x02019AA8
_080450A0: .4byte gCardIdToNumber
_080450A4: .4byte 0x000003EB
_080450A8:
	mov r2, #0
	mov r8, r2
	ldr r1, _0804510C @ =0x020192E4
	mov r2, #1
	ldr r3, [sp, #8]
	and r2, r3
	ldr r3, _08045110 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r8, r0
	bge _08045196
	add r5, r2, #0
	ldr r7, _08045114 @ =0x0201D810
	mov sl, r1
	mov r6, #0
	ldr r4, _08045118 @ =0x000007FF
	mov r9, r4
	mov r0, #0xC3
	lsl r0, r0, #2
	add r4, r7, r0
_080450D4:
	add r0, r5, #0
	mul r0, r3
	add r0, r6, r0
	ldr r1, _0804511C @ =0x02019AA8
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08045120 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08045182
	cmp r0, #0x15
	blt _0804512E
	cmp r0, #0x17
	ble _08045124
	cmp r0, #0x18
	beq _08045128
	b _0804512E
_0804510C: .4byte 0x020192E4
_08045110: .4byte 0x00000D64
_08045114: .4byte 0x0201D810
_08045118: .4byte 0x000007FF
_0804511C: .4byte 0x02019AA8
_08045120: .4byte gCardStats
_08045124:
	mov r1, #0
	b _08045146
_08045128:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _08045146
_0804512E:
	add r0, r2, #0
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _080451A4 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_08045146:
	ldr r0, _080451A8 @ =0x000005DC
	cmp r1, r0
	bhi _08045182
	add r0, r2, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08045182
	ldrh r0, [r4]
	lsl r2, r0, #2
	add r0, r7, #0
	add r0, #0xC
	add r2, r2, r0
	ldr r0, _080451AC @ =0x00000D64
	mul r0, r5
	add r0, r6, r0
	ldr r1, _080451B0 @ =0x000007C4
	add r1, sl
	add r0, r0, r1
	ldr r0, [r0]
	str r0, [r2]
	ldrh r1, [r4]
	lsl r0, r1, #1
	ldr r2, _080451B4 @ =0x0201DA1C
	add r0, r0, r2
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045182:
	add r6, #4
	mov r3, #1
	add r8, r3
	ldr r3, _080451AC @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, sl
	ldrb r0, [r0, #3]
	cmp r8, r0
	blt _080450D4
_08045196:
	mov r4, #1
	str r4, [sp, #0x10]
	ldr r5, _080451B8 @ =0x0201D810
	mov sl, r5
	bl _08046586 @ far jump
	.align 2, 0
_080451A4: .4byte gCardStats
_080451A8: .4byte 0x000005DC
_080451AC: .4byte 0x00000D64
_080451B0: .4byte 0x000007C4
_080451B4: .4byte 0x0201DA1C
_080451B8: .4byte 0x0201D810
_080451BC:
	mov r6, #1
	str r6, [sp, #0x14]
	mov r7, #0
	mov r8, r7
	ldr r6, _08045220 @ =0x020192E4
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x14]
	and r0, r1
	ldr r1, _08045224 @ =0x00000D64
	mul r0, r1
	add r1, r0, r6
	ldrb r2, [r1, #3]
	cmp r8, r2
	blt _080451DC
	bl _08046586 @ far jump
_080451DC:
	mov r5, sl
	mov r3, ip
	add r4, r1, #0
	add r2, r0, #0
	ldr r0, _08045228 @ =0x000007C4
	add r7, r6, r0
	mov r6, #0x83
	lsl r6, r6, #2
	add r6, sl
_080451EE:
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r0, r5, #0
	add r0, #0xC
	add r1, r1, r0
	add r0, r2, r7
	ldr r0, [r0]
	str r0, [r1]
	ldrh r1, [r3]
	lsl r0, r1, #1
	add r0, r0, r6
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r4, #3]
	cmp r8, r1
	blt _080451EE
	bl _08046586 @ far jump
	.align 2, 0
_08045220: .4byte 0x020192E4
_08045224: .4byte 0x00000D64
_08045228: .4byte 0x000007C4
_0804522C:
	mov r2, #0
	mov r8, r2
	ldr r3, _080452B0 @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #8]
	sub r1, r0, r4
	and r1, r0
	ldr r0, _080452B4 @ =0x00000D64
	mul r1, r0
	add r2, r1, r3
	ldrb r5, [r2, #4]
	cmp r8, r5
	blt _0804524A
	bl _08046586 @ far jump
_0804524A:
	ldr r6, _080452B8 @ =0x00000904
	add r0, r3, r6
	ldr r5, _080452BC @ =0x0201D810
	mov r7, #0xC3
	lsl r7, r7, #2
	add r4, r5, r7
	add r6, r2, #0
	add r3, r1, r0
	ldr r0, _080452C0 @ =0x000007FF
	mov r9, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r7, r5, r1
_08045264:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080452C4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0804529E
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_0804529E:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #4]
	cmp r8, r1
	blt _08045264
	bl _08046586 @ far jump
	.align 2, 0
_080452B0: .4byte 0x020192E4
_080452B4: .4byte 0x00000D64
_080452B8: .4byte 0x00000904
_080452BC: .4byte 0x0201D810
_080452C0: .4byte 0x000007FF
_080452C4: .4byte gCardStats
_080452C8:
	mov r2, #0
	mov r8, r2
	ldr r0, _08045328 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r3, _0804532C @ =0x00000D64
	mul r1, r3
	add r1, r1, r0
	ldrb r1, [r1, #3]
	cmp r8, r1
	blt _080452E4
	bl _08046586 @ far jump
_080452E4:
	mov r0, #1
	ldr r5, [sp, #8]
	and r0, r5
	mov r6, r8
	lsl r2, r6, #2
	mul r0, r3
	add r0, r2, r0
	ldr r1, _08045330 @ =0x02019AA8
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldr r0, _08045334 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _08045338 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r6, r2, #0
	cmp r0, #0x14
	bls _08045318
	b _08045648
_08045318:
	cmp r0, #0x15
	blt _08045346
	cmp r0, #0x17
	ble _0804533C
	cmp r0, #0x18
	beq _08045340
	b _08045346
	.align 2, 0
_08045328: .4byte 0x020192E4
_0804532C: .4byte 0x00000D64
_08045330: .4byte 0x02019AA8
_08045334: .4byte 0x000007FF
_08045338: .4byte gCardStats
_0804533C:
	mov r1, #0
	b _0804535C
_08045340:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _0804535C
_08045346:
	ldr r0, _0804537C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08045380 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_0804535C:
	ldr r0, _08045384 @ =0x000005DC
	cmp r1, r0
	bls _08045364
	b _08045648
_08045364:
	mov r2, #0
	ldr r5, _08045388 @ =0xFFFFFBAC
	add r0, r4, r5
	cmp r0, #0xF
	bls _08045370
	b _08045488
_08045370:
	lsl r0, r0, #2
	ldr r1, _0804538C @ =0x08045390
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804537C: .4byte 0x000007FF
_08045380: .4byte gCardStats
_08045384: .4byte 0x000005DC
_08045388: .4byte 0xFFFFFBAC
_0804538C: .4byte 0x08045390
_08045390:
	.4byte _080453D0
	.4byte _08045488
	.4byte _080453F0
	.4byte _08045488
	.4byte _08045488
	.4byte _08045488
	.4byte _08045488
	.4byte _08045488
	.4byte _08045488
	.4byte _08045410
	.4byte _08045488
	.4byte _08045430
	.4byte _08045450
	.4byte _08045488
	.4byte _08045488
	.4byte _08045470
_080453D0:
	ldr r0, _080453E8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _080453EC @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	mov r1, #0
	cmp r0, #5
	bne _08045486
	b _08045484
	.align 2, 0
_080453E8: .4byte 0x000007FF
_080453EC: .4byte gCardStats
_080453F0:
	ldr r0, _08045408 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0804540C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	mov r1, #0
	cmp r0, #4
	bne _08045486
	b _08045484
	.align 2, 0
_08045408: .4byte 0x000007FF
_0804540C: .4byte gCardStats
_08045410:
	ldr r0, _08045428 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r2, _0804542C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	mov r1, #0
	cmp r0, #1
	bne _08045486
	b _08045484
	.align 2, 0
_08045428: .4byte 0x000007FF
_0804542C: .4byte gCardStats
_08045430:
	ldr r0, _08045448 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r5, _0804544C @ =0x08621DE0
	add r0, r0, r5
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	mov r1, #0
	cmp r0, #3
	bne _08045486
	b _08045484
	.align 2, 0
_08045448: .4byte 0x000007FF
_0804544C: .4byte gCardStats
_08045450:
	ldr r0, _08045468 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _0804546C @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	mov r1, #0
	cmp r0, #6
	bne _08045486
	b _08045484
	.align 2, 0
_08045468: .4byte 0x000007FF
_0804546C: .4byte gCardStats
_08045470:
	ldr r0, _080454A0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080454A4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	mov r1, #0
	cmp r0, #2
	bne _08045486
_08045484:
	mov r1, #1
_08045486:
	add r2, r1, #0
_08045488:
	ldr r0, _080454A0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r5, _080454A8 @ =0x08622AB4
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _080454AC @ =0x00000776
	cmp r1, r0
	bne _080454B0
	mov r0, #3
	b _08045512
	.align 2, 0
_080454A0: .4byte 0x000007FF
_080454A4: .4byte gCardStats
_080454A8: .4byte gCardIdToNumber
_080454AC: .4byte 0x00000776
_080454B0:
	cmp r1, r0
	blt _080454C0
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080454C0
	mov r0, #1
	b _08045512
_080454C0:
	ldr r0, _080454E4 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _080454E8 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080454F2
	cmp r0, #0x16
	bgt _080454EC
	cmp r0, #0x15
	beq _080454F6
	b _080454FE
	.align 2, 0
_080454E4: .4byte 0x000007FF
_080454E8: .4byte gCardStats
_080454EC:
	cmp r0, #0x17
	beq _080454FA
	b _080454FE
_080454F2:
	mov r0, #7
	b _08045512
_080454F6:
	mov r0, #8
	b _08045512
_080454FA:
	mov r0, #9
	b _08045512
_080454FE:
	ldr r0, _0804552C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08045530 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08045512:
	cmp r0, #3
	beq _080455A2
	ldr r0, _0804552C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r5, _08045534 @ =0x08622AB4
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _08045538 @ =0x00000776
	cmp r1, r0
	bne _0804553C
	mov r0, #3
	b _0804559E
_0804552C: .4byte 0x000007FF
_08045530: .4byte gCardStats
_08045534: .4byte gCardIdToNumber
_08045538: .4byte 0x00000776
_0804553C:
	cmp r1, r0
	blt _0804554C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0804554C
	mov r0, #1
	b _0804559E
_0804554C:
	ldr r0, _08045570 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _08045574 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0804557E
	cmp r0, #0x16
	bgt _08045578
	cmp r0, #0x15
	beq _08045582
	b _0804558A
	.align 2, 0
_08045570: .4byte 0x000007FF
_08045574: .4byte gCardStats
_08045578:
	cmp r0, #0x17
	beq _08045586
	b _0804558A
_0804557E:
	mov r0, #7
	b _0804559E
_08045582:
	mov r0, #8
	b _0804559E
_08045586:
	mov r0, #9
	b _0804559E
_0804558A:
	ldr r0, _080455D0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080455D4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0804559E:
	cmp r0, #2
	bne _080455A4
_080455A2:
	mov r2, #0
_080455A4:
	ldr r0, _080455D0 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #1
	ldr r3, _080455D8 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	mov r0, #0xB8
	lsl r0, r0, #1
	cmp r1, r0
	beq _08045604
	cmp r1, r0
	bgt _080455E2
	cmp r1, #0x3E
	beq _08045604
	cmp r1, #0x3E
	bgt _080455DC
	cmp r1, #0x38
	bgt _08045606
	cmp r1, #0x37
	blt _08045606
	b _08045604
	.align 2, 0
_080455D0: .4byte 0x000007FF
_080455D4: .4byte gCardStats
_080455D8: .4byte gCardIdToNumber
_080455DC:
	cmp r1, #0x42
	beq _08045604
	b _08045606
_080455E2:
	ldr r0, _080455F4 @ =0x00000187
	cmp r1, r0
	beq _08045604
	cmp r1, r0
	bgt _080455F8
	sub r0, #0x12
	cmp r1, r0
	beq _08045604
	b _08045606
_080455F4: .4byte 0x00000187
_080455F8:
	ldr r0, _08045664 @ =0x000002E5
	cmp r1, r0
	beq _08045604
	add r0, #0x68
	cmp r1, r0
	bne _08045606
_08045604:
	mov r2, #0
_08045606:
	cmp r2, #0
	beq _08045648
	mov r3, #0xC3
	lsl r3, r3, #2
	add r3, sl
	ldrh r5, [r3]
	lsl r2, r5, #2
	mov r0, sl
	add r0, #0xC
	add r2, r2, r0
	mov r0, #1
	ldr r7, [sp, #8]
	and r0, r7
	ldr r1, _08045668 @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldr r5, _0804566C @ =0x020192E4
	ldr r6, _08045670 @ =0x000007C4
	add r1, r5, r6
	add r0, r0, r1
	ldr r0, [r0]
	str r0, [r2]
	ldrh r7, [r3]
	lsl r1, r7, #1
	mov r0, #0x83
	lsl r0, r0, #2
	add r0, sl
	add r1, r1, r0
	mov r0, #2
	strh r0, [r1]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
_08045648:
	mov r0, #1
	add r8, r0
	ldr r1, [sp, #8]
	and r0, r1
	ldr r3, _08045668 @ =0x00000D64
	mul r0, r3
	ldr r2, _0804566C @ =0x020192E4
	add r0, r0, r2
	ldrb r0, [r0, #3]
	cmp r8, r0
	bge _08045660
	b _080452E4
_08045660:
	bl _08046586 @ far jump
_08045664: .4byte 0x000002E5
_08045668: .4byte 0x00000D64
_0804566C: .4byte 0x020192E4
_08045670: .4byte 0x000007C4
_08045674:
	mov r3, #0
	mov r8, r3
	ldr r0, _080456D0 @ =0x020192E4
	mov r1, #1
	ldr r4, [sp, #8]
	and r1, r4
	ldr r2, _080456D4 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r5, [r2, #3]
	cmp r8, r5
	blt _08045690
	bl _08046586 @ far jump
_08045690:
	ldr r7, _080456D8 @ =0x000007FF
	mov r6, sl
	mov r5, ip
	add r4, r1, #0
	mov ip, r0
	mov r9, r2
_0804569C:
	ldr r0, _080456DC @ =0x02019AA8
	add r0, r4, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	add r2, r3, #0
	and r2, r7
	lsl r0, r2, #2
	ldr r1, _080456E0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08045776
	lsl r0, r2, #1
	ldr r2, _080456E4 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _080456E8 @ =0x00000776
	cmp r1, r0
	bne _080456EC
	mov r0, #3
	b _08045748
_080456D0: .4byte 0x020192E4
_080456D4: .4byte 0x00000D64
_080456D8: .4byte 0x000007FF
_080456DC: .4byte 0x02019AA8
_080456E0: .4byte gCardStats
_080456E4: .4byte gCardIdToNumber
_080456E8: .4byte 0x00000776
_080456EC:
	cmp r1, r0
	blt _080456FC
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080456FC
	mov r0, #1
	b _08045748
_080456FC:
	add r0, r3, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08045720 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0804572A
	cmp r0, #0x16
	bgt _08045724
	cmp r0, #0x15
	beq _0804572E
	b _08045736
	.align 2, 0
_08045720: .4byte gCardStats
_08045724:
	cmp r0, #0x17
	beq _08045732
	b _08045736
_0804572A:
	mov r0, #7
	b _08045748
_0804572E:
	mov r0, #8
	b _08045748
_08045732:
	mov r0, #9
	b _08045748
_08045736:
	and r3, r7
	lsl r0, r3, #2
	ldr r2, _08045788 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08045748:
	cmp r0, #3
	bne _08045776
	ldrh r3, [r5]
	lsl r1, r3, #2
	add r0, r6, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, _0804578C @ =0x000007C4
	add r0, ip
	add r0, r4, r0
	ldr r0, [r0]
	str r0, [r1]
	ldrh r0, [r5]
	lsl r1, r0, #1
	mov r2, #0x83
	lsl r2, r2, #2
	add r0, r6, r2
	add r1, r1, r0
	mov r0, #2
	strh r0, [r1]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08045776:
	add r4, #4
	mov r3, #1
	add r8, r3
	mov r0, r9
	ldrb r0, [r0, #3]
	cmp r8, r0
	blt _0804569C
	bl _08046586 @ far jump
_08045788: .4byte gCardStats
_0804578C: .4byte 0x000007C4
_08045790:
	mov r1, #0
	mov r8, r1
	ldr r3, _08045804 @ =0x020192E4
	mov r0, #1
	ldr r2, [sp, #8]
	and r0, r2
	ldr r1, _08045808 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #3]
	cmp r8, r5
	blt _080457AE
	bl _08046586 @ far jump
_080457AE:
	ldr r6, _0804580C @ =0x000007C4
	add r0, r3, r6
	ldr r6, _08045810 @ =0x0201D810
	mov r7, #0xC3
	lsl r7, r7, #2
	add r5, r6, r7
	add r7, r1, #0
	add r3, r2, r0
	mov r0, #0x83
	lsl r0, r0, #2
	add r0, r0, r6
	mov r9, r0
_080457C6:
	ldr r2, [r3]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08045814 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r4
	bne _080457F2
	ldrh r1, [r5]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r5]
	lsl r0, r2, #1
	add r0, r9
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_080457F2:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r7, #3]
	cmp r8, r1
	blt _080457C6
	bl _08046586 @ far jump
	.align 2, 0
_08045804: .4byte 0x020192E4
_08045808: .4byte 0x00000D64
_0804580C: .4byte 0x000007C4
_08045810: .4byte 0x0201D810
_08045814: .4byte gCardIdToNumber
_08045818:
	mov r2, #0
	mov r8, r2
	ldr r0, _08045884 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08045888 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r4, [r2, #3]
	cmp r8, r4
	blt _08045834
	bl _08046586 @ far jump
_08045834:
	ldr r6, _0804588C @ =0x000007FF
	mov r5, #0xF8
	lsl r5, r5, #0x11
	mov r4, sl
	mov r3, ip
	add r7, r0, #0
	ldr r0, _08045890 @ =0x000007C4
	add r7, r7, r0
	mov r9, r2
	add r2, r1, r7
_08045848:
	ldr r0, [r2]
	lsl r1, r0, #0x14
	lsr r0, r1, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _08045894 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _080458C2
	lsr r0, r1, #0x14
	and r0, r6
	lsl r0, r0, #2
	add r1, r7, #0
	add r0, r0, r1
	ldr r1, [r0]
	add r0, r1, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08045898
	cmp r0, #0x15
	blt _08045898
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0804589A
_08045884: .4byte 0x020192E4
_08045888: .4byte 0x00000D64
_0804588C: .4byte 0x000007FF
_08045890: .4byte 0x000007C4
_08045894: .4byte gCardStats
_08045898:
	mov r0, #0
_0804589A:
	cmp r0, #6
	bne _080458C2
	ldrh r7, [r3]
	lsl r1, r7, #2
	add r0, r4, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r2]
	str r0, [r1]
	ldrh r0, [r3]
	lsl r1, r0, #1
	mov r7, #0x83
	lsl r7, r7, #2
	add r0, r4, r7
	add r1, r1, r0
	mov r0, #2
	strh r0, [r1]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
_080458C2:
	add r2, #4
	mov r0, #1
	add r8, r0
	mov r1, r9
	ldrb r1, [r1, #3]
	cmp r8, r1
	blt _08045848
	bl _08046586 @ far jump
_080458D4:
	mov r2, #0
	mov r8, r2
	ldr r1, _08045974 @ =0x020192E4
	mov r2, #1
	ldr r3, [sp, #8]
	and r2, r3
	ldr r3, _08045978 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _080458F2
	bl _08046586 @ far jump
_080458F2:
	add r7, r2, #0
	ldr r6, _0804597C @ =0x0201D810
	mov r4, #0x83
	lsl r4, r4, #2
	add r4, r4, r6
	mov r9, r4
	mov r0, #0xC3
	lsl r0, r0, #2
	add r5, r6, r0
_08045904:
	mov r2, r8
	lsl r1, r2, #2
	add r0, r7, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08045980 @ =0x02019BE8
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r3, _08045984 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08045956
	ldr r0, [sp, #8]
	mov r1, r8
	bl CanReviveGraveyardCard
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08045956
	ldrh r0, [r5]
	lsl r1, r0, #2
	add r0, r6, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r4]
	str r0, [r1]
	ldrh r1, [r5]
	lsl r0, r1, #1
	add r0, r9
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08045956:
	mov r2, #1
	add r8, r2
	ldr r1, _08045974 @ =0x020192E4
	ldr r3, _08045978 @ =0x00000D64
	add r0, r7, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08045904
	ldr r3, _0804597C @ =0x0201D810
	mov sl, r3
	bl _08046586 @ far jump
	.align 2, 0
_08045974: .4byte 0x020192E4
_08045978: .4byte 0x00000D64
_0804597C: .4byte 0x0201D810
_08045980: .4byte 0x02019BE8
_08045984: .4byte gCardStats
_08045988:
	mov r4, #0
	mov r8, r4
	ldr r1, _08045AC4 @ =0x020192E4
	mov r2, #1
	ldr r5, [sp, #8]
	and r2, r5
	ldr r3, _08045AC8 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #4]
	cmp r8, r0
	bge _08045A16
	add r6, r2, #0
	mov r7, sl
	mov r0, #0x83
	lsl r0, r0, #2
	add r0, r0, r7
	mov r9, r0
	mov r5, ip
_080459B0:
	mov r2, r8
	lsl r1, r2, #2
	add r0, r6, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08045ACC @ =0x02019BE8
	add r4, r1, r0
	ldr r0, [r4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r3, _08045AD0 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08045A02
	ldr r0, [sp, #8]
	mov r1, r8
	bl CanReviveGraveyardCard
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08045A02
	ldrh r0, [r5]
	lsl r1, r0, #2
	add r0, r7, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r4]
	str r0, [r1]
	ldrh r1, [r5]
	lsl r0, r1, #1
	add r0, r9
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08045A02:
	mov r2, #1
	add r8, r2
	ldr r1, _08045AC4 @ =0x020192E4
	ldr r3, _08045AC8 @ =0x00000D64
	add r0, r6, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _080459B0
_08045A16:
	ldr r3, _08045AD4 @ =0x0201D810
	mov sl, r3
	ldr r4, [sp, #8]
	ldr r5, [sp, #0xC]
	cmp r4, r5
	beq _08045A26
	bl _08046586 @ far jump
_08045A26:
	mov r6, #1
	str r6, [sp, #0x18]
	mov r7, #0
	mov r8, r7
	mov r0, #0xC3
	lsl r0, r0, #2
	add r0, sl
	ldrh r1, [r0]
	cmp r8, r1
	blt _08045A3E
	bl _08046586 @ far jump
_08045A3E:
	mov r2, #0xC3
	lsl r2, r2, #2
	add r2, sl
_08045A44:
	mov r3, r8
	lsl r1, r3, #2
	ldr r4, _08045AD8 @ =0x0201D81C
	add r0, r1, r4
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r5, _08045ADC @ =0x08622AB4
	add r0, r0, r5
	mov r6, #1
	add r6, r8
	mov r9, r6
	ldrh r0, [r0]
	ldr r7, _08045AE0 @ =0x0000045C
	cmp r0, r7
	bne _08045AAC
	mov r0, #0
	str r0, [sp, #0x18]
	ldrh r0, [r2]
	sub r0, #1
	strh r0, [r2]
	mov r6, r8
	cmp r8, r0
	bge _08045AAC
	add r7, r4, #0
	mov r4, #0xC0
	lsl r4, r4, #2
	add r3, r7, r4
	lsl r0, r6, #1
	mov r5, #0x80
	lsl r5, r5, #2
	add r0, r0, r5
	add r4, r0, r7
	add r5, r1, r7
_08045A88:
	lsl r1, r6, #2
	add r0, r7, #4
	add r1, r1, r0
	add r0, r5, #0
	str r2, [sp, #0x20]
	str r3, [sp, #0x24]
	bl CopyDuelCard
	ldrh r0, [r4, #2]
	strh r0, [r4]
	add r4, #2
	add r5, #4
	add r6, #1
	ldr r3, [sp, #0x24]
	ldr r2, [sp, #0x20]
	ldrh r0, [r3]
	cmp r6, r0
	blt _08045A88
_08045AAC:
	mov r8, r9
	ldr r1, _08045AE4 @ =0x0201DB1C
	ldrh r1, [r1]
	cmp r8, r1
	blt _08045ABA
	bl _08046586 @ far jump
_08045ABA:
	ldr r3, [sp, #0x18]
	cmp r3, #0
	bne _08045A44
	bl _08046586 @ far jump
_08045AC4: .4byte 0x020192E4
_08045AC8: .4byte 0x00000D64
_08045ACC: .4byte 0x02019BE8
_08045AD0: .4byte gCardStats
_08045AD4: .4byte 0x0201D810
_08045AD8: .4byte 0x0201D81C
_08045ADC: .4byte gCardIdToNumber
_08045AE0: .4byte 0x0000045C
_08045AE4: .4byte 0x0201DB1C
_08045AE8:
	mov r4, #0
	mov r8, r4
	ldr r1, _08045B4C @ =0x020192E4
	mov r2, #1
	ldr r5, [sp, #8]
	and r2, r5
	ldr r3, _08045B50 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08045B06
	bl _08046586 @ far jump
_08045B06:
	add r5, r2, #0
	mov r7, sl
	mov sl, r1
	mov r6, #0
	ldr r0, _08045B54 @ =0x000007FF
	mov r9, r0
	mov r4, ip
_08045B14:
	add r0, r5, #0
	mul r0, r3
	add r0, r6, r0
	ldr r1, _08045B58 @ =0x02019BE8
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08045B5C @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08045BBE
	cmp r0, #0x15
	blt _08045B6A
	cmp r0, #0x17
	ble _08045B60
	cmp r0, #0x18
	beq _08045B64
	b _08045B6A
_08045B4C: .4byte 0x020192E4
_08045B50: .4byte 0x00000D64
_08045B54: .4byte 0x000007FF
_08045B58: .4byte 0x02019BE8
_08045B5C: .4byte gCardStats
_08045B60:
	mov r1, #0
	b _08045B82
_08045B64:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _08045B82
_08045B6A:
	add r0, r2, #0
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _08045BD8 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_08045B82:
	ldr r0, _08045BDC @ =0x000005DC
	cmp r1, r0
	bhi _08045BBE
	add r0, r2, #0
	bl IsEffectMonster
	cmp r0, #0
	bne _08045BBE
	ldrh r0, [r4]
	lsl r2, r0, #2
	add r0, r7, #0
	add r0, #0xC
	add r2, r2, r0
	ldr r0, _08045BE0 @ =0x00000D64
	mul r0, r5
	add r0, r6, r0
	ldr r1, _08045BE4 @ =0x00000904
	add r1, sl
	add r0, r0, r1
	ldr r0, [r0]
	str r0, [r2]
	ldrh r1, [r4]
	lsl r0, r1, #1
	ldr r2, _08045BE8 @ =0x0201DA1C
	add r0, r0, r2
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045BBE:
	add r6, #4
	mov r3, #1
	add r8, r3
	ldr r3, _08045BE0 @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, sl
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08045B14
	bl _080464EA @ far jump
	.align 2, 0
_08045BD8: .4byte gCardStats
_08045BDC: .4byte 0x000005DC
_08045BE0: .4byte 0x00000D64
_08045BE4: .4byte 0x00000904
_08045BE8: .4byte 0x0201DA1C
_08045BEC:
	mov r5, #0
	mov r8, r5
	ldr r3, _08045C6C @ =0x020192E4
	mov r0, #1
	ldr r6, [sp, #8]
	and r0, r6
	ldr r1, _08045C70 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r7, [r1, #3]
	cmp r8, r7
	blt _08045C0A
	bl _08046586 @ far jump
_08045C0A:
	ldr r4, _08045C74 @ =0x000007C4
	add r0, r3, r4
	mov r6, sl
	mov r4, ip
	add r5, r1, #0
	add r3, r2, r0
	ldr r7, _08045C78 @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_08045C20:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08045C7C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08045C5A
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045C5A:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #3]
	cmp r8, r1
	blt _08045C20
	bl _08046586 @ far jump
	.align 2, 0
_08045C6C: .4byte 0x020192E4
_08045C70: .4byte 0x00000D64
_08045C74: .4byte 0x000007C4
_08045C78: .4byte 0x000007FF
_08045C7C: .4byte gCardStats
_08045C80:
	mov r2, #0
	mov r8, r2
	ldr r0, _08045CCC @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08045CD0 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r4, [r2, #3]
	cmp r8, r4
	blt _08045C9C
	bl _08046586 @ far jump
_08045C9C:
	mov r6, sl
	mov r3, ip
	add r5, r0, #0
	ldr r7, _08045CD4 @ =0x000007C4
	add r0, r5, r7
	ldr r4, _08045CD8 @ =0x000004BA
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
	add r5, r2, #0
	add r2, r1, r0
_08045CB2:
	ldr r0, [r2]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08045CDC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, r4
	beq _08045CE6
	cmp r1, r4
	bgt _08045CE0
	cmp r1, #0x22
	beq _08045CE6
	b _08045D04
_08045CCC: .4byte 0x020192E4
_08045CD0: .4byte 0x00000D64
_08045CD4: .4byte 0x000007C4
_08045CD8: .4byte 0x000004BA
_08045CDC: .4byte gCardIdToNumber
_08045CE0:
	ldr r0, _08045D14 @ =0x000007F2
	cmp r1, r0
	bne _08045D04
_08045CE6:
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r0, r6, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r2]
	str r0, [r1]
	ldrh r1, [r3]
	lsl r0, r1, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
_08045D04:
	add r2, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #3]
	cmp r8, r1
	blt _08045CB2
	bl _08046586 @ far jump
_08045D14: .4byte 0x000007F2
_08045D18:
	mov r2, #0
	mov r8, r2
	ldr r3, _08045D88 @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #8]
	and r0, r4
	ldr r1, _08045D8C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #3]
	cmp r8, r5
	blt _08045D36
	bl _08046586 @ far jump
_08045D36:
	ldr r6, _08045D90 @ =0x000007C4
	add r0, r3, r6
	ldr r7, _08045D94 @ =0x000002EA
	mov r9, r7
	mov r4, ip
	add r6, r1, #0
	add r3, r2, r0
	mov r5, sl
	sub r7, #0xDE
	add r7, sl
_08045D4A:
	ldr r2, [r3]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08045D98 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r9
	bne _08045D76
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045D76:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #3]
	cmp r8, r1
	blt _08045D4A
	bl _08046586 @ far jump
	.align 2, 0
_08045D88: .4byte 0x020192E4
_08045D8C: .4byte 0x00000D64
_08045D90: .4byte 0x000007C4
_08045D94: .4byte 0x000002EA
_08045D98: .4byte gCardIdToNumber
_08045D9C:
	mov r2, #0
	mov r8, r2
	ldr r0, _08045E0C @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08045E10 @ =0x00000D64
	mul r1, r2
	add r0, r1, r0
	ldrb r4, [r0, #4]
	cmp r8, r4
	blt _08045DB6
	b _08046586
_08045DB6:
	mov r6, sl
	mov r4, ip
	add r3, r1, #0
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
	add r5, r0, #0
_08045DC4:
	ldr r0, _08045E14 @ =0x02019BE8
	add r0, r3, r0
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08045E18 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08045E1C @ =0x000002EA
	cmp r1, r0
	beq _08045DE2
	mov r0, #0x9B
	lsl r0, r0, #3
	cmp r1, r0
	bne _08045DFE
_08045DE2:
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045DFE:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _08045DC4
	b _08046586
_08045E0C: .4byte 0x020192E4
_08045E10: .4byte 0x00000D64
_08045E14: .4byte 0x02019BE8
_08045E18: .4byte gCardIdToNumber
_08045E1C: .4byte 0x000002EA
_08045E20:
	mov r2, #0
	mov r8, r2
	ldr r3, _08045E94 @ =0x020192E4
	mov r1, #1
	ldr r4, [sp, #8]
	and r1, r4
	ldr r2, _08045E98 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	add r0, r0, r3
	ldrb r0, [r0, #3]
	cmp r8, r0
	blt _08045E3C
	b _08046586
_08045E3C:
	add r5, r1, #0
	mov r6, #0
	mov r7, #0xF8
	lsl r7, r7, #0x11
	mov r9, r7
	mov r7, sl
	mov r0, #0x83
	lsl r0, r0, #2
	add r0, r0, r7
	mov sl, r0
_08045E50:
	add r0, r5, #0
	mul r0, r2
	add r0, r6, r0
	ldr r1, _08045E9C @ =0x02019AA8
	add r0, r0, r1
	ldr r0, [r0]
	lsl r1, r0, #0x14
	lsr r0, r1, #0x14
	ldr r2, _08045EA0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _08045EA4 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r4, r9
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0xA
	bne _08045F06
	lsr r1, r1, #0x14
	add r0, r1, #0
	and r0, r2
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08045EB0
	cmp r0, #0x17
	ble _08045EA8
	cmp r0, #0x18
	beq _08045EAC
	b _08045EB0
_08045E94: .4byte 0x020192E4
_08045E98: .4byte 0x00000D64
_08045E9C: .4byte 0x02019AA8
_08045EA0: .4byte 0x000007FF
_08045EA4: .4byte gCardStats
_08045EA8:
	mov r0, #0
	b _08045EC4
_08045EAC:
	mov r0, #0xA
	b _08045EC4
_08045EB0:
	ldr r0, _08045F24 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #2
	ldr r1, _08045F28 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08045EC4:
	ldr r2, [sp, #0xC]
	cmp r0, r2
	bne _08045F06
	ldr r0, _08045F2C @ =0x00000D64
	mul r0, r5
	add r0, r6, r0
	ldr r3, _08045F30 @ =0x02019AA8
	add r4, r0, r3
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08045F06
	mov r0, #0xC3
	lsl r0, r0, #2
	add r2, r7, r0
	ldrh r3, [r2]
	lsl r1, r3, #2
	add r0, r7, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r4]
	str r0, [r1]
	ldrh r4, [r2]
	lsl r0, r4, #1
	add r0, sl
	mov r1, #2
	strh r1, [r0]
	ldrh r0, [r2]
	add r0, #1
	strh r0, [r2]
_08045F06:
	add r6, #4
	mov r0, #1
	add r8, r0
	ldr r1, _08045F34 @ =0x020192E4
	ldr r2, _08045F2C @ =0x00000D64
	add r0, r5, #0
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r8, r0
	blt _08045E50
_08045F1C:
	ldr r1, _08045F38 @ =0x0201D810
	mov sl, r1
	b _08046586
	.align 2, 0
_08045F24: .4byte 0x000007FF
_08045F28: .4byte gCardStats
_08045F2C: .4byte 0x00000D64
_08045F30: .4byte 0x02019AA8
_08045F34: .4byte 0x020192E4
_08045F38: .4byte 0x0201D810
_08045F3C:
	mov r2, #0
	mov r8, r2
	ldr r0, _08045FC8 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r2, _08045FCC @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r4, [r2, #4]
	cmp r8, r4
	blt _08045F56
	b _08046586
_08045F56:
	add r5, r1, #0
	add r7, r0, #0
	ldr r0, _08045FD0 @ =0x00000904
	add r6, r7, r0
	mov r9, sl
	mov r4, ip
	add r3, r5, r6
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, sl
	mov ip, r1
	add r7, r2, #0
_08045F6E:
	add r0, r5, r6
	mov r2, r8
	lsl r1, r2, #2
	add r0, r0, r1
	ldr r2, [r0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	ldr r1, _08045FD4 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08045FD8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08045FB8
	lsl r0, r2, #0xA
	cmp r0, #0
	bge _08045FB8
	ldrh r2, [r4]
	lsl r1, r2, #2
	mov r0, r9
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r3]
	str r0, [r1]
	ldrh r1, [r4]
	lsl r0, r1, #1
	add r0, ip
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08045FB8:
	add r3, #4
	mov r2, #1
	add r8, r2
	ldrb r0, [r7, #4]
	cmp r8, r0
	blt _08045F6E
	b _08046586
	.align 2, 0
_08045FC8: .4byte 0x020192E4
_08045FCC: .4byte 0x00000D64
_08045FD0: .4byte 0x00000904
_08045FD4: .4byte 0x000007FF
_08045FD8: .4byte gCardStats
_08045FDC:
	mov r1, #0
	mov r8, r1
	ldr r0, _0804606C @ =0x020192E4
	mov r1, #1
	ldr r2, [sp, #8]
	and r1, r2
	ldr r2, _08046070 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r3, [r2, #4]
	cmp r8, r3
	blt _08045FF6
	b _08046586
_08045FF6:
	add r6, r1, #0
	add r4, r0, #0
	ldr r5, _08046074 @ =0x00000904
	add r7, r4, r5
	ldr r5, _08046078 @ =0x0201D810
	mov r0, #0xC3
	lsl r0, r0, #2
	add r4, r5, r0
	add r3, r6, r7
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, r1, r5
	mov ip, r1
	mov r9, r2
_08046012:
	add r0, r6, r7
	mov r2, r8
	lsl r1, r2, #2
	add r0, r0, r1
	ldr r2, [r0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	ldr r1, _0804607C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08046080 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0804605C
	lsl r0, r2, #9
	cmp r0, #0
	bge _0804605C
	ldrh r2, [r4]
	lsl r1, r2, #2
	add r0, r5, #0
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r3]
	str r0, [r1]
	ldrh r1, [r4]
	lsl r0, r1, #1
	add r0, ip
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_0804605C:
	add r3, #4
	mov r2, #1
	add r8, r2
	mov r0, r9
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08046012
	b _08046586
_0804606C: .4byte 0x020192E4
_08046070: .4byte 0x00000D64
_08046074: .4byte 0x00000904
_08046078: .4byte 0x0201D810
_0804607C: .4byte 0x000007FF
_08046080: .4byte gCardStats
_08046084:
	mov r1, #0
	mov r8, r1
	ldr r3, _08046100 @ =0x020192E4
	mov r0, #1
	ldr r2, [sp, #8]
	sub r1, r0, r2
	and r1, r0
	ldr r0, _08046104 @ =0x00000D64
	mul r1, r0
	add r2, r1, r3
	ldrb r4, [r2, #4]
	cmp r8, r4
	blt _080460A0
	b _08046586
_080460A0:
	ldr r5, _08046108 @ =0x00000904
	add r0, r3, r5
	mov r6, sl
	mov r4, ip
	add r5, r2, #0
	add r3, r1, r0
	ldr r7, _0804610C @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_080460B6:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08046110 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _080460F0
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_080460F0:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _080460B6
	b _08046586
	.align 2, 0
_08046100: .4byte 0x020192E4
_08046104: .4byte 0x00000D64
_08046108: .4byte 0x00000904
_0804610C: .4byte 0x000007FF
_08046110: .4byte gCardStats
_08046114:
	mov r2, #1
	str r2, [sp, #0x14]
	mov r3, #0
	mov r8, r3
	ldr r3, _08046190 @ =0x020192E4
	ldr r0, [sp, #8]
	and r0, r2
	ldr r1, _08046194 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r4, [r1, #4]
	cmp r8, r4
	blt _08046132
	b _08046586
_08046132:
	ldr r5, _08046198 @ =0x00000904
	add r0, r3, r5
	mov r6, sl
	mov r4, ip
	add r5, r1, #0
	add r3, r2, r0
	ldr r7, _0804619C @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_08046148:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080461A0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08046182
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08046182:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _08046148
	b _08046586
_08046190: .4byte 0x020192E4
_08046194: .4byte 0x00000D64
_08046198: .4byte 0x00000904
_0804619C: .4byte 0x000007FF
_080461A0: .4byte gCardStats
_080461A4:
	mov r2, #0
	mov r8, r2
	ldr r3, _08046224 @ =0x020192E4
	mov r0, #1
	ldr r4, [sp, #8]
	and r0, r4
	ldr r1, _08046228 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r5, [r1, #4]
	cmp r8, r5
	blt _080461C0
	b _08046586
_080461C0:
	ldr r6, _0804622C @ =0x00000904
	add r0, r3, r6
	ldr r5, _08046230 @ =0x0201D810
	mov r7, #0xC3
	lsl r7, r7, #2
	add r4, r5, r7
	add r6, r1, #0
	add r3, r2, r0
	ldr r0, _08046234 @ =0x000007FF
	mov r9, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r7, r5, r1
_080461DA:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08046238 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #3
	bne _08046214
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r5, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08046214:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #4]
	cmp r8, r1
	blt _080461DA
	b _08046586
	.align 2, 0
_08046224: .4byte 0x020192E4
_08046228: .4byte 0x00000D64
_0804622C: .4byte 0x00000904
_08046230: .4byte 0x0201D810
_08046234: .4byte 0x000007FF
_08046238: .4byte gCardStats
_0804623C:
	mov r2, #0
	mov r8, r2
	ldr r0, _08046284 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r3, _08046288 @ =0x00000D64
	mul r1, r3
	add r1, r1, r0
	ldrb r1, [r1, #4]
	cmp r8, r1
	blt _08046256
	b _08046586
_08046256:
	mov r0, #1
	ldr r5, [sp, #8]
	and r0, r5
	mov r6, r8
	lsl r2, r6, #2
	mul r0, r3
	add r0, r2, r0
	ldr r1, _0804628C @ =0x02019BE8
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	mov r5, #0
	ldr r7, _08046290 @ =0xFFFFFA15
	add r0, r4, r7
	add r6, r2, #0
	cmp r0, #4
	bhi _080462BE
	lsl r0, r0, #2
	ldr r1, _08046294 @ =0x08046298
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08046284: .4byte 0x020192E4
_08046288: .4byte 0x00000D64
_0804628C: .4byte 0x02019BE8
_08046290: .4byte 0xFFFFFA15
_08046294: .4byte 0x08046298
_08046298:
	.4byte _080462AC
	.4byte _080462B0
	.4byte _080462B4
	.4byte _080462B8
	.4byte _080462BC
_080462AC:
	mov r5, #1
	b _080462BE
_080462B0:
	mov r5, #4
	b _080462BE
_080462B4:
	mov r5, #3
	b _080462BE
_080462B8:
	mov r5, #5
	b _080462BE
_080462BC:
	mov r5, #6
_080462BE:
	ldr r0, _08046334 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _08046338 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0804631A
	lsr r0, r1, #0x1D
	cmp r0, r5
	bne _0804631A
	mov r3, #0xC3
	lsl r3, r3, #2
	add r3, sl
	ldrh r5, [r3]
	lsl r2, r5, #2
	mov r0, sl
	add r0, #0xC
	add r2, r2, r0
	mov r0, #1
	ldr r7, [sp, #8]
	and r0, r7
	ldr r1, _0804633C @ =0x00000D64
	mul r0, r1
	add r0, r6, r0
	ldr r5, _08046340 @ =0x020192E4
	ldr r6, _08046344 @ =0x00000904
	add r1, r5, r6
	add r0, r0, r1
	ldr r0, [r0]
	str r0, [r2]
	ldrh r7, [r3]
	lsl r1, r7, #1
	mov r0, #0x83
	lsl r0, r0, #2
	add r0, sl
	add r1, r1, r0
	mov r0, #4
	strh r0, [r1]
	ldrh r0, [r3]
	add r0, #1
	strh r0, [r3]
_0804631A:
	mov r0, #1
	add r8, r0
	ldr r1, [sp, #8]
	and r0, r1
	ldr r3, _0804633C @ =0x00000D64
	mul r0, r3
	ldr r2, _08046340 @ =0x020192E4
	add r0, r0, r2
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08046256
	b _08046586
	.align 2, 0
_08046334: .4byte 0x000007FF
_08046338: .4byte gCardStats
_0804633C: .4byte 0x00000D64
_08046340: .4byte 0x020192E4
_08046344: .4byte 0x00000904
_08046348:
	mov r3, #0
	mov r8, r3
	ldr r0, _080463BC @ =0x020192E4
	mov r1, #1
	ldr r4, [sp, #8]
	and r1, r4
	ldr r2, _080463C0 @ =0x00000D64
	mul r1, r2
	add r2, r1, r0
	ldrb r5, [r2, #4]
	cmp r8, r5
	blt _08046362
	b _08046586
_08046362:
	mov r6, sl
	mov r4, ip
	add r5, r2, #0
	add r3, r5, #0
	add r2, r1, #0
	add r7, r0, #0
	ldr r0, _080463C4 @ =0x00000904
	add r7, r7, r0
	mov r9, r7
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, sl
	mov ip, r1
_0804637C:
	ldr r7, _080463C8 @ =0x00000906
	add r0, r3, r7
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _080463AA
	ldrh r0, [r4]
	lsl r1, r0, #2
	add r0, r6, #0
	add r0, #0xC
	add r1, r1, r0
	mov r7, r9
	add r0, r2, r7
	ldr r0, [r0]
	str r0, [r1]
	ldrh r1, [r4]
	lsl r0, r1, #1
	add r0, ip
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_080463AA:
	add r3, #4
	add r2, #4
	mov r7, #1
	add r8, r7
	ldrb r0, [r5, #4]
	cmp r8, r0
	blt _0804637C
	b _08046586
	.align 2, 0
_080463BC: .4byte 0x020192E4
_080463C0: .4byte 0x00000D64
_080463C4: .4byte 0x00000904
_080463C8: .4byte 0x00000906
_080463CC:
	mov r1, #0
	mov r8, r1
	ldr r3, _08046448 @ =0x020192E4
	mov r0, #1
	ldr r2, [sp, #8]
	and r0, r2
	ldr r1, _0804644C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r1, r2, r3
	ldrb r4, [r1, #4]
	cmp r8, r4
	blt _080463E8
	b _08046586
_080463E8:
	ldr r5, _08046450 @ =0x00000904
	add r0, r3, r5
	mov r6, sl
	mov r4, ip
	add r5, r1, #0
	add r3, r2, r0
	ldr r7, _08046454 @ =0x000007FF
	mov r9, r7
	mov r7, #0x83
	lsl r7, r7, #2
	add r7, sl
_080463FE:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08046458 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08046438
	ldrh r1, [r4]
	lsl r0, r1, #2
	add r1, r6, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r7
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
_08046438:
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r5, #4]
	cmp r8, r1
	blt _080463FE
	b _08046586
	.align 2, 0
_08046448: .4byte 0x020192E4
_0804644C: .4byte 0x00000D64
_08046450: .4byte 0x00000904
_08046454: .4byte 0x000007FF
_08046458: .4byte gCardStats
_0804645C:
	mov r2, #0
	mov r8, r2
	ldr r2, _080464F0 @ =0x020192E4
	mov r1, #1
	ldr r3, [sp, #8]
	and r1, r3
	ldr r3, _080464F4 @ =0x00000D64
	add r0, r1, #0
	mul r0, r3
	add r0, r0, r2
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08046478
	b _08046586
_08046478:
	add r7, r1, #0
	mov r9, sl
	mov r6, ip
	mov r4, #0x83
	lsl r4, r4, #2
	add r4, r9
	mov sl, r4
_08046486:
	mov r5, r8
	lsl r1, r5, #2
	add r0, r7, #0
	mul r0, r3
	add r4, r1, r0
	ldr r0, _080464F8 @ =0x02019BE8
	add r5, r4, r0
	ldr r1, [r5]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r3, [sp, #0xC]
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	str r2, [sp, #0x20]
	bl IsMaterialOfFusion
	ldr r2, [sp, #0x20]
	cmp r0, #0
	beq _080464D8
	add r0, r4, r2
	ldr r4, _080464FC @ =0x00000906
	add r0, r0, r4
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _080464D8
	ldrh r0, [r6]
	lsl r1, r0, #2
	mov r0, r9
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r5]
	str r0, [r1]
	ldrh r1, [r6]
	lsl r0, r1, #1
	add r0, sl
	mov r1, #4
	strh r1, [r0]
	ldrh r0, [r6]
	add r0, #1
	strh r0, [r6]
_080464D8:
	mov r3, #1
	add r8, r3
	ldr r3, _080464F4 @ =0x00000D64
	add r0, r7, #0
	mul r0, r3
	add r0, r0, r2
	ldrb r0, [r0, #4]
	cmp r8, r0
	blt _08046486
_080464EA:
	ldr r4, _08046500 @ =0x0201D810
	mov sl, r4
	b _08046586
_080464F0: .4byte 0x020192E4
_080464F4: .4byte 0x00000D64
_080464F8: .4byte 0x02019BE8
_080464FC: .4byte 0x00000906
_08046500: .4byte 0x0201D810
_08046504:
	mov r5, #0
	mov r8, r5
	ldr r0, _08046598 @ =0x020192E4
	mov r1, #1
	ldr r6, [sp, #8]
	and r1, r6
	ldr r2, _0804659C @ =0x00000D64
	mul r2, r1
	add r6, r2, r0
	ldrb r7, [r6, #6]
	cmp r8, r7
	bge _08046586
	ldr r3, _080465A0 @ =0x00000B84
	add r1, r0, r3
	mov r7, sl
	mov r5, ip
	ldr r4, _080465A4 @ =0x00000CC4
	add r0, r0, r4
	add r4, r2, r0
	add r3, r2, r1
	ldr r0, _080465A8 @ =0x000007FF
	mov ip, r0
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, sl
	mov r9, r1
_08046538:
	ldr r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	mov r1, ip
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080465AC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08046578
	ldrb r0, [r4]
	cmp r0, #2
	beq _08046578
	ldrh r1, [r5]
	lsl r0, r1, #2
	add r1, r7, #0
	add r1, #0xC
	add r0, r0, r1
	str r2, [r0]
	ldrh r2, [r5]
	lsl r0, r2, #1
	add r0, r9
	mov r1, #0x10
	strh r1, [r0]
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08046578:
	add r4, #2
	add r3, #4
	mov r0, #1
	add r8, r0
	ldrb r1, [r6, #6]
	cmp r8, r1
	blt _08046538
_08046586:
	mov r5, #0xC3
	lsl r5, r5, #2
	add r5, sl
	ldrh r0, [r5]
	cmp r0, #0
	bne _080465B0
	mov r0, #0
	b _08046722
	.align 2, 0
_08046598: .4byte 0x020192E4
_0804659C: .4byte 0x00000D64
_080465A0: .4byte 0x00000B84
_080465A4: .4byte 0x00000CC4
_080465A8: .4byte 0x000007FF
_080465AC: .4byte gCardStats
_080465B0:
	ldr r0, [sp, #8]
	bl HasFaceUpToonWorld
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r2, [sp, #0x10]
	cmp r2, #0
	bne _080465C4
	cmp r0, #0
	bne _08046644
_080465C4:
	ldr r3, [sp, #0x14]
	cmp r3, #0
	bne _08046644
	mov r4, #0
	mov r8, r4
	ldrh r6, [r5]
	cmp r8, r6
	bge _08046644
	mov r7, #0xC3
	lsl r7, r7, #2
	add r7, sl
	str r7, [sp, #0x1C]
	mov r4, sl
	add r7, r5, #0
	mov r0, #0
	mov r9, r0
	add r5, r4, #0
	add r5, #0xC
_080465E8:
	ldr r0, [r5]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804662C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	beq _08046634
	mov r6, r8
	ldr r2, _08046630 @ =0x0201D810
	mov sl, r2
	ldr r3, [sp, #0x1C]
	ldrh r3, [r3]
	cmp r8, r3
	bge _08046622
	mov r2, #0xC3
	lsl r2, r2, #2
	add r2, sl
	mov r0, r9
	add r0, #0xC
	add r1, r0, r4
_08046616:
	ldr r0, [r1, #4]
	stmia r1!, {r0}
	add r6, #1
	ldrh r0, [r2]
	cmp r6, r0
	blt _08046616
_08046622:
	ldrh r0, [r7]
	sub r0, #1
	strh r0, [r7]
	b _0804663E
	.align 2, 0
_0804662C: .4byte gCardIdToNumber
_08046630: .4byte 0x0201D810
_08046634:
	mov r1, #4
	add r9, r1
	add r5, #4
	mov r2, #1
	add r8, r2
_0804663E:
	ldrh r3, [r7]
	cmp r8, r3
	blt _080465E8
_08046644:
	mov r4, #0
	mov r8, r4
	ldr r0, _08046694 @ =0x0201D810
	mov r5, #0xC3
	lsl r5, r5, #2
	add r1, r0, r5
	mov sl, r0
	ldrh r6, [r1]
	cmp r8, r6
	bge _080466A6
	add r7, r5, #0
	add r7, sl
	mov r9, r7
	add r3, r1, #0
	mov ip, r3
	mov r5, sl
	add r5, #0xC
	mov r7, sl
	mov r4, #0xC
_0804666A:
	ldr r0, [r5]
	lsl r0, r0, #0xE
	cmp r0, #0
	bge _08046698
	mov r6, r8
	mov r0, r9
	ldrh r0, [r0]
	cmp r8, r0
	bge _0804668C
	mov r2, ip
	add r1, r4, r7
_08046680:
	ldr r0, [r1, #4]
	stmia r1!, {r0}
	add r6, #1
	ldrh r0, [r2]
	cmp r6, r0
	blt _08046680
_0804668C:
	ldrh r0, [r3]
	sub r0, #1
	strh r0, [r3]
	b _080466A0
_08046694: .4byte 0x0201D810
_08046698:
	add r5, #4
	add r4, #4
	mov r1, #1
	add r8, r1
_080466A0:
	ldrh r2, [r3]
	cmp r8, r2
	blt _0804666A
_080466A6:
	mov r3, #0
	mov r8, r3
	mov r0, #0xC3
	lsl r0, r0, #2
	add r0, sl
	ldrh r4, [r0]
	cmp r8, r4
	bge _0804671A
	mov r5, #0xC3
	lsl r5, r5, #2
	add r5, sl
	mov r9, r5
	mov r4, sl
	add r7, r0, #0
	mov r3, #0xC
	add r5, r4, #0
	add r5, #0xC
_080466C8:
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	str r3, [sp, #0x24]
	bl IsCardProhibited
	ldr r3, [sp, #0x24]
	cmp r0, #0
	beq _08046708
	mov r6, r8
	ldr r0, _08046704 @ =0x0201D810
	mov sl, r0
	mov r1, r9
	ldrh r1, [r1]
	cmp r8, r1
	bge _080466FC
	mov r2, #0xC3
	lsl r2, r2, #2
	add r2, sl
	add r1, r3, r4
_080466F0:
	ldr r0, [r1, #4]
	stmia r1!, {r0}
	add r6, #1
	ldrh r0, [r2]
	cmp r6, r0
	blt _080466F0
_080466FC:
	ldrh r0, [r7]
	sub r0, #1
	strh r0, [r7]
	b _08046714
_08046704: .4byte 0x0201D810
_08046708:
	add r3, #4
	add r5, #4
	mov r1, #1
	add r8, r1
	ldr r2, _08046734 @ =0x0201D810
	mov sl, r2
_08046714:
	ldrh r6, [r7]
	cmp r8, r6
	blt _080466C8
_0804671A:
	mov r0, #0xC3
	lsl r0, r0, #2
	add r0, sl
	ldrh r0, [r0]
_08046722:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08046734: .4byte 0x0201D810
	thumb_func_end CollectEffectTargets

