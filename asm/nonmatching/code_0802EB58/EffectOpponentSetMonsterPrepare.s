	thumb_func_start EffectOpponentSetMonsterPrepare
EffectOpponentSetMonsterPrepare: @ 0x0802F9E4
	add r3, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FA40
	mov r0, #0xFC
	ldrb r1, [r3, #3]
	and r0, r1
	cmp r0, #0x20
	bne _0802FA40
	ldrb r2, [r3, #6]
	ldrh r0, [r3, #6]
	lsr r1, r0, #8
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r2, r0
	beq _0802FA40
	mov r3, #1
	and r2, r3
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0802FA38 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802FA3C @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802FA40
	ldrb r1, [r1, #6]
	add r0, r3, #0
	and r0, r1
	cmp r0, #0
	beq _0802FA40
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _0802FA40
	mov r0, #1
	b _0802FA42
	.align 2, 0
_0802FA38: .4byte 0x00000D64
_0802FA3C: .4byte 0x0201930C
_0802FA40:
	mov r0, #0
_0802FA42:
	bx lr
	thumb_func_end EffectOpponentSetMonsterPrepare

