	thumb_func_start EffectBlockMonsterZonesChainB
EffectBlockMonsterZonesChainB: @ 0x0804112C
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	ldr r0, _08041148 @ =0x02017A40
	ldr r1, _0804114C @ =0x000003E5
	add r6, r0, r1
	ldrb r0, [r6]
	cmp r0, #1
	beq _08041178
	cmp r0, #1
	bgt _08041150
	cmp r0, #0
	beq _0804115A
	b _08041284
	.align 2, 0
_08041148: .4byte 0x02017A40
_0804114C: .4byte 0x000003E5
_08041150:
	cmp r0, #2
	beq _080411D4
	cmp r0, #3
	beq _080411F8
	b _08041284
_0804115A:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r7, #0xA]
	and r0, r2
	strb r0, [r7, #0xA]
	ldr r0, _0804116C @ =0x00000206
	ldr r1, _08041170 @ =0x00000712
	ldr r3, _08041174 @ =0x08084A30
	b _080411DA
_0804116C: .4byte 0x00000206
_08041170: .4byte 0x00000712
_08041174: .4byte gStrSelectZoneToBlock
_08041178:
	ldr r1, _080411BC @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08041204
	bl DuelCursor_PickAny
	cmp r0, #0
	beq _080411E6
	ldr r0, _080411C0 @ =0x0201CFB0
	ldr r2, _080411C4 @ =0x00000824
	add r1, r0, r2
	ldr r5, [r1]
	add r2, #8
	add r1, r0, r2
	ldr r4, [r1]
	ldr r1, _080411C8 @ =0x00000828
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _080411CC
	add r0, r5, #0
	add r1, r4, #0
	bl IsMonsterZoneFree
	cmp r0, #0
	beq _080411CC
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl AddEffectTargetUnchecked
	b _080411E0
_080411BC: .4byte 0x03000040
_080411C0: .4byte 0x0201CFB0
_080411C4: .4byte 0x00000824
_080411C8: .4byte 0x00000828
_080411CC:
	mov r0, #3
	bl PlaySE
	b _080411E6
_080411D4:
	ldr r0, _080411EC @ =0x00000206
	ldr r1, _080411F0 @ =0x00000712
	ldr r3, _080411F4 @ =0x08084A68
_080411DA:
	mov r2, #0xB
	bl TextBoxOpen
_080411E0:
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
_080411E6:
	mov r0, #0
	b _08041286
	.align 2, 0
_080411EC: .4byte 0x00000206
_080411F0: .4byte 0x00000712
_080411F4: .4byte gStrSelectAnotherZoneToBlock
_080411F8:
	ldr r1, _0804120C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041210
_08041204:
	mov r0, #0
	strb r0, [r6]
	b _08041286
	.align 2, 0
_0804120C: .4byte 0x03000040
_08041210:
	bl DuelCursor_PickAny
	cmp r0, #0
	beq _080411E6
	ldr r0, _08041268 @ =0x0201CFB0
	ldr r2, _0804126C @ =0x00000824
	add r1, r0, r2
	ldr r4, [r1]
	add r2, #8
	add r1, r0, r2
	ldr r3, [r1]
	ldr r1, _08041270 @ =0x00000828
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _0804127C
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08041274 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08041278 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0804127C
	lsl r0, r4, #0x18
	lsl r1, r3, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	ldrh r2, [r7, #0xC]
	cmp r0, r2
	beq _0804127C
	add r0, r7, #0
	add r1, r4, #0
	add r2, r3, #0
	bl AddEffectTargetUnchecked
	b _080411E0
	.align 2, 0
_08041268: .4byte 0x0201CFB0
_0804126C: .4byte 0x00000824
_08041270: .4byte 0x00000828
_08041274: .4byte 0x00000D64
_08041278: .4byte 0x0201930C
_0804127C:
	mov r0, #3
	bl PlaySE
	b _080411E6
_08041284:
	mov r0, #1
_08041286:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectBlockMonsterZonesChainB

