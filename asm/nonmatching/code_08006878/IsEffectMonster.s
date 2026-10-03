	thumb_func_start IsEffectMonster
IsEffectMonster: @ 0x08007730
	push {r4, lr}
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r4, #0
	ldr r2, _08007754 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _08007758 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0800775C
	mov r0, #0
	b _08007828
_08007754: .4byte 0x000007FF
_08007758: .4byte gCardStats
_0800775C:
	lsl r0, r2, #1
	ldr r1, _08007770 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08007774 @ =0x00000776
	cmp r1, r0
	bne _08007778
	mov r0, #3
	b _080077DA
	.align 2, 0
_08007770: .4byte gCardIdToNumber
_08007774: .4byte 0x00000776
_08007778:
	cmp r1, r0
	blt _08007788
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08007788
	mov r0, #1
	b _080077DA
_08007788:
	ldr r0, _080077AC @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080077B0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080077BA
	cmp r0, #0x16
	bgt _080077B4
	cmp r0, #0x15
	beq _080077BE
	b _080077C6
	.align 2, 0
_080077AC: .4byte 0x000007FF
_080077B0: .4byte gCardStats
_080077B4:
	cmp r0, #0x17
	beq _080077C2
	b _080077C6
_080077BA:
	mov r0, #7
	b _080077DA
_080077BE:
	mov r0, #8
	b _080077DA
_080077C2:
	mov r0, #9
	b _080077DA
_080077C6:
	ldr r0, _08007804 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08007808 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080077DA:
	cmp r0, #1
	bne _080077E0
	mov r4, #1
_080077E0:
	ldr r0, _08007804 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0800780C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08007810 @ =0x000004D9
	cmp r1, r0
	beq _08007824
	cmp r1, r0
	bgt _08007818
	ldr r0, _08007814 @ =0x000002DA
	cmp r1, r0
	beq _08007824
	add r0, #0x52
	cmp r1, r0
	beq _08007824
	b _08007826
_08007804: .4byte 0x000007FF
_08007808: .4byte gCardStats
_0800780C: .4byte gCardIdToNumber
_08007810: .4byte 0x000004D9
_08007814: .4byte 0x000002DA
_08007818:
	ldr r0, _08007830 @ =0x00000536
	cmp r1, r0
	beq _08007824
	add r0, #0xC0
	cmp r1, r0
	bne _08007826
_08007824:
	mov r4, #1
_08007826:
	add r0, r4, #0
_08007828:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08007830: .4byte 0x00000536
	thumb_func_end IsEffectMonster

