	thumb_func_start EffectDeclareTypeResolve
EffectDeclareTypeResolve: @ 0x08037688
	push {r4, r5, r6, lr}
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _0803770A
	ldr r0, _080376D4 @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _080376DC
	mov r1, #7
	ldrb r0, [r2, #0xA]
	and r1, r0
	cmp r1, #1
	bne _0803770A
	ldrh r0, [r2, #0xC]
	cmp r0, #0
	beq _0803770A
	ldrb r3, [r2, #2]
	and r1, r3
	mov r0, #0x87
	cmp r1, #0
	beq _080376C0
	ldr r0, _080376D8 @ =0x00008087
_080376C0:
	ldrh r3, [r2, #2]
	lsl r1, r3, #0x16
	lsr r1, r1, #0x1A
	ldrh r2, [r2, #0xC]
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7F
	b _0803770C
	.align 2, 0
_080376D4: .4byte 0x02017A40
_080376D8: .4byte 0x00008087
_080376DC:
	ldr r0, _08037714 @ =0x000007FF
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _08037718 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0803771C @ =0x00000479
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803770A
	mov r5, #0
_080376F2:
	mov r4, #0
	add r6, r5, #1
_080376F6:
	add r0, r5, #0
	add r1, r4, #0
	bl DestroyInvalidEquips
	add r4, #1
	cmp r4, #4
	ble _080376F6
	add r5, r6, #0
	cmp r5, #1
	ble _080376F2
_0803770A:
	mov r0, #0
_0803770C:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08037714: .4byte 0x000007FF
_08037718: .4byte gCardIdToNumber
_0803771C: .4byte 0x00000479
	thumb_func_end EffectDeclareTypeResolve

