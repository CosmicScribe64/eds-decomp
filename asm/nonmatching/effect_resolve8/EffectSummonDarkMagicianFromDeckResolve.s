	thumb_func_start EffectSummonDarkMagicianFromDeckResolve
EffectSummonDarkMagicianFromDeckResolve: @ 0x08038E18
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r5, _08038E4C @ =0x0201D81C
	ldrb r1, [r4, #4]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _08038E54
	mov r0, #8
	and r0, r1
	cmp r0, #0
	bne _08038EE2
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x49
	cmp r0, #0
	beq _08038E3E
	ldr r1, _08038E50 @ =0x00008049
_08038E3E:
	add r0, r1, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	b _08038EE2
_08038E4C: .4byte 0x0201D81C
_08038E50: .4byte 0x00008049
_08038E54:
	ldr r0, _08038EA4 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08038EB4
	cmp r0, #0x80
	bne _08038EC8
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08038EA8 @ =0x000007FF
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	ldr r2, _08038EAC @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	beq _08038EE2
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0x65
	cmp r0, #0
	beq _08038E92
	ldr r3, _08038EB0 @ =0x00008065
_08038E92:
	ldrh r1, [r5]
	ldrh r2, [r5, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7F
	b _08038EE4
	.align 2, 0
_08038EA4: .4byte 0x02017A40
_08038EA8: .4byte 0x000007FF
_08038EAC: .4byte gCardIdToNumber
_08038EB0: .4byte 0x00008065
_08038EB4:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummonChoosePosition
	mov r0, #0x7E
	b _08038EE4
_08038EC8:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x49
	cmp r0, #0
	beq _08038ED6
	ldr r1, _08038EEC @ =0x00008049
_08038ED6:
	add r0, r1, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08038EE2:
	mov r0, #0
_08038EE4:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08038EEC: .4byte 0x00008049
	thumb_func_end EffectSummonDarkMagicianFromDeckResolve

