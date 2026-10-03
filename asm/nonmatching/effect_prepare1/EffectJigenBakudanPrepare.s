	thumb_func_start EffectJigenBakudanPrepare
EffectJigenBakudanPrepare: @ 0x0802DB30
	push {r4, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802DBA0
	mov r0, #0xFC
	ldrb r1, [r4, #3]
	and r0, r1
	cmp r0, #8
	bne _0802DBA0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountMonsters
	cmp r0, #1
	ble _0802DBA0
	ldrb r0, [r4, #2]
	lsl r2, r0, #0x1F
	lsr r2, r2, #0x1F
	ldrh r4, [r4, #2]
	lsl r0, r4, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r1, r0
	ldr r0, _0802DB94 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802DB98 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _0802DBA0
	ldr r4, _0802DB9C @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802DBA0
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802DBA0
	mov r0, #1
	b _0802DBA2
_0802DB94: .4byte 0x00000D64
_0802DB98: .4byte 0x0201930C
_0802DB9C: .4byte 0x0000058A
_0802DBA0:
	mov r0, #0
_0802DBA2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectJigenBakudanPrepare

