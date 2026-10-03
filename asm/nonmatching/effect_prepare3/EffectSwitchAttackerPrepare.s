	thumb_func_start EffectSwitchAttackerPrepare
EffectSwitchAttackerPrepare: @ 0x0802FE6C
	push {lr}
	add r2, r0, #0
	mov r0, #0xFC
	ldrb r1, [r2, #3]
	and r0, r1
	cmp r0, #0x40
	bne _0802FE9C
	ldrb r0, [r2, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	ldrb r2, [r2, #6]
	cmp r2, r0
	beq _0802FE9C
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	cmp r0, #1
	ble _0802FE9C
	mov r0, #1
	b _0802FE9E
_0802FE9C:
	mov r0, #0
_0802FE9E:
	pop {r1}
	bx r1
	thumb_func_end EffectSwitchAttackerPrepare
	.align 2, 0

