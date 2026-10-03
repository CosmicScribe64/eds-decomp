	thumb_func_start DuelCmd_NegateAttack
DuelCmd_NegateAttack: @ 0x0800D6FC
	push {r4, r5, lr}
	ldr r0, _0800D760 @ =0x020185C0
	ldrh r1, [r0]
	lsr r4, r1, #0xF
	ldrh r5, [r0, #2]
	ldr r1, _0800D764 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	ldr r3, _0800D768 @ =0x020192E0
	cmp r0, #0
	beq _0800D722
	ldr r2, _0800D76C @ =0x00001B12
	add r1, r3, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800D740
_0800D722:
	ldr r0, _0800D770 @ =0x00001B14
	add r2, r3, r0
	ldr r0, [r2]
	ldr r1, _0800D774 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xB0
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r2]
	ldr r2, _0800D778 @ =0x00001B16
	add r1, r3, r2
	ldr r0, _0800D77C @ =0xFFFFFE01
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
_0800D740:
	add r0, r4, #0
	add r1, r5, #0
	bl MarkMonsterAttacked
	ldr r1, _0800D760 @ =0x020185C0
	ldr r0, _0800D780 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800D760: .4byte 0x020185C0
_0800D764: .4byte 0x02015EE8
_0800D768: .4byte 0x020192E0
_0800D76C: .4byte 0x00001B12
_0800D770: .4byte 0x00001B14
_0800D774: .4byte 0xFFFE01FF
_0800D778: .4byte 0x00001B16
_0800D77C: .4byte 0xFFFFFE01
_0800D780: .4byte 0x0000080D
	thumb_func_end DuelCmd_NegateAttack

