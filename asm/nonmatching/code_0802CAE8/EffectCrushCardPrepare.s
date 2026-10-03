	thumb_func_start EffectCrushCardPrepare
EffectCrushCardPrepare: @ 0x0802D89C
	push {r4, r5, r6, lr}
	sub sp, #0xC
	add r5, r0, #0
	ldr r4, _0802D8C0 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802D936
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0802D8C8
	b _0802D936
	.align 2, 0
_0802D8C0: .4byte 0x0000058A
_0802D8C4:
	mov r0, #1
	b _0802D938
_0802D8C8:
	mov r4, #0
	ldr r6, _0802D940 @ =0x0000076B
_0802D8CC:
	ldrb r0, [r5, #2]
	lsl r2, r0, #0x1F
	lsr r1, r2, #0x1F
	ldr r0, _0802D944 @ =0x00000D64
	mul r1, r0
	ldr r0, _0802D948 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x94
	mul r0, r4
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0802D930
	ldr r3, _0802D94C @ =0x000007FF
	add r0, r3, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _0802D950 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, r6
	bhi _0802D930
	lsl r0, r1, #2
	ldr r1, _0802D954 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802D930
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, sp
	bl GetZoneCardStats
	ldr r1, [sp, #4]
	mov r0, #0xFA
	lsl r0, r0, #2
	cmp r1, r0
	bgt _0802D930
	mov r0, sp
	ldrb r1, [r0, #2]
	mov r0, #0xE0
	and r0, r1
	cmp r0, #0x40
	beq _0802D8C4
_0802D930:
	add r4, #1
	cmp r4, #4
	ble _0802D8CC
_0802D936:
	mov r0, #0
_0802D938:
	add sp, #0xC
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0802D940: .4byte 0x0000076B
_0802D944: .4byte 0x00000D64
_0802D948: .4byte 0x0201930C
_0802D94C: .4byte 0x000007FF
_0802D950: .4byte gCardIdToNumber
_0802D954: .4byte gCardStats
	thumb_func_end EffectCrushCardPrepare

