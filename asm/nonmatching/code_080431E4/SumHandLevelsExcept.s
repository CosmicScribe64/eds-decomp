	thumb_func_start SumHandLevelsExcept
SumHandLevelsExcept: @ 0x08043600
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r6, #0
	mov ip, r6
	ldr r2, _08043664 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08043668 @ =0x00000D64
	mul r1, r0
	add r0, r1, r2
	ldrb r0, [r0, #2]
	cmp r6, r0
	bge _080436A6
	ldr r7, _0804366C @ =0x000007FF
	ldr r3, _08043670 @ =0x00000684
	add r3, r3, r2
	mov r8, r3
	add r4, r1, #0
	add r3, r0, #0
_0804362C:
	mov r1, r8
	add r0, r4, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov r0, ip
	cmp r0, #0
	bne _08043640
	cmp r2, r5
	beq _08043696
_08043640:
	add r0, r2, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08043674 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08043680
	cmp r0, #0x17
	ble _08043678
	cmp r0, #0x18
	beq _0804367C
	b _08043680
	.align 2, 0
_08043664: .4byte 0x020192E4
_08043668: .4byte 0x00000D64
_0804366C: .4byte 0x000007FF
_08043670: .4byte 0x00000684
_08043674: .4byte gCardStats
_08043678:
	mov r0, #0
	b _08043694
_0804367C:
	mov r0, #0xA
	b _08043694
_08043680:
	add r0, r2, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _080436B4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043694:
	add r6, r0, r6
_08043696:
	cmp r5, r2
	bne _0804369E
	mov r0, #1
	mov ip, r0
_0804369E:
	add r4, #4
	sub r3, #1
	cmp r3, #0
	bne _0804362C
_080436A6:
	add r0, r6, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080436B4: .4byte gCardStats
	thumb_func_end SumHandLevelsExcept

