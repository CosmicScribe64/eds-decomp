	thumb_func_start sub_08007590
sub_08007590: @ 0x08007590
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r3, r2, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r0, _080075D0 @ =0x00000267
	cmp r2, r0
	bne _080075A2
	b _08007712
_080075A2:
	cmp r2, r0
	bgt _08007654
	sub r0, #0x93
	cmp r2, r0
	bne _080075AE
	b _08007712
_080075AE:
	cmp r2, r0
	bgt _08007604
	cmp r2, #0xAA
	bne _080075B8
	b _08007712
_080075B8:
	cmp r2, #0xAA
	bgt _080075DC
	cmp r2, #0x53
	bgt _080075D4
	cmp r2, #0x52
	blt _080075C6
	b _08007712
_080075C6:
	cmp r2, #0x27
	bne _080075CC
	b _08007712
_080075CC:
	b _0800772A
	.align 2, 0
_080075D0: .4byte 0x00000267
_080075D4:
	cmp r2, #0x65
	bne _080075DA
	b _08007712
_080075DA:
	b _0800772A
_080075DC:
	ldr r0, _080075F0 @ =0x00000109
	cmp r2, r0
	bne _080075E4
	b _08007712
_080075E4:
	cmp r2, r0
	bgt _080075F4
	cmp r2, #0xDF
	bne _080075EE
	b _08007712
_080075EE:
	b _0800772A
_080075F0: .4byte 0x00000109
_080075F4:
	ldr r0, _08007600 @ =0x000001AB
	cmp r2, r0
	bne _080075FC
	b _08007712
_080075FC:
	add r0, #0x22
	b _08007700
_08007600: .4byte 0x000001AB
_08007604:
	ldr r0, _08007620 @ =0x00000231
	cmp r2, r0
	bne _0800760C
	b _08007712
_0800760C:
	cmp r2, r0
	bgt _0800762A
	sub r0, #0x3D
	cmp r2, r0
	bne _08007618
	b _08007712
_08007618:
	cmp r2, r0
	bgt _08007624
	sub r0, #0xB
	b _08007700
_08007620: .4byte 0x00000231
_08007624:
	mov r0, #0x87
	lsl r0, r0, #2
	b _080076E2
_0800762A:
	ldr r0, _08007640 @ =0x0000024E
	cmp r2, r0
	beq _08007712
	cmp r2, r0
	bgt _08007644
	sub r0, #8
	cmp r2, r0
	beq _08007712
	add r0, #3
	b _08007700
	.align 2, 0
_08007640: .4byte 0x0000024E
_08007644:
	ldr r0, _08007650 @ =0x00000259
	cmp r2, r0
	beq _08007712
	add r0, #9
	b _08007700
	.align 2, 0
_08007650: .4byte 0x00000259
_08007654:
	mov r0, #0x9D
	lsl r0, r0, #3
	cmp r2, r0
	bgt _080076C0
	sub r0, #2
	cmp r2, r0
	bge _08007712
	ldr r0, _08007680 @ =0x000002FA
	cmp r2, r0
	beq _08007712
	cmp r2, r0
	bgt _08007694
	sub r0, #0x21
	cmp r2, r0
	beq _08007712
	cmp r2, r0
	bgt _08007684
	sub r0, #0x59
	cmp r2, r0
	beq _0800771C
	b _0800772A
	.align 2, 0
_08007680: .4byte 0x000002FA
_08007684:
	ldr r0, _08007690 @ =0x000002DB
	cmp r2, r0
	beq _08007712
	add r0, #4
	b _080076B4
	.align 2, 0
_08007690: .4byte 0x000002DB
_08007694:
	ldr r0, _080076A8 @ =0x0000045C
	cmp r2, r0
	beq _08007712
	cmp r2, r0
	bgt _080076AC
	sub r0, #0x44
	cmp r2, r0
	beq _08007712
	add r0, #0x3A
	b _08007700
_080076A8: .4byte 0x0000045C
_080076AC:
	ldr r0, _080076BC @ =0x0000048B
	cmp r2, r0
	beq _08007712
	add r0, #7
_080076B4:
	cmp r2, r0
	beq _08007726
	b _0800772A
	.align 2, 0
_080076BC: .4byte 0x0000048B
_080076C0:
	ldr r0, _080076DC @ =0x0000059C
	cmp r2, r0
	bgt _080076F4
	sub r0, #1
	cmp r2, r0
	bge _08007712
	sub r0, #0x69
	cmp r2, r0
	beq _08007712
	cmp r2, r0
	bgt _080076E0
	sub r0, #0x17
	b _08007700
	.align 2, 0
_080076DC: .4byte 0x0000059C
_080076E0:
	ldr r0, _080076F0 @ =0x0000053A
_080076E2:
	cmp r2, r0
	bgt _0800772A
	sub r0, #1
	cmp r2, r0
	blt _0800772A
	b _08007712
	.align 2, 0
_080076F0: .4byte 0x0000053A
_080076F4:
	ldr r0, _08007708 @ =0x000005F1
	cmp r2, r0
	beq _08007712
	cmp r2, r0
	bgt _0800770C
	sub r0, #9
_08007700:
	cmp r2, r0
	beq _08007712
	b _0800772A
	.align 2, 0
_08007708: .4byte 0x000005F1
_0800770C:
	ldr r0, _08007718 @ =0x000005F4
	cmp r3, r0
	bne _0800772A
_08007712:
	mov r0, #1
	b _0800772C
	.align 2, 0
_08007718: .4byte 0x000005F4
_0800771C:
	mov r0, #0
	cmp r1, #0
	bne _0800772C
	mov r0, #1
	b _0800772C
_08007726:
	add r0, r1, #0
	b _0800772C
_0800772A:
	mov r0, #0
_0800772C:
	bx lr
	thumb_func_end sub_08007590
	.align 2, 0

