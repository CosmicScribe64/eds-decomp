	thumb_func_start EffectSummonFourTokensResolve
EffectSummonFourTokensResolve: @ 0x08039B04
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08039B80
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #3
	ble _08039B80
	mov r5, #0
	mov r6, #0
_08039B24:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl IsMonsterZoneFree
	cmp r0, #0
	beq _08039B5C
	mov r0, #1
	ldrb r3, [r4, #2]
	and r0, r3
	mov r2, #0xA3
	cmp r0, #0
	beq _08039B42
	ldr r2, _08039B88 @ =0x000080A3
_08039B42:
	lsl r1, r5, #0x18
	lsr r1, r1, #0x18
	ldrh r3, [r4, #2]
	lsl r0, r3, #0x16
	lsr r0, r0, #0x1A
	lsl r0, r0, #8
	orr r1, r0
	add r0, r2, #0
	mov r2, #2
	mov r3, #0
	bl DuelCmd_Push
	add r6, #1
_08039B5C:
	add r5, #1
	cmp r5, #4
	bgt _08039B66
	cmp r6, #3
	ble _08039B24
_08039B66:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x49
	cmp r0, #0
	beq _08039B74
	ldr r1, _08039B8C @ =0x00008049
_08039B74:
	add r0, r1, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08039B80:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08039B88: .4byte 0x000080A3
_08039B8C: .4byte 0x00008049
	thumb_func_end EffectSummonFourTokensResolve

