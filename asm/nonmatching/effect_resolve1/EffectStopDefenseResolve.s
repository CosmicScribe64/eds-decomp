	thumb_func_start EffectStopDefenseResolve
EffectStopDefenseResolve: @ 0x08030E4C
	push {r4, r5, r6, r7, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08030EF6
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08030EF6
	ldrb r6, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r5, r1, #8
	and r2, r6
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08030EBC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r2, _08030EC0 @ =0x0201930C
	add r4, r1, r2
	ldr r0, [r4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08030EC4 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08030EC8 @ =0x000004B1
	add r7, r2, #0
	ldrh r0, [r0]
	cmp r0, r1
	bne _08030ED0
	mov r0, #3
	ldrb r2, [r4, #6]
	and r0, r2
	cmp r0, #1
	bne _08030ED0
	mov r0, #0x7F
	cmp r6, #0
	beq _08030EA2
	ldr r0, _08030ECC @ =0x0000807F
_08030EA2:
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r6, #0
	bl ShowActivatedCard
	b _08030EF6
	.align 2, 0
_08030EBC: .4byte 0x00000D64
_08030EC0: .4byte 0x0201930C
_08030EC4: .4byte gCardIdToNumber
_08030EC8: .4byte 0x000004B1
_08030ECC: .4byte 0x0000807F
_08030ED0:
	mov r3, #1
	add r2, r6, #0
	and r2, r3
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08030F00 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	add r0, r0, r7
	ldrb r0, [r0, #6]
	and r3, r0
	cmp r3, #0
	beq _08030EF6
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #1
	mov r3, #1
	bl ChangeBattlePosition
_08030EF6:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08030F00: .4byte 0x00000D64
	thumb_func_end EffectStopDefenseResolve

