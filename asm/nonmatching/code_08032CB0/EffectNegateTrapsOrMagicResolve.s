	thumb_func_start EffectNegateTrapsOrMagicResolve
EffectNegateTrapsOrMagicResolve: @ 0x08032F20
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08032F34
	b _08033206
_08032F34:
	ldr r1, _08032F54 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x78
	add r3, r1, #0
	cmp r0, #8
	bls _08032F48
	b _08033206
_08032F48:
	lsl r0, r0, #2
	ldr r1, _08032F58 @ =0x08032F5C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08032F54: .4byte 0x02017A40
_08032F58: .4byte 0x08032F5C
_08032F5C:
	.4byte _08033170
	.4byte _08033206
	.4byte _08033206
	.4byte _08033206
	.4byte _08033206
	.4byte _08033138
	.4byte _08032FA0
	.4byte _08032F8E
	.4byte _08032F80
_08032F80:
	mov r0, #0xA2
	lsl r0, r0, #3
	add r1, r3, r0
	mov r0, #0
	strb r0, [r1]
	mov r0, #0x7F
	b _08033208
_08032F8E:
	ldr r2, _08032F9C @ =0x00000511
	add r1, r3, r2
	mov r0, #5
	strb r0, [r1]
	mov r0, #0x7E
	b _08033208
	.align 2, 0
_08032F9C: .4byte 0x00000511
_08032FA0:
	mov r1, #0xA2
	lsl r1, r1, #3
	add r0, r3, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08032FB6
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r6, r0, #0x1F
	mov ip, r1
	b _08032FC2
_08032FB6:
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r2, #1
	sub r6, r2, r1
	mov ip, r0
_08032FC2:
	ldr r2, _08033030 @ =0x00000511
	add r0, r3, r2
	ldrb r0, [r0]
	mov r8, r0
	mov r1, #1
	and r1, r6
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	ldr r0, _08033034 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08033038 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	bne _08032FEA
	b _08033122
_08032FEA:
	mov r1, ip
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r6, r0
	bne _08033000
	ldrh r1, [r4, #2]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1A
	cmp r8, r0
	bne _08033000
	b _08033122
_08033000:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0803300C
	b _08033122
_0803300C:
	mov r3, #0
	ldr r2, _0803303C @ =0x000007FF
	add r0, r2, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08033040 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08033044 @ =0x00000409
	cmp r1, r0
	beq _08033064
	cmp r1, r0
	bgt _0803304C
	ldr r0, _08033048 @ =0x000002EF
	cmp r1, r0
	beq _08033064
	b _080330D2
_08033030: .4byte 0x00000511
_08033034: .4byte 0x00000D64
_08033038: .4byte 0x0201930C
_0803303C: .4byte 0x000007FF
_08033040: .4byte gCardIdToNumber
_08033044: .4byte 0x00000409
_08033048: .4byte 0x000002EF
_0803304C:
	ldr r0, _0803305C @ =0x00000482
	cmp r1, r0
	beq _08033088
	ldr r0, _08033060 @ =0x00000601
	cmp r1, r0
	beq _080330AC
	b _080330D2
	.align 2, 0
_0803305C: .4byte 0x00000482
_08033060: .4byte 0x00000601
_08033064:
	ldr r0, _08033080 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _08033084 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	mov r1, #0
	cmp r0, #0x15
	bne _080330A4
	b _080330A2
_08033080: .4byte 0x000007FF
_08033084: .4byte gCardStats
_08033088:
	add r0, r5, #0
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _080330A8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	mov r1, #0
	cmp r0, #0x16
	bne _080330A4
_080330A2:
	mov r1, #1
_080330A4:
	add r3, r1, #0
	b _080330D2
_080330A8: .4byte gCardStats
_080330AC:
	add r0, r5, #0
	and r0, r2
	lsl r0, r0, #2
	ldr r2, _08033128 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _080330D2
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #3
	bne _080330D2
	mov r3, #1
_080330D2:
	cmp r3, #0
	beq _08033122
	mov r7, #1
	add r0, r7, #0
	mov r1, ip
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _080330E6
	ldr r3, _0803312C @ =0x00008008
_080330E6:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	mov r0, r8
	lsl r2, r0, #8
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r7, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x74
	cmp r0, #0
	beq _08033104
	ldr r1, _08033130 @ =0x00008074
_08033104:
	add r0, r1, #0
	add r1, r5, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0xB1
	cmp r6, #0
	beq _08033118
	ldr r0, _08033134 @ =0x000080B1
_08033118:
	mov r1, r8
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08033122:
	mov r0, #0x7D
	b _08033208
	.align 2, 0
_08033128: .4byte gCardStats
_0803312C: .4byte 0x00008008
_08033130: .4byte 0x00008074
_08033134: .4byte 0x000080B1
_08033138:
	ldr r2, _08033150 @ =0x00000511
	add r1, r3, r2
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bhi _08033154
	mov r0, #0x7E
	b _08033208
	.align 2, 0
_08033150: .4byte 0x00000511
_08033154:
	mov r0, #0xA2
	lsl r0, r0, #3
	add r1, r3, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	bhi _0803316C
	mov r0, #0x7F
	b _08033208
_0803316C:
	mov r0, #0x78
	b _08033208
_08033170:
	ldr r0, _08033190 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08033194 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _08033198 @ =0x00000409
	cmp r1, r0
	beq _080331B8
	cmp r1, r0
	bgt _080331A0
	ldr r0, _0803319C @ =0x000002EF
	cmp r1, r0
	beq _080331B8
	b _08033206
_08033190: .4byte 0x000007FF
_08033194: .4byte gCardIdToNumber
_08033198: .4byte 0x00000409
_0803319C: .4byte 0x000002EF
_080331A0:
	ldr r0, _080331B0 @ =0x00000482
	cmp r1, r0
	beq _080331CC
	ldr r0, _080331B4 @ =0x00000601
	cmp r1, r0
	beq _080331EC
	b _08033206
	.align 2, 0
_080331B0: .4byte 0x00000482
_080331B4: .4byte 0x00000601
_080331B8:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x19
	cmp r0, #0
	beq _080331DA
	ldr r1, _080331C8 @ =0x00008019
	b _080331DA
_080331C8: .4byte 0x00008019
_080331CC:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x1A
	cmp r0, #0
	beq _080331DA
	ldr r1, _080331E8 @ =0x0000801A
_080331DA:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08033206
_080331E8: .4byte 0x0000801A
_080331EC:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x1B
	cmp r0, #0
	beq _080331FA
	ldr r1, _08033214 @ =0x0000801B
_080331FA:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08033206:
	mov r0, #0
_08033208:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08033214: .4byte 0x0000801B
	thumb_func_end EffectNegateTrapsOrMagicResolve

