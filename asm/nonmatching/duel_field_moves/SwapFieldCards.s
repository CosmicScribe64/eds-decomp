	thumb_func_start SwapFieldCards
SwapFieldCards: @ 0x0801919C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	lsr r0, r1, #0x10
	str r0, [sp, #4]
	lsl r2, r2, #0x10
	lsr r0, r2, #0x10
	mov sl, r0
	ldr r3, [sp, #4]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	lsr r1, r1, #0x18
	str r1, [sp, #8]
	mov r1, sl
	lsl r0, r1, #0x18
	lsr r7, r0, #0x18
	lsr r2, r2, #0x18
	mov r9, r2
	mov r2, #1
	mov ip, r2
	mov r0, r8
	and r0, r2
	mov r4, #0x94
	ldr r3, [sp, #8]
	add r1, r3, #0
	mul r1, r4
	ldr r3, _08019240 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r2, _08019244 @ =0x0201930C
	add r6, r1, r2
	ldr r0, [r6]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r5, r0, #0
	add r1, r7, #0
	mov r0, ip
	and r1, r0
	mov r0, r9
	mul r0, r4
	mul r1, r3
	add r0, r0, r1
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r5, #0
	bne _0801920A
	b _0801938E
_0801920A:
	cmp r4, #0
	bne _08019210
	b _0801938E
_08019210:
	mov r0, #0x84
	ldr r1, [sp, #0]
	cmp r1, #0
	beq _0801921A
	ldr r0, _08019248 @ =0x00008084
_0801921A:
	ldr r1, [sp, #4]
	mov r2, sl
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _0801924C @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r2, _08019250 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _08019254 @ =0x000001E3
	cmp r1, r0
	beq _08019258
	add r0, #0x3F
	cmp r1, r0
	beq _08019290
	b _080192C0
	.align 2, 0
_08019240: .4byte 0x00000D64
_08019244: .4byte 0x0201930C
_08019248: .4byte 0x00008084
_0801924C: .4byte 0x000007FF
_08019250: .4byte gCardIdToNumber
_08019254: .4byte 0x000001E3
_08019258:
	mov r0, #0x20
	ldrb r3, [r6, #7]
	and r0, r3
	cmp r0, #0
	beq _080192C0
	ldr r1, [r6]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r0, r8
	bl sub_080197C0
	mov r1, #0xFA
	lsl r1, r1, #3
	add r0, r7, #0
	bl LoseLifePoints
	mov r0, #0x92
	cmp r7, #0
	beq _08019280
	ldr r0, _0801928C @ =0x00008092
_08019280:
	mov r1, r9
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _080192C0
_0801928C: .4byte 0x00008092
_08019290:
	mov r0, #0x20
	ldrb r1, [r6, #7]
	and r0, r1
	cmp r0, #0
	beq _080192C0
	ldr r1, [r6]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r0, r8
	bl sub_080197C0
	ldr r1, _080192DC @ =0x00000BB8
	mov r0, r8
	bl GainLifePoints
	mov r0, #0x92
	cmp r7, #0
	beq _080192B6
	ldr r0, _080192E0 @ =0x00008092
_080192B6:
	mov r1, r9
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080192C0:
	ldr r0, _080192E4 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #1
	ldr r2, _080192E8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _080192EC @ =0x000001E3
	cmp r1, r0
	beq _080192F0
	add r0, #0x3F
	cmp r1, r0
	beq _08019348
	b _0801938E
	.align 2, 0
_080192DC: .4byte 0x00000BB8
_080192E0: .4byte 0x00008092
_080192E4: .4byte 0x000007FF
_080192E8: .4byte gCardIdToNumber
_080192EC: .4byte 0x000001E3
_080192F0:
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	mov r2, r9
	mul r2, r0
	ldr r0, _0801933C @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08019340 @ =0x0201930C
	add r1, r2, r0
	mov r0, #0x20
	ldrb r3, [r1, #7]
	and r0, r3
	cmp r0, #0
	beq _0801938E
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r7, #0
	bl sub_080197C0
	mov r1, #0xFA
	lsl r1, r1, #3
	mov r0, r8
	bl LoseLifePoints
	mov r0, #0x92
	mov r1, r8
	cmp r1, #0
	beq _0801932E
	ldr r0, _08019344 @ =0x00008092
_0801932E:
	ldr r1, [sp, #8]
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _0801938E
	.align 2, 0
_0801933C: .4byte 0x00000D64
_08019340: .4byte 0x0201930C
_08019344: .4byte 0x00008092
_08019348:
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	mov r2, r9
	mul r2, r0
	ldr r0, _080193A0 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080193A4 @ =0x0201930C
	add r1, r2, r0
	mov r0, #0x20
	ldrb r2, [r1, #7]
	and r0, r2
	cmp r0, #0
	beq _0801938E
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r7, #0
	bl sub_080197C0
	ldr r1, _080193A8 @ =0x00000BB8
	add r0, r7, #0
	bl GainLifePoints
	mov r0, #0x92
	mov r3, r8
	cmp r3, #0
	beq _08019384
	ldr r0, _080193AC @ =0x00008092
_08019384:
	ldr r1, [sp, #8]
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0801938E:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080193A0: .4byte 0x00000D64
_080193A4: .4byte 0x0201930C
_080193A8: .4byte 0x00000BB8
_080193AC: .4byte 0x00008092
	thumb_func_end SwapFieldCards

