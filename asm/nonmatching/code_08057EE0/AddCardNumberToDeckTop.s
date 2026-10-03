	thumb_func_start AddCardNumberToDeckTop
AddCardNumberToDeckTop: @ 0x08058EDC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r2, r1, #0x10
	ldr r0, _08058EF4 @ =0x0000FFFF
	cmp r2, r0
	bne _08058EF8
	mov r0, #0
	b _08058F26
_08058EF4: .4byte 0x0000FFFF
_08058EF8:
	ldr r0, _08058F0C @ =0x000007CF
	cmp r2, r0
	bhi _08058F14
	add r0, #0x30
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _08058F10 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08058F26
_08058F0C: .4byte 0x000007CF
_08058F10: .4byte gCardNumberToId
_08058F14:
	ldr r5, _08058F64 @ =0xFFFFF830
	add r0, r2, r5
	ldr r1, _08058F68 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08058F6C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_08058F26:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	ldr r0, _08058F70 @ =0x00000FFF
	cmp r2, r0
	bls _08058F34
	b _080590C2
_08058F34:
	ldr r2, _08058F68 @ =0x000007FF
	mov r5, r9
	and r2, r5
	lsl r0, r2, #2
	ldr r1, _08058F74 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08058F50
	b _0805904C
_08058F50:
	lsl r0, r2, #1
	ldr r2, _08058F78 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _08058F7C @ =0x00000776
	cmp r1, r0
	bne _08058F80
	mov r0, #3
	b _08058FE4
	.align 2, 0
_08058F64: .4byte 0xFFFFF830
_08058F68: .4byte 0x000007FF
_08058F6C: .4byte gCardNumberToId
_08058F70: .4byte 0x00000FFF
_08058F74: .4byte gCardStats
_08058F78: .4byte gCardIdToNumber
_08058F7C: .4byte 0x00000776
_08058F80:
	cmp r1, r0
	blt _08058F90
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08058F90
	mov r0, #1
	b _08058FE4
_08058F90:
	ldr r0, _08058FB4 @ =0x000007FF
	mov r5, r9
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _08058FB8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08058FC2
	cmp r0, #0x16
	bgt _08058FBC
	cmp r0, #0x15
	beq _08058FC6
	b _08058FCE
_08058FB4: .4byte 0x000007FF
_08058FB8: .4byte gCardStats
_08058FBC:
	cmp r0, #0x17
	beq _08058FCA
	b _08058FCE
_08058FC2:
	mov r0, #7
	b _08058FE4
_08058FC6:
	mov r0, #8
	b _08058FE4
_08058FCA:
	mov r0, #9
	b _08058FE4
_08058FCE:
	ldr r0, _08059038 @ =0x000007FF
	mov r2, r9
	and r0, r2
	lsl r0, r0, #2
	ldr r5, _0805903C @ =0x08621DE0
	add r0, r0, r5
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08058FE4:
	cmp r0, #2
	bne _0805904C
	ldr r3, _08059040 @ =0x020192E4
	mov r0, #1
	mov r1, r8
	and r0, r1
	ldr r1, _08059044 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r0, r2, r3
	ldrb r4, [r0, #5]
	cmp r4, #0
	ble _0805901C
	lsl r1, r4, #2
	sub r6, r1, #4
	ldr r5, _08059048 @ =0x00000A44
	add r0, r3, r5
	add r7, r2, r0
	add r5, r1, r7
_0805900A:
	add r1, r7, r6
	add r0, r5, #0
	bl CopyDuelCard
	sub r6, #4
	sub r5, #4
	sub r4, #1
	cmp r4, #0
	bgt _0805900A
_0805901C:
	ldr r2, _08059040 @ =0x020192E4
	mov r0, #1
	mov r1, r8
	and r0, r1
	ldr r1, _08059044 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	add r1, r3, r2
	ldrb r0, [r1, #5]
	add r0, #1
	strb r0, [r1, #5]
	ldr r5, _08059048 @ =0x00000A44
	b _08059098
	.align 2, 0
_08059038: .4byte 0x000007FF
_0805903C: .4byte gCardStats
_08059040: .4byte 0x020192E4
_08059044: .4byte 0x00000D64
_08059048: .4byte 0x00000A44
_0805904C:
	ldr r3, _080590D0 @ =0x020192E4
	mov r0, #1
	mov r5, r8
	and r0, r5
	ldr r1, _080590D4 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r0, r2, r3
	ldrb r4, [r0, #3]
	cmp r4, #0
	ble _08059080
	lsl r1, r4, #2
	sub r6, r1, #4
	ldr r5, _080590D8 @ =0x000007C4
	add r0, r3, r5
	add r7, r2, r0
	add r5, r1, r7
_0805906E:
	add r1, r7, r6
	add r0, r5, #0
	bl CopyDuelCard
	sub r6, #4
	sub r5, #4
	sub r4, #1
	cmp r4, #0
	bgt _0805906E
_08059080:
	ldr r2, _080590D0 @ =0x020192E4
	mov r0, #1
	mov r1, r8
	and r0, r1
	ldr r1, _080590D4 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	add r1, r3, r2
	ldrb r0, [r1, #3]
	add r0, #1
	strb r0, [r1, #3]
	ldr r5, _080590D8 @ =0x000007C4
_08059098:
	add r2, r2, r5
	add r3, r3, r2
	ldr r1, _080590DC @ =0x00000FFF
	add r0, r1, #0
	mov r1, r9
	and r1, r0
	ldr r0, _080590E0 @ =0xFFFFF000
	ldrh r2, [r3]
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	mov r0, #1
	mov r5, r8
	and r5, r0
	lsl r1, r5, #4
	mov r0, #0x11
	neg r0, r0
	ldrb r2, [r3, #1]
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
_080590C2:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080590D0: .4byte 0x020192E4
_080590D4: .4byte 0x00000D64
_080590D8: .4byte 0x000007C4
_080590DC: .4byte 0x00000FFF
_080590E0: .4byte 0xFFFFF000
	thumb_func_end AddCardNumberToDeckTop

