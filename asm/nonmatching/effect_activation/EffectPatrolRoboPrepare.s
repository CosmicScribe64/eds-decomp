	thumb_func_start EffectPatrolRoboPrepare
EffectPatrolRoboPrepare: @ 0x0802D7CC
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802D7FC
	ldrb r1, [r0, #2]
	lsl r2, r1, #0x1F
	lsr r2, r2, #0x1F
	ldrh r0, [r0, #2]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0802D7F4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802D7F8 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #7]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1F
	b _0802D7FE
_0802D7F4: .4byte 0x00000D64
_0802D7F8: .4byte 0x0201930C
_0802D7FC:
	mov r0, #0
_0802D7FE:
	bx lr
	thumb_func_end EffectPatrolRoboPrepare

