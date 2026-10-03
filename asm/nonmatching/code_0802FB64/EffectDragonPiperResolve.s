	thumb_func_start EffectDragonPiperResolve
EffectDragonPiperResolve: @ 0x080300D8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r1, #4
	ldrb r0, [r0, #4]
	and r1, r0
	cmp r1, #0
	bne _080301A0
	mov r5, #0
	mov r0, #1
	mov r9, r0
	mov r7, #0xA4
	lsl r7, r7, #1
	ldr r1, _080301B0 @ =0x00000D64
	mov r8, r1
_080300F8:
	mov r4, #5
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r6, r8
	mul r6, r0
_08030104:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _080301B4 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	ble _0803013C
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803013C
	ldr r0, _080301B8 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _080301BC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _0803013C
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_0803013C:
	add r4, #1
	cmp r4, #9
	ble _08030104
	add r5, #1
	cmp r5, #1
	ble _080300F8
	mov r5, #0
	mov r0, #1
	mov r9, r0
	ldr r1, _080301B0 @ =0x00000D64
	mov r8, r1
_08030152:
	mov r4, #0
	add r7, r5, #1
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r6, r8
	mul r6, r0
_08030160:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _080301B4 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08030194
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #3
	bne _08030194
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardType
	cmp r0, #1
	bne _08030194
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl ChangeBattlePosition
_08030194:
	add r4, #1
	cmp r4, #4
	ble _08030160
	add r5, r7, #0
	cmp r5, #1
	ble _08030152
_080301A0:
	mov r0, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080301B0: .4byte 0x00000D64
_080301B4: .4byte 0x0201930C
_080301B8: .4byte 0x000007FF
_080301BC: .4byte gCardIdToNumber
	thumb_func_end EffectDragonPiperResolve

