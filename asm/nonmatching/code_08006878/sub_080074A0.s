	thumb_func_start sub_080074A0
sub_080074A0: @ 0x080074A0
	push {r4, lr}
	ldr r3, _080074E8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _080074EC @ =0x08622AB4
	add r0, r0, r2
	ldrh r4, [r0]
	and r1, r3
	lsl r1, r1, #1
	add r1, r1, r2
	ldrh r1, [r1]
	ldr r2, _080074F0 @ =0x000007CF
	cmp r4, r2
	bls _080074C4
	ldr r3, _080074F4 @ =0xFFFFF830
	add r0, r4, r3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_080074C4:
	cmp r1, r2
	bls _080074D0
	ldr r2, _080074F4 @ =0xFFFFF830
	add r0, r1, r2
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_080074D0:
	cmp r4, r1
	beq _08007546
	ldr r0, _080074F8 @ =0x000003EB
	cmp r4, r0
	beq _08007520
	cmp r4, r0
	bgt _080074FC
	cmp r4, #0x22
	beq _08007530
	cmp r4, #0x3D
	beq _0800753C
	b _08007550
_080074E8: .4byte 0x000007FF
_080074EC: .4byte gUnk_08622AB4
_080074F0: .4byte 0x000007CF
_080074F4: .4byte 0xFFFFF830
_080074F8: .4byte 0x000003EB
_080074FC:
	ldr r0, _08007510 @ =0x000004BA
	cmp r4, r0
	beq _08007530
	cmp r4, r0
	bgt _08007514
	sub r0, #0xB0
	cmp r4, r0
	beq _08007520
	b _08007550
	.align 2, 0
_08007510: .4byte 0x000004BA
_08007514:
	ldr r0, _0800751C @ =0x000004E1
	cmp r4, r0
	beq _0800753C
	b _08007550
_0800751C: .4byte 0x000004E1
_08007520:
	ldr r0, _0800752C @ =0x000003EB
	cmp r1, r0
	beq _08007546
	add r0, #0x1F
	b _08007542
	.align 2, 0
_0800752C: .4byte 0x000003EB
_08007530:
	cmp r1, #0x22
	beq _08007546
	ldr r0, _08007538 @ =0x000004BA
	b _08007542
_08007538: .4byte 0x000004BA
_0800753C:
	cmp r1, #0x3D
	beq _08007546
	ldr r0, _0800754C @ =0x000004E1
_08007542:
	cmp r1, r0
	bne _08007550
_08007546:
	mov r0, #1
	b _08007552
	.align 2, 0
_0800754C: .4byte 0x000004E1
_08007550:
	mov r0, #0
_08007552:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080074A0

