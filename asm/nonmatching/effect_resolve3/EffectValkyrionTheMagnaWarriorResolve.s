	thumb_func_start EffectValkyrionTheMagnaWarriorResolve
EffectValkyrionTheMagnaWarriorResolve: @ 0x0803327C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #0
	beq _08033296
	b _080333F4
_08033296:
	ldr r0, _08033318 @ =0x02017A40
	mov r8, r0
	mov r6, #0xF8
	lsl r6, r6, #2
	add r6, r8
	ldrb r0, [r6]
	cmp r0, #0x7F
	beq _08033302
	cmp r0, #0x80
	beq _080332AC
	b _080333F4
_080332AC:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #1
	bgt _080332BC
	b _080333F4
_080332BC:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803331C @ =0x000002E1
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	bne _080332CE
	b _080333F4
_080332CE:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xBD
	lsl r1, r1, #2
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	bne _080332E2
	b _080333F4
_080332E2:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xC8
	lsl r1, r1, #2
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	bne _080332F6
	b _080333F4
_080332F6:
	ldr r0, _08033320 @ =0x000003E1
	add r0, r8
	strb r7, [r0]
	ldrb r0, [r6]
	sub r0, #1
	strb r0, [r6]
_08033302:
	ldr r0, _08033318 @ =0x02017A40
	ldr r1, _08033320 @ =0x000003E1
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #1
	beq _08033334
	cmp r0, #1
	bgt _08033324
	cmp r0, #0
	beq _0803332A
	b _0803333E
_08033318: .4byte 0x02017A40
_0803331C: .4byte 0x000002E1
_08033320: .4byte 0x000003E1
_08033324:
	cmp r0, #2
	beq _0803333A
	b _0803333E
_0803332A:
	ldr r5, _08033330 @ =0x000002E1
	b _0803333E
	.align 2, 0
_08033330: .4byte 0x000002E1
_08033334:
	mov r5, #0xBD
	lsl r5, r5, #2
	b _0803333E
_0803333A:
	mov r5, #0xC8
	lsl r5, r5, #2
_0803333E:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r2, r0, #0x1F
	ldr r0, _08033350 @ =0x0000FFFF
	cmp r5, r0
	bne _08033354
	mov r0, #0
	b _08033382
	.align 2, 0
_08033350: .4byte 0x0000FFFF
_08033354:
	ldr r0, _08033368 @ =0x000007CF
	cmp r5, r0
	bhi _08033370
	add r0, #0x30
	and r5, r0
	lsl r0, r5, #1
	ldr r1, _0803336C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08033382
_08033368: .4byte 0x000007CF
_0803336C: .4byte gCardNumberToId
_08033370:
	ldr r1, _080333D8 @ =0xFFFFF830
	add r0, r5, r1
	ldr r1, _080333DC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080333E0 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_08033382:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, sp
	bl GetGraveyardCardById
	cmp r0, #0
	beq _080333C0
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r3, #0xD3
	cmp r0, #0
	beq _080333A0
	ldr r3, _080333E4 @ =0x000080D3
_080333A0:
	ldr r2, [sp, #0]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, sp
	mov r2, #1
	mov r3, #0x20
	bl QueueSpecialSummonChoosePosition
_080333C0:
	ldr r1, _080333E8 @ =0x02017A40
	ldr r0, _080333EC @ =0x000003E1
	add r1, r1, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #2
	bhi _080333F0
	mov r0, #0x7F
	b _080333F6
_080333D8: .4byte 0xFFFFF830
_080333DC: .4byte 0x000007FF
_080333E0: .4byte gCardNumberToId
_080333E4: .4byte 0x000080D3
_080333E8: .4byte 0x02017A40
_080333EC: .4byte 0x000003E1
_080333F0:
	mov r0, #0x7E
	b _080333F6
_080333F4:
	mov r0, #0
_080333F6:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectValkyrionTheMagnaWarriorResolve
	.align 2, 0

