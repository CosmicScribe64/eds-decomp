	thumb_func_start CoinToss_Update
CoinToss_Update: @ 0x08024AA0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _08024B10 @ =0x02015DD0
	add r0, r4, #0
	bl CoinToss_UpdateSparkles
	add r0, r4, #0
	bl CoinToss_DrawSparkles
	add r0, r4, #0
	sub r0, #8
	bl FadeTick
	sub r0, r4, #2
	ldrb r0, [r0]
	cmp r0, #2
	bne _08024AC6
	b _08024CAC
_08024AC6:
	cmp r0, #3
	bne _08024AD4
	ldr r1, _08024B14 @ =0x04000050
	mov r2, #0x82
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
_08024AD4:
	add r5, r4, #0
	sub r5, #0x3F
	ldrb r0, [r5]
	cmp r0, #0
	bne _08024AFC
	add r0, r4, #0
	sub r0, #0xA4
	add r1, r4, #0
	sub r1, #0x44
	ldrb r1, [r1]
	bl CoinToss_CountUnfinished
	cmp r0, #0
	bne _08024AFC
	mov r0, #1
	strb r0, [r5]
	ldr r0, _08024B18 @ =0x00000105
	add r1, r4, r0
	mov r0, #0x64
	strb r0, [r1]
_08024AFC:
	ldr r3, _08024B1C @ =0x02015280
	ldr r1, _08024B20 @ =0x00000C54
	add r0, r3, r1
	ldrb r2, [r0]
	cmp r2, #0
	beq _08024B24
	cmp r2, #1
	beq _08024B84
	b _08024C1C
	.align 2, 0
_08024B10: .4byte 0x02015DD0
_08024B14: .4byte 0x04000050
_08024B18: .4byte 0x00000105
_08024B1C: .4byte 0x02015280
_08024B20: .4byte 0x00000C54
_08024B24:
	ldr r2, _08024B34 @ =0x00000B11
	add r0, r3, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08024B38
	cmp r0, #1
	beq _08024B6C
	b _08024C1C
_08024B34: .4byte 0x00000B11
_08024B38:
	ldr r4, _08024B68 @ =0x00000B0D
	add r2, r3, r4
	ldrb r1, [r2]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	sub r4, #0x5F
	add r0, r0, r4
	mov r1, #1
	strb r1, [r0]
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	ldrb r1, [r2]
	add r4, #0x5E
	add r0, r3, r4
	ldrb r3, [r0]
	add r0, r3, #1
	cmp r1, r0
	bne _08024BF2
	strb r3, [r2]
	b _08024C1C
	.align 2, 0
_08024B68: .4byte 0x00000B0D
_08024B6C:
	mov r1, #0xC0
	lsl r1, r1, #1
	ldr r0, _08024B80 @ =0x00000B48
	add r3, r3, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
	b _08024C1C
	.align 2, 0
_08024B80: .4byte 0x00000B48
_08024B84:
	ldr r4, _08024BAC @ =0x00000C55
	add r1, r3, r4
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	bne _08024C1C
	mov r0, #0xF
	strb r0, [r1]
	ldr r1, _08024BB0 @ =0x00000B11
	add r0, r3, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08024BB4
	cmp r0, #1
	beq _08024C0C
	b _08024C1C
	.align 2, 0
_08024BAC: .4byte 0x00000C55
_08024BB0: .4byte 0x00000B11
_08024BB4:
	ldr r0, _08024BFC @ =0x00000B0D
	add r4, r3, r0
	ldrb r1, [r4]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r3
	ldr r1, _08024C00 @ =0x00000AAE
	add r0, r0, r1
	strb r2, [r0]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	ldrb r1, [r4]
	ldr r2, _08024C04 @ =0x00000B0C
	add r5, r3, r2
	ldrb r2, [r5]
	add r0, r2, #1
	cmp r1, r0
	bne _08024BDE
	strb r2, [r4]
_08024BDE:
	ldr r4, _08024C08 @ =0x00000C56
	add r0, r3, r4
	ldrb r1, [r0]
	add r2, r1, #1
	strb r2, [r0]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldrb r5, [r5]
	cmp r1, r5
	bcs _08024C1C
_08024BF2:
	mov r0, #0x1E
	bl PlaySE
	b _08024C1C
	.align 2, 0
_08024BFC: .4byte 0x00000B0D
_08024C00: .4byte 0x00000AAE
_08024C04: .4byte 0x00000B0C
_08024C08: .4byte 0x00000C56
_08024C0C:
	mov r1, #0xC0
	lsl r1, r1, #1
	ldr r0, _08024CA0 @ =0x00000B48
	add r3, r3, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
_08024C1C:
	ldr r4, _08024CA4 @ =0x02015D2C
	ldr r1, _08024CA8 @ =0xFFFFF554
	add r1, r1, r4
	mov r8, r1
	add r5, r4, #0
	add r5, #0x60
	ldrb r1, [r5]
	add r0, r4, #0
	bl CoinToss_AnimateSpin
	ldrb r1, [r5]
	add r6, r4, #0
	add r6, #0x94
	ldrh r2, [r6]
	add r3, r4, #0
	add r3, #0x64
	add r0, r4, #0
	bl CoinToss_UpdateFlight
	ldrb r1, [r5]
	add r0, r4, #0
	bl CoinToss_DrawCoins
	ldrb r1, [r5]
	add r7, r4, #0
	add r7, #0x62
	ldrb r2, [r7]
	add r0, r4, #0
	bl CoinToss_MarkMatchingCoins
	ldrb r1, [r5]
	add r0, r4, #0
	bl CoinToss_AnimateHighlights
	ldrb r1, [r5]
	ldrb r2, [r7]
	add r0, r4, #0
	bl CoinToss_DrawGlints
	mov r0, r8
	bl OamListFlush
	mov r0, r8
	bl OamListClear
	add r5, #8
	add r0, r5, #0
	bl Scroller_Move
	add r0, r5, #0
	bl Scroller_SnapToStop
	add r4, #0x6C
	add r0, r4, #0
	bl Scroller_StopAtEnds
	ldrh r0, [r6]
	add r1, r0, #1
	strh r1, [r6]
	lsl r0, r0, #0x10
	mov r1, #0xA0
	lsl r1, r1, #0x11
	cmp r0, r1
	bhi _08024CAC
	mov r0, #0
	b _08024CAE
_08024CA0: .4byte 0x00000B48
_08024CA4: .4byte 0x02015D2C
_08024CA8: .4byte 0xFFFFF554
_08024CAC:
	mov r0, #1
_08024CAE:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CoinToss_Update

